/**
 * @file OPCUAServer.cpp
 * @brief Source file for class OPCUAServer
 * @date 24/01/2019
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
 * the class OPCUAServer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "OPCUAServer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

OPCUAServer::OPCUAServer() :
        Object(),
        EmbeddedServiceMethodBinderI(),
        service(*this) {
    opcuaConfig = NULL_PTR(UA_ServerConfig *);
    opcuaServer = NULL_PTR(UA_Server *);
    opcuaRunning = true;
    port = 4840u;
    cpuMask = 0xffu;
    stackSize = THREADS_DEFAULT_STACKSIZE;
    nodeNumber = 3000u;
}

OPCUAServer::~OPCUAServer() {
    SetRunning(false);
    if (!service.Stop()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "Service Stopped");
    }
    UA_Server_delete(opcuaServer);
    REPORT_ERROR(ErrorManagement::Information, "Server Deleted");
    UA_ServerConfig_delete(opcuaConfig);
    REPORT_ERROR(ErrorManagement::Information, "Server Config Deleted");
}

bool OPCUAServer::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Copy(cdb);
    }
    if (ok) {
        ok = data.Read("Port", port);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "No Port number defined. It will be 4840.");
            ok = true;
        }
    }
    if (ok) {
        ok = data.Read("CPUMask", cpuMask);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "No CPUMask defined. It will be default one.");
            ok = true;
        }
    }
    if (ok) {
        ok = data.Read("StackSize", stackSize);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "No StackSize defined. It will be default.");
            ok = true;
        }
    }
    if (ok) {
        ok = cdb.MoveRelative("AddressSpace");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No Address Space defined!");
        }
    }
    if (ok) {
        opcuaConfig = UA_ServerConfig_new_minimal(port, NULL);
        opcuaConfig->maxSessions = 1000;
        opcuaConfig->maxSecureChannels = 1000;
        opcuaServer = UA_Server_new(opcuaConfig);
        SetRunning(true);
        service.SetCPUMask(cpuMask);
        service.SetStackSize(stackSize);
        ok = (service.Start() == ErrorManagement::NoError);
    }
    return ok;
}

ErrorManagement::ErrorType OPCUAServer::Execute(ExecutionInfo & info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() == ExecutionInfo::StartupStage) {
        REPORT_ERROR(ErrorManagement::Information, "OPCUAServer Startup Stage");
        //Here read the structure and Initialise the Address Space
        bool ok = false;
        uint32 nOfChildren = cdb.GetNumberOfChildren();
        StreamString typeStr;
        uint32 i;
        for (i = 0u; i < nOfChildren; i++) {
            ok = cdb.MoveToChild(i);
            if (ok) {
                ok = cdb.Read("Type", typeStr);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "A Type shall be defined for every node");
                }
            }
            cdb.MoveToAncestor(1u);
            if (ok) {
                const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(typeStr.Buffer());
                if (cri != NULL_PTR(const ClassRegistryItem *)) {
                    ReferenceT<OPCUAObject> mainObject("OPCUAObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
                    mainObject->SetName(cdb.GetChildName(i));
                    mainObject->SetFirst(true);
                    const Introspection *intro = NULL_PTR(const Introspection *);
                    intro = cri->GetIntrospection();
                    ok = (intro != NULL_PTR(const Introspection *));
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Type %s has no introspection", typeStr.Buffer());
                    }
                    if (ok) {
                        ok = GetStructure(mainObject, intro);
                        if (ok) {
                            ok = InitAddressSpace(mainObject);
                        }
                    }
                }
                else {
                    ReferenceT<OPCUANode> mainNode("OPCUANode", GlobalObjectsDatabase::Instance()->GetStandardHeap());
                    mainNode->SetName(cdb.GetChildName(i));
                    ok = InitAddressSpace(mainNode);
                }
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot initialise Address Space");
            }
        }
        //This is a blocking call
        if (ok) {
            UA_Server_run(opcuaServer, &opcuaRunning);
        }
    }
    else if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        SetRunning(false);
    }
    else if (info.GetStage() != ExecutionInfo::AsyncTerminationStage) {
        SetRunning(false);
    }
    return err;
}

bool OPCUAServer::InitAddressSpace(ReferenceT<OPCUAReferenceContainer> ref) {
    bool ok = true;
    UA_StatusCode code = 1u;
    //const char* parentId = NULL_PTR(const char*);
    uint32 parentId = 0u;
    if (ref->IsObject()) {
        OPCUAObjectSettings settings = new ObjectProperties;
        ok = ref->GetOPCObject(settings, nodeNumber);
        do {
            if (!ref->IsFirstObject() && ok) {
                if (ok) {
                    code = UA_Server_addObjectNode(opcuaServer, settings->nodeId, settings->parentNodeId, settings->parentReferenceNodeId, settings->nodeName,
                                                   UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), settings->attr, NULL, NULL);
                }
            }
            else {
                code = UA_Server_addObjectNode(opcuaServer, settings->nodeId, settings->parentNodeId, settings->parentReferenceNodeId, settings->nodeName,
                                               UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), settings->attr, NULL, NULL);
            }
            if (code == UA_STATUSCODE_BADNODEIDEXISTS) {
                /*char8* newNodeId = new char[StringHelper::Length(ref->GetNodeId()) + StringHelper::Length(".") + StringHelper::Length(ref->GetParentNodeId())
                        + 1];
                StringHelper::Concatenate(ref->GetParentNodeId(), ".", newNodeId);
                StringHelper::Concatenate(newNodeId, ref->GetNodeId(), newNodeId);
                ref->SetNodeId(newNodeId);
                settings->nodeId = UA_NODEID_STRING(1, newNodeId);*/
                nodeNumber++;
                ref->SetNodeId(nodeNumber);
                settings->nodeId = UA_NODEID_NUMERIC(1, nodeNumber);
            }
            else {
                parentId = ref->GetNodeId();
            }
        }
        while (code != UA_STATUSCODE_GOOD);
        delete settings;

    }
    else if (ref->IsNode()) {
        TypeDescriptor typeName = ref->GetNodeType();
        OPCUANodeSettings settings = new NodeProperties;
        ok = ref->GetOPCVariable(settings, typeName, nodeNumber);
        do {
            if (ok) {
                code = UA_Server_addVariableNode(opcuaServer, settings->nodeId, settings->parentNodeId, settings->parentReferenceNodeId, settings->nodeName,
                                                 UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), settings->attr, NULL, NULL);
            }
            if (code == UA_STATUSCODE_BADNODEIDEXISTS) {
                /*char8* newNodeId = new char[StringHelper::Length(ref->GetNodeId()) + StringHelper::Length(".") + StringHelper::Length(ref->GetParentNodeId())
                        + 1];
                StringHelper::Concatenate(ref->GetParentNodeId(), ".", newNodeId);
                StringHelper::Concatenate(newNodeId, ref->GetNodeId(), newNodeId);
                ref->SetNodeId(newNodeId);
                settings->nodeId = UA_NODEID_STRING(1, newNodeId);*/
                nodeNumber++;
                ref->SetNodeId(nodeNumber);
                settings->nodeId = UA_NODEID_NUMERIC(1,nodeNumber);
            }
        }
        while (code != UA_STATUSCODE_GOOD);
        delete settings;
    }
    uint32 i = 0u;
    uint32 size = ref->Size();
    nodeNumber++;
    for (i = 0u; (i < size) && (ok); i++) {
        ReferenceT<OPCUAReferenceContainer> rc = ref->Get(i);
        if (rc.IsValid()) {
            rc->SetParent(parentId);
            ok = InitAddressSpace(rc);
        }
    }

    return (code == UA_STATUSCODE_GOOD);
}

