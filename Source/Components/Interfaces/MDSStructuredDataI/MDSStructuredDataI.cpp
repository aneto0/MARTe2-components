/**
 * @file MDSStructuredDataI.cpp
 * @brief Source file for class MDSStructuredDataI
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
 * the class MDSStructuredDataI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "MDSStructuredDataI.h"
#include "Reference.h"
#include "StreamString.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

MDSStructuredDataI::MDSStructuredDataI() :
        Object(),
        StructuredDataI() {
    currentNode = NULL_PTR(MDSplus::TreeNode *);
    rootNode = NULL_PTR(MDSplus::TreeNode *);
    tree = NULL_PTR(MDSplus::Tree *);
    editModeSet = false;
    internallyCreated = false;
}

MDSStructuredDataI::~MDSStructuredDataI() {
    if (internallyCreated) {
        CloseTree();
    }
}

bool MDSStructuredDataI::Read(const char8* const name,
                              const AnyType& value) {
    return false;
}

bool MDSStructuredDataI::Write(const char8 * const name,
                               const AnyType &value) {
    bool ok = true;
    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
    try {
        node = currentNode->getNode(name);
    }
    catch (const MDSplus::MdsException &exc) {
        node = NULL_PTR(MDSplus::TreeNode *);
    }
    if (node == NULL_PTR(MDSplus::TreeNode *)) {
        node = currentNode->addNode(name, "ANY");
    }
    if (ok) {
        ok = (node != NULL_PTR(MDSplus::TreeNode *));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Node %s does not exist and cannot be created", name);
        }
    }
    if (ok) {
        MDSplus::Data *data;
        TypeDescriptor mdsType = value.GetTypeDescriptor();
        if (mdsType == UnsignedInteger8Bit) {
            data = new MDSplus::Uint8(*reinterpret_cast<uint8 *>(value.GetDataPointer()));
        }
        else if (mdsType == SignedInteger8Bit) {
            data = new MDSplus::Int8(*reinterpret_cast<int8 *>(value.GetDataPointer()));
        }
        else if (mdsType == UnsignedInteger16Bit) {
            data = new MDSplus::Uint16(*reinterpret_cast<uint16 *>(value.GetDataPointer()));
        }
        else if (mdsType == SignedInteger16Bit) {
            data = new MDSplus::Int16(*reinterpret_cast<int16 *>(value.GetDataPointer()));
        }
        else if (mdsType == UnsignedInteger32Bit) {
            data = new MDSplus::Uint32(*reinterpret_cast<uint32 *>(value.GetDataPointer()));
        }
        else if (mdsType == SignedInteger32Bit) {
            data = new MDSplus::Int32(*reinterpret_cast<int32 *>(value.GetDataPointer()));
        }
        else if (mdsType == UnsignedInteger64Bit) {
            data = new MDSplus::Uint64(*reinterpret_cast<uint64 *>(value.GetDataPointer()));
        }
        else if (mdsType == SignedInteger64Bit) {
            data = new MDSplus::Int64(*reinterpret_cast<int64 *>(value.GetDataPointer()));
        }
        else if (mdsType == Float32Bit) {
            data = new MDSplus::Float32(*reinterpret_cast<float32 *>(value.GetDataPointer()));
        }
        else if (mdsType == Float64Bit) {
            data = new MDSplus::Float64(*reinterpret_cast<float64 *>(value.GetDataPointer()));
        }
        else if (mdsType == CharString) {
            data = new MDSplus::String(reinterpret_cast<char8 *>(value.GetDataPointer()));
        }
        else if (mdsType == ConstCharString) {
            data = new MDSplus::String(reinterpret_cast<const char8 *>(value.GetDataPointer()));
        }
        if (ok) {
            node->putData(data);
            MDSplus::deleteData(data);
        }
    }

    return ok;
}

AnyType MDSStructuredDataI::GetType(const char8* const name) {
    AnyType at;

    MDSplus::TreeNode *node = currentNode->getNode(name);
    if (node != NULL_PTR(MDSplus::TreeNode *)) {
        StreamString mdsType = node->getDType();
        TypeDescriptor marteType;
        if (mdsType == "DTYPE_BU") {
            marteType = UnsignedInteger8Bit;
        }
        else if (mdsType == "DTYPE_B") {
            marteType = SignedInteger8Bit;
        }
        else if (mdsType == "DTYPE_WU") {
            marteType = UnsignedInteger16Bit;
        }
        else if (mdsType == "DTYPE_W") {
            marteType = SignedInteger16Bit;
        }
        else if (mdsType == "DTYPE_LU") {
            marteType = UnsignedInteger32Bit;
        }
        else if (mdsType == "DTYPE_L") {
            marteType = SignedInteger32Bit;
        }
        else if (mdsType == "DTYPE_QU") {
            marteType = UnsignedInteger64Bit;
        }
        else if (mdsType == "DTYPE_Q") {
            marteType = SignedInteger64Bit;
        }
        else if (mdsType == "DTYPE_FS") {
            marteType = Float32Bit;
        }
        else if (mdsType == "DTYPE_FT") {
            marteType = Float64Bit;
        }
        else {

        }
        at = AnyType(marteType, 0u, NULL_PTR(void *));

    }
    return at;
}

bool MDSStructuredDataI::Copy(StructuredDataI& destination) {
    bool ok = (currentNode != NULL_PTR(MDSplus::TreeNode *));
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree not opened");
    }
    uint32 numberOfChildren = GetNumberOfChildren();
    for (uint32 i = 0u; (i < numberOfChildren) && (ok); i++) {
        if (ok) {
            ok = MoveToChild(i);
        }
        if (ok) {
            ok = destination.CreateRelative(GetName());
        }
        if (ok) {
            // go recursively !
            ok = Copy(destination);
        }
        if (ok) {
            ok = MoveToAncestor(1u);
        }
        if (ok) {
            ok = destination.MoveToAncestor(1u);
        }
    }
    return ok;
}

bool MDSStructuredDataI::AddToCurrentNode(Reference nodeRef) {

    return false;
    /*
     bool ok = editModeSet;
     if (ok) {
     MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
     ok = (currentNode != NULL_PTR(MDSplus::TreeNode *));
     if (!ok) {
     REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Relative node cannot be created because the tree is not opened");
     }
     if (ok) {
     ok = nodeRef.IsValid();
     if (!ok) {
     REPORT_ERROR_STATIC(ErrorManagement::FatalError, "node reference not valid");
     }
     }
     if (ok) {
     try {
     node = currentNode->addNode(nodeRef, "STRUCTURE");
     }
     catch (const MDSplus::MdsException &exc) {
     node = NULL_PTR(MDSplus::TreeNode *);
     ok = false;
     }
     }
     if (ok) {
     ok = (node != NULL_PTR(MDSplus::TreeNode *));
     if (ok) {
     currentNode = node;
     }
     }
     }
     return ok;
     */
}

