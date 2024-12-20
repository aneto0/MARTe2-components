/**
 * @file OPCUAServerTest.cpp
 * @brief Source file for class OPCUAServerTest
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
 * the class OPCUAServerTest (public, protected, and private). Be aware that some 
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
#include "OPCUAServerTest.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

bool OPCUAServerTest::TestInitialise_Default() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint8"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<OPCUAServer> server;
    ;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        server = ord->Find("ServerTest");
        ok = server.IsValid();
    }
    Sleep::MSec(100);
    if (ok) {
        ok = (server->GetCPUMask() == 0xffu);
    }
    if (ok) {
        ok = (server->GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    }
    if (ok) {
        ok = (server->GetPort() == 4840u);
    }
    ord->Purge();
    return ok;
}

bool OPCUAServerTest::TestInitialise_NoDefault() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     CPUMask = 0x4"
            "     StackSize = 1000000"
            "     Port = 4841"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint8"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<OPCUAServer> server;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        server = ord->Find("ServerTest");
        ok = server.IsValid();
    }
    Sleep::MSec(100);
    if (ok) {
        ok = (server->GetCPUMask() == 0x4);
    }
    if (ok) {
        ok = (server->GetStackSize() == 1000000);
    }
    if (ok) {
        ok = (server->GetPort() == 4841);
    }
    ord->Purge();
    return ok;
}

bool OPCUAServerTest::TestInitialise_NoAddressSpace() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     CPUMask = 0x4"
            "     StackSize = 1000000"
            "     Port = 4841"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return !ok;
}

bool OPCUAServerTest::TestServerStart() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Run = 0"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint8"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    ReferenceT<OPCUAServer> opcuaServer = ord->Find("ServerTest");
    ok = opcuaServer.IsValid();
    if (ok) {
        Sleep::MSec(100);
        UA_Client *client = UA_Client_new();
        UA_ClientConfig_setDefault(UA_Client_getConfig(client));
        UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
        ok = (retval != UA_STATUSCODE_GOOD);
        if (ok) {
            opcuaServer->ServerStart();
            Sleep::MSec(100);

            retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
            ok = (retval == UA_STATUSCODE_GOOD);

            if (ok) {
                UA_BrowseRequest bReq;
                UA_BrowseRequest_init(&bReq);
                bReq.requestedMaxReferencesPerNode = 0;
                bReq.nodesToBrowse = UA_BrowseDescription_new();
                bReq.nodesToBrowseSize = 1;
                bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(1u, 3000u);
                bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; /* return everything */
                UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
                ok = (bResp.responseHeader.serviceResult == 0x00U);
            }
        }
    }
    ord->Purge();

    return ok;
}

bool OPCUAServerTest::TestServerStop() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Run = 0"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint8"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    ReferenceT<OPCUAServer> opcuaServer = ord->Find("ServerTest");
    ok = opcuaServer.IsValid();
    if (ok) {
        Sleep::MSec(100);
        UA_Client *client = UA_Client_new();
        UA_ClientConfig_setDefault(UA_Client_getConfig(client));
        UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
        ok = (retval != UA_STATUSCODE_GOOD);
        if (ok) {
            opcuaServer->ServerStart();
            Sleep::MSec(100);

            retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
            ok = (retval == UA_STATUSCODE_GOOD);

            if (ok) {
                UA_BrowseRequest bReq;
                UA_BrowseRequest_init(&bReq);
                bReq.requestedMaxReferencesPerNode = 0;
                bReq.nodesToBrowse = UA_BrowseDescription_new();
                bReq.nodesToBrowseSize = 1;
                bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(1u, 3000u);
                bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; /* return everything */
                UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
                ok = (bResp.responseHeader.serviceResult == 0x00U);
            }

            if(ok){
                opcuaServer->ServerStop();
                UA_Client_delete(client);
                client = UA_Client_new();
                Sleep::MSec(100);

                retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
                ok = (retval != UA_STATUSCODE_GOOD);
            }
        }
    }
    ord->Purge();

    return ok;
}

