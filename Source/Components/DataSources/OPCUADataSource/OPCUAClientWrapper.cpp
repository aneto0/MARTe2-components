/**
 * @file OPCUAClientWrapper.cpp
 * @brief Source file for class OPCUAClientWrapper
 * @date 21 Nov 2018 TODO Verify the value and format of the date
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

//TEST
UA_Client* clients[10];
//TEST
OPCUAClientWrapper * wrappers[10];

/**
 * Top level callback functions
 */

void OPCUAClientWrapperUpdateMemory(UA_Client *client,
                                    UA_UInt32 subId,
                                    void *subContext,
                                    UA_UInt32 monId,
                                    void *monContext,
                                    UA_DataValue *value) {
    //REPORT_ERROR_STATIC(ErrorManagement::Information, "CLIENT ---------------------------> SensorGeometry: New Notification!");
    //loop in clients
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

/**
 * Class members implementation
 */

OPCUAClientWrapper::OPCUAClientWrapper() {
    config = UA_ClientConfig_default;
    config.stateCallback = OPCUAClientWrapperHandlerState;
    config.subscriptionInactivityCallback = OPCUAClientWrapperHandlerInactivity;
    opcuaClient = UA_Client_new(config);
    monitoredNode.identifier.numeric = 0;
    outValueMem = new UA_Variant;
    request = UA_CreateSubscriptionRequest_default();
    response.subscriptionId = 0;
    monitorRequest.itemToMonitor.nodeId.identifier.numeric = 0;
    monitorResponse.monitoredItemId = 0;
    nOfClients++;
    valueMemory = NULL_PTR(void *);
    clients[nOfClients - 1] = opcuaClient;
    wrappers[nOfClients - 1] = this;
    serverAddress = NULL_PTR(char *);
}

OPCUAClientWrapper::~OPCUAClientWrapper() {
    UA_Client_delete(opcuaClient);
    delete outValueMem;
    if (valueMemory != NULL_PTR(void *)) {
        free(valueMemory);
    }
}

void OPCUAClientWrapper::SetServerAddress(char* address) {
    serverAddress = new char[strlen(address)];
    strcpy(serverAddress, address);
}

bool OPCUAClientWrapper::Connect() {
    UA_StatusCode retval = UA_Client_connect(opcuaClient, serverAddress);
    return (retval == UA_STATUSCODE_GOOD);
}

bool OPCUAClientWrapper::GetSignalMemory(void *&mem) {
    bool ok = true;
    if (monitoredNode.identifier.numeric == 0) {
        REPORT_ERROR_STATIC(ErrorManagement::IllegalOperation, "Cannot get the signal memory.");
        ok = false;
    }
    else {
        valueMemory = malloc(sizeof(uint32));
        mem = valueMemory;
    }
    return ok;

}

/**
 * @brief Recursive search in the Server AddressSpace
 * @details It calls the private function SearchNode starting from the Object folder parameters.
 */
bool OPCUAClientWrapper::BrowseAddressSpace(uint32 namespaceIndex,
                                            StreamString nodePath) {
    bool ok = false;

    ok = nodePath.Seek(0LLU);
    StreamString pathTokenized;
    uint32 pathSize = 0u;
    char8 ignore;
    if (ok) {
        /* This cycle is only for getting the path size */
        do {
            ok = nodePath.GetToken(pathTokenized, ".", ignore);
            if (ok) {
                pathSize++;
            }
            pathTokenized = "";
        }
        while (ok);
    }
    char** path = new char*[pathSize];
    uint32* ids = new uint32[pathSize];
    ok = nodePath.Seek(0LLU);
    if (ok) {
        for (uint32 i = 0u; i < pathSize; i++) {
            ok = nodePath.GetToken(pathTokenized, ".", ignore);
            if (ok) {
                char * s = new char[strlen(pathTokenized.Buffer())];
                strcpy(s, pathTokenized.Buffer());
                path[i] = s;
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

    for (uint32 i = 0u; i < pathSize; i++) {
        ids[i] = GetReferenceType(bReq, path[i], tempNamespaceIndex, tempNumericNodeId, tempStringNodeId);
        UA_RelativePathElement *elem = &browsePath.relativePath.elements[i];
        elem->referenceTypeId = UA_NODEID_NUMERIC(0, ids[i]);
        elem->targetName = UA_QUALIFIEDNAME_ALLOC(namespaceIndex, path[i]);
    }
    UA_TranslateBrowsePathsToNodeIdsRequest tbpReq;
    UA_TranslateBrowsePathsToNodeIdsRequest_init(&tbpReq);
    tbpReq.browsePaths = &browsePath;
    tbpReq.browsePathsSize = 1;
    UA_TranslateBrowsePathsToNodeIdsResponse tbpResp = UA_Client_Service_translateBrowsePathsToNodeIds(opcuaClient, tbpReq);
    ok = (tbpResp.results[0].statusCode == UA_STATUSCODE_GOOD);
    if (ok) {
        UA_BrowsePathTarget *ref = &(tbpResp.results[0].targets[0]);
        monitoredNode = ref->targetId.nodeId;
    }
    return ok;
}

bool OPCUAClientWrapper::Monitor() {
    UA_StatusCode code = 1u;
    bool ok;
    if (monitoredNode.identifier.numeric != 0 && valueMemory != NULL_PTR(void *)) {
        /* Create a subscription */
        if (response.subscriptionId == 0) {
            response = UA_Client_Subscriptions_create(opcuaClient, request, NULL, NULL, NULL);
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
            //monitorRequest = UA_MonitoredItemCreateRequest_default(UA_NODEID_STRING(monitoredNode.namespaceIndex, nodeName));
        }
        if (monitorResponse.monitoredItemId == 0) {
            monitorResponse = UA_Client_MonitoredItems_createDataChange(opcuaClient, response.subscriptionId, UA_TIMESTAMPSTORETURN_BOTH, monitorRequest, NULL,
                                                                        OPCUAClientWrapperUpdateMemory, NULL);
            /* Debug only */
            ok = (monitorResponse.statusCode == UA_STATUSCODE_GOOD);
        }
        /* Asynchronous call */
        code = UA_Client_run_iterate(opcuaClient, 1000);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "CLIENT ---------------------------> Monitor: Waiting for initialization...");
        code = UA_STATUSCODE_GOOD;
    }
    return (code == UA_STATUSCODE_GOOD);
}

/**
 * NOT USED - This function is not yet used by the BrowseAddressSpace function
 *
 * @brief Search the requested node in the Server AddressSpace
 * @details Recursively calls itself to explore deeper nodes in the tree.
 * It uses the OPCUA Browse Service.
 */
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
    bool found = false;
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
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Browse Next Service");

            UA_BrowseNextRequest nextReq;
            UA_BrowseNextRequest_init(&nextReq);
            nextReq.continuationPoints = UA_ByteString_new();
            UA_ByteString_copy(&(bResp->results->continuationPoint), nextReq.continuationPoints);
            nextReq.continuationPointsSize = 1;
            *reinterpret_cast<UA_BrowseNextResponse *>(bResp) = UA_Client_Service_browseNext(opcuaClient, nextReq);
            UA_BrowseNextRequest_deleteMembers(&nextReq);
        }
    }
    return id;
}

