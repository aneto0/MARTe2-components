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

TEST(MDSReaderGTest, TestInitialiseNoShotNumber) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestInitialiseNoShotNumber());
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

TEST(MDSReaderGTest, TestInitialiseNoFrequency) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestInitialiseNoFrequency());
}

TEST(MDSReaderGTest, TestInitialiseNoSignals) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestInitialiseNoSignals());
}

TEST(MDSReaderGTest, TestInitialise) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestInitialise());
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

TEST(MDSReaderGTest, TestSetConfiguredDatabaseDiffSignalsAndFunctions) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseDiffSignalsAndFunctions());
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

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInconsistentType) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInconsistentType());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseNoNumberOfElements) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseNoNumberOfElements());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInvalidNumberOfElements) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInvalidNumberOfElements());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseNoTimeNumberOfElements) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseNoTimeNumberOfElements());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInvalidTimeNumberOfElements) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInvalidTimeNumberOfElements());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInvalidBytesSize) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInvalidBytesSize());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInvalidTimeBytesSize) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInvalidTimeBytesSize());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseNoDataManagement) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseNoDataManagement());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInvalidDataManagement) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInvalidDataManagement());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseDataManagement0) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseDataManagement0());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseNoHoleManagement) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseNoHoleManagement());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInvalidHoleManagement) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInvalidHoleManagement());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInvalidTimeType) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInvalidTimeType());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseInvalidSamplingTime) {
    MDSReaderTest test("s_tree", 1, 1, 2.0);
    ASSERT_TRUE(test.TestSetConfiguredDatabaseInvalidSamplingTime());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabase) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(MDSReaderGTest, TestSetConfiguredDatabaseShotNumber) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabaseShotNumber());
}

TEST(MDSReaderGTest, TestPrepareNextState) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(MDSReaderGTest, TestAllocateMemory) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(MDSReaderGTest, TestGetBrokerName) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(MDSReaderGTest, TestGetInputBroker) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestGetInputBroker());
}

TEST(MDSReaderGTest, TestGetOutputBroker) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestGetOutputBroker());
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

TEST(MDSReaderGTest, TestSynchronise) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(MDSReaderGTest, TestSynchronise1) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise1());
}

TEST(MDSReaderGTest, TestSynchronise2) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise2());
}

TEST(MDSReaderGTest, TestSynchronise3) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise3());
}

TEST(MDSReaderGTest, TestSynchronise4) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise4());
}

TEST(MDSReaderGTest, TestSynchronise5) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise5());
}

TEST(MDSReaderGTest, TestSynchronise6) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise6());
}

TEST(MDSReaderGTest, TestSynchronise7) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise7());
}

TEST(MDSReaderGTest, TestSynchronise8) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise8());
}

TEST(MDSReaderGTest, TestSynchronise9) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise9());
}

TEST(MDSReaderGTest, TestSynchronise10) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise10());
}

TEST(MDSReaderGTest, TestSynchronise11) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise11());
}

TEST(MDSReaderGTest, TestSynchronise11_1) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise11_1());
}

TEST(MDSReaderGTest, TestSynchronise11_2) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise11_2());
}

TEST(MDSReaderGTest, TestSynchronise11_3) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise11_3());
}

TEST(MDSReaderGTest, TestSynchronise11_4) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise11_4());
}

TEST(MDSReaderGTest, TestSynchronise11_5) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise11_5());
}

TEST(MDSReaderGTest, TestSynchronise12) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise12());
}

TEST(MDSReaderGTest, TestSynchronise13) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise13());
}

TEST(MDSReaderGTest, TestSynchronise14) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise14());
}

TEST(MDSReaderGTest, TestSynchronise15) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise15());
}

TEST(MDSReaderGTest, TestSynchronise16) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise16());
}

TEST(MDSReaderGTest, TestSynchronise17) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise17());
}

TEST(MDSReaderGTest, TestSynchronise18) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise18());
}

TEST(MDSReaderGTest, TestSynchronise19) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise19());
}

TEST(MDSReaderGTest, TestSynchronise20) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise20());
}

TEST(MDSReaderGTest, TestSynchronise21) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise21());
}

TEST(MDSReaderGTest, TestSynchronise22) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise22());
}

