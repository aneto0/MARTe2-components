/**
 * @file EpicsOutputDataSourceGTest.cpp
 * @brief Source file for class EpicsOutputDataSourceGTest
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
 * the class EpicsOutputDataSourceGTest (public, protected, and private). Be aware that some
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

#include "EpicsOutputDataSourceTest.h"

#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(EpicsOutputDataSourceGTest,TestConstructor) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

/*
TEST(EpicsOutputDataSourceGTest, TestSetConfiguredDatabase) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(EpicsOutputDataSourceGTest, TestSynchronise) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(EpicsOutputDataSourceGTest,TestAllocateMemory) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}
*/

TEST(EpicsOutputDataSourceGTest,TestGetNumberOfMemoryBuffers) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

/*

TEST(EpicsOutputDataSourceGTest,TestGetSignalMemoryBuffer) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(EpicsOutputDataSourceGTest,TestGetSignalMemoryBuffer_False) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer_False());
}
*/
TEST(EpicsOutputDataSourceGTest,TestGetBrokerName) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}
/*
TEST(EpicsOutputDataSourceGTest, TestGetOutputBrokers) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}
*/
TEST(EpicsOutputDataSourceGTest, TestGetOutputBrokers) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(EpicsOutputDataSourceGTest, TestPrepareNextState) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

/*
TEST(EpicsOutputDataSourceGTest, TestExecute) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(EpicsOutputDataSourceGTest, TestExecute_Busy) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_Busy());
}

TEST(EpicsOutputDataSourceGTest, TestInitialise_Default) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_Default());
}

TEST(EpicsOutputDataSourceGTest, TestInitialise_Busy) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_Busy());
}

TEST(EpicsOutputDataSourceGTest, TestInitialise_Empty) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_Empty());
}

TEST(EpicsOutputDataSourceGTest, TestInitialise_False) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False());
}

TEST(EpicsOutputDataSourceGTest, TestSetConfiguredDatabase_False_MoreThan2Signals) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_MoreThan2Signals());
}

TEST(EpicsOutputDataSourceGTest, TestSetConfiguredDatabase_False_No32BitsSignal1) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_No32BitsSignal1());
}

TEST(EpicsOutputDataSourceGTest, TestSetConfiguredDatabase_False_No32BitsSignal2) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_No32BitsSignal2());
}

TEST(EpicsOutputDataSourceGTest, TestSetConfiguredDatabase_False_NoFrequencySet) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_NoFrequencySet());
}

TEST(EpicsOutputDataSourceGTest, TestSetConfiguredDatabase_One_Signal_Per_GAM) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_One_Signal_Per_GAM());
}

TEST(EpicsOutputDataSourceGTest, TestSetConfiguredDatabase_False_IntegerSignal1) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_IntegerSignal1());
}

TEST(EpicsOutputDataSourceGTest, TestSetConfiguredDatabase_False_IntegerSignal2) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_IntegerSignal2());
}

*/

TEST(EpicsOutputDataSourceGTest, Test1) {
	EpicsOutputDataSourceTest test;
    ASSERT_TRUE(test.Test1<MARTe::uint32>());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

