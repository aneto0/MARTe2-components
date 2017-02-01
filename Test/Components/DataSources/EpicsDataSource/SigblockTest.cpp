/**
 * @file SigblockTest.cpp
 * @brief Source file for class SigblockTest
 * @date 30/01/2017
 * @author Ivan Herrero
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
 * the class SigblockTest (public, protected, and private). Be aware that some
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

#include "Sigblock.h"
#include "SigblockTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

//struct DataSet {
//	Sigblock::Metadata** items;
//	unsigned int size;
//	DataSet(const unsigned int size):
//		items(new Sigblock::Metadata*[size]),
//		size(size) {
//	}
//	~DataSet() {
//		delete[] items;
//	}
//};




void MakeDataSet(unsigned int& signalsCount, Signal::Metadata* signalsMetadata) {
	std::size_t sizes[] = { 4, 2, 8, 4, 10 };
	unsigned int i = 0;
	while (i < signalsCount) {
		std::strncpy(signalsMetadata[i].name, "Signal0", Signal::Metadata::NAME_MAX_LEN);
		signalsMetadata[i].size = sizes[i];
		i++;
	}
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#include "SharedDataAreaSupport.h"

bool SigblockTest::MetadataTest::TestSetSignalsMetadata() {
	bool ok = false;
	unsigned int signalsCount = 2;
	char rawmem[Sigblock::Metadata::SizeOf(signalsCount)];
	Sigblock::Metadata* target = reinterpret_cast<Sigblock::Metadata*>(&rawmem);

	Signal::Metadata signalsMetadata[signalsCount];

//	MakeDataSet(signalsCount, signalsMetadata);

	GenerateMetadataForSigblock<double>(signalsMetadata, signalsCount);

	target->SetSignalsMetadata(signalsCount, signalsMetadata);
	ok = (target->GetSignalsCount() == signalsCount);

	std::size_t offset = 0;
	for (unsigned int  i = 0u; (i < signalsCount); i++) {
		std::stringstream name; //TODO: Use MARTe StreamString class, instead.
		name << "Signal" << i;
		ok &= (target->GetSignalIndex(signalsMetadata[i].name) == static_cast<int>(i));
		ok &= (std::strncmp(target->GetSignalName(i), signalsMetadata[i].name, Signal::Metadata::NAME_MAX_LEN) == 0);
		ok &= (target->GetSignalOffsetByIndex(i) == static_cast<int>(offset));
		ok &= (target->GetSignalSizeByIndex(i) == signalsMetadata[i].size);
		offset += signalsMetadata[i].size;
	}

	ok &= (target->GetTotalSize() == (sizeof(Signal::Metadata) * signalsCount));

	return ok;
}

bool SigblockTest::TestGetSignalAddress() {
	bool ok;
	char rawmem[10];
	Sigblock* target = reinterpret_cast<Sigblock*>(&rawmem);
	ok = (target->GetSignalAddress(0) == (target + 0));
	ok &= (target->GetSignalAddress(4) == (target + 4));
	ok &= (target->GetSignalAddress(8) == (target + 8));
	ok &= (target->GetSignalAddress(10) == (target + 10));

	unsigned int signalsCount = 10;
	Signal::Metadata signalsMetadata[signalsCount];
	char rawmem2[Sigblock::Metadata::SizeOf(signalsCount)];
	Sigblock::Metadata* sbmd = reinterpret_cast<Sigblock::Metadata*>(&rawmem2);
	GenerateMetadataForSigblock<double>(signalsMetadata, signalsCount);
	sbmd->SetSignalsMetadata(signalsCount, signalsMetadata);
	std::size_t offset = 0;
	for (unsigned int  i = 0u; (i < signalsCount); i++) {
		ok &= (target->GetSignalAddress(sbmd->GetSignalOffsetByIndex(i)) == (target + offset));
		offset += signalsMetadata[i].size;
	}
	return ok;
}
