/**
 * @file RealTimeThreadSynchBroker.cpp
 * @brief Source file for class RealTimeThreadSynchBroker
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
 * the class RealTimeThreadSynchBroker (public, protected, and private). Be aware that some
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
#include "RealTimeThreadSynchBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RealTimeThreadSynchBroker::RealTimeThreadSynchBroker() :
        MemoryMapInputBroker() {
    functionIdx = 0u;
    numberOfDataSourceSignals = 0u;
    signalMemory = NULL_PTR(char8 **);
    signalSize = NULL_PTR(uint32 *);
    dataSourceMemory = NULL_PTR(char8 *);
    dataSourceMemoryOffsets = NULL_PTR(uint32 *);
    numberOfSamples = 0u;
    currentSample = 0u;
    dataSource = NULL_PTR(DataSourceI *);
}

RealTimeThreadSynchBroker::~RealTimeThreadSynchBroker() {
    if (signalMemory != NULL_PTR(char8 **)) {
        uint32 s;
        for (s = 0u; s < numberOfDataSourceSignals; s++) {
            if (signalMemory[s] != NULL_PTR(char8 *)) {
                GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(signalMemory[s]));
            }
        }
        delete[] signalMemory;
    }
    if (signalSize != NULL_PTR(uint32 *)) {
        delete[] signalSize;
    }

    (void) synchSem.Post();
    (void) synchSem.Close();
}

void RealTimeThreadSynchBroker::SetFunctionIndex(DataSourceI *dataSourceIn, uint32 functionIdxIn) {
    dataSource = dataSourceIn;
    functionIdx = functionIdxIn;
    if (dataSource != NULL_PTR(DataSourceI *)) {
        (void) dataSource->GetFunctionName(functionIdx, gamName);
    }
}

bool RealTimeThreadSynchBroker::AllocateMemory(char8 *dataSourceMemoryIn, uint32 *dataSourceMemoryOffsetsIn) {
    bool ok = false;
    if (dataSource != NULL_PTR(DataSourceI *)) {
        dataSourceMemory = dataSourceMemoryIn;
        dataSourceMemoryOffsets = dataSourceMemoryOffsetsIn;
        numberOfDataSourceSignals = dataSource->GetNumberOfSignals();
        signalMemory = new char8*[numberOfDataSourceSignals];
        signalSize = new uint32[numberOfDataSourceSignals];
        uint32 numberOfFunctionSignals;
        uint32 s;
        for (s = 0u; s < numberOfDataSourceSignals; s++) {
            signalMemory[s] = NULL_PTR(char8 *);
        }
        ok = dataSource->GetFunctionNumberOfSignals(InputSignals, functionIdx, numberOfFunctionSignals);
        if (ok) {
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
                StreamString functionSignalAlias;
                if (ok) {
                    ok = dataSource->GetFunctionSignalAlias(InputSignals, functionIdx, s, functionSignalAlias);
                }
                uint32 signalIdx = 0u;
                if (ok) {
                    ok = dataSource->GetSignalIndex(signalIdx, functionSignalAlias.Buffer());
                }
                if (ok) {
                    uint32 signalSizeRead;
                    ok = dataSource->GetSignalByteSize(s, signalSizeRead);
                    signalSize[signalIdx] = signalSizeRead;
                }
                //The memory has to be reordered so that each signal can store the numberOfSamples required.
                if (ok) {
                    signalMemory[signalIdx] = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(signalSize[signalIdx] * numberOfSamples));
                }
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

bool RealTimeThreadSynchBroker::GetSignalMemoryBuffer(const uint32 signalIdx, void *&signalAddress) {
    bool ok = (signalMemory != NULL_PTR(char8 **));
    if (ok) {
        signalAddress = reinterpret_cast<void *>(&signalMemory[signalIdx][0u]);
    }
    return ok;
}

bool RealTimeThreadSynchBroker::AddSample() {
    bool ok = false;
    if ((dataSource != NULL_PTR(DataSourceI *)) && (signalMemory != NULL_PTR(char8 **)) && (dataSourceMemory != NULL_PTR(void *))) {
        uint32 s;
        for (s = 0u; s < numberOfDataSourceSignals; s++) {
            if (signalMemory[s] != NULL_PTR(char8 *)) {
                ok = MemoryOperationsHelper::Copy(&signalMemory[s][currentSample * signalSize[s]], &dataSourceMemory[dataSourceMemoryOffsets[s]], signalSize[s]);
            }
        }
    }
    currentSample++;
    if (currentSample == numberOfSamples) {
        currentSample = 0u;
        ok = synchSem.Post();
    }
    return ok;
}

bool RealTimeThreadSynchBroker::Wait() {
    bool ok = (synchSem.Wait() == ErrorManagement::NoError);
    if (ok) {
        ok = synchSem.Reset();
    }
    return ok;
}

const char8 * const RealTimeThreadSynchBroker::GetGAMName() {
    return gamName.Buffer();
}

bool RealTimeThreadSynchBroker::Execute() {
    bool ret = Wait();
    if (ret) {
        ret = MemoryMapInputBroker::Execute();
    }
    return ret;
}

CLASS_REGISTER(RealTimeThreadSynchBroker, "1.0")

}

