/**
 * @file NI6368DACTest.cpp
 * @brief Source file for class NI6368DACTest
 * @date 23/01/2017
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
 * the class NI6368DACTest (public, protected, and private). Be aware that some
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
#include "NI6368DAC.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "NI6368DACTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class NI6368DACTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

NI6368DACTestGAM    () : GAM() {
        voltage = 1;
        signalPtr = NULL;
        totalSamples = 0;
    }

    ~NI6368DACTestGAM() {
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
CLASS_REGISTER(NI6368DACTestGAM, "1.0")

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
    ReferenceT<NI6368DACTestGAM> testGAM;
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
                               const MARTe::char8 * const parameterToPatch, const MARTe::char8 * const valueToPatch, xseries_ao_conf_t *conf) {
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
    ReferenceT<NI6368DAC> ni6368DAC = application->Find("Data.NI6368_0");
    if (ok) {
        ok = ni6368DAC.IsValid();
    }
    if (ok) {
        ni6368DAC->ReadAOConfiguration(conf);
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
        "            Class = NI6368DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = float32"
        "                    Samples = 1000"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368DAC"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputRange = 10"
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
        "            Class = NI6368DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                    Trigger = 1"
        "                }"
        "                DAC1_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                }"
        "                DAC2_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                }"
        "                DAC3_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368DAC"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputRange = 10"
        "                }"
        "                DAC1_0 = {"
        "                   Type = float32"
        "                   ChannelId = 1"
        "                   OutputRange = 10"
        "                }"
        "                DAC2_0 = {"
        "                   Type = float32"
        "                   ChannelId = 2"
        "                   OutputRange = 10"
        "                }"
        "                DAC3_0 = {"
        "                   Type = float32"
        "                   ChannelId = 3"
        "                   OutputRange = 10"
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
        "            Class = NI6368DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6368_0"
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
        "        +NI6368_0 = {"
        "            Class = NI6368DAC"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputRange = 10"
        "                }"
        "            }"
        "        }"
        "        +NI6368_1 = {"
        "            Class = NI6368DAC"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 1"
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputRange = 10"
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
        "            Class = NI6368DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6368_0"
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
        "        +NI6368_0 = {"
        "            Class = NI6368DAC"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = uint32"
        "                   ChannelId = 0"
        "                   OutputRange = 10"
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
        "            Class = NI6368DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6368_0"
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
        "        +NI6368_0 = {"
        "            Class = NI6368DAC"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputRange = 10"
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
        "            Class = NI6368DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6368_0"
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
        "        +NI6368_0 = {"
        "            Class = NI6368DAC"
        "            DeviceName = \"/tmp/pxie-6368\""
        "            BoardId = 0"
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputRange = 10"
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
        "            Class = NI6368DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = NI6368DACTestGAM"
        "            OutputSignals = {"
        "                DAC1_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368DAC"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputRange = 10"
        "                }"
        "                DAC1_0 = {"
        "                   Type = float32"
        "                   ChannelId = 1"
        "                   OutputRange = 10"
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
        "            Class = NI6368DACTestGAM"
        "            OutputSignals = {"
        "                DAC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368DAC"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   OutputRange = 10"
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
bool NI6368DACTest::TestConstructor() {
    using namespace MARTe;
    NI6368DAC ni6368DAC;
    return true;
}

bool NI6368DACTest::TestAllocateMemory() {
    using namespace MARTe;
    NI6368DAC ni6368DAC;
    return ni6368DAC.AllocateMemory();
}

bool NI6368DACTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    NI6368DAC ni6368DAC;
    return (ni6368DAC.GetNumberOfMemoryBuffers() == 1u);
}

bool NI6368DACTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    NI6368DAC test;
    float32 *dac;
    bool ok = test.GetSignalMemoryBuffer(0, 0, (void *&) dac);
    ok &= (dac == NULL);
    ok = test.GetSignalMemoryBuffer(1, 0, (void *&) dac);
    ok &= (dac == NULL);
    ok = test.GetSignalMemoryBuffer(2, 0, (void *&) dac);
    ok &= (dac == NULL);
    ok = test.GetSignalMemoryBuffer(3, 0, (void *&) dac);
    ok &= (dac == NULL);
    return ok;
}

bool NI6368DACTest::TestGetSignalMemoryBuffer_False() {
    using namespace MARTe;
    NI6368DAC test;
    uint32 *ptr;
    return !test.GetSignalMemoryBuffer(NI6368DAC_MAX_CHANNELS, 0, (void *&) ptr);
}

bool NI6368DACTest::TestGetBrokerName() {
    using namespace MARTe;
    NI6368DAC test;
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

bool NI6368DACTest::TestGetInputBrokers() {
    using namespace MARTe;
    NI6368DAC test;
    ReferenceContainer rc;
    return !test.GetInputBrokers(rc, "", NULL);
}

bool NI6368DACTest::TestGetOutputBrokers() {
    return TestIntegratedInApplication(config1);
}

bool NI6368DACTest::TestGetOutputBrokers_NoTriggerGAM() {
    return TestIntegratedInApplication(config7);
}

bool NI6368DACTest::TestSynchronise() {
    return TestIntegratedInApplication(config1);
}

bool NI6368DACTest::TestPrepareNextState() {
    using namespace MARTe;
    NI6368DAC test;
    return test.PrepareNextState("", "");
}

bool NI6368DACTest::TestInitialise() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    NI6368DAC test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_NoDeviceName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("DeviceName");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_NoBoardId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("BoardId");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_NoSignals() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("Signals");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_OutputRange() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.DAC0_0", "OutputRange", "10", &conf);
        }
        if (ok) {
            ok = (conf.aochan_conf[0].range == XSERIES_OUTPUT_RANGE_10V);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.DAC0_0", "OutputRange", "5", &conf);
        }
        if (ok) {
            ok = (conf.aochan_conf[0].range == XSERIES_OUTPUT_RANGE_5V);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.DAC0_0", "OutputRange", "APFI0", &conf);
        }
        if (ok) {
            ok = (conf.aochan_conf[0].range == XSERIES_OUTPUT_RANGE_APFI0);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.DAC0_0", "OutputRange", "APFI1", &conf);
        }
        if (ok) {
            ok = (conf.aochan_conf[0].range == XSERIES_OUTPUT_RANGE_APFI1);
        }
    }

    return ok;
}

bool NI6368DACTest::TestInitialise_False_BadOutputRange() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0.Signals.DAC0_0");
    cdb.Delete("OutputRange");
    cdb.Write("OutputRange", "INVALID");
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_DefaultOutputRange() {
    bool ok = true;
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.DAC0_0", "OutputRange", NULL, &conf);
        }
        if (ok) {
            ok = (conf.aochan_conf[0].range == XSERIES_OUTPUT_RANGE_10V);
        }
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_BadChannelId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0.Signals.DAC0_0");
    cdb.Delete("ChannelId");
    cdb.Write("ChannelId", "4");
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config1);
}

bool NI6368DACTest::TestSetConfiguredDatabase_AllChannels() {
    return TestIntegratedInApplication(config2);
}

bool NI6368DACTest::TestSetConfiguredDatabase_AllChannels_Parameters() {
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

    ReferenceT<NI6368DAC> ni6368DAC = application->Find("Data.NI6368_0");
    if (ok) {
        ok = ni6368DAC.IsValid();
    }
    if (ok) {
        xseries_ao_conf_t conf;
        ni6368DAC->ReadAOConfiguration(&conf);
        uint32 i;
        for (i = 0; (i < NI6368DAC_MAX_CHANNELS) && (ok); i++) {
            ok &= (conf.aochan_conf[i].range == XSERIES_OUTPUT_RANGE_10V);
            ok &= (conf.aochan_conf[i].is_enabled);
        }
    }

    god->Purge();
    return ok;
}

bool NI6368DACTest::TestSetConfiguredDatabase_False_NoChannels() {
    return !TestIntegratedInApplication(config3);
}

bool NI6368DACTest::TestSetConfiguredDatabase_False_SignalTypeMismatch() {
    return !TestIntegratedInApplication(config4);
}

bool NI6368DACTest::TestSetConfiguredDatabase_False_WrongDeviceName() {
    return !TestIntegratedInApplication(config6);
}

bool NI6368DACTest::TestSetConfiguredDatabase_False_NoTriggers() {
    return !TestIntegratedInApplication(config8);
}

