/**
 * @file RealTimeThreadSynchronisation.cpp
 * @brief Source file for class RealTimeThreadSynchronisation
 * @date 24/04/2017
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
 * the class RealTimeThreadSynchronisation (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "RealTimeThreadSynchBroker.h"
#include "RealTimeThreadSynchronisation.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

RealTimeThreadSynchronisation::RealTimeThreadSynchronisation() :
        DataSourceI() {
    memory = NULL_PTR(char8 *);
    memorySize = 0u;
    numberOfSyncGAMs = 0u;
    memoryOffsets = NULL_PTR(uint32 *);
    synchInputBrokers = NULL_PTR(RealTimeThreadSynchBroker **);
    currentInitBrokerIndex = -1;
    waitForNext = 0u;
}

/*lint -e{1551} must free the allocated memory in the destructor. */
RealTimeThreadSynchronisation::~RealTimeThreadSynchronisation() {
    if (synchInputBrokers != NULL_PTR(RealTimeThreadSynchBroker **)) {
        delete[] synchInputBrokers;
    }
    if (memory != NULL_PTR(char8 *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(memory));
    }
    if (memoryOffsets != NULL_PTR(uint32 *)) {
        delete[] memoryOffsets;
    }
}

bool RealTimeThreadSynchronisation::AllocateMemory() {
    return true;
}

uint32 RealTimeThreadSynchronisation::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool RealTimeThreadSynchronisation::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    bool ok = (signalIdx < GetNumberOfSignals());
    if (ok) {
        ok = (memoryOffsets != NULL_PTR(uint32 *));
    }
    if (ok) {
        ok = (memory != NULL_PTR(char8 *));
    }
    if (ok) {
        if (currentInitBrokerIndex == -1) {
            if ((memory != NULL_PTR(char8 *)) && (memoryOffsets != NULL_PTR(uint32 *))) {
                signalAddress = reinterpret_cast<void *>(&memory[memoryOffsets[signalIdx]]);
            }
        }
        else {
            if (synchInputBrokers != NULL_PTR(RealTimeThreadSynchBroker **)) {
                ok = synchInputBrokers[currentInitBrokerIndex]->GetSignalMemoryBuffer(signalIdx, signalAddress);
            }
        }
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction */
const char8 *RealTimeThreadSynchronisation::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    const char8 *brokerName = "RealTimeThreadSynchBroker";
    if (direction == OutputSignals) {
        brokerName = "MemoryMapSynchronisedOutputBroker";
    }
    return brokerName;
}

bool RealTimeThreadSynchronisation::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    ReferenceT<RealTimeThreadSynchBroker> broker;
    bool ok = false;
    if (synchInputBrokers != NULL_PTR(RealTimeThreadSynchBroker **)) {
        uint32 n;
        for (n = 0u; (n < numberOfSyncGAMs) && (!ok); n++) {
            StreamString gamName = synchInputBrokers[n]->GetGAMName();
            if (gamName == functionName) {
                currentInitBrokerIndex = static_cast<int32>(n);
                ok = true;
                broker = synchInputBrokersContainer.Get(n);
            }
        }
    }
    if (broker.IsValid()) {
        StreamString brokerName = functionName;
        brokerName += ".InputBroker.";
        brokerName += "RealTimeThreadSynchBroker";
        broker->SetName(brokerName.Buffer());
        ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ok) {
        ok = inputBrokers.Insert(broker);
    }

    return ok;
}

bool RealTimeThreadSynchronisation::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    ReferenceT<MemoryMapSynchronisedOutputBroker> broker("MemoryMapSynchronisedOutputBroker");
    currentInitBrokerIndex = -1;
    bool ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    if (ok) {
        StreamString brokerName = functionName;
        brokerName += ".OutputBroker.";
        brokerName += "MemoryMapSynchronisedOutputBroker";
        broker->SetName(brokerName.Buffer());

        ok = outputBrokers.Insert(broker);
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool RealTimeThreadSynchronisation::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool RealTimeThreadSynchronisation::Initialise(StructuredDataI & data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        uint32 timeoutMillis = 0u;
        if (data.Read("Timeout", timeoutMillis)) {
            timeout = timeoutMillis;
        }
        else {
            timeout = 1000u;
        }
    }
    if (!data.Read("WaitForNext", waitForNext)) {
        waitForNext = 0u;
    }
    return ok;
}

