/**
 * @file OPCUAClientWrapper.cpp
 * @brief Source file for class OPCUAClientWrapper
 * @date 12/03/2019
 * @author Luca Porzio
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
 * the class OPCUAClientWrapper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "OPCUAClientWrapper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

uint32 nOfClients = 0u;

/*************************************************************
 * Global arrays to store clients and wrapper pointer in order to call top level callback functions
 **************************************************************/
UA_Client* clients[1];
OPCUAClientWrapper * wrappers[1];

/*************************************************************
 * Top level callback functions
 **************************************************************/

/*************************************************************
 * Class members implementation
 **************************************************************/

OPCUAClientWrapper::OPCUAClientWrapper(const char8* const modeType) {
    mode = modeType;
    config = UA_ClientConfig_default;
    opcuaClient = UA_Client_new(config);
    monitoredNodes = NULL_PTR(UA_NodeId *);
    request = UA_CreateSubscriptionRequest_default();
    response.subscriptionId = 0;
    monitorRequest.itemToMonitor.nodeId.identifier.numeric = 0;
    monitorResponse.monitoredItemId = 0;
    nOfClients++;
    valueMemories = NULL_PTR(void **);
    clients[nOfClients - 1] = opcuaClient;
    wrappers[nOfClients - 1] = this;
    serverAddress = NULL_PTR(char8 *);
    samplingTime = 0;
    readValues = NULL_PTR(UA_ReadValueId *);
    writeValues = NULL_PTR(UA_WriteValue *);
    tempVariant = NULL_PTR(UA_Variant *);
    nOfNodes = 0u;
}

OPCUAClientWrapper::~OPCUAClientWrapper() {
    UA_Client_disconnect(opcuaClient);
    UA_Client_delete(opcuaClient);
    if (monitoredNodes != NULL_PTR(UA_NodeId *)) {
        delete[] monitoredNodes;
    }
    if (readValues != NULL_PTR(UA_ReadValueId *)) {
        delete[] readValues;
    }
    if (writeValues != NULL_PTR(UA_WriteValue *)) {
        delete[] writeValues;
    }
    if (tempVariant != NULL_PTR(UA_Variant *)) {
        delete[] tempVariant;
    }
    for (uint32 j = 0u; j < nOfNodes; j++) {
        if (valueMemories[j] != NULL_PTR(void*)) {
            free(valueMemories[j]);
        }
    }
    if (valueMemories != NULL_PTR(void **)) {
        delete[] valueMemories;
    }
}

void OPCUAClientWrapper::SetServerAddress(StreamString address) {
    serverAddress = address;
}

bool OPCUAClientWrapper::Connect() {
    UA_StatusCode retval = UA_Client_connect(opcuaClient, const_cast<char8*>(serverAddress.Buffer()));
    return (retval == UA_STATUSCODE_GOOD);
}

