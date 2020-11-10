/**
 * @file LoggerDataSourceGTest.cpp
 * @brief Source file for class LoggerDataSourceGTest
 * @date 25/10/2016
 * @author Andre Neto
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
 * the class LoggerDataSourceGTest (public, protected, and private). Be aware that some
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
#include "LoggerDataSourceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(LoggerDataSourceGTest,TestConstructor) {
    LoggerDataSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(LoggerDataSourceGTest,TestAllocateMemory) {
    LoggerDataSourceTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(LoggerDataSourceGTest,TestGetBrokerName) {
    LoggerDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(LoggerDataSourceGTest,TestGetInputBrokers) {
    LoggerDataSourceTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(LoggerDataSourceGTest,TestGetNumberOfMemoryBuffers) {
    LoggerDataSourceTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(LoggerDataSourceGTest,TestGetOutputBrokers) {
    LoggerDataSourceTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(LoggerDataSourceGTest,TestGetSignalMemoryBuffer) {
    LoggerDataSourceTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(LoggerDataSourceGTest,TestPrepareNextState) {
    LoggerDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(LoggerDataSourceGTest,TestSynchronise) {
    LoggerDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(LoggerDataSourceGTest,TestInitialise) {
    LoggerDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise());
}



/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

