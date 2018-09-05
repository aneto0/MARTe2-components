/**
 * @file HistogramComparatorTGTest.cpp
 * @brief Source file for class HistogramComparatorTGTest
 * @date 30/08/2018
 * @author Giuseppe Ferro
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
 * the class HistogramComparatorTGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "HistogramComparatorTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(HistogramComparatorTGTest,TestInRange_U8_Min) {
    HistogramComparatorTTest<uint8> test;
    ASSERT_TRUE(test.TestInRange(10, 100, 11, 1, 0));
}

TEST(HistogramComparatorTGTest,TestInRange_U8_Max) {
    HistogramComparatorTTest<uint8> test;
    ASSERT_TRUE(test.TestInRange(10, 100, 11, 101, 10));
}

TEST(HistogramComparatorTGTest,TestInRange_U8_In) {
    HistogramComparatorTTest<uint8> test;
    ASSERT_TRUE(test.TestInRange(10, 100, 11, 50, 5));
}


TEST(HistogramComparatorTGTest,TestInRange_I8_Min) {
    HistogramComparatorTTest<int8> test;
    ASSERT_TRUE(test.TestInRange(-10, 80, 11, -11, 0));
}

TEST(HistogramComparatorTGTest,TestInRange_I8_Max) {
    HistogramComparatorTTest<int8> test;
    ASSERT_TRUE(test.TestInRange(-10, 80, 11, 81, 10));
}

TEST(HistogramComparatorTGTest,TestInRange_I8_In) {
    HistogramComparatorTTest<int8> test;
    ASSERT_TRUE(test.TestInRange(-10, 80, 11, 30, 5));
}



TEST(HistogramComparatorTGTest,TestInRange_U16_Min) {
    HistogramComparatorTTest<uint16> test;
    ASSERT_TRUE(test.TestInRange(100, 1000, 11, 99, 0));
}

TEST(HistogramComparatorTGTest,TestInRange_U16_Max) {
    HistogramComparatorTTest<uint16> test;
    ASSERT_TRUE(test.TestInRange(100, 1000, 11, 1001, 10));
}

TEST(HistogramComparatorTGTest,TestInRange_U16_In) {
    HistogramComparatorTTest<uint16> test;
    ASSERT_TRUE(test.TestInRange(100, 1000, 11, 501, 5));
}


TEST(HistogramComparatorTGTest,TestInRange_I16_Min) {
    HistogramComparatorTTest<int16> test;
    ASSERT_TRUE(test.TestInRange(-100, 800, 11, -101, 0));
}

TEST(HistogramComparatorTGTest,TestInRange_I16_Max) {
    HistogramComparatorTTest<int16> test;
    ASSERT_TRUE(test.TestInRange(-100, 800, 11, 801, 10));
}

TEST(HistogramComparatorTGTest,TestInRange_I16_In) {
    HistogramComparatorTTest<int16> test;
    ASSERT_TRUE(test.TestInRange(-100, 800, 11, 300, 5));
}


TEST(HistogramComparatorTGTest,TestInRange_U32_Min) {
    HistogramComparatorTTest<uint32> test;
    ASSERT_TRUE(test.TestInRange(1000, 10000, 11, 999, 0));
}

TEST(HistogramComparatorTGTest,TestInRange_U32_Max) {
    HistogramComparatorTTest<uint32> test;
    ASSERT_TRUE(test.TestInRange(1000, 10000, 11, 10001, 10));
}

TEST(HistogramComparatorTGTest,TestInRange_U32_In) {
    HistogramComparatorTTest<uint32> test;
    ASSERT_TRUE(test.TestInRange(1000, 10000, 11, 5001, 5));
}


TEST(HistogramComparatorTGTest,TestInRange_I32_Min) {
    HistogramComparatorTTest<int32> test;
    ASSERT_TRUE(test.TestInRange(-1000, 8000, 11, -1001, 0));
}

TEST(HistogramComparatorTGTest,TestInRange_I32_Max) {
    HistogramComparatorTTest<int32> test;
    ASSERT_TRUE(test.TestInRange(-1000, 8000, 11, 8001, 10));
}

TEST(HistogramComparatorTGTest,TestInRange_I32_In) {
    HistogramComparatorTTest<int32> test;
    ASSERT_TRUE(test.TestInRange(-1000, 8000, 11, 3000, 5));
}



TEST(HistogramComparatorTGTest,TestInRange_U64_Min) {
    HistogramComparatorTTest<uint64> test;
    ASSERT_TRUE(test.TestInRange(1000, 10000, 11, 999, 0));
}

TEST(HistogramComparatorTGTest,TestInRange_U64_Max) {
    HistogramComparatorTTest<uint64> test;
    ASSERT_TRUE(test.TestInRange(1000, 10000, 11, 10001, 10));
}

TEST(HistogramComparatorTGTest,TestInRange_U64_In) {
    HistogramComparatorTTest<uint64> test;
    ASSERT_TRUE(test.TestInRange(1000, 10000, 11, 5001, 5));
}


TEST(HistogramComparatorTGTest,TestInRange_I64_Min) {
    HistogramComparatorTTest<int64> test;
    ASSERT_TRUE(test.TestInRange(-1000, 8000, 11, -1001, 0));
}

TEST(HistogramComparatorTGTest,TestInRange_I64_Max) {
    HistogramComparatorTTest<int64> test;
    ASSERT_TRUE(test.TestInRange(-1000, 8000, 11, 8001, 10));
}

TEST(HistogramComparatorTGTest,TestInRange_I64_In) {
    HistogramComparatorTTest<int64> test;
    ASSERT_TRUE(test.TestInRange(-1000, 8000, 11, 3000, 5));
}

TEST(HistogramComparatorTGTest,TestSetMin) {
    HistogramComparatorTTest<uint8> test;
    ASSERT_TRUE(test.TestSetMin());
}

TEST(HistogramComparatorTGTest,TestSetMax) {
    HistogramComparatorTTest<uint8> test;
    ASSERT_TRUE(test.TestSetMax());
}

TEST(HistogramComparatorTGTest,TestSetNumberOfBins) {
    HistogramComparatorTTest<uint8> test;
    ASSERT_TRUE(test.TestSetNumberOfBins());
}
