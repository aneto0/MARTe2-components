/**
 * @file EPICSPVStructureDataIGTest.cpp
 * @brief Source file for class EPICSPVStructureDataIGTest
 * @date 27/03/2017
 * @author Andre Neto
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
 * the class EPICSPVStructureDataIGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EPICSPVAStructureDataITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(EPICSPVAStructureDataIGTest,TestConstructor) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt8) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt8());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt16) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt16());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt32) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt32());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt64) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt64());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int8) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int8());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int16) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int16());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int32) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int32());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int64) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int64());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Float32) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int32());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Float64) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int64());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_StreamString) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_StreamString());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt8_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt8_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt16_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt16_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt32_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt32_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_UInt64_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_UInt64_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int8_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int8_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int16_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int16_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int32_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int32_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Int64_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Int64_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Float32_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Float32_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_Float64_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_Float64_Array());
}

TEST(EPICSPVAStructureDataIGTest,TestRead_StreamString_Array) {
    EPICSPVAStructureDataITest test;
    ASSERT_TRUE(test.TestRead_StreamString_Array());
}
