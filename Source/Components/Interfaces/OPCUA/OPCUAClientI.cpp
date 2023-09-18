/**
 * @file OPCUAClientI.cpp
 * @brief Source file for class OPCUAClientI
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
 * the class OPCUAClientI (public, protected, and private). Be aware that some 
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
#include "OPCUAClientI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/*-e909 and -e9133 redefines bool.*/
/*lint -save -e909 -e9133 -e578*/
namespace MARTe {

OPCUAClientI::OPCUAClientI() {
    /*lint -e{118} no argument needed*/
    opcuaClient = UA_Client_new();
    UA_ClientConfig *config = UA_Client_getConfig(opcuaClient);
    /*lint -e{526} -e{628} -e{1055} -e{746} function defined in open62541*/
    (void)UA_ClientConfig_setDefault(config);
    valueMemories = NULL_PTR(void**);
    serverAddress = "";
    dataPtr = NULL_PTR(void*);
    tempDataPtr = NULL_PTR(uint8*);
    nOfNodes = 0u;
}

/*lint -e{1740} opcuaClient freed by UA_Client_delete*/
OPCUAClientI::~OPCUAClientI() {
    if (dataPtr == NULL_PTR(void*)) {
        if (valueMemories != NULL_PTR(void**)) {
            for (uint32 i = 0u; i < nOfNodes; i++) {
                if (valueMemories[i] != NULL_PTR(void*)) {
                    /*lint -e{1551} no exception on delete*/
                    (void) HeapManager::Free(valueMemories[i]);
                }
            }
            delete[] valueMemories;
        }
    }
    else {
        if (valueMemories != NULL_PTR(void**)) {
            delete[] valueMemories;
        }
        /*lint -e{1551} no exception on delete*/
        (void) HeapManager::Free(dataPtr);
    }
    tempDataPtr = NULL_PTR(uint8*);
    /*lint -e{1551} no exception on delete*/
    UA_Client_delete(opcuaClient);
}

void OPCUAClientI::SetServerAddress(StreamString address) {
    serverAddress = address;
}

bool OPCUAClientI::Connect() {
    UA_StatusCode retval = UA_Client_connect(opcuaClient, const_cast<char8*>(serverAddress.Buffer()));
    return (retval == 0x00U); /* UA_STATUSCODE_GOOD */
}

void OPCUAClientI::SetValueMemories(const uint32 numberOfNodes) {
    valueMemories = new void*[numberOfNodes];
    for (uint32 i = 0u; i < numberOfNodes; i++) {
        valueMemories[i] = NULL_PTR(void*);
    }
}

void OPCUAClientI::SetDataPtr(const uint32 bodyLength) {
    dataPtr = HeapManager::Malloc(bodyLength);
    tempDataPtr = reinterpret_cast<uint8*>(dataPtr);
}

bool OPCUAClientI::GetSignalMemory(void *&mem,
                                   const uint32 idx,
                                   const TypeDescriptor &valueTd,
                                   const uint32 nElem) {
    bool ok = false;
    if (valueMemories != NULL_PTR(void**)) {
        if (dataPtr == NULL_PTR(void*)) {
            uint32 nOfBytes = valueTd.numberOfBits;
            nOfBytes /= 8u;
            nOfBytes *= nElem;
            valueMemories[idx] = HeapManager::Malloc(nOfBytes);
        }
        ok = true;
        mem = valueMemories[idx];
    }
    return ok;
}

void OPCUAClientI::SeekDataPtr(const uint32 bodyLength) {
    if (dataPtr != NULL_PTR(void*)) {
        dataPtr = reinterpret_cast<uint8*>(dataPtr) - bodyLength;
    }
}

/*lint -e{1746} no need to make bReq const reference.*/
uint32 OPCUAClientI::GetReferences(const UA_BrowseRequest bReq,
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
    UA_BrowseResponse bResp = UA_Client_Service_browse(opcuaClient, bReq);
    bool ok = (bResp.responseHeader.serviceResult == 0x00U); /* UA_STATUSCODE_GOOD */
    if (ok) {
        bool found = false;
        uint64 initCheckTime = HighResolutionTimer::Counter();
        uint64 finalCheckTime = 0u;
        uint32 count = 0u;
        while ((!found) && (bResp.resultsSize)) {
            for (uint32 i = 0u; i < bResp.resultsSize; ++i) {
                for (uint32 j = 0u; j < bResp.results[i].referencesSize; ++j) {
                    UA_ReferenceDescription ref;
                    (void) UA_ReferenceDescription_copy(&(bResp.results[i].references[j]), &ref);
                    if (StringHelper::Compare(reinterpret_cast<char8*>(ref.browseName.name.data), path) == 0) {
                        id = ref.referenceTypeId.identifier.numeric;
                        namespaceIndex = ref.nodeId.nodeId.namespaceIndex;
                        if (ref.nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
                            numericNodeId = ref.nodeId.nodeId.identifier.numeric;
                            ok = true;
                        }
                        else if (ref.nodeId.nodeId.identifierType == UA_NODEIDTYPE_STRING) {
                            if (stringNodeId != NULL_PTR(char8*)) {
                                delete stringNodeId;
                            }
                            stringNodeId = new char8[ref.nodeId.nodeId.identifier.string.length + 1u];
                            ok = StringHelper::Copy(stringNodeId, reinterpret_cast<char8*>(ref.nodeId.nodeId.identifier.string.data));
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
            if ((!found) && bResp.results->continuationPoint.length) {
                REPORT_ERROR_STATIC(ErrorManagement::Information, "BrowseNext");
                UA_BrowseNextRequest nextReq;
                UA_BrowseNextRequest_init(&nextReq);
                nextReq.continuationPoints = UA_ByteString_new();
                UA_StatusCode code = UA_ByteString_copy(&(bResp.results->continuationPoint), nextReq.continuationPoints);
                if (code == 0x00U) { /* UA_STATUSCODE_GOOD */
                    nextReq.continuationPointsSize = 1u;
                    /*lint -e{740} open62541 needs the recast of bResp .*/
                    *reinterpret_cast<UA_BrowseNextResponse*>(&bResp) = UA_Client_Service_browseNext(opcuaClient, nextReq);
                }
                /*lint -e{526} -e{628} -e{1551} -e{1055} -e{746} no exception thrown, function defined in open62541*/
                (void) UA_ByteString_clear(nextReq.continuationPoints);
                UA_ByteString_delete(nextReq.continuationPoints);
                //UA_BrowseNextRequest_clear(&nextReq);
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
    return id;
}

StreamString OPCUAClientI::GetServerAddress() const {
    return serverAddress;
}

void** OPCUAClientI::GetValueMemories() {
    return valueMemories;
}

uint32 OPCUAClientI::GetNumberOfNodes() const {
    return nOfNodes;
}

void* OPCUAClientI::GetDataPtr() {
    return dataPtr;
}

}
/*lint -restore*/

