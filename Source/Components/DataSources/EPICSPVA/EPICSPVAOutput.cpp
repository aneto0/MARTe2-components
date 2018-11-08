/**
 * @file EPICSPVAOutput.cpp
 * @brief Source file for class EPICSPVAOutput
 * @date 20/04/2017
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
 * the class EPICSPVAOutput (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryMapAsyncOutputBroker.h"
#include "EPICSPVAOutput.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
EPICSPVAOutput::EPICSPVAOutput() :
        DataSourceI() {
    stackSize = THREADS_DEFAULT_STACKSIZE * 4u;
    cpuMask = 0xffu;
    numberOfBuffers = 0u;
    numberOfChannels = 0u;
    ignoreBufferOverrun = 1u;
    channelList = NULL_PTR(EPICSPVAChannelWrapper *);
}

EPICSPVAOutput::~EPICSPVAOutput() {
    if (channelList != NULL_PTR(EPICSPVAChannelWrapper *)) {
        delete[] channelList;
    }
}

bool EPICSPVAOutput::Initialise(StructuredDataI & data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("NumberOfBuffers", numberOfBuffers);

        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfBuffers shall be specified");
        }
    }
    if (ok) {
        if (!data.Read("CPUs", cpuMask)) {
            REPORT_ERROR(ErrorManagement::Information, "No CPUs defined. Using default = %d", cpuMask);
        }
        if (!data.Read("StackSize", stackSize)) {
            REPORT_ERROR(ErrorManagement::Information, "No StackSize defined. Using default = %d", stackSize);
        }
        if (!data.Read("IgnoreBufferOverrun", ignoreBufferOverrun)) {
            REPORT_ERROR(ErrorManagement::Information, "No IgnoreBufferOverrun defined. Using default = %d", ignoreBufferOverrun);
        }

    }
    if (ok) {
        ok = data.MoveRelative("Signals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }
        if (ok) {
            ok = signalsDatabase.MoveAbsolute("Signals");
        }
        //Do not allow to add signals in run-time
        if (ok) {
            ok = signalsDatabase.Write("Locked", 1u);
            numberOfChannels = (signalsDatabase.GetNumberOfChildren() - 1u);
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
                ok = channelList[n].Setup(signalsDatabase);
            }
            if (ok) {
                ok = signalsDatabase.MoveToAncestor(1u);
            }
        }
    }
    if (ok) {
        ok = signalsDatabase.MoveToRoot();
    }
    if (ok) {
        ok = data.MoveToAncestor(1u);
    }
    return ok;
}

bool EPICSPVAOutput::SetConfiguredDatabase(StructuredDataI & data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
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
        if (GetFunctionNumberOfSignals(OutputSignals, 0u, functionNumberOfSignals)) {
            for (n = 0u; (n < functionNumberOfSignals) && (ok); n++) {
                uint32 nSamples;
                ok = GetFunctionSignalSamples(OutputSignals, 0u, n, nSamples);
                if (ok) {
                    ok = (nSamples == 1u);
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples shall be exactly 1");
                }
            }
        }
    }
//Only one and one GAM allowed to interact with this DataSourceI
    if (ok) {
        ok = (GetNumberOfFunctions() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly one Function allowed to interact with this DataSourceI");
        }
    }

    return ok;
}

bool EPICSPVAOutput::AllocateMemory() {
    return true;
}

uint32 EPICSPVAOutput::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool EPICSPVAOutput::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    StreamString fullQualifiedName;
    bool ok = (GetSignalName(signalIdx, fullQualifiedName));
    REPORT_ERROR(ErrorManagement::Information, "Searching for signal [%s]", fullQualifiedName.Buffer());

    if (ok) {
        ok = fullQualifiedName.Seek(0LLU);
    }
    StreamString unaliasedChannelName;
    if (ok) {
        char8 ignore;
        ok = fullQualifiedName.GetToken(unaliasedChannelName, ".", ignore);
    }
    bool found = false;
    uint32 n;
    for (n = 0u; (n < numberOfChannels) && (ok) && (!found); n++) {
        found = (unaliasedChannelName == channelList[n].GetChannelUnaliasedName());
        if (found) {
            const char8 *fullQualifiedNameBuffer = fullQualifiedName.Buffer();
            signalAddress = NULL_PTR(void *);
            channelList[n].GetSignalMemory(&fullQualifiedNameBuffer[unaliasedChannelName.Size() + 1u], signalAddress);
        }
    }
    if (ok) {
        ok = found;
    }
    if (ok) {
        ok = (signalAddress != NULL_PTR(void *));
    }
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "Signal [%s] was found in the declared structure", fullQualifiedName.Buffer());
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction */
const char8* EPICSPVAOutput::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8* brokerName = "";
    if (direction == OutputSignals) {
        brokerName = "MemoryMapAsyncOutputBroker";
    }
    return brokerName;
}

bool EPICSPVAOutput::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    ReferenceT<MemoryMapAsyncOutputBroker> broker("MemoryMapAsyncOutputBroker");
    bool ok = broker->InitWithBufferParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers, cpuMask, stackSize);
    if (ok) {
        ok = outputBrokers.Insert(broker);
        broker->SetIgnoreBufferOverrun(ignoreBufferOverrun == 1u);
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool EPICSPVAOutput::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

uint32 EPICSPVAOutput::GetStackSize() const {
    return stackSize;
}

uint32 EPICSPVAOutput::GetCPUMask() const {
    return cpuMask;
}

uint32 EPICSPVAOutput::GetNumberOfBuffers() const {
    return numberOfBuffers;
}

bool EPICSPVAOutput::Synchronise() {
    bool ok = true;
    uint32 n;
    for (n = 0u; (n < numberOfChannels) && (ok); n++) {
        ok = channelList[n].Put();
    }
    return ok;
}

bool EPICSPVAOutput::IsIgnoringBufferOverrun() const {
    return (ignoreBufferOverrun == 1u);
}

CLASS_REGISTER(EPICSPVAOutput, "1.0")

}

