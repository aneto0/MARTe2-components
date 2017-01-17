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
#include "../NI6368/NI6368ADCTest.h"

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
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

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
        signalPtr = reinterpret_cast<uint16 *>(GetInputSignalMemory(2));
        GetSignalNumberOfSamples(InputSignals, 2u, numberOfSamples);
        return true;
    }

    MARTe::uint16 rms;
    MARTe::uint32 *counterPtr;
    MARTe::uint32 *timerPtr;
    MARTe::uint16 *signalPtr;
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
        "                    Type = uint16"
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
        "                   Type = uint16"
        "                   ChannelId = 0"
        "                   InputType = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 1"
        "                   InputType = RSE"
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
        "                    Type = uint16"
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
        "                   Type = uint16"
        "                   ChannelId = 0"
        "                   InputType = RSE"
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
        "                    Type = uint16"
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
        "                   Type = uint16"
        "                   ChannelId = 0"
        "                   InputType = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 1"
        "                   InputType = RSE"
        "                }"
        "                ADC2_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 2"
        "                   InputType = RSE"
        "                }"
        "                ADC3_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 3"
        "                   InputType = RSE"
        "                }"
        "                ADC4_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 4"
        "                   InputType = RSE"
        "                }"
        "                ADC5_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 5"
        "                   InputType = RSE"
        "                }"
        "                ADC6_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 6"
        "                   InputType = RSE"
        "                }"
        "                ADC7_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 7"
        "                   InputType = RSE"
        "                }"
        "                ADC8_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 8"
        "                   InputType = RSE"
        "                }"
        "                ADC9_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 9"
        "                   InputType = RSE"
        "                }"
        "                ADC10_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 10"
        "                   InputType = RSE"
        "                }"
        "                ADC11_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 11"
        "                   InputType = RSE"
        "                }"
        "                ADC12_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 12"
        "                   InputType = RSE"
        "                }"
        "                ADC13_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 13"
        "                   InputType = RSE"
        "                }"
        "                ADC14_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 14"
        "                   InputType = RSE"
        "                }"
        "                ADC15_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 15"
        "                   InputType = RSE"
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
        "                    Type = uint16"
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
        "                   Type = uint16"
        "                   ChannelId = 0"
        "                   InputType = NRSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 1"
        "                   InputType = NRSE"
        "                }"
        "                ADC2_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 2"
        "                   InputType = NRSE"
        "                }"
        "                ADC3_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 3"
        "                   InputType = NRSE"
        "                }"
        "                ADC4_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 4"
        "                   InputType = NRSE"
        "                }"
        "                ADC5_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 5"
        "                   InputType = NRSE"
        "                }"
        "                ADC6_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 6"
        "                   InputType = NRSE"
        "                }"
        "                ADC7_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 7"
        "                   InputType = NRSE"
        "                }"
        "                ADC8_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 8"
        "                   InputType = NRSE"
        "                }"
        "                ADC9_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 9"
        "                   InputType = NRSE"
        "                }"
        "                ADC10_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 10"
        "                   InputType = NRSE"
        "                }"
        "                ADC11_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 11"
        "                   InputType = NRSE"
        "                }"
        "                ADC12_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 12"
        "                   InputType = NRSE"
        "                }"
        "                ADC13_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 13"
        "                   InputType = NRSE"
        "                }"
        "                ADC14_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 14"
        "                   InputType = NRSE"
        "                }"
        "                ADC15_0 = {"
        "                   InputRange = 5"
        "                   Type = uint16"
        "                   ChannelId = 15"
        "                   InputType = NRSE"
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

//Two channels base configuration including a not synchronising GAM
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
        "                    Type = uint16"
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
        "                    Type = uint16"
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
        "                   Type = uint16"
        "                   ChannelId = 0"
        "                   InputType = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 1"
        "                   InputType = RSE"
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
        "                    Type = uint16"
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
        "                   Type = uint16"
        "                   ChannelId = 0"
        "                   InputType = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 1"
        "                   InputType = RSE"
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
        "                    Type = uint16"
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
        "                   Type = uint16"
        "                   ChannelId = 0"
        "                   InputType = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 1"
        "                   InputType = RSE"
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
        "                   InputType = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = uint32"
        "                   ChannelId = 1"
        "                   InputType = RSE"
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
        "                    Type = uint16"
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
        "                    Type = uint16"
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
        "                   Type = uint16"
        "                   ChannelId = 0"
        "                   InputType = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 1"
        "                   InputType = RSE"
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
        "                    Type = uint16"
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
        "                   Type = uint16"
        "                   ChannelId = 0"
        "                   InputType = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 1"
        "                   InputType = RSE"
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
        "                    Type = uint16"
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
        "                   Type = uint16"
        "                   ChannelId = 0"
        "                   InputType = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 1"
        "                   InputType = RSE"
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
        "                    Type = uint16"
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
        "                   Type = uint16"
        "                   ChannelId = 0"
        "                   InputType = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
        "                   ChannelId = 1"
        "                   InputType = RSE"
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
        "                    Type = uint16"
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
        "                   Type = uint16"
        "                   ChannelId = 0"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = uint16"
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
    return (ni6368ADC.GetNumberOfMemoryBuffers() == 2u);
}

