/**
 * @file OPCUAClientWrapperGTest.cpp
 * @brief Source file for class OPCUAClientWrapperGTest
 * @date 15/03/2019
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
 * the class OPCUAClientWrapperGTest (public, protected, and private). Be aware that some 
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
#include "OPCUAClientWrapperTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(OPCUAClientWrapperGTest,TestConstructor) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(OPCUAClientWrapperGTest,Test_SetServerAddress) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_SetServerAddress());
}

TEST(OPCUAClientWrapperGTest,Test_Connect) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Connect());
}

TEST(OPCUAClientWrapperGTest,Test_GetSignalMemory) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_GetSignalMemory());
}

TEST(OPCUAClientWrapperGTest,Test_SetTargetNodes) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_SetTargetNodes());
}

TEST(OPCUAClientWrapperGTest,Test_Monitor) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Monitor());
}

TEST(OPCUAClientWrapperGTest,Test_SetSamplingTime) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_SetSamplingTime());
}

TEST(OPCUAClientWrapperGTest,Test_GetMonitoredNodes) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_GetMonitoredNodes());
}

TEST(OPCUAClientWrapperGTest,Test_SetOperationMode) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_SetOperationMode());
}

TEST(OPCUAClientWrapperGTest,Test_GetReferenceType) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_GetReferenceType());
}

TEST(OPCUAClientWrapperGTest,Test_SetWriteRequest) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_SetWriteRequest());
}

TEST(OPCUAClientWrapperGTest,Test_GetSamplingTime) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_GetSamplingTime());
}

TEST(OPCUAClientWrapperGTest,Test_GetOperationMode) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_GetOperationMode());
}

TEST(OPCUAClientWrapperGTest,Test_GetServerAddress) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_GetServerAddress());
}

TEST(OPCUAClientWrapperGTest,Test_Write_uint8) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_uint8());
}

TEST(OPCUAClientWrapperGTest,Test_Write_uint16) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_uint16());
}

TEST(OPCUAClientWrapperGTest,Test_Write_uint32) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_uint32());
}

TEST(OPCUAClientWrapperGTest,Test_Write_uint64) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_uint64());
}

TEST(OPCUAClientWrapperGTest,Test_Write_int8) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_int8());
}

TEST(OPCUAClientWrapperGTest,Test_Write_int16) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_int16());
}

TEST(OPCUAClientWrapperGTest,Test_Write_int32) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_int32());
}

TEST(OPCUAClientWrapperGTest,Test_Write_int64) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_int64());
}

TEST(OPCUAClientWrapperGTest,Test_Write_float32) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_float32());
}

TEST(OPCUAClientWrapperGTest,Test_Write_float64) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_float64());
}

TEST(OPCUAClientWrapperGTest,Test_WrongNodeId) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_WrongNodeId());
}

TEST(OPCUAClientWrapperGTest,Test_Write_uint8_array) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_uint8_array());
}

TEST(OPCUAClientWrapperGTest,Test_Write_uint16_array) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_uint16_array());
}

TEST(OPCUAClientWrapperGTest,Test_Write_uint32_array) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_uint32_array());
}

TEST(OPCUAClientWrapperGTest,Test_Write_uint64_array) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_uint64_array());
}

TEST(OPCUAClientWrapperGTest,Test_Write_int8_array) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_int8_array());
}

TEST(OPCUAClientWrapperGTest,Test_Write_int16_array) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_int16_array());
}

TEST(OPCUAClientWrapperGTest,Test_Write_int32_array) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_int32_array());
}

TEST(OPCUAClientWrapperGTest,Test_Write_int64_array) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_int64_array());
}

TEST(OPCUAClientWrapperGTest,Test_Write_float32_array) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_float32_array());
}

TEST(OPCUAClientWrapperGTest,Test_Write_float64_array) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Write_float64_array());
}

TEST(OPCUAClientWrapperGTest,Test_Read) {
    OPCUAClientWrapperTest test;
    ASSERT_TRUE(test.Test_Read());
}
