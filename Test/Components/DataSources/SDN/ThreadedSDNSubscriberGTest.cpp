/**
 * @file ThreadedSDNSubscriberGTest.cpp
 * @brief Source file for class ThreadedSDNSubscriberGTest
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

#include "ThreadedSDNSubscriberTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ThreadedSDNSubscriberGTest,TestConstructor) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_Empty) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Empty());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_Default) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Default());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_Address) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Address());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_Address_mcast) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Address_mcast());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_Address_ucast) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Address_ucast());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_ShortForm) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_ShortForm());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_Timeout_0ul) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Timeout_0ul());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_Timeout_10ms) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Timeout_10ms());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_Mode_Defaultl) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Mode_Default());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_Mode_Caching) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Mode_Default());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_Mode_Synchronising) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Mode_Default());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_Missing_Topic) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Missing_Topic());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_Missing_Interface) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_Missing_Interface());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_False_Interface_1) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Interface_1());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_False_Interface_2) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Interface_2());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_False_Address_1) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_1());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_False_Address_2) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_2());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_False_Address_3) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_3());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_False_Address_4) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_4());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_False_Mode) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Mode());
}

TEST(ThreadedSDNSubscriberGTest, TestInitialise_False_Address_5) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_5());
}

TEST(ThreadedSDNSubscriberGTest, TestSetConfiguredDatabase) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(ThreadedSDNSubscriberGTest, TestSetConfiguredDatabase_False_NOfSignals) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NOfSignals());
}

TEST(ThreadedSDNSubscriberGTest,TestAllocateMemory) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(ThreadedSDNSubscriberGTest,TestAllocateMemory_False) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestAllocateMemory_False());
}

TEST(ThreadedSDNSubscriberGTest,TestGetNumberOfMemoryBuffers) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(ThreadedSDNSubscriberGTest,TestGetSignalMemoryBuffer) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(ThreadedSDNSubscriberGTest,TestGetBrokerName_InputSignals) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetBrokerName_InputSignals());
}

TEST(ThreadedSDNSubscriberGTest,TestGetBrokerName_OutputSignals) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetBrokerName_OutputSignals());
}

TEST(ThreadedSDNSubscriberGTest, TestGetInputBrokers) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(ThreadedSDNSubscriberGTest, TestGetOutputBrokers) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(ThreadedSDNSubscriberGTest, TestPrepareNextState) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(ThreadedSDNSubscriberGTest, TestSynchronise) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(ThreadedSDNSubscriberGTest, TestSynchronise_MCAST_Topic_1) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_1());
}

TEST(ThreadedSDNSubscriberGTest, TestSynchronise_MCAST_Topic_2) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_2());
}

TEST(ThreadedSDNSubscriberGTest, TestSynchronise_UCAST_Topic_1) {
    ThreadedSDNSubscriberTest test;
    ASSERT_TRUE(test.TestSynchronise_UCAST_Topic_1());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

