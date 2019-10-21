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
/*-e909 and -e9133 redefines bool. -e1938 UA_ClientConfig_default is global*/
/*lint -save -e909 -e9133 -e1938*/
namespace MARTe {

OPCUAClientWrapper::OPCUAClientWrapper() {
    /* Client config */
    opcuaClient = UA_Client_new();
    config = UA_Client_getConfig(opcuaClient);
    UA_ClientConfig_setDefault(config);

    samplingTime = 0.0;
    nOfNodes = 0u;

    /* Requests init */
    UA_ReadRequest_init(&readRequest);
    UA_WriteRequest_init(&writeRequest);
    UA_BrowseRequest_init(&bReq);
    UA_BrowsePath_init(&browsePath);
    UA_BrowsePath_init(&mBrowsePath);
    UA_BrowseRequest_init(&mbReq);
    UA_TranslateBrowsePathsToNodeIdsRequest_init(&tbpReq);
    UA_TranslateBrowsePathsToNodeIdsRequest_init(&objectTbpReq);
    UA_TranslateBrowsePathsToNodeIdsRequest_init(&methodTbpReq);

    readValues = NULL_PTR(UA_ReadValueId*);
    writeValues = NULL_PTR(UA_WriteValue*);
    tempVariant = NULL_PTR(UA_Variant*);
    monitoredNodes = NULL_PTR(UA_NodeId*);
    valueMemories = NULL_PTR(void**);
    serverAddress = NULL_PTR(char8*);
    dataPtr = NULL_PTR(void*);
    tempDataPtr = NULL_PTR(uint8*);
    eos = NULL_PTR(UA_ExtensionObject*);
}

/*lint -e{1551} No exception thrown.*/
/*lint -e{1740} opcuaClient freed by UA_Client_delete.*/
OPCUAClientWrapper::~OPCUAClientWrapper() {
    bool ok = true;

    if (mode != "MethodCall") {
        ok = UnregisterNodes(monitoredNodes);
    }
    if (ok) {
        if (monitoredNodes != NULL_PTR(UA_NodeId*)) {
            delete[] monitoredNodes;
        }
        if (readValues != NULL_PTR(UA_ReadValueId*)) {
            delete[] readValues;
        }
        if (writeValues != NULL_PTR(UA_WriteValue*)) {
            delete[] writeValues;
        }
        if (tempVariant != NULL_PTR(UA_Variant*)) {
            delete[] tempVariant;
        }
        if (valueMemories != NULL_PTR(void**)) {
            for (uint32 j = 0u; j < nOfNodes; j++) {
                if ((valueMemories[j] != NULL_PTR(void*)) && ok) {
                    ok = HeapManager::Free(valueMemories[j]);
                }
            }
            if (ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Information, "Value memories have been freed successfully.");
            }
        }
//        if (mode == "Write") {
//            if (eos != NULL_PTR(UA_ExtensionObject*)) {
//                delete eos;
//            }
//        }
    }
    UA_Client_disconnect(opcuaClient); /* UA_STATUSCODE_GOOD */
    //UA_Client_delete(opcuaClient);
}

void OPCUAClientWrapper::SetServerAddress(StreamString address) {
    serverAddress = address;
}

bool OPCUAClientWrapper::Connect() {
    UA_StatusCode retval = UA_Client_connect(opcuaClient, const_cast<char8*>(serverAddress.Buffer()));
    return (retval == 0x00U); /* UA_STATUSCODE_GOOD */
}

