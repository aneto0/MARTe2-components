/**
 * @file NI6368DIOTest.cpp
 * @brief Source file for class NI6368DIOTest
 * @date 30/1/2017
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
 * the class NI6368DIOTest (public, protected, and private). Be aware that some
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
#include "NI6368DIO.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "NI6368DIOTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class NI6368DIOTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()NI6368DIOTestGAM () : GAM() {
        inSignalPtr = NULL;
        outSignalPtr = NULL;
        totalSamples = 0;
    }

    ~NI6368DIOTestGAM() {
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
CLASS_REGISTER(NI6368DIOTestGAM, "1.0")

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
    ReferenceT<NI6368DIOTestGAM> testGAM;
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
                               const MARTe::char8 * const parameterToPatch, const MARTe::char8 * const valueToPatch, xseries_di_conf_t *confDI,
                               xseries_do_conf_t *confDO) {
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
    ReferenceT<NI6368DIO> ni6368DIO = application->Find("Data.NI6368_0");
    if (ok) {
        ok = ni6368DIO.IsValid();
    }
    if (ok) {
        if (confDI != NULL) {
            if (confDO != NULL) {
                ni6368DIO->ReadDIOConfiguration(confDI, confDO);
            }
            else {
                xseries_do_conf_t ignore;
                ni6368DIO->ReadDIOConfiguration(confDI, &ignore);
            }
        }
        else {
            xseries_di_conf_t ignore;
            ni6368DIO->ReadDIOConfiguration(&ignore, confDO);
        }
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
        "            Class = NI6368DIOTestGAM"
        "            InputSignals = {"
        "                PORT_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint32"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368DIO"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            InputPortMask = 0x2"
        "            OutputPortMask = 0x1"
        "            ClockSampleSource = \"INTERNAL\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            StartTriggerSource = \"SW_PULSE\""
        "            StartTriggerPolarity = \"RISING_EDGE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
        "            Signals = {"
        "                PORT_0 = {"
        "                   Type = uint32"
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

//Configuration with no channels.
static const MARTe::char8 * const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368DIOTestGAM"
        "            InputSignals = {"
        "                PORT_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368DIO"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            InputPortMask = 0x2"
        "            OutputPortMask = 0x1"
        "            ClockSampleSource = \"INTERNAL\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            StartTriggerSource = \"SW_PULSE\""
        "            StartTriggerPolarity = \"RISING_EDGE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
        "            Signals = {"
        "                PORT_0 = {"
        "                   Type = uint32"
        "                }"
        "            }"
        "        }"
        "        +NI6368_1 = {"
        "            Class = NI6368DIO"
        "            DeviceName = \"/dev/pxie-6368\""
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
        "            Class = NI6368DIOTestGAM"
        "            InputSignals = {"
        "                PORT_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = float32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = float32"
        "                    Trigger = 1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368DIO"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            InputPortMask = 0x2"
        "            OutputPortMask = 0x1"
        "            ClockSampleSource = \"INTERNAL\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            StartTriggerSource = \"SW_PULSE\""
        "            StartTriggerPolarity = \"RISING_EDGE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
        "            Signals = {"
        "                PORT_0 = {"
        "                   Type = float32"
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
        "            Class = NI6368DIOTestGAM"
        "            InputSignals = {"
        "                PORT_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint32"
        "                    Samples = 2"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368DIO"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            InputPortMask = 0x2"
        "            OutputPortMask = 0x1"
        "            ClockSampleSource = \"INTERNAL\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            StartTriggerSource = \"SW_PULSE\""
        "            StartTriggerPolarity = \"RISING_EDGE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
        "            Signals = {"
        "                PORT_0 = {"
        "                   Type = uint32"
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
        "            Class = NI6368DIOTestGAM"
        "            InputSignals = {"
        "                PORT_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368DIO"
        "            DeviceName = \"/tmp/pxie-6368\""
        "            BoardId = 0"
        "            InputPortMask = 0x2"
        "            OutputPortMask = 0x1"
        "            ClockSampleSource = \"INTERNAL\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            StartTriggerSource = \"SW_PULSE\""
        "            StartTriggerPolarity = \"RISING_EDGE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
        "            Signals = {"
        "                PORT_0 = {"
        "                   Type = uint32"
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
bool NI6368DIOTest::TestConstructor() {
    using namespace MARTe;
    NI6368DIO ni6368DIO;
    return true;
}

bool NI6368DIOTest::TestAllocateMemory() {
    using namespace MARTe;
    NI6368DIO ni6368DIO;
    return ni6368DIO.AllocateMemory();
}

bool NI6368DIOTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    NI6368DIO ni6368DIO;
    return (ni6368DIO.GetNumberOfMemoryBuffers() == 1u);
}

bool NI6368DIOTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    NI6368DIO test;
    uint32 *dio0;
    bool ok = test.GetSignalMemoryBuffer(0, 0, (void *&) dio0);
    ok &= (dio0 != NULL);
    return ok;
}

bool NI6368DIOTest::TestGetSignalMemoryBuffer_False() {
    using namespace MARTe;
    NI6368DIO test;
    uint32 *ptr;
    return !test.GetSignalMemoryBuffer(1, 0, (void *&) ptr);
}

bool NI6368DIOTest::TestGetBrokerName() {
    using namespace MARTe;
    NI6368DIO test;
    ConfigurationDatabase config;
    StreamString brokerName = test.GetBrokerName(config, InputSignals);
    bool ok = (brokerName == "MemoryMapSynchronisedInputBroker");
    if (ok) {
        brokerName = test.GetBrokerName(config, OutputSignals);
        ok = (brokerName == "MemoryMapSynchronisedOutputBroker");
    }

    return ok;
}

bool NI6368DIOTest::TestGetInputBrokers() {
    return TestIntegratedInApplication(config1);
}

bool NI6368DIOTest::TestGetOutputBrokers() {
    return TestIntegratedInApplication(config1);
}

bool NI6368DIOTest::TestSynchronise() {
    return TestIntegratedInApplication(config1);
}

bool NI6368DIOTest::TestPrepareNextState() {
    using namespace MARTe;
    NI6368DIO test;
    return test.PrepareNextState("", "");
}

bool NI6368DIOTest::TestInitialise() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    NI6368DIO test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoDeviceName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("DeviceName");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoBoardId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("BoardId");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoInputPortMask() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("InputPortMask");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoOutputPortMask() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("OutputPortMask");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_ClockSampleSource() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTERNAL", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTERNAL);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI0", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI0);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI1", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI1);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI2", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI2);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI3", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI3);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI4", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI4);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI5", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI5);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI6", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI6);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI7", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI7);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI8", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI8);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI9", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI9);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI10", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI10);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI11", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI11);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI12", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI12);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI13", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI13);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI14", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI14);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI15", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI15);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI0", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI0);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI1", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI1);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI2", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI2);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI3", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI3);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI4", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI4);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI5", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI5);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI6", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI6);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI7", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI7);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "DIO_CHGDETECT", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_DIO_CHGDETECT);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G0_OUT", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_G0_OUT);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G1_OUT", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_G1_OUT);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G2_OUT", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_G2_OUT);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G3_OUT", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_G3_OUT);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "STAR_TRIGGER", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_STAR_TRIGGER);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "ATRIG", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_ATRIG);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "LOW", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_LOW);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PXIE_DSTARA", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PXIE_DSTARA);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PXIE_DSTARB", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_PXIE_DSTARB);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G0_SAMPLECLK", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_G0_SAMPLECLK);
        }
    }

    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G1_SAMPLECLK", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_G1_SAMPLECLK);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G2_SAMPLECLK", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_G2_SAMPLECLK);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G3_SAMPLECLK", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_G3_SAMPLECLK);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "AI_CONVERT", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_AI_CONVERT);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "AO_UPDATE", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_AO_UPDATE);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "DO_UPDATE", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_DO_UPDATE);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA0", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA0);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA1", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA1);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA2", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA2);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA3", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA3);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA4", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA4);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA5", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA5);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA6", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA6);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA7", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA7);
        }
    }

    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoClockSampleSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ClockSampleSource");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_BadClockSampleSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ClockSampleSource");
    cdb.Write("ClockSampleSource", "INVALID");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}


bool NI6368DIOTest::TestInitialise_ClockSamplePolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSamplePolarity", "ACTIVE_HIGH_OR_RISING_EDGE", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_polarity == XSERIES_DI_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSamplePolarity", "ACTIVE_LOW_OR_FALLING_EDGE", &conf, NULL);
        }
        if (ok) {
            ok = (conf.convert_polarity == XSERIES_DI_POLARITY_ACTIVE_LOW_OR_FALLING_EDGE);
        }
    }

    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoClockSamplePolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ClockSamplePolarity");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_BadClockSamplePolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ClockSamplePolarity");
    cdb.Write("ClockSamplePolarity", "INVALID");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}


bool NI6368DIOTest::TestInitialise_ScanIntervalCounterSource() {
    bool ok = true;
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "COUNTER_TB3", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_TB3);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "COUNTER_TB2", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_TB2);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "COUNTER_TB1", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_TB1);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI0", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI0);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI1", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI1);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI2", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI2);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI3", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI3);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI4", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI4);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI5", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI5);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI6", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI6);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI7", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI7);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI8", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI8);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI9", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI9);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI10", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI10);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI11", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI11);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI12", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI12);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI13", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI13);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI14", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI14);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI15", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI15);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI0", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI0);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI1", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI1);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI2", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI2);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI3", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI3);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI4", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI4);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI5", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI5);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI6", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI6);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI7", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI7);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PXI_CLK10", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PXI_CLK10);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "STAR_TRIGGER", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_STAR_TRIGGER);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "ANALOG_TRIGGER", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_ANALOG_TRIGGER);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "DSTARA", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_DSTARA);
        }
    }
    {
        xseries_di_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "DSTARB", &conf, NULL);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_DSTARB);
        }
    }

    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoScanIntervalCounterSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ScanIntervalCounterSource");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_BadScanIntervalCounterSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ScanIntervalCounterSource");
    cdb.Write("ScanIntervalCounterSource", "INVALID");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}


bool NI6368DIOTest::TestInitialise_False_NoScanIntervalCounterPeriod() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ScanIntervalCounterPeriod");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoScanIntervalCounterDelay() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ScanIntervalCounterDelay");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}


bool NI6368DIOTest::TestInitialise_StartTriggerSource() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "SW_PULSE", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_SW_PULSE);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI0", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI0);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI1", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI1);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI2", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI2);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI3", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI3);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI4", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI4);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI5", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI5);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI6", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI6);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI7", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI7);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI8", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI8);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI9", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI9);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI10", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI10);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI11", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI11);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI12", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI12);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI13", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI13);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI14", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI14);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI15", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PFI15);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI0", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_RTSI0);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI1", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_RTSI1);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI2", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_RTSI2);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI3", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_RTSI3);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI4", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_RTSI4);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI5", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_RTSI5);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI6", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_RTSI6);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI7", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_RTSI7);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "DIO_CHGDETECT", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_DIO_CHGDETECT);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "G0_OUT", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_G0_OUT);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "G1_OUT", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_G1_OUT);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "G2_OUT", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_G2_OUT);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "G3_OUT", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_G3_OUT);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "STAR_TRIGGER", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_STAR_TRIGGER);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "ANALOG_TRIGGER", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_ANALOG_TRIGGER);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "AI_START1", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_AI_START1);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "AI_START2", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_AI_START2);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "STAR_TRIGGER", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_STAR_TRIGGER);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "LOW", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_LOW);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PXIE_DSTARA", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PXIE_DSTARA);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PXIE_DSTARB", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_PXIE_DSTARB);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "DI_START1", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_DI_START1);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "DI_START2", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_DI_START2);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "FIFOCONDITION", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_FIFOCONDITION);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA0", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_INTTRIGGERA0);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA1", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_INTTRIGGERA1);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA2", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_INTTRIGGERA2);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA3", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_INTTRIGGERA3);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA4", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_INTTRIGGERA4);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA5", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_INTTRIGGERA5);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA6", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_INTTRIGGERA6);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA7", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_DO_START_TRIGGER_INTTRIGGERA7);
        }
    }

    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoStartTriggerSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("StartTriggerSource");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_BadStartTriggerSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("StartTriggerSource");
    cdb.Write("StartTriggerSource", "INVALID");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_StartTriggerPolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerPolarity", "RISING_EDGE", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_polarity == XSERIES_DO_POLARITY_RISING_EDGE);
        }
    }
    //TODO check that FALLING_EDGE is indeed not supported
#if 0
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerPolarity", "FALLING_EDGE", NULL, &conf);
        }
        if (ok) {
            ok = (conf.start1_polarity == XSERIES_DO_POLARITY_FALLING_EDGE);
        }
    }
#endif
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoStartTriggerPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("StartTriggerPolarity");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_BadStartTriggerPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("StartTriggerPolarity");
    cdb.Write("StartTriggerPolarity", "INVALID");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_UpdateCounterSource() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "UI_TC", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_UI_TC);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI0", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI0);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI1", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI1);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI2", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI2);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI3", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI3);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI4", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI4);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI5", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI5);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI6", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI6);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI7", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI7);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI8", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI8);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI9", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI9);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI10", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI10);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI11", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI11);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI12", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI12);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI13", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI13);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI14", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI14);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI15", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PFI15);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI0", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_RTSI0);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI1", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_RTSI1);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI2", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_RTSI2);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI3", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_RTSI3);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI4", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_RTSI4);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI5", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_RTSI5);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI6", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_RTSI6);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI7", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_RTSI7);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "DIO_CHGDETECT", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_DIO_CHGDETECT);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G0_OUT", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_G0_OUT);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G1_OUT", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_G1_OUT);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G2_OUT", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_G2_OUT);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G3_OUT", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_G3_OUT);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "STAR_TRIGGER", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_STAR_TRIGGER);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "ANALOG_TRIGGER", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_ANALOG_TRIGGER);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "STAR_TRIGGER", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_STAR_TRIGGER);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "LOW", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_LOW);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PXIE_DSTARA", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PXIE_DSTARA);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PXIE_DSTARB", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_PXIE_DSTARB);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G0_SAMPLECLK", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_G0_SAMPLECLK);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G1_SAMPLECLK", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_G1_SAMPLECLK);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G2_SAMPLECLK", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_G2_SAMPLECLK);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G3_SAMPLECLK", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_G3_SAMPLECLK);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "AI_CONVERT", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_AI_CONVERT);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "AI_START", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_AI_START);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "DI_CONVERT", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_DI_CONVERT);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA0", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA0);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA1", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA1);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA2", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA2);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA3", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA3);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA4", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA4);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA5", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA5);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA6", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA6);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA7", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA7);
        }
    }

    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoUpdateCounterSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateCounterSource");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_BadUpdateCounterSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateCounterSource");
    cdb.Write("UpdateCounterSource", "INVALID");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_UpdateCounterPolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterPolarity", "RISING_EDGE", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_polarity == XSERIES_DO_POLARITY_RISING_EDGE);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterPolarity", "FALLING_EDGE", NULL, &conf);
        }
        if (ok) {
            ok = (conf.update_polarity == XSERIES_DO_POLARITY_FALLING_EDGE);
        }
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoUpdateCounterPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateCounterPolarity");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_BadUpdateCounterPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateCounterPolarity");
    cdb.Write("UpdateCounterPolarity", "INVALID");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_UpdateIntervalCounterSource() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "TB3", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB3);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "TB2", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB2);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "TB1", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB1);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI0", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI0);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI1", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI1);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI2", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI2);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI3", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI3);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI4", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI4);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI5", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI5);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI6", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI6);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI7", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI7);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI8", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI8);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI9", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI9);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI10", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI10);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI11", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI11);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI12", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI12);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI13", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI13);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI14", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI14);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI15", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI15);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI0", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI0);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI1", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI1);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI2", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI2);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI3", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI3);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI4", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI4);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI5", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI5);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI6", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI6);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI7", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI7);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "STAR_TRIGGER", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_STAR_TRIGGER);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "ANALOG_TRIGGER", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_ANALOG_TRIGGER);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "STAR_TRIGGER", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_STAR_TRIGGER);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "DSTARA", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_DSTARA);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "DSTARB", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_DSTARB);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PXI_CLK10", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PXI_CLK10);
        }
    }

    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoUpdateIntervalCounterSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterSource");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_BadUpdateIntervalCounterSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterSource");
    cdb.Write("UpdateIntervalCounterSource", "INVALID");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_UpdateIntervalCounterPolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterPolarity", "RISING_EDGE", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_polarity == XSERIES_OUTTIMER_POLARITY_RISING_EDGE);
        }
    }
    {
        xseries_do_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterPolarity", "FALLING_EDGE", NULL, &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_polarity == XSERIES_OUTTIMER_POLARITY_FALLING_EDGE);
        }
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoUpdateIntervalCounterPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterPolarity");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_BadUpdateIntervalCounterPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterPolarity");
    cdb.Write("UpdateIntervalCounterPolarity", "INVALID");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoUpdateIntervalCounterPeriodDivisor() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterPeriodDivisor");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoUpdateIntervalCounterDelay() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterDelay");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_InvalidUpdateIntervalCounterPeriodDivisor() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterPeriodDivisor");
    cdb.Write("UpdateIntervalCounterPeriodDivisor", 0);
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_InvalidUpdateIntervalCounterDelay() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterDelay");
    cdb.Write("UpdateIntervalCounterDelay", 0);
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestInitialise_False_NoSignals() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("Signals");
    NI6368DIO test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DIOTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config1);
}

bool NI6368DIOTest::TestSetConfiguredDatabase_OnlyInputs() {
    return TestIntegratedInApplication(config1);
}

bool NI6368DIOTest::TestSetConfiguredDatabase_OnlyOutputs() {
    return TestIntegratedInApplication(config1);
}

bool NI6368DIOTest::TestSetConfiguredDatabase_False_NoChannels() {
    return !TestIntegratedInApplication(config4);
}

bool NI6368DIOTest::TestSetConfiguredDatabase_False_SignalTypeMismatch() {
    return !TestIntegratedInApplication(config5);
}

bool NI6368DIOTest::TestSetConfiguredDatabase_False_SignalNumberOfSamplesGreaterThanOne() {
    return !TestIntegratedInApplication(config6);
}

bool NI6368DIOTest::TestSetConfiguredDatabase_False_WrongDeviceName() {
    return !TestIntegratedInApplication(config7);
}

