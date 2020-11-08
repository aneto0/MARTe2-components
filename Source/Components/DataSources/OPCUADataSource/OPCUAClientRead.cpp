/**
 * @file OPCUAClientRead.cpp
 * @brief Source file for class OPCUAClientRead
 * @date 28/10/2019
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
 * the class OPCUAClientRead (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "OPCUAClientRead.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/*-e909 and -e9133 redefines bool.*/
/*lint -save -e909 -e9133 -e578*/
namespace MARTe {

/*lint -e{1401} no need to initialise readRequest*/
OPCUAClientRead::OPCUAClientRead() :
        OPCUAClientI() {
    monitoredNodes = NULL_PTR(UA_NodeId*);
    readValues = NULL_PTR(UA_ReadValueId*);
}

/*lint -e{1579} all pointers have been freed*/
OPCUAClientRead::~OPCUAClientRead() {
    /*lint -e{1551} no exception thrown*/
    bool ok = UnregisterNodes(monitoredNodes);
    if (ok) {
        /*lint -e{1551} no exception thrown*/
        UA_Array_delete(monitoredNodes, static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_NODEID]);
        /*lint -e{1551} no exception thrown*/
        UA_Array_delete(readValues, static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_READVALUEID]);
    }
}
bool OPCUAClientRead::SetServiceRequest(const uint16 *const namespaceIndexes,
                                        StreamString *const nodePaths,
                                        const uint32 numberOfNodes) {
    bool ok = false;
    nOfNodes = numberOfNodes;

    monitoredNodes = reinterpret_cast<UA_NodeId*>(UA_Array_new(static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_NODEID]));
    readValues = reinterpret_cast<UA_ReadValueId*>(UA_Array_new(static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_READVALUEID]));
    /* Setting up Read request */
    UA_ReadRequest_init(&readRequest);

    for (uint32 i = 0u; i < nOfNodes; i++) {
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
            //UA_BrowseDescription_init(bReq.nodesToBrowse);
            bReq.nodesToBrowseSize = 1u;

            /* Building request for TranslateBrowsePathsToNodeIds */
            UA_BrowsePath browsePath;
            UA_BrowsePath_init(&browsePath);
            browsePath.startingNode = UA_NODEID_NUMERIC(0u, 85u); /* UA_NS0ID_OBJECTSFOLDER */
            browsePath.relativePath.elements = reinterpret_cast<UA_RelativePathElement*>(UA_Array_new(static_cast<osulong>(pathSize),
                                                                                                      &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]));
            browsePath.relativePath.elementsSize = pathSize;

            uint32 tempNumericNodeId = 85u; /* UA_NS0ID_OBJECTSFOLDER */
            uint16 tempNamespaceIndex = 0u;
            char8 *tempStringNodeId = NULL_PTR(char8*);
            if ((path != NULL_PTR(StreamString*)) && (ids != NULL_PTR(uint32*))) {
                for (uint32 j = 0u; j < pathSize; j++) {
                    ids[j] = GetReferences(bReq, const_cast<char8*>(path[j].Buffer()), tempNamespaceIndex, tempNumericNodeId, tempStringNodeId);
                    UA_RelativePathElement *elem = &(browsePath.relativePath.elements[j]);
                    elem->referenceTypeId = UA_NODEID_NUMERIC(0u, ids[j]);
                    /*lint -e{1055} -e{64} -e{746} UA_QUALIFIEDNAME is declared in the open62541 library.*/
                    elem->targetName = UA_QUALIFIEDNAME_ALLOC(namespaceIndexes[i], const_cast<char8*>(path[j].Buffer()));
                }
                if (tempStringNodeId != NULL_PTR(char8*)) {
                    delete tempStringNodeId;
                }
            }

            UA_TranslateBrowsePathsToNodeIdsRequest tbpReq;
            UA_TranslateBrowsePathsToNodeIdsRequest_init(&tbpReq);
            tbpReq.browsePaths = &browsePath;
            tbpReq.browsePathsSize = 1u;

            UA_TranslateBrowsePathsToNodeIdsResponse tbpResp = UA_Client_Service_translateBrowsePathsToNodeIds(opcuaClient, tbpReq);
            ok = (tbpResp.responseHeader.serviceResult == 0x00U); /* UA_STATUSCODE_GOOD */
            if (ok) {
                if (tbpResp.results[0].statusCode == 0x00U) {
                    UA_BrowsePathTarget ref;
                    (void) UA_BrowsePathTarget_copy(&(tbpResp.results[0].targets[0]), &ref);
                    (void) UA_NodeId_copy(&(ref.targetId.nodeId), &monitoredNodes[i]);
                    if (readValues != NULL_PTR(UA_ReadValueId*)) {
                        //UA_ReadValueId_init(&readValues[i]);
                        readValues[i].attributeId = 13u; /* UA_ATTRIBUTEID_VALUE */
                        (void) UA_NodeId_copy(&(ref.targetId.nodeId), &(readValues[i].nodeId));
                    }
                }
                else {
                    REPORT_ERROR_STATIC(ErrorManagement::Information, "An Error occurred on TranslateBrowsePathsToNodeIds service call");
                    ok = false;
                }
            }
            UA_Array_delete(browsePath.relativePath.elements, static_cast<osulong>(pathSize), &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]);
        }
        delete[] ids;
        delete[] path;
    }
    if (ok) {
        readRequest.nodesToRead = readValues;
        readRequest.nodesToReadSize = nOfNodes;
        ok = RegisterNodes(monitoredNodes);
    }
    return ok;
}

