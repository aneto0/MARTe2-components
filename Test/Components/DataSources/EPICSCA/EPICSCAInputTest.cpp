/**
 * @file EPICSCAInputTest.cpp
 * @brief Source file for class EPICSCAInputTest
 * @date 21/04/2017
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
 * the class EPICSCAInputTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <cadef.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "Directory.h"
#include "DirectoryScanner.h"
#include "EPICSCAInput.h"
#include "EPICSCAInputTest.h"
#include "GAM.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief GAM which reads signals from a given EPICSCAInput
 */
class EPICSCAInputGAMTestHelper: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()EPICSCAInputGAMTestHelper() {
        uint32Signal = NULL;
        int32Signal = NULL;
        float32Signal = NULL;
        float64Signal = NULL;
    }

    virtual ~EPICSCAInputGAMTestHelper() {
        using namespace MARTe;
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        if (GetSignalType(InputSignals, 0u) == UnsignedInteger32Bit) {
            uint32Signal = reinterpret_cast<uint32 *>(GetInputSignalMemory(0u));
        }
        else {
            int32Signal = reinterpret_cast<int32 *>(GetInputSignalMemory(0u));
        }
        if (GetSignalType(InputSignals, 1u) == Float32Bit) {
            float32Signal = reinterpret_cast<float32 *>(GetInputSignalMemory(1u));
        }
        else {
            float64Signal = reinterpret_cast<float64 *>(GetInputSignalMemory(1u));
        }

        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        return true;
    }

    MARTe::uint32 *uint32Signal;
    MARTe::int32 *int32Signal;
    MARTe::float32 *float32Signal;
    MARTe::float64 *float64Signal;
};
CLASS_REGISTER(EPICSCAInputGAMTestHelper, "1.0")

/**
 * @brief Manual scheduler to test the correct interface between the EPICSCAInput and the GAMs
 */
class EPICSCAInputSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

EPICSCAInputSchedulerTestHelper    () : MARTe::GAMSchedulerI() {
        scheduledStates = NULL;
    }

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    void ExecuteThreadCycle(MARTe::uint32 threadId) {
        using namespace MARTe;
        ExecuteSingleCycle(scheduledStates[RealTimeApplication::GetIndex()]->threads[threadId].executables,
                scheduledStates[RealTimeApplication::GetIndex()]->threads[threadId].numberOfExecutables);
    }

    virtual bool ConfigureScheduler() {

        bool ret = GAMSchedulerI::ConfigureScheduler();
        if (ret) {
            scheduledStates = GetSchedulableStates();
        }
        return ret;
    }

    virtual void CustomPrepareNextState() {
    }

private:

    MARTe::ScheduledState * const * scheduledStates;
};

CLASS_REGISTER(EPICSCAInputSchedulerTestHelper, "1.0")

static bool TestIntegratedInApplication(const MARTe::char8 * const config, bool destroy) {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StreamString err;
    StandardParser parser(configStream, cdb, &err);

    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "%s", err.Buffer());
    }

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

    if (destroy) {
        god->Purge();
    }
    return ok;
}

//Standard configuration
static const MARTe::char8 * const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +EPICSCAInputTest = {"
        "            Class = EPICSCAInput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            Signals = {"
        "                SignalUInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt32\""
        "                }"
        "                SignalInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int32\""
        "                }"
        "                SignalFloat32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float32\""
        "                }"
        "                SignalFloat64 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float64\""
        "                }"
        "            }"
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
        "                    Functions = {GAM1 GAM2}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = EPICSCAInputSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Standard configuration no signals
static const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +EPICSCAInputTest = {"
        "            Class = EPICSCAInput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            Signals = {"
        "                SignalUInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt32\""
        "                }"
        "                SignalInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int32\""
        "                }"
        "                SignalFloat32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float32\""
        "                }"
        "                SignalFloat64 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float64\""
        "                }"
        "            }"
        "        }"
        "        +EPICSCAInputTestNoSignals = {"
        "            Class = EPICSCAInput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            Signals = {"
        "                Locked = 0"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = EPICSCAInputSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool EPICSCAInputTest::TestConstructor() {
    using namespace MARTe;
    EPICSCAInput test;
    bool ok = (test.GetCPUMask() == 0xff);
    ok &= (test.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 4u));
    return ok;
}

bool EPICSCAInputTest::TestAllocateMemory() {
    using namespace MARTe;
    EPICSCAInput test;
    return test.AllocateMemory();
}

bool EPICSCAInputTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    EPICSCAInput test;
    return (test.GetNumberOfMemoryBuffers() == 1);
}

bool EPICSCAInputTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    EPICSCAInput test;
    void *ptr = NULL;
    bool ok = !test.GetSignalMemoryBuffer(0, 0, ptr);
    if (ok) {
        ok = TestIntegratedInApplication(config1, true);
    }
    return ok;
}

