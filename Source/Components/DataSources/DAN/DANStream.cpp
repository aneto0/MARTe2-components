/**
 * @file DANStream.cpp
 * @brief Source file for class DANStream
 * @date Apr 4, 2017
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
 * the class DANStream (public, protected, and private). Be aware that some
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
#include "CompilerTypes.h"
#include "DANSource.h"
#include "DANStream.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

DANStream::DANStream(TypeDescriptor tdIn, StreamString baseNameIn, uint32 danBufferMultiplierIn, uint32 samplingFrequencyIn, uint32 numberOfSamplesIn) {
    td = tdIn;
    typeSize = td.numberOfBits / 8u;
    numberOfSignals = 0u;
    blockSize = 0u;
    useExternalAbsoluteTimingSignal = false;
    useExternalRelativeTimingSignal = false;
    signalIndexOffset = NULL_PTR(uint32 *);
    blockMemory = NULL_PTR(char8 *);
    blockInterleavedMemory = NULL_PTR(char8 *);
    danSource = NULL_PTR(dan_Source);
    baseName = baseNameIn;
    danBufferMultiplier = danBufferMultiplierIn;
    samplingFrequency = samplingFrequencyIn;
    numberOfSamples = numberOfSamplesIn;
    periodNanos = 1e9 / samplingFrequency;
    absoluteStartTime = 0u;
    timeAbsoluteSignal = NULL_PTR(uint64 *);
    timeRelativeSignal = NULL_PTR(uint32 *);
    writeCounts = 0u;
    danSourceName = "";
}

DANStream::~DANStream() {
    if (signalIndexOffset != NULL_PTR(uint32 *)) {
        delete[] signalIndexOffset;
    }
    if ((DANSource::GetDANDataCore() != NULL_PTR(dan_DataCore)) && (danSource != NULL_PTR(dan_Source))) {
        dan_publisher_unpublishSource(DANSource::GetDANDataCore(), danSource);
    }
}

TypeDescriptor DANStream::GetType() {
    return td;
}

uint32 DANStream::GetSamplingFrequency() {
    return samplingFrequency;
}

uint32 DANStream::GetDANBufferMultiplier() {
    return danBufferMultiplier;
}

uint32 DANStream::GetNumberOfSamples() {
    return numberOfSamples;
}

void DANStream::Reset() {
    writeCounts = 0u;
}

void DANStream::SetAbsoluteStartTime(uint64 absoluteStartTimeIn) {
    absoluteStartTime = absoluteStartTimeIn;
}

void DANStream::SetAbsoluteTimeSignal(uint64 *timeAbsoluteSignalIn) {
    timeAbsoluteSignal = timeAbsoluteSignalIn;
    if (timeAbsoluteSignal != NULL_PTR(uint64 *)) {
        useExternalAbsoluteTimingSignal = true;
    }
}

void DANStream::SetRelativeTimeSignal(uint32 *timeRelativeSignalIn) {
    timeRelativeSignal = timeRelativeSignalIn;
    if (timeRelativeSignal != NULL_PTR(uint32 *)) {
        useExternalRelativeTimingSignal = true;
    }
}

bool DANStream::PutData() {
    bool ok = true;
    uint64 timeStamp = 0u;
    if (useExternalAbsoluteTimingSignal) {
        timeStamp = *timeAbsoluteSignal;
    }
    else if (useExternalRelativeTimingSignal) {
        //Time in nanoseconds
        uint64 timeRelativeSignalNanos = static_cast<uint64>(*timeRelativeSignal);
        timeRelativeSignalNanos *= 1000u;
        timeStamp = absoluteStartTime;
        timeStamp += timeRelativeSignalNanos;
    }
    else {
        timeStamp = absoluteStartTime;
        timeStamp += writeCounts * static_cast<uint64>(numberOfSamples) * periodNanos;
        writeCounts++;
    }
    if ((blockInterleavedMemory != NULL_PTR(char8 *)) && (blockMemory != NULL_PTR(char8 *))) {
        uint32 s;
        uint32 z;
        //Interleave the memory data
        for (s = 0u; (s < numberOfSignals) && (ok); s++) {
            for (z = 0u; (z < numberOfSamples) && (ok); z++) {
                char8 *src = &blockMemory[s * numberOfSamples * typeSize + z * typeSize];
                char8 *dest = &blockInterleavedMemory[s * typeSize + (z * numberOfSignals * typeSize)];
                ok = MemoryOperationsHelper::Copy(dest, src, typeSize);
            }
        }
        if (ok) {
            //Check for >= 0 as true means == 1 but on CCS 6.0 true will be == 0
            ok = (dan_publisher_putDataBlock(danSource, timeStamp, blockInterleavedMemory, blockSize, NULL_PTR(char8 *)) >= 0);
        }
    }
    else {
        ok = false;
    }
    return ok;
}

bool DANStream::OpenStream() {
    bool ok = (dan_publisher_openStream(danSource, static_cast<float64>(samplingFrequency), 0u) == 0u);
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to dan_publisher_openStream for %s", danSourceName.Buffer());
    }
    return ok;
}

bool DANStream::CloseStream() {
    return (dan_publisher_closeStream(danSource) == 0u);
}

void DANStream::Finalise() {
    blockSize = numberOfSignals * typeSize * numberOfSamples;
    blockMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(blockSize));
    blockInterleavedMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(blockSize));
    danSourceName.Seek(0LLU);
    danSourceName.Printf("%s_%s", baseName.Buffer(), TypeDescriptor::GetTypeNameFromTypeDescriptor(td));
    danSourceName.Seek(0LLU);
    uint32 danBufferSize = blockSize * danBufferMultiplier;
    danSource = dan_publisher_publishSource_withDAQBuffer(DANSource::GetDANDataCore(), danSourceName.Buffer(), danBufferSize);
}

void DANStream::AddSignal(uint32 signalIdx) {
    uint32* newSignalIndexOffset = new uint32[numberOfSignals + 1u];
    uint32 i;
    for (i = 0u; i < numberOfSignals; i++) {
        newSignalIndexOffset[i] = signalIndexOffset[i];
    }
    if (numberOfSignals > 0u) {
        delete[] signalIndexOffset;
    }
    signalIndexOffset = newSignalIndexOffset;
    signalIndexOffset[numberOfSignals] = signalIdx;
    numberOfSignals++;
}

bool DANStream::GetSignalMemoryBuffer(const uint32 signalIdx, void*& signalAddress) {
    uint32 s;
    bool found = false;
    for (s = 0u; (s < numberOfSignals) && (!found); s++) {
        found = (signalIndexOffset[s] == signalIdx);
        if (found) {
            if (blockMemory != NULL_PTR(char8 *)) {
                char8* memSignalAddress = &blockMemory[s * typeSize * numberOfSamples];
                signalAddress = reinterpret_cast<void *&>(memSignalAddress);
            }
            else {
                found = false;
            }
        }
    }
    return found;
}
}
