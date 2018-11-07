/**
 * @file CRCHelperTTest.cpp
 * @brief Source file for class CRCHelperTTest
 * @date Oct 30, 2018 
 * @author Luca Porzio
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
 * the class CRCHelperTTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CRCHelperT.h"
#include "CRCHelperTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

template <typename T>
bool CRCHelperTTest::TestConstructor() {
    CRCHelperT<T> test;
    return true;
}

bool CRCHelperTTest::TestConstructorUint8() {
    return TestConstructor<uint8>();
}

bool CRCHelperTTest::TestConstructorUint16() {
    return TestConstructor<uint16>();
}

bool CRCHelperTTest::TestConstructorUint32() {
    return TestConstructor<uint32>();
}

bool CRCHelperTTest::TestComputeTable() {
    uint8 p8 = 0x7u;
    uint16 p16 = 0x1021u;
    uint32 p32 = 0x4C11DB7u;

    uint8 * pol8 = &p8;
    uint16 * pol16 = &p16;
    uint32 * pol32 = &p32;

    CRCHelperT<uint8> t8;
    t8.ComputeTable(pol8);

    CRCHelperT<uint8> t16;
    t16.ComputeTable(pol16);

    CRCHelperT<uint8> t32;
    t32.ComputeTable(pol32);

    return true;
}

bool CRCHelperTTest::TestCompute() {
    uint8 i8 = 0x0u;
    uint16 i16 = 0u;
    uint32 i32 = 0u;

    uint8 p8 = 0x7u;
    uint16 p16 = 0x1021u;
    uint32 p32 = 0x4C11DB7u;

    uint8 * pol8 = &p8;
    uint16 * pol16 = &p16;
    uint32 * pol32 = &p32;

    uint8 d = 10u;

    uint8 * init8 = &i8;
    uint16 * init16 = &i16;
    uint32 * init32 = &i32;

    uint8 * data = &d;

    uint8 * ret8 = &i8;
    uint16 * ret16 = &i16;
    uint32 * ret32 = &i32;

    int32 size = sizeof(uint8);

    CRCHelperT<uint8> t8;
    t8.ComputeTable(pol8);
    t8.Compute(data, size, init8, false, ret8);

    CRCHelperT<uint16> t16;
    t16.ComputeTable(pol16);
    t16.Compute(data, size, init16, false, ret16);

    CRCHelperT<uint16> t32;
    t32.ComputeTable(pol32);
    t32.Compute(data, size, init32, false, ret32);

    bool ok = (ret8 != 0 && ret16 != 0 && ret32 != 0);

    return ok;
}
	
