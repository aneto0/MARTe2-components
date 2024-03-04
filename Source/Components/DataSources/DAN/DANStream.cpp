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

DANStream::DANStream(const StreamString typeNameIn,
                     const StreamString baseNameIn,
                     const uint32 danBufferMultiplierIn,
                     const float64 samplingFrequencyIn,
                     const uint32 numberOfSamplesIn,
                     const bool interleaveIn) {
    typeName = typeNameIn;
    isStruct = (TypeDescriptor::GetTypeDescriptorFromTypeName(typeName.Buffer()) == InvalidType);
    typeSize = 0u;
    numberOfSignals = 0u;
    blockSize = 0u;
    useExternalAbsoluteTimingSignal = false;
    useExternalRelativeTimingSignal = false;
    signalIndexMap = NULL_PTR(uint32*);
    blockMemory = NULL_PTR(char8*);
    blockInterleavedMemory = NULL_PTR(char8*);
    danSource = NULL_PTR(void*);
    baseName = baseNameIn;
    danBufferMultiplier = danBufferMultiplierIn;
    samplingFrequency = samplingFrequencyIn;
    numberOfSamples = numberOfSamplesIn;
    float64 periodNanosF = (1e9 / samplingFrequency);
    periodNanos = static_cast<uint64>(periodNanosF);
    absoluteStartTime = 0u;
    timeAbsoluteSignal = NULL_PTR(uint64*);
    timeRelativeSignal = NULL_PTR(uint32*);
    writeCounts = 0u;
    danSourceName = "";
    interleave = interleaveIn;
    nFieldsAllocated = 0u;
    numberOfFields = 0u;
    fieldInStructOffset = NULL_PTR(uint32*);
    totalNumberOfFields = 0u;
    fieldIndexMap = NULL_PTR(uint32*);
    structIdx = NULL_PTR(uint32*);
    fieldIdx = NULL_PTR(uint32*);
    totalNumberOfFieldsAllocated = 0u;
    types = NULL_PTR(TypeDescriptor*);
    numberOfElements = NULL_PTR(uint32*);
    numberOfDimensions = NULL_PTR(uint8*);
    units = NULL_PTR(StreamString*);
    descriptions = NULL_PTR(StreamString*);
}

/*lint -e{1551} the destructor must guarantee that the DANSource is unpublished at the of the object life-cycle. The internal buffering memory is also cleaned in this function.*/
DANStream::~DANStream() {
    if (signalIndexMap != NULL_PTR(uint32*)) {
        delete[] signalIndexMap;
    }
    if (fieldInStructOffset != NULL_PTR(uint32*)) {
        delete[] fieldInStructOffset;
    }
    if (fieldIndexMap != NULL_PTR(uint32*)) {
        delete[] fieldIndexMap;
    }
    if (structIdx != NULL_PTR(uint32*)) {
        delete[] structIdx;
    }
    if (fieldIdx != NULL_PTR(uint32*)) {
        delete[] fieldIdx;
    }
    if (types != NULL_PTR(TypeDescriptor*)) {
        delete[] types;
    }
    if (numberOfElements != NULL_PTR(uint32*)) {
        delete[] numberOfElements;
    }
    if (numberOfDimensions != NULL_PTR(uint8*)) {
        delete[] numberOfDimensions;
    }
    if (units != NULL_PTR(StreamString*)) {
        delete[] units;
    }
    if (descriptions != NULL_PTR(StreamString*)) {
        delete[] descriptions;
    }

    if (danSource != NULL_PTR(void*)) {
        DANAPI::UnpublishSource(danSource);
    }
    if (blockMemory != NULL_PTR(char8*)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void*&>(blockMemory));
    }
    if (blockInterleavedMemory != NULL_PTR(char8*)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void*&>(blockInterleavedMemory));
    }
    /*lint -e{1740} the pointer danSource is cleaned by the dan_publisher_unpublishSource the pointers timeRelativeSignals and timeAbsoluteSignal are cleaned by the DANSource*/
}

