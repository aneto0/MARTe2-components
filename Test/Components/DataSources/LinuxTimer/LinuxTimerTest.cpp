/**
 * @file LinuxTimerTest.cpp
 * @brief Source file for class LinuxTimerTest
 * @date 25/10/2016
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
 * the class LinuxTimerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "LinuxTimer.h"
#include "LinuxTimerTest.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class LinuxTimerTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

LinuxTimerTestGAM    () : GAM() {
        val1 = 0u;
        val2 = 0u;
    }

    ~LinuxTimerTestGAM() {

    }

    bool Execute() {
        if(GetNumberOfInputSignals() > 0u) {
            MARTe::MemoryOperationsHelper::Copy(&val1, GetInputSignalMemory(0u), sizeof(MARTe::uint32));
        }
        if(GetNumberOfInputSignals() > 1u) {
            MARTe::MemoryOperationsHelper::Copy(&val2, GetInputSignalMemory(1u), sizeof(MARTe::uint32));
        }

        return true;
    }

    bool Setup() {
        return true;
    }

    MARTe::uint32 val1;

    MARTe::uint32 val2;
};
CLASS_REGISTER(LinuxTimerTestGAM, "1.0")

/**
 * Starts a MARTe application that uses this driver instance.
 */
static bool TestIntegratedInApplication(const MARTe::char8 * const config, bool checkVal1 = true, bool checkVal2 = true) {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config;
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

    ReferenceT<LinuxTimer> linuxTimer; 
    ReferenceT<LinuxTimerTestGAM> gama;
    
    if(ok) {
        linuxTimer = application->Find("Data.Timer");
        gama = application->Find("Functions.GAMA");
    }

    if (ok) {
        ok = linuxTimer.IsValid();
    }
    if (ok) {
        ok = gama.IsValid();
    }
    if (ok) {
        uint32 *counter;
        uint32 *timer;
        linuxTimer->GetSignalMemoryBuffer(0, 0, (void *&) counter);
        linuxTimer->GetSignalMemoryBuffer(1, 0, (void *&) timer);
        uint32 c = 0;
        ok = false;
        while (c < 500 && !ok) {
            ok = (((*counter) > 10) && ((*timer) > 10000));
            if (ok && checkVal1) {
                ok = (gama->val1 > 10);
            }
            if (ok && checkVal2) {
                ok = (gama->val2 > 10000);
            }
            c++;
            if (!ok) {
                Sleep::MSec(10);
            }
        }
    }

    if(ok) {
        Sleep::MSec(1000);
    }

    if (ok) {
        application->StopCurrentStateExecution();
    }

    god->Purge();
    return ok;
}

//Standard configuration for testing
const MARTe::char8 * const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000"
        "                }"
        "                AbsoluteTime = {"
        "                    DataSource = Timer"
        "                    Type = uint64"
        "                }"
        "                DeltaTime = {"
        "                    DataSource = Timer"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
		"		 +GAMB = {"
		"		 	Class = ConstantGAM"
		"			OutputSignals = {"
		"				Signal1 = {"
		"					DataSource = LoggerSink"
		"					Type = uint32"
		"					Default = 1"
		"				}"
		"			}"
		"        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "        }"
		"		 +LoggerSink = {"
		"			 Class = LoggerDataSource"
		"		 }"
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
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMB}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Busy"
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

const MARTe::char8 * const config21 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Busy"
        "            SleepPercentage = 50"
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

const MARTe::char8 * const config3 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Default"
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
        "                    Functions = {GAMA, GAMB}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

const MARTe::char8 * const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint16"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                AbsTime = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                DeltaTime = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                TrigRephase = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                WrongSignal = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Default"
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

const MARTe::char8 * const config5 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint16"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000.0"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Default"
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

const MARTe::char8 * const config6 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000.0"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint16"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Default"
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

const MARTe::char8 * const config7 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Default"
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

const MARTe::char8 * const config8 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Default"
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
        "                    Functions = {GAMA, GAMB, GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

const MARTe::char8 * const config9 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = float32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000.0"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Default"
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

const MARTe::char8 * const config10 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = float32"
        "                    Frequency = 1000.0"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Default"
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

//Standard configuration for testing in the context of the real-time thread
const MARTe::char8 * const config11 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            ExecutionMode = RealTimeThread"
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

