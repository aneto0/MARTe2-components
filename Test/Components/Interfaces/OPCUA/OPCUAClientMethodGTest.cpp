/**
 * @file OPCUAClientMethodGTest.cpp
 * @brief Source file for class OPCUAClientMethodGTest
 * @date 30/10/2019
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
 * the class OPCUAClientMethodGTest (public, protected, and private). Be aware that some 
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
#include "OPCUAClientMethodTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(OPCUAClientMethodGTest,TestConstructor) {
    OPCUAClientMethodTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(OPCUAClientMethodGTest,Test_SetServiceRequest) {
    OPCUAClientMethodTest test;
    ASSERT_TRUE(test.Test_SetServiceRequest());
}

TEST(OPCUAClientMethodGTest,Test_SetObjectRequest) {
    OPCUAClientMethodTest test;
    ASSERT_TRUE(test.Test_SetObjectRequest());
}

TEST(OPCUAClientMethodGTest,Test_SetMethodRequest) {
    OPCUAClientMethodTest test;
    ASSERT_TRUE(test.Test_SetMethodRequest());
}

TEST(OPCUAClientMethodGTest,Test_GetExtensionObjectByteString) {
    OPCUAClientMethodTest test;
    ASSERT_TRUE(test.Test_GetExtensionObjectByteString());
}

TEST(OPCUAClientMethodGTest,Test_SetExtensionObject) {
    OPCUAClientMethodTest test;
    ASSERT_TRUE(test.Test_SetExtensionObject());
}

TEST(OPCUAClientMethodGTest,Test_WrongNodeId) {
    OPCUAClientMethodTest test;
    ASSERT_TRUE(test.Test_WrongNodeId());
}

TEST(OPCUAClientMethodGTest,Test_WrongObjectNodeId) {
    OPCUAClientMethodTest test;
    ASSERT_TRUE(test.Test_WrongObjectNodeId());
}

TEST(OPCUAClientMethodGTest,Test_WrongMethodNodeId) {
    OPCUAClientMethodTest test;
    ASSERT_TRUE(test.Test_WrongMethodNodeId());
}

TEST(OPCUAClientMethodGTest,Test_MethodCall) {
    OPCUAClientMethodTest test;
    ASSERT_TRUE(test.Test_MethodCall());
}
	
