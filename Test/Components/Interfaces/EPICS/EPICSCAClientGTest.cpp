/**
 * @file EPICSCAClientGTest.cpp
 * @brief Source file for class EPICSCAClientGTest
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
 * the class EPICSCAClientGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EPICSCAClientTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(EPICSCAClientGTest,TestConstructor) {
    EPICSCAClientTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(EPICSCAClientGTest,TestExecute) {
    EPICSCAClientTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(EPICSCAClientGTest,TestGetCPUMask) {
    EPICSCAClientTest test;
    ASSERT_TRUE(test.TestGetCPUMask());
}

TEST(EPICSCAClientGTest,TestGetStackSize) {
    EPICSCAClientTest test;
    ASSERT_TRUE(test.TestGetStackSize());
}

TEST(EPICSCAClientGTest,TestInitialise_Defaults) {
    EPICSCAClientTest test;
    ASSERT_TRUE(test.TestInitialise_Defaults());
}
	
TEST(EPICSCAClientGTest,TestInitialise_NoDefaults) {
    EPICSCAClientTest test;
    ASSERT_TRUE(test.TestInitialise_NoDefaults());
}

TEST(EPICSCAClientGTest,TestStart) {
    EPICSCAClientTest test;
    ASSERT_TRUE(test.TestStart());
}

TEST(EPICSCAClientGTest,TestGetStatus) {
    EPICSCAClientTest test;
    ASSERT_TRUE(test.TestGetStatus());
}
