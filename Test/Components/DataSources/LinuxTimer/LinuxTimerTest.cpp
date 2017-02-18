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

    ReferenceT<LinuxTimer> linuxTimer = application->Find("Data.Timer");
    ReferenceT<LinuxTimerTestGAM> gama = application->Find("Functions.GAMA");
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
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timer = {"
        "            Class = LinuxTimer"
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
        "                Time2 = {"
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
    return !test.GetSignalMemoryBuffer(2, 0, (void *&) ptr);
}

bool LinuxTimerTest::TestGetBrokerName() {
    using namespace MARTe;
    LinuxTimer test;
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
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        application->StartNextStateExecution();
    }

    ReferenceT<LinuxTimer> linuxTimer = application->Find("Data.Timer");
    ReferenceT<LinuxTimerTestGAM> gama = application->Find("Functions.GAMA");
    ok = linuxTimer.IsValid();
    if (ok) {
        ok = gama.IsValid();
    }
    uint32 *counter;
    uint32 *timer;

    if (ok) {
        linuxTimer->GetSignalMemoryBuffer(0, 0, (void *&) counter);
        linuxTimer->GetSignalMemoryBuffer(1, 0, (void *&) timer);
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
    Sleep::MSec(1000);
    uint32 counterBefore = (*counter);
    uint32 timerBefore = (*timer);
    application->StopCurrentStateExecution();

    if (ok) {
        ok = (counterBefore > 1000) && (timerBefore > 1000000);
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
    return test.Initialise(cdb);
}

bool LinuxTimerTest::TestInitialise_False() {
    using namespace MARTe;
    LinuxTimer test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "False");
    return !test.Initialise(cdb);
}

bool LinuxTimerTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config3);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_One_Signal_Per_GAM() {
    return TestIntegratedInApplication(config8, true, false);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_MoreThan2Signals() {
    return !TestIntegratedInApplication(config4);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_No32BitsSignal1() {
    return !TestIntegratedInApplication(config5);
}

bool LinuxTimerTest::TestSetConfiguredDatabase_False_No32BitsSignal2() {
    return !TestIntegratedInApplication(config6);
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
