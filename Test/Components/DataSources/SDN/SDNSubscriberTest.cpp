/**
 * @file SDNSubscriberTest.cpp
 * @brief Source file for class SDNSubscriberTest
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
#include "SDNSubscriber.h"

#include "SDNSubscriberTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class SDNSubscriberTestGAM: public MARTe::GAM {

public:

    CLASS_REGISTER_DECLARATION()

    SDNSubscriberTestGAM() : GAM() {
        counter = 0lu;
        timestamp = 0lu;
    }

    ~SDNSubscriberTestGAM() { }

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
        log_info("SDNSubscriberTestGAM::TestCounter - '%lu %lu'", counter, value);
        return ok;
    }

    bool TestTimestamp(MARTe::uint64 value = 0ul) {
        bool ok = (timestamp == value);
        log_info("SDNSubscriberTestGAM::TestTimestamp - '%lu %lu'", timestamp, value);
        return ok;
    }

    MARTe::uint64 counter;
    MARTe::uint64 timestamp;

};

CLASS_REGISTER(SDNSubscriberTestGAM, "1.0")

class SinkGAM: public MARTe::GAM {

public:

    CLASS_REGISTER_DECLARATION()

    SinkGAM() : GAM() { }

    ~SinkGAM() {}

    bool Execute() {

        MARTe::uint32 nOfSignals = GetNumberOfInputSignals();
        MARTe::uint32 signalIndex = 0u;

        for (signalIndex = 0; signalIndex < nOfSignals; signalIndex++) {
            MARTe::uint32 byteSize = 0u; GetSignalByteSize(MARTe::InputSignals, signalIndex, byteSize);
        }

        return true;
    }

    bool Setup() {
       return true;
    }

    bool TestSignal(MARTe::uint32 signalIndex = 0u, MARTe::char8 byte = 0u) {

        MARTe::uint32 nOfSignals = GetNumberOfInputSignals();
        MARTe::uint32 byteSize = 0u; GetSignalByteSize(MARTe::InputSignals, signalIndex, byteSize);
        bool ok = (signalIndex < nOfSignals);

        if (ok) {
            ok = (byte == *((MARTe::char8 *) GetOutputSignalMemory(signalIndex)));
        }

        return ok;
    }

};

CLASS_REGISTER(SinkGAM, "1.0")

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

// Standard configuration for testing
const MARTe::char8 * const config_default = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +Sink = {"
        "            Class = SDNSubscriberTestGAM"
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
        "            Class = SDNSubscriber"
        "            Topic = Default"  
        "            Interface = lo"  
        "            Timeout = 100"
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

bool SDNSubscriberTest::TestConstructor() {
    using namespace MARTe;
    SDNSubscriber test;
    bool ok = (test.PrepareNextState("FromCurrent","ToNext")); // Test the instantiated class
    return !ok; // Expect failure since sdn::Subscriber has not yet been instantiated
}

bool SDNSubscriberTest::TestInitialise_Empty() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestInitialise_Default() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    return test.Initialise(cdb);
}

bool SDNSubscriberTest::TestInitialise_Address() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "127.0.0.1:60000");
    return test.Initialise(cdb);
}

bool SDNSubscriberTest::TestInitialise_Address_mcast() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "239.0.0.1:60000");
    return test.Initialise(cdb);
}

bool SDNSubscriberTest::TestInitialise_Address_ucast() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "10.0.0.1:60000");
    return test.Initialise(cdb);
}

bool SDNSubscriberTest::TestInitialise_ShortForm() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "sdn://127.0.0.1:60000/Default");
    cdb.Write("Interface", "lo");
    return test.Initialise(cdb);
}

bool SDNSubscriberTest::TestInitialise_Timeout_0ul() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    uint64 timeout = 0ul;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Timeout", timeout);
    return test.Initialise(cdb);
}

bool SDNSubscriberTest::TestInitialise_Timeout_10ms() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    uint64 timeout = 10000000ul;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Timeout", timeout);
    return test.Initialise(cdb);
}

bool SDNSubscriberTest::TestInitialise_Missing_Topic() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Interface", "lo");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestInitialise_Missing_Interface() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestInitialise_False_Interface_1() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestInitialise_False_Interface_2() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "NotAnInterface");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestInitialise_False_Address_1() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "NotAnAddress");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestInitialise_False_Address_2() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "255.255.255.255:60000");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestInitialise_False_Address_3() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "127.0.0.1");
    bool ok = test.Initialise(cdb);
    //return !ok; // Expect failure
    return ok; // It is actually successful !
}

bool SDNSubscriberTest::TestInitialise_False_Address_4() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", ":60000");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestInitialise_False_Address_5() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "127.0.0:60000");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config_default);
}

bool SDNSubscriberTest::TestSetConfiguredDatabase_False_NOfSignals() {
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Sink = {"
            "            Class = SDNSubscriberTestGAM"
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
            "            Class = SDNSubscriber"
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

bool SDNSubscriberTest::TestAllocateMemory() {
    return TestIntegratedInApplication(config_default);
}

bool SDNSubscriberTest::TestAllocateMemory_False() {
    using namespace MARTe;
    SDNSubscriber test;
    bool ok = test.AllocateMemory();
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    SDNSubscriber test;
    return (test.GetNumberOfMemoryBuffers() == 1u);
}

bool SDNSubscriberTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    return true;
}

bool SDNSubscriberTest::TestGetBrokerName_InputSignals() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    StreamString brokerName = test.GetBrokerName(cdb, OutputSignals);
    bool ok = (brokerName == "");

    if (ok) { // No trigger
        brokerName = test.GetBrokerName(cdb, InputSignals);
        ok = (brokerName == "MemoryMapInputBroker");
    }

    if (ok) { // With synchronisation
        cdb.Write("Frequency", 1.0F);
        brokerName = test.GetBrokerName(cdb, InputSignals);
        ok = (brokerName == "MemoryMapSynchronisedInputBroker");
    }

    return ok;
}

bool SDNSubscriberTest::TestGetBrokerName_OutputSignals() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    StreamString brokerName = test.GetBrokerName(cdb, OutputSignals);
    bool ok = (brokerName == "");
    return ok;
}

bool SDNSubscriberTest::TestGetInputBrokers() {
    return TestIntegratedInApplication(config_default);
}

bool SDNSubscriberTest::TestGetInputBrokers_1() {
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Sink = {"
            "            Class = SinkGAM"
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
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
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

    bool ok = ConfigureApplication(config);
    return ok;
}

bool SDNSubscriberTest::TestGetInputBrokers_2() {
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Sink = {"
            "            Class = SinkGAM"
            "            InputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNSub"
            "                    Type = uint64"
            "                    Frequency = 1."
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
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
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

    bool ok = ConfigureApplication(config);
    return ok;
}

bool SDNSubscriberTest::TestGetInputBrokers_3() {
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Sink = {"
            "            Class = SinkGAM"
            "            InputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNSub"
            "                    Type = uint64"
            "                    Frequency = 1."
            "                    Samples = 10"
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
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
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

    bool ok = ConfigureApplication(config);
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestGetOutputBrokers() {
    using namespace MARTe;
    SDNSubscriber test;
    ReferenceContainer container;
    bool ok = test.GetOutputBrokers(container, "Default", NULL_PTR(void *));
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestPrepareNextState() {
    using namespace MARTe;
    SDNSubscriber test;
    bool ok = (test.PrepareNextState("FromCurrent","ToNext")); // Test the instantiated class
    return !ok; // Expect failure since sdn::Subscriber has not yet been instantiated
}

bool SDNSubscriberTest::TestSynchronise() {
    return TestIntegratedInApplication(config_default);
}

bool SDNSubscriberTest::TestSynchronise_MCAST_Topic_1() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Sink = {"
            "            Class = SDNSubscriberTestGAM"
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
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
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
        ReferenceT<SDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<SDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
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

bool SDNSubscriberTest::TestSynchronise_MCAST_Topic_2() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Sink = {"
            "            Class = SDNSubscriberTestGAM"
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
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
            "            Timeout = 1000"
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
        ReferenceT<SDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<SDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
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

bool SDNSubscriberTest::TestSynchronise_MCAST_Topic_3() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Sink = {"
            "            Class = SDNSubscriberTestGAM"
            "            InputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNSub"
            "                    Type = uint64"
            "                    Frequency = 1."
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
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
            "            Timeout = 1000"
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
        ReferenceT<SDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<SDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
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

bool SDNSubscriberTest::TestSynchronise_UCAST_Topic_1() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Sink = {"
            "            Class = SDNSubscriberTestGAM"
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
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
            "            Address = \"127.0.0.1:60000\""
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
        ReferenceT<SDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<SDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
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

bool SDNSubscriberTest::TestSynchronise_UCAST_Topic_2() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Sink = {"
            "            Class = SDNSubscriberTestGAM"
            "            InputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNSub"
            "                    Type = uint64"
            "                    Frequency = 1."
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
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
            "            Address = \"127.0.0.1:60000\""
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
        ReferenceT<SDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<SDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
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

bool SDNSubscriberTest::TestSynchronise_UCAST_Topic_3() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Sink = {"
            "            Class = SDNSubscriberTestGAM"
            "            InputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNSub"
            "                    Type = uint64"
            "                    Frequency = 1."
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNSub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "        +RedSink = {"
            "            Class = SDNSubscriberTestGAM"
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
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
            "            Address = \"127.0.0.1:60000\""
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
            "                    Functions = {Sink RedSink}"
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
        ReferenceT<SDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<SDNSubscriberTestGAM> sink_pri = application->Find("Functions.Sink");
        ReferenceT<SDNSubscriberTestGAM> sink_red = application->Find("Functions.RedSink");
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
            ok = sink_pri->TestCounter(counter);
            ok = sink_red->TestCounter(counter);
        }
        if (ok) {
            ok = sink_pri->TestTimestamp(timestamp);
            ok = sink_red->TestTimestamp(timestamp);
        }
    }

    if (ok) {
        ok = StopApplication();
    }

    return ok;
}

