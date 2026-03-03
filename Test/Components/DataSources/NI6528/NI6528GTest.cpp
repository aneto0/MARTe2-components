/**
 * @file NI6528GTest.cpp
 * @brief Source file for class NI6528GTest
 * @date Mar 2, 2026 TODO Verify the value and format of the date
 * @author ferrog TODO Verify the name and format of the author
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
 * the class NI6528GTest (public, protected, and private). Be aware that some 
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

#include "NI6528Test.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(NI6528GTest,TestConstructor) {
    NI6528Test test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI6528GTest,TestInitialise) {
    NI6528Test test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(NI6528GTest,TestSetConfiguredDatabase) {
    NI6528Test test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(NI6528GTest,TestSynchronise) {
    NI6528Test test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(NI6528GTest,TestAllocateMemory) {
    NI6528Test test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(NI6528GTest,TestGetSignalMemoryBuffer) {
    NI6528Test test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(NI6528GTest,TestGetBrokerName) {
    NI6528Test test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(NI6528GTest,TestPrepareNextState) {
    NI6528Test test;
    ASSERT_TRUE(test.TestPrepareNextState());
}
