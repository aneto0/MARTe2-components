/**
 * @file MDSReaderGTest.cpp
 * @brief Source file for class MDSReaderGTest
 * @date 22/09/2017
 * @author Llorenc Capella
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
 * the class MDSReaderGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MDSReaderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(MDSReaderGTest, TestInitialiseNoTreeName) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestInitialiseNoTreeName());
}

TEST(MDSReaderGTest, TestInitialiseWrongShotNumber) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestInitialiseWrongShotNumber());
}

TEST(MDSReaderGTest, TestInitialiseWrongTreeName) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestInitialiseWrongTreeName());
}

TEST(MDSReaderGTest, TestInitialiseUnexistingShotNumber) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestInitialiseUnexistingShotNumber());
}

TEST(MDSReaderGTest, TestInitialiseNoSignals) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestInitialiseNoSignals());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseNoSignals) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseNoSignals());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabase0Functions) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase0Functions());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabase2Functions) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase2Functions());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabase0Signals) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase0Signals());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseWrongSamples) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseWrongSamples());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseNoNodeName) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseNoNodeName());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseNoNodeName_2) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseNoNodeName_2());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInvalidNodeName) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInvalidNodeName());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseEqualNodeName) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseEqualNodeName());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInvalidNodeType) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInvalidNodeType());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseWrongType) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseWrongType());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseNoNumberOfElements) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseNoNumberOfElements());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInvalidNumberOfElements) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInvalidNumberOfElements());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInvalidBytesSize) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInvalidBytesSize());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabase) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(MDSReaderGTest, TestPrepareNextState) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(MDSReaderGTest, TestAllocateMemory) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(MDSReaderGTest, TestGetNumberOfMemoryBuffers) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(MDSReaderGTest, TestGetSignalMemoryBufferNoMemory) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBufferNoMemory());
}

TEST(MDSReaderGTest, TestGetSignalMemoryBuffer) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

