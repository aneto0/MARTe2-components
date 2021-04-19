/**
 * @file EPICSCAOutput.cpp
 * @brief Source file for class EPICSCAOutput
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
 * the class EPICSCAOutput (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "EPICSCAOutput.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
EPICSCAOutput::EPICSCAOutput() :
        DataSourceI(), MessageI() {
    pvs = NULL_PTR(PVWrapper *);
    stackSize = THREADS_DEFAULT_STACKSIZE * 4u;
    cpuMask = 0xffu;
    numberOfBuffers = 0u;
    ignoreBufferOverrun = 1u;
    threadContextSet = false;
    signalFlag = NULL_PTR(uint8*);
    ReferenceT < RegisteredMethodsMessageFilter > filter = ReferenceT < RegisteredMethodsMessageFilter > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

/*lint -e{1551} must free the memory allocated to the different PVs.*/
EPICSCAOutput::~EPICSCAOutput() {
    uint32 nOfSignals = GetNumberOfSignals();
    if (pvs != NULL_PTR(PVWrapper *)) {
        uint32 n;
        for (n = 0u; (n < nOfSignals); n++) {
            if (pvs[n].pvChid != NULL_PTR(chid)) {
                (void) ca_clear_channel(pvs[n].pvChid);
            }
            if (pvs[n].memory != NULL_PTR(void *)) {
                GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(pvs[n].memory);
            }
        }
        delete[] pvs;
    }
    if (signalFlag != NULL_PTR(uint8*)) {
        delete[] signalFlag;
    }
}

void EPICSCAOutput::Purge(ReferenceContainer &purgeList) {
    if (broker.IsValid()) {
        (void) broker->Flush();
        broker->UnlinkDataSource();
    }
    DataSourceI::Purge(purgeList);
}


bool EPICSCAOutput::Initialise(StructuredDataI & data) {
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
        signalFlag = new uint8[nOfSignals];
        uint32 n;
        for (n = 0u; (n < nOfSignals); n++) {
            pvs[n].memory = NULL_PTR(void *);
            pvs[n].pvChid = NULL_PTR(chid);
            signalFlag[n] = 0u;
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
                    REPORT_ERROR(ErrorManagement::ParametersError,
                                 "Strings shall be defined with 40 elements char8[40]. Arrays of strings are not currently supported");
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
        //lint -e{613} pvs cannot as otherwise ok would be false
        signalAddress = pvs[signalIdx].memory;
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
    broker = ReferenceT<MemoryMapAsyncOutputBroker>("MemoryMapAsyncOutputBroker");
    bool ok = broker->InitWithBufferParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers, cpuMask, stackSize);
    if (ok) {
        ok = outputBrokers.Insert(broker);
        broker->SetIgnoreBufferOverrun(ignoreBufferOverrun == 1u);
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool EPICSCAOutput::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    for (uint32 i = 0u; (i < numberOfSignals); i++) {
        uint32 numberOfProducers = 0u;
        if (!GetSignalNumberOfProducers(i, nextStateName, numberOfProducers)) {
            numberOfProducers = 0u;
        }
        if (numberOfProducers > 0u) {
            /*lint -e{613} NULL pointer checked*/
            signalFlag[i] = 1u;
        }
    }
    return true;
}

uint32 EPICSCAOutput::GetStackSize() const {
    return stackSize;
}

uint32 EPICSCAOutput::GetCPUMask() const {
    return cpuMask;
}

uint32 EPICSCAOutput::GetNumberOfBuffers() const {
    return numberOfBuffers;
}

bool EPICSCAOutput::Synchronise() {
    bool ok = true;
    uint32 n;
    uint32 nOfSignals = GetNumberOfSignals();
    if (!threadContextSet) {
        /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
        ok = (ca_context_create(ca_enable_preemptive_callback) == ECA_NORMAL);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "ca_enable_preemptive_callback failed");
        }
        threadContextSet = ok;
        if ((pvs != NULL_PTR(PVWrapper *)) && (signalFlag != NULL_PTR(uint8*))) {
            for (n = 0u; (n < nOfSignals); n++) {
                if (signalFlag[n] > 0u) {
                    /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
                    ok = (ca_create_channel(&pvs[n].pvName[0], NULL_PTR(caCh *), NULL_PTR(void *), 20u, &pvs[n].pvChid) == ECA_NORMAL);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::FatalError, "ca_create_channel failed for PV with name %s", pvs[n].pvName);
                    }
                }
            }
        }
    }

    //Allow to write event at the first time!
    if (threadContextSet) {
        if ((pvs != NULL_PTR(PVWrapper *)) && (signalFlag != NULL_PTR(uint8*))) {
            for (n = 0u; (n < nOfSignals); n++) {
                if (signalFlag[n] > 0u) {

                    uint32 nRetries = 5u;
                    ok = false;
                    while ((nRetries > 0u) && (!ok)) {
                        /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
                        if (pvs[n].pvType == DBR_STRING) {
                            ok = (ca_put(pvs[n].pvType, pvs[n].pvChid, pvs[n].memory) == ECA_NORMAL);
                        }
                        else {
                            ok = (ca_array_put(pvs[n].pvType, pvs[n].numberOfElements, pvs[n].pvChid, pvs[n].memory) == ECA_NORMAL);
                        }
                        nRetries--;
                        (void) ca_pend_io(0.5);
                    }
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::FatalError, "ca_put failed for PV: %s", pvs[n].pvName);
                    }
                }
            }
        }
    }

    return ok;
}

