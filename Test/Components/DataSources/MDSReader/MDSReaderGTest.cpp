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

/*
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
*/

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





/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