void DANStream::GetType(StreamString &typeNameOut) const {
    typeNameOut = typeName;
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

void DANStream::SetAbsoluteTimeSignal(uint64 *const timeAbsoluteSignalIn) {
    timeAbsoluteSignal = timeAbsoluteSignalIn;
    if (timeAbsoluteSignal != NULL_PTR(uint64*)) {
        useExternalAbsoluteTimingSignal = true;
    }
}

void DANStream::SetRelativeTimeSignal(uint32 *const timeRelativeSignalIn) {
    timeRelativeSignal = timeRelativeSignalIn;
    if (timeRelativeSignal != NULL_PTR(uint32*)) {
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
    if ((blockInterleavedMemory != NULL_PTR(char8*)) && (blockMemory != NULL_PTR(char8*))) {
        char8 *src = NULL_PTR(char8*);
        char8 *dest = NULL_PTR(char8*);
        if (interleave) {
            //Interleave the memory data
            for (uint32 s = 0u; (s < numberOfSignals) && (ok); s++) {
                for (uint32 h = 0u; (h < numberOfFields) && (ok); h++) {
                    uint32 size = static_cast<uint32>(types[h].numberOfBits) / 8u;
                    for (uint32 z = 0u; (z < numberOfSamples) && (ok); z++) {
                        //interleave within the structure
                        uint32 blockMemoryIdx = (s * numberOfSamples + typeSize) + (fieldInStructOffset[h] * numberOfSamples) + (z * size);
                        src = &blockMemory[blockMemoryIdx];

                        uint32 blockInterleavedMemoryIdx = (s * typeSize) + (z * numberOfSignals * typeSize) + fieldInStructOffset[h];
                        dest = &blockInterleavedMemory[blockInterleavedMemoryIdx];
                        ok = MemoryOperationsHelper::Copy(dest, src, size);
                    }
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

bool DANStream::Finalise() {
    bool ret = true;
    blockSize = numberOfSignals * typeSize * numberOfSamples;
    blockMemory = reinterpret_cast<char8*>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(blockSize));
    blockInterleavedMemory = reinterpret_cast<char8*>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(blockSize));
    (void) danSourceName.Seek(0LLU);
    (void) danSourceName.Printf("%s_%s", baseName.Buffer(), typeName.Buffer());
    (void) danSourceName.Seek(0LLU);
    uint64 danBufferSize = static_cast<uint64>(blockSize);
    danBufferSize *= static_cast<uint64>(danBufferMultiplier);
    danSource = DANAPI::PublishSource(danSourceName.Buffer(), danBufferSize);
    if (isStruct) {
        int32 result = DANAPI::DeclareStruct(danSource, types, numberOfElements, numberOfDimensions, units, descriptions, numberOfFields);
        ret = (result >= 0);
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Incorrect setting of composite type fields from I&C");
        }
    }
    return ret;
}

void DANStream::AddSignal(const uint32 signalIdx) {
    uint32 numberOfSignalsP1 = (numberOfSignals + 1u);
    uint32 *newSignalIndexOffset = new uint32[numberOfSignalsP1];
    uint32 i;
    for (i = 0u; i < numberOfSignals; i++) {
        /*lint -e{613} signalIndexMap cannot be NULL as numberOfSignals is initialised to zero in the constructor*/
        newSignalIndexOffset[i] = signalIndexMap[i];
    }
    if (numberOfSignals > 0u) {
        delete[] signalIndexMap;
    }
    signalIndexMap = newSignalIndexOffset;
    signalIndexMap[numberOfSignals] = signalIdx;
    numberOfSignals++;
    numberOfFields = 0u;
    typeSize = 0u;
}

bool DANStream::AddToStructure(const uint32 fieldIdxIn,
                               const TypeDescriptor typeDesc,
                               const uint32 numberOfElementsIn,
                               const uint8 numberOfDimensionsIn,
                               const StreamString unitIn,
                               const StreamString descriptionIn) {
    bool ret = (numberOfSignals > 0u);
    if (ret) {
        uint32 fieldSize = (static_cast<uint32>(typeDesc.numberOfBits) / 8u) * numberOfElementsIn;
        uint32 numberOfFieldsP1 = (numberOfFields + 1u);
        if (numberOfFieldsP1 > nFieldsAllocated) {
            nFieldsAllocated += 8u;
            uint32 *newfieldInStructOffset = new uint32[nFieldsAllocated];
            TypeDescriptor *newTypes = new TypeDescriptor[nFieldsAllocated];
            uint32 *newNumberOfElements = new uint32[nFieldsAllocated];
            uint8 *newNumberOfDimensions = new uint8[nFieldsAllocated];
            StreamString *newUnits = new StreamString[nFieldsAllocated];
            StreamString *newDesc = new StreamString[nFieldsAllocated];
            for (uint32 i = 0u; i < nFieldsAllocated; i++) {
                if (i < numberOfFields) {
                    newfieldInStructOffset[i] = fieldInStructOffset[i];
                    newTypes[i] = types[i];
                    newNumberOfElements[i] = numberOfElements[i];
                    newNumberOfDimensions[i] = numberOfDimensions[i];
                    newUnits[i] = units[i];
                    newDesc[i] = descriptions[i];
                }
                else {
                    newfieldInStructOffset[i] = 0u;
                    newTypes[i] = InvalidType;
                    newNumberOfElements[i] = 0u;
                    newNumberOfDimensions[i] = 0u;
                    newUnits[i] = "Unknown";
                    newDesc[i] = "";
                }
            }
            if (numberOfFields > 0u) {
                delete[] fieldInStructOffset;
                delete[] types;
                delete[] numberOfElements;
                delete[] numberOfDimensions;
            }
            fieldInStructOffset = newfieldInStructOffset;
            types = newTypes;
            numberOfElements = newNumberOfElements;
            numberOfDimensions = newNumberOfDimensions;
            units = newUnits;
            descriptions = newDesc;
        }
        if (types[numberOfFields] != InvalidType) {
            ret = (types[numberOfFields] == typeDesc);
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Type mismatch for signal %d in struct %d. Found %s, Expected %s", fieldIdxIn,
                                    signalIndexMap[numberOfSignals - 1u], TypeDescriptor::GetTypeNameFromTypeDescriptor(types[numberOfFields]),
                                    TypeDescriptor::GetTypeNameFromTypeDescriptor(types[numberOfFields]));

            }
            if (ret) {
                ret = (numberOfElements[numberOfFields] == numberOfElementsIn);
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Number of elements mismatch for signal %d in struct %d. Found %d. Expected %d",
                                        fieldIdxIn, signalIndexMap[numberOfSignals - 1u], numberOfElements[numberOfFields], numberOfElementsIn);
                }
            }

            if (ret) {
                ret = (numberOfDimensions[numberOfFields] == numberOfDimensionsIn);
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Number of elements mismatch for signal %d in struct %d. Found %d. Expected %d",
                                        fieldIdxIn, signalIndexMap[numberOfSignals - 1u], numberOfDimensions[numberOfFields], numberOfDimensionsIn);
                }
            }

        }
        else {
            fieldInStructOffset[numberOfFields] = typeSize;
            types[numberOfFields] = typeDesc;
            numberOfElements[numberOfFields] = numberOfElementsIn;
            numberOfDimensions[numberOfFields] = numberOfElementsIn;
            units[numberOfFields] = unitIn;
            descriptions[numberOfFields] = descriptionIn;
        }
        typeSize += fieldSize;
        numberOfFields++;

        if (totalNumberOfFields > totalNumberOfFieldsAllocated) {
            totalNumberOfFieldsAllocated += 8u;
            uint32 *newFieldIndexOffset = new uint32[totalNumberOfFieldsAllocated];
            uint32 *newStructIdx = new uint32[totalNumberOfFieldsAllocated];
            uint32 *newFieldIdx = new uint32[totalNumberOfFieldsAllocated];
            for (uint32 i = 0u; i < totalNumberOfFields; i++) {
                newFieldIndexOffset[i] = fieldIndexMap[i];
                newStructIdx[i] = structIdx[i];
                newFieldIdx[i] = fieldIdx[i];
            }
            if (totalNumberOfFields > 0u) {
                delete[] fieldIndexMap;
                delete[] structIdx;
                delete[] fieldIdx;
            }
            fieldIndexMap = newFieldIndexOffset;
            structIdx = newStructIdx;
            fieldIdx = newFieldIdx;
        }
        //mapping
        fieldIndexMap[totalNumberOfFields] = fieldIdxIn;
        structIdx[totalNumberOfFields] = signalIndexMap[numberOfSignals - 1u];
        fieldIdx[totalNumberOfFields] = fieldInStructOffset[numberOfFields - 1u];
        totalNumberOfFields++;
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Number of Signals == 0 in AddToStructure. Did you call AddSignal before?");
    }
    return ret;
}

bool DANStream::GetSignalMemoryBuffer(const uint32 signalIdx,
                                      void *&signalAddress) {

    bool found = false;
    for (uint32 i = 0u; (i < numberOfSignals) && (!found); i++) {
        found = (fieldIndexMap[i] == signalIdx);
        if (found) {
            uint32 structFieldIdx = structIdx[i];
            uint32 fieldOffset = fieldInStructOffset[i];
            if (blockMemory != NULL_PTR(char8*)) {
                for (uint32 n = 0u; n < numberOfSamples; n++) {
                    uint32 memSignalAddressIdx = ((typeSize * structFieldIdx) + fieldOffset) * numberOfSamples;
                    char8 *memSignalAddress = &blockMemory[memSignalAddressIdx];
                    signalAddress = reinterpret_cast<void*&>(memSignalAddress);
                }
            }
            else {
                found = false;
            }
        }
    }
    return found;
}

}
