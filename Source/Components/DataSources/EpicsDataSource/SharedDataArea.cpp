/**
 * @file EpicsDataSource.cpp
 * @brief Source file for class EpicsDataSource
 * @date 01/12/2016
 * @author Ivan Herrero Molina
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
 * the class EpicsDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

//#include <cstdio>
//#ifndef LINT
//#include <cstring>
//#endif
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Platform.h"
#include "SharedDataArea.h"
#include "Types.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

/**
 * @brief Calculates the size of a sigblock from its metadata.
 * @param[in] signalsCount The number of signals in the sigblock.
 * @param[in] signalsMetadata[] The metadata for the sigblocks's signals.
 * @pre 0 <= number of elements into signalsMetadata array < signalsCount
 * @post Result == sum of size for all elements into signalsMetadata array
 * @returns Result
 */
static SDA::size_type CalculateSizeOfSigblock(const SDA::uint32 signalsCount,
                                              const SDA::Signal::Metadata signalsMetadata[]) {
    SDA::size_type sigblockSize = 0u;
    for (SDA::uint32 i = 0u; i < signalsCount; i++) {
        sigblockSize += signalsMetadata[i].size;
    }
    return sigblockSize;
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace SDA {

SharedDataArea::SharedDataArea() :
        shm(SDA_NULL_PTR(Representation*)) {

}

SharedDataArea::SharedDataArea(SharedDataArea::Representation* const obj) :
        shm(obj) {
}

SharedDataArea::SigblockProducer* SharedDataArea::GetSigblockProducerInterface() {
    //TODO: Check if it has already a writer registered
    //try CAS of hasReader to true
    if (shm != NULL) {
        shm->hasWriter = true;
    }
    /*lint -e{1939} The SigblockProducer does not add members to Representation (read Representation's documentation)*/
    return reinterpret_cast<SigblockProducer*>(shm);
}

SharedDataArea::SigblockConsumer* SharedDataArea::GetSigblockConsumerInterface() {
    //TODO: Check if it has already a reader registered
    //try CAS of hasReader to true
    if (shm != NULL) {
        shm->hasReader = true;
    }
    /*lint -e{1939} The SigblockProducer does not add members to Representation (read Representation's documentation)*/
    return reinterpret_cast<SigblockConsumer*>(shm);
}

void SharedDataArea::Representation::FillPreHeader(const SDA::size_type sizeOfHeader) {
    hasReader = false;
    hasWriter = false;
    droppedWrites = 0u;
    offsetOfHeader = 0u;
    offsetOfItems = sizeOfHeader;
}

void SharedDataArea::Representation::FillHeader(const SDA::uint32 signalsCount,
                                                const SDA::Signal::Metadata signalsMetadata[]) {
    SDA::Sigblock::Metadata* header = Header();
    header->Init(signalsCount, signalsMetadata);
}

void SharedDataArea::Representation::FillItems(const SDA::size_type sizeOfSigblock) {
    SDA::SigblockDoubleBuffer* items = Items();
    items->Init(sizeOfSigblock);
}

bool SharedDataArea::BuildSharedDataAreaForMARTe(SharedDataArea& sda,
                                                 const SDA::char8* const name,
                                                 const SDA::uint32 signalsCount,
                                                 const SDA::Signal::Metadata signalsMetadata[]) {
    bool ok;
    SDA::size_type sizeOfSigblock = CalculateSizeOfSigblock(signalsCount, signalsMetadata);
    SDA::size_type sizeOfHeader = SDA::Sigblock::Metadata::SizeOf(signalsCount);
    SDA::size_type sizeOfItems = SDA::SigblockDoubleBuffer::SizeOf(sizeOfSigblock);
    SDA::size_type totalSize = (sizeof(SharedDataArea::Representation) + sizeOfHeader + sizeOfItems);
    Representation* tmp_shm_ptr = SDA_NULL_PTR(Representation*);

    void* raw_shm_ptr = SDA::Platform::MakeShm(name, totalSize);
    if (raw_shm_ptr == SDA_NULL_PTR(void*)) {
        ok = false;
    }
    else {
        tmp_shm_ptr = static_cast<SharedDataArea::Representation*>(raw_shm_ptr);
        tmp_shm_ptr->FillPreHeader(sizeOfHeader);
        tmp_shm_ptr->FillHeader(signalsCount, signalsMetadata);
        tmp_shm_ptr->FillItems(sizeOfSigblock);
        sda.shm = tmp_shm_ptr;
        ok = true;
    }
    return ok;
}

bool SharedDataArea::BuildSharedDataAreaForEPICS(SharedDataArea& sda, const SDA::char8* const name) {
    bool ok;
    void* raw_shm_ptr;
    Representation* tmp_shm_ptr = SDA_NULL_PTR(Representation*);
    raw_shm_ptr = SDA::Platform::JoinShm(name);
    if (raw_shm_ptr == SDA_NULL_PTR(void*)) {
        ok = false;
    }
    else {
        tmp_shm_ptr = static_cast<SharedDataArea::Representation*>(raw_shm_ptr);

        //TODO: try CAS of hasReader to true
        //tmp_shm_ptr->hasReader = true;

        //if it was already true, return not success
        //it was false, return the address of the SharedData

        sda.shm = tmp_shm_ptr;
        ok = true;
    }
    return ok;
}

bool SharedDataArea::SigblockConsumer::ReadSigblock(SDA::Sigblock& sb) {
    bool fret = true;
    if (IsOperational()) {
        fret = Items()->Get(sb);
    }
    else {
        fret = false;
    }
    return fret;
}

SDA::Sigblock::Metadata* SharedDataArea::SigblockConsumer::GetSigblockMetadata() {
    return Header();
}

bool SharedDataArea::SigblockProducer::WriteSigblock(const SDA::Sigblock& sb) {
    bool fret = true;
    if (IsOperational()) {
        fret = Items()->Put(sb);
        if (!fret) {
            droppedWrites++;  //TODO handle overflow ...
        }
    }
    else {
        fret = false;
    }
    return (fret);
}

SDA::Sigblock::Metadata* SharedDataArea::SigblockProducer::GetSigblockMetadata() {
    return Header();
}

SDA::uint64 SharedDataArea::SigblockProducer::DroppedWrites() const {
    return SharedDataArea::Representation::droppedWrites;
}

}
