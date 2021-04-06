/**
 * @file NI9157DeviceGTest.cpp
 * @brief Source file for class NI9157DeviceGTest
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
 * the class NI9157DeviceGTest (public, protected, and private). Be aware that
 * some methods, such as those inline could be defined on the header file,
 * instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157DeviceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI9157DeviceGTest,TestConstructor) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI9157DeviceGTest,TestInitialise) {
    NI9157DeviceTest test;
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

TEST(NI9157DeviceGTest,TestInitialiseIsOpened) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialiseIsOpened(idx);
        }
        else {
            ret = test.TestInitialiseIsOpened(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestInitialiseRandomConfig) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialiseRandomConfig(idx);
        }
        else {
            ret = test.TestInitialiseRandomConfig(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestInitialise_FalseNoDeviceName) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_FalseNoDeviceName(idx);
        }
        else {
            ret = test.TestInitialise_FalseNoDeviceName(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestInitialise_FalseNoGenFile) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_FalseNoGenFile(idx);
        }
        else {
            ret = test.TestInitialise_FalseNoGenFile(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestInitialise_FalseNoGenSignature) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_FalseNoGenSignature(idx);
        }
        else {
            ret = test.TestInitialise_FalseNoGenSignature(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestInitialise_FalseNoType) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_FalseNoType(idx);
        }
        else {
            ret = test.TestInitialise_FalseNoType(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestInitialise_FalseVariableNotFound) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_FalseVariableNotFound(idx);
        }
        else {
            ret = test.TestInitialise_FalseVariableNotFound(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestInitialise_FalseOpenFakeDevice) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_FalseOpenFakeDevice(idx);
        }
        else {
            ret = test.TestInitialise_FalseOpenFakeDevice(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestInitialise_OpenResetRun) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_OpenResetRun(idx);
        }
        else {
            ret = test.TestInitialise_OpenResetRun(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestInitialise_NoOpenResetRun) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_NoOpenResetRun(idx);
        }
        else {
            ret = test.TestInitialise_NoOpenResetRun(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestInitialise_FalseNoConfig) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_FalseNoConfig(idx);
        }
        else {
            ret = test.TestInitialise_FalseNoConfig(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestOpen) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestOpen(idx);
        }
        else {
            ret = test.TestOpen(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestOpen_FalseParams) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestOpen_FalseParams(idx);
        }
        else {
            ret = test.TestOpen_FalseParams(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestIsOpened) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestIsOpened(idx);
        }
        else {
            ret = test.TestIsOpened(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestRun) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestRun(idx);
        }
        else {
            ret = test.TestRun(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestRun_FalseParams) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestRun_FalseParams(idx);
        }
        else {
            ret = test.TestRun_FalseParams(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestIsRunning) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestIsRunning(idx);
        }
        else {
            ret = test.TestIsRunning(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestGetSession) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestGetSession(idx);
        }
        else {
            ret = test.TestGetSession(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestReset) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestReset(idx);
        }
        else {
            ret = test.TestReset(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestClose) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestClose(idx);
        }
        else {
            ret = test.TestClose(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestCrioStart) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestCrioStart(idx);
        }
        else {
            ret = test.TestCrioStart(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestCrioStop) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestCrioStop(idx);
        }
        else {
            ret = test.TestCrioStop(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestFindResource) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestFindResource(idx);
        }
        else {
            ret = test.TestFindResource(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWrite) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWrite(idx);
        }
        else {
            ret = test.TestNiWrite(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiRead) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiRead(idx);
        }
        else {
            ret = test.TestNiRead(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiConfigureFifo) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiConfigureFifo(idx);
        }
        else {
            ret = test.TestNiConfigureFifo(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiStartFifo) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiStartFifo(idx);
        }
        else {
            ret = test.TestNiStartFifo(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiStopFifo) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiStopFifo(idx);
        }
        else {
            ret = test.TestNiStopFifo(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestWriteParam) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestWriteParam(idx);
        }
        else {
            ret = test.TestWriteParam(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestReadParam) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestReadParam(idx);
        }
        else {
            ret = test.TestReadParam(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteFifo_Bool) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo_Bool(idx);
        }
        else {
            ret = test.TestNiWriteFifo_Bool(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiReadFifo_Bool) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo_Bool(idx);
        }
        else {
            ret = test.TestNiReadFifo_Bool(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteFifo_U8) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo_U8(idx);
        }
        else {
            ret = test.TestNiWriteFifo_U8(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiReadFifo_U8) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo_U8(idx);
        }
        else {
            ret = test.TestNiReadFifo_U8(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteFifo_U16) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo_U16(idx);
        }
        else {
            ret = test.TestNiWriteFifo_U16(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiReadFifo_U16) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo_U16(idx);
        }
        else {
            ret = test.TestNiReadFifo_U16(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteFifo_U32) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo_U32(idx);
        }
        else {
            ret = test.TestNiWriteFifo_U32(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiReadFifo_U32) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo_U32(idx);
        }
        else {
            ret = test.TestNiReadFifo_U32(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteFifo_U64) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo_U64(idx);
        }
        else {
            ret = test.TestNiWriteFifo_U64(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiReadFifo_U64) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo_U64(idx);
        }
        else {
            ret = test.TestNiReadFifo_U64(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteFifo_I8) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo_I8(idx);
        }
        else {
            ret = test.TestNiWriteFifo_I8(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiReadFifo_I8) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo_I8(idx);
        }
        else {
            ret = test.TestNiReadFifo_I8(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteFifo_I16) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo_I16(idx);
        }
        else {
            ret = test.TestNiWriteFifo_I16(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiReadFifo_I16) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo_I16(idx);
        }
        else {
            ret = test.TestNiReadFifo_I16(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteFifo_I32) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo_I32(idx);
        }
        else {
            ret = test.TestNiWriteFifo_I32(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiReadFifo_I32) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo_I32(idx);
        }
        else {
            ret = test.TestNiReadFifo_I32(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteFifo_I64) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo_I64(idx);
        }
        else {
            ret = test.TestNiWriteFifo_I64(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiReadFifo_I64) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo_I64(idx);
        }
        else {
            ret = test.TestNiReadFifo_I64(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}
