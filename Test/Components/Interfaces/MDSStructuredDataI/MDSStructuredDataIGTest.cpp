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

TEST(MDSStructuredDataITest, TestGetChildName_setAndDestroy) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetChildName_setAndDestroy());
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

TEST(MDSStructuredDataITest, TestCreateRelative2) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateRelative2());
}

TEST(MDSStructuredDataITest, TestCreateRelative3) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateRelative3());
}

TEST(MDSStructuredDataITest, TestCreateRelative_AllNodesExist) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateRelative_AllNodesExist());
}

TEST(MDSStructuredDataITest, TestCreateRelative_NoEditable) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateRelative_NoEditable());
}

TEST(MDSStructuredDataITest, TestCreateRelative_closedTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateRelative_closedTree());
}

TEST(MDSStructuredDataITest, TestCreateRelative_setTreeAndDestroy) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateRelative_setTreeAndDestroy());
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

TEST(MDSStructuredDataITest, TestCreateAbsolute_AllNodesExist) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateAbsolute_AllNodesExist());
}

TEST(MDSStructuredDataITest, TestCreateAbsolute_setTreeAndDestroy) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCreateAbsolute_setTreeAndDestroy());
}

TEST(MDSStructuredDataITest, TestMoveToChild) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToChild());
}

TEST(MDSStructuredDataITest, TestMoveToChild2) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToChild2());
}

TEST(MDSStructuredDataITest, TestMoveToChild_invalidNode) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToChild_invalidNode());
}

TEST(MDSStructuredDataITest, TestGetChildName_treeClosed) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetChildName_treeClosed());
}

TEST(MDSStructuredDataITest, TestMoveToChild_ClosedTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToChild_ClosedTree());
}

TEST(MDSStructuredDataITest, TestMoveToChild_setTreeAndDestroy) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToChild_setTreeAndDestroy());
}

TEST(MDSStructuredDataITest, TestMoveRelative) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveRelative());
}

TEST(MDSStructuredDataITest, TestMoveRelative2) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveRelative2());
}

TEST(MDSStructuredDataITest, TestMoveRelative_InvalidNode) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveRelative_InvalidNode());
}

TEST(MDSStructuredDataITest, TestMoveRelative_closedTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveRelative_closedTree());
}

TEST(MDSStructuredDataITest, TestMoveRelative_setTreeAndDestroy) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveRelative_setTreeAndDestroy());
}

TEST(MDSStructuredDataITest, TestMoveAbsolute) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveAbsolute());
}

TEST(MDSStructuredDataITest, TestMoveAbsolute2) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveAbsolute2());
}

TEST(MDSStructuredDataITest, TestMoveAbsolute_InvalidNode) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveAbsolute_InvalidNode());
}

TEST(MDSStructuredDataITest, TestMoveAbsolute_closedTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveAbsolute_closedTree());
}

TEST(MDSStructuredDataITest, TestMoveAbsolute_setTreeAndDestroy) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveAbsolute_setTreeAndDestroy());
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

TEST(MDSStructuredDataITest, TestMoveToAncestor4) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToAncestor4());
}

TEST(MDSStructuredDataITest, TestMoveToAncestor_treeClosed) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToAncestor_treeClosed());
}

TEST(MDSStructuredDataITest, TestMoveToAncestor_setTreeAndDestroy) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToAncestor_setTreeAndDestroy());
}

TEST(MDSStructuredDataITest, TestMoveToRoot) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToRoot());
}

TEST(MDSStructuredDataITest, TestMoveToRoot_treeClosed) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToRoot_treeClosed());
}

TEST(MDSStructuredDataITest, TestMoveToRoot_setTreeAndDestroy) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestMoveToRoot_setTreeAndDestroy());
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

TEST(MDSStructuredDataITest, TestWriteUint8) {
    MDSStructuredDataITest test;
    MARTe::uint8 val = 3;
    ASSERT_TRUE(test.TestWrite(val));
}

