/**
 * @file LinuxTimer.cpp
 * @brief Source file for class LinuxTimer
 * @date 25/10/2016
 * @author Andre Neto
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
#include "HighResolutionTimeProvider.h"
#include "LinuxTimer.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "ReferenceContainerFilterReferences.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
    /**
     * @brief Execute in the context of the real-time thread.
     */
    const uint32 LINUX_TIMER_EXEC_MODE_RTTHREAD = 1u;

    /**
     * @brief Execute in the context of a spawned thread.
     */
    const uint32 LINUX_TIMER_EXEC_MODE_SPAWNED = 2u;

    /**
    * @brief Maximum phase of the signal (default)
    */
    const uint32 MAX_PHASE = 1000000u;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
LinuxTimer::LinuxTimer() :
    DataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this) {
    startTimeTicks = 0u;
    sleepTimeTicks[0] = 0u;
    sleepTimeTicks[1] = 0u;
    timerPeriodUsecTime[0] = 0u;
    timerPeriodUsecTime[1] = 0u;
    synchronisingFunctionIdx = 0u;
    counterAndTimer[0] = 0u;
    counterAndTimer[1] = 0u;
    sleepNature = Busy;
    sleepPercentage = 0u;
    synchronising = false;
    executionMode = 0u;
    absoluteTime = 0ull;
    deltaTime = 0ull;
    absoluteTime_1 = 0ull;
    ticksPerUs = 0.;
    phase = MAX_PHASE;
    phaseBackup = phase;
    trigRephase = 0u;
    trigRephase_1 = 0u;

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
    bool ok = DataSourceI::Initialise(data);
    StreamString sleepNatureStr;
    ConfigurationDatabase slaveCDB;

    if(ok) {
        if (!data.Read("SleepNature", sleepNatureStr)) {
            REPORT_ERROR(ErrorManagement::Information, "SleepNature was not set. Using Default.");
            sleepNatureStr = "Default";
            //ok = slaveCDB.Write("SleepNature", "Default");
        }
        else {
            ok = slaveCDB.Write("SleepNature", sleepNatureStr.Buffer());
        }

        if (!data.Read("Phase", phase)) {
            phase = MAX_PHASE;
            REPORT_ERROR(ErrorManagement::Information, "Phase was not configured, using default MAX value %d", phase);
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Phase manually configured to %d", phase);
        }
        
        if (ok) {
            phaseBackup = phase;

            if (sleepNatureStr == "Default") {
                sleepNature = Default;
            }
            else if (sleepNatureStr == "Busy") {
                sleepNature = Busy;
                if (!data.Read("SleepPercentage", sleepPercentage)) {
                    sleepPercentage = 0u;
                    REPORT_ERROR(ErrorManagement::Information, "SleepPercentage was not set. Using Default %d.", sleepPercentage);
                }
                if (sleepPercentage > 100u) {
                    sleepPercentage = 100u;
                }
                ok = slaveCDB.Write("SleepPercentage", sleepPercentage);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported SleepNature.");
                ok = false;
            }    
        }
        
        if(ok) {
            StreamString executionModeStr;

            if (!data.Read("ExecutionMode", executionModeStr)) {
                executionModeStr = "IndependentThread";
                REPORT_ERROR(ErrorManagement::Warning, "ExecutionMode not specified using: %s", executionModeStr.Buffer());
            }

            if (executionModeStr == "IndependentThread") {
                executionMode = LINUX_TIMER_EXEC_MODE_SPAWNED;
            }
            else if (executionModeStr == "RealTimeThread") {
                executionMode = LINUX_TIMER_EXEC_MODE_RTTHREAD;
            }
            else {
                ok = false;
                REPORT_ERROR(ErrorManagement::InitialisationError, "The Execution mode must be \"IndependentThread\" or \"RealTimeThread\"");
            }
        }

        if(ok) {
            if (executionMode == LINUX_TIMER_EXEC_MODE_SPAWNED) {
                // TODO (WARNING) Change type to support more than 32 cpus
                uint32 cpuMaskIn;
                if (!data.Read("CPUMask", cpuMaskIn)) {
                    cpuMaskIn = 0xFFu;
                    REPORT_ERROR(ErrorManagement::Warning, "CPUMask not specified using: %d", cpuMaskIn);
                }
                cpuMask = ProcessorType(cpuMaskIn);

                if (!data.Read("StackSize", stackSize)) {
                    stackSize = THREADS_DEFAULT_STACKSIZE;
                    REPORT_ERROR(ErrorManagement::Warning, "StackSize not specified using: %d", stackSize);
                }


                ok = (stackSize > 0u);

                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "StackSize shall be > 0u");
                }

                if (ok) {
                    executor.SetCPUMask(cpuMask);
                    executor.SetStackSize(stackSize);
                }
            }
        }

        if (ok) {
            ok = (Size() < 2u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Number of pluggable time providers can be 0 (Default) or 1 (Customized and specified)");
                REPORT_ERROR(ErrorManagement::ParametersError, "%d providers where specified instead", Size());
            }
        }

        if(ok) {
            uint8 tcnPollMode = 0u;
            bool isLegacyCfg = data.Read("TcnPoll", tcnPollMode);
            if(isLegacyCfg) {
                REPORT_ERROR(ErrorManagement::Information, "TcnPoll legacy configuration parameter found, its value is %d", tcnPollMode);
                REPORT_ERROR(ErrorManagement::Information, "TcnPoll value will be injected onto the TimeProvider");
                ok = slaveCDB.Write("TcnPoll", tcnPollMode);
                uint64 tempTCNFrequency = 0u;
                if(data.Read("TcnFrequency", tempTCNFrequency)) {
                    REPORT_ERROR(ErrorManagement::Information, "TcnFrequency legacy configuration parameter found, its value is %d", tempTCNFrequency);
                    REPORT_ERROR(ErrorManagement::Information, "TcnFrequency value will be injected onto the TimeProvider");
                    ok = slaveCDB.Write("TcnFrequency", tempTCNFrequency);
                }
            }
        }

        bool skipBackwardCompatibilityInjection = false;

        if (ok) {
            if(Size() == 0u) {
                REPORT_ERROR(ErrorManagement::Information, "No timer provider specified. Falling back to HighResolutionTimeProvider");
                timeProvider = ReferenceT<HighResolutionTimeProvider> (GlobalObjectsDatabase::Instance()->GetStandardHeap());
                timeProvider->SetName("DefaultHighResolutionTimeProvider");     
                ok = timeProvider->Initialise(slaveCDB);
                skipBackwardCompatibilityInjection = true;
                if(!ok) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failure to call the time provider initialise function");
                }
            }
            else {
                timeProvider = Get(0u);
                ok = timeProvider.IsValid();
                if (!ok) {
                     REPORT_ERROR(ErrorManagement::ParametersError, "Invalid timer provider was specified");
                }
            }
        }

        //Here we inject parameters into the Time Provider instance, only when the instance is automatically created from the cfg file
        //A check is done in order to avoid double injection/init
        if(ok) {
            bool hasChildren = slaveCDB.GetNumberOfChildren() != 0u;
            if(hasChildren && (!skipBackwardCompatibilityInjection)) {
                REPORT_ERROR(ErrorManagement::Information, "Backward compatibility parameters injection preconditions met, injecting %d parameters", slaveCDB.GetNumberOfChildren());
                ok = timeProvider->BackwardCompatibilityInit(slaveCDB);
                if(!ok) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Error while injecting backward compatibility parameters to the plugin");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::Information, "Backward compatibility parameters injection unnecessary");
            }
        }

        if(ok) {
            ticksPerUs = (static_cast<float64>(timeProvider->Frequency()) / 1.0e6);
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Failure in inner initialize function");
    }
   
    return ok;
}

