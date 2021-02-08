/**
 * @file EPICSCAOutputGTest.cpp
 * @brief Source file for class EPICSCAOutputGTest
 * @date 04/02/2021
 * @author Andre Neto
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class EPICSCAOutputGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
#include "EPICSCAOutputTest.h"

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(EPICSCAOutputGTest,TestConstructor) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(EPICSCAOutputGTest,TestAllocateMemory) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(EPICSCAOutputGTest,TestGetNumberOfMemoryBuffers) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(EPICSCAOutputGTest,TestGetSignalMemoryBuffer) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(EPICSCAOutputGTest,TestGetBrokerName) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(EPICSCAOutputGTest,TestGetBrokerName_InputSignals) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestGetBrokerName_InputSignals());
}

TEST(EPICSCAOutputGTest,TestGetInputBrokers) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(EPICSCAOutputGTest,TestGetOutputBrokers) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(EPICSCAOutputGTest,TestInitialise) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(EPICSCAOutputGTest,TestInitialise_Defaults) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestInitialise_Defaults());
}

TEST(EPICSCAOutputGTest,TestInitialise_False_Signals) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestInitialise_False_Signals());
}

TEST(EPICSCAOutputGTest,TestInitialise_False_NumberOfBuffers) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestInitialise_False_NumberOfBuffers());
}

// TEST(EPICSCAOutputGTest,TestAsyncCaPut) {
//     EPICSCAOutputTest test;
//     ASSERT_TRUE(test.TestAsyncCaPut());
// }

TEST(EPICSCAOutputGTest,TestGetCPUMask) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestGetCPUMask());
}

TEST(EPICSCAOutputGTest,TestGetStackSize) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestGetStackSize());
}

TEST(EPICSCAOutputGTest,TestGetNumberOfBuffers) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestGetNumberOfBuffers());
}

TEST(EPICSCAOutputGTest,TestSetConfiguredDatabase) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(EPICSCAOutputGTest,TestSetConfiguredDatabase_False_NoSignals) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoSignals());
}

TEST(EPICSCAOutputGTest,TestSetConfiguredDatabase_False_MoreThanOneGAM) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThanOneGAM());
}

TEST(EPICSCAOutputGTest,TestSetConfiguredDatabase_False_PVName) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_PVName());
}

TEST(EPICSCAOutputGTest,TestSetConfiguredDatabase_False_Samples) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_Samples());
}

TEST(EPICSCAOutputGTest,TestSetConfiguredDatabase_False_UnsupportedType) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_UnsupportedType());
}

TEST(EPICSCAOutputGTest,TestSetConfiguredDatabase_False_WrongStringSize) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_WrongStringSize());
}

TEST(EPICSCAOutputGTest,TestExecute) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(EPICSCAOutputGTest,TestExecute_Arrays) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestExecute_Arrays());
}

TEST(EPICSCAOutputGTest,TestIsIgnoringBufferOverrun) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestIsIgnoringBufferOverrun());
}

TEST(EPICSCAOutputGTest,TestAsyncCaPut) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestAsyncCaPut());
}