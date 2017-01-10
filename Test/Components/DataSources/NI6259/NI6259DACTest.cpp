/**
 * @file NI6259DACTest.cpp
 * @brief Source file for class NI6259DACTest
 * @date 29/11/2016
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
 * the class NI6259DACTest (public, protected, and private). Be aware that some
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
#include "NI6259DAC.h"
#include "NI6259DACTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class NI6259DACTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

NI6259DACTestGAM    () : GAM() {
        voltage = 1;
        signalPtr = NULL;
        totalSamples = 0;
    }

    ~NI6259DACTestGAM() {
    }

    bool Execute() {
        using namespace MARTe;
        *signalPtr = voltage;
        totalSamples++;
        return true;
    }

    bool Setup() {
        using namespace MARTe;
        signalPtr = reinterpret_cast<float *>(GetOutputSignalMemory(0));
        return true;
    }

    MARTe::float32 voltage;
    MARTe::float32 *signalPtr;
    MARTe::uint32 totalSamples;

};
CLASS_REGISTER(NI6259DACTestGAM, "1.0")

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
    ReferenceT<NI6259DACTestGAM> testGAM;
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

static bool PatchConfiguration(const MARTe::char8 * const config1, const MARTe::char8 * const absoluteParameterLocation,
                               const MARTe::char8 * const parameterToPatch, const MARTe::char8 * const valueToPatch, pxi6259_ao_conf_t *conf) {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute(absoluteParameterLocation);
    cdb.Delete(parameterToPatch);
    if (valueToPatch != NULL) {
        cdb.Write(parameterToPatch, valueToPatch);
    }
    cdb.MoveToRoot();

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
    ReferenceT<NI6259DAC> ni6259DAC = application->Find("Data.NI6259_0");
    if (ok) {
        ok = ni6259DAC.IsValid();
    }
    if (ok) {
        ni6259DAC->ReadAOConfiguration(conf);
    }
    god->Purge();

    return ok;
}

//One channel base configuration
static const MARTe::char8 * const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DAC"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            ClockUpdateSource = \"UI_TC\""
        "            ClockUpdatePolarity = \"RISING_EDGE\""
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputPolarity = Bipolar"
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

//Four channels base configuration
static const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                    Trigger = 1"
        "                }"
        "                DAC1_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                }"
        "                DAC2_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                }"
        "                DAC3_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DAC"
        "            DeviceName = \"/dev/pxi6259\""
        "            ClockUpdateSource = \"UI_TC\""
        "            ClockUpdatePolarity = \"RISING_EDGE\""
        "            BoardId = 0"
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputPolarity = Bipolar"
        "                }"
        "                DAC1_0 = {"
        "                   Type = float32"
        "                   ChannelId = 1"
        "                   OutputPolarity = Bipolar"
        "                }"
        "                DAC2_0 = {"
        "                   Type = float32"
        "                   ChannelId = 2"
        "                   OutputPolarity = Bipolar"
        "                }"
        "                DAC3_0 = {"
        "                   Type = float32"
        "                   ChannelId = 3"
        "                   OutputPolarity = Bipolar"
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

//One channel base configuration with no signals defined
static const MARTe::char8 * const config3 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DAC"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            ClockUpdateSource = \"UI_TC\""
        "            ClockUpdatePolarity = \"RISING_EDGE\""
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputPolarity = Bipolar"
        "                }"
        "            }"
        "        }"
        "        +NI6259_1 = {"
        "            Class = NI6259DAC"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 1"
        "            ClockUpdateSource = \"UI_TC\""
        "            ClockUpdatePolarity = \"RISING_EDGE\""
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputPolarity = Bipolar"
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

//One channel base configuration with a wrong signal type
static const MARTe::char8 * const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                    Samples = 1"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DAC"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            ClockUpdateSource = \"UI_TC\""
        "            ClockUpdatePolarity = \"RISING_EDGE\""
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = uint32"
        "                   ChannelId = 0"
        "                   OutputPolarity = Bipolar"
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

//One channel base configuration with more than one sample
static const MARTe::char8 * const config5 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 2"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DAC"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            ClockUpdateSource = \"UI_TC\""
        "            ClockUpdatePolarity = \"RISING_EDGE\""
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputPolarity = Bipolar"
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

//One channel base configuration with a wrong device name
static const MARTe::char8 * const config6 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DAC"
        "            DeviceName = \"/tmp/pxi6259\""
        "            BoardId = 0"
        "            ClockUpdateSource = \"UI_TC\""
        "            ClockUpdatePolarity = \"RISING_EDGE\""
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputPolarity = Bipolar"
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

//Two channel base configuration with two GAMs
static const MARTe::char8 * const config7 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = NI6259DACTestGAM"
        "            OutputSignals = {"
        "                DAC1_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DAC"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            ClockUpdateSource = \"UI_TC\""
        "            ClockUpdatePolarity = \"RISING_EDGE\""
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputPolarity = Bipolar"
        "                }"
        "                DAC1_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputPolarity = Bipolar"
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

//One channel base configuration without specifying triggers
static const MARTe::char8 * const config8 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259DAC"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            ClockUpdateSource = \"UI_TC\""
        "            ClockUpdatePolarity = \"RISING_EDGE\""
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputPolarity = Bipolar"
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
bool NI6259DACTest::TestConstructor() {
    using namespace MARTe;
    NI6259DAC ni6259DAC;
    return true;
}

bool NI6259DACTest::TestAllocateMemory() {
    using namespace MARTe;
    NI6259DAC ni6259DAC;
    return ni6259DAC.AllocateMemory();
}

bool NI6259DACTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    NI6259DAC ni6259DAC;
    return (ni6259DAC.GetNumberOfMemoryBuffers() == 1u);
}

bool NI6259DACTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    NI6259DAC test;
    float32 *dac;
    bool ok = test.GetSignalMemoryBuffer(0, 0, (void *&) dac);
    ok &= (dac != NULL);
    ok = test.GetSignalMemoryBuffer(1, 0, (void *&) dac);
    ok &= (dac != NULL);
    ok = test.GetSignalMemoryBuffer(2, 0, (void *&) dac);
    ok &= (dac != NULL);
    ok = test.GetSignalMemoryBuffer(3, 0, (void *&) dac);
    ok &= (dac != NULL);
    return ok;
}

bool NI6259DACTest::TestGetSignalMemoryBuffer_False() {
    using namespace MARTe;
    NI6259DAC test;
    uint32 *ptr;
    return !test.GetSignalMemoryBuffer(NI6259DAC_MAX_CHANNELS, 0, (void *&) ptr);
}

bool NI6259DACTest::TestGetBrokerName() {
    using namespace MARTe;
    NI6259DAC test;
    ConfigurationDatabase config;
    StreamString brokerName = test.GetBrokerName(config, InputSignals);
    bool ok = (brokerName == "");
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

bool NI6259DACTest::TestGetInputBrokers() {
    using namespace MARTe;
    NI6259DAC test;
    ReferenceContainer rc;
    return !test.GetInputBrokers(rc, "", NULL);
}

bool NI6259DACTest::TestGetOutputBrokers() {
    return TestIntegratedInApplication(config1);
}

bool NI6259DACTest::TestGetOutputBrokers_NoTriggerGAM() {
    return TestIntegratedInApplication(config7);
}

bool NI6259DACTest::TestSynchronise() {
    return TestIntegratedInApplication(config1);
}

bool NI6259DACTest::TestPrepareNextState() {
    using namespace MARTe;
    NI6259DAC test;
    return test.PrepareNextState("", "");
}

bool NI6259DACTest::TestInitialise() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    NI6259DAC test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DACTest::TestInitialise_False_NoDeviceName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("DeviceName");
    NI6259DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DACTest::TestInitialise_False_NoBoardId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("BoardId");
    NI6259DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DACTest::TestInitialise_False_NoSignals() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("Signals");
    NI6259DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DACTest::TestInitialise_ClockUpdateSource() {
    using namespace MARTe;
    bool ok = true;
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "UI_TC", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_UI_TC);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI0", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI0);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI1", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI1);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI2", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI2);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI3", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI3);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI4", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI4);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI5", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI5);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI6", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI6);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI7", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI7);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI8", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI8);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI9", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI9);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI10", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI10);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI11", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI11);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI12", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI12);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI13", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI13);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI14", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI14);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "PFI15", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_PFI15);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "RTSI0", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_RTSI0);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "RTSI1", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_RTSI1);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "RTSI2", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_RTSI2);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "RTSI3", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_RTSI3);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "RTSI4", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_RTSI4);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "RTSI5", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_RTSI5);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "RTSI6", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_RTSI6);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "RTSI7", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_RTSI7);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "GPCRT0_OUT", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_GPCTR0_Out);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "STAR_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_STAR_TRIGEGR);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "GPCTR1_OUT", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_GPCTR1_Out);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "ANALOG_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_ANALOG_TRIGGER);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateSource", "LOW", &conf);
        }
        if (ok) {
            ok = (conf.updateSource == AO_UPDATE_SOURCE_SELECT_LOW);
        }
    }

    return ok;
}

bool NI6259DACTest::TestInitialise_False_NoClockUpdateSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("ClockUpdateSource");
    NI6259DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DACTest::TestInitialise_False_BadClockUpdateSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("ClockUpdateSource");
    cdb.Write("ClockUpdateSource", "INVALID");
    NI6259DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DACTest::TestInitialise_ClockUpdatePolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdatePolarity", "RISING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.updatePolarity == AO_UPDATE_SOURCE_POLARITY_RISING_EDGE);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdatePolarity", "FALLING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.updatePolarity == AO_UPDATE_SOURCE_POLARITY_FALLING_EDGE);
        }
    }

    return ok;
}

bool NI6259DACTest::TestInitialise_False_NoClockUpdatePolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("ClockUpdatePolarity");
    NI6259DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DACTest::TestInitialise_False_BadClockUpdatePolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("ClockUpdatePolarity");
    cdb.Write("ClockUpdatePolarity", "INVALID");
    NI6259DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DACTest::TestInitialise_ClockUpdateDivisor() {
    using namespace MARTe;
    bool ok = true;
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateDivisor", "8", &conf);
        }
        if (ok) {
            ok = (conf.updatePeriodDivisor == 8);
        }
    }

    return ok;
}

bool NI6259DACTest::TestInitialise_DefaultClockUpdateDivisor() {
    using namespace MARTe;
    bool ok = true;
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockUpdateDivisor", NULL, &conf);
        }
        if (ok) {
            ok = (conf.updatePeriodDivisor == 10);
        }
    }

    return ok;
}

bool NI6259DACTest::TestInitialise_OutputPolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.DAC0_0", "OutputPolarity", "Unipolar", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].polarity == AO_DAC_POLARITY_UNIPOLAR);
        }
    }
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.DAC0_0", "OutputPolarity", "Bipolar", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].polarity == AO_DAC_POLARITY_BIPOLAR);
        }
    }

    return ok;
}

bool NI6259DACTest::TestInitialise_False_BadOutputPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0.Signals.DAC0_0");
    cdb.Delete("OutputPolarity");
    cdb.Write("OutputPolarity", "INVALID");
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    NI6259DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DACTest::TestInitialise_DefaultOutputPolarity() {
    bool ok = true;
    {
        pxi6259_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.DAC0_0", "OutputPolarity", NULL, &conf);
        }
        if (ok) {
            ok = (conf.channel[0].polarity == AO_DAC_POLARITY_UNIPOLAR);
        }
    }
    return ok;
}

bool NI6259DACTest::TestInitialise_False_BadChannelId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0.Signals.DAC0_0");
    cdb.Delete("ChannelId");
    cdb.Write("ChannelId", "4");
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    NI6259DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259DACTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config1);
}

bool NI6259DACTest::TestSetConfiguredDatabase_AllChannels() {
    return TestIntegratedInApplication(config2);
}

bool NI6259DACTest::TestSetConfiguredDatabase_AllChannels_Parameters() {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config2;
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

    ReferenceT<NI6259DAC> ni6259DAC = application->Find("Data.NI6259_0");
    if (ok) {
        ok = ni6259DAC.IsValid();
    }
    if (ok) {
        pxi6259_ao_conf_t conf;
        ni6259DAC->ReadAOConfiguration(&conf);
        uint32 i;
        for (i = 0; (i < NI6259DAC_MAX_CHANNELS) && (ok); i++) {
            ok &= (conf.channel[i].polarity == AO_DAC_POLARITY_BIPOLAR);
            ok &= (conf.channel[i].enabled);
        }
    }

    god->Purge();
    return ok;
}

bool NI6259DACTest::TestSetConfiguredDatabase_False_NoChannels() {
    return !TestIntegratedInApplication(config3);
}

bool NI6259DACTest::TestSetConfiguredDatabase_False_SignalTypeMismatch() {
    return !TestIntegratedInApplication(config4);
}

bool NI6259DACTest::TestSetConfiguredDatabase_False_SignalNumberOfSamplesGreaterThanOne() {
    return !TestIntegratedInApplication(config5);
}

bool NI6259DACTest::TestSetConfiguredDatabase_False_WrongDeviceName() {
    return !TestIntegratedInApplication(config6);
}

bool NI6259DACTest::TestSetConfiguredDatabase_False_NoTriggers() {
    return !TestIntegratedInApplication(config8);
}

