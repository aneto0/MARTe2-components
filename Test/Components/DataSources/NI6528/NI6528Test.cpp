/**
 * @file NI6528Test.cpp
 * @brief Source file for class NI6528Test
 * @date Mar 2, 2026 TODO Verify the value and format of the date
 * @author ferrog TODO Verify the name and format of the author
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
 * the class NI6528Test (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "BrokerI.h"
#include "NI6528Test.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class NI6528TestGAM: public GAM {
public:CLASS_REGISTER_DECLARATION()

    NI6528TestGAM() :
            GAM() {
        for (uint32 i = 0u; i < 3u; i++) {
            outputPorts[i] = 0u;
            inputPorts[i] = 0u;
        }
    }

    ~NI6528TestGAM() {
    }

    bool Execute() {
        for (uint32 i = 0u; i < 3u; i++) {
            *(outputPorts[i]) = *(inputPorts[i]);
        }
        return true;
    }

    bool Setup() {
        inputPorts[0] = reinterpret_cast<uint8*>(GetInputSignalMemory(0));
        inputPorts[1] = reinterpret_cast<uint8*>(GetInputSignalMemory(1));
        inputPorts[2] = reinterpret_cast<uint8*>(GetInputSignalMemory(2));

        outputPorts[0] = reinterpret_cast<uint8*>(GetOutputSignalMemory(0));
        outputPorts[1] = reinterpret_cast<uint8*>(GetOutputSignalMemory(1));
        outputPorts[2] = reinterpret_cast<uint8*>(GetOutputSignalMemory(2));

        return true;
    }

    uint8 GetInput(uint8 index) {
        uint8 port = index / 8u;
        uint8 pin = index % 8u;
        return (((*inputPorts[port]) >> pin) & (0x1u));
    }

    uint8 GetOutput(uint8 index) {
        uint8 port = index / 8u;
        uint8 pin = index % 8u;
        return (((*outputPorts[port]) >> pin) & (0x1u));
    }

    uint8 *inputPorts[3];
    uint8 *outputPorts[3];

};
CLASS_REGISTER(NI6528TestGAM, "1.0")

bool NI6528Test::TestConstructor() {
    NI6528 *test = new NI6528();
    bool ret = (test != NULL);
    delete test;
    return ret;
}

static const MARTe::char8 *const config1_full = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6528TestGAM"
        "            InputSignals = {"
        "                Port0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint8"
        "                    Trigger = 1"
        "                }"
        "                Port1 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint8"
        "                }"
        "                Port2 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint8"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Port3 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint8"
        "                }"
        "                Port4 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint8"
        "                }"
        "                Port5 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint8"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6528"
        "            DeviceName = \"/dev/pxi6528\""
        "            BoardId = 0"
        "            Signals = {"
        "                Port0 = {"
        "                    PortId = 0"
        "                    Type = uint8"
        "                    InversionMask = 0xF"
        "                }"
        "                Port1 = {"
        "                    PortId = 1"
        "                    Type = uint8"
        "                }"
        "                Port2 = {"
        "                    PortId = 2"
        "                    Type = uint8"
        "                }"
        "                Port3 = {"
        "                    PortId = 3"
        "                    Type = uint8"
        "                }"
        "                Port4 = {"
        "                    PortId = 4"
        "                    Type = uint8"
        "                }"
        "                Port5 = {"
        "                    PortId = 5"
        "                    Type = uint8"
        "                    InversionMask = 0xF"
        "                }"
        "            }"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

bool NI6528Test::TestInitialise() {
    NI6528 test;

    ConfigurationDatabase cdb;
    StreamString configStream = config1_full;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
        if (ok) {
            ok = test.Initialise(cdb);
        }

    }
    return ok;
}

bool NI6528Test::TestSetConfiguredDatabase() {
    ConfigurationDatabase cdb;
    StreamString configStream = config1_full;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}

bool NI6528Test::TestSynchronise() {

    ConfigurationDatabase cdb;
    StreamString configStream = config1_full;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    ReferenceT<NI6528TestGAM> gam;
    if (ok) {
        gam = application->Find("Functions.GAMA");
        ok = gam.IsValid();
    }

    if (ok) {
        ReferenceContainer inputBrokers;
        ReferenceContainer outputBrokers;
        gam->GetInputBrokers(inputBrokers);
        gam->GetOutputBrokers(outputBrokers);
        for (uint32 i = 0u; i < inputBrokers.Size(); i++) {
            ReferenceT<BrokerI> inputBroker = inputBrokers.Get(i);
            if (inputBroker.IsValid()) {
                ok &= inputBroker->Execute();
            }
        }
        ok &= gam->Execute();
        for (uint32 i = 0u; i < outputBrokers.Size(); i++) {
            ReferenceT<BrokerI> outputBroker = outputBrokers.Get(i);
            if (outputBroker.IsValid()) {
                ok &= outputBroker->Execute();
            }
        }
    }
    if (ok) {
        for (uint32 i = 0u; i < 24u; i++) {
            ok &= (gam->GetInput(i) == 0u) || (gam->GetInput(i) == 1u);
        }
    }

    return ok;
}

bool NI6528Test::TestAllocateMemory() {
    return true;
}

bool NI6528Test::TestGetSignalMemoryBuffer() {
    bool ok = true;
    void *ptr = NULL;
    NI6528 test;
    for (uint32 i = 0u; i < 6u; i++) {
        ok &= test.GetSignalMemoryBuffer(i, 0u, (void*&) ptr);
    }

    if (ok) {
        ok = !test.GetSignalMemoryBuffer(6u, 0u, (void*&) ptr);
    }
    return ok;
}

bool NI6528Test::TestGetBrokerName() {

    ConfigurationDatabase cdb;
    NI6528 test;
    StreamString brok = test.GetBrokerName(cdb, InputSignals);
    bool ok = (brok == "MemoryMapInputBroker");
    if (ok) {
        brok = test.GetBrokerName(cdb, OutputSignals);
        ok = (brok == "MemoryMapOutputBroker");
    }
    cdb.Write("Trigger", 1u);
    if (ok) {
        brok = test.GetBrokerName(cdb, InputSignals);
        ok = (brok == "MemoryMapSynchronisedInputBroker");
    }
    if (ok) {
        brok = test.GetBrokerName(cdb, OutputSignals);
        ok = (brok == "MemoryMapSynchronisedOutputBroker");
    }
    cdb.Delete("Trigger");
    cdb.Write("Frequency", 0u);
    if (ok) {
        brok = test.GetBrokerName(cdb, InputSignals);
        ok = (brok == "MemoryMapSynchronisedInputBroker");
    }
    if (ok) {
        brok = test.GetBrokerName(cdb, OutputSignals);
        ok = (brok == "MemoryMapSynchronisedOutputBroker");
    }
    cdb.Delete("Frequency");

    return ok;
}

bool NI6528Test::TestPrepareNextState() {
    return true;
}

