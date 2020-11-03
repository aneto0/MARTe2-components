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
#include <new> //For the std::nothrow.

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "BrokerI.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "SDNSubscriber.h"

#ifdef FEATURE_10840
#include "Endianity.h"
#endif
#include "sdn-api.h" /* SDN core library - API definition (sdn::core) */
/*lint -estring(843,"*crc.h*") ignore could be declared const warning from the crc.h header*/
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

SDNSubscriber::SDNSubscriber() :
        DataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this) {

    nOfSignals = 0u;
    nOfTriggers = 0u;
    synchronising = false;
    cpuMask = 0u;
    TTTimeout = TTInfiniteWait;
    sdnHeaderAsSignal = false;

    topic = NULL_PTR(sdn::Topic *);
    subscriber = NULL_PTR(sdn::Subscriber *);

    if (!synchronisingSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create EventSem");
    }
    payloadNumberOfBits = NULL_PTR(uint16 *);
    payloadNumberOfElements = NULL_PTR(uint32 *);
    payloadAddresses = NULL_PTR(void **);
}

/*lint -e{1551} the destructor must guarantee that the SDNSubscriber SingleThreadService is stopped and that all the SDN objects are destroyed.*/
SDNSubscriber::~SDNSubscriber() {

    if (!synchronisingSem.Post()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not post EventSem");
    }

    if (!executor.Stop()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService");
    }

    if (subscriber != NULL_PTR(sdn::Subscriber *)) {
        delete subscriber;
        subscriber = NULL_PTR(sdn::Subscriber *);
    }

    if (topic != NULL_PTR(sdn::Topic *)) {
        delete topic;
        topic = NULL_PTR(sdn::Topic *);
    }

    if (payloadNumberOfBits != NULL_PTR(uint16 *)) {
        delete[] payloadNumberOfBits;
    }

    if (payloadNumberOfElements != NULL_PTR(uint32 *)) {
        delete[] payloadNumberOfElements;
    }

    if (payloadAddresses != NULL_PTR(void **)) {
        delete[] payloadAddresses;
    }
}

bool SDNSubscriber::Initialise(StructuredDataI &data) {

    bool ok = DataSourceI::Initialise(data);

    // Retrieve and verify network interface name
    if (!data.Read("Interface", ifaceName)) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Interface must be specified");
        ok = false;
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "Interface is '%s'", ifaceName.Buffer());
    }
#ifdef FEATURE_10840
    if (!sdn::HelperTools::IsInterfaceValid(ifaceName.Buffer())) {
#else
    if (!net_is_interface_valid(ifaceName.Buffer())) {
#endif
        REPORT_ERROR(ErrorManagement::ParametersError, "Interface must be a valid identifier");
        ok = false;
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "Interface '%s' is valid", ifaceName.Buffer());
    }

    // Retrieve and verify topic name
    if (!data.Read("Topic", topicName)) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Topic must be specified");
        ok = false;
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "Topic name is '%s'", topicName.Buffer());
    }

    // The topic name is used to generate UDP/IPv4 multicast mapping. Optionally, the mapping
    // to a destination '<address>:<port>' can be explicitly defined
    if (data.Read("Address", destAddr)) {
#ifdef FEATURE_10840
        if (!sdn::HelperTools::IsAddressValid(destAddr.Buffer())) {
#else
        if (!sdn_is_address_valid(destAddr.Buffer())) {
#endif
            REPORT_ERROR(ErrorManagement::ParametersError, "Address must be a valid identifier, i.e. '<IP_addr>:<port>'");
            ok = false;
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Valid destination address '%s'", destAddr.Buffer());
        }
    }

    // Timeout parameter
    uint32 timeout;
    if (data.Read("Timeout", timeout)) {
        REPORT_ERROR(ErrorManagement::Information, "Explicit subscriber timeout '%u'", timeout);
        TTTimeout = timeout;
    }

    if (data.Read("CPUs", cpuMask)) {
        REPORT_ERROR(ErrorManagement::Information, "Explicit thread affinity '%u'", cpuMask);
    }

    return ok;
}

bool SDNSubscriber::SetConfiguredDatabase(StructuredDataI& data) {

    bool ok = DataSourceI::SetConfiguredDatabase(data);

    nOfSignals = GetNumberOfSignals();

    if (ok) {
        ok = (nOfSignals > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "nOfSignals must be > 0u");
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Number of signals '%u'", nOfSignals);
        }
    }

    if (ok) {
        ok = (nOfTriggers <= 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with multiple synchronising signals");
        }
    }

    if (ok) {
        StreamString firstSignalName;
        ok = GetSignalName(0u, firstSignalName);
        if (ok) {
            sdnHeaderAsSignal = (firstSignalName == "Header");
        }
    }

    //Check if someone is trying to read

    return ok;
}

