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

TEST(FileReaderGTest,TestInitialise_False_CorruptedFile) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_CorruptedFile());
}

TEST(FileReaderGTest,TestInitialise_False_XAxisSignal) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_XAxisSignal());
}

TEST(FileReaderGTest,TestInitialise_False_InterpolationPeriod) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_InterpolationPeriod());
}

TEST(FileReaderGTest,TestSetConfiguredDatabase) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}


TEST(FileReaderGTest,TestSetConfiguredDatabase_False_NumberOfSamples) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NumberOfSamples());
}

TEST(FileReaderGTest,TestSetConfiguredDatabase_False_MoreThanOneFunction) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThanOneFunction());
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
TEST(FileReaderGTest,TestSynchronise) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestSynchronise());
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

TEST(FileReaderGTest,TestInvalidMessageName) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInvalidMessageName());
}

TEST(FileReaderGTest,TestInvalidMessageType) {
    FileReaderTest test;
    ASSERT_TRUE(test.TestInvalidMessageType());
}