bool MDSStructuredDataI::MoveToRoot() {
    bool ret = (rootNode != NULL_PTR(MDSplus::TreeNode *));
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree not opened");
    }
    if (ret) {
        currentNode = rootNode;
    }
    return ret;
}

bool MDSStructuredDataI::MoveToAncestor(uint32 generations) {
    bool ok = (currentNode != NULL_PTR(MDSplus::TreeNode *));
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree not opened");
    }
    MDSplus::TreeNode *node = currentNode;
    uint32 i = 0u;
    while ((i < generations) && (node != NULL_PTR(MDSplus::TreeNode *))) {
        try {
            node = node->getParent();
        }
        catch (const MDSplus::MdsException &exc) {
            ok = false;
        }
        i++;
    }
    if (ok) {
        ok = (node != NULL_PTR(MDSplus::TreeNode *));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Impossible to move ancestor");
        }
    }
    if (ok) {
        currentNode = node;
    }
    return ok;
}

bool MDSStructuredDataI::MoveAbsolute(const char8* const path) {
    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
    bool ok = (tree != NULL_PTR(MDSplus::Tree*));
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree not opened");
    }
    if (ok) {
        try {
            node = tree->getNode(path);
        }
        catch (const MDSplus::MdsException &exc) {
            node = NULL_PTR(MDSplus::TreeNode *);
        }
    }
    if (ok) {
        ok = (node != NULL_PTR(MDSplus::TreeNode *));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Path not found");
        }
    }
    if (ok) {
        currentNode = node;
    }
    return ok;
}

