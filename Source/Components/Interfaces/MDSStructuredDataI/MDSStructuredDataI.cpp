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

#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "MDSStructuredDataI.h"
#include "Reference.h"
#include "AdvancedErrorManagement.h"
#include "TypeConversion.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

MDSStructuredDataI::MDSStructuredDataI() :
        Object() {
    currentNode = NULL_PTR(MDSplus::TreeNode *);
    rootNode = NULL_PTR(MDSplus::TreeNode *);
    tree = NULL_PTR(MDSplus::Tree *);
    editModeSet = false;
    internallyCreated = false;
    isOpen = false;
}

//lint -e{1551} Function may throw exception --> The exceptions are not managed
//lint -e{1579} Pointer member might have been freed by a separate function --> CloseTree checks if the pointers were freed previously
MDSStructuredDataI::~MDSStructuredDataI() {
    //lint -e{534} Ignoring return value of function --> The destructor is a void function the returned value of CloseTree cannot be used
    if (IsOpen()) {
        CloseTree();
    }
}

bool MDSStructuredDataI::Read(const char8* const name,
                              const AnyType& value) {
    bool ok = IsOpen();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. The tree is closed! Open it before calling read");
    }
    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
    if (ok) {
        //lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::rootNode' in left argument to operator '->'--> rootNode is not NULL because IsOpen() ensure that
        //the pointer is not NULL.
        try {
            node = currentNode->getNode(name);
        }
        catch (const MDSplus::MdsException &exc) {
            node = NULL_PTR(MDSplus::TreeNode *);
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Node %s does not exist: %s", name, exc.what());
            ok = false;
        }
    }
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Lokking for node Node %s", name);
    //lint -e{613} Possible use of null pointer 'node' in left argument to operator '->' --> if node is NULL ok is false and hence node never used.
    if (ok) {
        MDSplus::Data *dataD = node->getData();
        TypeDescriptor marteType = value.GetTypeDescriptor();
        void *data = NULL_PTR(void *);
        int32 numberOfElements = 0;
        if (marteType == UnsignedInteger8Bit) {
            data = dataD->getByteUnsignedArray(&numberOfElements);
        }
        else if (marteType == SignedInteger8Bit) {
            data = dataD->getByteArray(&numberOfElements);
        }
        else if (marteType == UnsignedInteger16Bit) {
            data = dataD->getShortUnsignedArray(&numberOfElements);
        }
        else if (marteType == SignedInteger16Bit) {
            data = dataD->getShortUnsignedArray(&numberOfElements);
        }
        else if (marteType == UnsignedInteger32Bit) {
            data = dataD->getIntUnsignedArray(&numberOfElements);
        }
        else if (marteType == SignedInteger32Bit) {
            data = dataD->getIntArray(&numberOfElements);
        }
        else if (marteType == UnsignedInteger64Bit) {
            data = dataD->getLongUnsignedArray(&numberOfElements);
        }
        else if (marteType == SignedInteger64Bit) {
            data = dataD->getLongArray(&numberOfElements);
        }
        else if (marteType == Float32Bit) {
            data = dataD->getFloatArray(&numberOfElements);
        }
        else if (marteType == Float64Bit) {
            data = dataD->getDoubleArray(&numberOfElements);
        }
        else if (marteType == CharString) {
            data = dataD->getString();
            StreamString auxStream = reinterpret_cast<char8 *>(data);
            numberOfElements = static_cast<int32>(auxStream.Size());
        }
        else if (marteType == ConstCharString) {
            data = dataD->getString();
            StreamString auxStream = reinterpret_cast<const char8 *>(data);
            numberOfElements = static_cast<int32>(auxStream.Size());
        }
        else {
            data = dataD->getString();
            StreamString auxStream = reinterpret_cast<char8 *>(data);
            numberOfElements = static_cast<int32>(auxStream.Size());
            marteType = UnsignedInteger8Bit;
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Not Valid Type %s %d  bits %d  %d",reinterpret_cast<char8 *>(data),numberOfElements,marteType.numberOfBits,(static_cast<uint32>(numberOfElements) * marteType.numberOfBits) / 8u);
        }

        ok = MemoryOperationsHelper::Copy(value.GetDataPointer(), data, (static_cast<uint32>(numberOfElements) * marteType.numberOfBits) / 8u);
        MDSplus::deleteData(dataD);
    }
    return ok;
}