bool OPCUAClientWrapper::SetServiceRequest(const uint16 *const namespaceIndexes,
                                           StreamString *const nodePaths,
                                           const uint32 numberOfNodes) {
    bool ok = false;
    nOfNodes = numberOfNodes;
    //monitoredNodes = new UA_NodeId[numberOfNodes];
    monitoredNodes = static_cast<UA_NodeId*>(UA_Array_new(static_cast<osulong>(numberOfNodes), &UA_TYPES[UA_TYPES_NODEID]));
    tempVariant = new UA_Variant[numberOfNodes];
    if (mode == "Read") {
        /* Setting up Read request */
        readValues = new UA_ReadValueId[numberOfNodes];
    }
    else if (mode == "Write") { /* mode == "Write" */
        /* Setting up write request */
        writeValues = new UA_WriteValue[numberOfNodes];
    }

    for (uint32 i = 0u; i < numberOfNodes; i++) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Registering Node %s", nodePaths[i].Buffer());
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
        StreamString *path = NULL_PTR(StreamString*);
        uint32 *ids;
        if (pathSize > 0u) {
            path = new StreamString[pathSize];
        }
        ids = new uint32[pathSize];
        ok = nodePaths[i].Seek(0LLU);

        if (ok) {
            if (path != NULL_PTR(StreamString*)) {
                for (uint32 k = 0u; k < pathSize; k++) {
                    ok = nodePaths[i].GetToken(pathTokenized, ".", ignore);
                    if (ok) {
                        StreamString s = pathTokenized;
                        path[k] = s;
                    }
                    pathTokenized = "";
                }
            }
        }

        if (ok) {
            /* Building request for Browse Service */
            bReq.requestedMaxReferencesPerNode = 0u;
            bReq.nodesToBrowse = UA_BrowseDescription_new();
            UA_BrowseDescription_init(bReq.nodesToBrowse);
            bReq.nodesToBrowseSize = 1u;

            /* Building request for TranslateBrowsePathsToNodeIds */
            browsePath.startingNode = UA_NODEID_NUMERIC(0u, 85u); /* UA_NS0ID_OBJECTSFOLDER */
            browsePath.relativePath.elements = static_cast<UA_RelativePathElement*>(UA_Array_new(static_cast<osulong>(pathSize),
                                                                                                 &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]));
            browsePath.relativePath.elementsSize = pathSize;

            uint32 tempNumericNodeId = 85u; /* UA_NS0ID_OBJECTSFOLDER */
            uint16 tempNamespaceIndex = 0u;
            char8 *tempStringNodeId = NULL_PTR(char8*);
            if ((path != NULL_PTR(StreamString*)) && (ids != NULL_PTR(uint32*))) {
                for (uint32 j = 0u; j < pathSize; j++) {
                    ids[j] = GetReferences(bReq, const_cast<char8*>(path[j].Buffer()), tempNamespaceIndex, tempNumericNodeId, tempStringNodeId);
                    UA_RelativePathElement *elem = &browsePath.relativePath.elements[j];
                    elem->referenceTypeId = UA_NODEID_NUMERIC(0u, ids[j]);
                    /*lint -e{1055} -e{64} -e{746} UA_QUALIFIEDNAME is declared in the open62541 library.*/
                    elem->targetName = UA_QUALIFIEDNAME_ALLOC(namespaceIndexes[i], const_cast<char8*>(path[j].Buffer()));
                }
                if (tempStringNodeId != NULL_PTR(char8*)) {
                    delete tempStringNodeId;
                }
            }

            tbpReq.browsePaths = &browsePath;
            tbpReq.browsePathsSize = 1u;
            UA_TranslateBrowsePathsToNodeIdsResponse *tbpResp = UA_TranslateBrowsePathsToNodeIdsResponse_new();
            UA_TranslateBrowsePathsToNodeIdsResponse_init(tbpResp);
            *tbpResp = UA_Client_Service_translateBrowsePathsToNodeIds(opcuaClient, tbpReq);
            ok = (tbpResp->results[0].statusCode == 0x00U); /* UA_STATUSCODE_GOOD */
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Impossible to register Node %s", nodePaths[i].Buffer());
            }
            if (ok) {
                UA_BrowsePathTarget *ref = &(tbpResp->results[0].targets[0]);
                UA_NodeId_copy(&(ref->targetId.nodeId), &monitoredNodes[i]);
                if (mode == "Read") {
                    if (readValues != NULL_PTR(UA_ReadValueId*)) {
                        UA_ReadValueId_init(&readValues[i]);
                        readValues[i].attributeId = 13u; /* UA_ATTRIBUTEID_VALUE */
                        UA_NodeId_copy(&monitoredNodes[i], &(readValues[i].nodeId));
                    }
                }
                else if (mode == "Write") {
                    if (writeValues != NULL_PTR(UA_WriteValue*)) {
                        UA_WriteValue_init(&writeValues[i]);
                        writeValues[i].attributeId = 13u; /* UA_ATTRIBUTEID_VALUE */
                        UA_NodeId_copy(&monitoredNodes[i], &(writeValues[i].nodeId));
                    }
                }
            }
            UA_TranslateBrowsePathsToNodeIdsResponse_clear(tbpResp);
            UA_TranslateBrowsePathsToNodeIdsResponse_delete(tbpResp);
            UA_Array_delete(browsePath.relativePath.elements, static_cast<osulong>(pathSize), &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]);
        }
        delete[] ids;
        delete[] path;
    }
    if (ok) {
        if (mode == "Read") {
            readRequest.nodesToRead = readValues;
            readRequest.nodesToReadSize = numberOfNodes;
        }
        /* Using OPCUA RegisterNodes Service to increase performances */
        if (monitoredNodes != NULL_PTR(UA_NodeId*)) {
            if (mode != "MethodCall") {
                ok = RegisterNodes(monitoredNodes);
            }
        }
    }
    return ok;
}

bool OPCUAClientWrapper::RegisterNodes(UA_NodeId *const monitoredItems) {
    bool ok;
    UA_RegisterNodesRequest rReq;
    UA_RegisterNodesRequest_init(&rReq);
    rReq.nodesToRegisterSize = nOfNodes;
    rReq.nodesToRegister = static_cast<UA_NodeId*>(UA_Array_new(static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_NODEID]));
    for (uint32 k = 0u; k < nOfNodes; k++) {
        UA_NodeId_copy(&monitoredNodes[k], &(rReq.nodesToRegister[k]));
    }
    UA_RegisterNodesResponse rResp = UA_Client_Service_registerNodes(opcuaClient, rReq);
    ok = (rResp.responseHeader.serviceResult == 0x00U);
    UA_Array_delete(rReq.nodesToRegister, static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_NODEID]);
    UA_RegisterNodesResponse_clear(&rResp);
    return ok;
}

