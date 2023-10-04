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
#include "File.h"
#include "StandardParser.h"

/*-e909 and -e9133 redefines bool. -e578 symbol ovveride in CLASS_REGISTER*/
/*lint -save -e652 -e909 -e9133 -e578 -e9141 -e830 -e952 -e9147 -e1013 these callback functions need to have global scope and the members cannot be const*/
#ifdef LINT
#define UA_Boolean bool
#endif

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static UA_Boolean allowAddNode(UA_Server *server, UA_AccessControl *ac,
                               const UA_NodeId *sessionId, 
                               void *sessionContext,
                               const UA_AddNodesItem *item) {
    (void) server;
    (void) ac;
    (void) sessionId;
    (void) sessionContext;
    (void) item;
    return UA_TRUE;
}

static UA_Boolean allowAddReference(UA_Server *server, UA_AccessControl *ac,
                                    const UA_NodeId *sessionId,
                                    void *sessionContext,
                                    const UA_AddReferencesItem *item) {
    (void) server;
    (void) ac;
    (void) sessionId;
    (void) sessionContext;
    (void) item;
    return UA_TRUE;
}

static UA_Boolean allowDeleteNode(UA_Server *server, UA_AccessControl *ac,
                                  const UA_NodeId *sessionId,
                                  void *sessionContext,
                                  const UA_DeleteNodesItem *item) {
    (void) server;
    (void) ac;
    (void) sessionId;
    (void) sessionContext;
    (void) item;
    return UA_FALSE; // Do not allow deletion from client
}

static UA_Boolean allowDeleteReference(UA_Server *server, UA_AccessControl *ac,
                                       const UA_NodeId *sessionId,
                                       void *sessionContext,
                                       const UA_DeleteReferencesItem *item) {
    (void) server;
    (void) ac;
    (void) sessionId;
    (void) sessionContext;
    (void) item;
    return UA_TRUE;
}

