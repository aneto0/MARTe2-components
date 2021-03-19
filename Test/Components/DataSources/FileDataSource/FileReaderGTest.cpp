/**
 * @file FileReaderGTest.cpp
 * @brief Source file for class FileReaderGTest
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
 * the class FileReaderGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>

#include "FileReaderTest.h"
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
TEST(FileReaderGTest,TestConstructor) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FileReaderGTest,TestAllocateMemory) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(FileReaderGTest,TestGetNumberOfMemoryBuffers) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(FileReaderGTest,TestGetSignalMemoryBuffer) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(FileReaderGTest,TestInitialise) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(FileReaderGTest,TestInitialise_1) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_1());
}

TEST(FileReaderGTest,TestInitialise_Binary) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_Binary());
}

TEST(FileReaderGTest,TestInitialise_False_Filename) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_Filename());
}

TEST(FileReaderGTest,TestInitialise_False_Filename_1) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_Filename_1());
}

TEST(FileReaderGTest,TestInitialise_False_Interpolate) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_Interpolate());
}

TEST(FileReaderGTest,TestInitialise_False_Interpolate_Invalid) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_Interpolate_Invalid());
}

TEST(FileReaderGTest,TestInitialise_False_FileFormat) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_FileFormat());
}

TEST(FileReaderGTest,TestInitialise_False_FileFormat_Invalid) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_FileFormat_Invalid());
}

TEST(FileReaderGTest,TestInitialise_False_CSVSeparator) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_CSVSeparator());
}

TEST(FileReaderGTest,TestInitialise_False_EOF) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_EOF());
}

TEST(FileReaderGTest,TestInitialise_False_CorruptedFile) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_CorruptedFile());
}

TEST(FileReaderGTest,TestInitialise_Warning_XAxisSignal) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_Warning_XAxisSignal());
}

TEST(FileReaderGTest,TestInitialise_Warning_InterpolationPeriod) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_Warning_InterpolationPeriod());
}

TEST(FileReaderGTest,TestInitialise_Preload_no) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_Preload_no());
}

TEST(FileReaderGTest,TestInitialise_Preload_yes_NoMaxSize) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_Preload_yes_NoMaxSize());
}

TEST(FileReaderGTest,TestInitialise_Preload_yes_MaxSizeToLarge) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_Preload_yes_MaxSizeToLarge());
}

TEST(FileReaderGTest,TestSetConfiguredDatabase) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(FileReaderGTest,TestSetConfiguradDatabase_InputSignalSize0) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguradDatabase_InputSignalSize0());
}

TEST(FileReaderGTest,TestSetConfiguredDatabase_False_NumberOfSamples) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NumberOfSamples());
}

TEST(FileReaderGTest,TestSetConfiguredDatabase_WrongFileSize_CSV) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_WrongFileSize_CSV());
}

TEST(FileReaderGTest,TestSetConfiguredDatabase_WrongFileSize_Bin) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_WrongFileSize_Bin());
}

TEST(FileReaderGTest,TestSetConfiguredDatabase_MaxSizeSmallerThanFileSize) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_MaxSizeSmallerThanFileSize());
}

TEST(FileReaderGTest,TestSetConfiguredDatabase_False_MoreThanOneFunction) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThanOneFunction());
}

TEST(FileReaderGTest,TestSetConfiguredDatabase_False_XAxisSignal) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_XAxisSignal());
}

TEST(FileReaderGTest,TestSetConfiguredDatabase_False_XAxisSignal_Dimensions) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_XAxisSignal_Dimensions());
}

TEST(FileReaderGTest,TestOpenFile) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestOpenFile());
}

TEST(FileReaderGTest,TestCloseFile) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestCloseFile());
}

TEST(FileReaderGTest,TestGetBrokerName_OutputSignals) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestGetBrokerName_OutputSignals());
}

TEST(FileReaderGTest,TestGetBrokerName_MemoryMapInterpolatedInputBroker) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestGetBrokerName_MemoryMapInterpolatedInputBroker());
}

TEST(FileReaderGTest,TestGetBrokerName_MemoryMapInputBroker) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestGetBrokerName_MemoryMapInputBroker());
}

TEST(FileReaderGTest,TestGetInputBrokers) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(FileReaderGTest,TestGetOutputBrokers) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(FileReaderGTest,TestSynchronise_Binary) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSynchronise_Binary());
}

TEST(FileReaderGTest,TestSynchronise_Binary_Interpolation) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSynchronise_Binary_Interpolation());
}

TEST(FileReaderGTest,TestSynchronise_Binary_Interpolation_Preload) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSynchronise_Binary_Interpolation_Preload());
}

TEST(FileReaderGTest,TestSynchronise_CSV) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSynchronise_CSV());
}

TEST(FileReaderGTest,TestSynchronise_CSV_Strings) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSynchronise_CSV_Strings());
}

TEST(FileReaderGTest,TestSynchronise_CSV_Comma) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSynchronise_CSV_Comma());
}

TEST(FileReaderGTest,TestSynchronise_CSV_Interpolation) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSynchronise_CSV_Interpolation());
}

TEST(FileReaderGTest,TestSynchronise_CSV_Interpolation_Preload) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSynchronise_CSV_Interpolation_Preload());
}

TEST(FileReaderGTest,TestSynchronise_Frequency) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSynchronise_Frequency());
}

TEST(FileReaderGTest,TestSynchronise_XAxisSignals) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSynchronise_XAxisSignals());
}

TEST(FileReaderGTest,TestPrepareNextState) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(FileReaderGTest,TestGetFilename) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestGetFilename());
}

TEST(FileReaderGTest,TestGetFileFormat) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestGetFileFormat());
}

TEST(FileReaderGTest,TestGetCSVSeparator) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestGetCSVSeparator());
}

TEST(FileReaderGTest,TestGetXAxisSignal) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestGetXAxisSignal());
}

TEST(FileReaderGTest,TestGetInterpolationPeriod) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestGetInterpolationPeriod());
}

TEST(FileReaderGTest,TestIsInterpolate) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestIsInterpolate());
}

TEST(FileReaderGTest,TestRuntimeErrorMessage) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestRuntimeErrorMessage());
}

TEST(FileReaderGTest,TestRuntimeErrorMessage_2) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestRuntimeErrorMessage_2());
}

TEST(FileReaderGTest,TestRuntimeErrorMessage_Fail) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestRuntimeErrorMessage_Fail());
}

TEST(FileReaderGTest,TestRuntimeErrorMessage_Fail_Preload) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestRuntimeErrorMessage_Fail_Preload());
}

TEST(FileReaderGTest,TestInvalidMessageName) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInvalidMessageName());
}

TEST(FileReaderGTest,TestInvalidMessageType) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInvalidMessageType());
}

TEST(FileReaderGTest,TestInvalidMessageType_Preload) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInvalidMessageType_Preload());
}

TEST(FileReaderGTest,TestEOF_CSV) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_CSV());
}

TEST(FileReaderGTest,TestEOF_CSV_Preload) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_CSV_Preload());
}

TEST(FileReaderGTest,TestEOF_Binary) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Binary());
}

TEST(FileReaderGTest,TestEOF_Binary_JumboFile) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Binary_JumboFile());
}

TEST(FileReaderGTest,TestEOF_Binary_Preload) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Binary_Preload());
}

TEST(FileReaderGTest,TestEOF_Rewind_CSV) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Rewind_CSV());
}

TEST(FileReaderGTest,TestEOF_Rewind_CSV_Preload) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Rewind_CSV_Preload());
}

TEST(FileReaderGTest,TestEOF_Rewind_Binary) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Rewind_Binary());
}

TEST(FileReaderGTest,TestEOF_Rewind_Binary_Preload) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Rewind_Binary_Preload());
}

TEST(FileReaderGTest,TestEOF_Last_CSV) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Last_CSV());
}

TEST(FileReaderGTest,TestEOF_Last_CSV_Preload) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Last_CSV_Preload());
}

TEST(FileReaderGTest,TestEOF_Last_Binary) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Last_Binary());
}

TEST(FileReaderGTest,TestEOF_Last_Binary_Preload) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Last_Binary_Preload());
}

TEST(FileReaderGTest,TestEOF_Error_CSV) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Error_CSV());
}

TEST(FileReaderGTest,TestEOF_Error_CSV_Preload) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Error_CSV_Preload());
}

TEST(FileReaderGTest,TestEOF_Error_Binary) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Error_Binary());
}

TEST(FileReaderGTest,TestEOF_Error_Binary_Preload) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestEOF_Error_Binary_Preload());
}