bool OPCUAClientWrapper::UnregisterNodes(UA_NodeId *const monitoredItems) {
    bool ok;
    UA_UnregisterNodesRequest rReq;
    UA_UnregisterNodesRequest_init(&rReq);
    rReq.nodesToUnregisterSize = nOfNodes;
    rReq.nodesToUnregister = static_cast<UA_NodeId*>(UA_Array_new(static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_NODEID]));
    for (uint32 k = 0u; k < nOfNodes; k++) {
        UA_NodeId_copy(&monitoredNodes[k], &(rReq.nodesToUnregister[k]));
    }
    UA_UnregisterNodesResponse rResp = UA_Client_Service_unregisterNodes(opcuaClient, rReq);
    ok = (rResp.responseHeader.serviceResult == 0x00U);
    UA_Array_delete(rReq.nodesToUnregister, static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_NODEID]);
    UA_UnregisterNodesResponse_clear(&rResp);
    return ok;
}

bool OPCUAClientWrapper::SetMethodRequest(const uint16 methodNamespaceIndex,
                                          StreamString methodPath) {
    bool ok = true;
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Registering Method %s", methodPath.Buffer());
    ok = methodPath.Seek(0LLU);

    StreamString pathTokenized;
    uint32 pathSize = 0u;
    char8 ignore;
    if (ok) {
        /* This cycle is for getting the path size only */
        do {
            ok = methodPath.GetToken(pathTokenized, ".", ignore);
            if (ok) {
                pathSize++;
            }
            pathTokenized = "";
        }
        while (ok);
    }
    StreamString *path = NULL_PTR(StreamString*);
    uint32 *ids;
    if (pathSize > 0u) {
        path = new StreamString[pathSize];
    }
    ids = new uint32[pathSize];
    ok = methodPath.Seek(0LLU);

    if (ok) {
        if (path != NULL_PTR(StreamString*)) {
            for (uint32 k = 0u; k < pathSize; k++) {
                ok = methodPath.GetToken(pathTokenized, ".", ignore);
                if (ok) {
                    StreamString s = pathTokenized;
                    path[k] = s;
                }
                pathTokenized = "";
            }
        }
    }
    if (ok) {
        /* Building request for Browse Service */
        mbReq.requestedMaxReferencesPerNode = 0u;
        mbReq.nodesToBrowse = UA_BrowseDescription_new();
        UA_BrowseDescription_init(mbReq.nodesToBrowse);
        mbReq.nodesToBrowseSize = 1u;

        /* Getting Information for Owner Object */
        /* Building request for TranslateBrowsePathsToNodeIds */
        mBrowsePath.startingNode = UA_NODEID_NUMERIC(0u, 85u); /* UA_NS0ID_OBJECTSFOLDER */
        mBrowsePath.relativePath.elements = static_cast<UA_RelativePathElement*>(UA_Array_new(static_cast<osulong>(pathSize - 1),
                                                                                              &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]));
        mBrowsePath.relativePath.elementsSize = pathSize - 1;

        uint32 tempNumericNodeId = 85u; /* UA_NS0ID_OBJECTSFOLDER */
        uint16 tempNamespaceIndex = 0u;
        char8 *tempStringNodeId = NULL_PTR(char8*);
        if ((path != NULL_PTR(StreamString*)) && (ids != NULL_PTR(uint32*))) {
            for (uint32 j = 0u; j < pathSize - 1; j++) {
                ids[j] = GetReferences(mbReq, const_cast<char8*>(path[j].Buffer()), tempNamespaceIndex, tempNumericNodeId, tempStringNodeId);
                UA_RelativePathElement *objElem = &mBrowsePath.relativePath.elements[j];
                objElem->referenceTypeId = UA_NODEID_NUMERIC(0u, ids[j]);
                /*lint -e{1055} -e{64} -e{746} UA_QUALIFIEDNAME is declared in the open62541 library.*/
                objElem->targetName = UA_QUALIFIEDNAME_ALLOC(methodNamespaceIndex, const_cast<char8*>(path[j].Buffer()));
            }
        }

        objectTbpReq.browsePaths = &mBrowsePath;
        objectTbpReq.browsePathsSize = 1u;
        UA_TranslateBrowsePathsToNodeIdsResponse *objectTbpResp = UA_TranslateBrowsePathsToNodeIdsResponse_new();
        UA_TranslateBrowsePathsToNodeIdsResponse_init(objectTbpResp);
        *objectTbpResp = UA_Client_Service_translateBrowsePathsToNodeIds(opcuaClient, objectTbpReq);
        ok = (objectTbpResp->results[0].statusCode == 0x00U); /* UA_STATUSCODE_GOOD */
        if (ok) {
            UA_BrowsePathTarget *ref = &(objectTbpResp->results[0].targets[0]);
            UA_NodeId_copy(&(ref->targetId.nodeId), &objectMethod);
        }

        UA_TranslateBrowsePathsToNodeIdsResponse_clear(objectTbpResp);
        UA_TranslateBrowsePathsToNodeIdsResponse_delete(objectTbpResp);
        UA_Array_delete(mBrowsePath.relativePath.elements, static_cast<osulong>(pathSize - 1), &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]);

        /* Getting Information for Method Node */
        mbReq.nodesToBrowse = UA_BrowseDescription_new();
        mbReq.nodesToBrowseSize = 1u;
        mBrowsePath.relativePath.elements = static_cast<UA_RelativePathElement*>(UA_Array_new(static_cast<osulong>(pathSize),
                                                                                              &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]));
        mBrowsePath.relativePath.elementsSize = pathSize;

        tempNumericNodeId = 85u; /* UA_NS0ID_OBJECTSFOLDER */
        tempNamespaceIndex = 0u;
        tempStringNodeId = NULL_PTR(char8*);

        if ((path != NULL_PTR(StreamString*)) && (ids != NULL_PTR(uint32*))) {
            for (uint32 j = 0u; j < pathSize; j++) {
                ids[j] = GetReferences(mbReq, const_cast<char8*>(path[j].Buffer()), tempNamespaceIndex, tempNumericNodeId, tempStringNodeId);
                UA_RelativePathElement *methodElem = &mBrowsePath.relativePath.elements[j];
                methodElem->referenceTypeId = UA_NODEID_NUMERIC(0u, ids[j]);
                /*lint -e{1055} -e{64} -e{746} UA_QUALIFIEDNAME is declared in the open62541 library.*/
                methodElem->targetName = UA_QUALIFIEDNAME_ALLOC(methodNamespaceIndex, const_cast<char8*>(path[j].Buffer()));
            }
            if (tempStringNodeId != NULL_PTR(char8*)) {
                delete tempStringNodeId;
            }
        }

        methodTbpReq.browsePaths = &mBrowsePath;
        methodTbpReq.browsePathsSize = 1u;
        UA_TranslateBrowsePathsToNodeIdsResponse *methodTbpResp = UA_TranslateBrowsePathsToNodeIdsResponse_new();
        UA_TranslateBrowsePathsToNodeIdsResponse_init(methodTbpResp);
        *methodTbpResp = UA_Client_Service_translateBrowsePathsToNodeIds(opcuaClient, methodTbpReq);
        ok = (methodTbpResp->results[0].statusCode == 0x00U); /* UA_STATUSCODE_GOOD */
        if (ok) {
            UA_BrowsePathTarget *ref = &(methodTbpResp->results[0].targets[0]);
            UA_NodeId_copy(&(ref->targetId.nodeId), &methodNodeId);
        }

        UA_TranslateBrowsePathsToNodeIdsResponse_clear(methodTbpResp);
        UA_TranslateBrowsePathsToNodeIdsResponse_delete(methodTbpResp);
        UA_Array_delete(mBrowsePath.relativePath.elements, static_cast<osulong>(pathSize), &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]);
    }
    delete[] ids;
    delete[] path;
    return ok;
}

