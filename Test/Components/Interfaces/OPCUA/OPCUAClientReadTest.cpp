/**
 * @file OPCUAClientReadTest.cpp
 * @brief Source file for class OPCUAClientReadTest
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
 * the class OPCUAClientReadTest (public, protected, and private). Be aware that some 
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
#include "StandardParser.h"
#include "OPCUAClientRead.h"
#include "OPCUAClientReadTest.h"
#include "OPCUADSInput.h"
#include "RealTimeApplication.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool OPCUAClientReadTest::TestConstructor() {
    using namespace MARTe;
    OPCUAClientRead ocr;
    return (ocr.GetMonitoredNodes() == NULL_PTR(UA_NodeId*));
}

bool OPCUAClientReadTest::Test_SetServiceRequest() {
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
    OPCUAClientRead ocr;
    ocr.SetServerAddress("opc.tcp://localhost:4840");
    ok = ocr.Connect();
    Sleep::MSec(200);
    if (ok) {
        ok = ocr.SetServiceRequest(nsp, path, nOfNodes);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUAClientReadTest::Test_GetExtensionObjectByteString() {
    using namespace MARTe;
    OPCUATestServer ots;
    ots.service.Start();
    StreamString config = ""
            "+OPCUATypes = {\n"
            "     Class = ReferenceContainer\n"
            "     +Point = {\n"
            "         Class = IntrospectionStructure\n"
            "         x = {\n"
            "             Type = float32\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "         y = {\n"
            "             Type = float32\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "         z = {\n"
            "             Type = float32\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "     }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Point = {\n"
            "                    Type = Point\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Point = {\n"
            "                    Type = Point\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
            "            Synchronise = \"yes\""
            "            Signals = {\n"
            "                Point = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = Point\n"
            "                    Type = Point\n"
            "                    ExtensionObject = \"yes\"\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("Test");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    ReferenceT<OPCUADSInput> odi;
    if (ok) {
        odi = ord->Find("Test.Data.OPCUA");
        ok = odi.IsValid();
    }
    if (ok) {
        OPCUAClientRead* ocr = odi->GetOPCUAClient();
        void **mem = ocr->GetValueMemories();
        void *dataPtr = ocr->GetDataPtr();
        ok = (mem[0u] == dataPtr);
    }
    ots.SetRunning(false);
    ots.service.Stop();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUAClientReadTest::Test_Read_Single() {
    using namespace MARTe;
    StreamString config = ""
            "+OPCUATypes = {\n"
            "     Class = ReferenceContainer\n"
            "     +SensorPackage = {\n"
            "         Class = IntrospectionStructure\n"
            "         Sensor1 = {\n"
            "             Type = uint32\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "         Sensor2 = {\n"
            "             Type = uint32\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "         Sensor3 = {\n"
            "             Type = uint32\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "     }\n"
            "     +MasterSet = {\n"
            "         Class = IntrospectionStructure\n"
            "         SensorPackage1 = {\n"
            "             Type = SensorPackage\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "     }\n"
            "}\n"
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4840"
            "     AddressSpace = {"
            "         MasterSet = {"
            "             Type = MasterSet"
            "         }"
            "     }"
            "}"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Sensor3 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Sensor3 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
            "            Synchronise = \"yes\""
            "            Signals = {\n"
            "                Sensor3 = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MasterSet.SensorPackage1.Sensor3\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("Test");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    ReferenceT<OPCUADSInput> odi;
    if (ok) {
        odi = ord->Find("Test.Data.OPCUA");
        ok = odi.IsValid();
    }
    if (ok) {
        OPCUAClientRead * ocr = odi->GetOPCUAClient();
        void **mem = ocr->GetValueMemories();
        uint32 value = 13u;
        ok = MemoryOperationsHelper::Copy(mem[0u], &value, sizeof(uint32));
    }
    if (ok) {
        ok = odi->Synchronise();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUAClientReadTest::Test_Read_ExtensionObject() {
    using namespace MARTe;
    OPCUATestServer ots;
    ots.service.Start();
    StreamString config = ""
            "+OPCUATypes = {\n"
            "     Class = ReferenceContainer\n"
            "     +Point = {\n"
            "         Class = IntrospectionStructure\n"
            "         x = {\n"
            "             Type = float32\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "         y = {\n"
            "             Type = float32\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "         z = {\n"
            "             Type = float32\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "     }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Point = {\n"
            "                    Type = Point\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Point = {\n"
            "                    Type = Point\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
            "            Synchronise = \"yes\""
            "            Signals = {\n"
            "                Point = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = Point\n"
            "                    Type = Point\n"
            "                    ExtensionObject = \"yes\"\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("Test");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(1000);
    if (ok) {
        app->PrepareNextState("State1");
        app->StartNextStateExecution();
    }
    Sleep::MSec(2000);
    ReferenceT<OPCUADSInput> odi;
    if (ok) {
        odi = ord->Find("Test.Data.OPCUA");
        ok = odi.IsValid();
    }
    if (ok) {
        OPCUAClientRead * ocr = odi->GetOPCUAClient();
        float32 *ptr = reinterpret_cast<float32*>(ocr->GetDataPtr());
        ok = (ptr[0] == 1.0);
    }
    app->StopCurrentStateExecution();
    ots.SetRunning(false);
    ots.service.Stop();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUAClientReadTest::Test_Read_ExtensionObject_Complex() {
    using namespace MARTe;
    StreamString config = ""
            "+OPCUATypes = {\n"
            "     Class = ReferenceContainer\n"
            "     +Threshold = {\n"
            "         Class = IntrospectionStructure\n"
            "         Param1 = {\n"
            "             Type = uint8\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "         Param2 = {\n"
            "             Type = uint8\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "     }\n"
            "     +Equal = {\n"
            "         Class = IntrospectionStructure\n"
            "         Param1 = {\n"
            "             Type = uint8\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "         Param2 = {\n"
            "             Type = uint8\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "     }\n"
            "     +Mode_Config = {\n"
            "         Class = IntrospectionStructure\n"
            "         FunctionEqual = {\n"
            "             Type = Equal\n"
            "             NumberOfElements = 2\n"
            "         }\n"
            "         FunctionThreshold = {\n"
            "             Type = Threshold\n"
            "             NumberOfElements = 2\n"
            "         }\n"
            "     }\n"
            "     +SCU = {\n"
            "         Class = IntrospectionStructure\n"
            "         ID = {\n"
            "             Type = uint8\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "         Mode = {\n"
            "             Type = Mode_Config\n"
            "             NumberOfElements = 2\n"
            "         }\n"
            "     }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                SCU_Config = {\n"
            "                    Type = SCU\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                SCU_Config = {\n"
            "                    Type = SCU\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://192.168.1.89:4840\""
            "            Synchronise = \"yes\""
            "            Signals = {\n"
            "                SCU_Config = {\n"
            "                    NamespaceIndex = 3\n"
            "                    Path = FAT_Tools_Proto.DataBlocksGlobal.SCUs_Config.SCU_Config\n"
            "                    Type = SCU\n"
            "                    ExtensionObject = \"yes\"\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("Test");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(1000);
    if (ok) {
        app->PrepareNextState("State1");
        ok = (app->StartNextStateExecution() == ErrorManagement::NoError);
    }
    Sleep::MSec(1000);
    app->StopCurrentStateExecution();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUAClientReadTest::Test_Read_ExtensionObject_Array() {
    using namespace MARTe;
    StreamString config = ""
            "+OPCUATypes = {\n"
            "     Class = ReferenceContainer\n"
            "     +PointArray = {\n"
            "         Class = IntrospectionStructure\n"
            "         x = {\n"
            "             Type = float64\n"
            "             NumberOfElements = 2\n"
            "         }\n"
            "         y = {\n"
            "             Type = float64\n"
            "             NumberOfElements = 2\n"
            "         }\n"
            "         z = {\n"
            "             Type = float64\n"
            "             NumberOfElements = 2\n"
            "         }\n"
            "     }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                PointArray = {\n"
            "                    Type = PointArray\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                PointArray = {\n"
            "                    Type = PointArray\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://192.168.1.89:4840\""
            "            Synchronise = \"yes\""
            "            Signals = {\n"
            "                PointArray = {\n"
            "                    NamespaceIndex = 3\n"
            "                    Path = FAT_Tools_Proto.DataBlocksGlobal.OPCUA_TestDB.PointArray\n"
            "                    Type = PointArray\n"
            "                    ExtensionObject = \"yes\"\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("Test");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(1000);
    if (ok) {
        app->PrepareNextState("State1");
        ok = (app->StartNextStateExecution() == ErrorManagement::NoError);
    }
    Sleep::MSec(1000);
    app->StopCurrentStateExecution();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}
