/**
 * @file EPICSPVAChannelWrapperGTest.cpp
 * @brief Source file for class EPICSPVAChannelWrapperGTest
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
 * the class EPICSPVAChannelWrapperGTest (public, protected, and private). Be aware that some
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
#include "EPICSPVAChannelWrapperTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(EPICSPVAChannelWrapperGTest,TestConstructor) {
    EPICSPVAChannelWrapperTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(EPICSPVAChannelWrapperGTest,TestSetup) {
    EPICSPVAChannelWrapperTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(EPICSPVAChannelWrapperGTest,TestSetup_False_Dimensions) {
    EPICSPVAChannelWrapperTest test;
    ASSERT_TRUE(test.TestSetup_False_Dimensions());
}

TEST(EPICSPVAChannelWrapperGTest,TestGetChannelName) {
    EPICSPVAChannelWrapperTest test;
    ASSERT_TRUE(test.TestGetChannelName());
}

TEST(EPICSPVAChannelWrapperGTest,TestGetChannelUnaliasedName) {
    EPICSPVAChannelWrapperTest test;
    ASSERT_TRUE(test.TestGetChannelUnaliasedName());
}

TEST(EPICSPVAChannelWrapperGTest,TestGetSignalMemory) {
    EPICSPVAChannelWrapperTest test;
    ASSERT_TRUE(test.TestGetSignalMemory());
}

TEST(EPICSPVAChannelWrapperGTest,TestMonitor) {
    EPICSPVAChannelWrapperTest test;
    ASSERT_TRUE(test.TestMonitor());
}

TEST(EPICSPVAChannelWrapperGTest,TestPut) {
    EPICSPVAChannelWrapperTest test;
    ASSERT_TRUE(test.TestPut());
}
