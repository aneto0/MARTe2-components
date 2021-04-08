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

    SDNSubscriberTestGAM() :
            GAM() {
        counter = 0lu;
        timestamp = 0lu;
        headerSize = 0u;
        header = NULL_PTR(MARTe::char8 *);
    }

    virtual ~SDNSubscriberTestGAM() {
        if (header != NULL_PTR(MARTe::char8 *)) {
            delete[] header;
        }
    }

    bool Execute() {

        if (GetNumberOfInputSignals() > 0u) {
            MARTe::MemoryOperationsHelper::Copy(&counter, GetInputSignalMemory(0u), sizeof(MARTe::uint64));
        }

        if (GetNumberOfInputSignals() > 1u) {
            MARTe::MemoryOperationsHelper::Copy(&timestamp, GetInputSignalMemory(1u), sizeof(MARTe::uint64));
        }

        if (GetNumberOfInputSignals() > 2u) {
            MARTe::MemoryOperationsHelper::Copy(&header[0], GetInputSignalMemory(2u), headerSize);
        }

        return true;
    }

    bool Setup() {
        bool ok = true;
        if (GetNumberOfInputSignals() > 2u) {
            ok = GetSignalByteSize(MARTe::InputSignals, 2u, headerSize);
            if (ok) {
                header = new MARTe::char8[headerSize];
            }
        }
        return ok;
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

    bool TestHeader(sdn::Header_t *publishedHeader) {
        using namespace MARTe;
        bool ok = (header != NULL_PTR(char8 *));
        sdn::Header_t *h;
        if (ok) {
            h = reinterpret_cast<sdn::Header_t *>(&header[0]);
        }
        if (ok) {
            ok = (h->header_size == headerSize);
        }
        if (ok) {
            ok = (h->header_uid[0] == publishedHeader->header_uid[0]);
            ok &= (h->header_uid[1] == publishedHeader->header_uid[1]);
            ok &= (h->header_uid[2] == publishedHeader->header_uid[2]);
            ok &= (h->header_uid[3] == publishedHeader->header_uid[3]);
        }
        if (ok) {
            ok = (h->header_version[0] == publishedHeader->header_version[0]);
            ok &= (h->header_version[1] == publishedHeader->header_version[1]);
            ok &= (h->header_version[2] == publishedHeader->header_version[2]);
            ok &= (h->header_version[3] == publishedHeader->header_version[3]);
        }
        if (ok) {
            ok = (h->topic_uid == publishedHeader->topic_uid);
        }
        if (ok) {
            ok = (h->topic_version == publishedHeader->topic_version);
        }
        if (ok) {
            ok = (h->recv_time > 0u);
            log_info("SDNSubscriberTestGAM::TestHeader - recv_time - '%lu' '%lu'", h->recv_time, publishedHeader->recv_time);
        }
        if (ok) {
            ok = (h->send_time == publishedHeader->send_time);
            log_info("SDNSubscriberTestGAM::TestHeader - send_time - '%lu' '%lu'", h->send_time, publishedHeader->send_time);
        }
        return ok;
    }

    MARTe::uint64 counter;
    MARTe::uint64 timestamp;
    MARTe::char8 *header;
    MARTe::uint32 headerSize;

};

CLASS_REGISTER(SDNSubscriberTestGAM, "1.0")

class SDNSubscriberTestConstantGAM: public MARTe::GAM {

public:

    CLASS_REGISTER_DECLARATION()

    SDNSubscriberTestConstantGAM() :
            GAM() {
        dflt = 0;
    }

    ~SDNSubscriberTestConstantGAM() {
    }

    bool Execute() {

        MARTe::uint32 nOfSignals = GetNumberOfOutputSignals();
        MARTe::uint32 signalIndex = 0u;

        for (signalIndex = 0; signalIndex < nOfSignals; signalIndex++) {
            MARTe::uint32 byteSize = 0u;
            GetSignalByteSize(MARTe::OutputSignals, signalIndex, byteSize);
            MARTe::MemoryOperationsHelper::Set(GetOutputSignalMemory(signalIndex), (MARTe::char8) dflt, byteSize);
        }

        return true;
    }

    bool Setup() {
        return true;
    }

    bool SetDefault(MARTe::char8 dflt) {
        this->dflt = dflt;
        return true;
    }

    MARTe::char8 dflt;

};

