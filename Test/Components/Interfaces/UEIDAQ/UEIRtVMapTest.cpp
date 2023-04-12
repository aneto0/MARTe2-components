/**
 * @file UEIRtVMapTest.cpp
 * @brief Source file for class UEIRtVMapTest
 * @date 22/03/2023
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
 * the class UEIRtVMapTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "StreamString.h"
#include "TypeDescriptor.h"
#include "UEIRtVMapTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class UEIRtVMapHL : public UEIRtVMap{ 
    public:
        CLASS_REGISTER_DECLARATION()
        /**
        * @brief Constructs the interface
        */
        UEIRtVMapHL();
        
        /**
        * @brief Destroys the interface
        */
        ~UEIRtVMapHL();
        void SetDAQ_handleHL(int32 newHandle)   {DAQ_handle = newHandle;}
        void SetMapIdHL(int32 newMapId)         {mapid = newMapId;}
        //Getter functions for UEIMapContainer derived parameters
        bool    GetoutputAssignedToDSHL()       {return outputAssignedToDS;}
        bool    GetinputAssignedToDSHL()        {return inputAssignedToDS;}
        int32   GetMapIdHL()                    {return mapid;}
        uint32  GetnInputChannelsHL()           {return nInputChannels;}
        uint32  GetnOutputChannelsHL()          {return nOutputChannels;}
        int32   GetDAQ_handleHL()               {return DAQ_handle;}
        bool   GetfragmentedMapHL()             {return fragmentedMap;}
        MapMember* GetMembersHL()               {return members;}
        MapMember** GetoutputMembersOrderedHL() {return outputMembersOrdered;}
        MapMember** GetinputMembersOrderedHL()  {return inputMembersOrdered;}
        uint32 GetnInputMembersHL()             {return nInputMembers;}
        uint32 GetnOutputMembersHL()            {return nOutputMembers;}
        uint32 GetsampleNumberHL()              {return sampleNumber;}
        uint32 GettimestampCorrectorHL()        {return timestampCorrector;}
        uint32 GetlastTimestampHL()             {return lastTimestamp;}
        uint8** GetinputSignalAddressesHL()     {return inputSignalAddresses;}
        TypeDescriptor* GetinputSignalTypesHL() {return inputSignalTypes;}
        uint32 GetSampleNumberHL()              {return sampleNumber;}
        uint32 GetNBuffersHL()                  {return nBuffers;}
        bool EnableMapHL()                      {return EnableMap();}
        bool ResetVMapHL()                      {return ResetVMap();}
        void SetnReadSamplesHL(uint32 newNReadSamples)  {nReadSamples = newNReadSamples;}
};
UEIRtVMapHL::UEIRtVMapHL() : UEIRtVMap() {
}

UEIRtVMapHL::~UEIRtVMapHL(){
}

CLASS_REGISTER(UEIRtVMapHL, "1.0")

class UEIDeviceDL : public UEIDevice{ 
    public:
        CLASS_REGISTER_DECLARATION()
        /**
        * @brief Constructs the interface
        */
        UEIDeviceDL();
        
        /**
        * @brief Destroys the interface
        */
        ~UEIDeviceDL();

        uint8 GetSampleSize()       {return 4;}
};
UEIDeviceDL::UEIDeviceDL() : UEIDevice() {
}

UEIDeviceDL::~UEIDeviceDL(){
}

CLASS_REGISTER(UEIDeviceDL, "1.0")

const MARTe::char8 * const configUEIRtVMap = ""
    "+dummyDev = {"
    "    Class = UEIDeviceDL"
    "    Devn = 0"
    "    SamplingFrequency = 1000.0"
    "}"
    "+dev0 = {"
    "    Class = UEIAI217_803"
    "    Devn = 0"
    "    SamplingFrequency = 1000.0"
    "    Gains = {1,2,4,8,16,32,64,1,1,1,1,1,1,1,1,1}"
    "    ADCMode = \"DEFAULT\""
    "}"
    "+dev1 = {"
    "    Class = UEIAI217_803"
    "    Devn = 1"
    "    SamplingFrequency = 1000.0"
    "    Gains = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}"
    "    ADCMode = \"DEFAULT\""
    "}"
    "+dev2 = {"
    "    Class = UEIAI217_803"
    "    Devn = 2"
    "    SamplingFrequency = 1000.0"
    "    Gains = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}"
    "    ADCMode = \"DEFAULT\""
    "}"
    "+Map = {"
    "    Class = UEIRtVMapHL"
    "    Samples = 100"
    "    NumberOfBuffers = 10"
    "    Outputs = {"
    "        dev0 = {"
    "            Devn = 0"
    "            Channels = {0, 1, 2, 3}"
    "        }"
    "    }"
    "    Inputs = {"
    "        dev1 = {"
    "            Devn = 1"
    "            Channels = {4, 5, 6, 7}"
    "        }"
    "        dev2 = {"
    "            Devn = 2"
    "            Channels = {0, 1, 2, 3}"
    "        }"
    "    }"
    "}";

