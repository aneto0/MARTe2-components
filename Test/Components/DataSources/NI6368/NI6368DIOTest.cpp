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

//Two ports configuration. One port writing with no trigger.
static const MARTe::char8 * const config2 = ""
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
        "        +GAMB = {"
        "            Class = NI6368DIOTestGAM"
        "            InputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT0_0 = {"
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
        "        +GAMB = {"
        "            Class = NI6368DIOTestGAM"
        "            InputSignals = {"
        "                PORT0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                PORT0_0 = {"
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

//One port base configuration without specifying a triggering signal
static const MARTe::char8 * const config8 = ""
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

//One port base configuration with only inputs
static const MARTe::char8 * const config9 = ""
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

//One port base configuration with only outputs
static const MARTe::char8 * const config10 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368DIOTestGAM"
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

bool NI6368DIOTest::TestGetOutputBrokers_NoTriggerGAM() {
    return TestIntegratedInApplication(config2);
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

bool NI6368DIOTest::TestInitialise_False_BadPortId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0.Signals.PORT_0");
    cdb.Delete("PortId");
    cdb.Write("PortId", "3");
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
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

bool NI6368DIOTest::TestSetConfiguredDatabase_AllChannels() {
    return TestIntegratedInApplication(config3);
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

bool NI6368DIOTest::TestSetConfiguredDatabase_False_NoTriggers() {
    return !TestIntegratedInApplication(config8);
}