bool MDSStructuredDataI::Write(const char8 * const name,
                               const AnyType &value) {
    bool ok = IsOpen();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree is closed. Open it before calling Write()");
    }
    if(ok){
        ok = editModeSet;
        if(!ok){
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Tree is not in editable mode");
        }
    }
    if (ok) {
        try {
            //lint -e{534} Ignoring return value of function --> It is a mechanism to test if tree pointers are valid
            //lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::rootNode' in left argument to operator '->'--> rootNode is not NULL because IsOpen() ensure that
            //the pointer is not NULL.
            rootNode->getNodeName();
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Impossible to write: %s", exc.what());
            ok = false;
        }
    }
    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
    if (ok) {
        try {
            //lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::rootNode' in left argument to operator '->'--> currentNode is not NULL because IsOpen() ensure that
            //the pointer is not NULL.
            node = currentNode->getNode(name);
        }
        //lint -e{715} Symbol 'exc' (line 190) not referenced [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12] --> only used to catch the exception
        catch (const MDSplus::MdsException &exc) {
            node = NULL_PTR(MDSplus::TreeNode *);
            //REPORT_ERROR_STATIC(ErrorManagement::Information, "System error: %s. Node %s not found try to add it.", exc.what(), name);
        }
        if (node == NULL_PTR(MDSplus::TreeNode *)) {
            //lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::rootNode' in left argument to operator '->'--> currentNode is not NULL because IsOpen() ensure that
            //the pointer is not NULL.
            node = currentNode->addNode(name, "ANY");
            REPORT_ERROR(ErrorManagement::Debug, "going to create Node %s", name);
        }
    }
    if (ok) {
        ok = (node != NULL_PTR(MDSplus::TreeNode *));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Node %s does not exist and cannot be created", name);
        }
    }
    //lint -e{429} Custodial pointer 'data' (line 181) has not been freed or returned --> If it is initialised it is freed via MDSplus::deleteData(data)
    if (ok) {
        MDSplus::Data *data = NULL_PTR(MDSplus::Data *);
        TypeDescriptor mdsType = value.GetTypeDescriptor();
        if (mdsType == UnsignedInteger8Bit) {
            data = new MDSplus::Uint8Array(reinterpret_cast<uint8 *>(value.GetDataPointer()), static_cast<int32>(value.GetNumberOfElements(0u)));
        }
        else if (mdsType == SignedInteger8Bit) {
            data = new MDSplus::Int8Array(reinterpret_cast<char8 *>(value.GetDataPointer()), static_cast<int32>(value.GetNumberOfElements(0u)));
        }
        else if (mdsType == UnsignedInteger16Bit) {
            data = new MDSplus::Uint16Array(reinterpret_cast<uint16 *>(value.GetDataPointer()), static_cast<int32>(value.GetNumberOfElements(0u)));
        }
        else if (mdsType == SignedInteger16Bit) {
            data = new MDSplus::Int16Array(reinterpret_cast<int16 *>(value.GetDataPointer()), static_cast<int32>(value.GetNumberOfElements(0u)));
        }
        else if (mdsType == UnsignedInteger32Bit) {
            data = new MDSplus::Uint32Array(reinterpret_cast<uint32 *>(value.GetDataPointer()), static_cast<int32>(value.GetNumberOfElements(0u)));
        }
        else if (mdsType == SignedInteger32Bit) {
            data = new MDSplus::Int32Array(reinterpret_cast<int32 *>(value.GetDataPointer()), static_cast<int32>(value.GetNumberOfElements(0u)));
        }
        else if (mdsType == UnsignedInteger64Bit) {
            data = new MDSplus::Uint64Array(reinterpret_cast<uint64_t *>(value.GetDataPointer()), static_cast<int32>(value.GetNumberOfElements(0u)));
        }
        else if (mdsType == SignedInteger64Bit) {
            data = new MDSplus::Int64Array(reinterpret_cast<int64_t *>(value.GetDataPointer()), static_cast<int32>(value.GetNumberOfElements(0u)));
        }
        else if (mdsType == Float32Bit) {
            data = new MDSplus::Float32Array(reinterpret_cast<float32 *>(value.GetDataPointer()), static_cast<int32>(value.GetNumberOfElements(0u)));
        }
        else if (mdsType == Float64Bit) {
            data = new MDSplus::Float64Array(reinterpret_cast<float64 *>(value.GetDataPointer()), static_cast<int32>(value.GetNumberOfElements(0u)));
        }
        else if (mdsType == CharString) {
            StreamString auxStream = reinterpret_cast<char8 *>(value.GetDataPointer());
            data = new MDSplus::String(reinterpret_cast<char8 *>(value.GetDataPointer()), static_cast<int32>(auxStream.Size()));
        }
        else if (mdsType == ConstCharString) {
            StreamString auxStream = reinterpret_cast<char8 *>(value.GetDataPointer());
            data = new MDSplus::String(reinterpret_cast<const char8 *>(value.GetDataPointer()), static_cast<int32>(auxStream.Size()));
        }
        else if (mdsType == Character8Bit) {
            data = new MDSplus::String(reinterpret_cast<const char8 *>(value.GetDataPointer()), static_cast<int32>(value.GetNumberOfElements(0u)));
        }
        else {
            ok = false;
        }
        if (ok) {
            //lint -e{613} Possible use of null pointer 'node' in left argument to operator '->' --> if the data == NULL then ok is false
            node->putData(data);
            MDSplus::deleteData(data);
        }
    }
    return ok;
}

