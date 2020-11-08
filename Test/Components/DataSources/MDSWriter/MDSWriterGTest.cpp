/**
 * @file MDSWriterGTest.cpp
 * @brief Source file for class MDSWriterGTest
 * @date 14/02/2017
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
 * the class MDSWriterGTest (public, protected, and private). Be aware that some
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
#include "MDSWriterTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(MDSWriterGTest,TestConstructor) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(MDSWriterGTest,TestInitialise) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(MDSWriterGTest,TestInitialise_False_NumberOfBuffers) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfBuffers());
}

TEST(MDSWriterGTest,TestInitialise_False_CPUMask) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_CPUMask());
}

TEST(MDSWriterGTest,TestInitialise_False_StackSize) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_StackSize());
}

TEST(MDSWriterGTest,TestInitialise_False_TreeName) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_TreeName());
}

TEST(MDSWriterGTest,TestInitialise_False_EventName) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_EventName());
}

TEST(MDSWriterGTest,TestInitialise_False_TimeRefresh) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_TimeRefresh());
}

TEST(MDSWriterGTest,TestInitialise_False_StoreOnTrigger) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_StoreOnTrigger());
}

TEST(MDSWriterGTest,TestInitialise_False_NumberOfPreTriggers) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfPreTriggers());
}

TEST(MDSWriterGTest,TestInitialise_False_NumberOfPostTriggers) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfPostTriggers());
}

TEST(MDSWriterGTest,TestInitialise_False_Signals) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_Signals());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_NoTrigger) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_NoTrigger());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_NoTrigger_Flush) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_NoTrigger_Flush());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger_NotMultiple) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_NotMultiple());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger_NotMultiple_Samples) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_NotMultiple_Samples());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger_Time8) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_Time8());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger_Time16) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_Time16());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger_Time64) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_Time64());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger_TimeS8) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_TimeS8());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger_TimeS16) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_TimeS16());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger_TimeS64) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_TimeS64());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger_Discontinuity) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_Discontinuity());
}
TEST(MDSWriterGTest,TestIntegratedInApplication_NoTrigger_Elements) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_NoTrigger_Elements());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger_Elements) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_Elements());
}

TEST(MDSWriterGTest,TestSetConfiguredDatabase) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_NoTrigger_AutomaticSegmentation) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_NoTrigger_AutomaticSegmentation());
}

TEST(MDSWriterGTest,TestSetConfiguredDatabase_False_MoreThanOneTimeSignal) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThanOneTimeSignal());
}

TEST(MDSWriterGTest,TestSetConfiguredDatabase_False_NoMDSSignals) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoMDSSignals());
}

TEST(MDSWriterGTest,TestSetConfiguredDatabase_False_NoTimeSignal) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoTimeSignal());
}

TEST(MDSWriterGTest,TestSetConfiguredDatabase_False_NumberOfDimensions) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NumberOfDimensions());
}

TEST(MDSWriterGTest,TestSetConfiguredDatabase_False_NumberOfSamples) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NumberOfSamples());
}

TEST(MDSWriterGTest,TestSetConfiguredDatabase_False_TimeSignal_NotInteger) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_TimeSignal_NotInteger());
}

TEST(MDSWriterGTest,TestSetConfiguredDatabase_False_TimeSignal_MoreThanOneFunction) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_TimeSignal_MoreThanOneFunction());
}

TEST(MDSWriterGTest,TestOpenTree) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestOpenTree());
}

TEST(MDSWriterGTest,TestAllocateMemory) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(MDSWriterGTest,TestGetNumberOfMemoryBuffers) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(MDSWriterGTest,TestGetSignalMemoryBuffer) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(MDSWriterGTest,TestGetBrokerName_InputSignals) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetBrokerName_InputSignals());
}

TEST(MDSWriterGTest,TestGetBrokerName_MemoryMapAsyncOutputBroker) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetBrokerName_MemoryMapAsyncOutputBroker());
}

TEST(MDSWriterGTest,TestGetBrokerName_MemoryMapAsyncTriggerOutputBroker) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetBrokerName_MemoryMapAsyncTriggerOutputBroker());
}

TEST(MDSWriterGTest,TestGetInputBrokers) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(MDSWriterGTest,TestGetOutputBrokers) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(MDSWriterGTest,TestSynchronise) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(MDSWriterGTest,TestPrepareNextState) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(MDSWriterGTest,TestGetCPUMask) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetCPUMask());
}

TEST(MDSWriterGTest,TestGetEventName) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetEventName());
}

TEST(MDSWriterGTest,TestGetNumberOfBuffers) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfBuffers());
}

TEST(MDSWriterGTest,TestGetNumberOfMdsSignals) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfMdsSignals());
}

TEST(MDSWriterGTest,TestGetNumberOfPostTriggers) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfPostTriggers());
}

TEST(MDSWriterGTest,TestGetNumberOfPreTriggers) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfPreTriggers());
}

TEST(MDSWriterGTest,TestGetPulseNumber) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetPulseNumber());
}

TEST(MDSWriterGTest,TestGetTreeName) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetTreeName());
}

TEST(MDSWriterGTest,TestGetRefreshEveryCounts) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetRefreshEveryCounts());
}

TEST(MDSWriterGTest,TestGetStackSize) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetStackSize());
}

TEST(MDSWriterGTest,TestIsStoreOnTrigger) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIsStoreOnTrigger());
}

TEST(MDSWriterGTest,TestGetTimeSignalIdx) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestGetTimeSignalIdx());
}

TEST(MDSWriterGTest,TestOpenTreeOKMessage) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestOpenTreeOKMessage());
}

TEST(MDSWriterGTest,TestOpenTreeFailMessage) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestOpenTreeFailMessage());
}

TEST(MDSWriterGTest,TestFlushSegmentsMessage) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestFlushSegmentsMessage());
}

TEST(MDSWriterGTest,TestOpenTreeOKMessage_Fail) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestOpenTreeOKMessage_Fail());
}

TEST(MDSWriterGTest,TestOpenTreeFailMessage_Fail) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestOpenTreeFailMessage_Fail());
}

TEST(MDSWriterGTest,TestFlushSegmentsMessage_Fail) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestFlushSegmentsMessage_Fail());
}

TEST(MDSWriterGTest,TestInvalidMessageName) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInvalidMessageName());
}

TEST(MDSWriterGTest,TestInvalidMessageType) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestInvalidMessageType());
}
