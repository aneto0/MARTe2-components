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
#include "LinuxTimer.h"
#include "MemoryMapSynchronisedInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * Execute in the context of the real-time thread.
 */
const uint32 LINUX_TIMER_EXEC_MODE_RTTHREAD = 1u;
/**
 * Execute in the context of a spawned thread.
 */
const uint32 LINUX_TIMER_EXEC_MODE_SPAWNED = 2u;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
LinuxTimer::LinuxTimer() :
        DataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this) {
    lastTimeTicks = 0u;
    sleepTimeTicks = 0u;
    timerPeriodUsecTime = 0u;
    synchronisingFunctionIdx = 0u;
    counterAndTimer[0] = 0u;
    counterAndTimer[1] = 0u;
    sleepNature = Busy;
    sleepPercentage = 0u;
    synchronising = false;
    executionMode = 0u;
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
    if (!data.Read("SleepNature", sleepNatureStr)) {
        REPORT_ERROR(ErrorManagement::Information, "SleepNature was not set. Using Default.");
        sleepNatureStr = "Default";
    }
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
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported SleepNature.");
        ok = false;
    }
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

    if (executionMode == LINUX_TIMER_EXEC_MODE_SPAWNED) {
        if (ok) {
            uint32 cpuMaskIn;
            if (!data.Read("CPUMask", cpuMaskIn)) {
                cpuMaskIn = 0xFFu;
                REPORT_ERROR(ErrorManagement::Warning, "CPUMask not specified using: %d", cpuMaskIn);
            }
            cpuMask = cpuMaskIn;
        }
        if (ok) {
            if (!data.Read("StackSize", stackSize)) {
                stackSize = THREADS_DEFAULT_STACKSIZE;
                REPORT_ERROR(ErrorManagement::Warning, "StackSize not specified using: %d", stackSize);
            }
        }
        if (ok) {
            ok = (stackSize > 0u);

            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "StackSize shall be > 0u");
            }
        }
        if (ok) {
            executor.SetCPUMask(cpuMask);
            executor.SetStackSize(stackSize);
        }
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
            REPORT_ERROR(ErrorManagement::ParametersError, "The second signal shall have 32 bits and %d were specified",
                         uint16(GetSignalType(1u).numberOfBits));
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
        REPORT_ERROR(ErrorManagement::Information, "The timer will be set using a frequency of %f Hz", frequency);

        float64 periodUsec = (1e6 / frequency);
        timerPeriodUsecTime = static_cast<uint32>(periodUsec);
        float64 sleepTimeT = (static_cast<float64>(HighResolutionTimer::Frequency()) / frequency);
        sleepTimeTicks = static_cast<uint64>(sleepTimeT);
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
bool LinuxTimer::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
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

const char8* LinuxTimer::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
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
bool LinuxTimer::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    bool ok = true;
    if (executionMode == LINUX_TIMER_EXEC_MODE_SPAWNED) {
        if (executor.GetStatus() == EmbeddedThreadI::OffState) {
            executor.SetName(GetName());
            ok = executor.Start();
        }
    }
    counterAndTimer[0] = 0u;
    counterAndTimer[1] = 0u;
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the method sleeps for the given period irrespectively of the input info.*/
ErrorManagement::ErrorType LinuxTimer::Execute(ExecutionInfo& info) {
    if (lastTimeTicks == 0u) {
        lastTimeTicks = HighResolutionTimer::Counter();
    }

    uint64 startTicks = HighResolutionTimer::Counter();
    //If we lose cycle, rephase to a multiple of the period.
    uint32 nCycles = 0u;
    while (lastTimeTicks < startTicks) {
        lastTimeTicks += sleepTimeTicks;
        nCycles++;
    }
    lastTimeTicks -= sleepTimeTicks;

    //Sleep until the next period. Cannot be < 0 due to while(lastTimeTicks < startTicks) above
    uint64 sleepTicksCorrection = (startTicks - lastTimeTicks);
    uint64 deltaTicks = sleepTimeTicks - sleepTicksCorrection;

    if (sleepNature == Busy) {
        if (sleepPercentage == 0u) {
            while ((HighResolutionTimer::Counter() - startTicks) < deltaTicks) {
            }
        }
        else {
            float32 totalSleepTime = static_cast<float32>(static_cast<float64>(deltaTicks) * HighResolutionTimer::Period());
            uint32 busyPercentage = (100u - sleepPercentage);
            float32 busyTime = totalSleepTime * (static_cast<float32>(busyPercentage) / 100.F);
            Sleep::SemiBusy(totalSleepTime, busyTime);
        }
    }
    else {
        float32 sleepTime = static_cast<float32>(static_cast<float64>(deltaTicks) * HighResolutionTimer::Period());
        Sleep::NoMore(sleepTime);
    }
    lastTimeTicks = HighResolutionTimer::Counter();

    ErrorManagement::ErrorType err;
    if (executionMode == LINUX_TIMER_EXEC_MODE_SPAWNED) {
        err = !(synchSem.Post());
    }
    counterAndTimer[0] += nCycles;
    counterAndTimer[1] = counterAndTimer[0] * timerPeriodUsecTime;

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

CLASS_REGISTER(LinuxTimer, "1.0")

}

