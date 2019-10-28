/**
 * @file OPCUAClientWrite.cpp
 * @brief Source file for class OPCUAClientWrite
 * @date Oct 28, 2019 TODO Verify the value and format of the date
 * @author lporzio TODO Verify the name and format of the author
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
 * the class OPCUAClientWrite (public, protected, and private). Be aware that some 
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
#include "OPCUAClientWrite.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

OPCUAClientWrite::OPCUAClientWrite() :
        OPCUAClientI() {
    monitoredNodes = NULL_PTR(UA_NodeId*);
    writeValues = NULL_PTR(UA_WriteValue*);
    tempVariant = NULL_PTR(UA_Variant*);
    eos = NULL_PTR(UA_ExtensionObject*);
    valuePtr = NULL_PTR(UA_ExtensionObject*);
    nOfEos = 0u;
}

OPCUAClientWrite::~OPCUAClientWrite() {
    bool ok = UnregisterNodes(monitoredNodes);
    if (ok) {
        UA_Array_delete(tempVariant, static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_VARIANT]);
        UA_Array_delete(monitoredNodes, static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_NODEID]);
        UA_Array_delete(writeValues, static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_WRITEVALUE]);
    }
    if (nOfEos > 1u) {
        UA_Array_delete(eos, static_cast<osulong>(nOfEos), &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
    }
    else {
        UA_ExtensionObject_clear(eos);
    }
}

bool OPCUAClientWrite::SetServiceRequest(const uint16 *const namespaceIndexes,
                                         StreamString *const nodePaths,
                                         const uint32 numberOfNodes) {
    bool ok = false;
    nOfNodes = numberOfNodes;

    monitoredNodes = reinterpret_cast<UA_NodeId*>(UA_Array_new(static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_NODEID]));
    writeValues = reinterpret_cast<UA_WriteValue*>(UA_Array_new(static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_WRITEVALUE]));
    tempVariant = reinterpret_cast<UA_Variant*>(UA_Array_new(static_cast<osulong>(nOfNodes), &UA_TYPES[UA_TYPES_VARIANT]));
    /* Setting up write request */
    UA_WriteRequest_init(&writeRequest);

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
            UA_BrowseDescription_init(bReq.nodesToBrowse);
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
                    UA_RelativePathElement *elem = &browsePath.relativePath.elements[j];
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
                UA_BrowsePathTarget *ref = &(tbpResp.results[0].targets[0]);
                UA_NodeId_copy(&(ref->targetId.nodeId), &monitoredNodes[i]);
                if (writeValues != NULL_PTR(UA_WriteValue*)) {
                    UA_WriteValue_init(&writeValues[i]);
                    writeValues[i].attributeId = 13u; /* UA_ATTRIBUTEID_VALUE */
                    UA_NodeId_copy(&monitoredNodes[i], &(writeValues[i].nodeId));
                }
            }
            UA_Array_delete(browsePath.relativePath.elements, static_cast<osulong>(pathSize), &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]);
        }
        delete[] ids;
        delete[] path;
    }
    if (ok) {
        ok = RegisterNodes(monitoredNodes);
    }
    return ok;
}

bool OPCUAClientWrite::GetExtensionObjectByteString(const TypeDescriptor *const&entryTypes,
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
            //tempDataPtr = tempDataPtr + nOfBytes;
            tempDataPtr = &(tempDataPtr[nOfBytes]);
        }
        else {
            ok = MemoryOperationsHelper::Copy(tempDataPtr, &entryArrayElements[index], sizeof(int32));
            if (ok) {
                tempDataPtr = tempDataPtr + 4; /* Skip 4 bytes */
                uint32 nOfBytes = entryTypes[index].numberOfBits;
                nOfBytes /= 8u;
                nOfBytes *= entryArrayElements[index];
                valueMemories[nodeCounter] = reinterpret_cast<void*>(tempDataPtr);
                //tempDataPtr = tempDataPtr + nOfBytes;
                tempDataPtr = &(tempDataPtr[nOfBytes]);
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
                GetExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, index);
            }
        }
        if (entryArrayElements[index] > 1u) {
            ok = MemoryOperationsHelper::Copy(tempDataPtr, &entryArrayElements[index], sizeof(int32));
            if (ok) {
                //tempDataPtr = tempDataPtr + 4; /* Skip 4 bytes */
                tempDataPtr = &(tempDataPtr[4]);
                uint32 nMembers = entryNumberOfMembers[index];
                for (uint32 i = 0u; i < entryArrayElements[index]; i++) {
                    newIndex = index + 1u;
                    for (uint32 j = 0u; j < entryNumberOfMembers[index]; j++) {
                        GetExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, newIndex);
                    }
                }
                index += (nMembers + 1u);
            }
        }
    }
    return ok;
}

bool OPCUAClientWrite::GetExtensionObject() {
    bool ok;
    /* Reading Extension Object Information */
    UA_ReadValueId *readValues = UA_ReadValueId_new();
    UA_ReadValueId_init(&readValues[0u]);
    readValues[0u].attributeId = 13u; /* UA_ATTRIBUTEID_VALUE */

    UA_NodeId_copy(&monitoredNodes[0u], &(readValues[0u].nodeId));
    UA_ReadRequest readRequest;
    UA_ReadRequest_init(&readRequest);
    readRequest.nodesToRead = readValues;
    readRequest.nodesToReadSize = 1u;
    readResponse = UA_Client_Service_read(opcuaClient, readRequest);
    ok = (readResponse.responseHeader.serviceResult == 0x00);
    if (ok) {
        valuePtr = reinterpret_cast<UA_ExtensionObject*>(readResponse.results[0].value.data);
        /* Setting EO Memory */
        if (readResponse.results[0].value.arrayLength > 1u) {
            nOfEos = static_cast<uint32>(readResponse.results[0].value.arrayLength);
            eos = reinterpret_cast<UA_ExtensionObject*>(UA_Array_new(static_cast<osulong>(nOfEos), &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]));
            UA_Variant_setArray(&tempVariant[0u], eos, static_cast<osulong>(readResponse.results[0].value.arrayLength), &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
        }
        else {
            nOfEos = 1u;
            eos = UA_ExtensionObject_new();
            UA_ExtensionObject_init(eos);
            UA_Variant_setScalar(&tempVariant[0u], eos, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT]);
        }
    }
    UA_ReadValueId_clear(readValues);
    UA_ReadValueId_delete(readValues);
    return ok;
}

void OPCUAClientWrite::SetWriteRequest(const uint32 idx,
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

bool OPCUAClientWrite::Write() {
    bool ok = true;
    if (dataPtr != NULL_PTR(void*)) {
        uint32 actualBodyLength;
        if (nOfEos > 1u) {
            for (uint32 j = 0u; j < nOfEos; j++) {
                if (ok) {
                    UA_ExtensionObject_copy(&valuePtr[j], &eos[j]);
                    ok = MemoryOperationsHelper::Copy(eos[j].content.encoded.body.data, dataPtr, eos[j].content.encoded.body.length);
                    dataPtr = reinterpret_cast<uint8*>(dataPtr) + eos[j].content.encoded.body.length;
                }
            }
            actualBodyLength = (nOfEos * eos[0u].content.encoded.body.length);
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
    return ok;
}

bool OPCUAClientWrite::RegisterNodes(UA_NodeId *const monitoredItems) {
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
    return ok;
}

bool OPCUAClientWrite::UnregisterNodes(UA_NodeId *const monitoredItems) {
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
    return ok;
}

}

