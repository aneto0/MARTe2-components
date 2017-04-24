/**
 * @file RealTimeThreadSynchBuffer.cpp
 * @brief Source file for class RealTimeThreadSynchBuffer
 * @date Apr 24, 2017
 * @author aneto
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
 * the class RealTimeThreadSynchBuffer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "RealTimeThreadSynchBuffer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RealTimeThreadSynchBuffer::RealTimeThreadSynchBuffer(DataSourceI *dataSourceIn, uint32 functionIdxIn) {
    numberOfSignals = 0u;
    functionIdx = functionIdxIn;
    dataSource = dataSourceIn;
    memory = NULL_PTR(char8 *);
    memoryOffsets = NULL_PTR(uint32 *);
    numberOfSamples = 0u;
    currentSample = 0u;
    sampleMemorySize = 0u;
}

RealTimeThreadSynchBuffer::~RealTimeThreadSynchBuffer() {
    if (memory != NULL_PTR(void **)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(memory));
    }
    if (memoryOffsets != NULL_PTR(uint32 *)) {
        delete[] memoryOffsets;
    }
    (void) synchSem.Post();
    (void) synchSem.Close();
}

bool RealTimeThreadSynchBuffer::AllocateMemory() {
    bool ok = false;
    if (dataSource != NULL_PTR(DataSourceI *)) {
        uint32 numberOfFunctionSignals;
        ok = dataSource->GetFunctionNumberOfSignals(InputSignals, functionIdx, numberOfFunctionSignals);
        if (ok) {
            uint32 s;
            for (s = 0u; (s < numberOfFunctionSignals) && (ok); s++) {
                uint32 numberOfSamplesRead;
                ok = dataSource->GetFunctionSignalSamples(InputSignals, functionIdx, s, numberOfSamplesRead);
                if (numberOfSamples == 0u) {
                    numberOfSamples = numberOfSamplesRead;
                }
                if (ok) {
                    ok = (numberOfSamples == numberOfSamplesRead);
                    if (!ok) {
                        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "The number of samples shall be the same for all signals (%d != %d)", numberOfSamples, numberOfSamplesRead);
                    }
                }
            }
        }
        if (ok) {
            memoryOffsets = new uint32[numberOfFunctionSignals];
            uint32 s;
            for (s = 0u; (s < numberOfFunctionSignals) && (ok); s++) {
                StreamString signalAlias;
                ok = dataSource->GetFunctionSignalAlias(InputSignals, functionIdx, s, signalAlias);
                uint32 signalIdx = 0u;
                if (ok) {
                    ok = dataSource->GetSignalIndex(signalIdx, signalAlias.Buffer());
                }
                uint32 size = 0u;
                if (ok) {
                    ok = dataSource->GetSignalByteSize(signalIdx, size);
                    memoryOffsets[s] = sampleMemorySize;
                    sampleMemorySize += size;
                }
            }
            if (ok) {
                memory[s] = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(sampleMemorySize * numberOfSamples);
            }
        }
    }
    if (ok) {
        ok = synchSem.Create();
    }
    if (ok) {
        ok = synchSem.Reset();
    }
    return ok;
}

bool RealTimeThreadSynchBuffer::GetSignalMemoryBuffer(const uint32 signalIdx, void *&signalAddress) {
    bool ok = false;
    if ((dataSource != NULL_PTR(DataSourceI *)) && (memory != NULL_PTR(void *))) {
        StreamString signalName;
        ok = dataSource->GetSignalName(signalIdx, signalName);
        uint32 idx;
        if (ok) {
            ok = dataSource->GetFunctionSignalIndex(InputSignals, functionIdx, idx, signalName.Buffer());
        }
        if (ok) {
            signalAddress = reinterpret_cast<void *&>(&memory[memoryOffsets[idx]]);
        }
    }
    return ok;
}

bool RealTimeThreadSynchBuffer::AddSample(void *sampleToAdd) {
    bool ok = false;
    if ((dataSource != NULL_PTR(DataSourceI *)) && (memory != NULL_PTR(void *)) && (sampleToAdd != NULL_PTR(void *))) {
        ok = MemoryOperationsHelper::Copy(&memory[currentSample * sampleMemorySize], sampleToAdd, sampleMemorySize);
    }
    currentSample++;
    if (currentSample == numberOfSamples) {
        currentSample = 0u;
        ok = synchSem.Post();
    }
    return ok;
}

bool RealTimeThreadSynchBuffer::Wait() {
    bool ok = (synchSem.Wait() == ErrorManagement::NoError);
    if (ok) {
        ok = synchSem.Reset();
    }
    return ok;
}

}

