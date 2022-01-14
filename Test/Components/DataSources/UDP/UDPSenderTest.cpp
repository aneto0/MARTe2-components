/**
 * @file UDPSenderTest.cpp
 * @brief Source file for class UDPSenderTest
 * @date 10/06/2021
 * @author Luca Porzio
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
 * the class UDPSenderTest (public, protected, and private). Be aware that some 
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
#include "GAMScheduler.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "StreamString.h"
#include "UDPSender.h"
#include "UDPSenderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/* Helper Client to check data sent */
class ReceiverClientHelper: public MARTe::EmbeddedServiceMethodBinderI {
public:
    ReceiverClientHelper() :
            EmbeddedServiceMethodBinderI(),
            executor(*this) {
        using namespace MARTe;
        totalMemorySize = 13u;
        memory = NULL_PTR(void*);
        port = 45678u;
        timeout.SetTimeoutSec(2u);
    }

    ~ReceiverClientHelper() {
        using namespace MARTe;
        if (!executor.Stop()) {
            if (!executor.Stop()) {

            }
        }
        if (!socket.Close()) {

        }
        if (memory != NULL_PTR(void*)) {
            (void) HeapManager::Free(memory);
        }
    }

    bool InitiliaseThread() {
        using namespace MARTe;
        bool ok = socket.Open();
        if (ok) {
            ok = socket.Listen(port);
        }
        if (ok) {
            memory = HeapManager::Malloc(totalMemorySize);
        }
        return ok;
    }

    bool StartThread() {
        using namespace MARTe;
        return (executor.Start() == ErrorManagement::NoError);
    }

    bool StopThread() {
        using namespace MARTe;
        executor.Stop();
        return (executor.Stop() == ErrorManagement::NoError);
    }

    virtual MARTe::ErrorManagement::ErrorType Execute(MARTe::ExecutionInfo &info) {
        using namespace MARTe;
        char8 *const dataBuffer = reinterpret_cast<char8*>(memory);
        bool ok = socket.Read(dataBuffer, totalMemorySize, timeout);
        Sleep::MSec(100);
        if (ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Received! %d", *reinterpret_cast<uint32*>(&dataBuffer[9]));
        }
        return ok;
    }

    void* const GetMemory() {
        return memory;
    }
private:
    MARTe::SingleThreadService executor;

    MARTe::BasicUDPSocket socket;

    void *memory;

    MARTe::uint32 totalMemorySize;

    MARTe::uint16 port;

    MARTe::TimeoutType timeout;
};

/**
 * @brief Manual scheduler to test the correct interface between the UDPSender and the GAMs
 */
class UDPSenderSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

    UDPSenderSchedulerTestHelper() :
            MARTe::GAMSchedulerI() {
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
        ReferenceT<RealTimeApplication> rtAppT = realTimeApp;
        ExecuteSingleCycle(scheduledStates[rtAppT->GetIndex()]->threads[threadId].executables,
                scheduledStates[rtAppT->GetIndex()]->threads[threadId].numberOfExecutables);
    }

    virtual bool ConfigureScheduler(MARTe::Reference realTimeApp) {

        bool ret = GAMSchedulerI::ConfigureScheduler(realTimeApp);
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

CLASS_REGISTER(UDPSenderSchedulerTestHelper, "1.0")

static bool TestIntegratedExecution(const MARTe::char8 *const config,
                                    MARTe::uint32 sleepMSec = 10) {
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
    ReferenceT<UDPSenderSchedulerTestHelper> scheduler;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }

    if (ok) {
        for (uint32 i = 0; (i < 5u); i++) {
            scheduler->ExecuteThreadCycle(0);
            Sleep::MSec(sleepMSec);
        }
    }
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    godb->Purge();

    return ok;
}

bool TestSendReceiveApplication(const MARTe::char8 *const config,
                                MARTe::uint32 sleepMSec = 10) {
    using namespace MARTe;

    ReceiverClientHelper rcvThread;

    bool ok = rcvThread.InitiliaseThread();
    if (ok) {
        ok = rcvThread.StartThread();
    }

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    if (ok) {
        ok = parser.Parse();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    ReferenceT<UDPSenderSchedulerTestHelper> scheduler;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }

    if (ok) {
        for (uint32 i = 0; (i < 5u); i++) {
            scheduler->ExecuteThreadCycle(0);
            Sleep::MSec(sleepMSec);
        }
    }

    char8 *const mem = reinterpret_cast<char8*>(rcvThread.GetMemory());
    uint32 valueReceived = *reinterpret_cast<uint32*>(&mem[9]);
    ok = (valueReceived == 99u);

    if (ok) {
        ok = rcvThread.StopThread();
    }

    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    godb->Purge();

    return ok;
}

