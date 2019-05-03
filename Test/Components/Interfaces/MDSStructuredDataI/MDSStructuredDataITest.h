/**
 * @file MDSStructuredDataITest.h
 * @brief Header file for class MDSStructuredDataITest
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

 * @details This header file contains the declaration of the class MDSStructuredDataITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSSTRUCTUREDDATAITEST_H_
#define MDSSTRUCTUREDDATAITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StreamString.h"
#include "MDSStructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the MDSStructuredDataI public method.
 */
class MDSStructuredDataITest {
public:
    MDSStructuredDataITest();
    virtual ~MDSStructuredDataITest();

    /**
     * @brief Test MDSStructuredDataI::CreateTree() on succeed
     * @details the tree wasn't created previously. force = false
     */
    bool TestCreateTree();

    /**
     * @brief Test MDSStructuredDataI::CreateTree() on error
     * @details the tree was created previously in TestCreateTree(). force = false
     */
    bool TestCreateTree2();

    /**
     * @brief Test MDSStructuredDataI::CreateTree() on succeed
     * @details the tree was created previously in TestCreateTree(). force = true
     */
    bool TestCreateTree_force();

    /**
     * @brief Test MDSStructuredDataI::OpenTree() on succeed
     * @details the tree model is already created and editModeSet =false
     */
    bool TestOpentree();

    /**
     * @brief Test MDSStructuredDataI::OpenTree() on succeed
     * @details the tree model is already created and editModeSet = true
     */
    bool TestOpentree2();

    /**
     * @brief Test MDSStructuredDataI::OpenTree() on succeed
     * @details Open twice the same tree with the model already created and editModeSet = false.
     */
    bool TestOpentree_sameTree();

    /**
     * @brief Test MDSStructuredDataI::OpenTree() on error
     * @details Open two different trees with the model already created and editModeSet = false.
     */
    bool TestOpentree_diffTree();

    /**
     * @brief Test MDSStructuredDataI::OpenTree() on error
     * @details Open a tree without creating it. editModeSet = false.
     */
    bool TestOpentree_NoCreate();

    /**
     * @brief Test MDSStructuredDataI::SetTree() on succeed
     * @details Set a tree.
     */
    bool TestSetTree();

    /**
     * @brief Test MDSStructuredDataI::SetTree() on error
     * @details Set a tree when the tree is already open.
     */
    bool TestSetTree_openTree();

    /**
     * @brief Test MDSStructuredDataI::CloseTree() on succeed
     * @details Close a tree which was previously opened.
     */
    bool TestcloseTree();

    /**
     * @brief Test MDSStructuredDataI::CloseTree() on succeed
     * @details Close a tree which was Set.
     */
    bool TestcloseTree_SetTree();

    /**
     * @brief Test MDSStructuredDataI::CloseTree() on error
     * @details Close a tree which wasn't previously opened.
     */
    bool TestcloseTree_noOpened();

    /**
     * @brief Test MDSStructuredDataI::Delete() on succeed
     * @details Delete a node which was previously created.
     */
    bool TestDelete();

    /**
     * @brief Test MDSStructuredDataI::Delete() on error
     * @details Delete a node which when editMode is false.
     */
    bool TestDelete_noEditable();

    /**
     * @brief Test MDSStructuredDataI::Delete() on error
     * @details Delete a node which when the tree is not Opened.
     */
    bool TestDelete_noOpened();

    /**
     * @brief Test MDSStructuredDataI::Delete() on failure
     * @details Delete a node which does not exist.
     */
    bool TestDelete_noNode();

    /**
     * @brief Test MDSStructuredDataI::SetEditMode() on succeed
     * @details Open the tree in not editable mode, change to editable mode and add nodes.
     */
    bool TestSetEditMode();

    /**
     * @brief Test MDSStructuredDataI::SetEditMode() on succeed
     * @details Open the tree in  editable mode, add nodes, change mode to edit mode again add relative nodes.
     */
    bool TestSetEditMode2();

    /**
     * @brief Test MDSStructuredDataI::SetEditMode() on error
     * @details Open the tree in  editable mode, add nodes, change mode to no editable mode before saving changes.
     */
    bool TestSetEditMode3();

    /**
     * @brief Test MDSStructuredDataI::SaveTree() on succeed
     */
    bool TestSaveTree();

