/**
 * @file NI9157DeviceGTest.cpp
 * @brief Source file for class NI9157DeviceGTest
 * @date 22/05/2018
 * @author Giuseppe Ferrò
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
 * the class NI9157DeviceGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "NI9157DeviceTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI9157DeviceGTest,TestConstructor) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI9157DeviceGTest,TestInitialise) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(NI9157DeviceGTest,TestInitialiseIsOpened) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestInitialiseIsOpened());
}

TEST(NI9157DeviceGTest,TestInitialiseRandomConfig) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestInitialiseRandomConfig());
}

TEST(NI9157DeviceGTest,TestInitialise_FalseNoDeviceName) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoDeviceName());
}

TEST(NI9157DeviceGTest,TestInitialise_FalseNoGenFile) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoGenFile());
}

TEST(NI9157DeviceGTest,TestInitialise_FalseNoGenSignature) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoGenSignature());
}

TEST(NI9157DeviceGTest,TestInitialise_FalseNoType) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoType());
}

TEST(NI9157DeviceGTest,TestInitialise_FalseVariableNotFound) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestInitialise_FalseVariableNotFound());
}

TEST(NI9157DeviceGTest,TestOpen) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestOpen());
}

TEST(NI9157DeviceGTest,TestRun) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestRun());
}

TEST(NI9157DeviceGTest,TestIsOpened) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestIsOpened());
}

TEST(NI9157DeviceGTest,TestIsRunning) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestIsRunning());
}

TEST(NI9157DeviceGTest,TestGetSession) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestGetSession());
}

TEST(NI9157DeviceGTest,TestReset) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestReset());
}

TEST(NI9157DeviceGTest,TestClose) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestClose());
}

TEST(NI9157DeviceGTest,TestFindResource) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestFindResource());
}

TEST(NI9157DeviceGTest,TestNiRead) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestNiRead());
}

TEST(NI9157DeviceGTest,TestNiWrite) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestNiWrite());
}

TEST(NI9157DeviceGTest,TestNiConfigureFifo) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestNiConfigureFifo());
}

TEST(NI9157DeviceGTest,TestNiStartFifo) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestNiStartFifo());
}

TEST(NI9157DeviceGTest,TestNiStopFifo) {
    NI9157DeviceTest test;
    ASSERT_TRUE(test.TestNiStopFifo());
}
