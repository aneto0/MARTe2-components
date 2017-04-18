/**
 * @file DANStreamGTest.cpp
 * @brief Source file for class DANStreamGTest
 * @date 11/04/2017
 * @author Andre Neto
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
 * the class DANStreamGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "DANStreamTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(DANStreamGTest,TestConstructor) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(DANStreamGTest,TestGetType) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestGetType());
}

TEST(DANStreamGTest,TestGetSamplingFrequency) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestGetSamplingFrequency());
}

TEST(DANStreamGTest,TestGetDANBufferMultiplier) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestGetDANBufferMultiplier());
}

TEST(DANStreamGTest,TestGetNumberOfSamples) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestGetNumberOfSamples());
}

TEST(DANStreamGTest,TestAddSignal) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestAddSignal());
}

TEST(DANStreamGTest,TestFinalise) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestFinalise());
}
	
TEST(DANStreamGTest,TestPutData_UInt16) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestPutData_UInt16());
}

TEST(DANStreamGTest,TestPutData_Int16) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestPutData_Int16());
}

TEST(DANStreamGTest,TestPutData_UInt32) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestPutData_UInt32());
}

TEST(DANStreamGTest,TestPutData_Int32) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestPutData_Int32());
}

TEST(DANStreamGTest,TestPutData_UInt64) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestPutData_UInt64());
}

TEST(DANStreamGTest,TestPutData_Int64) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestPutData_Int64());
}

TEST(DANStreamGTest,TestPutData_Float32) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestPutData_Float32());
}

TEST(DANStreamGTest,TestPutData_Float64) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestPutData_Float64());
}

TEST(DANStreamGTest,TestOpenStream) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestOpenStream());
}

TEST(DANStreamGTest,TestOpenStream_NoFinalise) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestOpenStream_NoFinalise());
}

TEST(DANStreamGTest,TestCloseStream) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestCloseStream());
}

TEST(DANStreamGTest,TestGetSignalMemoryBuffer) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(DANStreamGTest,TestReset) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestReset());
}

TEST(DANStreamGTest,TestSetAbsoluteTimeSignal) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestSetAbsoluteTimeSignal());
}

TEST(DANStreamGTest,TestSetRelativeTimeSignal) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestSetRelativeTimeSignal());
}

TEST(DANStreamGTest,TestSetAbsoluteStartTime) {
    DANStreamTest test;
    ASSERT_TRUE(test.TestSetAbsoluteStartTime());
}