bool NI6368ADCTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    NI6368ADC test;
    uint32 *counter;
    uint32 *timer;
    uint16 *adc0;
    uint16 *adc16;
    test.GetSignalMemoryBuffer(0, 0, (void *&) counter);
    test.GetSignalMemoryBuffer(1, 0, (void *&) timer);
    test.GetSignalMemoryBuffer(2, 0, (void *&) adc0);
    test.GetSignalMemoryBuffer(16, 0, (void *&) adc16);
    bool ok = (*counter == 0);
    ok &= (*timer == 0);
    ok &= (adc0 == NULL);
    ok &= (adc16 == NULL);
    return ok;
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

bool NI6368ADCTest::TestGetInputBrokers_NotSynchronisingGAM() {
    return TestIntegratedInApplication(config5);
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

bool NI6368ADCTest::TestInitialise_False_NoDMABufferSize() {
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
        ok = !test.Initialise(cdb);
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

bool NI6368ADCTest::TestInitialise_False_NoClockSampleSource() {
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
        ok = !test.Initialise(cdb);
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

bool NI6368ADCTest::TestInitialise_False_NoClockSamplePolarity() {
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
        ok = !test.Initialise(cdb);
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

bool NI6368ADCTest::TestInitialise_False_NoClockConvertSource() {
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
        ok = !test.Initialise(cdb);
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

bool NI6368ADCTest::TestInitialise_False_NoClockConvertPolarity() {
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
        ok = !test.Initialise(cdb);
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
            ok = (conf.aichan_conf[0].gain == 1);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputRange", "5", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].gain == 2);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputRange", "2", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].gain == 3);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputRange", "1", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].gain == 4);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputRange", "0.5", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].gain == 5);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputRange", "0.2", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].gain == 6);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputRange", "0.1", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].gain == 7);
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
            ok = (conf.aichan_conf[0].gain == 1);
        }
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_InputType() {
    using namespace MARTe;
    bool ok = true;
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputType", "Differential", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].channel_type == XSERIES_AI_CHANNEL_TYPE_DIFFERENTIAL);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputType", "NRSE", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].channel_type == XSERIES_AI_CHANNEL_TYPE_NRSE);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputType", "RSE", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].channel_type == XSERIES_AI_CHANNEL_TYPE_RSE);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputType", "Loopback", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].channel_type == XSERIES_AI_CHANNEL_TYPE_LOOPBACK);
        }
    }
    {
        xseries_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6368_0.Signals.ADC0_0", "InputType", "Internal", &conf);
        }
        if (ok) {
            ok = (conf.aichan_conf[0].channel_type == XSERIES_AI_CHANNEL_TYPE_INTERNAL);
        }
    }
    return ok;
}

bool NI6368ADCTest::TestInitialise_False_BadInputType() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0.Signals.ADC0_0");
    cdb.Delete("InputType");
    cdb.Write("InputType", "INVALID");
    cdb.MoveAbsolute("$Test.+Data.+NI6368_0");
    NI6368ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
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
            ok = (conf.aichan_conf[0].channel_type == XSERIES_AI_CHANNEL_TYPE_RSE);
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
            ok = (conf.aichan_conf[i].channel_type == XSERIES_AI_CHANNEL_TYPE_RSE);
            ok &= (conf.aichan_conf[i].gain == 2);
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

bool NI6368ADCTest::TestSetConfiguredDatabase_False_WrongFrequencyVsSamples() {
    return !TestIntegratedInApplication(config12);
}

bool NI6368ADCTest::TestSetConfiguredDatabase_False_CounterSamples() {
    return !TestIntegratedInApplication(config13);
}

bool NI6368ADCTest::TestSetConfiguredDatabase_False_TimerSamples() {
    return !TestIntegratedInApplication(config14);
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
        while (testGAM->totalSamples < 100000)
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