bool SDNSubscriber::AllocateMemory() {

    // Instantiate a sdn::Metadata structure to configure the topic
    sdn::Metadata_t mdata;

    if (destAddr.Size() == 0u) { // Topic defined by name
        sdn::Topic_InitializeMetadata(mdata, topicName.Buffer(), 0u);
    }
    else { // An address as been explicitly provided
        sdn::Topic_InitializeMetadata(mdata, topicName.Buffer(), 0u, destAddr.Buffer());
    }

    // Instantiate SDN topic from metadata specification
    topic = new sdn::Topic;
    topic->SetMetadata(mdata);

    bool ok = true;

    payloadNumberOfBits = new uint16[nOfSignals];
    payloadNumberOfElements = new uint32[nOfSignals];
    payloadAddresses = new void *[nOfSignals];

    uint32 signalIndex;

    // Create one topic attribute for each signal
    for (signalIndex = 0u; (signalIndex < nOfSignals) && (ok); signalIndex++) {

        TypeDescriptor signalType = GetSignalType(signalIndex);
        StreamString signalTypeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(signalType);
        StreamString signalName;

        ok = GetSignalName(signalIndex, signalName);

        uint32 signalNOfElements = 0u;

        if (ok) {
            ok = GetSignalNumberOfElements(signalIndex, signalNOfElements);
        }

        uint8 signalNOfDimensions = 0u;

        if (ok) {
            ok = GetSignalNumberOfDimensions(signalIndex, signalNOfDimensions);
        }

        if (signalNOfDimensions > 1u) {
            signalNOfElements *= signalNOfDimensions;
        }

        //lint -e{613} payloadNumberOfBits and payloadNumberOfElements cannot be NULL otherwise ok would be false
        if (ok) {
            payloadNumberOfBits[signalIndex] = signalType.numberOfBits;
            payloadNumberOfElements[signalIndex] = signalNOfElements;
        }

        if (ok) {
            if (sdnHeaderAsSignal) {
                if (signalIndex > 0u) {
                    ok = (topic->AddAttribute(signalIndex - 1u, signalName.Buffer(), signalTypeName.Buffer(), signalNOfElements) == STATUS_SUCCESS);
                }
            }
            else {
                ok = (topic->AddAttribute(signalIndex, signalName.Buffer(), signalTypeName.Buffer(), signalNOfElements) == STATUS_SUCCESS);
            }
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
        subscriber = new (std::nothrow) sdn::Subscriber(*topic);
        //lint -e{948} std::nothrow => subscriber may be NULL
        ok = (NULL_PTR(sdn::Subscriber *)!= subscriber);
    }

    if (ok) {
        /*lint -e{613} The reference can not be NULL in this portion of the code.*/
        ok = (subscriber->SetInterface(ifaceName.Buffer()) == STATUS_SUCCESS);
    }

    if (ok) {
        /*lint -e{613} The reference can not be NULL in this portion of the code.*/
        ok = (subscriber->Configure() == STATUS_SUCCESS);
    }

    sdn::Header_t *header = NULL_PTR(sdn::Header_t *);
    if (ok) {
        /*lint -e{613} subscriber cannot be NULL in this portion of the code as otherwise ok would be false.*/
        header = static_cast<sdn::Header_t *>(subscriber->GetTopicHeader());
        ok = (header != NULL_PTR(sdn::Header_t *));
    }
    if (ok) {
        if (sdnHeaderAsSignal) {
            /*lint -e{613} header cannot be NULL in this portion of the code as otherwise ok would be false.*/
            uint32 expectedSdnHeaderSize = header->header_size;
            uint32 sdnHeaderSignalSize;
            ok = GetSignalByteSize(0u, sdnHeaderSignalSize);
            if (ok) {
                ok = (expectedSdnHeaderSize == sdnHeaderSignalSize);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Incompatible header size. Expected %d and read %d", expectedSdnHeaderSize, sdnHeaderSignalSize);
                }
            }
        }

        /*lint -e{613} payloadAddresses cannot be NULL in this portion of the code as otherwise ok would be false.*/
        for (signalIndex = 0u; (signalIndex < nOfSignals) && (ok); signalIndex++) {
            if (sdnHeaderAsSignal) {
                if (signalIndex > 0u) {
                    payloadAddresses[signalIndex] = topic->GetTypeDefinition()->GetAttributeReference(signalIndex - 1u);
                }
                else {
                    payloadAddresses[signalIndex] = subscriber->GetTopicHeader();
                }
            }
            else {
                payloadAddresses[signalIndex] = topic->GetTypeDefinition()->GetAttributeReference(signalIndex);
            }
        }
    }

    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "Failed to instantiate sdn::Subscriber");
    }

    return ok;
}