CLASS_REGISTER(SDNSubscriberTestConstantGAM, "1.0")

class SDNSubscriberTestSinkGAM: public MARTe::GAM {

public:

    CLASS_REGISTER_DECLARATION()

    SDNSubscriberTestSinkGAM() :
            GAM() {
    }

    ~SDNSubscriberTestSinkGAM() {
    }

    bool Execute() {
        return true;
    }

    bool Setup() {
        return true;
    }

    template<typename Type> bool TestSignal(MARTe::uint32 signalIndex = 0u,
                                            Type value = 0) {

        bool ok = (signalIndex < GetNumberOfInputSignals());

        if (ok) {
            ok = (value == *(static_cast<Type*>(GetInputSignalMemory(signalIndex))));
        }

        return ok;
    }

};

CLASS_REGISTER(SDNSubscriberTestSinkGAM, "1.0")

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
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "StandardParser::Parse failed");
    }
    else {
        god->Purge();
        ok = god->Initialise(cdb);
    }

    ReferenceT<RealTimeApplication> application;

    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "RealTimeApplication::IsValid failed");
    }
    else {
        ok = application->ConfigureApplication();
    }

    return ok;

}

static inline bool StartApplication(const MARTe::char8 * const state = "Running") {

    using namespace MARTe;

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");
    ;
    bool ok = application.IsValid();

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "RealTimeApplication::IsValid failed");
    }
    else {
        ok = application->PrepareNextState(state);
    }
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "RealTimeApplication::PrepareNextState failed");
    }
    else {
        ok = application->StartNextStateExecution();
    }

    return ok;

}

static inline bool StopApplication() {

    using namespace MARTe;

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");
    ;
    bool ok = application.IsValid();

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "RealTimeApplication::IsValid failed");
    }
    else {
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

// Standard configuration for testing with header
const MARTe::char8 * const config_default_header = ""
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
        "                Header = {"
        "                    Type = uint8"
        "                    NumberOfElements = 48"
        "                }"
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
    bool ok = (test.PrepareNextState("FromCurrent", "ToNext")); // Test the instantiated class
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

bool SDNSubscriberTest::TestInitialise_CPUMask() {
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    uint32 cpumask = 1u;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("CPUs", cpumask);
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

bool SDNSubscriberTest::TestInitialise_False_Invalid_ExecutionMode(){
    using namespace MARTe;
    SDNSubscriber test;
    ConfigurationDatabase cdb;
    uint32 cpumask = 1u;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("CPUs", cpumask);
    cdb.Write("ExecutionMode", "Invalid");
    return !test.Initialise(cdb);
}


bool SDNSubscriberTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config_default);
}

bool SDNSubscriberTest::TestSetConfiguredDatabase_Header() {
    return TestIntegratedInApplication(config_default_header);
}