TEST(MDSStructuredDataITest, TestWriteInt8) {
    MDSStructuredDataITest test;
    MARTe::int8 val = 3;
    ASSERT_TRUE(test.TestWrite(val));
}

TEST(MDSStructuredDataITest, TestWriteUInt16) {
    MDSStructuredDataITest test;
    MARTe::uint16 val = 3;
    ASSERT_TRUE(test.TestWrite(val));
}

TEST(MDSStructuredDataITest, TestWriteInt16) {
    MDSStructuredDataITest test;
    MARTe::int16 val = 3;
    ASSERT_TRUE(test.TestWrite(val));
}

TEST(MDSStructuredDataITest, TestWriteUInt32) {
    MDSStructuredDataITest test;
    MARTe::uint32 val = 3;
    ASSERT_TRUE(test.TestWrite(val));
}

TEST(MDSStructuredDataITest, TestWriteInt32) {
    MDSStructuredDataITest test;
    MARTe::int32 val = 3;
    ASSERT_TRUE(test.TestWrite(val));
}

TEST(MDSStructuredDataITest, TestWriteUInt64) {
    MDSStructuredDataITest test;
    MARTe::uint64 val = 3;
    ASSERT_TRUE(test.TestWrite(val));
}

TEST(MDSStructuredDataITest, TestWriteFloat32) {
    MDSStructuredDataITest test;
    MARTe::float32 val = 3.0;
    ASSERT_TRUE(test.TestWrite(val));
}

TEST(MDSStructuredDataITest, TestWriteFloat64) {
    MDSStructuredDataITest test;
    MARTe::float64 val = 3.0;
    ASSERT_TRUE(test.TestWrite(val));
}

TEST(MDSStructuredDataITest, TestWrite_string) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestWrite_string());
}

TEST(MDSStructuredDataITest, TestWrite_constString) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestWrite_constString());
}

TEST(MDSStructuredDataITest, TestWrite_char) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestWrite_char());
}

TEST(MDSStructuredDataITest, TestWrite_charArray) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestWrite_charArray());
}

TEST(MDSStructuredDataITest, TestWrite_Recursively) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestWrite_Recursively());
}

TEST(MDSStructuredDataITest, TestWrite_noEditable) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestWrite_noEditable());
}

TEST(MDSStructuredDataITest, TestWrite_InvalidType) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestWrite_InvalidType());
}

TEST(MDSStructuredDataITest, TestWrite_treeClosed) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestWrite_treeClosed());
}

TEST(MDSStructuredDataITest, TestWrite_setTreeAndDestroy) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestWrite_setTreeAndDestroy());
}

TEST(MDSStructuredDataITest, TestGetType_Recursively) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetType_Recursively());
}

TEST(MDSStructuredDataITest, TestUInt8GetType) {
    MDSStructuredDataITest test;
    MARTe::uint8 val = 12;
    ASSERT_TRUE(test.TestGetType(val));
}

TEST(MDSStructuredDataITest, TestInt8GetType) {
    MDSStructuredDataITest test;
    MARTe::int8 val = 12;
    ASSERT_TRUE(test.TestGetType(val));
}

TEST(MDSStructuredDataITest, TestUInt16GetType) {
    MDSStructuredDataITest test;
    MARTe::uint16 val = 12;
    ASSERT_TRUE(test.TestGetType(val));
}

TEST(MDSStructuredDataITest, TestInt16GetType) {
    MDSStructuredDataITest test;
    MARTe::int16 val = 12;
    ASSERT_TRUE(test.TestGetType(val));
}

TEST(MDSStructuredDataITest, TestUInt32GetType) {
    MDSStructuredDataITest test;
    MARTe::uint32 val = 12;
    ASSERT_TRUE(test.TestGetType(val));
}

TEST(MDSStructuredDataITest, TestInt32GetType) {
    MDSStructuredDataITest test;
    MARTe::int32 val = 12;
    ASSERT_TRUE(test.TestGetType(val));
}

TEST(MDSStructuredDataITest, TestUInt64GetType) {
    MDSStructuredDataITest test;
    MARTe::uint64 val = 12;
    ASSERT_TRUE(test.TestGetType(val));
}

