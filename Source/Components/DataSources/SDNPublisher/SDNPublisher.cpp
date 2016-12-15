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

    topicSize = 0u;

    topic = NULL_PTR(sdn::Topic *);
    publisher = NULL_PTR(sdn::Publisher *);

    log_trace("SDNPublisher::SDNPublisher - Leaving method");

}

SDNPublisher::~SDNPublisher() {

    log_trace("SDNPublisher::~SDNPublisher - Entering method");

    log_trace("SDNPublisher::~SDNPublisher - Leaving method");

}

bool SDNPublisher::SetConfiguredDatabase(StructuredDataI& data) {

    log_trace("SDNPublisher::SetConfiguredDatabase - Entering method");

    bool ok = DataSourceI::SetConfiguredDatabase(data);

    uint32 nOfSignals = GetNumberOfSignals();

    log_info("SDNPublisher::SetConfiguredDatabase - Number of signals '%u'", nOfSignals);

    log_trace("SDNPublisher::SetConfiguredDatabase - Leaving method");

    return ok;
}

bool SDNPublisher::AllocateMemory() {

    log_trace("SDNPublisher::AllocateMemory - Entering method");

    uint32 nOfSignals = GetNumberOfSignals();

    log_info("SDNPublisher::AllocateMemory - Number of signals '%u'", nOfSignals);

    bool ok = (nOfSignals > 0u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "nOfSignals must be > 0u");
    }
    uint32 totalSignalSize = 0u;
    if (ok) {
        uint32 n;
        for (n = 0u; (n < nOfSignals) && (ok); n++) {
            uint32 signalByteSize;
            ok = GetSignalByteSize(n, signalByteSize);
            if (ok) {
                totalSignalSize += signalByteSize;
            }
        }
    }
    if (ok) {
        //topicMemory = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalSignalSize);
        topicSize = totalSignalSize;

        // Instantiate a sdn::Metadata structure to configure the topic
        sdn::Metadata_t mdata; 

	if (destAddr.Size() == 0) { // Topic defined by name
	    sdn::Topic_InitializeMetadata(mdata, (const char*) topicName.Buffer(), 0);
	} else { // An address as been explicitly provided 
	    sdn::Topic_InitializeMetadata(mdata, (const char*) topicName.Buffer(), 0, (const char*) destAddr.Buffer());
	}

        // Instantiate SDN topic from metadata specification
        topic = new sdn::Topic; topic->SetMetadata(mdata);

        uint32 n;
        for (n = 0u; (n < nOfSignals) && (ok); n++) {

            StreamString signalName;
            if (ok) {
                ok = GetSignalName(n, signalName);
            }

            TypeDescriptor signalType;
            StreamString signalTypeName;
            if (ok) {
                signalType = GetSignalType(n);
                signalTypeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(signalType);
            }

            uint32 nOfElements;
            if (ok) {
                ok = GetSignalNumberOfElements(n, nOfElements);
            }

            topic->AddAttribute(n, signalName.Buffer(), signalTypeName.Buffer(), nOfElements);

        }

	ok = (topic->Configure() == STATUS_SUCCESS);

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

    bool ok = (signalIdx < GetNumberOfSignals());
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

bool SDNPublisher::Initialise(StructuredDataI &data) {

    log_trace("SDNPublisher::Initialise - Entering method");

    bool ok = DataSourceI::Initialise(data);

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

bool SDNPublisher::Synchronise() {

    log_trace("SDNPublisher::Synchronise - Entering method");

    bool ok = (publisher->Publish() == STATUS_SUCCESS);

    log_trace("SDNPublisher::Synchronise - Leaving method");

    return ok;
}

CLASS_REGISTER(SDNPublisher, "1.0.9")

#if 0


LinuxTimer::LinuxTimer() :
        DataSourceI(),
        EmbeddedServiceMethodBinderI(),
        executor(*this) {
    lastTimeTicks = 0u;
    sleepTimeTicks = 0u;
    timerPeriodUsecTime = 0u;
    synchronisingFunctionIdx = 0u;
    counterAndTimer[0] = 0u;
    counterAndTimer[1] = 0u;
    sleepNature = Busy;
    synchronising = false;
    if (!synchSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create EventSem.");
    }
}

/*lint -e{1551} the destructor must guarantee that the Timer SingleThreadService is stopped.*/
LinuxTimer::~LinuxTimer() {
    if (!synchSem.Post()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not post EventSem.");
    }
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
}

bool LinuxTimer::AllocateMemory() {
    return true;
}

bool LinuxTimer::Initialise(StructuredDataI& data) {
    bool ok = true;
    StreamString sleepNatureStr;
    if (!data.Read("SleepNature", sleepNatureStr)) {
        REPORT_ERROR(ErrorManagement::Information, "SleepNature was not set. Using Default.");
        sleepNatureStr = "Default";
    }
    if (sleepNatureStr == "Default") {
        sleepNature = Default;
    }
    else if (sleepNatureStr == "Busy") {
        sleepNature = Busy;
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported SleepNature.");
        ok = false;
    }

    return ok;
}

bool LinuxTimer::SetConfiguredDatabase(StructuredDataI& data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = (GetNumberOfSignals() == 2u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Exactly two signals shall be configured");
    }
    if (ok) {
        ok = (GetSignalType(0u).numberOfBits == 32u);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The first signal shall have 32 bits and %d were specified",
                                    uint16(GetSignalType(0u).numberOfBits))
        }
    }
    if (ok) {
        ok = (GetSignalType(0u).type == SignedInteger);
        if (!ok) {
            ok = (GetSignalType(0u).type == UnsignedInteger);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The first signal shall SignedInteger or UnsignedInteger type");
        }
    }
    if (ok) {
        ok = (GetSignalType(1u).numberOfBits == 32u);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The second signal shall have 32 bits and %d were specified",
                                    uint16(GetSignalType(1u).numberOfBits))
        }
    }
    if (ok) {
        ok = (GetSignalType(1u).type == SignedInteger);
        if (!ok) {
            ok = (GetSignalType(1u).type == UnsignedInteger);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The second signal shall SignedInteger or UnsignedInteger type");
        }
    }
    uint32 nOfFunctions = GetNumberOfFunctions();
    float32 frequency = -1.0F;
    bool found = false;
    uint32 functionIdx;
    for (functionIdx = 0u; (functionIdx < nOfFunctions) && (ok); functionIdx++) {
        uint32 nOfSignals = 0u;
        ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfSignals);

        if (ok) {
            uint32 i;
            for (i = 0u; (i < nOfSignals) && (ok) && (!found); i++) {
                ok = GetFunctionSignalReadFrequency(InputSignals, functionIdx, i, frequency);
                found = (frequency > 0.F);
                if (found) {
                    synchronisingFunctionIdx = functionIdx;
                }
            }
        }

    }
    ok = found;
    if (ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "The timer will be set using a frequency of %f Hz", frequency)
        float64 periodUsec = (1e6 / frequency);
        timerPeriodUsecTime = static_cast<uint32>(periodUsec);
        uint64 sleepTimeTicks64 = HighResolutionTimer::Frequency() / static_cast<uint64>(frequency);
        sleepTimeTicks = static_cast<uint32>(sleepTimeTicks64);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "No frequency > 0 was set (i.e. no signal synchronises on this LinuxTimer).");
    }
    return ok;
}

