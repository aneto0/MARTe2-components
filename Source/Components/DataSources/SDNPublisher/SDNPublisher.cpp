/**
 * @file SDNPublisher.cpp
 * @brief Source file for class SDNPublisher
 * @date 12/12/2016
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
#include "ErrorManagement.h"
#include "ErrorInformation.h"
#include "GlobalObjectsDatabase.h"
#include "GAM.h"
#include "SDNPublisher.h"

#include "sdn-api.h" /* SDN core library - API definition (sdn::core) */

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
#if 0
static ccs::log::Severity_t org_filter = ccs::log::SetFilter(LOG_TRACE);
#else
static ccs::log::Severity_t org_filter = ccs::log::SetFilter(LOG_INFO);
#undef log_trace
#define log_trace log_info
#endif
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

SDNPublisher::SDNPublisher() :
        DataSourceI() {

    log_trace("SDNPublisher::SDNPublisher - Entering method");

    nOfSignals = 0u;

    topic = NULL_PTR(sdn::Topic *);
    publisher = NULL_PTR(sdn::Publisher *);

    log_trace("SDNPublisher::SDNPublisher - Leaving method");

}

SDNPublisher::~SDNPublisher() {

    log_trace("SDNPublisher::~SDNPublisher - Entering method");

    if (topic != NULL_PTR(sdn::Topic *)) {
        delete topic; topic = NULL_PTR(sdn::Topic *);
    }

    if (publisher != NULL_PTR(sdn::Publisher *)) {
        delete publisher; publisher = NULL_PTR(sdn::Publisher *);
    }

    log_trace("SDNPublisher::~SDNPublisher - Leaving method");

}

bool SDNPublisher::Initialise(StructuredDataI &data) {

    log_trace("SDNPublisher::Initialise - Entering method");

    bool ok = DataSourceI::Initialise(data);

    if (!ok) {
        log_error("SDNPublisher::SetConfiguredDatabase - Parent virtual method failed");
        REPORT_ERROR(ErrorManagement::InternalSetupError, "Parent virtual method failed");
    }

    // Retrieve and verify network interface name
    if (!data.Read("Interface", ifaceName)) {
        log_error("SDNPublisher::Initialise - Interface attribute must be specified");
        REPORT_ERROR(ErrorManagement::ParametersError, "Interface must be specified");
	ok = false;
    } else {
        log_info("SDNPublisher::Initialise - Interface is '%s'", ifaceName.Buffer());
    } 

    if (!net_is_interface_valid(ifaceName.Buffer())) {
        log_error("SDNPublisher::Initialise - Interface '%s' is not valid", ifaceName.Buffer());
        REPORT_ERROR(ErrorManagement::ParametersError, "Interface must be a valid identifier");
	ok = false;
    } else {
        log_info("SDNPublisher::Initialise - Interface '%s' is valid", ifaceName.Buffer());
    } 

    // Retrieve and verify topic name
    if (!data.Read("Topic", topicName)) {
        log_error("SDNPublisher::Initialise - Topic must be specified");
        REPORT_ERROR(ErrorManagement::ParametersError, "Topic must be specified");
	ok = false;
    } else {
        log_info("SDNPublisher::Initialise - Topic name is '%s'", topicName.Buffer());
    }

    // The topic name is used to generate UDP/IPv4 multicast mapping. Optionally, the mapping
    // to a destination '<address>:<port>' can be explicitly defined
    if (data.Read("Address", destAddr)) {

        log_info("SDNPublisher::Initialise - Explicit destination address '%s'", destAddr.Buffer());

	if (!sdn_is_address_valid(destAddr.Buffer())) {
	    log_error("SDNPublisher::Initialise - Address '%s' is not valid", destAddr.Buffer());
	    REPORT_ERROR(ErrorManagement::ParametersError, "Address must be a valid identifier, i.e. '<IP_addr>:<port>'");
	    ok = false;
	} else {
	    log_info("SDNPublisher::Initialise - Valid destination address '%s'", destAddr.Buffer());
	}

    }

    log_trace("SDNPublisher::Initialise - Leaving method");

    return ok;
}