bool EPICSCAOutput::IsIgnoringBufferOverrun() const {
    return (ignoreBufferOverrun == 1u);
}

ErrorManagement::ErrorType EPICSCAOutput::AsyncCaPut(StreamString pvName, StreamString pvVal) {
    ErrorManagement::ErrorType err;
    TypeDescriptor td;
    AnyType at;
    uint32 nRetries = 5u;
    bool ok = false;
    if (threadContextSet) {
        if (pvs != NULL_PTR(PVWrapper *)) {
            for (uint32 n = 0u; (n < numberOfSignals); n++) {
                /*lint -e{9132} False positive. pvs[n].pvName is a pointer to a string.*/
                if (StringHelper::Compare(pvName.Buffer(), pvs[n].pvName)==0) {
                    if (err.ErrorsCleared()) {
                        td = GetSignalType(n);
                        at = AnyType(td, 0u, pvs[n].memory);
                        err = !TypeConvert(at, pvVal);
                        if (err.ErrorsCleared()) {
                            /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
                            err = (ca_create_channel(&pvs[n].pvName[0], NULL_PTR(caCh *), NULL_PTR(void *), 20u, &pvs[n].pvChid) != ECA_NORMAL);
                            if (err.ErrorsCleared()) {
                                while ((nRetries > 0u) && (!ok)) {
                                    /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
                                    if (pvs[n].pvType == DBR_STRING) {
                                        ok = (ca_put(pvs[n].pvType, pvs[n].pvChid, pvs[n].memory) == ECA_NORMAL);
                                    }
                                    else {
                                        ok = (ca_array_put(pvs[n].pvType, pvs[n].numberOfElements, pvs[n].pvChid, pvs[n].memory) == ECA_NORMAL);
                                    }
                                    (void) ca_pend_io(0.5);
                                    nRetries--;
                                }
                                err = !ok;
                            }
                            else {
                                REPORT_ERROR(ErrorManagement::FatalError, "Failed to cn PV with name %s", pvs[n].pvName);
                            }
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::FatalError, "ca_create_channel failed for PV with name %s", pvs[n].pvName);
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "PV %s must be scalar", pvs[n].pvName);
                    }
                }
            }
        }
    }
    return err;
}

CLASS_REGISTER(EPICSCAOutput, "1.0")
CLASS_METHOD_REGISTER(EPICSCAOutput, AsyncCaPut)

}