bool OPCUAClientRead::GetExtensionObjectByteString(const TypeDescriptor *const&entryTypes,
                                                   const uint32 *const&entryArrayElements,
                                                   const uint32 *const&entryNumberOfMembers,
                                                   const uint32 entryArraySize,
                                                   uint32 &nodeCounter,
                                                   uint32 &index) {
    bool ok = false;
    if (!entryTypes[index].isStructuredData) {
        if ((valueMemories != NULL_PTR(void**)) && (tempDataPtr != NULL_PTR(uint8*))) {
            if (entryArrayElements[index] == 1u) {
                uint32 nOfBytes = entryTypes[index].numberOfBits;
                nOfBytes /= 8u;
                valueMemories[nodeCounter] = reinterpret_cast<void*>(tempDataPtr);
                tempDataPtr = &(tempDataPtr[nOfBytes]);
                ok = true;
            }
            else {
                tempDataPtr = &(tempDataPtr[4u]); /* Skip 4 bytes */
                uint32 nOfBytes = entryTypes[index].numberOfBits;
                nOfBytes /= 8u;
                nOfBytes *= entryArrayElements[index];
                valueMemories[nodeCounter] = reinterpret_cast<void*>(tempDataPtr);
                tempDataPtr = &(tempDataPtr[nOfBytes]);
                ok = true;
            }
            nodeCounter++;
            index++;
        }
    }
    else { /* if isStructuredData */
        uint32 newIndex;
        if (entryArrayElements[index] == 1u) {
            index++;
            for (uint32 j = 0u; j < entryNumberOfMembers[index - 1u]; j++) {
                ok = GetExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, index);
            }
        }
        if (entryArrayElements[index] > 1u) {
            if (tempDataPtr != NULL_PTR(uint8*)) {
                tempDataPtr = &(tempDataPtr[4u]); /* Skip 4 bytes */
            }
            uint32 nMembers = entryNumberOfMembers[index];
            for (uint32 i = 0u; i < entryArrayElements[index]; i++) {
                newIndex = index + 1u;
                for (uint32 j = 0u; j < entryNumberOfMembers[index]; j++) {
                    ok = GetExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, newIndex);
                }
            }
            index += (nMembers + 1u);
        }
    }
    return ok;
}

