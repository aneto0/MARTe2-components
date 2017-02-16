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
static SDA::size_type CalculateSizeOfSigblock(const SDA::uint32 signalsCount, const SDA::Signal::Metadata signalsMetadata[]) {
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

SharedDataArea::SharedDataArea(): shm(SDA_NULL_PTR(Representation*)) {

}

SharedDataArea::SharedDataArea(SharedDataArea::Representation* const obj):
		shm(obj) {
}

SharedDataArea::SigblockProducer* SharedDataArea::GetSigblockProducerInterface() {
    //TODO: Check if it has already a writer registered
    //try CAS of hasReader to true
    if (shm != NULL) {
        shm->hasWriter = true;
    }
    /*lint -e{1939} The SigblockProducer does not add members to Representation (read Representaion's documentation)*/
    return reinterpret_cast<SigblockProducer*>(shm);
}

SharedDataArea::SigblockConsumer* SharedDataArea::GetSigblockConsumerInterface() {
    //TODO: Check if it has already a reader registered
    //try CAS of hasReader to true
    if (shm != NULL) {
        shm->hasReader = true;
    }
    /*lint -e{1939} The SigblockProducer does not add members to Representation (read Representaion's documentation)*/
    return reinterpret_cast<SigblockConsumer*>(shm);
}

void SharedDataArea::Representation::FillPreHeader(const SDA::size_type sizeOfHeader, const SDA::size_type sizeOfItems) {
    //TODO: Purge sizeOfItems parameter.
	hasReader = false;
	hasWriter = false;
	droppedWrites = 0u;
    offsetOfHeader = 0u;
    offsetOfItems = sizeOfHeader;
}

void SharedDataArea::Representation::FillHeader(const SDA::uint32 signalsCount, const SDA::Signal::Metadata signalsMetadata[]) {
	SDA::Sigblock::Metadata* header = Header();
	header->Init(signalsCount, signalsMetadata);
}

void SharedDataArea::Representation::FillItems(const SDA::uint32 bufferSize, const SDA::size_type sizeOfSigblock) {
	SDA::SigblockDoubleBuffer* items = Items();
	items->Init(bufferSize, sizeOfSigblock);
}

SharedDataArea SharedDataArea::BuildSharedDataAreaForMARTe(const SDA::char8* const name, const SDA::uint32 signalsCount, const SDA::Signal::Metadata signalsMetadata[], const SDA::uint32 bufferSize) {
    SDA::size_type sizeOfSigblock = CalculateSizeOfSigblock(signalsCount, signalsMetadata);
    SDA::size_type sizeOfHeader = SDA::Sigblock::Metadata::SizeOf(signalsCount);
    SDA::size_type sizeOfItems = SDA::SigblockDoubleBuffer::SizeOf(sizeOfSigblock);
    SDA::size_type totalSize = (sizeof(SharedDataArea::Representation) + sizeOfHeader + sizeOfItems);
//	SharedDataArea* obj = NULL;
	Representation* tmp_shm_ptr = SDA_NULL_PTR(Representation*);

//	printf("*** SharedDataArea::BuildSharedDataAreaForMARTe name=%s sizeOfHeader=%lu, sizeOfSigblock=%lu, sizeOfItems=%lu, totalSize=%lu ***\n", name, sizeOfHeader, sizeOfSigblock, sizeOfItems, totalSize);
	void* raw_shm_ptr = SDA::Platform::MakeShm(name, totalSize);
    if (raw_shm_ptr == NULL) {
    	//error
    }
    else {
    	tmp_shm_ptr = static_cast<SharedDataArea::Representation*>(raw_shm_ptr);
//std::memset(tmp_shm_ptr+sizeof(size_t), 88, totalSize);
		tmp_shm_ptr->FillPreHeader(sizeOfHeader, sizeOfItems);
//		printf("*** SharedDataArea::BuildSharedDataAreaForMARTe tmp_shm_ptr->rawmem=%p tmp_shm_ptr->Header()=%p, tmp_shm_ptr->Items()=%p ***\n", tmp_shm_ptr->rawmem, tmp_shm_ptr->Header(), tmp_shm_ptr->Items());
//std::memset(tmp_shm_ptr->Header(), 89, sizeOfHeader);
//std::memset(tmp_shm_ptr->Items(), 90, sizeOfItems);
		tmp_shm_ptr->FillHeader(signalsCount, signalsMetadata);
		tmp_shm_ptr->FillItems(bufferSize, sizeOfSigblock);
//		obj = reinterpret_cast<SharedDataArea*>(tmp_shm_ptr);
    }
    return SharedDataArea(tmp_shm_ptr);
}

SharedDataArea SharedDataArea::BuildSharedDataAreaForEPICS(const SDA::char8* const name) {
	void* raw_shm_ptr;
//	SharedDataArea* obj = NULL;
	Representation* tmp_shm_ptr = SDA_NULL_PTR(Representation*);
    raw_shm_ptr = SDA::Platform::JoinShm(name);
    if (raw_shm_ptr == NULL) {
    	//error
    }
    else {
    	tmp_shm_ptr = static_cast<SharedDataArea::Representation*>(raw_shm_ptr);

		//TODO: try CAS of hasReader to true
		//tmp_shm_ptr->hasReader = true;

	    //if it was already true, return not success
	    //it was false, return the address of the SharedData

//	    obj = reinterpret_cast<SharedDataArea*>(tmp_shm_ptr);
//    	printf("The device has joined the shared data area \"%s\"\n", name);
    }
//	printf("*** SharedDataArea::BuildSharedDataAreaForEPICS obj=%p ***\n", obj);
    return SharedDataArea(tmp_shm_ptr);
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
			droppedWrites++;  //TODO check overflow ...
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
