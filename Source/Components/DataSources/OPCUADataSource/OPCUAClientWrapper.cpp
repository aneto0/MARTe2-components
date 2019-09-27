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
    //config = UA_ClientConfig_default;
    //opcuaClient = UA_Client_new(config);
    opcuaClient = UA_Client_new();
    config = UA_Client_getConfig(opcuaClient);
    UA_ClientConfig_setDefault(config);
    monitoredNodes = NULL_PTR(UA_NodeId*);
    request = UA_CreateSubscriptionRequest_default();
    response.subscriptionId = 0u;
    monitorRequest.itemToMonitor.nodeId.identifier.numeric = 0u;
    monitorResponse.monitoredItemId = 0u;
    valueMemories = NULL_PTR(void**);
    serverAddress = NULL_PTR(char8*);
    samplingTime = 0.0;
    readValues = NULL_PTR(UA_ReadValueId*);
    writeValues = NULL_PTR(UA_WriteValue*);
    tempVariant = NULL_PTR(UA_Variant*);
    nOfNodes = 0u;
    UA_ReadRequest_init(&readRequest);
    UA_WriteRequest_init(&writeRequest);
    readResponse.responseHeader.serviceResult = 0x00U; /* UA_STATUSCODE_GOOD */
}

/*lint -e{1551} No exception thrown.*/
/*lint -e{1740} opcuaClient freed by UA_Client_delete.*/
OPCUAClientWrapper::~OPCUAClientWrapper() {
    UA_StatusCode code = UA_Client_disconnect(opcuaClient);
    if (code == 0x00U) { /* UA_STATUSCODE_GOOD */
        if (opcuaClient != NULL_PTR(UA_Client*)) {
            UA_Client_delete(opcuaClient);
        }
    }
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
        bool ok = true;
        for (uint32 j = 0u; j < nOfNodes; j++) {
            if ((valueMemories[j] != NULL_PTR(void*)) && ok) {
                ok = HeapManager::Free(valueMemories[j]);
            }
        }
        if (ok) {
            if (ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Information, "Value memories have been freed successfully.");
            }
        }
        delete[] valueMemories;
    }
}

void OPCUAClientWrapper::SetServerAddress(StreamString address) {
    serverAddress = address;
}

bool OPCUAClientWrapper::Connect() {
    UA_StatusCode retval = UA_Client_connect(opcuaClient, const_cast<char8*>(serverAddress.Buffer()));
    return (retval == 0x00U); /* UA_STATUSCODE_GOOD */
}