void OPCUAClientWrapper::SetValueMemories() {
    valueMemories = new void*[nOfNodes];
    for (uint32 i = 0u; i < nOfNodes; i++) {
        valueMemories[i] = NULL_PTR(void*);
    }
}

void OPCUAClientWrapper::SetDataPtr(const uint32 bodyLength) {
    dataPtr = HeapManager::Malloc(bodyLength);
    tempDataPtr = reinterpret_cast<uint8*>(dataPtr);
}

void OPCUAClientWrapper::SeekDataPtr(const uint32 bodyLength) {
    dataPtr = reinterpret_cast<uint8*>(dataPtr) - bodyLength;
}

bool OPCUAClientWrapper::GetSignalMemory(void *&mem,
                                         const uint32 idx,
                                         const TypeDescriptor &valueTd,
                                         const uint32 nElem,
                                         const uint8 nDimensions) {
    bool ok = true;
    if (valueMemories != NULL_PTR(void**)) {
        if (dataPtr == NULL_PTR(void*)) {
            uint32 nOfBytes = valueTd.numberOfBits;
            nOfBytes /= 8u;
            nOfBytes *= nElem;
            valueMemories[idx] = HeapManager::Malloc(nOfBytes);
        }
        mem = valueMemories[idx];
    }

    return ok;

}

bool OPCUAClientWrapper::Monitor() const {
    bool ok = false;
    REPORT_ERROR_STATIC(ErrorManagement::UnsupportedFeature, "Monitored Items not supported yet.");

    return ok;
}

