/**
 * @file UEIAI217_803GTest.cpp
 * @brief Source file for class UEIAI217_803GTest
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
#include "UEIAI217_803Test.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEIAI217_803GTest,TestConstructor) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEIAI217_803GTest,TestFixedParameters) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestFixedParameters());
}

TEST(UEIAI217_803GTest,TestAcceptedSignalType) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestAcceptedSignalType());
}

TEST(UEIAI217_803GTest,TestInitialise) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(UEIAI217_803GTest,TestInitialise_class) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_class());
}

TEST(UEIAI217_803GTest,TestInitialise_NoGains) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_NoGains());
}

TEST(UEIAI217_803GTest,TestInitialise_InvalidGainNumber) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_InvalidGainNumber());
}

TEST(UEIAI217_803GTest,TestInitialise_InvalidGains) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_InvalidGains());
}

TEST(UEIAI217_803GTest,TestInitialise_ADCModeEnhanced) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_ADCModeEnhanced());
}

TEST(UEIAI217_803GTest,TestInitialise_ADCModeDefault) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_ADCModeDefault());
}

TEST(UEIAI217_803GTest,TestInitialise_InvalidADCMode) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_InvalidADCMode());
}

TEST(UEIAI217_803GTest,TestInitialise_NoADCMode) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_NoADCMode());
}

TEST(UEIAI217_803GTest,TestInitialise_NoFIRBanks) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_NoFIRBanks());
}

TEST(UEIAI217_803GTest,TestInitialise_NoFIRBankDefined) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_NoFIRBankDefined());
}

TEST(UEIAI217_803GTest,TestInitialise_ManyFIRTaps) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_ManyFIRTaps());
}

TEST(UEIAI217_803GTest,TestInitialise_InvalidDefaultSetting) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_InvalidDefaultSetting());
}

TEST(UEIAI217_803GTest,TestInitialise_InvalidFIRtSetting_CustomAndDefault) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestInitialise_InvalidFIRtSetting_CustomAndDefault());
}

TEST(UEIAI217_803GTest,TestCheckChannelAndDirection) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestCheckChannelAndDirection());
}

TEST(UEIAI217_803GTest,TestConfigureChannel) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestConfigureChannel());
}

TEST(UEIAI217_803GTest,TestSignalScaling) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestSignalScaling());
}

TEST(UEIAI217_803GTest,TestConfigureDevice) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestConfigureDevice());
}

TEST(UEIAI217_803GTest,TestConfigureDevice_SetCfgLayerError) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestConfigureDevice_SetCfgLayerError());
}

TEST(UEIAI217_803GTest,TestConfigureDevice_SetDefaultError) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestConfigureDevice_SetDefaultError());
}

TEST(UEIAI217_803GTest,TestConfigureDevice_SetBank1Error) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestConfigureDevice_SetBank1Error());
}

TEST(UEIAI217_803GTest,TestConfigureDevice_FIRTapsWarning) {
    UEIAI217_803Test test;
    ASSERT_TRUE(test.TestConfigureDevice_FIRTapsWarning());
}