bool OPCUAServerTest::TestExecute() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint8"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    UA_Client *client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
    ok = (retval == UA_STATUSCODE_GOOD);
    if (ok) {
        UA_BrowseRequest bReq;
        UA_BrowseRequest_init(&bReq);
        bReq.requestedMaxReferencesPerNode = 0;
        bReq.nodesToBrowse = UA_BrowseDescription_new();
        bReq.nodesToBrowseSize = 1;
        bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(1u, 3000u);
        bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; /* return everything */
        UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
        ok = (bResp.responseHeader.serviceResult == 0x00U);
    }
    ord->Purge();
    return ok;
}

bool OPCUAServerTest::TestExecute_Introspection() {
    using namespace MARTe;
    StreamString config = ""
            "+OPCUATypes = {"
            "     Class = ReferenceContainer"
            "     +SensorPackage = {"
            "         Class = IntrospectionStructure"
            "         Sensor1 = {"
            "             Type = float64"
            "             NumberOfElements = 1"
            "         }"
            "     }"
            "     +MasterSet = {"
            "         Class = IntrospectionStructure"
            "         SensorPackage1 = {"
            "             Type = SensorPackage"
            "             NumberOfElements = 1"
            "         }"
            "     }"
            "}"
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         TestStructure = {"
            "             Type = MasterSet"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    UA_Client *client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
    ok = (retval == UA_STATUSCODE_GOOD);
    if (ok) {
        UA_BrowseRequest bReq;
        UA_BrowseRequest_init(&bReq);
        bReq.requestedMaxReferencesPerNode = 0;
        bReq.nodesToBrowse = UA_BrowseDescription_new();
        bReq.nodesToBrowseSize = 1;
        bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(1u, 3002u);
        bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; /* return everything */
        UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
        ok = (bResp.responseHeader.serviceResult == 0x00U);
    }
    ord->Purge();
    return ok;
}

bool OPCUAServerTest::TestExecute_IntrospectionArray() {
    using namespace MARTe;
    StreamString config = ""
            "+OPCUATypes = {"
            "     Class = ReferenceContainer"
            "     +SensorPackage = {"
            "         Class = IntrospectionStructure"
            "         Sensor1 = {"
            "             Type = float64"
            "             NumberOfElements = 10"
            "             NumberOfDimensions = 1"
            "         }"
            "     }"
            "     +MasterSet = {"
            "         Class = IntrospectionStructure"
            "         SensorPackage1 = {"
            "             Type = SensorPackage"
            "             NumberOfElements = 1"
            "         }"
            "     }"
            "}"
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         TestStructure = {"
            "             Type = MasterSet"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ReferenceT<OPCUAServer> server;
    if (ok) {
        server = ord->Find("ServerTest");
        ok = server.IsValid();
    }
    ok = server->GetRunning();
    ord->Purge();
    return ok;
}

bool OPCUAServerTest::TestExecute_WrongNDimensions() {
    using namespace MARTe;
    StreamString config = ""
            "+OPCUATypes = {"
            "     Class = ReferenceContainer"
            "     +SensorPackage = {"
            "         Class = IntrospectionStructure"
            "         Sensor1 = {"
            "             Type = float64"
            "             NumberOfElements = {5 2}"
            "             NumberOfDimensions = 2"
            "         }"
            "     }"
            "     +MasterSet = {"
            "         Class = IntrospectionStructure"
            "         SensorPackage1 = {"
            "             Type = SensorPackage"
            "             NumberOfElements = 1"
            "         }"
            "     }"
            "}"
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         TestStructure = {"
            "             Type = MasterSet"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ReferenceT<OPCUAServer> server;
    if (ok) {
        server = ord->Find("ServerTest");
        ok = server.IsValid();
    }
    ok = server->GetRunning();
    ord->Purge();
    return !ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

