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
}

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

bool RealTimeThreadSynchronisation::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    bool ok = (signalIdx < GetNumberOfSignals());
    if (ok) {
        ok = (memoryOffsets != NULL_PTR(uint32 *));
    }
    if (ok) {
        ok = (memory != NULL_PTR(char8 *));
    }
    if (ok) {
        ok = (synchInputBrokers != NULL_PTR(RealTimeThreadSynchBroker **));
    }
    if (ok) {
        if (currentInitBrokerIndex == -1) {
            signalAddress = reinterpret_cast<void *>(&memory[memoryOffsets[signalIdx]]);
        }
        else {
            ok = synchInputBrokers[currentInitBrokerIndex]->GetSignalMemoryBuffer(signalIdx, signalAddress);
        }
    }
    return ok;
}

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
                currentInitBrokerIndex = static_cast<uint32>(n);
                ok = true;
                broker = synchInputBrokersContainer.Get(n);
            }
        }
    }
    if (broker.IsValid()) {
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
        ok = outputBrokers.Insert(broker);
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool RealTimeThreadSynchronisation::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool RealTimeThreadSynchronisation::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
    return ok;
}

bool RealTimeThreadSynchronisation::SetConfiguredDatabase(StructuredDataI & data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    //At most one output function is allowed to interact with the RealTimeThreadSynchronisation
    uint32 numberOfFunctions = GetNumberOfFunctions();
    if (ok) {
        //At least one consumer function (plus the producer)
        ok = (numberOfFunctions > 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "One and exactly one function shall write into the DataSourceI and at least one function shall read from it");
        }
    }
    //A function shall either or write from this DataSourceI but not both things at the same time
    bool producerFound = false;
    numberOfSyncGAMs = (numberOfFunctions - 1u);
    uint32 n;
    if (ok) {
        synchInputBrokers = new RealTimeThreadSynchBroker*[numberOfSyncGAMs];
        //Compute the memory for the output function
        for (n = 0u; (n < numberOfFunctions) && (ok); n++) {
            uint32 numberOfSignals;
            ok = GetFunctionNumberOfSignals(OutputSignals, n, numberOfSignals);
            bool isProducer = (numberOfSignals > 0u);
            if (isProducer) {
                ok = !producerFound;
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "One and exactly one function shall write into this DataSourceI and more than one was found");
                }
                producerFound = isProducer;
                uint32 numberOfFunctionSignals;
                if (ok) {
                    ok = GetFunctionNumberOfSignals(OutputSignals, n, numberOfFunctionSignals);
                }
                //Check that the number of samples is exactly one.
                if (ok) {
                    if (numberOfFunctionSignals != numberOfSignals) {
                        REPORT_ERROR_STATIC(ErrorManagement::Warning, "The GAM which writes to this RealTimeThreadSynchronisation does not produce all the signals.");
                    }

                    memoryOffsets = new uint32[numberOfFunctionSignals];

                    uint32 numberOfSamplesRead;
                    uint32 s;
                    for (s = 0u; (s < numberOfFunctionSignals) && (ok); s++) {
                        ok = GetFunctionSignalSamples(OutputSignals, n, s, numberOfSamplesRead);
                        if (ok) {
                            ok = (numberOfSamplesRead == 1);
                            if (!ok) {
                                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "The number of samples of the output function shall be one");
                            }
                        }
                    }
                }

                if (ok) {
                    uint32 numberOfSignals = GetNumberOfSignals();
                    uint32 s;
                    for (s = 0u; (s < numberOfSignals) && (ok); s++) {
                        uint32 size = 0u;
                        if (ok) {
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
                synchInputBrokersContainer.Insert(synchInputBroker);
                synchInputBroker->SetFunctionIndex(this, n);
            }
        }
    }
    if (ok) {
        ok = producerFound;
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "One and exactly one function shall write into this DataSourceI and none was found");
        }
    }
    //Create the synchInputBrokers
    for (n = 0u; (n < numberOfSyncGAMs) && (ok); n++) {
        ReferenceT<RealTimeThreadSynchBroker> synchInputBroker = synchInputBrokersContainer.Get(n);
        synchInputBrokers[n] = dynamic_cast<RealTimeThreadSynchBroker *>(synchInputBroker.operator ->());
        synchInputBrokers[n]->AllocateMemory(memory, memoryOffsets);
    }

    return ok;
}

bool RealTimeThreadSynchronisation::Synchronise() {
    bool ok = true;
    uint32 u;
    for (u = 0u; (u < numberOfSyncGAMs) && (ok); u++) {
        ok = synchInputBrokers[u]->AddSample();
    }

    return ok;
}

CLASS_REGISTER(RealTimeThreadSynchronisation, "1.0")
}

