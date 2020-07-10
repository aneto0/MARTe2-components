/**
 * @file OPCUAServerGTest.cpp
 * @brief Source file for class OPCUAServerGTest
 * @date 12/03/2019
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
 * the class OPCUAServerGTest (public, protected, and private). Be aware that some 
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

#include "OPCUAServerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(OPCUAServerGTest,TestConstructor) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(OPCUAServerGTest,Test_SetRunning) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.Test_SetRunning());
}

TEST(OPCUAServerGTest,Test_GetRunning) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.Test_GetRunning());
}

TEST(OPCUAServerGTest,Test_GetCPUMask) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.Test_GetCPUMask());
}

TEST(OPCUAServerGTest,Test_GetStackSize) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.Test_GetStackSize());
}

TEST(OPCUAServerGTest,Test_GetPort) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.Test_GetPort());
}

TEST(OPCUAServerGTest,Test_InitAddressSpace) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.Test_InitAddressSpace());
}

TEST(OPCUAServerGTest,TestInitialise_Default) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.TestInitialise_Default());
}

TEST(OPCUAServerGTest,TestInitialise_NoDefault) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.TestInitialise_NoDefault());
}

TEST(OPCUAServerGTest,TestInitialise_NoAddressSpace) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.TestInitialise_NoAddressSpace());
}

TEST(OPCUAServerGTest,TestExecute) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(OPCUAServerGTest,TestExecute_Introspection) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.TestExecute_Introspection());
}

TEST(OPCUAServerGTest,TestExecute_IntrospectionArray) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.TestExecute_IntrospectionArray());
}

TEST(OPCUAServerGTest,TestExecute_WrongNDimensions) {
    OPCUAServerTest test;
    ASSERT_TRUE(test.TestExecute_WrongNDimensions());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
