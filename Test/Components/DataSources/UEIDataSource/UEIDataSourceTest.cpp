/**
 * @file UEIDataSourceTest.cpp
 * @brief Source file for class UEIDataSourceTest
 * @date 20/03/2023
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
 * the class UEIDataSourceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UEIDataSourceTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//Helper Client to send test data

class IOGAMTest: public IOGAM {
    public:
    CLASS_REGISTER_DECLARATION()
    IOGAMTest() : IOGAM() {}

    ~IOGAMTest() {}

};
CLASS_REGISTER(IOGAMTest, "1.0")

class UEIDataSourceHL : public UEIDataSource {
    public:
    CLASS_REGISTER_DECLARATION()
    UEIDataSourceHL();
    ~UEIDataSourceHL();
    StreamString GetDeviceNameHL ()             {return deviceName;}
    StreamString GetMapNameHL ()                {return mapName;}
    ReferenceT<UEIMasterObject> GetDeviceHL()   {return device;}
    ReferenceT<UEIMapContainer> GetMapHL()      {return map;}
    uint32 GetPollSleepPeriodHL ()              {return PollSleepPeriod;}
    uint32 GetNOutputChannelsHL()               {return nOutputChannels;}
    uint32 GetNSamplesHL()                      {return nSamples;}
    bool GetFirstSyncHL()                       {return firstSync;}
};

UEIDataSourceHL::UEIDataSourceHL() : UEIDataSource(){

}
UEIDataSourceHL::~UEIDataSourceHL(){
    
}
CLASS_REGISTER(UEIDataSourceHL, "1.0")

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
    "   Class = UEIDataSourceHL"
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
    "            Class = IOGAMTest"
    "            InputSignals = {"
    "		        Status = {"
    "                    DataSource = UEIDataSource1"
    "                    Type = uint32"
    "                }"
    "                Timestamp = {"
    "                    DataSource = UEIDataSource1"
    "                    Type = uint64"
    "                }"
    "		        Output0 = {"
    "                    DataSource = UEIDataSource1"
    "                    Type = uint32"
    "                }"
    "		        Output1 = {"
    "                    DataSource = UEIDataSource1"
    "                    Type = float32"
    "                }"
    "		        Output2 = {"
    "                    DataSource = UEIDataSource1"
    "                    Type = uint64"
    "                }"
    "		        Output3 = {"
    "                    DataSource = UEIDataSource1"
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
    "        +UEIDataSource1 = {"
    "            Class = UEIDataSourceHL"
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

bool UEIDataSourceTest::TestConstructor(){
    bool ok = true;
    UEIDataSourceHL myDSHL;
    ok &= SafeMath::IsEqual(myDSHL.GetPollSleepPeriodHL(), 100u);
    ok &= SafeMath::IsEqual(myDSHL.GetFirstSyncHL(), true);
    //test the default implementation for TerminateInputCopy
    UEIDataSource myDS;
    ok &= SafeMath::IsEqual(myDS.TerminateInputCopy(0,0,0), true);
    return ok;
}

bool UEIDataSourceTest::TestInitialise(){
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
    ReferenceT<UEIDataSourceHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("UEIDS");
        ok &= myDS.IsValid();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestInitialise_NoDevice(){
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

bool UEIDataSourceTest::TestInitialise_NoMap(){
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

bool UEIDataSourceTest::TestInitialise_InvalidDevice(){
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

bool UEIDataSourceTest::TestInitialise_InvalidMap(){
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

bool UEIDataSourceTest::TestInitialise_TwoDSperMap(){
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
    //We add an additional Ds with the same map configured, an error must be prompted due to a map not being able
    //to be assigned to two DataSources
    const MARTe::char8 * const additionalDevice = ""
        "+UEIDS1 = {"
        "   Class = UEIDataSourceHL"
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
    StreamString configStream = configUEIDS;
    configStream += additionalDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup(){
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
 /*    ReferenceT<UEIDataSourceHL> myDS;
    if (ok) {
        myDS = ObjectRegistryDatabase::Instance()->Find("Test.Data.UEIDataSource1");
        ok &= myDS.IsValid();
    }*/
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok){
        ok = application->ConfigureApplication();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        application->StartNextStateExecution();
    }
    if (ok) {
        application->StopCurrentStateExecution();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup_MismatchedSignalNumber(){
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
        ok &= cdb.MoveAbsolute("$Test.+Data.+UEIDataSource1.Signals");
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
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup_BadStatusSignal(){
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
    const MARTe::char8 * const localConfig = ""
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
        "	    BufferSamples   = 1000"
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
        "            Class = IOGAMTest"
        "            InputSignals = {"
        "		        Stata = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint8"
        "                }"
        "                Timestamp = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint64"
        "                }"
        "		        Output0 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint32"
        "                }"
        "		        Output1 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = float32"
        "                }"
        "		        Output2 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint64"
        "                }"
        "		        Output3 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = float64"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Stata = {"
        "                    DataSource = DDB1"
        "                    Type = uint8"
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
        "        +UEIDataSource1 = {"
        "            Class = UEIDataSourceHL"
        "            Device = \"UEIDevice\""
        "            Map = \"Map1\""
        "	        PollSleepPeriod = 1"
        "            Signals = {"
        "                Stata = {"
        "                    Type = uint8"
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
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = localConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
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
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup_BadTimestampSignal(){
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
    const MARTe::char8 * const localConfig = ""
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
        "	    BufferSamples   = 1000"
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
        "            Class = IOGAMTest"
        "            InputSignals = {"
        "		        Status = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint32"
        "                }"
        "                Timestamps = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint32"
        "                }"
        "		        Output0 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint32"
        "                }"
        "		        Output1 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = float32"
        "                }"
        "		        Output2 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint64"
        "                }"
        "		        Output3 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = float64"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Status = {"
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                }		"
        "		        Timestamps = {"
        "                    DataSource = DDB1"
        "                    Type = uint32"
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
        "        +UEIDataSource1 = {"
        "            Class = UEIDataSourceHL"
        "            Device = \"UEIDevice\""
        "            Map = \"Map1\""
        "	        PollSleepPeriod = 1"
        "            Signals = {"
        "                Status = {"
        "                    Type = uint32"
        "                }"
        "                Timestamps = {"
        "                    Type = uint32"
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
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = localConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
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
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup_MultipleElements(){
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
    const MARTe::char8 * const localConfig = ""
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
        "	    BufferSamples   = 1000"
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
        "            Class = IOGAMTest"
        "            InputSignals = {"
        "		        Status = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint32"
        "                    NumberOfElements=10"
        "                }"
        "                Timestamp = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint64"
        "                }"
        "		        Output0 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint32"
        "                }"
        "		        Output1 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = float32"
        "                }"
        "		        Output2 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint64"
        "                }"
        "		        Output3 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = float64"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Status = {"
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                    NumberOfElements=10"
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
        "        +UEIDataSource1 = {"
        "            Class = UEIDataSourceHL"
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
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = localConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
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
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup_MultipleFunctions(){
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
    const MARTe::char8 * const localConfig = ""
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
        "	    BufferSamples   = 1000"
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
        "            Class = IOGAMTest"
        "            InputSignals = {"
        "		        Status = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint32"
        "                }"
        "                Timestamp = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint64"
        "                }"
        "		        Output0 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint32"
        "                }"
        "		        Output1 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = float32"
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
        "            }"
        "        }"
        "        +CopyGAM1 = {"
        "            Class = IOGAMTest"
        "            InputSignals = {"
        "		        Output2 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint64"
        "                }"
        "		        Output3 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = float64"
        "                }"
        "            }"
        "            OutputSignals = {"
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
        "        +UEIDataSource1 = {"
        "            Class = UEIDataSourceHL"
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
        "		         Output3 = {"
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
        "                    Functions = {CopyGAM, CopyGAM1}"
        "                }"
        "            }"
        "        }        "
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = localConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
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
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup_MismatchedStatusSamples(){
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
        ok &= cdb.Write("Samples", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Status");
        ok &= cdb.Write("NumberOfElements", 2u);
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
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup_MismatchedSampleNumber(){
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
        ok &= cdb.Write("Samples", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output0");
        ok &= cdb.Write("NumberOfElements", 2u);
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
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup_MultipleSamplesDMap(){
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
        ok &= cdb.Write("Samples", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output0");
        ok &= cdb.Write("Samples", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output1");
        ok &= cdb.Write("Samples", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output2");
        ok &= cdb.Write("Samples", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output3");
        ok &= cdb.Write("Samples", 2u);

        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Timestamp");
        ok &= cdb.Write("NumberOfElements", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output0");
        ok &= cdb.Write("NumberOfElements", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output1");
        ok &= cdb.Write("NumberOfElements", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output2");
        ok &= cdb.Write("NumberOfElements", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output3");
        ok &= cdb.Write("NumberOfElements", 2u);

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
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup_SingleSamplesVMap(){
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
        ok &= cdb.MoveAbsolute("+UEIDevice.+Maps.+Map1");
        ok &= cdb.Write("Class", "UEIRtVMap");
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
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup_MultipleSamplesVMap(){
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
        ok &= cdb.Write("Samples", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output0");
        ok &= cdb.Write("Samples", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output1");
        ok &= cdb.Write("Samples", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output2");
        ok &= cdb.Write("Samples", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.InputSignals.Output3");
        ok &= cdb.Write("Samples", 2u);

        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Timestamp");
        ok &= cdb.Write("NumberOfElements", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output0");
        ok &= cdb.Write("NumberOfElements", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output1");
        ok &= cdb.Write("NumberOfElements", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output2");
        ok &= cdb.Write("NumberOfElements", 2u);
        ok &= cdb.MoveAbsolute("$Test.+Functions.+CopyGAM.OutputSignals.Output3");
        ok &= cdb.Write("NumberOfElements", 2u);

        ok &= cdb.MoveAbsolute("+UEIDevice.+Maps.+Map1");
        ok &= cdb.Write("Class", "UEIRtVMap");

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
    if (ok){
        ok = application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup_OutputSignals(){
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
    const MARTe::char8 * const localConfig = ""
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
        "	    BufferSamples   = 1000"
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
        "            Class = IOGAMTest"
        "            InputSignals = {"
        "		        Stata = {"
        "                    DataSource = DDB1"
        "                    Type = uint8"
        "                }"
        "                Timestamp = {"
        "                    DataSource = DDB1"
        "                    Type = uint64"
        "                }"
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
        "            OutputSignals = {"
        "                Stata = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint8"
        "                }		"
        "		        Timestamp = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint64"
        "		        }"
        "		        Output0 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint32"
        "                }"
        "		        Output1 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = float32"
        "                }"
        "		        Output2 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = uint64"
        "                }"
        "		        Output3 = {"
        "                    DataSource = UEIDataSource1"
        "                    Type = float64"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +UEIDataSource1 = {"
        "            Class = UEIDataSource"
        "            Device = \"UEIDevice\""
        "            Map = \"Map1\""
        "	        PollSleepPeriod = 1"
        "            Signals = {"
        "                Stata = {"
        "                    Type = uint8"
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

    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
     StreamString configStream = localConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
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
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}

bool UEIDataSourceTest::TestStartup_InvalidMapType(){
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
    if(ok){
        ok &= cdb.MoveAbsolute("+UEIDevice.+Maps.+Map1");
        ok &= cdb.Write("Class", "UEIMapContainer");
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
    if (ok){
        ok = !application->ConfigureApplication();
    }
    god->Purge();
    return ok;
}