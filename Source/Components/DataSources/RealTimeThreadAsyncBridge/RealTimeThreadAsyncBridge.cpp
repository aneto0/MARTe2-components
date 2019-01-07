/**
 * @file RealTimeThreadAsyncBridge.cpp
 * @brief Source file for class RealTimeThreadAsyncBridge
 * @date 13/04/2018
 * @author Giuseppe Ferro
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
 * the class RealTimeThreadAsyncBridge (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "Atomic.h"
#include "RealTimeThreadAsyncBridge.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//If all the buffer are busy by readers or other writers, the writer returns without doing anything jumping a cycle
//Trigger=1 for the writers
//Last refreshed mechanism based on an incrementing counter... write on the oldest and read on the newest
namespace MARTe {

RealTimeThreadAsyncBridge::RealTimeThreadAsyncBridge() :
        MemoryDataSourceI(), resetTimeout() {
    spinlocksRead = NULL_PTR(volatile int32 *);
    spinlocksWrite = NULL_PTR(FastPollingMutexSem *);
    whatIsNewestCounter = NULL_PTR(uint32 *);
    whatIsNewestGlobCounter = NULL_PTR(uint32 *);
    writeOp = NULL_PTR(uint32 *);
    writeOpCounter = NULL_PTR(uint32 *);
    offsetStore = NULL_PTR(uint32 *);
}

RealTimeThreadAsyncBridge::~RealTimeThreadAsyncBridge() {
    if (spinlocksRead != NULL_PTR(volatile int32 *)) {
        delete[] spinlocksRead;
        spinlocksRead = NULL_PTR(volatile int32 *);
    }
    if (spinlocksWrite != NULL_PTR(FastPollingMutexSem *)) {
        delete[] spinlocksWrite;
        spinlocksWrite = NULL_PTR(FastPollingMutexSem *);
    }
    if (offsetStore != NULL_PTR(uint32 *)) {
        delete[] offsetStore;
        spinlocksWrite = NULL_PTR(FastPollingMutexSem *);
    }
    if (whatIsNewestCounter != NULL_PTR(uint32 *)) {
        delete[] whatIsNewestCounter;
        whatIsNewestCounter = NULL_PTR(uint32 *);
    }
    if (whatIsNewestGlobCounter != NULL_PTR(uint32 *)) {
        delete[] whatIsNewestGlobCounter;
        whatIsNewestGlobCounter = NULL_PTR(uint32 *);
    }
    if (writeOp != NULL_PTR(uint32 *)) {
        delete[] writeOp;
        writeOp = NULL_PTR(uint32 *);
    }
    if (writeOpCounter != NULL_PTR(uint32 *)) {
        delete[] writeOpCounter;
        writeOpCounter = NULL_PTR(uint32 *);
    }
}

bool RealTimeThreadAsyncBridge::Initialise(StructuredDataI &data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        ret = (numberOfBuffers <= 64u);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The maximum allowed numberOfBuffers is 64");
        }

        uint32 resetTimeoutT;
        if (!data.Read("ResetMSecTimeout", resetTimeoutT)) {
            resetTimeout = TTInfiniteWait;
        }
    }

    return ret;
}

/*lint -e{715} symbols not referenced.*/
/*lint -e{613} null pointer checked before.*/
bool RealTimeThreadAsyncBridge::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {

    bool ret = true;

    for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
        writeOp[i] = 0u;
        writeOpCounter[i] = 0u;

        uint32 numberOfStates = 0u;
        ret = GetSignalNumberOfStates(i, numberOfStates);
        for (uint32 j = 0u; (j < numberOfStates) && (ret); j++) {
            StreamString stateName;
            ret = GetSignalStateName(i, j, stateName);
            if (ret) {
                if (stateName == nextStateName) {
                    uint32 numberOfFunctions = GetNumberOfFunctions();

                    for (uint32 h = 0u; (h < numberOfFunctions) && (ret); h++) {
                        uint32 numberOfFunctionSignals = 0u;
                        ret = GetFunctionNumberOfSignals(OutputSignals, h, numberOfFunctionSignals);

                        for (uint32 k = 0u; (k < numberOfFunctionSignals) && (ret); k++) {
                            StreamString signalName;
                            ret = GetFunctionSignalAlias(OutputSignals, h, k, signalName);
                            uint32 signalIdx = 0u;
                            if (ret) {
                                ret = GetSignalIndex(signalIdx, signalName.Buffer());
                            }
                            if (signalIdx == i) {
                                uint32 numberOfByteOffsets = 0u;
                                if (ret) {
                                    ret = GetFunctionSignalNumberOfByteOffsets(OutputSignals, h, k, numberOfByteOffsets);
                                }
                                if (ret) {
                                    writeOp[i] += numberOfByteOffsets;
                                }
                            }
                        }

                    }
                }
            }
        }
        if (ret) {
            writeOpCounter[i] = writeOp[i];
        }
    }
    return true;
}