bool RealTimeThreadSynchronisation::SetConfiguredDatabase(StructuredDataI & data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    //At most one output function is allowed to interact with the RealTimeThreadSynchronisation
    uint32 numberOfFunctions = GetNumberOfFunctions();
    //A function shall either read or write from/to this DataSourceI but not both things at the same time
    bool producerFound = false;
    if (numberOfFunctions > 0u) {
        numberOfSyncGAMs = (numberOfFunctions - 1u);
    }
    else {
        numberOfSyncGAMs = 0u;
    }
    uint32 n;
    if (ok) {
        if (numberOfSyncGAMs > 0u) {
            synchInputBrokers = new RealTimeThreadSynchBroker*[numberOfSyncGAMs];
        }
        //Compute the memory for the output function
        for (n = 0u; (n < numberOfFunctions) && (ok); n++) {
            uint32 numberOfFunctionSignals;
            ok = GetFunctionNumberOfSignals(OutputSignals, n, numberOfFunctionSignals);
            bool isProducer = (numberOfFunctionSignals > 0u);
            if (isProducer) {
                ok = !producerFound;
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "One and exactly one function shall write into this DataSourceI and more than one was found");
                }
                if (ok) {
                    producerFound = isProducer;
                    if (numberOfFunctionSignals != GetNumberOfSignals()) {
                        REPORT_ERROR_STATIC(ErrorManagement::Warning, "The GAM which writes to this RealTimeThreadSynchronisation does not produce all the signals.");
                    }

                    memoryOffsets = new uint32[numberOfFunctionSignals];
                    //Check that the number of samples is exactly one.

                    uint32 numberOfSamplesRead;
                    uint32 s;
                    for (s = 0u; (s < numberOfFunctionSignals) && (ok); s++) {
                        ok = GetFunctionSignalSamples(OutputSignals, n, s, numberOfSamplesRead);
                        if (ok) {
                            ok = (numberOfSamplesRead == 1u);
                            if (!ok) {
                                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "The number of samples of the output function shall be one");
                            }
                        }
                    }
                }

                if (ok) {
                    uint32 nOfSignals = GetNumberOfSignals();
                    uint32 s;
                    if (memoryOffsets != NULL_PTR(uint32 *)) {
                        for (s = 0u; (s < nOfSignals) && (ok); s++) {
                            uint32 size = 0u;
                            ok = GetSignalByteSize(s, size);
                            memoryOffsets[s] = memorySize;
                            memorySize += size;
                        }
                    }
                    if (ok) {
                        memory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(memorySize));
                    }
                }
            }
            else {
                ReferenceT<RealTimeThreadSynchBroker> synchInputBroker(new RealTimeThreadSynchBroker());
                (void) synchInputBrokersContainer.Insert(synchInputBroker);
                synchInputBroker->SetFunctionIndex(this, n, timeout, waitForNext);
            }
        }
    }
    if (ok) {
        if (numberOfFunctions > 0u) {
            ok = producerFound;
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "One and exactly one function shall write into this DataSourceI and none was found");
            }
        }
    }
    //Create the synchInputBrokers
    if (synchInputBrokers != NULL_PTR(RealTimeThreadSynchBroker **)) {
        for (n = 0u; (n < numberOfSyncGAMs) && (ok); n++) {
            ReferenceT<RealTimeThreadSynchBroker> synchInputBroker = synchInputBrokersContainer.Get(n);
            synchInputBrokers[n] = dynamic_cast<RealTimeThreadSynchBroker *>(synchInputBroker.operator ->());
            ok = synchInputBrokers[n]->AllocateMemory(memory, memoryOffsets);
        }
    }
    return ok;
}

TimeoutType RealTimeThreadSynchronisation::GetSynchroniseTimeout() const {
    return timeout;
}

bool RealTimeThreadSynchronisation::Synchronise() {
    bool ok = true;
    uint32 u;
    if (synchInputBrokers != NULL_PTR(RealTimeThreadSynchBroker **)) {
        for (u = 0u; (u < numberOfSyncGAMs) && (ok); u++) {
            ok = synchInputBrokers[u]->AddSample();
        }
    }

    return ok;
}

CLASS_REGISTER(RealTimeThreadSynchronisation, "1.0")
}

