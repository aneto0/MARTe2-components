/**
 * @file RealTimeThreadSynchronisationGTest.cpp
 * @brief Source file for class RealTimeThreadSynchronisationGTest
 * @date 05/05/2017
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
 * the class RealTimeThreadSynchronisationGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "RealTimeThreadSynchronisationTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(RealTimeThreadSynchronisationGTest,TestConstructor) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(RealTimeThreadSynchronisationGTest,TestAllocateMemory) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(RealTimeThreadSynchronisationGTest,TestGetNumberOfMemoryBuffers) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(RealTimeThreadSynchronisationGTest,TestGetSignalMemoryBuffer) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(RealTimeThreadSynchronisationGTest,TestGetBrokerName) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(RealTimeThreadSynchronisationGTest,TestGetInputBrokers) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(RealTimeThreadSynchronisationGTest,TestGetOutputBrokers) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(RealTimeThreadSynchronisationGTest,TestPrepareNextState) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(RealTimeThreadSynchronisationGTest,TestInitialise) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(RealTimeThreadSynchronisationGTest,TestInitialise_Timeout) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestInitialise_Timeout());
}

TEST(RealTimeThreadSynchronisationGTest,TestGetSynchroniseTimeout) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestGetSynchroniseTimeout());
}

TEST(RealTimeThreadSynchronisationGTest,TestSetConfiguredDatabase) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(RealTimeThreadSynchronisationGTest,TestSetConfiguredDatabase_NoInteraction) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_NoInteraction());
}

TEST(RealTimeThreadSynchronisationGTest,TestSetConfiguredDatabase_ProducerOnly) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_ProducerOnly());
}

TEST(RealTimeThreadSynchronisationGTest,TestSetConfiguredDatabase_False_NoProducer) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoProducer());
}

TEST(RealTimeThreadSynchronisationGTest,TestSetConfiguredDatabase_False_MoreThanOneProducer) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThanOneProducer());
}

TEST(RealTimeThreadSynchronisationGTest,TestSetConfiguredDatabase_Warning_NotAllSignals) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_Warning_NotAllSignals());
}

TEST(RealTimeThreadSynchronisationGTest,TestSetConfiguredDatabase_False_MoreThanOneOutputSample) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThanOneOutputSample());
}

TEST(RealTimeThreadSynchronisationGTest,TestSetConfiguredDatabase_False_DifferentInputSamplesFromSameFunction) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_DifferentInputSamplesFromSameFunction());
}

TEST(RealTimeThreadSynchronisationGTest,TestSynchronise) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(RealTimeThreadSynchronisationGTest,TestSetConfiguredDatabase_WaitForNext) {
    RealTimeThreadSynchronisationTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_WaitForNext());
}

	