TEST(MDSStructuredDataITest, TestInt64GetType) {
    MDSStructuredDataITest test;
    MARTe::int64 val = 12;
    ASSERT_TRUE(test.TestGetType(val));
}

TEST(MDSStructuredDataITest, TestFloat32GetType) {
    MDSStructuredDataITest test;
    MARTe::float32 val = 12;
    ASSERT_TRUE(test.TestGetType(val));
}

TEST(MDSStructuredDataITest, TestFloat64GetType) {
    MDSStructuredDataITest test;
    MARTe::float64 val = 12;
    ASSERT_TRUE(test.TestGetType(val));
}

TEST(MDSStructuredDataITest, TestGetType_treeClosed) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetType_treeClosed());
}

TEST(MDSStructuredDataITest, TestGetType_setTreeAndDestroy) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestGetType_setTreeAndDestroy());
}

TEST(MDSStructuredDataITest, TestUint8ArrRead) {
    MDSStructuredDataITest test;
    MARTe::uint8 val[2] = { 12, 3 };
    MARTe::Vector<MARTe::uint8> valVec(val, 2);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, Testint8ArrRead) {
    MDSStructuredDataITest test;
    MARTe::int8 val[2] = { 12, 3 };
    MARTe::Vector<MARTe::int8> valVec(val, 2);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, TestUint16ArrRead) {
    MDSStructuredDataITest test;
    MARTe::uint16 val[2] = { 12, 3 };
    MARTe::Vector<MARTe::uint16> valVec(val, 2);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, Testint16ArrRead) {
    MDSStructuredDataITest test;
    MARTe::int16 val[2] = { 12, 3 };
    MARTe::Vector<MARTe::int16> valVec(val, 2);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, TestUint32ArrRead) {
    MDSStructuredDataITest test;
    MARTe::uint32 val[2] = { 12, 3 };
    MARTe::Vector<MARTe::uint32> valVec(val, 2);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, Testint32ArrRead) {
    MDSStructuredDataITest test;
    MARTe::int32 val[2] = { 12, 3 };
    MARTe::Vector<MARTe::int32> valVec(val, 2);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, TestUint64ArrRead) {
    MDSStructuredDataITest test;
    MARTe::uint64 val[2] = { 12, 3 };
    MARTe::Vector<MARTe::uint64> valVec(val, 2);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, Testint64ArrRead) {
    MDSStructuredDataITest test;
    MARTe::int64 val[2] = { 12, 3 };
    MARTe::Vector<MARTe::int64> valVec(val, 2);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, TestFloat32ArrRead) {
    MDSStructuredDataITest test;
    MARTe::float32 val[2] = { 12, 3 };
    MARTe::Vector<MARTe::float32> valVec(val, 2);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, TestFloat64ArrRead) {
    MDSStructuredDataITest test;
    MARTe::float64 val[2] = { 12, 3 };
    MARTe::Vector<MARTe::float64> valVec(val, 2);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, TestUint8Arr1Read) {
    MDSStructuredDataITest test;
    MARTe::uint8 val[1] = { 12 };
    MARTe::Vector<MARTe::uint8> valVec(val, 1);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, Testint8Arr1Read) {
    MDSStructuredDataITest test;
    MARTe::int8 val[2] = { 12 };
    MARTe::Vector<MARTe::int8> valVec(val, 1);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, TestUint16Arr1Read) {
    MDSStructuredDataITest test;
    MARTe::uint16 val[2] = { 12 };
    MARTe::Vector<MARTe::uint16> valVec(val, 1);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, Testint16Arr1Read) {
    MDSStructuredDataITest test;
    MARTe::int16 val[2] = { 12 };
    MARTe::Vector<MARTe::int16> valVec(val, 1);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, TestUint32Arr1Read) {
    MDSStructuredDataITest test;
    MARTe::uint32 val[2] = { 12 };
    MARTe::Vector<MARTe::uint32> valVec(val, 1);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, Testint32Arr1Read) {
    MDSStructuredDataITest test;
    MARTe::int32 val[2] = { 12 };
    MARTe::Vector<MARTe::int32> valVec(val, 1);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, TestUint64Arr1Read) {
    MDSStructuredDataITest test;
    MARTe::uint64 val[2] = { 12 };
    MARTe::Vector<MARTe::uint64> valVec(val);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, Testint64Arr1Read) {
    MDSStructuredDataITest test;
    MARTe::int64 val[2] = { 12, 3 };
    MARTe::Vector<MARTe::int64> valVec(val, 1);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, TestFloat32Arr1Read) {
    MDSStructuredDataITest test;
    MARTe::float32 val[2] = { 12 };
    MARTe::Vector<MARTe::float32> valVec(val, 1);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, TestFloat64Arr1Read) {
    MDSStructuredDataITest test;
    MARTe::float64 val[2] = { 12 };
    MARTe::Vector<MARTe::float64> valVec(val, 1);
    ASSERT_TRUE(test.TestReadArr(valVec));
}

