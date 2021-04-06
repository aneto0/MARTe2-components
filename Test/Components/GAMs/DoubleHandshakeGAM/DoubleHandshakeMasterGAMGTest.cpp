/**
 * @file DoubleHandshakeMasterGAMGTest.cpp
 * @brief Source file for class DoubleHandshakeMasterGAMGTest
 * @date 07/08/2019
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
 * the class DoubleHandshakeMasterGAMGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "DoubleHandshakeMasterGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(DoubleHandshakeMasterGAMGTest,TestConstructor) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(DoubleHandshakeMasterGAMGTest,TestSetup) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(DoubleHandshakeMasterGAMGTest,TestSetup_UselessSignal) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestSetup_UselessSignal());
}

////Commented out, it was only returning without executing
//TEST(DoubleHandshakeMasterGAMGTest,TestSetup_NumberOfElements) {
//    DoubleHandshakeMasterGAMTest test;
//    ASSERT_TRUE(test.TestSetup_NumberOfElements());
//}

TEST(DoubleHandshakeMasterGAMGTest,TestSetup_FalseNInCommandsDiffThanNInAcks) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseNInCommandsDiffThanNInAcks());
}

TEST(DoubleHandshakeMasterGAMGTest,TestSetup_FalseNInCommandsDiffThanClearTrigs) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseNInCommandsDiffThanClearTrigs());
}

TEST(DoubleHandshakeMasterGAMGTest,TestSetup_FalseClearTrigsWrongType) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseClearTrigsWrongType());
}

TEST(DoubleHandshakeMasterGAMGTest,TestSetup_FalseInternalStateWrongType) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseInternalStateWrongType());
}

TEST(DoubleHandshakeMasterGAMGTest,TestSetup_FalseNInCommandsDiffThanNOutCommands) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseNInCommandsDiffThanNOutCommands());
}

TEST(DoubleHandshakeMasterGAMGTest,TestSetup_FalseNInCommandsDiffThanNOutStatus) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseNInCommandsDiffThanNOutStatus());
}

TEST(DoubleHandshakeMasterGAMGTest,TestSetup_FalseSizeInCommandDiffThanSizeInAck) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseSizeInCommandDiffThanSizeInAck());
}

TEST(DoubleHandshakeMasterGAMGTest,TestSetup_FalseSizeInCommandDiffThanSizeOutCommand) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseSizeInCommandDiffThanSizeOutCommand());
}

TEST(DoubleHandshakeMasterGAMGTest,TestExecute) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(DoubleHandshakeMasterGAMGTest,TestInitialise) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(DoubleHandshakeMasterGAMGTest,TestExecute_ErrorFromReady) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestExecute_ErrorFromReady());
}

TEST(DoubleHandshakeMasterGAMGTest,TestExecute_ErrorFromSending) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestExecute_ErrorFromSending());
}

TEST(DoubleHandshakeMasterGAMGTest,TestExecute_ErrorFromClear) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestExecute_ErrorFromClear());
}

TEST(DoubleHandshakeMasterGAMGTest,TestExecute_ErrorFromDone) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestExecute_ErrorFromDone());
}

TEST(DoubleHandshakeMasterGAMGTest,TestExecute_ErrorFromSendingTimeout) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestExecute_ErrorFromSendingTimeout());
}

TEST(DoubleHandshakeMasterGAMGTest,TestExecute_ErrorFromClearTimeout) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestExecute_ErrorFromClearTimeout());
}

TEST(DoubleHandshakeMasterGAMGTest,TestExecute_ErrorFromDoneTimeout) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestExecute_ErrorFromDoneTimeout());
}

TEST(DoubleHandshakeMasterGAMGTest,TestExecute_RecoverFromError) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestExecute_RecoverFromError());
}

TEST(DoubleHandshakeMasterGAMGTest,TestExecute_DoubleCycle) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestExecute_DoubleCycle());
}

/*
TEST(DoubleHandshakeMasterGAMGTest,TestExecute_Interactive) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestExecute_Interactive());
}
*/

/*
TEST(DoubleHandshakeMasterGAMGTest,TestExecute_InteractiveManual) {
    DoubleHandshakeMasterGAMTest test;
    ASSERT_TRUE(test.TestExecute_InteractiveManual());
}
*/