bool RealTimeThreadAsyncBridge::Synchronise() {

    return true;

}

/*lint -e{613} null pointer checked before.*/
bool RealTimeThreadAsyncBridge::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = MemoryDataSourceI::SetConfiguredDatabase(data);

    // check the number of samples is 1 for each signal
    if (ret) {
        uint32 numberOfFunctions = GetNumberOfFunctions();
        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
            uint32 numberOfFunctionSignals = 0u;
            ret = GetFunctionNumberOfSignals(InputSignals, i, numberOfFunctionSignals);
            for (uint32 j = 0u; (j < numberOfFunctionSignals) && (ret); j++) {
                uint32 samples = 0u;
                ret = GetFunctionSignalSamples(InputSignals, i, j, samples);
                if (ret) {
                    ret = (samples == 1u);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "This DataSource does not support a number of samples > 1");
                    }
                }
            }

            if (ret) {
                ret = GetFunctionNumberOfSignals(OutputSignals, i, numberOfFunctionSignals);
                for (uint32 j = 0u; (j < numberOfFunctionSignals) && (ret); j++) {
                    uint32 samples = 0u;
                    ret = GetFunctionSignalSamples(OutputSignals, i, j, samples);
                    if (ret) {
                        ret = (samples == 1u);
                        if (!ret) {
                            REPORT_ERROR(ErrorManagement::InitialisationError, "This DataSource does not support a number of samples > 1");
                        }
                    }
                }

            }
        }
    }

    if (ret) {
        if (spinlocksRead == NULL_PTR(volatile int32 *)) {
            uint32 index = (numberOfSignals * numberOfBuffers);
            spinlocksRead = new volatile int32[index];
            ret = (spinlocksRead != NULL_PTR(volatile int32 *));
        }
        if (ret) {
            if (spinlocksWrite == NULL) {
                uint32 index = (numberOfSignals * numberOfBuffers);
                spinlocksWrite = new FastPollingMutexSem[index];
                ret = (spinlocksWrite != NULL_PTR(FastPollingMutexSem *));
            }
        }
        if (ret) {

            if (whatIsNewestCounter == NULL) {
                uint32 index = (numberOfSignals * numberOfBuffers);
                whatIsNewestCounter = new uint32[index];
                ret = (whatIsNewestCounter != NULL_PTR(uint32 *));
            }
        }
        if (ret) {

            if (whatIsNewestGlobCounter == NULL) {
                whatIsNewestGlobCounter = new uint32[numberOfSignals];
                ret = (whatIsNewestGlobCounter != NULL_PTR(uint32 *));
            }
        }
        if (ret) {
            if (writeOp == NULL) {
                writeOp = new uint32[numberOfSignals];
                ret = (writeOp != NULL_PTR(uint32 *));
            }
        }
        if (ret) {
            if (writeOpCounter == NULL) {
                writeOpCounter = new uint32[numberOfSignals];
                ret = (writeOpCounter != NULL_PTR(uint32 *));
            }
        }
        if (ret) {
            if (offsetStore == NULL) {
                offsetStore = new uint32[numberOfSignals];
                ret = (offsetStore != NULL_PTR(uint32 *));
            }
        }
        if (ret) {
            uint32 numberOfElements = (numberOfSignals * numberOfBuffers);
            for (uint32 i = 0u; i < numberOfElements; i++) {
                spinlocksRead[i] = 0;
                spinlocksWrite[i].Create(false);
                whatIsNewestCounter[i] = 0u;
                if (i < numberOfSignals) {
                    whatIsNewestGlobCounter[i] = 0u;
                }
            }
        }

    }

    return ret;

}

/*lint -e{715} symbols not referenced.*/
/*lint -e{613} null pointer checked before.*/
bool RealTimeThreadAsyncBridge::GetInputOffset(const uint32 signalIdx, const uint32 numberOfSamples, uint32 &offset) {

    bool ok = false;
    uint64 checkedMask = 0ull;
    for (uint32 k = 0u; (k < numberOfBuffers) && (!ok); k++) {
        //check the last written
        uint32 bufferIdx = 0u;
        uint32 max = 0u;
        for (uint32 h = 0u; (h < numberOfBuffers); h++) {
            uint32 index = (signalIdx * numberOfBuffers) + h;
            if (spinlocksWrite[index].FastTryLock()) {
                uint32 tempMax = whatIsNewestCounter[index];
                spinlocksWrite[index].FastUnLock();
                if ((tempMax >= max) && (((1ull << h) & checkedMask) == 0u)) {
                    max = tempMax;
                    bufferIdx = h;
                }
            }
        }

        checkedMask |= (1ull << bufferIdx);
        ok = false;
        //try to lock the next buffer to write
        uint32 index = (signalIdx * numberOfBuffers) + bufferIdx;

        if (spinlocksWrite[index].FastTryLock()) {
            Atomic::Increment(&spinlocksRead[index]);
            spinlocksWrite[index].FastUnLock();
            offset = (signalSize[signalIdx] * bufferIdx);
            ok = true;
        }
    }

    return ok;
}