bool SDNPublisher::SetConfiguredDatabase(StructuredDataI& data) {

    log_trace("SDNPublisher::SetConfiguredDatabase - Entering method");

    bool ok = DataSourceI::SetConfiguredDatabase(data);

    if (!ok) {
        log_error("SDNPublisher::SetConfiguredDatabase - Parent virtual method failed");
        REPORT_ERROR(ErrorManagement::InternalSetupError, "Parent virtual method failed");
    }

    nOfSignals = GetNumberOfSignals();

    ok = (nOfSignals > 0u);

    if (!ok) {
        log_error("SDNPublisher::SetConfiguredDatabase - nOfSignals must be > 0u");
        REPORT_ERROR(ErrorManagement::ParametersError, "nOfSignals must be > 0u");
    } else {
        log_info("SDNPublisher::SetConfiguredDatabase - Number of signals '%u'", nOfSignals);
    }
      
    log_trace("SDNPublisher::SetConfiguredDatabase - Leaving method");

    return ok;
}

bool SDNPublisher::AllocateMemory() {

    log_trace("SDNPublisher::AllocateMemory - Entering method");

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
	  log_warning("SDNPublisher::AllocateMemory - Unable to query signal '%u' name", signalIndex);
	  REPORT_ERROR(ErrorManagement::Warning, "Unable to query signal name");
	  // This is actually not so important ... would synthetise signal name in this case ... if I knew how
	  ok = true;
	} 

	uint32 signalNOfElements;

	if (ok) {
	    ok = GetSignalNumberOfElements(signalIndex, signalNOfElements);
	}

	if (ok) {
	    ok = (topic->AddAttribute(signalIndex, signalName.Buffer(), signalTypeName.Buffer(), signalNOfElements) == STATUS_SUCCESS);
	}

    }

    if (ok) {
        ok = (topic->Configure() == STATUS_SUCCESS);
    }

    if (ok) {
        ok = topic->IsInitialized();
    }

    // Create sdn::Publisher
    if (ok) {
        publisher = new sdn::Publisher(*topic);
    }

    if (ok) {
        ok = (publisher->SetInterface((char*) ifaceName.Buffer()) == STATUS_SUCCESS);
    }

    if (ok) {
        ok = (publisher->Configure() == STATUS_SUCCESS);
    }

    log_trace("SDNPublisher::AllocateMemory - Leaving method");

    return ok;
}

uint32 SDNPublisher::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool SDNPublisher::GetSignalMemoryBuffer(const uint32 signalIdx,
                                         const uint32 bufferIdx,
                                         void*& signalAddress) {

    log_trace("SDNPublisher::GetSignalMemoryBuffer - Entering method");

    bool ok = (signalIdx < nOfSignals);

    if (ok) {
        ok = (topic != NULL_PTR(sdn::Topic *));
    }

    if (ok) {
        ok = (topic->GetTypeDefinition() != NULL_PTR(sdn::base::AnyType *));
    }

    if (ok) {
        signalAddress = topic->GetTypeDefinition()->GetAttributeReference(signalIdx);
        log_info("SDNPublisher::GetSignalMemoryBuffer - Reference of signal '%u' if '%p'", signalIdx, signalAddress);
    }

    log_trace("SDNPublisher::GetSignalMemoryBuffer - Leaving method");

    return ok;
}

const char8* SDNPublisher::GetBrokerName(StructuredDataI& data,
                                         const SignalDirection direction) {
#if 1
    return "MemoryMapSynchronisedOutputBroker";
#else
    return "MemoryMapOutputBroker";
#endif
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns false irrespectively of the input parameters.*/
bool SDNPublisher::GetInputBrokers(ReferenceContainer& inputBrokers,
                                   const char8* const functionName,
                                   void* const gamMemPtr) {
    return false;
}

bool SDNPublisher::GetOutputBrokers(ReferenceContainer& outputBrokers,
                                    const char8* const functionName,
                                    void* const gamMemPtr) {

    log_trace("SDNPublisher::GetOutputBrokers - Entering method");
#if 1
    ReferenceT<BrokerI> broker("MemoryMapSynchronisedOutputBroker");
#else
    ReferenceT<BrokerI> broker("MemoryMapOutputBroker");
#endif
    bool ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    if (ok) {
        ok = outputBrokers.Insert(broker);
    }

    log_trace("SDNPublisher::GetOutputBrokers - Leaving method");

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns true irrespectively of the input parameters.*/
bool SDNPublisher::PrepareNextState(const char8* const currentStateName,
                                    const char8* const nextStateName) {
    return true;
}

bool SDNPublisher::Synchronise() {

    log_trace("SDNPublisher::Synchronise - Entering method");

    bool ok = (publisher->Publish() == STATUS_SUCCESS);

    log_trace("SDNPublisher::Synchronise - Leaving method");

    return ok;
}

CLASS_REGISTER(SDNPublisher, "1.0.9")

} /* namespace MARTe */
