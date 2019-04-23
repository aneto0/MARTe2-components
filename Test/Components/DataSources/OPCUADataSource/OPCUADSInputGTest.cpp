/**
 * @file OPCUADSInputGTest.cpp
 * @brief Source file for class OPCUADSInputGTest
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
 * the class OPCUADSInputGTest (public, protected, and private). Be aware that some 
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

#include "OPCUADSInputTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(OPCUADSInputGTest,TestConstructor) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(OPCUADSInputGTest,TestInitialise_Default) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_Default());
}

TEST(OPCUADSInputGTest,TestInitialise_Introspection) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_Introspection());
}

TEST(OPCUADSInputGTest,TestInitialise_Sync) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_Sync());
}

TEST(OPCUADSInputGTest,TestInitialise_SyncMonitor) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_SyncMonitor());
}

TEST(OPCUADSInputGTest,TestInitialise_SyncWrongMode) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_SyncWrongMode());
}

TEST(OPCUADSInputGTest,TestInitialise_Thread_Default) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_Thread_Default());
}

TEST(OPCUADSInputGTest,TestInitialise_Thread_NoDefault) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_Thread_NoDefault());
}

TEST(OPCUADSInputGTest,TestInitialise_ThreadMonitor) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_ThreadMonitor());
}

TEST(OPCUADSInputGTest,TestInitialise_ThreadWrongMode) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_ThreadWrongMode());
}

TEST(OPCUADSInputGTest,TestInitialise_NoAddress) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_NoAddress());
}

TEST(OPCUADSInputGTest,TestInitialise_NoSignals) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_NoSignals());
}

TEST(OPCUADSInputGTest,TestInitialise_NoPath) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_NoPath());
}

TEST(OPCUADSInputGTest,TestInitialise_NoNamespaceIndex) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.TestInitialise_NoNamespaceIndex());
}

TEST(OPCUADSInputGTest,Test_NumberOfDimensionsGreater1) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.Test_NumberOfDimensionsGreater1());
}

TEST(OPCUADSInputGTest,Test_FailSetTargetNodes) {
    OPCUADSInputTest test;
    ASSERT_TRUE(test.Test_FailSetTargetNodes());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