UEIRtVMapTest::UEIRtVMapTest(){
}

UEIRtVMapTest::~UEIRtVMapTest(){
}

bool UEIRtVMapTest::TestConstructor() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    UEIRtVMapHL map;
    ok &= SafeMath::IsEqual(map.GetType(), RTVMAP);
    //Trigger errors during map close and stop on destructor
    map.SetDAQ_handleHL(1);
    map.SetMapIdHL(1);
    manager->SetErrorCode(DqRtVmapStop_ERROR);
    //Invoke a new map just to test the no DAQ_handle defined cleanup procedure
    UEIRtVMapHL NoDAQhandlemap;
    return ok;
}

bool UEIRtVMapTest::TestInitialise() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtVMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtVMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        ok &= SafeMath::IsEqual(myMap->GetSampleNumberHL(), 100u);
        ok &= SafeMath::IsEqual(myMap->GetNBuffersHL(), 10u);
        ok &= SafeMath::IsEqual(myMap->GetnInputMembersHL(), 2u);
        ok &= SafeMath::IsEqual(myMap->GetnOutputMembersHL(), 1u);
        MapMember** myMapList =  myMap->GetinputMembersOrderedHL();
        ok &= SafeMath::IsEqual(myMapList[0]->Inputs.timestampRequired, true);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtVMapTest::TestInitialise_TrueClass() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtVMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map");
        ok &= cdb.Write("Class", "UEIRtVMap");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtVMap> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtVMapTest::TestInitialiseNoSamples() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtVMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map");
        ok &= cdb.Delete("Samples");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtVMapTest::TestInitialiseNumberOfBuffers() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtVMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map");
        ok &= cdb.Delete("NumberOfBuffers");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtVMapTest::TestConfigureInputsForDataSource() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtVMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtVMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        uint64 dummyTimestampMemory;
        uint8* signalAddresses[8];
        TypeDescriptor signalTypes[8];
        ok &= SafeMath::IsEqual(myMap->ConfigureInputsForDataSource(0u, 8u, &dummyTimestampMemory, signalAddresses, signalTypes), false);
        ok &= SafeMath::IsEqual(myMap->ConfigureInputsForDataSource(1u, 8u, &dummyTimestampMemory, signalAddresses, signalTypes), true);
        ok &= SafeMath::IsEqual(myMap->ConfigureInputsForDataSource(2u, 8u, &dummyTimestampMemory, signalAddresses, signalTypes), true);
        ok &= SafeMath::IsEqual(myMap->ConfigureInputsForDataSource(3u, 8u, &dummyTimestampMemory, signalAddresses, signalTypes), true);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtVMapTest::TestEnableMap() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtVMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtVMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    ReferenceT<UEIAI217_803> dev1;
    if (ok) {
        dev1 = ObjectRegistryDatabase::Instance()->Find("dev1");
        ok &= dev1.IsValid();
    }
    ReferenceT<UEIAI217_803> dev2;
    if (ok) {
        dev2 = ObjectRegistryDatabase::Instance()->Find("dev2");
        ok &= dev2.IsValid();
    }
    if (ok){
        //An error must be prompted if we try to Enable the map with no devices reference set previously (no device configuration is possible)
        ok &= SafeMath::IsEqual(myMap->EnableMapHL(), false);
        //Set the devices
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dev1), true);
        ok &= SafeMath::IsEqual(myMap->SetDevice(2, dev2), true);
        //Now the map enabling must succeed
        ok &= SafeMath::IsEqual(myMap->EnableMapHL(), true);
        //Now force an error during map Start
        manager->SetErrorCode(DqRtVmapStart_ERROR);
        ok &= SafeMath::IsEqual(myMap->EnableMapHL(), false);
        //Now force an error during DqRtVmapRqInputDataSz call
        manager->SetErrorCode(DqRtVmapRqInputDataSz_ERROR);
        ok &= SafeMath::IsEqual(myMap->EnableMapHL(), false);
        //Clean errors and try to enable the map
        manager->resetManager();
        ok &= SafeMath::IsEqual(myMap->EnableMapHL(), true);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtVMapTest::TestResetMap() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtVMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtVMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    ReferenceT<UEIAI217_803> dev1;
    if (ok) {
        dev1 = ObjectRegistryDatabase::Instance()->Find("dev1");
        ok &= dev1.IsValid();
    }
    ReferenceT<UEIAI217_803> dev2;
    if (ok) {
        dev2 = ObjectRegistryDatabase::Instance()->Find("dev2");
        ok &= dev2.IsValid();
    }
    if (ok){
        //Set the devices
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dev1), true);
        ok &= SafeMath::IsEqual(myMap->SetDevice(2, dev2), true);
        myMap->SetDAQ_handleHL(1);
        myMap->SetMapIdHL(1);
        //Try and reset the map, must suceed
        ok &= SafeMath::IsEqual(myMap->ResetVMapHL(), true);
        //Force an error during Map stop call
        manager->SetErrorCode(DqRtVmapStop_ERROR);
        ok &= SafeMath::IsEqual(myMap->ResetVMapHL(), false);
        //Force an error during map re-start
        manager->SetErrorCode(DqRtVmapStart_ERROR);
        ok &= SafeMath::IsEqual(myMap->ResetVMapHL(), false);
        //Clean errors and retry a successful restart
        manager->resetManager();
        ok &= SafeMath::IsEqual(myMap->ResetVMapHL(), true);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtVMapTest::TestStartMap() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtVMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtVMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    ReferenceT<UEIAI217_803> dev1;
    if (ok) {
        dev1 = ObjectRegistryDatabase::Instance()->Find("dev1");
        ok &= dev1.IsValid();
    }
    ReferenceT<UEIAI217_803> dev2;
    if (ok) {
        dev2 = ObjectRegistryDatabase::Instance()->Find("dev2");
        ok &= dev2.IsValid();
    }
    ReferenceT<UEIDevice> dummyDev;
    if (ok) {
        dummyDev = ObjectRegistryDatabase::Instance()->Find("dummyDev");
        ok &= dummyDev.IsValid();
    }
    if (ok){
        //Try starting the map without devices references, it must fail
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        //Set the devices
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dev1), true);
        ok &= SafeMath::IsEqual(myMap->SetDevice(2, dev2), true);
        //If we try to start the map after the devices are it must fail as the number of samples to be read from 
        //CircularBuffer into MARTe are not yet set
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        uint64 timestampAddr;
        uint8* signalAddresses[8];
        TypeDescriptor types[8];
        //Configure the circularBuffers to retrieve 10 samples into MARTe
        ok &= SafeMath::IsEqual(myMap->ConfigureInputsForDataSource(10u, 8u, &timestampAddr, signalAddresses, types), true);
        //Now the StartMap call must succeed
        ok &= SafeMath::IsEqual(myMap->StartMap(), true);
        //Now we force an error on DqRtVmapInitEx call
        manager->SetErrorCode(DqRtVmapInitEx_ERROR);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        //Now we force an error on DqRtVmapInitEx call
        manager->SetErrorCode(DqRtVmapAddChannel_ERROR);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        //Now we force an error on DqRtVmapSetChannelList call
        manager->SetErrorCode(DqRtVmapSetChannelList_ERROR);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        //Now we force an error on DqRtVmapSetScanRate call
        manager->SetErrorCode(DqRtVmapSetScanRate_ERROR);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        //Now we force an error During map enabling
        manager->SetErrorCode(DqRtVmapStart_ERROR);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        //To cause an error of configure channels, set a dummy device reference
        manager->resetManager();
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dummyDev), true);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtVMapTest::TestPollForNewPacket() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtVMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtVMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    ReferenceT<UEIAI217_803> dev1;
    if (ok) {
        dev1 = ObjectRegistryDatabase::Instance()->Find("dev1");
        ok &= dev1.IsValid();
    }
    ReferenceT<UEIAI217_803> dev2;
    if (ok) {
        dev2 = ObjectRegistryDatabase::Instance()->Find("dev2");
        ok &= dev2.IsValid();
    }
    ReferenceT<UEIDevice> dummyDev;
    if (ok) {
        dummyDev = ObjectRegistryDatabase::Instance()->Find("dummyDev");
        ok &= dummyDev.IsValid();
    }
    if (ok){
        //Try to poll for new packet if the map is not started
        MapReturnCode outputCode;
        ok &= SafeMath::IsEqual(myMap->PollForNewPacket(outputCode), false);
        ok &= SafeMath::IsEqual(outputCode, ERROR);
        //Set the devices and start the map
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dev1), true);
        ok &= SafeMath::IsEqual(myMap->SetDevice(2, dev2), true);
        myMap->SetnReadSamplesHL(10u);
        ok &= SafeMath::IsEqual(myMap->StartMap(), true);
        ok &= SafeMath::IsEqual(myMap->PollForNewPacket(outputCode), false);
        ok &= SafeMath::IsEqual(outputCode, ERROR);
        //We still need to set the configure the input signals for the VMap
        uint64 timestampAddr;
        uint8* signalAddresses[8];
        TypeDescriptor types[8];
        //Configure the circularBuffers to retrieve 10 samples into MARTe
        ok &= SafeMath::IsEqual(myMap->ConfigureInputsForDataSource(10u, 8u, &timestampAddr, signalAddresses, types), true);
        ok &= SafeMath::IsEqual(myMap->PollForNewPacket(outputCode), true);
        //TODO
    }
    if (ok){
        god->Purge();
    }
    return ok;
}