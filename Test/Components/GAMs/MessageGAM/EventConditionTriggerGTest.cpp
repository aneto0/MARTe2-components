/**
 * @file EventConditionTriggerGTest.cpp
 * @brief Source file for class EventConditionTriggerGTest
 * @date 19/07/2018
 * @author Giuseppe Ferro
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
 * the class EventConditionTriggerGTest (public, protected, and private). Be aware that some 
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
#include "EventConditionTriggerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(EventConditionTriggerGTest,TestConstructor) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(EventConditionTriggerGTest,TestInitialise) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(EventConditionTriggerGTest,TestInitialise_CPUMask) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestInitialise_CPUMask());
}

TEST(EventConditionTriggerGTest,TestInitialise_FalseNoEventTrigger) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoEventTrigger());
}

TEST(EventConditionTriggerGTest,TestInitialise_FalseNotOnlyMessages) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNotOnlyMessages());
}

TEST(EventConditionTriggerGTest,TestSetMetadataConfig) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestSetMetadataConfig());
}

TEST(EventConditionTriggerGTest,TestSetMetadataConfig_FalseNoMetadataFieldMatch) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestSetMetadataConfig_FalseNoMetadataFieldMatch());
}

TEST(EventConditionTriggerGTest,TestSetMetadataConfig_FalseReadFailedTypeMismatch) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestSetMetadataConfig_FalseReadFailedTypeMismatch());
}

TEST(EventConditionTriggerGTest,TestCheck) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestCheck());
}

TEST(EventConditionTriggerGTest,TestExecute_ImmediateReply) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestExecute_ImmediateReply());
}

TEST(EventConditionTriggerGTest,TestPurge) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestPurge());
}

TEST(EventConditionTriggerGTest,TestEventConditionField) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestEventConditionField());
}

TEST(EventConditionTriggerGTest,TestExecute_IndirectReply) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestExecute_IndirectReply());
}

TEST(EventConditionTriggerGTest,TestExecute_Mixed) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestExecute_Mixed());
}

TEST(EventConditionTriggerGTest,TestReplied_ImmediateReply) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestReplied_ImmediateReply());
}

TEST(EventConditionTriggerGTest,TestReplied_IndirectReply) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestReplied_IndirectReply());
}

TEST(EventConditionTriggerGTest,TestReplied_Mixed) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestReplied_Mixed());
}

TEST(EventConditionTriggerGTest,TestGetCPUMask) {
    EventConditionTriggerTest test;
    ASSERT_TRUE(test.TestGetCPUMask());
}
