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

void Sigblock::Metadata::SetSignalsMetadata(const unsigned int signalsCount, const Signal::Metadata signalsMetadata[]) {
	std::size_t offset = 0;
	for (unsigned int i = 0; i < signalsCount; i++) {
		std::strncpy(this->signalsMetadata[i].name, signalsMetadata[i].name, Signal::Metadata::NAME_MAX_LEN);
		this->signalsMetadata[i].size = signalsMetadata[i].size;
		this->signalsMetadata[i].offset = offset;
		printf("Sigblock::Metadata::SetSignalsMetadata >>> i=%u offset=%zu this->signalsMetadata[i].offset=%zu this->signalsMetadata[i].name=%s\n", i, offset, this->signalsMetadata[i].offset, this->signalsMetadata[i].name);
		offset += signalsMetadata[i].size;
	}
	this->signalsCount = signalsCount;
}

unsigned int Sigblock::Metadata::GetSignalsCount() {
	return signalsCount;
}

int Sigblock::Metadata::GetSignalIndex(const char* name) {
    int result = -1;
	unsigned int i = 0;
	int found = false;
	while (i < signalsCount && !found) {
		if (std::strncmp(signalsMetadata[i].name, name, Signal::Metadata::NAME_MAX_LEN) == 0) {
            found = true;
        }
		i++;
	}
	if (found) {
		result = (i-1);
	}
	return result;
}

char* Sigblock::Metadata::GetSignalName(const int index) {
    return signalsMetadata[index].name;
}

int Sigblock::Metadata::GetSignalOffsetByIndex(const int index) {
	return signalsMetadata[index].offset;
}

std::size_t Sigblock::Metadata::GetSignalSizeByIndex(const int index) {
	return signalsMetadata[index].size;
}

std::size_t Sigblock::Metadata::GetTotalSize() {
	return (sizeof(Signal::Metadata) * signalsCount);
}

std::size_t Sigblock::Metadata::SizeOf(const unsigned int signalsCount) {
	return (sizeof(unsigned int) + (signalsCount * (sizeof(Signal::Metadata)+sizeof(std::size_t))));
}

void* Sigblock::GetSignalAddress(std::size_t offset) {
	return this + offset;
}
