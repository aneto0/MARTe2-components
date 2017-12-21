/**
 * @file WaveformSinGAMGTest.cpp
 * @brief Source file for class WaveformSinGAMGTest
 * @date 22/05/2017
 * @author Llorenc Capella
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
 * the class WaveformSinGAMGTest (public, protected, and private). Be aware that some
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
#include "WaveformSin.h"
#include "../WaveformGAM/WaveformSinGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(WaveformSinGAMTest, TestInitialise_MissingAmplitude) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingAmplitude());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingFrequency) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingFrequency());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingPhase) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingPhase());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingOffset) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingOffset());
}

TEST(WaveformSinGAMTest, TestInitialise_Frequency0) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Frequency0());
}

TEST(WaveformSinGAMTest, TestInitialise_Amplitude0) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Amplitude0());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingInputSignal) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingInputSignal());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingOutputSignal) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingOutputSignal());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingInputNumberOfElements) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingInputNumberOfElements());
}

TEST(WaveformSinGAMTest, TestInitialise_WrongInputNumberOfElements) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_WrongInputNumberOfElements());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingOutputNumberOfElements) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingOutputNumberOfElements());
}

TEST(WaveformSinGAMTest, TestInitialise_WrongOutputNumberOfElements) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_WrongOutputNumberOfElements());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingSecondOutputNumberOfElements) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingSecondOutputNumberOfElements());
}

TEST(WaveformSinGAMTest, TestInitialise_WrongSecondOutputNumberOfElements) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_WrongSecondOutputNumberOfElements());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingInputNumberOfSamples) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingInputNumberOfSamples());
}

TEST(WaveformSinGAMTest, TestInitialise_WrongInputNumberOfSamples) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_WrongInputNumberOfSamples());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingOutputNumberOfSamples) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingOutputNumberOfSamples());
}

TEST(WaveformSinGAMTest, TestInitialise_WrongOutputNumberOfSamples) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_WrongOutputNumberOfSamples());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingSeondOutputNumberOfSamples) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingSeondOutputNumberOfSamples());
}

TEST(WaveformSinGAMTest, TestInitialise_WrongSecondOutputNumberOfSamples) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_WrongSecondOutputNumberOfSamples());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingInputType) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingInputType());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingOutputType) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingOutputType());
}

TEST(WaveformSinGAMTest, TestInitialise_MissingSecondOutputType) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingSecondOutputType());
}

TEST(WaveformSinGAMTest, TestUInt8Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestUInt8Execute());
}


TEST(WaveformSinGAMTest, TestInt8Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInt8Execute());
}

TEST(WaveformSinGAMTest, TestInt16Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInt16Execute());
}

TEST(WaveformSinGAMTest, TestUInt16Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestUInt16Execute());
}

TEST(WaveformSinGAMTest, TestUInt32Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestUInt32Execute());
}

TEST(WaveformSinGAMTest, TestInt32Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInt32Execute());
}


TEST(WaveformSinGAMTest, TestInt64Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInt64Execute());
}


TEST(WaveformSinGAMTest, TestUInt64Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestUInt64Execute());
}


TEST(WaveformSinGAMTest, TestFloat32Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestFloat32Execute());
}

TEST(WaveformSinGAMTest, TestFloat64Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestFloat64Execute());
}

TEST(WaveformSinGAMTest, TestSetup_StopTriggerGreaterThanStartTrigger) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestSetup_StopTriggerGreaterThanStartTrigger());
}

TEST(WaveformSinGAMTest, TestSetup_StartTriggerGreaterThanStopTriggerPlus2) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestSetup_StartTriggerGreaterThanStopTriggerPlus2());
}

TEST(WaveformSinGAMTest, TestSetup_StartTriggerTimeArrayInconsistent) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestSetup_StartTriggerTimeArrayInconsistent());
}

TEST(WaveformSinGAMTest, TestUInt8ExecuteTrigger) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestUInt8ExecuteTrigger());
}

TEST(WaveformSinGAMTest, TestInt8ExecuteTrigger) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInt8ExecuteTrigger());
}

TEST(WaveformSinGAMTest, TestUInt16ExecuteTrigger) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestUInt16ExecuteTrigger());
}

TEST(WaveformSinGAMTest, TestInt16ExecuteTrigger) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInt16ExecuteTrigger());
}

TEST(WaveformSinGAMTest, TestUInt32ExecuteTrigger) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestUInt32ExecuteTrigger());
}

TEST(WaveformSinGAMTest, TestInt32ExecuteTrigger) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInt32ExecuteTrigger());
}

TEST(WaveformSinGAMTest, TestUInt64ExecuteTrigger) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestUInt64ExecuteTrigger());
}

TEST(WaveformSinGAMTest, TestInt64ExecuteTrigger) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInt64ExecuteTrigger());
}

TEST(WaveformSinGAMTest, TestFloat32ExecuteTrigger) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestFloat32ExecuteTrigger());
}

TEST(WaveformSinGAMTest, TestFloat64ExecuteTrigger) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestFloat64ExecuteTrigger());
}

TEST(WaveformSinGAMTest, TestExecuteTriggerDiffArrayLength) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestExecuteTriggerDiffArrayLength());
}

TEST(WaveformSinGAMTest, TestExecuteTigger1Element) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestExecuteTigger1Element());
}

TEST(WaveformSinGAMTest, TestExecuteWrongInput) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestExecuteWrongInput());
}

TEST(WaveformSinGAMTest, TestExecuteWrongInput_2) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestExecuteWrongInput_2());
}





/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