bool OPCUAClientWrapper::SetTargetNodes(const uint16 *const namespaceIndexes,
                                        StreamString *const nodePaths,
                                        const uint32 numberOfNodes) {
    bool ok = false;
    nOfNodes = numberOfNodes;
    monitoredNodes = new UA_NodeId[numberOfNodes];
    tempVariant = new UA_Variant[numberOfNodes];
    if (mode == "Read") {
        /* Setting up Read request */
        readValues = new UA_ReadValueId[numberOfNodes];
    }
    else { /* mode == "Write" */
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
            UA_BrowseRequest bReq;
            UA_BrowseRequest_init(&bReq);
            bReq.requestedMaxReferencesPerNode = 0u;
            bReq.nodesToBrowse = UA_BrowseDescription_new();
            bReq.nodesToBrowseSize = 1u;

            /* Building request for TranslateBrowsePathsToNodeIds */
            UA_BrowsePath browsePath;
            UA_BrowsePath_init(&browsePath);
            browsePath.startingNode = UA_NODEID_NUMERIC(0u, 85u); /* UA_NS0ID_OBJECTSFOLDER */
            browsePath.relativePath.elements = static_cast<UA_RelativePathElement*>(UA_Array_new(static_cast<osulong>(pathSize),
                                                                                                 &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]));
            browsePath.relativePath.elementsSize = pathSize;

            uint32 tempNumericNodeId = 85u; /* UA_NS0ID_OBJECTSFOLDER */
            uint16 tempNamespaceIndex = 0u;
            char8 *tempStringNodeId;
            if ((path != NULL_PTR(StreamString*)) && (ids != NULL_PTR(uint32*))) {
                for (uint32 j = 0u; j < pathSize; j++) {
                    ids[j] = GetReferenceType(bReq, const_cast<char8*>(path[j].Buffer()), tempNamespaceIndex, tempNumericNodeId, tempStringNodeId);
                    UA_RelativePathElement *elem = &browsePath.relativePath.elements[j];
                    elem->referenceTypeId = UA_NODEID_NUMERIC(0u, ids[j]);
                    /*lint -e{1055} -e{64} -e{746} UA_QUALIFIEDNAME is declared in the open62541 library.*/
                    elem->targetName = UA_QUALIFIEDNAME_ALLOC(namespaceIndexes[i], const_cast<char8*>(path[j].Buffer()));
                }
            }

            UA_TranslateBrowsePathsToNodeIdsRequest tbpReq;
            UA_TranslateBrowsePathsToNodeIdsRequest_init(&tbpReq);
            tbpReq.browsePaths = &browsePath;
            tbpReq.browsePathsSize = 1u;
            UA_TranslateBrowsePathsToNodeIdsResponse tbpResp = UA_Client_Service_translateBrowsePathsToNodeIds(opcuaClient, tbpReq);
            ok = (tbpResp.results[0].statusCode == 0x00U); /* UA_STATUSCODE_GOOD */
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Impossible to register Node %s", nodePaths[i].Buffer());
            }
            if (ok) {
                UA_BrowsePathTarget *ref = &(tbpResp.results[0].targets[0]);
                monitoredNodes[i] = ref->targetId.nodeId;
                if (mode == "Read") {
                    if (readValues != NULL_PTR(UA_ReadValueId*)) {
                        UA_ReadValueId_init(&readValues[i]);
                        readValues[i].attributeId = 13u; /* UA_ATTRIBUTEID_VALUE */
                        readValues[i].nodeId = monitoredNodes[i];
                    }
                }
                else {
                    if (writeValues != NULL_PTR(UA_WriteValue*)) {
                        UA_WriteValue_init(&writeValues[i]);
                        writeValues[i].attributeId = 13u; /* UA_ATTRIBUTEID_VALUE */
                        writeValues[i].nodeId = monitoredNodes[i];
                    }
                }
                //UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&tbpResp);
                UA_Array_delete(browsePath.relativePath.elements, static_cast<osulong>(pathSize), &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]);
                UA_BrowseDescription_delete(bReq.nodesToBrowse);
            }
        }
        delete[] ids;
        delete[] path;
    }
    if (ok) {
        if (mode == "Read") {
            readRequest.nodesToRead = readValues;
            readRequest.nodesToReadSize = numberOfNodes;
        }
    }
    return ok;
}

void OPCUAClientWrapper::SetValueMemories(const uint32 numberOfNodes) {
    valueMemories = new void*[numberOfNodes];
    for (uint32 i = 0u; i < numberOfNodes; i++) {
        valueMemories[i] = NULL_PTR(void*);
    }
}

bool OPCUAClientWrapper::GetSignalMemory(void *&mem,
                                         const uint32 idx,
                                         const TypeDescriptor &valueTd,
                                         const uint32 nElem,
                                         const uint8 nDimensions) {
    bool ok = true;
    if (valueMemories != NULL_PTR(void**)) {
        uint32 nOfBytes = valueTd.numberOfBits;
        nOfBytes /= 8u;
        nOfBytes *= nElem;
        valueMemories[idx] = HeapManager::Malloc(nOfBytes);

        mem = valueMemories[idx];
    }

    return ok;

}

bool OPCUAClientWrapper::Monitor() const {
    bool ok = false;
    REPORT_ERROR_STATIC(ErrorManagement::UnsupportedFeature, "Monitored Items not supported yet.");

    return ok;
}

