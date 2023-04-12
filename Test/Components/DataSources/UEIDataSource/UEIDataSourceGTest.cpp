/**
 * @file UEIDataSourceGTest.cpp
 * @brief Source file for class UEIDataSourceGTest
 * @date 10/06/2021
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
 * the class UEIDataSourceGTest (public, protected, and private). Be aware that some 
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
#include "UEIDataSourceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEIDataSourceGTest,TestConstructor) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEIDataSourceGTest,TestInitialise) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(UEIDataSourceGTest,TestInitialise_NoDevice) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_NoDevice());
}

TEST(UEIDataSourceGTest,TestInitialise_NoMap) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_NoMap());
}

TEST(UEIDataSourceGTest,TestInitialise_InvalidDevice) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidDevice());
}

TEST(UEIDataSourceGTest,TestInitialise_InvalidMap) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidMap());
}

TEST(UEIDataSourceGTest,TestInitialise_TwoDSperMap) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_TwoDSperMap());
}

TEST(UEIDataSourceGTest,TestStartup) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup());
}

TEST(UEIDataSourceGTest,TestStartup_MismatchedSignalNumber) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup_MismatchedSignalNumber());
}

TEST(UEIDataSourceGTest,TestStartup_BadStatusSignal) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup_BadStatusSignal());
}

TEST(UEIDataSourceGTest,TestStartup_BadTimestampSignal) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup_BadTimestampSignal());
}

TEST(UEIDataSourceGTest,TestStartup_MultipleElements) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup_MultipleElements());
}

TEST(UEIDataSourceGTest,TestStartup_MultipleFunctions) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup_MultipleFunctions());
}

TEST(UEIDataSourceGTest,TestStartup_MismatchedStatusSamples) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup_MismatchedStatusSamples());
}

TEST(UEIDataSourceGTest,TestStartup_MismatchedSampleNumber) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup_MismatchedSampleNumber());
}

TEST(UEIDataSourceGTest,TestStartup_MultipleSamplesDMap) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup_MultipleSamplesDMap());
}

TEST(UEIDataSourceGTest,TestStartup_SingleSamplesVMap) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup_SingleSamplesVMap());
}

TEST(UEIDataSourceGTest,TestStartup_MultipleSamplesVMap) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup_MultipleSamplesVMap());
}

TEST(UEIDataSourceGTest,TestStartup_OutputSignals) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup_OutputSignals());
}

TEST(UEIDataSourceGTest,TestStartup_InvalidMapType) {
    UEIDataSourceTest test;
    ASSERT_TRUE(test.TestStartup_InvalidMapType());
}
