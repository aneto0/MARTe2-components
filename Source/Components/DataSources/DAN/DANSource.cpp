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
#ifdef CCS_LT_60
#include <tcn.h>
#else
#include <common/TimeTools.h> // ccs::HelperTools::GetCurrentTime, etc.
#endif

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
        DataSourceI(),
        MessageI() {
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
    interleave = true;
    fullStreamName = false;
    danStreams = NULL_PTR(DANStream**);
    filter = ReferenceT < RegisteredMethodsMessageFilter > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

/*lint -e{1551} -e{1740} must destroy all the DANStreams in the destructor.*/
DANSource::~DANSource() {
    if (danStreams != NULL_PTR(DANStream**)) {
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
bool DANSource::GetSignalMemoryBuffer(const uint32 signalIdx,
                                      const uint32 bufferIdx,
                                      void *&signalAddress) {
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
    if (danStreams != NULL_PTR(DANStream**)) {
        for (d = 0u; (d < nOfDANStreams) && (!ok); d++) {
            ok = danStreams[d]->GetSignalMemoryBuffer(signalIdx, signalAddress);
        }
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction and on the storeOnTrigger property (which is load before).*/
const char8* DANSource::GetBrokerName(StructuredDataI &data,
                                      const SignalDirection direction) {
    const char8 *brokerName = "";
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
bool DANSource::GetInputBrokers(ReferenceContainer &inputBrokers,
                                const char8 *const functionName,
                                void *const gamMemPtr) {
    return false;
}

bool DANSource::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                 const char8 *const functionName,
                                 void *const gamMemPtr) {
    bool ok = true;
    if (storeOnTrigger) {
        brokerAsyncTrigger = ReferenceT < MemoryMapAsyncTriggerOutputBroker > ("MemoryMapAsyncTriggerOutputBroker");
        ok = brokerAsyncTrigger->InitWithTriggerParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers, numberOfPreTriggers,
                                                           numberOfPostTriggers, cpuMask, stackSize);
        if (ok) {
            ok = outputBrokers.Insert(brokerAsyncTrigger);
        }
    }
    else {
        brokerAsyncOutput = ReferenceT < MemoryMapAsyncOutputBroker > ("MemoryMapAsyncOutputBroker");
        ok = brokerAsyncOutput->InitWithBufferParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers, cpuMask, stackSize);
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
bool DANSource::PrepareNextState(const char8 *const currentStateName,
                                 const char8 *const nextStateName) {
    uint32 s;
    for (s = 0u; (s < nOfDANStreams); s++) {
        /*lint -e{613} danStream cannot be NULL as nOfDANStreams is initialised to zero in the constructor*/
        danStreams[s]->Reset();
    }
    bool ok = true;
    if (!useAbsoluteTime) {

#ifdef CCS_LT_60
        hpn_timestamp_t hpnTimeStamp;
        ok = (tcn_get_time(&hpnTimeStamp) == TCN_SUCCESS);
        if (ok) {
            absoluteStartTime = static_cast<uint64>(hpnTimeStamp);
        }
#else                                
        absoluteStartTime = ccs::HelperTools::GetCurrentTime();
#endif

        for (s = 0u; (s < nOfDANStreams); s++) {
            /*lint -e{613} danStream cannot be NULL as nOfDANStreams is initialised to zero in the constructor*/
            danStreams[s]->SetAbsoluteStartTime(absoluteStartTime);
        }
    }
    return ok;
}

bool DANSource::Initialise(StructuredDataI &data) {
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
        uint8 interleaveU8 = 1u;
        if (!data.Read("Interleave", interleaveU8)) {
            interleaveU8 = 1u;
        }
        interleave = (interleaveU8 > 0u);
        REPORT_ERROR(ErrorManagement::Information, "Interleave parameter set to %d", interleave ? 1u : 0u);

        uint8 fullStreamNameU8 = 0u;
        if (!data.Read("FullStreamName", fullStreamNameU8)) {
            fullStreamNameU8 = 0u;
        }
        fullStreamName = (fullStreamNameU8 > 0u);
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
#ifdef CCS_LT_60
    if (ok) {
        ok = (tcn_init() == TCN_SUCCESS);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to tcn_init");
        }
    }
#endif
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
bool DANSource::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = data.MoveRelative("Signals");
    }
    //Do not allow samples
    if (ok) {
        uint32 functionNumberOfSignals = 0u;
        if (GetFunctionNumberOfSignals(OutputSignals, 0u, functionNumberOfSignals)) {
            for (uint32 n = 0u; (n < functionNumberOfSignals) && (ok); n++) {
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
    uint32 cnt = 0u;
    if (ok) {
        for (uint32 n = 0u; (n < originalSignalInformation.GetNumberOfChildren()) && (ok); n++) {
            //Have to read properties from the original configuration file
            ok = originalSignalInformation.MoveRelative(originalSignalInformation.GetChildName(n));

            //Check if the signal is defined as a TimeSignal
            uint32 timeSignal;
            if (originalSignalInformation.Read("TimeSignal", timeSignal)) {
                if (timeSignalIdx != -1) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Only one TimeSignal shall be defined");
                    ok = false;
                }
                if (timeSignal > 0u) {
                    timeSignalIdx = static_cast<int32>(cnt);
                    uint32 absoluteTime;
                    if (originalSignalInformation.Read("AbsoluteTime", absoluteTime)) {
                        useAbsoluteTime = (absoluteTime == 1u);
                    }
                }
            }

            bool addSignal = false;
            float64 period;
            float64 samplingFrequency = 0.F;
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
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "No Period nor SamplingFrequency specified for %s", originalSignalInformation.GetChildName(n));
            }

            if ((ok) && (addSignal)) {
                float64 periodNanosF = (1e9 / samplingFrequency);
                uint64 periodNanos = static_cast<uint64>(periodNanosF);

                StreamString structType;
                if (originalSignalInformation.Read("StructType", structType)) {

                    uint32 numberOfSamples = 1u;
                    originalSignalInformation.Read("Samples", numberOfSamples);

                    uint32 danSourceIdx = 0u;
                    bool found = false;
                    bool typeOk = false;
                    for (uint32 t = 0u; (t < nOfDANStreams) && (!found); t++) {
                        StreamString typeNameStream;
                        danStreams[t]->GetType(typeNameStream);
                        if (!typeOk) {
                            typeOk = (typeNameStream == structType);
                        }
                        found = (typeNameStream == structType);
                        if (found) {
                            found = (danStreams[t]->GetPeriodNanos() == periodNanos);
                        }
                        if (found) {
                            found = (danStreams[t]->GetNumberOfSamples() == numberOfSamples);
                        }
                        if (found) {
                            danStreams[t]->AddSignal(cnt);
                            danSourceIdx = t;
                        }
                    }
                    if (!found) {
                        uint32 nOfDANStreamsP1 = (nOfDANStreams + 1u);
                        DANStream **newDanStreams = new DANStream*[nOfDANStreamsP1];
                        for (uint32 t = 0u; t < nOfDANStreams; t++) {
                            newDanStreams[t] = danStreams[t];
                        }
                        if (danStreams != NULL_PTR(DANStream**)) {
                            delete[] danStreams;
                        }
                        danStreams = newDanStreams;

                        StreamString name = GetName();
                        if ((typeOk) || (fullStreamName)) {
                            //found another with same type... don't overwrite it... use a different name
                            name = "";
                            name.Printf("%s_T%d_N%d", GetName(), periodNanos, numberOfSamples);
                        }
                        REPORT_ERROR_STATIC(ErrorManagement::Information, "Creating stream %s_%s", name.Buffer(), structType.Buffer());

                        danStreams[nOfDANStreams] = new DANStream(structType, name.Buffer(), danBufferMultiplier, samplingFrequency, numberOfSamples,
                                                                  interleave);
                        danStreams[nOfDANStreams]->AddSignal(cnt);
                        danSourceIdx = nOfDANStreams;
                        nOfDANStreams++;
                    }

                    for (uint32 h = 0u; (h < originalSignalInformation.GetNumberOfChildren()) && ok; h++) {
                        if (originalSignalInformation.MoveRelative(originalSignalInformation.GetChildName(h))) {
                            StreamString unit;
                            if (!originalSignalInformation.Read("Unit", unit)) {
                                unit = "Unknown";
                            }
                            StreamString description;
                            if (!originalSignalInformation.Read("Description", description)) {
                                description = "Unknown";
                            }

                            uint32 numberOfElements;
                            uint8 numberOfDimensions;
                            ok = GetSignalNumberOfElements(cnt, numberOfElements);
                            if (ok) {
                                ok = GetSignalNumberOfDimensions(cnt, numberOfDimensions);
                            }
                            if (ok) {
                                TypeDescriptor typeDesc = GetSignalType(cnt);
                                danStreams[danSourceIdx]->AddToStructure(cnt, typeDesc, numberOfElements, numberOfDimensions, unit, description);
                                originalSignalInformation.MoveToAncestor(1u);
                            }
                            cnt++;
                        }
                    }
                }
                else {
                    uint32 numberOfElements;
                    ok = GetSignalNumberOfElements(cnt, numberOfElements);
                    if (ok) {
                        TypeDescriptor typeDesc = GetSignalType(cnt);
                        uint32 fieldSize = static_cast<uint32>(typeDesc.numberOfBits) / 8u;
                        StreamString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(typeDesc);
                        bool found = false;
                        bool typeOk = false;
                        /*lint -e{414} samplingFrequency != 0 is checked above*/
                        for (uint32 t = 0u; (t < nOfDANStreams) && (!found); t++) {
                            StreamString typeNameStream;
                            danStreams[t]->GetType(typeNameStream);
                            if (!typeOk) {
                                typeOk = (typeNameStream == typeName);
                            }
                            found = (typeNameStream == typeName);
                            if (found) {
                                found = (danStreams[t]->GetPeriodNanos() == periodNanos);
                            }
                            if (found) {
                                found = (danStreams[t]->GetNumberOfSamples() == numberOfElements);
                            }
                            if (found) {
                                danStreams[t]->AddSignal(cnt);
                                danStreams[t]->AddToStructure(cnt, typeDesc, 1u, 0u, "Unknown", "Unknown");
                            }
                        }
                        if (!found) {
                            uint32 nOfDANStreamsP1 = (nOfDANStreams + 1u);
                            DANStream **newDanStreams = new DANStream*[nOfDANStreamsP1];
                            for (uint32 t = 0u; t < nOfDANStreams; t++) {
                                newDanStreams[t] = danStreams[t];
                            }
                            if (danStreams != NULL_PTR(DANStream**)) {
                                delete[] danStreams;
                            }
                            danStreams = newDanStreams;

                            StreamString name = GetName();
                            if ((typeOk) || (fullStreamName)) {
                                //found another with same type... don't overwrite it... use a different name
                                name = "";
                                name.Printf("%s_T%d_N%d", GetName(), periodNanos, numberOfElements);
                            }
                            REPORT_ERROR_STATIC(ErrorManagement::Information, "Creating stream %s_%s", name.Buffer(), typeName.Buffer());

                            danStreams[nOfDANStreams] = new DANStream(typeName, name.Buffer(), danBufferMultiplier, samplingFrequency, numberOfElements,
                                                                      interleave);
                            danStreams[nOfDANStreams]->AddSignal(cnt);
                            danStreams[nOfDANStreams]->AddToStructure(cnt, typeDesc, 1u, 0u, "Unknown", "Unknown");
                            nOfDANStreams++;
                        }
                    }
                    cnt++;
                }
            }
            else {
                cnt++;
            }
            if (ok) {
                ok = originalSignalInformation.MoveToAncestor(1u);
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
        for (s = 0u; (s < nOfDANStreams) && ok; s++) {
            ok = (danStreams[s]->Finalise());
            if (ok) {
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
    }

    return ok;
}

ErrorManagement::ErrorType DANSource::OpenStream() {
    uint32 t;
    bool ok = (danStreams != NULL_PTR(DANStream**));
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
    bool ok = (danStreams != NULL_PTR(DANStream**));
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

