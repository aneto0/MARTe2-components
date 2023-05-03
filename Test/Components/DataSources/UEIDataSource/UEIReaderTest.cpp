/**
 * @file UEIReaderTest.cpp
 * @brief Source file for class UEIReaderTest
 * @date 2/05/2023
 * @author Xavier Ruche
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
 * the class UEIReaderTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UEIReaderTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//Helper Client to send test data

class UEIReaderHL : public UEIReader {
    public:
    CLASS_REGISTER_DECLARATION()
    UEIReaderHL();
    ~UEIReaderHL();
    StreamString GetDeviceNameHL ()             {return deviceName;}
    StreamString GetMapNameHL ()                {return mapName;}
    ReferenceT<UEIMasterObject> GetDeviceHL()   {return device;}
    ReferenceT<UEIMapContainer> GetMapHL()      {return map;}
    uint32 GetPollSleepPeriodHL ()              {return PollSleepPeriod;}
    uint32 GetNSamplesHL()                      {return nSamples;}
    bool GetFirstSyncHL()                       {return firstSync;}

};

UEIReaderHL::UEIReaderHL() : UEIReader(){

}
UEIReaderHL::~UEIReaderHL(){
    
}
CLASS_REGISTER(UEIReaderHL, "1.0")


const MARTe::char8 * const configUEIDS = ""
    "+UEIDAQ = {"
    "    Class    = UEIMasterObject"
    "    Ip       = {127, 0, 0, 1}"
    "    Port     =  6334"
    "    ConnectionTimeOut = 100"
    "    +Devices = {"
    "        Class=ReferenceContainer"
    "        +dev0={"
    "            Class               = UEIAI217_803"
    "            Devn                = 0"
    "            Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
    "            SamplingFrequency  = 1000.0"
    "        }"
    "        +dev1={"
    "            Class               = UEIAI217_803"
    "            Devn                = 1"
    "            Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
    "            SamplingFrequency  = 1000.0"
    "        }"
    "    }"
    "    +Maps = {"
    "        Class=ReferenceContainer"
    "        +Map1 = {"
    "            Class           = UEIRtDMap"
    "            ExecutionMode   = RealTimeThread"
    "            ScanRate        = 1.0"
    "            Inputs = {"
    "                dev0 = {"
    "                    Devn        = 0"
    "                    Channels    = {0, 1}"
    "                }"
    "            }"
    "        }"
    "        +Map2 = {"
    "            Class           = UEIRtDMap"
    "            ScanRate        = 1.0"
    "            ExecutionMode   = RealTimeThread"
    "            Inputs = {"
    "                dev1 = {"
    "                    Devn        = 1"
    "                    Channels    = {0, 1}"
    "                }"
    "            }"
    "        } "
    "    }"
    "}"
    "+Reader = {"
    "   Class = UEIReaderHL"
    "   Device = \"UEIDAQ\""
    "   Map = \"Map1\""
    "   PollSleepPeriod = 1"
    "   Signals = {"
    "       Status = {"
    "           Type = uint32"
    "       }"
    "       Timestamp = {"
    "           Type = uint64"
    "       }"
    "       Output0 = {"
    "           Type = float32"
    "       }"
    "       Output1 = {"
    "           Type = float32"
    "       }"
    "   }"
    "}";

const MARTe::char8 * const SampleApplication = ""
    "+UEIDevice = {"
    "    Class = UEIMasterObject"
    "    Ip = {127, 0, 0, 1}"
    "    Port = 6334"
    "    +Devices = {"
    "      Class=ReferenceContainer"
    "      +dev0={"
    "         Class               = UEIAI217_803"
    "         Devn                = 0"
    "         Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
    "         SamplingFrequency  = 10000"
    "      }"
    "      +dev1={"
    "         Class               = UEIAI217_803"
    "         Devn                = 1"
    "         Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
    "         SamplingFrequency  = 10000"
    "      }"
    "   }"
    "   +Maps = {"
    "      Class=ReferenceContainer"
    "      +Map1 = {"
    "       Class           = UEIRtDMap         "
    "       ScanRate 	    = 1.0"
    "	    NumberOfBuffers = 10"
    "       Samples         = 10"
    "       ExecutionMode   = RealTimeThread"
    "	    Inputs = {             "
    "            dev0 = {"
    "                Devn        = 0"
    "                Channels    = {0, 1}"
    "            }"
    "            dev1 = {"
    "                Devn        = 1"
    "                Channels    = {0, 1}"
    "            }"
    "          }"
    "       }"
    "    }"
    "}"
    "$Test = {"
    "    Class = RealTimeApplication"
    "    +Functions = {"
    "        Class = ReferenceContainer"
    "        +CopyGAM = {"
    "            Class = IOGAM"
    "            InputSignals = {"
    "		        Status = {"
    "                    DataSource = UEIReader1"
    "                    Type = uint32"
    "                }"
    "                Timestamp = {"
    "                    DataSource = UEIReader1"
    "                    Type = uint64"
    "                }"
    "		        Output0 = {"
    "                    DataSource = UEIReader1"
    "                    Type = uint32"
    "                }"
    "		        Output1 = {"
    "                    DataSource = UEIReader1"
    "                    Type = float32"
    "                }"
    "		        Output2 = {"
    "                    DataSource = UEIReader1"
    "                    Type = uint64"
    "                }"
    "		        Output3 = {"
    "                    DataSource = UEIReader1"
    "                    Type = float64"
    "                }"
    "            }"
    "            OutputSignals = {"
    "                Status = {"
    "                    DataSource = DDB1"
    "                    Type = uint32"
    "                }		"
    "		        Timestamp = {"
    "                    DataSource = DDB1"
    "                    Type = uint64"
    "		        }"
    "		        Output0 = {"
    "                    DataSource = DDB1"
    "                    Type = uint32"
    "                }"
    "		        Output1 = {"
    "                    DataSource = DDB1"
    "                    Type = float32"
    "                }"
    "		        Output2 = {"
    "                    DataSource = DDB1"
    "                    Type = uint64"
    "                }"
    "		        Output3 = {"
    "                    DataSource = DDB1"
    "                    Type = float64"
    "                }"
    "            }"
    "        }"
    "    }"
    "    +Data = {"
    "        Class = ReferenceContainer"
    "        +UEIReader1 = {"
    "            Class = UEIReaderHL"
    "            Device = \"UEIDevice\""
    "            Map = \"Map1\""
    "	         PollSleepPeriod = 1"
    "            Signals = {"
    "                Status = {"
    "                    Type = uint32"
    "                }"
    "                Timestamp = {"
    "                    Type = uint64"
    "                }"
    "                Output0 = {"
    "                    Type = uint32"
    "                }"
    "                Output1 = {"
    "                    Type = float32"
    "                }"
    "                Output2 = {"
    "                    Type = uint64"
    "                }"
    "		        Output3 = {"
    "                    Type = float64"
    "                }"
    "            }"
    "        }"
    "        +Timings = {"
    "            Class = TimingDataSource"
    "        }"
    "        +DDB1 = {"
    "            Class = GAMDataSource	"
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
    "                    Functions = {CopyGAM}"
    "                }"
    "            }"
    "        }        "
    "    }"
    "    +Scheduler = {"
    "        Class = GAMScheduler"
    "        TimingDataSource = Timings"
    "    }"
    "}";





bool UEIReaderTest::TestConstructor(){
    bool ok = true;
    UEIReaderHL myReader;
    ok &= SafeMath::IsEqual(myReader.GetMapNameHL(), StreamString(""));
    ok &= SafeMath::IsEqual(myReader.GetMapNameHL(), StreamString(""));
    ok &= SafeMath::IsEqual(myReader.GetPollSleepPeriodHL(), (uint32) 100);
    ok &= SafeMath::IsEqual(myReader.GetFirstSyncHL(), true);
    return ok;
}

bool UEIReaderTest::TestInitialise(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDS;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIReaderHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Reader");
        ok &= myDS.IsValid();
    }
    god->Purge();
    return ok;
}

bool UEIReaderTest::TestInitialise_TrueClass(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 404
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDS;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Reader");
        ok &= cdb.Write("Class", "UEIReader");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIReader> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Reader");
        ok &= myDS.IsValid();
    }
    god->Purge();
    return ok;
}

bool UEIReaderTest::TestSimpleMethods(){
    bool ok = true;
    UEIReaderHL myReader;
    ok &= SafeMath::IsEqual(myReader.TerminateInputCopy(0, 0, 0), true);
    ConfigurationDatabase dummyData;
    const char8* broker = myReader.GetBrokerName(dummyData, InputSignals);
    StreamString desiredBroker = "MemoryMapSynchronisedInputBroker";
    ok &= (desiredBroker == broker);
    broker = myReader.GetBrokerName(dummyData, OutputSignals);
    desiredBroker = "";
    ok &= (desiredBroker == broker);
    ok &= SafeMath::IsEqual(myReader.PrepareNextState("One state", "Other state"), true);
    return ok;
}

bool UEIReaderTest::TestInitialise_TwoDsPerMap(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    const MARTe::char8 * const additionalDS = ""
        "+Reader2 = {"
        "   Class = UEIReaderHL"
        "   Device = \"UEIDAQ\""
        "   Map = \"Map1\""
        "   PollSleepPeriod = 1"
        "   Signals = {"
        "       Status = {"
        "           Type = uint32"
        "       }"
        "       Timestamp = {"
        "           Type = uint64"
        "       }"
        "       Output0 = {"
        "           Type = float32"
        "       }"
        "       Output1 = {"
        "           Type = float32"
        "       }"
        "   }"
        "}";
    
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString finalConf = configUEIDS;
    finalConf += additionalDS;
    StreamString configStream = finalConf;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    god->Purge();
    return ok;
}

bool UEIReaderTest::TestStartup(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIReaderHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.UEIReader1");
        ok &= myDS.IsValid();
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok){
        ok = application->ConfigureApplication();
    }
    //Test the prepareNextState method
    if(ok){
        ok &= myDS->PrepareNextState("NoState", "OneState");
    }
    god->Purge();
    return ok;
}

bool UEIReaderTest::TestStartup_InvalidChannelNumber(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals");
        ok &= cdb.Delete("Output0");
        ok &= cdb.MoveAbsolute("$Test.+Data.+UEIReader1.Signals");
        ok &= cdb.Delete("Output0");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIReaderHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.UEIReader1");
        ok &= myDS.IsValid();
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIReaderTest::TestStartup_InvalidStatusChannel(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals");
        ok &= cdb.Delete("Status");
        ok &= cdb.CreateRelative("State");
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.State");
        ok &= cdb.Write("Type", "uint32");
        ok &= cdb.Write("DataSource", "UEIReader1");
        ok &= cdb.MoveAbsolute("$Test.+Data.+UEIReader1.Signals");
        ok &= cdb.Delete("Status");
        ok &= cdb.CreateRelative("State");
        ok &= cdb.MoveAbsolute("$Test.+Data.+UEIReader1.Signals.State");
        ok &= cdb.Write("Type", "uint32");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIReaderHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.UEIReader1");
        ok &= myDS.IsValid();
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIReaderTest::TestStartup_InvalidTimestampChannel(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals");
        ok &= cdb.Delete("Timestamp");
        ok &= cdb.CreateRelative("Time");
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Time");
        ok &= cdb.Write("Type", "float32");
        ok &= cdb.Write("DataSource", "UEIReader1");
        ok &= cdb.MoveAbsolute("$Test.+Data.+UEIReader1.Signals");
        ok &= cdb.Delete("Timestamp");
        ok &= cdb.CreateRelative("Time");
        ok &= cdb.MoveAbsolute("$Test.+Data.+UEIReader1.Signals.Time");
        ok &= cdb.Write("Type", "float32");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIReaderHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.UEIReader1");
        ok &= myDS.IsValid();
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIReaderTest::TestStartup_InvalidNumberOfElements(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Status");
        ok &= cdb.Write("NumberOfElements", 2);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIReaderHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.UEIReader1");
        ok &= myDS.IsValid();
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIReaderTest::TestStartup_InvalidNumberOfDimensions(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Status");
        ok &= cdb.Write("NumberOfDimensions", 2);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIReaderHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.UEIReader1");
        ok &= myDS.IsValid();
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}
bool UEIReaderTest::TestStartup_MultipleSamplesForStatus(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Status");
        ok &= cdb.Write("Samples", 12);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIReaderHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.UEIReader1");
        ok &= myDS.IsValid();
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}
 
bool UEIReaderTest::TestStartup_MismatchedSampleNumber(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output0");
        ok &= cdb.Write("Samples", 12);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIReaderHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.UEIReader1");
        ok &= myDS.IsValid();
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIReaderTest::TestStartup_MultipleDMapSamples(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Timestamp");
        ok &= cdb.Write("Samples", 10);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output0");
        ok &= cdb.Write("Samples", 10);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output1");
        ok &= cdb.Write("Samples", 10);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output2");
        ok &= cdb.Write("Samples", 10);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output3");
        ok &= cdb.Write("Samples", 10);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIReaderHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.UEIReader1");
        ok &= myDS.IsValid();
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIReaderTest::TestSync(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIReaderHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.UEIReader1");
        ok &= myDS.IsValid();
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok){
        ok = application->ConfigureApplication();
    }
    //Test the Sync method
    if(ok){
        //First set an error on the IOM to check the failure of the Sync method on Map Startup call
        manager->SetErrorCode(DqRtDmapInit_ERROR);
        ok &= !myDS->Synchronise();
        //Clear the error and try to get a dummy sample from Sync
        //Carefull with this call as the Data acquisition is performed in loop for VMap full buffer retrieval
        manager->resetManager();
        ok &= myDS->Synchronise();
    }
    god->Purge();
    return ok;
}