//Full 4 signal configuration for testing in the context of the real-time thread
const MARTe::char8 * const config12 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000"
        "                }"
        "                AbsTime = {"
        "                   DataSource = Timer"
        "                   Type = uint64"
        "                }"
        "                DeltaTime = {"
        "                    DataSource = Timer"
        "                    Type = uint64"
        "                }"
        "                RephaseTrigger = {"
        "                    DataSource = Timer"
        "                    Type = uint8"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            ExecutionMode = RealTimeThread"
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

const MARTe::char8 * const config13 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            ExecutionMode = RealTimeThread"
		"			 +TimeProvider = {"
		"				 Class = HighResolutionTimeProvider"
		"			 }"
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

const MARTe::char8 * const config14 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            ExecutionMode = RealTimeThread"
		"			 +WrongTimeProvider = {"
		"				 Class = ReferenceContainer"
		"			 }"
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

const MARTe::char8 * const config15 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            ExecutionMode = RealTimeThread"
		"			 Phase = 500000"
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

const MARTe::char8 * const config16 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000.0"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                WrongTypeAbsolute = {"
        "                    DataSource = Timer"
        "                    Type = int16"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Default"
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

const MARTe::char8 * const config17 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000.0"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                AbsoluteTime = {"
        "                    DataSource = Timer"
        "                    Type = uint64"
        "                }"
        "                WrongDelta = {"
        "                    DataSource = Timer"
        "                    Type = int16"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Default"
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

const MARTe::char8 * const config18 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            ExecutionMode = RealTimeThread"
		"			 +TheGoodOne = {"
		"				 Class = HighResolutionTimeProvider"
		"			 }"
		"			 +TheOneInExcess = {"
		"				 Class = HighResolutionTimeProvider"
		"			 }"
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

const MARTe::char8 * const config19 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000.0"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                AbsoluteTime = {"
        "                    DataSource = Timer"
        "                    Type = uint64"
        "                }"
        "                DeltaTime = {"
        "                    DataSource = Timer"
        "                    Type = int64"
        "                }"
        "                WrongTriggerRephase = {"
        "                    DataSource = Timer"
        "                    Type = int64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Default"
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

const MARTe::char8 * const config20 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 5.0"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            +TimeProvider = {"
        "                Class = HighResolutionTimeProvider"
        "                SleepNature = Busy"
        "                SleepPercentage = 50"
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

const MARTe::char8 * const config31 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 5.0"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            +TimeProvider = {"
        "                Class = HighResolutionTimeProvider"
        "                SleepNature = InvalidSleepNature"
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

const MARTe::char8 * const config32 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 5.0"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            +TimeProvider = {"
        "                Class = HighResolutionTimeProvider"
        "                SleepNature = Busy"
        "                SleepPercentage = 200"
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

const MARTe::char8 * const config33 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 5.0"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            +TimeProvider = {"
        "                Class = HighResolutionTimeProvider"
        "                SleepNature = Busy"
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

const MARTe::char8 * const config34 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = LinuxTimerTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 5.0"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = Busy"
        "            +TimeProvider = {"
        "                Class = HighResolutionTimeProvider"
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
bool LinuxTimerTest::TestConstructor() {
    using namespace MARTe;
    LinuxTimer test;
    uint32 *counter;
    uint32 *timer;
    test.GetSignalMemoryBuffer(0, 0, (void *&) counter);
    test.GetSignalMemoryBuffer(1, 0, (void *&) timer);
    bool ok = (*counter == 0);
    ok &= (*timer == 0);
    return ok;
}

bool LinuxTimerTest::TestAllocateMemory() {
    using namespace MARTe;
    LinuxTimer test;
    return test.AllocateMemory();
}

bool LinuxTimerTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    LinuxTimer test;
    return (test.GetNumberOfMemoryBuffers() == 1u);
}

bool LinuxTimerTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    LinuxTimer test;
    uint32 *counter;
    uint32 *timer;
    test.GetSignalMemoryBuffer(0, 0, (void *&) counter);
    test.GetSignalMemoryBuffer(1, 0, (void *&) timer);
    bool ok = (*counter == 0);
    ok &= (*timer == 0);
    return ok;
}