//Correct configuration
static const MARTe::char8 *const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMTimer = {"
        "            Class = IOGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                    DataSource = Timer"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                    DataSource = Timer"
        "                    Frequency = 1"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
        "                }"
        "            }"
        "        }"
        "        +TriggerGAM = {"
        "            Class = ConstantGAM"
        "            OutputSignals = {"
        "                Trigger = {"
        "                    Type = uint8"
        "                    DataSource = DDB1"
        "                    Default = 1"
        "                }"
        "            }"
        "        }"
        "        +PayloadGAM = {"
        "            Class = ConstantGAM"
        "            OutputSignals = {"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
        "                    Default = 99"
        "                }"
        "            }"
        "        }"
        "        +GAMSender = {"
        "            Class = IOGAM"
        "            InputSignals = {"
        "                Trigger = {"
        "                    Type = uint8"
        "                    DataSource = DDB1"
        "                }"
        "                Counter = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
        "                }"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Trigger = {"
        "                    Type = uint8"
        "                    DataSource = UDP"
        "                }"
        "                Counter = {"
        "                    Type = uint32"
        "                    DataSource = UDP"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                    DataSource = UDP"
        "                }"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = UDP"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +UDP = {"
        "            Class = UDP::UDPSender"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            NumberOfPreTriggers = 0"
        "            NumberOfPostTriggers = 0"
        "            Address = \"127.0.0.1\""
        "            Port = 45678"
        "            ExecutionMode = IndependentThread"
        "            Signals = {"
        "                Trigger = {"
        "                    Type = uint8"
        "                }"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                Payload = {"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = \"Default\""
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
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
        "                    Functions = {GAMTimer TriggerGAM PayloadGAM GAMSender}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = UDPSenderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

static const MARTe::char8 *const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMTimer = {"
        "            Class = IOGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                    DataSource = Timer"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                    DataSource = Timer"
        "                    Frequency = 1"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
        "                }"
        "            }"
        "        }"
        "        +TriggerGAM = {"
        "            Class = ConstantGAM"
        "            OutputSignals = {"
        "                Trigger = {"
        "                    Type = uint8"
        "                    DataSource = DDB1"
        "                    Default = 1"
        "                }"
        "            }"
        "        }"
        "        +PayloadGAM = {"
        "            Class = ConstantGAM"
        "            OutputSignals = {"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
        "                    Default = 99"
        "                }"
        "            }"
        "        }"
        "        +GAMSender = {"
        "            Class = IOGAM"
        "            InputSignals = {"
        "                Trigger = {"
        "                    Type = uint8"
        "                    DataSource = DDB1"
        "                }"
        "                Counter = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
        "                }"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Trigger = {"
        "                    Type = uint8"
        "                    DataSource = UDP"
        "                }"
        "                Counter = {"
        "                    Type = uint32"
        "                    DataSource = UDP"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                    DataSource = UDP"
        "                }"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = UDP"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +UDP = {"
        "            Class = UDP::UDPSender"
        "            Address = \"127.0.0.1\""
        "            Port = 45678"
        "            ExecutionMode = RealTimeThread"
        "            Signals = {"
        "                Trigger = {"
        "                    Type = uint8"
        "                }"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
        "                }"
        "                Payload = {"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "            SleepNature = \"Default\""
        "            Signals = {"
        "                Counter = {"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    Type = uint32"
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
        "                    Functions = {GAMTimer TriggerGAM PayloadGAM GAMSender}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = UDPSenderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool UDPSenderTest::TestConstructor() {
    using namespace MARTe;
    UDPSender us;
    bool ok = (us.GetCPUMask() == 0xffffffffu);
    ok &= (us.GetNumberOfPostTriggers() == 0u);
    ok &= (us.GetNumberOfPreTriggers() == 0u);
    ok &= (us.GetStackSize() == 0u);
    ok &= (us.GetPort() == 44488u);
    ok &= (us.GetAddress() == "");
    return ok;
}

bool UDPSenderTest::TestInitialise_No_NumberOfPreTriggers() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.Write("Address", "234.0.0.1");
    cdb.Write("Port", 45678);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool UDPSenderTest::TestInitialise_No_NumberOfPostTriggers() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("NumberOfPreTriggers", 3);
    cdb.Write("Address", "234.0.0.1");
    cdb.Write("Port", 45678);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool UDPSenderTest::TestInitialise_No_Address() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("NumberOfPreTriggers", 3);
    cdb.Write("NumberOfPostTriggers", 2);
    cdb.Write("Port", 45678);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool UDPSenderTest::TestInitialise_No_Port() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("NumberOfPreTriggers", 3);
    cdb.Write("NumberOfPostTriggers", 2);
    cdb.Write("Address", "234.0.0.1");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool UDPSenderTest::TestInitialise_Forbidden_Port() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("NumberOfPreTriggers", 3);
    cdb.Write("NumberOfPostTriggers", 2);
    cdb.Write("Address", "234.0.0.1");
    cdb.Write("Port", 500);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool UDPSenderTest::TestInitialise_No_CpuMaks() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("StackSize", 10000000);
    cdb.Write("NumberOfPreTriggers", 3);
    cdb.Write("NumberOfPostTriggers", 2);
    cdb.Write("Address", "234.0.0.1");
    cdb.Write("Port", 45678);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool UDPSenderTest::TestInitialise_No_StackSize() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUMask", 15);
    cdb.Write("NumberOfPreTriggers", 3);
    cdb.Write("NumberOfPostTriggers", 2);
    cdb.Write("Address", "234.0.0.1");
    cdb.Write("Port", 45678);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool UDPSenderTest::TestInitialise_StackSize_Zero() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 0);
    cdb.Write("NumberOfPreTriggers", 3);
    cdb.Write("NumberOfPostTriggers", 2);
    cdb.Write("Address", "234.0.0.1");
    cdb.Write("Port", 45678);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool UDPSenderTest::TestInitialise_No_TriggerSignal() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 1000000);
    cdb.Write("NumberOfPreTriggers", 3);
    cdb.Write("NumberOfPostTriggers", 2);
    cdb.Write("Address", "234.0.0.1");
    cdb.Write("Port", 45678);
    cdb.CreateRelative("Signals");
    cdb.MoveRelative("Signals");
    cdb.CreateRelative("Counter");
    cdb.MoveRelative("Counter");
    cdb.Write("Type", "uint32");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool UDPSenderTest::TestInitialise() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 1000000);
    cdb.Write("NumberOfPreTriggers", 3);
    cdb.Write("NumberOfPostTriggers", 2);
    cdb.Write("Address", "234.0.0.1");
    cdb.Write("Port", 45678);
    cdb.Write("ExecutionMode", "IndependentThread");
    cdb.CreateRelative("Signals");
    cdb.MoveRelative("Signals");
    cdb.CreateRelative("Trigger");
    cdb.MoveRelative("Trigger");
    cdb.Write("Type", "uint8");
    cdb.MoveToAncestor(1u);
    cdb.CreateRelative("Counter");
    cdb.MoveRelative("Counter");
    cdb.Write("Type", "uint32");
    cdb.MoveToAncestor(1u);
    cdb.CreateRelative("Time");
    cdb.MoveRelative("Time");
    cdb.Write("Type", "uint32");
    cdb.MoveToRoot();
    return test.Initialise(cdb);
}

