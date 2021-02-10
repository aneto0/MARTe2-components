/**
 * @file EPICSCAInput.cpp
 * @brief Source file for class EPICSCAInput
 * @date 04/02/2021
 * @author Andre Neto
 * @author Pedro Lourenco
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
 * the class EPICSCAInput (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EPICSCAInput.h"
#include "MemoryMapInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Callback function for the ca_create_subscription. Single point of access which
 * delegates the events to the corresponding EPICSPV instance.
 */
static FastPollingMutexSem eventCallbackFastMux;
/*lint -e{1746} function must match required prototype and thus cannot be changed to constant reference.*/
void EPICSCAInputEventCallback(struct event_handler_args const args) {
    (void) eventCallbackFastMux.FastLock();
    PVWrapper *pv = static_cast<PVWrapper *>(args.usr);
    if (pv != NULL_PTR(PVWrapper *)) {
        (void) MemoryOperationsHelper::Copy(pv->memory, args.dbr, pv->memorySize);
    }
    eventCallbackFastMux.FastUnLock();
}
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
EPICSCAInput::EPICSCAInput() :
        DataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this) {
    pvs = NULL_PTR(PVWrapper *);
    stackSize = THREADS_DEFAULT_STACKSIZE * 4u;
    cpuMask = 0xffu;
    eventCallbackFastMux.Create();
}

/*lint -e{1551} must stop the SingleThreadService in the destructor.*/
EPICSCAInput::~EPICSCAInput() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    (void) eventCallbackFastMux.FastLock();
    uint32 nOfSignals = GetNumberOfSignals();
    if (pvs != NULL_PTR(PVWrapper *)) {
        uint32 n;
        for (n = 0u; (n < nOfSignals); n++) {
            if (pvs[n].memory != NULL_PTR(void *)) {
                GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(pvs[n].memory);
            }
        }
        delete[] pvs;
    }
    eventCallbackFastMux.FastUnLock();
}

