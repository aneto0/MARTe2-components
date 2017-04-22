/**
 * @file EPICSCAOutputGTest.cpp
 * @brief Source file for class EPICSCAOutputGTest
 * @date 21/04/2017
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
 * the class EPICSCAOutputGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

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

TEST(EPICSCAOutputGTest,TestGetBrokerName_OutputSignals) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestGetBrokerName_OutputSignals());
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

TEST(EPICSCAOutputGTest,TestSetConfiguredDatabase) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(EPICSCAOutputGTest,TestSetConfiguredDatabase_False_NoSignals) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoSignals());
}

TEST(EPICSCAOutputGTest,TestExecute) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(EPICSCAOutputGTest,TestExecute_Arrays) {
    EPICSCAOutputTest test;
    ASSERT_TRUE(test.TestExecute_Arrays());
}
	
