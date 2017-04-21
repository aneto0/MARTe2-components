/**
 * @file EPICSCADataSource.cpp
 * @brief Source file for class EPICSCADataSource
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
 * the class EPICSCADataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "EPICSCAOutput.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryMapAsyncOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
EPICSCAOutput::EPICSCAOutput() :
        DataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this) {
    pvs = NULL_PTR(PVWrapper *);
    stackSize = THREADS_DEFAULT_STACKSIZE * 4u;
    cpuMask = 0xffu;
    if (!evtSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create the EventSem");
    }
    (void) evtSem.Reset();
    fastMux.Create();
    evtSemPosted = false;
}

/*lint -e{1551} must stop the SingleThreadService in the destructor.*/
EPICSCAOutput::~EPICSCAOutput() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
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
    (void) evtSem.Close();
}

bool EPICSCAOutput::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
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
            //Do not allow to add signals in run-time
            ok = data.Write("Locked", 1);
        }
        if (ok) {
            ok = data.Copy(originalSignalInformation);
        }
        if (ok) {
            ok = originalSignalInformation.MoveToRoot();
        }
    }
    if (ok) {
        ok = data.MoveToAncestor(1u);
    }
    return ok;
}

bool EPICSCAOutput::SetConfiguredDatabase(StructuredDataI & data) {
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

    if (ok) {
        pvs = new PVWrapper[nOfSignals];
        uint32 n;
        for (n = 0u; (n < nOfSignals); n++) {
            pvs[n].memory = NULL_PTR(void *);
        }
        for (n = 0u; (n < nOfSignals) && (ok); n++) {
            //Have to mix and match between the original setting of the DataSource signal
            //and the ones which are later added by the RealTimeApplicationConfigurationBuilder
            ok = originalSignalInformation.MoveRelative(originalSignalInformation.GetChildName(n));
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
                (void) StringHelper::CopyN(&pvs[n].pvName[0], pvName.Buffer(), PV_NAME_MAX_SIZE);
                if (td == SignedInteger32Bit) {
                    pvs[n].pvType = DBR_INT;
                }
                else if (td == UnsignedInteger32Bit) {
                    pvs[n].pvType = DBR_INT;
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
            if (ok) {
                uint32 mallocSize = td.numberOfBits;
                mallocSize /= 8u;
                pvs[n].memory = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(mallocSize);
                ok = originalSignalInformation.MoveToAncestor(1u);
            }
        }
    }

    if (ok) {
        ok = (executor.Start() == ErrorManagement::NoError);
    }
    return ok;
}

bool EPICSCAOutput::AllocateMemory() {
    return true;
}

uint32 EPICSCAOutput::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool EPICSCAOutput::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (pvs != NULL_PTR(PVWrapper *));
    if (ok) {
        ok = (signalIdx < GetNumberOfSignals());
    }
    if (ok) {
        if (pvs != NULL_PTR(PVWrapper *)) {
            signalAddress = pvs[signalIdx].memory;
        }
        else {
            ok = false;
        }
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction */
const char8* EPICSCAOutput::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8* brokerName = "";
    if (direction == OutputSignals) {
        brokerName = "MemoryMapAsyncOutputBroker";
    }
    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: InputBrokers are not supported. Function returns false irrespectively of the parameters.*/
bool EPICSCAOutput::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

bool EPICSCAOutput::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    ReferenceT<MemoryMapAsyncOutputBroker> broker("MemoryMapAsyncOutputBroker");
    bool ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    if (ok) {
        ok = outputBrokers.Insert(broker);
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool EPICSCAOutput::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

ErrorManagement::ErrorType EPICSCAOutput::Execute(const ExecutionInfo& info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() == ExecutionInfo::StartupStage) {
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
            }
        }
    }
    else if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        uint32 n;
        uint32 nOfSignals = GetNumberOfSignals();
        if (fastMux.FastLock() == ErrorManagement::NoError) {
            evtSemPosted = false;
            fastMux.FastUnLock();
        }
        //Wait at most one second.
        if (evtSem.Wait(1000u) == ErrorManagement::NoError) {
            if (pvs != NULL_PTR(PVWrapper *)) {
                for (n = 0u; (n < nOfSignals); n++) {
                    /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
                    if (ca_put(pvs[n].pvType, pvs[n].pvChid, pvs[n].memory) != ECA_NORMAL) {
                        err = ErrorManagement::FatalError;
                        REPORT_ERROR(err, "ca_put failed for PV: %s", pvs[n].pvName);
                    }
                }
            }
        }
        //Try to reset the semaphore unless there was a new Post already performed.
        if (fastMux.FastLock() == ErrorManagement::NoError) {
            if (!evtSemPosted) {
                (void) evtSem.Reset();
            }
            fastMux.FastUnLock();
        }
    }
    else {
        uint32 n;
        uint32 nOfSignals = GetNumberOfSignals();
        if (pvs != NULL_PTR(PVWrapper *)) {
            for (n = 0u; (n < nOfSignals); n++) {
                (void) ca_clear_channel(pvs[n].pvChid);
            }
        }
        ca_detach_context();
        ca_context_destroy();
    }

    return err;
}

uint32 EPICSCAOutput::GetStackSize() const {
    return stackSize;
}

uint32 EPICSCAOutput::GetCPUMask() const {
    return cpuMask;
}

bool EPICSCAOutput::Synchronise() {
    bool ok = true;
    if (fastMux.FastLock() == ErrorManagement::NoError) {
        (void) evtSem.Post();
        evtSemPosted = true;
        fastMux.FastUnLock();
    }

    return ok;
}

CLASS_REGISTER(EPICSCAOutput, "1.0")

}

