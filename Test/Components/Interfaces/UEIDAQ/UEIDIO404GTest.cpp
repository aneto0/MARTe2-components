/**
 * @file UEIDIO404GTest.cpp
 * @brief Source file for class UEIDIO404GTest
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
#include "UEIDIO404Test.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEIDIO404GTest,TestConstructor) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEIDIO404GTest,TestFixedParameters) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestFixedParameters());
}

TEST(UEIDIO404GTest,TestAcceptedSignalType) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestAcceptedSignalType());
}

TEST(UEIDIO404GTest,TestInitialise) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(UEIDIO404GTest,TestInitialise_InvalidVoltageReference) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestInitialise_InvalidVoltageReference());
}

TEST(UEIDIO404GTest,TestInitialise_NoVoltageReference) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestInitialise_NoVoltageReference());
}

TEST(UEIDIO404GTest,TestInitialise_IncompleteHysteresis) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestInitialise_IncompleteHysteresis());
}

TEST(UEIDIO404GTest,TestInitialise_TooLowHysteresis) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestInitialise_TooLowHysteresis());
}

TEST(UEIDIO404GTest,TestInitialise_TooHighHysteresis) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestInitialise_TooHighHysteresis());
}

TEST(UEIDIO404GTest,TestInitialise_InvalidHysteresis) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestInitialise_InvalidHysteresis());
}

TEST(UEIDIO404GTest,TestInitialise_ValidHysteresis) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestInitialise_ValidHysteresis());
}

TEST(UEIDIO404GTest,TestInitialise_class) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestInitialise_class());
}


TEST(UEIDIO404GTest,TestCheckChannelAndDirection) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestCheckChannelAndDirection());
}

TEST(UEIDIO404GTest,TestConfigureChannel) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestConfigureChannel());
}

TEST(UEIDIO404GTest,TestConfigureDevice) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestConfigureDevice());
}

TEST(UEIDIO404GTest,TestSignalScaling) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestSignalScaling());
}

TEST(UEIDIO404GTest,TestOutputSignalSetter) {
    UEIDIO404Test test;
    ASSERT_TRUE(test.TestOutputSignalSetter());
}
