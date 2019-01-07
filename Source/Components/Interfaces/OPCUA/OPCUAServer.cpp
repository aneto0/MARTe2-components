/**
 * @file OPCUAServer.cpp
 * @brief Source file for class OPCUAServer
 * @date Nov 8, 2018 TODO Verify the value and format of the date
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

namespace MARTe {

OPCUAServer::OPCUAServer() :
        Object(),
        EmbeddedServiceMethodBinderI(),
        service(*this) {
    /* Insert the port number and the certificate if needed */
    opcuaConfig = UA_ServerConfig_new_minimal(4841, NULL);
    opcuaServer = UA_Server_new(opcuaConfig);
    opcuaRunning = true;
}

OPCUAServer::~OPCUAServer() {
    SetRunning(false);
    UA_Server_delete(opcuaServer);
    UA_ServerConfig_delete(opcuaConfig);
    if (!service.Stop()) {
        if (!service.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
}

bool OPCUAServer::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Copy(cdb);
    }
    if (ok) {
        ok = cdb.MoveRelative("AddressSpace");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No Address Space defined!");
        }
    }
    if (ok) {
        SetRunning(true);
        ok = (service.Start() == ErrorManagement::NoError);
    }
    return ok;
}

ErrorManagement::ErrorType OPCUAServer::Execute(ExecutionInfo & info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() == ExecutionInfo::StartupStage) {
        REPORT_ERROR(ErrorManagement::Information, "OPCUAServer Startup Stage");
        //Here read the structure and Initialise the Address Space
        ReferenceT<OPCUAObject> mainObject("OPCUAObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        mainObject->SetName("MARTe2_Variables");
        bool ok = false;
        uint32 nOfChildren = cdb.GetNumberOfChildren();
        StreamString typeStr;
        REPORT_ERROR(ErrorManagement::Information, "Number of children = %u", nOfChildren);
        uint32 i;
        for (i = 0u; i < nOfChildren; i++) {
            ok = cdb.MoveToChild(i);
            if (ok) {
                ok = cdb.Read("Type", typeStr);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "A Type shall be defined for every node");
                }
            }
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(typeStr.Buffer());
            const Introspection *intro = NULL_PTR(const Introspection *);
            ok = (cri != NULL_PTR(const ClassRegistryItem *));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Type %s is not registered", typeStr.Buffer());
            }
            if (ok) {
                intro = cri->GetIntrospection();
                ok = (intro != NULL_PTR(const Introspection *));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Type %s has no introspection", typeStr.Buffer());
                }
                /* The parameter is 0 because the first structure is always an object */
                ok = GetStructure(intro, mainObject);
            }
            if (ok) {
                ok = InitAddressSpace(mainObject);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Cannot initialise Address Space");
                }
            }
        }
        //This is a blocking call
        UA_Server_run(opcuaServer, &opcuaRunning);
    }
    else if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        Sleep::Sec(1.0);
    }
    return err;
}

void OPCUAServer::SetRunning(bool const running) {
    opcuaRunning = running;
}

bool OPCUAServer::InitAddressSpace(ReferenceT<OPCUAReferenceContainer> ref) {
    bool ok = true;
    UA_StatusCode code = 1u;
    const char* parentId = NULL_PTR(const char*);
    if (ref->IsObject()) {
        OPCUAObjectSettings settings = new ObjectProperties;
        ok = ref->GetOPCObject(settings);
        if (ok) {
            code = UA_Server_addObjectNode(opcuaServer, settings->nodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                           UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), settings->nodeName, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                                           settings->attr, NULL, NULL);
        }
        parentId = ref->GetNodeId();
    }
    else if (ref->IsNode()) {
        OPCUANodeSettings settings = new NodeProperties;
        ok = ref->GetOPCVariable(settings);
        if (ok) {
            code = UA_Server_addVariableNode(opcuaServer, settings->nodeId, settings->parentNodeId, settings->parentReferenceNodeId, settings->nodeName,
                                             UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), settings->attr, NULL, NULL);
        }
        parentId = ref->GetNodeId();
    }
    uint32 i = 0u;
    uint32 size = ref->Size();
    for (i = 0u; (i < size) && (ok); i++) {
        ReferenceT<OPCUAReferenceContainer> rc = ref->Get(i);
        if (rc.IsValid()) {
            REPORT_ERROR(ErrorManagement::Information, "ParentId = %s", parentId);
            rc->SetParent(parentId);
            ok = InitAddressSpace(rc);
        }

    }

    return (code == UA_STATUSCODE_GOOD);
}

bool OPCUAServer::GetStructure(const Introspection *intro,
                               ReferenceT<OPCUAReferenceContainer> refContainer) {
    bool ok = true;
    uint32 numberOfMembers = intro->GetNumberOfMembers();
    uint32 j;
    for (j = 0u; j < numberOfMembers; j++) {
        const IntrospectionEntry entry = intro->operator[](j);
        const char8 * const memberName = entry.GetMemberName();
        const char8 * const memberTypeName = entry.GetMemberTypeName();
        bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
        ReferenceT<OPCUANode> node("OPCUANode", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        node->SetName(memberName);
        refContainer->Insert(node);
        if (isStructured) {
            REPORT_ERROR(ErrorManagement::Information, "Member = %s", memberName);
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
            ok = (cri != NULL_PTR(const ClassRegistryItem *));
            if (ok) {
                ok = GetStructure(cri->GetIntrospection(), node);
            }
        }
    }
    return ok;
}

CLASS_REGISTER(OPCUAServer, "");

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

