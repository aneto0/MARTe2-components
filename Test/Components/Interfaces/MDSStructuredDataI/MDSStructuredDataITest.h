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
     * @brief Tests the constructor.
     */
    bool TestConstructor();

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
     * @brief Test MDSStructuredDataI::GetChildName() on error
     */
    bool TestGetChildName_noExistingNode();

    /**
     * @brief Test MDSStructuredDataI::GetChildName() on error
     */
    bool TestGetChildName_noExistingNode2();

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
     */
    bool TestCreateRelative();

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
    bool TestCreateAbsolute_invalidPath();

    /**
     * @brief Test MDSStructuredDataI::MoveToChild() on succeed.
     * @details Move from node A to child 0 (node B)
     */
    bool TestMoveToChild();

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
     * @brief Test MDSStructuredDataI::MoveRelative() on succeed.
     * @details Move to a relative node
     */
    bool TestMoveRelative();

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
     * @brief Test MDSStructuredDataI::MoveAbsolute() on succeed.
     * @details Move to a specific node
     */
    bool TestMoveAbsolute();

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
     * @details Move to ancestor 2
     */
    bool TestMoveToAncestor3();

    /**
     * @brief Test MDSStructuredDataI::MoveToAncestor() on error.
     * @details Move to ancestor when tree is closed
     */
    bool TestMoveToAncestor_treeClosed();

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


private:
    MARTe::StreamString treeName;
    MARTe::StreamString fullPath;
    MARTe::StreamString treeName2;
    MARTe::StreamString fullPath2;
    bool remove_mds_sdi;
    bool remove_mds_sdi2;
    bool CreateNodes(const MARTe::char8 * const path);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSSTRUCTUREDDATAITEST_H_ */
