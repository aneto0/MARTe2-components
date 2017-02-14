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
#include "SigblockSupport.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

template bool SigblockTest::TestGetSignalAddress<int>(const int value);
template bool SigblockTest::TestGetSignalAddress<double>(const double value);

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool SigblockTest::MetadataTest::TestSetSignalsMetadata() {
	bool ok = false;
	unsigned int signalsCount = 2;
	SDA::Sigblock::Metadata* target;

	//Allocate memory for sigblock's metadata:
	target = MallocSigblockMetadata(signalsCount);

	//Check allocation of sigblock's metadata:
	ok = (target != NULL);

	if (ok) {
		SDA::Signal::Metadata rawMetadata[signalsCount];

		//Generate metadata values for testing:
		GenerateMetadataForSigblock<double>(rawMetadata, signalsCount);

		//Set testing metadata values:
		target->SetSignalsMetadata(signalsCount, rawMetadata);

		//Check signals count:
		ok &= (target->GetSignalsCount() == signalsCount);

		//Check total size:
		ok &= (target->GetTotalSize() == (sizeof(SDA::Signal::Metadata) * signalsCount));

		//Check signal metadata for each signal:
		{
			std::size_t offset = 0;
			for (SDA::uint32 i = 0u; (i < signalsCount); i++) {
				//Check signal index:
				ok &= (target->GetSignalIndex(rawMetadata[i].name) == i);
				//Check signal name:
				ok &= (std::strncmp(target->GetSignalName(i), rawMetadata[i].name, SDA::Signal::Metadata::NAME_MAX_LEN) == 0);
				//Check signal offset:
				ok &= (target->GetSignalOffsetByIndex(i) == static_cast<SDA::uint32>(offset));
				//Check signal size:
				ok &= (target->GetSignalSizeByIndex(i) == rawMetadata[i].size);
				//Update signal offset:
				offset += rawMetadata[i].size;
			}
		}
	}

	//Free memory of sigblock's metadata:
	FreeSigblockMetadata(target);

	//Check release of sigblock's metadata:
	ok &= (target == NULL);

	return ok;
}

template<typename SignalType>
bool SigblockTest::TestGetSignalAddress(const SignalType value) {
	const unsigned int signalsCount = 10;
	bool ok = false;
	SDA::Sigblock* target = NULL;
	SDA::Sigblock::Metadata* metadata = NULL;
	void* addresses[signalsCount];

	//Allocate memory for sigblock's metadata:
	metadata = MallocSigblockMetadata(signalsCount);

	//Check allocation of sigblock's metadata:
	ok = (metadata != NULL);

	if (ok) {
		SDA::Signal::Metadata rawMetadata[signalsCount];

		//Generate metadata values for testing:
		GenerateMetadataForSigblock<SignalType>(rawMetadata, signalsCount);

		//Set testing metadata values:
		metadata->SetSignalsMetadata(signalsCount, rawMetadata);

		//Allocate memory for sigblock:
		target = MallocSigblock(metadata->GetTotalSize());

		//Check allocation of sigblock:
		ok = (target != NULL);

		if (ok) {

			//Collect signals' addresses:
			{
				for (unsigned int  i = 0u; i < signalsCount; i++) {
					addresses[i] = target->GetSignalAddress(metadata->GetSignalOffsetByIndex(i));
				}
			}

			//Check addresses' offsets:
			{
				std::size_t offset = 0;
				for (unsigned int  i = 0u; i < signalsCount; i++) {
					ok &= (addresses[i] == (target + offset));
					offset += metadata->GetSignalSizeByIndex(i);
				}
				ok &= ((target + offset) == (target + (sizeof(SignalType) * signalsCount)));
			}

			if (ok) {

				//Store testing values:
				{
					for (unsigned int  i = 0u; i < signalsCount; i++) {
						SignalType* signal = static_cast<SignalType*>(addresses[i]);
						*signal = value;
					}
				}

				//Check testing values:
				{
					for (unsigned int  i = 0u; i < signalsCount; i++) {
						SignalType* signal = static_cast<SignalType*>(addresses[i]);
						ok &= (*signal == value);
					}
				}

			}
		}
	}

	//Free memory of sigblock:
	FreeSigblock(target);

	//Check release of sigblock:
	ok &= (target == NULL);

	//Free memory of sigblock's metadata:
	FreeSigblockMetadata(metadata);

	//Check release of sigblock's metadata:
	ok &= (metadata == NULL);

	return ok;
}
