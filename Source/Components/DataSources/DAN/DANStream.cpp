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
#include <tcn.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "DANAPI.h"
#include "DANSource.h"
#include "DANStream.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

DANStream::DANStream(const TypeDescriptor & tdIn, const StreamString baseNameIn, const uint32 danBufferMultiplierIn, const float64 samplingFrequencyIn, const uint32 numberOfSamplesIn, const bool interleaveIn) {
    td = tdIn;
    typeSize = static_cast<uint32>(td.numberOfBits) / 8u;
    numberOfSignals = 0u;
    blockSize = 0u;
    useExternalAbsoluteTimingSignal = false;
    useExternalRelativeTimingSignal = false;
    signalIndexOffset = NULL_PTR(uint32 *);
    blockMemory = NULL_PTR(char8 *);
    blockInterleavedMemory = NULL_PTR(char8 *);
    danSource = NULL_PTR(void *);
    baseName = baseNameIn;
    danBufferMultiplier = danBufferMultiplierIn;
    samplingFrequency = samplingFrequencyIn;
    numberOfSamples = numberOfSamplesIn;
    float64 periodNanosF = (1e9 / samplingFrequency);
    periodNanos = static_cast<uint64>(periodNanosF);
    absoluteStartTime = 0u;
    timeAbsoluteSignal = NULL_PTR(uint64 *);
    timeRelativeSignal = NULL_PTR(uint32 *);
    writeCounts = 0u;
    danSourceName = "";
    interleave = interleaveIn;
}

/*lint -e{1551} the destructor must guarantee that the DANSource is unpublished at the of the object life-cycle. The internal buffering memory is also cleaned in this function.*/
DANStream::~DANStream() {
    if (signalIndexOffset != NULL_PTR(uint32 *)) {
        delete[] signalIndexOffset;
    }
    if (danSource != NULL_PTR(void *)) {
        DANAPI::UnpublishSource(danSource);
    }
    if (blockMemory != NULL_PTR(char8 *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(blockMemory));
    }
    if (blockInterleavedMemory != NULL_PTR(char8 *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(blockInterleavedMemory));
    }
    /*lint -e{1740} the pointer danSource is cleaned by the dan_publisher_unpublishSource the pointers timeRelativeSignals and timeAbsoluteSignal are cleaned by the DANSource*/
}

TypeDescriptor DANStream::GetType() const {
    return td;
}

uint64 DANStream::GetPeriodNanos() const {
    return periodNanos;
}

uint32 DANStream::GetDANBufferMultiplier() const {
    return danBufferMultiplier;
}

uint32 DANStream::GetNumberOfSamples() const {
    return numberOfSamples;
}

void DANStream::Reset() {
    writeCounts = 0u;
}

void DANStream::SetAbsoluteStartTime(const uint64 absoluteStartTimeIn) {
    absoluteStartTime = absoluteStartTimeIn;
}

void DANStream::SetAbsoluteTimeSignal(uint64 * const timeAbsoluteSignalIn) {
    timeAbsoluteSignal = timeAbsoluteSignalIn;
    if (timeAbsoluteSignal != NULL_PTR(uint64 *)) {
        useExternalAbsoluteTimingSignal = true;
    }
}

void DANStream::SetRelativeTimeSignal(uint32 * const timeRelativeSignalIn) {
    timeRelativeSignal = timeRelativeSignalIn;
    if (timeRelativeSignal != NULL_PTR(uint32 *)) {
        useExternalRelativeTimingSignal = true;
    }
}

bool DANStream::PutData() {
    bool ok = true;
    uint64 timeStamp = 0u;
    if (useExternalAbsoluteTimingSignal) {
        /*lint -e{613} timeAbsoluteSignal cannot be NULL as otherwise useExternalAbsoluteTimingSignal=false*/
        timeStamp = *timeAbsoluteSignal;
    }
    else if (useExternalRelativeTimingSignal) {
        //Time in nanoseconds
        /*lint -e{613} timeRelativeSignal cannot be NULL as otherwise useExternalRelativeTimingSignal=false*/
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
        char8 *src = NULL_PTR(char8 *);
        char8 *dest = NULL_PTR(char8 *);
        if (interleave) {
            uint32 s;
            uint32 z;
            //Interleave the memory data
            for (s = 0u; (s < numberOfSignals) && (ok); s++) {
                for (z = 0u; (z < numberOfSamples) && (ok); z++) {
                    uint32 blockMemoryIdx = s * numberOfSamples * typeSize;
                    blockMemoryIdx += (z * typeSize);
                    src = &blockMemory[blockMemoryIdx];

                    uint32 blockInterleavedMemoryIdx = s * typeSize;
                    blockInterleavedMemoryIdx += (z * numberOfSignals * typeSize);
                    dest = &blockInterleavedMemory[blockInterleavedMemoryIdx];
                    ok = MemoryOperationsHelper::Copy(dest, src, typeSize);
                }
            }
        }
        else {
            src = &blockMemory[0u];
            dest = &blockInterleavedMemory[0u];
            ok = MemoryOperationsHelper::Copy(dest, src, blockSize);
        }
        if (ok) {
            //Check for >= 0 as true means == 1 but on CCS 6.0 true will be == 0
            ok = DANAPI::PutDataBlock(danSource, timeStamp, blockInterleavedMemory, blockSize);
        }
    }
    else {
        ok = false;
    }
    return ok;
}

bool DANStream::OpenStream() {
    bool ok = DANAPI::OpenStream(danSource, samplingFrequency);
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to dan_publisher_openStream for %s", danSourceName.Buffer());
    }
    return ok;
}

bool DANStream::CloseStream() {
    return DANAPI::CloseStream(danSource);
}

void DANStream::Finalise() {
    blockSize = numberOfSignals * typeSize * numberOfSamples;
    blockMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(blockSize));
    blockInterleavedMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(blockSize));
    (void) danSourceName.Seek(0LLU);
    (void) danSourceName.Printf("%s_%s", baseName.Buffer(), TypeDescriptor::GetTypeNameFromTypeDescriptor(td));
    (void) danSourceName.Seek(0LLU);
    uint64 danBufferSize = static_cast<uint64>(blockSize);
    danBufferSize *= static_cast<uint64>(danBufferMultiplier);
    danSource = DANAPI::PublishSource(danSourceName.Buffer(), danBufferSize);
}

void DANStream::AddSignal(const uint32 signalIdx) {
    uint32 numberOfSignalsP1 = (numberOfSignals + 1u);
    uint32 *newSignalIndexOffset = new uint32[numberOfSignalsP1];
    uint32 i;
    for (i = 0u; i < numberOfSignals; i++) {
        /*lint -e{613} signalIndexOffset cannot be NULL as numberOfSignals is initialised to zero in the constructor*/
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
        /*lint -e{613} signalIndexOffset cannot be NULL as numberOfSignals is initialised to zero in the constructor*/
        found = (signalIndexOffset[s] == signalIdx);
        if (found) {
            if (blockMemory != NULL_PTR(char8 *)) {
                uint32 memSignalAddressIdx = (s * typeSize * numberOfSamples);
                char8* memSignalAddress = &blockMemory[memSignalAddressIdx];
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
