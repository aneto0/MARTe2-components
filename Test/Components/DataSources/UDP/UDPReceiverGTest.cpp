/**
 * @file UDPReceiverGTest.cpp
 * @brief Source file for class UDPReceiverGTest
 * @date 10/06/2021
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
 * the class UDPReceiverGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
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

TEST(UDPReceiverGTest,TestInitialise_No_Address) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_No_Address());
}

TEST(UDPReceiverGTest,TestInitialise_InterfaceAddress) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_InterfaceAddress());
}

TEST(UDPReceiverGTest,TestInitialise_No_Port) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_No_Port());
}

TEST(UDPReceiverGTest,TestInitialise_Forbidden_Port) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_Forbidden_Port());
}

TEST(UDPReceiverGTest,TestInitialise_No_Timeout) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_No_Timeout());
}

TEST(UDPReceiverGTest,TestInitialise_No_ExecutionMode) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_No_ExecutionMode());
}

TEST(UDPReceiverGTest,TestInitialise_No_ExecutionMode_No_Cpu) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_No_ExecutionMode_No_Cpu());
}

TEST(UDPReceiverGTest,TestInitialise_No_ExecutionMode_No_StackSize) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_No_ExecutionMode_No_StackSize());
}

TEST(UDPReceiverGTest,TestInitialise_IndependentThread) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_IndependentThread());
}

TEST(UDPReceiverGTest,TestInitialise_Wrong_ExecutionMode) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestInitialise_Wrong_ExecutionMode());
}

TEST(UDPReceiverGTest,TestSetConfiguredDatabase_ValidAddress) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_ValidAddress());
}

TEST(UDPReceiverGTest,TestSetConfiguredDatabase_MulticastAddress_Interface) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_MulticastAddress_Interface());
}

TEST(UDPReceiverGTest,TestSetConfiguredDatabase_InvalidAddress) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_InvalidAddress());
}

TEST(UDPReceiverGTest,TestGetBrokerName) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(UDPReceiverGTest,TestPrepareNextState) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(UDPReceiverGTest,TestSynchronise) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(UDPReceiverGTest,TestExecute_Timeout) {
    UDPReceiverTest test;
    ASSERT_TRUE(test.TestExecute_Timeout());
}
