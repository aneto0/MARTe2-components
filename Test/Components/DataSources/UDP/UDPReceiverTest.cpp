/**
 * @file UDPReceiverTest.cpp
 * @brief Source file for class UDPReceiverTest
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
 * the class UDPReceiverTest (public, protected, and private). Be aware that some 
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
#include "UDPSocket.h"
#include "UDPReceiver.h"
#include "UDPReceiverTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//Helper Client to send test data
class SenderClientHelper: public MARTe::EmbeddedServiceMethodBinderI {
public:
    SenderClientHelper() :
            EmbeddedServiceMethodBinderI(),
            executor(*this) {
        using namespace MARTe;
        address = "127.0.0.1";
        memory = NULL_PTR(void*);
        port = 45678u;
        totalMemorySize = 4u;
    }

    ~SenderClientHelper() {
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
            ok = socket.Connect(address.Buffer(), port);
        }
        if (ok) {
            memory = HeapManager::Malloc(totalMemorySize);
            uint32 value = 99u;
            ok = MemoryOperationsHelper::Copy(memory, &value, totalMemorySize);
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
        const char8 *const dataBuffer = reinterpret_cast<char8*>(memory);
        bool ok = socket.Write(dataBuffer, totalMemorySize);
        if (ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Sent! %d", *reinterpret_cast<uint32*>(memory));
        }
        Sleep::MSec(100);
        return ok;
    }

private:

    MARTe::SingleThreadService executor;

    MARTe::UDPSocket socket;

    MARTe::StreamString address;

    MARTe::uint16 port;

    void *memory;

    MARTe::uint32 totalMemorySize;

};

/**
 * @brief Manual scheduler to test the correct interface between the UDPSender and the GAMs
 */
class UDPReceiverSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

    UDPReceiverSchedulerTestHelper() :
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

CLASS_REGISTER(UDPReceiverSchedulerTestHelper, "1.0")

class TestHelperGAM: public MARTe::GAM {
public:CLASS_REGISTER_DECLARATION()
    TestHelperGAM() :
            MARTe::GAM() {
        mem = NULL_PTR(void*);
    }

    ~TestHelperGAM() {
        (void) MARTe::HeapManager::Free(mem);
    }

    bool Setup() {
        mem = MARTe::HeapManager::Malloc(4u);
        return true;
    }

    bool Execute() {
        using namespace MARTe;
        if (*reinterpret_cast<uint32*>(GetInputSignalsMemory()) != 0u) {
            (void) MemoryOperationsHelper::Copy(mem, GetInputSignalsMemory(), 4u);
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Received! %d", *reinterpret_cast<uint32*>(mem));
        }
        return MemoryOperationsHelper::Copy(GetOutputSignalsMemory(), GetInputSignalsMemory(), 4u);
    }

    void* const GetMemory() {
        return mem;
    }

private:
    void *mem;

};

CLASS_REGISTER(TestHelperGAM, "1.0")

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
    ReferenceT<UDPReceiverSchedulerTestHelper> scheduler;
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

