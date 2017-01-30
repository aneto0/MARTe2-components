/**
 * @file SDNSubscriber.cpp
 * @brief Source file for class SDNSubscriber
 * @date 20/12/2016
 * @author Bertrand Bauvir
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class LinuxTimer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "BrokerI.h"
#include "SDNSubscriber.h"

#include "sdn-api.h" /* SDN core library - API definition (sdn::core) */

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

SDNSubscriber::SDNSubscriber() :
        DataSourceI(),
        EmbeddedServiceMethodBinderI(),
        executor(*this) {

    log_trace("SDNSubscriber::SDNSubscriber - Entering method");

    mode = Default;
    nOfSignals = 0u;
    TTTimeout = TTInfiniteWait;

    topic = NULL_PTR(sdn::Topic *);
    subscriber = NULL_PTR(sdn::Subscriber *);

    if (!synchronisingSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create EventSem");
    }

    log_trace("SDNSubscriber::SDNSubscriber - Leaving method");

}

SDNSubscriber::~SDNSubscriber() {

    log_trace("SDNSubscriber::~SDNSubscriber - Entering method");

    if (!synchronisingSem.Post()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not post EventSem");
    }

    if (!executor.Stop()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService");
    }

    if (subscriber != NULL_PTR(sdn::Subscriber *)) {
        delete subscriber; subscriber = NULL_PTR(sdn::Subscriber *);
    }

    if (topic != NULL_PTR(sdn::Topic *)) {
        delete topic; topic = NULL_PTR(sdn::Topic *);
    }

    log_trace("SDNSubscriber::~SDNSubscriber - Leaving method");

}

bool SDNSubscriber::Initialise(StructuredDataI &data) {

    log_trace("SDNSubscriber::Initialise - Entering method");

    bool ok = DataSourceI::Initialise(data);

    if (!ok) {
        log_error("SDNSubscriber::SetConfiguredDatabase - Parent virtual method failed");
        REPORT_ERROR(ErrorManagement::InternalSetupError, "Parent virtual method failed");
    }

    // Retrieve and verify network interface name
    if (!data.Read("Interface", ifaceName)) {
        log_error("SDNSubscriber::Initialise - Interface attribute must be specified");
        REPORT_ERROR(ErrorManagement::ParametersError, "Interface must be specified");
ok = false;
    } else {
        log_info("SDNSubscriber::Initialise - Interface is '%s'", ifaceName.Buffer());
    } 

    if (!net_is_interface_valid(ifaceName.Buffer())) {
        log_error("SDNSubscriber::Initialise - Interface '%s' is not valid", ifaceName.Buffer());
        REPORT_ERROR(ErrorManagement::ParametersError, "Interface must be a valid identifier");
ok = false;
    } else {
        log_info("SDNSubscriber::Initialise - Interface '%s' is valid", ifaceName.Buffer());
    } 

    // Retrieve and verify topic name
    if (!data.Read("Topic", topicName)) {
        log_error("SDNSubscriber::Initialise - Topic must be specified");
        REPORT_ERROR(ErrorManagement::ParametersError, "Topic must be specified");
ok = false;
    } else {
        log_info("SDNSubscriber::Initialise - Topic name is '%s'", topicName.Buffer());
    }

    // The topic name is used to generate UDP/IPv4 multicast mapping. Optionally, the mapping
    // to a destination '<address>:<port>' can be explicitly defined
    if (data.Read("Address", destAddr)) {

        log_info("SDNSubscriber::Initialise - Explicit destination address '%s'", destAddr.Buffer());

if (!sdn_is_address_valid(destAddr.Buffer())) {
    log_error("SDNSubscriber::Initialise - Address '%s' is not valid", destAddr.Buffer());
    REPORT_ERROR(ErrorManagement::ParametersError, "Address must be a valid identifier, i.e. '<IP_addr>:<port>'");
    ok = false;
} else {
    log_info("SDNSubscriber::Initialise - Valid destination address '%s'", destAddr.Buffer());
}

    }

    // Timeout parameter
    uint32 timeout;
    if (data.Read("Timeout", timeout)) {
        log_info("SDNSubscriber::Initialise - Explicit timeout '%u'", timeout);
TTTimeout = timeout;
    }

    // Synchronising mode parameter
    if (data.Read("Mode", modeName)) {
        log_info("SDNSubscriber::Initialise - Explicit synchronisation mode '%s'", modeName.Buffer());
if ((modeName == "Default") || (modeName == "Caching")) {
    mode = Default;
} else if (modeName == "Synchronising") {
    mode = Synchronising;
} else {
    log_error("SDNSubscriber::Initialise - Mode '%s' is not valid", modeName.Buffer());
    REPORT_ERROR(ErrorManagement::ParametersError, "Mode must be a valid identifier, i.e. Default|Caching|Synchronising");
    ok = false;
}
    }

    log_trace("SDNSubscriber::Initialise - Leaving method");

    return ok;
}

