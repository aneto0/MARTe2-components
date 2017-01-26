/**
 * @file NI6368ADCInputBrokerTest.cpp
 * @brief Source file for class NI6368ADCInputBrokerTest
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
 * the class NI6368ADCInputBrokerTest (public, protected, and private). Be aware that some
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
#include "NI6368ADCInputBroker.h"
#include "NI6368ADCInputBrokerTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class NI6368ADCInputBrokerTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

NI6368ADCInputBrokerTestGAM    () : GAM() {
        rms = 0;
        numberOfSamples = 0;
        signalPtr = NULL;
        counterPtr = NULL;
        timerPtr = NULL;
        totalSamples = 0;
    }

    ~NI6368ADCInputBrokerTestGAM() {
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
CLASS_REGISTER(NI6368ADCInputBrokerTestGAM, "1.0")

//Two channels base configuration
static const MARTe::char8 * const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = NI6368ADCInputBrokerTestGAM"
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
        "                   InputType = RSE"
        "                }"
        "                ADC1_0 = {"
        "                   InputRange = 10"
        "                   Type = int16"
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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool NI6368ADCInputBrokerTest::TestConstructor() {
    using namespace MARTe;
    NI6368ADCInputBroker ni6368ADCInputBroker;
    return true;
}

bool NI6368ADCInputBrokerTest::TestConstructor_NI6368ADC() {
    using namespace MARTe;
    NI6368ADC ni6368ADC;
    NI6368ADCInputBroker ni6368ADCInputBroker(&ni6368ADC);
    return true;
}

bool NI6368ADCInputBrokerTest::TestExecute() {
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
    ReferenceT<NI6368ADCInputBrokerTestGAM> testGAM;
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
        application->StopCurrentStateExecution();
    }
    god->Purge();
    return ok;
}

