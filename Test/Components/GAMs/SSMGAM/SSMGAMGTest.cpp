/**
 * @file SSMGAMGTest.cpp
 * @brief Source file for class SSMGAMGTest
 * @date 01/09/2017
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
 * the class SSMGAMGTest (public, protected, and private). Be aware that some 
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
#include "SSMGAM.h"
#include "SSMGAMTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

TEST(SSMGAMGTest,TestInitialiseNoStateMatrix) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialiseNoStateMatrix());
}

TEST(SSMGAMGTest,TestInitialiseStateMatrix1x2) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialiseStateMatrix1x2());
}

TEST(SSMGAMGTest,TestInitialiseNoInputMatrix) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialiseNoInputMatrix());
}

TEST(SSMGAMGTest,TestInitialiseWrongInputMatrixDimension) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialiseWrongInputMatrixDimension());
}

TEST(SSMGAMGTest,TestInitialiseNoOutputMatrix) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialiseNoOutputMatrix());
}

TEST(SSMGAMGTest,TestInitialiseWrongOutputMatrixDimension) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialiseWrongOutputMatrixDimension());
}

TEST(SSMGAMGTest,TestInitialiseNoFeedthoughMatrix) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialiseNoFeedthoughMatrix());
}

TEST(SSMGAMGTest,TestInitialiseWrongFeedthroughMatrixDimension) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialiseWrongFeedthroughMatrixDimension());
}

TEST(SSMGAMGTest,TestInitialiseWrongFeedthroughMatrixDimension_2) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialiseWrongFeedthroughMatrixDimension_2());
}

TEST(SSMGAMGTest, TestInitialiseNoResetInEachState) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialiseNoResetInEachState());
}

TEST(SSMGAMGTest, TestInitialiseWrongResetInEachState) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialiseWrongResetInEachState());
}

TEST(SSMGAMGTest,TestInitialise) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(SSMGAMGTest,TestInitialise_2) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(SSMGAMGTest,TestSetupWrongInputSignals) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputSignals());
}

TEST(SSMGAMGTest,TestSetupWrongOutputsSignals) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputsSignals());
}

TEST(SSMGAMGTest,TestSetupWrongInputType) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputType());
}

TEST(SSMGAMGTest,TestSetupWrongOutputType) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputType());
}

TEST(SSMGAMGTest,TestSetupNoInputElements) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputElements());
}

TEST(SSMGAMGTest,TestSetupWrongInputElements) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputElements());
}

TEST(SSMGAMGTest,TestSetupNoOutputElements) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputElements());
}

TEST(SSMGAMGTest,TestSetupWrongOutputElements) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputElements());
}

TEST(SSMGAMGTest,TestSetupNoInputDimensions) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputDimensions());
}

TEST(SSMGAMGTest,TestSetupWrongInputDimensions) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputDimensions());
}

TEST(SSMGAMGTest,TestSetupNoOutputDimensions) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputDimensions());
}

TEST(SSMGAMGTest,TestSetupWrongOutputDimensions) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputDimensions());
}

TEST(SSMGAMGTest,TestSetupNoInputSamples) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputSamples());
}

TEST(SSMGAMGTest,TestSetupWrongInputSamples) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputSamples());
}

TEST(SSMGAMGTest,TestSetupNoOutputSamples) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputSamples());
}

TEST(SSMGAMGTest,TestSetupWrongOutputSamples) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputSamples());
}

TEST(SSMGAMGTest,TestSetupWrongOutputSamples_2) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputSamples_2());
}

TEST(SSMGAMGTest,TestSetupWrongOutputSamples_3) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputSamples_3());
}

TEST(SSMGAMGTest,TestSetupNoMemoryInitialised) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetupNoMemoryInitialised());
}

TEST(SSMGAMGTest,TestSetup) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(SSMGAMGTest,TestExecute) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(SSMGAMGTest,TestExecuteSpring) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestExecuteSpring());
}

TEST(SSMGAMGTest, TestExecuteSpringNoFeedthroughMatrix) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestExecuteSpringNoFeedthroughMatrix());
}

TEST(SSMGAMGTest, TestPrepareNextStateReset) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestPrepareNextStateReset(1));
}

TEST(SSMGAMGTest, TestPrepareNextStateReset1) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestPrepareNextStateReset(0));
}

TEST(SSMGAMGTest, TestPrepareNextStateNoReset) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestPrepareNextStateNoReset(0));
}

TEST(SSMGAMGTest, TestPrepareNextStateNoMemoryAllocated) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestPrepareNextStateNoMemoryAllocated(0));
}

TEST(SSMGAMGTest, TestPrepareNextStateNoMemoryAllocated2) {
    SSMGAMTest test;
    ASSERT_TRUE(test.TestPrepareNextStateNoMemoryAllocated(1));
}


}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
