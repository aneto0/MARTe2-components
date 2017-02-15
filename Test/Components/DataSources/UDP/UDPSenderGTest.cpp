/**
 * @file UDPSenderGTest.cpp
 * @brief Source file for class UDPSenderGTest
 * @date 25/10/2016
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
 * the class UDPSenderGTest (public, protected, and private). Be aware that some 
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
#include "UDPSenderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(UDPSenderGTest,TestConstructor) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UDPSenderGTest,TestInitialise_Empty) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_Empty());
}

TEST(UDPSenderGTest,TestInitialise_Port) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_Port());
}

TEST(UDPSenderGTest,TestInitialise_Restricted_Port) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_Restricted_Port());
}

TEST(UDPSenderGTest,TestInitialise_False_Port_1) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_False_Port_1());
}

TEST(UDPSenderGTest,TestInitialise_False_Port_2) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_False_Port_2());
}

TEST(UDPSenderGTest,TestInitialise_False_Port_3) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_False_Port_3());
}

TEST(UDPSenderGTest,TestInitialise_False_Port_4) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_False_Port_4());
}

TEST(UDPSenderGTest,TestInitialise_TargetAddress) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_TargetAddress());
}

TEST(UDPSenderGTest,TestInitialise_False_TargetAddress_1) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_False_TargetAddress_1());
}

TEST(UDPSenderGTest,TestInitialise_False_TargetAddress_2) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_False_TargetAddress_2());
}

TEST(UDPSenderGTest,TestInitialise_False_TargetAddress_3) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_False_TargetAddress_3());
}

TEST(UDPSenderGTest,TestInitialise_False_TargetAddress_4) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_False_TargetAddress_4());
}

TEST(UDPSenderGTest,TestInitialise_False_TargetAddress_5) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_False_TargetAddress_5());
}

TEST(UDPSenderGTest,TestSetConfiguredDatabase) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(UDPSenderGTest, TestSetConfiguredDatabase_More_Minimum_Signals) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_More_Minimum_Signals());
}

TEST(UDPSenderGTest,TestSetConfiguredDatabase_False_NOfSignals) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NOfSignals());
}

TEST(UDPSenderGTest, TestSetConfiguredDatabase_Incorrect_Signal_Size1) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_Incorrect_Signal_Size1());
}

TEST(UDPSenderGTest, TestSetConfiguredDatabase_Incorrect_Signal_Size2) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_Incorrect_Signal_Size2());
}

TEST(UDPSenderGTest, TestSetConfiguredDatabase_Incorrect_Signal_Type1) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_Incorrect_Signal_Type1());
}

TEST(UDPSenderGTest, TestSetConfiguredDatabase_Incorrect_Signal_Type2) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_Incorrect_Signal_Type2());
}

TEST(UDPSenderGTest,TestAllocateMemory) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(UDPSenderGTest,TestAllocateMemory_False) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestAllocateMemory_False());
}

TEST(UDPSenderGTest,TestGetNumberOfMemoryBuffers) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(UDPSenderGTest,TestGetSignalMemoryBuffer) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(UDPSenderGTest,TestGetBrokerName_InputSignals) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestGetBrokerName_InputSignals());
}

TEST(UDPSenderGTest,TestGetBrokerName_OutputSignals) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestGetBrokerName_OutputSignals());
}

TEST(UDPSenderGTest,TestGetOutputBrokers) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(UDPSenderGTest,TestGetInputBrokers) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(UDPSenderGTest,TestPrepareNextState) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(UDPSenderGTest,TestSynchronise) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

