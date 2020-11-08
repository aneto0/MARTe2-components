/**
 * @file OPCUAMessageClientTest.cpp
 * @brief Source file for class OPCUAMessageClientTest
 * @date 21/10/2019
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
 * the class OPCUAMessageClientTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "OPCUAMessageClientTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

CLASS_REGISTER(OPCUATestCall, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool OPCUAMessageClientTest::TestConstructor() {
    using namespace MARTe;
    OPCUAMessageClient client;
    StreamString addr = client.GetServerAddress();
    return (addr == "");
}

bool OPCUAMessageClientTest::TestInitialise() {
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
            "       Point = {"
            "        NamespaceIndex = 1"
            "        Path = Point"
            "        NumberOfElements = 1"
            "        Type = Point"
            "       }"
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
    Sleep::MSec(500);
    ots.SetRunning(false);
    ots.service.Stop();
    ord->Purge();
    return ok;
}

bool OPCUAMessageClientTest::TestInitialise_NoServerAddress() {
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
                          "    Method = {"
                          "        NamespaceIndex = 1"
                          "        Path = Test_Object.UpdatePoint"
                          "    }"
                          "    Structure = {"
                          "       Point = {"
                          "        NamespaceIndex = 1"
                          "        Path = Point"
                          "        NumberOfElements = 1"
                          "        Type = Point"
                          "       }"
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
    ots.SetRunning(false);
    ots.service.Stop();
    ord->Purge();
    return (!ok);
}

bool OPCUAMessageClientTest::TestInitialise_NoMethodSection() {
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
                          "    Structure = {"
                          "       Point = {"
                          "        NamespaceIndex = 1"
                          "        Path = Point"
                          "        NumberOfElements = 1"
                          "        Type = Point"
                          "       }"
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
    ots.SetRunning(false);
    ots.service.Stop();
    ord->Purge();
    return (!ok);
}

bool OPCUAMessageClientTest::TestInitialise_NoNamespaceIndex_Method() {
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
                          "        Path = Test_Object.UpdatePoint"
                          "    }"
                          "    Structure = {"
                          "       Point = {"
                          "        NamespaceIndex = 1"
                          "        Path = Point"
                          "        NumberOfElements = 1"
                          "        Type = Point"
                          "       }"
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
    ots.SetRunning(false);
    ots.service.Stop();
    ord->Purge();
    return (!ok);
}

bool OPCUAMessageClientTest::TestInitialise_NoPath_Method() {
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
                          "    }"
                          "    Structure = {"
                          "       Point = {"
                          "        NamespaceIndex = 1"
                          "        Path = Point"
                          "        NumberOfElements = 1"
                          "        Type = Point"
                          "       }"
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
    ots.SetRunning(false);
    ots.service.Stop();
    ord->Purge();
    return (!ok);
}

bool OPCUAMessageClientTest::TestInitialise_NoStructureSection() {
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
    ots.SetRunning(false);
    ots.service.Stop();
    ord->Purge();
    return (!ok);
}

bool OPCUAMessageClientTest::TestInitialise_NoPath_Structure() {
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
                          "       Point = {"
                          "        NamespaceIndex = 1"
                          "        NumberOfElements = 1"
                          "        Type = Point"
                          "       }"
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
    ots.SetRunning(false);
    ots.service.Stop();
    ord->Purge();
    return (!ok);
}

bool OPCUAMessageClientTest::TestInitialise_NoNamespaceIndex_Structure() {
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
                          "       Point = {"
                          "        Path = Point"
                          "        NumberOfElements = 1"
                          "        Type = Point"
                          "       }"
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
    ots.SetRunning(false);
    ots.service.Stop();
    ord->Purge();
    return (!ok);
}

bool OPCUAMessageClientTest::TestInitialise_NoNumberOfElements_Structure() {
    using namespace MARTe;
    OPCUATestServer ots;
    ots.service.Start();
    Sleep::MSec(500);
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
            "       Point = {"
            "        NamespaceIndex = 1"
            "        Path = Point"
            "        Type = Point"
            "       }"
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
    Sleep::MSec(500);
    ots.SetRunning(false);
    ots.service.Stop();
    ord->Purge();
    return ok;
}

bool OPCUAMessageClientTest::TestInitialise_NoType_Structure() {
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
                          "       Point = {"
                          "        NamespaceIndex = 1"
                          "        Path = Point"
                          "        NumberOfElements = 1"
                          "       }"
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
    ots.SetRunning(false);
    ots.service.Stop();
    ord->Purge();
    return (!ok);
}

bool OPCUAMessageClientTest::TestOPCUAMethodCall_WrongMessage() {
    using namespace MARTe;
    OPCUATestServer ots;
    ots.service.Start();
    Sleep::MSec(500);
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
            "       Point = {"
            "        NamespaceIndex = 1"
            "        Path = Point"
            "        NumberOfElements = 1"
            "        Type = Point"
            "       }"
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
    ReferenceT<OPCUATestCall> testClass;
    if (ok) {
        ok = (testClass->TestMethodCall_Wrong() == ErrorManagement::NoError);
    }
    Sleep::MSec(500);
    ots.SetRunning(false);
    ots.service.Stop();
    ord->Purge();
    return !ok;
}

bool OPCUAMessageClientTest::TestOPCUAMethodCall_Single() {
    using namespace MARTe;
    OPCUATestServer ots;
    ots.service.Start();
    Sleep::MSec(500);
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
            "       Point = {"
            "        NamespaceIndex = 1"
            "        Path = Point"
            "        NumberOfElements = 1"
            "        Type = Point"
            "       }"
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
    ReferenceT<OPCUATestCall> testClass;
    if (ok) {
        ok = (testClass->TestMethodCall_Single() == ErrorManagement::NoError);
    }
    Sleep::MSec(500);
    ots.SetRunning(false);
    ots.service.Stop();
    ord->Purge();
    return ok;
}

bool OPCUAMessageClientTest::TestOPCUAMethodCall_Array() {
    using namespace MARTe;
    StreamString config = ""
            "+OPCUATypes = {"
            "    Class = ReferenceContainer"
            "    +Point = {"
            "        Class = IntrospectionStructure"
            "        x = {"
            "            Type = float64"
            "            NumberOfElements = 1"
            "        }"
            "        y = {"
            "            Type = float64"
            "            NumberOfElements = 1"
            "        }"
            "        z = {"
            "            Type = float64"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "}"
            "+MessageClient = {"
            "    Class = OPCUA::OPCUAMessageClient"
            "    Address = \"opc.tcp://192.168.1.89:4840\""
            "    Method = {"
            "        NamespaceIndex = 3"
            "        Path = FAT_Tools_Proto.DataBlocksInstance.OPC_UA_Method_DB.Method"
            "    }"
            "    Structure = {"
            "       Point = {"
            "        NamespaceIndex = 3"
            "        Path = FAT_Tools_Proto.DataBlocksGlobal.OPCUA_TestDB.Point"
            "        NumberOfElements = 1"
            "        Type = Point"
            "       }"
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
    ReferenceT<OPCUATestCall> testClass;
    if (ok) {
        ok = (testClass->TestMethodCall_Array() == ErrorManagement::NoError);
    }
    ord->Purge();
    return ok;
}