bool MDSStructuredDataI::MoveRelative(const char8* const path) {
    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
    bool ok = (currentNode != NULL_PTR(MDSplus::TreeNode *));
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree not opened");
    }
    if (ok) {
        try {
            node = currentNode->getNode(path);
        }
        catch (const MDSplus::MdsException &exc) {
            node = NULL_PTR(MDSplus::TreeNode *);
        }
    }
    if (ok) {
        ok = (node != NULL_PTR(MDSplus::TreeNode *));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Child node does not exist");
        }
    }
    if (ok) {
        currentNode = node;
    }
    return ok;
}

bool MDSStructuredDataI::MoveToChild(const uint32 childIdx) {
    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
    bool ok = (currentNode != NULL_PTR(MDSplus::TreeNode *));
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree not opened");
    }
    if (ok) {
        try {
            node = currentNode->getChild();
        }
        catch (const MDSplus::MdsException &exc) {
            node = NULL_PTR(MDSplus::TreeNode *);
        }
    }
    uint32 i = 0u;
    if (ok) {
        ok = (childIdx < GetNumberOfChildren());
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Child node does not exist");
        }
    }
    while ((i < childIdx) && (node != NULL_PTR(MDSplus::TreeNode *))) {
        try {
            node = node->getBrother();
        }
        catch (const MDSplus::MdsException &exc) {
            node = NULL_PTR(MDSplus::TreeNode *);
        }
        i++;
    }
    if (ok) {
        ok = (node != NULL_PTR(MDSplus::TreeNode *));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Child node does not exist");
        }
    }
    if (ok) {
        currentNode = node;
    }
    return ok;
}

bool MDSStructuredDataI::CreateAbsolute(const char8* const path) {
    bool ok = (tree != NULL_PTR(MDSplus::Tree *));
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error, tree is not opened");
    }
    if (ok) {
        ok = tree->isOpenForEdit();
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Trying to modify tree but it is not open in edit mode");
        }
    }
    if (ok) {
        MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
        try {
            node = rootNode->addNode(path, "STRUCTURE");
        }
        catch (const MDSplus::MdsException &exc) {
            node = NULL_PTR(MDSplus::TreeNode *);
        }
        ok = (node != NULL_PTR(MDSplus::TreeNode *));
        if (ok) {
            currentNode = node;
        }
    }
    return ok;
}

bool MDSStructuredDataI::CreateRelative(const char8* const path) {
    bool ok = editModeSet;
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Relative node cannot be created because the tree is not set editable");
    }
    if (ok) {
        ok = (currentNode != NULL_PTR(MDSplus::TreeNode *));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Relative node cannot be created because the tree is not opened");
        }
    }
    if (ok) {
        ok = CreateNodes(path);
    }
    return ok;
    /*
     bool ok = editModeSet;
     if (ok) {
     MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
     ok = (currentNode != NULL_PTR(MDSplus::TreeNode *));
     if (!ok) {
     REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Relative node cannot be created because the tree is not opened");
     }
     if (ok) {
     try {
     node = currentNode->addNode(path, "STRUCTURE");
     }
     catch (const MDSplus::MdsException &exc) {
     node = NULL_PTR(MDSplus::TreeNode *);
     ok = false;
     }
     }
     if (ok) {
     ok = (node != NULL_PTR(MDSplus::TreeNode *));
     if (ok) {
     currentNode = node;
     }
     }
     }
     return ok;
     */
}