uint32 LinuxTimer::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool LinuxTimer::GetSignalMemoryBuffer(const uint32 signalIdx,
                                       const uint32 bufferIdx,
                                       void*& signalAddress) {
    bool ok = true;
    if (signalIdx == 0u) {
        signalAddress = &counterAndTimer[0];
    }
    else if (signalIdx == 1u) {
        signalAddress = &counterAndTimer[1];
    }
    else {
        ok = false;
    }
    return ok;
}

const char8* LinuxTimer::GetBrokerName(StructuredDataI& data,
                                       const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == InputSignals) {
        float32 frequency = 0.F;
        if (!data.Read("Frequency", frequency)) {
            frequency = -1.F;
        }

        if (frequency > 0.F) {
            brokerName = "MemoryMapSynchronisedInputBroker";
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

bool LinuxTimer::GetInputBrokers(ReferenceContainer& inputBrokers,
                                 const char8* const functionName,
                                 void* const gamMemPtr) {
    //Check if this function has a synchronisation point (i.e. a signal which has Frequency > 0)
    uint32 functionIdx = 0u;
    bool ok = GetFunctionIndex(functionIdx, functionName);

    if ((synchronising) && (synchronisingFunctionIdx == functionIdx)) {
        ReferenceT<MemoryMapSynchronisedInputBroker> brokerSync("MemoryMapSynchronisedInputBroker");
        if (ok) {
            ok = brokerSync.IsValid();
        }
        if (ok) {
            //This call will only add the Synchronous signal (given that GetBrokerName returned MemoryMapSynchronisedInputBroker at most for one signal)
            ok = brokerSync->Init(InputSignals, *this, functionName, gamMemPtr);
        }
        if (ok) {
            ok = inputBrokers.Insert(brokerSync);
        }
        uint32 nOfFunctionSignals = 0u;
        if (ok) {
            ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfFunctionSignals);
        }
        //Must also add the signals which are not synchronous but that belong to the same GAM...
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

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns false irrespectively of the input parameters.*/
bool LinuxTimer::GetOutputBrokers(ReferenceContainer& outputBrokers,
                                  const char8* const functionName,
                                  void* const gamMemPtr) {
    return false;
}

bool LinuxTimer::Synchronise() {
    ErrorManagement::ErrorType err = synchSem.ResetWait(TTInfiniteWait);
    return err.ErrorsCleared();
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the counter and the timer are always reset irrespectively of the states being changed.*/
bool LinuxTimer::PrepareNextState(const char8* const currentStateName,
                                  const char8* const nextStateName) {
    bool ok = true;
    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
        ok = executor.Start();
    }
    counterAndTimer[0] = 0u;
    counterAndTimer[1] = 0u;
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the method sleeps for the given period irrespectively of the input info.*/
ErrorManagement::ErrorType LinuxTimer::Execute(const ExecutionInfo& info) {
    if (lastTimeTicks == 0u) {
        lastTimeTicks = HighResolutionTimer::Counter();
    }

    float64 sleepTime = 0.F;
    uint64 sleepTicksCorrection = (HighResolutionTimer::Counter() - lastTimeTicks);
    if (sleepTicksCorrection < sleepTimeTicks) {
        uint64 deltaTicks = sleepTimeTicks - sleepTicksCorrection;
        sleepTime = static_cast<float64>(deltaTicks) * HighResolutionTimer::Period();
    }
    else {
        sleepTime = static_cast<float64>(sleepTimeTicks) * HighResolutionTimer::Period();
    }

    if (sleepNature == Busy) {
        Sleep::Busy(sleepTime);
    }
    else {
        Sleep::NoMore(sleepTime);
    }
    lastTimeTicks = HighResolutionTimer::Counter();

    ErrorManagement::ErrorType err(synchSem.Post());
    counterAndTimer[0]++;
    counterAndTimer[1] = counterAndTimer[0] * timerPeriodUsecTime;

    return err;
}

CLASS_REGISTER(LinuxTimer, "1.0")

#endif

}

