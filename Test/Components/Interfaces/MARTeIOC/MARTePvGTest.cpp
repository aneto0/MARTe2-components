/**
 * @file MARTePvGTest.cpp
 * @brief Source file for class MARTePvGTest
 * @date 26/08/2019
 * @author Giuseppe Ferro
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
 * the class MARTePvGTest (public, protected, and private). Be aware that some 
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

#include "MARTePvTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(MARTePvGTest,TestInitialise) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestInitialise());
}

TEST(MARTePvGTest,TestInitialise_DefaultLen) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestInitialise_DefaultLen());
}

TEST(MARTePvGTest,TestInitialise_FalseUndefinedType) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestInitialise_FalseUndefinedType());
}

TEST(MARTePvGTest,TestinterestRegister) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestinterestRegister());
}

TEST(MARTePvGTest,TestinterestDelete) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestinterestDelete());
}

TEST(MARTePvGTest,TestbestExternalType) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestbestExternalType());
}

TEST(MARTePvGTest,TestGetScanPeriod) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestGetScanPeriod());
}

TEST(MARTePvGTest,Testdestroy) {
   MARTePvTest test;
   ASSERT_TRUE(test.Testdestroy());
}

TEST(MARTePvGTest,TestgetName) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestgetName());
}

TEST(MARTePvGTest,TestcreateChannel) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestcreateChannel());
}

TEST(MARTePvGTest,TestSetIocServer) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestSetIocServer());
}

TEST(MARTePvGTest,TestGetNumberOfElements) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestGetNumberOfElements());
}

TEST(MARTePvGTest,TestmaxDimension) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestmaxDimension());
}

TEST(MARTePvGTest,TestmaxBound) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestmaxBound());
}

TEST(MARTePvGTest,TestSetUseLocalTimeStamp) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestSetUseLocalTimeStamp());
}

TEST(MARTePvGTest,TestUseLocalTimeStamp) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestUseLocalTimeStamp());
}

TEST(MARTePvGTest,TestSetTimeStamp) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestSetTimeStamp());
}

TEST(MARTePvGTest,TestGddToAnyTypeConverter_uint8) {
   MARTePvTest test;
   uint8 x=1u;
   ASSERT_TRUE(test.TestGddToAnyTypeConverter(x));
}

TEST(MARTePvGTest,TestGddToAnyTypeConverter_int8) {
   MARTePvTest test;
   int8 x=-1;
   ASSERT_TRUE(test.TestGddToAnyTypeConverter(x));
}

TEST(MARTePvGTest,TestGddToAnyTypeConverter_uint16) {
   MARTePvTest test;
   uint16 x=1u;
   ASSERT_TRUE(test.TestGddToAnyTypeConverter(x));
}

TEST(MARTePvGTest,TestGddToAnyTypeConverter_int16) {
   MARTePvTest test;
   int16 x=-1;
   ASSERT_TRUE(test.TestGddToAnyTypeConverter(x));
}

TEST(MARTePvGTest,TestGddToAnyTypeConverter_uint32) {
   MARTePvTest test;
   uint32 x=1u;
   ASSERT_TRUE(test.TestGddToAnyTypeConverter(x));
}

TEST(MARTePvGTest,TestGddToAnyTypeConverter_int32) {
   MARTePvTest test;
   int32 x=-1;
   ASSERT_TRUE(test.TestGddToAnyTypeConverter(x));
}

TEST(MARTePvGTest,TestGddToAnyTypeConverter_float32) {
   MARTePvTest test;
   float32 x=1;
   ASSERT_TRUE(test.TestGddToAnyTypeConverter(x));
}

TEST(MARTePvGTest,TestGddToAnyTypeConverter_float64) {
   MARTePvTest test;
   float64 x=-1;
   ASSERT_TRUE(test.TestGddToAnyTypeConverter(x));
}

TEST(MARTePvGTest,TestGddToAnyTypeConverter_created) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestGddToAnyTypeConverter_created());
}

TEST(MARTePvGTest,TestConvertToTypeDescriptor) {
   MARTePvTest test;
   ASSERT_TRUE(test.TestConvertToTypeDescriptor());
}


TEST(MARTePvGTest,TestGetAnyType_uint8) {
   MARTePvTest test;
   uint8 x=1;
   ASSERT_TRUE(test.TestGetAnyType(x));
}

TEST(MARTePvGTest,TestGetAnyType_int8) {
   MARTePvTest test;
   int8 x=1;
   ASSERT_TRUE(test.TestGetAnyType(x));
}

TEST(MARTePvGTest,TestGetAnyType_uint16) {
   MARTePvTest test;
   uint16 x=1;
   ASSERT_TRUE(test.TestGetAnyType(x));
}

TEST(MARTePvGTest,TestGetAnyType_int16) {
   MARTePvTest test;
   int16 x=1;
   ASSERT_TRUE(test.TestGetAnyType(x));
}

TEST(MARTePvGTest,TestGetAnyType_uint32) {
   MARTePvTest test;
   uint32 x=1;
   ASSERT_TRUE(test.TestGetAnyType(x));
}

TEST(MARTePvGTest,TestGetAnyType_int32) {
   MARTePvTest test;
   int32 x=1;
   ASSERT_TRUE(test.TestGetAnyType(x));
}

TEST(MARTePvGTest,TestGetAnyType_float32) {
   MARTePvTest test;
   float32 x=1;
   ASSERT_TRUE(test.TestGetAnyType(x));
}

TEST(MARTePvGTest,TestGetAnyType_float64) {
   MARTePvTest test;
   float64 x=1;
   ASSERT_TRUE(test.TestGetAnyType(x));
}



