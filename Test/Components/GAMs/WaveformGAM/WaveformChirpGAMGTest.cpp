/**
 * @file WaveformChirpGAMGTest.cpp
 * @brief Source file for class WaveformChirpGAMGTest
 * @date 02/06/2017
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
 * the class WaveformChirpGAMGTest (public, protected, and private). Be aware that some 
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

#include "WaveformChirpGAMTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;


TEST(WaveformChirpGAMTest, TestInitialiseMissingAmplitude) {
    WaveformChirpGAMTest test;
    ASSERT_TRUE(test.TestInitialiseMissingAmplitude());
}

TEST(WaveformChirpGAMTest, TestInitialise0Amplitude) {
    WaveformChirpGAMTest test;
    ASSERT_TRUE(test.TestInitialise0Amplitude());
}

TEST(WaveformChirpGAMTest, TestInitialiseMissingPhase) {
    WaveformChirpGAMTest test;
    ASSERT_TRUE(test.TestInitialiseMissingPhase());
}

TEST(WaveformChirpGAMTest, TestInitialiseMissingOffset) {
    WaveformChirpGAMTest test;
    ASSERT_TRUE(test.TestInitialiseMissingOffset());
}

TEST(WaveformChirpGAMTest, TestInitialiseMissingFreq1) {
    WaveformChirpGAMTest test;
    ASSERT_TRUE(test.TestInitialiseMissingFreq1());
}

TEST(WaveformChirpGAMTest, TestInitialiseMissingFreq2) {
    WaveformChirpGAMTest test;
    ASSERT_TRUE(test.TestInitialiseMissingFreq2());
}

TEST(WaveformChirpGAMTest, TestInitialiseMissingChirpDuration) {
    WaveformChirpGAMTest test;
    ASSERT_TRUE(test.TestInitialiseMissingChirpDuration());
}

TEST(WaveformChirpGAMTest, TestInitialise0ChirpDuration) {
    WaveformChirpGAMTest test;
    ASSERT_TRUE(test.TestInitialise0ChirpDuration());
}

TEST(WaveformChirpGAMTest, TestExecuteNyquistViolation) {
    WaveformChirpGAMTest test;
    ASSERT_TRUE(test.TestExecuteNyquistViolation());
}

TEST(WaveformChirpGAMTest, TestExecuteUInt8) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint8";
    ASSERT_TRUE(test.TestExecute<uint8>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteInt8) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int8";
    ASSERT_TRUE(test.TestExecute<int8>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteUInt16) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint16";
    ASSERT_TRUE(test.TestExecute<uint16>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteInt16) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int16";
    ASSERT_TRUE(test.TestExecute<int16>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteUIn32) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint32";
    ASSERT_TRUE(test.TestExecute<uint32>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteInt32) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int32";
    ASSERT_TRUE(test.TestExecute<int32>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteUInt64) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint64";
    ASSERT_TRUE(test.TestExecute<uint64>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteInt64) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int64";
    ASSERT_TRUE(test.TestExecute<int64>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteFloat32) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "float32";
    ASSERT_TRUE(test.TestExecute<float32>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteFloat64) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "float64";
    ASSERT_TRUE(test.TestExecute<float64>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteTriggerUInt8) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint8";
    ASSERT_TRUE(test.TestExecuteTrigger<uint8>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteTriggerInt8) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int8";
    ASSERT_TRUE(test.TestExecuteTrigger<int8>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteTriggerUInt16) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint16";
    ASSERT_TRUE(test.TestExecuteTrigger<uint16>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteTriggerInt16) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int16";
    ASSERT_TRUE(test.TestExecuteTrigger<int16>(auxStr));
}


TEST(WaveformChirpGAMTest, TestExecuteTriggerUInt32) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint32";
    ASSERT_TRUE(test.TestExecuteTrigger<uint32>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteTriggerUnt32) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int32";
    ASSERT_TRUE(test.TestExecuteTrigger<int32>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteTriggerUInt64) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint64";
    ASSERT_TRUE(test.TestExecuteTrigger<uint64>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteTriggerInt64) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int64";
    ASSERT_TRUE(test.TestExecuteTrigger<int64>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteTriggerFloat32) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "float32";
    ASSERT_TRUE(test.TestExecuteTrigger<float32>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecuteTriggerFloat64) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "float64";
    ASSERT_TRUE(test.TestExecuteTrigger<float64>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute2SignalsUInt8) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint8";
    ASSERT_TRUE(test.TestExecute2Signals<uint8>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute2SignalsInt8) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int8";
    ASSERT_TRUE(test.TestExecute2Signals<int8>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute2SignalsUInt16) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint16";
    ASSERT_TRUE(test.TestExecute2Signals<uint16>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute2SignalsInt16) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int16";
    ASSERT_TRUE(test.TestExecute2Signals<int16>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute2SignalsUInt32) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint32";
    ASSERT_TRUE(test.TestExecute2Signals<uint32>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute2SignalsInt32) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int32";
    ASSERT_TRUE(test.TestExecute2Signals<int32>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute2SignalsUInt64) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint64";
    ASSERT_TRUE(test.TestExecute2Signals<uint64>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute2SignalsInt64) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int64";
    ASSERT_TRUE(test.TestExecute2Signals<int64>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute2SignalsFloat32) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "float32";
    ASSERT_TRUE(test.TestExecute2Signals<float32>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute2SignalsFloat64) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "float64";
    ASSERT_TRUE(test.TestExecute2Signals<float64>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute1ElementPerCycleUInt8) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint8";
    ASSERT_TRUE(test.TestExecute1ElementPerCycle<uint8>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute1ElementPerCycleInt8) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int8";
    ASSERT_TRUE(test.TestExecute1ElementPerCycle<int8>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute1ElementPerCycleUInt16) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint16";
    ASSERT_TRUE(test.TestExecute1ElementPerCycle<uint16>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute1ElementPerCycleInt16) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int16";
    ASSERT_TRUE(test.TestExecute1ElementPerCycle<int16>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute1ElementPerCycleUInt32) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint32";
    ASSERT_TRUE(test.TestExecute1ElementPerCycle<uint32>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute1ElementPerCycleInt32) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int32";
    ASSERT_TRUE(test.TestExecute1ElementPerCycle<int32>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute1ElementPerCycleUInt64) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "uint64";
    ASSERT_TRUE(test.TestExecute1ElementPerCycle<uint64>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute1ElementPerCycleInt64) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "int64";
    ASSERT_TRUE(test.TestExecute1ElementPerCycle<int64>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute1ElementPerCycleFloat32) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "float32";
    ASSERT_TRUE(test.TestExecute1ElementPerCycle<float32>(auxStr));
}

TEST(WaveformChirpGAMTest, TestExecute1ElementPerCycleFloat64) {
    WaveformChirpGAMTest test;
    StreamString auxStr = "float64";
    ASSERT_TRUE(test.TestExecute1ElementPerCycle<float64>(auxStr));
}




/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
