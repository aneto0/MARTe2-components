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
    ReferenceT<NI6368DAC> ni6368DAC;
    if (ok) {
        ni6368DAC = application->Find("Data.NI6368_0");
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
        "                    NumberOfElements = 1000"
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
        "            StartTriggerSource = \"SW_PULSE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
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
        "            StartTriggerSource = \"SW_PULSE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
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
        "            StartTriggerSource = \"SW_PULSE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
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
        "            StartTriggerSource = \"SW_PULSE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
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
        "            StartTriggerSource = \"SW_PULSE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
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
        "            StartTriggerSource = \"SW_PULSE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
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
        "            StartTriggerSource = \"SW_PULSE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
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
        "            StartTriggerSource = \"SW_PULSE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
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
        "            StartTriggerSource = \"SW_PULSE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
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

//Two channel base configuration with different number of elements
static const MARTe::char8 * const config9 = ""
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
        "                    NumberOfElements = 10"
        "                    Trigger = 1"
        "                }"
        "                DAC1_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = float32"
        "                    Samples = 1"
        "                    NumberOfElements = 9"
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
        "            StartTriggerSource = \"SW_PULSE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
        "            Signals = {"
        "                DAC0_0 = {"
        "                   Type = float32"
        "                   ChannelId = 1"
        "                   OutputRange = 10"
        "                }"
        "                DAC1_0 = {"
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

//Two channel base configuration with more than one sample
static const MARTe::char8 * const config10 = ""
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
        "                    NumberOfElements = 10"
        "                    Trigger = 1"
        "                }"
        "                DAC1_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = float32"
        "                    Samples = 2"
        "                    NumberOfElements = 10"
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
        "            StartTriggerSource = \"SW_PULSE\""
        "            UpdateCounterSource = \"UI_TC\""
        "            UpdateCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterSource = \"TB3\""
        "            UpdateIntervalCounterPolarity = \"RISING_EDGE\""
        "            UpdateIntervalCounterPeriodDivisor = 100000"
        "            UpdateIntervalCounterDelay = 2"
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

bool NI6368DACTest::TestInitialise_StartTriggerSource() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "SW_PULSE", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_SW_PULSE);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI0", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI0);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI1", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI1);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI2", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI2);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI3", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI3);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI4", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI4);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI5", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI5);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI6", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI6);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI7", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI7);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI8", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI8);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI9", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI9);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI10", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI10);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI11", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI11);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI12", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI12);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI13", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI13);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI14", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI14);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PFI15", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PFI15);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI0", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_RTSI0);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI1", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_RTSI1);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI2", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_RTSI2);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI3", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_RTSI3);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI4", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_RTSI4);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI5", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_RTSI5);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI6", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_RTSI6);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "RTSI7", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_RTSI7);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "DIO_CHGDETECT", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_DIO_CHGDETECT);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "G0_OUT", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_G0_OUT);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "G1_OUT", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_G1_OUT);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "G2_OUT", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_G2_OUT);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "G3_OUT", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_G3_OUT);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "STAR_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_STAR_TRIGGER);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "ANALOG_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_ANALOG_TRIGGER);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "AI_START1", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_AI_START1);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "AI_START2", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_AI_START2);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "STAR_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_STAR_TRIGGER);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "LOW", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_LOW);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PXIE_DSTARA", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PXIE_DSTARA);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "PXIE_DSTARB", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_PXIE_DSTARB);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "DI_START1", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_DI_START1);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "DI_START2", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_DI_START2);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "DO_START1", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_DO_START1);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "FIFOCONDITION", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_FIFOCONDITION);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA0", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_INTTRIGGERA0);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA1", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_INTTRIGGERA1);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA2", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_INTTRIGGERA2);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA3", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_INTTRIGGERA3);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA4", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_INTTRIGGERA4);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA5", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_INTTRIGGERA5);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA6", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_INTTRIGGERA6);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "StartTriggerSource", "INTTRIGGERA7", &conf);
        }
        if (ok) {
            ok = (conf.start1_source == XSERIES_AO_START_TRIGGER_INTTRIGGERA7);
        }
    }

    return ok;
}

