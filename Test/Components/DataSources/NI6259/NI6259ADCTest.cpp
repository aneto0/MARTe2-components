/**
 * @file NI6259ADCTest.cpp
 * @brief Source file for class NI6259ADCTest
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
 * the class NI6259ADCTest (public, protected, and private). Be aware that some 
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
#include "NI6259ADC.h"
#include "NI6259ADCTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class NI6259TestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

NI6259TestGAM    () : GAM() {
        rms = 0;
        numberOfSamples = 0;
        signalPtr = NULL;
        counterPtr = NULL;
        timerPtr = NULL;
        totalSamples = 0;
    }

    ~NI6259TestGAM() {
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
        signalPtr = reinterpret_cast<float *>(GetInputSignalMemory(2));
        GetSignalNumberOfSamples(InputSignals, 2u, numberOfSamples);
        return true;
    }

    MARTe::float32 rms;
    MARTe::uint32 *counterPtr;
    MARTe::uint32 *timerPtr;
    MARTe::float32 *signalPtr;
    MARTe::uint32 numberOfSamples;
    MARTe::uint32 totalSamples;

};
CLASS_REGISTER(NI6259TestGAM, "1.0")

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
    ReferenceT<NI6259TestGAM> testGAM;
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
                               const MARTe::char8 * const parameterToPatch, const MARTe::char8 * const valueToPatch, pxi6259_ai_conf_t *conf) {
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
    ReferenceT<NI6259ADC> ni6259ADC = application->Find("Data.NI6259_0");
    if (ok) {
        ok = ni6259ADC.IsValid();
    }
    if (ok) {
        ni6259ADC->ReadAIConfiguration(conf);
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
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 10000"
        "                    Frequency = 50"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
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
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 1"
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

//One channel base configuration
static const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 10000"
        "                    Frequency = 100"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
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

//All channels base configuration
static const MARTe::char8 * const config3 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 3125"
        "                    Frequency = 10"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 1"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC2_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 2"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC3_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 3"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC4_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 4"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC5_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 5"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC6_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 6"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC7_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 7"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC8_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 8"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC9_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 9"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC10_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 10"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC11_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 11"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC12_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 12"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC13_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 13"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC14_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 14"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC15_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 15"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC16_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 16"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC17_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 17"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC18_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 18"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC19_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 19"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC20_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 20"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC21_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 21"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC22_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 22"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC23_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 23"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC24_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 24"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC25_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 25"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC26_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 26"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC27_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 27"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC28_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 28"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC29_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 29"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC30_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 30"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC31_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 31"
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

//All channels base configuration with no default parameters
static const MARTe::char8 * const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 3125"
        "                    Frequency = 10"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 1"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC2_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 2"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC3_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 3"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC4_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 4"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC5_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 5"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC6_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 6"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC7_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 7"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC8_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 8"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC9_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 9"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC10_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 10"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC11_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 11"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC12_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 12"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC13_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 13"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC14_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 14"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC15_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 15"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC16_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 16"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC17_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 17"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC18_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 18"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC19_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 19"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC20_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 20"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC21_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 21"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC22_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 22"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC23_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 23"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC24_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 24"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC25_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 25"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC26_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 26"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC27_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 27"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC28_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 28"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC29_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 29"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC30_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 30"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
        "                }"
        "                ADC31_0 = {"
        "                   InputRange = 5"
        "                   Type = float32"
        "                   ChannelId = 31"
        "                   InputPolarity = Bipolar"
        "                   InputMode = NRSE"
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
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 10000"
        "                    Frequency = 50"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                ADC1_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 10000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 1"
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
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
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
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 10000"
        "                    Frequency = 50"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            CPUMask = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = float32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 1"
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

//Two channels base configuration with a wrong counter type
static const MARTe::char8 * const config8 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 10000"
        "                    Frequency = 50"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
        "            CPUMask = 0xf"
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = float32"
        "                }"
        "                ADC0_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 1"
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

//Two channels base configuration with a wrong signal type
static const MARTe::char8 * const config9 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = uint32"
        "                    Samples = 10000"
        "                    Frequency = 50"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
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
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 1"
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

//Two channels base configuration with a number of samples mismatch
static const MARTe::char8 * const config10 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 10000"
        "                    Frequency = 50"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                ADC1_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 5000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
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
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 1"
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
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 10000"
        "                    Frequency = 50"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/tmp/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
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
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 1"
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

//Two channels base configuration with a wrong configuration of the frequency vs samples
static const MARTe::char8 * const config12 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 10000"
        "                    Frequency = 100"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
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
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 1"
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

//Two channels base configuration with more than one sample for the counter
static const MARTe::char8 * const config13 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                    Samples = 2"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 10000"
        "                    Frequency = 50"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
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
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 1"
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

//Two channels base configuration with more than one sample for the timer
static const MARTe::char8 * const config14 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6259TestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = NI6259_0"
        "                }"
        "                Time = {"
        "                    DataSource = NI6259_0"
        "                    Samples = 2"
        "                }"
        "                ADC0_0 = {"
        "                    DataSource = NI6259_0"
        "                    Type = float32"
        "                    Samples = 10000"
        "                    Frequency = 50"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +NI6259_0 = {"
        "            Class = NI6259ADC"
        "            SamplingFrequency = 1000000"
        "            DeviceName = \"/dev/pxi6259\""
        "            BoardId = 0"
        "            DelayDivisor = 3"
        "            ClockSource = \"SI_TC\""
        "            ClockPolarity = \"ACTIVE_HIGH_OR_RISING_EDGE\""
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
        "                   Type = float32"
        "                   ChannelId = 0"
        "                   InputPolarity = Bipolar"
        "                   InputMode = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = float32"
        "                   ChannelId = 1"
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
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool NI6259ADCTest::TestConstructor() {
    using namespace MARTe;
    NI6259ADC ni6259ADC;
    return true;
}

bool NI6259ADCTest::TestAllocateMemory() {
    using namespace MARTe;
    NI6259ADC ni6259ADC;
    return ni6259ADC.AllocateMemory();
}

bool NI6259ADCTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    NI6259ADC ni6259ADC;
    return (ni6259ADC.GetNumberOfMemoryBuffers() == 1u);
}

bool NI6259ADCTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    NI6259ADC test;
    uint32 *counter;
    uint32 *timer;
    float32 *adc0;
    float32 *adc31;
    test.GetSignalMemoryBuffer(0, 0, (void *&) counter);
    test.GetSignalMemoryBuffer(1, 0, (void *&) timer);
    test.GetSignalMemoryBuffer(2, 0, (void *&) adc0);
    test.GetSignalMemoryBuffer(33, 0, (void *&) adc31);
    bool ok = (*counter == 0);
    ok &= (*timer == 0);
    ok &= (adc0 == NULL);
    ok &= (adc31 == NULL);
    return ok;
}

bool NI6259ADCTest::TestGetSignalMemoryBuffer_False() {
    using namespace MARTe;
    NI6259ADC test;
    uint32 *ptr;
    return !test.GetSignalMemoryBuffer(NI6259ADC_HEADER_SIZE + NI6259ADC_MAX_CHANNELS, 0, (void *&) ptr);
}

bool NI6259ADCTest::TestGetBrokerName() {
    using namespace MARTe;
    NI6259ADC test;
    ConfigurationDatabase config;
    StreamString brokerName = test.GetBrokerName(config, OutputSignals);
    bool ok = (brokerName == "");
    if (ok) {
        brokerName = test.GetBrokerName(config, InputSignals);
        ok = (brokerName == "MemoryMapInputBroker");
    }
    if (ok) {
        config.Write("Frequency", 10);
        brokerName = test.GetBrokerName(config, InputSignals);
        ok = (brokerName == "MemoryMapSynchronisedInputBroker");
    }

    return ok;
}

bool NI6259ADCTest::TestGetInputBrokers() {
    return TestIntegratedInApplication(config1);
}

bool NI6259ADCTest::TestGetInputBrokers_NotSynchronisingGAM() {
    return TestIntegratedInApplication(config5);
}

bool NI6259ADCTest::TestGetOutputBrokers() {
    using namespace MARTe;
    NI6259ADC test;
    ReferenceContainer rc;
    return !test.GetOutputBrokers(rc, "", NULL);
}

bool NI6259ADCTest::TestSynchronise() {
    return TestIntegratedInApplication(config1);
}

bool NI6259ADCTest::TestExecute() {
    return TestIntegratedInApplication(config1);
}

bool NI6259ADCTest::TestPrepareNextState() {
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

    ReferenceT<NI6259ADC> ni6259ADC = application->Find("Data.NI6259_0");
    if (ok) {
        ok = ni6259ADC.IsValid();
    }
    uint32 *counter;
    uint32 *timer;

    if (ok) {
        ni6259ADC->GetSignalMemoryBuffer(0, 0, (void *&) counter);
        ni6259ADC->GetSignalMemoryBuffer(1, 0, (void *&) timer);
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

bool NI6259ADCTest::TestInitialise() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    NI6259ADC test;
    if (ok) {
        ok = test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_False_NoSamplingFrequency() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("SamplingFrequency");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_False_BadSamplingFrequency() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("SamplingFrequency");
    cdb.Write("SamplingFrequency", 1000001);
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_False_BadZeroSamplingFrequency() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("SamplingFrequency");
    cdb.Write("SamplingFrequency", 0);
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_False_NoDeviceName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("DeviceName");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_False_NoBoardId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("BoardId");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_False_NoDelayDivisor() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("DelayDivisor");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_ClockSource() {
    using namespace MARTe;
    bool ok = true;
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "SI_TC", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_SI_TC);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI0", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI0);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI1", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI1);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI2", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI2);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI3", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI3);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI4", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI4);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI5", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI5);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI6", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI6);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI7", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI7);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI8", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI8);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI9", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI9);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI10", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI10);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI11", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI11);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI12", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI12);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI13", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI13);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI14", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI14);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PFI15", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PFI15);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "RTSI0", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_RTSI0);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "RTSI1", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_RTSI1);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "RTSI2", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_RTSI2);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "RTSI3", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_RTSI3);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "RTSI4", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_RTSI4);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "RTSI5", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_RTSI5);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "RTSI6", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_RTSI6);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "RTSI7", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_RTSI7);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "PULSE", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_PULSE);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "GPCRT0_OUT", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_GPCRT0_OUT);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "STAR_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_STAR_TRIGGER);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "GPCTR1_OUT", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_GPCTR1_OUT);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "SCXI_TRIG1", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_SCXI_TRIG1);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "ANALOG_TRIGGER", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_ANALOG_TRIGGER);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockSource", "LOW", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkSource == AI_SAMPLE_SELECT_LOW);
        }
    }

    return ok;
}

bool NI6259ADCTest::TestInitialise_False_NoClockSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("ClockSource");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_False_BadClockSource() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("ClockSource");
    cdb.Write("ClockSource", "INVALID");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_ClockPolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockPolarity", "ACTIVE_HIGH_OR_RISING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkPolarity == AI_SAMPLE_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0", "ClockPolarity", "ACTIVE_LOW_OR_FALLING_EDGE", &conf);
        }
        if (ok) {
            ok = (conf.aiSampleClkPolarity == AI_SAMPLE_POLARITY_ACTIVE_LOW_OR_FALLING_EDGE);
        }
    }

    return ok;
}

bool NI6259ADCTest::TestInitialise_False_NoClockPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("ClockPolarity");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_False_BadClockPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("ClockPolarity");
    cdb.Write("ClockPolarity", "INVALID");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_False_NoSignals() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    cdb.Delete("Signals");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_InputRange() {
    using namespace MARTe;
    bool ok = true;
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputRange", "10", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].gain == 1);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputRange", "5", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].gain == 2);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputRange", "2", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].gain == 3);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputRange", "1", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].gain == 4);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputRange", "0.5", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].gain == 5);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputRange", "0.2", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].gain == 6);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputRange", "0.1", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].gain == 7);
        }
    }

    return ok;
}

bool NI6259ADCTest::TestInitialise_False_BadInputRange() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0.Signals.ADC0_0");
    cdb.Delete("InputRange");
    cdb.Write("InputRange", "20");
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_DefaultInputRange() {
    bool ok = true;
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputRange", NULL, &conf);
        }
        if (ok) {
            ok = (conf.channel[0].gain == 1);
        }
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_InputPolarity() {
    using namespace MARTe;
    bool ok = true;
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputPolarity", "Unipolar", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].polarity == AI_POLARITY_UNIPOLAR);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputPolarity", "Bipolar", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].polarity == AI_POLARITY_BIPOLAR);
        }
    }

    return ok;
}

bool NI6259ADCTest::TestInitialise_False_BadInputPolarity() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0.Signals.ADC0_0");
    cdb.Delete("InputPolarity");
    cdb.Write("InputPolarity", "INVALID");
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_DefaultInputPolarity() {
    bool ok = true;
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputPolarity", NULL, &conf);
        }
        if (ok) {
            ok = (conf.channel[0].polarity == AI_POLARITY_UNIPOLAR);
        }
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_InputMode() {
    using namespace MARTe;
    bool ok = true;
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputMode", "Differential", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].channelType == AI_CHANNEL_TYPE_DIFFERENTIAL);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputMode", "NRSE", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].channelType == AI_CHANNEL_TYPE_NRSE);
        }
    }
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputMode", "RSE", &conf);
        }
        if (ok) {
            ok = (conf.channel[0].channelType == AI_CHANNEL_TYPE_RSE);
        }
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_False_BadInputMode() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0.Signals.ADC0_0");
    cdb.Delete("InputMode");
    cdb.Write("InputMode", "INVALID");
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_DefaultInputMode() {
    bool ok = true;
    {
        pxi6259_ai_conf_t conf;
        if (ok) {
            ok = PatchConfiguration(config1, "$Test.+Data.+NI6259_0.Signals.ADC0_0", "InputMode", NULL, &conf);
        }
        if (ok) {
            ok = (conf.channel[0].channelType == AI_CHANNEL_TYPE_RSE);
        }
    }
    return ok;
}

bool NI6259ADCTest::TestInitialise_False_BadChannelId() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0.Signals.ADC0_0");
    cdb.Delete("ChannelId");
    cdb.Write("ChannelId", "32");
    cdb.MoveAbsolute("$Test.+Data.+NI6259_0");
    NI6259ADC test;
    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool NI6259ADCTest::TestSetConfiguredDatabase_OneChannel() {
    return TestIntegratedInApplication(config2);
}

bool NI6259ADCTest::TestSetConfiguredDatabase_AllChannels() {
    return TestIntegratedInApplication(config3);
}

bool NI6259ADCTest::TestSetConfiguredDatabase_AllChannels_Parameters() {
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

    ReferenceT<NI6259ADC> ni6259ADC = application->Find("Data.NI6259_0");
    if (ok) {
        ok = ni6259ADC.IsValid();
    }
    if (ok) {
        pxi6259_ai_conf_t conf;
        ni6259ADC->ReadAIConfiguration(&conf);
        uint32 i;
        for (i = 0; (i < NI6259ADC_MAX_CHANNELS) && (ok); i++) {
            ok = (conf.channel[i].channelType == AI_CHANNEL_TYPE_NRSE);
            ok &= (conf.channel[i].gain == 2);
            ok &= (conf.channel[i].polarity == AI_POLARITY_BIPOLAR);
            ok &= (conf.channel[i].enabled);
        }
    }

    god->Purge();
    return ok;
}

bool NI6259ADCTest::TestSetConfiguredDatabase_False_NoChannels() {
    return !TestIntegratedInApplication(config6);
}

bool NI6259ADCTest::TestSetConfiguredDatabase_False_CounterTypeMismatch() {
    return !TestIntegratedInApplication(config7);
}

bool NI6259ADCTest::TestSetConfiguredDatabase_False_TimerTypeMismatch() {
    return !TestIntegratedInApplication(config8);
}

bool NI6259ADCTest::TestSetConfiguredDatabase_False_SignalTypeMismatch() {
    return !TestIntegratedInApplication(config9);
}

bool NI6259ADCTest::TestSetConfiguredDatabase_False_SignalNumberOfSamplesMismatch() {
    return !TestIntegratedInApplication(config10);
}

bool NI6259ADCTest::TestSetConfiguredDatabase_False_WrongDeviceName() {
    return !TestIntegratedInApplication(config11);
}

bool NI6259ADCTest::TestSetConfiguredDatabase_False_WrongFrequencyVsSamples() {
    return !TestIntegratedInApplication(config12);
}

bool NI6259ADCTest::TestSetConfiguredDatabase_False_CounterSamples() {
    return !TestIntegratedInApplication(config13);
}

bool NI6259ADCTest::TestSetConfiguredDatabase_False_TimerSamples() {
    return !TestIntegratedInApplication(config14);
}

bool NI6259ADCTest::TestIntegrated() {
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
    ReferenceT<NI6259TestGAM> testGAM;
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
        float32 rms = testGAM->rms;
        if (testGAM->totalSamples > 0) {
            rms = sqrtf(rms / testGAM->totalSamples);
        }
        printf("--->>>RMS = %f\n", rms);
        application->StopCurrentStateExecution();
    }
    god->Purge();
    return ok;
}

