/**
 * @file DANSource.cpp
 * @brief Source file for class DANSource
 * @date 04/04/2017
 * @author Andre Neto
 * @author Lana Abadie
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
 * the class DANSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <tcn.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "DANAPI.h"
#include "DANSource.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

DANSource::DANSource() :
        DataSourceI(), MessageI() {
    storeOnTrigger = false;
    numberOfPreTriggers = 0u;
    numberOfPostTriggers = 0u;
    numberOfBuffers = 0u;
    timeSignalIdx = -1;
    cpuMask = 0xfu;
    stackSize = 0u;
    danBufferMultiplier = 0u;
    nOfDANStreams = 0u;
    timeUInt32 = 0u;
    timeUInt64 = 0u;
    trigger = 0u;
    useAbsoluteTime = false;
    absoluteStartTime = 0LLU;
    danStreams = NULL_PTR(DANStream **);
    filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

/*lint -e{1551} -e{1740} must destroy all the DANStreams in the destructor.*/
DANSource::~DANSource() {
    if (danStreams != NULL_PTR(DANStream **)) {
        uint32 s;
        for (s = 0u; s < nOfDANStreams; s++) {
            delete danStreams[s];
        }

        delete[] danStreams;
    }
    DANAPI::CloseLibrary();
}

bool DANSource::AllocateMemory() {
    return true;
}

uint32 DANSource::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool DANSource::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    uint32 d;
    bool ok = false;
    if (storeOnTrigger) {
        if (signalIdx == 0u) {
            signalAddress = &trigger;
            ok = true;
        }
    }
    bool useTimeSignal = (timeSignalIdx > -1);
    if (useTimeSignal) {
        if (static_cast<uint32>(timeSignalIdx) == signalIdx) {
            if (useAbsoluteTime) {
                signalAddress = &timeUInt64;
            }
            else {
                signalAddress = &timeUInt32;
            }
            ok = true;
        }
    }
    if (danStreams != NULL_PTR(DANStream **)) {
        for (d = 0u; (d < nOfDANStreams) && (!ok); d++) {
            ok = danStreams[d]->GetSignalMemoryBuffer(signalIdx, signalAddress);
        }
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction and on the storeOnTrigger property (which is load before).*/
const char8* DANSource::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8* brokerName = "";
    if (direction == OutputSignals) {
        if (storeOnTrigger) {
            brokerName = "MemoryMapAsyncTriggerOutputBroker";
        }
        else {
            brokerName = "MemoryMapAsyncOutputBroker";
        }
    }
    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: InputBrokers are not supported. Function returns false irrespectively of the parameters.*/
bool DANSource::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

bool DANSource::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    bool ok = true;
    if (storeOnTrigger) {
        brokerAsyncTrigger = ReferenceT<MemoryMapAsyncTriggerOutputBroker>("MemoryMapAsyncTriggerOutputBroker");
        ok = brokerAsyncTrigger->InitWithTriggerParameters(OutputSignals, *this, functionName, gamMemPtr,
                                                              numberOfBuffers, numberOfPreTriggers,
                                                              numberOfPostTriggers, cpuMask, stackSize);
        if (ok) {
            ok = outputBrokers.Insert(brokerAsyncTrigger);
        }
    }
    else {
        brokerAsyncOutput = ReferenceT<MemoryMapAsyncOutputBroker> ("MemoryMapAsyncOutputBroker");
        ok = brokerAsyncOutput->InitWithBufferParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers,
                                                   cpuMask, stackSize);
        if (ok) {
            ok = outputBrokers.Insert(brokerAsyncOutput);
        }
    }
    return ok;
}