uint32 SDNSubscriber::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool SDNSubscriber::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {

    bool ok = (signalIdx < nOfSignals);

    if (ok) {
        ok = (topic != NULL_PTR(sdn::Topic *));
    }

    if (ok) {
        /*lint -e{613} The reference can not be NULL in this portion of the code.*/
        ok = (topic->GetTypeDefinition() != NULL_PTR(sdn::base::AnyType *));
    }

    if (ok) {
        /*lint -e{613} The reference can not be NULL in this portion of the code.*/
        signalAddress = payloadAddresses[signalIdx];
    }

    return ok;
}

const char8* SDNSubscriber::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {

    const char8 *brokerName = NULL_PTR(const char8 *);

    if (direction == InputSignals) {

        float32 frequency = 0.F;

        if (!data.Read("Frequency", frequency)) {
            frequency = -1.F;
        }

        if (frequency > 0.F) {
            brokerName = "MemoryMapSynchronisedInputBroker";
            nOfTriggers++;
            synchronising = true;
        }
        else {
            brokerName = "MemoryMapInputBroker";
        }

    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with OutputSignals");
    }

    return brokerName;
}

bool SDNSubscriber::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {

    // Check if this function has a synchronisation point (i.e. a signal which has Frequency > 0)
    uint32 functionIdx = 0u;
    uint32 nOfFunctionSignals = 0u;
    uint32 signalIndex;
    bool synchGAM = false;
    bool ok = GetFunctionIndex(functionIdx, functionName);

    if (ok) {
        ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfFunctionSignals);
    }

    float32 frequency = 0.F;

    for (signalIndex = 0u; (signalIndex < nOfFunctionSignals) && (ok) && (!synchGAM); signalIndex++) {
        ok = GetFunctionSignalReadFrequency(InputSignals, functionIdx, signalIndex, frequency);

        if (ok) {
            synchGAM = (frequency > 0.F);
        }
    }

    // Test if there is a multi-sample signal for this function.
    for (signalIndex = 0u; (signalIndex < nOfFunctionSignals) && (ok); signalIndex++) {
        // This version does not support multi-sample signals
        uint32 samples = 0u;
        ok = GetFunctionSignalSamples(InputSignals, functionIdx, signalIndex, samples);

        if (ok) {
            ok = (samples == 1u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Multi-sample signals are not supported");
        }
    }

    if (ok) {
        if (synchGAM) {

            // A synchronizing broker is inserted in case one signal at least is declared with
            // synchronising property. The GAM may also encompass non-synchronizing signals
            // which require a standard broker.

            ReferenceT<MemoryMapSynchronisedInputBroker> brokerSync("MemoryMapSynchronisedInputBroker");
            ok = brokerSync.IsValid();

            if (ok) {
                ok = brokerSync->Init(InputSignals, *this, functionName, gamMemPtr);
            }

            if (ok) {
                ok = inputBrokers.Insert(brokerSync);
            }

            // Must also add the signals which are not synchronous but that belong to the same GAM...
            if (ok) {
                if (nOfFunctionSignals > 1u) {
                    ReferenceT<MemoryMapInputBroker> brokerNotSync("MemoryMapInputBroker");
                    ok = brokerNotSync.IsValid();

                    if (ok) {
                        ok = brokerNotSync->Init(InputSignals, *this, functionName, gamMemPtr);
                    }

                    if (ok) {
                        ok = inputBrokers.Insert(brokerNotSync);
                    }
                }
            }
        }
        else {
            ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
            ok = broker.IsValid();

            if (ok) {
                ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
            }

            if (ok) {
                ok = inputBrokers.Insert(broker);
            }
        }
    }

    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "Failed to insert InputBroker");
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns false irrespectively of the input parameters.*/
bool SDNSubscriber::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns irrespectively of the input parameters.*/
bool SDNSubscriber::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {

    bool ok = (subscriber != NULL_PTR(sdn::Subscriber *));

    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "sdn::Subscriber has not been initialised");
    }

    if (ok) {
        bool empty = false;
        while (!empty) {
            /*lint -e{613} The reference can not be NULL in this portion of the code.*/
            empty = (subscriber->Receive(0ul) != STATUS_SUCCESS);
        }
    }

    if (ok) {
        if (executor.GetStatus() == EmbeddedThreadI::OffState) {
            if (cpuMask != 0u) {
                executor.SetCPUMask(cpuMask);
            }
            // Start the SingleThreadService
            executor.SetName(GetName());
            ok = executor.Start();
        }
    }

    return ok;
}

