/**
 * @file ObjectConnectionIGTest.cpp
 * @brief Source file for class ObjectConnectionIGTest
 * @date 15/5/2025
 * @author nferron
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
 * the class ObjectConnectionIGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ObjectConnectionITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ObjectConnectionIGTest,TestConstructor) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(ObjectConnectionIGTest,TestInitialise) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(ObjectConnectionIGTest,TestTransposeAndCopy_Uint8) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestTransposeAndCopy<uint8>(MARTe::UnsignedInteger8Bit));
}

TEST(ObjectConnectionIGTest,TestTransposeAndCopy_Uint16) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestTransposeAndCopy<uint16>(MARTe::UnsignedInteger16Bit));
}

TEST(ObjectConnectionIGTest,TestTransposeAndCopy_Uint32) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestTransposeAndCopy<uint32>(MARTe::UnsignedInteger32Bit));
}

TEST(ObjectConnectionIGTest,TestTransposeAndCopy_Uint64) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestTransposeAndCopy<uint64>(MARTe::UnsignedInteger64Bit));
}

TEST(ObjectConnectionIGTest,TestTransposeAndCopy_Int8) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestTransposeAndCopy<int8>(MARTe::SignedInteger8Bit));
}

TEST(ObjectConnectionIGTest,TestTransposeAndCopy_Int16) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestTransposeAndCopy<int16>(MARTe::SignedInteger16Bit));
}

TEST(ObjectConnectionIGTest,TestTransposeAndCopy_Int32) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestTransposeAndCopy<int32>(MARTe::SignedInteger32Bit));
}

TEST(ObjectConnectionIGTest,TestTransposeAndCopy_Int64) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestTransposeAndCopy<int64>(MARTe::SignedInteger64Bit));
}

TEST(ObjectConnectionIGTest,TestTransposeAndCopy_Float32) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestTransposeAndCopy<float32>(MARTe::Float32Bit));
}

TEST(ObjectConnectionIGTest,TestTransposeAndCopy_Float64) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestTransposeAndCopy<float64>(MARTe::Float64Bit));
}

TEST(ObjectConnectionIGTest,TestTransposeAndCopy_Failed_InvalidType) {
    ObjectConnectionITest test;
    ASSERT_TRUE(test.TestTransposeAndCopy_Failed_InvalidType());
}

