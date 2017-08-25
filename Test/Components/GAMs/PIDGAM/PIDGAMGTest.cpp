/**
 * @file FilterGAMGTest.cpp
 * @brief Source file for class FilterGAMGTest
 * @date 22/08/2017
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
 * the class FilterGAMGTest (public, protected, and private). Be aware that some
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

#include "PIDGAM.h"
#include "PIDGAMTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

TEST(PIDGAMGTest,TestInitialiseMissingKpKiKd) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestInitialiseMissingKpKiKd());
}

TEST(PIDGAMGTest,TestInitialiseMissingSampleTime) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestInitialiseMissingSampleTime());
}

TEST(PIDGAMGTest,TestInitialiseWrongSampleTime) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestInitialiseWrongSampleTime());
}

TEST(PIDGAMGTest,TestInitialiseWrongSaturationLimits) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestInitialiseWrongSaturationLimits());
}

TEST(PIDGAMGTest,TestInitialiseWrongSaturationLimits2) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestInitialiseWrongSaturationLimits2());
}

TEST(PIDGAMGTest,TestInitialise) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(PIDGAMGTest,TestSetupWrongNumberOfInputs) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongNumberOfInputs());
}

TEST(PIDGAMGTest,TestSetupWrongNumberOfInputs2) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongNumberOfInputs2());
}

TEST(PIDGAMGTest,TestSetupWrongNumberOfOutputs) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongNumberOfOutputs());
}

TEST(PIDGAMGTest,TestSetupWrongNumberOfOutputs2) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongNumberOfOutputs2());
}

TEST(PIDGAMGTest,TestSetupNoInputElementsReference) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputElementsReference());
}

TEST(PIDGAMGTest,TestSetupWrongInputElementsReference) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputElementsReference());
}

TEST(PIDGAMGTest,TestSetupNoInputElementsMeasurement) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputElementsMeasurement());
}

TEST(PIDGAMGTest,TestSetupWrongInputElementsMeasurement) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputElementsMeasurement());
}

TEST(PIDGAMGTest,TestSetupNoOutputElements) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputElements());
}

TEST(PIDGAMGTest,TestSetupWrongOutputElements) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputElements());
}

TEST(PIDGAMGTest,TestSetupNoInputSamplesReference) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputSamplesReference());
}

TEST(PIDGAMGTest,TestSetupWrongInputSamplesReference) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputSamplesReference());
}

TEST(PIDGAMGTest,TestSetupNoInputSamplesMeasurement) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputSamplesMeasurement());
}

TEST(PIDGAMGTest,TestSetupWrongInputSamplesMeasurement) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputSamplesMeasurement());
}

TEST(PIDGAMGTest,TestSetupNoOutputSamples) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputSamples());
}

TEST(PIDGAMGTest,TestSetupWrongOutputSamples) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputSamples());
}

TEST(PIDGAMGTest,TestSetupNoInputDimensionReference) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputDimensionReference());
}

TEST(PIDGAMGTest,TestSetupWrongInputDimensionReference) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputDimensionReference());
}

TEST(PIDGAMGTest,TestSetupNoInputDimensionMeasurement) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputDimensionMeasurement());
}

TEST(PIDGAMGTest,TestSetupWrongInputDimensionMeasurement) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputDimensionMeasurement());
}

TEST(PIDGAMGTest,TestSetupNoOutputDimension) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputDimension());
}

TEST(PIDGAMGTest, TestSetupWrongOutputDimension) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputDimension());
}

TEST(PIDGAMGTest, TestSetupNoReferenceInputType) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupNoReferenceInputType());
}

TEST(PIDGAMGTest, TestSetupWrongReferenceInputType) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongReferenceInputType());
}

TEST(PIDGAMGTest, TestSetupNoMeasurementInputType) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupNoMeasurementInputType());
}

TEST(PIDGAMGTest, TestSetupWrongMeasurementInputType) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongMeasurementInputType());
}

TEST(PIDGAMGTest, TestSetupNoOutputType) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputType());
}

TEST(PIDGAMGTest, TestSetupWrongOutputType) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputType());
}

TEST(PIDGAMGTest, TestSetup1InputSignal) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetup1InputSignal());
}

TEST(PIDGAMGTest, TestSetup2InputSignals) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestSetup2InputSignals());
}

TEST(PIDGAMGTest, TestExecutekpSubtract) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecutekpSubtract());
}

TEST(PIDGAMGTest, TestExecutekp) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecutekp());
}

TEST(PIDGAMGTest, TestExecutekiSubtract) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecutekiSubtract());
}

TEST(PIDGAMGTest, TestExecuteki) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecuteki());
}

TEST(PIDGAMGTest, TestExecutekiSubtract2) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecutekiSubtract2());
}

TEST(PIDGAMGTest, TestExecuteki2) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecuteki2());
}

TEST(PIDGAMGTest, TestExecutekdSubtract) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecutekdSubtract());
}

TEST(PIDGAMGTest, TestExecutekd) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecutekd());
}

TEST(PIDGAMGTest, TestExecutekpkikdSubtract) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecutekpkikdSubtract());
}

TEST(PIDGAMGTest, TestExecutekpkikdCompareBoth) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecutekpkikdCompareBoth());
}

TEST(PIDGAMGTest, TestExecuteSaturationkp) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecuteSaturationkp());
}

TEST(PIDGAMGTest, TestExecuteSaturationki) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecuteSaturationki());
}

TEST(PIDGAMGTest, TestExecuteSaturationki2) {
    PIDGAMTest test;
    ASSERT_TRUE(test.TestExecuteSaturationki2());
}