bool EPICSCAInput::Initialise(StructuredDataI & data) {
    bool ok = DataSourceI::Initialise(data);
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
        if (ok) {
            ok = data.Copy(originalSignalInformation);
        }
        if (ok) {
            ok = originalSignalInformation.MoveToRoot();
        }
        //Do not allow to add signals in run-time
        if (ok) {
            ok = signalsDatabase.MoveRelative("Signals");
        }
        if (ok) {
            ok = signalsDatabase.Write("Locked", 1u);
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

bool EPICSCAInput::SetConfiguredDatabase(StructuredDataI & data) {
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
    if (ok) {
        pvs = new PVWrapper[nOfSignals];
        uint32 n;
        for (n = 0u; (n < nOfSignals); n++) {
            pvs[n].memory = NULL_PTR(void *);
        }
        for (n = 0u; (n < nOfSignals) && (ok); n++) {
            //Note that the RealTimeApplicationConfigurationBuilder is allowed to change the order of the signals w.r.t. to the originalSignalInformation
            StreamString orderedSignalName;
            ok = GetSignalName(n, orderedSignalName);
            if (ok) {
                //Have to mix and match between the original setting of the DataSource signal
                //and the ones which are later added by the RealTimeApplicationConfigurationBuilder
                ok = originalSignalInformation.MoveRelative(orderedSignalName.Buffer());
            }
            StreamString pvName;
            if (ok) {
                ok = originalSignalInformation.Read("PVName", pvName);
                if (!ok) {
                    uint32 nn = n;
                    REPORT_ERROR(ErrorManagement::ParametersError, "No PVName specified for signal at index %d", nn);
                }
            }
            TypeDescriptor td = GetSignalType(n);
            if (ok) {
                pvs[n].td = td;
                (void) StringHelper::CopyN(&pvs[n].pvName[0], pvName.Buffer(), PV_NAME_MAX_SIZE);
                if (td == CharString) {
                    pvs[n].pvType = DBR_STRING;
                }
                else if (td == Character8Bit) {
                    pvs[n].pvType = DBR_STRING;
                }
                else if (td == SignedInteger8Bit) {
                    pvs[n].pvType = DBR_CHAR;
                }
                else if (td == UnsignedInteger8Bit) {
                    pvs[n].pvType = DBR_CHAR;
                }
                else if (td == SignedInteger16Bit) {
                    pvs[n].pvType = DBR_SHORT;
                }
                else if (td == UnsignedInteger16Bit) {
                    pvs[n].pvType = DBR_SHORT;
                }
                else if (td == SignedInteger32Bit) {
                    pvs[n].pvType = DBR_LONG;
                }
                else if (td == UnsignedInteger32Bit) {
                    pvs[n].pvType = DBR_LONG;
                }
                else if (td == Float32Bit) {
                    pvs[n].pvType = DBR_FLOAT;
                }
                else if (td == Float64Bit) {
                    pvs[n].pvType = DBR_DOUBLE;
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Type %s is not supported", TypeDescriptor::GetTypeNameFromTypeDescriptor(td));
                    ok = false;
                }
            }
            uint32 numberOfElements = 1u;
            if (ok) {
                ok = GetSignalNumberOfElements(n, numberOfElements);
            }
            if (ok) {
                if (pvs[n].pvType == DBR_STRING) {
                    ok = (numberOfElements == 40u);
                }
                if (!ok) {
                    //Could support arrays of strings with multiples of char8[40]
                    REPORT_ERROR(ErrorManagement::ParametersError, "Strings shall be defined with 40 elements char8[40]. Arrays of strings are not currently supported");
                }
            }
            if (ok) {
                pvs[n].numberOfElements = numberOfElements;
            }
            if (ok) {
                pvs[n].memorySize = td.numberOfBits;
                pvs[n].memorySize /= 8u;
                pvs[n].memorySize *= numberOfElements;
                pvs[n].memory = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(pvs[n].memorySize);
                ok = originalSignalInformation.MoveToAncestor(1u);
            }
        }
    }

    if (ok) {
        executor.SetName(GetName());
        ok = (executor.Start() == ErrorManagement::NoError);
    }
    return ok;
}

bool EPICSCAInput::AllocateMemory() {
    return true;
}

uint32 EPICSCAInput::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool EPICSCAInput::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (pvs != NULL_PTR(PVWrapper *));
    if (ok) {
        ok = (signalIdx < GetNumberOfSignals());
    }
    if (ok) {
        //lint -e{613} pvs cannot as otherwise ok would be false
        signalAddress = pvs[signalIdx].memory;
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction */
const char8* EPICSCAInput::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8* brokerName = "";
    if (direction == InputSignals) {
        brokerName = "MemoryMapInputBroker";
    }
    return brokerName;
}

bool EPICSCAInput::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
    bool ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    if (ok) {
        ok = inputBrokers.Insert(broker);
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: OutputBrokers are not supported. Function returns false irrespectively of the parameters.*/
bool EPICSCAInput::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool EPICSCAInput::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

ErrorManagement::ErrorType EPICSCAInput::Execute(ExecutionInfo& info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() == ExecutionInfo::StartupStage) {
        (void) eventCallbackFastMux.FastLock();
        /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
        if (ca_context_create(ca_enable_preemptive_callback) != ECA_NORMAL) {
            err = ErrorManagement::FatalError;
            REPORT_ERROR(err, "ca_enable_preemptive_callback failed");
        }

        uint32 n;
        uint32 nOfSignals = GetNumberOfSignals();
        if (pvs != NULL_PTR(PVWrapper *)) {
            for (n = 0u; (n < nOfSignals); n++) {
                /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
                if (ca_create_channel(&pvs[n].pvName[0], NULL_PTR(caCh *), NULL_PTR(void *), 20u, &pvs[n].pvChid) != ECA_NORMAL) {
                    err = ErrorManagement::FatalError;
                    REPORT_ERROR(err, "ca_create_channel failed for PV with name %s", pvs[n].pvName);
                }
                if (err.ErrorsCleared()) {
                    /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
                    if (ca_create_subscription(pvs[n].pvType, pvs[n].numberOfElements, pvs[n].pvChid, DBE_VALUE, &EPICSCAInputEventCallback, &pvs[n],
                                               &pvs[n].pvEvid) != ECA_NORMAL) {
                        err = ErrorManagement::FatalError;
                        REPORT_ERROR(err, "ca_create_subscription failed for PV %s", pvs[n].pvName);
                    }
                }
            }
        }
        eventCallbackFastMux.FastUnLock();
    }
    else if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        Sleep::Sec(1.0F);
    }
    else {
        (void) eventCallbackFastMux.FastLock();
        uint32 n;
        uint32 nOfSignals = GetNumberOfSignals();
        if (pvs != NULL_PTR(PVWrapper *)) {
            for (n = 0u; (n < nOfSignals); n++) {
                (void) ca_clear_subscription(pvs[n].pvEvid);
                (void) ca_clear_event(pvs[n].pvEvid);
                (void) ca_clear_channel(pvs[n].pvChid);
            }
        }
        ca_detach_context();
        ca_context_destroy();
        eventCallbackFastMux.FastUnLock();
    }

    return err;
}

uint32 EPICSCAInput::GetStackSize() const {
    return stackSize;
}

uint32 EPICSCAInput::GetCPUMask() const {
    return cpuMask;
}

bool EPICSCAInput::Synchronise() {
    return false;
}

CLASS_REGISTER(EPICSCAInput, "1.0")

}

