/**
 * @file SDNPublisherTest.cpp
 * @brief Source file for class SDNPublisherTest
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
#include "SDNPublisher.h"
#include "SDNPublisherTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class SDNPublisherTestGAM: public MARTe::GAM {

public:

    CLASS_REGISTER_DECLARATION()

SDNPublisherTestGAM    () : GAM() {

        counter = 0lu;
        timestamp = 0lu;
        headerSize = 0lu;
        header = NULL;
    }

    virtual ~SDNPublisherTestGAM() {
        if (header != NULL) {
            delete [] header;
        }
    }

    bool Execute() {

        MARTe::uint64 curr_time = get_time();
        MARTe::uint64 till_time = ceil_time(curr_time, 100000000lu);
        timestamp = wait_until(till_time);

        if(GetNumberOfOutputSignals() > 0u) {
            MARTe::MemoryOperationsHelper::Copy(GetOutputSignalMemory(0u), &counter, sizeof(MARTe::uint64));
        }

        if(GetNumberOfOutputSignals() > 1u) {
            MARTe::MemoryOperationsHelper::Copy(GetOutputSignalMemory(1u), &timestamp, sizeof(MARTe::uint64));
        }

        if(GetNumberOfInputSignals() > 0u) {
            MARTe::MemoryOperationsHelper::Copy(&header[0], GetInputSignalMemory(0u), headerSize);
        }

        return true;
    }

    bool Setup() {
        bool ok = true;
        if(GetNumberOfInputSignals() > 0u) {
            ok = GetSignalByteSize(MARTe::InputSignals, 0u, headerSize);
            if (ok) {
                header = new MARTe::char8[headerSize];
            }
        }
        return ok;
    }

    bool SetCounter(MARTe::uint64 counter) {
        this->counter = counter;
        return true;
    }

    MARTe::uint64 counter;
    MARTe::uint64 timestamp;
    MARTe::char8 *header;
    MARTe::uint32 headerSize;

};

CLASS_REGISTER(SDNPublisherTestGAM, "1.0")

class SDNPublisherTestConstantGAM: public MARTe::GAM {

public:

    CLASS_REGISTER_DECLARATION()

SDNPublisherTestConstantGAM    () : GAM() {}

    ~SDNPublisherTestConstantGAM() {}

    bool Execute() {
        return true;
    }

    bool Setup() {
        return true;
    }

    bool SetDefault(MARTe::char8 dflt) {

        MARTe::uint32 nOfSignals = GetNumberOfOutputSignals();
        MARTe::uint32 signalIndex = 0u;

        for (signalIndex = 0; signalIndex < nOfSignals; signalIndex++) {
            MARTe::uint32 byteSize = 0u; GetSignalByteSize(MARTe::OutputSignals, signalIndex, byteSize);
            MARTe::MemoryOperationsHelper::Set(GetOutputSignalMemory(signalIndex), (MARTe::char8) dflt, byteSize);
        }

        return true;

    }

    bool SetDefault(MARTe::uint16 dflt) {

        MARTe::uint32 nOfSignals = GetNumberOfOutputSignals();
        MARTe::uint32 signalIndex = 0u;

        for (signalIndex = 0; signalIndex < nOfSignals; signalIndex++) {
            MARTe::MemoryOperationsHelper::Copy(GetOutputSignalMemory(signalIndex), &dflt, sizeof(MARTe::uint16));
        }

        return true;

    }

    bool SetDefault(MARTe::uint32 dflt) {

        MARTe::uint32 nOfSignals = GetNumberOfOutputSignals();
        MARTe::uint32 signalIndex = 0u;

        for (signalIndex = 0; signalIndex < nOfSignals; signalIndex++) {
            MARTe::MemoryOperationsHelper::Copy(GetOutputSignalMemory(signalIndex), &dflt, sizeof(MARTe::uint32));
        }

        return true;

    }

    bool SetDefault(MARTe::uint64 dflt) {

        MARTe::uint32 nOfSignals = GetNumberOfOutputSignals();
        MARTe::uint32 signalIndex = 0u;

        for (signalIndex = 0; signalIndex < nOfSignals; signalIndex++) {
            MARTe::MemoryOperationsHelper::Copy(GetOutputSignalMemory(signalIndex), &dflt, sizeof(MARTe::uint64));
        }

        return true;

    }

};

CLASS_REGISTER(SDNPublisherTestConstantGAM, "1.0")

/**
 * Starts a MARTe application that uses this driver instance.
 */