bool DANSource::Synchronise() {
    bool ok = true;
    uint32 s;
    for (s = 0u; (s < nOfDANStreams) && (ok); s++) {
        /*lint -e{613} danStream cannot be NULL as nOfDANStreams is initialised to zero in the constructor*/
        ok = danStreams[s]->PutData();
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool DANSource::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    uint32 s;
    for (s = 0u; (s < nOfDANStreams); s++) {
        /*lint -e{613} danStream cannot be NULL as nOfDANStreams is initialised to zero in the constructor*/
        danStreams[s]->Reset();
    }
    bool ok = true;
    if (!useAbsoluteTime) {
        hpn_timestamp_t hpnTimeStamp;
        ok = (tcn_get_time(&hpnTimeStamp) == TCN_SUCCESS);
        if (ok) {
            absoluteStartTime = static_cast<uint64>(hpnTimeStamp);
            for (s = 0u; (s < nOfDANStreams); s++) {
                /*lint -e{613} danStream cannot be NULL as nOfDANStreams is initialised to zero in the constructor*/
                danStreams[s]->SetAbsoluteStartTime(absoluteStartTime);
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to tcn_get_time");
        }
    }
    return ok;
}

bool DANSource::Initialise(StructuredDataI& data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("NumberOfBuffers", numberOfBuffers);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfBuffers shall be specified");
        }
    }
    if (ok) {
        ok = (numberOfBuffers > 0u);

        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfBuffers shall be > 0u");
        }
    }
    if (ok) {
        uint64 cpuMaskIn;
        ok = data.Read("CPUMask", cpuMaskIn);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "CPUMask shall be specified");
        }
        else {
            cpuMask = BitSet(cpuMaskIn);
        }
    }
    if (ok) {
        ok = data.Read("StackSize", stackSize);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "StackSize shall be specified");
        }
    }
    if (ok) {
        ok = (stackSize > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "StackSize shall be > 0u");
        }
    }
    if (ok) {
        ok = data.Read("DanBufferMultiplier", danBufferMultiplier);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "DanBufferMultiplier shall be specified");
        }
    }
    uint32 storeOnTriggerU = 0u;
    if (ok) {
        ok = data.Read("StoreOnTrigger", storeOnTriggerU);
        storeOnTrigger = (storeOnTriggerU == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "StoreOnTrigger shall be specified");
        }
    }
    if (storeOnTrigger) {
        if (ok) {
            ok = data.Read("NumberOfPreTriggers", numberOfPreTriggers);

            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfPreTriggers shall be specified");
            }
        }
        if (ok) {
            ok = data.Read("NumberOfPostTriggers", numberOfPostTriggers);

            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfPostTriggers shall be specified");
            }
        }
    }
    if (ok) {
        StreamString icProgName;
        if (data.Read("ICProgName", icProgName)) {
            REPORT_ERROR(ErrorManagement::Debug, "Calling dan_initLibrary_icprog with program name %s", icProgName.Buffer());
            ok = DANAPI::InitLibraryICProg(icProgName.Buffer());
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Failed to dan_initLibrary_icprog with program name %s", icProgName.Buffer());
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Debug, "Calling dan_initLibrary");
            ok = DANAPI::InitLibrary();
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Failed to dan_initLibrary");
            }
        }
    }
    if (ok) {
        ok = (tcn_init() == TCN_SUCCESS);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to tcn_init");
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

/*lint -e{613} danStream cannot be NULL as nOfDANStreams is initialised to zero in the constructor*/
bool DANSource::SetConfiguredDatabase(StructuredDataI& data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = data.MoveRelative("Signals");
    }
    //Do not allow samples
    if (ok) {
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

    //Check the signal index of the timing signal.
    uint32 nOfSignals = GetNumberOfSignals();
    if (ok) {
        uint32 n;
        for (n = 0u; (n < nOfSignals) && (ok); n++) {
            ok = data.MoveRelative(data.GetChildName(n));
            if (ok) {
                //Have to read properties from the original configuration file
                ok = originalSignalInformation.MoveRelative(originalSignalInformation.GetChildName(n));
            }
            //Check if the signal is defined as a TimeSignal
            uint32 timeSignal;
            if (originalSignalInformation.Read("TimeSignal", timeSignal)) {
                if (timeSignalIdx != -1) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Only one TimeSignal shall be defined");
                    ok = false;
                }
                if (timeSignal > 0u) {
                    timeSignalIdx = static_cast<int32>(n);
                    uint32 absoluteTime;
                    if (originalSignalInformation.Read("AbsoluteTime", absoluteTime)) {
                        useAbsoluteTime = (absoluteTime == 1u);
                    }
                }
            }
            //Check if the signal Period or the SamplingFrequency are defined
            float64 period;
            float64 samplingFrequency = 0.F;
            uint32 numberOfElements;
            TypeDescriptor typeDesc;
            //Only add signals that have Period SamplingFrequency specified
            bool addSignal = false;
            if (ok) {
                addSignal = (originalSignalInformation.Read("Period", period));
                if (addSignal) {
                    if (period > 0.F) {
                        samplingFrequency = (1.F / period);
                    }
                    else {
                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Period shall be > 0");
                        ok = false;
                    }
                }
                else {
                    addSignal = (originalSignalInformation.Read("SamplingFrequency", samplingFrequency));
                    if (addSignal) {
                        ok = (samplingFrequency > 0.F);
                        if (!ok) {
                            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "SamplingFrequency shall be > 0");
                        }
                    }
                }
                if (!addSignal) {
                    StreamString signalName;
                    (void) GetSignalName(n, signalName);
                    REPORT_ERROR_STATIC(ErrorManagement::Warning, "No Period nor SamplingFrequency specified for signal %s", signalName.Buffer());
                }
            }
            //Only add signals that had Period or SamplingFrequency specified
            if ((ok) && (addSignal)) {
                ok = GetSignalNumberOfElements(n, numberOfElements);
                if (ok) {
                    typeDesc = GetSignalType(n);
                    bool found = false;
                    uint32 t;
                    for (t = 0u; (t < nOfDANStreams) && (!found); t++) {
                        /*lint -e{414} samplingFrequency != 0 is checked above*/
                        float64 periodNanosF = (1e9 / samplingFrequency);
                        uint64 periodNanos = static_cast<uint64>(periodNanosF);
                        found = (danStreams[t]->GetPeriodNanos() == periodNanos);
                        if (found) {
                            found = (danStreams[t]->GetType() == typeDesc);
                        }
                        if (found) {
                            danStreams[t]->AddSignal(n);
                        }
                    }
                    if (!found) {
                        uint32 nOfDANStreamsP1 = (nOfDANStreams + 1u);
                        DANStream **newDanStreams = new DANStream *[nOfDANStreamsP1];
                        for (t = 0u; t < nOfDANStreams; t++) {
                            newDanStreams[t] = danStreams[t];
                        }
                        if (danStreams != NULL_PTR(DANStream **)) {
                            delete[] danStreams;
                        }
                        danStreams = newDanStreams;

                        danStreams[nOfDANStreams] = new DANStream(typeDesc, GetName(), danBufferMultiplier, samplingFrequency, numberOfElements);
                        danStreams[nOfDANStreams]->AddSignal(n);
                        nOfDANStreams++;
                    }
                }
            }
            if (ok) {
                ok = originalSignalInformation.MoveToAncestor(1u);
            }
            if (ok) {
                ok = data.MoveToAncestor(1u);
            }
        }
    }
    if (ok) {
        ok = (nOfDANStreams > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "At least one signal to be stored in DAN shall be defined.");
        }
    }
    if (ok) {
        ok = data.MoveToAncestor(1u);
    }
    //Only one and one GAM allowed to interact with this DataSourceI
    if (ok) {
        ok = (GetNumberOfFunctions() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly one Function allowed to interact with this DataSourceI");
        }
    }

    //Check if a time signal was set
    bool useTimeSignal = (timeSignalIdx > -1);
    if (storeOnTrigger) {
        if (ok) {
            ok = (useTimeSignal);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "StoreOnTrigger was specified but no TimeSignal was found");
            }
        }
    }
    if (useTimeSignal) {
        if (ok) {
            if (useAbsoluteTime) {
                ok = (GetSignalType(static_cast<uint32>(timeSignalIdx)) == UnsignedInteger64Bit);
                if (!ok) {
                    ok = (GetSignalType(static_cast<uint32>(timeSignalIdx)) == SignedInteger64Bit);
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "TimeSignal with IsAbsoluteTime shall have type uint64 or int64");
                }
            }
            else {
                ok = (GetSignalType(static_cast<uint32>(timeSignalIdx)) == UnsignedInteger32Bit);
                if (!ok) {
                    ok = (GetSignalType(static_cast<uint32>(timeSignalIdx)) == SignedInteger32Bit);
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "TimeSignal shall have type uint32 or int32");
                }
            }
        }
    }
    uint32 s;
    if (ok) {
        for (s = 0u; (s < nOfDANStreams); s++) {
            danStreams[s]->Finalise();
            if (useTimeSignal) {
                if (useAbsoluteTime) {
                    danStreams[s]->SetAbsoluteTimeSignal(&timeUInt64);
                }
                else {
                    danStreams[s]->SetRelativeTimeSignal(&timeUInt32);
                }
            }
        }
    }

    return ok;
}

