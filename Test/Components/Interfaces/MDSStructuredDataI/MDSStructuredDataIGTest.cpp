/**
 * @file MDSStructuredDataIGTest.cpp
 * @brief Source file for class MDSStructuredDataIGTest
 * @date 04/09/2018
 * @author Andre Neto
 * @author Llorenc Capella
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
 * the class MDSStructuredDataIGTest (public, protected, and private). Be aware that some
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
#include "MDSStructuredDataITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

// TEST(MDSStructuredDataITest,TestConstructor) {
// MDSStructuredDataITest test;
// ASSERT_TRUE(test.TestConstructor());
// }

TEST(MDSStructuredDataITest, TestCreateTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateTree());
}

TEST(MDSStructuredDataITest, TestCreateTree2) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateTree2());
}

TEST(MDSStructuredDataITest, TestCreateTree_force) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateTree_force());
}

TEST(MDSStructuredDataITest, TestOpentree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestOpentree());
}

TEST(MDSStructuredDataITest, TestOpentree2) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestOpentree2());
}

TEST(MDSStructuredDataITest, TestOpentree_sameTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestOpentree_sameTree());
}

TEST(MDSStructuredDataITest, TestOpentree_diffTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestOpentree_diffTree());
}

TEST(MDSStructuredDataITest, TestOpentree_NoCreate) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestOpentree_NoCreate());
}

TEST(MDSStructuredDataITest, TestSetTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestSetTree());
}

TEST(MDSStructuredDataITest, TestSetTree_openTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestSetTree_openTree());
}

TEST(MDSStructuredDataITest, TestcloseTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestcloseTree());
}

TEST(MDSStructuredDataITest, TestcloseTree_SetTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestcloseTree_SetTree());
}

TEST(MDSStructuredDataITest, TestcloseTree_noOpened) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestcloseTree_noOpened());
}

TEST(MDSStructuredDataITest, TestDelete) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestDelete());
}

TEST(MDSStructuredDataITest, TestDelete_noEditable) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestDelete_noEditable());
}

TEST(MDSStructuredDataITest, TestDelete_noOpened) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestDelete_noOpened());
}

TEST(MDSStructuredDataITest, TestDelete_noNode) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestDelete_noNode());
}

TEST(MDSStructuredDataITest, TestSetEditMode) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestSetEditMode());
}

TEST(MDSStructuredDataITest, TestSetEditMode2) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestSetEditMode2());
}

TEST(MDSStructuredDataITest, TestSetEditMode3) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestSetEditMode3());
}

TEST(MDSStructuredDataITest, TestSaveTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestSaveTree());
}

TEST(MDSStructuredDataITest, TestSaveTree_noEditable) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestSaveTree_noEditable());
}

TEST(MDSStructuredDataITest, TestSaveTree_noOpen) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestSaveTree_noOpen());
}

TEST(MDSStructuredDataITest, TestGetNumberOfChildren) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetNumberOfChildren());
}

TEST(MDSStructuredDataITest, TestGetNumberOfChildren2) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetNumberOfChildren2());
}

TEST(MDSStructuredDataITest, TestGetNumberOfChildren_closedTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetNumberOfChildren_closedTree());
}

TEST(MDSStructuredDataITest, TestGetChildName) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetChildName());
}

TEST(MDSStructuredDataITest, TestGetChildName_noExistingNode) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetChildName_noExistingNode());
}

TEST(MDSStructuredDataITest, TestGetChildName_noExistingNode2) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetChildName_noExistingNode2());
}

TEST(MDSStructuredDataITest, TestGetName) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetName());
}

TEST(MDSStructuredDataITest, TestGetName_closedTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetName_closedTree());
}

TEST(MDSStructuredDataITest, TestCreateRelative) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateRelative());
}

TEST(MDSStructuredDataITest, TestCreateRelative_NoEditable) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateRelative_NoEditable());
}

TEST(MDSStructuredDataITest, TestCreateRelative_closedTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateRelative_closedTree());
}

TEST(MDSStructuredDataITest, TestCreateAbsolute) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateAbsolute());
}

TEST(MDSStructuredDataITest, TestCreateAbsolute_noEditable) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateAbsolute_noEditable());
}

TEST(MDSStructuredDataITest, TestCreateAbsolute_closedTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateAbsolute_closedTree());
}

TEST(MDSStructuredDataITest, TestCreateAbsolute_invalidPath) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateAbsolute_invalidPath());
}

TEST(MDSStructuredDataITest, TestMoveToChild) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToChild());
}

TEST(MDSStructuredDataITest, TestMoveToChild_invalidNode) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToChild_invalidNode());
}

TEST(MDSStructuredDataITest, TestMoveToChild_ClosedTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToChild_ClosedTree());
}

TEST(MDSStructuredDataITest, TestMoveRelative) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveRelative());
}

TEST(MDSStructuredDataITest, TestMoveRelative_InvalidNode) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveRelative_InvalidNode());
}

TEST(MDSStructuredDataITest, TestMoveRelative_closedTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveRelative_closedTree());
}

TEST(MDSStructuredDataITest, TestMoveAbsolute) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveAbsolute());
}

TEST(MDSStructuredDataITest, TestMoveAbsolute_InvalidNode) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveAbsolute_InvalidNode());
}

TEST(MDSStructuredDataITest, TestMoveAbsolute_closedTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveAbsolute_closedTree());
}

TEST(MDSStructuredDataITest, TestMoveToAncestor) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToAncestor());
}

TEST(MDSStructuredDataITest, TestMoveToAncestor2) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToAncestor2());
}

TEST(MDSStructuredDataITest, TestMoveToAncestor3) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToAncestor3());
}

TEST(MDSStructuredDataITest, TestMoveToAncestor_treeClosed) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToAncestor_treeClosed());
}

TEST(MDSStructuredDataITest, TestMoveToRoot) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToRoot());
}

TEST(MDSStructuredDataITest, TestMoveToRoot_treeClosed) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToRoot_treeClosed());
}

TEST(MDSStructuredDataITest, TestCopy) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCopy());
}

TEST(MDSStructuredDataITest, TestCopy2) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCopy2());
}

TEST(MDSStructuredDataITest, TestCopy_treeClosed) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCopy_treeClosed());
}

TEST(MDSStructuredDataITest, TestCopy_treeClosed2) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCopy_treeClosed2());
}