AnyType MDSStructuredDataI::GetType(const char8* const name) {
    AnyType at;
    MDSplus::TreeNode * node = NULL_PTR(MDSplus::TreeNode *);
    int32 numberOfElements = 0;
    bool ok = IsOpen();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree is closed. Open it before calling GetType()");
    }
    if (ok) {
        //lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::rootNode' in left argument to operator '->'--> currentNode is not NULL because IsOpen() ensure that
        //the pointer is not NULL.
        try {
            node = currentNode->getNode(name);
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Impossible to GetType: %s", exc.what());
            node = NULL_PTR(MDSplus::TreeNode *);
        }
        if (node != NULL_PTR(MDSplus::TreeNode *)) {
            StreamString mdsType = node->getDType();
            TypeDescriptor marteType;
            MDSplus::Data *dataD = node->getData();
            if (mdsType == "DTYPE_BU") {
                numberOfElements = dataD->getSize();
                marteType = UnsignedInteger8Bit;
            }
            else if (mdsType == "DTYPE_B") {
                numberOfElements = dataD->getSize();
                marteType = SignedInteger8Bit;
            }
            else if (mdsType == "DTYPE_WU") {
                numberOfElements = dataD->getSize();
                marteType = UnsignedInteger16Bit;
            }
            else if (mdsType == "DTYPE_W") {
                numberOfElements = dataD->getSize();
                marteType = SignedInteger16Bit;
            }
            else if (mdsType == "DTYPE_LU") {
                numberOfElements = dataD->getSize();
                marteType = UnsignedInteger32Bit;
            }
            else if (mdsType == "DTYPE_L") {
                numberOfElements = dataD->getSize();
                marteType = SignedInteger32Bit;
            }
            else if (mdsType == "DTYPE_QU") {
                numberOfElements = dataD->getSize();
                marteType = UnsignedInteger64Bit;
            }
            else if (mdsType == "DTYPE_Q") {
                numberOfElements = dataD->getSize();
                marteType = SignedInteger64Bit;
            }
            else if (mdsType == "DTYPE_FS") {
                numberOfElements = dataD->getSize();
                marteType = Float32Bit;
            }
            else if (mdsType == "DTYPE_FT") {
                numberOfElements = dataD->getSize();
                marteType = Float64Bit;
            }
            else if (mdsType == "DTYPE_T") {
                void * data = dataD->getString();
                StreamString auxString = reinterpret_cast<char8 *>(data);
                numberOfElements = static_cast<int32>(auxString.Size());
                marteType = CharString;
            }
            else {
            }
            at = AnyType(marteType, 0u, NULL_PTR(void *));
            at.SetNumberOfElements(0u, static_cast<uint32>(numberOfElements));
            MDSplus::deleteData(dataD);
        }
    }
    return at;
}

