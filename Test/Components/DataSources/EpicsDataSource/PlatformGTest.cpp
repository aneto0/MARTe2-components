/**
 * @file PlatformGTest.cpp
 * @brief Source file for class PlatformGTest
 * @date 31/01/2017
 * @author Ivan Herrero
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
 * the class PlatformGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "PlatformTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static char NAME[] = "MARTe_PlatformTest_TestMakeShm";
static char FULL_NAME[] = "/dev/shm/MARTe_PlatformTest_TestMakeShm";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(PlatformGTest,TestMakeShm) {
	PlatformTest test;
    ASSERT_TRUE(test.TestMakeShm(NAME, FULL_NAME));
}

TEST(PlatformGTest,TestJoinShm) {
	PlatformTest test;
    ASSERT_TRUE(test.TestJoinShm(NAME, FULL_NAME));
}

TEST(PlatformGTest,TestDettachShm) {
	PlatformTest test;
    ASSERT_TRUE(test.TestDettachShm(NAME, FULL_NAME));
}

TEST(PlatformGTest,TestDestroyShm) {
	PlatformTest test;
    ASSERT_TRUE(test.TestDestroyShm(NAME, FULL_NAME));
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
