/**
 * @file MessageGAMGTest.cpp
 * @brief Source file for class MessageGAMGTest
 * @date 02/08/2019
 * @author Giuseppe Ferrò
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
 * the class MessageGAMGTest (public, protected, and private). Be aware that some 
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
#include "MessageGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(MessageGAMGTest,TestConstructor) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(MessageGAMGTest,TestSetup) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(MessageGAMGTest,TestSetupNumberOfElements) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestSetupNumberOfElements());
}

TEST(MessageGAMGTest,TestSetup_FalseInOutMismatch) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseInOutMismatch());
}

TEST(MessageGAMGTest,TestSetup_FalseNoCommands) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseNoCommands());
}

TEST(MessageGAMGTest,TestSetup_FalseStateBadType) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseStateBadType());
}

TEST(MessageGAMGTest,TestExecute) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(MessageGAMGTest,TestPrepareNextState) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(MessageGAMGTest,TestPurge) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestPurge());
}

TEST(MessageGAMGTest,TestExecute_MoreCommands) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestExecute_MoreCommands());
}

TEST(MessageGAMGTest,TestExecute_CommandsAndStates) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestExecute_CommandsAndStates());
}

TEST(MessageGAMGTest,TestExecute_Commands) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestExecute_Commands());
}

TEST(MessageGAMGTest,TestExecute_WithoutTriggerOnChange) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestExecute_WithoutTriggerOnChange());
}

TEST(MessageGAMGTest,TestGetNumberOfCommands) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestGetNumberOfCommands());
}

TEST(MessageGAMGTest,TestGetNumberOfEvents) {
    MessageGAMTest test;
    ASSERT_TRUE(test.TestGetNumberOfEvents());
}
