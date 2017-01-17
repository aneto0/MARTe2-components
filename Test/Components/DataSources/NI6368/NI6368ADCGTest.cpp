/**
 * @file NI6368ADCGTest.cpp
 * @brief Source file for class NI6368ADCGTest
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
 * the class NI6368ADCGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>

#include "../NI6368/NI6368ADCTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI6368ADCGTest,TestConstructor) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI6368ADCGTest,TestAllocateMemory) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(NI6368ADCGTest,TestGetNumberOfMemoryBuffers) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(NI6368ADCGTest,TestGetSignalMemoryBuffer_False) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer_False());
}

TEST(NI6368ADCGTest,TestGetSignalMemoryBuffer) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(NI6368ADCGTest,TestGetBrokerName) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(NI6368ADCGTest,TestGetInputBrokers) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(NI6368ADCGTest,TestGetInputBrokers_NotSynchronisingGAM) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestGetInputBrokers_NotSynchronisingGAM());
}

TEST(NI6368ADCGTest,TestGetOutputBrokers) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(NI6368ADCGTest,TestSynchronise) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(NI6368ADCGTest,TestExecute) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(NI6368ADCGTest,TestPrepareNextState) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(NI6368ADCGTest,TestInitialise) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(NI6368ADCGTest,TestInitialise_False_NoDeviceName) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoDeviceName());
}

TEST(NI6368ADCGTest,TestInitialise_False_NoBoardId) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoBoardId());
}

TEST(NI6368ADCGTest,TestInitialise_False_NoDMABufferSize) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoDMABufferSize());
}

TEST(NI6368ADCGTest,TestInitialise_ClockSampleSource) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_ClockSampleSource());
}

TEST(NI6368ADCGTest,TestInitialise_False_NoClockSampleSource) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoClockSampleSource());
}

TEST(NI6368ADCGTest,TestInitialise_False_BadClockSampleSource) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadClockSampleSource());
}

TEST(NI6368ADCGTest,TestInitialise_ClockSamplePolarity) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_ClockSamplePolarity());
}

TEST(NI6368ADCGTest,TestInitialise_False_NoClockSamplePolarity) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoClockSamplePolarity());
}

TEST(NI6368ADCGTest,TestInitialise_False_BadClockSamplePolarity) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadClockSamplePolarity());
}

TEST(NI6368ADCGTest,TestInitialise_ClockConvertSource) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_ClockConvertSource());
}

TEST(NI6368ADCGTest,TestInitialise_False_NoClockConvertSource) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoClockConvertSource());
}

TEST(NI6368ADCGTest,TestInitialise_False_BadClockConvertSource) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadClockConvertSource());
}

TEST(NI6368ADCGTest,TestInitialise_ClockConvertPolarity) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_ClockConvertPolarity());
}

TEST(NI6368ADCGTest,TestInitialise_False_NoClockConvertPolarity) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoClockConvertPolarity());
}

TEST(NI6368ADCGTest,TestInitialise_False_BadClockConvertPolarity) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadClockConvertPolarity());
}

TEST(NI6368ADCGTest,TestInitialise_False_NoSignals) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoSignals());
}

TEST(NI6368ADCGTest,TestInitialise_InputRange) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_InputRange());
}

TEST(NI6368ADCGTest,TestInitialise_False_BadInputRange) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadInputRange());
}

TEST(NI6368ADCGTest,TestInitialise_DefaultInputRange) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultInputRange());
}

TEST(NI6368ADCGTest,TestInitialise_InputType) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_InputType());
}

TEST(NI6368ADCGTest,TestInitialise_False_BadInputType) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadInputType());
}

TEST(NI6368ADCGTest,TestInitialise_DefaultInputType) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultInputType());
}

TEST(NI6368ADCGTest,TestInitialise_False_BadChannelId) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadChannelId());
}

TEST(NI6368ADCGTest,TestSetConfiguredDatabase_OneChannel) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_OneChannel());
}

TEST(NI6368ADCGTest,TestSetConfiguredDatabase_AllChannels) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_AllChannels());
}

TEST(NI6368ADCGTest,TestSetConfiguredDatabase_AllChannels_Parameters) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_AllChannels_Parameters());
}

TEST(NI6368ADCGTest,TestSetConfiguredDatabase_False_NoChannels) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoChannels());
}

TEST(NI6368ADCGTest,TestSetConfiguredDatabase_False_CounterTypeMismatch) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_CounterTypeMismatch());
}

TEST(NI6368ADCGTest,TestSetConfiguredDatabase_False_TimerTypeMismatch) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_TimerTypeMismatch());
}

TEST(NI6368ADCGTest,TestSetConfiguredDatabase_False_SignalTypeMismatch) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_SignalTypeMismatch());
}

TEST(NI6368ADCGTest,TestSetConfiguredDatabase_False_SignalNumberOfSamplesMismatch) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_SignalNumberOfSamplesMismatch());
}

TEST(NI6368ADCGTest,TestSetConfiguredDatabase_False_WrongDeviceName) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_WrongDeviceName());
}

TEST(NI6368ADCGTest,TestSetConfiguredDatabase_False_WrongFrequencyVsSamples) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_WrongFrequencyVsSamples());
}

TEST(NI6368ADCGTest,TestSetConfiguredDatabase_False_CounterSamples) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_CounterSamples());
}

TEST(NI6368ADCGTest,TestSetConfiguredDatabase_False_TimerSamples) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_TimerSamples());
}

TEST(NI6368ADCGTest,TestIntegrated) {
    NI6368ADCTest test;
    ASSERT_TRUE(test.TestIntegrated());
}
