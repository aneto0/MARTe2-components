/**
 * @file OPCUAClientWrapperTest.cpp
 * @brief Source file for class OPCUAClientWrapperTest
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
 * the class OPCUAClientWrapperTest (public, protected, and private). Be aware that some 
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
#include "OPCUAClientWrapper.h"
#include "OPCUAClientWrapperTest.h"
#include "OPCUADSOutput.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class OPCUATestHelperGam: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()OPCUATestHelperGam() {
        nElem = 0u;
        output = NULL_PTR(void*);
    }
    virtual ~OPCUATestHelperGam() {

    }

    virtual bool Setup() {
        MARTe::TypeDescriptor outputType = MARTe::GAM::GetSignalType(MARTe::OutputSignals, 0u);
        MARTe::GAM::GetSignalNumberOfElements(MARTe::OutputSignals, 0u, nElem);
        if(outputType == MARTe::UnsignedInteger8Bit) {
            output = reinterpret_cast<MARTe::uint8*>(GetOutputSignalMemory(0u));
            MARTe::uint8* values = new MARTe::uint8[nElem];
            for(MARTe::uint32 j = 0u; j<nElem; j++) {
                values[j] = 1u;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::uint8)*nElem);
            delete [] values;
        }
        else if(outputType == MARTe::UnsignedInteger16Bit) {
            output = reinterpret_cast<MARTe::uint16*>(GetOutputSignalMemory(0u));
            MARTe::uint16* values = new MARTe::uint16[nElem];
            for(MARTe::uint32 j = 0u; j<nElem; j++) {
                values[j] = 1u;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::uint16)*nElem);
            delete [] values;
        }
        else if(outputType == MARTe::UnsignedInteger32Bit) {
            output = reinterpret_cast<MARTe::uint32*>(GetOutputSignalMemory(0u));
            MARTe::uint32* values = new MARTe::uint32[nElem];
            for(MARTe::uint32 j = 0u; j<nElem; j++) {
                values[j] = 1u;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::uint32)*nElem);
            delete [] values;
        }
        else if(outputType == MARTe::UnsignedInteger64Bit) {
            output = reinterpret_cast<MARTe::uint64*>(GetOutputSignalMemory(0u));
            MARTe::uint64* values = new MARTe::uint64[nElem];
            for(MARTe::uint32 j = 0u; j<nElem; j++) {
                values[j] = 1u;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::uint64)*nElem);
            delete [] values;
        }
        else if(outputType == MARTe::SignedInteger8Bit) {
            output = reinterpret_cast<MARTe::int8*>(GetOutputSignalMemory(0u));
            MARTe::int8* values = new MARTe::int8[nElem];
            for(MARTe::uint32 j = 0u; j<nElem; j++) {
                values[j] = 1;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::int8)*nElem);
            delete [] values;
        }
        else if(outputType == MARTe::SignedInteger16Bit) {
            output = reinterpret_cast<MARTe::int16*>(GetOutputSignalMemory(0u));
            MARTe::int16* values = new MARTe::int16[nElem];
            for(MARTe::uint32 j = 0u; j<nElem; j++) {
                values[j] = 1;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::int16)*nElem);
            delete [] values;
        }
        else if(outputType == MARTe::SignedInteger32Bit) {
            output = reinterpret_cast<MARTe::int32*>(GetOutputSignalMemory(0u));
            MARTe::int32* values = new MARTe::int32[nElem];
            for(MARTe::uint32 j = 0u; j<nElem; j++) {
                values[j] = 1;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::int32)*nElem);
            delete [] values;
        }
        else if(outputType == MARTe::SignedInteger64Bit) {
            output = reinterpret_cast<MARTe::int64*>(GetOutputSignalMemory(0u));
            MARTe::int64* values = new MARTe::int64[nElem];
            for(MARTe::uint32 j = 0u; j<nElem; j++) {
                values[j] = 1;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::int64)*nElem);
            delete [] values;
        }
        else if(outputType == MARTe::Float32Bit) {
            output = reinterpret_cast<MARTe::float32*>(GetOutputSignalMemory(0u));
            MARTe::float32* values = new MARTe::float32[nElem];
            for(MARTe::uint32 j = 0u; j<nElem; j++) {
                values[j] = 1.0;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::float32)*nElem);
            delete [] values;
        }
        else if(outputType == MARTe::Float64Bit) {
            output = reinterpret_cast<MARTe::float64*>(GetOutputSignalMemory(0u));
            MARTe::float64* values = new MARTe::float64[nElem];
            for(MARTe::uint32 j = 0u; j<nElem; j++) {
                values[j] = 1.0;
            }
            MARTe::MemoryOperationsHelper::Copy(output, values, sizeof(MARTe::float64)*nElem);
            delete [] values;
        }
        return true;
    }

    virtual bool Execute() {
        return true;
    }
private:
    void* output;
    MARTe::uint32 nElem;
};
CLASS_REGISTER(OPCUATestHelperGam, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool OPCUAClientWrapperTest::Test_Write_uint8() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_uint16() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_uint32() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_uint64() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_int8() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_int16() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_int32() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_int64() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_float32() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_float64() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_uint8_array() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_uint16_array() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_uint32_array() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_uint64_array() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_int8_array() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_int16_array() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_int32_array() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_int64_array() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_float32_array() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_Write_float64_array() {
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
            "       Class = OPCUATestHelperGam\n"
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
            "          Functions = {GAMTimer GAMValue}\n"
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

bool OPCUAClientWrapperTest::Test_WrongNodeId() {
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

bool OPCUAClientWrapperTest::Test_Read() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
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
            "            Synchronise = \"yes\""
            "            SamplingTime = 1"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
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
    ReferenceT < RealTimeApplication > app;
    if (ok) {
        app = ord->Find("Test");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    if (ok) {
        app->StartNextStateExecution();
    }
    Sleep::MSec(200);
    app->StopCurrentStateExecution();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

