/**
 * @file RealTimeThreadAsyncBridgeGTest.cpp
 * @brief Source file for class RealTimeThreadAsyncBridgeGTest
 * @date 25/04/2018
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
 * the class RealTimeThreadAsyncBridgeGTest (public, protected, and private). Be aware that some 
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

#include "RealTimeThreadAsyncBridgeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeThreadAsyncBridgeGTest,TestConstructor) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestInitialise) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestInitialise_False_GreaterNumberOfBuffers) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestInitialise_False_GreaterNumberOfBuffers());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestSetConfiguredDatabase) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestSetConfiguredDatabase_False_InputSamples) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InputSamples());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestSetConfiguredDatabase_False_OutputSamples) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_OutputSamples());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestSynchronise) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestGetBrokerName) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestPrepareNextState) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestGetInputOffset) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestGetInputOffset());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestGetOutputOffset) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestGetOutputOffset());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestGetInputOffset_2Readers) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestGetInputOffset_2Readers());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestGetInputOffset_Ranges) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestGetInputOffset_Ranges());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestGetOutputOffset_Ranges) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestGetOutputOffset_Ranges());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestGetInputOffset_False_BufferBusy) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestGetInputOffset_False_BufferBusy());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestGetOutputOffset_False_BufferBusy) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestGetOutputOffset_False_BufferBusy());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestGetInputOffset_Blocking) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestGetInputOffset_Blocking());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestGetOutputOffset_Blocking) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestGetOutputOffset_Blocking());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestTerminateRead) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestTerminateRead());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestTerminateWrite) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestTerminateWrite());
}

TEST(RealTimeThreadAsyncBridgeGTest,TestTerminateWrite_ResetCounter) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestTerminateWrite_ResetCounter());
}


TEST(RealTimeThreadAsyncBridgeGTest,TestResetSignalValue) {
    RealTimeThreadAsyncBridgeTest test;
    ASSERT_TRUE(test.TestResetSignalValue());
}
