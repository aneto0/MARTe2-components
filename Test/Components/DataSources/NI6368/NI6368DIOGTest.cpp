/**
 * @file NI6368DIOGTest.cpp
 * @brief Source file for class NI6368DIOGTest
 * @date 30/1/2017
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
 * the class NI6368DIOGTest (public, protected, and private). Be aware that some
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
#include "NI6368DIOTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI6368DIOGTest,TestConstructor) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI6368DIOGTest,TestAllocateMemory) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(NI6368DIOGTest,TestGetNumberOfMemoryBuffers) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(NI6368DIOGTest,TestGetSignalMemoryBuffer_False) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer_False());
}

TEST(NI6368DIOGTest,TestGetSignalMemoryBuffer) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(NI6368DIOGTest,TestGetBrokerName) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(NI6368DIOGTest,TestGetInputBrokers) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(NI6368DIOGTest,TestGetOutputBrokers) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(NI6368DIOGTest,TestGetOutputBrokers_NoTriggerGAM) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers_NoTriggerGAM());
}

TEST(NI6368DIOGTest,TestSynchronise) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(NI6368DIOGTest,TestPrepareNextState) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(NI6368DIOGTest,TestInitialise) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(NI6368DIOGTest,TestInitialise_False_NoDeviceName) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoDeviceName());
}

TEST(NI6368DIOGTest,TestInitialise_False_NoBoardId) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoBoardId());
}

TEST(NI6368DIOGTest,TestInitialise_False_NoSignals) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoSignals());
}

TEST(NI6368DIOGTest,TestInitialise_False_BadPortId) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadPortId());
}

TEST(NI6368DIOGTest,TestSetConfiguredDatabasel) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(NI6368DIOGTest,TestSetConfiguredDatabase_OnlyInputs) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_OnlyInputs());
}

TEST(NI6368DIOGTest,TestSetConfiguredDatabase_OnlyOutputs) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_OnlyOutputs());
}

TEST(NI6368DIOGTest,TestSetConfiguredDatabase_AllChannels) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_AllChannels());
}

TEST(NI6368DIOGTest,TestSetConfiguredDatabase_AllChannels_Parameters) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_AllChannels_Parameters());
}

TEST(NI6368DIOGTest,TestSetConfiguredDatabase_False_NoChannels) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoChannels());
}

TEST(NI6368DIOGTest,TestSetConfiguredDatabase_False_SignalTypeMismatch) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_SignalTypeMismatch());
}

TEST(NI6368DIOGTest,TestSetConfiguredDatabase_False_SignalNumberOfSamplesGreaterThanOne) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_SignalNumberOfSamplesGreaterThanOne());
}

TEST(NI6368DIOGTest,TestSetConfiguredDatabase_False_WrongDeviceName) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_WrongDeviceName());
}

TEST(NI6368DIOGTest,TestSetConfiguredDatabase_False_NoTriggers) {
    NI6368DIOTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoTriggers());
}
