/**
 * @file UEIReaderGTest.cpp
 * @brief Source file for class UEIReaderGTest
 * @date 02/05/2023
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
 * the class UEIReaderGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEIReaderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEIReaderGTest,TestConstructor) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEIReaderGTest,TestInitialise) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(UEIReaderGTest,TestInitialise_TrueClass) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestInitialise_TrueClass());
}

TEST(UEIReaderGTest,TestInitialise_TwoDsPerMap) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestInitialise_TwoDsPerMap());
}

TEST(UEIReaderGTest,TestSimpleMethods) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestSimpleMethods());
}

TEST(UEIReaderGTest,TestStartup) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestStartup());
}

TEST(UEIReaderGTest,TestStartup_InvalidChannelNumber) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestStartup_InvalidChannelNumber());
}

TEST(UEIReaderGTest,TestStartup_InvalidStatusChannel) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestStartup_InvalidStatusChannel());
}

TEST(UEIReaderGTest,TestStartup_InvalidTimestampChannel) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestStartup_InvalidTimestampChannel());
}

TEST(UEIReaderGTest,TestStartup_InvalidNumberOfElements) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestStartup_InvalidNumberOfElements());
}

TEST(UEIReaderGTest,TestStartup_InvalidNumberOfDimensions) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestStartup_InvalidNumberOfDimensions());
}

TEST(UEIReaderGTest,TestStartup_MultipleSamplesForStatus) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestStartup_MultipleSamplesForStatus());
}

TEST(UEIReaderGTest,TestStartup_MismatchedSampleNumber) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestStartup_MismatchedSampleNumber());
}

TEST(UEIReaderGTest,TestStartup_MultipleDMapSamples) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestStartup_MultipleDMapSamples());
}

TEST(UEIReaderGTest,TestSync) {
    UEIReaderTest test;
    ASSERT_TRUE(test.TestSync());
}