bool MDSStructuredDataI::Delete(const char8* const name) {
    bool ok = editModeSet;
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. The node = %s  of the tree cannot be deleted because the tree is not in editable mode", name);
    }
    if (ok) {
        ok = (tree != NULL_PTR(MDSplus::Tree *));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. node = %s of the tree cannot be deleted because the tree is not opened", name);
        }
        if (ok) {
            try {
                tree->remove(name);
            }
            catch (const MDSplus::MdsException &exc) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Fail deleting node %s: %s", name, exc.what());
                ok = false;
            }
        }
    }
    return ok;
}

const char8* MDSStructuredDataI::GetName() {
    char8 * retChar = NULL_PTR(char8*);
    if (currentNode != NULL_PTR(MDSplus::TreeNode *)) {
        retChar = currentNode->getNodeName();
    }
    return retChar;
}

const char8* MDSStructuredDataI::GetChildName(const uint32 index) {
    const char8* ret = NULL_PTR(const char8*);
    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
    if (currentNode != NULL_PTR(MDSplus::TreeNode *)) {
        try {
            node = currentNode->getChild();
        }
        catch (const MDSplus::MdsException &exc) {
        }
    }
    if (index >= GetNumberOfChildren()) { //If the index is too high for sure the node name doesn't exist
        node = NULL_PTR(MDSplus::TreeNode *);
    }
    uint32 i = 0u;
    while ((i < index) && (node != NULL_PTR(MDSplus::TreeNode *))) {
        try {
            node = node->getBrother();
        }
        catch (const MDSplus::MdsException &exc) {
            node = NULL_PTR(MDSplus::TreeNode *);
        }
        i++;
    }
    if (node != NULL_PTR(MDSplus::TreeNode *)) {
        try {
            ret = node->getNodeName();
        }
        catch (const MDSplus::MdsException &exc) {
        }
    }
    return ret;
}

uint32 MDSStructuredDataI::GetNumberOfChildren() {
    uint32 ret = 0u;
    if (currentNode == NULL_PTR(MDSplus::TreeNode *)) {
        ret = 0u;
    }
    else {
        try {
            ret = currentNode->getNumChildren();
        }
        catch (const MDSplus::MdsException &exc) {
            ret = 0u;
        }
    }
    return ret;
}

bool MDSStructuredDataI::SetTree(MDSplus::Tree *treeIn) {
    bool ret = (tree == NULL_PTR(MDSplus::Tree *));
    if (!ret) {
        REPORT_ERROR_STATIC(
                ErrorManagement::FatalError,
                "The tree cannot be set because the tree = %s with pulse number = %d is already opened. Please close the current tree before set a new tree",
                tree->getName(), tree->getCurrent(tree->getName()));
    }
    if (ret) {
        tree = treeIn;
        rootNode = tree->getDefault();
        currentNode = rootNode;
    }
    return ret;
}

bool MDSStructuredDataI::SetEditMode(bool edit) {
    editModeSet = edit;
    bool ok = true;
    if (tree != NULL_PTR(MDSplus::Tree*)) {
        try {
            tree->edit(edit);
        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error changing the mode:  %s", exc.what());
            ok = false;
        }
    }
    return ok;
}

bool MDSStructuredDataI::OpenTree(const char8 * const treeName,
                                  int32 pulseNumber) {
    bool ok = (tree == NULL_PTR(MDSplus::Tree *));
    if (!ok) {
        StreamString openedTreeName(tree->getName());
        ok = (openedTreeName == treeName);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                                "The tree is already opened. Trying to open %s current opened tree name = %s and pulse number = %d", treeName, tree->getName(),
                                tree->getCurrent(tree->getName()));
        }
    }
    if (ok) {
        try {
            if (editModeSet) {
                //open tree in edit mode to access the tree structure for writing
                tree = new MDSplus::Tree(treeName, pulseNumber, "EDIT");
            }
            else {
                //set the tree for normal operations reading and writing data
                tree = new MDSplus::Tree(treeName, pulseNumber, "NORMAL");
            }
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Fail opening tree %s with shotNumber %d:  %s", treeName, pulseNumber, exc.what());
            ok = false;
        }
    }
    if (ok) {
        rootNode = tree->getDefault();
        currentNode = rootNode;
        internallyCreated = true;
    }
    return ok;
}