static bool ReadAuthenticationKeys(MARTe::StructuredDataI &data, UA_UsernamePasswordLogin *&authKeys, MARTe::uint32 &nOfAuthKeys) {
    /*lint -e9144 allow using directive*/
    using namespace MARTe;
    StreamString userPasswordFile;
    bool ok = data.Read("UserPasswordFile", userPasswordFile);
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "'UserPasswordFile' not defined!");
    }
    File f;
    if (ok) {
        (void) userPasswordFile.Seek(0LLU);
        ok = f.Open(userPasswordFile.Buffer(), BasicFile::ACCESS_MODE_R);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to open the file at path '%s'!", userPasswordFile.Buffer());
        }
    }
    ConfigurationDatabase userPasswordDatabase;
    if (ok) {
        StandardParser parser(f, userPasswordDatabase);
        ok = parser.Parse();
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to parse the UserPasswordFile!");
        }
    }
    if (f.IsOpen()) {
        ok = f.Close();
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to close the 'UserPasswordFile'!");
        }
    }
    if (ok) {
        ok = userPasswordDatabase.MoveRelative("AuthenticationKeys");
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "'AuthenticationKeys' not defined in the UserPasswordFile!");
        }
    }
    if (ok) {
        nOfAuthKeys = userPasswordDatabase.GetNumberOfChildren();
        ok = nOfAuthKeys > 0u;
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "At least one authentication key must be defined in 'AuthenticationKeys'!");
        }
    }
    if (ok) {
        authKeys = new UA_UsernamePasswordLogin[nOfAuthKeys];
        ok = (authKeys != NULL_PTR(UA_UsernamePasswordLogin *));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to allocate memory for authentication keys!");
        }
        for (uint32 i = 0u; (i < nOfAuthKeys) && ok; i++) {
            (void) userPasswordDatabase.MoveToChild(i);
            StreamString temp;
            ok = userPasswordDatabase.Read("Username", temp);
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to read 'Username' for %s", userPasswordDatabase.GetName());
            }
            if (ok) {
                authKeys[i].username = UA_String_fromChars(temp.Buffer());
                ok = (authKeys[i].username.data != NULL_PTR(UA_Byte *));
                if (!ok) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to copy UA username string!");
                }
            }
            if (ok) {
                temp = "";
                ok = userPasswordDatabase.Read("Password", temp);
                if (!ok) {
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to read 'Password' for %s", userPasswordDatabase.GetName());
                }
            }
            if (ok) {
                authKeys[i].password = UA_String_fromChars(temp.Buffer());
                ok = (authKeys[i].password.data != NULL_PTR(UA_Byte *));
                if (!ok) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to copy UA password string!");
                }
            }
            (void) userPasswordDatabase.MoveToAncestor(1u);
        }
    }
    return ok;
}
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
    opcuaRunning = false;
    port = 4840u;
    cpuMask = 0xffu;
    stackSize = THREADS_DEFAULT_STACKSIZE;
    nodeNumber = 3000u;
}
/*lint -e{1551} No exception thrown.*/
/*lint -e{1579} opcuaConfig and opcuaServer haven't been freed by any function before.*/
OPCUAServer::~OPCUAServer() {
    SetRunning(false);
    (void) service.Stop();
    if (opcuaConfig != NULL_PTR(UA_ServerConfig *)) {
        UA_Server_delete(opcuaServer);
    }
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
    StreamString authentication;
    if (ok) {
        if (!data.Read("Authentication", authentication)) {
            authentication = "None";
            REPORT_ERROR(ErrorManagement::Warning, "'Authentication' not defined.");
        }
    }
    bool authenticate = false;
    if (ok) {
        if (authentication == "None") {
            authenticate = false;
            REPORT_ERROR(ErrorManagement::Information, "Using 'None' authentication.");
        }
        else if (authentication == "UserPassword") {
            authenticate = true;
            REPORT_ERROR(ErrorManagement::Information, "Using 'UserPassword' authentication.");
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "'Authentication' parameter invalid (expected 'None' or 'UserPassword')!");
        }
    }
    /*lint -e438 authKeys is not ignored.*/
    UA_UsernamePasswordLogin * authKeys = NULL_PTR(UA_UsernamePasswordLogin *);
    uint32 nOfAuthKeys = 0u;
    if (ok && authenticate) {
        ok = ReadAuthenticationKeys(data, authKeys, nOfAuthKeys);
    }
    if (ok) {
        /*lint -e{118} no argument needed*/
        opcuaServer = UA_Server_new();
        
        UA_ServerConfig *config = UA_Server_getConfig(opcuaServer);
        /*lint -e{526} -e{628} -e{1055} -e{746} function defined in open62541*/
        (void)UA_ServerConfig_setDefault(config);

        /*lint -e40 -e64 -e9117 -e732 the callback functions are defined. Loss of sign is not an issue here.*/
        if (authenticate) {
            /* Disable anonymous logins, enable user/password logins */
            config->accessControl.deleteMembers(&config->accessControl);
            UA_StatusCode retval = UA_AccessControl_default(config, false,
                    &config->securityPolicies[config->securityPoliciesSize-1].policyUri, nOfAuthKeys, authKeys);
            if (retval != UA_STATUSCODE_GOOD) {
                ok = false;
                REPORT_ERROR(ErrorManagement::FatalError, "Setting access control configuration returned %d", retval);
            }
            if (ok) {
                /* Set accessControl functions for nodeManagement */
                config->accessControl.allowAddNode = allowAddNode;
                config->accessControl.allowAddReference = allowAddReference;
                config->accessControl.allowDeleteNode = allowDeleteNode;
                config->accessControl.allowDeleteReference = allowDeleteReference;
            }
        }
    }
    if (authKeys != NULL_PTR(UA_UsernamePasswordLogin *)) {
        for (uint32 i = 0u; i < nOfAuthKeys; i++) {
            if (authKeys[i].username.data != NULL_PTR(UA_Byte *)) {
                delete authKeys[i].username.data;
            }
            if (authKeys[i].password.data != NULL_PTR(UA_Byte *)) {
                delete authKeys[i].password.data;
            }
        }
        delete[] authKeys;
        authKeys = NULL_PTR(UA_UsernamePasswordLogin *);
    }
    if (ok) {
        ok = cdb.MoveRelative("AddressSpace");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No Address Space defined!");
        }
    }
    if (ok) {
        service.SetCPUMask(cpuMask);
        service.SetStackSize(stackSize);
        service.SetName(GetName());
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
        (void) typeStr.SetSize(0LLU);
        uint32 i;
        for (i = 0u; i < nOfChildren; i++) {
            ok = cdb.MoveToChild(i);
            if (ok) {
                ok = cdb.Read("Type", typeStr);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "A Type shall be defined for every node");
                }
            }
            if (ok) {
                ok = cdb.MoveToAncestor(1u);
            }
            if (ok) {
                const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(typeStr.Buffer());
                if (cri != NULL_PTR(const ClassRegistryItem *)) {
                    ReferenceT<OPCUAObject> mainObject("OPCUAObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
                    mainObject->SetName(cdb.GetChildName(i));
                    mainObject->SetFirst(true);
                    const Introspection *intro = cri->GetIntrospection();
                    ok = (intro != NULL_PTR(const Introspection *));
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
                    TypeDescriptor td = TypeDescriptor::GetTypeDescriptorFromTypeName(typeStr.Buffer());
                    mainNode->SetNodeType(td);
                    if (cdb.MoveToChild(i)) {
                        uint32 nElem = 1u;
                        ok = cdb.Read("NumberOfElements", nElem);
                        if ((nElem > 1u) && ok) {
                            mainNode->SetNumberOfDimensions(1u);
                            mainNode->SetNumberOfElements(0u, nElem);
                        }
                        REPORT_ERROR(ErrorManagement::Information, "Number Of Elements = %d", nElem);
                    }
                    ok = InitAddressSpace(mainNode);
                    if (ok) {
                        ok = cdb.MoveToAncestor(1u);
                    }

                }
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot initialise Address Space");
            }
            typeStr = "";
        }
        if (ok) {
            SetRunning(true);
        }
        //This is a blocking call
        /*lint -e{64} No type mismatch because open62541 redefines boolean.*/
        if (ok) {
            UA_StatusCode code = UA_Server_run(opcuaServer, &opcuaRunning);
            if (code != 0x00U) { /* UA_STATUSCODE_GOOD */
                err = ErrorManagement::CommunicationError;
            }
        }
    }
    else if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        SetRunning(false);
    }
    else if (info.GetStage() != ExecutionInfo::AsyncTerminationStage) {
        SetRunning(false);
    }
    else {
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
        OPCUA::OPCUAObjectSettings settings = new OPCUA::ObjectProperties;
        ok = ref->GetOPCObject(settings, nodeNumber);
        do {
            if (ok) {
                if (!ref->IsFirstObject()) {
                    code = UA_Server_addObjectNode(opcuaServer, settings->nodeId, settings->parentNodeId, settings->parentReferenceNodeId, settings->nodeName,
                                                   UA_NODEID_NUMERIC(0u, 61u), settings->attr, NULL_PTR(void *), NULL_PTR(UA_NodeId *)); /* UA_NS0ID_FOLDERTYPE = 61 */
                }
                else {
                    code = UA_Server_addObjectNode(opcuaServer, settings->nodeId, settings->parentNodeId, settings->parentReferenceNodeId, settings->nodeName,
                                                   UA_NODEID_NUMERIC(0u, 58u), settings->attr, NULL_PTR(void *), NULL_PTR(UA_NodeId *)); /* UA_NS0ID_BASEOBJECTTYPE = 58 */
                }
            }
            if (code == 0x805E0000U) { /* UA_STATUSCODE_BADNODEIDEXISTS */
                nodeNumber++;
                ref->SetNodeId(nodeNumber);
                settings->nodeId = UA_NODEID_NUMERIC(1u, nodeNumber);
            }
            else {
                parentId = ref->GetNodeId();
            }
        }
        while (code != 0x00U); /* UA_STATUSCODE_GOOD */
        delete settings;
    }
    else if (ref->IsNode()) {
        TypeDescriptor typeName = ref->GetNodeType();
        OPCUA::OPCUANodeSettings settings = new OPCUA::NodeProperties;
        ok = ref->GetOPCVariable(settings, typeName, nodeNumber);
        do {
            if (ok) {
                code = UA_Server_addVariableNode(opcuaServer, settings->nodeId, settings->parentNodeId, settings->parentReferenceNodeId, settings->nodeName,
                                                 UA_NODEID_NUMERIC(0u, 63u), settings->attr, NULL_PTR(void *), NULL_PTR(UA_NodeId *)); /* UA_NS0ID_BASEDATAVARIABLETYPE = 63 */
            }
            if (code == 0x805E0000U) { /* UA_STATUSCODE_BADNODEIDEXISTS */
                nodeNumber++;
                ref->SetNodeId(nodeNumber);
                settings->nodeId = UA_NODEID_NUMERIC(1u, nodeNumber);
            }
        }
        while (code != 0x00U); /* UA_STATUSCODE_GOOD */
        delete settings;
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot initialise Address Space");
    }
    uint32 i;
    uint32 size = ref->Size();
    nodeNumber++;
    for (i = 0u; (i < size) && (ok); i++) {
        ReferenceT<OPCUAReferenceContainer> rc = ref->Get(i);
        if (rc.IsValid()) {
            rc->SetParent(parentId);
            ok = InitAddressSpace(rc);
        }
    }

    return (code == 0x00U); /* UA_STATUSCODE_GOOD */
}