bool MDSStructuredDataI::Copy(StructuredDataI& destination) {
    bool ok = IsOpen();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree closed. Open it before calling Copy");
    }
    uint32 numberOfChildren = 0u;
    if(ok){
        numberOfChildren = GetNumberOfChildren();
    }
    for (uint32 i = 0u; (i < numberOfChildren) && (ok); i++) {
        ok = MoveToChild(i);
        if (ok) {
            ok = destination.CreateRelative(GetName());

            int32 numDescendants = 0;
            try {
                MDSplus::TreeNode **descendants = currentNode->getDescendants(&numDescendants);
                //Traverse hierarchy
                int32 d = 0;
                for(d = 0; (d < numDescendants) && (ok); d++){ 
                    char8* name = descendants[d]->getNodeName();
                    int32 descNumberOfChildren = descendants[d]->getNumChildren();
                    if(descNumberOfChildren == 0) {
                        MDSplus::Data *dataD = descendants[d]->getData();
                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Descendant %s %s", name, dataD->getString());
                        ok = destination.Write(name, dataD->getString());
                    }
                }
                for(d = 0; d < numDescendants; d++) {
                    delete descendants[d];
                }
                if (descendants != NULL_PTR(MDSplus::TreeNode **)) {
                    delete [] descendants;
                }
            }
            catch (const MDSplus::MdsException &exc) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error::%s", exc.what());
            }
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
//lint -e{715} Symbol 'node' (line 319) not referenced --> function not supported
bool MDSStructuredDataI::AddToCurrentNode(Reference node) {
    return false;
}

bool MDSStructuredDataI::MoveToRoot() {
    bool ret = IsOpen();
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree closed. Open it before calling MoveToRoot");
    }
    if (ret) {
        //lint -e{534} Ignoring return value of function --> It is a mechanism to test if tree pointers are valid
        //lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::rootNode' in left argument to operator '->'--> rootNode is not NULL because IsOpen() ensure that
        //the pointer is not NULL.
        try {
            currentNode->getNodeName();
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Impossible to move root: %s", exc.what());
            ret = false;
        }
    }
//lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::rootNode' in left argument to operator '->'--> rootNode is not NULL because IsOpen() ensure that
//the pointer is not NULL.
    if (ret) {
        currentNode = rootNode;
    }
    return ret;
}

bool MDSStructuredDataI::MoveToAncestor(const uint32 generations) {
    bool ok = IsOpen();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree closed. Open it before calling MoveToAncestor");
    }
    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
//lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::currentNode' in left argument to operator '->'--> currentNode is not NULL because IsOpen() ensures that
//the pointer is not NULL.
    if (ok) {
        node = currentNode;
    }
    uint32 i = 0u;
    if (ok) {
        while ((i < generations) && (node != NULL_PTR(MDSplus::TreeNode *))) {
            try {
                node = node->getParent();
            }
            catch (const MDSplus::MdsException &exc) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Impossible to move ancestor: %s", exc.what());
                ok = false;
            }
            i++;
        }
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
    bool ok = IsOpen();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree closed. Open it before calling MoveAbsolute.");
    }
//lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::tree' in left argument to operator '->' IsOpen guarantees that the node is not NULL
    if (ok) {
        try {
            node = tree->getNode(path);
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Impossible to move to %s: %s", path, exc.what());
            node = NULL_PTR(MDSplus::TreeNode *);
            ok = false;
        }
    }
    if (ok) {
        currentNode = node;
    }
    return ok;
}

bool MDSStructuredDataI::MoveRelative(const char8* const path) {
    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
    bool ok = IsOpen();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree closed. Open it before calling MoveRelative");
    }
    if (ok) {
        //lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::currentNode' in left argument to operator '->'. IsOpen() ensures that the currentNode is not NULL
        try {
            node = currentNode->getNode(path);
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not move to %s: %s", path, exc.what());
            node = NULL_PTR(MDSplus::TreeNode *);
            ok = false;
        }
    }
    if (ok) {
        //lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::node' --> It was previously check. if node == NULL ok = false
        currentNode = node;
    }
    return ok;
}

bool MDSStructuredDataI::MoveToChild(const uint32 childIdx) {
    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
    bool ok = IsOpen();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree closed. Open it before calling MoveToChild()");
    }
//lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::tree' in left argument to operator '->' IsOpen() guarantees that the currentNode is not NULL
    if (ok) {
        try {
            node = currentNode->getChild();
        }
        catch (const MDSplus::MdsException &exc) {
            node = NULL_PTR(MDSplus::TreeNode *);
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Impossible to get child %u: %s", childIdx, exc.what());
            ok = false;
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
        catch (const MDSplus::MdsException &exc) {        //Never should happen
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Child node does not exist. System error %s", exc.what());
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
    bool ok = IsOpen();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error, Tree closed. Open it before calling CreateAbsolute()");
    }
    if (ok) {
        ok = editModeSet;
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Trying to modify tree but it is not open in edit mode");
        }
    }
    MDSplus::TreeNode *auxNode = NULL_PTR(MDSplus::TreeNode *);
    if (ok) {
        auxNode = currentNode;
        currentNode = rootNode;
        ok = CreateNodes(path);
        if (!ok) {
            currentNode = auxNode;
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
        ok = IsOpen();
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Relative node cannot be created because the tree is not opened");
        }
    }
    if (ok) {
        ok = CreateNodes(path);
    }
    return ok;
}

bool MDSStructuredDataI::Delete(const char8* const name) {
    bool ok = editModeSet;
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. The node = %s  of the tree cannot be deleted because the tree is not in editable mode", name);
    }
    if (ok) {
        ok = IsOpen();
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. node = %s of the tree cannot be deleted because the tree is not opened", name);
        }
        if (ok) {
            try {
                //lint -e{613} Possible use of null pointer --> Not Possible because IsOpen() returns false if tree == NULL
                currentNode->remove(name);
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
    bool ok = IsOpen();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree is closed. Open it before calling GetName()");
    }
    if (ok) {
        //lint -e{613} Possible use of null pointer --> Not possible because IsOpen() return false if currentNode = NULL
        retChar = currentNode->getNodeName();
    }
    return retChar;
}

const char8* MDSStructuredDataI::GetChildName(const uint32 index) {
    const char8* ret = NULL_PTR(const char8*);
    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);
    bool ok = IsOpen();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree is closed. Open it before calling GetChildName()");
    }
    if (ok) {
        //lint -e{613} Possible use of null pointer --> Not possible because IsOpen() checks that the pointer is not NULL
        try {
            node = currentNode->getChild();
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error::%s", exc.what());
            ok = false;
        }
    }
    if (ok) {
        ok = (index < GetNumberOfChildren()); //If the index is too high for sure the node name doesn't exist
    }
    uint32 i = 0u;
    if (ok) {
        while ((i < index) && (node != NULL_PTR(MDSplus::TreeNode *))) {
            try {
                node = node->getBrother();
            }
            catch (const MDSplus::MdsException &exc) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "No more brothers to consume. Child not found::%s", exc.what());
                node = NULL_PTR(MDSplus::TreeNode *);
            }
            i++;
        }
        if (node != NULL_PTR(MDSplus::TreeNode *)) {
            ret = node->getNodeName();
        }
    }
    return ret;
}

uint32 MDSStructuredDataI::GetNumberOfChildren() {
    uint32 ret = 0u;
    bool ok = IsOpen();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree is closed. Open it before calling GetNumberOfChildren()");
        ret = 0u;
    }
    else {
        //lint -e{613} Possible use of null pointer --> Not possible because IsOpen() checks that the pointer is not NULL
        try {
            ret = static_cast<uint32>(currentNode->getNumChildren());
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error::%s", exc.what());
            ret = 0u;
        }
    }
    return ret;
}

bool MDSStructuredDataI::SetTree(MDSplus::Tree * const treeIn) {
    bool ret = !IsOpen();
//lint -e{613} Possible use of null pointer --> Not possible because IsOpen() checks that the pointer is not NULL
    if (!ret) {
        const char8 * treeNameOpened = tree->getName();
        int32 treeShot = MDSplus::Tree::getCurrent(treeNameOpened);
        REPORT_ERROR_STATIC(
                ErrorManagement::FatalError,
                "The tree cannot be set because tree = %s with pulse number = %d is already opened. Please close the current tree before set a new tree",
                tree->getName(), treeShot);
    }
    if (ret) {
        tree = treeIn;
        rootNode = tree->getDefault();
        currentNode = rootNode;
        editModeSet = tree->isOpenForEdit();
        isOpen = true;
    }
    return ret;
}