bool SDNSubscriber::SetConfiguredDatabase(StructuredDataI& data) {

    log_trace("SDNSubscriber::SetConfiguredDatabase - Entering method");

    bool ok = DataSourceI::SetConfiguredDatabase(data);

    if (!ok) {
        log_error("SDNSubscriber::SetConfiguredDatabase - Parent virtual method failed");
        REPORT_ERROR(ErrorManagement::InternalSetupError, "Parent virtual method failed");
    }

    nOfSignals = GetNumberOfSignals();

    ok = (nOfSignals > 0u);

    if (!ok) {
        log_error("SDNSubscriber::SetConfiguredDatabase - nOfSignals must be > 0u");
        REPORT_ERROR(ErrorManagement::ParametersError, "nOfSignals must be > 0u");
    } else {
        log_info("SDNSubscriber::SetConfiguredDatabase - Number of signals '%u'", nOfSignals);
    }
      
    log_trace("SDNSubscriber::SetConfiguredDatabase - Leaving method");

    return ok;
}

bool SDNSubscriber::AllocateMemory() {

    log_trace("SDNSubscriber::AllocateMemory - Entering method");

    // Instantiate a sdn::Metadata structure to configure the topic
    sdn::Metadata_t mdata; 
    
    if (destAddr.Size() == 0) { // Topic defined by name
        sdn::Topic_InitializeMetadata(mdata, (const char*) topicName.Buffer(), 0);
    } else { // An address as been explicitly provided
        sdn::Topic_InitializeMetadata(mdata, (const char*) topicName.Buffer(), 0, (const char*) destAddr.Buffer());
    }

    // Instantiate SDN topic from metadata specification
    topic = new sdn::Topic; topic->SetMetadata(mdata);
    
    bool ok = true;
    uint32 signalIndex = 0u;

    // Create one topic attribute for each signal
    for (signalIndex = 0u; (signalIndex < nOfSignals) && (ok); signalIndex++) {

        TypeDescriptor signalType = GetSignalType(signalIndex);
StreamString signalTypeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(signalType);

        StreamString signalName;

ok = GetSignalName(signalIndex, signalName);

if (!ok) {
  log_warning("SDNSubscriber::AllocateMemory - Unable to query signal '%u' name", signalIndex);
  REPORT_ERROR(ErrorManagement::Warning, "Unable to query signal name");
  // This is actually not so important ... would synthetise signal name in this case ... if I knew how
  ok = true;
}

uint32 signalNOfElements;

if (ok) {
    ok = GetSignalNumberOfElements(signalIndex, signalNOfElements);
}

uint8 signalNOfDimensions;

if (ok) {
    ok = GetSignalNumberOfDimensions(signalIndex, signalNOfDimensions);
}

if (signalNOfDimensions > 1u) {
    signalNOfElements *= signalNOfDimensions;
}

if (ok) {
    ok = (topic->AddAttribute(signalIndex, signalName.Buffer(), signalTypeName.Buffer(), signalNOfElements) == STATUS_SUCCESS);
}

    }

    if (ok) {
        topic->SetUID(0u); // UID corresponds to the data type but it includes attributes name - Safer to clear with SDN core library 1.0.10
        ok = (topic->Configure() == STATUS_SUCCESS);
    }

    if (ok) {
        ok = topic->IsInitialized();
    }

    // Create sdn::Subscriber
    if (ok) {
        subscriber = new sdn::Subscriber(*topic);
    }

    if (ok) {
        ok = (subscriber->SetInterface((char*) ifaceName.Buffer()) == STATUS_SUCCESS);
    }

    if (ok) {
        ok = (subscriber->Configure() == STATUS_SUCCESS);
    }

    log_trace("SDNSubscriber::AllocateMemory - Leaving method");

    return ok;
}

uint32 SDNSubscriber::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool SDNSubscriber::GetSignalMemoryBuffer(const uint32 signalIdx,
  const uint32 bufferIdx,
  void*& signalAddress) {

    log_trace("SDNSubscriber::GetSignalMemoryBuffer - Entering method");

    bool ok = (signalIdx < nOfSignals);

    if (ok) {
        ok = (topic != NULL_PTR(sdn::Topic *));
    }

    if (ok) {
        ok = (topic->GetTypeDefinition() != NULL_PTR(sdn::base::AnyType *));
    }

    if (ok) {
        signalAddress = topic->GetTypeDefinition()->GetAttributeReference(signalIdx);
        log_info("SDNSubscriber::GetSignalMemoryBuffer - Reference of signal '%u' if '%p'", signalIdx, signalAddress);
    }

    log_trace("SDNSubscriber::GetSignalMemoryBuffer - Leaving method");

    return ok;
}

