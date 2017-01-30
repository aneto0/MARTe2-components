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
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "SDNPublisher.h"

#include "sdn-api.h" /* SDN core library - API definition (sdn::core) */

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

    topic = NULL_PTR(sdn::Topic *);
    publisher = NULL_PTR(sdn::Publisher *);

}

SDNPublisher::~SDNPublisher() {

    if (publisher != NULL_PTR(sdn::Publisher *)) {
        delete publisher; publisher = NULL_PTR(sdn::Publisher *);
    }

    if (topic != NULL_PTR(sdn::Topic *)) {
        delete topic; topic = NULL_PTR(sdn::Topic *);
    }

}

bool SDNPublisher::Initialise(StructuredDataI &data) {

    bool ok = DataSourceI::Initialise(data);

    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "Parent virtual method failed");
    }

    // Retrieve and verify network interface name
    if (!data.Read("Interface", ifaceName)) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Interface must be specified");
        ok = false;
    } else {
        log_info("SDNPublisher::Initialise - Interface is '%s'", ifaceName.Buffer());
    } 

    if (!net_is_interface_valid(ifaceName.Buffer())) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Interface must be a valid identifier");
        ok = false;
    } else {
        log_info("SDNPublisher::Initialise - Interface '%s' is valid", ifaceName.Buffer());
    } 

    // Retrieve and verify topic name
    if (!data.Read("Topic", topicName)) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Topic must be specified");
        ok = false;
    } else {
        log_info("SDNPublisher::Initialise - Topic name is '%s'", topicName.Buffer());
    }

    // The topic name is used to generate UDP/IPv4 multicast mapping. Optionally, the mapping
    // to a destination '<address>:<port>' can be explicitly defined
    if (data.Read("Address", destAddr)) {

        if (!sdn_is_address_valid(destAddr.Buffer())) {
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

    bool ok = DataSourceI::SetConfiguredDatabase(data);

    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "Parent virtual method failed");
    }

    nOfSignals = GetNumberOfSignals();

    ok = (nOfSignals > 0u);

    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "nOfSignals must be > 0u");
    } else {
        log_info("SDNPublisher::SetConfiguredDatabase - Number of signals '%u'", nOfSignals);
    }

    if (ok) {
        ok = (nOfTriggers == 1u);
    }

    if (!ok) {
        if (nOfTriggers == 0u) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Missing trigger signal");
        } else {
            REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with multiple synchronising signals");
        }
    }

    return ok;
}

bool SDNPublisher::AllocateMemory() {

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
            REPORT_ERROR(ErrorManagement::Warning, "Unable to query signal name");
            // This is actually not so important ... could synthesize signal name in this case ... if I knew how
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

    return ok;
}

uint32 SDNPublisher::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool SDNPublisher::GetSignalMemoryBuffer(const uint32 signalIdx,
                                         const uint32 bufferIdx,
                                         void*& signalAddress) {

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

    return ok;
}

// The method is called for each signal connected to the DataSource
const char8* SDNPublisher::GetBrokerName(StructuredDataI& data,
                                         const SignalDirection direction) {

    const char8 *brokerName = NULL_PTR(const char8 *);

    if (direction == OutputSignals) {

        uint32 trigger = 0u;

        if (!data.Read("Trigger", trigger)) {
            trigger = 0u;
        }

        if (trigger == 1u) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
            nOfTriggers++;
        } else {
            brokerName = "MemoryMapOutputBroker";
        }

    } else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with InputSignals");
    }

    return brokerName;
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

    uint32 functionIdx = 0u;
    uint32 nOfSignals = 0u; // Number of signals associated to the function
    uint32 signalIndex = 0u;
    uint32 trigger = 0u;
    bool triggerGAM = false;
    bool ok = GetFunctionIndex(functionIdx, functionName);

    if (ok) {
        ok = GetFunctionNumberOfSignals(OutputSignals, functionIdx, nOfSignals);
    }

    // Test if there is a Trigger signal for this function.
    for (signalIndex = 0u; (signalIndex < nOfSignals) && (ok) && (!triggerGAM); signalIndex++) {
        ok = GetFunctionSignalTrigger(OutputSignals, functionIdx, signalIndex, trigger);

        if (ok) {
            triggerGAM = (trigger == 1u);
        }

        // This version does not support multi-sample signals
        uint32 samples = 0u;
        ok = GetFunctionSignalSamples(OutputSignals,functionIdx, signalIndex, samples);

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
             * sure all the signals are copied before the synch. is performed.
             */

            // A synchronizing broker is inserted in case one signal at least is declared with
            // Trigger property. The GAM may also encompass non-synchronizing signals which require
            // a standard broker.

            // Must add the signals which are not triggering but that belong to the same GAM...
            if (nOfSignals > 1u) {
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
            ok = broker.IsValid();

            // Associate all signals for this GAM which are compatible with broker (normally 1)
            if (ok) {
                ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
            }

            if (ok) {
                ok = outputBrokers.Insert(broker);
            }
        } else {
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

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns true irrespectively of the input parameters.*/
bool SDNPublisher::PrepareNextState(const char8* const currentStateName,
                                    const char8* const nextStateName) {
    return true;
}

bool SDNPublisher::Synchronise() {

    bool ok = (publisher != NULL_PTR(sdn::Publisher *));

    if (!ok) {
        log_error("SDNPublisher::Synchronise - sdn::Publisher has not been initiaised");
        REPORT_ERROR(ErrorManagement::FatalError, "sdn::Publisher has not been initiaised");
    }

    if (ok) {
        ok = (publisher->Publish() == STATUS_SUCCESS);
    }

    return ok;
}

CLASS_REGISTER(SDNPublisher, "1.0.10")

} /* namespace MARTe */
