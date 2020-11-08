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
 *
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
#include "Endianity.h"
#include "ErrorInformation.h"
#include "ErrorManagement.h"
#include "GAM.h"
#include "GlobalObjectsDatabase.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "SDNPublisher.h"
#include "sdn-api.h" /* SDN core library - API definition (sdn::core) */
/*lint -estring(843,"*crc.h*") ignore could be declared const warning from the crc.h header*/
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

SDNPublisher::SDNPublisher() :
        DataSourceI() {

    nOfSignals = 0u;
    nOfTriggers = 0u;
    sourcePort = 0u;
    networkByteOrder = false; // Assume host native byte order used for SDN payload
    topic = NULL_PTR(sdn::Topic *);
    publisher = NULL_PTR(sdn::Publisher *);
    payloadNumberOfBits = NULL_PTR(uint16 *);
    payloadNumberOfElements = NULL_PTR(uint32 *);
    payloadAddresses = NULL_PTR(void **);
    sdnHeaderAsSignal = false;
}

/*lint -e{1551} the destructor must guarantee that all the SDN objects are destroyed.*/
SDNPublisher::~SDNPublisher() {

    if (publisher != NULL_PTR(sdn::Publisher *)) {
        delete publisher;
        publisher = NULL_PTR(sdn::Publisher *);
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

bool SDNPublisher::Initialise(StructuredDataI &data) {

    bool ok = DataSourceI::Initialise(data);

    // Retrieve and verify network interface name
    if (!data.Read("Interface", ifaceName)) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Interface must be specified");
        ok = false;
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "SDN interface is '%s'", ifaceName.Buffer());
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
        REPORT_ERROR(ErrorManagement::Information, "SDN interface '%s' is valid", ifaceName.Buffer());
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
    // Read optional source port
    if (data.Read("SourcePort", sourcePort)) {
        REPORT_ERROR(ErrorManagement::Information, "Source port is '%!'", sourcePort);
    }

    // Read optional wire byte ordering
    uint32 byteOrder = 0u;
    if (data.Read("NetworkByteOrder", byteOrder)) {
        REPORT_ERROR(ErrorManagement::Information, "Network byte order is '%!'", byteOrder);
    }

    networkByteOrder = (0u != byteOrder);
    return ok;
}

bool SDNPublisher::SetConfiguredDatabase(StructuredDataI& data) {

    bool ok = DataSourceI::SetConfiguredDatabase(data);

    nOfSignals = GetNumberOfSignals();

    if (ok) {
        ok = (nOfSignals > 0u);
    }

    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "nOfSignals must be > 0u");
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "Number of signals '%u'", nOfSignals);
    }

    if (ok) {
        ok = (nOfTriggers == 1u);
    }

    if (!ok) {
        if (nOfTriggers == 0u) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Missing trigger signal");
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "DataSource not compatible with multiple synchronising signals");
        }
    }
    if (ok) {
        StreamString firstSignalName;
        ok = GetSignalName(0u, firstSignalName);
        if (ok) {
            sdnHeaderAsSignal = (firstSignalName == "Header");
        }
    }

    return ok;
}