bool EPICSCAInputTest::TestGetBrokerName_OutputSignals() {
    using namespace MARTe;
    EPICSCAInput test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, OutputSignals), "") == 0);

    return ok;
}

bool EPICSCAInputTest::TestGetBrokerName() {
    using namespace MARTe;
    EPICSCAInput test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, InputSignals), "MemoryMapInputBroker") == 0);

    return ok;
}

bool EPICSCAInputTest::TestGetInputBrokers() {
    return TestIntegratedInApplication(config1, true);
}

bool EPICSCAInputTest::TestGetOutputBrokers() {
    using namespace MARTe;
    EPICSCAInput test;
    ReferenceContainer rc;
    return !test.GetOutputBrokers(rc, "", NULL);
}

bool EPICSCAInputTest::TestInitialise() {
    using namespace MARTe;
    EPICSCAInput test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 1);
    cdb.Write("StackSize", 100000);
    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (test.GetCPUMask() == 1);
        ok &= (test.GetStackSize() == 100000);
    }
    return ok;
}

bool EPICSCAInputTest::TestInitialise_Defaults() {
    using namespace MARTe;
    EPICSCAInput test;
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (test.GetCPUMask() == 0xff);
        ok &= (test.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 4u));
    }
    return ok;
}

bool EPICSCAInputTest::TestInitialise_False_Signals() {
    using namespace MARTe;
    EPICSCAInput test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 1);
    cdb.Write("StackSize", 100000);
    return !test.Initialise(cdb);
}

bool EPICSCAInputTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config1, true);
}

bool EPICSCAInputTest::TestSetConfiguredDatabase_False_NoSignals() {
    return !TestIntegratedInApplication(config2, true);
}

bool EPICSCAInputTest::TestExecute() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config1, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<EPICSCAInputGAMTestHelper> gam1;
    ReferenceT<EPICSCAInputGAMTestHelper> gam2;
    ReferenceT<EPICSCAInputSchedulerTestHelper> scheduler;
    ReferenceT<RealTimeApplication> application;

    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        gam1 = godb->Find("Test.Functions.GAM1");
        ok = gam1.IsValid();
    }
    if (ok) {
        gam2 = godb->Find("Test.Functions.GAM2");
        ok = gam2.IsValid();
    }
    if (ok) {
        scheduler = godb->Find("Test.Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }

    if (ok) {
        ok = (ca_context_create(ca_enable_preemptive_callback));
    }
    const uint32 NUMBER_OF_PVS = 4u;
    chid pvChids[NUMBER_OF_PVS];
    chtype pvTypes[] = { DBR_INT, DBR_INT, DBR_FLOAT, DBR_DOUBLE };
    const char8 *pvNames[] = { "MARTe2::EPICSCAInput::Test::UInt32", "MARTe2::EPICSCAInput::Test::Int32", "MARTe2::EPICSCAInput::Test::Float32", "MARTe2::EPICSCAInput::Test::Float64" };
    uint32 uint32Value = 3u;
    int32 int32Value = 5;
    float32 float32Value = 7.0;
    float64 float64Value = 9.0;
    void *pvMemory[] = { &uint32Value, &int32Value, &float32Value, &float64Value };

    if (ok) {
        uint32 n;
        for (n = 0u; (n < NUMBER_OF_PVS) && (ok); n++) {
            /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
            ok = (ca_create_channel(pvNames[n], NULL_PTR(caCh *), NULL_PTR(void *), 20u, &pvChids[n]) == ECA_NORMAL);
        }
    }

    if (ok) {
        uint32 n;
        bool done = 0;
        uint32 doneC = 0;
        uint32 timeOutCounts = 50;
        while ((!done) && (timeOutCounts > 0u) && (ok)) {
            for (n = 0u; (n < NUMBER_OF_PVS) && (ok); n++) {
                ca_put(pvTypes[n], pvChids[n], pvMemory[n]);
            }
            if (ok) {
                ca_pend_io(1.0);
            }
            scheduler->ExecuteThreadCycle(0);

            done = (*gam1->uint32Signal == uint32Value);
            done &= (*gam1->float64Signal == float64Value);
            done &= (*gam2->int32Signal == int32Value);
            done &= (*gam2->float32Signal == float32Value);
            if (!done) {
                timeOutCounts--;
                Sleep::Sec(0.1);
            }
            if ((done) && (doneC == 0)) {
                uint32Value *= 2;
                int32Value *= 2;
                float32Value *= 2;
                float64Value *= 2;
                done = false;
                doneC++;
            }
        }
        if (ok) {
            ok = done;
        }
    }

    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        uint32 n;
        for (n = 0u; (n < NUMBER_OF_PVS); n++) {
            ca_clear_channel(pvChids[n]);
        }
    }
    ca_detach_context();
    ca_context_destroy();
    godb->Purge();
    return ok;
}