bool OPCUAClientWrapper::Read(const TypeDescriptor *const types,
                              const uint32 *const nElements) {
    readResponse = UA_Client_Service_read(opcuaClient, readRequest);
    bool ok = (readResponse.responseHeader.serviceResult == 0x00U); /* UA_STATUSCODE_GOOD */
    if (ok) {
        if (valueMemories != NULL_PTR(void**)) {
            UA_ExtensionObject *valuePtr = reinterpret_cast<UA_ExtensionObject*>(readResponse.results[0].value.data);
            if (readResponse.results[0].value.type->typeId.identifier.numeric == 22) { /* EXTENSION_OBJECT */
                if (readResponse.results[0].value.arrayLength > 1u) {
                    eos = new UA_ExtensionObject[readResponse.results[0].value.arrayLength];
                    for (uint32 j = 0u; j < readResponse.results[0].value.arrayLength; j++) {
                        if (ok) {
                            ok = (UA_ExtensionObject_copy(&valuePtr[j], &eos[j]) == 0x00U);
                        }
                    }
                    if (ok) {
                        for (uint32 i = 0u; i < readResponse.results[0].value.arrayLength; i++) {
                            if (ok) {
                                ok = MemoryOperationsHelper::Copy(dataPtr, eos[i].content.encoded.body.data, eos[i].content.encoded.body.length);
                                dataPtr = reinterpret_cast<uint8*>(dataPtr) + eos[i].content.encoded.body.length;
                            }
                        }
                    }
                    uint32 actualBodyLength = (readResponse.results[0].value.arrayLength * eos[0u].content.encoded.body.length);
                    SeekDataPtr(actualBodyLength);
                    delete[] eos;
                }
                else {
                    eos = reinterpret_cast<UA_ExtensionObject*>(HeapManager::Malloc(sizeof(UA_ExtensionObject)));
                    UA_ExtensionObject_copy(valuePtr, eos);
                    ok = MemoryOperationsHelper::Copy(dataPtr, eos->content.encoded.body.data, eos->content.encoded.body.length);
                }
            }
            else {
                for (uint32 i = 0u; i < nOfNodes; i++) {
                    if (valueMemories[i] != NULL_PTR(void*)) {
                        uint32 nOfBytes = types[i].numberOfBits;
                        nOfBytes /= 8u;
                        nOfBytes *= nElements[i];
                        ok = MemoryOperationsHelper::Copy(valueMemories[i], readResponse.results[i].value.data, nOfBytes);
                    }
                }
            }
        }
    }
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "ReadError - OPC UA Status Code (Part 4 - 7.34): %x", readResponse.responseHeader.serviceResult);
        UA_Client_run_iterate(opcuaClient, 100u);
    }
    return ok;
}
bool OPCUAClientWrapper::GetExtensionObject() {
    bool ok;
    /* Reading Extension Object Information */
    readValues = UA_ReadValueId_new();
    //UA_ReadValueId_init(&readValues[0u]);
    readValues[0u].attributeId = 13u; /* UA_ATTRIBUTEID_VALUE */
    UA_NodeId_copy(&monitoredNodes[0u], &(readValues[0u].nodeId));
    readRequest.nodesToRead = readValues;
    readRequest.nodesToReadSize = 1u;
    readResponse = UA_Client_Service_read(opcuaClient, readRequest);
    ok = (readResponse.responseHeader.serviceResult == 0x00);
    if (ok) {
        /* Setting EO Memory */
        if (readResponse.results[0].value.arrayLength > 1u) {
            eos = new UA_ExtensionObject[readResponse.results[0].value.arrayLength];
            UA_Variant_setArray(&tempVariant[0u], eos, static_cast<osulong>(readResponse.results[0].value.arrayLength), &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
        }
        else {
            //eos = reinterpret_cast<UA_ExtensionObject*>(HeapManager::Malloc(sizeof(UA_ExtensionObject)));
            eos = UA_ExtensionObject_new();
            UA_Variant_setScalar(&tempVariant[0u], eos, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
        }
    }
    UA_ReadValueId_clear(readValues);
    return ok;
}

uint32 OPCUAClientWrapper::GetReferences(const UA_BrowseRequest bReq,
                                         const char8 *const path,
                                         uint16 &namespaceIndex,
                                         uint32 &numericNodeId,
                                         char8 *&stringNodeId) {
    uint32 id = 0u;
    if (numericNodeId == 0u) {
        bReq.nodesToBrowse[0].nodeId = UA_NODEID_STRING(namespaceIndex, stringNodeId);
    }
    else {
        bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(namespaceIndex, numericNodeId);
    }
    /*lint -e{1013} -e{63} -e{40} includeSubtypes is a member of struct UA_BrowseDescription.*/
    bReq.nodesToBrowse[0].includeSubtypes = true;
    bReq.nodesToBrowse[0].resultMask = 63u; /* UA_BROWSERESULTMASK_ALL */
    UA_BrowseResponse *bResp = UA_BrowseResponse_new();
    *bResp = UA_Client_Service_browse(opcuaClient, bReq);
    bool ok = (bResp->responseHeader.serviceResult == 0x00U); /* UA_STATUSCODE_GOOD */
    if (ok) {
        bool found = false;
        uint64 initCheckTime = HighResolutionTimer::Counter();
        uint64 finalCheckTime = 0u;
        uint32 count = 0u;
        while ((!found) && (bResp->resultsSize)) {
            for (uint32 i = 0u; i < bResp->resultsSize; ++i) {
                for (uint32 j = 0u; j < bResp->results[i].referencesSize; ++j) {
                    UA_ReferenceDescription *ref = &(bResp->results[i].references[j]);
                    if (StringHelper::Compare(reinterpret_cast<char8*>(ref->browseName.name.data), path) == 0) {
                        id = ref->referenceTypeId.identifier.numeric;
                        namespaceIndex = ref->nodeId.nodeId.namespaceIndex;
                        if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
                            numericNodeId = ref->nodeId.nodeId.identifier.numeric;
                            ok = true;
                        }
                        else if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_STRING) {
                            if (stringNodeId != NULL_PTR(char8*)) {
                                delete stringNodeId;
                            }
                            stringNodeId = new char8[ref->nodeId.nodeId.identifier.string.length];
                            ok = StringHelper::Copy(stringNodeId, reinterpret_cast<char8*>(ref->nodeId.nodeId.identifier.string.data));
                            if (ok) {
                                numericNodeId = 0u;
                            }
                        }
                        else {
                            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "NodeID identifier type not supported.");
                            ok = false;
                        }
                        if (ok) {
                            found = true;
                        }
                    }
                }
            }
            if ((!found) && bResp->results->continuationPoint.length) {
                UA_BrowseNextRequest nextReq;
                UA_BrowseNextRequest_init(&nextReq);
                nextReq.continuationPoints = UA_ByteString_new();
                UA_StatusCode code = UA_ByteString_copy(&(bResp->results->continuationPoint), nextReq.continuationPoints);
                if (code == 0x00U) { /* UA_STATUSCODE_GOOD */
                    nextReq.continuationPointsSize = 1u;
                    /*lint -e{740} open62541 needs the recast of bResp .*/
                    *reinterpret_cast<UA_BrowseNextResponse*>(bResp) = UA_Client_Service_browseNext(opcuaClient, nextReq);
                    UA_BrowseNextRequest_clear(&nextReq);
                }
            }
            finalCheckTime = HighResolutionTimer::Counter();
            if (HighResolutionTimer::TicksToTime(finalCheckTime, initCheckTime) > 5.0) {
                count++;
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Browse Service is taking too long. Is the NodeId or the Path right?");
                initCheckTime = HighResolutionTimer::Counter();
            }
            if (count > 1u) {
                count = 0u;
                break;
            }
        }
        UA_BrowseResponse_clear(bResp);
    }
    return id;
}