bool UDPSenderTest::TestInitialise_RealTimeThread() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("Address", "234.0.0.1");
    cdb.Write("Port", 45678);
    cdb.Write("ExecutionMode", "RealTimeThread");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return test.Initialise(cdb);
}

bool UDPSenderTest::TestInitialise_Wrong_ExecutionMode() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("Address", "234.0.0.1");
    cdb.Write("Port", 45678);
    cdb.Write("ExecutionMode", "RTThread");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool UDPSenderTest::TestInitialise_RealTimeThread_IgnoreParameters() {
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 1000000);
    cdb.Write("NumberOfPreTriggers", 3);
    cdb.Write("NumberOfPostTriggers", 2);
    cdb.Write("Address", "234.0.0.1");
    cdb.Write("Port", 45678);
    cdb.Write("ExecutionMode", "RealTimeThread");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return test.Initialise(cdb);
}

bool UDPSenderTest::TestGetInputBrokers() {
    using namespace MARTe;
    UDPSender test;
    ReferenceContainer rc;
    return !test.GetInputBrokers(rc, "", NULL);
}

bool UDPSenderTest::TestGetOutputBrokers() {
    return TestIntegratedExecution(config1);
}

bool UDPSenderTest::TestGetBrokerName() {
    using namespace MARTe;
    bool ok = true;
    UDPSender us;
    ConfigurationDatabase cdb;
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 1000000);
    cdb.Write("NumberOfPreTriggers", 3);
    cdb.Write("NumberOfPostTriggers", 2);
    cdb.Write("Address", "234.0.0.1");
    cdb.Write("Port", 45678);
    cdb.Write("ExecutionMode", "IndependentThread");
    cdb.CreateRelative("Signals");
    cdb.MoveRelative("Signals");
    cdb.CreateRelative("Trigger");
    cdb.MoveRelative("Trigger");
    cdb.Write("Type", "uint8");
    cdb.MoveToAncestor(1u);
    cdb.MoveToRoot();
    ok = us.Initialise(cdb);
    if (ok) {
        ok = (StringHelper::Compare(us.GetBrokerName(cdb, OutputSignals), "MemoryMapAsyncTriggerOutputBroker") == 0);
    }
    return ok;
}

bool UDPSenderTest::TestPrepareNextState() {
    return TestIntegratedExecution(config1);
}

bool UDPSenderTest::TestSynchronise() {
    return TestSendReceiveApplication(config1);
}

bool UDPSenderTest::TestSynchronise_RealTimeThread() {
    return TestSendReceiveApplication(config2);
}

bool UDPSenderTest::TestSetConfiguredDatabase_Correct() {
    return TestIntegratedExecution(config1);
}

