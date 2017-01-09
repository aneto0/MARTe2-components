/**
 * @file NI6259DACGTest.cpp
 * @brief Source file for class NI6259DACGTest
 * @date 29/11/2016
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
 * the class NI6259DACGTest (public, protected, and private). Be aware that some
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
#include "NI6259DACTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI6259DACGTest,TestConstructor) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI6259DACGTest,TestAllocateMemory) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(NI6259DACGTest,TestGetNumberOfMemoryBuffers) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(NI6259DACGTest,TestGetSignalMemoryBuffer_False) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer_False());
}

TEST(NI6259DACGTest,TestGetSignalMemoryBuffer) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(NI6259DACGTest,TestGetBrokerName) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(NI6259DACGTest,TestGetInputBrokers) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(NI6259DACGTest,TestGetOutputBrokers) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(NI6259DACGTest,TestGetOutputBrokers_NoTriggerGAM) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers_NoTriggerGAM());
}

TEST(NI6259DACGTest,TestSynchronise) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(NI6259DACGTest,TestPrepareNextState) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(NI6259DACGTest,TestInitialise) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(NI6259DACGTest,TestInitialise_False_NoDeviceName) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoDeviceName());
}

TEST(NI6259DACGTest,TestInitialise_False_NoBoardId) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoBoardId());
}

TEST(NI6259DACGTest,TestInitialise_False_NoSignals) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoSignals());
}

TEST(NI6259DACGTest,TestInitialise_OutputPolarity) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestInitialise_OutputPolarity());
}

TEST(NI6259DACGTest,TestInitialise_False_BadOutputPolarity) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadOutputPolarity());
}

TEST(NI6259DACGTest,TestInitialise_DefaultOutputPolarity) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultOutputPolarity());
}

TEST(NI6259DACGTest,TestInitialise_False_BadChannelId) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadChannelId());
}

TEST(NI6259DACGTest,TestSetConfiguredDatabasel) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(NI6259DACGTest,TestSetConfiguredDatabase_AllChannels) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_AllChannels());
}

TEST(NI6259DACGTest,TestSetConfiguredDatabase_AllChannels_Parameters) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_AllChannels_Parameters());
}

TEST(NI6259DACGTest,TestSetConfiguredDatabase_False_NoChannels) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoChannels());
}

TEST(NI6259DACGTest,TestSetConfiguredDatabase_False_SignalTypeMismatch) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_SignalTypeMismatch());
}

TEST(NI6259DACGTest,TestSetConfiguredDatabase_False_SignalNumberOfSamplesGreaterThanOne) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_SignalNumberOfSamplesGreaterThanOne());
}

TEST(NI6259DACGTest,TestSetConfiguredDatabase_False_WrongDeviceName) {
    NI6259DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_WrongDeviceName());
}