const char8* SDNSubscriber::GetBrokerName(StructuredDataI& data,
  const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == InputSignals) {
        if (mode == Default) { // Caching, non-synchronising for RT thread
            brokerName = "MemoryMapInputBroker";
} else { // Synchronising RT thread
    brokerName = "MemoryMapSynchronisedInputBroker";
}
    } else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with OutputSignals");
    }
    return brokerName;
}

bool SDNSubscriber::GetInputBrokers(ReferenceContainer& inputBrokers,
    const char8* const functionName,
    void* const gamMemPtr) {

    log_trace("SDNSubscriber::GetInputBrokers - Entering method");

    bool ok = true;

    if (mode == Default) { // Caching, non-synchronising for RT thread
        ReferenceT<BrokerI> broker("MemoryMapInputBroker");

if (ok) {
    ok = broker.IsValid();
}

if (ok) {
    ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
}

if (ok) {
    ok = inputBrokers.Insert(broker);
}
    } else { // Synchronising RT thread
        ReferenceT<BrokerI> broker("MemoryMapSynchronisedInputBroker");

if (ok) {
    ok = broker.IsValid();
}

if (ok) {
    ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
}

if (ok) {
    ok = inputBrokers.Insert(broker);
}
    }

    log_trace("SDNSubscriber::GetInputBrokers - Leaving method");

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns false irrespectively of the input parameters.*/
bool SDNSubscriber::GetOutputBrokers(ReferenceContainer& outputBrokers,
     const char8* const functionName,
     void* const gamMemPtr) {
    return false;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns irrespectively of the input parameters.*/
bool SDNSubscriber::PrepareNextState(const char8* const currentStateName,
     const char8* const nextStateName) {

    log_trace("SDNSubscriber::PrepareNextState - Entering method");

    bool ok = (subscriber != NULL_PTR(sdn::Subscriber *));

    if (!ok) {
        log_error("SDNSubscriber::PrepareNextState - sdn::Subscriber has not been initiaised");
        REPORT_ERROR(ErrorManagement::FatalError, "sdn::Subscriber has not been initiaised");
    }

    if (ok) {
        // Empty the receive buffer (probably not necessary anymore since the embedded thread should take care of that)
        while (subscriber->Receive(0ul) == STATUS_SUCCESS);
    }

    if (ok) {
        if (executor.GetStatus() == EmbeddedThreadI::OffState) {
            // Start the SingleThreadService
            ok = executor.Start();
}
    }

    log_trace("SDNSubscriber::PrepareNextState - Leaving method");

    return ok;
}

bool SDNSubscriber::Synchronise() {

    log_trace("SDNSubscriber::Synchronise - Entering method");

    bool ok = (mode == Synchronising); // Synchronising RT thread

    if (!ok) {
        log_error("SDNSubscriber::Synchronise - Invalid method call for non-synchronising DataSourceI");
        REPORT_ERROR(ErrorManagement::FatalError, "SDNSubscriber operates in Default (caching) mode");
    }

    if (ok) {
        // Wait till next SDN topic is received
        ErrorManagement::ErrorType err = synchronisingSem.ResetWait(TTTimeout);
        ok = err.ErrorsCleared();
    }

    log_trace("SDNSubscriber::Synchronise - Leaving method");

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the method operates regardles of the input parameter.*/
ErrorManagement::ErrorType SDNSubscriber::Execute(const ExecutionInfo& info) {

    ErrorManagement::ErrorType err = ErrorManagement::NoError;

    bool ok = (subscriber != NULL_PTR(sdn::Subscriber *));

    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "sdn::Subscriber has not been initiaised");
err.SetError(ErrorManagement::FatalError);
        Sleep::MSec(100u);
    }

    if (ok) {
        // Receive with timeout in order to avoid unresponsive thread
        ok = (subscriber->Receive(100000000ul) == STATUS_SUCCESS);
if (!ok) {
            REPORT_ERROR(ErrorManagement::Timeout, "Failed to receive");
    err.SetError(ErrorManagement::Timeout);
}
    }

    if (ok) {
        err = synchronisingSem.Post();
    }

    if (err.Contains(ErrorManagement::Timeout)) {
        // Ignore Timeout error for now
        err.ClearError(ErrorManagement::Timeout);
    }

    return err;
}

CLASS_REGISTER(SDNSubscriber, "1.0.10")

} /* namespace MARTe */
