/**
 * @file NI9157DeviceOperatorDatabaseGTest.cpp
 * @brief Source file for class NI9157DeviceOperatorDatabaseGTest.
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
 * the class NI9157DeviceOperatorDatabaseGTest (public, protected, and private).
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
#include "NI9157DeviceOperatorDatabaseTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI9157DeviceOperatorDatabaseGTest,TestGetCreateNI9157DeviceOperatorFromName) {
    NI9157DeviceOperatorDatabaseTest test;
    ASSERT_TRUE(test.TestGetCreateNI9157DeviceOperatorFromName());
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetCreateNI9157DeviceOperatorFromTypeDes) {
    NI9157DeviceOperatorDatabaseTest test;
    ASSERT_TRUE(test.TestGetCreateNI9157DeviceOperatorFromTypeDes());
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromName_Bool) {
    NI9157DeviceOperatorDatabaseTest test;
    bool x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromName("NI9157DeviceBool", x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromName_U8) {
    NI9157DeviceOperatorDatabaseTest test;
    uint8 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromName("NI9157DeviceU8", x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromName_I8) {
    NI9157DeviceOperatorDatabaseTest test;
    int8 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromName("NI9157DeviceI8", x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromName_U16) {
    NI9157DeviceOperatorDatabaseTest test;
    uint16 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromName("NI9157DeviceU16", x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromName_I16) {
    NI9157DeviceOperatorDatabaseTest test;
    int16 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromName("NI9157DeviceI16", x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromName_U32) {
    NI9157DeviceOperatorDatabaseTest test;
    uint32 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromName("NI9157DeviceU32", x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromName_I32) {
    NI9157DeviceOperatorDatabaseTest test;
    int32 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromName("NI9157DeviceI32", x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromName_U64) {
    NI9157DeviceOperatorDatabaseTest test;
    uint64 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromName("NI9157DeviceU64", x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromName_I64) {
    NI9157DeviceOperatorDatabaseTest test;
    int64 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromName("NI9157DeviceI64", x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromTypeDes_Bool) {
    NI9157DeviceOperatorDatabaseTest test;
    uint8 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromTypeDes(UnsignedInteger8Bit, x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromTypeDes_U8) {
    NI9157DeviceOperatorDatabaseTest test;
    uint8 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromTypeDes(UnsignedInteger8Bit, x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromTypeDes_I8) {
    NI9157DeviceOperatorDatabaseTest test;
    int8 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromTypeDes(SignedInteger8Bit, x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromTypeDes_U16) {
    NI9157DeviceOperatorDatabaseTest test;
    uint16 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromTypeDes(UnsignedInteger16Bit, x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromTypeDes_I16) {
    NI9157DeviceOperatorDatabaseTest test;
    int16 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromTypeDes(SignedInteger16Bit, x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromTypeDes_U32) {
    NI9157DeviceOperatorDatabaseTest test;
    uint32 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromTypeDes(UnsignedInteger32Bit, x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromTypeDes_I32) {
    NI9157DeviceOperatorDatabaseTest test;
    int32 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromTypeDes(SignedInteger32Bit, x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromTypeDes_U64) {
    NI9157DeviceOperatorDatabaseTest test;
    uint64 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromTypeDes(UnsignedInteger64Bit, x));
}

TEST(NI9157DeviceOperatorDatabaseGTest,TestGetNI9157DeviceOperatorFromTypeDes_I64) {
    NI9157DeviceOperatorDatabaseTest test;
    int64 x=0;
    ASSERT_TRUE(test.TestGetNI9157DeviceOperatorFromTypeDes(SignedInteger64Bit, x));
}
