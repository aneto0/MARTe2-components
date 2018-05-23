/**
 * @file NI6259ADCGTest.cpp
 * @brief Source file for class NI6259ADCGTest
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
 * the class NI6259ADCGTest (public, protected, and private). Be aware that some 
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
#include "NI6259ADCTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI6259ADCGTest,TestConstructor) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI6259ADCGTest,TestAllocateMemory) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(NI6259ADCGTest,TestGetNumberOfMemoryBuffers) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(NI6259ADCGTest,TestGetSignalMemoryBuffer_False) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer_False());
}

TEST(NI6259ADCGTest,TestGetSignalMemoryBuffer) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(NI6259ADCGTest,TestGetBrokerName) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(NI6259ADCGTest,TestGetInputBrokers) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(NI6259ADCGTest,TestGetOutputBrokers) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(NI6259ADCGTest,TestSynchronise) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(NI6259ADCGTest,TestExecute) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(NI6259ADCGTest,TestPrepareNextState) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(NI6259ADCGTest,TestInitialise) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(NI6259ADCGTest,TestInitialise_False_NoSamplingFrequency) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoSamplingFrequency());
}

TEST(NI6259ADCGTest,TestInitialise_False_BadSamplingFrequency) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadSamplingFrequency());
}

TEST(NI6259ADCGTest,TestInitialise_False_BadZeroSamplingFrequency) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadZeroSamplingFrequency());
}

TEST(NI6259ADCGTest,TestInitialise_False_NoDeviceName) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoDeviceName());
}

TEST(NI6259ADCGTest,TestInitialise_False_NoBoardId) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoBoardId());
}

TEST(NI6259ADCGTest,TestInitialise_False_NoDelayDivisor) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoDelayDivisor());
}

TEST(NI6259ADCGTest,TestInitialise_ClockSampleSource) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_ClockSampleSource());
}

TEST(NI6259ADCGTest,TestInitialise_False_NoClockSampleSource) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoClockSampleSource());
}

TEST(NI6259ADCGTest,TestInitialise_False_BadClockSampleSource) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadClockSampleSource());
}

TEST(NI6259ADCGTest,TestInitialise_ClockSamplePolarity) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_ClockSamplePolarity());
}

TEST(NI6259ADCGTest,TestInitialise_False_NoClockSamplePolarity) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoClockSamplePolarity());
}

TEST(NI6259ADCGTest,TestInitialise_False_BadClockSamplePolarity) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadClockSamplePolarity());
}

TEST(NI6259ADCGTest,TestInitialise_ClockConvertSource) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_ClockConvertSource());
}

TEST(NI6259ADCGTest,TestInitialise_False_NoClockConvertSource) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoClockConvertSource());
}

TEST(NI6259ADCGTest,TestInitialise_False_BadClockConvertSource) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadClockConvertSource());
}

TEST(NI6259ADCGTest,TestInitialise_ClockConvertPolarity) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_ClockConvertPolarity());
}

TEST(NI6259ADCGTest,TestInitialise_False_NoClockConvertPolarity) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoClockConvertPolarity());
}

TEST(NI6259ADCGTest,TestInitialise_False_BadClockConvertPolarity) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadClockConvertPolarity());
}

TEST(NI6259ADCGTest,TestInitialise_False_NoSignals) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoSignals());
}

TEST(NI6259ADCGTest,TestInitialise_InputRange) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_InputRange());
}

TEST(NI6259ADCGTest,TestInitialise_False_BadInputRange) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadInputRange());
}

TEST(NI6259ADCGTest,TestInitialise_DefaultInputRange) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultInputRange());
}

TEST(NI6259ADCGTest,TestInitialise_InputPolarity) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_InputPolarity());
}

TEST(NI6259ADCGTest,TestInitialise_False_BadInputPolarity) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadInputPolarity());
}

TEST(NI6259ADCGTest,TestInitialise_DefaultInputPolarity) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultInputPolarity());
}

TEST(NI6259ADCGTest,TestInitialise_InputMode) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_InputMode());
}

TEST(NI6259ADCGTest,TestInitialise_False_BadInputMode) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadInputMode());
}

TEST(NI6259ADCGTest,TestInitialise_DefaultInputMode) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultInputMode());
}

TEST(NI6259ADCGTest,TestInitialise_False_BadChannelId) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestInitialise_False_BadChannelId());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_OneChannel) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_OneChannel());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_AllChannels) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_AllChannels());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_AllChannels_Parameters) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_AllChannels_Parameters());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_False_NoChannels) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoChannels());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_False_CounterTypeMismatch) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_CounterTypeMismatch());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_False_TimerTypeMismatch) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_TimerTypeMismatch());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_False_SignalTypeMismatch) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_SignalTypeMismatch());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_False_SignalNumberOfSamplesMismatch) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_SignalNumberOfSamplesMismatch());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_False_WrongDeviceName) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_WrongDeviceName());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_False_WrongFrequencyVsSamples) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_WrongFrequencyVsSamples());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_False_CounterSamples) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_CounterSamples());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_False_TimerSamples) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_TimerSamples());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_False_MoreThanOneGAM) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThanOneGAM());
}

TEST(NI6259ADCGTest,TestSetConfiguredDatabase_False_NotSynchronisingGAM) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NotSynchronisingGAM());
}

TEST(NI6259ADCGTest,TestIntegrated) {
    NI6259ADCTest test;
    ASSERT_TRUE(test.TestIntegrated());
}
