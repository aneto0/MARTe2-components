/**
 * @file UEIRtVMapGTest.cpp
 * @brief Source file for class UEIRtVMapGTest
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
#include "UEIRtVMapTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEIRtVMapGTest,TestConstructor) {
    UEIRtVMapTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEIRtVMapGTest,TestInitialise) {
    UEIRtVMapTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(UEIRtVMapGTest,TestInitialise_TrueClass) {
    UEIRtVMapTest test;
    ASSERT_TRUE(test.TestInitialise_TrueClass());
}

TEST(UEIRtVMapGTest,TestInitialiseNoSamples) {
    UEIRtVMapTest test;
    ASSERT_TRUE(test.TestInitialiseNoSamples());
}

TEST(UEIRtVMapGTest,TestInitialiseNumberOfBuffers) {
    UEIRtVMapTest test;
    ASSERT_TRUE(test.TestInitialiseNumberOfBuffers());
}

TEST(UEIRtVMapGTest,TestConfigureInputsForDataSource) {
    UEIRtVMapTest test;
    ASSERT_TRUE(test.TestConfigureInputsForDataSource());
}

TEST(UEIRtVMapGTest,TestEnableMap) {
    UEIRtVMapTest test;
    ASSERT_TRUE(test.TestEnableMap());
}

TEST(UEIRtVMapGTest,TestResetMap) {
    UEIRtVMapTest test;
    ASSERT_TRUE(test.TestResetMap());
}

TEST(UEIRtVMapGTest,TestStartMap) {
    UEIRtVMapTest test;
    ASSERT_TRUE(test.TestStartMap());
}