bool OPCUAServer::GetStructure(ReferenceT<OPCUAReferenceContainer> refContainer,
                               const Introspection *intro) {
    bool ok = true;
    uint32 numberOfMembers = intro->GetNumberOfMembers();
    uint32 j;
    for (j = 0u; j < numberOfMembers; j++) {
        const IntrospectionEntry entry = intro->operator[](j);
        const char8 * const memberName = entry.GetMemberName();
        const char8 * const memberTypeName = entry.GetMemberTypeName();
        TypeDescriptor td = entry.GetMemberTypeDescriptor();
        bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
        if (isStructured) {
            ReferenceT<OPCUAObject> node("OPCUAObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
            //node->SetNodeType(td);
            node->SetName(memberName);
            refContainer->Insert(node);
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
            ok = (cri != NULL_PTR(const ClassRegistryItem *));
            if (ok) {
                ok = GetStructure(node, cri->GetIntrospection());
            }
        }
        else {
            ReferenceT<OPCUANode> finalNode("OPCUANode", GlobalObjectsDatabase::Instance()->GetStandardHeap());
            uint8 nDimensions = entry.GetNumberOfDimensions();
            if (nDimensions > 1) {
                REPORT_ERROR(ErrorManagement::ParametersError,
                             "the Introspection entry [%s] has Number of Dimensions = %d. Multidimensional array not supported yet. ", memberName, nDimensions);
                return false;
            }
            finalNode->SetNumberOfDimensions(nDimensions);
            for (uint32 i = 0; i < nDimensions; i++) {
                finalNode->SetNumberOfElements(i, entry.GetNumberOfElements(i));
            }
            finalNode->SetNodeType(td);
            finalNode->SetName(memberName);
            refContainer->Insert(finalNode);
        }
    }
    return ok;
}

void OPCUAServer::SetRunning(bool const running) {
    opcuaRunning = running;
}

CLASS_REGISTER(OPCUAServer, "");

}
