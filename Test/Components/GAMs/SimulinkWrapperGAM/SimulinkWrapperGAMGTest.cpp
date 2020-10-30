/**
 * @file SimulinkWrapperGAMGTest.cpp
 * @brief Source file for class SimulinkWrapperGAMGTest
 * @date 10/08/2020
 * @author RFX
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
 * the class SimulinkWrapperGAMGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SimulinkWrapperGAM.h"
#include "SimulinkWrapperGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


TEST(SimulinkWrapperGAMGTest, TestConstructor) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_MissingOptionalConfigurationSettings) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingOptionalConfigurationSettings());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_MissingTunableParamExternalSource) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingTunableParamExternalSource());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_MissingParametersLeaf) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingParametersLeaf());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_WrongNonVirtualBusMode) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_WrongNonVirtualBusMode());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_LoadLibrary) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_LoadLibrary());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_MissingLibrary) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_MissingLibrary());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_MissingSymbolPrefix) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_MissingSymbolPrefix());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_LoadSymbols) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_LoadSymbols());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_LibraryMissingGetMmiFunction) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_LibraryMissingGetMmiFunction());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_LibraryMissingAllocFunction) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_LibraryMissingAllocFunction());
}

TEST(SimulinkWrapperGAMGTest, TestSetup) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_StructTunableParameters_1) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_StructTunableParameters_1());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_StructTunableParameters_2) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_StructTunableParameters_2());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_StructTunableParameters_3) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_StructTunableParameters_3());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_StructTunableParametersFromExternalSource) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_StructTunableParametersFromExternalSource());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_NoTunableParameters) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_NoTunableParameters());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_SkipInvalidTunableParams) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_SkipInvalidTunableParams());
}


TEST(SimulinkWrapperGAMGTest, TestSetup_WithStructSignals) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_WithStructSignals());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_WithNestedStructSignals) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_WithNestedStructSignals());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_DontSkipUnlinkedTunableParams) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_DontSkipUnlinkedTunableParams());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_WrongNumberOfInputs) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_WrongNumberOfInputs());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_WrongNumberOfOutputs) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_WrongNumberOfOutputs());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_WrongInputName) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_WrongInputName());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_WrongOutputName) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_WrongOutputName());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_WrongNumberOfElements) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_WrongNumberOfElements());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_WrongNumberOfDimensions) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_WrongNumberOfDimensions());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_WrongDatatype) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_WrongDatatype());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_ParamWrongNumberOfDimensions) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_ParamWrongNumberOfDimensions());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_ParamWrongDimensions) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_ParamWrongDimensions());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_ParamWrongDimensions_Matrix) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_ParamWrongDimensions_Matrix());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_ParamWrongDatatype) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_ParamWrongDatatype());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_StructArraysAsParams) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_StructArraysAsParams());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_NestedStructArraysAsParams) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_NestedStructArraysAsParams());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_NoInputs) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_NoInputs());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_NoOutputs) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_NoOutputs());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_WrongNumberOfDimensionsWithStructSignals) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_WrongNumberOfDimensionsWithStructSignals());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_Failed_WrongDatatypeWithStructSignals) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_WrongDatatypeWithStructSignals());
}

TEST(SimulinkWrapperGAMGTest, TestParameterActualisation_RowMajorModel) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestParameterActualisation_RowMajorModel());
}

TEST(SimulinkWrapperGAMGTest, TestParameterActualisation_ColumnMajorModel) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestParameterActualisation_ColumnMajorModel());
}

TEST(SimulinkWrapperGAMGTest, TestParameterActualisation_Uint) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestParameterActualisation_Uint());
}

TEST(SimulinkWrapperGAMGTest, TestParameterActualisation_Int) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestParameterActualisation_Int());
}

TEST(SimulinkWrapperGAMGTest, TestParameterActualisation_Float) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestParameterActualisation_Float());
}

TEST(SimulinkWrapperGAMGTest, TestExecute) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(SimulinkWrapperGAMGTest, TestPrintAlgoInfo) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestPrintAlgoInfo());
}

TEST(SimulinkWrapperGAMGTest, Test_StructuredSignals) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.Test_StructuredSignals());
}

TEST(SimulinkWrapperGAMGTest, Test_StructuredSignals_Failed) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.Test_StructuredSignals_Failed());
}

TEST(SimulinkWrapperGAMGTest, TestExecute_WithStructuredSignals) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestExecute_WithStructuredSignals());
}

TEST(SimulinkWrapperGAMGTest, Test_MultiMixedSignalsTranspose) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.Test_MultiMixedSignalsTranspose(true));
}

TEST(SimulinkWrapperGAMGTest, Test_MultiMixedSignalsNorm) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.Test_MultiMixedSignalsTranspose(false));
}

TEST(SimulinkWrapperGAMGTest, TestSetup_WithNotFoundParameterAndProcessUnlinked_Failed) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_WithNotFoundParameter_Failed(true));
}

TEST(SimulinkWrapperGAMGTest, TestSetup_WithNotFoundParameterAndSkipInvalid) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_WithNotFoundParameter_Failed(false));
}

TEST(SimulinkWrapperGAMGTest, TestSetup_WithNestedSingleSignals) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_WithNestedSingleSignals());
}

TEST(SimulinkWrapperGAMGTest, TestSetup_StructTunableParametersFromExternalSource_Failed) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup_StructTunableParametersFromExternalSource_Failed());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

