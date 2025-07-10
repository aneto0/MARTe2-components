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
}

MDSObjectConnection::~MDSObjectConnection() {

    while (deallocationList.GetSize() > 0u) {
        void* toDelete;
        deallocationList.Extract(0u, toDelete);
        HeapManager::Free(toDelete);
    }

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
        status.parametersError = !data.Read("Server", serverName);
        if (status.parametersError) {
            REPORT_ERROR(status, "[%s] - 'Server' parameter not found", GetName());
        }
    }
    if (status) {
        status.parametersError = !data.Read("Shot", shotNumber);
        if (status.parametersError) {
            REPORT_ERROR(status, "[%s] - 'Shot' parameter not found", GetName());
        }
    }
    if (status) {
        if (data.Read("ClientType", clientTypeName)) {
            if (clientTypeName == "Thin") {
                clientType = ThinClient;
            }
            else if (clientTypeName = "Distributed") {
                clientType = DistributedClient;
            }
            else {
                status.parametersError = true;
                REPORT_ERROR(status, "[%s] - 'ClientType' parameter can only be `Thin` or `Distributed`", GetName());
            }
        }
        else {
            clientType = ThinClient;
            REPORT_ERROR(ErrorManagement::Warning, "[%s] - 'ClientType' not set. By default it is set to Thin", GetName());
        }
    }

    if (status) {

        status.parametersError = !data.MoveRelative("Parameters");
        if (status.parametersError) {
            REPORT_ERROR(status, "[%s] - 'Parameters' node not found", GetName());
        }

        // traverse and flatten the `Parameters` node (iteratively to avoid recursion)
        if (status) {

            status.exception = !data.Copy(parametersCdb);
            StaticList<StreamString*> nodeStack;

            // add root node to the stack
            StreamString* currentNodePtr = new StreamString("");
            status.exception = !nodeStack.Add(currentNodePtr) || status.exception;

            while ((nodeStack.GetSize() > 0u) && status) {

                StreamString stackNodePath = "";
                StreamString separator = "";

                // pop element from the stack
                stackNodePath = *(nodeStack[nodeStack.GetSize() - 1u]);
                delete nodeStack[nodeStack.GetSize() - 1u];
                status.exception = !nodeStack.Remove(nodeStack.GetSize() - 1u);

                if ((stackNodePath.Size() > 0u) && status) { // not on the root node
                    status.illegalOperation = !parametersCdb.MoveAbsolute(stackNodePath.Buffer());
                    separator = ".";
                    if (status.illegalOperation) {
                        REPORT_ERROR(status, "[%s] - failed MoveAbsolute()", GetName());
                    }
                }

                for (uint32 elemIdx = 0u; (elemIdx < parametersCdb.GetNumberOfChildren()) && status; elemIdx++) {

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
                            status.exception = !nodeStack.Add(currentNodePtr);
                        }
                        // is node with leaves: add parameter to this connection
                        else {
                            // substitute any dash `-` with dots `.` for retrocompatibility
                            while (currentNodePath.Locate("-") != -1) {
                                int32 dashIdx = currentNodePath.Locate("-");
                                (currentNodePath.BufferReference())[dashIdx] = '.';
                            }

                            ConfigurationDatabase nodeParameters;
                            status.illegalOperation = !parametersCdb.Copy(nodeParameters);
                            if (status) {
                                status = ConnectParameter(currentNodePath, nodeParameters);
                            }
                        }

                        if (parametersCdb.MoveToAncestor(1u)) {}
                    }
                }

                if ((stackNodePath.Size() > 0u) && status) { // not on the root node
                    status.illegalOperation = !parametersCdb.MoveToAncestor(1u);
                }
            }

            if (data.MoveToAncestor(1u)) {}
        }
    }


    return status.ErrorsCleared();
}