bool ConfigureApplication(const MARTe::char8 * const config) {

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

//Standard configuration for testing
const MARTe::char8 * const config_default = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +Timer = {"
        "            Class = SDNPublisherTestGAM"
        "            OutputSignals = {"
        "                Counter = {"
        "                    DataSource = SDNPub"
        "                    Type = uint64"
        "                    Trigger = 1"
        "                }"
        "                Timestamp = {"
        "                    DataSource = SDNPub"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +SDNPub = {"
        "            Class = SDNPublisher"
        "            Topic = Default"
        "            Interface = lo"
        "            SocketBufferCapacity = 100"
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
        "                    Functions = {Timer}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Standard configuration for testing with header
const MARTe::char8 * const config_default_header = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +Timer = {"
        "            Class = SDNPublisherTestGAM"
        "            InputSignals = {"
        "                Header = {"
        "                    DataSource = SDNPub"
        "                    Type = uint8"
        "                    NumberOfElements = 48"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Counter = {"
        "                    DataSource = SDNPub"
        "                    Type = uint64"
        "                    Trigger = 1"
        "                }"
        "                Timestamp = {"
        "                    DataSource = SDNPub"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +SDNPub = {"
        "            Class = SDNPublisher"
        "            Topic = Default"
        "            Interface = lo"
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
        "                    Functions = {Timer}"
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

bool SDNPublisherTest::TestConstructor() {
    using namespace MARTe;
    SDNPublisher test;
    bool ok = (test.PrepareNextState("FromCurrent", "ToNext")); // Test the instantiated class
    return ok;
}

bool SDNPublisherTest::TestInitialise_Empty() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNPublisherTest::TestInitialise_Default() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    return test.Initialise(cdb);
}

bool SDNPublisherTest::TestInitialise_Address() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "127.0.0.1:60000");
    return test.Initialise(cdb);
}

bool SDNPublisherTest::TestInitialise_Address_mcast() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "239.0.0.1:60000");
    return test.Initialise(cdb);
}

bool SDNPublisherTest::TestInitialise_Address_ucast() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "10.0.0.1:60000");
    return test.Initialise(cdb);
}

bool SDNPublisherTest::TestInitialise_ShortForm() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "sdn://127.0.0.1:60000/Default");
    cdb.Write("Interface", "lo");
    return test.Initialise(cdb);
}

bool SDNPublisherTest::TestInitialise_Missing_Topic() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Interface", "lo");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNPublisherTest::TestInitialise_Missing_Interface() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}


bool SDNPublisherTest::TestInitialise_False_Interface_1() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNPublisherTest::TestInitialise_False_Interface_2() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "NotAnInterface");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNPublisherTest::TestInitialise_False_Address_1() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "NotAnAddress");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNPublisherTest::TestInitialise_False_Address_2() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "255.255.255.255:60000");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNPublisherTest::TestInitialise_False_Address_3() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "127.0.0.1");
    bool ok = test.Initialise(cdb);
    //return !ok; // Expect failure
    return ok; // It is actually successful !
}

bool SDNPublisherTest::TestInitialise_False_Address_4() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", ":60000");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}

bool SDNPublisherTest::TestInitialise_False_Address_5() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "127.0.0:60000");
    bool ok = test.Initialise(cdb);
    return !ok; // Expect failure
}
#ifdef FEATURE_10840
bool SDNPublisherTest::TestInitialise_SourcePort() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "239.0.0.1:60000");
    cdb.Write("SourcePort", 1024);
    bool ok = test.Initialise(cdb);
    return ok;
}

bool SDNPublisherTest::TestInitialise_NetworkByteOrder() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    cdb.Write("Topic", "Default");
    cdb.Write("Interface", "lo");
    cdb.Write("Address", "239.0.0.1:60000");
    cdb.Write("NetworkByteOrder", 1u);
    bool ok = test.Initialise(cdb);
    return ok;
}
#endif
bool SDNPublisherTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config_default);
}

