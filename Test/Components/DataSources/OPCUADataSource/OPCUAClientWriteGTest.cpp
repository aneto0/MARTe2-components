/**
 * @file OPCUAClientWriteGTest.cpp
 * @brief Source file for class OPCUAClientWriteGTest
 * @date 29/10/2019
 * @author Luca Porzio
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
 * the class OPCUAClientWriteGTest (public, protected, and private). Be aware that some 
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

#include "OPCUAClientWriteTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(OPCUAClientWriteGTest,TestConstructor) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(OPCUAClientWriteGTest,Test_SetServiceRequest) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetServiceRequest());
}

TEST(OPCUAClientWriteGTest,Test_GetExtensionObjectByteString) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_GetExtensionObjectByteString());
}

TEST(OPCUAClientWriteGTest,Test_SetExtensionObject) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetExtensionObject());
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_uint8) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("uint8", 1));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_uint16) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("uint16", 1));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_uint32) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("uint32", 1));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_uint64) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("uint64", 1));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_int8) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("int8", 1));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_int16) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("int16", 1));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_int32) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("int32", 1));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_int64) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("int64", 1));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_float32) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("float32", 1));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_float64) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("float64", 1));
}

TEST(OPCUAClientWriteGTest,Test_WrongNodeId) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_WrongNodeId());
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_uint8_array) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("uint8", 5));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_uint16_array) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("uint16", 5));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_uint32_array) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("uint32", 5));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_uint64_array) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("uint64", 5));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_int8_array) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("int8", 5));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_int16_array) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("int16", 5));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_int32_array) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("int32", 5));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_int64_array) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("int64", 5));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_float32_array) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("float32", 5));
}

TEST(OPCUAClientWriteGTest,Test_SetWriteRequest_float64_array) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest("float64", 5));
}

TEST(OPCUAClientWriteGTest,Test_Write_ExtensionObject) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_Write_ExtensionObject());
}

TEST(OPCUAClientWriteGTest,Test_Write_ExtensionObject_Complex) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_Write_ExtensionObject_Complex());
}

TEST(OPCUAClientWriteGTest,Test_Write_ExtensionObject_Array) {
    OPCUAClientWriteTest test;
    ASSERT_TRUE(test.Test_Write_ExtensionObject_Array());
}

