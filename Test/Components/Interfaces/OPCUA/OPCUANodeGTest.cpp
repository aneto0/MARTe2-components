/**
 * @file OPCUANodeGTest.cpp
 * @brief Source file for class OPCUANodeGTest
 * @date 12/03/2019
 * @author Luca Porzio
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
 * the class OPCUANodeGTest (public, protected, and private). Be aware that some 
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

#include "OPCUANodeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(OPCUANodeGTest,TestGetOPCVariable_uint8) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_uint8());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_uint16) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_uint16());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_uint32) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_uint32());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_uint64) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_uint64());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_float32) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_float32());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_float64) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_float64());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_int8) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_int8());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_int16) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_int16());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_int32) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_int32());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_int64) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_int64());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_array_uint8) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_array_uint8());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_array_uint16) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_array_uint16());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_array_uint32) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_array_uint32());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_array_uint64) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_array_uint64());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_array_float32) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_array_float32());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_array_float64) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_array_float64());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_array_int8) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_array_int8());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_array_int16) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_array_int16());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_array_int32) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_array_int32());
}

TEST(OPCUANodeGTest,TestGetOPCVariable_array_int64) {
    OPCUANodeTest test;
    ASSERT_TRUE(test.TestGetOPCVariable_array_int64());
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