void OPCUAClientWrapper::WriteValueAttribute() {
    UA_Variant *tempVariant = UA_Variant_new();
    uint32 value = *reinterpret_cast<uint32*>(valueMemory);
    UA_Variant_setScalarCopy(tempVariant, &value, &UA_TYPES[UA_TYPES_UINT32]);
    if (monitoredNode.identifierType == UA_NODEIDTYPE_NUMERIC) {
        UA_Client_writeValueAttribute(opcuaClient, UA_NODEID_NUMERIC(monitoredNode.namespaceIndex, monitoredNode.identifier.numeric), tempVariant);
    }
    else if (monitoredNode.identifierType == UA_NODEIDTYPE_STRING) {
        UA_Client_writeValueAttribute(opcuaClient,
                                      UA_NODEID_STRING(monitoredNode.namespaceIndex, reinterpret_cast<char*>(monitoredNode.identifier.string.data)),
                                      tempVariant);
    }
    UA_Variant_delete(tempVariant);
}

void OPCUAClientWrapper::UpdateMemory(UA_DataValue *value) {
    outValueMem->data = value->value.data;
    if (valueMemory != NULL_PTR(void *)) {
        memcpy(valueMemory, outValueMem->data, sizeof(uint32));
        //REPORT_ERROR_STATIC(ErrorManagement::Information, "CLIENT ---------------------------> UPDATE CALLBACK - Value %u", *static_cast<uint32*>(valueMemory));
    }
}

void OPCUAClientWrapper::StateCallback(UA_ClientState clientState) {
    if (clientState == UA_CLIENTSTATE_DISCONNECTED) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "CLIENT ---------------------------> DISCONNECTED");
    }
    else if (clientState == UA_CLIENTSTATE_CONNECTED) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "CLIENT ---------------------------> TCP CONNECTION IS OPEN");
    }
    else if (clientState == UA_CLIENTSTATE_SECURECHANNEL) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "CLIENT ---------------------------> SECURE CHANNEL IS OPEN");
    }
    else if (clientState == UA_CLIENTSTATE_SESSION) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "CLIENT ---------------------------> SESSION IS OPEN");
    }
    else if (clientState == UA_CLIENTSTATE_SESSION_DISCONNECTED) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "CLIENT ---------------------------> SESSION IS CLOSED");
    }
}

void OPCUAClientWrapper::SubscriptionInactivity() {
    REPORT_ERROR_STATIC(ErrorManagement::Information, "CLIENT ---------------------------> SUBSCRIPTION INACTIVITY");
}

}