static bool TestSendReceiveExecution(const MARTe::char8 *const config,
                                     MARTe::uint32 sleepMSec = 10) {
    using namespace MARTe;

    bool ok = true;
    SenderClientHelper sndThread;
    if (ok) {
        ok = sndThread.InitiliaseThread();
        if (ok) {
            ok = sndThread.StartThread();
        }
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
    ReferenceT<UDPReceiverSchedulerTestHelper> scheduler;
    ReferenceT<TestHelperGAM> hGam;
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
        hGam = application->Find("Functions.GAMReceiver");
        ok = hGam.IsValid();
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
        uint32 *const dataBuffer = reinterpret_cast<uint32*>(hGam->GetMemory());
        uint32 valueSent = *dataBuffer;
        ok = (valueSent == 99u);
    }
    if (ok) {
        ok = sndThread.StopThread();
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
        "        +GAMReceiver = {"
        "            Class = TestHelperGAM"
        "            InputSignals = {"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = UDP"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
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
        "            Class = UDP::UDPReceiver"
        "            ExecutionMode = RealTimeThread"
        "            Address = \"234.0.0.1\""
        "            Port = 45678"
        "            Timeout = 0.5"
        "            Signals = {"
        "                Payload = {"
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
        "                    Functions = {GAMReceiver}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = UDPReceiverSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Invalid address
static const MARTe::char8 *const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMReceiver = {"
        "            Class = TestHelperGAM"
        "            InputSignals = {"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = UDP"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
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
        "            Class = UDP::UDPReceiver"
        "            ExecutionMode = RealTimeThread"
        "            Address = \"223.0.0.1\""
        "            Port = 45678"
        "            Timeout = 0.5"
        "            Signals = {"
        "                Payload = {"
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
        "                    Functions = {GAMReceiver}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = UDPReceiverSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Correct configuration with localhost
static const MARTe::char8 *const config3 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMReceiver = {"
        "            Class = TestHelperGAM"
        "            InputSignals = {"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = UDP"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
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
        "            Class = UDP::UDPReceiver"
        "            ExecutionMode = RealTimeThread"
        "            Port = 45678"
        "            Timeout = 4"
        "            Signals = {"
        "                Payload = {"
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
        "                    Functions = {GAMReceiver}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = UDPReceiverSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Correct configuration with localhost and decoupled thread
static const MARTe::char8 *const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMReceiver = {"
        "            Class = TestHelperGAM"
        "            InputSignals = {"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = UDP"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
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
        "            Class = UDP::UDPReceiver"
        "            CPUMask = 0x2"
        "            ExecutionMode = IndependentThread"
        "            Port = 45678"
        "            Timeout = 10"
        "            Signals = {"
        "                Payload = {"
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
        "                    Functions = {GAMReceiver}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = UDPReceiverSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

static const MARTe::char8 *const config5 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMReceiver = {"
        "            Class = TestHelperGAM"
        "            InputSignals = {"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = UDP"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Payload = {"
        "                    Type = uint32"
        "                    DataSource = DDB1"
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
        "            Class = UDP::UDPReceiver"
        "            ExecutionMode = RealTimeThread"
        "            Address = \"234.0.0.1\""
        "            InterfaceAddress = \"0.0.0.0\""
        "            Port = 45678"
        "            Timeout = 0.5"
        "            Signals = {"
        "                Payload = {"
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
        "                    Functions = {GAMReceiver}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = UDPReceiverSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";



/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool UDPReceiverTest::TestConstructor() {
    using namespace MARTe;
    UDPReceiver test;
    bool ok = (test.GetCPUMask() == 0u);
    ok &= (test.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    ok &= (test.GetPort() == 0u);
    ok &= (test.GetAddress() == "");
    ok &= (test.GetExecutionMode() == UDPReceiverExecutionModeRealTime);
    return ok;
}

bool UDPReceiverTest::TestInitialise_No_Address() {
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", 45678);
    cdb.Write("Timeout", 10);
    cdb.Write("ExecutionMode", "RealTimeThread");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    ok &= (test.GetAddress() == "");
    return ok;
}

bool UDPReceiverTest::TestInitialise_No_Port() {
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Timeout", 10);
    cdb.Write("ExecutionMode", "RealTimeThread");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    ok &= (test.GetPort() == 44488u);
    return ok;
}

bool UDPReceiverTest::TestInitialise_Forbidden_Port() {
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", 500);
    cdb.Write("Timeout", 10);
    cdb.Write("ExecutionMode", "RealTimeThread");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    ok &= (test.GetPort() == 500);
    return ok;
}

bool UDPReceiverTest::TestInitialise_No_Timeout() {
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", 500);
    cdb.Write("ExecutionMode", "RealTimeThread");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_No_ExecutionMode() {
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", 500);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 1000000);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_No_ExecutionMode_No_Cpu() {
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", 500);
    cdb.Write("StackSize", 1000000);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_No_ExecutionMode_No_StackSize() {
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", 500);
    cdb.Write("CPUMask", 15);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_InterfaceAddress() {
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", 45678);
    cdb.Write("Timeout", 10);
    cdb.Write("ExecutionMode", "RealTimeThread");
    cdb.Write("Address", "127.0.0.1");
    cdb.Write("InterfaceAddress", "0.0.0.0");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    return ok;

}

bool UDPReceiverTest::TestInitialise_IndependentThread() {
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", 500);
    cdb.Write("ExecutionMode", "IndependentThread");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    ok &= (test.GetExecutionMode() == UDPReceiverExecutionModeIndependent);
    return ok;
}

bool UDPReceiverTest::TestInitialise_Wrong_ExecutionMode() {
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", 500);
    cdb.Write("ExecutionMode", "IThread");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    return !ok;
}

bool UDPReceiverTest::TestSetConfiguredDatabase_ValidAddress() {
    return TestIntegratedExecution(config1);
}

bool UDPReceiverTest::TestSetConfiguredDatabase_MulticastAddress_Interface() {
    return TestIntegratedExecution(config5);
}

bool UDPReceiverTest::TestSetConfiguredDatabase_InvalidAddress() {
    return !TestIntegratedExecution(config2);
}

bool UDPReceiverTest::TestGetBrokerName() {
    using namespace MARTe;
    bool ok = true;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", 45678);
    cdb.Write("ExecutionMode", "RealTimeThread");
    cdb.Write("Timeout", 0.5);
    cdb.CreateRelative("Signals");
    cdb.MoveRelative("Signals");
    cdb.CreateRelative("Payload");
    cdb.MoveRelative("Payload");
    cdb.Write("Type", "uint32");
    cdb.MoveToRoot();
    ok = test.Initialise(cdb);
    if (ok) {
        ok = (StringHelper::Compare(test.GetBrokerName(cdb, InputSignals), "MemoryMapSynchronisedInputBroker") == 0);
    }
    return ok;
}

bool UDPReceiverTest::TestPrepareNextState() {
    return TestIntegratedExecution(config1);
}

bool UDPReceiverTest::TestSynchronise() {
    return TestSendReceiveExecution(config3);
}

bool UDPReceiverTest::TestExecute_Timeout() {
    using namespace MARTe;

    bool ok = true;
    ConfigurationDatabase cdb;
    StreamString configStream = config4;
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
    ReferenceT<UDPReceiverSchedulerTestHelper> scheduler;
    ReferenceT<TestHelperGAM> hGam;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();
    SenderClientHelper sndThread;

    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = sndThread.InitiliaseThread();
        if (ok) {
            ok = sndThread.StartThread();
        }
    }

    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        hGam = application->Find("Functions.GAMReceiver");
        ok = hGam.IsValid();
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
            Sleep::MSec(10u);
        }
    }
    if (ok) {
        ok = sndThread.StopThread();
    }
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    godb->Purge();

    return ok;
}
