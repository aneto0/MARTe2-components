/**
 * @file HistogramGAMGTest.cpp
 * @brief Source file for class HistogramGAMGTest
 * @date 31/08/2018
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
 * the class HistogramGAMGTest (public, protected, and private). Be aware that some 
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
#include "HistogramGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(HistogramGAMGTest,TestConstructor) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(HistogramGAMGTest,TestInitialise) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(HistogramGAMGTest,TestSetup) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(HistogramGAMGTest,TestSetup_DifferentInputOutputNSignals) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestSetup_DifferentInputOutputNSignals());
}

TEST(HistogramGAMGTest,TestSetup_FalseInputNotScalar) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseInputNotScalar());
}

TEST(HistogramGAMGTest,TestSetup_FalseSamplesNotOne) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseSamplesNotOne());
}

TEST(HistogramGAMGTest,TestSetup_UndefinedMaxLim) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestSetup_UndefinedMaxLim());
}

TEST(HistogramGAMGTest,TestSetup_UndefinedMinLim) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestSetup_UndefinedMinLim());
}

TEST(HistogramGAMGTest,TestSetup_NBindMinorThan3) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestSetup_NBindMinorThan3());
}

TEST(HistogramGAMGTest,TestSetup_OutputTypeNotUint32) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestSetup_OutputTypeNotUint32());
}

TEST(HistogramGAMGTest,TestExecute) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(HistogramGAMGTest,TestExecute_BeginCycleNumber) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestExecute_BeginCycleNumber());
}

TEST(HistogramGAMGTest,TestPrepareNextState) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(HistogramGAMGTest,TestPrepareNextState_Reset_All) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestPrepareNextState_Reset_All());
}

TEST(HistogramGAMGTest,TestPrepareNextState_Reset_State) {
    HistogramGAMTest test;
    ASSERT_TRUE(test.TestPrepareNextState_Reset_State());
}


