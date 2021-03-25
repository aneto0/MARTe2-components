/**
 * @file ProfinetDataStructureGTest.cpp
 * @brief Source file for class ProfinetDataStructureTest
 * @date 14/01/2021
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
 * the class ProfinetDataStructureTest (public, protected, and private). Be aware that some
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
#include "ProfinetDataStructureTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(ProfinetDataStructureGTest, TestConstructor) {
    ProfinetDataStructureTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(ProfinetDataStructureGTest, TestInsertHead) {
    ProfinetDataStructureTest test;
    ASSERT_TRUE(test.TestInsertHead());
}

TEST(ProfinetDataStructureGTest, TestInsertTail) {
    ProfinetDataStructureTest test;
    ASSERT_TRUE(test.TestInsertTail());
}

TEST(ProfinetDataStructureGTest, TestRemoveHead) {
    ProfinetDataStructureTest test;
    ASSERT_TRUE(test.TestRemoveHead());
}

TEST(ProfinetDataStructureGTest, TestRemoveTail) {
    ProfinetDataStructureTest test;
    ASSERT_TRUE(test.TestRemoveTail());
}

TEST(ProfinetDataStructureGTest, TestEmptyLinkedList) {
    ProfinetDataStructureTest test;
    ASSERT_TRUE(test.TestEmptyLinkedList());
}

TEST(ProfinetDataStructureGTest, TestLinkedListStatus) {
    ProfinetDataStructureTest test;
    ASSERT_TRUE(test.TestLinkedListStatus());
}

TEST(ProfinetDataStructureGTest, TestGetIterator) {
    ProfinetDataStructureTest test;
    ASSERT_TRUE(test.TestGetIterator());
}

TEST(ProfinetDataStructureGTest, TestFirst) {
    ProfinetDataStructureTest test;
    ASSERT_TRUE(test.TestFirst());
}

TEST(ProfinetDataStructureGTest, TestNext) {
    ProfinetDataStructureTest test;
    ASSERT_TRUE(test.TestNext());
}

TEST(ProfinetDataStructureGTest, TestLast) {
    ProfinetDataStructureTest test;
    ASSERT_TRUE(test.TestLast());
}

TEST(ProfinetDataStructureGTest, TestPrevious) {
    ProfinetDataStructureTest test;
    ASSERT_TRUE(test.TestPrevious());
}
