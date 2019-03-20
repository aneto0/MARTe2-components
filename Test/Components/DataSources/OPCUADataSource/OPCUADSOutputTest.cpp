/**
 * @file OPCUADSOutputTest.cpp
 * @brief Source file for class OPCUADSOutputTest
 * @date 14 Mar 2019 TODO Verify the value and format of the date
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
 * the class OPCUADSOutputTest (public, protected, and private). Be aware that some 
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
#include "OPCUADSOutputTest.h"
#include "OPCUADSOutput.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

bool OPCUADSOutputTest::TestConstructor() {
    using namespace MARTe;
    OPCUADSOutput odo;
    StreamString s = odo.GetServerAddress();
    StreamString c = "";
    return (s == c);
}

bool OPCUADSOutputTest::TestInitialise() {
    using namespace MARTe;
    StreamString config = ""
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
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
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
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
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
    return ok;
}

bool OPCUADSOutputTest::TestInitialise_Introspection() {
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
            "     }\n"
            "     +MasterSet = {\n"
            "         Class = IntrospectionStructure\n"
            "         SensorPackage1 = {\n"
            "             Type = SensorPackage\n"
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
            "                    Alias = \"Sensor1\"\n"
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
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUAOut\n"
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
            "        +OPCUAOut = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            Signals = {\n"
            "                Sensor1 = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MasterSet.SensorPackage1.Sensor1\n"
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
    return ok;
}

bool OPCUADSOutputTest::TestInitialise_NoAddress() {
    using namespace MARTe;
    StreamString config = ""
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
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
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
            "            Class = OPCUADataSource::OPCUADSOutput\n"
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

bool OPCUADSOutputTest::TestInitialise_NoSignals() {
    using namespace MARTe;
    StreamString config = ""
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
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
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
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
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

bool OPCUADSOutputTest::TestInitialise_NoPath() {
    using namespace MARTe;
    StreamString config = ""
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
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
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
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
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

bool OPCUADSOutputTest::TestInitialise_NoNamespaceIndex() {
    using namespace MARTe;
    StreamString config = ""
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
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
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
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
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

bool OPCUADSOutputTest::Test_NumberOfDimensionsGreater1() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint32\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           Alias = \"MyNode\"\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = uint32\n"
            "          NumberOfDimensions = 2\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
            "}\n";
    config.Seek(0LLU);
    //HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
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
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_FailSetTargetNodes() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint32\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           Alias = \"MyNode\"\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode1\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_uint8() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint8\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint8\n"
            "           Default = 2\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint8\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint8\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = uint8\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_uint16() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint16\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint16\n"
            "           Default = 2\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint16\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint16\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = uint16\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_uint32() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint32\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "           Default = 2\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_uint64() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint64\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint64\n"
            "           Default = 2\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint64\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint64\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = uint64\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_int8() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = int8\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int8\n"
            "           Default = 2\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int8\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = int8\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = int8\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_int16() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = int16\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int16\n"
            "           Default = 2\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int16\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = int16\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = int16\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_int32() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = int32\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int32\n"
            "           Default = 2\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = int32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = int32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_int64() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = int64\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int64\n"
            "           Default = 2\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int64\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = int64\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = int64\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_float32() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = float32\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = float32\n"
            "           Default = 2\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = float32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = float32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = float32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_float64() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = float64\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = float64\n"
            "           Default = 2\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = float64\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = float64\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = float64\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_WrongNodeId() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint32\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode1 = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "           Default = 2\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode1 = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode1 = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode1 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode1\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_uint8_array() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint8\n"
            "      NumberOfElements = 5\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint8\n"
            "           NumberOfElements = 5\n"
            "           Default = {2 2 2 2 2}\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint8\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint8\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = uint8\n"
            "           NumberOfElements = 5\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(500);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(500);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_uint16_array() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint16\n"
            "      NumberOfElements = 5\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint16\n"
            "           NumberOfElements = 5\n"
            "           Default = {2 2 2 2 2}\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint16\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint16\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = uint16\n"
            "           NumberOfElements = 5\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(500);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(500);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_uint32_array() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint32\n"
            "      NumberOfElements = 5\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "           NumberOfElements = 5\n"
            "           Default = {2 2 2 2 2}\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint32\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = uint32\n"
            "           NumberOfElements = 5\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_uint64_array() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint64\n"
            "      NumberOfElements = 5\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint64\n"
            "           NumberOfElements = 5\n"
            "           Default = {2 2 2 2 2}\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint64\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint64\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = uint64\n"
            "           NumberOfElements = 5\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_int8_array() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = int8\n"
            "      NumberOfElements = 5\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int8\n"
            "           NumberOfElements = 5\n"
            "           Default = {2 2 2 2 2}\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int8\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = int8\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = int8\n"
            "           NumberOfElements = 5\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_int16_array() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = int16\n"
            "      NumberOfElements = 5\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int16\n"
            "           NumberOfElements = 5\n"
            "           Default = {2 2 2 2 2}\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int16\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = int16\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = int16\n"
            "           NumberOfElements = 5\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_int32_array() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = int32\n"
            "      NumberOfElements = 5\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int32\n"
            "           NumberOfElements = 5\n"
            "           Default = {2 2 2 2 2}\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int32\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = int32\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = int32\n"
            "           NumberOfElements = 5\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_int64_array() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = int64\n"
            "      NumberOfElements = 5\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int64\n"
            "           NumberOfElements = 5\n"
            "           Default = {2 2 2 2 2}\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = int64\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = int64\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = int64\n"
            "           NumberOfElements = 5\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_float32_array() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 5\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = float32\n"
            "           NumberOfElements = 5\n"
            "           Default = {2.0 2.0 2.0 2.0 2.0}\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = float32\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = float32\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = float32\n"
            "           NumberOfElements = 5\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_Write_float64_array() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = float64\n"
            "      NumberOfElements = 5\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMValue = {\n"
            "       Class = ConstantGAM\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = float64\n"
            "           NumberOfElements = 5\n"
            "           Default = {2.0 2.0 2.0 2.0 2.0}\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = float64\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = float64\n"
            "           NumberOfElements = 5\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = float64\n"
            "           NumberOfElements = 5\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
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
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay GAMValue}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    if (ok) {
        OPCUAClientWrapper * ocw = odo->GetClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrapper *));
        if (ok) {
            UA_NodeId * nodes = ocw->GetMonitoredNodes();
            ok = (nodes == NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