bool NI6368DACTest::TestInitialise_False_NoStartTriggerSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("StartTriggerSource");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_BadStartTriggerSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("StartTriggerSource");
    cdb.Write("StartTriggerSource", "INVALID");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_UpdateCounterSource() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "UI_TC", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_UI_TC);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI0", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI0);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI1", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI1);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI2", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI2);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI3", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI3);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI4", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI4);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI5", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI5);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI6", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI6);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI7", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI7);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI8", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI8);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI9", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI9);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI10", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI10);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI11", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI11);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI12", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI12);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI13", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI13);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI14", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI14);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PFI15", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PFI15);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI0", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_RTSI0);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI1", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_RTSI1);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI2", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_RTSI2);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI3", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_RTSI3);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI4", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_RTSI4);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI5", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_RTSI5);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI6", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_RTSI6);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "RTSI7", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_RTSI7);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "DIO_CHGDETECT", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_DIO_CHGDETECT);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G0_OUT", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_G0_OUT);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G1_OUT", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_G1_OUT);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G2_OUT", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_G2_OUT);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G3_OUT", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_G3_OUT);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "STAR_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_STAR_TRIGGER);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "ANALOG_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_ANALOG_TRIGGER);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "STAR_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_STAR_TRIGGER);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "LOW", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_LOW);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PXIE_DSTARA", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PXIE_DSTARA);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "PXIE_DSTARB", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_PXIE_DSTARB);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G0_SAMPLECLK", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_G0_SAMPLECLK);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G1_SAMPLECLK", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_G1_SAMPLECLK);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G2_SAMPLECLK", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_G2_SAMPLECLK);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "G3_SAMPLECLK", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_G3_SAMPLECLK);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "AI_CONVERT", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_AI_CONVERT);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "AI_START", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_AI_START);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "DI_CONVERT", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_DI_CONVERT);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "DO_UPDATE", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_DO_UPDATE);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "AUTOUPDATE", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_AUTOUPDATE);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA0", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA0);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA1", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA1);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA2", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA2);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA3", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA3);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA4", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA4);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA5", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA5);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA6", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA6);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterSource", "INTTRIGGERA7", &conf);
        }
        if (ok) {
            ok = (conf.update_source == XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA7);
        }
    }

    return ok;
}

bool NI6368DACTest::TestInitialise_False_NoUpdateCounterSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateCounterSource");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_BadUpdateCounterSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateCounterSource");
    cdb.Write("UpdateCounterSource", "INVALID");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_UpdateCounterPolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterPolarity", "RISING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.update_polarity == XSERIES_AO_POLARITY_RISING_EDGE);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateCounterPolarity", "FALLING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.update_polarity == XSERIES_AO_POLARITY_FALLING_EDGE);
        }
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_NoUpdateCounterPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateCounterPolarity");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_BadUpdateCounterPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateCounterPolarity");
    cdb.Write("UpdateCounterPolarity", "INVALID");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_UpdateIntervalCounterSource() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "TB3", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB3);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "TB2", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB2);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "TB1", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB1);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI0", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI0);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI1", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI1);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI2", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI2);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI3", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI3);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI4", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI4);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI5", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI5);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI6", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI6);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI7", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI7);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI8", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI8);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI9", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI9);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI10", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI10);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI11", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI11);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI12", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI12);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI13", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI13);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI14", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI14);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PFI15", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI15);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI0", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI0);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI1", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI1);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI2", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI2);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI3", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI3);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI4", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI4);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI5", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI5);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI6", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI6);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "RTSI7", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI7);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "STAR_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_STAR_TRIGGER);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "ANALOG_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_ANALOG_TRIGGER);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "STAR_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_STAR_TRIGGER);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "DSTARA", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_DSTARA);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "DSTARB", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_DSTARB);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterSource", "PXI_CLK10", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_source == XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PXI_CLK10);
        }
    }

    return ok;
}

bool NI6368DACTest::TestInitialise_False_NoUpdateIntervalCounterSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterSource");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_BadUpdateIntervalCounterSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterSource");
    cdb.Write("UpdateIntervalCounterSource", "INVALID");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_UpdateIntervalCounterPolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterPolarity", "RISING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_polarity == XSERIES_OUTTIMER_POLARITY_RISING_EDGE);
        }
    }
    {
        xseries_ao_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "UpdateIntervalCounterPolarity", "FALLING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.outtimer_params.ui_polarity == XSERIES_OUTTIMER_POLARITY_FALLING_EDGE);
        }
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_NoUpdateIntervalCounterPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterPolarity");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_BadUpdateIntervalCounterPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterPolarity");
    cdb.Write("UpdateIntervalCounterPolarity", "INVALID");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_NoUpdateIntervalCounterPeriodDivisor() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterPeriodDivisor");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_NoUpdateIntervalCounterDelay() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterDelay");
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_InvalidUpdateIntervalCounterPeriodDivisor() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterPeriodDivisor");
    cdb.Write("UpdateIntervalCounterPeriodDivisor", 0);
    NI6368DAC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368DACTest::TestInitialise_False_InvalidUpdateIntervalCounterDelay() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("UpdateIntervalCounterDelay");
    cdb.Write("UpdateIntervalCounterDelay", 0);
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

    ReferenceT<NI6368DAC> ni6368DAC;
    if (ok) {
        ni6368DAC = application->Find("Data.NI6368_0");
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

bool NI6368DACTest::TestInitialise_False_NotMonotonicallyIncreasingChannelId() {
    return !TestIntegratedInApplication(config9);
}

bool NI6368DACTest::TestSetConfiguredDatabase_False_MoreThanOneSample() {
    return !TestIntegratedInApplication(config10);
}

