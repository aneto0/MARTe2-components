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

#include <cstring>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SigblockDoubleBuffer.h"
#include "Atomic.h"
#include "Platform.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace SDA {

void SigblockDoubleBuffer::Reset(const SDA::uint32 bufferSize, const std::size_t sizeOfSigblock) {
	//this->bufferSize = bufferSize;	//TODO: Purge this field.
	this->sizeOfSigblock = sizeOfSigblock;
	frontbuffer = 0u;
	status = FREE;
	/*lint -e{9132} buffer is the base address of the allocated memory*/
	std::memset(buffer, 0, sizeOfSigblock*2u);
}

bool SigblockDoubleBuffer::Get(SDA::Sigblock& sb) {
	bool fret = true;
	bool acquired = CAS<BufferStatus>(&status, FULL, READING);
	if (acquired) {
		//[[assert: status == READING]]
		std::memcpy(&sb, &(buffer[sizeOfSigblock * (frontbuffer)]), sizeOfSigblock);
        /*lint -e{534} the old value of status is not needed*/
		XCHG<BufferStatus>(&status, FREE);
	}
	else {
		fret = false;
	}
	return fret;
}

bool SigblockDoubleBuffer::Put(const SDA::Sigblock& sb) {
	bool fret = true;
	SDA::uint32 backbuffer = ((frontbuffer + 1u) % TWO);
	std::memcpy(&(buffer[sizeOfSigblock * backbuffer]), &sb, sizeOfSigblock);
	/*lint -e{9007} if left hand of logical operator is true the CAS on right hand must not be executed*/
	bool acquired = (CAS<BufferStatus>(&status, FREE, WRITING) || CAS<BufferStatus>(&status, FULL, WRITING));
	if (acquired) {
		//[[assert: status == WRITING]]
		frontbuffer = backbuffer;
		/*lint -e{534} the old value of status is not needed*/
		XCHG<BufferStatus>(&status, FULL);
	}
	else {
		fret = false;
	}
	return fret;
}

}
