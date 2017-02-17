/**
 * @file UDPReceiverTest.cpp
 * @brief Source file for class UDPReceiverTest
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
#include "GAM.h"
#include "GAMScheduler.h"
#include "UDPReceiver.h"
#include "UDPReceiverTest.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class UDPReceiverTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

UDPReceiverTestGAM    () : GAM() {
        counter = 0ul;
        timer = 0ul;
    }

    ~UDPReceiverTestGAM() {

    }

    bool Execute() {
        if(GetNumberOfInputSignals() > 0u) {
            MARTe::MemoryOperationsHelper::Copy(&counter, GetInputSignalMemory(0u), sizeof(MARTe::uint64));
        }
        if(GetNumberOfInputSignals() > 1u) {
            MARTe::MemoryOperationsHelper::Copy(&timer, GetInputSignalMemory(1u), sizeof(MARTe::uint64));
        }

        return true;
    }

    bool Setup() {
        return true;
    }

    bool TestCounter(MARTe::uint64 value = 0ul){
        bool ok = (counter == value);
        //log_info("UDPReceiverTest::TestCounter - '%lu %lu'", counter, value);
        return ok;
    }

    bool TestTimer(MARTe::uint64 value = 0ul){
        bool ok = (timer == value);
        //log_info("UDPReceiverTest::TestTimer - '%lu %lu'", timer, value);
        return ok;
    }

    MARTe::uint64 counter;

    MARTe::uint64 timer;
};
CLASS_REGISTER(UDPReceiverTestGAM, "1.0")

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
    } else {
        god->Purge();
        ok = god->Initialise(cdb);
    }

    ReferenceT<RealTimeApplication> application;

    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    ReferenceT<GAMScheduler> scheduler;

    if (!ok) {
        REPORT_ERROR(ErrorManagement::InternalSetupError, "RealTimeApplication::IsValid failed");
    } else {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (!ok){
        REPORT_ERROR(ErrorManagement::InternalSetupError, "RealTimeScheduler::IsValid failed");
    }else{
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
    
    Sleep::Sec(5lu);

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
        "            Class = UDPReceiverTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPReceiver = {"
        "            Class = UDPDrv::UDPReceiver"
        "            Port = \"44488\""
        "            Timeout = \"3\""
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



/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool UDPReceiverTest::TestConstructor() {
    using namespace MARTe;
    bool ok =true;
    return ok;
}

bool UDPReceiverTest::TestInitialise_Empty(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_Port(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", "44488");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_Restricted_Port(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", "666");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_False_Port_1(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", "");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_False_Port_2(){
    using namespace MARTe;
    UDPReceiver test;
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

bool UDPReceiverTest::TestInitialise_False_Port_3(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", "-43565");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_False_Port_4(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Port", "I am not a port");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_Timeout(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Timeout", "5.3");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_Timeout_Infinite(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Timeout", "0");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_Timeout_NotDefined(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_False_Timeout_1(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Timeout", "-4");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_False_Timeout_2(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Timeout", "");
    bool ok = test.Initialise(cdb);
    return ok;
}

bool UDPReceiverTest::TestInitialise_False_Timeout_3(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    cdb.Write("Timeout", "I am not a timeout value");
    bool ok = test.Initialise(cdb);
    return ok;
}


bool UDPReceiverTest::TestSetConfiguredDatabase(){
    return TestIntegratedInApplication(config1);
}

bool UDPReceiverTest::TestSetConfiguredDatabase_More_Minimum_Signals(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPReceiverTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                Signal2 = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPReceiver = {"
        "            Class = UDPDrv::UDPReceiver"
        "            Port = \"44488\""
        "            Timeout = \"3\""
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

bool UDPReceiverTest::TestSetConfiguredDatabase_False_NOfSignals1(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPReceiverTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                Time = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPReceiver = {"
        "            Class = UDPDrv::UDPReceiver"
        "            Port = \"44488\""
        "            Timeout = \"3\""
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

        return !ConfigureApplication(config2);
}

bool UDPReceiverTest::TestSetConfiguredDatabase_False_NOfSignals2(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPReceiverTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
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
        "        +UDPReceiver = {"
        "            Class = UDPDrv::UDPReceiver"
        "            Port = \"44488\""
        "            Timeout = \"3\""
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
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMA GAMB}"
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

bool UDPReceiverTest::TestSetConfiguredDatabase_False_NOfSignals3(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPReceiverTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
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
        "        +UDPReceiver = {"
        "            Class = UDPDrv::UDPReceiver"
        "            Port = \"44488\""
        "            Timeout = \"3\""
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
        "           }"
        "       }"
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
        "                    Functions = {GAMA GAMB}"
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

bool UDPReceiverTest::TestSetConfiguredDatabase_Incorrect_Signal_Size1(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPReceiverTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint16"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPReceiver = {"
        "            Class = UDPDrv::UDPReceiver"
        "            Port = \"44488\""
        "            Timeout = \"3\""
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

bool UDPReceiverTest::TestSetConfiguredDatabase_Incorrect_Signal_Size2(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPReceiverTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint16"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPReceiver = {"
        "            Class = UDPDrv::UDPReceiver"
        "            Port = \"44488\""
        "            Timeout = \"3\""
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

bool UDPReceiverTest::TestSetConfiguredDatabase_Incorrect_Signal_Type1(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPReceiverTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPReceiver"
        "                    Type = float64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPReceiver = {"
        "            Class = UDPDrv::UDPReceiver"
        "            Port = \"44488\""
        "            Timeout = \"3\""
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

bool UDPReceiverTest::TestSetConfiguredDatabase_Incorrect_Signal_Type2(){
    using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPReceiverTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPReceiver"
        "                    Type = float64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPReceiver = {"
        "            Class = UDPDrv::UDPReceiver"
        "            Port = \"44488\""
        "            Timeout = \"3\""
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

bool UDPReceiverTest::TestAllocateMemory(){
    return TestIntegratedInApplication(config1);
}

bool UDPReceiverTest::TestAllocateMemory_False(){
    using namespace MARTe;
    UDPReceiver test;
    bool ok = test.AllocateMemory();
    return !ok;
}

bool UDPReceiverTest::TestGetNumberOfMemoryBuffers(){
    using namespace MARTe;
    UDPReceiver test;
    return (test.GetNumberOfMemoryBuffers() == 1u);
}

bool UDPReceiverTest::TestGetSignalMemoryBuffer(){
    using namespace MARTe;
    return true; //Look into why this is not a proper test
}

bool UDPReceiverTest::TestGetBrokerName_InputSignals(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    StreamString brokerName = test.GetBrokerName(cdb, InputSignals);
    bool ok = (brokerName == "MemoryMapInputBroker");
    return ok;
}

bool UDPReceiverTest::TestGetBrokerName_OutputSignals(){
    using namespace MARTe;
    UDPReceiver test;
    ConfigurationDatabase cdb;
    StreamString brokerName = test.GetBrokerName(cdb, OutputSignals);
    bool ok = (brokerName == "");
    return ok;
}

bool UDPReceiverTest::TestGetInputBrokers(){
    using namespace MARTe;
    return TestIntegratedInApplication(config1);
}

bool UDPReceiverTest::TestGetOutputBrokers(){
    using namespace MARTe;
    UDPReceiver test;
    ReferenceContainer container;
    bool ok = test.GetOutputBrokers(container, "Default", NULL_PTR(void *));
    return !ok;
}
bool UDPReceiverTest::TestPrepareNextState(){
    using namespace MARTe;
    UDPReceiver test;
    bool ok = (test.PrepareNextState("FromCurrent", "ToNext"));
    return !ok;
}

bool UDPReceiverTest::TestSynchronise(){
    return TestIntegratedInApplication(config1);
}

bool UDPReceiverTest::TestReceiving(){
        using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPReceiverTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
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
        "        +UDPReceiver = {"
        "            Class = UDPDrv::UDPReceiver"
        "            Port = \"44488\""
        "            Timeout = \"3\""
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
        "           }"
        "       }"
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
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMA GAMB}"
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

bool UDPReceiverTest::TestReceivingWithSynchronsing(){
        using namespace MARTe;
    const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = UDPReceiverTestGAM"
        "            InputSignals = {"
        "                Counter = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                    Frequency = 10"
        "                }"
        "                SequenceNumber = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "                Signal1 = {"
        "                    DataSource = UDPReceiver"
        "                    Type = uint64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +UDPReceiver = {"
        "            Class = UDPDrv::UDPReceiver"
        "            Port = \"44488\""
        "            Timeout = \"3\""
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