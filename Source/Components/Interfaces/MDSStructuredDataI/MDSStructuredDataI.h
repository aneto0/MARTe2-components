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

#ifndef MDSStructuredDataI_H_
#define MDSStructuredDataI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include <mdsobjects.h>
/*lint -D__cplusplus*/
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief
 * @details Read/Write String arrays is not supported.
 */
class MDSStructuredDataI: public Object, public StructuredDataI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     */
MDSStructuredDataI    ();

    /**
     * @brief Default destructor
     * @details Checks if the tree is still open and close the tree freeing resources if they were allocate internally
     * @post
     *   tree = NULL_PTR(MDSplus::Tree *)
     *   RootNode = NULL_PTR(MDSplus::TreeNode *)
     *   currentNode = NULL_PTR(MDSplus::TreeNode *)
     *   internallyCreated = false;
     *   isOpen = false;
     */
    virtual ~MDSStructuredDataI();

    /**
     * @brief Read the value of the node specified by the name
     * @details Reads the value of a leaf from the current node. The type of the variable to be read is given by the type of value (See class description for a list of supported types).
     * Before calling Read() the memory to hold the values must be allocated.
     * @param[in] name Is the name of the leave to be read. name could include a relative path from the current node position. i.e name = "A.B.nameleafToBeRead"
     * @param[in, out] value Is the variable where node data is stored. value give the type to be read. i.e. if value is the type float whatever is in the node will be converted in floats.
     * @pre
     *   IsOpen() == true
     *   rootNode must be valid
     *   currentNode must be valid
     * @return true if the values are successfully read.
     */
    virtual bool Read(const char8 * const name, const AnyType &value);

    /**
     * @brief Queries the type of the leaf node.
     * @details The function creates an AnyType from the characteristics of the leaf node specified. The functions loads in the return value
     * the type of the leaf and the number of elements.
     * @param[in] name Is the name of the leaf. name could include the relative path the the current position. i.e name = "A.B.nameLeaf"
     * @pre
     *   IsOpen() == true
     *   currentNode must be valid
     * @return an AnyType which includes the type and the number of elements of the leaf.
     */
    virtual AnyType GetType(const char8 * const name);

    /**
     * @brief Write value in the node specified by name
     * @details If the node does not exist it is created.
     * @param[in] name Is the name of the leaf where the value is stored. name can include relative path. i.e name = "A.B.nameLeaf"
     * @param[in] value Is the data to be stored. The type of value give the type of the node.
     * @pre
     *   editModeSet == true
     *   IsOpen() == true
     *   rootNode must be valid
     *   currentNode must be valid
     * @return true if the vale is successfully written
     */
    virtual bool Write(const char8 * const name, const AnyType &value);

    /**
     * @brief Copy the tree to the specified destination
     * @details Copy recursively all nodes from the current node. Notice that the current note is not
     * copied to the destination.
     * @param[out] destination Is where the current tree will be copied to.
     * @pre
     *  IsOpen() == True
     * @return true if the structure is copied to the destination.
     */
    virtual bool Copy(StructuredDataI &destination);

    /**
     * @brief function not supported
     * @return false.
     */
    virtual bool AddToCurrentNode(Reference node);

    /**
     * @details Moves the currentNode to root node
     * @pre
     *   IsOpen() == true
     *   rootNode must be valid
     *   currentNode must be valid
     * @return tree is the current node is updated with root node
     */
    virtual bool MoveToRoot();

    /**
     * @brief Move to n ancestor.
     * @details move the current node to the specified ancestor. if the generation is larger
     * than the depth of the tree, the node is moved to "TOP" (which is the first node by default).
     * @param[in] generations Indicates how many positions the tree must be moved
     * @pre
     *   IsOpen() == true
     *   rootNode must be valid
     *   currentNode must be valid
     * @return true if the current node is updated
     */
    virtual bool MoveToAncestor(const uint32 generations);

    /**
     * @brief Move to specified path.
     * @details If the tree is opened and the path exists, this function move from the current node to
     * the specified node.
     * @param[in] path Indicates the absolute path to the target node.
     * @pre
     *   IsOpen() == true
     *   rootNode must be valid
     *   currentNode must be valid
     * @return true if the function can move to the specified node.
     */
    virtual bool MoveAbsolute(const char8 * const path);

    /**
     * @brief Move from the current node to the child specified by path.
     * @param[in] path Indicated the relative path to the target node.
     * @pre
     *   IsOpen() == true
     *   currentNode must be valid
     * @return true if it is moved to the specified path.
     */
    virtual bool MoveRelative(const char8 * const path);

    /**
     * @brief Move to the specified child
     * @details move from the current node to the specified child. If the tree is not opened the function
     * returns error
     * @param[in] childIdx Indicates the number of the child to be moved. It starts to count from 0
     * @pre
     *   IsOpen() == true
     *   currentNode must be valid
     * @return true if it is moved to the specified child
     */
    virtual bool MoveToChild(const uint32 childIdx);

    /**
     * @brief Create a node from the root node
     * @details the tree must be opened in the editable mode and the path must exist.
     * If the function succeeds the current node is updated with the node created.
     * @param[in] path indicated the absolute path where the new node will be created.
     * @pre
     *   editModeSet == true
     *   IsOpen() == true
     *   rootNode must be valid
     *   currentNode must be valid
     * @return true if the node is created.
     */
    virtual bool CreateAbsolute(const char8 * const path);

    /**
     * @brief Create a child node from the current node
     * @details if editable mode and tree open the function creates a child node with name path and the current node
     * is updated to the new child.
     * @param[in] path indicated the relative path where the new node will be created.
     * @pre
     *   editModeSet == true
     *   IsOpen() == true
     *   currentNode must be valid
     * @return true if the node has been created.
     */
    virtual bool CreateRelative(const char8 * const path);

    /**
     * @brief Delete the node specified by name
     * @details if the tree is open in editable mode the node specified by name is removed.
     * Notice that the name must be full path
     * @param[in] name is the name of the node to be removed. Notice that name must include the absolute path
     * @pre
     *   editModeSet == true
     *   IsOpen() == true
     *   tree must be valid
     * @return true if the node is deleted.
     */
    virtual bool Delete(const char8 * const name);

    /**
     * @brief Get the current node name
     * @details if the tree is not opened GetName returns NULL pointer
     * @pre
     *   IsOpen() == true
     *   currentNode must be valid
     * @return the current node name on succeed.
     */
    virtual const char8 *GetName();

    /**
     * @brief get the name of the children which corresponds to the index
     * @details if the index is not found in the children list the function returns a NULL pointer
     * @param[in] index is the child number target. Notice that index starts in 0.
     * @pre
     *   IsOpen() == true
     *   currentNode must be valid
     * return the name of the children in the position index.
     */
    virtual const char8 *GetChildName(const uint32 index);

    /**
     * @brief Query the number of children of the current node.
     * @pre
     *   IsOpen() == true
     *   currentNode must be valid
     * @return return the number of children of the current node
     */
    virtual uint32 GetNumberOfChildren();

    /**
     * @brief set a tree
     * @details if the tree is already open the treeIn isn't open.
     * @return true if the treeIn is opened
     */
    bool SetTree(MDSplus::Tree * const treeIn);

    /**
     * @brief Select if the tree is in editable mode or not.
     * @details In MDSplus editable mode means that the structure of the tree can be changed.
     * In no editable mode the nodes cannot be created but data can be added to a node.
     * If SetEditMode is called when the tree is already opened, it is reopened in the specified mode. Nevertheless
     * if changes on the structure of the tree are done but are not save, the mode cannot be changed.
     * @param[in] edit Selects the editable mode. true--> editable, false--> not editable
     * @return true if the mode is changed
     */
    bool SetEditMode(bool edit);

    /**
     * @brief Open a tree
     * @details Open a tree with name treeName and pulse number pulseNumber with the mode
     * specified in editModeSet. If a tree is already opened and the opened tree name is treeName the function
     * exits normally
     * @param[in] treeName Is the tree name to be opened
     * @param[in] pulseNumber is the pulse number to be open.
     * @pre
     *   the tree must exist (be created previously)
     *   tree = NULL_PTR(Tree *)
     * @post
     *   openPulseNumber = pulseNumber
     *   tree != NULL_PTR(Tree *)
     * @return true if a tree with name treeName is opened
     */
    bool OpenTree(const char8 *const treeName, int32 pulseNumber);

    /**
     * @brief close the tree
     * @warning If changes are done in the structure of the tree and they are not saved, closing the
     * tree will lose all the changes made.
     * If the tree is not open CloseTree() return false
     * @post
     *   tree = NULL_PTR(MDSplus::Tree *)
     *   RootNode = NULL_PTR(MDSplus::TreeNode *)
     *   currentNode = NULL_PTR(MDSplus::TreeNode *)
     *   internallyCreated = false;
     *   isOpen = false;
     * @return true if the tree is closed.
     */
    bool CloseTree();

    /**
     * @brief Creates a tree
     * @details creates a tree if it is not created. If the tree already exist and force is false no
     * tree is created but the function exits with true value.
     * @param[in] treeName tree name to be created
     * @param[in] force Indicates if the tree must be created even it is already created.
     * @return true if the tree is created or if it already exists
     */
    bool CreateTree(const char8 * const treeName, const bool force = false) const;

    /**
     * @brief Save all the changes done to the tree.
     * @details If the tree is not opened the function return false.
     * If the tree is open but the mode is not editable the function return false.
     * @return true if the changes are successfully saved
     */
    bool SaveTree();

    /**
     * @brief Queries if the tree is open
     * @details Queries if the tree is open. Even the tree is open the tree nodes may are not valid
     * since the tree can be opened externally and passed to the MDSStructuredDataI
     * @return true if the tree is open
     */
    bool IsOpen () const;

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

    /**
     * Indicates if the tree is open
     */
    bool isOpen;

    /**
     * @brief creates a set of nodes specified in the path
     * @details given a path the function creates the nodes if they do not exist. Each node must be separated
     * by . string. If the function succeeds the current node is updated with the last child created.
     * i.e "A.B.C" create (if they do not exist) the node A, a child of A called B and a child of
     * B called C, the currentNode = C.
     * @return true if the path is created.
     * @pre currentNode != NULL_PTR(MDSplus::TreeNode *)
     * editModeSet = true
     */
    bool CreateNodes(const MARTe::char8 * const path);

    /**
     * @brief Create a child node to the current node position.
     * @details Creates a child node from the current node and updates the current node with the new child created
     * @pre currentNode != NULL_PTR(MDSpluse::TreeNode *)
     * editModeSet = true
     * @return true if the node is added.
     */
    bool AddChildToCurrentNode(const MARTe::char8 * const path);
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSStructuredDataI_H_ */
