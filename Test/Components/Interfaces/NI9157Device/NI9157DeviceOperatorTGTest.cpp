/**
 * @file NI9157DeviceOperatorTGTest.cpp
 * @brief Source file for class NI9157DeviceOperatorTGTest
 * @date 23/05/2018
 * @author Giuseppe Ferrò
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
 * the class NI9157DeviceOperatorTGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157DeviceOperatorTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(NI9157DeviceOperatorTGTest,TestConstructor) {
    NI9157DeviceOperatorTTest<uint8> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    ASSERT_TRUE(test.TestCompare(0xff, 0xff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    ASSERT_TRUE(test.TestCompare(1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    ASSERT_TRUE(test.TestCompare(2, 1));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    ASSERT_TRUE(test.TestCompare(0x7f, 0x7f));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    ASSERT_TRUE(test.TestCompare(-1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    ASSERT_TRUE(test.TestCompare(2, -1));
}


TEST(NI9157DeviceOperatorTGTest,TestCompare_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    ASSERT_TRUE(test.TestCompare(0xffff, 0xffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    ASSERT_TRUE(test.TestCompare(1, 2));
}


TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    ASSERT_TRUE(test.TestCompare(2, 1));
}


TEST(NI9157DeviceOperatorTGTest,TestCompare_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    ASSERT_TRUE(test.TestCompare(0x7fff, 0x7fff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    ASSERT_TRUE(test.TestCompare(-1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    ASSERT_TRUE(test.TestCompare(2, -1));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    ASSERT_TRUE(test.TestCompare(0xffffffff, 0xffffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    ASSERT_TRUE(test.TestCompare(1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    ASSERT_TRUE(test.TestCompare(2, 1));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    ASSERT_TRUE(test.TestCompare(0x7fffffff, 0x7fffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    ASSERT_TRUE(test.TestCompare(-1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    ASSERT_TRUE(test.TestCompare(2, -1));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    ASSERT_TRUE(test.TestCompare(0xffffffffffffffff, 0xffffffffffffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    ASSERT_TRUE(test.TestCompare(1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    ASSERT_TRUE(test.TestCompare(2, 1));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    ASSERT_TRUE(test.TestCompare(0x7fffffffffffffff, 0x7fffffffffffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    ASSERT_TRUE(test.TestCompare(-1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    ASSERT_TRUE(test.TestCompare(2, -1));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    ASSERT_TRUE(test.TestCopy(0xff));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    ASSERT_TRUE(test.TestCopy(0x7f));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    ASSERT_TRUE(test.TestCopy(0xffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    ASSERT_TRUE(test.TestCopy(0x7fff));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    ASSERT_TRUE(test.TestCopy(0xffffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    ASSERT_TRUE(test.TestCopy(0x7fffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    ASSERT_TRUE(test.TestCopy(0xffffffffffffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    ASSERT_TRUE(test.TestCopy(0x7fffffffffffffff));
}
