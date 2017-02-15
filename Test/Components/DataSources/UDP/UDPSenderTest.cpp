/**
 * @file UDPSenderTest.cpp
 * @brief Source file for class UDPSenderTest
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
#include "GAM.h"
#include "GAMScheduler.h"
#include "UDPSender.h"
#include "UDPSenderTest.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class UDPSenderTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

UDPSenderTestGAM    () : GAM() {
        counter = 0ul;
        timer = 0ul;
    }

    ~UDPSenderTestGAM() {

    }

    bool Execute() {
        if(GetNumberOfOutputSignals() > 0u) {
            MARTe::MemoryOperationsHelper::Copy(GetOutputSignalMemory(0u), &counter, sizeof(MARTe::uint64));
        }
        if(GetNumberOfOutputSignals() > 1u) {
            MARTe::MemoryOperationsHelper::Copy(GetOutputSignalMemory(1u), &timer, sizeof(MARTe::uint64));
        }

        return true;
    }

    bool Setup() {
        return true;
    }

    bool TestCounter(MARTe::uint64 value = 0ul){
        bool ok = (counter == value);
        //log_info("UDPSenderTest::TestCounter - '%lu %lu'", counter, value);
        return ok;
    }

    bool TestTimer(MARTe::uint64 value = 0ul){
        bool ok = (timer == value);
        //log_info("UDPSenderTest::TestTimer - '%lu %lu'", timer, value);
        return ok;
    }

    MARTe::uint64 counter;

    MARTe::uint64 timer;
};
CLASS_REGISTER(UDPSenderTestGAM, "1.0")

/**
 * Starts a MARTe application that uses this driver instance.
 */

static inline bool ConfigureApplication(const MARTe::char8 * const config){
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    StreamString err;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb, &err);

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    bool ok = parser.Parse();

    if (!ok){
        REPORT_ERROR(ErrorManagement::InternalSetupError, "StandardParser::Parse failed");
        //log_error("StandardParser::Parse failed with '%s'", err.Buffer());
    }else{
        god->Purge();
        ok = god->Initialise(cdb);
    }

    ReferenceT<RealTimeApplication> application;

    if (ok){
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (!ok){
        REPORT_ERROR(ErrorManagement::InternalSetupError, "RealTimeApplication::IsValid failed");
    }else{
        ok = application->ConfigureApplication();
    }
    return ok;
}


static inline bool TestIntegratedInApplication(const MARTe::char8 * const config){

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
        //log_error("StandardParser::Parse failed with '%s'", err.Buffer());
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
        ok = application->PrepareNextState("State1");
        
    }

    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "RealTimeApplication::PrepareNextState failed");
    } else {
        application->StartNextStateExecution();
    }
    Sleep::Sec(10lu);


    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "RealTimeApplication::StartNextStateExecution failed");
    } else {
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
        "            Class = UDPSenderTestGAM"
        "            OutputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPSender = {"
        "            Class = UDPDrv::UDPSender"
        "            TargetAddress = \"127.0.0.1\""
        "            Port = \"44488\""
        "            Signals = {"
        "               Counter = {"
        "                   Type = uint64"
        "               }"
        "               SequenceNumber = {"
        "                   Type = uint64"
        "               }"
        "               Signal1 = {"
        "                   Type = uint64"
        "               }"
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
        "                +Thread = {"
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
bool UDPSenderTest::TestConstructor() {
    using namespace MARTe;
    bool ok =true;
    return ok;
}

bool UDPSenderTest::TestInitialise_Empty(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    bool ok = test.Initialise(cdb);
    return ok;
}
bool UDPSenderTest::TestInitialise_Port(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", "44488");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPSenderTest::TestInitialise_Restricted_Port(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", "666");
    bool ok = test.Initialise(cdb);
    return ok;
}


bool UDPSenderTest::TestInitialise_False_Port_1(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", "");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPSenderTest::TestInitialise_False_Port_2(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", "66666");
    bool ok = test.Initialise(cdb);
    /*uint64 *ActualPort;
    if (ok){
        cdb.Read("Port", ActualPort);
        ok = (*ActualPort == 44488);
        REPORT_ERROR_PARAMETERS(ErrorManagement::InternalSetupError, "actual port number value is %d", *ActualPort);
    }*/
    return ok;
}

