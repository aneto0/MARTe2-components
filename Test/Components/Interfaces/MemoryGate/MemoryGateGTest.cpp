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
#include "MemoryGateTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(MemoryGateGTest,TestConstructor) {
    MemoryGateTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(MemoryGateGTest,TestInitialise) {
    MemoryGateTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(MemoryGateGTest,TestInitialise_FalseTooManyBuffers) {
    MemoryGateTest test;
    ASSERT_TRUE(test.TestInitialise_FalseTooManyBuffers());
}

TEST(MemoryGateGTest,TestInitialise_DefaultTimeout) {
    MemoryGateTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultTimeout());
}

TEST(MemoryGateGTest,TestInitialise_DefaultNBuffers) {
    MemoryGateTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultNBuffers());
}

TEST(MemoryGateGTest,TestInitialise_NoMemSize) {
    MemoryGateTest test;
    ASSERT_TRUE(test.TestInitialise_NoMemSize());
}

TEST(MemoryGateGTest,TestSetMemorySize_Initialised) {
    MemoryGateTest test;
    ASSERT_TRUE(test.TestSetMemorySize_Initialised());
}

TEST(MemoryGateGTest,TestSetMemorySize_NotInitialised) {
    MemoryGateTest test;
    ASSERT_TRUE(test.TestSetMemorySize_NotInitialised());
}

TEST(MemoryGateGTest,TestMemoryRead) {
    MemoryGateTest test;
    ASSERT_TRUE(test.TestMemoryRead());
}

TEST(MemoryGateGTest,TestMemoryWrite) {
    MemoryGateTest test;
    ASSERT_TRUE(test.TestMemoryWrite());
}

TEST(MemoryGateGTest,TestMemoryWrite_Overflow) {
    MemoryGateTest test;
    ASSERT_TRUE(test.TestMemoryWrite_Overflow());
}

