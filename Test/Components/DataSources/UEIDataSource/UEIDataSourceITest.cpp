/**
 * @file UEIDataSourceITest.cpp
 * @brief Source file for class UEIDataSourceITest
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
 * the class UEIDataSourceITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UEIDataSourceITest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//Helper Client to send test data

class UEIDataSourceIHL : public UEIDataSourceI {
    public:
    CLASS_REGISTER_DECLARATION()
    UEIDataSourceIHL();
    ~UEIDataSourceIHL();
    StreamString GetDeviceNameHL ()             {return deviceName;}
    StreamString GetMapNameHL ()                {return mapName;}
    ReferenceT<UEIMasterObject> GetDeviceHL()   {return device;}
    ReferenceT<UEIMapContainer> GetMapHL()      {return map;}
    uint32 GetPollSleepPeriodHL ()              {return PollSleepPeriod;}
    uint32 GetNSamplesHL()                      {return nSamples;}
    bool GetFirstSyncHL()                       {return firstSync;}
};

UEIDataSourceIHL::UEIDataSourceIHL() : UEIDataSourceI(){

}
UEIDataSourceIHL::~UEIDataSourceIHL(){
    
}
CLASS_REGISTER(UEIDataSourceIHL, "1.0")

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
    "+UEIDS = {"
    "   Class = UEIDataSourceIHL"
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
    "                    DataSource = UEIDataSourceI1"
    "                    Type = uint32"
    "                }"
    "                Timestamp = {"
    "                    DataSource = UEIDataSourceI1"
    "                    Type = uint64"
    "                }"
    "		        Output0 = {"
    "                    DataSource = UEIDataSourceI1"
    "                    Type = uint32"
    "                }"
    "		        Output1 = {"
    "                    DataSource = UEIDataSourceI1"
    "                    Type = float32"
    "                }"
    "		        Output2 = {"
    "                    DataSource = UEIDataSourceI1"
    "                    Type = uint64"
    "                }"
    "		        Output3 = {"
    "                    DataSource = UEIDataSourceI1"
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
    "        +UEIDataSourceI1 = {"
    "            Class = UEIDataSourceIHL"
    "            Device = \"UEIDevice\""
    "            Map = \"Map1\""
    "	        PollSleepPeriod = 1"
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

bool UEIDataSourceITest::TestConstructor(){
    bool ok = true;
    UEIDataSourceIHL myDSHL;
    ok &= SafeMath::IsEqual(myDSHL.GetPollSleepPeriodHL(), 100u);
    ok &= SafeMath::IsEqual(myDSHL.GetFirstSyncHL(), true);
    //test the default implementation for TerminateInputCopy
    ok &= SafeMath::IsEqual(myDSHL.TerminateInputCopy(0,0,0), true);
    UEIDataSourceI myDS;
    ok &= SafeMath::IsEqual(myDS.Synchronise(), false);
    ConfigurationDatabase cdb;
    const char8* expectedBroker = myDSHL.GetBrokerName(cdb, InputSignals);
    StreamString desiredBroker = "";
    ok &= (desiredBroker == expectedBroker);
    ok &= SafeMath::IsEqual(myDS.Synchronise(), false);
    //Test base mthods
    return ok;
}

bool UEIDataSourceITest::TestInitialise(){
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
    ReferenceT<UEIDataSourceIHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("UEIDS");
        ok &= myDS.IsValid();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceITest::TestInitialise_TrueClass(){
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
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDS");
        ok &= cdb.Write("Class", "UEIDataSourceI");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIDataSourceI> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("UEIDS");
        ok &= myDS.IsValid();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceITest::TestInitialise_NoDevice(){
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
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDS");
        ok &= cdb.Delete("Device");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceITest::TestInitialise_NoMap(){
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
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDS");
        ok &= cdb.Delete("Map");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceITest::TestInitialise_InvalidDevice(){
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
    if (ok){
        ok &= cdb.Delete("+UEIDAQ");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceITest::TestInitialise_InvalidMap(){
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
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDS");
        ok &= cdb.Write("Map", "map3");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceITest::TestBaseMethods(){
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
        ok &= cdb.MoveAbsolute("$Test.+Data.+UEIDataSourceI1.Signals");
        ok &= cdb.Delete("Output3");
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals");
        ok &= cdb.Delete("Output3");
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals");
        ok &= cdb.Delete("Output3");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    ReferenceT<UEIDataSourceIHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.UEIDataSourceI1");
        ok &= myDS.IsValid();
    }
    if (ok){
        ok &= SafeMath::IsEqual(myDS->PrepareNextState("None", "Any"), true);
        ok &= SafeMath::IsEqual(myDS->Synchronise(), false);
        ok &= SafeMath::IsEqual(myDS->SetConfiguredDatabase(), false);
    }
    god->Purge();
    return ok;
}