    /**
     * @brief Test MDSStructuredDataI::SaveTree() on succeed
     * @details try to save tree in no editable mode
     */
    bool TestSaveTree_noEditable();

    /**
     * @brief Test MDSStructuredDataI::SaveTree() on error
     * @details try to save changes in a closed tree
     */
    bool TestSaveTree_noOpen();

    /**
     * @brief Test MDSStructuredDataI::GetNumberOfChildren() on succeed
     */
    bool TestGetNumberOfChildren();

    /**
     * @brief Test MDSStructuredDataI::GetNumberOfChildren() on succeed
     */
    bool TestGetNumberOfChildren2();

    /**
     * @brief Test MDSStructuredDataI::GetNumberOfChildren() on error
     */
    bool TestGetNumberOfChildren_closedTree();

    /**
     * @brief Test MDSStructuredDataI::GetChildName() on succeed
     */
    bool TestGetChildName();

    /**
     * @brief Test MDSStructuredDataI::GetChildName() on succeed
     */
    bool TestGetChildName_treeClosed();

    /**
     * @brief Test MDSStructuredDataI::GetChildName() on error
     */
    bool TestGetChildName_noExistingNode();

    /**
     * @brief Test MDSStructuredDataI::GetChildName() on error
     */
    bool TestGetChildName_noExistingNode2();

    /**
     * @brief Test MDSStructuredDataI::GetChildName() on error
     */
    bool TestGetChildName_setAndDestroy();

    /**
     * @brief Test MDSStructuredDataI::GetName() on succeed
     */
    bool TestGetName();

    /**
     * @brief Test MDSStructuredDataI::GetName() on error
     */
    bool TestGetName_closedTree();
    /**
     * @brief Test MDSStructuredDataI::CreateRelative() on succeed
     * @details the path contains a unique node
     */
    bool TestCreateRelative();

    /**
     * @brief Test MDSStructuredDataI::CreateRelative() on succeed
     * @details the path contains several nodes not created
     */
    bool TestCreateRelative2();

    /**
     * @brief Test MDSStructuredDataI::CreateRelative() on succeed
     * @details the path contains several nodes some of them already created
     */
    bool TestCreateRelative3();

    /**
     * @brief Test MDSStructuredDataI::CreateRelative() on error.
     * @details create relative nodes which already exist
     */
    bool TestCreateRelative_AllNodesExist();

    /**
     * @brief Test MDSStructuredDataI::CreateRelative() on error.
     * @details create relative node in no editable mode.
     */
    bool TestCreateRelative_NoEditable();

    /**
     * @brief Test MDSStructuredDataI::CreateRelative() on error.
     * @details create relative when tree is not opened
     */
    bool TestCreateRelative_closedTree();

    /**
     * @brief Test MDSStructuredDataI::CreateRelative() on error.
     * @details Set tree, destroy tree and try to CreateRelative
     */
    bool TestCreateRelative_setTreeAndDestroy();

    /**
     * @brief Test MDSStructuredDataI::CreateAbsolute() on succeed.
     */
    bool TestCreateAbsolute();

    /**
     * @brief Test MDSStructuredDataI::CreateAbsolute() on error.
     * @details create absolute node with no editable mode
     */
    bool TestCreateAbsolute_noEditable();

    /**
     * @brief Test MDSStructuredDataI::CreateAbsolute() on error.
     * @details create absolute node with tree closed
     */
    bool TestCreateAbsolute_closedTree();

    /**
     * @brief Test MDSStructuredDataI::CreateAbsolute() on error.
     * @details create absolute node with invalid path
     */
    bool TestCreateAbsolute_AllNodesExist();

    /**
     * @brief Test MDSStructuredDataI::CreateAbsolute() on error.
     * @details Set tree, destroy tree and try to CreateAbsolute
     */
    bool TestCreateAbsolute_setTreeAndDestroy();

    /**
     * @brief Test MDSStructuredDataI::MoveToChild() on succeed.
     * @details Move from node A to child 0 (node B)
     */
    bool TestMoveToChild();

    /**
     * @brief Test MDSStructuredDataI::MoveToChild() on succeed.
     * @details Create several nodes remove intermediate nodes use MoveToChild
     */
    bool TestMoveToChild2();

    /**
     * @brief Test MDSStructuredDataI::MoveToChild() on error.
     * @details Move from node A to invalid node.
     */
    bool TestMoveToChild_invalidNode();

    /**
     * @brief Test MDSStructuredDataI::MoveToChild() on error.
     * @details Move with the tree closed.
     */
    bool TestMoveToChild_ClosedTree();

    /**
     * @brief Test MDSStructuredDataI::MoveToChild() on error.
     * @details Set tree, destroy tree and try to MoveToChild()
     */
    bool TestMoveToChild_setTreeAndDestroy();

    /**
     * @brief Test MDSStructuredDataI::MoveRelative() on succeed.
     * @details Move to a relative node
     */
    bool TestMoveRelative();

    /**
     * @brief Test MDSStructuredDataI::MoveRelative() on succeed.
     * @details Move to a relative node which is far away.
     */
    bool TestMoveRelative2();

    /**
     * @brief Test MDSStructuredDataI::MoveRelative() on error.
     * @details Move to an invalid node
     */
    bool TestMoveRelative_InvalidNode();

    /**
     * @brief Test MDSStructuredDataI::MoveRelative() on error.
     * @details Move with the tree closed
     */
    bool TestMoveRelative_closedTree();

    /**
     * @brief Test MDSStructuredDataI::MoveRelative() on error.
     * @details Set tree, destroy tree and try to MoveToRelative()
     */
    bool TestMoveRelative_setTreeAndDestroy();

    /**
     * @brief Test MDSStructuredDataI::MoveAbsolute() on succeed.
     * @details Move to a specific node
     */
    bool TestMoveAbsolute();

    /**
     * @brief Test MDSStructuredDataI::MoveAbsolute() on succeed.
     * @details Move to a specific node far away
     */
    bool TestMoveAbsolute2();

    /**
     * @brief Test MDSStructuredDataI::MoveAbsolute() on error.
     * @details Move to an invalid node
     */
    bool TestMoveAbsolute_InvalidNode();

    /**
     * @brief Test MDSStructuredDataI::MoveAbsolute() on error.
     * @details Move when the tree is closed.
     */
    bool TestMoveAbsolute_closedTree();

    /**
     * @brief Test MDSStructuredDataI::MoveAbsolute() on error.
     * @details Set tree, destroy tree and try to MoveToAbsolute()
     */
    bool TestMoveAbsolute_setTreeAndDestroy();

    /**
     * @brief Test MDSStructuredDataI::MoveToAncestor() on succeed.
     * @details Move to ancestor 1
     */
    bool TestMoveToAncestor();

    /**
     * @brief Test MDSStructuredDataI::MoveToAncestor() on succeed.
     * @details Move to ancestor 2
     */
    bool TestMoveToAncestor2();

    /**
     * @brief Test MDSStructuredDataI::MoveToAncestor() on succeed.
     * @details Move to ancestor 3
     */
    bool TestMoveToAncestor3();

    /**
     * @brief Test MDSStructuredDataI::MoveToAncestor() on succeed.
     * @details Move to ancestor 4
     */
    bool TestMoveToAncestor4();

    /**
     * @brief Test MDSStructuredDataI::MoveToAncestor() on error.
     * @details Move to ancestor when tree is closed
     */
    bool TestMoveToAncestor_treeClosed();

    /**
     * @brief Test MDSStructuredDataI::MoveToAncestor() on error.
     * @details Set tree, destroy tree and try to MoveToAncestor()
     */
    bool TestMoveToAncestor_setTreeAndDestroy();

    /**
     * @brief Test MDSStructuredDataI::MoveToRoot() on succeed.
     * @details Move to root
     */
    bool TestMoveToRoot();

    /**
     * @brief Test MDSStructuredDataI::MoveToRoot() on error.
     * @details Move to root when tree is closed
     */
    bool TestMoveToRoot_treeClosed();

    /**
     * @brief Test MDSStructuredDataI::MoveToRoot() on error.
     * @details Set tree, destroy tree and try to MoveToRoot()
     */
    bool TestMoveToRoot_setTreeAndDestroy();

    /**
     * @brief Test MDSStructuredDataI::Copy() on succeed.
     * @details copy from MDSStructuredDataI to MDSStructuredDataI structure
     */
    bool TestCopy();

    /**
     * @brief Test MDSStructuredDataI::Copy() on succeed.
     * @details copy from MDSStructuredDataI to configurationDatabase structure
     */
    bool TestCopy2();

