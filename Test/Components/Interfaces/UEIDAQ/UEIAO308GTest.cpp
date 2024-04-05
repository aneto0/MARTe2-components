/**
 * @file UEIAO308GTest.cpp
 * @brief Source file for class UEIAO308GTest
 * @date 22/03/2023
 * @author Xavier Ruche
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
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEIAO308Test.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEIAO308GTest,TestConstructor) {
    UEIAO308Test test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEIAO308GTest,TestFixedParameters) {
    UEIAO308Test test;
    ASSERT_TRUE(test.TestFixedParameters());
}

TEST(UEIAO308GTest,TestAcceptedSignalType) {
    UEIAO308Test test;
    ASSERT_TRUE(test.TestAcceptedSignalType());
}

TEST(UEIAO308GTest,TestInitialise) {
    UEIAO308Test test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(UEIAO308GTest,TestInitialise_class) {
    UEIAO308Test test;
    ASSERT_TRUE(test.TestInitialise_class());
}

TEST(UEIAO308GTest,TestCheckChannelAndDirection) {
    UEIAO308Test test;
    ASSERT_TRUE(test.TestCheckChannelAndDirection());
}

TEST(UEIAO308GTest,TestConfigureChannel) {
    UEIAO308Test test;
    ASSERT_TRUE(test.TestConfigureChannel());
}

TEST(UEIAO308GTest,TestConfigureDevice) {
    UEIAO308Test test;
    ASSERT_TRUE(test.TestConfigureDevice());
}

TEST(UEIAO308GTest,TestSetOutputSignal) {
    UEIAO308Test test;
    ASSERT_TRUE(test.TestSetOutputSignal());
}
