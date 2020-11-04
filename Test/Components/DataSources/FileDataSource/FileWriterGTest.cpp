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
#include "gtest/gtest.h"
#include <limits.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "FileWriterTest.h"

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

TEST(FileWriterGTest,TestInitialise) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(FileWriterGTest,TestInitialise_Binary) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_Binary());
}

TEST(FileWriterGTest,TestInitialise_False_NumberOfBuffers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfBuffers());
}

TEST(FileWriterGTest,TestInitialise_False_Overwrite) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_Overwrite());
}

TEST(FileWriterGTest,TestInitialise_False_Overwrite_Invalid) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_Overwrite_Invalid());
}

TEST(FileWriterGTest,TestInitialise_False_NumberOfBuffers_GT_0) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfBuffers_GT_0());
}

TEST(FileWriterGTest,TestInitialise_False_CPUMask) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_CPUMask());
}

TEST(FileWriterGTest,TestInitialise_False_StackSize) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_StackSize());
}

TEST(FileWriterGTest,TestInitialise_False_StackSize_GT_0) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_StackSize_GT_0());
}

TEST(FileWriterGTest,TestInitialise_Warning_Filename) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_Warning_Filename());
}

TEST(FileWriterGTest,TestInitialise_False_FileFormat) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_FileFormat());
}

TEST(FileWriterGTest,TestInitialise_False_FileFormat_Invalid) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_FileFormat_Invalid());
}

TEST(FileWriterGTest,TestInitialise_False_CSVSeparator) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInitialise_False_CSVSeparator());
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

TEST(FileWriterGTest,TestSetConfiguredDatabase) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(FileWriterGTest,TestSetConfiguredDatabase_False_NoFileSignals) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoFileSignals());
}

TEST(FileWriterGTest,TestSetConfiguredDatabase_False_NumberOfSamples) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NumberOfSamples());
}

TEST(FileWriterGTest,TestSetConfiguredDatabase_False_TimeSignal_MoreThanOneFunction) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_TimeSignal_MoreThanOneFunction());
}

TEST(FileWriterGTest,TestOpenFile) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestOpenFile());
}

TEST(FileWriterGTest,TestOpenFile_Overwrite) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestOpenFile_Overwrite());
}

TEST(FileWriterGTest,TestCloseFile) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestCloseFile());
}

TEST(FileWriterGTest,TestFlushFile) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestFlushFile());
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

TEST(FileWriterGTest,TestGetFilename) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetFilename());
}

TEST(FileWriterGTest,TestGetFileFormat) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetFileFormat());
}

TEST(FileWriterGTest,TestGetCSVSeparator) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetCSVSeparator());
}

TEST(FileWriterGTest,TestGetNumberOfBuffers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfBuffers());
}

TEST(FileWriterGTest,TestGetNumberOfPostTriggers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfPostTriggers());
}

TEST(FileWriterGTest,TestGetNumberOfPreTriggers) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetNumberOfPreTriggers());
}

TEST(FileWriterGTest,TestGetStackSize) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestGetStackSize());
}

TEST(FileWriterGTest,TestIsOverwrite) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestIsOverwrite());
}

TEST(FileWriterGTest,TestIsStoreOnTrigger) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestIsStoreOnTrigger());
}

TEST(FileWriterGTest,TestOpenFileOKMessage) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestOpenFileOKMessage());
}

TEST(FileWriterGTest,TestOpenFileFailMessage) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestOpenFileFailMessage());
}

TEST(FileWriterGTest,TestCloseFileMessage) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestCloseFileMessage());
}

TEST(FileWriterGTest,TestRuntimeErrorMessage) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestRuntimeErrorMessage());
}

TEST(FileWriterGTest,TestOpenFileOKMessage_Fail) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestOpenFileOKMessage_Fail());
}

TEST(FileWriterGTest,TestOpenFileFailMessage_Fail) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestOpenFileFailMessage_Fail());
}

TEST(FileWriterGTest,TestCloseFileMessage_Fail) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestCloseFileMessage_Fail());
}

TEST(FileWriterGTest,TestRuntimeErrorMessage_Fail) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestRuntimeErrorMessage_Fail());
}

TEST(FileWriterGTest,TestInvalidMessageName) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInvalidMessageName());
}

TEST(FileWriterGTest,TestInvalidMessageType) {
    FileWriterTest test;
    ASSERT_TRUE(test.TestInvalidMessageType());
}
