/**
 * @file NI6259DIOTest.cpp
 * @brief Source file for class NI6259DIOTest
 * @date 9/1/2017
 * @author Andre Neto
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
 * the class NI6259DIOTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "NI6259DIO.h"
#include "NI6259DIOTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class NI6259DIOTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()NI6259DIOTestGAM () : GAM() {
        inSignalPtr = NULL;
        outSignalPtr = NULL;
        totalSamples = 0;
    }

    ~NI6259DIOTestGAM() {
    }

    bool Execute() {
        using namespace MARTe;
        *outSignalPtr = *inSignalPtr;
        totalSamples++;
        return true;
    }

    bool Setup() {
        using namespace MARTe;
        inSignalPtr = reinterpret_cast<uint32 *>(GetInputSignalMemory(0));
        outSignalPtr = reinterpret_cast<uint32 *>(GetOutputSignalMemory(0));
        return true;
    }

    MARTe::uint32 *inSignalPtr;
    MARTe::uint32 *outSignalPtr;
    MARTe::uint32 totalSamples;

};
CLASS_REGISTER(NI6259DIOTestGAM, "1.0")

static bool TestIntegratedInApplication(const MARTe::char8 * const config1) {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config1;
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
    ReferenceT<NI6259DIOTestGAM> testGAM;
    if (ok) {
        testGAM = application->Find("Functions.GAMA");
        ok = testGAM.IsValid();
    }
    ReferenceT<GAMScheduler> scheduler;
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        application->PrepareNextState("State1");

        application->StartNextStateExecution();
        uint32 timeout = 10;
        while (testGAM->totalSamples < 1) {
            if (timeout == 0) {
                ok = false;
                break;
            }
            timeout--;
            Sleep::MSec(500);
        }
        application->StopCurrentStateExecution();
    }
    god->Purge();
    return ok;
}

//One port base configuration
static const MARTe::char8 * const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DIOTestGAM"
        "            InputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DIO"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            Signals = {"
        "                PORT0_0 = {"
        "                   Type = uint32"
        "                   PortId = 0"
        "                   Mask = 0x1"
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

//Two ports configuration. One port writing with no trigger.
static const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DIOTestGAM"
        "            InputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = NI6259DIOTestGAM"
        "            InputSignals = {"
        "                PORT1_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT1_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DIO"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            Signals = {"
        "                PORT0_0 = {"
        "                   Type = uint32"
        "                   PortId = 0"
        "                   Mask = 0x1"
        "                }"
        "                PORT1_0 = {"
        "                   Type = uint32"
        "                   PortId = 1"
        "                   Mask = 0x3"
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
        "                    Functions = {GAMA GAMB}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Three ports configuration. One port writing with no trigger.
static const MARTe::char8 * const config3 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DIOTestGAM"
        "            InputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = NI6259DIOTestGAM"
        "            InputSignals = {"
        "                PORT1_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "                PORT2_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT1_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                    Trigger = 1"
        "                }"
        "                PORT2_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DIO"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            Signals = {"
        "                PORT0_0 = {"
        "                   Type = uint32"
        "                   PortId = 0"
        "                   Mask = 0x5"
        "                }"
        "                PORT1_0 = {"
        "                   Type = uint32"
        "                   PortId = 1"
        "                   Mask = 0x5"
        "                }"
        "                PORT2_0 = {"
        "                   Type = uint32"
        "                   PortId = 2"
        "                   Mask = 0x5"
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
        "                    Functions = {GAMA GAMB}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Configuration with no channels.
static const MARTe::char8 * const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DIOTestGAM"
        "            InputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DIO"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            Signals = {"
        "                PORT0_0 = {"
        "                   Type = uint32"
        "                   PortId = 0"
        "                   Mask = 0x5"
        "                }"
        "            }"
        "        }"
        "        +NI6259_1 = {"
        "            Class = NI6259DIO"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 1"
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
        "                    Functions = {GAMA GAMB}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//One port base configuration with signal type mismatch
static const MARTe::char8 * const config5 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DIOTestGAM"
        "            InputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DIO"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            Signals = {"
        "                PORT0_0 = {"
        "                   Type = float32"
        "                   PortId = 0"
        "                   Mask = 0x1"
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

//One port base configuration with more than one sample
static const MARTe::char8 * const config6 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DIOTestGAM"
        "            InputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                    Samples = 2"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DIO"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            Signals = {"
        "                PORT0_0 = {"
        "                   Type = uint32"
        "                   PortId = 0"
        "                   Mask = 0x1"
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

//One port base configuration with a wrong device name
static const MARTe::char8 * const config7 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DIOTestGAM"
        "            InputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DIO"
        "            DeviceName = \"/tmp/pxi6259\""
        "            BoardId = 0"
        "            Signals = {"
        "                PORT0_0 = {"
        "                   Type = uint32"
        "                   PortId = 0"
        "                   Mask = 0x1"
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

//One port base configuration without specifying a triggering signal
static const MARTe::char8 * const config8 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DIOTestGAM"
        "            InputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DIO"
        "            DeviceName = \"/tmp/pxi6259\""
        "            BoardId = 0"
        "            Signals = {"
        "                PORT0_0 = {"
        "                   Type = uint32"
        "                   PortId = 0"
        "                   Mask = 0x1"
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


//One port base configuration with only inputs
static const MARTe::char8 * const config9 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DIOTestGAM"
        "            InputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DIO"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            Signals = {"
        "                PORT0_0 = {"
        "                   Type = uint32"
        "                   PortId = 0"
        "                   Mask = 0x1"
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

//One port base configuration with only outputs
static const MARTe::char8 * const config10 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DIOTestGAM"
        "            OutputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DIO"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            Signals = {"
        "                PORT0_0 = {"
        "                   Type = uint32"
        "                   PortId = 0"
        "                   Mask = 0x1"
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
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool NI6259DIOTest::TestConstructor() {
    using namespace MARTe;
    NI6259DIO ni6259DIO;
    return true;
}

bool NI6259DIOTest::TestAllocateMemory() {
    using namespace MARTe;
    NI6259DIO ni6259DIO;
    return ni6259DIO.AllocateMemory();
}

bool NI6259DIOTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    NI6259DIO ni6259DIO;
    return (ni6259DIO.GetNumberOfMemoryBuffers() == 1u);
}

bool NI6259DIOTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    NI6259DIO test;
    uint32 *dio0;
    bool ok = test.GetSignalMemoryBuffer(0, 0, (void *&) dio0);
    ok &= (dio0 != NULL);
    return ok;
}

bool NI6259DIOTest::TestGetSignalMemoryBuffer_False() {
    using namespace MARTe;
    NI6259DIO test;
    uint32 *ptr;
    return !test.GetSignalMemoryBuffer(PXI6259_NUMBER_OF_PORTS, 0, (void *&) ptr);
}

bool NI6259DIOTest::TestGetBrokerName() {
    using namespace MARTe;
    NI6259DIO test;
    ConfigurationDatabase config;
    StreamString brokerName = test.GetBrokerName(config, InputSignals);
    bool ok = (brokerName == "MemoryMapSynchronisedInputBroker");
    if (ok) {
        brokerName = test.GetBrokerName(config, OutputSignals);
        ok = (brokerName == "MemoryMapOutputBroker");
    }
    if (ok) {
        config.Write("Trigger", 1);
        brokerName = test.GetBrokerName(config, OutputSignals);
        ok = (brokerName == "MemoryMapSynchronisedOutputBroker");
    }

    return ok;
}

bool NI6259DIOTest::TestGetInputBrokers() {
    return TestIntegratedInApplication(config1);
}

bool NI6259DIOTest::TestGetOutputBrokers() {
    return TestIntegratedInApplication(config1);
}

bool NI6259DIOTest::TestGetOutputBrokers_NoTriggerGAM() {
    return TestIntegratedInApplication(config2);
}

bool NI6259DIOTest::TestSynchronise() {
    return TestIntegratedInApplication(config1);
}

bool NI6259DIOTest::TestPrepareNextState() {
    using namespace MARTe;
    NI6259DIO test;
    return test.PrepareNextState("", "");
}

bool NI6259DIOTest::TestInitialise() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    NI6259DIO test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DIOTest::TestInitialise_False_NoDeviceName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("DeviceName");
    NI6259DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DIOTest::TestInitialise_False_NoBoardId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("BoardId");
    NI6259DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DIOTest::TestInitialise_False_NoSignals() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("Signals");
    NI6259DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DIOTest::TestInitialise_False_BadPortId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0.Signals.PORT0_0");
    cdb.Delete("PortId");
    cdb.Write("PortId", "3");
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    NI6259DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DIOTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config1);
}

bool NI6259DIOTest::TestSetConfiguredDatabase_OnlyInputs() {
    return TestIntegratedInApplication(config1);
}

bool NI6259DIOTest::TestSetConfiguredDatabase_OnlyOutputs() {
    return TestIntegratedInApplication(config1);
}

bool NI6259DIOTest::TestSetConfiguredDatabase_AllChannels() {
    return TestIntegratedInApplication(config3);
}

bool NI6259DIOTest::TestSetConfiguredDatabase_AllChannels_Parameters() {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config3;
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

    ReferenceT<NI6259DIO> ni6259DIO = application->Find("Data.NI6259_0");
    if (ok) {
        ok = ni6259DIO.IsValid();
    }
    if (ok) {
        pxi6259_dio_conf_t conf;
        ni6259DIO->ReadDIOConfiguration(&conf);
        uint32 i;
        for (i = 0; (i < PXI6259_NUMBER_OF_PORTS) && (ok); i++) {
            ok &= (conf.channel[i].mask == 0x5);
            ok &= (conf.channel[i].enabled);
        }
    }

    god->Purge();
    return ok;
}

bool NI6259DIOTest::TestSetConfiguredDatabase_False_NoChannels() {
    return !TestIntegratedInApplication(config4);
}

bool NI6259DIOTest::TestSetConfiguredDatabase_False_SignalTypeMismatch() {
    return !TestIntegratedInApplication(config5);
}

bool NI6259DIOTest::TestSetConfiguredDatabase_False_SignalNumberOfSamplesGreaterThanOne() {
    return !TestIntegratedInApplication(config6);
}

bool NI6259DIOTest::TestSetConfiguredDatabase_False_WrongDeviceName() {
    return !TestIntegratedInApplication(config7);
}

bool NI6259DIOTest::TestSetConfiguredDatabase_False_NoTriggers() {
    return !TestIntegratedInApplication(config8);
}

