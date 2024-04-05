/**
 * @file UEIMapContainerGTest.cpp
 * @brief Source file for class UEIMapContainerGTest
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
#include "UEIMapContainerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEIMapContainerGTest,TestConstructor) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEIMapContainerGTest,TestBaseImplementations_Default) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestBaseImplementations_Default());
}

TEST(UEIMapContainerGTest,TestConfigurationSetter) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestConfigurationSetter());
}

TEST(UEIMapContainerGTest,TestInitialise) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(UEIMapContainerGTest,TestInitialise_General) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_General());
}

TEST(UEIMapContainerGTest,TestInitialise_NoOutputs) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_NoOutputs());
}

TEST(UEIMapContainerGTest,TestInitialise_NoInputs) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_NoInputs());
}

TEST(UEIMapContainerGTest,TestInitialise_NoDevices) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_NoDevices());
}

TEST(UEIMapContainerGTest,TestInitialise_InvalidDevn) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidDevn());
}

TEST(UEIMapContainerGTest,TestInitialise_NoDevn) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_NoDevn());
}

TEST(UEIMapContainerGTest,TestInitialise_NegativeDevn) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_NegativeDevn());
}

TEST(UEIMapContainerGTest,TestInitialise_RepeatedDevn) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_RepeatedDevn());
}

TEST(UEIMapContainerGTest,TestInitialise_NoChannels) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_NoChannels());
}

TEST(UEIMapContainerGTest,TestInitialise_NegativeChannels) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_NegativeChannels());
}

TEST(UEIMapContainerGTest,TestInitialise_DescendingChannels) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_DescendingChannels());
}

TEST(UEIMapContainerGTest,TestInitialise_InvalidChannelType) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidChannelType());
}

TEST(UEIMapContainerGTest,TestInitialise_EmptyBlock) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_EmptyBlock());
}

TEST(UEIMapContainerGTest,TestInitialise_TooManyOutputDevices) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_TooManyOutputDevices());
}

TEST(UEIMapContainerGTest,TestInitialise_TooManyInputDevices) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_TooManyInputDevices());
}

TEST(UEIMapContainerGTest,TestInitialise_IndependentThread) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_IndependentThread());
}

TEST(UEIMapContainerGTest,TestInitialise_NoExecutionMode) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_NoExecutionMode());
}

TEST(UEIMapContainerGTest,TestInitialise_InvalidExecutionMode) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidExecutionMode());
}

TEST(UEIMapContainerGTest,TestDAQHandleSetter) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestDAQHandleSetter());
}

TEST(UEIMapContainerGTest,TestTimestampMethods) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestTimestampMethods());
}

TEST(UEIMapContainerGTest,TestDSRegistration) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestDSRegistration());
}

TEST(UEIMapContainerGTest,TestSetDevice) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestSetDevice());
}

TEST(UEIMapContainerGTest,TestSetDevices) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestSetDevices());
}

TEST(UEIMapContainerGTest,TestSignalAllowed) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestSignalAllowed());
}

TEST(UEIMapContainerGTest,TestMapCoherency) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestMapCoherency());
}

TEST(UEIMapContainerGTest,TestMapCoherency_InvalidInputChannels) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestMapCoherency_InvalidInputChannels());
}

TEST(UEIMapContainerGTest,TestMapCoherency_InvalidOutputChannels) {
    UEIMapContainerTest test;
    ASSERT_TRUE(test.TestMapCoherency_InvalidOutputChannels());
}