bool LinuxTimerTest::TestGetSignalMemoryBuffer_False() {
    using namespace MARTe;
    LinuxTimer test;
    uint32 *ptr;
    return !test.GetSignalMemoryBuffer(5, 0, (void *&) ptr);
}

bool LinuxTimerTest::TestGetBrokerName() {
    using namespace MARTe;
    LinuxTimer test;
    ConfigurationDatabase config;
    StreamString brokerName = test.GetBrokerName(config, OutputSignals);

	REPORT_ERROR_STATIC(ErrorManagement::Information, "Detected %s as broker name", brokerName.Buffer());

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

bool LinuxTimerTest::TestGetInputBrokers() {
    return TestIntegratedInApplication(config1);
}

bool LinuxTimerTest::TestGetOutputBrokers() {
    return TestIntegratedInApplication(config1);
}

bool LinuxTimerTest::TestSynchronise() {
    return TestIntegratedInApplication(config1);
}

bool LinuxTimerTest::TestExecute() {
    return TestIntegratedInApplication(config1);
}

bool LinuxTimerTest::TestExecute_Busy() {
    return TestIntegratedInApplication(config2);
}

bool LinuxTimerTest::TestExecute_Busy_SleepPercentage() {
    return TestIntegratedInApplication(config21);
}

bool LinuxTimerTest::TestExecute_RTThread() {
    return TestIntegratedInApplication(config11);
}

bool LinuxTimerTest::TestExecute_RTThread_WithFive() {
    return TestIntegratedInApplication(config12);
}

bool LinuxTimerTest::TestExecute_RTThread_WithPhase() {
    return TestIntegratedInApplication(config15);
}

bool LinuxTimerTest::TestInitialise_ExplicitTimeProvider() {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config13;
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

	return ok;
}


bool LinuxTimerTest::TestInitialise_False_ExplicitWrongTimeProvider() {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config14;
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

	return !ok;
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_MoreThan1Provider() {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config18;
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

	return !ok;
}

bool LinuxTimerTest::TestPrepareNextState() {
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
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failure, application is not valid");
    }

    if (ok) {
        ok = application->ConfigureApplication();
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failure in configure application");
    }

    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failure in prepare State1");
    }

    if (ok) {
        application->StartNextStateExecution();
    }

    ReferenceT<LinuxTimer> linuxTimer; 
    ReferenceT<LinuxTimerTestGAM> gama; 

    if(ok) {
        gama = application->Find("Functions.GAMA");
        linuxTimer = application->Find("Data.Timer");
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failure finding GAMA and Timer instances");
    }

    ok = linuxTimer.IsValid() && gama.IsValid();

    if(!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "LinuxTimer and GAMA instances are not valid");
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "GAMA :%s and Timer instance :%s", gama.IsValid()?"true":"false", linuxTimer.IsValid()?"true":"false");
    }

    uint32 *counter;
    uint32 *timer;
	uint64 *absoluteTime;
	uint64 *deltaTime;
    uint32 counterBefore;
    uint32 timerBefore;

    if (ok) {
        linuxTimer->GetSignalMemoryBuffer(0, 0, (void *&) counter);
        linuxTimer->GetSignalMemoryBuffer(1, 0, (void *&) timer);
		linuxTimer->GetSignalMemoryBuffer(2, 0, (void *&) absoluteTime);
		linuxTimer->GetSignalMemoryBuffer(3, 0, (void *&) deltaTime);

        uint32 c = 0;
        ok = false;
        while (c < 500 && !ok) {
            ok = (((*counter) > 100) && ((*timer) > 100000) && (gama->val1 > 100) && (gama->val2 > 100000));
            c++;
            if (!ok) {
                Sleep::MSec(10);
            }
        }
    }
	
    if(ok) {
        Sleep::MSec(1000);
        counterBefore = (*counter);
        timerBefore = (*timer);
        ok = application->PrepareNextState("State2");
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failure in prepare State1");
    }

    if (ok) {
        application->StartNextStateExecution();
	    Sleep::MSec(10);
        application->StopCurrentStateExecution();    
        ok = (counterBefore > 1000) && (timerBefore > 1000000);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failure in prepare State2");
    }

    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failure in prepare State1");
    }

    if (ok) {
		//We are checking that the state change is resetting the counter and the timer
        ok = (((*counter) < counterBefore) && ((*timer) < timerBefore));
    }

    if(!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failure in check timer rewinding");
    }

    god->Purge();
    return ok;
}