bool MDSStructuredDataI::SetEditMode(const bool edit) {
    editModeSet = edit;
    bool ok = true;
    if (tree != NULL_PTR(MDSplus::Tree*)) {
        try {
            tree->edit(edit);
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error changing the mode:  %s", exc.what());
            ok = false;
        }
    }
    return ok;
}

bool MDSStructuredDataI::OpenTree(const char8 * const treeName,
                                  int32 pulseNumber) {
    bool ok = (tree == NULL_PTR(MDSplus::Tree *));
//lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::tree' in left argument to operator '->'. tree != NULL since is previously checked
    if (!ok) {
        StreamString openedTreeName(tree->getName());
        ok = (openedTreeName == treeName);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                                "The tree is already opened. Trying to open %s current opened tree name = %s and pulse number = %d", treeName, tree->getName(),
                                MDSplus::Tree::getCurrent(tree->getName()));
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
        //lint -e{613} Possible use of null pointer 'MARTe::MDSStructuredDataI::tree' in left argument to operator '->' --> tree pointer check previously.
        rootNode = tree->getDefault();
        currentNode = rootNode;
        internallyCreated = true;
        isOpen = true;
    }
    return ok;
}

bool MDSStructuredDataI::CloseTree() {
    bool ret = IsOpen();
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error closing the tree. Tree was not opened");
    }
    if (ret) {
        if (internallyCreated) {
            delete tree;
        }
        tree = NULL_PTR(MDSplus::Tree *);

        if (rootNode != NULL_PTR(MDSplus::TreeNode *)) {
            if (internallyCreated) {
                delete rootNode;
            }
            rootNode = NULL_PTR(MDSplus::TreeNode *);
        }
        if (currentNode != NULL_PTR(MDSplus::TreeNode *)) {
            currentNode = NULL_PTR(MDSplus::TreeNode *);
        }
    }
    internallyCreated = false;
    isOpen = false;
    return ret;
}

bool MDSStructuredDataI::SaveTree() {
    bool ret = IsOpen();
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error saving the tree because the tree is closed.");
    }
    if (ret) {
        ret = editModeSet;
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error saving tree because it is not in editable mode");
        }
        if (ret) {
            try {
                //lint -e{613} Possible use of null pointer --> Not possible because ret is true
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

bool MDSStructuredDataI::IsOpen() const {
    return isOpen;
}

bool MDSStructuredDataI::CreateTree(const char8 * const treeName,
                                    const bool force) const{
    bool ret = true;
    MDSplus::Tree *auxTree = NULL_PTR(MDSplus::Tree *);
    if (!force) {
        try {
            auxTree = new MDSplus::Tree(treeName, -1);
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Tree %s does not exist. Trying to create tree. System info : %s", treeName, exc.what());
        }
    }
    if ((auxTree == NULL_PTR(MDSplus::Tree *)) || force) {
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
    StreamString pathStr = path;
    bool ok = IsOpen();
    if (!ok) { //since it is a private function. IsOpen was already check. So this statment is always true
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree is closed. Open it before calling CreateNode()");
    }
    if (ok) {
        ok = pathStr.Seek(0Lu);
    }
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
                ok = MoveToChild(i - 1u);
            }
            else {
                ok = AddChildToCurrentNode(token.Buffer());
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

bool MDSStructuredDataI::AddChildToCurrentNode(const MARTe::char8 * const path) {
    bool ok = IsOpen();
    if (!ok) { //Since it is private and IsOpen() is already checked this statement always evaluates true.
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree is closed. Open it before calling AddChildToCurrentNode()");
    }
    if (ok) { //Since it is private and editModeSet is already checked this statement always evaluates true.
        ok = editModeSet;
    }
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error. Tree is not in editable mode");
    }

    MDSplus::TreeNode *node = NULL_PTR(MDSplus::TreeNode *);

    if (ok) {
        try {
            //lint -e{613} Possible use of null pointer --> Not possible because ok checks that the pointer is not NULL
            node = currentNode->addNode(path, "STRUCTURE");
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Fail to create node %s: %s", path, exc.what());
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

    return ok;
}

CLASS_REGISTER(MDSStructuredDataI, "1.0")
}

