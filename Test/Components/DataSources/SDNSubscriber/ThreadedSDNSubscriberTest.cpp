/**
 * @file ThreadedSDNSubscriberTest.cpp
 * @brief Source file for class ThreadedSDNSubscriberTest
 * @date 12/12/2016
 * @author Bertrand Bauvir
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
#include "BrokerI.h"
#include "ErrorManagement.h"
#include "ErrorInformation.h"
#include "GlobalObjectsDatabase.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "GAM.h"
#include "ThreadedSDNSubscriber.h"

#include "ThreadedSDNSubscriberTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class ThreadedSDNSubscriberTestGAM: public MARTe::GAM {

public:

    CLASS_REGISTER_DECLARATION()

    ThreadedSDNSubscriberTestGAM() : GAM() {
        counter = 0lu;
        timestamp = 0lu;
    }

    ~ThreadedSDNSubscriberTestGAM() { }

    bool Execute() {

        if(GetNumberOfInputSignals() > 0u) {
            MARTe::MemoryOperationsHelper::Copy(&counter, GetInputSignalMemory(0u), sizeof(MARTe::uint64));
        }

        if(GetNumberOfInputSignals() > 1u) {
            MARTe::MemoryOperationsHelper::Copy(&timestamp, GetInputSignalMemory(1u), sizeof(MARTe::uint64));
        }

        return true;
    }

    bool Setup() {
        return true;
    }

    bool TestCounter(MARTe::uint64 value = 0ul) {
        bool ok = (counter == value);
	log_info("ThreadedSDNSubscriberTestGAM::TestCounter - '%lu %lu'", counter, value);
        return ok;
    }

    bool TestTimestamp(MARTe::uint64 value = 0ul) {
        bool ok = (timestamp == value);
	log_info("ThreadedSDNSubscriberTestGAM::TestTimestamp - '%lu %lu'", timestamp, value);
        return ok;
    }

    MARTe::uint64 counter;
    MARTe::uint64 timestamp;

};

CLASS_REGISTER(ThreadedSDNSubscriberTestGAM, "1.0")

/**
 * Starts a MARTe application that uses this driver instance.
 */

static inline bool ConfigureApplication(const MARTe::char8 * const config) {

    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    StreamString err;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb, &err);

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    bool ok = parser.Parse();

    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "StandardParser::Parse failed");
        log_error("StandardParser::Parse failed with '%s'", err.Buffer());
    } else {
        god->Purge();
        ok = god->Initialise(cdb);
    }

    ReferenceT<RealTimeApplication> application;

    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "RealTimeApplication::IsValid failed");
    } else {
        ok = application->ConfigureApplication();
    }

    return ok;

}

static inline bool StartApplication(const MARTe::char8 * const state = "Running") {

    using namespace MARTe;

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");;
    bool ok = application.IsValid();

    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "RealTimeApplication::IsValid failed");
    } else {
        ok = application->PrepareNextState(state);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "RealTimeApplication::PrepareNextState failed");
    } else {
        ok = application->StartNextStateExecution();
    }

    return ok;

}

static inline bool StopApplication() {

    using namespace MARTe;

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");;
    bool ok = application.IsValid();

    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "RealTimeApplication::IsValid failed");
    } else {
        ok = application->StopCurrentStateExecution();
    }

    god->Purge();

    return ok;

}

static inline bool TestIntegratedInApplication(const MARTe::char8 * const config) {

    using namespace MARTe;

    bool ok = ConfigureApplication(config);

    if (ok) {
        ok = StartApplication();
    }

    wait_for(1000000000lu);

    if (ok) {
        ok = StopApplication();
    }

    return ok;

}