/*lint -e{746} -e{1055} -e{534} -e{516} -e{526} -e{628} UA_Variant_setScalar is defined in open62541.*/
void OPCUAClientWrapper::SetWriteRequest(const uint32 idx,
                                         const uint8 nDimensions,
                                         const uint32 nElements,
                                         const TypeDescriptor &type) {
    if (tempVariant != NULL_PTR(UA_Variant*)) {
        bool isArray = (nDimensions > 0u);
        if (valueMemories != NULL_PTR(void**)) {
            if (valueMemories[idx] != NULL_PTR(void*)) {
                if (type == UnsignedInteger8Bit) {
                    uint8 *value = reinterpret_cast<uint8*>(valueMemories[idx]);
                    if (isArray) {
                        UA_Variant_setArray(&tempVariant[idx], value, static_cast<osulong>(nElements), &UA_TYPES[UA_TYPES_BYTE]);
                    }
                    else {
                        UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_BYTE]);
                    }
                }
                else if (type == UnsignedInteger16Bit) {
                    uint16 *value = reinterpret_cast<uint16*>(valueMemories[idx]);
                    if (isArray) {
                        UA_Variant_setArray(&tempVariant[idx], value, static_cast<osulong>(nElements), &UA_TYPES[UA_TYPES_UINT16]);
                    }
                    else {
                        UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_UINT16]);
                    }
                }
                else if (type == UnsignedInteger32Bit) {
                    uint32 *value = reinterpret_cast<uint32*>(valueMemories[idx]);
                    if (isArray) {
                        UA_Variant_setArray(&tempVariant[idx], value, static_cast<osulong>(nElements), &UA_TYPES[UA_TYPES_UINT32]);
                    }
                    else {
                        UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_UINT32]);
                    }
                }
                else if (type == UnsignedInteger64Bit) {
                    uint64 *value = reinterpret_cast<uint64*>(valueMemories[idx]);
                    if (isArray) {
                        UA_Variant_setArray(&tempVariant[idx], value, static_cast<osulong>(nElements), &UA_TYPES[UA_TYPES_UINT64]);
                    }
                    else {
                        UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_UINT64]);
                    }
                }
                else if (type == SignedInteger8Bit) {
                    int8 *value = reinterpret_cast<int8*>(valueMemories[idx]);
                    if (isArray) {
                        UA_Variant_setArray(&tempVariant[idx], value, static_cast<osulong>(nElements), &UA_TYPES[UA_TYPES_SBYTE]);
                    }
                    else {
                        UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_SBYTE]);
                    }
                }
                else if (type == SignedInteger16Bit) {
                    int16 *value = reinterpret_cast<int16*>(valueMemories[idx]);
                    if (isArray) {
                        UA_Variant_setArray(&tempVariant[idx], value, static_cast<osulong>(nElements), &UA_TYPES[UA_TYPES_INT16]);
                    }
                    else {
                        UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_INT16]);
                    }
                }
                else if (type == SignedInteger32Bit) {
                    int32 *value = reinterpret_cast<int32*>(valueMemories[idx]);
                    if (isArray) {
                        UA_Variant_setArray(&tempVariant[idx], value, static_cast<osulong>(nElements), &UA_TYPES[UA_TYPES_INT32]);
                    }
                    else {
                        UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_INT32]);
                    }
                }
                else if (type == SignedInteger64Bit) {
                    int64 *value = reinterpret_cast<int64*>(valueMemories[idx]);
                    if (isArray) {
                        UA_Variant_setArray(&tempVariant[idx], value, static_cast<osulong>(nElements), &UA_TYPES[UA_TYPES_INT64]);
                    }
                    else {
                        UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_INT64]);
                    }
                }
                else if (type == Float32Bit) {
                    float32 *value = reinterpret_cast<float32*>(valueMemories[idx]);
                    if (isArray) {
                        UA_Variant_setArray(&tempVariant[idx], value, static_cast<osulong>(nElements), &UA_TYPES[UA_TYPES_FLOAT]);
                    }
                    else {
                        UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_FLOAT]);
                    }
                }
                else if (type == Float64Bit) {
                    float64 *value = reinterpret_cast<float64*>(valueMemories[idx]);
                    if (isArray) {
                        UA_Variant_setArray(&tempVariant[idx], value, static_cast<osulong>(nElements), &UA_TYPES[UA_TYPES_DOUBLE]);
                    }
                    else {
                        UA_Variant_setScalar(&tempVariant[idx], value, &UA_TYPES[UA_TYPES_DOUBLE]);
                    }
                }
                else {
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Write request: Type not supported.");
                }
            }
        }
        if (writeValues != NULL_PTR(UA_WriteValue*)) {
            writeValues[idx].value.value = static_cast<const UA_Variant>(tempVariant[idx]);
            /*lint -e{1013} -e{63} -e{40} hasValue is a member of struct UA_DataValue.*/
            writeValues[idx].value.hasValue = true;
        }
    }
}

