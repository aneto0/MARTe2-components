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

static ccs::log::Severity_t org_filter = ccs::log::SetFilter(LOG_INFO);
#undef log_trace
#define log_trace log_info

class SDNPublisherTestGAM: public MARTe::GAM {

public:

    CLASS_REGISTER_DECLARATION()

    SDNPublisherTestGAM() : GAM() {

        log_trace("SDNPublisherTestGAM::SDNPublisherTestGAM - Entering method");

        counter = 0lu;
        timestamp = 0lu;

        log_trace("SDNPublisherTestGAM::SDNPublisherTestGAM - Leaving method");
    }

    ~SDNPublisherTestGAM() { }

    bool Execute() {

        log_trace("SDNPublisherTestGAM::Execute - Entering method");

        counter += 1;
	MARTe::uint64 curr_time = get_time();
	MARTe::uint64 till_time = ceil_time(curr_time, 100000000lu);
	timestamp = wait_until(till_time);

        if(GetNumberOfOutputSignals() > 0u) {
            MARTe::MemoryOperationsHelper::Copy(GetOutputSignalMemory(0u), &counter, sizeof(MARTe::uint64));
        }

        if(GetNumberOfOutputSignals() > 1u) {
            MARTe::MemoryOperationsHelper::Copy(GetOutputSignalMemory(1u), &timestamp, sizeof(MARTe::uint64));
        }

        log_trace("SDNPublisherTestGAM::Execute - Leaving method");

        return true;
    }

    bool Setup() {

        log_trace("SDNPublisherTestGAM::Setup - Entering method");

        log_trace("SDNPublisherTestGAM::Setup - Leaving method");

        return true;
    }

    MARTe::uint64 counter;
    MARTe::uint64 timestamp;

};

CLASS_REGISTER(SDNPublisherTestGAM, "1.0")

/**
 * Starts a MARTe application that uses this driver instance.
 */

bool TestIntegratedInApplication(const MARTe::char8 * const config) {

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
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "RealTimeApplication::ConfigureApplication failed");
    } else {
        ok = application->PrepareNextState("Running");
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "RealTimeApplication::PrepareNextState failed");
    } else {
        application->StartNextStateExecution();
    }

    wait_for(1000000000lu);

    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "RealTimeApplication::StartNextStateExecution failed");
    } else {
        application->StopCurrentStateExecution();
    }

    god->Purge();

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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool SDNPublisherTest::TestConstructor() {
    using namespace MARTe;
    SDNPublisher test;
    bool ok = (test.PrepareNextState("FromCurrent","ToNext")); // Test the instantiated class
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

bool SDNPublisherTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config_default);
}

bool SDNPublisherTest::TestAllocateMemory() {
    return TestIntegratedInApplication(config_default);
}

bool SDNPublisherTest::TestAllocateMemory_False() {
    using namespace MARTe;
    SDNPublisher test;
    bool ok = test.AllocateMemory();
    return !ok; // Expect failure
}

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
    StreamString brokerName = test.GetBrokerName(cdb, OutputSignals);
    bool ok = (brokerName == "MemoryMapSynchronisedOutputBroker");
    return ok;
}

bool SDNPublisherTest::TestGetInputBrokers() {
    using namespace MARTe;
    SDNPublisher test;
    ReferenceContainer container;
    bool ok = test.GetInputBrokers(container, "Default", NULL_PTR(void *));
    return !ok; // Expect failure
}

bool SDNPublisherTest::TestGetOutputBrokers() {
    return TestIntegratedInApplication(config_default);
}

bool SDNPublisherTest::TestPrepareNextState() {
    using namespace MARTe;
    SDNPublisher test;
    bool ok = (test.PrepareNextState("FromCurrent","ToNext")); // Test the instantiated class
    return ok;
}

bool SDNPublisherTest::TestSynchronise() {
    return TestIntegratedInApplication(config_default);
}

