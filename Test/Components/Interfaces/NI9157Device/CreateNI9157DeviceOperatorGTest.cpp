/**
 * @file CreateNI9157DeviceOperatorGTest.cpp
 * @brief Source file for class CreateNI9157DeviceOperatorGTest
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
 * the class CreateNI9157DeviceOperatorGTest (public, protected, and private).
 * Be aware that some methods, such as those inline could be defined on the
 * header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

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
TEST(CreateNI9157DeviceOperatorGTest,TestConstructor) {
    CreateNI9157DeviceOperatorTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(CreateNI9157DeviceOperatorGTest,TestConstructorArgs) {
    CreateNI9157DeviceOperatorTest test;
    ASSERT_TRUE(test.TestConstructorArgs());
}

TEST(CreateNI9157DeviceOperatorGTest,TestCreate_U8) {
    CreateNI9157DeviceOperatorTest test;
    uint8 x=0;
    ASSERT_TRUE(test.TestCreate(x));
}

TEST(CreateNI9157DeviceOperatorGTest,TestCreate_I8) {
    CreateNI9157DeviceOperatorTest test;
    int8 x=0;
    ASSERT_TRUE(test.TestCreate(x));
}

TEST(CreateNI9157DeviceOperatorGTest,TestCreate_U16) {
    CreateNI9157DeviceOperatorTest test;
    uint16 x=0;
    ASSERT_TRUE(test.TestCreate(x));
}

TEST(CreateNI9157DeviceOperatorGTest,TestCreate_I16) {
    CreateNI9157DeviceOperatorTest test;
    int16 x=0;
    ASSERT_TRUE(test.TestCreate(x));
}

TEST(CreateNI9157DeviceOperatorGTest,TestCreate_U32) {
    CreateNI9157DeviceOperatorTest test;
    uint32 x=0;
    ASSERT_TRUE(test.TestCreate(x));
}

TEST(CreateNI9157DeviceOperatorGTest,TestCreate_I32) {
    CreateNI9157DeviceOperatorTest test;
    int32 x=0;
    ASSERT_TRUE(test.TestCreate(x));
}

TEST(CreateNI9157DeviceOperatorGTest,TestCreate_U64) {
    CreateNI9157DeviceOperatorTest test;
    uint64 x=0;
    ASSERT_TRUE(test.TestCreate(x));
}

TEST(CreateNI9157DeviceOperatorGTest,TestCreate_I64) {
    CreateNI9157DeviceOperatorTest test;
    int64 x=0;
    ASSERT_TRUE(test.TestCreate(x));
}

TEST(CreateNI9157DeviceOperatorGTest,TestGetIdentifier) {
    CreateNI9157DeviceOperatorTest test;
    ASSERT_TRUE(test.TestGetIdentifier());
}

TEST(CreateNI9157DeviceOperatorGTest,TestGetTypeDescriptor) {
    CreateNI9157DeviceOperatorTest test;
    ASSERT_TRUE(test.TestGetTypeDescriptor());
}
