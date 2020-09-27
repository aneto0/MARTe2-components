/**
 * @file OPCUADSOutputGTest.cpp
 * @brief Source file for class OPCUADSOutputGTest
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

TEST(OPCUADSOutputGTest,TestInitialise) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(OPCUADSOutputGTest,Test_SetConfiguredDatabase) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_SetConfiguredDatabase());
}

TEST(OPCUADSOutputGTest,Test_AllocateMemory) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_AllocateMemory());
}

TEST(OPCUADSOutputGTest,Test_GetSignalMemoryBuffer) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_GetSignalMemoryBuffer());
}

TEST(OPCUADSOutputGTest,Test_PrepareNextState) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_PrepareNextState());
}

TEST(OPCUADSOutputGTest,Test_Synchronise) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_Synchronise());
}

TEST(OPCUADSOutputGTest,Test_GetBrokerName) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_GetBrokerName());
}

TEST(OPCUADSOutputGTest,Test_GetServerAddress) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_GetServerAddress());
}

TEST(OPCUADSOutputGTest,Test_GetClient) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_GetClient());
}

TEST(OPCUADSOutputGTest,TestInitialise_Introspection) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.TestInitialise_Introspection());
}

TEST(OPCUADSOutputGTest,TestInitialise_ExtensionObject) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.TestInitialise_ExtensionObject());
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

TEST(OPCUADSOutputGTest,Test_SetConfiguredDatabase_FailSetServiceRequest) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_SetConfiguredDatabase_FailSetServiceRequest());
}

TEST(OPCUADSOutputGTest,Test_SetConfiguredDatabase_ExtensionObject) {
    OPCUADSOutputTest test;
    ASSERT_TRUE(test.Test_SetConfiguredDatabase_ExtensionObject());
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
