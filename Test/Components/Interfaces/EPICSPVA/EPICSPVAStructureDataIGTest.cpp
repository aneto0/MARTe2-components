/**
 * @file EPICSPVAStructureDataIGTest.cpp
 * @brief Source file for class EPICSPVAStructureDataIGTest
 * @date 27/03/2017
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
 * the class EPICSPVAStructureDataIGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EPICSPVAStructureDataITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(EPICSPVAStructureDataIGTest,TestConstructor) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt8) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt8());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Boolean) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Boolean());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt16) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt16());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt32) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt32());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt64) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt64());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int8) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int8());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int16) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int16());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int32) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int32());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int64) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int64());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Float32) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Float32());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Float64) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Float64());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_StreamString) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_StreamString());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt8_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt8_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Boolean_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Boolean_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt16_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt16_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt32_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt32_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt64_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt64_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int8_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int8_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int16_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int16_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int32_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int32_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int64_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int64_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Float32_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Float32_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Float64_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Float64_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_StreamString_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_StreamString_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_False_Array_Char8String) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_False_Array_Char8String());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_False_Array_Dimension) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_False_Array_Dimension());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_False_Array_InvalidType) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_False_Array_InvalidType());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_False_Char8String) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_False_Char8String());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_False_FinaliseStructure) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_False_FinaliseStructure());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_False_InvalidType) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_False_InvalidType());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_DifferentTypes) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_DifferentTypes());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Numeric_ToString) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Numeric_ToString());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_String_ToNumeric) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_String_ToNumeric());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_DifferentTypes_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_DifferentTypes_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Numeric_ToString_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Numeric_ToString_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_String_ToNumeric_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_String_ToNumeric_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_UInt8) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_UInt8());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_UInt16) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_UInt16());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_UInt32) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_UInt32());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_UInt64) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_UInt64());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Int8) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Int8());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Int16) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Int16());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Int32) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Int32());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Int64) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Int64());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Float32) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Int32());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Float64) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Float64());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Boolean) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Boolean());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_StreamString) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_StreamString());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_UInt8_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_UInt8_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_UInt16_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_UInt16_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_UInt32_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_UInt32_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_UInt64_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_UInt64_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Int8_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Int8_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Int16_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Int16_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Int32_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Int32_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Int64_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Int64_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Float32_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Float32_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Float64_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Float64_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_Boolean_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_Boolean_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_StreamString_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_StreamString_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_False_FinaliseStructure) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_False_FinaliseStructure());
}

TEST(EPICSPVAStructureDataIGTest,TestGetType_False_Parameter_Does_Not_Exist) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetType_False_Parameter_Does_Not_Exist());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_UInt8) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_UInt8());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_UInt16) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_UInt16());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_UInt32) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_UInt32());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_UInt64) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_UInt8());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Int8) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Int8());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Int16) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Int16());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Int32) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Int32());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Int64) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Int64());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Float32) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Float32());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Float64) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Float64());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Boolean) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Boolean());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_StreamString) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_StreamString());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Char8String) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Char8String());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_UInt8_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_UInt8_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_UInt16_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_UInt16_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_UInt32_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_UInt32_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_UInt64_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_UInt64_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Int8_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Int8_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Int16_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Int16_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Int32_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Int32_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Int64_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Int64_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Float32_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Float32_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Float64_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Float64_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Boolean_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Boolean_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_StreamString_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_StreamString_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_Char8String_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_Char8String_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_False_Scalar_Vector) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_False_Scalar_Vector());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_False_Vector_Scalar) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_False_Vector_Scalar());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_False_InvalidType) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_False_InvalidType());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_False_Array_Dimension) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_False_Array_Dimension());
}

TEST(EPICSPVAStructureDataIGTest,TestWrite_False_Array_InvalidType) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestWrite_False_Array_InvalidType());
}

TEST(EPICSPVAStructureDataIGTest,TestCopy) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestCopy());
}

TEST(EPICSPVAStructureDataIGTest,TestCopy_Structures) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestCopy_Structures());
}

TEST(EPICSPVAStructureDataIGTest,TestCopy_False_FinaliseStructure) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestCopy_False_FinaliseStructure());
}

TEST(EPICSPVAStructureDataIGTest,TestMoveAbsolute) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestMoveAbsolute());
}

TEST(EPICSPVAStructureDataIGTest,TestMoveRelative) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestMoveRelative());
}

TEST(EPICSPVAStructureDataIGTest,TestMoveToAncestor) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestMoveToAncestor());
}

TEST(EPICSPVAStructureDataIGTest,TestMoveToRoot) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestMoveToRoot());
}

TEST(EPICSPVAStructureDataIGTest,TestMoveToChild) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestMoveToChild());
}

TEST(EPICSPVAStructureDataIGTest,TestCreateAbsolute) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestCreateAbsolute());
}

TEST(EPICSPVAStructureDataIGTest,TestCreateAbsolute_False_FinaliseStructure) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestCreateAbsolute_False_FinaliseStructure());
}

TEST(EPICSPVAStructureDataIGTest,TestCreateRelative) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestCreateRelative());
}

TEST(EPICSPVAStructureDataIGTest,TestCreateRelative_False_FinaliseStructure) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestCreateRelative_False_FinaliseStructure());
}

TEST(EPICSPVAStructureDataIGTest,TestDelete) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestDelete());
}

TEST(EPICSPVAStructureDataIGTest,TestDelete_False_FinaliseStructure) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestDelete_False_FinaliseStructure());
}

TEST(EPICSPVAStructureDataIGTest,TestGetName) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetName());
}

TEST(EPICSPVAStructureDataIGTest,TestGetName_False_FinaliseStructure) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetName_False_FinaliseStructure());
}

TEST(EPICSPVAStructureDataIGTest,TestGetChildName) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetChildName());
}

TEST(EPICSPVAStructureDataIGTest,TestGetChildName_False_FinaliseStructure) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetChildName_False_FinaliseStructure());
}

TEST(EPICSPVAStructureDataIGTest,TestGetNumberOfChildren) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetNumberOfChildren());
}

TEST(EPICSPVAStructureDataIGTest,TestGetNumberOfChildren_False_FinaliseStructure) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetNumberOfChildren_False_FinaliseStructure());
}

TEST(EPICSPVAStructureDataIGTest,TestSetStructure) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestSetStructure());
}

TEST(EPICSPVAStructureDataIGTest,TestInitStructure) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestInitStructure());
}

TEST(EPICSPVAStructureDataIGTest,TestFinaliseStructure) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestFinaliseStructure());
}

TEST(EPICSPVAStructureDataIGTest,TestGetRootStruct) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestGetRootStruct());
}

TEST(EPICSPVAStructureDataIGTest,TestIsStructureFinalised) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestIsStructureFinalised());
}

TEST(EPICSPVAStructureDataIGTest,TestCopyValuesFrom) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestCopyValuesFrom());
}

TEST(EPICSPVAStructureDataIGTest,TestCopyValuesFrom_False) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestCopyValuesFrom_False());
}

TEST(EPICSPVAStructureDataIGTest,TestPerformance) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestPerformance());
}

TEST(EPICSPVAStructureDataIGTest,TestToString) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestToString());
}

TEST(EPICSPVAStructureDataIGTest,TestToString_False) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestToString_False());
}