bool UDPSenderTest::TestInitialise_False_Port_3(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", "-43565");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPSenderTest::TestInitialise_False_Port_4(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", "I am not a port");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPSenderTest::TestInitialise_TargetAddress(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("TargetAddress", "127.0.0.1");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPSenderTest::TestInitialise_False_TargetAddress_1(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("TargetAddress", "");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPSenderTest::TestInitialise_False_TargetAddress_2(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("TargetAddress", "266.266.266.266");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPSenderTest::TestInitialise_False_TargetAddress_3(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("TargetAddress", "0.0");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPSenderTest::TestInitialise_False_TargetAddress_4(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("TargetAddress", "-130.0.2.3");
    bool ok = test.Initialise(cdb);
    return ok;
} 

bool UDPSenderTest::TestInitialise_False_TargetAddress_5(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    cdb.Write("TargetAddress", "I am not an address");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPSenderTest::TestSetConfiguredDatabase(){
    return TestIntegratedInApplication(config1);
}

bool UDPSenderTest::TestSetConfiguredDatabase_More_Minimum_Signals(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPSenderTestGAM"
        "            OutputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "                Signal2 = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPSender = {"
        "            Class = UDPDrv::UDPSender"
        "            TargetAddress = \"127.0.0.1\""
        "            Port = \"44488\""
        "            Signals = {"
        "               Counter = {"
        "                   Type = uint64"
        "               }"
        "               SequenceNumber = {"
        "                   Type = uint64"
        "               }"
        "               Signal1 = {"
        "                   Type = uint64"
        "               }"
        "               Signal2 = {"
        "                   Type = uint64"
        "               }"
        "           }"
        "       }"
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
        return TestIntegratedInApplication(config2);
}

bool UDPSenderTest::TestSetConfiguredDatabase_False_NOfSignals(){
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPSenderTestGAM"
        "            OutputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPSender = {"
        "            Class = UDPDrv::UDPSender"
        "            TargetAddress = \"127.0.0.1\""
        "            Port = \"44488\""
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

        bool ok = ConfigureApplication(config2);
        return !ok;
}

bool UDPSenderTest::TestSetConfiguredDatabase_Incorrect_Signal_Size1(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPSenderTestGAM"
        "            OutputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPSender"
        "                    Type = uint16"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPSender = {"
        "            Class = UDPDrv::UDPSender"
        "            TargetAddress = \"127.0.0.1\""
        "            Port = \"44488\""
        "            Signals = {"
        "               Counter = {"
        "                   Type = uint16"
        "               }"
        "               SequenceNumber = {"
        "                   Type = uint64"
        "               }"
        "               Signal1 = {"
        "                   Type = uint64"
        "               }"
        "           }"
        "       }"
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
        return !ConfigureApplication(config2);
}

bool UDPSenderTest::TestSetConfiguredDatabase_Incorrect_Signal_Size2(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPSenderTestGAM"
        "            OutputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPSender"
        "                    Type = uint16"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPSender = {"
        "            Class = UDPDrv::UDPSender"
        "            TargetAddress = \"127.0.0.1\""
        "            Port = \"44488\""
        "            Signals = {"
        "               Counter = {"
        "                   Type = uint64"
        "               }"
        "               SequenceNumber = {"
        "                   Type = uint16"
        "               }"
        "               Signal1 = {"
        "                   Type = uint64"
        "               }"
        "           }"
        "       }"
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
        return !ConfigureApplication(config2);
}

bool UDPSenderTest::TestSetConfiguredDatabase_Incorrect_Signal_Type1(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPSenderTestGAM"
        "            OutputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPSender"
        "                    Type = float64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPSender = {"
        "            Class = UDPDrv::UDPSender"
        "            TargetAddress = \"127.0.0.1\""
        "            Port = \"44488\""
        "            Signals = {"
        "               Counter = {"
        "                   Type = float64"
        "               }"
        "               SequenceNumber = {"
        "                   Type = uint64"
        "               }"
        "               Signal1 = {"
        "                   Type = uint64"
        "               }"
        "           }"
        "       }"
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
        return !ConfigureApplication(config2);
}

bool UDPSenderTest::TestSetConfiguredDatabase_Incorrect_Signal_Type2(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPSenderTestGAM"
        "            OutputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPSender"
        "                    Type = float64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPSender"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPSender = {"
        "            Class = UDPDrv::UDPSender"
        "            TargetAddress = \"127.0.0.1\""
        "            Port = \"44488\""
        "            Signals = {"
        "               Counter = {"
        "                   Type = uint64"
        "               }"
        "               SequenceNumber = {"
        "                   Type = float64"
        "               }"
        "               Signal1 = {"
        "                   Type = uint64"
        "               }"
        "           }"
        "       }"
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
        return !ConfigureApplication(config2);
}

bool UDPSenderTest::TestAllocateMemory(){
    return TestIntegratedInApplication(config1);
}

bool UDPSenderTest::TestAllocateMemory_False(){
    using namespace MARTe;
    UDPSender test;
    bool ok = test.AllocateMemory();
    return !ok;
}

bool UDPSenderTest::TestGetNumberOfMemoryBuffers(){
    using namespace MARTe;
    UDPSender test;
    return (test.GetNumberOfMemoryBuffers() == 1u);
}

bool UDPSenderTest::TestGetSignalMemoryBuffer(){
    using namespace MARTe;
    return true; //Look into why this is not a proper test
}

bool UDPSenderTest::TestGetBrokerName_InputSignals(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    StreamString brokerName = test.GetBrokerName(cdb, InputSignals);
    bool ok = (brokerName == "");
    return ok;
}

bool UDPSenderTest::TestGetBrokerName_OutputSignals(){
    using namespace MARTe;
    UDPSender test;
    ConfigurationDatabase cdb;
    StreamString brokerName = test.GetBrokerName(cdb, OutputSignals);
    bool ok = (brokerName == "MemoryMapSynchronisedOutputBroker");
    return ok;
}

bool UDPSenderTest::TestGetOutputBrokers(){
    return TestIntegratedInApplication(config1);
}

bool UDPSenderTest::TestGetInputBrokers(){
    using namespace MARTe;
    UDPSender test;
    ReferenceContainer container;
    bool ok = test.GetInputBrokers(container, "Default", NULL_PTR(void *));
    return !ok;
}

bool UDPSenderTest::TestPrepareNextState(){
    using namespace MARTe;
    UDPSender test;
    bool ok = (test.PrepareNextState("FromCurrent", "ToNext"));
    return ok;
}

bool UDPSenderTest::TestSynchronise(){
    return TestIntegratedInApplication(config1);
}
