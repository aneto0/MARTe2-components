/**
 * @file CreateNI9157DeviceOperatorTest.cpp
 * @brief Source file for class CreateNI9157DeviceOperatorTest
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class CreateNI9157DeviceOperatorTest (public, protected, and private).
 * Be aware that some methods, such as those inline could be defined on the
 * header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CreateNI9157DeviceOperatorTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
CreateNI9157DeviceOperatorTest::CreateNI9157DeviceOperatorTest() {
}

CreateNI9157DeviceOperatorTest::~CreateNI9157DeviceOperatorTest() {
}

bool CreateNI9157DeviceOperatorTest::TestConstructor() {
    CreateNI9157DeviceOperator<uint8> test;
    bool ret = test.GetIdentifier() == NULL;
    ret &= test.GetTypeDescriptor() == InvalidType;

    return ret;
}

bool CreateNI9157DeviceOperatorTest::TestConstructorArgs() {
    CreateNI9157DeviceOperator<uint8> test("ciao");
    bool ret = StringHelper::Compare(test.GetIdentifier(), "ciao") == 0;
    ret &= test.GetTypeDescriptor() == UnsignedInteger8Bit;

    return ret;
}

bool CreateNI9157DeviceOperatorTest::TestGetIdentifier() {
    return TestConstructorArgs();
}

bool CreateNI9157DeviceOperatorTest::TestGetTypeDescriptor() {
    return TestConstructorArgs();
}
