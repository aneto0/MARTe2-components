/**
 * @file TimeCorrectionGAMGTest.cpp
 * @brief Source file for class TimeCorrectionGAMGTest
 * @date 07/06/2018
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
 * the class TimeCorrectionGAMGTest (public, protected, and private). Be aware that some 
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
#include "TimeCorrectionGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(TimeCorrectionGAMGTest,TestConstructor) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(TimeCorrectionGAMGTest,TestInitialise) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(TimeCorrectionGAMGTest,TestInitialise_FalseNoExpectedDelta) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoExpectedDelta());
}


TEST(TimeCorrectionGAMGTest,TestInitialise_FalseNoDeltaTolerance) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoDeltaTolerance());
}

TEST(TimeCorrectionGAMGTest,TestInitialise_FalseNoFilterGain) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoFilterGain());
}

TEST(TimeCorrectionGAMGTest,TestSetup) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(TimeCorrectionGAMGTest,TestSetup_OneOutputSignal) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestSetup_OneOutputSignal());
}

TEST(TimeCorrectionGAMGTest,TestSetup_FalseNumberOfInputSignals) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseNumberOfInputSignals());
}

TEST(TimeCorrectionGAMGTest,TestSetup_FalseNumberOfOutputSignals) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseNumberOfOutputSignals());
}

TEST(TimeCorrectionGAMGTest,TestSetup_FalseBadInputType) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseBadInputType());
}

TEST(TimeCorrectionGAMGTest,TestSetup_FalseBadOutputType1) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseBadOutputType1());
}

TEST(TimeCorrectionGAMGTest,TestSetup_FalseBadOutputType2) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseBadOutputType2());
}

TEST(TimeCorrectionGAMGTest,TestSetup_FalseBadInputNElements) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseBadInputNElements());
}

TEST(TimeCorrectionGAMGTest,TestSetup_FalseBadOutputNElements1) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseBadOutputNElements1());
}

TEST(TimeCorrectionGAMGTest,TestSetup_FalseBadOutputNElements2) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseBadOutputNElements2());
}

TEST(TimeCorrectionGAMGTest,TestExecute) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(TimeCorrectionGAMGTest,TestExecute_OneOutputSignal) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestExecute_OneOutputSignal());
}

TEST(TimeCorrectionGAMGTest,TestExecute_EstimationConsecutiveJumps) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestExecute_EstimationConsecutiveJumps());
}

TEST(TimeCorrectionGAMGTest,TestExecute_EstimationSlowChange) {
    TimeCorrectionGAMTest test;
    ASSERT_TRUE(test.TestExecute_EstimationSlowChange());
}
