/**
 * @file NI6368DACGTest.cpp
 * @brief Source file for class NI6368DACGTest
 * @date 23/01/2017
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
 * the class NI6368DACGTest (public, protected, and private). Be aware that some
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
#include "NI6368DACTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI6368DACGTest,TestConstructor) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI6368DACGTest,TestAllocateMemory) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(NI6368DACGTest,TestGetNumberOfMemoryBuffers) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(NI6368DACGTest,TestGetSignalMemoryBuffer_False) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer_False());
}

TEST(NI6368DACGTest,TestGetSignalMemoryBuffer) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(NI6368DACGTest,TestGetBrokerName) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(NI6368DACGTest,TestGetInputBrokers) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(NI6368DACGTest,TestGetOutputBrokers) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(NI6368DACGTest,TestGetOutputBrokers_NoTriggerGAM) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers_NoTriggerGAM());
}

TEST(NI6368DACGTest,TestSynchronise) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(NI6368DACGTest,TestPrepareNextState) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(NI6368DACGTest,TestInitialise) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(NI6368DACGTest,TestInitialise_False_NoDeviceName) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoDeviceName());
}

TEST(NI6368DACGTest,TestInitialise_False_NoBoardId) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoBoardId());
}

TEST(NI6368DACGTest,TestInitialise_False_NoSignals) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoSignals());
}

TEST(NI6368DACGTest,TestInitialise_OutputRange) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestInitialise_OutputRange());
}

TEST(NI6368DACGTest,TestInitialise_False_BadOutputRange) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadOutputRange());
}

TEST(NI6368DACGTest,TestInitialise_DefaultOutputRange) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultOutputRange());
}

TEST(NI6368DACGTest,TestInitialise_False_BadChannelId) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadChannelId());
}

TEST(NI6368DACGTest,TestSetConfiguredDatabasel) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(NI6368DACGTest,TestSetConfiguredDatabase_AllChannels) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_AllChannels());
}

TEST(NI6368DACGTest,TestSetConfiguredDatabase_AllChannels_Parameters) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_AllChannels_Parameters());
}

TEST(NI6368DACGTest,TestSetConfiguredDatabase_False_NoChannels) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoChannels());
}

TEST(NI6368DACGTest,TestSetConfiguredDatabase_False_SignalTypeMismatch) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_SignalTypeMismatch());
}

TEST(NI6368DACGTest,TestSetConfiguredDatabase_False_WrongDeviceName) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_WrongDeviceName());
}

TEST(NI6368DACGTest,TestSetConfiguredDatabase_False_NoTriggers) {
    NI6368DACTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoTriggers());
}
