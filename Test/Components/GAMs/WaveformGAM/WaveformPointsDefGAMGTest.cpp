/**
 * @file WaveformPointsDefGAMGTest.cpp
 * @brief Source file for class WaveformPointsDefGAMGTest
 * @date 30/05/2017
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
 * the class WaveformPointsDefGAMGTest (public, protected, and private). Be aware that some 
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


#include "WaveformPointsDefGAMTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;


TEST(WaveformPointsDefGAMTest, TestMissingPoints) {
    WaveformPointsDefGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingPoints());
}

TEST(WaveformPointsDefGAMTest, TestFailingReadingPointsValues) {
    WaveformPointsDefGAMTest test;
    ASSERT_TRUE(test.TestInitialise_FailingReadingPointsValues());
}

TEST(WaveformPointsDefGAMTest, Test1Point) {
    WaveformPointsDefGAMTest test;
    ASSERT_TRUE(test.TestInitialise_1Point());
}

TEST(WaveformPointsDefGAMTest, TestMissingTimes) {
    WaveformPointsDefGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingTimes());
}

TEST(WaveformPointsDefGAMTest, TestDifferentSizePointsTimes) {
    WaveformPointsDefGAMTest test;
    ASSERT_TRUE(test.TestInitialise_DifferentSizePointsTimes());
}

TEST(WaveformPointsDefGAMTest, TestFailingReadingTimesValues) {
    WaveformPointsDefGAMTest test;
    ASSERT_TRUE(test.TestInitialise_FailingReadingTimesValues());
}

TEST(WaveformPointsDefGAMTest, TestInvalidTimes) {
    WaveformPointsDefGAMTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidTimes());
}

TEST(WaveformPointsDefGAMTest, TestInvalidTimes2) {
    WaveformPointsDefGAMTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidTimes2());
}

TEST(WaveformPointsDefGAMTest, TestFailWaveformSetup) {
    WaveformPointsDefGAMTest test;
    ASSERT_TRUE(test.TestInitialise_FailWaveformSetup());
}

TEST(WaveformPointsDefGAMTest, TestExecuteInt8) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int8";
    ASSERT_TRUE(test.TestExecute<int8>(str));

}

TEST(WaveformPointsDefGAMTest, TestExecuteUInt8) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint8";
    ASSERT_TRUE(test.TestExecute<uint8>(str));

}

TEST(WaveformPointsDefGAMTest, TestExecuteInt16) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int16";
    ASSERT_TRUE(test.TestExecute<int16>(str));

}

TEST(WaveformPointsDefGAMTest, TestExecuteUInt16) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint16";
    ASSERT_TRUE(test.TestExecute<uint16>(str));

}

TEST(WaveformPointsDefGAMTest, TestExecuteInt32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int32";
    ASSERT_TRUE(test.TestExecute<int32>(str));

}

TEST(WaveformPointsDefGAMTest, TestExecuteUInt32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint32";
    ASSERT_TRUE(test.TestExecute<uint32>(str));

}

TEST(WaveformPointsDefGAMTest, TestExecuteInt64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int64";
    ASSERT_TRUE(test.TestExecute<int64>(str));

}

TEST(WaveformPointsDefGAMTest, TestExecuteUInt64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint64";
    ASSERT_TRUE(test.TestExecute<uint64>(str));

}

TEST(WaveformPointsDefGAMTest, TestExecuteFloat32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "float32";
    ASSERT_TRUE(test.TestExecute<float32>(str));

}

TEST(WaveformPointsDefGAMTest, TestExecuteFloat64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "float64";
    ASSERT_TRUE(test.TestExecute<float64>(str));

}

TEST(WaveformPointsDefGAMTest, TestExecuteTriggerInt8) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int8";
    ASSERT_TRUE(test.TestExecuteTrigger<int8>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTriggerUInt8) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint8";
    ASSERT_TRUE(test.TestExecuteTrigger<uint8>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTriggerInt16) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int16";
    ASSERT_TRUE(test.TestExecuteTrigger<int16>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTriggerUInt16) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint16";
    ASSERT_TRUE(test.TestExecuteTrigger<uint16>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTriggerInt32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int32";
    ASSERT_TRUE(test.TestExecuteTrigger<int32>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTriggerUInt32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint32";
    ASSERT_TRUE(test.TestExecuteTrigger<uint32>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTriggerInt64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int64";
    ASSERT_TRUE(test.TestExecuteTrigger<int64>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTriggerUInt64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint64";
    ASSERT_TRUE(test.TestExecuteTrigger<uint64>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTriggerFloa32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "float32";
    ASSERT_TRUE(test.TestExecuteTrigger<float32>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTriggerFloat64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "float64";
    ASSERT_TRUE(test.TestExecuteTrigger<float64>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecute2SignalsInt8) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int8";
    ASSERT_TRUE(test.TestExecute2Signals<int8>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecute2SignalsUInt8) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint8";
    ASSERT_TRUE(test.TestExecute2Signals<uint8>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecute2SignalsInt16) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int16";
    ASSERT_TRUE(test.TestExecute2Signals<int16>(str));
}


TEST(WaveformPointsDefGAMTest, TestExecute2SignalsUInt16) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint16";
    ASSERT_TRUE(test.TestExecute2Signals<uint16>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecute2SignalsInt32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int32";
    ASSERT_TRUE(test.TestExecute2Signals<int32>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecute2SignalsUInt32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint32";
    ASSERT_TRUE(test.TestExecute2Signals<uint32>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecute2SignalsInt64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int64";
    ASSERT_TRUE(test.TestExecute2Signals<int64>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecute2SignalsUInt64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint64";
    ASSERT_TRUE(test.TestExecute2Signals<uint64>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecute2SignalsFloat32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "float32";
    ASSERT_TRUE(test.TestExecute2Signals<float32>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecute2SignalsFloat64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "float64";
    ASSERT_TRUE(test.TestExecute2Signals<float64>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger2PointUInt8) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint8";
    ASSERT_TRUE(test.TestExecuteTrigger2Point<uint8>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger2PointInt8) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int8";
    ASSERT_TRUE(test.TestExecuteTrigger2Point<int8>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger2PointUInt16) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint16";
    ASSERT_TRUE(test.TestExecuteTrigger2Point<uint16>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger2PointInt16) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint16";
    ASSERT_TRUE(test.TestExecuteTrigger2Point<uint16>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger2PointUInt32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint32";
    ASSERT_TRUE(test.TestExecuteTrigger2Point<uint32>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger2PointInt32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int32";
    ASSERT_TRUE(test.TestExecuteTrigger2Point<int32>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger2PointUInt64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint64";
    ASSERT_TRUE(test.TestExecuteTrigger2Point<uint64>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger2PointInt64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int64";
    ASSERT_TRUE(test.TestExecuteTrigger2Point<int64>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger2PointFloat32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "float32";
    ASSERT_TRUE(test.TestExecuteTrigger2Point<float32>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger2PointFloat64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "float64";
    ASSERT_TRUE(test.TestExecuteTrigger2Point<float64>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger_1ElementUInt8) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint8";
    ASSERT_TRUE(test.TestExecuteTrigger_1Element<uint8>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger_1ElementInt8) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int8";
    ASSERT_TRUE(test.TestExecuteTrigger_1Element<int8>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger_1ElementUInt16) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint16";
    ASSERT_TRUE(test.TestExecuteTrigger_1Element<uint16>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger_1ElementInt16) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int16";
    ASSERT_TRUE(test.TestExecuteTrigger_1Element<int16>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger_1ElementUInt32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint32";
    ASSERT_TRUE(test.TestExecuteTrigger_1Element<uint32>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger_1ElementInt32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int32";
    ASSERT_TRUE(test.TestExecuteTrigger_1Element<int32>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger_1ElementUInt64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "uint64";
    ASSERT_TRUE(test.TestExecuteTrigger_1Element<uint64>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger_1ElementInt648) {
    WaveformPointsDefGAMTest test;
    StreamString str = "int64";
    ASSERT_TRUE(test.TestExecuteTrigger_1Element<int64>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger_1ElementFloat32) {
    WaveformPointsDefGAMTest test;
    StreamString str = "float32";
    ASSERT_TRUE(test.TestExecuteTrigger_1Element<float32>(str));
}

TEST(WaveformPointsDefGAMTest, TestExecuteTrigger_1ElementFloat64) {
    WaveformPointsDefGAMTest test;
    StreamString str = "float64";
    ASSERT_TRUE(test.TestExecuteTrigger_1Element<float64>(str));
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
