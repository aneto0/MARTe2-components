/**
 * @file NI6368ADCTest.cpp
 * @brief Source file for class NI6368ADCTest
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
 * the class NI6368ADCTest (public, protected, and private). Be aware that some
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
#include "NI6368ADC.h"
#include "NI6368ADCTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "BrokerI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class NI6368ADCTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

NI6368ADCTestGAM    () : GAM() {
        rms = 0;
        numberOfSamples = 0;
        signalPtr = NULL;
        counterPtr = NULL;
        timerPtr = NULL;
        totalSamples = 0;
    }

    ~NI6368ADCTestGAM() {
    }

    bool Execute() {
        using namespace MARTe;
        uint32 i;
        for (i=0; i<numberOfSamples; i++) {
            rms += (signalPtr[i] *signalPtr[i]);
        }
        totalSamples += numberOfSamples;
        return true;
    }

    bool Setup() {
        using namespace MARTe;
        counterPtr = reinterpret_cast<uint32 *>(GetInputSignalMemory(0));
        timerPtr = reinterpret_cast<uint32 *>(GetInputSignalMemory(1));
        signalPtr = reinterpret_cast<int16 *>(GetInputSignalMemory(2));
        GetSignalNumberOfSamples(InputSignals, 2u, numberOfSamples);
        return true;
    }

    MARTe::uint16 rms;
    MARTe::uint32 *counterPtr;
    MARTe::uint32 *timerPtr;
    MARTe::int16 *signalPtr;
    MARTe::uint32 numberOfSamples;
    MARTe::uint32 totalSamples;

};
CLASS_REGISTER(NI6368ADCTestGAM, "1.0")

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
    ReferenceT<NI6368ADCTestGAM> testGAM;
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
        while (testGAM->totalSamples < 10000) {
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
                               const MARTe::char8 * const parameterToPatch, const MARTe::char8 * const valueToPatch, xseries_ai_conf_t *conf) {
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
    ReferenceT<NI6368ADC> ni6368ADC = application->Find("Data.NI6368_0");
    if (ok) {
        ok = ni6368ADC.IsValid();
    }
    if (ok) {
        ni6368ADC->ReadAIConfiguration(conf);
    }
    god->Purge();

    return ok;
}

//Two channels base configuration
static const MARTe::char8 * const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 2000"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUs = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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

static const MARTe::char8 * const config1b = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 2000"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUs = 0xf"
        "            ExecutionMode = RealTimeThread"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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

//As config1 but with RealTimeMode = 1
static const MARTe::char8 * const config1c = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 2000"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUs = 0xf"
        "            RealTimeMode = 1"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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


//One channel base configuration
static const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 1000"
        "                    Frequency = 2000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUs = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
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

//All channels base configuration
static const MARTe::char8 * const config3 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 2000"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
        "                }"
        "                ADC2_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 2"
        "                }"
        "                ADC3_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 3"
        "                }"
        "                ADC4_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 4"
        "                }"
        "                ADC5_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 5"
        "                }"
        "                ADC6_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 6"
        "                }"
        "                ADC7_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 7"
        "                }"
        "                ADC8_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 8"
        "                }"
        "                ADC9_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 9"
        "                }"
        "                ADC10_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 10"
        "                }"
        "                ADC11_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 11"
        "                }"
        "                ADC12_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 12"
        "                }"
        "                ADC13_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 13"
        "                }"
        "                ADC14_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 14"
        "                }"
        "                ADC15_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 15"
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

//All channels base configuration with no default parameters
static const MARTe::char8 * const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 2000"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 1"
        "                }"
        "                ADC2_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 2"
        "                }"
        "                ADC3_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 3"
        "                }"
        "                ADC4_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 4"
        "                }"
        "                ADC5_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 5"
        "                }"
        "                ADC6_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 6"
        "                }"
        "                ADC7_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 7"
        "                }"
        "                ADC8_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 8"
        "                }"
        "                ADC9_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 9"
        "                }"
        "                ADC10_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 10"
        "                }"
        "                ADC11_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 11"
        "                }"
        "                ADC12_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 12"
        "                }"
        "                ADC13_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 13"
        "                }"
        "                ADC14_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 14"
        "                }"
        "                ADC15_0 = {"
        "                   InputRange = 5"
        "                   Type = int16"
        "                   ChannelId = 15"
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

//Two channels base configuration with more than one function
static const MARTe::char8 * const config5 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 2000"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                ADC1_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 2000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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

//Two channels base configuration including a not synchronising GAM
static const MARTe::char8 * const config5b = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 2000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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


//No channels defined
static const MARTe::char8 * const config6 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUMask = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
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

//Two channels base configuration with a wrong counter type
static const MARTe::char8 * const config7 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 1000"
        "                    Frequency = 2000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUMask = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint16"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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

//Two channels base configuration with a wrong counter type
static const MARTe::char8 * const config8 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 1000"
        "                    Frequency = 2000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUMask = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint16"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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

//Two channels base configuration with a wrong signal type
static const MARTe::char8 * const config9 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = uint32"
        "                    Samples = 1000"
        "                    Frequency = 2000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUMask = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = uint32"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = uint32"
        "                   ChannelId = 1"
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

//Two channels base configuration with a number of samples mismatch
static const MARTe::char8 * const config10 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 1000"
        "                    Frequency = 2000"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                ADC1_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 5000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUMask = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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

//Two channels base configuration with a wrong device name
static const MARTe::char8 * const config11 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 1000"
        "                    Frequency = 2000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/tmp/pxie-116368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUMask = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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

//Two channels base configuration with a wrong configuration of the frequency vs samples
static const MARTe::char8 * const config12 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 1000"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUMask = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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

//Two channels base configuration with more than one sample for the counter
static const MARTe::char8 * const config13 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                    Samples = 2"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 1000"
        "                    Frequency = 2000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUMask = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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

//Two channels base configuration with more than one sample for the timer
static const MARTe::char8 * const config14 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                    Samples = 2"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 1000"
        "                    Frequency = 2000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUMask = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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

//Not monotonically increasing channel identifier
static const MARTe::char8 * const config15 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6368_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6368_0"
        "                    Type = int16"
        "                    Samples = 2000"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6368_0 = {"
        "            Class = NI6368ADC"
        "            DeviceName = \"/dev/pxie-6368\""
        "            BoardId = 0"
        "            DMABufferSize = 1000"
        "            ClockSampleSource = \"INTERNALTIMING\""
        "            ClockSamplePolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ClockConvertSource = \"INTERNALTIMING\""
        "            ClockConvertPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            ScanIntervalCounterSource = \"COUNTER_TB3\""
        "            ScanIntervalCounterPolarity = \"RISING_EDGE\""
        "            ScanIntervalCounterPeriod = 50"
        "            ScanIntervalCounterDelay = 2"
        "            CPUs = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 0"
        "                }"
        "                ADC2_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 2"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
        "                   ChannelId = 1"
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
bool NI6368ADCTest::TestConstructor() {
    using namespace MARTe;
    NI6368ADC ni6368ADC;
    return true;
}

bool NI6368ADCTest::TestAllocateMemory() {
    using namespace MARTe;
    NI6368ADC ni6368ADC;
    return ni6368ADC.AllocateMemory();
}

bool NI6368ADCTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    NI6368ADC ni6368ADC;
    return (ni6368ADC.GetNumberOfMemoryBuffers() == 32u);
}

bool NI6368ADCTest::TestGetSignalMemoryBuffer() {
    return TestIntegratedInApplication(config1);
}

bool NI6368ADCTest::TestGetSignalMemoryBuffer_False() {
    using namespace MARTe;
    NI6368ADC test;
    uint32 *ptr;
    return !test.GetSignalMemoryBuffer(NI6368ADC_HEADER_SIZE + NI6368ADC_MAX_CHANNELS, 0, (void *&) ptr);
}

bool NI6368ADCTest::TestGetBrokerName() {
    using namespace MARTe;
    NI6368ADC test;
    ConfigurationDatabase config;
    StreamString brokerName = test.GetBrokerName(config, OutputSignals);
    bool ok = (brokerName == "");
    if (ok) {
        brokerName = test.GetBrokerName(config, InputSignals);
        ok = (brokerName == "NI6368ADCInputBroker");
    }
    if (ok) {
        config.Write("Frequency", 10);
        brokerName = test.GetBrokerName(config, InputSignals);
        ok = (brokerName == "NI6368ADCInputBroker");
    }

    return ok;
}

bool NI6368ADCTest::TestGetInputBrokers() {
    return TestIntegratedInApplication(config1);
}

bool NI6368ADCTest::TestGetOutputBrokers() {
    using namespace MARTe;
    NI6368ADC test;
    ReferenceContainer rc;
    return !test.GetOutputBrokers(rc, "", NULL);
}

bool NI6368ADCTest::TestSynchronise() {
    return TestIntegratedInApplication(config1);
}

bool NI6368ADCTest::TestExecute() {
    return TestIntegratedInApplication(config1);
}

bool NI6368ADCTest::TestExecute_RealTimeThread() {
    return TestIntegratedInApplication(config1b);
}

bool NI6368ADCTest::TestExecute_RealTimeMode() {
    return TestIntegratedInApplication(config1c);
}

bool NI6368ADCTest::TestPrepareNextState() {
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
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        application->StartNextStateExecution();
    }

    ReferenceT<NI6368ADC> ni6368ADC = application->Find("Data.NI6368_0");
    if (ok) {
        ok = ni6368ADC.IsValid();
    }
    uint32 *counter;
    uint32 *timer;

    if (ok) {
        ni6368ADC->GetSignalMemoryBuffer(0, 0, (void *&) counter);
        ni6368ADC->GetSignalMemoryBuffer(1, 0, (void *&) timer);
        uint32 c = 0;
        ok = false;
        while (c < 500 && !ok) {
            ok = ((*counter) > 1) && ((*timer) > 1);
            c++;
            if (!ok) {
                Sleep::MSec(10);
            }
        }
    }
    if (ok) {
        Sleep::MSec(1000);
        uint32 counterBefore = (*counter);
        uint32 timerBefore = (*timer);
        application->StopCurrentStateExecution();

        if (ok) {
            ok = (counterBefore > 1) && (timerBefore > 1);
        }
        if (ok) {
            ok = application->PrepareNextState("State1");
        }
        if (ok) {
            ok = (((*counter) < counterBefore) && ((*timer) < timerBefore));
        }
    }

    god->Purge();
    return ok;
}

bool NI6368ADCTest::TestInitialise() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    NI6368ADC test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_NoDeviceName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("DeviceName");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadSamplingFrequency(){
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Write("SamplingFrequency", 3000000);
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadSamplingFrequency2(){
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Write("SamplingFrequency", 1999999);
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadConvertFrequency(){
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Write("IsMultiplexed", 1);
    cdb.Delete("DeviceName");
    cdb.Write("DeviceName", "/dev/pxie-6363");
    cdb.Write("SamplingFrequency", 2000000);
    cdb.Write("ConvertFrequency", 3000000);
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadConvertFrequency2(){
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Write("IsMultiplexed", 1);
    cdb.Delete("DeviceName");
    cdb.Write("DeviceName", "/dev/pxie-6363");
    cdb.Write("SamplingFrequency", 250000);
    cdb.Write("ConvertFrequency", 200000);
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}


bool NI6368ADCTest::TestInitialise_False_NoBoardId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("BoardId");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_Default_NoDMABufferSize() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("DMABufferSize");
    NI6368ADC test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_NoScanIntervalCounterPeriod() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ScanIntervalCounterPeriod");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_Default_NoScanIntervalCounterDelay() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ScanIntervalCounterDelay");
    NI6368ADC test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_ClockSampleSource() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTERNALTIMING", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTERNALTIMING);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI0", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI0);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI1", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI1);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI2", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI2);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI3", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI3);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI4", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI4);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI5", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI5);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI6", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI6);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI7", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI7);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI8", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI8);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI9", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI9);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI10", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI10);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI11", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI11);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI12", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI12);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI13", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI13);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI14", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI14);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PFI15", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI15);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI0", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI0);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI1", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI1);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI2", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI2);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI3", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI3);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI4", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI4);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI5", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI5);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI6", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI6);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "RTSI7", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI7);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "DIO_CHGDETECT", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_DIO_CHGDETECT);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G0_OUT", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G0_OUT);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G1_OUT", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G1_OUT);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G2_OUT", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G2_OUT);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G3_OUT", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G3_OUT);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "STAR_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_STAR_TRIGGER);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "SCXI_TRIG1", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_SCXI_TRIG1);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "ATRIG", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_ATRIG);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "LOW", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_LOW);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PXIE_DSTARA", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PXIE_DSTARA);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "PXIE_DSTARB", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PXIE_DSTARB);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G0_SAMPLECLK", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G0_SAMPLECLK);
        }
    }

    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G1_SAMPLECLK", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G1_SAMPLECLK);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G2_SAMPLECLK", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G2_SAMPLECLK);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "G3_SAMPLECLK", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G3_SAMPLECLK);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "DI_CONVERT", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_DI_CONVERT);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "AO_UPDATE", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_AO_UPDATE);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "DO_UPDATE", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_DO_UPDATE);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA0", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA0);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA1", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA1);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA2", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA2);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA3", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA3);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA4", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA4);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA5", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA5);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA6", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA6);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSampleSource", "INTTRIGGERA7", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA7);
        }
    }

    return ok;
}

bool NI6368ADCTest::TestInitialise_Default_NoClockSampleSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ClockSampleSource");
    NI6368ADC test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadClockSampleSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ClockSampleSource");
    cdb.Write("ClockSampleSource", "INVALID");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_ClockSamplePolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSamplePolarity", "ACTIVE_HIGH_OR_RISING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.start_polarity == XSERIES_AI_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockSamplePolarity", "ACTIVE_LOW_OR_FALLING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.start_polarity == XSERIES_AI_POLARITY_ACTIVE_LOW_OR_FALLING_EDGE);
        }
    }

    return ok;
}

bool NI6368ADCTest::TestInitialise_Default_NoClockSamplePolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ClockSamplePolarity");
    NI6368ADC test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadClockSamplePolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ClockSamplePolarity");
    cdb.Write("ClockSamplePolarity", "INVALID");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_ClockConvertSource() {
    bool ok = true;
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "INTERNALTIMING", &conf);
        }
        if (ok) {
            ok = (conf.start_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTERNALTIMING);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI0", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI0);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI1", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI1);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI2", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI2);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI3", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI3);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI4", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI4);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI5", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI5);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI6", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI6);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI7", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI7);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI8", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI8);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI9", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI9);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI10", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI10);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI11", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI11);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI12", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI12);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI13", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI13);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI14", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI14);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PFI15", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI15);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "RTSI0", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI0);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "RTSI1", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI1);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "RTSI2", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI2);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "RTSI3", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI3);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "RTSI4", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI4);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "RTSI5", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI5);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "RTSI6", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI6);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "RTSI7", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI7);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "DIO_CHGDETECT", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_DIO_CHGDETECT);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "G0_OUT", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G0_OUT);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "G1_OUT", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G1_OUT);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "G2_OUT", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G2_OUT);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "G3_OUT", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G3_OUT);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "STAR_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_STAR_TRIGGER);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "SCXI_TRIG1", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_SCXI_TRIG1);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "ATRIG", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_ATRIG);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "LOW", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_LOW);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PXIE_DSTARA", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PXIE_DSTARA);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "PXIE_DSTARB", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_PXIE_DSTARB);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "G0_SAMPLECLK", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G0_SAMPLECLK);
        }
    }

    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "G1_SAMPLECLK", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G1_SAMPLECLK);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "G2_SAMPLECLK", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G2_SAMPLECLK);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "G3_SAMPLECLK", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_G3_SAMPLECLK);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "DI_CONVERT", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_DI_CONVERT);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "AO_UPDATE", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_AO_UPDATE);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "DO_UPDATE", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_DO_UPDATE);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "INTTRIGGERA0", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA0);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "INTTRIGGERA1", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA1);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "INTTRIGGERA2", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA2);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "INTTRIGGERA3", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA3);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "INTTRIGGERA4", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA4);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "INTTRIGGERA5", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA5);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "INTTRIGGERA6", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA6);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertSource", "INTTRIGGERA7", &conf);
        }
        if (ok) {
            ok = (conf.convert_source == XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA7);
        }
    }

    return ok;
}

bool NI6368ADCTest::TestInitialise_Default_NoClockConvertSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ClockConvertSource");
    NI6368ADC test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadClockConvertSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ClockConvertSource");
    cdb.Write("ClockConvertSource", "INVALID");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_ScanIntervalCounterSource() {
    bool ok = true;
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "COUNTER_TB3", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_TB3);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "COUNTER_TB2", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_TB2);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "COUNTER_TB1", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_TB1);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI0", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI0);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI1", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI1);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI2", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI2);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI3", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI3);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI4", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI4);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI5", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI5);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI6", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI6);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI7", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI7);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI8", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI8);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI9", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI9);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI10", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI10);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI11", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI11);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI12", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI12);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI13", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI13);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI14", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI14);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PFI15", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PFI15);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI0", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI0);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI1", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI1);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI2", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI2);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI3", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI3);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI4", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI4);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI5", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI5);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI6", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI6);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "RTSI7", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_RTSI7);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "PXI_CLK10", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_PXI_CLK10);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "STAR_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_STAR_TRIGGER);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "ANALOG_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_ANALOG_TRIGGER);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "DSTARA", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_DSTARA);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterSource", "DSTARB", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_source == XSERIES_SCAN_INTERVAL_COUNTER_DSTARB);
        }
    }

    return ok;
}

bool NI6368ADCTest::TestInitialise_Default_NoScanIntervalCounterSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ScanIntervalCounterSource");
    NI6368ADC test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadScanIntervalCounterSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ScanIntervalCounterSource");
    cdb.Write("ScanIntervalCounterSource", "INVALID");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_ScanIntervalCounterPolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterPolarity", "RISING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_polarity == XSERIES_SCAN_INTERVAL_COUNTER_POLARITY_RISING_EDGE);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ScanIntervalCounterPolarity", "FALLING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.intimer_params.si_polarity == XSERIES_SCAN_INTERVAL_COUNTER_POLARITY_FALLING_EDGE);
        }
    }

    return ok;
}

bool NI6368ADCTest::TestInitialise_Default_NoScanIntervalCounterPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ScanIntervalCounterPolarity");
    NI6368ADC test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadScanIntervalCounterPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ScanIntervalCounterPolarity");
    cdb.Write("ScanIntervalCounterPolarity", "INVALID");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_ClockConvertPolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertPolarity", "ACTIVE_HIGH_OR_RISING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.convert_polarity == XSERIES_AI_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0", "ClockConvertPolarity", "ACTIVE_LOW_OR_FALLING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.convert_polarity == XSERIES_AI_POLARITY_ACTIVE_LOW_OR_FALLING_EDGE);
        }
    }

    return ok;
}

bool NI6368ADCTest::TestInitialise_Default_NoClockConvertPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ClockConvertPolarity");
    NI6368ADC test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadClockConvertPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("ClockConvertPolarity");
    cdb.Write("ClockConvertPolarity", "INVALID");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_NoSignals() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    cdb.Delete("Signals");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_InputRange() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputRange", "10", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].range == XSERIES_INPUT_RANGE_10V);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputRange", "5", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].range == XSERIES_INPUT_RANGE_5V);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputRange", "2", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].range == XSERIES_INPUT_RANGE_2V);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputRange", "1", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].range == XSERIES_INPUT_RANGE_1V);
        }
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadInputRange() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0.Signals.ADC0_0");
    cdb.Delete("InputRange");
    cdb.Write("InputRange", "20");
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_DefaultInputRange() {
    bool ok = true;
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputRange", NULL, &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].range == XSERIES_INPUT_RANGE_10V);
        }
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_DefaultInputType() {
    bool ok = true;
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputType", NULL, &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].channel_type == XSERIES_AI_CHANNEL_TYPE_DIFFERENTIAL);
        }
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadChannelId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0.Signals.ADC0_0");
    cdb.Delete("ChannelId");
    cdb.Write("ChannelId", "32");
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_NotMonotonicallyIncreasingChannelId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config15;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6368ADCTest::TestSetConfiguredDatabase_OneChannel() {
    return TestIntegratedInApplication(config2);
}

bool NI6368ADCTest::TestSetConfiguredDatabase_AllChannels() {
    return TestIntegratedInApplication(config3);
}

bool NI6368ADCTest::TestSetConfiguredDatabase_AllChannels_Parameters() {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config4;
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

    ReferenceT<NI6368ADC> ni6368ADC = application->Find("Data.NI6368_0");
    if (ok) {
        ok = ni6368ADC.IsValid();
    }
    if (ok) {
        xseries_ai_conf_t conf;
        ni6368ADC->ReadAIConfiguration(&conf);
        uint32 i;
        for (i = 0; (i < NI6368ADC_MAX_CHANNELS) && (ok); i++) {
            ok = (conf.aichan_conf[i].channel_type == XSERIES_AI_CHANNEL_TYPE_DIFFERENTIAL);
            ok &= (conf.aichan_conf[i].range == XSERIES_INPUT_RANGE_5V);
            ok &= (conf.aichan_conf[i].is_enabled);
        }
    }

    god->Purge();
    return ok;
}

bool NI6368ADCTest::TestSetConfiguredDatabase_False_NoChannels() {
    return !TestIntegratedInApplication(config6);
}

bool NI6368ADCTest::TestSetConfiguredDatabase_False_CounterTypeMismatch() {
    return !TestIntegratedInApplication(config7);
}

bool NI6368ADCTest::TestSetConfiguredDatabase_False_TimerTypeMismatch() {
    return !TestIntegratedInApplication(config8);
}

bool NI6368ADCTest::TestSetConfiguredDatabase_False_SignalTypeMismatch() {
    return !TestIntegratedInApplication(config9);
}

bool NI6368ADCTest::TestSetConfiguredDatabase_False_SignalNumberOfSamplesMismatch() {
    return !TestIntegratedInApplication(config10);
}

bool NI6368ADCTest::TestSetConfiguredDatabase_False_WrongDeviceName() {
    return !TestIntegratedInApplication(config11);
}

bool NI6368ADCTest::TestSetConfiguredDatabase_False_CounterSamples() {
    return !TestIntegratedInApplication(config13);
}

bool NI6368ADCTest::TestSetConfiguredDatabase_False_TimerSamples() {
    return !TestIntegratedInApplication(config14);
}

bool NI6368ADCTest::TestSetConfiguredDatabase_False_MoreThanOneGAM() {
    return !TestIntegratedInApplication(config5);
}

bool NI6368ADCTest::TestGetLastBufferIdx() {
    return TestIntegratedInApplication(config1);
}

bool NI6368ADCTest::TestIsSynchronising() {
    return TestIntegratedInApplication(config1);
}

bool NI6368ADCTest::TestIntegrated() {
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
    ReferenceT<NI6368ADCTestGAM> testGAM;
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
        while (testGAM->totalSamples < 500000)
            ;
        uint16 rms = testGAM->rms;
        if (testGAM->totalSamples > 0) {
            rms = sqrtf(rms / testGAM->totalSamples);
        }
        printf("--->>>RMS = %u\n", rms);
        application->StopCurrentStateExecution();
    }
    god->Purge();
    return ok;
}



bool NI6368ADCTest::TestIntegrated_RealTimeThreadExecutionMode() {
    using namespace MARTe;

    //Two channels base configuration
    static const MARTe::char8 *const configRealTimeThreadExecutionMode = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI6368ADCTestGAM"
            "            InputSignals = {"
            "                Counter = {"
            "                    DataSource = NI6368_0"
            "                }"
            "                Time = {"
            "                    DataSource = NI6368_0"
            "                }"
            "                ADC0_0 = {"
            "                    DataSource = NI6368_0"
            "                    Type = int16"
            "                    Samples = 1000"
            "                    Frequency = 1"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +NI6368_0 = {"
            "            Class = NI6368ADC"
            "            SamplingFrequency = 1000"
            "            ConvertFrequency = 1000000"
            "            IsMultiplexed = 1"
            "            DeviceName = \"/dev/pxie-6363\""
            "            BoardId = 0"
            "            SampleDelayDivisor = 2"
            "            ConvertDivisor = 100"
            "            ExecutionMode = RealTimeThread"
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint64"
            "                    ResetOnBufferChange = 1"
            "                }"
            "                Time = {"
            "                    Type = uint64"
            "                }"
            "                ADC0_0 = {"
            "                   InputRange = 10"
            "                   Type = int16"
            "                   ChannelId = 0"
            "                   InputPolarity = Bipolar"
            "                   InputMode = RSE"
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

    ConfigurationDatabase cdb;
    StreamString configStream = configRealTimeThreadExecutionMode;
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
    ReferenceT<NI6368ADCTestGAM> testGAM;
    if (ok) {
        testGAM = application->Find("Functions.GAMA");
        ok = testGAM.IsValid();
    }
    ReferenceT<NI6368ADC> testADC;
    if (ok) {
        testADC = application->Find("Data.NI6368_0");
        ok = testADC.IsValid();
    }
    ReferenceT<GAMScheduler> scheduler;
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        uint64 *counter;
        testADC->GetSignalMemoryBuffer(0u, 0u, (void*&) counter);
        uint64 *time;
        testADC->GetSignalMemoryBuffer(1u, 0u, (void*&) time);
        int16 *adc0;
        testADC->GetSignalMemoryBuffer(2u, 0u, (void*&) adc0);

        ReferenceContainer inputBrokers;
        testGAM->GetInputBrokers(inputBrokers);

        application->PrepareNextState("State1");
        //Sleep::MSec(500);
        for (uint32 i = 0u; i < inputBrokers.Size(); i++) {
            ReferenceT<BrokerI> broker = inputBrokers.Get(i);
            if (broker.IsValid()) {
                broker->Execute();
            }
        }

        printf("Counter %llu\n", *counter);
        printf("Time %llu\n", *time);
        printf("adc0[0] %d\n", adc0[0]);

        ok = (*counter == 1000ull);
        ok &= (*time == 1000000ull);
    }
    god->Purge();
    return ok;
}

bool NI6368ADCTest::TestIntegrated_RealTimeThreadExecutionMode_NoSync() {
    using namespace MARTe;

    //Two channels base configuration
    static const MARTe::char8 *const configRealTimeThreadExecutionModeNoSync = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI6368ADCTestGAM"
            "            InputSignals = {"
            "                Counter = {"
            "                    DataSource = NI6368_0"
            "                }"
            "                Time = {"
            "                    DataSource = NI6368_0"
            "                }"
            "                ADC0_0 = {"
            "                    DataSource = NI6368_0"
            "                    Type = int16"
            "                    Samples = 1000"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +NI6368_0 = {"
            "            Class = NI6368ADC"
            "            SamplingFrequency = 1000"
            "            ConvertFrequency = 1000000"
            "            IsMultiplexed = 1"
            "            DeviceName = \"/dev/pxie-6363\""
            "            BoardId = 0"
            "            SampleDelayDivisor = 2"
            "            ConvertDivisor = 100"
            "            ExecutionMode = RealTimeThread"
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint64"
            "                    ResetOnBufferChange = 1"
            "                }"
            "                Time = {"
            "                    Type = uint64"
            "                }"
            "                ADC0_0 = {"
            "                   InputRange = 10"
            "                   Type = int16"
            "                   ChannelId = 0"
            "                   InputPolarity = Bipolar"
            "                   InputMode = RSE"
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

    ConfigurationDatabase cdb;
    StreamString configStream = configRealTimeThreadExecutionModeNoSync;
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
    ReferenceT<NI6368ADCTestGAM> testGAM;
    if (ok) {
        testGAM = application->Find("Functions.GAMA");
        ok = testGAM.IsValid();
    }
    ReferenceT<NI6368ADC> testADC;
    if (ok) {
        testADC = application->Find("Data.NI6368_0");
        ok = testADC.IsValid();
    }
    ReferenceT<GAMScheduler> scheduler;
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        uint64 *counter;
        testADC->GetSignalMemoryBuffer(0u, 0u, (void*&) counter);
        uint64 *time;
        testADC->GetSignalMemoryBuffer(1u, 0u, (void*&) time);
        int16 *adc0;
        testADC->GetSignalMemoryBuffer(2u, 0u, (void*&) adc0);

        ReferenceContainer inputBrokers;
        testGAM->GetInputBrokers(inputBrokers);

        application->PrepareNextState("State1");
        Sleep::MSec(500);
        for (uint32 i = 0u; i < inputBrokers.Size(); i++) {
            ReferenceT<BrokerI> broker = inputBrokers.Get(i);
            if (broker.IsValid()) {
                broker->Execute();
            }
        }

        printf("Counter %llu\n", *counter);
        printf("Time %llu\n", *time);
        printf("adc0[0] %d\n", adc0[0]);

        ok = (*counter < 600ull && *counter > 400ull);
        ok &= (*time < 600000 && *time > 400000ull);
    }
    god->Purge();
    return ok;
}
