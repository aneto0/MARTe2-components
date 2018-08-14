/**
 * @file LinkDataSourceGTest.cpp
 * @brief Source file for class LinkDataSourceGTest
 * @date 23/07/2018
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
 * the class LinkDataSourceGTest (public, protected, and private). Be aware that some 
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
#include "LinkDataSourceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(LinkDataSourceGTest,TestConstructor) {
    LinkDataSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(LinkDataSourceGTest,TestInitialise) {
    LinkDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(LinkDataSourceGTest,TestInitialise_FalseNoIsWriter) {
    LinkDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoIsWriter());
}

TEST(LinkDataSourceGTest,TestInitialise_FalseNoLink) {
    LinkDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoLink());
}

TEST(LinkDataSourceGTest,TestInitialise_FalseInvalidLink) {
    LinkDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_FalseInvalidLink());
}

TEST(LinkDataSourceGTest,TestAllocateMemory) {
    LinkDataSourceTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(LinkDataSourceGTest,TestAllocateMemory_FalseDifferentSizes) {
    LinkDataSourceTest test;
    ASSERT_TRUE(test.TestAllocateMemory_FalseDifferentSizes());
}

TEST(LinkDataSourceGTest,TestPrepareNextState) {
    LinkDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(LinkDataSourceGTest,TestSynchronise) {
    LinkDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(LinkDataSourceGTest,TestGetBrokerName) {
    LinkDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

