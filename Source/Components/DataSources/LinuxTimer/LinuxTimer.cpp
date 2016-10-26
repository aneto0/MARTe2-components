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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
LinuxTimer::LinuxTimer() :
        DataSourceI(),
        EmbeddedServiceMethodBinderI(),
        executor(*this) {
    lastTimeTicks = 0u;
    sleepTimeTicks = 0u;
    synchronisingFunctionIdx = 0u;
    counterAndTimer[0] = 0u;
    counterAndTimer[1] = 0u;
    sleepNature = Busy;
    if (!synchSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create EventSem.");
    }
}

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
    bool ok = (GetNumberOfSignals() == 2u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Exactly two signals shall be configured");
    }
    if (ok) {
        ok = (GetSignalType(0u).numberOfBits != 32u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The first signal shall have 32 bits");
        }
    }
    if (ok) {
        ok = (GetSignalType(1u).numberOfBits != 32u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The second signal shall have 32 bits");
        }
    }

    uint32 nOfFunctions = GetNumberOfFunctions();
    float32 frequency = -1.0F;
    bool found = false;
    uint32 functionIdx;
    for (functionIdx = 0u; (functionIdx < nOfFunctions) && (ok); functionIdx++) {
        uint32 nOfSignals = 0u;
        if (ok) {
            ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfSignals);
        }

        if (ok) {
            uint32 i;
            for (i = 0u; (i < nOfSignals) && (ok) && (!found); i++) {
                ok = GetFunctionSignalReadFrequency(InputSignals, functionIdx, i, frequency);
                found = (frequency > 0.F);
                if (found) {
                    synchronisingFunctionIdx = i;
                }
            }
        }

    }
    ok = found;
    if (ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "The timer will be set using a frequency of %f", frequency)
        uint32 timerPeriodUsecTime = 1e6 / frequency;
        sleepTimeTicks = timerPeriodUsecTime * HighResolutionTimer::Frequency();
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "No frequency was set (i.e. no signal synchronises on this LinuxTimer).");
    }
    return ok;
}

uint32 LinuxTimer::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool LinuxTimer::GetSignalMemoryBuffer(const uint32 signalIdx,
                                       const uint32 bufferIdx,
                                       void*& signalAddress) {
    bool ok = true;
    if (signalIdx == 0u) {
        signalAddress = &counterAndTimer[0];
    }
    if (signalIdx == 1u) {
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
    float32 frequency = 0;
    if (data.Read("Frequency", frequency)) {
        brokerName = "MemoryMapSynchronisedInputBroker";
    }
    else {
        brokerName = "MemoryMapInputBroker";
    }
    return brokerName;
}

bool LinuxTimer::GetInputBrokers(ReferenceContainer& inputBrokers,
                                 const char8* const functionName,
                                 void* const gamMemPtr) {
    //Check if this function has a synchronisation point (i.e. a signal which has Frequency > 0)
    uint32 functionIdx = 0u;
    bool ok = GetFunctionIndex(functionIdx, functionName);

    ReferenceT<MemoryMapBroker> broker;
    if (ok) {
        if (synchronisingFunctionIdx == functionIdx) {
            ReferenceT<MemoryMapSynchronisedInputBroker> brokerSync("MemoryMapSynchronisedInputBroker");
            broker = brokerSync;
        }
        else {
            ReferenceT<MemoryMapInputBroker> brokerNotSync("MemoryMapInputBroker");
            broker = brokerNotSync;
        }
    }
    if (ok) {
        ok = broker.IsValid();
    }
    if (ok) {
        ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ok) {
        ok = inputBrokers.Insert(broker);
    }

    return ok;
}

bool LinuxTimer::GetOutputBrokers(ReferenceContainer& outputBrokers,
                                  const char8* const functionName,
                                  void* const gamMemPtr) {
    return false;
}

bool LinuxTimer::Synchronise() {
    ErrorManagement::ErrorType err = synchSem.ResetWait(TTInfiniteWait);
    return err.ErrorsCleared();
}

bool LinuxTimer::PrepareNextState(const char8* const currentStateName,
                                  const char8* const nextStateName) {
    bool ok = true;
    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
        ok = executor.Start();
    }
    counterAndTimer[0] = 0u;
    counterAndTimer[1] = 1u;
    return ok;
}

ErrorManagement::ErrorType LinuxTimer::Execute(const ExecutionInfo& info) {
    uint64 sleepTicksCorrection = (HighResolutionTimer::Counter() - lastTimeTicks);
    float64 sleepTime = (sleepTimeTicks - sleepTicksCorrection) * HighResolutionTimer::Period();

    if (sleepNature == Busy) {
        Sleep::Busy(sleepTime);
    }
    else {
        Sleep::NoMore(sleepTime);
    }
    lastTimeTicks = HighResolutionTimer::Counter();

    ErrorManagement::ErrorType err = synchSem.Post();

    return err;
}

CLASS_REGISTER(LinuxTimer, "1.0")

}

