/**
 * @file UEICircularBufferGTest.cpp
 * @brief Source file for class UEICircularBufferGTest
 * @date 22/03/2023
 * @author Xavier Ruche
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
 * the class MemoryGateGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEICircularBufferTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEICircularBufferGTest,TestConstructor) {
    UEICircularBufferTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEICircularBufferGTest,TestInitialiseBuffer) {
    UEICircularBufferTest test;
    ASSERT_TRUE(test.TestInitialiseBuffer());
}

TEST(UEICircularBufferGTest,TestInitialiseBuffer_NChannelsFail) {
    UEICircularBufferTest test;
    ASSERT_TRUE(test.TestInitialiseBuffer_NChannelsFail());
}

TEST(UEICircularBufferGTest,TestInitialiseBuffer_MapSamplesFail) {
    UEICircularBufferTest test;
    ASSERT_TRUE(test.TestInitialiseBuffer_MapSamplesFail());
}

TEST(UEICircularBufferGTest,TestInitialiseBuffer_ReadSamplesFail) {
    UEICircularBufferTest test;
    ASSERT_TRUE(test.TestInitialiseBuffer_ReadSamplesFail());
}

TEST(UEICircularBufferGTest,TestInitialiseBuffer_SampleSize) {
    UEICircularBufferTest test;
    ASSERT_TRUE(test.TestInitialiseBuffer_SampleSize());
}

TEST(UEICircularBufferGTest,TestInitialiseBuffer_nBuffersFail) {
    UEICircularBufferTest test;
    ASSERT_TRUE(test.TestInitialiseBuffer_nBuffersFail());
}

TEST(UEICircularBufferGTest,FunctionalTest_WithTimestamp) {
    UEICircularBufferTest test;
    ASSERT_TRUE(test.FunctionalTest_WithTimestamp());
}

TEST(UEICircularBufferGTest,FunctionalTest) {
    UEICircularBufferTest test;
    ASSERT_TRUE(test.FunctionalTest());
}