TEST(MDSStructuredDataITest, TestUInt8Read) {
    MDSStructuredDataITest test;
    MARTe::uint8 val = 3;
    ASSERT_TRUE(test.TestRead(val));
}

TEST(MDSStructuredDataITest, TestInt8Read) {
    MDSStructuredDataITest test;
    MARTe::int8 val = 3;
    ASSERT_TRUE(test.TestRead(val));
}

TEST(MDSStructuredDataITest, TestUInt16Read) {
    MDSStructuredDataITest test;
    MARTe::uint16 val = 3;
    ASSERT_TRUE(test.TestRead(val));
}

TEST(MDSStructuredDataITest, TestInt16Read) {
    MDSStructuredDataITest test;
    MARTe::int16 val = 3;
    ASSERT_TRUE(test.TestRead(val));
}

TEST(MDSStructuredDataITest, TestUInt32Read) {
    MDSStructuredDataITest test;
    MARTe::uint32 val = 3;
    ASSERT_TRUE(test.TestRead(val));
}

TEST(MDSStructuredDataITest, TestInt32Read) {
    MDSStructuredDataITest test;
    MARTe::int32 val = 3;
    ASSERT_TRUE(test.TestRead(val));
}

TEST(MDSStructuredDataITest, TestUInt64Read) {
    MDSStructuredDataITest test;
    MARTe::uint64 val = 3;
    ASSERT_TRUE(test.TestRead(val));
}

TEST(MDSStructuredDataITest, TestInt64Read) {
    MDSStructuredDataITest test;
    MARTe::int64 val = 3;
    ASSERT_TRUE(test.TestRead(val));
}

TEST(MDSStructuredDataITest, TestFloat32Read) {
    MDSStructuredDataITest test;
    MARTe::float32 val = 3;
    ASSERT_TRUE(test.TestRead(val));
}

TEST(MDSStructuredDataITest, TestFloat64Read) {
    MDSStructuredDataITest test;
    MARTe::float64 val = 3;
    ASSERT_TRUE(test.TestRead(val));
}

TEST(MDSStructuredDataITest, TestReadString) {
    MDSStructuredDataITest test;
    MARTe::StreamString val = "F4E";
    ASSERT_TRUE(test.TestRead_String(val));
}

TEST(MDSStructuredDataITest, TestRead_Recursively) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestRead_Recursively());
}

TEST(MDSStructuredDataITest, TestRead_StreamString) {
    MDSStructuredDataITest test;
    MARTe::StreamString val = "F4E";
    ASSERT_TRUE(test.TestRead_StreamString(val));
}

TEST(MDSStructuredDataITest, TestRead_NoOpenTree) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestRead_NoOpenTree());
}

TEST(MDSStructuredDataITest, TestRead_InvalidNode) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestRead_InvalidNode());
}

TEST(MDSStructuredDataITest, TestRead_setTreeAndDestroy) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestRead_setTreeAndDestroy());
}

TEST(MDSStructuredDataITest,TestCfgDB) {
    MDSStructuredDataITest test;
    ASSERT_TRUE(test.TestCfgDB());
}