/*lint -e{715} symbols not referenced.*/
/*lint -e{613} null pointer checked before.*/
bool RealTimeThreadAsyncBridge::GetOutputOffset(const uint32 signalIdx, const uint32 numberOfSamples, uint32 &offset) {
    bool ok = false;

    if (writeOpCounter[signalIdx] < writeOp[signalIdx]) {
        offset = offsetStore[signalIdx];
        ok = true;
    }
    uint64 checkedMask = 0ull;
    for (uint32 k = 0u; (k < numberOfBuffers) && (!ok); k++) {
        //check the oldest written
        uint32 min = 0xFFFFFFFFu;
        uint32 bufferIdx = 0u;
        for (uint32 h = 0u; (h < numberOfBuffers); h++) {
            uint32 index = (signalIdx * numberOfBuffers) + h;
            uint32 tempMin = whatIsNewestCounter[index];
            if ((tempMin < min) && (((1ull << h) & checkedMask) == 0ull)) {
                min = tempMin;
                bufferIdx = h;
            }
        }
        checkedMask |= (1ull << bufferIdx);
        ok = false;
        //try to lock the next buffer to write
        uint32 index = (signalIdx * numberOfBuffers) + bufferIdx;
        if (spinlocksWrite[index].FastTryLock()) {
            if (spinlocksRead[index] == 0) {
                offset = (signalSize[signalIdx] * bufferIdx);
                ok = true;
            }
            else {
                spinlocksWrite[index].FastUnLock();
            }
        }
    }
    offsetStore[signalIdx] = offset;

    return ok;
}

/*lint -e{715} numberOfSamples not required for this implementation.*/
/*lint -e{613} null pointer checked before.*/
bool RealTimeThreadAsyncBridge::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples) {

    uint32 buffNumber = (offset / signalSize[signalIdx]);
    uint32 index = (signalIdx * numberOfBuffers) + buffNumber;
    Atomic::Decrement(&spinlocksRead[index]);
    return true;
}

/*lint -e{715} numberOfSamples not required for this implementation.*/
/*lint -e{613} null pointer checked before.*/
bool RealTimeThreadAsyncBridge::TerminateOutputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples) {
    writeOpCounter[signalIdx]--;
    if (writeOpCounter[signalIdx] == 0u) {
        uint32 buffNumber = (offset / signalSize[signalIdx]);

        uint32 index = (signalIdx * numberOfBuffers) + buffNumber;
        whatIsNewestGlobCounter[signalIdx]++;
        whatIsNewestCounter[index] = whatIsNewestGlobCounter[signalIdx];

        //overflow... subtract the smaller one
        if (whatIsNewestGlobCounter[signalIdx] == 0u) {

            uint32 min = 0xFFFFFFFFu;
            for (uint32 h = 0u; (h < numberOfBuffers); h++) {
                if (h != buffNumber) {
                    uint32 index1 = (signalIdx * numberOfBuffers) + h;
                    if (whatIsNewestCounter[index1] < min) {
                        min = whatIsNewestCounter[index1];
                    }
                }

            }

            for (uint32 h = 0u; (h < numberOfBuffers); h++) {
                uint32 index1 = (signalIdx * numberOfBuffers) + h;
                if (h != buffNumber) {
                    bool blocked = spinlocksWrite[index1].FastLock(resetTimeout);
                    if (blocked) {
                        whatIsNewestCounter[index1] -= min;
                        spinlocksWrite[index1].FastUnLock();
                    }
                }
            }

            whatIsNewestGlobCounter[signalIdx] -= min;
            whatIsNewestCounter[index] -= min;

        }

        writeOpCounter[signalIdx] = writeOp[signalIdx];
        spinlocksWrite[index].FastUnLock();
    }
    return true;
}

const char8 *RealTimeThreadAsyncBridge::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {

    const char8* brokerName = NULL_PTR(const char8 *);

    float32 frequency = 0.F;
    if (data.Read("Frequency", frequency)) {
        REPORT_ERROR(ErrorManagement::Warning, "Signal Frequency definition will be ignored.");
    }
    uint32 trigger = 0u;
    if (data.Read("Trigger", trigger)) {
        REPORT_ERROR(ErrorManagement::Warning, "Signal Trigger definition will be ignored.");
    }
    if (direction == InputSignals) {
        brokerName = "MemoryMapMultiBufferInputBroker";
    }
    if (direction == OutputSignals) {
        brokerName = "MemoryMapMultiBufferOutputBroker";
    }

    return brokerName;
}

CLASS_REGISTER(RealTimeThreadAsyncBridge, "1.0")
}