bool OPCUAClientWrapper::SetTargetNodes(uint32 * namespaceIndexes,
                                        StreamString * nodePaths,
                                        uint32 numberOfNodes) {
    bool ok = false;
    nOfNodes = numberOfNodes;
    monitoredNodes = new UA_NodeId[numberOfNodes];
    tempVariant = new UA_Variant[numberOfNodes];
    if (mode == "Read") {
        /* Setting up Read request */
        UA_ReadRequest_init(&readRequest);
        //UA_ReadResponse_init(&readResponse);
        //readResponse.resultsSize = numberOfNodes;
        readValues = new UA_ReadValueId[numberOfNodes];
    }
    else if (mode == "Write") {
        /* Setting up write request */
        UA_WriteRequest_init(&writeRequest);
        writeValues = new UA_WriteValue[numberOfNodes];
    }
    for (uint32 i = 0u; i < numberOfNodes; i++) {
        ok = nodePaths[i].Seek(0LLU);
        StreamString pathTokenized;
        uint32 pathSize = 0u;
        char8 ignore;
        if (ok) {
            /* This cycle is for getting the path size only */
            do {
                ok = nodePaths[i].GetToken(pathTokenized, ".", ignore);
                if (ok) {
                    pathSize++;
                }
                pathTokenized = "";
            }
            while (ok);
        }
        StreamString* path = new StreamString[pathSize];
        uint32* ids = new uint32[pathSize];
        ok = nodePaths[i].Seek(0LLU);
        if (ok) {
            for (uint32 k = 0u; k < pathSize; k++) {
                ok = nodePaths[i].GetToken(pathTokenized, ".", ignore);
                if (ok) {
                    //char * s = new char[strlen(pathTokenized.Buffer())];
                    StreamString s = pathTokenized;
                    //strcpy(s, pathTokenized.Buffer());
                    path[k] = s;
                    //delete [] s;
                }
                pathTokenized = "";
            }
        }
        if (ok) {
            /* Building request for Browse Service */
            UA_BrowseRequest bReq;
            UA_BrowseRequest_init(&bReq);
            bReq.requestedMaxReferencesPerNode = 0;
            bReq.nodesToBrowse = UA_BrowseDescription_new();
            bReq.nodesToBrowseSize = 1;

            /* Building request for TranslateBrowsePathsToNodeIds */
            UA_BrowsePath browsePath;
            UA_BrowsePath_init(&browsePath);
            browsePath.startingNode = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
            browsePath.relativePath.elements = (UA_RelativePathElement*) UA_Array_new(pathSize, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]);
            browsePath.relativePath.elementsSize = pathSize;

            uint32 tempNumericNodeId = UA_NS0ID_OBJECTSFOLDER;
            uint16 tempNamespaceIndex = 0u;
            char* tempStringNodeId;

            for (uint32 j = 0u; j < pathSize; j++) {
                ids[j] = GetReferenceType(bReq, const_cast<char8*>(path[j].Buffer()), tempNamespaceIndex, tempNumericNodeId, tempStringNodeId);
                UA_RelativePathElement *elem = &browsePath.relativePath.elements[j];
                elem->referenceTypeId = UA_NODEID_NUMERIC(0, ids[j]);
                elem->targetName = UA_QUALIFIEDNAME_ALLOC(namespaceIndexes[i], const_cast<char8*>(path[j].Buffer()));
            }
            UA_TranslateBrowsePathsToNodeIdsRequest tbpReq;
            UA_TranslateBrowsePathsToNodeIdsRequest_init(&tbpReq);
            tbpReq.browsePaths = &browsePath;
            tbpReq.browsePathsSize = 1;
            UA_TranslateBrowsePathsToNodeIdsResponse tbpResp = UA_Client_Service_translateBrowsePathsToNodeIds(opcuaClient, tbpReq);
            ok = (tbpResp.results[0].statusCode == UA_STATUSCODE_GOOD);
            if (ok) {
                UA_BrowsePathTarget *ref = &(tbpResp.results[0].targets[0]);
                monitoredNodes[i] = ref->targetId.nodeId;
            }

            if (ok && mode == "Read") {
                UA_ReadValueId_init(&readValues[i]);
                readValues[i].attributeId = UA_ATTRIBUTEID_VALUE;
                readValues[i].nodeId = monitoredNodes[i];
            }
            else if (ok && mode == "Write") {
                UA_WriteValue_init(&writeValues[i]);
                writeValues[i].attributeId = UA_ATTRIBUTEID_VALUE;
                writeValues[i].nodeId = monitoredNodes[i];
            }
            if (ok) {
                UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&tbpResp);
                UA_Array_delete(browsePath.relativePath.elements, pathSize, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]);
                UA_BrowseDescription_delete(bReq.nodesToBrowse);
                //UA_BrowseRequest_delete(&bReq);
                delete[] ids;
                delete[] path;
            }
        }
    }
    if (ok && mode == "Read") {
        readRequest.nodesToRead = readValues;
        readRequest.nodesToReadSize = numberOfNodes;
    }
    valueMemories = new void*[numberOfNodes];
    for (uint32 i = 0u; i < numberOfNodes; i++) {
        valueMemories[i] = NULL_PTR(void*);
    }
    return ok;
}

bool OPCUAClientWrapper::GetSignalMemory(void *&mem,
                                         uint32 idx,
                                         const TypeDescriptor & valueTd,
                                         const uint32 nElem,
                                         const uint8 nDimensions) {
    bool ok = true;
    //uint32 valueTypeSize = sizeof(TypeDescriptor::GetTypeNameFromTypeDescriptor(valueTd));
    valueMemories[idx] = malloc((valueTd.numberOfBits / 8) * nElem);

    mem = valueMemories[idx];
    if (mode == "Write") {
        this->SetWriteRequest(idx, nDimensions, nElem, valueTd);
    }

    return ok;

}

bool OPCUAClientWrapper::Monitor() {
    bool ok = false;
    REPORT_ERROR_STATIC(ErrorManagement::UnsupportedFeature, "Monitored Items not supported yet.");

    return ok;
}

bool OPCUAClientWrapper::Read(uint32 numberOfNodes,
                              TypeDescriptor * types,
                              uint32 * nElements) {
    bool ok = true;
    readResponse = UA_Client_Service_read(opcuaClient, readRequest);
    ok = (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD);
    if (ok) {
        for (uint32 i = 0u; i < numberOfNodes; i++) {
            memcpy(valueMemories[i], readResponse.results[i].value.data, sizeof(TypeDescriptor::GetTypeNameFromTypeDescriptor(types[i])) * nElements[i]);
        }
    }
    return ok;
}

uint32 OPCUAClientWrapper::GetReferenceType(UA_BrowseRequest bReq,
                                            char* path,
                                            uint16 &namespaceIndex,
                                            uint32 &numericNodeId,
                                            char* &stringNodeId) {
    uint32 id = 0u;
    if (numericNodeId == 0u) {
        bReq.nodesToBrowse[0].nodeId = UA_NODEID_STRING(namespaceIndex, stringNodeId);
    }
    else {
        bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(namespaceIndex, numericNodeId);
    }
    bReq.nodesToBrowse[0].includeSubtypes = true;
    bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; /* return everything */
    UA_BrowseResponse *bResp = UA_BrowseResponse_new();
    *bResp = UA_Client_Service_browse(opcuaClient, bReq);
    bool ok = (bResp->responseHeader.serviceResult == UA_STATUSCODE_GOOD);
    if (ok) {
        bool found = false;
        uint64 initCheckTime = HighResolutionTimer::Counter();
        uint64 finalCheckTime = 0u;
        uint32 count = 0u;
        while ((!found) && (bResp->resultsSize)) {
            for (uint32 i = 0u; i < bResp->resultsSize; ++i) {
                for (uint32 j = 0u; j < bResp->results[i].referencesSize; ++j) {
                    UA_ReferenceDescription *ref = &(bResp->results[i].references[j]);
                    if (strcmp(reinterpret_cast<char*>(ref->browseName.name.data), path) == 0) {
                        id = ref->referenceTypeId.identifier.numeric;
                        namespaceIndex = ref->nodeId.nodeId.namespaceIndex;
                        if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
                            numericNodeId = ref->nodeId.nodeId.identifier.numeric;
                        }
                        else if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_STRING) {
                            stringNodeId = new char[ref->nodeId.nodeId.identifier.string.length];
                            strcpy(stringNodeId, reinterpret_cast<char*>(ref->nodeId.nodeId.identifier.string.data));
                            numericNodeId = 0u;
                        }
                        found = true;
                    }
                }
            }
            if (!found && bResp->results->continuationPoint.length) {
                /* Debug only */
                //REPORT_ERROR_STATIC(ErrorManagement::Information, "Browse Next Service");
                UA_BrowseNextRequest nextReq;
                UA_BrowseNextRequest_init(&nextReq);
                nextReq.continuationPoints = UA_ByteString_new();
                UA_ByteString_copy(&(bResp->results->continuationPoint), nextReq.continuationPoints);
                nextReq.continuationPointsSize = 1;
                *reinterpret_cast<UA_BrowseNextResponse *>(bResp) = UA_Client_Service_browseNext(opcuaClient, nextReq);
                UA_BrowseNextRequest_deleteMembers(&nextReq);
            }
            finalCheckTime = HighResolutionTimer::Counter();
            if (HighResolutionTimer::TicksToTime(finalCheckTime, initCheckTime) > 5.0) {
                count++;
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Browse Service is taking too long. Is the NodeId or the Path right?");
                initCheckTime = HighResolutionTimer::Counter();
            }
            if (count > 1u) {
                break;
            }
        }
        UA_BrowseResponse_delete(bResp);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Browse Service fail: bad status code.");
    }
    return id;
}

