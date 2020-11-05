/**
 * @file OPCUAMessageClientGTest.cpp
 * @brief Source file for class OPCUAMessageClientGTest
 * @date 21/10/2019
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
 * the class OPCUAMessageClientGTest (public, protected, and private). Be aware that some 
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

#include "OPCUAMessageClientTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(OPCUAMessageClientGTest,TestConstructor) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(OPCUAMessageClientGTest,TestInitialise) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(OPCUAMessageClientGTest,TestInitialise_NoServerAddress) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestInitialise_NoServerAddress());
}
	
TEST(OPCUAMessageClientGTest,TestInitialise_NoMethodSection) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestInitialise_NoMethodSection());
}

TEST(OPCUAMessageClientGTest,TestInitialise_NoNamespaceIndex_Method) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestInitialise_NoNamespaceIndex_Method());
}

TEST(OPCUAMessageClientGTest,TestInitialise_NoPath_Method) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestInitialise_NoPath_Method());
}

TEST(OPCUAMessageClientGTest,TestInitialise_NoStructureSection) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestInitialise_NoStructureSection());
}

TEST(OPCUAMessageClientGTest,TestInitialise_NoNumberOfElements_Structure) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestInitialise_NoNumberOfElements_Structure());
}

TEST(OPCUAMessageClientGTest,TestInitialise_NoPath_Structure) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestInitialise_NoPath_Structure());
}

TEST(OPCUAMessageClientGTest,TestInitialise_NoNamespaceIndex_Structure) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestInitialise_NoNamespaceIndex_Structure());
}

TEST(OPCUAMessageClientGTest,TestInitialise_NoType_Structure) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestInitialise_NoType_Structure());
}

TEST(OPCUAMessageClientGTest,TestOPCUAMethodCall_WrongMessage) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestOPCUAMethodCall_WrongMessage());
}

TEST(OPCUAMessageClientGTest,TestOPCUAMethodCall_Single) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestOPCUAMethodCall_Single());
}

TEST(OPCUAMessageClientGTest,TestOPCUAMethodCall_Array) {
    OPCUAMessageClientTest test;
    ASSERT_TRUE(test.TestOPCUAMethodCall_Array());
}
