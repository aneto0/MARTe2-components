/**
 * @file LinuxTimerGTest.cpp
 * @brief Source file for class SDNPublisherGTest
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

#include "SDNPublisherTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(SDNPublisherGTest,TestConstructor) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(SDNPublisherGTest, TestInitialise_Empty) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_Empty());
}

TEST(SDNPublisherGTest, TestInitialise_Default) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_Default());
}

TEST(SDNPublisherGTest, TestInitialise_Address) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_Address());
}

TEST(SDNPublisherGTest, TestInitialise_Address_mcast) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_Address_mcast());
}

TEST(SDNPublisherGTest, TestInitialise_Address_ucast) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_Address_ucast());
}

TEST(SDNPublisherGTest, TestInitialise_ShortForm) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_ShortForm());
}

TEST(SDNPublisherGTest, TestInitialise_Missing_Topic) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_Missing_Topic());
}

TEST(SDNPublisherGTest, TestInitialise_Missing_Interface) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_Missing_Interface());
}

TEST(SDNPublisherGTest, TestInitialise_False_Interface_1) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_False_Interface_1());
}

TEST(SDNPublisherGTest, TestInitialise_False_Interface_2) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_False_Interface_2());
}

TEST(SDNPublisherGTest, TestInitialise_False_Address_1) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_1());
}

TEST(SDNPublisherGTest, TestInitialise_False_Address_2) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_2());
}

TEST(SDNPublisherGTest, TestInitialise_False_Address_3) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_3());
}

TEST(SDNPublisherGTest, TestInitialise_False_Address_4) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_4());
}

TEST(SDNPublisherGTest, TestInitialise_False_Address_5) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_5());
}

TEST(SDNPublisherGTest, TestSetConfiguredDatabase) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(SDNPublisherGTest, TestSetConfiguredDatabase_False_NOfSignals) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NOfSignals());
}

TEST(SDNPublisherGTest,TestAllocateMemory) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(SDNPublisherGTest,TestAllocateMemory_False) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestAllocateMemory_False());
}

TEST(SDNPublisherGTest,TestGetNumberOfMemoryBuffers) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(SDNPublisherGTest,TestGetSignalMemoryBuffer) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(SDNPublisherGTest,TestGetBrokerName_InputSignals) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestGetBrokerName_InputSignals());
}

TEST(SDNPublisherGTest,TestGetBrokerName_OutputSignals) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestGetBrokerName_OutputSignals());
}

TEST(SDNPublisherGTest, TestGetInputBrokers) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(SDNPublisherGTest, TestGetOutputBrokers) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(SDNPublisherGTest, TestPrepareNextState) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(SDNPublisherGTest, TestSynchronise) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(SDNPublisherGTest, TestSynchronise_MCAST_Topic_1) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_1());
}

TEST(SDNPublisherGTest, TestSynchronise_MCAST_Topic_2) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestSynchronise_MCAST_Topic_2());
}

TEST(SDNPublisherGTest, TestSynchronise_UCAST_Topic_1) {
    SDNPublisherTest test;
    ASSERT_TRUE(test.TestSynchronise_UCAST_Topic_1());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

