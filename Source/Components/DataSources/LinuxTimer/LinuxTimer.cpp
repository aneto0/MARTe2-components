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
    counterAndTimer[0] = 0u;
    counterAndTimer[1] = 0u;
    sleepNature = Busy;
    if (!synchSem.Create()) {
        //TODO log
    }
}

LinuxTimer::~LinuxTimer() {
    if (!synchSem.Post()) {
        //TODO log
    }
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            //TODO log
        }
    }
}

bool LinuxTimer::AllocateMemory() {
    return true;
}

bool LinuxTimer::Initialise(StructuredDataI& data) {
    uint32 timerPeriodUsecTime = 0u;
    bool ok = data.Read("TimerPeriodUsec", timerPeriodUsecTime);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "TimerPeriodUsec shall be set.");
    }
    if (ok) {
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
    }
    if (ok) {
        sleepTimeTicks = timerPeriodUsecTime * HighResolutionTimer::Frequency();
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
    return "MemoryMapSynchronisedInputBroker";
}

bool LinuxTimer::GetInputBrokers(ReferenceContainer& inputBrokers,
                                 const char8* const functionName,
                                 void* const gamMemPtr) {
    ReferenceT<MemoryMapSynchronisedInputBroker> broker("MemoryMapSynchronisedInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
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
    counterAndTimer[0] = 0u;
    counterAndTimer[1] = 1u;
    return true;
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

