/**
 * @file NI9157CircularFifoReaderGTest.cpp
 * @brief Source file for class NI9157CircularFifoReaderGTest
 * @date 11/02/2021
 * @author Giuseppe Ferro
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
 * the class NI9157CircularFifoReaderGTest (public, protected, and private). Be aware that some 
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
#include "NI9157CircularFifoReaderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI9157CircularFifoReaderGTest,TestConstructor) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultRunNi) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultRunNi());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultNumberOfPacketsInFIFO) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultNumberOfPacketsInFIFO());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultAcquiredFromPacket) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultAcquiredFromPacket());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultCheckCounterAfterNPackets) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultCheckCounterAfterNPackets());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultCheckPacketCounter) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultCheckPacketCounter());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultCounterStep) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultCounterStep());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultNFrameForSync) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultNFrameForSync());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_NFrameForSyncEqualTo1) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_NFrameForSyncEqualTo1());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_CounterStepZero) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_CounterStepZero());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_CheckAfterNotDivideCounterStep) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_CheckAfterNotDivideCounterStep());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_CounterStepNotDivideAcquireFromCounterDifference) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_CounterStepNotDivideAcquireFromCounterDifference());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_NoFifoName) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoFifoName());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_NoNI9157Device) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoNI9157Device());
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_Invalidi9157Device) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestInitialise_False_Invalidi9157Device());
}

TEST(NI9157CircularFifoReaderGTest,TestSetConfiguredDatabase) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(NI9157CircularFifoReaderGTest,TestSetConfiguredDatabase_False_MoreThanOneChannel) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThanOneChannel());
}

TEST(NI9157CircularFifoReaderGTest,TestSetConfiguredDatabase_False_NiDevOperatorNotFound) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NiDevOperatorNotFound());
}

TEST(NI9157CircularFifoReaderGTest,TestSetConfiguredDatabase_False_InvalidFifoName) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidFifoName());
}

TEST(NI9157CircularFifoReaderGTest,TestPrepareNextState) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestDriverRead());
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_AllSignals) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestDriverRead_AllSignals());
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_AcquiredFromCounter) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestDriverRead_AcquiredFromCounter());
}

TEST(NI9157CircularFifoReaderGTest,TestDriverReadCompleteCycle) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestDriverReadCompleteCycle());
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_Resync) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestDriverRead_Resync());
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_NoCheckCounter) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestDriverRead_NoCheckCounter());
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_CheckAfterNPackets) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestDriverRead_CheckAfterNPackets());
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_InternalInterleaved) {
    NI9157CircularFifoReaderTest test;
    ASSERT_TRUE(test.TestDriverRead_InternalInterleaved());
}