bool OPCUAClientWrapper::Write() {
    bool ok = true;
    if (dataPtr != NULL_PTR(void*)) {
        UA_ExtensionObject *valuePtr = reinterpret_cast<UA_ExtensionObject*>(readResponse.results[0].value.data);
        uint32 actualBodyLength;
        if (readResponse.results[0].value.arrayLength > 1u) {
            for (uint32 j = 0u; j < readResponse.results[0].value.arrayLength; j++) {
                if (ok) {
                    UA_ExtensionObject_copy(&valuePtr[j], &eos[j]);
                    ok = MemoryOperationsHelper::Copy(eos[j].content.encoded.body.data, dataPtr, eos[j].content.encoded.body.length);
                    dataPtr = reinterpret_cast<uint8*>(dataPtr) + eos[j].content.encoded.body.length;
                }
            }
            actualBodyLength = (readResponse.results[0].value.arrayLength * eos[0u].content.encoded.body.length);
            SeekDataPtr(actualBodyLength);
        }
        else {
            UA_ExtensionObject_copy(valuePtr, eos);
            ok = MemoryOperationsHelper::Copy(eos->content.encoded.body.data, dataPtr, eos->content.encoded.body.length);
        }
        if (writeValues != NULL_PTR(UA_WriteValue*)) {
            writeValues[0u].value.value = static_cast<const UA_Variant>(tempVariant[0u]);
            /*lint -e{1013} -e{63} -e{40} hasValue is a member of struct UA_DataValue.*/
            writeValues[0u].value.hasValue = true;
        }
        writeRequest.nodesToWriteSize = 1u;
    }
    else {
        writeRequest.nodesToWriteSize = nOfNodes;
    }
    writeRequest.nodesToWrite = writeValues;
    UA_WriteResponse wResp = UA_Client_Service_write(opcuaClient, writeRequest);
    ok = (wResp.responseHeader.serviceResult == 0x00U); /* UA_STATUSCODE_GOOD */
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "WriteError - OPC UA Status Code (Part 4 - 7.34): %x", wResp.responseHeader.serviceResult);
        UA_Client_run_iterate(opcuaClient, 100u);
    }
    UA_WriteResponse_deleteMembers(&wResp);
    return ok;
}

bool OPCUAClientWrapper::MethodCall() {
    bool ok = true;
    UA_StatusCode retval;
    retval = readResponse.responseHeader.serviceResult;
    if (retval == 0x00) {
        uint32 actualBodyLength;
        UA_ExtensionObject *valuePtr = reinterpret_cast<UA_ExtensionObject*>(readResponse.results[0].value.data);
        if (readResponse.results[0].value.arrayLength > 1u) {
            for (uint32 j = 0u; j < readResponse.results[0].value.arrayLength; j++) {
                if (ok) {
                    UA_ExtensionObject_copy(&valuePtr[j], &eos[j]);
                    ok = MemoryOperationsHelper::Copy(eos[j].content.encoded.body.data, dataPtr, eos[j].content.encoded.body.length);
                    dataPtr = reinterpret_cast<uint8*>(dataPtr) + eos[j].content.encoded.body.length;
                }
            }
            actualBodyLength = (readResponse.results[0].value.arrayLength * eos[0u].content.encoded.body.length);
            SeekDataPtr(actualBodyLength);
        }
        else {
            UA_ExtensionObject_copy(valuePtr, eos);
            ok = MemoryOperationsHelper::Copy(eos->content.encoded.body.data, dataPtr, eos->content.encoded.body.length);
        }
        UA_Variant *output;
        size_t outSize = 1u;
        retval = UA_Client_call(opcuaClient, objectMethod, methodNodeId, 1u, tempVariant, &outSize, &output);
        valuePtr = NULL_PTR(UA_ExtensionObject*);
    }
    /* Renew Secure Channel when timed out */
    UA_Client_run_iterate(opcuaClient, 100u);
    ok = (retval == 0x00U); /* UA_STATUSCODE_GOOD */
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "METHOD CALL FAILED  - STATUSCODE: %x", retval);
    }
    return ok;
}

