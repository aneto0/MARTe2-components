/**
 * @file NI9157MxiDataSourceGTest.cpp
 * @brief Source file for class NI9157MxiDataSourceGTest
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class NI9157MxiDataSourceGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157MxiDataSourceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI9157MxiDataSourceGTest,TestConstructor) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI9157MxiDataSourceGTest,TestInitialise) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(NI9157MxiDataSourceGTest,TestInitialise_DefaultRunNi) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultRunNi());
}
TEST(NI9157MxiDataSourceGTest,TestInitialise_False_NoNiDev) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoNiDev());
}

TEST(NI9157MxiDataSourceGTest,TestInitialise_False_InvalidNiDevPath) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_InvalidNiDevPath());
}

TEST(NI9157MxiDataSourceGTest,TestGetBrokerName) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(NI9157MxiDataSourceGTest,TestSetConfiguredDatabase) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(NI9157MxiDataSourceGTest,TestSetConfiguredDatabase_False_InvalidLabviewVar) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidLabviewVar());
}

TEST(NI9157MxiDataSourceGTest,TestSetConfiguredDatabase_False_InvalidType) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidType());
}

TEST(NI9157MxiDataSourceGTest,TestPrepareNextState) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(NI9157MxiDataSourceGTest,TestSynchronise) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(NI9157MxiDataSourceGTest,TestAsyncRead) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestAsyncRead());
}

TEST(NI9157MxiDataSourceGTest,TestSynchronise_Oscilloscope) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise_Oscilloscope());
}

TEST(NI9157MxiDataSourceGTest,TestSynchronise_Oscilloscope_OutputFIFO) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise_Oscilloscope_OutputFIFO());
}

/*TEST(NI9157MxiDataSourceGTest,TestSynchronise_Oscilloscope_2MHz) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise_Oscilloscope_2MHz());
}*/

/*TEST(NI9157MxiDataSourceGTest,TestSynchronise_Oscilloscope_OutputFIFO_2MHz) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise_Oscilloscope_OutputFIFO_2MHz());
}*/

TEST(NI9157MxiDataSourceGTest,TestAsyncWrite) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestAsyncWrite());
}
