/**
 * @file SigblockDoubleBuffer.cpp
 * @brief Source file for class SigblockDoubleBuffer
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
 * the class SigblockDoubleBuffer (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#ifndef LINT
#include <cstring>
#endif


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SigblockDoubleBuffer.h"
#include "Atomic2.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace SDA {

void SigblockDoubleBuffer::Reset(const SDA::uint32 bufferSize, const SDA::size_type sigblockSize) {
	//this->bufferSize = bufferSize;	//TODO: Purge this field.
	this->sizeOfSigblock = sigblockSize;
	frontbuffer = 0u;
	status = FREE;
	/*lint -e{9132} buffer is the base address of the allocated memory*/
	(void)std::memset(buffer, 0, sigblockSize*TWO);
}

bool SigblockDoubleBuffer::Get(SDA::Sigblock& item) {
	bool fret = true;
	bool acquired = CAS<BufferStatus>(&status, FULL, READING);
	if (acquired) {
		//[[assert: status == READING]]
		(void)std::memcpy(&item, &(buffer[sizeOfSigblock * (frontbuffer)]), sizeOfSigblock);
		(void)XCHG<BufferStatus>(&status, FREE);
	}
	else {
		fret = false;
	}
	return fret;
}

bool SigblockDoubleBuffer::Put(const SDA::Sigblock& item) {
	bool fret = true;
	SDA::uint32 backbuffer = ((frontbuffer + 1u) % TWO);
	(void)std::memcpy(&(buffer[sizeOfSigblock * backbuffer]), &item, sizeOfSigblock);
	/*lint -e{9007} if left hand of logical operator is true the CAS on right hand must not be executed*/
	bool acquired = (CAS<BufferStatus>(&status, FREE, WRITING) || CAS<BufferStatus>(&status, FULL, WRITING));
	if (acquired) {
		//[[assert: status == WRITING]]
		frontbuffer = backbuffer;
		(void)XCHG<BufferStatus>(&status, FULL);
	}
	else {
		fret = false;
	}
	return fret;
}

SDA::size_type SigblockDoubleBuffer::SizeOf(const SDA::size_type sigblockSize) {
    return (sizeof(SigblockDoubleBuffer) + (sigblockSize * TWO));
}

}