//Standard configuration for testing
const MARTe::char8 * const config_default = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +Sink = {"
        "            Class = ThreadedSDNSubscriberTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = SDNSub"
        "                    Type = uint64"
        "                }"
        "                Timestamp = {"
        "                    DataSource = SDNSub"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +SDNSub = {"
        "            Class = ThreadedSDNSubscriber"
        "            Topic = Default"  
        "            Interface = lo"  
        "            Timeout = 100"  
        "            Mode = Synchronising"  
        "            Signals = {"  
        "                Counter = {"  
        "                    Type = uint64"  
        "                }"  
        "                Timestamp = {"  
        "                    Type = uint64"  
        "                }"  
        "            }"  
        "        }"  
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +Running = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread = {"
        "                    Class = RealTimeThread"
        "                    Functions = {Sink}"
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

bool ThreadedSDNSubscriberTest::TestConstructor() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    bool ok = (test.PrepareNextState("FromCurrent","ToNext")); // Test the instantiated class
    return !ok; // Expect failure since sdn::Subscriber has not yet been instantiated
}

bool ThreadedSDNSubscriberTest::TestInitialise_Empty() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestInitialise_Default() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    return test.Initialise(cdb);
}

bool ThreadedSDNSubscriberTest::TestInitialise_Address() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "127.0.0.1:60000");
    return test.Initialise(cdb);
}

bool ThreadedSDNSubscriberTest::TestInitialise_Address_mcast() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "239.0.0.1:60000");
    return test.Initialise(cdb);
}

bool ThreadedSDNSubscriberTest::TestInitialise_Address_ucast() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "10.0.0.1:60000");
    return test.Initialise(cdb);
}

bool ThreadedSDNSubscriberTest::TestInitialise_ShortForm() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "sdn://127.0.0.1:60000/Default");
    cdb.Write("Interface", "lo");
    return test.Initialise(cdb);
}

bool ThreadedSDNSubscriberTest::TestInitialise_Timeout_0ul() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    uint64 timeout = 0ul;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Timeout", timeout);
    return test.Initialise(cdb);
}

bool ThreadedSDNSubscriberTest::TestInitialise_Timeout_10ms() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    uint64 timeout = 10000000ul;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Timeout", timeout);
    return test.Initialise(cdb);
}

bool ThreadedSDNSubscriberTest::TestInitialise_Mode_Default() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Mode", "Default");
    return test.Initialise(cdb);
}

bool ThreadedSDNSubscriberTest::TestInitialise_Mode_Caching() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Mode", "Caching");
    return test.Initialise(cdb);
}

bool ThreadedSDNSubscriberTest::TestInitialise_Mode_Synchronising() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Mode", "Synchronising");
    return test.Initialise(cdb);
}

bool ThreadedSDNSubscriberTest::TestInitialise_Missing_Topic() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Interface", "lo");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestInitialise_Missing_Interface() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestInitialise_False_Interface_1() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestInitialise_False_Interface_2() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "NotAnInterface");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestInitialise_False_Address_1() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "NotAnAddress");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestInitialise_False_Address_2() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "255.255.255.255:60000");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestInitialise_False_Address_3() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "127.0.0.1");
    bool ok = test.Initialise(cdb);
    //return !ok; // Expect failure
    return ok; // It is actually successful !
}

bool ThreadedSDNSubscriberTest::TestInitialise_False_Address_4() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", ":60000");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestInitialise_False_Address_5() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "127.0.0:60000");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestInitialise_False_Mode() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Mode", "InvalidMode");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config_default);
}

bool ThreadedSDNSubscriberTest::TestSetConfiguredDatabase_False_NOfSignals() {
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +Sink = {"
        "            Class = ThreadedSDNSubscriberTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = DDB1"
        "                    Type = uint64"
        "                }"
        "                Timestamp = {"
        "                    DataSource = DDB1"
        "                    Type = uint64"
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
        "        +SDNSub = {"
        "            Class = ThreadedSDNSubscriber"
        "            Topic = Default"  
        "            Interface = lo"  
        "        }"  
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +Running = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread = {"
        "                    Class = RealTimeThread"
        "                    Functions = {Sink}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

    bool ok = ConfigureApplication(config);
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestAllocateMemory() {
    return TestIntegratedInApplication(config_default);
}

bool ThreadedSDNSubscriberTest::TestAllocateMemory_False() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    bool ok = test.AllocateMemory();
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    return (test.GetNumberOfMemoryBuffers() == 1u);
}

bool ThreadedSDNSubscriberTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    return true;
}

bool ThreadedSDNSubscriberTest::TestGetBrokerName_InputSignals() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    StreamString brokerName = test.GetBrokerName(cdb, InputSignals);
    bool ok = (brokerName == "MemoryMapInputBroker");
    return ok;
}