bool SDNSubscriberTest::TestSetConfiguredDatabase_False_NOfSignals_1() {
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Source = {"
            "            Class = SDNSubscriberTestConstantGAM"
            "            OutputSignals = {"
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
            "        +Sink = {"
            "            Class = SDNSubscriberTestSinkGAM"
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
            "                    Functions = {Source Sink}"
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

bool SDNSubscriberTest::TestSetConfiguredDatabase_False_NOfSignals_2() {
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Sink = {"
            "            Class = SDNSubscriberTestSinkGAM"
            "            InputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNSub"
            "                    Type = uint64"
            "                    Frequency = 1."
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNSub"
            "                    Type = uint64"
            "                    Frequency = 1."
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

bool SDNSubscriberTest::TestAllocateMemory() {
    return TestIntegratedInApplication(config_default);
}

bool SDNSubscriberTest::TestAllocateMemory_False() {
    using namespace MARTe;
    SDNSubscriber test;
    bool ok = test.AllocateMemory();
    return !ok; // Expect failure
}

bool SDNSubscriberTest::TestAllocateMemory_False_Header_Size() {
    // Standard configuration for testing with header
    const MARTe::char8 * const config_default_header_wrong_size = ""
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
            "                Header = {"
            "                    Type = uint8"
            "                    NumberOfElements = 49"
            "                }"
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
    return !TestIntegratedInApplication(config_default_header_wrong_size);
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
            "            Class = SDNSubscriberTestSinkGAM"
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
            "            Class = SDNSubscriberTestSinkGAM"
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
            "            Class = SDNSubscriberTestSinkGAM"
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
    bool ok = (test.PrepareNextState("FromCurrent", "ToNext")); // Test the instantiated class
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
            wait_for(500000000ul);
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

bool SDNSubscriberTest::TestSynchronise_MCAST_Topic_1_Header() {
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
            "                Header = {"
            "                    DataSource = SDNSub"
            "                    Type = uint8"
            "                    NumberOfElements = 48"
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
            "                Header = {"
            "                    Type = uint8"
            "                    NumberOfElements = 48"
            "                }"
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
            wait_for(500000000ul);
        }
        // Test reception
        if (ok) {
            ok = sink->TestCounter(counter);
        }
        if (ok) {
            ok = sink->TestTimestamp(timestamp);
        }
        if (ok) {
            sdn::Header_t *pheader = static_cast<sdn::Header_t *>(publisher->GetTopicHeader());
            ok = sink->TestHeader(pheader);
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
            "            ExecutionMode = IndependentThread"
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
            wait_for(500000000ul);
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
    // Synchronising signal
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
            wait_for(500000000ul);
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

bool SDNSubscriberTest::TestSynchronise_MCAST_Topic_4() {
    using namespace MARTe;
    // Two identical signal consumers, 1 synchronising signal
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
            wait_for(500000000ul);
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

bool SDNSubscriberTest::TestSynchronise_MCAST_Topic_5() {
    using namespace MARTe;
    // Two redundant subscriber-consumer, synchronising signal
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
            "                    DataSource = RedSDNSub"
            "                    Type = uint64"
            "                    Frequency = 1."
            "                }"
            "                Timestamp = {"
            "                    DataSource = RedSDNSub"
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
            "        +RedSDNSub = {"
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
            "                +RedThread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {RedSink}"
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
            wait_for(500000000ul);
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

bool SDNSubscriberTest::TestSynchronise_MCAST_Topic_6() {
    using namespace MARTe;
    // Two consumer, distinct portions of the payload, synchronising signal
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
            "        +DAC = {"
            "            Class = SDNSubscriberTestSinkGAM"
            "            InputSignals = {"
            "                ArrayInt32_1D = {"
            "                    DataSource = SDNSub"
            "                    Type = uint32"
            "                    NumberOfElements = 10"
            "                    NumberOfDimensions = 1"
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
            "            CPUs = 1"
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
            "                    Functions = {Sink DAC}"
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
        ReferenceT<SDNSubscriberTestSinkGAM> dac = application->Find("Functions.DAC");
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
        if (ok) {
            *((MARTe::uint32 *) topic->GetTypeDefinition()->GetAttributeReference(2u)) = 101u;
        }
        // Send data
        if (ok) {
            ok = (publisher->Publish() == STATUS_SUCCESS);
        }
        // Let the application run
        if (ok) {
            wait_for(500000000ul);
        }
        // Test reception
        if (ok) {
            ok = sink.IsValid();
        }
        if (ok) {
            ok = sink->TestCounter(counter);
        }
        if (ok) {
            ok = sink->TestTimestamp(timestamp);
        }
        if (ok) {
            ok = dac.IsValid();
        }
        if (ok) {
            ok = dac->TestSignal(0u, (MARTe::uint32) 101u);
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
            "            CPUs = 1"
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0, "127.0.0.1:60000");
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
            wait_for(500000000ul);
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
            "            CPUs = 1"
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0, "127.0.0.1:60000");
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
            wait_for(500000000ul);
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
            "            CPUs = 1"
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0, "127.0.0.1:60000");
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
            wait_for(500000000ul);
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

bool SDNSubscriberTest::TestSynchronise_MCAST_Topic_RTT() {
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
            "                    Frequency = 1."
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
            "            ExecutionMode = RealTimeThread"
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
            "            InternalTimeout = 1000000"
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
            wait_for(500000000ul);
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

bool SDNSubscriberTest::TestSynchronise_MCAST_Topic_RTT_Trigger() {
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
            "                    Trigger = 1"
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
            "            ExecutionMode = RealTimeThread"
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
            "            InternalTimeout = 1000000"
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
            wait_for(500000000ul);
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

bool SDNSubscriberTest::TestSynchronise_MCAST_Topic_RTT_GetLatest() {
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
            "                    Frequency = 0."
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
            "            ExecutionMode = RealTimeThread"
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
            "            InternalTimeout = 1000000"
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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

        ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
        ReferenceT<RealTimeApplication> application = god->Find("Test");
        ReferenceT<SDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<SDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
        ok = subscriber.IsValid();

        // Prepare data
        MARTe::uint64 counter = 1ul;
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

        ReferenceContainer inputBrokers;
        sink->GetInputBrokers(inputBrokers);

        for (uint32 i = 0u; i < inputBrokers.Size(); i++) {
            ReferenceT<BrokerI> broker = inputBrokers.Get(i);
            if (broker.IsValid()) {
                broker->Execute();
            }
        }

        sink->Execute();
        // Test reception
        if (ok) {
            ok = sink->TestCounter(counter);
        }
        if (ok) {
            ok = sink->TestTimestamp(timestamp);
        }

        for (uint32 i = 0u; i < 10u; i++) {
            // Prepare data
            counter = i;
            timestamp = get_time();
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
        }

        for (uint32 i = 0u; i < inputBrokers.Size(); i++) {
            ReferenceT<BrokerI> broker = inputBrokers.Get(i);
            if (broker.IsValid()) {
                broker->Execute();
            }
        }
        // Test reception last packet
        sink->Execute();
        if (ok) {
            ok = sink->TestCounter(counter);
        }
        if (ok) {
            ok = sink->TestTimestamp(timestamp);
        }
    }

    return ok;
}

bool SDNSubscriberTest::TestSynchronise_MCAST_Timeout() {
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
            "                    Frequency = 0."
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

    bool ok = ConfigureApplication(config);

    if (ok) {

        ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
        ReferenceT<RealTimeApplication> application = god->Find("Test");
        ReferenceT<SDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<SDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
        ok = subscriber.IsValid();

        if (ok) {
            //fails after timeout
            ok = !subscriber->Synchronise();
        }
    }

    return ok;
}

bool SDNSubscriberTest::TestSynchronise_MCAST_RTT_Timeout() {
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
            "                    Frequency = 0."
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
            "            ExecutionMode = RealTimeThread"
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
            "            InternalTimeout = 1000000000"
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

    bool ok = ConfigureApplication(config);

    if (ok) {

        ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
        ReferenceT<RealTimeApplication> application = god->Find("Test");
        ReferenceT<SDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<SDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
        ok = subscriber.IsValid();

        if (ok) {
            //fails after timeout
            ok = !subscriber->Synchronise();
        }
    }

    return ok;
}



bool SDNSubscriberTest::TestSynchronise_MCAST_IgnoreTimeoutError() {
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
            "                    Frequency = 0."
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
            "            IgnoreTimeoutError = 1"
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

    bool ok = ConfigureApplication(config);

    if (ok) {

        ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
        ReferenceT<RealTimeApplication> application = god->Find("Test");
        ReferenceT<SDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<SDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
        ok = subscriber.IsValid();

        if (ok) {
            //ignore timeout
            ok = subscriber->Synchronise();
        }
    }

    return ok;
}

bool SDNSubscriberTest::TestSynchronise_MCAST_RTT_IgnoreTimeoutError() {
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
            "                    Frequency = 0."
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
            "            ExecutionMode = RealTimeThread"
            "            Class = SDNSubscriber"
            "            Topic = Default"
            "            Interface = lo"
            "            InternalTimeout = 1000000000"
            "            IgnoreTimeoutError = 1"
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

    bool ok = ConfigureApplication(config);

    if (ok) {

        ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
        ReferenceT<RealTimeApplication> application = god->Find("Test");
        ReferenceT<SDNSubscriber> subscriber = application->Find("Data.SDNSub");
        ReferenceT<SDNSubscriberTestGAM> sink = application->Find("Functions.Sink");
        ok = subscriber.IsValid();

        if (ok) {
            //ignore timeout
            ok = subscriber->Synchronise();
        }
    }

    return ok;
}


#ifdef FEATURE_10840
bool SDNSubscriberTest::TestSynchronise_NetworkByteOrder_Topic_1() {
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
        publisher->SetPayloadOrder(sdn::types::NetworkByteOrder);
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
        // Convert to network byte order, if required
        if (ok) {
            if (sdn::HelperTools::IsLittleEndian()) {
                ok = (topic->SetAttribute(0u, sdn::HelperTools::ToNetworkByteOrder<sdn::types::uint64_t>(static_cast<sdn::types::uint64_t>(counter)))
                        == STATUS_SUCCESS);
            }
            else {
                ok = (topic->SetAttribute(0u, counter) == STATUS_SUCCESS);
            }
        }
        if (ok) {
            if (sdn::HelperTools::IsLittleEndian()) {
                ok = (topic->SetAttribute(1u, sdn::HelperTools::ToNetworkByteOrder<sdn::types::uint64_t>(static_cast<sdn::types::uint64_t>(timestamp)))
                        == STATUS_SUCCESS);
            }
            else {
                ok = (topic->SetAttribute(1u, timestamp) == STATUS_SUCCESS);
            }
        }
        // Send data
        if (ok) {
            ok = (publisher->Publish() == STATUS_SUCCESS);
        }
        // Let the application run
        if (ok) {
            wait_for(500000000ul);
        }
        // Test reception
        if (ok) {
            ok = sink.IsValid();
        }
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

bool SDNSubscriberTest::TestSynchronise_NetworkByteOrder_Topic_2() {
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
            "        +DAC = {"
            "            Class = SDNSubscriberTestSinkGAM"
            "            InputSignals = {"
            "                ArrayInt32_1D = {"
            "                    DataSource = SDNSub"
            "                    Type = uint32"
            "                    NumberOfElements = 10"
            "                    NumberOfDimensions = 1"
            "                }"
            "            }"
            "        }"
            "        +DAC16 = {"
            "            Class = SDNSubscriberTestSinkGAM"
            "            InputSignals = {"
            "                ArrayInt16_1D = {"
            "                    DataSource = SDNSub"
            "                    Type = uint16"
            "                    NumberOfElements = 10"
            "                    NumberOfDimensions = 1"
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
            "                ArrayInt16_1D = {"
            "                    Type = uint16"
            "                    NumberOfElements = 10"
            "                    NumberOfDimensions = 1"
            "                }"
            "                ArrayInt32_2D = {"
            "                    Type = uint32"
            "                    NumberOfElements = 4"
            "                    NumberOfDimensions = 2"
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
            "                    Functions = {Sink DAC DAC16}"
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
    sdn::Publisher* publisher;

    bool ok = true;

    if (ok) {
        ok = (topic->AddAttribute(0u, "Counter", "uint64") == STATUS_SUCCESS);
    }
    if (ok) {
        ok = (topic->AddAttribute(1u, "Timestamp", "uint64") == STATUS_SUCCESS);
    }
    if (ok) {
        ok = (topic->AddAttribute(2u, "ArrayInt32_1D", "uint32", 10) == STATUS_SUCCESS);
    }
    if (ok) {
        ok = (topic->AddAttribute(3u, "ArrayInt16_1D", "uint16", 10) == STATUS_SUCCESS);
    }
    if (ok) {
        ok = (topic->AddAttribute(4u, "Reserved", "uint8", 64) == STATUS_SUCCESS);
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
        publisher->SetPayloadOrder(sdn::types::NetworkByteOrder);
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
        ReferenceT<SDNSubscriberTestSinkGAM> dac = application->Find("Functions.DAC");
        ReferenceT<SDNSubscriberTestSinkGAM> dac16 = application->Find("Functions.DAC16");
        ok = subscriber.IsValid();

        // Prepare data
        MARTe::uint64 counter = 10ul;
        MARTe::uint64 timestamp = get_time();
        MARTe::uint32 element = 101u;
        MARTe::uint16 element16 = 24u;
        // Convert to network byte order, if required
        if (ok) {
            if (sdn::HelperTools::IsLittleEndian()) {
                ok = (topic->SetAttribute(0u, sdn::HelperTools::ToNetworkByteOrder<sdn::types::uint64_t>(static_cast<sdn::types::uint64_t>(counter)))
                        == STATUS_SUCCESS);
            }
            else {
                ok = (topic->SetAttribute(0u, counter) == STATUS_SUCCESS);
            }
        }
        if (ok) {
            if (sdn::HelperTools::IsLittleEndian()) {
                ok = (topic->SetAttribute(1u, sdn::HelperTools::ToNetworkByteOrder<sdn::types::uint64_t>(static_cast<sdn::types::uint64_t>(timestamp)))
                        == STATUS_SUCCESS);
            }
            else {
                ok = (topic->SetAttribute(1u, timestamp) == STATUS_SUCCESS);
            }
        }
        if (ok) {
            MARTe::uint32* ref = static_cast<MARTe::uint32*>(topic->GetTypeDefinition()->GetAttributeReference(2u));
            if (sdn::HelperTools::IsLittleEndian()) {
                *ref = sdn::HelperTools::ToNetworkByteOrder<sdn::types::uint32_t>(static_cast<sdn::types::uint32_t>(element));
            }
            else {
                *ref = element;
            }
        }
        if (ok) {
            MARTe::uint16* ref = static_cast<MARTe::uint16*>(topic->GetTypeDefinition()->GetAttributeReference(3u));
            if (sdn::HelperTools::IsLittleEndian()) {
                *ref = sdn::HelperTools::ToNetworkByteOrder<sdn::types::uint16_t>(static_cast<sdn::types::uint16_t>(element16));
            }
            else {
                *ref = element16;
            }
        }
        // Send data
        if (ok) {
            ok = (publisher->Publish() == STATUS_SUCCESS);
        }
        // Let the application run
        if (ok) {
            wait_for(500000000ul);
        }
        // Test reception
        if (ok) {
            ok = sink.IsValid();
        }
        if (ok) {
            ok = sink->TestCounter(counter);
        }
        if (ok) {
            ok = sink->TestTimestamp(timestamp);
        }
        if (ok) {
            ok = dac.IsValid();
        }
        if (ok) {
            ok = dac->TestSignal(0u, (MARTe::uint32) 101u);
        }
        if (ok) {
            ok = dac16->TestSignal(0u, (MARTe::uint16) 24u);
        }

    }

    if (ok) {
        ok = StopApplication();
    }

    return ok;
}

bool SDNSubscriberTest::TestSynchronise_NetworkByteOrder_Topic_1_Header() {
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
            "                Header = {"
            "                    DataSource = SDNSub"
            "                    Type = uint8"
            "                    NumberOfElements = 48"
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
            "                Header = {"
            "                    Type = uint8"
            "                    NumberOfElements = 48"
            "                }"
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
    sdn::Metadata_t mdata;
    sdn::Topic_InitializeMetadata(mdata, "Default", 0);
    // Instantiate SDN topic from metadata specification
    sdn::Topic* topic = new sdn::Topic;
    topic->SetMetadata(mdata);
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
        publisher->SetPayloadOrder(sdn::types::NetworkByteOrder);
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
        // Convert to network byte order, if required
        if (ok) {
            if (sdn::HelperTools::IsLittleEndian()) {
                ok = (topic->SetAttribute(0u, sdn::HelperTools::ToNetworkByteOrder<sdn::types::uint64_t>(static_cast<sdn::types::uint64_t>(counter)))
                        == STATUS_SUCCESS);
            }
            else {
                ok = (topic->SetAttribute(0u, counter) == STATUS_SUCCESS);
            }
        }
        if (ok) {
            if (sdn::HelperTools::IsLittleEndian()) {
                ok = (topic->SetAttribute(1u, sdn::HelperTools::ToNetworkByteOrder<sdn::types::uint64_t>(static_cast<sdn::types::uint64_t>(timestamp)))
                        == STATUS_SUCCESS);
            }
            else {
                ok = (topic->SetAttribute(1u, timestamp) == STATUS_SUCCESS);
            }
        }
        // Send data
        if (ok) {
            ok = (publisher->Publish() == STATUS_SUCCESS);
        }
        // Let the application run
        if (ok) {
            wait_for(500000000ul);
        }
        // Test reception
        if (ok) {
            ok = sink.IsValid();
        }
        if (ok) {
            ok = sink->TestCounter(counter);
        }
        if (ok) {
            ok = sink->TestTimestamp(timestamp);
        }
        if (ok) {
            sdn::Header_t *pheader = static_cast<sdn::Header_t *>(publisher->GetTopicHeader());
            ok = sink->TestHeader(pheader);
        }
    }

    if (ok) {
        ok = StopApplication();
    }

    return ok;
}

#endif
bool SDNSubscriberTest::TestExecute_False() {
    using namespace MARTe;
    SDNSubscriber test;
    ExecutionInfo info;
    bool ok = test.Execute(info);
    return !ok; // Expect failure
}

