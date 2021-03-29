/**
 * @file NI9157DeviceOperatorTIGTest.cpp
 * @brief Source file for class NI9157DeviceOperatorTIGTest.
 * @date 22/03/2021
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
 * the class NI9157DeviceOperatorTIGTest (public, protected, and private). Be
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
#include "NI9157DeviceOperatorTITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI9157DeviceOperatorTIGTest,TestConstructor) {
    NI9157DeviceOperatorTITest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI9157DeviceOperatorTIGTest,TestIsValid) {
    NI9157DeviceOperatorTITest test;
    ASSERT_TRUE(test.TestIsValid());
}

TEST(NI9157DeviceOperatorTIGTest,TestGetByteSize) {
    NI9157DeviceOperatorTITest test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTIGTest,TestGetNI9157Device) {
    NI9157DeviceOperatorTITest test;
    ASSERT_TRUE(test.TestGetNI9157Device());
}

TEST(NI9157DeviceOperatorTIGTest,TestCompare) {
    NI9157DeviceOperatorTITest test;
    ASSERT_TRUE(test.TestCompare());
}

TEST(NI9157DeviceOperatorTIGTest,TestCopy) {
    NI9157DeviceOperatorTITest test;
    ASSERT_TRUE(test.TestCopy());
}

TEST(NI9157DeviceOperatorTIGTest,TestFindResource) {
    NI9157DeviceOperatorTITest test;
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

TEST(NI9157DeviceOperatorTIGTest,TestNiWrite) {
    NI9157DeviceOperatorTITest test;
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

TEST(NI9157DeviceOperatorTIGTest,TestNiRead) {
    NI9157DeviceOperatorTITest test;
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

TEST(NI9157DeviceOperatorTIGTest,TestNiWriteFifo) {
    NI9157DeviceOperatorTITest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo(idx);
        }
        else {
            ret = test.TestNiWriteFifo(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTIGTest,TestNiReadFifo) {
    NI9157DeviceOperatorTITest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo(idx);
        }
        else {
            ret = test.TestNiReadFifo(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}