bool MDSStructuredDataI::CloseTree() {
    bool ret = (tree != NULL_PTR(MDSplus::Tree *));
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error closing the tree. Tree was not opened");
    }
    if (ret) {
        if (internallyCreated) {
            delete tree;
        }
        tree = NULL_PTR(MDSplus::Tree *);
    }
    if (rootNode != NULL_PTR(MDSplus::TreeNode *)) {
        if (internallyCreated) {
            delete rootNode;
        }
        rootNode = NULL_PTR(MDSplus::TreeNode *);
    }
    if (currentNode != NULL_PTR(MDSplus::TreeNode *)) {
        /*
         if(internallyCreated){
         delete currentNode;
         }
         */
        currentNode = NULL_PTR(MDSplus::TreeNode *);
    }
    internallyCreated = false;
    return ret;
}

bool MDSStructuredDataI::SaveTree() {
    bool ret = (tree != NULL_PTR(MDSplus::Tree *));
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error saving the tree because tree = NULL_PTR(Tree *).");
    }
    if (ret) {
        ret = editModeSet;
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error saving tree because it is not in editable mode");
        }
        if (ret) {
            try {
                tree->write();
            }
            catch (const MDSplus::MdsException &exc) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Fail saving tree structure: %s", exc.what());
                ret = false;
            }
        }
    }
    return ret;
}

bool MDSStructuredDataI::CreateTree(const char8 * const treeName,
                                    bool force) {
    bool ret = true;
    MDSplus::Tree *auxTree = NULL_PTR(MDSplus::Tree *);
    try {
//open tree in new mode
        auxTree = new MDSplus::Tree(treeName, -1);
    }
    catch (const MDSplus::MdsException &exc) {
    }
    if ((auxTree == NULL_PTR(MDSplus::Tree *)) | force) {
        try {
            //open tree in new mode
            auxTree = new MDSplus::Tree(treeName, -1, "NEW");
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Fail to create %s: %s", treeName, exc.what());
            ret = false;
        }
    }
    if (auxTree != NULL_PTR(MDSplus::Tree *)) {
        delete auxTree;
    }
    return ret;
}

bool MDSStructuredDataI::CreateNodes(const MARTe::char8 * const path) {
    using namespace MARTe;
    StreamString pathStr = path;
    bool ok = pathStr.Seek(0Lu);
    if (ok) {
        ok = (pathStr.Size() > 0u);
    }
    StreamString token;
    char8 c;
    bool created = false;
    MDSplus::TreeNode *currentNodeOld = currentNode;

    while ((pathStr.GetToken(token, ".", c)) && (ok)) {
        ok = (token.Size() > 0u);
        if (ok) {
            //Check if a node with this name already exists
            bool found = false;
            uint32 i;
            uint32 numberOfChildren = GetNumberOfChildren();
            for (i = 0u; (i < numberOfChildren) && (!found); i++) {
                //foundReference = GetChildName(i);
                found = (StringHelper::Compare(GetChildName(i), token.Buffer()) == 0);
            }
            if (found) {
                MoveToChild(i);
            }
            else {
                Reference ref1("StreamString", GlobalObjectsDatabase::Instance()->GetStandardHeap());
                ok = AddToCurrentNode(token.Buffer());
                if (ok) {
                    created = true;
                }
            }
        }
        if (ok) {
            ok = token.Seek(0Lu);
            if (ok) {
                ok = token.SetSize(0Lu);
            }

        }
    }
    if (ok) {
        ok = created;
    }
    if (!ok) {
        currentNode = currentNodeOld;
    }
    return ok;
}

CLASS_REGISTER(MDSStructuredDataI, "1.0")
}
