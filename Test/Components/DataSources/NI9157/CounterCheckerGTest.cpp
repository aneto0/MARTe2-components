/**
 * @file CounterCheckerGTest.cpp
 * @brief Source file for class CounterCheckerGTest.
 * @date 26/03/2021
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
 * the class CounterCheckerGTest (public, protected, and private). Be aware
 * that some methods, such as those inline could be defined on the header
 * file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CounterCheckerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(CounterCheckerGTest,TestConstructor) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(CounterCheckerGTest,TestInitialise) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(CounterCheckerGTest,TestInitialise_FailSampleChecker) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_FailSampleChecker());
}

TEST(CounterCheckerGTest,TestInitialise_FailNumOfFrameForSync) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_FailNumOfFrameForSync());
}

TEST(CounterCheckerGTest,TestInitialise_NoFirstPacketCounter) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_NoFirstPacketCounter());
}

TEST(CounterCheckerGTest,TestInitialise_NoAcquireFromCounter) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_NoAcquireFromCounter());
}

TEST(CounterCheckerGTest,TestInitialise_NoCounterStep) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_NoCounterStep());
}

TEST(CounterCheckerGTest,TestInitialise_CounterStepZero) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_CounterStepZero());
}

TEST(CounterCheckerGTest,TestInitialise_NoCheckCounterAfterNSteps) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_NoCheckCounterAfterNSteps());
}

TEST(CounterCheckerGTest,TestInitialise_NoCheckCounterAfterNStepsDivCounterStep) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_NoCheckCounterAfterNStepsDivCounterStep());
}

TEST(CounterCheckerGTest,TestInitialise_NoAcquireFromCounterMinusPacketCounterDivCounterStep) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_NoAcquireFromCounterMinusPacketCounterDivCounterStep());
}

TEST(CounterCheckerGTest,TestCheck) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestCheck());
}

TEST(CounterCheckerGTest,TestCheck_FalseWrite) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestCheck_FalseWrite());
}

TEST(CounterCheckerGTest,TestCheck_ReturnFalse) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestCheck_ReturnFalse());
}

TEST(CounterCheckerGTest,TestCheck_ConsecutiveCalls) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestCheck_ConsecutiveCalls());
}

TEST(CounterCheckerGTest,TestSynchronise) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(CounterCheckerGTest,TestSynchronise_WriteFalse) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestSynchronise_WriteFalse());
}

TEST(CounterCheckerGTest,TestSynchronise_FalseSizeToReadZero) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestSynchronise_FalseSizeToReadZero());
}

TEST(CounterCheckerGTest,TestSynchronise_FalseFrameIsWrong) {
    CounterCheckerTest test;
    ASSERT_TRUE(test.TestSynchronise_FalseFrameIsWrong());
}