bool OPCUAServer::GetStructure(ReferenceT<OPCUAReferenceContainer> refContainer,
                               const Introspection * const intro) {
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
            ok = refContainer->Insert(node);
            if (ok) {
                const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
                ok = (cri != NULL_PTR(const ClassRegistryItem *));
                if (ok) {
                    ok = GetStructure(node, cri->GetIntrospection());
                }
            }
        }
        else {
            ReferenceT<OPCUANode> finalNode("OPCUANode", GlobalObjectsDatabase::Instance()->GetStandardHeap());
            uint8 nDimensions = entry.GetNumberOfDimensions();
            bool ret = true;
            if (nDimensions > 1u) {
                REPORT_ERROR(ErrorManagement::ParametersError,
                             "the Introspection entry [%s] has Number of Dimensions = %d. Multidimensional array not supported yet. ", memberName, nDimensions);
                ret = false;
                ok = false;
            }
            if (ret) {
                finalNode->SetNumberOfDimensions(nDimensions);
                for (uint32 i = 0u; i < nDimensions; i++) {
                    finalNode->SetNumberOfElements(i, entry.GetNumberOfElements(i));
                }
                finalNode->SetNodeType(td);
                finalNode->SetName(memberName);
                ok = refContainer->Insert(finalNode);
            }
        }
    }
    return ok;
}

void OPCUAServer::SetRunning(bool const running) {
    opcuaRunning = running;
}

const bool OPCUAServer::GetRunning() const {
    return opcuaRunning;
}

const uint32 OPCUAServer::GetCPUMask() const {
    return cpuMask;
}

const uint32 OPCUAServer::GetStackSize() const {
    return stackSize;
}

const uint16 OPCUAServer::GetPort() const {
    return port;
}


CLASS_REGISTER(OPCUAServer, "");

}
/*lint -restore*/
