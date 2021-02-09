/**
 * @file ProfinetDataSourceGTest.cpp
 * @brief Source file for class ProfinetDataSourceGTest
 * @date 21/01/2021
 * @author Giuseppe Avon
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
 * the class ProfinetDataSourceGTest (public, protected, and private). Be aware that some
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
#include "ProfinetDataSourceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ProfinetDataSourceGTest, TestStandaloneInitialise) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialise());
}

TEST(ProfinetDataSourceGTest, TestStandaloneInitialiseFailBaseNet) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialiseFailBaseNet());
}

TEST(ProfinetDataSourceGTest, TestStandaloneInitialiseFailBaseId) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialiseFailBaseId());
}

TEST(ProfinetDataSourceGTest, TestStandaloneInitialiseFailInM) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialiseFailInM());
}

TEST(ProfinetDataSourceGTest, TestStandaloneInitialiseNoSlots) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialiseNoSlots());
}

TEST(ProfinetDataSourceGTest, TestStandaloneInitialiseWrongSlots) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialiseWrongSlots());
}

TEST(ProfinetDataSourceGTest, TestStandaloneInitialiseNoSubslots) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialiseNoSubslots());
}

TEST(ProfinetDataSourceGTest, TestStandaloneInitialiseWrongDirection) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialiseWrongDirection());
}


TEST(ProfinetDataSourceGTest, TestStandaloneInitialiseWrongCoordinates) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialiseWrongCoordinates());
}

TEST(ProfinetDataSourceGTest, TestStandaloneInitialiseFailSlotsSection) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialiseFailSlotsSection());
}

TEST(ProfinetDataSourceGTest, TestStandaloneInitialiseFailSignalsSection) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialiseFailSignalsSection());
}

TEST(ProfinetDataSourceGTest, TestStandaloneInitialiseFailTimerHelperSection) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialiseFailTimerHelperSection());
}

TEST(ProfinetDataSourceGTest, TestStandaloneInitialiseFailMainHelperSection) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneInitialiseFailMainHelperSection());
}

TEST(ProfinetDataSourceGTest, TestStandaloneSweepSoftwareRevision) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestStandaloneSweepSoftwareRevision());
}

TEST(ProfinetDataSourceGTest, TestIntegratedRunProfinetSlave) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestIntegratedRunProfinetSlave());
}

TEST(ProfinetDataSourceGTest, TestIntegratedRunPLCLoopback) {
    ProfinetDataSourceTest* test = new ProfinetDataSourceTest();
    bool temp = test->TestIntegratedRunPLCLoopback();
    delete test;
    ASSERT_TRUE(temp);
}

TEST(ProfinetDataSourceGTest, TestIntegratedRunFailModulePlugging) {
    ProfinetDataSourceTest test;
    ASSERT_TRUE(test.TestIntegratedRunFailModulePlugging());
}
