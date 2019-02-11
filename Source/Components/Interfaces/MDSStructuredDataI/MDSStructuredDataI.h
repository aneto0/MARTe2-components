/**
 * @file MDSStructuredDataI.h
 * @brief Header file for class MDSStructuredDataI
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

 * @details This header file contains the declaration of the class MDSStructuredDataI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSSTRUCTUREDDATAI_H_
#define MDSSTRUCTUREDDATAI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <mdsobjects.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
class MDSStructuredDataI: public Object, public StructuredDataI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * TODO
     */
MDSStructuredDataI    ();

    /**
     * TODO
     */
    virtual ~MDSStructuredDataI();

    /**
     * TODO
     */
    virtual bool Read(const char8 * const name, const AnyType &value);

    /**
     * TODO
     */
    virtual AnyType GetType(const char8 * const name);

    /**
     * @brief write value in the node specified by name
     * @details if the node does not exist it is created
     * @pre
     */
    virtual bool Write(const char8 * const name, const AnyType &value);

    /**
     * @brief copy the tree to the specified destination
     * @details copy recursively all nodes from the current node. Notice that the current note is not
     * copied to the destination.
     * @return true if the structure is copied to the destination.
     */
    virtual bool Copy(StructuredDataI &destination);

    /**
     * TODO
     */
    virtual bool AddToCurrentNode(Reference node);

    /**
     *@details move the currentNode to root node
     *@return tree is the current node is updated with root node
     */
    virtual bool MoveToRoot();

    /**
     * @brief move to n ancestor.
     * @details move the current node to the specified ancestor. if the generation is larger
     * than the depth of the tree, the node is moved to "TOP" (which is the first node by default.
     * @return true if the current node is updated
     */
    virtual bool MoveToAncestor(uint32 generations);

    /**
     * @brief move to specified path.
     * @details if the tree is opened and the path exists, this function move from the current node to
     * the specified node.
     * @return true if the function can move to the specified node.
     */
    virtual bool MoveAbsolute(const char8 * const path);

    /**
     * @brief move from the current node to the child specified by path.
     * @details The function only can move to immediate child (i.e if A has a child B, and B has a child C
     * if we are in A, we only can move to B, the function not allow to move from A to C).
     * The  tree must be opened.
     * @true if it is moved to the specified path.
     */
    virtual bool MoveRelative(const char8 * const path);

    /**
     * @brief move to the specified child
     * @details move from the current node to the specified child. If the tree is not opened the function
     * returns error
     * @return true if it is moved to the specified child
     */
    virtual bool MoveToChild(const uint32 childIdx);

    /**
     * @brief Create node from the root node
     * @details the tree must be opened in the editable mode and the path must exist.
     * If the function succeeds the current node is updated with the node created.
     * @return true if the  node is created.
     */
    virtual bool CreateAbsolute(const char8 * const path);

    /**
     * @brief Create a child node from the current node
     * @details if editable mode and tree open the function creates a child node with name path and the current node
     * is updated to the new child.
     * @return true if the node has been created.
     */
    virtual bool CreateRelative(const char8 * const path);

    /**
     * @brief Delete the node specified by name
     * @details if the tree is open in editable mode the node specified by name is removed.
     * Notice that the name must be full path
     * @return true if the node is deleted.
     */
    virtual bool Delete(const char8 * const name);

    /**
     * @brief Get the current node name
     * @details if the tree is not opened GetName returns NULL pointer
     * @return the current node name on succeed.
     */
    virtual const char8 *GetName();

    /**
     * @brief get the name of the children which corresponds to the index
     * @details if the index is not found in the children list the function returns a NULL pointer
     * return the name of the children in the position index.
     */
    virtual const char8 *GetChildName(const uint32 index);

    /**
     * @return return the number of children of the current node
     */
    virtual uint32 GetNumberOfChildren();

    /**
     * @brief set a tree
     * @details if the tree is already open the treeIn isn't open.
     * @return true if the treeIn is opened
     */
    bool SetTree(MDSplus::Tree *treeIn);

    /**
     * @brief Select if the tree is in editable mode or not.
     * @details In MDSplus editable mode means that the structure of the tree can be changed.
     * In no editable mode the nodes cannot be created but data can be added to a node.
     * If SetEditMode is called when the tree is already opened, it is reopened in the specified mode. Nevertheless
     * if changes on the structure of the tree are done but are not save, the mode cannot be changed.
     */
    bool SetEditMode(bool edit);

    /**
     * @brief open a tree
     * @details open a tree with name treeName and pulse number pulseNumber with the mode
     * specified in editModeSet. If a tree is already opened and the opened tree name is treeName the function
     * exits normally
     * @pre the tree must exist (be created previously)
     * @pre tree = NULL_PTR(Tree *)
     * @post openPulseNumber = pulseNumber
     * @post tree != NULL_PTR(Tree *)
     * @return true if a tree with name treeName is opened
     */
    bool OpenTree(const char8 *const treeName, int32 pulseNumber);

    /**
     * @brief close the tree
     * @details If changes are done in the structure of the tree and they are not saved, closing the
     * tree will lose all the changes made.
     * If the tree is not open CloseTree() return false
     * @return true if the tree is closed.
     */
    bool CloseTree();

    /**
     * @brief Creates a tree
     * @details creates a tree if it is not created. If the tree already exist and force is false no
     * tree is created but the function exits with true value.
     * @return true if the tree is created or if it already exists
     */
    bool CreateTree(const char8 *const treeName, bool force = false);

    /**
     * @brief save all the changes done to the tree.
     * @details If the tree is not opened the function return false.
     * If the tree is open but the mode is not editable the function return false.
     * @return true if the changes are successfully saved
     */
    bool SaveTree();

private:
    /**
     * The MDSplus tree.
     */
    MDSplus::Tree *tree;

    /**
     * The MDSplus tree root node.
     */
    MDSplus::TreeNode *rootNode;

    /**
     * The current MDSplus tree node.
     */
    MDSplus::TreeNode *currentNode;

    /**
     * True if the tree can be edited.
     */
    bool editModeSet;

    /**
     * Indicates if the tree was open internally and hence must be destroyed internally.
     */
    bool internallyCreated;

    bool CreateNodes(const MARTe::char8 * const path);
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSSTRUCTUREDDATAI_H_ */
