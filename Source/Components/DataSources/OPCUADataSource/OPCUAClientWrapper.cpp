/**
 * @file OPCUAClientWrapper.cpp
 * @brief Source file for class OPCUAClientWrapper
 * @date 21 Nov 2018 TODO Verify the value and format of the date
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

/* Global arrays to store clients and wrapper pointer in order to call top level callback functions */
UA_Client* clients[1000];
OPCUAClientWrapper * wrappers[1000];

/**
 * Top level callback functions
 */
void OPCUAClientWrapperUpdateMemory(UA_Client *client,
                                    UA_UInt32 subId,
                                    void *subContext,
                                    UA_UInt32 monId,
                                    void *monContext,
                                    UA_DataValue *value) {
    uint32 i;
    for (i = 0u; i < nOfClients; i++) {
        if (clients[i] == client) {
            wrappers[i]->UpdateMemory(value);
        }
    }
}

void OPCUAClientWrapperHandlerInactivity(UA_Client *client,
                                         UA_UInt32 subId,
                                         void *subContext) {
    uint32 i;
    for (i = 0u; i < nOfClients; i++) {
        if (clients[i] == client) {
            wrappers[i]->SubscriptionInactivity();
        }
    }
}

void OPCUAClientWrapperHandlerState(UA_Client *client,
                                    UA_ClientState clientState) {
    uint32 i;
    for (i = 0u; i < nOfClients; i++) {
        if (clients[i] == client) {
            wrappers[i]->StateCallback(clientState);
        }
    }
}

void OPCUAClientWrapperSubscriptionStatus(UA_Client *client,
                                          UA_UInt32 subId,
                                          void *subContext,
                                          UA_StatusChangeNotification *notification) {
    REPORT_ERROR_STATIC(ErrorManagement::Information, "SUBSCRIPTION STATUS CHANGED");
}

/**
 * Class members implementation
 */
OPCUAClientWrapper::OPCUAClientWrapper(const char8* const modeType) {
    mode = modeType;
    config = UA_ClientConfig_default;
    config.stateCallback = OPCUAClientWrapperHandlerState;
    config.subscriptionInactivityCallback = OPCUAClientWrapperHandlerInactivity;
    opcuaClient = UA_Client_new(config);
    monitoredNodes = NULL_PTR(UA_NodeId *);
    outValueMem = new UA_Variant;
    request = UA_CreateSubscriptionRequest_default();
    response.subscriptionId = 0;
    monitorRequest.itemToMonitor.nodeId.identifier.numeric = 0;
    monitorResponse.monitoredItemId = 0;
    nOfClients++;
    valueMemories = NULL_PTR(void **);
    clients[nOfClients - 1] = opcuaClient;
    wrappers[nOfClients - 1] = this;
    serverAddress = NULL_PTR(char *);
    numberOfCharElements = 0u;
    samplingTime = 0;
    readValues = NULL_PTR(UA_ReadValueId *);
    writeValues = NULL_PTR(UA_WriteValue *);
    tempVariant = NULL_PTR(UA_Variant *);
}

OPCUAClientWrapper::~OPCUAClientWrapper() {
    UA_Client_disconnect(opcuaClient);
    UA_Client_delete(opcuaClient);
    delete[] monitoredNodes;
    delete[] valueMemories;

}

void OPCUAClientWrapper::SetServerAddress(char* address) {
    serverAddress = new char[strlen(address)];
    strcpy(serverAddress, address);
}

bool OPCUAClientWrapper::Connect() {
    UA_StatusCode retval = UA_Client_connect(opcuaClient, serverAddress);
    return (retval == UA_STATUSCODE_GOOD);
}

bool OPCUAClientWrapper::SetTargetNodes(uint32 * namespaceIndexes,
                                        StreamString * nodePaths,
                                        uint32 numberOfNodes) {
    bool ok = false;

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
        char** path = new char*[pathSize];
        uint32* ids = new uint32[pathSize];
        ok = nodePaths[i].Seek(0LLU);
        if (ok) {
            for (uint32 k = 0u; k < pathSize; k++) {
                ok = nodePaths[i].GetToken(pathTokenized, ".", ignore);
                if (ok) {
                    char * s = new char[strlen(pathTokenized.Buffer())];
                    strcpy(s, pathTokenized.Buffer());
                    path[k] = s;
                }
                pathTokenized = "";
            }
        }

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
            ids[j] = GetReferenceType(bReq, path[j], tempNamespaceIndex, tempNumericNodeId, tempStringNodeId);
            UA_RelativePathElement *elem = &browsePath.relativePath.elements[j];
            elem->referenceTypeId = UA_NODEID_NUMERIC(0, ids[j]);
            elem->targetName = UA_QUALIFIEDNAME_ALLOC(namespaceIndexes[i], path[j]);
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
        if (mode == "Read") {
            UA_ReadValueId_init(&readValues[i]);
            readValues[i].attributeId = UA_ATTRIBUTEID_VALUE;
            readValues[i].nodeId = monitoredNodes[i];
        }
        else if (mode == "Write") {
            UA_WriteValue_init(&writeValues[i]);
            writeValues[i].attributeId = UA_ATTRIBUTEID_VALUE;
            writeValues[i].nodeId = monitoredNodes[i];
        }
    }
    if (mode == "Read") {
        readRequest.nodesToRead = readValues;
        readRequest.nodesToReadSize = numberOfNodes;
    }
    valueMemories = new void*[numberOfNodes];
    return ok;
}

bool OPCUAClientWrapper::GetSignalMemory(void *&mem,
                                         uint32 idx,
                                         const TypeDescriptor & valueTd,
                                         const uint32 nElem,
                                         const uint8 nDimensions) {
    bool ok = true;
    if (monitoredNodes[idx].identifier.numeric == 0) {
        REPORT_ERROR_STATIC(ErrorManagement::IllegalOperation, "Cannot get the signal memory.");
        ok = false;
    }
    else {
        if (valueTd == CharString || valueTd == Character8Bit) {
            //valueMemory = malloc(sizeof(char) * numberOfCharElements);
            valueMemories[idx] = malloc(sizeof(char) * 100);
        }
        else {
            uint32 valueTypeSize = sizeof(TypeDescriptor::GetTypeNameFromTypeDescriptor(valueTd));
            valueMemories[idx] = malloc(valueTypeSize * nElem);
        }
        mem = valueMemories[idx];
        if (mode == "Write") {
            this->SetWriteRequest(idx, nDimensions, nElem, valueTd);
        }
    }
    return ok;

}

bool OPCUAClientWrapper::Monitor() {
    bool ok = false;
#if 0
    UA_StatusCode code = 1u;
    if (monitoredNode.identifier.numeric != 0 && valueMemory != NULL_PTR(void *)) {
        /* Create a subscription */
        if (response.subscriptionId == 0) {
            request.requestedPublishingInterval = samplingTime;
            request.maxNotificationsPerPublish = 10;
            request.requestedLifetimeCount = 3;
            request.requestedMaxKeepAliveCount = 1;
            response = UA_Client_Subscriptions_create(opcuaClient, request, NULL, OPCUAClientWrapperSubscriptionStatus, NULL);
            ok = (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD);
        }
        /* Add a MonitoredItem */
        if (monitorRequest.itemToMonitor.nodeId.identifier.numeric == 0 && ok) {
            if (monitoredNode.identifierType == UA_NODEIDTYPE_NUMERIC) {
                monitorRequest = UA_MonitoredItemCreateRequest_default(UA_NODEID_NUMERIC(monitoredNode.namespaceIndex, monitoredNode.identifier.numeric));
            }
            else if (monitoredNode.identifierType == UA_NODEIDTYPE_STRING) {
                monitorRequest = UA_MonitoredItemCreateRequest_default(
                        UA_NODEID_STRING(monitoredNode.namespaceIndex, reinterpret_cast<char*>(monitoredNode.identifier.string.data)));
            }
            monitorRequest.requestedParameters.samplingInterval = samplingTime;
            monitorRequest.requestedParameters.queueSize = 1;
        }
        if (monitorResponse.monitoredItemId == 0) {
            monitorResponse = UA_Client_MonitoredItems_createDataChange(opcuaClient, response.subscriptionId, UA_TIMESTAMPSTORETURN_BOTH, monitorRequest, NULL,
                    OPCUAClientWrapperUpdateMemory, NULL);
        }
        /* Asynchronous call */
        code = UA_Client_run_iterate(opcuaClient, static_cast<UA_UInt16>(samplingTime + 1));
        ok = (code == UA_STATUSCODE_GOOD);
    }
    else {
        ok = true;
    }
#endif
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

//namespaceIndex, numericNodeId e stringNodeId servono davvero?
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
    /*outValueMem->data = value->value.data;
     uint32 arraySize = value->value.arrayLength;
     bool isArray = (arraySize > 1);
     if (isArray) {
     if (valueTypeDesc == UnsignedInteger8Bit) {
     uint8 * vals = reinterpret_cast<uint8*>(outValueMem->data);
     uint8 * vMem = reinterpret_cast<uint8*>(valueMemory);
     for (uint32 i = 0u; i < arraySize; i++) {
     vMem[i] = vals[i];
     }
     }
     else if (valueTypeDesc == UnsignedInteger16Bit) {
     uint16 * vals = reinterpret_cast<uint16*>(outValueMem->data);
     uint16 * vMem = reinterpret_cast<uint16*>(valueMemory);
     for (uint32 i = 0u; i < arraySize; i++) {
     vMem[i] = vals[i];
     }
     }
     else if (valueTypeDesc == UnsignedInteger32Bit) {
     uint32 * vals = reinterpret_cast<uint32*>(outValueMem->data);
     uint32 * vMem = reinterpret_cast<uint32*>(valueMemory);
     for (uint32 i = 0u; i < arraySize; i++) {
     vMem[i] = vals[i];
     }
     }
     else if (valueTypeDesc == UnsignedInteger64Bit) {
     uint64 * vals = reinterpret_cast<uint64*>(outValueMem->data);
     uint64 * vMem = reinterpret_cast<uint64*>(valueMemory);
     for (uint32 i = 0u; i < arraySize; i++) {
     vMem[i] = vals[i];
     }
     }
     else if (valueTypeDesc == SignedInteger8Bit) {
     int8 * vals = reinterpret_cast<int8*>(outValueMem->data);
     int8 * vMem = reinterpret_cast<int8*>(valueMemory);
     for (uint32 i = 0u; i < arraySize; i++) {
     vMem[i] = vals[i];
     }
     }
     else if (valueTypeDesc == SignedInteger16Bit) {
     int16 * vals = reinterpret_cast<int16*>(outValueMem->data);
     int16 * vMem = reinterpret_cast<int16*>(valueMemory);
     for (uint32 i = 0u; i < arraySize; i++) {
     vMem[i] = vals[i];
     }
     }
     else if (valueTypeDesc == SignedInteger32Bit) {
     int32 * vals = reinterpret_cast<int32*>(outValueMem->data);
     int32 * vMem = reinterpret_cast<int32*>(valueMemory);
     for (uint32 i = 0u; i < arraySize; i++) {
     vMem[i] = vals[i];
     }
     }
     else if (valueTypeDesc == SignedInteger64Bit) {
     int64 * vals = reinterpret_cast<int64*>(outValueMem->data);
     int64 * vMem = reinterpret_cast<int64*>(valueMemory);
     for (uint32 i = 0u; i < arraySize; i++) {
     vMem[i] = vals[i];
     }
     }
     else if (valueTypeDesc == Float32Bit) {
     float32 * vals = reinterpret_cast<float32*>(outValueMem->data);
     float32 * vMem = reinterpret_cast<float32*>(valueMemory);
     for (uint32 i = 0u; i < arraySize; i++) {
     vMem[i] = vals[i];
     }
     }
     else if (valueTypeDesc == Float64Bit) {
     float64 * vals = reinterpret_cast<float64*>(outValueMem->data);
     float64 * vMem = reinterpret_cast<float64*>(valueMemory);
     for (uint32 i = 0u; i < arraySize; i++) {
     vMem[i] = vals[i];
     }
     }
     else if (valueTypeDesc == CharString || valueTypeDesc == Character8Bit) {
     REPORT_ERROR_STATIC(ErrorManagement::Information, "Array of String non supported yet.");
     }
     }
     else if (valueTypeDesc == CharString || valueTypeDesc == Character8Bit) {
     UA_String * stringValueMem = reinterpret_cast<UA_String*>(outValueMem->data);
     char8 * vMem = reinterpret_cast<char8*>(valueMemory);
     //memcpy(vMem, stringValueMem->data, numberOfCharElements);
     memcpy(vMem, stringValueMem->data, 100);
     vMem[stringValueMem->length] = '\0';
     }
     else {
     memcpy(valueMemory, outValueMem->data, valueTypeDesc.numberOfBits / 8);
     }*/
}

void OPCUAClientWrapper::StateCallback(UA_ClientState clientState) {
}

void OPCUAClientWrapper::SubscriptionInactivity() {
}

void OPCUAClientWrapper::SetMaxNumberOfChar(uint32 nElements) {
    numberOfCharElements = nElements;
}

void OPCUAClientWrapper::SetSamplingTime(float64 sampleTime) {
    samplingTime = sampleTime;
}

}
