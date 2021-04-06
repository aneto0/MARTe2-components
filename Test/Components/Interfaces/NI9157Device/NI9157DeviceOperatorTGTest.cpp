/**
 * @file NI9157DeviceOperatorTGTest.cpp
 * @brief Source file for class NI9157DeviceOperatorTGTest.
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
 * the class NI9157DeviceOperatorTGTest (public, protected, and private). Be
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
#include "NI9157DeviceOperatorTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI9157DeviceOperatorTGTest,TestConstructor) {
    NI9157DeviceOperatorTTest<bool> test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI9157DeviceOperatorTGTest,TestIsValid) {
    NI9157DeviceOperatorTTest<bool> test;
    ASSERT_TRUE(test.TestIsValid());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_Bool) {
    NI9157DeviceOperatorTTest<bool> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestGetByteSize_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    ASSERT_TRUE(test.TestGetByteSize());
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    ASSERT_TRUE(test.TestCompare(0xff, 0xff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    ASSERT_TRUE(test.TestCompare(1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    ASSERT_TRUE(test.TestCompare(2, 1));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    ASSERT_TRUE(test.TestCompare(0x7f, 0x7f));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    ASSERT_TRUE(test.TestCompare(-1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    ASSERT_TRUE(test.TestCompare(2, -1));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    ASSERT_TRUE(test.TestCompare(0xffff, 0xffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    ASSERT_TRUE(test.TestCompare(1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    ASSERT_TRUE(test.TestCompare(2, 1));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    ASSERT_TRUE(test.TestCompare(0x7fff, 0x7fff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    ASSERT_TRUE(test.TestCompare(-1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    ASSERT_TRUE(test.TestCompare(2, -1));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    ASSERT_TRUE(test.TestCompare(0xffffffff, 0xffffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    ASSERT_TRUE(test.TestCompare(1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    ASSERT_TRUE(test.TestCompare(2, 1));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    ASSERT_TRUE(test.TestCompare(0x7fffffff, 0x7fffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    ASSERT_TRUE(test.TestCompare(-1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    ASSERT_TRUE(test.TestCompare(2, -1));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    ASSERT_TRUE(test.TestCompare(0xffffffffffffffff, 0xffffffffffffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    ASSERT_TRUE(test.TestCompare(1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    ASSERT_TRUE(test.TestCompare(2, 1));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    ASSERT_TRUE(test.TestCompare(0x7fffffffffffffff, 0x7fffffffffffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Minor_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    ASSERT_TRUE(test.TestCompare(-1, 2));
}

TEST(NI9157DeviceOperatorTGTest,TestCompare_Greater_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    ASSERT_TRUE(test.TestCompare(2, -1));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    ASSERT_TRUE(test.TestCopy(0xff));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    ASSERT_TRUE(test.TestCopy(0x7f));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    ASSERT_TRUE(test.TestCopy(0xffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    ASSERT_TRUE(test.TestCopy(0x7fff));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    ASSERT_TRUE(test.TestCopy(0xffffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    ASSERT_TRUE(test.TestCopy(0x7fffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    ASSERT_TRUE(test.TestCopy(0xffffffffffffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestCopy_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    ASSERT_TRUE(test.TestCopy(0x7fffffffffffffff));
}

TEST(NI9157DeviceOperatorTGTest,TestFindResource_Bool) {
    NI9157DeviceOperatorTTest<bool> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestFindResource(idx, "IndBool");
        }
        else {
            ret = test.TestFindResource(idx, "IndBool");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestFindResource_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestFindResource(idx, "IndU8");
        }
        else {
            ret = test.TestFindResource(idx, "IndU8");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestFindResource_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestFindResource(idx, "IndI8");
        }
        else {
            ret = test.TestFindResource(idx, "IndI8");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestFindResource_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestFindResource(idx, "IndU16");
        }
        else {
            ret = test.TestFindResource(idx, "IndU16");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestFindResource_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestFindResource(idx, "IndI16");
        }
        else {
            ret = test.TestFindResource(idx, "IndI16");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestFindResource_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestFindResource(idx, "IndU32");
        }
        else {
            ret = test.TestFindResource(idx, "IndU32");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestFindResource_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestFindResource(idx, "IndI32");
        }
        else {
            ret = test.TestFindResource(idx, "IndI32");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestFindResource_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestFindResource(idx, "IndU64");
        }
        else {
            ret = test.TestFindResource(idx, "IndU64");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestFindResource_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestFindResource(idx, "IndI64");
        }
        else {
            ret = test.TestFindResource(idx, "IndI64");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWrite_Bool) {
    NI9157DeviceOperatorTTest<bool> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWrite(idx, "ContBool");
        }
        else {
            ret = test.TestNiWrite(idx, "ContBool");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiRead_Bool) {
    NI9157DeviceOperatorTTest<bool> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiRead(idx, "ContBool", "IndBool");
        }
        else {
            ret = test.TestNiRead(idx, "ContBool", "IndBool");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWrite_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWrite(idx, "ContU8");
        }
        else {
            ret = test.TestNiWrite(idx, "ContU8");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiRead_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiRead(idx, "ContU8", "IndU8");
        }
        else {
            ret = test.TestNiRead(idx, "ContU8", "IndU8");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWrite_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWrite(idx, "ContU16");
        }
        else {
            ret = test.TestNiWrite(idx, "ContU16");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiRead_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiRead(idx, "ContU16", "IndU16");
        }
        else {
            ret = test.TestNiRead(idx, "ContU16", "IndU16");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWrite_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWrite(idx, "ContU32");
        }
        else {
            ret = test.TestNiWrite(idx, "ContU32");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiRead_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiRead(idx, "ContU32", "IndU32");
        }
        else {
            ret = test.TestNiRead(idx, "ContU32", "IndU32");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWrite_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWrite(idx, "ContU64");
        }
        else {
            ret = test.TestNiWrite(idx, "ContU64");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiRead_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiRead(idx, "ContU64", "IndU64");
        }
        else {
            ret = test.TestNiRead(idx, "ContU64", "IndU64");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWrite_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWrite(idx, "ContI8");
        }
        else {
            ret = test.TestNiWrite(idx, "ContI8");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiRead_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiRead(idx, "ContI8", "IndI8");
        }
        else {
            ret = test.TestNiRead(idx, "ContI8", "IndI8");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWrite_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWrite(idx, "ContI16");
        }
        else {
            ret = test.TestNiWrite(idx, "ContI16");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiRead_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiRead(idx, "ContI16", "IndI16");
        }
        else {
            ret = test.TestNiRead(idx, "ContI16", "IndI16");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWrite_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWrite(idx, "ContI32");
        }
        else {
            ret = test.TestNiWrite(idx, "ContI32");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiRead_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiRead(idx, "ContI32", "IndI32");
        }
        else {
            ret = test.TestNiRead(idx, "ContI32", "IndI32");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWrite_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWrite(idx, "ContI64");
        }
        else {
            ret = test.TestNiWrite(idx, "ContI64");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiRead_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiRead(idx, "ContI64", "IndI64");
        }
        else {
            ret = test.TestNiRead(idx, "ContI64", "IndI64");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWriteFifo_Bool) {
    NI9157DeviceOperatorTTest<bool> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo(idx, "FIFO0_BOOL_W");
        }
        else {
            ret = test.TestNiWriteFifo(idx, "FIFO0_BOOL_W");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiReadFifo_Bool) {
    NI9157DeviceOperatorTTest<bool> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo(idx, "FIFO0_BOOL_W", "FIFO1_BOOL_R");
        }
        else {
            ret = test.TestNiReadFifo(idx, "FIFO0_BOOL_W", "FIFO1_BOOL_R");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWriteFifo_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo(idx, "FIFO0_U8_W");
        }
        else {
            ret = test.TestNiWriteFifo(idx, "FIFO0_U8_W");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiReadFifo_U8) {
    NI9157DeviceOperatorTTest<uint8> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo(idx, "FIFO0_U8_W", "FIFO1_U8_R");
        }
        else {
            ret = test.TestNiReadFifo(idx, "FIFO0_U8_W", "FIFO1_U8_R");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWriteFifo_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo(idx, "FIFO0_I8_W");
        }
        else {
            ret = test.TestNiWriteFifo(idx, "FIFO0_I8_W");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiReadFifo_I8) {
    NI9157DeviceOperatorTTest<int8> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo(idx, "FIFO0_I8_W", "FIFO1_I8_R");
        }
        else {
            ret = test.TestNiReadFifo(idx, "FIFO0_I8_W", "FIFO1_I8_R");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWriteFifo_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo(idx, "FIFO0_U16_W");
        }
        else {
            ret = test.TestNiWriteFifo(idx, "FIFO0_U16_W");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiReadFifo_U16) {
    NI9157DeviceOperatorTTest<uint16> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo(idx, "FIFO0_U16_W", "FIFO1_U16_R");
        }
        else {
            ret = test.TestNiReadFifo(idx, "FIFO0_U16_W", "FIFO1_U16_R");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWriteFifo_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo(idx, "FIFO0_I16_W");
        }
        else {
            ret = test.TestNiWriteFifo(idx, "FIFO0_I16_W");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiReadFifo_I16) {
    NI9157DeviceOperatorTTest<int16> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo(idx, "FIFO0_I16_W", "FIFO1_I16_R");
        }
        else {
            ret = test.TestNiReadFifo(idx, "FIFO0_I16_W", "FIFO1_I16_R");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWriteFifo_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo(idx, "FIFO0_U32_W");
        }
        else {
            ret = test.TestNiWriteFifo(idx, "FIFO0_U32_W");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiReadFifo_U32) {
    NI9157DeviceOperatorTTest<uint32> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo(idx, "FIFO0_U32_W", "FIFO1_U32_R");
        }
        else {
            ret = test.TestNiReadFifo(idx, "FIFO0_U32_W", "FIFO1_U32_R");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWriteFifo_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo(idx, "FIFO0_I32_W");
        }
        else {
            ret = test.TestNiWriteFifo(idx, "FIFO0_I32_W");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiReadFifo_I32) {
    NI9157DeviceOperatorTTest<int32> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo(idx, "FIFO0_I32_W", "FIFO1_I32_R");
        }
        else {
            ret = test.TestNiReadFifo(idx, "FIFO0_I32_W", "FIFO1_I32_R");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWriteFifo_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo(idx, "FIFO0_U64_W");
        }
        else {
            ret = test.TestNiWriteFifo(idx, "FIFO0_U64_W");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiReadFifo_U64) {
    NI9157DeviceOperatorTTest<uint64> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo(idx, "FIFO0_U64_W", "FIFO1_U64_R");
        }
        else {
            ret = test.TestNiReadFifo(idx, "FIFO0_U64_W", "FIFO1_U64_R");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiWriteFifo_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiWriteFifo(idx, "FIFO0_I64_W");
        }
        else {
            ret = test.TestNiWriteFifo(idx, "FIFO0_I64_W");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestNiReadFifo_I64) {
    NI9157DeviceOperatorTTest<int64> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestNiReadFifo(idx, "FIFO0_I64_W", "FIFO1_I64_R");
        }
        else {
            ret = test.TestNiReadFifo(idx, "FIFO0_I64_W", "FIFO1_I64_R");
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157DeviceOperatorTGTest,TestGetNI9157Device) {
    NI9157DeviceOperatorTTest<int64> test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestGetNI9157Device(idx);
        }
        else {
            ret = test.TestGetNI9157Device(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}