ErrorManagement::ErrorType DANSource::OpenStream() {
    uint32 t;
    bool ok = (danStreams != NULL_PTR(DANStream **));
    for (t = 0u; (t < nOfDANStreams) && (ok); t++) {
        /*lint -e{613} danStream cannot be NULL as nOfDANStreams is initialised to zero in the constructor*/
        ok = danStreams[t]->OpenStream();
    }
    ErrorManagement::ErrorType ret(ok);
    return ret;
    /*lint -e{1762} function cannot be constant as it is registered as an RPC for CLASS_METHOD_REGISTER*/
}

ErrorManagement::ErrorType DANSource::CloseStream() {
    uint32 t;
    bool ok = (danStreams != NULL_PTR(DANStream **));
    for (t = 0u; (t < nOfDANStreams) && (ok); t++) {
        /*lint -e{613} danStream cannot be NULL as nOfDANStreams is initialised to zero in the constructor*/
        ok = danStreams[t]->CloseStream();
    }
    ErrorManagement::ErrorType ret(ok);
    return ret;
    /*lint -e{1762} function cannot be constant as it is registered as an RPC for CLASS_METHOD_REGISTER*/
}

const ProcessorType& DANSource::GetCPUMask() const {
    return cpuMask;
}

uint32 DANSource::GetNumberOfBuffers() const {
    return numberOfBuffers;
}