bool SDNPublisher::AllocateMemory() {

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

    uint32 signalIndex = 0u;
    if (sdnHeaderAsSignal) {
        signalIndex = 1u;
    }
    // Create one topic attribute for each signal
    for (; (signalIndex < nOfSignals) && (ok); signalIndex++) {

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
                ok = (topic->AddAttribute(signalIndex - 1u, signalName.Buffer(), signalTypeName.Buffer(),
                                          signalNOfElements) == STATUS_SUCCESS);
            }
            else {
                ok = (topic->AddAttribute(signalIndex, signalName.Buffer(), signalTypeName.Buffer(), signalNOfElements)
                        == STATUS_SUCCESS);
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

    // Create sdn::Publisher
    if (ok) {
        publisher = new (std::nothrow) sdn::Publisher(*topic);
        //lint -e{948} std::nothrow => publisher may be NULL
        ok = (NULL_PTR(sdn::Publisher *)!= publisher);
    }

    if (ok) {
        /*lint -e{613} The reference can not be NULL in this portion of the code.*/
#ifdef FEATURE_10840
        if (0u != sourcePort) {
            ok = (publisher->SetInterface(ifaceName.Buffer(), sourcePort) == STATUS_SUCCESS);
        }
        else {
            ok = (publisher->SetInterface(ifaceName.Buffer()) == STATUS_SUCCESS);
        }
#else
        ok = (publisher->SetInterface(ifaceName.Buffer()) == STATUS_SUCCESS);
#endif
    }
#ifdef FEATURE_10840
    if (ok) {
        /*lint -e{613} The reference can not be NULL in this portion of the code.*/
        if (networkByteOrder) {
            REPORT_ERROR(ErrorManagement::Information, "Use network byte ordering on the wire");
            publisher->SetPayloadOrder(sdn::types::NetworkByteOrder);
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Use native host byte ordering on the wire");
        }
    }
#endif
    if (ok) {
        /*lint -e{613} The reference can not be NULL in this portion of the code.*/
        ok = (publisher->Configure() == STATUS_SUCCESS);
    }

    if (ok) {
        for (signalIndex = 0u; (signalIndex < nOfSignals) && (ok); signalIndex++) {
            void *signalAddress;
            ok = GetSignalMemoryBuffer(signalIndex, 0u, signalAddress);
            if (ok) {
                payloadAddresses[signalIndex] = signalAddress;
            }
        }
    }
    if (ok) {
        if (sdnHeaderAsSignal) {
            /*lint -e{613} header cannot be NULL in this portion of the code as otherwise ok would be false.*/
            sdn::Header_t * header = static_cast<sdn::Header_t *>(publisher->GetTopicHeader());
            uint32 expectedSdnHeaderSize = header->header_size;
            uint32 sdnHeaderSignalSize;
            ok = GetSignalByteSize(0u, sdnHeaderSignalSize);
            if (ok) {
                ok = (expectedSdnHeaderSize == sdnHeaderSignalSize);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Incompatible header size. Expected %d and read %d",
                                 expectedSdnHeaderSize, sdnHeaderSignalSize);
                }
            }
        }
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "Failed to instantiate sdn::Publisher");
    }

    return ok;
}

uint32 SDNPublisher::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool SDNPublisher::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {

    bool ok = (signalIdx < nOfSignals);

    if (ok) {
        ok = (NULL_PTR(sdn::Topic *)!= topic);
    }

    if (ok) {
        /*lint -e{613} The reference can not be NULL in this portion of the code.*/
        ok = (NULL_PTR(sdn::base::AnyType *)!= topic->GetTypeDefinition());
    }

    if (ok) {
        /*lint -e{613} The reference can not be NULL in this portion of the code.*/
        if (sdnHeaderAsSignal) {
            if (signalIdx > 0u) {
                signalAddress = topic->GetTypeDefinition()->GetAttributeReference(signalIdx - 1u);
            }
            else {
                signalAddress = publisher->GetTopicHeader();
            }
        }
        else {
            signalAddress = topic->GetTypeDefinition()->GetAttributeReference(signalIdx);
        }

    }

    return ok;
}

// The method is called for each signal connected to the DataSource
const char8* SDNPublisher::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {

    const char8 *brokerName = NULL_PTR(const char8 *);

    if (direction == OutputSignals) {

        uint32 trigger = 0u;

        if (!data.Read("Trigger", trigger)) {
            trigger = 0u;
        }

        if (trigger == 1u) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
            nOfTriggers++;
        }
        else {
            brokerName = "MemoryMapOutputBroker";
        }

    }
    else {
        StreamString aliasName;
        bool ok = data.Read("Alias", aliasName);
        if (ok) {
            ok = (aliasName == "Header");
        }
        if (ok) {
            brokerName = "MemoryMapInputBroker";
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with InputSignals");
        }
    }

    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns false irrespectively of the input parameters.*/
bool SDNPublisher::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName,
                                   void* const gamMemPtr) {
    bool ok = false;
    if (sdnHeaderAsSignal) {
        ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
        ok = broker.IsValid();

        if (ok) {
            ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
        }

        if (ok) {
            ok = inputBrokers.Insert(broker);
        }
    }
    return ok;
}