    /**
     * @brief Test MDSStructuredDataI::Copy() on error.
     * @details copy from MDSStructuredDataI to MDSStructuredDataI with destination tree closed
     */
    bool TestCopy_treeClosed();

    /**
     * @brief Test MDSStructuredDataI::Copy() on error.
     * @details copy from MDSStructuredDataI to MDSStructuredDataI with origin tree closed
     */
    bool TestCopy_treeClosed2();

    /**
     * @brief Test MDSStructuredDataI::Write() on succeed.
     * @details Write several types on the tree and validate what is saved
     */
    template<typename T>
    bool TestWrite(T value);

    /**
     * @brief Test MDSStructuredDataI::Write() on succeed.
     */
    bool TestWrite_string();

    /**
     * @brief Test MDSStructuredDataI::Write() on succeed.
     */
    bool TestWrite_constString();

    /**
     * @brief Test MDSStructuredDataI::Write() on succeed.
     */
    bool TestWrite_char();

    /**
     * @brief Test MDSStructuredDataI::Write() on succeed.
     */
    bool TestWrite_charArray();

    /**
     * @brief Test MDSStructuredDataI::Write() on succeed.
     * @details the name includes a relative path
     */
    bool TestWrite_Recursively();

    /**
     * @brief Test MDSStructuredDataI::Write() on error.
     * @details Try to write when mode is not editable
     */
    bool TestWrite_noEditable();

    /**
     * @brief Test MDSStructuredDataI::Write() on error.
     * @brief Passing to Write() a non supported type
     */
    bool TestWrite_InvalidType();

    /**
     * @brief Test MDSStructuredDataI::Write() on error.
     * @details try to write with the tree closed
     */
    bool TestWrite_treeClosed();

    /**
     * @brief Test MDSStructuredDataI::Write() on error.
     * @details Set tree, destroy tree and try to Write()
     */
    bool TestWrite_setTreeAndDestroy();

    /**
     * @brief Test MDSStructuredDataI::GetType() on succeed
     * @details checks that the type and numberOfElements is as expected.
     */
    template<typename T>
    bool TestGetType(T type);

    /**
     * @brief Test MDSStructuredDataI::GetType() on succeed.
     * @details Call type including a relative path to the name
     */
    bool TestGetType_Recursively();

    /**
     * @brief Test MDSStructuredDataI::GetType() on error.
     * @details try to call GetType() when tree is closed.
     */
    bool TestGetType_treeClosed();

    /**
     * @brief Test MDSStructuredDataI::Write() on error.
     * @details Set tree, destroy tree and try to GetType()
     */
    bool TestGetType_setTreeAndDestroy();

    /**
     * @brief Test MDSStructuredDataI::Read() on succeed
     */
    template<typename T>
    bool TestReadArr(T type);

    /**
     * @brief Test MDSStructuredDataI::Read() on succeed
     */
    template<typename T>
    bool TestRead(T type);

    /**
     * @brief Test MDSStructuredDataI::Read() on succeed
     * @details specialization
     */
    bool TestRead_String(MARTe::StreamString val);

    /**
     * @brief Test MDSStructuredDataI::Read() on succeed
     * @details try to read including a path
     */
    bool TestRead_Recursively();

    /**
     * @brief Test MDSStructuredDataI::Read() on error
     * @details try to read StreamString (which is not MDSplus supported type)
     */
    bool TestRead_StreamString(MARTe::StreamString val);

    /**
     * @brief Test MDSStructuredDataI::Read() on error
     * @details Try to read a tree without opening it
     */
    bool TestRead_NoOpenTree();

    /**
     * @brief Test MDSStructuredDataI::Read() on error
     * @details Try to read a non existing node
     */
    bool TestRead_InvalidNode();

    /**
     * @brief Test MDSStructuredDataI::Read() on error
     * @details First set a tree remove the pointer and try to read it
     */
    bool TestRead_setTreeAndDestroy();

    /**
     * @brief General test.
     * @detials copy a cfg to MDSplus tree
     */
    bool TestCfgDB();

private:
    MARTe::StreamString treeName;
    MARTe::StreamString fullPath;
    MARTe::StreamString treeName2;
    MARTe::StreamString fullPath2;
    bool remove_mds_sdi;
    bool remove_mds_sdi2;
    bool CreateNodes(const MARTe::char8 * const path);

};

