/**
 * @file TcnTimeProviderTest.cpp
 * @brief Source file for class TcnTimeProviderTest
 * @date 19/03/2021
 * @author Giuseppe Avon
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
 * the class TcnTimeProviderTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "LinuxTimer.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "TcnTimeProviderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

const MARTe::char8 * const configTestIntegratedNoPoll = ""
    "$Test = {"
    "    Class = RealTimeApplication"
    "    +Functions = {"
    "        Class = ReferenceContainer"
    "        +GAMA = {"
    "            Class = TcnTimeProviderTestGAM"
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
    "            TcnPoll = 0"
    "            +TimeProvider = {"
    "               Class = TcnTimeProvider"
    "               TcnDevice = \"/etc/opt/codac/tcn/tcn-default.xml\""
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

const MARTe::char8 * const configTestIntegratedPoll = ""
    "$Test = {"
    "    Class = RealTimeApplication"
    "    +Functions = {"
    "        Class = ReferenceContainer"
    "        +GAMA = {"
    "            Class = TcnTimeProviderTestGAM"
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
    "            TcnPoll = 1"
    "            TcnFrequency = 10000000"
    "            +TimeProvider = {"
    "               Class = TcnTimeProvider"
    "               TcnDevice = \"/etc/opt/codac/tcn/tcn-default.xml\""
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

class TcnTimeProviderTestGAM: public MARTe::GAM {
    public:
        CLASS_REGISTER_DECLARATION()

        TcnTimeProviderTestGAM    () : GAM() {
            val1 = 0u;
            val2 = 0u;
        }

        ~TcnTimeProviderTestGAM() {

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

CLASS_REGISTER(TcnTimeProviderTestGAM, "1.0")


bool TcnTimeProviderTest::PreInitialise(bool noPreInit) {
    bool retVal = true;
    timeProvider = new MARTe::TcnTimeProvider();

    retVal = tcnCfg.Write("TcnDevice", "/etc/opt/codac/tcn/tcn-default.xml");

    if(retVal && !noPreInit) {
        retVal = timeProvider->Initialise(tcnCfg);
    }

    return retVal; 
}

TcnTimeProviderTest::TcnTimeProviderTest(bool noPreInit) : TimeProviderTest() {
    PreInitialise(noPreInit);
}

TcnTimeProviderTest::TcnTimeProviderTest() : TimeProviderTest() {
    PreInitialise(false);   
}

TcnTimeProviderTest::~TcnTimeProviderTest() {

}

bool TcnTimeProviderTest::TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode mode) {
   
    StreamString operationModeString;
    operationModeString.Seek(0);

    bool skipOperationModeSet = false;

    switch(mode) {
        case TcnTimeProviderTestInitialiseMode_NoPollLegacyMode:
            operationModeString.Printf("%s", "NoPollLegacyMode");
            break;
        case TcnTimeProviderTestInitialiseMode_PollLegacyMode:
            operationModeString.Printf("%s", "PollLegacyMode");
            break;
        case TcnTimeProviderTestInitialiseMode_WaitUntilMode:
            operationModeString.Printf("%s", "WaitUntilMode");
            break;
        case TcnTimeProviderTestInitialiseMode_WaitUntilHRMode:
            operationModeString.Printf("%s", "WaitUntilHRMode");
            break;
        case TcnTimeProviderTestInitialiseMode_SleepMode:
            operationModeString.Printf("%s", "SleepMode");
            break;
        case TcnTimeProviderTestInitialiseMode_SleepHRMode:
            operationModeString.Printf("%s", "SleepHRMode");
            break;
        case TcnTimeProviderTestInitialiseMode_InvalidMode:
            operationModeString.Printf("%s", "InvalidMode");
            break;
        case TcnTimeProviderTestInitialiseMode_LegacyTcnPollTrue:
            tcnCfg.Write("TcnPoll", 1);
            skipOperationModeSet = true;
            break;
        case TcnTimeProviderTestInitialiseMode_LegacyTcnPollFalse:
            tcnCfg.Write("TcnPoll", 0);
            skipOperationModeSet = true;
            break;
    }

    if(!skipOperationModeSet) {
        tcnCfg.Write("OperationMode", operationModeString.Buffer());
    }

    bool retVal = timeProvider->Initialise(tcnCfg);

    if(retVal) {
        bool testCounterRes = TestCounter();
        bool testPeriodRes = TestPeriod();
        bool testFrequencyRes = TestFrequency();
        bool testSleepRes = TestSleep();

        REPORT_ERROR_STATIC(testCounterRes?ErrorManagement::Information:ErrorManagement::FatalError, "Counter [%s]", testCounterRes?"passed":"failed");
        REPORT_ERROR_STATIC(testPeriodRes?ErrorManagement::Information:ErrorManagement::FatalError, "Period [%s]", testPeriodRes?"passed":"failed");
        REPORT_ERROR_STATIC(testFrequencyRes?ErrorManagement::Information:ErrorManagement::FatalError, "Frequency [%s]", testFrequencyRes?"passed":"failed");
        REPORT_ERROR_STATIC(testSleepRes?ErrorManagement::Information:ErrorManagement::FatalError, "Sleep [%s]", testSleepRes?"passed":"failed");

        retVal = testCounterRes && testPeriodRes && testFrequencyRes && testSleepRes;
    }

    return retVal;
}

bool TcnTimeProviderTest::TestInitialise_LegacyTcnPollTrue() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_LegacyTcnPollTrue);
}

bool TcnTimeProviderTest::TestInitialise_LegacyTcnPollFalse() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_LegacyTcnPollFalse);
}

bool TcnTimeProviderTest::TestInitialise_NoPollLegacyMode() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_NoPollLegacyMode);
}

bool TcnTimeProviderTest::TestInitialise_PollLegacyMode() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_PollLegacyMode);
}

bool TcnTimeProviderTest::TestInitialise_WaitUntilMode() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_WaitUntilMode);
}

bool TcnTimeProviderTest::TestInitialise_WaitUntilHRMode() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_WaitUntilHRMode);
}

bool TcnTimeProviderTest::TestInitialise_SleepMode() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_SleepMode);
}

bool TcnTimeProviderTest::TestInitialise_SleepHRMode() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_SleepHRMode);
}

bool TcnTimeProviderTest::TestInitialise_InvalidMode_Fail() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_InvalidMode);
}

bool TcnTimeProviderTest::TestInitialise_WithTolerance() {
    tcnCfg.Write("Tolerance", 2000);
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_WaitUntilHRMode);
}

bool TcnTimeProviderTest::TestInitialise_WithInvalidTcnDevice_Fail() {
    tcnCfg.Write("TcnDevice", "/path/to/invalid/file.xml");
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_SleepMode);    
}

bool TcnTimeProviderTest::TestInitialise_WithMissingTcnDevice_Fail() {
    tcnCfg.Delete("TcnDevice");
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_SleepMode);    
}

bool TcnTimeProviderTest::TestInitialise_WrongXmlPlugin_Fail() {
    tcnCfg.Write("TcnDevice", "/home/codac-dev/VNAProjects/tcn-fail-1.xml");
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_SleepMode); 
}

bool TcnTimeProviderTest::TestInitialise_WithFrequency() {
    tcnCfg.Write("TcnFrequency", 1000u);
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_SleepMode); 
}

static bool TestIntegratedRun(const MARTe::char8 * const configFile) {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFile;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if(ok) {
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
    ReferenceT<TcnTimeProviderTestGAM> gama = application->Find("Functions.GAMA");

    if (ok) {
        ok = linuxTimer.IsValid();
    }

    if (ok) {
        ok = gama.IsValid();
    }

    uint32 *counter;
    uint32 *timer;

    uint32 startCounter = 0u;
    uint32 startTimer = 0u;

    uint32 endCounter = 0u;
    uint32 endTimer = 0u;

    if (ok) {
        linuxTimer->GetSignalMemoryBuffer(0, 0, (void *&) counter);
        linuxTimer->GetSignalMemoryBuffer(1, 0, (void *&) timer);
        uint32 c = 0;
        ok = false;
        while (c < 500 && !ok) {
            ok = (((*counter) > 10) && ((*timer) > 10000));
            if (ok) {
                ok = (gama->val1 > 10);
            }
            if (ok) {
                ok = (gama->val2 > 10000);
            }
            c++;
            if (!ok) {
                Sleep::MSec(10);
            }
        }
        startCounter = *counter;
        startTimer = *timer;
    }

    if(ok) {
        Sleep::Sec(5);
    }

    endCounter = *counter;
    endTimer = *timer;

    if (ok) {
        application->StopCurrentStateExecution();
    }

    god->Purge();


    REPORT_ERROR_STATIC(ErrorManagement::Information, "Delta Counter %d, Delta Timer %d", endCounter - startCounter, endTimer - startTimer);

    return ok;
}

bool TcnTimeProviderTest::TestIntegrated_WithTcnPollDisabled() {
    return TestIntegratedRun(configTestIntegratedNoPoll);
}

bool TcnTimeProviderTest::TestIntegrated_WithTcnPollEnabled() {
    return TestIntegratedRun(configTestIntegratedPoll);
}
