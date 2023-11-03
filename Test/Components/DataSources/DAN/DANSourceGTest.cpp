/**
 * @file DANSourceGTest.cpp
 * @brief Source file for class DANSourceGTest
 * @date 18/04/2017
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
 * the class DANSourceGTest (public, protected, and private). Be aware that some 
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
#include "DANSourceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(DANSourceGTest,TestConstructor) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(DANSourceGTest,TestAllocateMemory) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(DANSourceGTest,TestGetNumberOfMemoryBuffers) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(DANSourceGTest,TestGetSignalMemoryBuffer) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(DANSourceGTest,TestGetBrokerName_InputSignals) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName_InputSignals());
}

TEST(DANSourceGTest,TestGetBrokerName_MemoryMapAsyncOutputBroker) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName_MemoryMapAsyncOutputBroker());
}

TEST(DANSourceGTest,TestGetBrokerName_MemoryMapAsyncTriggerOutputBroker) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName_MemoryMapAsyncTriggerOutputBroker());
}

TEST(DANSourceGTest,TestGetInputBrokers) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(DANSourceGTest,TestGetOutputBrokers) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(DANSourceGTest,TestIntegratedInApplication_NoTrigger) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_NoTrigger());
}

TEST(DANSourceGTest,TestIntegratedInApplication_Interleave) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Interleave());
}

TEST(DANSourceGTest,TestIntegratedInApplication_FullStream) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_FullStream());
}

TEST(DANSourceGTest,TestIntegratedInApplication_Trigger) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger());
}

TEST(DANSourceGTest,TestIntegratedInApplication_Trigger_AbsoluteTime) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_AbsoluteTime());
}

TEST(DANSourceGTest,TestInitialise) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(DANSourceGTest,TestInitialise_False_NumberOfBuffers) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfBuffers());
}

TEST(DANSourceGTest,TestInitialise_False_NumberOfBuffers_0) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfBuffers_0());
}

TEST(DANSourceGTest,TestInitialise_False_CPUMask) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_CPUMask());
}

TEST(DANSourceGTest,TestInitialise_False_StackSize) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_StackSize());
}

TEST(DANSourceGTest,TestInitialise_False_StackSize_0) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_StackSize_0());
}

TEST(DANSourceGTest,TestInitialise_False_DanBufferMultiplier) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_DanBufferMultiplier());
}

TEST(DANSourceGTest,TestInitialise_False_StoreOnTrigger) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_StoreOnTrigger());
}

TEST(DANSourceGTest,TestInitialise_False_NumberOfPreTriggers) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfPreTriggers());
}

TEST(DANSourceGTest,TestInitialise_False_NumberOfPostTriggers) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfPostTriggers());
}

TEST(DANSourceGTest,TestInitialise_False_Signals) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_Signals());
}

TEST(DANSourceGTest,TestSetConfiguredDatabase_False_MoreThanOneTimeSignal) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThanOneTimeSignal());
}

TEST(DANSourceGTest,TestSetConfiguredDatabase_False_NoDANSignals) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoDANSignals());
}

TEST(DANSourceGTest,TestSetConfiguredDatabase_False_NoTimeSignal) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoTimeSignal());
}

TEST(DANSourceGTest,TestSetConfiguredDatabase_False_NumberOfSamples) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NumberOfSamples());
}

TEST(DANSourceGTest,TestSetConfiguredDatabase_False_TimeSignal_NotUInt32) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_TimeSignal_NotUInt32());
}

TEST(DANSourceGTest,TestSetConfiguredDatabase_False_AbsTimeSignal_NotUInt64) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_AbsTimeSignal_NotUInt64());
}

TEST(DANSourceGTest,TestSetConfiguredDatabase_False_TimeSignal_MoreThanOneFunction) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_TimeSignal_MoreThanOneFunction());
}

TEST(DANSourceGTest,TestSetConfiguredDatabase_False_Period_0) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_Period_0());
}

TEST(DANSourceGTest,TestSetConfiguredDatabase_False_SamplingFrequency_0) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_SamplingFrequency_0());
}

TEST(DANSourceGTest,TestGetCPUMask) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetCPUMask());
}

TEST(DANSourceGTest,TestGetNumberOfBuffers) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetNumberOfBuffers());
}

TEST(DANSourceGTest,TestGetNumberOfPostTriggers) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetNumberOfPostTriggers());
}

TEST(DANSourceGTest,TestGetNumberOfPreTriggers) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetNumberOfPreTriggers());
}

TEST(DANSourceGTest,TestGetStackSize) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetStackSize());
}

TEST(DANSourceGTest,TestIsStoreOnTrigger) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestIsStoreOnTrigger());
}

TEST(DANSourceGTest,TestGetTimeSignalIdx) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestGetTimeSignalIdx());
}

TEST(DANSourceGTest,TestIsAbsoluteTime) {
    DANSourceTest test;
    ASSERT_TRUE(test.TestIsAbsoluteTime());
}
