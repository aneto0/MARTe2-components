/**
 * @file SDNSubscriberGTest.cpp
 * @brief Source file for class SDNSubscriberGTest
 * @date 12/12/2016
 * @author Bertrand Bauvir
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
 * the class LinuxTimerGTest (public, protected, and private). Be aware that some 
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
#include "SDNSubscriber.h"
#include "SDNSubscriberTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(SDNSubscriberGTest,TestConstructor) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(SDNSubscriberGTest, TestInitialise_Empty) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Empty());
}

TEST(SDNSubscriberGTest, TestInitialise_Default) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Default());
}

TEST(SDNSubscriberGTest, TestInitialise_Address) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Address());
}

TEST(SDNSubscriberGTest, TestInitialise_Address_mcast) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Address_mcast());
}

TEST(SDNSubscriberGTest, TestInitialise_Address_ucast) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Address_ucast());
}

TEST(SDNSubscriberGTest, TestInitialise_ShortForm) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_ShortForm());
}

TEST(SDNSubscriberGTest, TestInitialise_Timeout_0ul) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Timeout_0ul());
}

TEST(SDNSubscriberGTest, TestInitialise_Timeout_10ms) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Timeout_10ms());
}

TEST(SDNSubscriberGTest, TestInitialise_CPUMask) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_CPUMask());
}

TEST(SDNSubscriberGTest, TestInitialise_Missing_Topic) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Missing_Topic());
}

TEST(SDNSubscriberGTest, TestInitialise_Missing_Interface) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Missing_Interface());
}

TEST(SDNSubscriberGTest, TestInitialise_False_Interface_1) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Interface_1());
}

TEST(SDNSubscriberGTest, TestInitialise_False_Interface_2) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Interface_2());
}

TEST(SDNSubscriberGTest, TestInitialise_False_Address_1) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_1());
}

TEST(SDNSubscriberGTest, TestInitialise_False_Address_2) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_2());
}

TEST(SDNSubscriberGTest, TestInitialise_False_Address_3) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_3());
}

TEST(SDNSubscriberGTest, TestInitialise_False_Address_4) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_4());
}

TEST(SDNSubscriberGTest, TestInitialise_False_Address_5) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_5());
}

TEST(SDNSubscriberGTest, TestInitialise_False_Invalid_ExecutionMode) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Invalid_ExecutionMode());
}

TEST(SDNSubscriberGTest, TestSetConfiguredDatabase) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(SDNSubscriberGTest, TestSetConfiguredDatabase_Header) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_Header());
}

TEST(SDNSubscriberGTest, TestSetConfiguredDatabase_False_NOfSignals_1) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NOfSignals_1());
}

TEST(SDNSubscriberGTest, TestSetConfiguredDatabase_False_NOfSignals_2) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NOfSignals_2());
}

TEST(SDNSubscriberGTest,TestAllocateMemory) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(SDNSubscriberGTest,TestAllocateMemory_False) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestAllocateMemory_False());
}

TEST(SDNSubscriberGTest,TestAllocateMemory_False_Header_Size) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestAllocateMemory_False_Header_Size());
}

TEST(SDNSubscriberGTest,TestGetNumberOfMemoryBuffers) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(SDNSubscriberGTest,TestGetSignalMemoryBuffer) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(SDNSubscriberGTest,TestGetBrokerName_InputSignals) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetBrokerName_InputSignals());
}

TEST(SDNSubscriberGTest,TestGetBrokerName_OutputSignals) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetBrokerName_OutputSignals());
}

TEST(SDNSubscriberGTest, TestGetInputBrokers) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(SDNSubscriberGTest, TestGetInputBrokers_1) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetInputBrokers_1());
}

TEST(SDNSubscriberGTest, TestGetInputBrokers_2) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetInputBrokers_2());
}

TEST(SDNSubscriberGTest, TestGetInputBrokers_3) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetInputBrokers_3());
}

TEST(SDNSubscriberGTest, TestGetOutputBrokers) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(SDNSubscriberGTest, TestPrepareNextState) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(SDNSubscriberGTest, TestSynchronise) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_Topic_1) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_1());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_Topic_1_Header) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_1_Header());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_Topic_2) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_2());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_Topic_3) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_3());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_Topic_4) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_4());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_Topic_5) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_5());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_Topic_6) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_6());
}

TEST(SDNSubscriberGTest, TestSynchronise_UCAST_Topic_1) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_UCAST_Topic_1());
}

TEST(SDNSubscriberGTest, TestSynchronise_UCAST_Topic_2) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_UCAST_Topic_2());
}

TEST(SDNSubscriberGTest, TestSynchronise_UCAST_Topic_3) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_UCAST_Topic_3());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_Topic_RTT) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_RTT());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_Topic_RTT_Trigger) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_RTT_Trigger());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_Topic_RTT_GetLatest) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_RTT_GetLatest());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_Timeout) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Timeout());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_RTT_Timeout) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_RTT_Timeout());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_IgnoreTimeoutError) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_IgnoreTimeoutError());
}

TEST(SDNSubscriberGTest, TestSynchronise_MCAST_RTT_IgnoreTimeoutError) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_RTT_IgnoreTimeoutError());
}


#ifdef FEATURE_10840
TEST(SDNSubscriberGTest, TestSynchronise_NetworkByteOrder_Topic_1) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_NetworkByteOrder_Topic_1());
}

TEST(SDNSubscriberGTest, TestSynchronise_NetworkByteOrder_Topic_2) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_NetworkByteOrder_Topic_2());
}

TEST(SDNSubscriberGTest, TestSynchronise_NetworkByteOrder_Topic_1_Header) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_NetworkByteOrder_Topic_1_Header());
}
#endif
TEST(SDNSubscriberGTest, TestExecute_False) {
    SDNSubscriberTest test;
    ASSERT_TRUE(test.TestExecute_False());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

