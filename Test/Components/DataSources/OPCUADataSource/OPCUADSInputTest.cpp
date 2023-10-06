/**
 * @file OPCUADSInputTest.cpp
 * @brief Source file for class OPCUADSInputTest
 * @date 15/03/2019
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
 * the class OPCUADSInputTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ObjectRegistryDatabase.h"
#include "OPCUADSInputTest.h"
#include "OPCUADSInput.h"
#include "OPCUAServer.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "File.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool OPCUADSInputTest::TestConstructor() {
    using namespace MARTe;
    OPCUADSInput odi;
    StreamString s = odi.GetServerAddress();
    StreamString c = "";
    return (s == c);
}

bool OPCUADSInputTest::TestInitialise_Default() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4840"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                MyNode = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MyNode = {\n"
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
            "            Authentication = None\n"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::TestInitialise_Introspection() {
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
            "     Authentication = None\n"
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
            "            Authentication = None\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::TestInitialise_ExtensionObject() {
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
            "     Authentication = None\n"
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
            "                MasterSet = {\n"
            "                    Type = MasterSet\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MasterSet = {\n"
            "                    Type = MasterSet\n"
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
            "            Authentication = None\n"
            "            Synchronise = \"yes\""
            "            Signals = {\n"
            "                MasterSet = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MasterSet\n"
            "                    Type = MasterSet\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::TestSynchronise_Default() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4840"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                MyNode = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MyNode = {\n"
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
            "            Authentication = None\n"
            "            Synchronise = \"yes\""
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::TestSynchronise_Monitor() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4840"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                MyNode = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MyNode = {\n"
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
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            Authentication = None\n"
            "            Synchronise = \"yes\"\n"
            "            ReadMode = \"Monitor\"\n"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::TestSynchronise_WrongMode() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4840"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                MyNode = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MyNode = {\n"
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
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            Authentication = None\n"
            "            Synchronise = \"yes\"\n"
            "            ReadMode = \"PubSub\"\n"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::TestExecute_Default() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4840"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                MyNode = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MyNode = {\n"
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
            "            Authentication = None\n"
            "            Synchronise = \"no\""
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::TestExecute_NoDefault() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4840"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                MyNode = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MyNode = {\n"
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
            "            Authentication = None\n"
            "            Synchronise = \"no\""
            "            CpuMask = 0x2"
            "            StackSize = 10000000"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::TestExecute_Monitor() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4840"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                MyNode = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MyNode = {\n"
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
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            Authentication = None\n"
            "            Synchronise = \"no\"\n"
            "            ReadMode = \"Monitor\"\n"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::TestExecute_WrongMode() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4840"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                MyNode = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MyNode = {\n"
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
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            Authentication = None\n"
            "            Synchronise = \"no\"\n"
            "            ReadMode = \"PubSub\"\n"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::TestInitialise_NoAddress() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4850"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Authentication = None\n"
            "            Synchronise = \"no\""
            "            SamplingTime = 1"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSInputTest::TestInitialise_NoSignals() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4851"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
            "            Authentication = None\n"
            "            Synchronise = \"no\""
            "            SamplingTime = 1"
            "        }\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSInputTest::TestInitialise_NoPath() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4852"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
            "            Authentication = None\n"
            "            Synchronise = \"no\""
            "            SamplingTime = 1"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSInputTest::TestInitialise_NoNamespaceIndex() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Port = 4853"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
            "            Authentication = None\n"
            "            Synchronise = \"no\""
            "            SamplingTime = 1"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    Path = MyNode\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSInputTest::Test_NumberOfDimensionsGreater1() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                MyNode = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUAIn\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MyNode = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +DDB1 = {\n"
            "            Class = GAMDataSource\n"
            "        }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUAIn = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
            "            Authentication = None\n"
            "            Synchronise = \"yes\""
            "            SamplingTime = 1"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
            "                    Type = uint32\n"
            "                    NumberOfDimensions = 2\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "      +Timer = {\n"
            "          Class = LinuxTimer\n"
            "          SleepNature = \"Default\"\n"
            "          Signals = {\n"
            "             Counter = {\n"
            "               Type = uint32\n"
            "             }\n"
            "             Time = {\n"
            "               Type = uint32\n"
            "             }\n"
            "       }\n"
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
            "                    CPUs = 0x2\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSInputTest::Test_SetConfiguredDatabase_FailSetServiceRequest() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     Authentication = None\n"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
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
            "                MyNode = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUAIn\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MyNode = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +DDB1 = {\n"
            "            Class = GAMDataSource\n"
            "        }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUAIn = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
            "            Authentication = None\n"
            "            Synchronise = \"yes\""
            "            SamplingTime = 1"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode1\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "      +Timer = {\n"
            "          Class = LinuxTimer\n"
            "          SleepNature = \"Default\"\n"
            "          Signals = {\n"
            "             Counter = {\n"
            "               Type = uint32\n"
            "             }\n"
            "             Time = {\n"
            "               Type = uint32\n"
            "             }\n"
            "       }\n"
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
            "                    CPUs = 0x2\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSInputTest::Test_SetConfiguredDatabase_ExtensionObject() {
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
            "     Authentication = None\n"
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
            "                MasterSet = {\n"
            "                    Type = MasterSet\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MasterSet = {\n"
            "                    Type = MasterSet\n"
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
            "            Authentication = None\n"
            "            Synchronise = \"yes\""
            "            Signals = {\n"
            "                MasterSet = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MasterSet\n"
            "                    Type = MasterSet\n"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::Test_Authentication() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "    Class = OPCUA::OPCUAServer\n"
            "    Port = 4840\n"
            "    Authentication = UserPassword\n"
            "    UserPasswordFile = /tmp/opcua_server_test_auth_keys.cfg\n"
            "    AddressSpace = {\n"
            "        Number = {\n"
            "            Type = uint32\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Number = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUAIn\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Number = {\n"
            "                    Type = uint32\n"
            "                    DataSource = LoggerDataSource\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +LoggerDataSource = {\n"
            "            Class = LoggerDataSource\n"
            "        }\n"
            "        +OPCUAIn = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            ReadMode = Read\n"
            "            Synchronise = yes\n"
            "            Authentication = UserPassword\n"
            "            UserPasswordFile = /tmp/opcua_client_test_auth_key.cfg\n"
            "            Signals = {\n"
            "                Number = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = Number\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +Timer = {\n"
            "            Class = LinuxTimer\n"
            "            SleepNature = \"Default\"\n"
            "            Signals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    StreamString serverAuthKeys = ""
            "AuthenticationKeys = {"
            "   KeyA = {"
            "       Username = user1"
            "       Password = password1"
            "   }"
            "   KeyB = {"
            "       Username = user2"
            "       Password = password2"
            "   }"
            "}\n";
    uint32 serverAuthKeysSize = serverAuthKeys.Size();
    File serverAuthKeysFile;
    bool ok = serverAuthKeysFile.Open("/tmp/opcua_server_test_auth_keys.cfg",
    	BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_TRUNC | BasicFile::FLAG_APPEND);
    ok = ok && serverAuthKeysFile.Write(serverAuthKeys.Buffer(), serverAuthKeysSize);
    (void) serverAuthKeysFile.Close();

    StreamString clientAuthKey = ""
            "AuthenticationKey = {"
            "    Username = user2"
            "    Password = password2"
            "}\n";
    uint32 clientAuthKeySize = clientAuthKey.Size();
    File clientAuthKeyFile;
    ok = ok && clientAuthKeyFile.Open("/tmp/opcua_client_test_auth_key.cfg",
        BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_TRUNC | BasicFile::FLAG_APPEND);
    ok = ok && clientAuthKeyFile.Write(clientAuthKey.Buffer(), clientAuthKeySize);
    (void) clientAuthKeyFile.Close();

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    ok = ok && parser.Parse();
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
        // ConfigureApplication() calls SetConfiguredDatabase() on the DS
        // which uses a client to connect with authentication
        ok = app->ConfigureApplication();
    }
    (void) remove("/tmp/opcua_server_test_auth_keys.cfg");
    (void) remove("/tmp/opcua_client_test_auth_key.cfg");
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::Test_Authentication_NoCreds() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "    Class = OPCUA::OPCUAServer\n"
            "    Port = 4840\n"
            "    Authentication = UserPassword\n"
            "    UserPasswordFile = /tmp/opcua_server_test_auth_keys.cfg\n"
            "    AddressSpace = {\n"
            "        Number = {\n"
            "            Type = uint32\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Number = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUAIn\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Number = {\n"
            "                    Type = uint32\n"
            "                    DataSource = LoggerDataSource\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +LoggerDataSource = {\n"
            "            Class = LoggerDataSource\n"
            "        }\n"
            "        +OPCUAIn = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            ReadMode = Read\n"
            "            Synchronise = yes\n"
            "            Authentication = UserPassword\n"
            "            Signals = {\n"
            "                Number = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = Number\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +Timer = {\n"
            "            Class = LinuxTimer\n"
            "            SleepNature = \"Default\"\n"
            "            Signals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    StreamString serverAuthKeys = ""
            "AuthenticationKeys = {"
            "   KeyA = {"
            "       Username = user1"
            "       Password = password1"
            "   }"
            "   KeyB = {"
            "       Username = user2"
            "       Password = password2"
            "   }"
            "}\n";
    uint32 serverAuthKeysSize = serverAuthKeys.Size();
    File serverAuthKeysFile;
    bool ok = serverAuthKeysFile.Open("/tmp/opcua_server_test_auth_keys.cfg",
    	BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_TRUNC | BasicFile::FLAG_APPEND);
    ok = ok && serverAuthKeysFile.Write(serverAuthKeys.Buffer(), serverAuthKeysSize);
    (void) serverAuthKeysFile.Close();

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    ok = ok && parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = !(ord->Initialise(cdb));
    }
    Sleep::MSec(200);
    (void) remove("/tmp/opcua_server_test_auth_keys.cfg");
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSInputTest::Test_Authentication_BadCreds() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "    Class = OPCUA::OPCUAServer\n"
            "    Port = 4840\n"
            "    Authentication = UserPassword\n"
            "    UserPasswordFile = /tmp/opcua_server_test_auth_keys.cfg\n"
            "    AddressSpace = {\n"
            "        Number = {\n"
            "            Type = uint32\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Number = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUAIn\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Number = {\n"
            "                    Type = uint32\n"
            "                    DataSource = LoggerDataSource\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +LoggerDataSource = {\n"
            "            Class = LoggerDataSource\n"
            "        }\n"
            "        +OPCUAIn = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            ReadMode = Read\n"
            "            Synchronise = yes\n"
            "            Authentication = UserPassword\n"
            "            UserPasswordFile = /tmp/opcua_client_test_auth_key.cfg\n"
            "            Signals = {\n"
            "                Number = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = Number\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +Timer = {\n"
            "            Class = LinuxTimer\n"
            "            SleepNature = \"Default\"\n"
            "            Signals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    StreamString serverAuthKeys = ""
            "AuthenticationKeys = {"
            "   KeyA = {"
            "       Username = user1"
            "       Password = password1"
            "   }"
            "   KeyB = {"
            "       Username = user2"
            "       Password = password2"
            "   }"
            "}\n";
    uint32 serverAuthKeysSize = serverAuthKeys.Size();
    File serverAuthKeysFile;
    bool ok = serverAuthKeysFile.Open("/tmp/opcua_server_test_auth_keys.cfg",
    	BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_TRUNC | BasicFile::FLAG_APPEND);
    ok = ok && serverAuthKeysFile.Write(serverAuthKeys.Buffer(), serverAuthKeysSize);
    (void) serverAuthKeysFile.Close();

    StreamString clientAuthKey = ""
            "AuthenticationKey = {"
            "    Username = user2"
            "    Password = password77"
            "}\n";
    uint32 clientAuthKeySize = clientAuthKey.Size();
    File clientAuthKeyFile;
    ok = ok && clientAuthKeyFile.Open("/tmp/opcua_client_test_auth_key.cfg",
        BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_TRUNC | BasicFile::FLAG_APPEND);
    ok = ok && clientAuthKeyFile.Write(clientAuthKey.Buffer(), clientAuthKeySize);
    (void) clientAuthKeyFile.Close();

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    ok = ok && parser.Parse();
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
        // ConfigureApplication() calls SetConfiguredDatabase() on the DS
        // which uses a client to connect with authentication
        ok = !(app->ConfigureApplication());
    }
    (void) remove("/tmp/opcua_server_test_auth_keys.cfg");
    (void) remove("/tmp/opcua_client_test_auth_key.cfg");
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}
