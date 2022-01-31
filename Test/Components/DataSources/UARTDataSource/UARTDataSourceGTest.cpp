/**
 * @file UARTDataSourceGTest.cpp
 * @brief Source file for class UARTDataSourceGTest
 * @date 07/08/2018
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
 * the class UARTDataSourceTestGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
#include "UARTDataSourceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(UARTDataSourceTestGTest,TestConstructor) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UARTDataSourceTestGTest,TestCRIOThreadCallback) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestCRIOThreadCallback());
}

TEST(UARTDataSourceTestGTest,TestCRIOThreadCallback_FailedRead) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestCRIOThreadCallback_FailedRead());
}

TEST(UARTDataSourceTestGTest,TestGetBrokerName) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(UARTDataSourceTestGTest,TestGetInputOffset) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestGetInputOffset());
}

TEST(UARTDataSourceTestGTest,TestInitialise) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(UARTDataSourceTestGTest,TestInitialise_False_Baud) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_Baud());
}

TEST(UARTDataSourceTestGTest,TestInitialise_TimestampProvider) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_TimestampProvider());
}

TEST(UARTDataSourceTestGTest,TestInitialise_TooManyTimestampProviders) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_TooManyTimestampProviders());
}

TEST(UARTDataSourceTestGTest,TestInitialise_False_BaudRate) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_BaudRate());
}

TEST(UARTDataSourceTestGTest,TestInitialise_False_Port) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_Port());
}

TEST(UARTDataSourceTestGTest,TestInitialise_False_PortName) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_PortName());
}

TEST(UARTDataSourceTestGTest,TestInitialise_False_Timeout) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_Timeout());
}

TEST(UARTDataSourceTestGTest,TestInitialise_False_SerialTimeout) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_SerialTimeout());
}

TEST(UARTDataSourceTestGTest,TestPrepareInputOffsets) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareInputOffsets());
}

TEST(UARTDataSourceTestGTest,TestPrepareNextState) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(UARTDataSourceTestGTest,TestSetConfiguredDatabase) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(UARTDataSourceTestGTest,TestSetConfiguredDatabase_False_Signal1_Not_UInt8) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_Signal1_Not_UInt8());
}

TEST(UARTDataSourceTestGTest,TestSetConfiguredDatabase_False_Signal1_Not_1_Element) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_Signal1_Not_1_Element());
}

TEST(UARTDataSourceTestGTest,TestSetConfiguredDatabase_False_Signal2_Not_UInt64) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_Signal2_Not_UInt64());
}

TEST(UARTDataSourceTestGTest,TestSetConfiguredDatabase_False_3_Signals) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_3_Signals());
}

TEST(UARTDataSourceTestGTest,TestSynchronise) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(UARTDataSourceTestGTest,TestTerminateInputCopy) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestTerminateInputCopy());
}

TEST(UARTDataSourceTestGTest,TestStopAcquisition) {
    UARTDataSourceTest test;
    ASSERT_TRUE(test.TestStopAcquisition());
}
