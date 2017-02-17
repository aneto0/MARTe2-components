/**
 * @file EpicsOutputDataSourceGTest.cpp
 * @brief Source file for class EpicsOutputDataSourceGTest
 * @date 24/01/2017
 * @author Ivan Herrero
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
 * the class EpicsOutputDataSourceGTest (public, protected, and private). Be aware that some
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

#include "EpicsOutputDataSourceTest.h"

#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(EpicsOutputDataSourceGTest,TestConstructor) {
    EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(EpicsOutputDataSourceGTest, TestSynchronise) {
    EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise<MARTe::uint32>());
    ASSERT_TRUE(test.TestSynchronise<MARTe::float32>());
}

TEST(EpicsOutputDataSourceGTest,TestAllocateMemory) {
    EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(EpicsOutputDataSourceGTest,TestGetNumberOfMemoryBuffers) {
    EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(EpicsOutputDataSourceGTest,TestGetSignalMemoryBuffer) {
    EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(EpicsOutputDataSourceGTest,TestGetBrokerName) {
    EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(EpicsOutputDataSourceGTest, TestGetInputBrokers) {
    EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(EpicsOutputDataSourceGTest, TestGetOutputBrokers) {
    EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(EpicsOutputDataSourceGTest, TestPrepareNextState) {
    EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(EpicsOutputDataSourceGTest, TestGetSharedDataAreaName) {
    EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestGetSharedDataAreaName());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
