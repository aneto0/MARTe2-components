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
#include "dan/dan_DataCore.h"
#include "dan/dan_Source.h"
#include "tcn.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "DANSource.h"
#include "MemoryMapAsyncOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
dan_DataCore MARTe::DANSource::danDataCore = NULL_PTR(dan_DataCore);

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
    dataSourceMemory = NULL_PTR(char8 *);
    offsets = NULL_PTR(uint32 *);
    cpuMask = 0xfu;
    stackSize = 0u;
    danBufferMultiplier = 0u;
    totalSignalMemory = 0u;
    samplingRate = 0.0F;
    nOfDANStreams = 0u;
    danStreams = NULL_PTR(DANStream *);
    brokerAsyncTrigger = NULL_PTR(MemoryMapAsyncTriggerOutputBroker *);
    filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

DANSource::~DANSource() {
    if (dataSourceMemory != NULL_PTR(char8 *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(dataSourceMemory));
    }
    if (offsets != NULL_PTR(uint32 *)) {
        delete[] offsets;
    }
}

bool DANSource::AllocateMemory() {
    return true;
}

uint32 DANSource::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool DANSource::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (dataSourceMemory != NULL_PTR(char8 *));
    if (ok) {
        /*lint -e{613} dataSourceMemory cannot be NULL here*/
        char8 *memPtr = &dataSourceMemory[offsets[signalIdx]];
        signalAddress = reinterpret_cast<void *&>(memPtr);
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
        ReferenceT<MemoryMapAsyncTriggerOutputBroker> brokerAsyncTriggerNew("MemoryMapAsyncTriggerOutputBroker");
        brokerAsyncTrigger = brokerAsyncTriggerNew.operator ->();
        ok = brokerAsyncTriggerNew->InitWithTriggerParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers, numberOfPreTriggers, numberOfPostTriggers, cpuMask, stackSize);
        if (ok) {
            ok = outputBrokers.Insert(brokerAsyncTriggerNew);
        }
    }
    else {
        ReferenceT<MemoryMapAsyncOutputBroker> brokerAsync("MemoryMapAsyncOutputBroker");
        ok = brokerAsync->InitWithBufferParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers, cpuMask, stackSize);
        if (ok) {
            ok = outputBrokers.Insert(brokerAsync);
        }
    }
    return ok;
}

//TODO handle absolute time
bool DANSource::Synchronise() {
    hpn_timestamp_t hpnTimeStamp;
    bool ok = (tcn_get_time(&hpnTimeStamp) == TCN_SUCCESS);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to tcn_get_time");
    }
    uint32 s;
    for(s=0u; (s<nOfDANStreams) && (ok); s++) {
        ok = danStreams[s]->PutData(hpnTimeStamp);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool DANSource::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool DANSource::Initialise(StructuredDataI& data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("NumberOfBuffers", numberOfBuffers);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfBuffers shall be specified");
    }
    if (ok) {
        ok = (numberOfBuffers > 0u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfBuffers shall be > 0u");
    }
    if (ok) {
        uint32 cpuMaskIn;
        ok = data.Read("CPUMask", cpuMaskIn);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "CPUMask shall be specified");
        }
        else {
            cpuMask = cpuMaskIn;
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
    if (ok) {
        ok = data.Read("SamplingRate", samplingRate);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "SamplingRate shall be specified");
        }
    }
    uint32 storeOnTriggerU = 0u;
    if (ok) {
        ok = data.Read("StoreOnTrigger", storeOnTriggerU);
        storeOnTrigger = (storeOnTriggerU == 1u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "StoreOnTrigger shall be specified");
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
        if (danDataCore != NULL_PTR(dan_DataCore)) {
            danDataCore = dan_initLibrary();
            ok = (danDataCore != NULL_PTR(dan_DataCore));
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to dan_initLibrary");
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
    return ok;
}

bool DANSource::SetConfiguredDatabase(StructuredDataI& data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = data.MoveRelative("Signals");
    }
    //Check signal properties and compute memory
    totalSignalMemory = 0u;
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

        offsets = new uint32[GetNumberOfSignals()];
        uint32 nOfSignals = GetNumberOfSignals();
        //Count the number of bytes
        for (n = 0u; (n < nOfSignals) && (ok); n++) {
            offsets[n] = totalSignalMemory;
            uint32 nBytes = 0u;
            ok = GetSignalByteSize(n, nBytes);
            totalSignalMemory += nBytes;
        }
    }
    //Allocate memory
    if (ok) {
        dataSourceMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalSignalMemory));
    }

    if (ok) {
        //If the trigger is defined this signal will not be added to the data block (as it will have a different type)
        uint32 danBufferSize = totalSignalMemory;
        if (IsStoreOnTrigger()) {
            danBufferSize -= sizeof(uint8);
        }
        danBufferSize *= danBufferMultiplier;
        danSource = dan_publisher_publishSource_withDAQBuffer(danDataCore, GetName(), danBufferSize);
    }

    //Check the signal index of the timing signal.
    uint32 nOfSignals = GetNumberOfSignals();
    if (ok) {
        //Count the number of signals
        uint32 n;
        for (n = 0u; (n < nOfSignals) && (ok); n++) {
            ok = data.MoveRelative(data.GetChildName(n));
            if (ok) {
                //Have to mix and match between the original setting of the DataSource signal (i.e. the one MDS+ related)
                //and the ones which are later added by the RealTimeApplicationConfigurationBuilder
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
            ok = (GetSignalType(static_cast<uint32>(timeSignalIdx)) == UnsignedInteger32Bit);
            if (!ok) {
                ok = (GetSignalType(static_cast<uint32>(timeSignalIdx)) == SignedInteger32Bit);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "TimeSignal shall have type uint32 or int32");
            }
        }
    }

    return ok;
}

ErrorManagement::ErrorType DANSource::OpenStream() {
    bool ok = (dan_publisher_openStream(danSource, samplingRate, 0u) == 0u);
    if (ok) {
        if (brokerAsyncTrigger != NULL_PTR(MemoryMapAsyncTriggerOutputBroker *)) {
            brokerAsyncTrigger->ResetPreTriggerBuffers();
        }
    }
    ErrorManagement::ErrorType ret(ok);
    return ret;
}

ErrorManagement::ErrorType DANSource::CloseStream() {
    bool ok;
    if (danSource != NULL_PTR(dan_Source)) {
        ok = (dan_publisher_closeStream(danSource) == 0u);
    }

    ErrorManagement::ErrorType err(ok);
    return err;
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

bool DANSource::IsStoreOnTrigger() const {
    return storeOnTrigger;
}

int32 DANSource::GetTimeSignalIdx() const {
    return timeSignalIdx;
}

dan_DataCore DANSource::GetDANDataCore() const {
    return danDataCore;
}


CLASS_REGISTER(DANSource, "1.0")
CLASS_METHOD_REGISTER(DANSource, OpenStream)
CLASS_METHOD_REGISTER(DANSource, CloseStream)

}

