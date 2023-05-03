/**
 * @file UEIWriterTest.cpp
 * @brief Source file for class UEIWriterTest
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
 * the class UEIWriterTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UEIWriterTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//Helper Client to send test data

class UEIWriterHL : public UEIWriter {
    public:
    CLASS_REGISTER_DECLARATION()
    UEIWriterHL();
    ~UEIWriterHL();
    StreamString GetDeviceNameHL ()             {return deviceName;}
    StreamString GetMapNameHL ()                {return mapName;}
    ReferenceT<UEIMasterObject> GetDeviceHL()   {return device;}
    ReferenceT<UEIMapContainer> GetMapHL()      {return map;}
    uint32 GetPollSleepPeriodHL ()              {return PollSleepPeriod;}
    uint32 GetNSamplesHL()                      {return nSamples;}
    bool GetFirstSyncHL()                       {return firstSync;}

};

UEIWriterHL::UEIWriterHL() : UEIWriter(){

}
UEIWriterHL::~UEIWriterHL(){
    
}
CLASS_REGISTER(UEIWriterHL, "1.0")


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
    "            Class               = UEIDIO404"
    "            Devn                = 1"
    "            VoltageReference    = 5"
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
    "            Outputs = {"
    "                dev1 = {"
    "                    Devn        = 1"
    "                    Channels    = {0, 1}"
    "                }"
    "            }"
    "        } "
    "    }"
    "}"
    "+Writer = {"
    "   Class = UEIWriterHL"
    "   Device = \"UEIDAQ\""
    "   Map = \"Map2\""
    "   PollSleepPeriod = 1"
    "   Signals = {"
    "       Output0 = {"
    "           Type = uint32"
    "       }"
    "       Output1 = {"
    "           Type = uint32"
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
    "         Class               = UEIDIO404"
    "         Devn                = 1"
    "         VoltageReference    = 5.0"
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
    "       }"
    "       Outputs = {"
    "           dev1 = {"
    "                Devn        = 1"
    "                Channels    = {0, 1}"
    "            }"
    "        }"
    "      }"
    "   }"
    "}"
    "$Test = {"
    "    Class = RealTimeApplication"
    "    +Functions = {"
    "        Class = ReferenceContainer"
    "        +CopyGAM = {"
    "            Class = IOGAM"
    "            InputSignals = {"
    "		        State1_Thread1_CycleTime = {"
    "                    Alias = State1.Thread1_CycleTime"
    "                    DataSource = Timings"
    "                    Type = uint32"
    "                }"
    "		         CopyGAM_ExecTime  = {"
    "                    DataSource = Timings"
    "                    Type = uint32"
    "                 }"
    "            }"
    "            OutputSignals = {"
    "		        Output0 = {"
    "                    DataSource = Writer"
    "                    Type = uint32"
    "                }"
    "		        Output1 = {"
    "                    DataSource = Writer"
    "                    Type = uint32"
    "                }"
    "            }"
    "        }"
    "    }"
    "    +Data = {"
    "        Class = ReferenceContainer"
    "        +Writer = {"
    "            Class = UEIWriterHL"
    "            Device = \"UEIDevice\""
    "            Map = \"Map1\""
    "            Signals = {"
    "                Output0 = {"
    "                    Type = uint32"
    "                }"
    "                Output1 = {"
    "                    Type = uint32"
    "                }"
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





bool UEIWriterTest::TestConstructor(){
    bool ok = true;
    UEIWriterHL myReader;
    ok &= SafeMath::IsEqual(myReader.GetMapNameHL(), StreamString(""));
    ok &= SafeMath::IsEqual(myReader.GetMapNameHL(), StreamString(""));
    return ok;
}

bool UEIWriterTest::TestInitialise(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x404); //Slot 1, devn 1, model 404
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
    ReferenceT<UEIWriterHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Writer");
        ok &= myDS.IsValid();
    }
    god->Purge();
    return ok;
}

bool UEIWriterTest::TestInitialise_TrueClass(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x404); //Slot 1, devn 1, model 404
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDS;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Writer");
        ok &= cdb.Write("Class", "UEIWriter");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIWriter> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Writer");
        ok &= myDS.IsValid();
    }
    god->Purge();
    return ok;
}

bool UEIWriterTest::TestSimpleMethods(){
    bool ok = true;
    UEIWriterHL myReader;
    ok &= SafeMath::IsEqual(myReader.TerminateInputCopy(0, 0, 0), true);
    ConfigurationDatabase dummyData;
    const char8* broker = myReader.GetBrokerName(dummyData, InputSignals);
    StreamString desiredBroker = "";
    ok &= (desiredBroker == broker);
    broker = myReader.GetBrokerName(dummyData, OutputSignals);
    desiredBroker = "MemoryMapSynchronisedOutputBroker";
    ok &= (desiredBroker == broker);
    ok &= SafeMath::IsEqual(myReader.PrepareNextState("One state", "Other state"), true);
    return ok;
}


bool UEIWriterTest::TestInitialise_TwoDsPerMap(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x404); //Slot 1, devn 1, model 404
    }
    const MARTe::char8 * const additionalDS = ""
        "+Reader2 = {"
        "   Class = UEIWriterHL"
        "   Device = \"UEIDAQ\""
        "   Map = \"Map2\""
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

bool UEIWriterTest::TestStartup(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x404); //Slot 1, devn 1, model 404
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
    ReferenceT<UEIWriterHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.Writer");
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

bool UEIWriterTest::TestStartup_InvalidChannelNumber(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x404); //Slot 1, devn 1, model 404
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals");
        ok &= cdb.Delete("State1_Thread1_CycleTime");
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals");
        ok &= cdb.Delete("Output0");
        ok &= cdb.MoveAbsolute("$Test.+Data.+Writer.Signals");
        ok &= cdb.Delete("Output0");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIWriterHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.Writer");
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

bool UEIWriterTest::TestStartup_InvalidNumberOfElements(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x404); //Slot 1, devn 1, model 404
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output0");
        ok &= cdb.Write("NumberOfElements", 2);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIWriterHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.Writer");
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

bool UEIWriterTest::TestStartup_InvalidNumberOfDimensions(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x404); //Slot 1, devn 1, model 404
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output0");
        ok &= cdb.Write("NumberOfDimensions", 2);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIWriterHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.Writer");
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
 
bool UEIWriterTest::TestStartup_MismatchedSampleNumber(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x404); //Slot 1, devn 1, model 404
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output0");
        ok &= cdb.Write("Samples", 12);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIWriterHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.Writer");
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

bool UEIWriterTest::TestStartup_MultipleDMapSamples(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x404); //Slot 1, devn 1, model 404
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = SampleApplication;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output0");
        ok &= cdb.Write("Samples", 10);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output1");
        ok &= cdb.Write("Samples", 10);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIWriterHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.Writer");
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

bool UEIWriterTest::TestSync(){
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x404); //Slot 1, devn 1, model 404
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
    ReferenceT<UEIWriterHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.Writer");
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
    //If everything is ok, try the failure on map startup inside the Sync method, by setting an error on the IOM
    if (ok){
        manager->SetErrorCode(DqRtDmapInit_ERROR);
        //This should fail due to the map not being able to start
        ok &= !myDS->Synchronise();
        //Clear the error and make a dummy sync
        manager->resetManager();
        ok &= myDS->Synchronise();
    }
    god->Purge();
    return ok;
}