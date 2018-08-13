/**
 * @file TriggerOnChangeGAMGTest.cpp
 * @brief Source file for class TriggerOnChangeGAMGTest
 * @date 17/07/2018
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
 * the class TriggerOnChangeGAMGTest (public, protected, and private). Be aware that some 
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

#include "TriggerOnChangeGAM.h"
#include "TriggerOnChangeGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(TriggerOnChangeGAMGTest,TestConstructor) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(TriggerOnChangeGAMGTest,TestSetup) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(TriggerOnChangeGAMGTest,TestSetup_MoreCommands) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestSetup_MoreCommands());
}

TEST(TriggerOnChangeGAMGTest,TestSetup_NotAllCommands) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestSetup_NotAllCommands());
}

TEST(TriggerOnChangeGAMGTest,TestSetup_InputCommandArray) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestSetup_InputCommandArray());
}

TEST(TriggerOnChangeGAMGTest,TestSetup_False_NoCommands) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_NoCommands());
}

TEST(TriggerOnChangeGAMGTest,TestSetup_False_WrongStateNElements) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_WrongStateNElements());
}

TEST(TriggerOnChangeGAMGTest,TestSetup_False_WrongStateType) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_WrongStateType());
}

TEST(TriggerOnChangeGAMGTest,TestSetup_False_WrongIOSizeMatch) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_WrongIOSizeMatch());
}

TEST(TriggerOnChangeGAMGTest,TestExecute) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(TriggerOnChangeGAMGTest,TestExecute_MoreCommands) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestExecute_MoreCommands());
}

TEST(TriggerOnChangeGAMGTest,TestExecute_NotAllCommands) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestExecute_NotAllCommands());
}

TEST(TriggerOnChangeGAMGTest,TestExecute_InputCommandArray) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestExecute_InputCommandArray());
}

TEST(TriggerOnChangeGAMGTest,TestExecute_NotAllCommandsArray) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestExecute_NotAllCommandsArray());
}

TEST(TriggerOnChangeGAMGTest,TestExecute_WaitIndirectReply) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestExecute_WaitIndirectReply());
}

TEST(TriggerOnChangeGAMGTest,TestExecute_MoreCommandsWaitIndirectReply) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestExecute_MoreCommandsWaitIndirectReply());
}

TEST(TriggerOnChangeGAMGTest,TestExecute_NotAllCommandsWaitIndirectReply) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestExecute_NotAllCommandsWaitIndirectReply());
}

TEST(TriggerOnChangeGAMGTest,TestExecute_ErrorState) {
    TriggerOnChangeGAMTest test;
    ASSERT_TRUE(test.TestExecute_ErrorState());
}
