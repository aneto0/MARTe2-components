/**
 * @file OPCUADSOutputGTest.cpp
 * @brief Source file for class OPCUADSOutputGTest
 * @date 14 Mar 2019 TODO Verify the value and format of the date
 * @author lporzio TODO Verify the name and format of the author
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
 * the class OPCUADSOutputGTest (public, protected, and private). Be aware that some 
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

#include "OPCUADSOutputTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(OPCUADSOutputGTest,TestConstructor) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(OPCUADSOutputGTest,Test_Write_uint64) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_uint64());
}

TEST(OPCUADSOutputGTest,Test_Write_float32) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_float32());
}

TEST(OPCUADSOutputGTest,Test_Write_float64) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_float64());
}

TEST(OPCUADSOutputGTest,Test_Write_uint8) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_uint8());
}

TEST(OPCUADSOutputGTest,Test_Write_uint16) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_uint16());
}

TEST(OPCUADSOutputGTest,Test_Write_uint32) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_uint32());
}


TEST(OPCUADSOutputGTest,Test_Write_int8) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_int8());
}

TEST(OPCUADSOutputGTest,Test_Write_int16) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_int16());
}

TEST(OPCUADSOutputGTest,Test_Write_int32) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_int32());
}

TEST(OPCUADSOutputGTest,Test_Write_int64) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_int64());
}

TEST(OPCUADSOutputGTest,Test_Write_uint8_array) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_uint8_array());
}

TEST(OPCUADSOutputGTest,Test_Write_uint16_array) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_uint16_array());
}

TEST(OPCUADSOutputGTest,Test_Write_uint32_array) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_uint32_array());
}

TEST(OPCUADSOutputGTest,Test_Write_uint64_array) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_uint64_array());
}

TEST(OPCUADSOutputGTest,Test_Write_int8_array) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_int8_array());
}

TEST(OPCUADSOutputGTest,Test_Write_int16_array) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_int16_array());
}

TEST(OPCUADSOutputGTest,Test_Write_int32_array) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_int32_array());
}

TEST(OPCUADSOutputGTest,Test_Write_int64_array) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_int64_array());
}

TEST(OPCUADSOutputGTest,Test_Write_float32_array) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_float32_array());
}

TEST(OPCUADSOutputGTest,Test_Write_float64_array) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Write_float64_array());
}

TEST(OPCUADSOutputGTest,TestInitialise) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(OPCUADSOutputGTest,TestInitialise_Introspection) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.TestInitialise_Introspection());
}

TEST(OPCUADSOutputGTest,TestInitialise_NoAddress) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.TestInitialise_NoAddress());
}

TEST(OPCUADSOutputGTest,TestInitialise_NoSignals) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.TestInitialise_NoSignals());
}

TEST(OPCUADSOutputGTest,TestInitialise_NoPath) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.TestInitialise_NoPath());
}

TEST(OPCUADSOutputGTest,TestInitialise_NoNamespaceIndex) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.TestInitialise_NoNamespaceIndex());
}

TEST(OPCUADSOutputGTest,Test_NumberOfDimensionsGreater1) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_NumberOfDimensionsGreater1());
}

TEST(OPCUADSOutputGTest,Test_FailSetTargetNodes) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_FailSetTargetNodes());
}

TEST(OPCUADSOutputGTest,Test_WrongNodeId) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_WrongNodeId());
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
