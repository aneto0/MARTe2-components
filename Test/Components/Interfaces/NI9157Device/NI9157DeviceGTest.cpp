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
 * the class NI9157DeviceGTest (public, protected, and private). Be aware that some 
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
#include "NI9157DeviceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * The number of devices to be test. Must match firmware config paths.
 */
static uint32 nDevices = 1;

/**
 * Test for all devices (true) or until one matches the firmware config paths
 * (false).
 */
static bool testAll = false;

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
        if(testAll) {
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
        if(testAll) {
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
        if(testAll) {
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
        if(testAll) {
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
        if(testAll) {
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
        if(testAll) {
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
        if(testAll) {
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
        if(testAll) {
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

TEST(NI9157DeviceGTest,TestOpenIsOpened) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestOpenIsOpened(idx);
        }
        else {
            ret = test.TestOpenIsOpened(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestRunIsRunning) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestRunIsRunning(idx);
        }
        else {
            ret = test.TestRunIsRunning(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestGetSessionResetClose) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestGetSessionResetClose(idx);
        }
        else {
            ret = test.TestGetSessionResetClose(idx);
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
        if(testAll) {
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

TEST(NI9157DeviceGTest,TestNiWriteRead) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestNiWriteRead(idx);
        }
        else {
            ret = test.TestNiWriteRead(idx);
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
        if(testAll) {
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

TEST(NI9157DeviceGTest,TestNiStartStopFifo) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestNiStartStopFifo(idx);
        }
        else {
            ret = test.TestNiStartStopFifo(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteReadFifo_U8) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestNiWriteReadFifo_U8(idx);
        }
        else {
            ret = test.TestNiWriteReadFifo_U8(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteReadFifo_U16) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestNiWriteReadFifo_U16(idx);
        }
        else {
            ret = test.TestNiWriteReadFifo_U16(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteReadFifo_U32) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestNiWriteReadFifo_U32(idx);
        }
        else {
            ret = test.TestNiWriteReadFifo_U32(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteReadFifo_U64) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestNiWriteReadFifo_U64(idx);
        }
        else {
            ret = test.TestNiWriteReadFifo_U64(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteReadFifo_I8) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestNiWriteReadFifo_I8(idx);
        }
        else {
            ret = test.TestNiWriteReadFifo_I8(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteReadFifo_I16) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestNiWriteReadFifo_I16(idx);
        }
        else {
            ret = test.TestNiWriteReadFifo_I16(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteReadFifo_I32) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestNiWriteReadFifo_I32(idx);
        }
        else {
            ret = test.TestNiWriteReadFifo_I32(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceGTest,TestNiWriteReadFifo_I64) {
    NI9157DeviceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAll) {
            ret &= test.TestNiWriteReadFifo_I64(idx);
        }
        else {
            ret = test.TestNiWriteReadFifo_I64(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}
