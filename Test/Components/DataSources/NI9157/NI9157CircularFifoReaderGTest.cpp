/**
 * @file NI9157CircularFifoReaderGTest.cpp
 * @brief Source file for class NI9157CircularFifoReaderGTest.
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
 * the class NI9157CircularFifoReaderGTest (public, protected, and private). Be
 * aware that some methods, such as those inline could be defined on the header
 * file, instead.
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
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise(idx);
        }
        else {
            ret = test.TestInitialise(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultRunNi) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_DefaultRunNi(idx);
        }
        else {
            ret = test.TestInitialise_DefaultRunNi(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultNumberOfPacketsInFIFO) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_DefaultNumberOfPacketsInFIFO(idx);
        }
        else {
            ret = test.TestInitialise_DefaultNumberOfPacketsInFIFO(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultAcquiredFromPacket) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_DefaultAcquiredFromPacket(idx);
        }
        else {
            ret = test.TestInitialise_DefaultAcquiredFromPacket(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultCheckCounterAfterNPackets) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_DefaultCheckCounterAfterNPackets(idx);
        }
        else {
            ret = test.TestInitialise_DefaultCheckCounterAfterNPackets(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultCheckPacketCounter) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_DefaultCheckPacketCounter(idx);
        }
        else {
            ret = test.TestInitialise_DefaultCheckPacketCounter(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultCounterStep) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_DefaultCounterStep(idx);
        }
        else {
            ret = test.TestInitialise_DefaultCounterStep(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_DefaultNFrameForSync) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_DefaultNFrameForSync(idx);
        }
        else {
            ret = test.TestInitialise_DefaultNFrameForSync(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_NFrameForSyncEqualTo1) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_False_NFrameForSyncEqualTo1(idx);
        }
        else {
            ret = test.TestInitialise_False_NFrameForSyncEqualTo1(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_CounterStepZero) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_False_CounterStepZero(idx);
        }
        else {
            ret = test.TestInitialise_False_CounterStepZero(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_CheckAfterNotDivideCounterStep) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_False_CheckAfterNotDivideCounterStep(idx);
        }
        else {
            ret = test.TestInitialise_False_CheckAfterNotDivideCounterStep(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_CounterStepNotDivideAcquireFromCounterDifference) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_False_CounterStepNotDivideAcquireFromCounterDifference(idx);
        }
        else {
            ret = test.TestInitialise_False_CounterStepNotDivideAcquireFromCounterDifference(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_NoFifoName) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_False_NoFifoName(idx);
        }
        else {
            ret = test.TestInitialise_False_NoFifoName(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_NoNI9157Device) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_False_NoNI9157Device(idx);
        }
        else {
            ret = test.TestInitialise_False_NoNI9157Device(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestInitialise_False_Invalidi9157Device) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_False_Invalidi9157Device(idx);
        }
        else {
            ret = test.TestInitialise_False_Invalidi9157Device(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestSetConfiguredDatabase) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSetConfiguredDatabase(idx);
        }
        else {
            ret = test.TestSetConfiguredDatabase(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestSetConfiguredDatabase_False_MoreThanOneChannel) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSetConfiguredDatabase_False_MoreThanOneChannel(idx);
        }
        else {
            ret = test.TestSetConfiguredDatabase_False_MoreThanOneChannel(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestSetConfiguredDatabase_False_NiDevOperatorNotFound) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSetConfiguredDatabase_False_NiDevOperatorNotFound(idx);
        }
        else {
            ret = test.TestSetConfiguredDatabase_False_NiDevOperatorNotFound(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestSetConfiguredDatabase_False_InvalidFifoName) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSetConfiguredDatabase_False_InvalidFifoName(idx);
        }
        else {
            ret = test.TestSetConfiguredDatabase_False_InvalidFifoName(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestPrepareNextState) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestPrepareNextState(idx);
        }
        else {
            ret = test.TestPrepareNextState(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestPrepareNextState_TwoStates) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestPrepareNextState_TwoStates(idx);
        }
        else {
            ret = test.TestPrepareNextState_TwoStates(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}
 
TEST(NI9157CircularFifoReaderGTest,TestStopAcquisition) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestStopAcquisition(idx);
        }
        else {
            ret = test.TestStopAcquisition(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestStartAcquisition) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestStartAcquisition(idx);
        }
        else {
            ret = test.TestStartAcquisition(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestSynchronise) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSynchronise(idx);
        }
        else {
            ret = test.TestSynchronise(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestSynchronise_NoStart) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSynchronise_NoStart(idx);
        }
        else {
            ret = test.TestSynchronise_NoStart(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead(idx);
        }
        else {
            ret = test.TestDriverRead(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_AllSignals) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead_AllSignals(idx);
        }
        else {
            ret = test.TestDriverRead_AllSignals(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_AcquiredFromCounter) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead_AcquiredFromCounter(idx);
        }
        else {
            ret = test.TestDriverRead_AcquiredFromCounter(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverReadCompleteCycle) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverReadCompleteCycle(idx);
        }
        else {
            ret = test.TestDriverReadCompleteCycle(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_Resync) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead_Resync(idx);
        }
        else {
            ret = test.TestDriverRead_Resync(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_NoCheckCounter) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead_NoCheckCounter(idx);
        }
        else {
            ret = test.TestDriverRead_NoCheckCounter(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_CheckAfterNPackets) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead_CheckAfterNPackets(idx);
        }
        else {
            ret = test.TestDriverRead_CheckAfterNPackets(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_InternalInterleaved) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead_InternalInterleaved(idx);
        }
        else {
            ret = test.TestDriverRead_InternalInterleaved(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_NoCheckFrame) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead_NoCheckFrame(idx);
        }
        else {
            ret = test.TestDriverRead_NoCheckFrame(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_SetNonBlockSleepT) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead_SetNonBlockSleepT(idx);
        }
        else {
            ret = test.TestDriverRead_SetNonBlockSleepT(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_FifoTOutNoErrorChecker) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead_FifoTOutNoErrorChecker(idx);
        }
        else {
            ret = test.TestDriverRead_FifoTOutNoErrorChecker(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_FifoTOutWithErrorChecker) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead_FifoTOutWithErrorChecker(idx);
        }
        else {
            ret = test.TestDriverRead_FifoTOutWithErrorChecker(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_HeaderSize) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead_HeaderSize(idx);
        }
        else {
            ret = test.TestDriverRead_HeaderSize(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157CircularFifoReaderGTest,TestDriverRead_HeaderSizeCheckFrame) {
    NI9157CircularFifoReaderTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestDriverRead_HeaderSizeCheckFrame(idx);
        }
        else {
            ret = test.TestDriverRead_HeaderSizeCheckFrame(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}
