/**
 * @file MuxGAMGTest.cpp
 * @brief Source file for class MuxGAMGTest
 * @date 28/08/2017
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
 * the class MuxGAMGTest (public, protected, and private). Be aware that some 
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
#include "MuxGAM.h"
#include "MuxGAMTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

TEST(MuxGAMGTest,TestInitialise) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(MuxGAMGTest,TestSetupNoOutputs) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputs());
}

TEST(MuxGAMGTest,TestSetupNoInputs) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputs());
}

TEST(MuxGAMGTest,TestSetupWrongInputs) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputs());
}

TEST(MuxGAMGTest,TestSetupWrongInputs2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputs2());
}

TEST(MuxGAMGTest,TestSetupWrongInputs3) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputs3());
}

TEST(MuxGAMGTest,TestSetupNoSelectorType) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupNoSelectorType());
}

TEST(MuxGAMGTest,TestSetupWrongSelectorType) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongSelectorType());
}

TEST(MuxGAMGTest,TestSetupWrongInputType) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputType());
}

TEST(MuxGAMGTest,TestSetupWrongSelectorType2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongSelectorType2());
}

TEST(MuxGAMGTest,TestSetupWrongInputType2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputType2());
}

TEST(MuxGAMGTest,TestSetupWrongOutputType) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputType());
}

TEST(MuxGAMGTest,TestSetupWrongOutputType2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputType2());
}

TEST(MuxGAMGTest,TestSetupNoOutputElements) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputElements());
}

TEST(MuxGAMGTest,TestSetupNoOutputElements_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputElements_2());
}

TEST(MuxGAMGTest,TestSetupWrongOutputElements) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputElements());
}

TEST(MuxGAMGTest,TestSetupWrongOutputElements2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputElements2());
}

TEST(MuxGAMGTest,TestSetupNoSelectorElements) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupNoSelectorElements());
}

TEST(MuxGAMGTest,TestSetupWrongSelectorElements) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongSelectorElements());
}

TEST(MuxGAMGTest,TestSetupWrongSelectorElements2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongSelectorElements2());
}

TEST(MuxGAMGTest,TestSetupNoInputElements) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputElements());
}

TEST(MuxGAMGTest,TestSetupWrongInputElements) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputElements());
}

TEST(MuxGAMGTest,TestSetupWrongInputElements2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputElements2());
}

TEST(MuxGAMGTest,TestSetupNoInputDiemnsion) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputDiemnsion());
}

TEST(MuxGAMGTest,TestSetupWrongInputDiemnsion) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputDiemnsion());
}

TEST(MuxGAMGTest,TestSetupWrongInputDiemnsion2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputDiemnsion2());
}

TEST(MuxGAMGTest,TestSetupNoOutputDiemnsion) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputDiemnsion());
}

TEST(MuxGAMGTest,TestSetupWrongOutputDiemnsion) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputDiemnsion());
}

TEST(MuxGAMGTest,TestSetupWrongOutputDiemnsion2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputDiemnsion2());
}

TEST(MuxGAMGTest,TestSetupNoInputSamples) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputSamples());
}

TEST(MuxGAMGTest,TestSetupWrongInputSamples) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputSamples());
}

TEST(MuxGAMGTest,TestSetupWrongInputSamples2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputSamples2());
}

TEST(MuxGAMGTest,TestSetupWrongInputSamples3) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputSamples3());
}

TEST(MuxGAMGTest,TestSetupWrongInputSamples4) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputSamples4());
}

TEST(MuxGAMGTest,TestSetupNoOutputSamples) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputSamples());
}

TEST(MuxGAMGTest,TestSetupWrongOutputSamples) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputSamples());
}

TEST(MuxGAMGTest,TestSetupWrongOutputSamples2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputSamples2());
}

TEST(MuxGAMGTest,TestSetup4I2Ouint8) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("uint8", 1, 1));
}

TEST(MuxGAMGTest,TestSetup4I2Oint8) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("int8", 1, 1));
}
TEST(MuxGAMGTest,TestSetup4I2Ouint16) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("uint16", 1, 1));
}

TEST(MuxGAMGTest,TestSetup4I2Oint16) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("int16", 1, 1));
}

TEST(MuxGAMGTest,TestSetup4I2Ouint32) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("uint32", 1, 1));
}

TEST(MuxGAMGTest,TestSetup4I2Oint32) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("int32", 1, 1));
}

TEST(MuxGAMGTest,TestSetup4I2Ouint64) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("uint64", 1, 1));
}

TEST(MuxGAMGTest,TestSetup4I2Oint64) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("int64", 1, 1));
}

TEST(MuxGAMGTest,TestSetup4I2Ofloat32) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("float32", 1, 1));
}

TEST(MuxGAMGTest,TestSetup4I2Ofloat64) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("float64", 1, 1));
}

TEST(MuxGAMGTest,TestSetup4I2Ouint8_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("uint8", 10, 10));
}

TEST(MuxGAMGTest,TestSetup4I2Oint8_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("int8", 10, 10));
}
TEST(MuxGAMGTest,TestSetup4I2Ouint16_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("uint16", 10, 10));
}

TEST(MuxGAMGTest,TestSetup4I2Oint16_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("int16", 10, 10));
}

TEST(MuxGAMGTest,TestSetup4I2Ouint32_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("uint32", 10, 10));
}

TEST(MuxGAMGTest,TestSetup4I2Oint32_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("int32", 10, 10));
}

TEST(MuxGAMGTest,TestSetup4I2Ouint64_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("uint64", 10, 10));
}

TEST(MuxGAMGTest,TestSetup4I2Oint64_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("int64", 10, 10));
}

TEST(MuxGAMGTest,TestSetup4I2Ofloat32_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("float32", 10, 10));
}

TEST(MuxGAMGTest,TestSetup4I2Ofloat64_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestSetup4I2O("float64", 10, 10));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayuint8) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray<uint8>("uint8", 10, 10));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayint8) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray<int8>("int8", 10, 10));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayuint16) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray<uint16>("uint16", 10, 10));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayint16) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray<int16>("int16", 10, 10));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayuint32) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray<uint32>("uint32", 10, 10));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayint32) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray<int32>("int32", 10, 10));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayuint64) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray<uint64>("uint64", 10, 10));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayint64) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray<int64>("int64", 10, 10));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayfloat32) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray<float32>("float32", 10, 10));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayfloat64) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray<float64>("float64", 10, 10));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayuint8_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray_2<uint8>("uint8", 80, 80));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayint8_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray_2<int8>("int8", 80, 80));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayuint16_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray_2<uint16>("uint16", 80, 80));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayint16_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray_2<int16>("int16", 80, 80));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayuint32_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray_2<uint32>("uint32", 80, 80));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayint32_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray_2<int32>("int32", 80, 80));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayuint64_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray_2<uint64>("uint64", 80, 80));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayint64_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray_2<int64>("int64", 80, 80));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayfloat32_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray_2<float32>("float32", 80, 80));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorArrayfloat64_2) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelectorArray_2<float64>("float64", 80, 80));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectoruint8) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelector<uint8>("uint8", 80, 1));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorint8) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelector<int8>("int8", 80, 1));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectoruint16) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelector<uint16>("uint16", 80, 1));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorint16) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelector<int16>("int16", 80, 1));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectoruint32) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelector<uint32>("uint32", 80, 1));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorint32) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelector<int32>("int32", 80, 1));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectoruint64) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelector<uint64>("uint64", 80, 1));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorint64) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelector<int64>("int64", 80, 1));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorfloat32) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelector<float32>("float32", 80, 1));
}

TEST(MuxGAMGTest,TestExecute4I2OSelectorfloat64) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OSelector<float64>("float64", 80, 1));
}

TEST(MuxGAMGTest,TestExecute4I2OInvalidSelectorArray) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OInvalidSelector<float64>("float64", 80, 80));
}

TEST(MuxGAMGTest,TestExecute4I2OInvalidSelectorSingle) {
    MuxGAMTest test;
    ASSERT_TRUE(test.TestExecute4I2OInvalidSelector<float64>("float64", 80, 1));
}





/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