#if 0
void OPCUAClientWrapper::UpdateMemory(UA_DataValue *value) {
    //Delete this before implementing this method
    value = NULL_PTR(UA_DataValue *);

}
#endif

void OPCUAClientWrapper::SetSamplingTime(const float64 sampleTime) {
    samplingTime = sampleTime;
}

UA_NodeId* OPCUAClientWrapper::GetMonitoredNodes() {
    return monitoredNodes;
}

void OPCUAClientWrapper::SetOperationMode(const char8 *const modeType) {
    mode = modeType;
}

bool OPCUAClientWrapper::DecodeExtensionObjectByteString(const TypeDescriptor *const&entryTypes,
                                                         const uint32 *const&entryArrayElements,
                                                         const uint32 *const&entryNumberOfMembers,
                                                         const uint32 entryArraySize,
                                                         uint32 &nodeCounter,
                                                         uint32 &index) {
    bool ok = true;
    if (!entryTypes[index].isStructuredData) {
        if (entryArrayElements[index] == 1u) {
            uint32 nOfBytes = entryTypes[index].numberOfBits;
            nOfBytes /= 8u;
            valueMemories[nodeCounter] = reinterpret_cast<void*>(tempDataPtr);
            tempDataPtr = tempDataPtr + nOfBytes;
        }
        else {
            tempDataPtr = tempDataPtr + 4; /* Skip 4 bytes */
            uint32 nOfBytes = entryTypes[index].numberOfBits;
            nOfBytes /= 8u;
            nOfBytes *= entryArrayElements[index];
            valueMemories[nodeCounter] = reinterpret_cast<void*>(tempDataPtr);
            tempDataPtr = tempDataPtr + nOfBytes;
        }
        nodeCounter++;
        index++;
    }
    else { /* if isStructuredData */
        uint32 newIndex;
        if (entryArrayElements[index] == 1u) {
            index++;
            for (uint32 j = 0u; j < entryNumberOfMembers[index - 1u]; j++) {
                ok = DecodeExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, index);
            }
        }
        if (entryArrayElements[index] > 1u) {
            tempDataPtr = tempDataPtr + 4; /* Skip 4 bytes */
            uint32 nMembers = entryNumberOfMembers[index];
            for (uint32 i = 0u; i < entryArrayElements[index]; i++) {
                newIndex = index + 1u;
                for (uint32 j = 0u; j < entryNumberOfMembers[index]; j++) {
                    ok = DecodeExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, newIndex);
                }
            }
            index += (nMembers + 1u);
        }
    }
    return ok;
}

bool OPCUAClientWrapper::EncodeExtensionObjectByteString(const TypeDescriptor *const&entryTypes,
                                                         const uint32 *const&entryArrayElements,
                                                         const uint32 *const&entryNumberOfMembers,
                                                         const uint32 entryArraySize,
                                                         uint32 &nodeCounter,
                                                         uint32 &index) {
    bool ok = true;
    if (!entryTypes[index].isStructuredData) {
        if (entryArrayElements[index] == 1u) {
            uint32 nOfBytes = entryTypes[index].numberOfBits;
            nOfBytes /= 8u;
            valueMemories[nodeCounter] = reinterpret_cast<void*>(tempDataPtr);
            tempDataPtr = tempDataPtr + nOfBytes;
        }
        else {
            ok = MemoryOperationsHelper::Copy(tempDataPtr, &entryArrayElements[index], sizeof(int32));
            if (ok) {
                tempDataPtr = tempDataPtr + 4; /* Skip 4 bytes */
                uint32 nOfBytes = entryTypes[index].numberOfBits;
                nOfBytes /= 8u;
                nOfBytes *= entryArrayElements[index];
                valueMemories[nodeCounter] = reinterpret_cast<void*>(tempDataPtr);
                tempDataPtr = tempDataPtr + nOfBytes;
            }
        }
        if (ok) {
            nodeCounter++;
            index++;
        }
    }
    else { /* if isStructuredData */
        uint32 newIndex;
        if (entryArrayElements[index] == 1u) {
            index++;
            for (uint32 j = 0u; j < entryNumberOfMembers[index - 1u]; j++) {
                EncodeExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, index);
            }
        }
        if (entryArrayElements[index] > 1u) {
            ok = MemoryOperationsHelper::Copy(tempDataPtr, &entryArrayElements[index], sizeof(int32));
            if (ok) {
                tempDataPtr = tempDataPtr + 4; /* Skip 4 bytes */
                uint32 nMembers = entryNumberOfMembers[index];
                for (uint32 i = 0u; i < entryArrayElements[index]; i++) {
                    newIndex = index + 1u;
                    for (uint32 j = 0u; j < entryNumberOfMembers[index]; j++) {
                        EncodeExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, newIndex);
                    }
                }
                index += (nMembers + 1u);
            }
        }
    }
    return ok;
}
}
/*lint -restore*/
