/**
 * @file StatisticsGAMGTest.cpp
 * @brief Source file for class StatisticsGAMGTest
 * @date 30/01/2017
 * @author Llorenc Capella
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
 * the class StatisticsGAMGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StatisticsGAM.h"
#include "StatisticsGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(StatisticsGAMGTest,TestSquareRoot) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestSquareRoot());
}

TEST(StatisticsGAMGTest,TestConstructor) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(StatisticsGAMGTest,TestInitialise) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(StatisticsGAMGTest,TestInitialise_WindowSize) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestInitialise_WindowSize());
}

TEST(StatisticsGAMGTest,TestSetup_float32) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestSetup_float32());
}

TEST(StatisticsGAMGTest,TestSetup_NoInputSignal) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestSetup_NoInputSignal());
}

TEST(StatisticsGAMGTest,TestSetup_NoOutputSignal) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestSetup_NoOutputSignal());
}

TEST(StatisticsGAMGTest,TestSetup_NotAScalarInput) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestSetup_NotAScalarInput());
}

TEST(StatisticsGAMGTest,TestSetup_NotAScalarOutput) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestSetup_NotAScalarOutput());
}

TEST(StatisticsGAMGTest,TestSetup_DistinctTypes_1) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestSetup_DistinctTypes_1());
}

TEST(StatisticsGAMGTest,TestSetup_DistinctTypes_2) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestSetup_DistinctTypes_2());
}

TEST(StatisticsGAMGTest,TestExecute_Error) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_Error());
}

TEST(StatisticsGAMGTest,TestExecute_uint8) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_uint8());
}

TEST(StatisticsGAMGTest,TestExecute_int8) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_int8());
}

TEST(StatisticsGAMGTest,TestExecute_uint16) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_uint16());
}

TEST(StatisticsGAMGTest,TestExecute_int16) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_int16());
}

TEST(StatisticsGAMGTest,TestExecute_int32) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_int32());
}

TEST(StatisticsGAMGTest,TestExecute_uint32) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_uint32());
}

TEST(StatisticsGAMGTest,TestExecute_int64) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_int64());
}

TEST(StatisticsGAMGTest,TestExecute_uint64) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_uint64());
}

TEST(StatisticsGAMGTest,TestExecute_float32) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_float32());
}

TEST(StatisticsGAMGTest,TestExecute_float64) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_float64());
}

TEST(StatisticsGAMGTest,TestPrepareForNextState_Error) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestPrepareForNextState_Error());
}

TEST(StatisticsGAMGTest,TestPrepareForNextState_Success) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestPrepareForNextState_Success());
}

TEST(StatisticsGAMGTest,TestExecute_uint32_withCycleCounter) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_uint32_withCycleCounter());
}

TEST(StatisticsGAMGTest,TestExecute_uint32_withAbsoluteMaxMin) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute_uint32_withAbsoluteMaxMin());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

