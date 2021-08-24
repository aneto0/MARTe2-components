/**
 * @file UDPSenderGTest.cpp
 * @brief Source file for class UDPSenderGTest
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
 * the class UDPSenderGTest (public, protected, and private). Be aware that some 
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

TEST(UDPSenderGTest,TestInitialise_No_NumberOfPreTriggers) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_No_NumberOfPreTriggers());
}

TEST(UDPSenderGTest,TestInitialise_No_NumberOfPostTriggers) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_No_NumberOfPostTriggers());
}

TEST(UDPSenderGTest,TestInitialise_No_Address) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_No_Address());
}

TEST(UDPSenderGTest,TestInitialise_No_Port) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_No_Port());
}

TEST(UDPSenderGTest,TestInitialise_Forbidden_Port) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_Forbidden_Port());
}

TEST(UDPSenderGTest,TestInitialise_No_CpuMaks) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_No_CpuMaks());
}

TEST(UDPSenderGTest,TestInitialise_No_StackSize) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_No_StackSize());
}

TEST(UDPSenderGTest,TestInitialise_StackSize_Zero) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_StackSize_Zero());
}

TEST(UDPSenderGTest,TestInitialise_No_TriggerSignal) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_No_TriggerSignal());
}

TEST(UDPSenderGTest,TestInitialise_No_CounterSignal) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_No_CounterSignal());
}

TEST(UDPSenderGTest,TestInitialise_No_TimeSignal) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_No_TimeSignal());
}

TEST(UDPSenderGTest,TestInitialise_Correct) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestInitialise_Correct());
}

TEST(UDPSenderGTest,TestGetInputBrokers) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(UDPSenderGTest,TestGetOutputBrokers) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(UDPSenderGTest,TestGetBrokerName) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(UDPSenderGTest,TestPrepareNextState) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(UDPSenderGTest,TestSynchronise) {
    UDPSenderTest test;
    ASSERT_TRUE(test.TestSynchronise());
}


	
