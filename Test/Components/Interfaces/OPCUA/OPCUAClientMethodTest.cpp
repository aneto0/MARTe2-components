/**
 * @file OPCUAClientMethodTest.cpp
 * @brief Source file for class OPCUAClientMethodTest
 * @date 30/10/2019
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
 * the class OPCUAClientMethodTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "ObjectRegistryDatabase.h"
#include "OPCUAClientMethod.h"
#include "OPCUAClientMethodTest.h"
#include "OPCUAMessageClient.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool OPCUAClientMethodTest::TestConstructor() {
	using namespace MARTe;
	OPCUAClientMethod ocm;
return (ocm.GetMonitoredNodes() == NULL_PTR(UA_NodeId*));
}

bool OPCUAClientMethodTest::Test_SetServiceRequest() {
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
StreamString *path = new StreamString("MyNode");
uint16 ns = 1;
uint16 *nsp = &ns;
const uint32 nOfNodes = 1u;
OPCUAClientMethod ocm;
ocm.SetServerAddress("opc.tcp://localhost:4840");
ok = ocm.Connect();
Sleep::MSec(200);
if (ok) {
	ok = ocm.SetServiceRequest(nsp, path, nOfNodes);
}
Sleep::MSec(500);
ObjectRegistryDatabase::Instance()->Purge();
delete path;
return ok;
}

bool OPCUAClientMethodTest::Test_SetObjectRequest() {
using namespace MARTe;
OPCUATestServer ots;
ots.service.Start();
Sleep::MSec(200);
StreamString path = "Test_Object.UpdatePoint";
uint16 ns = 1;
OPCUAClientMethod ocm;
ocm.SetServerAddress("opc.tcp://localhost:4840");
bool ok = ocm.Connect();
Sleep::MSec(200);
if (ok) {
	ok = ocm.SetObjectRequest(ns, path);
}
Sleep::MSec(500);
ots.SetRunning(false);
ots.service.Stop();
return ok;
}

bool OPCUAClientMethodTest::Test_SetMethodRequest() {
using namespace MARTe;
OPCUATestServer ots;
ots.service.Start();
Sleep::MSec(200);
StreamString path = "Test_Object.UpdatePoint";
uint16 ns = 1;
OPCUAClientMethod ocm;
ocm.SetServerAddress("opc.tcp://localhost:4840");
bool ok = ocm.Connect();
Sleep::MSec(200);
if (ok) {
	ok = ocm.SetMethodRequest(ns, path);
}
Sleep::MSec(500);
ots.SetRunning(false);
ots.service.Stop();
return ok;
}

bool OPCUAClientMethodTest::Test_GetExtensionObjectByteString() {
using namespace MARTe;
OPCUATestServer ots;
ots.service.Start();
StreamString config = ""
		"+OPCUATypes = {"
		"    Class = ReferenceContainer"
		"    +Point = {"
		"        Class = IntrospectionStructure"
		"        x = {"
		"            Type = float32"
		"            NumberOfElements = 1"
		"        }"
		"        y = {"
		"            Type = float32"
		"            NumberOfElements = 1"
		"        }"
		"        z = {"
		"            Type = float32"
		"            NumberOfElements = 1"
		"        }"
		"    }"
		"}"
		"+MessageClient = {"
		"    Class = OPCUA::OPCUAMessageClient"
		"    Address = \"opc.tcp://127.0.0.1:4840\""
		"    Method = {"
		"        NamespaceIndex = 1"
		"        Path = Test_Object.UpdatePoint"
		"    }"
		"    Structure = {"
		"        Point = {"
		"            NamespaceIndex = 1"
		"            Path = Point"
		"            NumberOfElements = 1"
		"            Type = Point"
		"        }"
		"    }"
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
if (ok) {
	ReferenceT<OPCUAMessageClient> omc;
	omc = ord->Find("MessageClient");
	ok = omc.IsValid();
	if (ok) {
		OPCUAClientMethod *ocm = omc->GetOPCUAClient();
		void **mem = ocm->GetValueMemories();
		void *dataPtr = ocm->GetDataPtr();
		ok = (mem[0u] == dataPtr);
	}
}
ots.SetRunning(false);
ots.service.Stop();
ObjectRegistryDatabase::Instance()->Purge();
return ok;
}

bool OPCUAClientMethodTest::Test_SetExtensionObject() {
using namespace MARTe;
OPCUATestServer ots;
ots.service.Start();
StreamString config = ""
		"+OPCUATypes = {"
		"    Class = ReferenceContainer"
		"    +Point = {"
		"        Class = IntrospectionStructure"
		"        x = {"
		"            Type = float32"
		"            NumberOfElements = 1"
		"        }"
		"        y = {"
		"            Type = float32"
		"            NumberOfElements = 1"
		"        }"
		"        z = {"
		"            Type = float32"
		"            NumberOfElements = 1"
		"        }"
		"    }"
		"}"
		"+MessageClient = {"
		"    Class = OPCUA::OPCUAMessageClient"
		"    Address = \"opc.tcp://127.0.0.1:4840\""
		"    Method = {"
		"        NamespaceIndex = 1"
		"        Path = Test_Object.UpdatePoint"
		"    }"
		"    Structure = {"
		"        Point = {"
		"            NamespaceIndex = 1"
		"            Path = Point"
		"            NumberOfElements = 1"
		"            Type = Point"
		"        }"
		"    }"
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
if (ok) {
	ReferenceT<OPCUAMessageClient> omc;
	omc = ord->Find("MessageClient");
	ok = omc.IsValid();
	if (ok) {
		OPCUAClientMethod *ocm = omc->GetOPCUAClient();
		UA_ExtensionObject *eos = ocm->GetExtensionObject();
	ok = !(eos == NULL_PTR(UA_ExtensionObject*));
}
}
ots.SetRunning(false);
ots.service.Stop();
ObjectRegistryDatabase::Instance()->Purge();
return ok;
}

bool OPCUAClientMethodTest::Test_WrongNodeId() {
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
StreamString *path = new StreamString("MyNode1");
uint16 ns = 1u;
uint16 *nsp = &ns;
const uint32 nOfNodes = 1u;
OPCUAClientMethod ocm;
ocm.SetServerAddress("opc.tcp://localhost:4840");
ok = ocm.Connect();
Sleep::MSec(200);
if (ok) {
ok = ocm.SetServiceRequest(nsp, path, nOfNodes);
}
ObjectRegistryDatabase::Instance()->Purge();
return !ok;
}

bool OPCUAClientMethodTest::Test_WrongObjectNodeId() {
using namespace MARTe;
OPCUATestServer ots;
ots.service.Start();
StreamString path = "Test_Object1";
uint16 ns = 1u;
OPCUAClientMethod ocm;
ocm.SetServerAddress("opc.tcp://localhost:4840");
bool ok = ocm.Connect();
Sleep::MSec(200);
if (ok) {
ok = ocm.SetObjectRequest(ns, path);
}
ots.SetRunning(false);
ots.service.Stop();
return !ok;
}

bool OPCUAClientMethodTest::Test_WrongMethodNodeId() {
using namespace MARTe;
OPCUATestServer ots;
ots.service.Start();
StreamString path = "Test_Object.UpdatePoint1";
uint16 ns = 1u;
OPCUAClientMethod ocm;
ocm.SetServerAddress("opc.tcp://localhost:4840");
bool ok = ocm.Connect();
Sleep::MSec(200);
if (ok) {
ok = ocm.SetMethodRequest(ns, path);
}
ots.SetRunning(false);
ots.service.Stop();
return !ok;
}

bool OPCUAClientMethodTest::Test_MethodCall() {
using namespace MARTe;
OPCUATestServer ots;
ots.service.Start();
StreamString config = ""
	"+OPCUATypes = {"
	"    Class = ReferenceContainer"
	"    +Point = {"
	"        Class = IntrospectionStructure"
	"        x = {"
	"            Type = float32"
	"            NumberOfElements = 1"
	"        }"
	"        y = {"
	"            Type = float32"
	"            NumberOfElements = 1"
	"        }"
	"        z = {"
	"            Type = float32"
	"            NumberOfElements = 1"
	"        }"
	"    }"
	"}"
	"+MessageClient = {"
	"    Class = OPCUA::OPCUAMessageClient"
	"    Address = \"opc.tcp://127.0.0.1:4840\""
	"    Method = {"
	"        NamespaceIndex = 1"
	"        Path = Test_Object.UpdatePoint"
	"    }"
	"    Structure = {"
	"        Point = {"
	"            NamespaceIndex = 1"
	"            Path = Point"
	"            NumberOfElements = 1"
	"            Type = Point"
	"        }"
	"    }"
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
if (ok) {
ReferenceT<OPCUAMessageClient> omc;
omc = ord->Find("MessageClient");
ok = omc.IsValid();
if (ok) {
	OPCUAClientMethod *ocm = omc->GetOPCUAClient();
	void *dataPtr = ocm->GetDataPtr();
	uint8 *tempDataPtr = reinterpret_cast<uint8*>(dataPtr);
	float32 x = 4.0;
	float32 y = 5.0;
	float32 z = 6.0;
	ok = MemoryOperationsHelper::Copy(tempDataPtr, &x, sizeof(float32));
	if (ok) {
		tempDataPtr = &(tempDataPtr[sizeof(float32)]);
		ok = MemoryOperationsHelper::Copy(tempDataPtr, &y, sizeof(float32));
	}
	if (ok) {
		tempDataPtr = &(tempDataPtr[sizeof(float32)]);
		ok = MemoryOperationsHelper::Copy(tempDataPtr, &z, sizeof(float32));
	}
	if (ok) {
		ok = ocm->MethodCall();
	}
	if (ok) {
		UA_DataType types[1u];
		types[0u] = PointType;
		UA_DataTypeArray customDataTypes = { NULL, 1, types };
		UA_Client *client = UA_Client_new();
		UA_ClientConfig *cc = UA_Client_getConfig(client);
		UA_ClientConfig_setDefault(cc);
		cc->customDataTypes = &customDataTypes;
		UA_StatusCode retval = UA_Client_connect(client,
				"opc.tcp://localhost:4840");
		if (retval == UA_STATUSCODE_GOOD) {
			UA_Variant value; /* Variants can hold scalar values and arrays of any type */
			UA_Variant_init(&value);
			UA_NodeId nodeId = UA_NODEID_STRING(1u, const_cast<char8*>("Point"));
			retval = UA_Client_readValueAttribute(client, nodeId, &value);

			if (retval == UA_STATUSCODE_GOOD) {
				Point *p = (Point*) value.data;
				ok = (p->x == 4.0);
				if (ok) {
					ok = (p->y == 5.0);
				}
				if (ok) {
					ok = (p->z == 6.0);
				}
			} else {
				ok = false;
			}
		}
		else {
			ok = false;
		}
	}
}
}
ots.SetRunning(false);
ots.service.Stop();
ObjectRegistryDatabase::Instance()->Purge();
return ok;
}