bool LinuxTimer::SetConfiguredDatabase(StructuredDataI& data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    uint32 tempNumOfSignals = GetNumberOfSignals();

    if (ok) {
        ok = (tempNumOfSignals >= 2u) && (tempNumOfSignals <= 5u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Number of signal must be between 2 and 5");
    }
    if (ok) {
        ok = (GetSignalType(0u).numberOfBits == 32u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The first signal shall have 32 bits and %d were specified", uint16(GetSignalType(0u).numberOfBits));
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
            REPORT_ERROR(ErrorManagement::ParametersError, "The second signal shall have 32 bits and %d were specified", uint16(GetSignalType(1u).numberOfBits));
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
    
    if(tempNumOfSignals > 2u) {
        uint16 tempNumOfBits = GetSignalType(2u).numberOfBits;
        
        ok = ((GetSignalType(2u).type == UnsignedInteger) && (tempNumOfBits == 64u));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The third signal must be a 64 bit unsigned integer");
        }
    }

    if(tempNumOfSignals > 3u) {
        uint16 tempNumOfBits = GetSignalType(3u).numberOfBits;
        ok = ((GetSignalType(3u).type == UnsignedInteger) && (tempNumOfBits == 64u));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The fourth signal must be a 64 bit unsigned integer");
        }
    }
    
    if(tempNumOfSignals > 4u) {
        uint16 tempNumOfBits = GetSignalType(4u).numberOfBits;
        ok = ((GetSignalType(4u).type == UnsignedInteger) && (tempNumOfBits == 8u));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The fourth signal must be a 8 bit unsigned integer");
        }
    }

    if (ok) {
        ReferenceContainer result;
        ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH, this);
        ObjectRegistryDatabase::Instance()->ReferenceContainer::Find(result, filter);
        rtApp = result.Get(0u);
        ok = rtApp.IsValid();
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
    else if (signalIdx == 2u) {
        signalAddress = &absoluteTime;
    }
    else if (signalIdx == 3u) {
        signalAddress = &deltaTime;
    }
    else if (signalIdx == 4u) {
        signalAddress = &trigRephase;
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
        float32 tempFrequency = 0.F;
        if (!data.Read("Frequency", tempFrequency)) {
            tempFrequency = -1.F;
        }

        if (tempFrequency > 0.F) {
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

bool LinuxTimer::Synchronise() {
    ErrorManagement::ErrorType err;
    if (executionMode == LINUX_TIMER_EXEC_MODE_SPAWNED) {
        err = synchSem.ResetWait(TTInfiniteWait);
    }
    else {
        ExecutionInfo info;
        info.SetStage(ExecutionInfo::MainStage);
        err = Execute(info);
    }
    return err.ErrorsCleared();
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the counter and the timer are always reset irrespectively of the states being changed.*/
bool LinuxTimer::PrepareNextState(const char8* const currentStateName,
                                  const char8* const nextStateName) {
    bool ok = true;
    uint32 numberOfTotalConsumers = 0u;

    float32 tempFrequency = -1.0F;

    uint32 nextIndex = rtApp->GetIndex();
    nextIndex++;
    nextIndex &= 0x1u;

    bool notConsumed = true;
    for (uint32 idx = 0u; (idx < numberOfSignals) && ok; idx++) {
    uint32 i = idx;
        uint32 numberOfProducersNextState;
        if (!GetSignalNumberOfProducers(i, nextStateName, numberOfProducersNextState)) {
            numberOfProducersNextState = 0u;
        }

        ok = (numberOfProducersNextState == 0u);

        if (ok) {
            uint32 numberOfConsumers;
            if (!GetSignalNumberOfConsumers(i, currentStateName, numberOfConsumers)) {
                numberOfConsumers = 0u;
            }
            //reset if not used in current state
            if (numberOfConsumers > 0u) {
                notConsumed = false;
            }

            if (!GetSignalNumberOfConsumers(i, nextStateName, numberOfConsumers)) {
                numberOfConsumers = 0u;
            }
            numberOfTotalConsumers += numberOfConsumers;

            for (uint32 j = 0u; (j < numberOfConsumers) && ok; j++) {
                StreamString consumerName;
                ok = GetSignalConsumerName(i, nextStateName, j, consumerName);
                uint32 functionIdx = 0u;
                if (ok) {
                    ok = GetFunctionIndex(functionIdx, consumerName.Buffer());
                }
                if (ok) {
                    uint32 nOfFunSignals = 0u;
                    ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfFunSignals);
                    bool foundFreq = false;
                    for (uint32 k = 0u; (k < nOfFunSignals) && ok && (!foundFreq); k++) {
                        float32 freqRead = -1.0F;
                        ok = GetFunctionSignalReadFrequency(InputSignals, functionIdx, k, freqRead);
                        if (freqRead >= 0.F) {
                            tempFrequency = freqRead;
                            foundFreq = true;
                            REPORT_ERROR(ErrorManagement::Information, "Frequency found = %!", tempFrequency);
                        }
                    }
                }
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Signal %d cannot be produced!", i);
        }
    }

    if (ok) {
        if (notConsumed) {
            counterAndTimer[0] = 0u;
            counterAndTimer[1] = 0u;

            startTimeTicks = 0u;
            phase = phaseBackup;
        }
        if (numberOfTotalConsumers > 0u) {
            ok = (tempFrequency >= 0.F);

            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "The timer will be set using a frequency of %f Hz", tempFrequency);

                float64 periodUsec = (1.0e6 / tempFrequency);
                float64 sleepTimeT = (static_cast<float64> (timeProvider->Frequency()) / tempFrequency);
                timerPeriodUsecTime[nextIndex] = static_cast<uint32> (periodUsec);
                sleepTimeTicks[nextIndex] = static_cast<uint64> (sleepTimeT);
                
                //Act as we already slept, to avoid unnecessary idling on state changes
                //But only if the startTimeTicks was touched (only on first change)
//                if(startTimeTicks != 0u) {
//                    startTimeTicks = timeProvider->Counter() + sleepTimeTicks[nextIndex];
//                }
                
                if (executionMode == LINUX_TIMER_EXEC_MODE_SPAWNED) {
                    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
                        ok = executor.Start();
                    }
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "No frequency > 0 was set (i.e. no signal synchronises on this LinuxTimer).");
            }
        }
    }

    if (!ok) {
        if (!synchSem.Post()) {
            ok = false;
            REPORT_ERROR(ErrorManagement::FatalError, "Could not post EventSem.");
        }
        if (!executor.Stop()) {
            if (!executor.Stop()) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
                ok = false;
            }
        }
    }

    REPORT_ERROR(ok?ErrorManagement::Information:ErrorManagement::FatalError, "LinuxTimer::Prepared = %s", (ok?"true":"false"));

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the method sleeps for the given period irrespectively of the input info.*/
ErrorManagement::ErrorType LinuxTimer::Execute(ExecutionInfo& info) {

    uint32 appIndex = rtApp->GetIndex();
    appIndex %= 0x1u;

    uint64 sleepTimeTicksT;
    uint32 timerPeriodUsecTimeT;
    timerPeriodUsecTimeT = timerPeriodUsecTime[appIndex];
    sleepTimeTicksT = sleepTimeTicks[appIndex];

    if(trigRephase > trigRephase_1) {
        float64 seconds0 = static_cast<float64>(timeProvider->Counter()) * (timeProvider->Period());
        absoluteTime_1 = static_cast<uint64>(seconds0) * static_cast<uint64>(1e6);
        phase = static_cast<uint32>(absoluteTime_1 % MAX_PHASE);
        float64 secondsT = static_cast<float64>(absoluteTime_1) / 1e6;
        startTimeTicks = static_cast<uint64>(secondsT) * static_cast<uint64>(timeProvider->Frequency());
    }

    if(startTimeTicks == 0u) {
        startTimeTicks = timeProvider->Counter();
        float64 seconds0 = static_cast<float64>(startTimeTicks) * (timeProvider->Period());
        absoluteTime_1 = static_cast<uint64>(seconds0) * static_cast<uint64>(1e6);
        if(phase < MAX_PHASE) {
            //Try to synchronize on the second
            uint64 overSec = (absoluteTime_1 % MAX_PHASE);
            absoluteTime_1 += (phase - overSec);
            float64 secondsT = static_cast<float64> (absoluteTime_1) / 1e6;
            startTimeTicks = static_cast<uint64>(secondsT) * static_cast<uint64>(timeProvider->Frequency());
        }
    }

    uint64 cycleEndTicks = timeProvider->Counter();

    //If we lose cycle (i.e. if startTimeTicks < N * cycleEndTicks), rephase to a multiple of the period
    uint32 nCycles = 0u;

    while(startTimeTicks < cycleEndTicks) {
        startTimeTicks += sleepTimeTicksT;
        nCycles++;
    }

    startTimeTicks -= sleepTimeTicksT;
    
    //Sleep until the next period. Cannot be < 0 due to while(startTimeTicks < startTicks) above
    uint64 deltaTicks = sleepTimeTicksT + startTimeTicks;
    deltaTicks -= cycleEndTicks;

    bool sleepResult = timeProvider->Sleep(cycleEndTicks, deltaTicks);

    uint64 newCounter = timeProvider->Counter();
    startTimeTicks += sleepTimeTicksT;

    ErrorManagement::ErrorType err;
    if (!sleepResult) {
        err = ErrorManagement::FatalError;
    }
    else {
        counterAndTimer[0] += nCycles;
        counterAndTimer[1] = counterAndTimer[0] * timerPeriodUsecTimeT;
        float64 seconds = static_cast<float64> (newCounter) * (timeProvider->Period());
        absoluteTime = static_cast<uint64> (startTimeTicks) / static_cast<uint64>(ticksPerUs);
        uint64 microsecs = static_cast<uint64> (seconds) * static_cast<uint64>(1e6);
        deltaTime = (microsecs - absoluteTime_1);
        absoluteTime_1 = microsecs;
        trigRephase_1 = trigRephase;
    }

    if (executionMode == LINUX_TIMER_EXEC_MODE_SPAWNED) {
        err = !(synchSem.Post());
    }
    //REPORT_ERROR(ErrorManagement::Information, "time = %! %! delta = %!", absoluteTime, nCycles, deltaTime);
    return err;
}

const ProcessorType& LinuxTimer::GetCPUMask() const {
    return cpuMask;
}

uint32 LinuxTimer::GetStackSize() const {
    return stackSize;
}

uint32 LinuxTimer::GetSleepPercentage() const {
    return sleepPercentage;
}

void LinuxTimer::Purge(ReferenceContainer &purgeList) {
    if (rtApp.IsValid()) {
        rtApp->Purge(purgeList);
    }
    ReferenceContainer::Purge(purgeList);
}

CLASS_REGISTER(LinuxTimer, "1.0")

}

