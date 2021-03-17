/**
 * @file MemoryGateGTest.cpp
 * @brief Source file for class MemoryGateGTest
 * @date 24/07/2018
 * @author Giuseppe Ferro
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
 * the class MemoryGateGTest (public, protected, and private). Be aware that some 
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
#include "TcnTimeProviderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(TcnTimeProviderGTest,TestConstructor) {
    TcnTimeProviderTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(TcnTimeProviderGTest,TestCounter) {
    TcnTimeProviderTest test;
    ASSERT_TRUE(test.TestCounter());
}

TEST(TcnTimeProviderGTest,TestPeriod) {
    TcnTimeProviderTest test;
    ASSERT_TRUE(test.TestPeriod());
}

TEST(TcnTimeProviderGTest,TestFrequency) {
    TcnTimeProviderTest test;
    ASSERT_TRUE(test.TestFrequency());
}

TEST(TcnTimeProviderGTest,TestSleep) {
    TcnTimeProviderTest test;
    ASSERT_TRUE(test.TestSleep());
}

TEST(TcnTimeProviderGTest, TestInitialise_NoPollLegacyMode) {
    TcnTimeProviderTest test(true);
    ASSERT_TRUE(test.TestInitialise_NoPollLegacyMode());
}

TEST(TcnTimeProviderGTest, TestInitialise_PollLegacyMode) {
    TcnTimeProviderTest test(true);
    ASSERT_TRUE(test.TestInitialise_PollLegacyMode());
}

TEST(TcnTimeProviderGTest, TestInitialise_WaitUntilMode) {
    TcnTimeProviderTest test(true);
    ASSERT_TRUE(test.TestInitialise_WaitUntilMode());
}

TEST(TcnTimeProviderGTest, TestInitialise_WaitUntilHRMode) {
    TcnTimeProviderTest test(true);
    ASSERT_TRUE(test.TestInitialise_WaitUntilHRMode());
}

TEST(TcnTimeProviderGTest, TestInitialise_SleepMode) {
    TcnTimeProviderTest test(true);
    ASSERT_TRUE(test.TestInitialise_SleepMode());
}

TEST(TcnTimeProviderGTest, TestInitialise_SleepHRMode) {
    TcnTimeProviderTest test(true);
    ASSERT_TRUE(test.TestInitialise_SleepHRMode());
}

TEST(TcnTimeProviderGTest, TestInitialise_InvalidMode_Fail) {
    TcnTimeProviderTest test(true);
    ASSERT_TRUE(test.TestInitialise_InvalidMode_Fail());
}
