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

#include "Sigblock.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace SDA {

void Sigblock::Metadata::SetSignalsMetadata(const SDA::uint32 signalsCount, const SDA::Signal::Metadata signalsMetadata[]) {
	std::size_t offset = 0u;
	for (SDA::uint32 i = 0u; i < signalsCount; i++) {
		std::strncpy(this->signalsMetadata[i].name, signalsMetadata[i].name, SDA::Signal::Metadata::NAME_MAX_LEN);
		this->signalsMetadata[i].size = signalsMetadata[i].size;
		this->signalsMetadata[i].offset = offset;
//		printf("SDA::Sigblock::Metadata::SetSignalsMetadata >>> i=%u offset=%zu this->signalsMetadata[i].offset=%zu this->signalsMetadata[i].name=%s\n", i, offset, this->signalsMetadata[i].offset, this->signalsMetadata[i].name);
		offset += signalsMetadata[i].size;
	}
	this->signalsCount = signalsCount;
}

SDA::uint32 Sigblock::Metadata::GetSignalsCount() {
	return signalsCount;
}

SDA::int32 Sigblock::Metadata::GetSignalIndex(const SDA::char8* const name) {
	SDA::int32 result = -1;
    SDA::uint32 i = 0u;
    bool found = false;
	while ((i < signalsCount) && (!found)) {
		if (std::strncmp(signalsMetadata[i].name, name, SDA::Signal::Metadata::NAME_MAX_LEN) == 0) {
            found = true;
        }
		i++;
	}
	if (found) {
		result = (i-1);
	}
	return result;
}

SDA::char8* Sigblock::Metadata::GetSignalName(const SDA::int32 index) {
    return signalsMetadata[index].name;
}

SDA::int32 Sigblock::Metadata::GetSignalOffsetByIndex(const SDA::int32 index) {
	return signalsMetadata[index].offset;
}

std::size_t Sigblock::Metadata::GetSignalSizeByIndex(const SDA::int32 index) {
	return signalsMetadata[index].size;
}

std::size_t Sigblock::Metadata::GetTotalSize() {
	return (sizeof(SDA::Signal::Metadata) * signalsCount);
}

std::size_t Sigblock::Metadata::SizeOf(const SDA::uint32 signalsCount) {
	return (sizeof(SDA::uint32) + (signalsCount * (sizeof(SDA::Signal::Metadata)+sizeof(std::size_t))));
}

void* Sigblock::GetSignalAddress(const std::size_t offset) {
	/*lint -e{9016} [MISRA C++ Rule 5-0-15] pointer arithmetic is needed in this case*/
	return this + offset;
}

}