uint32 DANSource::GetNumberOfPostTriggers() const {
    return numberOfPostTriggers;
}

uint32 DANSource::GetNumberOfPreTriggers() const {
    return numberOfPreTriggers;
}

uint32 DANSource::GetStackSize() const {
    return stackSize;
}

bool DANSource::IsAbsoluteTime() const {
    return useAbsoluteTime;
}

bool DANSource::IsStoreOnTrigger() const {
    return storeOnTrigger;
}

int32 DANSource::GetTimeSignalIdx() const {
    return timeSignalIdx;
}

uint64 DANSource::GetAbsoluteStartTime() const {
    return absoluteStartTime;
}

uint32 DANSource::GetDANBufferMultiplier() const {
    return danBufferMultiplier;
}

void DANSource::Purge(ReferenceContainer &purgeList) {
    if (brokerAsyncTrigger.IsValid()) {
        (void) brokerAsyncTrigger->FlushAllTriggers();
        brokerAsyncTrigger->UnlinkDataSource();
    }
    if (brokerAsyncOutput.IsValid()) {
        (void) brokerAsyncOutput->Flush();
        brokerAsyncOutput->UnlinkDataSource();
    }
    DataSourceI::Purge(purgeList);
}

/*
 dan_DataCore DANSource::GetDANDataCore() {
 return danDataCore;
 }**/

CLASS_REGISTER(DANSource, "1.0")
CLASS_METHOD_REGISTER(DANSource, OpenStream)
CLASS_METHOD_REGISTER(DANSource, CloseStream)

}

