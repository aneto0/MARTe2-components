/**
 * @file DoubleHandshakeSlaveGAMGTest.cpp
 * @brief Source file for class DoubleHandshakeSlaveGAMGTest
 * @date 13/08/2019
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
 * the class DoubleHandshakeSlaveGAMGTest (public, protected, and private). Be aware that some 
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
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
#include "DoubleHandshakeSlaveGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(DoubleHandshakeSlaveGAMGTest,TestConstructor) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestSetup) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestSetup_UselessSignal) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestSetup_UselessSignal());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestSetup_NumberOfElements) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestSetup_NumberOfElements());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestSetup_FalseNCommansDiffThanNAcks) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseNCommansDiffThanNAcks());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestSetup_FalseNCommansDiffThanNClears) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseNCommansDiffThanNClears());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestSetup_FalseNCommansDiffThanNStatuses) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseNCommansDiffThanNStatuses());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestSetup_FalseClearTrigsWrongType) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseClearTrigsWrongType());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestSetup_FalseInternalStateWrongType) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseInternalStateWrongType());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestSetup_FalseCommandAndAckMismatch) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestSetup_FalseCommandAndAckMismatch());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestExecute) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestInitialise) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestExecute_ErrorFromSending) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestExecute_ErrorFromSending());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestExecute_ErrorFromDone) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestExecute_ErrorFromDone());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestExecute_ErrorFromSendingTimeout) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestExecute_ErrorFromSendingTimeout());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestExecute_ErrorFromDoneTimeout) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestExecute_ErrorFromDoneTimeout());
}

TEST(DoubleHandshakeSlaveGAMGTest,TestExecute_RecoverFromError) {
    DoubleHandshakeSlaveGAMTest test;
    ASSERT_TRUE(test.TestExecute_RecoverFromError());
}


/*
 TEST(DoubleHandshakeSlaveGAMGTest,TestExecute_Interactive) {
 DoubleHandshakeSlaveGAMTest test;
 ASSERT_TRUE(test.TestExecute_Interactive());
 }
 */

