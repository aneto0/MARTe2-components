/**
 * @file EPICSPVGTest.cpp
 * @brief Source file for class EPICSPVGTest
 * @date 04/02/2021
 * @author Andre Neto
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class EPICSPVGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EPICSPVTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(EPICSPVGTest,TestConstructor) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(EPICSPVGTest,TestInitialise_NoEvent_UInt16) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_NoEvent_UInt16());
}

TEST(EPICSPVGTest,TestInitialise_NoEvent_Int16) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_NoEvent_Int16());
}

TEST(EPICSPVGTest,TestInitialise_NoEvent_UInt32) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_NoEvent_UInt32());
}

TEST(EPICSPVGTest,TestInitialise_NoEvent_Int32) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_NoEvent_Int32());
}

TEST(EPICSPVGTest,TestInitialise_NoEvent_Float32) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_NoEvent_Float32());
}

TEST(EPICSPVGTest,TestInitialise_NoEvent_Float64) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_NoEvent_Float64());
}

TEST(EPICSPVGTest,TestInitialise_NoEvent_String) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_NoEvent_String());
}

TEST(EPICSPVGTest,TestInitialise_NoEvent_Array) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_NoEvent_Array());
}

TEST(EPICSPVGTest,TestInitialise_NoEvent_Timeout) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_NoEvent_Timeout());
}
	
TEST(EPICSPVGTest,TestInitialise_False_PVName) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_False_PVName());
}

TEST(EPICSPVGTest,TestInitialise_False_PVType) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_False_PVType());
}

TEST(EPICSPVGTest,TestInitialise_False_PVType_Invalid) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_False_PVType_Invalid());
}

TEST(EPICSPVGTest,TestInitialise_False_ZeroTimeout) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_False_ZeroTimeout());
}

TEST(EPICSPVGTest,TestInitialise_Event_Function) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_Function());
}

TEST(EPICSPVGTest,TestInitialise_Event_FunctionMap) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_FunctionMap());
}

TEST(EPICSPVGTest,TestInitialise_Event_Parameter) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_Parameter());
}

TEST(EPICSPVGTest,TestInitialise_Event_ParameterName) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_ParameterName());
}

TEST(EPICSPVGTest,TestInitialise_Event_Ignore) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_Ignore());
}

TEST(EPICSPVGTest,TestInitialise_Event_Message) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_Message());
}

TEST(EPICSPVGTest,TestInitialise_Event_False_PVValue) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_False_PVValue());
}

TEST(EPICSPVGTest,TestInitialise_Event_False_PVValue_Invalid) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_False_PVValue_Invalid());
}

TEST(EPICSPVGTest,TestInitialise_Event_False_Function_AndFunctionName) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_False_Function_AndFunctionName());
}

TEST(EPICSPVGTest,TestInitialise_Event_False_Parameter_NoFunctionName) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_False_Parameter_NoFunctionName());
}

TEST(EPICSPVGTest,TestInitialise_Event_False_Ignore_NoFunctionName) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_False_Ignore_NoFunctionName());
}

TEST(EPICSPVGTest,TestInitialise_Event_False_Function_And_FunctionMap) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_False_Function_And_FunctionMap());
}

TEST(EPICSPVGTest,TestInitialise_Event_False_FunctionMap_NotMatrix) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_False_FunctionMap_NotMatrix());
}

TEST(EPICSPVGTest,TestInitialise_Event_False_FunctionMap_MatrixNotTwoColumns) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_False_FunctionMap_MatrixNotTwoColumns());
}

TEST(EPICSPVGTest,TestInitialise_Event_False_NotFunction_And_FunctionMap) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_False_NotFunction_And_FunctionMap());
}

TEST(EPICSPVGTest,TestInitialise_Event_False_NoDestination) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestInitialise_Event_False_NoDestination());
}

TEST(EPICSPVGTest,TestSetContext) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestSetContext());
}

TEST(EPICSPVGTest,TestGetContext) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestGetContext());
}

TEST(EPICSPVGTest,TestGetPVName) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestGetPVName());
}

TEST(EPICSPVGTest,TestGetPVChid) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestGetPVChid());
}

TEST(EPICSPVGTest,TestSetPVChid) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestSetPVChid());
}

TEST(EPICSPVGTest,TestGetPVEvid) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestGetPVEvid());
}

TEST(EPICSPVGTest,TestSetPVEvid) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestSetPVEvid());
}

TEST(EPICSPVGTest,TestGetPVType) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestGetPVType());
}

TEST(EPICSPVGTest,TestGetMode) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestGetMode());
}

TEST(EPICSPVGTest,TestGetCATimeout) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestGetCATimeout());
}

TEST(EPICSPVGTest,TestGetDestination) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestGetDestination());
}

TEST(EPICSPVGTest,TestGetFunction) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestGetFunction());
}

TEST(EPICSPVGTest,TestGetFunctionFromMap) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestGetFunctionFromMap());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_NoParameter) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_NoParameter());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_NoParameter_False) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_NoParameter_False());
}

TEST(EPICSPVGTest,TestHandlePVEvent_FunctionMap) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_FunctionMap());
}

TEST(EPICSPVGTest,TestHandlePVEvent_FunctionMap_NoKey) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_FunctionMap_NoKey());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Parameter_Int16) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Parameter_Int16());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Parameter_UInt16) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Parameter_UInt16());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Parameter_Int32) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Parameter_Int32());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Parameter_UInt32) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Parameter_UInt32());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Parameter_Float32) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Parameter_Float32());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Parameter_Float64) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Parameter_Float64());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Parameter_String) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Parameter_String());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Parameter_Array) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Parameter_Array());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Parameter_String_Array) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Parameter_String_Array());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_ParameterName_Int) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_ParameterName_Int());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Ignore) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Ignore());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Message) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Message());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Message_PVName) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Message_PVName());
}

TEST(EPICSPVGTest,TestHandlePVEvent_Function_Message_PVValue) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestHandlePVEvent_Function_Message_PVValue());
}

TEST(EPICSPVGTest,TestCAPut_UInt16) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_UInt16());
}

TEST(EPICSPVGTest,TestCAPut_Int16) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_Int16());
}

TEST(EPICSPVGTest,TestCAPut_UInt32) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_UInt32());
}

TEST(EPICSPVGTest,TestCAPut_Int32) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_Int32());
}

TEST(EPICSPVGTest,TestCAPut_Float32) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_Float32());
}

TEST(EPICSPVGTest,TestCAPut_Float64) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_Float64());
}

TEST(EPICSPVGTest,TestCAPut_String) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_String());
}

TEST(EPICSPVGTest,TestCAPut_Int16_Arr) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_Int16_Arr());
}

TEST(EPICSPVGTest,TestCAPut_UInt16_Arr) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_UInt16_Arr());
}

TEST(EPICSPVGTest,TestCAPut_Int32_Arr) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_Int32_Arr());
}

TEST(EPICSPVGTest,TestCAPut_UInt32_Arr) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_UInt32_Arr());
}

TEST(EPICSPVGTest,TestCAPut_Float32_Arr) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_Float32_Arr());
}

TEST(EPICSPVGTest,TestCAPut_Float64_Arr) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_Float64_Arr());
}

TEST(EPICSPVGTest,TestCAPut_String_Arr) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPut_String_Arr());
}

TEST(EPICSPVGTest,TestCAGet) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAGet());
}

TEST(EPICSPVGTest,TestCAPutRaw) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestCAPutRaw());
}

TEST(EPICSPVGTest,TestGetAnyType) {
    EPICSPVTest test;
    ASSERT_TRUE(test.TestGetAnyType());
}
