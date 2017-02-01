/**
 * @file SigblockDoubleBufferTest.cpp
 * @brief Source file for class SigblockDoubleBufferTest
 * @date 31/01/2017
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
 * the class SigblockDoubleBufferTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SigblockDoubleBuffer.h"
#include "SigblockDoubleBufferTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool SigblockDoubleBufferTest::TestReset() {
	bool ok = false;
	//idem TestGet() ...
	return ok;
}

bool SigblockDoubleBufferTest::TestGet() {
	bool ok = false;
//	char rawmem1[10];
//	char rawmem2[20];
//	char rawmem3[30];
//	SigblockDoubleBuffer* target = reinterpret_cast<SigblockDoubleBuffer*>(rawmem3);
//	//target->Reset(...);
//
//	Sigblock* sb1 = reinterpret_cast<Sigblock*>(rawmem1);
//	//fill sb with test data ...
//	target->Put(*sb1);
//
//	Sigblock* sb2 = reinterpret_cast<Sigblock*>(rawmem2);
//	target->Get(*sb2);
//
//	//check if *sb1 == *sb2

	return ok;
}

bool SigblockDoubleBufferTest::TestPut() {
	bool ok = false;
	//idem TestGet() ...
	return ok;
}
