/**
 * @file MDSObjectConnection.h
 * @brief Source file for class MDSObjectConnection
 * @date 16/09/2024
 * @author Cristian Galperti
 * @author Nicolo Ferron
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
 * the class MDSObjectConnection (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MDSObjectConnection.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

MDSObjectConnection::MDSObjectConnection() :
        ObjectConnectionI() {
    mdsTree = NULL_PTR(MDSplus::Tree*);
    mdsConnection = NULL_PTR(MDSplus::Connection*);
}

MDSObjectConnection::~MDSObjectConnection() {

    if (mdsTree != NULL_PTR(MDSplus::Tree*)) {
        delete mdsTree;
    }
    if (mdsConnection != NULL_PTR(MDSplus::Connection*)) {
        mdsConnection->closeAllTrees();
        delete mdsConnection;
    }

    ErrorManagement::ErrorType ret = Clean();
    if (!ret) {
        REPORT_ERROR(ret, "[%s] - Failed freeing memory in destructor.", GetName());
    }
}

ErrorManagement::ErrorType MDSObjectConnection::Clean() {

    ErrorManagement::ErrorType ret = ObjectConnectionI::Clean();

    while ( (deallocationList.GetSize() > 0u) && ret ) {
        void* toDelete;
        ret.notCompleted = !deallocationList.Extract(0u, toDelete);
        if (ret) {
            ret.notCompleted = !HeapManager::Free(toDelete);
        }
    }

    return ret;
}

bool MDSObjectConnection::Initialise(StructuredDataI & data) {

    status.initialisationError = !ObjectConnectionI::Initialise(data);

    if (status) {
        status.parametersError = !data.Read("Tree", treeName);
        if (status.parametersError) {
            REPORT_ERROR(status, "[%s] - 'Tree' parameter not found", GetName());
        }
    }
    if (status) {
        status.parametersError = !data.Read("Shot", shotNumber);
        if (status.parametersError) {
            REPORT_ERROR(status, "[%s] - 'Shot' parameter not found", GetName());
        }
    }
    if (status) {
        // ClientType declared
        if (data.Read("ClientType", clientTypeName)) {
            if (clientTypeName == "Thin") {
                clientType = ThinClient;
                status.internalSetupError = !data.Read("Server", serverName);
                if (status.internalSetupError) {
                    REPORT_ERROR(status, "[%s] - 'Server' parameter not found. 'Server' parameter is required if 'ClientType = Thin'.", GetName());
                }
            }
            else if (clientTypeName == "Distributed") {
                clientType = DistributedClient;
            }
            else {
                status.unsupportedFeature = true;
                REPORT_ERROR(status, "[%s] - 'ClientType' parameter can only be `Thin` or `Distributed`", GetName());
            }
        }
        // ClientType not declared
        else {
            if (data.Read("Server", serverName)) {
                clientType = ThinClient;
                REPORT_ERROR(ErrorManagement::Warning, "[%s] - 'ClientType' not set. By default it is set to 'Thin'", GetName());
            }
            else {
                clientType = DistributedClient;
                REPORT_ERROR(ErrorManagement::Warning, "[%s] - 'ClientType' not set. By default it is set to 'Distributed'", GetName());
            }
        }
    }

    // open MDSplus tree
    if (status) {
        try {
            if (clientType == DistributedClient) {
                mdsTree = new MDSplus::Tree(treeName.Buffer(), shotNumber, "NORMAL");
            } else if (clientType == ThinClient) {
                mdsConnection = new MDSplus::Connection(serverName.BufferReference());
                mdsConnection->openTree(treeName.BufferReference(), shotNumber);
            }
            else {
                status.exception = true;
                REPORT_ERROR(status, "[%s] - Invalid client type.", GetName());
            }
        }
        catch (MDSplus::MdsException &ex) {
            status.exception = true;
            REPORT_ERROR(status, "[%s] - MDSplus error opening tree %s. MDSplus error: \n%s", GetName(), treeName.Buffer(), ex.what());
        }
    }

    if (status) {

        status.parametersError = !data.MoveRelative("Parameters");
        if (status.parametersError) {
            REPORT_ERROR(status, "[%s] - 'Parameters' node not found", GetName());
        }

        if (status) {
            status.exception = !data.Copy(parametersCdb);
            if (data.MoveToAncestor(1u)) {}
        }

        if (status) {
            status = UpdateParameters();
            if (!status) {
                REPORT_ERROR(status, "[%s] - Failed 'UpdateParameters' in Initialise.", GetName());
            }
        }
    }


    return status.ErrorsCleared();
}

ErrorManagement::ErrorType MDSObjectConnection::UpdateParameters() {

    ErrorManagement::ErrorType ret = Clean();

    // traverse and flatten the `Parameters` node (iteratively to avoid recursion)
    StaticList<StreamString*> nodeStack;

    // add root node to the stack
    StreamString* currentNodePtr = new StreamString("");
    ret.exception = !nodeStack.Add(currentNodePtr);

    while ((nodeStack.GetSize() > 0u) && ret) {

        StreamString stackNodePath = "";
        StreamString separator = "";

        // pop element from the stack
        stackNodePath = *(nodeStack[nodeStack.GetSize() - 1u]);
        delete nodeStack[nodeStack.GetSize() - 1u];
        ret.exception = !nodeStack.Remove(nodeStack.GetSize() - 1u);

        if ((stackNodePath.Size() > 0u) && ret) { // not on the root node
            ret.illegalOperation = !parametersCdb.MoveAbsolute(stackNodePath.Buffer());
            separator = ".";
            if (ret.illegalOperation) {
                REPORT_ERROR(ret, "[%s] - failed MoveAbsolute()", GetName());
            }
        }

        for (uint32 elemIdx = 0u; (elemIdx < parametersCdb.GetNumberOfChildren()) && ret; elemIdx++) {

            StreamString currentNodePath = stackNodePath;
                currentNodePath += separator;
                currentNodePath += parametersCdb.GetChildName(elemIdx);

            if (parametersCdb.MoveToChild(elemIdx)) {

                // tell apart if current node contains only leaves or also subnodes
                bool isParameter = true;
                for (uint32 subElemIdx = 0u; subElemIdx < parametersCdb.GetNumberOfChildren(); subElemIdx++) {
                    if (parametersCdb.MoveToChild(subElemIdx)) {
                        isParameter = false;
                        if (parametersCdb.MoveToAncestor(1u)) {}
                    }
                }

                // is node with subnodes: add to the stack
                if (!isParameter) {
                    currentNodePtr = new StreamString(currentNodePath);
                    ret.exception = !nodeStack.Add(currentNodePtr);
                }
                // is node with leaves: add parameter to this connection
                else {
                    // substitute any dash `-` with dots `.` for retrocompatibility
                    while (currentNodePath.Locate("-") != -1) {
                        int32 dashIdx = currentNodePath.Locate("-");
                        (currentNodePath.BufferReference())[dashIdx] = '.';
                    }

                    ConfigurationDatabase nodeParameters;
                    ret.illegalOperation = !parametersCdb.Copy(nodeParameters);
                    if (ret) {
                        ret = ConnectParameter(currentNodePath, nodeParameters);
                    }
                }

                if (parametersCdb.MoveToAncestor(1u)) {}
            }
        }

        if ((stackNodePath.Size() > 0u) && ret) { // not on the root node
            ret.illegalOperation = !parametersCdb.MoveToAncestor(1u);
        }
    }


    return ret;
}


ErrorManagement::ErrorType MDSObjectConnection::ConnectParameter(StreamString nodeName, ConfigurationDatabase nodeParams) {

    ErrorManagement::ErrorType ret = ErrorManagement::NoError;

    StreamString MDSPath;
    StreamString expandedMDSPath;
    uint32 startIdx  = 0u;
    uint32 stopIdx   = 0u;
    uint32 targetDim = 0u;
    StreamString orientation = "";
    bool unlinked = false;

    if (ret) {
        ret.parametersError = !nodeParams.Read("Path", MDSPath);
        if (ret.parametersError) {
            REPORT_ERROR(ret, "[%s] - Parameter %s: no 'Path' defined.", GetName(), nodeName.Buffer());
        }
    }

    // skip parameter if Path is empty
    if (ret) {
        unlinked = (MDSPath.Size() == 0u);
        if (unlinked) {
            REPORT_ERROR(ErrorManagement::Warning, "[%s] - Parameter %s: unlinked, no value stored.", GetName(), nodeName.Buffer());
            AnyType* anyTypeParam = new AnyType(0u);
            anyTypeParam->SetStaticDeclared(false);   // unlinked
            Add(anyTypeParam);
            paramNames.Add(new StreamString(nodeName));
        }
    }

    if (!unlinked) {
        if (ret) {
            if (nodeParams.Read("StartIdx",  startIdx)) {}
            if (nodeParams.Read("StopIdx",    stopIdx)) {}
            if (nodeParams.Read("TargetDim", targetDim)) {}
            if (nodeParams.Read("DataOrientation", orientation)) {
                if ( (orientation != "RowMajor") && (orientation != "ColumnMajor") ) {
                    ret.unsupportedFeature = true;
                    REPORT_ERROR(ret, "[%s] - Parameter %s: invalid 'DataOrientation' (can only be 'RowMajor' or 'ColumnMajor')", GetName(), nodeName.Buffer());
                }
            }
            else {
                orientation = "ColumnMajor"; // default
            }
        }

        // evaluate node usage
        usage_t nodeUsage = TreeUSAGE_MAXIMUM;
        if (ret) {

            // create a valid temporary path for nodes with %u in the path
            StreamString tempMDSPath = "";
            if ((startIdx != 0u) && (stopIdx != 0u)) {
                tempMDSPath.Printf(MDSPath.Buffer(), startIdx);
            } else {
                tempMDSPath = MDSPath;
            }

            try {
                if (clientType == DistributedClient) {
                    MDSplus::TreeNode* node = mdsTree->getNode(tempMDSPath.Buffer());
                    StreamString nodeUsageString = node->getUsage();

                    if (nodeUsageString == "ANY") {
                        nodeUsage = TreeUSAGE_ANY;
                    } else if (nodeUsageString == "STRUCTURE") {
                        nodeUsage = TreeUSAGE_STRUCTURE;
                    } else if (nodeUsageString == "NUMERIC") {
                        nodeUsage = TreeUSAGE_NUMERIC;
                    } else if (nodeUsageString == "TEXT") {
                        nodeUsage = TreeUSAGE_TEXT;
                    } else {
                        nodeUsage = TreeUSAGE_MAXIMUM;
                    }
                }
                else /*if (clientType == ThinClient)*/ {
                    StreamString usageExpr = "";
                    usageExpr.Printf("GETNCI('%s', 'USAGE')", tempMDSPath.Buffer());
                    MDSplus::Data* nodeUsageData = mdsConnection->get(usageExpr.Buffer());
                    nodeUsage = static_cast<usage_t>(nodeUsageData->getShort());
                }

                ret.unsupportedFeature = !((nodeUsage == TreeUSAGE_ANY) || (nodeUsage == TreeUSAGE_STRUCTURE) || (nodeUsage == TreeUSAGE_NUMERIC) || (nodeUsage == TreeUSAGE_TEXT));
                if (ret.unsupportedFeature) {
                    REPORT_ERROR(ret, "[%s] - Parameter %s: unsupported node usage.", GetName(), nodeName.Buffer());
                }
            }
            catch (MDSplus::MdsException &ex) {
                ret.exception = true;
                REPORT_ERROR(ret, "[%s] - Parameter %s: MDSplus error getting USAGE for node %s. MDSplus error: \n%s", GetName(), nodeName.Buffer(), MDSPath.Buffer(), ex.what());
            }
        }

        // modify path
        if (ret) {
            // Modify path if TargetDim option is specified
            if (targetDim != 0u && startIdx == 0u && stopIdx == 0u) {

                StreamString tdiExpr = ""
                    " _swgTargetDim = %u;                                          "
                    " _swgVec       = %s;                                          "
                    " _swgVecSize   = SHAPE(_swgVec, 0);                           "
                    " if(_swgTargetDim > _swgVecSize)                              "
                    "     _swgPad = ZERO(_swgTargetDim - _swgVecSize, _swgVec[0]), "
                    "     _swgVec = [_swgVec, _swgPad];                            "
                    " else                                                         "
                    "     _swgVec = _swgVec[0:%u];                                 "
                    " _swgVec                                                      ";

                expandedMDSPath.Printf(tdiExpr.Buffer(), targetDim, MDSPath.Buffer(), targetDim - 1u);
            }
            // Modify path if StartIdx and StopIdx options are specified
            else if (targetDim == 0u && (startIdx != 0u || stopIdx != 0u)) {

                // Concatenate scalar values in an array in the form "[\DATA001, \DATA002, \DATA003, ...]"
                expandedMDSPath = "[";
                for (uint32 currIdx = startIdx; currIdx <= stopIdx; currIdx++)
                {
                    StreamString currNode;
                    currNode.Printf(MDSPath.Buffer(), currIdx);
                    expandedMDSPath += currNode;
                    if (currIdx != stopIdx)
                    {
                        expandedMDSPath += ", ";
                    }
                }
                expandedMDSPath += "]";
            }
            // Error
            else if (targetDim != 0u && startIdx != 0u && stopIdx != 0u) {

                ret.unsupportedFeature = true;
                REPORT_ERROR(ret, "[%s] - Parameter %s: both TargetDim and StartIdx/StopIdx used, unsupported.", GetName(), nodeName.Buffer());
            }
            // Path is ok as it is
            else {
                expandedMDSPath = MDSPath;
            }
        }

        // get the actual data
        if (ret) {
            // ordinay node, read node Data
            if (nodeUsage != TreeUSAGE_STRUCTURE) {
                MDSplus::Data* nodeData = NULL;
                try {
                    if (clientType == DistributedClient) {
                        nodeData = mdsTree->tdiExecute(expandedMDSPath.Buffer());
                    }
                    else /*if (clientType == ThinClient)*/ {
                        nodeData = mdsConnection->get(expandedMDSPath.Buffer());
                    }
                }
                catch (MDSplus::MdsException &ex) {
                    ret.exception = true;
                    REPORT_ERROR(ret, "[%s] - Parameter %s: MDSplus error getting data for node %s. MDSplus error: \n%s", GetName(), nodeName.Buffer(), MDSPath.Buffer(), ex.what());
                }

                if (ret) {
                    ret = AddAnyType(nodeName, orientation, nodeData);
                    if (!ret) {
                        REPORT_ERROR(ret, "[%s] - Parameter %s: failed loading from MDSplus", GetName(), nodeName.Buffer());
                    }
                }
            }
            // structured node, traverse all subtree
            else /*if (nodeUsage == TreeUSAGE_STRUCTURE)*/ {
                try {
                    if (clientType == DistributedClient) {

                        MDSplus::TreeNode* structNode = mdsTree->getNode(expandedMDSPath.Buffer());
                        StaticStack<MDSplus::TreeNode*> nodeStack;
                        ret.exception = !nodeStack.Push(structNode);

                        // obtain member paths relatively to structNode
                        MDSplus::TreeNode* defaultNode = mdsTree->getDefault();
                        mdsTree->setDefault(structNode);

                        while ((nodeStack.GetSize() > 0u) && ret) {
                            MDSplus::TreeNode* currentNode;
                            ret.exception = !nodeStack.Pop(currentNode);

                            // subnodes: add to the stack
                            int32 numChildren = 0;
                            MDSplus::TreeNode** childrenArray = currentNode->getChildren(&numChildren);
                            for (int32 elemIdx = 0u; (elemIdx < numChildren) && ret; elemIdx++) {
                                ret.exception = !nodeStack.Push(childrenArray[elemIdx]);
                            }

                            // leaves: add parameters to this connection
                            int32 numMembers = 0;
                            MDSplus::TreeNode** membersArray = currentNode->getMembers(&numMembers);
                            for (int32 elemIdx = 0u; (elemIdx < numMembers) && ret; elemIdx++) {
                                StreamString minPath = membersArray[elemIdx]->getMinPath();
                                while (minPath.Locate(":") != -1) {
                                    int32 dashIdx = minPath.Locate(":");
                                    (minPath.BufferReference())[dashIdx] = '.';
                                }
                                StreamString relativePath = "";
                                relativePath.Printf("%s%s%s", nodeName.Buffer(), (minPath[0u] == '.') ? "" : ".", minPath.Buffer());

                                ret = AddAnyType(relativePath, orientation, membersArray[elemIdx]->getData());
                            }
                        }

                        mdsTree->setDefault(defaultNode);
                    }
                    else /*if (clientType == ThinClient)*/ {

                        StreamString tdiCall = "";
                        tdiCall.Printf("GETNCI(%s, 'PATH')", expandedMDSPath.Buffer());
                        MDSplus::Data* structNodePath = mdsConnection->get(tdiCall.Buffer());
                        StaticStack<MDSplus::Data*> nodeStack;
                        ret.exception = !nodeStack.Push(structNodePath);

                        // obtain member paths relatively to structNode
                        mdsConnection->setDefault(structNodePath->getString());

                        while ((nodeStack.GetSize() > 0u) && ret) {
                            MDSplus::Data* currentNodePath;
                            ret.exception = !nodeStack.Pop(currentNodePath);

                            // subnodes: add to the stack
                            tdiCall = "";
                            tdiCall.Printf("GETNCI(%s, 'NUMBER_OF_CHILDREN')", currentNodePath->getString());
                            int32 numChildren = (mdsConnection->get(tdiCall.Buffer()))->getInt();
                            for (int32 elemIdx = 0u; (elemIdx < numChildren) && ret; elemIdx++) {
                                tdiCall = "";
                                tdiCall.Printf("GETNCI(GETNCI(%s, 'CHILDREN_NIDS'), 'MINPATH')[%i]", currentNodePath->getString(), elemIdx);
                                MDSplus::Data* childrenArrayElem = mdsConnection->get(tdiCall.Buffer());
                                ret.exception = !nodeStack.Push(childrenArrayElem);
                            }

                            // leaves: add parameters to this connection
                            tdiCall = "";
                            tdiCall.Printf("GETNCI(%s, 'NUMBER_OF_MEMBERS')", currentNodePath->getString());
                            int32 numMembers = (mdsConnection->get(tdiCall.Buffer()))->getInt();
                            for (int32 elemIdx = 0u; (elemIdx < numMembers) && ret; elemIdx++) {
                                tdiCall = "";
                                tdiCall.Printf("GETNCI(GETNCI(%s, 'MEMBER_NIDS'),'MINPATH')[%i]", currentNodePath->getString(), elemIdx);
                                MDSplus::Data* memberPath = mdsConnection->get(tdiCall.Buffer());

                                StreamString minPath = memberPath->getString();
                                while (minPath.Locate(":") != -1) {
                                    int32 dashIdx = minPath.Locate(":");
                                    (minPath.BufferReference())[dashIdx] = '.';
                                }
                                StreamString relativePath = "";
                                relativePath.Printf("%s%s%s", nodeName.Buffer(), (minPath[0u] == '.') ? "" : ".", minPath.Buffer());

                                ret = AddAnyType(relativePath, orientation, mdsConnection->get(minPath.Buffer()));
                            }

                        }

                        StreamString topNode = "\\TOP";
                        mdsConnection->setDefault(topNode.BufferReference());

                    }
                }
                catch (MDSplus::MdsException &ex) {
                    ret.exception = true;
                    REPORT_ERROR(ret, "[%s] - Parameter %s: MDSplus error getting structure from node %s. MDSplus error: \n%s", GetName(), nodeName.Buffer(), MDSPath.Buffer(), ex.what());
                }
            }
        }

    } // if (!unlinked) - end

    return ret;
}