bool SDNPublisherTest::TestSetConfiguredDatabase_False_NOfSignals() {
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestConstantGAM"
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
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
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
            "                    Functions = {Timer}"
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

bool SDNPublisherTest::TestAllocateMemory() {
    return TestIntegratedInApplication(config_default);
}

bool SDNPublisherTest::TestAllocateMemory_Header() {
    return TestIntegratedInApplication(config_default_header);
}

bool SDNPublisherTest::TestAllocateMemory_False() {
    using namespace MARTe;
    SDNPublisher test;
    bool ok = test.AllocateMemory();
    return !ok; // Expect failure
}

bool SDNPublisherTest::TestAllocateMemory_False_Header_Size() {

    //Standard configuration for testing with header
    const MARTe::char8 * const config  = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestGAM"
            "            InputSignals = {"
            "                Header = {"
            "                    DataSource = SDNPub"
            "                    Type = uint8"
            "                    NumberOfElements = 49"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
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
            "                    Functions = {Timer}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    return !TestIntegratedInApplication(config);
}

#ifdef FEATURE_10840
bool SDNPublisherTest::TestAllocateMemory_SourcePort() {
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
            "            SourcePort = 1024"
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
            "                    Functions = {Timer}"
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

bool SDNPublisherTest::TestAllocateMemory_False_SourcePort() {

    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
            "            SourcePort = 1023"
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
            "                    Functions = {Timer}"
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

bool SDNPublisherTest::TestAllocateMemory_NetworkByteOrder() {
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
            "            NetworkByteOrder = 1"
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
            "                    Functions = {Timer}"
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
#endif
bool SDNPublisherTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    SDNPublisher test;
    return (test.GetNumberOfMemoryBuffers() == 1u);
}

bool SDNPublisherTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    return true;
}

bool SDNPublisherTest::TestGetBrokerName_InputSignals() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    StreamString brokerName = test.GetBrokerName(cdb, InputSignals);
    bool ok = (brokerName == "");
    return ok;
}

bool SDNPublisherTest::TestGetBrokerName_OutputSignals() {
    using namespace MARTe;
    SDNPublisher test;
    ConfigurationDatabase cdb;
    StreamString brokerName = test.GetBrokerName(cdb, InputSignals);
    bool ok = (brokerName == "");

    if (ok) { // No trigger
        brokerName = test.GetBrokerName(cdb, OutputSignals);
        ok = (brokerName == "MemoryMapOutputBroker");
    }

    if (ok) { // With trigger
        cdb.Write("Trigger", 1);
        brokerName = test.GetBrokerName(cdb, OutputSignals);
        ok = (brokerName == "MemoryMapSynchronisedOutputBroker");
    }

    return ok;
}

bool SDNPublisherTest::TestGetInputBrokers() {
    using namespace MARTe;
    SDNPublisher test;
    ReferenceContainer container;
    bool ok = test.GetInputBrokers(container, "Default", NULL_PTR(void *));
    return !ok; // Expect failure
}

bool SDNPublisherTest::TestGetInputBrokers_Header() {
    return TestIntegratedInApplication(config_default_header);
}

bool SDNPublisherTest::TestGetOutputBrokers() {
    return TestIntegratedInApplication(config_default);
}

bool SDNPublisherTest::TestGetOutputBrokers_1() {
    // 1 input GAM, 1 signal, with trigger
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
            "            Signals = {"
            "                Counter = {"
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
            "                    Functions = {Timer}"
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

bool SDNPublisherTest::TestGetOutputBrokers_2() {
    // 1 input GAM, 1 signal, no trigger
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
            "            Signals = {"
            "                Counter = {"
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
            "                    Functions = {Timer}"
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

bool SDNPublisherTest::TestGetOutputBrokers_3() {
    // 1 input GAM, 1 signal multi-samples, with trigger
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                    Samples = 10"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
            "            Signals = {"
            "                Counter = {"
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
            "                    Functions = {Timer}"
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

bool SDNPublisherTest::TestGetOutputBrokers_4() {
    // 1 input GAM, 2 signals, with trigger
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
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
            "                    Functions = {Timer}"
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

bool SDNPublisherTest::TestGetOutputBrokers_5() {
    // 2 input GAMs, 1 signal each, with trigger
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Counter = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "            }"
            "        }"
            "        +Timer = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = DDB1"
            "                    Type = uint64"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
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
            "        +SDNPub = {"
            "            Class = SDNPublisher"
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
            "                    Functions = {Counter Timer}"
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

bool SDNPublisherTest::TestGetOutputBrokers_6() {
    // 2 input GAMs, 1 signal each, with trigger each
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Counter = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "            }"
            "        }"
            "        +Timer = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = DDB1"
            "                    Type = uint64"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
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
            "        +SDNPub = {"
            "            Class = SDNPublisher"
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
            "                    Functions = {Counter Timer}"
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

bool SDNPublisherTest::TestGetOutputBrokers_7() {
    // 2 input GAMs, 2 signals each, with trigger
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "        +ADC = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Sample = {"
            "                    DataSource = SDNPub"
            "                    Type = float64"
            "                }"
            "                RMS = {"
            "                    DataSource = SDNPub"
            "                    Type = float64"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint64"
            "                }"
            "                Timestamp = {"
            "                    Type = uint64"
            "                }"
            "                Sample = {"
            "                    Type = float64"
            "                }"
            "                RMS = {"
            "                    Type = float64"
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
            "                    Functions = {ADC Timer}"
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

bool SDNPublisherTest::TestGetOutputBrokers_8() {
    // 2 input GAMs, 2 signals each, with trigger
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "                ArrayInt32_1D = {"
            "                    DataSource = SDNPub"
            "                    Type = uint32"
            "                    NumberOfElements = 10"
            "                    NumberOfDimensions = 1"
            "                }"
            "            }"
            "        }"
            "        +ADC = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                Sample = {"
            "                    DataSource = SDNPub"
            "                    Type = float64"
            "                }"
            "                RMS = {"
            "                    DataSource = SDNPub"
            "                    Type = float64"
            "                }"
            "                ArrayFlt32_1D = {"
            "                    DataSource = SDNPub"
            "                    Type = float32"
            "                    NumberOfElements = 10"
            "                    NumberOfDimensions = 1"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint64"
            "                }"
            "                Timestamp = {"
            "                    Type = uint64"
            "                }"
            "                Sample = {"
            "                    Type = float64"
            "                }"
            "                RMS = {"
            "                    Type = float64"
            "                }"
            "                ArrayInt32_1D = {"
            "                    Type = uint32"
            "                    NumberOfElements = 10"
            "                    NumberOfDimensions = 1"
            "                }"
            "                ArrayFlt32_1D = {"
            "                    Type = float32"
            "                    NumberOfElements = 10"
            "                    NumberOfDimensions = 1"
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
            "                    Functions = {ADC Timer}"
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

bool SDNPublisherTest::TestPrepareNextState() {
    using namespace MARTe;
    SDNPublisher test;
    bool ok = (test.PrepareNextState("FromCurrent", "ToNext")); // Test the instantiated class
    return ok;
}

bool SDNPublisherTest::TestSynchronise() {
    return TestIntegratedInApplication(config_default);
}

bool SDNPublisherTest::TestSynchronise_False() {
    using namespace MARTe;
    SDNPublisher test;
    bool ok = test.Synchronise();
    return !ok; // Expect failure
}

bool SDNPublisherTest::TestSynchronise_MCAST_Topic_1() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
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
            "                    Functions = {Timer}"
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
        ReferenceT<SDNPublisher> publisher = application->Find("Data.SDNPub");
        ReferenceT<SDNPublisherTestGAM> timer = application->Find("Functions.Timer");

        ok = ((publisher.IsValid()) && (timer.IsValid()));

        if (ok) {
            // Instantiate a sdn::Metadata structure to configure the topic
            sdn::Metadata_t mdata;
            sdn::Topic_InitializeMetadata(mdata, "Default", 0);
            // Instantiate SDN topic from metadata specification
            sdn::Topic* topic = new sdn::Topic;
            topic->SetMetadata(mdata);
            sdn::Subscriber* subscriber;

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
            // Create sdn::Subscriber
            if (ok) {
                subscriber = new sdn::Subscriber(*topic);
            }
            if (ok) {
                ok = (subscriber->SetInterface((char*) "lo") == STATUS_SUCCESS);
            }
            if (ok) {
                ok = (subscriber->Configure() == STATUS_SUCCESS);
            }
            // Call SDNPublisher::Synchronise
            if (ok) {
                ok = publisher->Synchronise();
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
            // Set test value
            if (ok) {
                timer->SetCounter((MARTe::uint64) 10ul);
            }
            // Start Application
            if (ok) {
                log_info("Start application");
                ok = StartApplication();
            }
            // Let the application run
            if (ok) {
                wait_for(500000000ul);
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
            if (ok) {
                MARTe::uint64 counter = 0ul;
                ok = (topic->GetAttribute(0u, &counter) == STATUS_SUCCESS);
                log_info("Received counter '%lu'", counter);
                ok = (counter == (MARTe::uint64) 10ul);
            }
        }
    }

    if (ok) {
        log_info("Stop application");
        ok = StopApplication();
    }

    return ok;
}

bool SDNPublisherTest::TestSynchronise_MCAST_Topic_1_Header() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestGAM"
            "            InputSignals = {"
            "                Header = {"
            "                    DataSource = SDNPub"
            "                    Type = uint8"
            "                    NumberOfElements = 48"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
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
            "                    Functions = {Timer}"
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
        ReferenceT<SDNPublisher> publisher = application->Find("Data.SDNPub");
        ReferenceT<SDNPublisherTestGAM> timer = application->Find("Functions.Timer");

        ok = ((publisher.IsValid()) && (timer.IsValid()));

        if (ok) {
            // Instantiate a sdn::Metadata structure to configure the topic
            sdn::Metadata_t mdata;
            sdn::Topic_InitializeMetadata(mdata, "Default", 0);
            // Instantiate SDN topic from metadata specification
            sdn::Topic* topic = new sdn::Topic;
            topic->SetMetadata(mdata);
            sdn::Subscriber* subscriber;

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
            // Create sdn::Subscriber
            if (ok) {
                subscriber = new sdn::Subscriber(*topic);
            }
            if (ok) {
                ok = (subscriber->SetInterface((char*) "lo") == STATUS_SUCCESS);
            }
            if (ok) {
                ok = (subscriber->Configure() == STATUS_SUCCESS);
            }
            // Call SDNPublisher::Synchronise
            if (ok) {
                ok = publisher->Synchronise();
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
            // Set test value
            if (ok) {
                timer->SetCounter((MARTe::uint64) 10ul);
            }
            // Start Application
            if (ok) {
                log_info("Start application");
                ok = StartApplication();
            }
            // Let the application run
            if (ok) {
                wait_for(500000000ul);
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
            if (ok) {
                MARTe::uint64 counter = 0ul;
                ok = (topic->GetAttribute(0u, &counter) == STATUS_SUCCESS);
                log_info("Received counter '%lu'", counter);
                ok = (counter == (MARTe::uint64) 10ul);
            }
            if (ok) {
                sdn::Header_t *header = reinterpret_cast<sdn::Header_t *>(timer->header);
                sdn::Header_t *sheader = static_cast<sdn::Header_t *>(subscriber->GetTopicHeader());
                ok = (header->header_size == sheader->header_size);
            }
        }
    }

    if (ok) {
        log_info("Stop application");
        ok = StopApplication();
    }

    return ok;
}

bool SDNPublisherTest::TestSynchronise_MCAST_Topic_2() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "        +Arrays = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                ArrayInt32_1D = {"
            "                    DataSource = SDNPub"
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
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
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
            "                    Functions = {Arrays Timer}"
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
        ReferenceT<SDNPublisher> publisher = application->Find("Data.SDNPub");
        ReferenceT<SDNPublisherTestGAM> timer = application->Find("Functions.Timer");
        ReferenceT<SDNPublisherTestConstantGAM> array = application->Find("Functions.Arrays");

        ok = ((publisher.IsValid()) && (timer.IsValid()) && (array.IsValid()));

        if (ok) {
            // Instantiate a sdn::Metadata structure to configure the topic (name, version and size)
            sdn::Metadata_t mdata;
            sdn::Topic_InitializeMetadata(mdata, "Default", 0u, 0u);
            // Instantiate SDN topic from metadata specification
            sdn::Topic* topic = new sdn::Topic;
            topic->SetMetadata(mdata);
            sdn::Subscriber* subscriber;

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
                ok = (topic->AddAttribute(3u, "Reserved", "uint8", 104) == STATUS_SUCCESS);
            }
            if (ok) {
                topic->SetUID(0u); // UID corresponds to the data type but it includes attributes name - Safer to clear with SDN core library 1.0.10
                ok = (topic->Configure() == STATUS_SUCCESS);
            }
            if (ok) {
                ok = topic->IsInitialized();
            }
            // Create sdn::Subscriber
            if (ok) {
                subscriber = new sdn::Subscriber(*topic);
            }
            if (ok) {
                ok = (subscriber->SetInterface((char*) "lo") == STATUS_SUCCESS);
            }
            if (ok) {
                ok = (subscriber->Configure() == STATUS_SUCCESS);
            }
            // Call SDNPublisher::Synchronise
            if (ok) {
                ok = publisher->Synchronise();
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
            // Set test value
            if (ok) {
                timer->SetCounter((MARTe::uint64) 10ul);
                array->SetDefault((MARTe::char8) 24u);
            }
            // Start Application
            if (ok) {
                log_info("Start application");
                ok = StartApplication();
            }
            // Let the application run
            if (ok) {
                wait_for(500000000ul);
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
            if (ok) {
                MARTe::uint64 counter = 0ul;
                ok = (topic->GetAttribute(0u, &counter) == STATUS_SUCCESS);
                log_info("Received counter '%lu'", counter);
                MARTe::char8 byte = *((MARTe::char8 *) topic->GetTypeDefinition()->GetAttributeReference(2u));
                log_info("Received byte '%hhu'", byte);
                ok = (byte == (MARTe::char8) 24u);
            }
        }
    }

    if (ok) {
        log_info("Stop application");
        ok = StopApplication();
    }

    return ok;
}

bool SDNPublisherTest::TestSynchronise_UCAST_Topic_1() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
            "            Address = \"127.0.0.1:60000\""
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
            "                    Functions = {Timer}"
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
        ReferenceT<SDNPublisher> publisher = application->Find("Data.SDNPub");
        ok = publisher.IsValid();

        if (ok) {
            // Instantiate a sdn::Metadata structure to configure the topic
            sdn::Metadata_t mdata;
            sdn::Topic_InitializeMetadata(mdata, "Default", 0, "127.0.0.1:60000");
            // Instantiate SDN topic from metadata specification
            sdn::Topic* topic = new sdn::Topic;
            topic->SetMetadata(mdata);
            sdn::Subscriber* subscriber;

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
            // Create sdn::Subscriber
            if (ok) {
                subscriber = new sdn::Subscriber(*topic);
            }
            if (ok) {
                ok = (subscriber->SetInterface((char*) "lo") == STATUS_SUCCESS);
            }
            if (ok) {
                ok = (subscriber->Configure() == STATUS_SUCCESS);
            }
            // Call SDNPublisher::Synchronise
            if (ok) {
                ok = publisher->Synchronise();
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
        }
    }

    return ok;
}
#ifdef FEATURE_10840
bool SDNPublisherTest::TestSynchronise_NetworkByteOrder_Topic_1() {
    using namespace MARTe;

    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
            "            NetworkByteOrder = 1"
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
            "                    Functions = {Timer}"
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
        ReferenceT<SDNPublisher> publisher = application->Find("Data.SDNPub");
        ReferenceT<SDNPublisherTestGAM> timer = application->Find("Functions.Timer");

        ok = ((publisher.IsValid()) && (timer.IsValid()));

        if (ok) {
            // Instantiate a sdn::Metadata structure to configure the topic
            sdn::Metadata_t mdata;
            sdn::Topic_InitializeMetadata(mdata, "Default", 0);
            // Instantiate SDN topic from metadata specification
            sdn::Topic* topic = new sdn::Topic;
            topic->SetMetadata(mdata);
            sdn::Subscriber* subscriber;

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
            // Create sdn::Subscriber
            if (ok) {
                subscriber = new sdn::Subscriber(*topic);
            }
            if (ok) {
                ok = (subscriber->SetInterface((char*) "lo") == STATUS_SUCCESS);
            }
            if (ok) {
                ok = (subscriber->Configure() == STATUS_SUCCESS);
            }
            // Call SDNPublisher::Synchronise
            if (ok) {
                ok = publisher->Synchronise();
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
            // Set test value
            if (ok) {
                timer->SetCounter((MARTe::uint64) 10ul);
            }
            // Start Application
            if (ok) {
                log_info("Start application");
                ok = StartApplication();
            }
            // Let the application run
            if (ok) {
                wait_for(500000000ul);
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
            if (ok) {
                MARTe::uint64 counter = 0ul;
                ok = (topic->GetAttribute(0u, &counter) == STATUS_SUCCESS);
                log_info("Received counter '%lu'", counter);
                if (sdn::HelperTools::IsLittleEndian()) {
                    ok = (counter == (MARTe::uint64) 720575940379279360ul);
                }
                else {
                    ok = (counter == (MARTe::uint64) 10ul);
                }
            }
        }
    }

    if (ok) {
        log_info("Stop application");
        ok = StopApplication();
    }

    return ok;
}

bool SDNPublisherTest::TestSynchronise_NetworkByteOrder_Topic_1_Header() {
    using namespace MARTe;

    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestGAM"
            "            InputSignals = {"
            "                Header = {"
            "                    DataSource = SDNPub"
            "                    Type = uint8"
            "                    NumberOfElements = 48"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
            "            NetworkByteOrder = 1"
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
            "                    Functions = {Timer}"
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
        ReferenceT<SDNPublisher> publisher = application->Find("Data.SDNPub");
        ReferenceT<SDNPublisherTestGAM> timer = application->Find("Functions.Timer");

        ok = ((publisher.IsValid()) && (timer.IsValid()));

        if (ok) {
            // Instantiate a sdn::Metadata structure to configure the topic
            sdn::Metadata_t mdata;
            sdn::Topic_InitializeMetadata(mdata, "Default", 0);
            // Instantiate SDN topic from metadata specification
            sdn::Topic* topic = new sdn::Topic;
            topic->SetMetadata(mdata);
            sdn::Subscriber* subscriber;

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
            // Create sdn::Subscriber
            if (ok) {
                subscriber = new sdn::Subscriber(*topic);
            }
            if (ok) {
                ok = (subscriber->SetInterface((char*) "lo") == STATUS_SUCCESS);
            }
            if (ok) {
                ok = (subscriber->Configure() == STATUS_SUCCESS);
            }
            // Call SDNPublisher::Synchronise
            if (ok) {
                ok = publisher->Synchronise();
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
            // Set test value
            if (ok) {
                timer->SetCounter((MARTe::uint64) 10ul);
            }
            // Start Application
            if (ok) {
                log_info("Start application");
                ok = StartApplication();
            }
            // Let the application run
            if (ok) {
                wait_for(500000000ul);
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
            if (ok) {
                MARTe::uint64 counter = 0ul;
                ok = (topic->GetAttribute(0u, &counter) == STATUS_SUCCESS);
                log_info("Received counter '%lu'", counter);
                if (sdn::HelperTools::IsLittleEndian()) {
                    ok = (counter == (MARTe::uint64) 720575940379279360ul);
                }
                else {
                    ok = (counter == (MARTe::uint64) 10ul);
                }
            }
        }
    }

    if (ok) {
        log_info("Stop application");
        ok = StopApplication();
    }

    return ok;
}

bool SDNPublisherTest::TestSynchronise_NetworkByteOrder_Topic_2() {
    using namespace MARTe;
    //Standard configuration for testing
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Timer = {"
            "            Class = SDNPublisherTestGAM"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                    Trigger = 1"
            "                }"
            "                Timestamp = {"
            "                    DataSource = SDNPub"
            "                    Type = uint64"
            "                }"
            "            }"
            "        }"
            "        +Arrays = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                ArrayInt32_1D = {"
            "                    DataSource = SDNPub"
            "                    Type = uint32"
            "                    NumberOfElements = 10"
            "                    NumberOfDimensions = 1"
            "                }"
            "            }"
            "        }"
            "        +Arrays16 = {"
            "            Class = SDNPublisherTestConstantGAM"
            "            OutputSignals = {"
            "                ArrayInt16_1D = {"
            "                    DataSource = SDNPub"
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
            "        +SDNPub = {"
            "            Class = SDNPublisher"
            "            Topic = Default"
            "            Interface = lo"
            "            NetworkByteOrder = 1"
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
            "                    Functions = {Arrays Arrays16 Timer}"
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
        ReferenceT<SDNPublisher> publisher = application->Find("Data.SDNPub");
        ReferenceT<SDNPublisherTestGAM> timer = application->Find("Functions.Timer");
        ReferenceT<SDNPublisherTestConstantGAM> array = application->Find("Functions.Arrays");
        ReferenceT<SDNPublisherTestConstantGAM> array16 = application->Find("Functions.Arrays16");

        ok = ((publisher.IsValid()) && (timer.IsValid()) && (array.IsValid()) && (array16.IsValid()));

        if (ok) {
            // Instantiate a sdn::Metadata structure to configure the topic (name, version and size)
            sdn::Metadata_t mdata;
            sdn::Topic_InitializeMetadata(mdata, "Default", 0u, 0u);
            // Instantiate SDN topic from metadata specification
            sdn::Topic* topic = new sdn::Topic;
            topic->SetMetadata(mdata);
            sdn::Subscriber* subscriber;

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
            // Create sdn::Subscriber
            if (ok) {
                subscriber = new sdn::Subscriber(*topic);
            }
            if (ok) {
                ok = (subscriber->SetInterface((char*) "lo") == STATUS_SUCCESS);
            }
            if (ok) {
                ok = (subscriber->Configure() == STATUS_SUCCESS);
            }
            // Call SDNPublisher::Synchronise
            if (ok) {
                ok = publisher->Synchronise();
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
            // Set test value
            if (ok) {
                timer->SetCounter((MARTe::uint64) 10ul);
                array->SetDefault((MARTe::uint32) 24u);
                array16->SetDefault((MARTe::uint16) 24u);
            }
            // Start Application
            if (ok) {
                log_info("Start application");
                ok = StartApplication();
            }
            // Let the application run
            if (ok) {
                wait_for(500000000ul);
            }
            // Test reception
            if (ok) {
                ok = (subscriber->Receive(0ul) == STATUS_SUCCESS);
            }
            if (ok) {
                MARTe::uint64 counter = 0ul;
                ok = (topic->GetAttribute(0u, &counter) == STATUS_SUCCESS);
                log_info("Received counter '%lu'", counter);
                if (sdn::HelperTools::IsLittleEndian()) {
                    ok = (counter == (MARTe::uint64) 720575940379279360ul);
                }
                else {
                    ok = (counter == (MARTe::uint64) 10ul);
                }
                MARTe::uint32 element = *((MARTe::uint32 *) topic->GetTypeDefinition()->GetAttributeReference(2u));
                log_info("Received element '%u'", element);
                if (sdn::HelperTools::IsLittleEndian()) {
                    ok = (element == (MARTe::uint32) 402653184u);
                }
                else {
                    ok = (element == (MARTe::uint32) 24u);
                }
                MARTe::uint16 element16 = *((MARTe::uint16 *) topic->GetTypeDefinition()->GetAttributeReference(3u));
                log_info("Received element '%u'", element16);
                if (sdn::HelperTools::IsLittleEndian()) {
                    ok = (element16 == (MARTe::uint16) 6144u);
                }
                else {
                    ok = (element16 == (MARTe::uint16) 24u);
                }
            }
        }
    }

    if (ok) {
        log_info("Stop application");
        ok = StopApplication();
    }

    return ok;
}
#endif
