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

#include <cstdio>
#include <cstring>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Platform.h"
#include "SharedDataArea.h"

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
static std::size_t CalculateSizeOfSigblock(const unsigned int signalsCount, const Signal::Metadata signalsMetadata[]) {
	std::size_t sigblockSize = 0;
	for (unsigned int i = 0; i < signalsCount; i++) {
		sigblockSize += signalsMetadata[i].size;
	}
	return sigblockSize;
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

SharedDataArea::SharedDataArea(): shm(NULL) {

}

SharedDataArea::SharedDataArea(SharedDataArea::Representation* shm):
		shm(shm) {
}

SharedDataArea::SigblockProducer* SharedDataArea::GetSigblockProducerInterface() {
	//Check if it has already a writer registered
	//try CAS of hasReader to true
	shm->hasWriter = true;
	return reinterpret_cast<SigblockProducer*>(shm);
}

SharedDataArea::SigblockConsumer* SharedDataArea::GetSigblockConsumerInterface() {
	//Check if it has already a reader registered
	//try CAS of hasReader to true
	shm->hasReader = true;
	return reinterpret_cast<SigblockConsumer*>(shm);
}

void SharedDataArea::Representation::FillPreHeader(std::size_t sizeOfHeader, std::size_t sizeOfItems) {
	hasReader = false;
	hasWriter = false;
	droppedWrites = 0;
    offsetOfHeader = 0;
    offsetOfItems = sizeOfHeader;
}

void SharedDataArea::Representation::FillHeader(const unsigned int signalsCount, const Signal::Metadata signalsMetadata[]) {
	Sigblock::Metadata* header = Header();
	header->SetSignalsMetadata(signalsCount, signalsMetadata);
}

void SharedDataArea::Representation::FillItems(const unsigned int bufferSize, std::size_t sizeOfSigblock) {
	SigblockDoubleBuffer* items = Items();
	items->Reset(bufferSize, sizeOfSigblock);
}

SharedDataArea SharedDataArea::BuildSharedDataAreaForMARTe(const char* const name, const unsigned int signalsCount, const Signal::Metadata signalsMetadata[], const unsigned int bufferSize) {
	std::size_t sizeOfSigblock = CalculateSizeOfSigblock(signalsCount, signalsMetadata);
	std::size_t sizeOfHeader = Sigblock::Metadata::SizeOf(signalsCount);
	std::size_t sizeOfItems = (bufferSize * sizeOfSigblock); //TODO: Abstract this private knowledge
	std::size_t totalSize = (sizeof(SharedDataArea::Representation) + sizeOfHeader + sizeOfItems);
//	SharedDataArea* obj = NULL;
	SharedDataArea::Representation* tmp_shm_ptr = NULL;

	printf("*** SharedDataArea::BuildSharedDataAreaForMARTe name=%s sizeOfHeader=%lu, sizeOfSigblock=%lu, sizeOfItems=%lu, totalSize=%lu ***\n", name, sizeOfHeader, sizeOfSigblock, sizeOfItems, totalSize);
	void* raw_shm_ptr = Platform::MakeShm(name, totalSize);
    if (raw_shm_ptr == NULL) {
    	//error
    }
    else {
    	tmp_shm_ptr = static_cast<SharedDataArea::Representation*>(raw_shm_ptr);
std::memset(tmp_shm_ptr+sizeof(size_t), 88, totalSize);
		tmp_shm_ptr->FillPreHeader(sizeOfHeader, sizeOfItems);
		printf("*** SharedDataArea::BuildSharedDataAreaForMARTe tmp_shm_ptr->rawmem=%p tmp_shm_ptr->Header()=%p, tmp_shm_ptr->Items()=%p ***\n", tmp_shm_ptr->rawmem, tmp_shm_ptr->Header(), tmp_shm_ptr->Items());
std::memset(tmp_shm_ptr->Header(), 89, sizeOfHeader);
std::memset(tmp_shm_ptr->Items(), 90, sizeOfItems);
		tmp_shm_ptr->FillHeader(signalsCount, signalsMetadata);
		tmp_shm_ptr->FillItems(bufferSize, sizeOfSigblock);
//		obj = reinterpret_cast<SharedDataArea*>(tmp_shm_ptr);
    }
    return SharedDataArea(tmp_shm_ptr);
}

SharedDataArea SharedDataArea::BuildSharedDataAreaForEPICS(const char* const name) {
	void* raw_shm_ptr = NULL;
//	SharedDataArea* obj = NULL;
	SharedDataArea::Representation* tmp_shm_ptr = NULL;
    raw_shm_ptr = Platform::JoinShm(name);
    if (raw_shm_ptr == NULL) {
    	//error
    }
    else {
    	tmp_shm_ptr = static_cast<SharedDataArea::Representation*>(raw_shm_ptr);

		//try CAS of hasReader to true
		//tmp_shm_ptr->hasReader = true;

	    //if it was already true, return not success
	    //it was false, return the address of the SharedData

//	    obj = reinterpret_cast<SharedDataArea*>(tmp_shm_ptr);
    	printf("The device has joined the shared data area \"%s\"\n", name);
    }
//	printf("*** SharedDataArea::BuildSharedDataAreaForEPICS obj=%p ***\n", obj);
    return SharedDataArea(tmp_shm_ptr);
}

bool SharedDataArea::SigblockConsumer::ReadSigblock(Sigblock& sb) {
	bool fret = true;
	if (IsOperational()) {
		fret = Items()->Get(sb);
	}
	else {
		fret = false;
	}
	return fret;
}

Sigblock::Metadata* SharedDataArea::SigblockConsumer::GetSigblockMetadata() {
	return Header();
}

bool SharedDataArea::SigblockProducer::WriteSigblock(const Sigblock& sb) {
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

Sigblock::Metadata* SharedDataArea::SigblockProducer::GetSigblockMetadata() {
	return Header();
}

unsigned long SharedDataArea::SigblockProducer::DroppedWrites() const {
	return SharedDataArea::Representation::droppedWrites;
}
