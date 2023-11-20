/**
 * @file FilterGAMGTest.cpp
 * @brief Source file for class FilterGAMGTest
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

#include "FilterGAM.h"
#include "FilterGAMTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
TEST(FilterGAMGTest,TestInitialise_GAMInitialiseFail) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_GAMInitialiseFail());
}

TEST(FilterGAMGTest,TestInitialise_ZeroInputSignals) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_ZeroInputSignals());
}

TEST(FilterGAMGTest,TestInitialise_UnsupportedInputType) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_UnsupportedInputType());
}

TEST(FilterGAMGTest,TestInitialise_NoNum) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_NoNum());
}

TEST(FilterGAMGTest,TestInitialise_EmptyNumFloat32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_EmptyNum<float32>());
}

TEST(FilterGAMGTest,TestInitialise_EmptyNumFloat64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_EmptyNum<float64>());
}

TEST(FilterGAMGTest,TestInitialise_EmptyNumInt32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_EmptyNum<int32>());
}

TEST(FilterGAMGTest,TestInitialise_EmptyNumInt4) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_EmptyNum<int64>());
}

TEST(FilterGAMGTest,TestInitialise_EmptyDenFlota32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_EmptyDen<float32>());
}

TEST(FilterGAMGTest,TestInitialise_EmptyDenFlota64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_EmptyDen<float64>());
}

TEST(FilterGAMGTest,TestInitialise_EmptyDenInt32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_EmptyDen<int32>());
}

TEST(FilterGAMGTest,TestInitialise_EmptyDenInt64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_EmptyDen<int64>());
}

TEST(FilterGAMGTest,TestInitialise_WrongFilterCoeff) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_WrongFilterCoeff());
}

TEST(FilterGAMGTest,TestInitialise_WrongRestInEachState) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_WrongRestInEachState());
}

TEST(FilterGAMGTest,TestInitialise_CheckNormalisationBeforeInitialise) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise_CheckNormalisationBeforeInitialise());
}

TEST(FilterGAMGTest,TestConstructor) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FilterGAMGTest,TestInitialiseFloat32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise<float32>());
}

TEST(FilterGAMGTest,TestInitialiseFloat64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise<float64>());
}

TEST(FilterGAMGTest,TestInitialiseInt32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise<int32>());
}

TEST(FilterGAMGTest,TestInitialiseInt64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialise<int64>());
}

TEST(FilterGAMGTest,TestInitialiseNoResetInEachState) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialiseNoResetInEachState());
}

TEST(FilterGAMGTest,TestInitialiseWrongResetInEachState) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialiseWrongResetInEachState());
}

TEST(FilterGAMGTest,TestInitialiseNoNum) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialiseNoNum());
}

TEST(FilterGAMGTest,TestInitialiseWrongNumType) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialiseWrongNumType());
}

TEST(FilterGAMGTest,TestInitialiseNoDen) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialiseNoDen());
}

TEST(FilterGAMGTest,TestInitialiseWrongDenType) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestInitialiseWrongDenType());
}

TEST(FilterGAMGTest,TestStaticGainFIRFloat32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestStaticGainFIR<float32>());
}
TEST(FilterGAMGTest,TestStaticGainFIRFloat64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestStaticGainFIR<float64>());
}
TEST(FilterGAMGTest,TestStaticGainFIRInt32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestStaticGainFIR<int32>());
}
TEST(FilterGAMGTest,TestStaticGainFIRInt64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestStaticGainFIR<int64>());
}

TEST(FilterGAMGTest,TestStaticGainIIR) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestStaticGainIIR<float32>());
}

TEST(FilterGAMGTest,TestFailNormalise) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestFailNormalise());
}

TEST(FilterGAMGTest,TestSetup) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetup<float32>());
}

TEST(FilterGAMGTest,TestSetupWrongInputSignalTypeFloat32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputSignalType<float32>());
}
TEST(FilterGAMGTest,TestSetupWrongInputSignalTypeFloat64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputSignalType<float64>());
}
TEST(FilterGAMGTest,TestSetupWrongInputSignalTypeInt32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputSignalType<int32>());
}
TEST(FilterGAMGTest,TestSetupWrongInputSignalTypeInt64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputSignalType<int64>());
}

TEST(FilterGAMGTest,TestSetupWrongOutputSignalType) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputSignalType<float32>());
}

TEST(FilterGAMGTest,TestSetupNoInputSignal) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputSignal());
}

TEST(FilterGAMGTest,TestSetupNoOutputSignal) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputSignal());
}

TEST(FilterGAMGTest,TestSetupNoInputSamples) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputSamples());
}

TEST(FilterGAMGTest,TestSetupNoOutputSamples) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputSamples());
}

TEST(FilterGAMGTest,TestSetupNoNumberOfElementsInput) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoNumberOfElementsInput());
}

TEST(FilterGAMGTest,TestSetupNoNumberOfElementsOutput) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoNumberOfElementsOutput());
}

TEST(FilterGAMGTest,TestSetupNumberOfSamplesOutput2) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNumberOfSamplesOutput2());
}
TEST(FilterGAMGTest,TestSetup0NumberOfElements) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetup0NumberOfElements());
}

TEST(FilterGAMGTest,TestSetupDifferentInputOutputSamples) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupDifferentInputOutputSamples());
}

TEST(FilterGAMGTest,TestSetupFailNumberOfSamples) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupFailNumberOfSamples());
}

TEST(FilterGAMGTest,TestSetupNoInputDimension) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoInputDimension());
}

TEST(FilterGAMGTest,TestSetupWrongInputDimension) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongInputDimension());
}

TEST(FilterGAMGTest,TestSetupNoOutputDimension) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupNoOutputDimension());
}

TEST(FilterGAMGTest,TestSetupWrongOutputDimension) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputDimension());
}

TEST(FilterGAMGTest,TestExecuteNoFilter) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteNoFilter<float32>());
}

TEST(FilterGAMGTest,TestExecuteFIRConstantInputFloat32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInput<float32>());
}

TEST(FilterGAMGTest,TestExecuteFIRConstantInputFloat64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInput<float64>());
}
TEST(FilterGAMGTest,TestExecuteFIRConstantInputInt32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInput<int32>());
}
TEST(FilterGAMGTest,TestExecuteFIRConstantInputInt64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInput<int64>());
}

TEST(FilterGAMGTest,TestExecuteFIRConstantInputInputElements1Float32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInputInputElements1<float32>());
}

TEST(FilterGAMGTest,TestExecuteFIRConstantInputInputElements1Float64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInputInputElements1<float64>());
}

TEST(FilterGAMGTest,TestExecuteFIRConstantInputInputElements1Int32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInputInputElements1<int32>());
}

TEST(FilterGAMGTest,TestExecuteFIRConstantInputInputElements1Int64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInputInputElements1<int64>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInputFloat32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput<float32>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInputFloat64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput<float64>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInputInt32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput<int32>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInputInt64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput<int64>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInputInputElements1Float32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInputInputElements1<float32>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInputInputElements1Float64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInputInputElements1<float64>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInputInputElements1Int32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInputInputElements1<int32>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInputInputElements1Int64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInputInputElements1<int64>());
}
TEST(FilterGAMGTest,TestExecuteAVG10RampInputInputElements1Float32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteAVG10RampInputInputElements1<float32>());
}
TEST(FilterGAMGTest,TestExecuteAVG10RampInputInputElements1Float64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteAVG10RampInputInputElements1<float64>());
}
TEST(FilterGAMGTest,TestExecuteAVG10RampInputInputElements1Int32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteAVG10RampInputInputElements1<int32>());
}
TEST(FilterGAMGTest,TestExecuteAVG10RampInputInputElements1Int64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteAVG10RampInputInputElements1<int64>());
}

TEST(FilterGAMGTest,TestExecuteFIRConstantInput2Float32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInput2<float32>());
}

TEST(FilterGAMGTest,TestExecuteFIRConstantInput2Float64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInput2<float64>());
}

TEST(FilterGAMGTest,TestExecuteFIRConstantInput2Int32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInput2<int32>());
}

TEST(FilterGAMGTest,TestExecuteFIRConstantInput2Int64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRConstantInput2<int64>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInput2Float32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput2<float32>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInput2Float64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput2<float64>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInput2Int32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput2<int32>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInput2Int64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput2<int64>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInput2DiffCoefFloat32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput2DiffCoef<float32>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInput2DiffCoefFloat64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput2DiffCoef<float64>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInput2DiffCoefInt32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput2DiffCoef<int32>());
}

TEST(FilterGAMGTest,TestExecuteFIRRampInput2DiffCoefInt64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteFIRRampInput2DiffCoef<int64>());
}

TEST(FilterGAMGTest,TestExecuteIIRConstantFloat32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstant<float32>());
}

TEST(FilterGAMGTest,TestExecuteIIRConstantFlota64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstant<float64>());
}

TEST(FilterGAMGTest,TestExecuteIIRConstantInt32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstant<int32>());
}

TEST(FilterGAMGTest,TestExecuteIIRConstantInt64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstant<int64>());
}

TEST(FilterGAMGTest,TestExecuteIIRInputRamp1InputElementFloat32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRInputRamp1InputElement<float32>());
}

TEST(FilterGAMGTest,TestExecuteIIRInputRamp1InputElementFloat64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRInputRamp1InputElement<float64>());
}

TEST(FilterGAMGTest,TestExecuteIIRInputRamp1InputElementInt32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRInputRamp1InputElement<int32>());
}

TEST(FilterGAMGTest,TestExecuteIIRInputRamp1InputElementInt64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRInputRamp1InputElement<int64>());
}

TEST(FilterGAMGTest,TestExecuteIIRConstant2Float32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstant2<float32>());
}

TEST(FilterGAMGTest,TestExecuteIIRConstant2Float64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstant2<float64>());
}

TEST(FilterGAMGTest,TestExecuteIIRConstant2Int32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstant2<int32>());
}

TEST(FilterGAMGTest,TestExecuteIIRConstant2Int64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstant2<int64>());
}

TEST(FilterGAMGTest,TestExecuteIIRConstantDimArray1Float32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstantDimArray1<float32>());
}

TEST(FilterGAMGTest,TestExecuteIIRConstantDimArray1Float64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstantDimArray1<float64>());
}

TEST(FilterGAMGTest,TestExecuteIIRConstantDimArray1Int32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstantDimArray1<int32>());
}

TEST(FilterGAMGTest,TestExecuteIIRConstantDimArray1Int64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteIIRConstantDimArray1<int64>());
}

TEST(FilterGAMGTest,TestExecuteElements1Samples10Float32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteElements1Samples10<float32>());
}

TEST(FilterGAMGTest,TestExecuteElements1Samples10Float64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteElements1Samples10<float64>());
}

TEST(FilterGAMGTest,TestExecuteElements1Samples10Int32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteElements1Samples10<int32>());
}

TEST(FilterGAMGTest,TestExecuteElements1Samples10Int6) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteElements1Samples10<int64>());
}

TEST(FilterGAMGTest,TestSetupSeveralSignalsFoat32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignals<float32>());
}
TEST(FilterGAMGTest,TestSetupSeveralSignalsFoat64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignals<float64>());
}
TEST(FilterGAMGTest,TestSetupSeveralSignalsInt32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignals<int32>());
}
TEST(FilterGAMGTest,TestSetupSeveralSignalsInt64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignals<int64>());
}

TEST(FilterGAMGTest,TestSetupSeveralSignalsDiffNumberOfInputElementsFloat32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignalsDiffNumberOfInputElements<float32>());
}

TEST(FilterGAMGTest,TestSetupSeveralSignalsDiffNumberOfInputElementsFloat64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignalsDiffNumberOfInputElements<float64>());
}

TEST(FilterGAMGTest,TestSetupSeveralSignalsDiffNumberOfInputElementsInt32) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignalsDiffNumberOfInputElements<int32>());
}

TEST(FilterGAMGTest,TestSetupSeveralSignalsDiffNumberOfInputElementsInt64) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignalsDiffNumberOfInputElements<int64>());
}

TEST(FilterGAMGTest,TestSetupSeveralSignalsDiffNumberOfOutputElements) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignalsDiffNumberOfOutputElements<float32>());
}

TEST(FilterGAMGTest,TestSetupSeveralSignalsDiffInputSamples) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestSetupSeveralSignalsDiffInputSamples<float32>());
}

TEST(FilterGAMGTest,TestExecuteSeveralSignalsFIR) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestExecuteSeveralSignalsFIR<float32>());
}

TEST(FilterGAMGTest,TestAlwaysResetFIR) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestAlwaysResetFIR<float32>());
}

TEST(FilterGAMGTest,TestAlwaysResetIIR) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestAlwaysResetIIR<float32>());
}

TEST(FilterGAMGTest,TestAlwaysResetMemoryNotInt) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestAlwaysResetMemoryNotInt<float32>());
}

TEST(FilterGAMGTest,TestResetOnlyWhenRequired) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestResetOnlyWhenRequired<float32>());
}

TEST(FilterGAMGTest,TestResetOnlyWhenRequired2) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestResetOnlyWhenRequired2<float32>());
}

TEST(FilterGAMGTest,TestResetOnlyWhenRequired3) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestResetOnlyWhenRequired3<float32>());
}

TEST(FilterGAMGTest,TestResetOnlyWhenRequiredMemoryNotInit) {
    FilterGAMTest test;
    ASSERT_TRUE(test.TestResetOnlyWhenRequiredMemoryNotInit<float32>());
}

}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