template<typename T>
bool MDSStructuredDataITest::TestWrite(T value) {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.Write("NodeVal", value);
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    T returnValue;
    if (ret) {
        ret = mdsStructuredDataI.Read("NodeVal", returnValue);
    }
    if (ret) {
        returnValue = 3;
    }
    return ret;
}

template<typename T>
bool MDSStructuredDataITest::TestGetType(T value) {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.Write("NodeWithVal", value);
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    AnyType returnedType;
    AnyType inputType(value);
    if (ret) {
        returnedType = mdsStructuredDataI.GetType("NodeWithVal");
        ret = (returnedType.GetTypeDescriptor() == inputType.GetTypeDescriptor());
    }
    if (ret) {
        ret = returnedType.GetNumberOfElements(0u) == inputType.GetNumberOfElements(0u);
    }
    return ret;
}

template<typename T>
bool MDSStructuredDataITest::TestReadArr(T value) {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.Write("NodeVal", value);
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }

    AnyType returnedType = mdsStructuredDataI.GetType("NodeVal");
    TypeDescriptor mdsType = returnedType.GetTypeDescriptor();
    void *data = NULL_PTR(void *);
    if (mdsType == UnsignedInteger8Bit) {
        data = new uint8[returnedType.GetNumberOfElements(0u)];
    }
    else if (mdsType == SignedInteger8Bit) {
        data = new int8[returnedType.GetNumberOfElements(0u)];
    }
    else if (mdsType == UnsignedInteger16Bit) {
        data = new uint16[returnedType.GetNumberOfElements(0u)];
    }
    else if (mdsType == SignedInteger16Bit) {
        data = new int16[returnedType.GetNumberOfElements(0u)];
    }
    else if (mdsType == UnsignedInteger32Bit) {
        data = new uint32[returnedType.GetNumberOfElements(0u)];
    }
    else if (mdsType == SignedInteger32Bit) {
        data = new int32[returnedType.GetNumberOfElements(0u)];
    }
    else if (mdsType == UnsignedInteger64Bit) {
        data = new uint64[returnedType.GetNumberOfElements(0u)];
    }
    else if (mdsType == SignedInteger64Bit) {
        data = new int64[returnedType.GetNumberOfElements(0u)];
    }
    else if (mdsType == Float32Bit) {
        data = new float32[returnedType.GetNumberOfElements(0u)];
    }
    else if (mdsType == Float64Bit) {
        data = new float64[returnedType.GetNumberOfElements(0u)];
    }
    else if (mdsType == CharString) {
        data = new char8[returnedType.GetNumberOfElements(0u)];
    }
    else if (mdsType == ConstCharString) {
        data = new char8[returnedType.GetNumberOfElements(0u)];
    }
    else {
        ret = false;
    }
    AnyType typeRead(mdsType, 0u, data);
    if (ret) {
        ret = mdsStructuredDataI.Read("NodeVal", typeRead);
    }

    if (ret) {
        if (mdsType == UnsignedInteger8Bit) {
            for (uint32 i = 0; i < returnedType.GetNumberOfElements(0u) && ret; i++) {
                ret = (reinterpret_cast<uint8 *>(typeRead.GetDataPointer()))[i] == value[i];
            }
            delete[] reinterpret_cast<uint8 *>(data);
        }
        else if (mdsType == SignedInteger8Bit) {
            for (uint32 i = 0; i < returnedType.GetNumberOfElements(0u) && ret; i++) {
                int8 aux = (reinterpret_cast<int8 *>(typeRead.GetDataPointer()))[i];
                int8 aux2 = value[i];
                ret = (aux == aux2);
            }
            delete[] reinterpret_cast<int8 *>(data);
        }
        else if (mdsType == UnsignedInteger16Bit) {
            for (uint32 i = 0; i < returnedType.GetNumberOfElements(0u) && ret; i++) {
                ret = (reinterpret_cast<uint16 *>(typeRead.GetDataPointer()))[i] == value[i];
            }
            delete[] reinterpret_cast<uint16 *>(data);
        }
        else if (mdsType == SignedInteger16Bit) {
            for (uint32 i = 0; i < returnedType.GetNumberOfElements(0u) && ret; i++) {
                int16 aux = (reinterpret_cast<int16 *>(typeRead.GetDataPointer()))[i];
                int16 aux2 = value[i];
                ret = (aux == aux2);
            }
            delete[] reinterpret_cast<int16 *>(data);
        }
        else if (mdsType == UnsignedInteger32Bit) {
            for (uint32 i = 0; i < returnedType.GetNumberOfElements(0u) && ret; i++) {
                uint32 aux = (reinterpret_cast<uint32 *>(typeRead.GetDataPointer()))[i];
                uint32 aux2 = value[i];
                ret = (aux == aux2);
            }
            delete[] reinterpret_cast<uint32 *>(data);
        }
        else if (mdsType == SignedInteger32Bit) {
            for (uint32 i = 0; i < returnedType.GetNumberOfElements(0u) && ret; i++) {
                int32 aux = (reinterpret_cast<int32 *>(typeRead.GetDataPointer()))[i];
                int32 aux2 = value[i];
                ret = (aux == aux2);
            }
            delete[] reinterpret_cast<int32 *>(data);
        }
        else if (mdsType == UnsignedInteger64Bit) {
            for (uint32 i = 0; i < returnedType.GetNumberOfElements(0u) && ret; i++) {
                uint64 aux = (reinterpret_cast<uint64 *>(typeRead.GetDataPointer()))[i];
                uint64 aux2 = value[i];
                ret = (aux == aux2);
            }
            delete[] reinterpret_cast<uint64 *>(data);
        }
        else if (mdsType == SignedInteger64Bit) {
            for (uint32 i = 0; i < returnedType.GetNumberOfElements(0u) && ret; i++) {
                int64 aux = (reinterpret_cast<int64 *>(typeRead.GetDataPointer()))[i];
                int64 aux2 = value[i];
                ret = (aux == aux2);
            }
            delete[] reinterpret_cast<int64 *>(data);
        }
        else if (mdsType == Float32Bit) {
            for (uint32 i = 0; i < returnedType.GetNumberOfElements(0u) && ret; i++) {
                ret = ((reinterpret_cast<float32 *>(typeRead.GetDataPointer()))[i] == value[i]);
            }
            delete[] reinterpret_cast<float32 *>(data);
        }
        else if (mdsType == Float64Bit) {
            for (uint32 i = 0; i < returnedType.GetNumberOfElements(0u) && ret; i++) {
                ret = ((reinterpret_cast<float64 *>(typeRead.GetDataPointer()))[i] == value[i]);
            }
            delete[] reinterpret_cast<float64 *>(data);
        }
        else if (mdsType == CharString) {
            for (uint32 i = 0; i < returnedType.GetNumberOfElements(0u) && ret; i++) {
                char8 aux = value[i];
                ret = ((reinterpret_cast<char8 *>(typeRead.GetDataPointer()))[i] == aux);
            }
            delete[] reinterpret_cast<char8 *>(data);
        }
        else if (mdsType == ConstCharString) {
            for (uint32 i = 0; i < returnedType.GetNumberOfElements(0u) && ret; i++) {
                char8 aux = value[i];
                ret = ((reinterpret_cast<char8 *>(typeRead.GetDataPointer()))[i] == aux);
            }
            delete[] reinterpret_cast<char8 *>(data);
        }
    }
    return ret;
}

template<typename T>
bool MDSStructuredDataITest::TestRead(T value) {
    using namespace MARTe;
    MDSStructuredDataI mdsStructuredDataI;
    bool force = true;
    bool ret = mdsStructuredDataI.CreateTree(treeName.Buffer(), force);
    if (ret) {
        remove_mds_sdi = true;
    }
    if (ret) {
        mdsStructuredDataI.SetEditMode(true);
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.OpenTree(treeName.Buffer(), -1);
    }
    if (ret) {
        ret = mdsStructuredDataI.CreateAbsolute("A");
    }
    if (ret) {
        ret = mdsStructuredDataI.Write("NodeVal", value);
    }
    if (ret) {
        ret = mdsStructuredDataI.SaveTree();
    }
    T typeRead;
    if (ret) {
        ret = mdsStructuredDataI.Read("NodeVal", typeRead);
    }
    if (ret) {
        ret = (typeRead == value);
    }
    return ret;
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSSTRUCTUREDDATAITEST_H_ */
