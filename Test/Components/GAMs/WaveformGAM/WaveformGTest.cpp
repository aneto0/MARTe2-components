/**
 * @file WaveformGTest.cpp
 * @brief Source file for class WaveformGTest
 * @date 22/02/2018
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
 * the class WaveformGTest (public, protected, and private). Be aware that some 
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

#include "WaveformTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


using namespace MARTe;


TEST(WaveformTest, SetupNoName) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupNoName());
}

TEST(WaveformTest, SetupNoInputSignals) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupNoInputSignals());
}

TEST(WaveformTest, SetupNoOutputSignals) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupNoOutputSignals());
}

TEST(WaveformTest, SetupNoInputElements) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupNoInputElements());
}

TEST(WaveformTest, SetupWrongInputElements) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupWrongInputElements());
}

TEST(WaveformTest, SetupNoOutputElements) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupNoOutputElements());
}

TEST(WaveformTest, SetupWrongOutputElements) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupWrongOutputElements());
}

TEST(WaveformTest, SetupWrongOutputElements_2) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupWrongOutputElements_2());
}

TEST(WaveformTest, SetupWrongOutputElements_3) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupWrongOutputElements_3());
}

TEST(WaveformTest, SetupNoInputSamples) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupNoInputSamples());
}

TEST(WaveformTest, SetupWrongInputSamples) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupWrongInputSamples());
}

TEST(WaveformTest, SetupNoOutputSamples) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupNoOutputSamples());
}

TEST(WaveformTest, SetupWrongOutputSamples) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupWrongOutputSamples());
}

TEST(WaveformTest, SetupInvalitInputType) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupInvalitInputType());
}

TEST(WaveformTest, SetupInvalitOutputType) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupInvalitOutputType());
}

TEST(WaveformTest, SetupNoInputDimension) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupNoInputDimension());
}

TEST(WaveformTest, SetupWrongInputDimension) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupWrongInputDimension());
}

TEST(WaveformTest, SetupNoOutputDimension) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupNoOutputDimension());
}

TEST(WaveformTest, SetupWrongOutputDimension) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupWrongOutputDimension());
}

TEST(WaveformTest, SetupNULLInputPointer) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupNULLInputPointer());
}

TEST(WaveformTest, SetupNULLOutputPointer) {
    WaveformTest test;
    ASSERT_TRUE(test.SetupNULLOutputPointer());
}

TEST(WaveformTest, Setup) {
    WaveformTest test;
    ASSERT_TRUE(test.Setup());
}


	
