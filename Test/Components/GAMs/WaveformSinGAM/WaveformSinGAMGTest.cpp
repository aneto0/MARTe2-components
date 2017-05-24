/**
 * @file WaveformSinGAMGTest.cpp
 * @brief Source file for class WaveformSinGAMGTest
 * @date 22/05/2017
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
 * the class WaveformSinGAMGTest (public, protected, and private). Be aware that some
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
#include "WaveformSin.h"
#include "WaveformSinGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


TEST(WaveformSinGAMTest, TestUInt8Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestUInt8Execute());
}

TEST(WaveformSinGAMTest, TestInt8Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInt8Execute());
}

TEST(WaveformSinGAMTest, TestInt16Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInt16Execute());
}

TEST(WaveformSinGAMTest, TestUInt16Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestUInt16Execute());
}

TEST(WaveformSinGAMTest, TestUInt32Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestUInt32Execute());
}

TEST(WaveformSinGAMTest, TestInt32Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInt32Execute());
}
/*
TEST(WaveformSinGAMTest, TestInt64Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestInt64Execute());
}

TEST(WaveformSinGAMTest, TestUInt64Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestUInt64Execute());
}
*/

TEST(WaveformSinGAMTest, TestFloat32Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestFloat32Execute());
}
/*
TEST(WaveformSinGAMTest, TestFloat64Execute) {
    WaveformSinGAMTest test;
    ASSERT_TRUE(test.TestFloat64Execute());
}
*/




/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
