/**
 * @file EpicsInputDataSourceGTest.cpp
 * @brief Source file for class EpicsInputDataSourceGTest
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
 * the class EpicsInputDataSourceGTest (public, protected, and private). Be aware that some
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

#include "EpicsInputDataSourceTest.h"

#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(EpicsInputDataSourceGTest,TestConstructor) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

/*
TEST(EpicsInputDataSourceGTest, TestSetConfiguredDatabase) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(EpicsInputDataSourceGTest, TestSynchronise) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(EpicsInputDataSourceGTest,TestAllocateMemory) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}
*/

TEST(EpicsInputDataSourceGTest,TestGetNumberOfMemoryBuffers) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

/*

TEST(EpicsInputDataSourceGTest,TestGetSignalMemoryBuffer) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(EpicsInputDataSourceGTest,TestGetSignalMemoryBuffer_False) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer_False());
}
*/
TEST(EpicsInputDataSourceGTest,TestGetBrokerName) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}
/*
TEST(EpicsInputDataSourceGTest, TestGetInputBrokers) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}
*/
TEST(EpicsInputDataSourceGTest, TestGetOutputBrokers) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(EpicsInputDataSourceGTest, TestPrepareNextState) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

/*
TEST(EpicsInputDataSourceGTest, TestExecute) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(EpicsInputDataSourceGTest, TestExecute_Busy) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_Busy());
}

TEST(EpicsInputDataSourceGTest, TestInitialise_Default) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_Default());
}

TEST(EpicsInputDataSourceGTest, TestInitialise_Busy) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_Busy());
}

TEST(EpicsInputDataSourceGTest, TestInitialise_Empty) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_Empty());
}

TEST(EpicsInputDataSourceGTest, TestInitialise_False) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False());
}

TEST(EpicsInputDataSourceGTest, TestSetConfiguredDatabase_False_MoreThan2Signals) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThan2Signals());
}

TEST(EpicsInputDataSourceGTest, TestSetConfiguredDatabase_False_No32BitsSignal1) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_No32BitsSignal1());
}

TEST(EpicsInputDataSourceGTest, TestSetConfiguredDatabase_False_No32BitsSignal2) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_No32BitsSignal2());
}

TEST(EpicsInputDataSourceGTest, TestSetConfiguredDatabase_False_NoFrequencySet) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoFrequencySet());
}

TEST(EpicsInputDataSourceGTest, TestSetConfiguredDatabase_One_Signal_Per_GAM) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_One_Signal_Per_GAM());
}

TEST(EpicsInputDataSourceGTest, TestSetConfiguredDatabase_False_IntegerSignal1) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_IntegerSignal1());
}

TEST(EpicsInputDataSourceGTest, TestSetConfiguredDatabase_False_IntegerSignal2) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_IntegerSignal2());
}

*/

TEST(EpicsInputDataSourceGTest, Test1) {
	EpicsInputDataSourceTest test;
    ASSERT_TRUE(test.Test1<MARTe::uint32>());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