void OPCUAClientWrapper::SetWriteRequest(uint32 idx,
                                         uint8 nDimensions,
                                         uint32 nElements,
                                         TypeDescriptor type) {
    bool isArray = false;
    isArray = (nDimensions > 0);
    TypeDescriptor::GetTypeNameFromTypeDescriptor(type);
    if (type == UnsignedInteger8Bit) {
        uint8 *value = reinterpret_cast<uint8*>(valueMemories[idx]);
        if (isArray) {
            UA_Variant_setArray(&tempVariant[idx], value, nElements, &UA_TYPES[UA_TYPES_BYTE]);
        }
        else {
            UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_BYTE]);
        }
    }
    else if (type == UnsignedInteger16Bit) {
        uint16 *value = reinterpret_cast<uint16*>(valueMemories[idx]);
        if (isArray) {
            UA_Variant_setArray(&tempVariant[idx], value, nElements, &UA_TYPES[UA_TYPES_UINT16]);
        }
        else {
            UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_UINT16]);
        }
    }
    else if (type == UnsignedInteger32Bit) {
        uint32 *value = reinterpret_cast<uint32*>(valueMemories[idx]);
        if (isArray) {
            UA_Variant_setArray(&tempVariant[idx], value, nElements, &UA_TYPES[UA_TYPES_UINT32]);
        }
        else {
            UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_UINT32]);
        }
    }
    else if (type == UnsignedInteger64Bit) {
        uint64 *value = reinterpret_cast<uint64*>(valueMemories[idx]);
        if (isArray) {
            UA_Variant_setArray(&tempVariant[idx], value, nElements, &UA_TYPES[UA_TYPES_UINT64]);
        }
        else {
            UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_UINT64]);
        }
    }
    else if (type == SignedInteger8Bit) {
        int8 *value = reinterpret_cast<int8*>(valueMemories[idx]);
        if (isArray) {
            UA_Variant_setArray(&tempVariant[idx], value, nElements, &UA_TYPES[UA_TYPES_SBYTE]);
        }
        else {
            UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_SBYTE]);
        }
    }
    else if (type == SignedInteger16Bit) {
        int16 *value = reinterpret_cast<int16*>(valueMemories[idx]);
        if (isArray) {
            UA_Variant_setArray(&tempVariant[idx], value, nElements, &UA_TYPES[UA_TYPES_INT16]);
        }
        else {
            UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_INT16]);
        }
    }
    else if (type == SignedInteger32Bit) {
        int32 *value = reinterpret_cast<int32*>(valueMemories[idx]);
        if (isArray) {
            UA_Variant_setArray(&tempVariant[idx], value, nElements, &UA_TYPES[UA_TYPES_INT32]);
        }
        else {
            UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_INT32]);
        }
    }
    else if (type == SignedInteger64Bit) {
        int64 *value = reinterpret_cast<int64*>(valueMemories[idx]);
        if (isArray) {
            UA_Variant_setArray(&tempVariant[idx], value, nElements, &UA_TYPES[UA_TYPES_INT64]);
        }
        else {
            UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_INT64]);
        }
    }
    else if (type == Float32Bit) {
        float32 *value = reinterpret_cast<float32*>(valueMemories[idx]);
        if (isArray) {
            UA_Variant_setArray(&tempVariant[idx], value, nElements, &UA_TYPES[UA_TYPES_FLOAT]);
        }
        else {
            UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_FLOAT]);
        }
    }
    else if (type == Float64Bit) {
        float64 *value = reinterpret_cast<float64*>(valueMemories[idx]);
        if (isArray) {
            UA_Variant_setArray(&tempVariant[idx], value, nElements, &UA_TYPES[UA_TYPES_DOUBLE]);
        }
        else {
            UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_DOUBLE]);
        }
    }
    else if ((type.type == CArray) || (type.type == BT_CCString) || (type.type == PCString) || (type.type == SString)) {
    }

    writeValues[idx].value.value = static_cast<const UA_Variant>(tempVariant[idx]);
    writeValues[idx].value.hasValue = true;

}

bool OPCUAClientWrapper::Write(uint32 numberOfNodes) {
    writeRequest.nodesToWrite = writeValues;
    writeRequest.nodesToWriteSize = numberOfNodes;
    bool ok = true;
    UA_WriteResponse response = UA_Client_Service_write(opcuaClient, writeRequest);
    ok = (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD);
    UA_WriteResponse_deleteMembers(&response);
    return ok;
}

void OPCUAClientWrapper::UpdateMemory(UA_DataValue *value) {
    //Delete this before implementing this method
    value = NULL_PTR(UA_DataValue *);

}
void OPCUAClientWrapper::SetSamplingTime(float64 sampleTime) {
    samplingTime = sampleTime;
}

UA_NodeId * OPCUAClientWrapper::GetMonitoredNodes() {
    return monitoredNodes;
}

}