ErrorManagement::ErrorType MDSObjectConnection::AddAnyType(StreamString nodeName, StreamString orientation, MDSplus::Data* const nodeData) {

    ErrorManagement::ErrorType ret = ErrorManagement::NoError;

    // Introspection information from MDSplus will be stored here
    char8 MDSDataClass; // Scalar (0) or array (4), see mdsdescrip.h
    char8 MDSDataType;
    char8 MDSNumOfDims;
    int16 MDSDataByteSize;
    int  *MDSDimArray;
    void *MDSDataPtr;

    try {
        // MDSplus C++ API are used to retrieve informations about the parameter
        nodeData->getInfo(&MDSDataClass, &MDSDataType, &MDSDataByteSize, &MDSNumOfDims, &MDSDimArray, &MDSDataPtr);
    }
    catch (MDSplus::MdsException &ex) {
        ret.exception = true;
        REPORT_ERROR(ret, "[%s] - Parameter %s: MDSplus error getting the associated node. MDSplus error: \n%s", GetName(), nodeName.Buffer(), ex.what());
    }

    if (ret) {
        ret.unsupportedFeature = (MDSNumOfDims > 3);
        if (ret.unsupportedFeature) {
            REPORT_ERROR(ret, "[%s] - Parameter %s: NumberOfDimensions > 3. Not supported.", GetName(), nodeName.Buffer());
        }
    }

    AnyType* anyTypeParam = new AnyType();
    uint8 castMdsType = 0u;

    if (ret) {

        // type
        castMdsType = static_cast<uint8>(MDSDataType);

        if (castMdsType == DTYPE_BU) {
            anyTypeParam->SetTypeDescriptor(UnsignedInteger8Bit);
        }
        else if (castMdsType == DTYPE_WU) {
            anyTypeParam->SetTypeDescriptor(UnsignedInteger16Bit);
        }
        else if (castMdsType == DTYPE_LU) {
            anyTypeParam->SetTypeDescriptor(UnsignedInteger32Bit);
        }
        else if (castMdsType == DTYPE_QU) {
            anyTypeParam->SetTypeDescriptor(UnsignedInteger64Bit);
        }
        else if (castMdsType == DTYPE_B) {
            anyTypeParam->SetTypeDescriptor(SignedInteger8Bit);
        }
        else if (castMdsType == DTYPE_W) {
            anyTypeParam->SetTypeDescriptor(SignedInteger16Bit);
        }
        else if (castMdsType == DTYPE_L) {
            anyTypeParam->SetTypeDescriptor(SignedInteger32Bit);
        }
        else if (castMdsType == DTYPE_Q) {
            anyTypeParam->SetTypeDescriptor(SignedInteger64Bit);
        }
        else if (castMdsType == DTYPE_F || castMdsType == DTYPE_FS) {
            anyTypeParam->SetTypeDescriptor(Float32Bit);
        }
        else if (castMdsType == DTYPE_D || castMdsType == DTYPE_FT) {
            anyTypeParam->SetTypeDescriptor(Float64Bit);
        }
        else if (castMdsType == DTYPE_T) {
            anyTypeParam->SetTypeDescriptor(Character8Bit);
        }
        else if (castMdsType == DTYPE_DICTIONARY || castMdsType == DTYPE_LIST) { // struct or array of sruct
            anyTypeParam->SetTypeDescriptor(StructuredDataInterfaceType);
        }
        else {
            ret.unsupportedFeature = true;
            REPORT_ERROR(ret, "[%s] - Parameter %s: unsupported MDSplus type", GetName(), nodeName.Buffer());
        }

        if (castMdsType == DTYPE_T) {
            ret.illegalOperation = !(orientation == "RowMajor");
            if (ret.illegalOperation) {
                REPORT_ERROR(ret, "[%s] - Parameter %s: is a String, ColumnMajor orientation not supported. Set `DataOrientation = RowMajor`.", GetName(), nodeName.Buffer());
            }
        }
    }


    // shape
    if (ret) {
        anyTypeParam->SetNumberOfDimensions(MDSNumOfDims);

        for (uint16 dimIdx = 0u; dimIdx < MDSNumOfDims; dimIdx++) {
            if (dimIdx <= 2u && orientation == "RowMajor") {
                anyTypeParam->SetNumberOfElements(dimIdx, MDSDimArray[MDSNumOfDims - dimIdx - 1u]);
            }
            else {
                anyTypeParam->SetNumberOfElements(dimIdx, MDSDimArray[dimIdx]);
            }
        }
    }

    // data pointer
    if (ret) {

        if (castMdsType == DTYPE_DICTIONARY) {

            MDSplus::Apd* apdData = static_cast<MDSplus::Apd*>(nodeData);

            for (uint32 itemIdx = 0u; (itemIdx < apdData->getDimension()) && (ret); itemIdx = itemIdx + 2u) {
                MDSplus::Data* itemNameField;
                MDSplus::Data* itemDataField;
                StreamString itemName = "";

                try {
                    itemNameField = apdData->getDescAt(itemIdx);
                    itemDataField = apdData->getDescAt(itemIdx + 1u);
                    itemName = nodeName;
                    itemName += ".";
                    itemName += itemNameField->getString();
                }
                catch (MDSplus::MdsException &ex) {
                    ret.exception = true;
                    REPORT_ERROR(ret, "[%s] - Parameter %s: MDSplus error getting Dictionary item %d. MDSplus error: \n%s", GetName(), nodeName.Buffer(), itemIdx/2u, ex.what());
                }

                if (ret) {
                    ret = AddAnyType(itemName, orientation, itemDataField);
                }
            }
        }
        else if (castMdsType == DTYPE_LIST) {

            MDSplus::Apd* apdData = static_cast<MDSplus::Apd*>(nodeData);

            for (uint32 itemIdx = 0u; (itemIdx < apdData->getDimension()) && (ret); itemIdx++) {
                MDSplus::Data* itemData;
                StreamString itemName = "";

                char8 itemDataClass;
                char8 itemDataType;
                char8 itemNumOfDims;
                int16 itemDataByteSize;
                int  *itemDimArray;
                void *itemDataPtr;

                try {
                    itemData = apdData->getDescAt(itemIdx);
                    itemData->getInfo(&itemDataClass, &itemDataType, &itemDataByteSize, &itemNumOfDims, &itemDimArray, &itemDataPtr);
                    itemName = nodeName;
                    itemName.Printf("[%d]", itemIdx);
                }
                catch (MDSplus::MdsException &ex) {
                    ret.exception = true;
                    REPORT_ERROR(ret, "[%s] - Parameter %s: MDSplus error getting List item %d. MDSplus error: \n%s", GetName(), nodeName.Buffer(), itemIdx, ex.what());
                }

                if (ret) {
                    ret.unsupportedFeature = ((uint8) itemDataType != DTYPE_DICTIONARY) && ((uint8) itemDataType != DTYPE_LIST);
                    if (ret.unsupportedFeature) {
                        REPORT_ERROR(ret, "[%s] - Parameter %s: not a Dictionary or List. List elements shall be all MDSplus::Dictionary or all MDSplus::List", GetName(), itemName.Buffer(), itemIdx);
                    }
                }

                if (ret) {
                    ret = AddAnyType(itemName, orientation, itemData);
                }
            }

        }
        else {
            if (orientation == "RowMajor") {
                anyTypeParam->SetDataPointer(MDSDataPtr);
            }
            else { // ColumnMajor
                // when source is col-major, a local row-major copy is Malloc'd and the anyTypeParam is associated to that copy
                void* localBuffer = HeapManager::Malloc(anyTypeParam->GetDataSize());
                ret.exception = (localBuffer == NULL_PTR(void*));
                if (ret) {

                    ret.exception = !TransposeAndCopy(localBuffer, MDSDataPtr, anyTypeParam->GetTypeDescriptor(),
                                                    anyTypeParam->GetNumberOfElements(0u),
                                                    anyTypeParam->GetNumberOfElements(1u),
                                                    anyTypeParam->GetNumberOfDimensions() > 1u ? anyTypeParam->GetNumberOfElements(2u) : 1u);

                    anyTypeParam->SetDataPointer(localBuffer);

                    // local row-major copy will be Freed in the destructor
                    deallocationList.Add(localBuffer);
                }
            }
            if (ret) {
                anyTypeParam->SetStaticDeclared(true);   // linked

                ret.fatalError = !Add(anyTypeParam);
                ret.exception  = !paramNames.Add(new StreamString(nodeName));
                if (ret) {
                    REPORT_ERROR(ret, "[%s] - Parameter %s: correctly linked", GetName(), nodeName.Buffer());
                }
            }
        }
    }

    if (!ret) {
        delete anyTypeParam;
    }

    return ret;
}

CLASS_REGISTER(MDSObjectConnection, "1.1")
} /* namespace MARTe */
