/**
 * @file FileWriterGTest.cpp
 * @brief Source file for class FileWriterGTest
 * @date 14/08/2017
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
 * the class FileWriterGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>

#include "FileWriterTest.h"
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
TEST(FileWriterGTest,TestConstructor) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FileWriterGTest,TestAllocateMemory) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(FileWriterGTest,TestGetNumberOfMemoryBuffers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(FileWriterGTest,TestGetSignalMemoryBuffer) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

#if 0
TEST(FileWriterGTest,TestInitialise) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(FileWriterGTest,TestInitialise_False_NumberOfBuffers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfBuffers());
}

TEST(FileWriterGTest,TestInitialise_False_CPUMask) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_CPUMask());
}

TEST(FileWriterGTest,TestInitialise_False_StackSize) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_StackSize());
}

TEST(FileWriterGTest,TestInitialise_False_TreeName) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_TreeName());
}

TEST(FileWriterGTest,TestInitialise_False_EventName) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_EventName());
}

TEST(FileWriterGTest,TestInitialise_False_TimeRefresh) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_TimeRefresh());
}

TEST(FileWriterGTest,TestInitialise_False_StoreOnTrigger) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_StoreOnTrigger());
}

TEST(FileWriterGTest,TestInitialise_False_NumberOfPreTriggers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfPreTriggers());
}

TEST(FileWriterGTest,TestInitialise_False_NumberOfPostTriggers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfPostTriggers());
}

TEST(FileWriterGTest,TestInitialise_False_Signals) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_Signals());
}

TEST(FileWriterGTest,TestIntegratedInApplication_NoTrigger) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_NoTrigger());
}

TEST(FileWriterGTest,TestIntegratedInApplication_NoTrigger_Flush) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_NoTrigger_Flush());
}

TEST(FileWriterGTest,TestIntegratedInApplication_Trigger) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger());
}

TEST(FileWriterGTest,TestIntegratedInApplication_Trigger_Discontinuity) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_Discontinuity());
}
TEST(FileWriterGTest,TestIntegratedInApplication_NoTrigger_Elements) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_NoTrigger_Elements());
}

TEST(FileWriterGTest,TestIntegratedInApplication_Trigger_Elements) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_Elements());
}

TEST(FileWriterGTest,TestSetConfiguredDatabase) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(FileWriterGTest,TestSetConfiguredDatabase_False_MoreThanOneTimeSignal) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThanOneTimeSignal());
}

TEST(FileWriterGTest,TestSetConfiguredDatabase_False_NoFileSignals) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoFileSignals());
}

TEST(FileWriterGTest,TestSetConfiguredDatabase_False_NoTimeSignal) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoTimeSignal());
}

TEST(FileWriterGTest,TestSetConfiguredDatabase_False_NumberOfSamples) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NumberOfSamples());
}

TEST(FileWriterGTest,TestSetConfiguredDatabase_False_TimeSignal_NotUInt32) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_TimeSignal_NotUInt32());
}

TEST(FileWriterGTest,TestSetConfiguredDatabase_False_TimeSignal_MoreThanOneFunction) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_TimeSignal_MoreThanOneFunction());
}

TEST(FileWriterGTest,TestOpenTree) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestOpenTree());
}


TEST(FileWriterGTest,TestGetBrokerName_InputSignals) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetBrokerName_InputSignals());
}

TEST(FileWriterGTest,TestGetBrokerName_MemoryMapAsyncOutputBroker) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetBrokerName_MemoryMapAsyncOutputBroker());
}

TEST(FileWriterGTest,TestGetBrokerName_MemoryMapAsyncTriggerOutputBroker) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetBrokerName_MemoryMapAsyncTriggerOutputBroker());
}

TEST(FileWriterGTest,TestGetInputBrokers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(FileWriterGTest,TestGetOutputBrokers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(FileWriterGTest,TestSynchronise) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(FileWriterGTest,TestPrepareNextState) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(FileWriterGTest,TestGetCPUMask) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetCPUMask());
}

TEST(FileWriterGTest,TestGetEventName) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetEventName());
}

TEST(FileWriterGTest,TestGetNumberOfBuffers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfBuffers());
}

TEST(FileWriterGTest,TestGetNumberOfMdsSignals) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfMdsSignals());
}

TEST(FileWriterGTest,TestGetNumberOfPostTriggers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfPostTriggers());
}

TEST(FileWriterGTest,TestGetNumberOfPreTriggers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfPreTriggers());
}

TEST(FileWriterGTest,TestGetPulseNumber) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetPulseNumber());
}

TEST(FileWriterGTest,TestGetTreeName) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetTreeName());
}

TEST(FileWriterGTest,TestGetRefreshEveryCounts) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetRefreshEveryCounts());
}

TEST(FileWriterGTest,TestGetStackSize) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetStackSize());
}

TEST(FileWriterGTest,TestIsStoreOnTrigger) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestIsStoreOnTrigger());
}

TEST(FileWriterGTest,TestGetTimeSignalIdx) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetTimeSignalIdx());
}

TEST(FileWriterGTest,TestOpenTreeOKMessage) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestOpenTreeOKMessage());
}

TEST(FileWriterGTest,TestOpenTreeFailMessage) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestOpenTreeFailMessage());
}

TEST(FileWriterGTest,TestFlushSegmentsMessage) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestFlushSegmentsMessage());
}

TEST(FileWriterGTest,TestOpenTreeOKMessage_Fail) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestOpenTreeOKMessage_Fail());
}

TEST(FileWriterGTest,TestOpenTreeFailMessage_Fail) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestOpenTreeFailMessage_Fail());
}

TEST(FileWriterGTest,TestFlushSegmentsMessage_Fail) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestFlushSegmentsMessage_Fail());
}

TEST(FileWriterGTest,TestInvalidMessageName) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInvalidMessageName());
}

TEST(FileWriterGTest,TestInvalidMessageType) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInvalidMessageType());
}
#endif
