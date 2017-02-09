/**
 * @file UDPReceiverGTest.cpp
 * @brief Source file for class UDPReceiverGTest
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
 * the class UDPReceiverGTest (public, protected, and private). Be aware that some 
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
#include "UDPReceiverTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(UDPReceiverGTest,TestConstructor) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UDPReceiverGTest,TestInitialise_Empty) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_Empty());
}

TEST(UDPReceiverGTest,TestInitialise_Port) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_Port());
}

TEST(UDPReceiverGTest, TestInitialise_Restricted_Port) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_Restricted_Port());
}

TEST(UDPReceiverGTest,TestInitialise_False_Port_1) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_False_Port_1());
}

TEST(UDPReceiverGTest,TestInitialise_False_Port_2) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_False_Port_2());
}

TEST(UDPReceiverGTest,TestInitialise_False_Port_3) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_False_Port_3());
}

TEST(UDPReceiverGTest, TestInitialise_False_Port_4) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_False_Port_4());
}

TEST(UDPReceiverGTest, TestInitialise_Timeout) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_Timeout());
}

TEST(UDPReceiverGTest, TestInitialise_Timeout_Infinite) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_Timeout_Infinite());
}

TEST(UDPReceiverGTest, TestInitialise_Timeout_NotDefined) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_Timeout_NotDefined());
}

TEST(UDPReceiverGTest, TestInitialise_False_Timeout_1) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_False_Timeout_1());
}

TEST(UDPReceiverGTest, TestInitialise_False_Timeout_2) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_False_Timeout_2());
}

TEST(UDPReceiverGTest, TestInitialise_False_Timeout_3) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_False_Timeout_3());
}

TEST(UDPReceiverGTest, TestSetConfiguredDatabase) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(UDPReceiverGTest, TestSetConfiguredDatabase_More_Minimum_Signals) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_More_Minimum_Signals());
}

TEST(UDPReceiverGTest, TestSetConfiguredDatabase_False_NOfSignals1) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NOfSignals1());
}

TEST(UDPReceiverGTest, TestSetConfiguredDatabase_False_NOfSignals2) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NOfSignals2());
}

TEST(UDPReceiverGTest, TestSetConfiguredDatabase_False_NOfSignals3) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NOfSignals3());
}

TEST(UDPReceiverGTest, TestSetConfiguredDatabase_Incorrect_Signal_Size1) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_Incorrect_Signal_Size1());
}

TEST(UDPReceiverGTest, TestSetConfiguredDatabase_Incorrect_Signal_Size2) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_Incorrect_Signal_Size2());
}

TEST(UDPReceiverGTest, TestSetConfiguredDatabase_Incorrect_Signal_Type1) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_Incorrect_Signal_Type1());
}

TEST(UDPReceiverGTest, TestSetConfiguredDatabase_Incorrect_Signal_Type2) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_Incorrect_Signal_Type2());
}

TEST(UDPReceiverGTest, TestAllocateMemory) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(UDPReceiverGTest, TestAllocateMemory_False) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestAllocateMemory_False());
}

TEST(UDPReceiverGTest, TestGetNumberOfMemoryBuffers) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(UDPReceiverGTest, TestGetSignalMemoryBuffer) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(UDPReceiverGTest, TestGetBrokerName_InputSignals) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestGetBrokerName_InputSignals());
}

TEST(UDPReceiverGTest, TestGetBrokerName_OutputSignals) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestGetBrokerName_OutputSignals());
}

TEST(UDPReceiverGTest, TestGetInputBrokers) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(UDPReceiverGTest, TestGetOutputBrokers) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(UDPReceiverGTest, TestPrepareNextState) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(UDPReceiverGTest, TestSynchronise) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(UDPReceiverGTest, TestReceiving) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestReceiving());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