// The method is called for each GAM connected to the DataSource
bool SDNPublisher::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName,
                                    void* const gamMemPtr) {

    uint32 functionIdx = 0u;
    uint32 nOfFunctionSignals = 0u; // Number of signals associated to the function
    uint32 signalIndex;
    uint32 trigger = 0u;
    bool triggerGAM = false;
    bool ok = GetFunctionIndex(functionIdx, functionName);

    if (ok) {
        ok = GetFunctionNumberOfSignals(OutputSignals, functionIdx, nOfFunctionSignals);
    }

    // Test if there is a Trigger signal for this function.
    for (signalIndex = 0u; (signalIndex < nOfFunctionSignals) && (ok) && (!triggerGAM); signalIndex++) {
        ok = GetFunctionSignalTrigger(OutputSignals, functionIdx, signalIndex, trigger);

        if (ok) {
            triggerGAM = (trigger == 1u);
        }
    }

    // Test if there is a multi-sample signal for this function.
    for (signalIndex = 0u; (signalIndex < nOfFunctionSignals) && (ok); signalIndex++) {
// This version does not support multi-sample signals
        uint32 samples = 0u;
        ok = GetFunctionSignalSamples(OutputSignals, functionIdx, signalIndex, samples);

        if (ok) {
            ok = (samples == 1u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Multi-sample signals are not supported");
        }
    }

    if (ok) {
        if (triggerGAM) {

            /**
             * @warning The case where part of the GAM signals are non-synchronizing requires the
             * non-synchronizing broker to be inserted before the synchronizing one in order to make
             * sure all the signals are copied before the synchronisation is performed.
             */

            // A synchronizing broker is inserted in case one signal at least is declared with
            // Trigger property. The GAM may also encompass non-synchronizing signals which require
            // a standard broker.
            // Must add the signals which are not triggering but that belong to the same GAM...
            if (nOfFunctionSignals > 1u) {
                ReferenceT<MemoryMapOutputBroker> brokerNotSync("MemoryMapOutputBroker");
                ok = brokerNotSync.IsValid();

                if (ok) {
                    ok = brokerNotSync->Init(OutputSignals, *this, functionName, gamMemPtr);
                }

                if (ok) {
                    ok = outputBrokers.Insert(brokerNotSync);
                }
            }

            // Instantiate appropriate output broker
            ReferenceT<MemoryMapSynchronisedOutputBroker> broker("MemoryMapSynchronisedOutputBroker");

            if (ok) {
                ok = broker.IsValid();
            }

            // Associate all signals for this GAM which are compatible with broker (normally 1)
            if (ok) {
                ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
            }

            if (ok) {
                ok = outputBrokers.Insert(broker);
            }
        }
        else {
            // Instantiate appropriate output broker
            ReferenceT<MemoryMapOutputBroker> brokerNotSync("MemoryMapOutputBroker");
            ok = brokerNotSync.IsValid();

            if (ok) {
                ok = brokerNotSync->Init(OutputSignals, *this, functionName, gamMemPtr);
            }

            if (ok) {
                ok = outputBrokers.Insert(brokerNotSync);
            }
        }
    }

    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "Failed to insert OutputBroker");
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns true irrespectively of the input parameters.*/
bool SDNPublisher::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool SDNPublisher::Synchronise() {

    bool ok = (NULL_PTR(sdn::Publisher *)!=publisher);

    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "sdn::Publisher has not been initialised");
    }
    if (ok) {
        if (networkByteOrder) {
            // Convert payload to network byte order
            uint32 signalIndex = 0u;
            if (sdnHeaderAsSignal) {
                signalIndex = 1u;
            }
            //lint -e{613} payloadNumberOfElements, payloadAddresses and payloadNumberOfBits should not be NULL as otherwise Synchronise would not be called
            for (; (signalIndex < nOfSignals); signalIndex++) {
                if (payloadNumberOfBits[signalIndex] == 16u) {
                    uint32 elementIndex;
                    for (elementIndex = 0u; (elementIndex < payloadNumberOfElements[signalIndex]); elementIndex++) {
                        Endianity::ToBigEndian(reinterpret_cast<uint16 *>(payloadAddresses[signalIndex])[elementIndex]);
                    }
                }
                if (payloadNumberOfBits[signalIndex] == 32u) {
                    uint32 elementIndex;
                    for (elementIndex = 0u; (elementIndex < payloadNumberOfElements[signalIndex]); elementIndex++) {
                        Endianity::ToBigEndian(reinterpret_cast<uint32 *>(payloadAddresses[signalIndex])[elementIndex]);
                    }
                }
                if (payloadNumberOfBits[signalIndex] == 64u) {
                    uint32 elementIndex;
                    for (elementIndex = 0u; (elementIndex < payloadNumberOfElements[signalIndex]); elementIndex++) {
                        Endianity::ToBigEndian(reinterpret_cast<uint64 *>(payloadAddresses[signalIndex])[elementIndex]);
                    }
                }
            }
        }
    }
    if (ok) {
        /*lint -e{613} The reference can not be NULL in this portion of the code.*/
        ok = (publisher->Publish() == STATUS_SUCCESS);
    }

    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "Failed to publish");
    }

#ifdef FEATURE_10840
    // Perform housekeeping activities .. irrespective of status
    if (NULL_PTR(sdn::Publisher *) != publisher) {
        /*lint -e{613} The reference can not be NULL in this portion of the code.*/
	(void)publisher->DoBackgroundActivity();
    }
#endif

    return ok;
}
#ifdef FEATURE_10840
CLASS_REGISTER(SDNPublisher, "1.2")
// Or above
#else
CLASS_REGISTER(SDNPublisher, "1.0.12")
#endif
}
/* namespace MARTe */

