/**
 * @file EPICSPVAInput.cpp
 * @brief Source file for class EPICSPVAInput
 * @date 11/06/2018
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
 * the class EPICSPVAInput (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EPICSPVAInput.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
EPICSPVAInput::EPICSPVAInput() :
        MemoryDataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this) {
    numberOfChannels = 0u;
    channelList = NULL_PTR(EPICSPVAChannelWrapper *);
    stackSize = THREADS_DEFAULT_STACKSIZE * 4u;
    cpuMask = 0xffu;
}

/*lint -e{1551} must stop the SingleThreadService in the destructor.*/
EPICSPVAInput::~EPICSPVAInput() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    if (channelList != NULL_PTR(EPICSPVAChannelWrapper *)) {
        delete[] channelList;
    }
}

bool EPICSPVAInput::Initialise(StructuredDataI & data) {
    bool ok = MemoryDataSourceI::Initialise(data);
    if (ok) {
        if (!data.Read("CPUs", cpuMask)) {
            REPORT_ERROR(ErrorManagement::Information, "No CPUs defined. Using default = %d", cpuMask);
        }
        if (!data.Read("StackSize", stackSize)) {
            REPORT_ERROR(ErrorManagement::Information, "No StackSize defined. Using default = %d", stackSize);
        }
        executor.SetStackSize(stackSize);
        executor.SetCPUMask(cpuMask);
    }
    if (ok) {
        ok = data.MoveRelative("Signals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }
        //Do not allow to add signals in run-time
        if (ok) {
            ok = signalsDatabase.MoveRelative("Signals");
        }
        if (ok) {
            ok = signalsDatabase.Write("Locked", 1u);
            numberOfChannels = (signalsDatabase.GetNumberOfChildren() - 1u);
            executor.SetNumberOfPoolThreads(numberOfChannels);
            REPORT_ERROR(ErrorManagement::Information, "Found %d channels", numberOfChannels);
        }
        //Create the channel wrapper list.
        if (ok) {
            if (numberOfChannels > 0u) {
                channelList = new EPICSPVAChannelWrapper[numberOfChannels];
            }
        }
        uint32 n;
        for (n = 0u; (n < numberOfChannels) && (ok); n++) {
            ok = signalsDatabase.MoveToChild(n);
            if (ok) {
                ok = channelList[n].SetAliasAndField(signalsDatabase);
            }
            if (ok) {
                ok = signalsDatabase.MoveToAncestor(1u);
            }
        }
        if (ok) {
            ok = signalsDatabase.MoveToAncestor(1u);
        }
    }
    if (ok) {
        ok = data.MoveToAncestor(1u);
    }
    return ok;
}

bool EPICSPVAInput::SetConfiguredDatabase(StructuredDataI & data) {
    bool ok = MemoryDataSourceI::SetConfiguredDatabase(data);
    //Check the signal index of the timing signal.
    uint32 nOfSignals = GetNumberOfSignals();
    if (ok) {
        ok = (nOfSignals > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "At least one signal shall be defined");
        }
    }
    if (ok) {
        //Do not allow samples
        uint32 functionNumberOfSignals = 0u;
        uint32 n;
        if (GetFunctionNumberOfSignals(InputSignals, 0u, functionNumberOfSignals)) {
            for (n = 0u; (n < functionNumberOfSignals) && (ok); n++) {
                uint32 nSamples;
                ok = GetFunctionSignalSamples(InputSignals, 0u, n, nSamples);
                if (ok) {
                    ok = (nSamples == 1u);
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples shall be exactly 1");
                }
            }
        }
    }
    return ok;
}

bool EPICSPVAInput::AllocateMemory() {
    bool ok = MemoryDataSourceI::AllocateMemory();
    uint32 n;
    for (n = 0u; (n < numberOfChannels) && (ok); n++) {
        ok = channelList[n].Setup(*this);
    }
    if (ok) {
        executor.SetName(GetName());
        ok = (executor.Start() == ErrorManagement::NoError);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction */
const char8* EPICSPVAInput::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8* brokerName = "";
    if (direction == InputSignals) {
        brokerName = "MemoryMapInputBroker";
    }
    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool EPICSPVAInput::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

ErrorManagement::ErrorType EPICSPVAInput::Execute(ExecutionInfo& info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        err.fatalError = !channelList[info.GetThreadNumber()].Monitor();
    }
    return err;
}

uint32 EPICSPVAInput::GetStackSize() const {
    return stackSize;
}

uint32 EPICSPVAInput::GetCPUMask() const {
    return cpuMask;
}

bool EPICSPVAInput::Synchronise() {
    return false;
}

CLASS_REGISTER(EPICSPVAInput, "1.0")

}