bool OPCUAClientWrapper::Read(const uint32 numberOfNodes,
                              const TypeDescriptor *const types,
                              const uint32 *const nElements,
                              const uint32 *const entryArrayElements,
                              const TypeDescriptor *const entryTypes,
                              const uint32 *const entryNumberOfMembers,
                              const uint32 entryArraySize) {
    readResponse = UA_Client_Service_read(opcuaClient, readRequest);
    bool ok = (readResponse.responseHeader.serviceResult == 0x00U); /* UA_STATUSCODE_GOOD */
    if (ok) {
        if (valueMemories != NULL_PTR(void**)) {
            if (readResponse.results[0].value.type->typeId.identifier.numeric == 22) { /* EXTENSION_OBJECT */
                if (entryArrayElements != NULL_PTR(uint32*)) {
                    UA_ExtensionObject *eo = UA_ExtensionObject_new();
                    UA_ExtensionObject *valuePtr = reinterpret_cast<UA_ExtensionObject*>(readResponse.results[0].value.data);
                    uint32 nodeCounter = 0u;
                    uint32 index = 0u;
                    uint8 *dataPtr = NULL_PTR(uint8*);
                    for (uint32 j = 0u; j < readResponse.results[0].value.arrayLength; j++) {
                        ok = (UA_ExtensionObject_copy(&valuePtr[j], eo) == 0x00U);
                        if (ok) {
                            dataPtr = reinterpret_cast<uint8*>(eo->content.encoded.body.data);
                            index = 1u;
                            uint32 numberOfNodesForEachIteration = (numberOfNodes / readResponse.results[0].value.arrayLength) * (j + 1);
                            while (nodeCounter < numberOfNodesForEachIteration) {
                                DecodeExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, dataPtr,
                                                                index);
                            }
                            dataPtr = NULL_PTR(uint8*);
                        }
                    }
                }
            }
            else {

                for (uint32 i = 0u; i < numberOfNodes; i++) {
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
    }
    return ok;
}

uint32 OPCUAClientWrapper::GetReferenceType(const UA_BrowseRequest &bReq,
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
                            stringNodeId = new char8[ref->nodeId.nodeId.identifier.string.length + 1];
                            ok = StringHelper::CopyN(stringNodeId, reinterpret_cast<char8*>(ref->nodeId.nodeId.identifier.string.data),
                                                     ref->nodeId.nodeId.identifier.string.length);
                            stringNodeId[ref->nodeId.nodeId.identifier.string.length] = '\0';
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
                    /*lint -e{740} The library need the recast of bResp .*/
                    *reinterpret_cast<UA_BrowseNextResponse*>(bResp) = UA_Client_Service_browseNext(opcuaClient, nextReq);
                    UA_BrowseNextRequest_deleteMembers(&nextReq);
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
        UA_BrowseResponse_delete(bResp);
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

bool OPCUAClientWrapper::Write(const uint32 numberOfNodes,
                               const uint32 extensionObjectFlag,
                               const uint32 *const entryArrayElements,
                               const TypeDescriptor *const entryTypes,
                               const uint32 *const entryNumberOfMembers,
                               const uint32 entryArraySize) {
    if (extensionObjectFlag == 1u) {
        readValues = new UA_ReadValueId[1u];
        UA_ReadValueId_init(&readValues[0u]);
        readValues[0u].attributeId = 13u; /* UA_ATTRIBUTEID_VALUE */
        readValues[0u].nodeId = monitoredNodes[0u];
        readRequest.nodesToRead = readValues;
        readRequest.nodesToReadSize = 1u;
        readResponse = UA_Client_Service_read(opcuaClient, readRequest);
        UA_ExtensionObject *valuePtr = reinterpret_cast<UA_ExtensionObject*>(readResponse.results[0].value.data);
        UA_ExtensionObject *eos = new UA_ExtensionObject[readResponse.results[0].value.arrayLength];
        uint8 *dataPtr = NULL_PTR(uint8*);
        uint32 index = 0u;
        uint32 nodeCounter = 0u;
        for (uint32 j = 0u; j < readResponse.results[0].value.arrayLength; j++) {
            UA_ExtensionObject_copy(&valuePtr[j], &eos[j]);
            dataPtr = reinterpret_cast<uint8*>(eos[j].content.encoded.body.data);
            index = 1u;
            uint32 numberOfNodesForEachIteration = (numberOfNodes / readResponse.results[0].value.arrayLength) * (j + 1);
            while (nodeCounter < numberOfNodesForEachIteration) {
                EncodeExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, dataPtr, index);
            }
        }
        if (readResponse.results[0].value.arrayLength > 1u) {
            UA_Variant_setArray(&tempVariant[0u], eos, static_cast<osulong>(readResponse.results[0].value.arrayLength), &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
        }
        else {
            UA_Variant_setScalar(&tempVariant[0u], eos, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
        }

        if (writeValues != NULL_PTR(UA_WriteValue*)) {
            writeValues[0u].value.value = static_cast<const UA_Variant>(tempVariant[0u]);
            /*lint -e{1013} -e{63} -e{40} hasValue is a member of struct UA_DataValue.*/
            writeValues[0u].value.hasValue = true;
        }
        writeRequest.nodesToWriteSize = 1u;
    }
    else {
        writeRequest.nodesToWriteSize = numberOfNodes;
    }
    writeRequest.nodesToWrite = writeValues;
    UA_WriteResponse wResp = UA_Client_Service_write(opcuaClient, writeRequest);
    bool ok = (wResp.responseHeader.serviceResult == 0x00U); /* UA_STATUSCODE_GOOD */
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "WriteError - OPC UA Status Code (Part 4 - 7.34): %x", wResp.responseHeader.serviceResult);
    }
    UA_WriteResponse_deleteMembers(&wResp);
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
                                                         uint8 *&dataPtr,
                                                         uint32 &index) {
    bool ok;
    if (!entryTypes[index].isStructuredData) {
        if (entryArrayElements[index] == 1u) {
            uint32 nOfBytes = entryTypes[index].numberOfBits;
            nOfBytes /= 8u;
            ok = MemoryOperationsHelper::Copy(valueMemories[nodeCounter], dataPtr, nOfBytes);
            //valueMemories[nodeCounter] = &dataPtr;
            dataPtr += nOfBytes;
        }
        else {
            dataPtr += 4; /* Skip 4 bytes */
            uint32 nOfBytes = entryTypes[index].numberOfBits;
            nOfBytes /= 8u;
            nOfBytes *= entryArrayElements[index];
            ok = MemoryOperationsHelper::Copy(valueMemories[nodeCounter], dataPtr, nOfBytes);
            //valueMemories[nodeCounter] = &dataPtr;
            dataPtr += nOfBytes;
        }
        nodeCounter++;
        index++;
    }
    else { /* if isStructuredData */
        uint32 newIndex;
        if (entryArrayElements[index] == 1u) {
            index++;
            for (uint32 j = 0u; j < entryNumberOfMembers[index - 1u]; j++) {
                DecodeExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, dataPtr, index);
            }
        }
        if (entryArrayElements[index] > 1u) {
            dataPtr += 4u; /* Skip 4 bytes */
            uint32 nMembers = entryNumberOfMembers[index];
            for (uint32 i = 0u; i < entryArrayElements[index]; i++) {
                newIndex = index + 1u;
                for (uint32 j = 0u; j < entryNumberOfMembers[index]; j++) {
                    DecodeExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, dataPtr, newIndex);
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
                                                         uint8 *&dataPtr,
                                                         uint32 &index) {
    bool ok;
    if (!entryTypes[index].isStructuredData) {
        if (entryArrayElements[index] == 1u) {
            uint32 nOfBytes = entryTypes[index].numberOfBits;
            nOfBytes /= 8u;
            ok = MemoryOperationsHelper::Copy(dataPtr, valueMemories[nodeCounter], nOfBytes);
            dataPtr += nOfBytes;
        }
        else {
            ok = MemoryOperationsHelper::Copy(dataPtr, &entryArrayElements[index], sizeof(int32));
            if (ok) {
                dataPtr += 4; /* Skip 4 bytes */
                uint32 nOfBytes = entryTypes[index].numberOfBits;
                nOfBytes /= 8u;
                nOfBytes *= entryArrayElements[index];
                ok = MemoryOperationsHelper::Copy(dataPtr, valueMemories[nodeCounter], nOfBytes);
                dataPtr += nOfBytes;
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
                EncodeExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, dataPtr, index);
            }
        }
        if (entryArrayElements[index] > 1u) {
            ok = MemoryOperationsHelper::Copy(dataPtr, &entryArrayElements[index], sizeof(int32));
            if (ok) {
                dataPtr += 4u; /* Skip 4 bytes */
                uint32 nMembers = entryNumberOfMembers[index];
                for (uint32 i = 0u; i < entryArrayElements[index]; i++) {
                    newIndex = index + 1u;
                    for (uint32 j = 0u; j < entryNumberOfMembers[index]; j++) {
                        EncodeExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, dataPtr, newIndex);
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
