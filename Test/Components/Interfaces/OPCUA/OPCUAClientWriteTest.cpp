/**
 * @file OPCUAClientWriteTest.cpp
 * @brief Source file for class OPCUAClientWriteTest
 * @date 29/10/2019
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
 * the class OPCUAClientWriteTest (public, protected, and private). Be aware that some 
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
#include "GAM.h"
#include "IOGAM.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "OPCUAClientWrite.h"
#include "OPCUAClientWriteTest.h"
#include "OPCUADSOutput.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include <iostream>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class OPCUATestHelperGam: public MARTe::GAM {
public:CLASS_REGISTER_DECLARATION()
    OPCUATestHelperGam() {
        nElem = 0u;
        output = NULL_PTR(void*);
    }
    virtual ~OPCUATestHelperGam() {

    }

    virtual bool Setup() {
        MARTe::TypeDescriptor outputType = MARTe::GAM::GetSignalType(MARTe::OutputSignals, 0u);
        MARTe::GAM::GetSignalNumberOfElements(MARTe::OutputSignals, 0u, nElem);
        if (outputType == MARTe::UnsignedInteger8Bit) {
            output = reinterpret_cast<MARTe::uint8*>(GetOutputSignalMemory(0u));
            MARTe::uint8 *values = new MARTe::uint8[nElem];
            for (MARTe::uint32 j = 0u; j < nElem; j++) {
                values[j] = 1u;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::uint8) * nElem);
            delete[] values;
        }
        else if (outputType == MARTe::UnsignedInteger16Bit) {
            output = reinterpret_cast<MARTe::uint16*>(GetOutputSignalMemory(0u));
            MARTe::uint16 *values = new MARTe::uint16[nElem];
            for (MARTe::uint32 j = 0u; j < nElem; j++) {
                values[j] = 1u;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::uint16) * nElem);
            delete[] values;
        }
        else if (outputType == MARTe::UnsignedInteger32Bit) {
            output = reinterpret_cast<MARTe::uint32*>(GetOutputSignalMemory(0u));
            MARTe::uint32 *values = new MARTe::uint32[nElem];
            for (MARTe::uint32 j = 0u; j < nElem; j++) {
                values[j] = 1u;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::uint32) * nElem);
            delete[] values;
        }
        else if (outputType == MARTe::UnsignedInteger64Bit) {
            output = reinterpret_cast<MARTe::uint64*>(GetOutputSignalMemory(0u));
            MARTe::uint64 *values = new MARTe::uint64[nElem];
            for (MARTe::uint32 j = 0u; j < nElem; j++) {
                values[j] = 1u;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::uint64) * nElem);
            delete[] values;
        }
        else if (outputType == MARTe::SignedInteger8Bit) {
            output = reinterpret_cast<MARTe::int8*>(GetOutputSignalMemory(0u));
            MARTe::int8 *values = new MARTe::int8[nElem];
            for (MARTe::uint32 j = 0u; j < nElem; j++) {
                values[j] = 1;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::int8) * nElem);
            delete[] values;
        }
        else if (outputType == MARTe::SignedInteger16Bit) {
            output = reinterpret_cast<MARTe::int16*>(GetOutputSignalMemory(0u));
            MARTe::int16 *values = new MARTe::int16[nElem];
            for (MARTe::uint32 j = 0u; j < nElem; j++) {
                values[j] = 1;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::int16) * nElem);
            delete[] values;
        }
        else if (outputType == MARTe::SignedInteger32Bit) {
            output = reinterpret_cast<MARTe::int32*>(GetOutputSignalMemory(0u));
            MARTe::int32 *values = new MARTe::int32[nElem];
            for (MARTe::uint32 j = 0u; j < nElem; j++) {
                values[j] = 1;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::int32) * nElem);
            delete[] values;
        }
        else if (outputType == MARTe::SignedInteger64Bit) {
            output = reinterpret_cast<MARTe::int64*>(GetOutputSignalMemory(0u));
            MARTe::int64 *values = new MARTe::int64[nElem];
            for (MARTe::uint32 j = 0u; j < nElem; j++) {
                values[j] = 1;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::int64) * nElem);
            delete[] values;
        }
        else if (outputType == MARTe::Float32Bit) {
            output = reinterpret_cast<MARTe::float32*>(GetOutputSignalMemory(0u));
            MARTe::float32 *values = new MARTe::float32[nElem];
            for (MARTe::uint32 j = 0u; j < nElem; j++) {
                values[j] = 1.0;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::float32) * nElem);
            delete[] values;
        }
        else if (outputType == MARTe::Float64Bit) {
            output = reinterpret_cast<MARTe::float64*>(GetOutputSignalMemory(0u));
            MARTe::float64 *values = new MARTe::float64[nElem];
            for (MARTe::uint32 j = 0u; j < nElem; j++) {
                values[j] = 1.0;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::float64) * nElem);
            delete[] values;
        }
        return true;
    }

    virtual bool Execute() {
        return true;
    }
private:
    void *output;
    MARTe::uint32 nElem;
};
CLASS_REGISTER(OPCUATestHelperGam, "1.0")
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool OPCUAClientWriteTest::TestConstructor() {
    using namespace MARTe;
    OPCUAClientWrite ocw;
    return (ocw.GetMonitoredNodes() == NULL_PTR(UA_NodeId*));
}

bool OPCUAClientWriteTest::Test_SetServiceRequest() {
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
    OPCUAClientWrite ocr;
    ocr.SetServerAddress("opc.tcp://localhost:4840");
    ok = ocr.Connect();
    Sleep::MSec(200);
    if (ok) {
        ok = ocr.SetServiceRequest(nsp, path, nOfNodes);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUAClientWriteTest::Test_GetExtensionObjectByteString() {
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
            "                    DataSource = OPCUAIn\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Point = {\n"
            "                    Type = Point\n"
            "                    DataSource = OPCUAOut\n"
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
            "        +OPCUAIn = {\n"
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
            "        +OPCUAOut = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("Test.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    if (ok) {
        OPCUAClientWrite *ocr = odo->GetOPCUAClient();
        void **mem = ocr->GetValueMemories();
        void *dataPtr = ocr->GetDataPtr();
        ok = (mem[0u] == dataPtr);
    }
    ots.SetRunning(false);
    ots.service.Stop();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUAClientWriteTest::Test_SetExtensionObject() {
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
            "                    DataSource = OPCUAIn\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Point = {\n"
            "                    Type = Point\n"
            "                    DataSource = OPCUAOut\n"
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
            "        +OPCUAIn = {\n"
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
            "        +OPCUAOut = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
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
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("Test.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    if (ok) {
        OPCUAClientWrite *ocr = odo->GetOPCUAClient();
        UA_ExtensionObject *eos = ocr->GetExtensionObject();
        ok = !(eos == NULL_PTR(UA_ExtensionObject*));
    }
    ots.SetRunning(false);
    ots.service.Stop();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUAClientWriteTest::Test_SetWriteRequest(const MARTe::char8 *typeUT, MARTe::uint8 numberOfElementsUT) {
    using namespace MARTe;

    MARTe::StreamString config;
    config.Printf(SET_WRITE_REQUEST_CONFIG_TEMPLATE, typeUT, numberOfElementsUT, typeUT, numberOfElementsUT, numberOfElementsUT, typeUT, numberOfElementsUT);
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
        OPCUAClientWrite *ocw = odo->GetOPCUAClient();
        ok = (ocw != NULL_PTR(OPCUAClientWrite*));
        if (ok) {
            UA_NodeId *nodes = ocw->GetMonitoredNodes();
            ok = (nodes != NULL_PTR(UA_NodeId*));
        }
    }
    if (ok) {
        ok = app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUAClientWriteTest::Test_WrongNodeId() {
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

    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUAClientWriteTest::Test_Write_ExtensionObject() {
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
            "                    DataSource = OPCUAIn\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Point = {\n"
            "                    Type = Point\n"
            "                    DataSource = OPCUAOut\n"
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
            "        +OPCUAIn = {\n"
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
            "        +OPCUAOut = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
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
    }
    Sleep::MSec(1000);
    ReferenceT<IOGAM> gam;
    if (ok) {
        gam = ord->Find("Test.Functions.GAMDisplay");
        ok = gam.IsValid();
    }
    if(ok) {
        ReferenceContainer rc;
        ok = gam->GetInputBrokers(rc);
        if(ok) {
            ok = gam->GetOutputBrokers(rc);
        }
        if(ok) {
            ReferenceT<MemoryMapSynchronisedInputBroker> ib = rc.Get(0u);
            ReferenceT<MemoryMapSynchronisedOutputBroker> ob = rc.Get(1u);
            ok = ib->Execute();
            if(ok) {
                ok = gam->Execute();
            }
            if(ok) {
                ok = ob->Execute();
            }
        }
    }
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("Test.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    if (ok) {
        OPCUAClientWrite *ocr = odo->GetOPCUAClient();
        UA_ExtensionObject *eos = ocr->GetExtensionObject();
        void * dataPtr = ocr->GetDataPtr();
        ok = (MemoryOperationsHelper::Compare(eos->content.encoded.body.data, dataPtr, 8) == 0);
    }
    ots.SetRunning(false);
    ots.service.Stop();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUAClientWriteTest::Test_Write_ExtensionObject_Complex() {
    using namespace MARTe;
    OPCUATestServer ots;
    ots.service.Start();
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
            "                    DataSource = OPCUAIn\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                SCU_Config = {\n"
            "                    Type = SCU\n"
            "                    DataSource = OPCUAOut\n"
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
            "        +OPCUAIn = {\n"
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
            "        +OPCUAOut = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://192.168.1.89:4840\""
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
    }
    Sleep::MSec(1000);
    ReferenceT<IOGAM> gam;
    if (ok) {
        gam = ord->Find("Test.Functions.GAMDisplay");
        ok = gam.IsValid();
    }
    if(ok) {
        ReferenceContainer rc;
        ok = gam->GetInputBrokers(rc);
        if(ok) {
            ok = gam->GetOutputBrokers(rc);
        }
        if(ok) {
            ReferenceT<MemoryMapSynchronisedInputBroker> ib = rc.Get(0u);
            ReferenceT<MemoryMapSynchronisedOutputBroker> ob = rc.Get(1u);
            ok = ib->Execute();
            if(ok) {
                ok = gam->Execute();
            }
            if(ok) {
                ok = ob->Execute();
            }
        }
    }
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("Test.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    if (ok) {
        OPCUAClientWrite *ocr = odo->GetOPCUAClient();
        UA_ExtensionObject *eos = ocr->GetExtensionObject();
        void * dataPtr = ocr->GetDataPtr();
        ok = (MemoryOperationsHelper::Compare(eos->content.encoded.body.data, dataPtr, 8) == 0);
    }
    ots.SetRunning(false);
    ots.service.Stop();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUAClientWriteTest::Test_Write_ExtensionObject_Array() {
    using namespace MARTe;
    OPCUATestServer ots;
    ots.service.Start();
    StreamString config = ""
            "+OPCUATypes = {\n"
            "     Class = ReferenceContainer\n"
            "     +Point = {\n"
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
            "                    Type = Point\n"
            "                    DataSource = OPCUAIn\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                PointArray = {\n"
            "                    Type = Point\n"
            "                    DataSource = OPCUAOut\n"
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
            "        +OPCUAIn = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://192.168.1.89:4840\""
            "            Synchronise = \"yes\""
            "            Signals = {\n"
            "                PointArray = {\n"
            "                    NamespaceIndex = 3\n"
            "                    Path = FAT_Tools_Proto.DataBlocksGlobal.OPCUA_TestDB.PointArray\n"
            "                    Type = Point\n"
            "                    ExtensionObject = \"yes\"\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +OPCUAOut = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://192.168.1.89:4840\""
            "            Signals = {\n"
            "                PointArray = {\n"
            "                    NamespaceIndex = 3\n"
            "                    Path = FAT_Tools_Proto.DataBlocksGlobal.OPCUA_TestDB.PointArray\n"
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
    }
    Sleep::MSec(1000);
    ReferenceT<IOGAM> gam;
    if (ok) {
        gam = ord->Find("Test.Functions.GAMDisplay");
        ok = gam.IsValid();
    }
    if(ok) {
        ReferenceContainer rc;
        ok = gam->GetInputBrokers(rc);
        if(ok) {
            ok = gam->GetOutputBrokers(rc);
        }
        if(ok) {
            ReferenceT<MemoryMapSynchronisedInputBroker> ib = rc.Get(0u);
            ReferenceT<MemoryMapSynchronisedOutputBroker> ob = rc.Get(1u);
            ok = ib->Execute();
            if(ok) {
                ok = gam->Execute();
            }
            if(ok) {
                ok = ob->Execute();
            }
        }
    }
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("Test.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    if (ok) {
        OPCUAClientWrite *ocr = odo->GetOPCUAClient();
        UA_ExtensionObject *eos = ocr->GetExtensionObject();
        void * dataPtr = ocr->GetDataPtr();
        ok = (MemoryOperationsHelper::Compare(eos->content.encoded.body.data, dataPtr, 8) == 0);
    }
    ots.SetRunning(false);
    ots.service.Stop();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

