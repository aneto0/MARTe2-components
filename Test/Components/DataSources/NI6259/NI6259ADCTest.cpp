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
    }

    ~NI6259TestGAM() {

    }

    bool Execute() {
        using namespace MARTe;
        uint32 numberOfSignals = GetNumberOfInputSignals();
        uint32 n;
        for(n=0u; n<numberOfSignals; n++) {
            float *signalPtr = reinterpret_cast<float *>(GetInputSignalMemory(n));
            printf("signal[%d][%d] = %f\n", n, 0, signalPtr[0]);
            //Sleep::Sec(1.0);
        }

        return true;
    }

    bool Setup() {
        return true;
    }

};
CLASS_REGISTER(NI6259TestGAM, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool NI6259ADCTest::TestConstructor() {
    using namespace MARTe;
    NI6259ADC ni6259ADC;
    return true;
}

bool NI6259ADCTest::TestIntegrated() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI6259TestGAM"
            "            InputSignals = {"
            "                ADC1_0 = {"
            "                    DataSource = NI6259_0"
            "                    Type = float32"
            "                    Samples = 1000"
            "                    Frequency = 500"
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
            "                   Type = float32"
            "                   ChannelId = 1"
            "                }"
            "                ADC2_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC3_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC4_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC6_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC7_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC8_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC9_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC10_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC11_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC12_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC13_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC14_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC15_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC16_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC17_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC18_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC19_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC20_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC21_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC22_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC23_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC24_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC25_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC26_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC27_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC28_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC29_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC30_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC31_0 = {"
            "                    Type = float32"
            "                }"
            "                ADC32_0 = {"
            "                    Type = float32"
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
    ReferenceT<GAMScheduler> scheduler;
    if(ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        application->PrepareNextState("State1");

        application->StartNextStateExecution();
        Sleep::Sec(100.0);
        application->StopCurrentStateExecution();
    }

    god->Purge();

    return ok;
}

