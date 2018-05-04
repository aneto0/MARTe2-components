/**
 * @file  Interleaved2FlatGAMGTest.cpp
 * @brief Source file for class  Interleaved2FlatGAMGTest
 * @date 24/04/2018
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
 * the class  Interleaved2FlatGAMGTest (public, protected, and private). Be aware that some
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
#include "Interleaved2FlatGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


TEST(Interleaved2FlatGAMGTest,TestConstructor) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(Interleaved2FlatGAMGTest,TestSetup) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(Interleaved2FlatGAMGTest,TestSetup_MultiPacketNoSamples) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestSetup_MultiPacketNoSamples());
}

TEST(Interleaved2FlatGAMGTest,TestSetup_False_SignalOverlap_In) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_SignalOverlap_In());
}

TEST(Interleaved2FlatGAMGTest,TestSetup_False_DoublePacketDef_In) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_DoublePacketDef_In());
}

TEST(Interleaved2FlatGAMGTest,TestSetup_False_SignalOverlap_Out) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_SignalOverlap_Out());
}

TEST(Interleaved2FlatGAMGTest,TestSetup_False_DoublePacketDef_Out) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_DoublePacketDef_Out());
}

TEST(Interleaved2FlatGAMGTest,TestSetup_False_DifferentIOSizes) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_DifferentIOSizes());
}

TEST(Interleaved2FlatGAMGTest,TestSetup_False_DifferentPacketSize_In) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_DifferentPacketSize_In());
}

TEST(Interleaved2FlatGAMGTest,TestSetup_False_DifferentPacketSize_Out) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_DifferentPacketSize_Out());
}

TEST(Interleaved2FlatGAMGTest,TestExecute) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(Interleaved2FlatGAMGTest,TestExecute_Interleaved2FlatOnly) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestExecute_Interleaved2FlatOnly());
}

TEST(Interleaved2FlatGAMGTest,TestExecute_Flat2InterleavedOnly) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestExecute_Flat2InterleavedOnly());
}

TEST(Interleaved2FlatGAMGTest,TestExecute_MultiPacketNoSamples) {
    Interleaved2FlatGAMTest test;
    ASSERT_TRUE(test.TestExecute_MultiPacketNoSamples());
}