bool LinuxTimerTest::TestInitialise_Empty() {
    using namespace MARTe;
    LinuxTimer test;
    ConfigurationDatabase cdb;
    return test.Initialise(cdb);
}

bool LinuxTimerTest::TestInitialise_Default() {
    using namespace MARTe;
    LinuxTimer test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "Default");
    return test.Initialise(cdb);
}

bool LinuxTimerTest::TestInitialise_Busy() {
    using namespace MARTe;
    LinuxTimer test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "Busy");
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = test.GetSleepPercentage() == 0;
    }
    return ok;
}

bool LinuxTimerTest::TestInitialise_Busy_SleepPercentage() {
    using namespace MARTe;
    LinuxTimer test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "Busy");
    cdb.Write("SleepPercentage", "50");
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = test.GetSleepPercentage() == 50;
    }
    return ok;
}

bool LinuxTimerTest::TestInitialise_Busy_SleepPercentage_gt_100() {
    using namespace MARTe;
    LinuxTimer test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "Busy");
    cdb.Write("SleepPercentage", "150");
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = test.GetSleepPercentage() == 100;
    }
    return ok;
}

bool LinuxTimerTest::TestInitialise_CPUMask() {
    using namespace MARTe;
    LinuxTimer test;
    ConfigurationDatabase cdb;
    uint32 cpuMask = 0x5;
    cdb.Write("CPUMask", cpuMask);
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (test.GetCPUMask() == cpuMask);
    }
    return ok;
}

bool LinuxTimerTest::TestInitialise_StackSize() {
    using namespace MARTe;
    LinuxTimer test;
    ConfigurationDatabase cdb;
    uint32 stackSize = 1024000;
    cdb.Write("StackSize", stackSize);
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (test.GetStackSize() == stackSize);
    }
    return ok;
}

bool LinuxTimerTest::TestGetCPUMask() {
    return TestInitialise_CPUMask();
}

bool LinuxTimerTest::TestGetStackSize() {
    return TestInitialise_StackSize();
}

bool LinuxTimerTest::TestInitialise_False_SleepNature() {
    using namespace MARTe;
    LinuxTimer test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "False");
    return !test.Initialise(cdb);
}

bool LinuxTimerTest::TestInitialise_False_ExecutionMode() {
    using namespace MARTe;
    LinuxTimer test;
    ConfigurationDatabase cdb;
    cdb.Write("ExecutionMode", "False");
    return !test.Initialise(cdb);
}

bool LinuxTimerTest::TestInitialise_False_StackSize() {
    using namespace MARTe;
    LinuxTimer test;
    ConfigurationDatabase cdb;
    cdb.Write("StackSize", 0);
    return !test.Initialise(cdb);
}

bool LinuxTimerTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config3);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_One_Signal_Per_GAM() {
    return TestIntegratedInApplication(config8, true, false);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_MoreThan5Signals() {
    return !TestIntegratedInApplication(config4);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_No32BitsSignal1() {
    return !TestIntegratedInApplication(config5);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_No32BitsSignal2() {
    return !TestIntegratedInApplication(config6);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_InvalidSignal3() {
    return !TestIntegratedInApplication(config16);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_InvalidSignal4() {
    return !TestIntegratedInApplication(config17);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_InvalidSignal5() {
    return !TestIntegratedInApplication(config19);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_NoFrequencySet() {
    return !TestIntegratedInApplication(config7);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_IntegerSignal1() {
    return !TestIntegratedInApplication(config9);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_IntegerSignal2() {
    return !TestIntegratedInApplication(config10);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_UseBusySleepAndPercentage() {
    return TestIntegratedInApplication(config20);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_InvalidSleepNature() {
    return !TestIntegratedInApplication(config31);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_ExplicitHRTWithMoreThan100Perc() {
    return TestIntegratedInApplication(config32);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_PureBusySleep() {
    return TestIntegratedInApplication(config33);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_WithBackwardCompatOnHRT() {
    return TestIntegratedInApplication(config34);
}

bool LinuxTimerTest::TestGetSleepPercentage() {
    return TestInitialise_Busy_SleepPercentage();
}

