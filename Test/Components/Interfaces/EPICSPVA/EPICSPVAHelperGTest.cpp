/**
 * @file EPICSPVAHelperGTest.cpp
 * @brief Source file for class EPICSPVAHelperGTest
 * @date 21/11/2018
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
 * the class EPICSPVAHelperGTest (public, protected, and private). Be aware that some 
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
#include "CompilerTypes.h"
#include "EPICSPVAHelperTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(EPICSPVAHelperGTest,TestGetStructure) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetStructure());
}

TEST(EPICSPVAHelperGTest,TestGetStructure_StructuredDataI) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetStructure_StructuredDataI());
}

TEST(EPICSPVAHelperGTest,TestInitStructure) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitStructure());
}

TEST(EPICSPVAHelperGTest,TestInitStructure_StructuredDataI) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitStructure_StructuredDataI());
}

TEST(EPICSPVAHelperGTest,TestReplaceStructureArray) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestReplaceStructureArray());
}

TEST(EPICSPVAHelperGTest,TestReplaceStructureArray_StructuredDataI) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestReplaceStructureArray_StructuredDataI());
}

TEST(EPICSPVAHelperGTest,TestInitArray_UInt8) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitArray<MARTe::uint8>(static_cast<MARTe::uint8>(0)));
}

TEST(EPICSPVAHelperGTest,TestInitArray_UInt16) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitArray<MARTe::uint16>(static_cast<MARTe::uint16>(0)));
}

TEST(EPICSPVAHelperGTest,TestInitArray_UInt32) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitArray<MARTe::uint32>(static_cast<MARTe::uint32>(0)));
}

TEST(EPICSPVAHelperGTest,TestInitArray_UInt64) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitArray<MARTe::uint64>(static_cast<MARTe::uint64>(0)));
}

TEST(EPICSPVAHelperGTest,TestInitArray_Int8) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitArray<MARTe::int8>(static_cast<MARTe::int8>(0)));
}

TEST(EPICSPVAHelperGTest,TestInitArray_Int16) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitArray<MARTe::int16>(static_cast<MARTe::int16>(0)));
}

TEST(EPICSPVAHelperGTest,TestInitArray_Int32) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitArray<MARTe::int32>(static_cast<MARTe::int32>(0)));
}

TEST(EPICSPVAHelperGTest,TestInitArray_Int64) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitArray<MARTe::int64>(static_cast<MARTe::int64>(0)));
}

TEST(EPICSPVAHelperGTest,TestInitArray_Float32) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitArray<MARTe::float32>(static_cast<MARTe::float32>(0)));
}

TEST(EPICSPVAHelperGTest,TestInitArray_Float64) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitArray<MARTe::float64>(static_cast<MARTe::float64>(0)));
}

TEST(EPICSPVAHelperGTest,TestInitArray_String) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitArray<MARTe::char8>(static_cast<MARTe::char8>(0)));
}

TEST(EPICSPVAHelperGTest,TestInitArray_Boolean) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestInitArray<bool>(bool(true)));
}


TEST(EPICSPVAHelperGTest,TestGetType_TypeDescriptor_UInt8) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeDescriptor<MARTe::uint8>(static_cast<MARTe::uint8>(0)));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeDescriptor_UInt16) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeDescriptor<MARTe::uint16>(static_cast<MARTe::uint16>(0)));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeDescriptor_UInt32) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeDescriptor<MARTe::uint32>(static_cast<MARTe::uint32>(0)));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeDescriptor_UInt64) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeDescriptor<MARTe::uint64>(static_cast<MARTe::uint64>(0)));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeDescriptor_Int8) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeDescriptor<MARTe::int8>(static_cast<MARTe::int8>(0)));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeDescriptor_Int16) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeDescriptor<MARTe::int16>(static_cast<MARTe::int16>(0)));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeDescriptor_Int32) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeDescriptor<MARTe::int32>(static_cast<MARTe::int32>(0)));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeDescriptor_Int64) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeDescriptor<MARTe::int64>(static_cast<MARTe::int64>(0)));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeDescriptor_Float32) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeDescriptor<MARTe::float32>(static_cast<MARTe::float32>(0)));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeDescriptor_Float64) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeDescriptor<MARTe::float64>(static_cast<MARTe::float64>(0)));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeDescriptor_Boolean) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeDescriptor<bool>(bool(false)));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeDescriptor_String) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeDescriptor<MARTe::char8>(static_cast<MARTe::char8>(0)));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_UInt8) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeName("uint8"));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_UInt16) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeName("uint16"));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_UInt32) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeName("uint32"));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_UInt64) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeName("uint64"));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_Int8) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeName("int8"));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_Int16) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeName("int16"));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_Int32) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeName("int32"));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_Int64) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeName("int64"));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_Float32) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeName("float32"));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_Float64) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeName("float64"));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_String) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeName("char8"));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_Boolean) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(test.TestGetType_TypeName("bool"));
}

TEST(EPICSPVAHelperGTest,TestGetType_TypeName_BadType) {
    EPICSPVAHelperTest test;
    ASSERT_TRUE(!test.TestGetType_TypeName("uint4"));
}