ErrorManagement::ErrorType MDSObjectConnection::ConnectParameter(StreamString nodeName, ConfigurationDatabase nodeParams) {

    ErrorManagement::ErrorType ret = ErrorManagement::NoError;

    StreamString MDSPath;
    uint32 startIdx  = 0u;
    uint32 stopIdx   = 0u;
    uint32 targetDim = 0u;
    StreamString orientation = "";

    ret.parametersError = !nodeParams.Read("Path", MDSPath);
    if (ret.parametersError) {
        REPORT_ERROR(ret, "[%s] - Parameter %s: no 'Path' node defined.", GetName(), nodeName.Buffer());
    }

    if (ret) {
        if (nodeParams.Read("StartIdx",  startIdx)) {}
        if (nodeParams.Read("StopIdx",    stopIdx)) {}
        if (nodeParams.Read("TargetDim", targetDim)) {}
        if (nodeParams.Read("DataOrientation", orientation)) {
            if ( (orientation != "RowMajor") && (orientation != "ColumnMajor") ) {
                ret.parametersError = true;
                REPORT_ERROR(ret, "[%s] - Parameter %s: invalid DataOrientation (can only be `RowMajor` or `ColumnMajor`)", GetName(), nodeName.Buffer());
            }
        }
        else {
            orientation = "ColumnMajor"; // default
        }
    }

    StreamString expandedMDSPath;
    bool unlinked = false;

    if (ret) {
        // Modify path if Dim option is specified
        if (targetDim != 0u && startIdx == 0u && stopIdx == 0u) {

            // Add indices to the path in the form "[startIdx:stopIdx]"
            StreamString nodeWithIndices;
            StreamString indices;

            indices.Printf("[0:%u]", targetDim);
            nodeWithIndices = MDSPath;
            nodeWithIndices += indices;

            // Now use TDI syntax to append zeroes if indices exceed array dimension
            StreamString tdiExpr = ""
                                    "_swgTargetDim = %u;"                // store targetDim in MDSplus
                                    "_swgVec       = %s;"                // store the current vector with indices
                                    "_swgVecSize   = shape(_swgVec, 0);" // calculate actual vector dimensions
                                    "if(_swgTargetDim > _swgVecSize)"    // if vector size is less than required...
                                    "    for(_i = 0; _i < _swgTargetDim - _swgVecSize; _i++)"
                                    "        _swgVec = [_swgVec, 0];" // ... fill up with zeroes
                                    "_swgVec";                        // return the updated vector as result of this expression

            expandedMDSPath.Printf(tdiExpr.Buffer(), targetDim, nodeWithIndices.Buffer());
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

            ret.parametersError = true;
            REPORT_ERROR(ret, "[%s] - Parameter %s: both Dim and StartIdx/StopIdx used, unsupported.", GetName(), nodeName.Buffer());
        }
        // Path is ok as it is
        else {
            expandedMDSPath = MDSPath;
        }
    }

    MDSplus::Data* nodeData = NULL;

    if (ret) {
        // If the path is empty, this parameter shall be skipped (unlinked parameter).
        unlinked = (MDSPath.Size() == 0u);

        if (unlinked) {
            REPORT_ERROR(ErrorManagement::Warning, "[%s] - Parameter %s: unlinked, no value stored.", GetName(), nodeName.Buffer());
        }
        else {
            try {

                if (clientType == DistributedClient) {
                    MDSplus::Tree* tree;
                    tree = new MDSplus::Tree(treeName.Buffer(), shotNumber, "NORMAL");
                    nodeData = tree->tdiExecute(expandedMDSPath.Buffer());

                    delete tree;
                }
                else if (clientType == ThinClient) {
                    MDSplus::Connection* mdsConnection;
                    mdsConnection = new MDSplus::Connection(serverName.BufferReference());
                    mdsConnection->openTree(treeName.BufferReference(), shotNumber);
                    nodeData = mdsConnection->get(expandedMDSPath.Buffer());

                    delete mdsConnection;
                }
                else {
                    ret.exception = true;
                    REPORT_ERROR(ret, "[%s] - Invalid client type.", GetName());
                }
            }
            catch (MDSplus::MdsException &ex) {
                ret.exception = true;
                REPORT_ERROR(ret, "[%s] - Parameter %s: MDSplus error getting node %s", GetName(), nodeName.Buffer(), MDSPath.Buffer());
                REPORT_ERROR(ret, "[%s] - MDSplus error: %s", GetName(), ex.what());
            }
        }
    }

    if (ret) {
        if (unlinked) {
            AnyType* anyTypeParam = new AnyType(0u);
            anyTypeParam->SetStaticDeclared(false);   // unlinked
            Add(anyTypeParam);
            paramNames.Add(new StreamString(nodeName));
        }
        else {
            ret = AddAnyType(nodeName, orientation, nodeData);
            if (ret) {
                REPORT_ERROR(ret, "[%s] - Parameter %s: correctly linked", GetName(), nodeName.Buffer());
            }
            else {
                ret.internalSetupError = true;
                REPORT_ERROR(ret, "[%s] - Parameter %s: failed loading from MDSplus", GetName(), nodeName.Buffer());
            }
        }
    }

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
        REPORT_ERROR(ret, "[%s] - Parameter %s: MDSplus error getting the associated node", GetName(), nodeName.Buffer());
        REPORT_ERROR(ret, "[%s] - MDSplus error: %s", GetName(), ex.what());
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
                    REPORT_ERROR(ret, "[%s] - Parameter %s: MDSplus error getting Dictionary item %d", GetName(), nodeName.Buffer(), itemIdx/2u);
                    REPORT_ERROR(ret, "[%s] - MDSplus error: %s", GetName(), ex.what());
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
                    REPORT_ERROR(ret, "[%s] - Parameter %s: MDSplus error getting List item %d", GetName(), nodeName.Buffer(), itemIdx);
                    REPORT_ERROR(ret, "[%s] - MDSplus error: %s", GetName(), ex.what());
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

                Add(anyTypeParam);
                paramNames.Add(new StreamString(nodeName));
            }
        }
    }

    if (!ret) {
        delete anyTypeParam;
    }

    return ret;
}

CLASS_REGISTER(MDSObjectConnection, "1.0")
} /* namespace MARTe */