bool OPCUAClientRead::Read(const TypeDescriptor *const types,
                           const uint32 *const nElements) {
    UA_ReadResponse readResponse;
    readResponse = UA_Client_Service_read(opcuaClient, readRequest);
    bool ok = (readResponse.responseHeader.serviceResult == 0x00U); /* UA_STATUSCODE_GOOD */
    if (ok) {
        if (valueMemories != NULL_PTR(void**)) {
            if (readResponse.results[0].value.type->typeId.identifier.numeric == 22u) { /* EXTENSION_OBJECT */
                if (dataPtr != NULL_PTR(void*)) {
                    UA_ExtensionObject *valuePtr = reinterpret_cast<UA_ExtensionObject*>(readResponse.results[0].value.data);
                    UA_ExtensionObject *eos;
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
                                    ok = MemoryOperationsHelper::Copy(dataPtr, eos[i].content.encoded.body.data,
                                                                      static_cast<uint32>(eos[i].content.encoded.body.length));
                                    dataPtr = &reinterpret_cast<uint8*>(dataPtr)[eos[i].content.encoded.body.length];
                                }
                            }
                        }
                        uint32 actualBodyLength = (static_cast<uint32>(readResponse.results[0].value.arrayLength)
                                * static_cast<uint32>(eos[0u].content.encoded.body.length));
                        SeekDataPtr(actualBodyLength);
                        delete[] eos;
                    }
                    else {
                        eos = UA_ExtensionObject_new();
                        (void) UA_ExtensionObject_copy(valuePtr, eos);
                        ok = MemoryOperationsHelper::Copy(dataPtr, eos->content.encoded.body.data, static_cast<uint32>(eos->content.encoded.body.length));
                        if (ok) {
                            /*lint -e{526} -e{628} -e{1551} -e{1055} no exception thrown, function defined in open62541*/
                            (void) UA_ExtensionObject_clear(eos);
                            UA_ExtensionObject_delete(eos);
                        }
                    }
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
        (void) UA_Client_run_iterate(opcuaClient, 100u);
    }
    return ok;
}

bool OPCUAClientRead::RegisterNodes(const UA_NodeId *const monitoredNodes) {
    bool ok = false;
    if (monitoredNodes != NULL_PTR(UA_NodeId*)) {
        UA_RegisterNodesRequest rReq;
        UA_RegisterNodesRequest_init(&rReq);
        rReq.nodesToRegisterSize = nOfNodes;
        rReq.nodesToRegister = static_cast<UA_NodeId*>(UA_Array_new(static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_NODEID]));
        for (uint32 k = 0u; k < nOfNodes; k++) {
            (void) UA_NodeId_copy(&monitoredNodes[k], &(rReq.nodesToRegister[k]));
        }
        UA_RegisterNodesResponse rResp = UA_Client_Service_registerNodes(opcuaClient, rReq);
        ok = (rResp.responseHeader.serviceResult == 0x00U);
        UA_Array_delete(rReq.nodesToRegister, static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_NODEID]);
    }
    return ok;
}

bool OPCUAClientRead::UnregisterNodes(const UA_NodeId *const monitoredNodes) {
    bool ok = false;
    if (monitoredNodes != NULL_PTR(UA_NodeId*)) {
        UA_UnregisterNodesRequest rReq;
        UA_UnregisterNodesRequest_init(&rReq);
        rReq.nodesToUnregisterSize = nOfNodes;
        rReq.nodesToUnregister = static_cast<UA_NodeId*>(UA_Array_new(static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_NODEID]));
        for (uint32 k = 0u; k < nOfNodes; k++) {
            (void) UA_NodeId_copy(&monitoredNodes[k], &(rReq.nodesToUnregister[k]));
        }
        UA_UnregisterNodesResponse rResp = UA_Client_Service_unregisterNodes(opcuaClient, rReq);
        ok = (rResp.responseHeader.serviceResult == 0x00U);
        UA_Array_delete(rReq.nodesToUnregister, static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_NODEID]);
    }
    return ok;
}

UA_NodeId* OPCUAClientRead::GetMonitoredNodes() {
    return monitoredNodes;
}

}
/*lint -restore*/