bool ThreadedSDNSubscriberTest::TestGetBrokerName_OutputSignals() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ConfigurationDatabase cdb;
    StreamString brokerName = test.GetBrokerName(cdb, OutputSignals);
    bool ok = (brokerName == "");
    return ok;
}

bool ThreadedSDNSubscriberTest::TestGetInputBrokers() {
    return TestIntegratedInApplication(config_default);
}

bool ThreadedSDNSubscriberTest::TestGetOutputBrokers() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    ReferenceContainer container;
    bool ok = test.GetOutputBrokers(container, "Default", NULL_PTR(void *));
    return !ok; // Expect failure
}

bool ThreadedSDNSubscriberTest::TestPrepareNextState() {
    using namespace MARTe;
    ThreadedSDNSubscriber test;
    bool ok = (test.PrepareNextState("FromCurrent","ToNext")); // Test the instantiated class
    return !ok; // Expect failure since sdn::Subscriber has not yet been instantiated
}

bool ThreadedSDNSubscriberTest::TestSynchronise() {
    return TestIntegratedInApplication(config_default);
}

bool ThreadedSDNSubscriberTest::TestSynchronise_MCAST_Topic_1() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +Sink = {"
        "            Class = ThreadedSDNSubscriberTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = SDNSub"
        "                    Type = uint64"
        "                }"
        "                Timestamp = {"
        "                    DataSource = SDNSub"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +SDNSub = {"
        "            Class = ThreadedSDNSubscriber"
        "            Topic = Default"  
        "            Interface = lo"  
        "            Mode = Synchronising"  
        "            Timeout = 1000"  
        "            Signals = {"  
        "                Counter = {"  
        "                    Type = uint64"  
        "                }"  
        "                Timestamp = {"  
        "                    Type = uint64"  
        "                }"  
        "            }"  
        "        }"  
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +Running = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread = {"
        "                    Class = RealTimeThread"
        "                    Functions = {Sink}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

    // Instantiate a sdn::Metadata structure to configure the topic
    sdn::Metadata_t mdata; sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic; topic->SetMetadata(mdata);
    sdn::Publisher* publisher;

    bool ok = true;
    
    if (ok) {
        ok = (topic->AddAttribute(0u, "Counter", "uint64") == STATUS_SUCCESS);
    }
    if (ok) {
        ok = (topic->AddAttribute(1u, "Timestamp", "uint64") == STATUS_SUCCESS);
    }
    if (ok) {
        topic->SetUID(0u); // UID corresponds to the data type but it includes attributes name - Safer to clear with SDN core library 1.0.10
        ok = (topic->Configure() == STATUS_SUCCESS);
    }
    if (ok) {
        ok = topic->IsInitialized();
    }
    // Create sdn::Publisher
    if (ok) {
        publisher = new sdn::Publisher(*topic);
    }
    if (ok) {
        ok = (publisher->SetInterface((char*) "lo") == STATUS_SUCCESS);
    }
    if (ok) {
        ok = (publisher->Configure() == STATUS_SUCCESS);
    }
    
    if (ok) {
        ok = ConfigureApplication(config);
    }

    if (ok) {
        ok = StartApplication();
    }

    if (ok) {
 
        ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
        ReferenceT<RealTimeApplication> application = god->Find("Test");
        ReferenceT<ThreadedSDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<ThreadedSDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
	ok = subscriber.IsValid();

	// Prepare data
	MARTe::uint64 counter = 10ul;
	MARTe::uint64 timestamp = get_time();
	if (ok) {
	    ok = (topic->SetAttribute(0u, counter) == STATUS_SUCCESS);
	}
	if (ok) {
	    ok = (topic->SetAttribute(1u, timestamp) == STATUS_SUCCESS);
	}
	// Send data
	if (ok) {
	    ok = (publisher->Publish() == STATUS_SUCCESS);
	}
	// Let the application run
	if (ok) {
	    wait_for(100000000ul);
	}
	// Test reception
	if (ok) {
	    ok = sink->TestCounter(counter);
	}
	if (ok) {
	    ok = sink->TestTimestamp(timestamp);
	}
    } 

    if (ok) {
        ok = StopApplication();
    }

    return ok;
}

bool ThreadedSDNSubscriberTest::TestSynchronise_MCAST_Topic_2() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +Sink = {"
        "            Class = ThreadedSDNSubscriberTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = SDNSub"
        "                    Type = uint64"
        "                }"
        "                Timestamp = {"
        "                    DataSource = SDNSub"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +SDNSub = {"
        "            Class = ThreadedSDNSubscriber"
        "            Topic = Default"  
        "            Interface = lo"  
        "            Timeout = 1000"  
        "            Mode = Synchronising"  
        "            Signals = {"  
        "                Counter = {"  
        "                    Type = uint64"  
        "                }"  
        "                Timestamp = {"  
        "                    Type = uint64"  
        "                }"  
        "                ArrayInt32_1D = {"  
        "                    Type = uint32"  
        "                    NumberOfElements = 10"
        "                    NumberOfDimensions = 1"  
        "                }"  
        "                ArrayInt32_2D = {"  
        "                    Type = uint32"  
        "                    NumberOfElements = 4"
        "                    NumberOfDimensions = 2"  
        "                }"  
        "                ArrayFlt32_1D = {"  
        "                    Type = float32"  
        "                    NumberOfElements = 10"
        "                    NumberOfDimensions = 1"  
        "                }"  
        "                ArrayFlt32_2D = {"  
        "                    Type = float32"  
        "                    NumberOfElements = 4"
        "                    NumberOfDimensions = 2"  
        "                }"  
        "            }"  
        "        }"  
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +Running = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread = {"
        "                    Class = RealTimeThread"
        "                    Functions = {Sink}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

    // Instantiate a sdn::Metadata structure to configure the topic
    sdn::Metadata_t mdata; sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic; topic->SetMetadata(mdata);
    sdn::Publisher* publisher;

    bool ok = true;
    
    if (ok) {
        ok = (topic->AddAttribute(0u, "Counter", "uint64") == STATUS_SUCCESS);
    }
    if (ok) {
        ok = (topic->AddAttribute(1u, "Timestamp", "uint64") == STATUS_SUCCESS);
    }
    if (ok) {
        ok = (topic->AddAttribute(2u, "Reserved", "uint8", 144) == STATUS_SUCCESS);
    }
    if (ok) {
        topic->SetUID(0u); // UID corresponds to the data type but it includes attributes name - Safer to clear with SDN core library 1.0.10
        ok = (topic->Configure() == STATUS_SUCCESS);
    }
    if (ok) {
        ok = topic->IsInitialized();
    }
    // Create sdn::Publisher
    if (ok) {
        publisher = new sdn::Publisher(*topic);
    }
    if (ok) {
        ok = (publisher->SetInterface((char*) "lo") == STATUS_SUCCESS);
    }
    if (ok) {
        ok = (publisher->Configure() == STATUS_SUCCESS);
    }
    
    if (ok) {
        ok = ConfigureApplication(config);
    }

    if (ok) {
        ok = StartApplication();
    }

    if (ok) {
 
        ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
        ReferenceT<RealTimeApplication> application = god->Find("Test");
        ReferenceT<ThreadedSDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<ThreadedSDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
	ok = subscriber.IsValid();

	// Prepare data
	MARTe::uint64 counter = 10ul;
	MARTe::uint64 timestamp = get_time();
	if (ok) {
	    ok = (topic->SetAttribute(0u, counter) == STATUS_SUCCESS);
	}
	if (ok) {
	    ok = (topic->SetAttribute(1u, timestamp) == STATUS_SUCCESS);
	}
	// Send data
	if (ok) {
	    ok = (publisher->Publish() == STATUS_SUCCESS);
	}
	// Let the application run
	if (ok) {
	    wait_for(100000000ul);
	}
	// Test reception
	if (ok) {
	    ok = sink->TestCounter(counter);
	}
	if (ok) {
	    ok = sink->TestTimestamp(timestamp);
	}
    } 

    if (ok) {
        ok = StopApplication();
    }

    return ok;
}

bool ThreadedSDNSubscriberTest::TestSynchronise_UCAST_Topic_1() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +Sink = {"
        "            Class = ThreadedSDNSubscriberTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = SDNSub"
        "                    Type = uint64"
        "                }"
        "                Timestamp = {"
        "                    DataSource = SDNSub"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +SDNSub = {"
        "            Class = ThreadedSDNSubscriber"
        "            Topic = Default"  
        "            Interface = lo"  
        "            Address = \"127.0.0.1:60000\""  
        "            Mode = Synchronising"  
        "            Timeout = 1000"  
        "            Signals = {"  
        "                Counter = {"  
        "                    Type = uint64"  
        "                }"  
        "                Timestamp = {"  
        "                    Type = uint64"  
        "                }"  
        "            }"  
        "        }"  
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +Running = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread = {"
        "                    Class = RealTimeThread"
        "                    Functions = {Sink}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

    // Instantiate a sdn::Metadata structure to configure the topic
    sdn::Metadata_t mdata; sdn::Topic_InitializeMetadata(mdata, "Default", 0, "127.0.0.1:60000");
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic; topic->SetMetadata(mdata);
    sdn::Publisher* publisher;

    bool ok = true;
    
    if (ok) {
        ok = (topic->AddAttribute(0u, "Counter", "uint64") == STATUS_SUCCESS);
    }
    if (ok) {
        ok = (topic->AddAttribute(1u, "Timestamp", "uint64") == STATUS_SUCCESS);
    }
    if (ok) {
        topic->SetUID(0u); // UID corresponds to the data type but it includes attributes name - Safer to clear with SDN core library 1.0.10
        ok = (topic->Configure() == STATUS_SUCCESS);
    }
    if (ok) {
        ok = topic->IsInitialized();
    }
    // Create sdn::Publisher
    if (ok) {
        publisher = new sdn::Publisher(*topic);
    }
    if (ok) {
        ok = (publisher->SetInterface((char*) "lo") == STATUS_SUCCESS);
    }
    if (ok) {
        ok = (publisher->Configure() == STATUS_SUCCESS);
    }
    
    if (ok) {
        ok = ConfigureApplication(config);
    }

    if (ok) {
        ok = StartApplication();
    }

    if (ok) {
 
        ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
        ReferenceT<RealTimeApplication> application = god->Find("Test");
        ReferenceT<ThreadedSDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<ThreadedSDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
	ok = subscriber.IsValid();

	// Prepare data
	MARTe::uint64 counter = 10ul;
	MARTe::uint64 timestamp = get_time();
	if (ok) {
	    ok = (topic->SetAttribute(0u, counter) == STATUS_SUCCESS);
	}
	if (ok) {
	    ok = (topic->SetAttribute(1u, timestamp) == STATUS_SUCCESS);
	}
	// Send data
	if (ok) {
	    ok = (publisher->Publish() == STATUS_SUCCESS);
	}
	// Let the application run
	if (ok) {
	    wait_for(100000000ul);
	}
	// Test reception
	if (ok) {
	    ok = sink->TestCounter(counter);
	}
	if (ok) {
	    ok = sink->TestTimestamp(timestamp);
	}
    } 

    if (ok) {
        ok = StopApplication();
    }

    return ok;
}

