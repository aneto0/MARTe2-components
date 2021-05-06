/**
 * @file EPICSPVAInputGTest.cpp
 * @brief Source file for class EPICSPVAInputGTest
 * @date 23/10/2018
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
 * the class EPICSPVAInputGTest (public, protected, and private). Be aware that some
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
#include "EPICSPVAInputTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(EPICSPVAInputGTest,TestConstructor) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(EPICSPVAInputGTest,TestGetNumberOfMemoryBuffers) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(EPICSPVAInputGTest,TestGetSignalMemoryBuffer) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(EPICSPVAInputGTest,TestGetBrokerName) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(EPICSPVAInputGTest,TestGetBrokerName_OutputSignals) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestGetBrokerName_OutputSignals());
}

TEST(EPICSPVAInputGTest,TestInitialise) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(EPICSPVAInputGTest,TestInitialise_Defaults) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestInitialise_Defaults());
}

TEST(EPICSPVAInputGTest,TestInitialise_False_Signals) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestInitialise_False_Signals());
}

TEST(EPICSPVAInputGTest,TestGetCPUMask) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestGetCPUMask());
}

TEST(EPICSPVAInputGTest,TestGetStackSize) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestGetStackSize());
}

TEST(EPICSPVAInputGTest,TestSetConfiguredDatabase) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(EPICSPVAInputGTest,TestSetConfiguredDatabase_False_NoSignals) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoSignals());
}

TEST(EPICSPVAInputGTest,TestSetConfiguredDatabase_False_Samples) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_Samples());
}

TEST(EPICSPVAInputGTest,TestPrepareNextState) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(EPICSPVAInputGTest,TestSynchronise) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(EPICSPVAInputGTest,TestGetBrokerNameSynchronised_too_many_channels) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestGetBrokerNameSynchronised_too_many_channels());
}

TEST(EPICSPVAInputGTest,TestExecute) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(EPICSPVAInputGTest,TestExecute_StructuredType) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestExecute_StructuredType());
}

TEST(EPICSPVAInputGTest,TestExecute_StructuredType_Arrays) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestExecute_StructuredType_Arrays());
}

TEST(EPICSPVAInputGTest,TestExecute_Arrays) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestExecute_Arrays());
}

TEST(EPICSPVAInputGTest,TestExecute_False_CharString) {
    EPICSPVAInputTest test;
    ASSERT_TRUE(test.TestExecute_False_CharString());
}