bool SDNSubscriber::Synchronise() {

    bool ok = synchronising; // DataSource is synchronising RT thread

    if (ok) {
        // Wait till next SDN topic is received
        ErrorManagement::ErrorType err = synchronisingSem.ResetWait(TTTimeout);
        ok = err.ErrorsCleared();
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the method operates regardless of the input parameter.*/
ErrorManagement::ErrorType SDNSubscriber::Execute(ExecutionInfo& info) {

    ErrorManagement::ErrorType err = ErrorManagement::NoError;

    bool ok = (subscriber != NULL_PTR(sdn::Subscriber *));

    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "sdn::Subscriber has not been initiaised");
        err.SetError(ErrorManagement::FatalError);
        Sleep::MSec(100u);
    }

    if (ok) {
        /*lint -e{613} The reference can not be NULL in this portion of the code.*/
        ok = (subscriber->Receive(100000000ul) == STATUS_SUCCESS);

        if (!ok) {
            //REPORT_ERROR(ErrorManagement::Timeout, "sdn::Subscriber failed to receive topic");
            err.SetError(ErrorManagement::Timeout);
        }
#ifdef FEATURE_10840
        else {
            if (!subscriber->IsPayloadOrdered()) {
                // Convert payload from network byte order
                uint32 signalIndex = 0u;
                if (sdnHeaderAsSignal) {
                    sdn::Header_t *header = static_cast<sdn::Header_t *>(payloadAddresses[0u]);
                    Endianity::FromBigEndian(header->header_size);
                    Endianity::FromBigEndian(reinterpret_cast<uint64 &>(header->recv_time));
                    Endianity::FromBigEndian(reinterpret_cast<uint64 &>(header->send_time));
                    Endianity::FromBigEndian(reinterpret_cast<uint64 &>(header->topic_counter));
                    Endianity::FromBigEndian(header->topic_size);
                    Endianity::FromBigEndian(header->topic_uid);
                    Endianity::FromBigEndian(header->topic_version);
                    signalIndex = 1u;
                }
                for (; (signalIndex < nOfSignals); signalIndex++) {
                    if (payloadNumberOfBits[signalIndex] == 16u) {
                        uint32 elementIndex;
                        for (elementIndex = 0u; (elementIndex < payloadNumberOfElements[signalIndex]); elementIndex++) {
                            Endianity::FromBigEndian(reinterpret_cast<uint16 *>(payloadAddresses[signalIndex])[elementIndex]);
                        }
                    }
                    if (payloadNumberOfBits[signalIndex] == 32u) {
                        uint32 elementIndex;
                        for (elementIndex = 0u; (elementIndex < payloadNumberOfElements[signalIndex]); elementIndex++) {
                            Endianity::FromBigEndian(reinterpret_cast<uint32 *>(payloadAddresses[signalIndex])[elementIndex]);
                        }
                    }
                    if (payloadNumberOfBits[signalIndex] == 64u) {
                        uint32 elementIndex;
                        for (elementIndex = 0u; (elementIndex < payloadNumberOfElements[signalIndex]); elementIndex++) {
                            Endianity::FromBigEndian(reinterpret_cast<uint64 *>(payloadAddresses[signalIndex])[elementIndex]);
                        }
                    }
                }
            }
        }
#endif
    }

    if (ok) {
        ok = synchronisingSem.Post();

        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "EventSem::Post failed");
            err.SetError(ErrorManagement::FatalError);
        }
    }

    if (err.Contains(ErrorManagement::Timeout)) {
        // Ignore Timeout error for now
        err.ClearError(ErrorManagement::Timeout);
    }

#ifdef FEATURE_10840
    if (subscriber != NULL_PTR(sdn::Subscriber *)) {
       // Perform housekeeping activities .. irrespective of status
       (void)subscriber->DoBackgroundActivity();
    }
#endif

    return err;
}
#ifdef FEATURE_10840
CLASS_REGISTER(SDNSubscriber, "1.2")
#else
CLASS_REGISTER(SDNSubscriber, "1.0.11")
#endif
} /* namespace MARTe */
