/**
 * @file ConversionGAMGTest.cpp
 * @brief Source file for class ConversionGAMGTest
 * @date 21/01/2017
 * @author Andre Neto
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
 * the class ConversionGAMGTest (public, protected, and private). Be aware that some
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
#include "ConversionGAM.h"
#include "ConversionGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ConversionGAMGTest,TestConstructor) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(ConversionGAMGTest,TestSetup) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(ConversionGAMGTest,TestExecute_FromUInt8) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::uint8>(255));
}

TEST(ConversionGAMGTest,TestExecute_FromInt8) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::int8>(-1));
}

TEST(ConversionGAMGTest,TestExecute_FromUInt16) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::uint16>(0xffff));
}

TEST(ConversionGAMGTest,TestExecute_FromInt16) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::int16>(-1));
}

TEST(ConversionGAMGTest,TestExecute_FromUInt32) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::uint32>(0xffffffff));
}

TEST(ConversionGAMGTest,TestExecute_FromInt32) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::int32>(-1));
}

TEST(ConversionGAMGTest,TestExecute_FromUInt64) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::uint64>(0xffffffffffffffff));
}

TEST(ConversionGAMGTest,TestExecute_FromInt64) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::int64>(-1));
}

TEST(ConversionGAMGTest,TestExecute_FromFloat32) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::float32>(-1.0));
}

TEST(ConversionGAMGTest,TestExecute_FromFloat64) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::float64>(-1.0));
}

TEST(ConversionGAMGTest,TestExecute_FromUInt8_Gain) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::uint8>(255, true, 2.0));
}

TEST(ConversionGAMGTest,TestExecute_FromInt8_Gain) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::int8>(-1, true, 2.0));
}

TEST(ConversionGAMGTest,TestExecute_FromUInt16_Gain) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::uint16>(0xffff, true, 2.0));
}

TEST(ConversionGAMGTest,TestExecute_FromInt16_Gain) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::int16>(-1, true, 2.0));
}

TEST(ConversionGAMGTest,TestExecute_FromUInt32_Gain) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::uint32>(0xffffffff, true, 2.0));
}

TEST(ConversionGAMGTest,TestExecute_FromInt32_Gain) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::int32>(-1, true, 2.0));
}

TEST(ConversionGAMGTest,TestExecute_FromUInt64_Gain) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::uint64>(0xffffffffffffffff, true, 2.0));
}

TEST(ConversionGAMGTest,TestExecute_FromInt64_Gain) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::int64>(-1, true, 2.0));
}

TEST(ConversionGAMGTest,TestExecute_FromFloat32_Gain) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::float32>(-1.0, true, 2.0));
}

TEST(ConversionGAMGTest,TestExecute_FromFloat64_Gain) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::float64>(-1.0, true, 2.0));
}

TEST(ConversionGAMGTest,TestExecute_FromFloat64_Gain_Offset) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::float64>(-1.0, true, 2.0, true, 1));
}

TEST(ConversionGAMGTest,TestExecute_FromFloat32_Gain_Offset) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::float32>(-1.0, true, 2.0, true, 1));
}

TEST(ConversionGAMGTest,TestExecute_FromInt64_Gain_Offset) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::int64>(-1.0, true, 2.0, true, 1));
}

TEST(ConversionGAMGTest,TestExecute_FromUInt64_Gain_Offset) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::uint64>(0xffffffffffffffff, true, 2.0, true, 1));
}

TEST(ConversionGAMGTest,TestExecute_FromInt32_Gain_Offset) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::int32>(-1.0, true, 2.0, true, 1));
}

TEST(ConversionGAMGTest,TestExecute_FromUInt32_Gain_Offset) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::uint32>(0xffffffff, true, 2.0, true, 1));
}

TEST(ConversionGAMGTest,TestExecute_FromInt16_Gain_Offset) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::int16>(-1.0, true, 2.0, true, 1));
}

TEST(ConversionGAMGTest,TestExecute_FromUInt16_Gain_Offset) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::uint16>(0xffff, true, 2.0, true, 1));
}

TEST(ConversionGAMGTest,TestExecute_FromInt8_Gain_Offset) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::int8>(-1.0, true, 2.0, true, 1));
}

TEST(ConversionGAMGTest,TestExecute_FromUInt8_Gain_Offset) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestExecute<MARTe::uint8>(0xff, true, 2.0, true, 1));
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidOutputSignalType_FromUInt8) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidOutputSignalType<MARTe::uint8>());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidOutputSignalType_FromInt8) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidOutputSignalType<MARTe::int8>());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidOutputSignalType_FromUInt16) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidOutputSignalType<MARTe::uint16>());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidOutputSignalType_FromInt16) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidOutputSignalType<MARTe::int16>());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidOutputSignalType_FromUInt32) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidOutputSignalType<MARTe::uint32>());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidOutputSignalType_FromInt32) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidOutputSignalType<MARTe::int32>());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidOutputSignalType_FromUInt64) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidOutputSignalType<MARTe::uint64>());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidOutputSignalType_FromInt64) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidOutputSignalType<MARTe::int64>());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidOutputSignalType_FromFloat32) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidOutputSignalType<MARTe::float32>());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidOutputSignalType_FromFloat64) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidOutputSignalType<MARTe::float64>());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidInputSignalType) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidInputSignalType());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidNumberOfInputsOutputs) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidNumberOfInputsOutputs());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidInputElementsMismatch) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidInputElementsMismatch());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidInputSamplesMismatch) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidInputSamplesMismatch());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidOutputElementsMismatch) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidOutputElementsMismatch());
}

TEST(ConversionGAMGTest,TestSetup_False_InvalidOutputSamplesMismatch) {
    ConversionGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_InvalidOutputSamplesMismatch());
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