TEST(MDSReaderGTest, TestSynchronise23) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise23());
}

TEST(MDSReaderGTest, TestSynchronise24) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise24());
}

TEST(MDSReaderGTest, TestSynchronise25) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise25());
}

TEST(MDSReaderGTest, TestSynchronise26) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise26());
}

TEST(MDSReaderGTest, TestSynchronise27) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise27());
}

TEST(MDSReaderGTest, TestSynchronise28) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise28());
}

TEST(MDSReaderGTest, TestSynchronise29) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise29());
}

TEST(MDSReaderGTest, TestSynchronise30) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise30());
}

TEST(MDSReaderGTest, TestSynchronise31) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise31());
}

TEST(MDSReaderGTest, TestSynchronise32) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise32());
}

TEST(MDSReaderGTest, TestSynchronise33) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise33());
}

TEST(MDSReaderGTest, TestSynchronise34) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise34());
}

TEST(MDSReaderGTest, TestSynchronise35) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise35());
}

TEST(MDSReaderGTest, TestSynchronise36) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise36());
}

TEST(MDSReaderGTest, TestSynchronise37) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise37());
}

TEST(MDSReaderGTest, TestSynchronise38) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise38());
}

TEST(MDSReaderGTest, TestSynchronise39) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise39());
}

TEST(MDSReaderGTest, TestSynchronise40) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise40());
}

TEST(MDSReaderGTest, TestSynchronise41) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise41());
}

TEST(MDSReaderGTest, TestSynchronise42) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise42());
}

TEST(MDSReaderGTest, TestSynchronise43) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise43());
}

TEST(MDSReaderGTest, TestSynchronise44) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise44());
}

TEST(MDSReaderGTest, TestSynchronise45) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise45());
}

TEST(MDSReaderGTest, TestSynchronise46) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise46());
}

TEST(MDSReaderGTest, TestSynchronise47) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise47());
}

TEST(MDSReaderGTest, TestSynchronise48) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise48());
}

TEST(MDSReaderGTest, TestSynchronise49) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise49());
}

TEST(MDSReaderGTest, TestSynchronise50) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise50());
}

TEST(MDSReaderGTest, TestSynchronise51) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise51());
}

TEST(MDSReaderGTest, TestSynchronise52) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise52());
}

TEST(MDSReaderGTest, TestSynchronise53) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise53());
}

TEST(MDSReaderGTest, TestSynchronise54) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise54());
}

TEST(MDSReaderGTest, TestSynchronise55) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise55());
}

TEST(MDSReaderGTest, TestSynchronise56) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise56());
}

TEST(MDSReaderGTest, TestSynchronise57) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise57());
}

TEST(MDSReaderGTest, TestSynchronise58) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise58());
}

TEST(MDSReaderGTest, TestSynchronise59) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise59());
}

TEST(MDSReaderGTest, TestSynchronise60) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise60());
}

TEST(MDSReaderGTest, TestSynchronise61) {
    MDSReaderTest test("single_elem", 1, 1000, 2.0);
    ASSERT_TRUE(test.TestSynchronise61());
}

TEST(MDSReaderGTest, TestSynchronise62) {
    MDSReaderTest test("single_elem", 1, 1000, 2.0);
    ASSERT_TRUE(test.TestSynchronise62());
}

TEST(MDSReaderGTest, TestSynchronise63) {
    MDSReaderTest test("single_elem", 1, 1000, 2.0);
    ASSERT_TRUE(test.TestSynchronise63());
}

TEST(MDSReaderGTest, TestSynchronise64) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise64());
}

TEST(MDSReaderGTest, TestSynchronise65) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise65());
}

TEST(MDSReaderGTest, TestSynchronise66) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise66());
}

TEST(MDSReaderGTest, TestSynchronise67) {
    MDSReaderTest test;
    ASSERT_TRUE(test.TestSynchronise67());
}

TEST(MDSReaderGTest, TestSynchronise68) {
    uint32 nOfSegments = 1000;
    MDSReaderTest test("test_tree", 128, nOfSegments, 0.000064000); //treeName failes to create.. who knows why...
    ASSERT_TRUE(test.TestSynchronise68(nOfSegments));
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

