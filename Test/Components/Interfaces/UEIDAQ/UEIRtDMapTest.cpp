/**
 * @file UEIRtDMapTest.cpp
 * @brief Source file for class UEIRtDMapTest
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
 * the class UEIRtDMapTest (public, protected, and private). Be aware that some 
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
#include "UEIRtDMapTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class UEIRtDMapHL : public UEIRtDMap{ 
    public:
        CLASS_REGISTER_DECLARATION()
        /**
        * @brief Constructs the interface
        */
        UEIRtDMapHL();
        
        /**
        * @brief Destroys the interface
        */
        ~UEIRtDMapHL();
        float32 GetScanRateHL()                 {return scanRate;}
        uint8* GetInputMapHL()                 {return inputMap;}
        void SetDAQ_handleHL(int32 newHandle)   {DAQ_handle = newHandle;}
        void SetMapIdHL(int32 newMapId)         {mapid = newMapId;}
        void SetInputMapHL(uint8* newInputMap) {inputMap = newInputMap;}
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
        bool GetFirstPcktHL()                   {return firstPckt;}
        bool GetMapStartedHL()                  {return mapStarted;}
        bool GetSignalsConfiguredHL()           {return signalsConfigured;}
        bool GetMapCoherentHL()                 {return mapCoherent;}
        bool GetMapPointersHL()                 {return GetMapPointers();}  //proxy function
};
UEIRtDMapHL::UEIRtDMapHL() : UEIRtDMap() {
}

UEIRtDMapHL::~UEIRtDMapHL(){
}

CLASS_REGISTER(UEIRtDMapHL, "1.0")

const MARTe::char8 * const configUEIRtDMap = ""
    "+dummyDev = {"
    "    Class = UEIDevice"
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
    "+Map = {"
    "    Class = UEIRtDMapHL"
    "    ScanRate = 1.0"
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
    "    }"
    "}";

UEIRtDMapTest::UEIRtDMapTest(){
}

UEIRtDMapTest::~UEIRtDMapTest(){
}

bool UEIRtDMapTest::TestConstructor() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    UEIRtDMapHL map;
    ok &= SafeMath::IsEqual(map.GetScanRateHL(), (float32) 0.0);
    ok &= SafeMath::IsEqual(map.GetInputMapHL(), NULL_PTR(uint8*));
    ok &= SafeMath::IsEqual(map.GetType(), RTDMAP);
    ok &= SafeMath::IsEqual(map.GetFirstPcktHL(), true);
    ok &= SafeMath::IsEqual(map.GetMapStartedHL(), false);
    ok &= SafeMath::IsEqual(map.GetSignalsConfiguredHL(), false);
    ok &= SafeMath::IsEqual(map.GetMapCoherentHL(), false);
    //Unsafe, just to trigger inputMap pointer deassignment during destructor execution
    uint8 thisMap;
    map.SetInputMapHL(&thisMap);
    //Trigger errors during map close and stop on destructor
    map.SetDAQ_handleHL(1);
    map.SetMapIdHL(1);
    manager->SetErrorCode(DqRtDmapStop_ERROR);
    //Invoke a new map just to test the no DAQ_handle defined cleanup procedure
    UEIRtDMapHL NoDAQhandlemap;
    return ok;
}

bool UEIRtDMapTest::TestInitialise() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtDMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtDMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        ok &= SafeMath::IsEqual(myMap->GetScanRateHL(), (float32) 1.0);
        ok &= SafeMath::IsEqual(myMap->GetScanRateHL(), (float32) 1.0);
        ok &= SafeMath::IsEqual(myMap->GetnInputMembersHL(), 1u);
        ok &= SafeMath::IsEqual(myMap->GetnOutputMembersHL(), 1u);
        MapMember** myMapList =  myMap->GetinputMembersOrderedHL();
        ok &= SafeMath::IsEqual(myMapList[0]->Inputs.timestampRequired, true);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtDMapTest::TestInitialise_TrueClass() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtDMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map");
        ok &= cdb.Write("Class", "UEIRtDMap");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtDMap> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtDMapTest::TestInitialise_NoScanRate() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtDMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map");
        ok &= cdb.Delete("ScanRate");
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

bool UEIRtDMapTest::TestConfigureInputsForDataSource() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtDMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtDMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        //Dummy inputs to the function to validate its behavior without the signals being accessed.
        uint64 dummyTimestampMemory;
        uint8* signalAddresses[4];
        TypeDescriptor signalTypes[4];
        ok &= SafeMath::IsEqual(myMap->ConfigureInputsForDataSource(1u, 4u, &dummyTimestampMemory, signalAddresses, signalTypes), true);
        ok &= SafeMath::IsEqual(myMap->GetSignalsConfiguredHL(), true);
        ok &= SafeMath::IsEqual(myMap->ConfigureInputsForDataSource(2u, 4u, &dummyTimestampMemory, signalAddresses, signalTypes), false);
        ok &= SafeMath::IsEqual(myMap->GetSignalsConfiguredHL(), false);
        ok &= SafeMath::IsEqual(myMap->ConfigureInputsForDataSource(0u, 4u, &dummyTimestampMemory, signalAddresses, signalTypes), false);
        ok &= SafeMath::IsEqual(myMap->GetSignalsConfiguredHL(), false);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtDMapTest::TestStartMap() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    IOM* myIOM;
    ok &= manager->GetIOM(1, &myIOM);

    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtDMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtDMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    ReferenceT<UEIDevice> dev0;
    if (ok) {
        dev0 = ObjectRegistryDatabase::Instance()->Find("dev0");
        ok &= dev0.IsValid();
    }
    ReferenceT<UEIDevice> dev1;
    if (ok) {
        dev1 = ObjectRegistryDatabase::Instance()->Find("dev1");
        ok &= dev1.IsValid();
    }
    ReferenceT<UEIDevice> dummyDev;
    if (ok) {
        dummyDev = ObjectRegistryDatabase::Instance()->Find("dummyDev");
        ok &= dummyDev.IsValid();
    }
    if (ok){
        //Dummy inputs to the function to validate its behavior without the signals being accessed.
        //First set the handle to the IOM module (the mockup handle is set to 1)
        myMap->SetDAQHandle(1);
        //First test an unsuccessful Map Start process due to lack of input/output devices references
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dev1), true);
        //Test an unsuccessful Map Start process due to lack of output devices references
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);
        ok &= SafeMath::IsEqual(myMap->SetDevice(0, dev0), true);
        //Test a dummy but successful Map Start process
        ok &= SafeMath::IsEqual(myMap->StartMap(), true);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), true);
        //Now test with an error during map opening
        manager->SetErrorCode(DqRtDmapInit_ERROR);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);
        //Now test with an error during timestamp channel setting
        manager->SetErrorCode(DqRtDmapAddChannel_TIMESTAMP_ERROR);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);
        //Now test with an error during input channel setting
        manager->SetErrorCode(DqRtDmapAddChannel_INPUT_ERROR);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);
        //Now test with an error during output channel setting
        manager->SetErrorCode(DqRtDmapAddChannel_OUTPUT_ERROR);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);
        //Test error on DmapStart call
        manager->SetErrorCode(DqRtDmapStart_ERROR);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);
        //Test map failure due to bad refresh in GetMapPointers
        manager->SetErrorCode(DqRtDmapRefresh_ERROR);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);
        //Test what happens if the configuration process for a input device fails, the easiest way is to use a 
        //reference to a simple UEIDevice (returns false by default on ConfigureChannel)

        manager->resetManager();
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dummyDev), true);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);
        //Restore the input device
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dev1), true);
        ok &= SafeMath::IsEqual(myMap->SetDevice(0, dummyDev), true);
        ok &= SafeMath::IsEqual(myMap->StartMap(), false);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);
        ok &= SafeMath::IsEqual(myMap->SetDevice(0, dev0), true);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtDMapTest::TestCoherencyCheck() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();

    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtDMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        //We need to remove the Outputs of this map, as UEIAI217_803 does not
        //allow for output signals, otherwise UEIDevice::CheckMapCoherency will be false
        ok &= cdb.MoveAbsolute("+Map");
        ok &= cdb.Delete("Outputs");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtDMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    ReferenceT<UEIDevice> dev1;
    if (ok) {
        dev1 = ObjectRegistryDatabase::Instance()->Find("dev1");
        ok &= dev1.IsValid();
    }
    if (ok){
        //Set dev1, otherwise UEIDevice::CheckMapCoherency will fail due to invalid references
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dev1), true);
        //This call must succeed, as no error is present
        ok &= SafeMath::IsEqual(myMap->CheckMapCoherency(), true);
        ok &= SafeMath::IsEqual(myMap->GetMapCoherentHL(), true);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtDMapTest::TestCoherencyCheckFail() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();

    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtDMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtDMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    ReferenceT<UEIDevice> dev1;
    if (ok) {
        dev1 = ObjectRegistryDatabase::Instance()->Find("dev1");
        ok &= dev1.IsValid();
    }
    ReferenceT<UEIDevice> dev0;
    if (ok) {
        dev0 = ObjectRegistryDatabase::Instance()->Find("dev0");
        ok &= dev0.IsValid();
    }
    if (ok){
        //Set dev1 and dev0, otherwise UEIDevice::CheckMapCoherency will fail due to invalid references
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dev1), true);
        ok &= SafeMath::IsEqual(myMap->SetDevice(0, dev0), true);
        //This call must fail due to UEIDevice::CheckMapCoherency failing as no outputs are allowed for UEIAI217_803
        ok &= SafeMath::IsEqual(myMap->CheckMapCoherency(), false);
        ok &= SafeMath::IsEqual(myMap->GetMapCoherentHL(), false);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtDMapTest::TestCoherencyCheckHighScanRate() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();

    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtDMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map");
        ok &= cdb.Delete("Outputs");
        ok &= cdb.Write("ScanRate", (float32) 1001.0);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtDMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    ReferenceT<UEIDevice> dev1;
    if (ok) {
        dev1 = ObjectRegistryDatabase::Instance()->Find("dev1");
        ok &= dev1.IsValid();
    }
    if (ok){
        //Set dev1, otherwise UEIDevice::CheckMapCoherency will fail due to invalid references
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dev1), true);
        //This call must fail due to scan rate on the map being higher than the sampling rate of the hardware layers
        ok &= SafeMath::IsEqual(myMap->CheckMapCoherency(), false);
        ok &= SafeMath::IsEqual(myMap->GetMapCoherentHL(), false);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIRtDMapTest::TestGetMapPointers() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    UEIRtDMapHL myMap;
    //Test failure in call to GetMapPointers due to DAQ_handle being not set properly
    ok &= SafeMath::IsEqual(myMap.GetMapPointersHL(), false);
    //Set the DAQ handle
    ok &= SafeMath::IsEqual(myMap.SetDAQHandle(1), true);
    //The next call to GetMapPointers must succeed, due to the calls of DmapRefresh  and DmapGetInputMap being successful by default
    ok &= SafeMath::IsEqual(myMap.GetMapPointersHL(), true);
    //Set an error during map refresh and the call must fail
    manager->SetErrorCode(DqRtDmapRefresh_ERROR);
    ok &= SafeMath::IsEqual(myMap.GetMapPointersHL(), false);
    //Set an error during map pointer getter and the call must fail
    manager->SetErrorCode(DqRtDmapGetInputMap_ERROR);
    ok &= SafeMath::IsEqual(myMap.GetMapPointersHL(), false);
    //Set an error during map pointer getter which forces the map pointer to not be set, call must fail due to 200ms timeout
    manager->SetErrorCode(DqRtDmapGetInputMap_NULL);
    ok &= SafeMath::IsEqual(myMap.GetMapPointersHL(), false);
    return ok;
}

bool UEIRtDMapTest::TestPollForNewPacket() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    IOM* myIOM;
    MapReturnCode errorCode;
    //Obtain a reference to the IOM
    ok &= SafeMath::IsEqual(manager->GetIOM(1, &myIOM), true);
    uint32* memoryInput;
    if (ok){
        memoryInput = reinterpret_cast<uint32*>(myIOM->MemoryExchangeArea);
    }
    //First we do need to provide a memory region in which to store the incoming samples from the UEIDAQ
    //Since the RtDMap only supports a single sample per channel, we do need to define a total of 4 input signals + a timestamp signal
    //Since we are using UEIAI217_803 hardware layers, input signals types can be uint32, uint64, float32 and float64, we'll try all of them
    uint32  channel4;
    uint64  channel5;
    float32 channel6;
    float64 channel7;
    //We define the timestamp channel as a uint64 (only allowed)
    uint64 timestamp;
    //We also need to define the input signal types 
    TypeDescriptor typesList [4] = {UnsignedInteger32Bit, UnsignedInteger64Bit, Float32Bit, Float64Bit};
    //We make a list of the memory locations of the different signals
    uint8* signalList[4];
    signalList[0] = reinterpret_cast<uint8*>(&channel4);
    signalList[1] = reinterpret_cast<uint8*>(&channel5);
    signalList[2] = reinterpret_cast<uint8*>(&channel6);
    signalList[3] = reinterpret_cast<uint8*>(&channel7);

    //Start the Map
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIRtDMap;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        //We need to remove the Outputs of this map, as UEIAI217_803 does not
        //allow for output signals, otherwise UEIDevice::CheckMapCoherency will be false
        ok &= cdb.MoveAbsolute("+Map");
        ok &= cdb.Delete("Outputs");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIRtDMapHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    ReferenceT<UEIDevice> dev1;
    if (ok) {
        dev1 = ObjectRegistryDatabase::Instance()->Find("dev1");
        ok &= dev1.IsValid();
    }
    if (ok){
        //Set dev1, otherwise UEIDevice::CheckMapCoherency will fail due to invalid references
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dev1), true);
        //This call must succeed, as no error is present
        ok &= SafeMath::IsEqual(myMap->SetDAQHandle(1), true);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);      
        ok &= SafeMath::IsEqual(myMap->GetSignalsConfiguredHL(), false);      
        ok &= SafeMath::IsEqual(myMap->GetMapCoherentHL(), false);
        ok &= SafeMath::IsEqual(myMap->PollForNewPacket(errorCode), false);
        ok &= SafeMath::IsEqual(errorCode, ERROR);
        //Configure inputs
        ok &= SafeMath::IsEqual(myMap->ConfigureInputsForDataSource(1u, 4u, &timestamp, signalList, typesList), true);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);      
        ok &= SafeMath::IsEqual(myMap->GetSignalsConfiguredHL(), true);      
        ok &= SafeMath::IsEqual(myMap->GetMapCoherentHL(), false);
        ok &= SafeMath::IsEqual(myMap->PollForNewPacket(errorCode), false);
        ok &= SafeMath::IsEqual(errorCode, ERROR);
        //Check coherency
        ok &= SafeMath::IsEqual(myMap->CheckMapCoherency(), true);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), false);      
        ok &= SafeMath::IsEqual(myMap->GetSignalsConfiguredHL(), true);      
        ok &= SafeMath::IsEqual(myMap->GetMapCoherentHL(), true);
        ok &= SafeMath::IsEqual(myMap->PollForNewPacket(errorCode), false);
        ok &= SafeMath::IsEqual(errorCode, ERROR);
        //Start the map
        ok &= SafeMath::IsEqual(myMap->StartMap(), true);
        ok &= SafeMath::IsEqual(myMap->GetMapStartedHL(), true);      
        ok &= SafeMath::IsEqual(myMap->GetSignalsConfiguredHL(), true);      
        ok &= SafeMath::IsEqual(myMap->GetMapCoherentHL(), true);
        //Check the pointer to the input memory map is correctyl acquired
        ok &= SafeMath::IsEqual(reinterpret_cast<uint8*>(myMap->GetInputMapHL()), (myIOM->MemoryExchangeArea));      
    }
    //The Map is therefore ready for data acquisition
    if (ok){
        //First set some new data into the MemoryExchangeArea of the IOM in the UEIDAQ Mockup
        //The call to inputMap will return the address to such location and therefore we can directly
        //read and write into the Map from there.
        //The call to refresh does not have any effect when using the mockup, only returns ok or not ok depending
        //On the current error code installed in the manager
        //Write some data into the input Map, (timestamp and UEIAi217 both use the samples as uint32)
        //We write a 100 in the timestamp and a 0v in all the samples
        uint32  newSamples [5] = {100, 0x00800000, 0x00800000, 0x00800000, 0x00800000};
        uint64 expectedTimestamp = 100; 
        //Copy the new data
        memcpy(memoryInput, newSamples, 5*4);
        //Try to poll the new data from the Map
        ok &= SafeMath::IsEqual(myMap->PollForNewPacket(errorCode), true);
        ok &= SafeMath::IsEqual(errorCode, NEW_DATA_AVAILABLE);
        //Test the data retrieved for correctness    
        ok &= SafeMath::IsEqual(timestamp, expectedTimestamp);
        ok &= SafeMath::IsEqual(channel4, (uint32) newSamples[1]);
        ok &= SafeMath::IsEqual(channel5, (uint64) newSamples[2]);
        ok &= SafeMath::IsEqual(channel6, (float32)(((newSamples[3]&0x00FFFFFF)*(20.0/0x00FFFFFF))-10.0));
        ok &= SafeMath::IsEqual(channel7, (float64) (((newSamples[4]&0x00FFFFFF)*(20.0/0x00FFFFFF))-10.0));
        //If we try to re-poll the IOM again, we find no new data (we have not written new data into the MemoryExchangeArea)
        //We must obtain a NO_NEW_DATA_AVAILABLE code
        ok &= SafeMath::IsEqual(myMap->PollForNewPacket(errorCode), true);
        ok &= SafeMath::IsEqual(errorCode, NO_NEW_DATA_AVAILABLE);
        //Test the data retrieved for correctness    
        ok &= SafeMath::IsEqual(timestamp, expectedTimestamp);
        ok &= SafeMath::IsEqual(channel4, (uint32) newSamples[1]);
        ok &= SafeMath::IsEqual(channel5, (uint64) newSamples[2]);
        ok &= SafeMath::IsEqual(channel6, (float32)(((newSamples[3]&0x00FFFFFF)*(20.0/0x00FFFFFF))-10.0));
        ok &= SafeMath::IsEqual(channel7, (float64) (((newSamples[4]&0x00FFFFFF)*(20.0/0x00FFFFFF))-10.0));
        //Repeat this process a number of times to check
        uint32 numberOfSteps = 1000;
        for(uint32 i = 0; i < numberOfSteps && ok; i++){
            //This are just some sweeping values, not very interesting apart from testing
            //the full range of the measurements
            //Note that the timestamp has a periodicity of 10, by doing so we aim to see the 
            //effects of the uint32 to uint64 timestamp corrector working properly
            newSamples[0] = (uint32) (0xFFFFFFFF/(numberOfSteps/10))*(i%(numberOfSteps/10));
            newSamples[1] = (uint32) (0x00800000/numberOfSteps)*i; 
            newSamples[2] = (uint32) (0xFFFFFFFF-(0x00800000/numberOfSteps)*i);
            newSamples[3] = (uint32)((0x00FFFFFF/numberOfSteps)*i);
            newSamples[4] = (uint32)(0x00FFFFFF-(0x00FFFFFF/numberOfSteps)*i);
            //Copy the new data
            memcpy(memoryInput, newSamples, 5*4);
            //Try to poll the new data from the Map
            ok &= SafeMath::IsEqual(myMap->PollForNewPacket(errorCode), true);
            ok &= SafeMath::IsEqual(errorCode, NEW_DATA_AVAILABLE);
            //Calculate the expected results
            uint32 upperTimestamp = ((uint32)(i/(numberOfSteps/10))+1);
            uint32 lowerTimestamp = ((uint32) ((0xFFFFFFFF/(numberOfSteps/10))*(i%(numberOfSteps/10))));
            expectedTimestamp = lowerTimestamp+upperTimestamp*0x100000000;
            //Test the data retrieved for correctness    
            ok &= SafeMath::IsEqual(timestamp, expectedTimestamp);
            ok &= SafeMath::IsEqual(channel4, (uint32) newSamples[1]&0x00FFFFFF);
            ok &= SafeMath::IsEqual(channel5, (uint64) newSamples[2]&0x00FFFFFF);
            ok &= SafeMath::IsEqual(channel6, (float32)(((newSamples[3]&0x00FFFFFF)*(20.0/0x00FFFFFF))-10.0));
            ok &= SafeMath::IsEqual(channel7, (float64) (((newSamples[4]&0x00FFFFFF)*(20.0/0x00FFFFFF))-10.0));
        }

    }
    //If the data retrieving test is passed, test the possible errors on the PollForNewPacket method
    if (ok){
        //Previously it has been tested the first error condition, if the map is not ready to receive
        //Test for error condition when DqRtDmapRefresh call fails
        manager->SetErrorCode(DqRtDmapRefresh_ERROR);
        ok &= SafeMath::IsEqual(myMap->PollForNewPacket(errorCode), false);
        ok &= SafeMath::IsEqual(errorCode, ERROR);
        manager->resetManager();
        //Next error is accessing an invalid device reference, for that we must change the reference of the device to
        //an invalid one, it cannot be done through the setters as they're protected and safe from such operation.
        //To do it, access directly the underlying pointer to the reference, this is very dirty and only allowed for testing
        MapMember** memberList = myMap->GetinputMembersOrderedHL();
        memberList[0]->reference = ReferenceT<UEIDevice>();
        uint32 newTimestamp = 0x01;
        memcpy(memoryInput, &newTimestamp, 1*4);
        ok &= SafeMath::IsEqual(myMap->PollForNewPacket(errorCode), false);
        ok &= SafeMath::IsEqual(errorCode, ERROR);
        //After this error is checked, a new valid reference must be set in place again, otherwise a segfault may occur when MARTe
        //tries to destroy the reference later on.
        //Change the current device used for data conversion for an invalid one, which returns always false on data conversion
        ReferenceT<UEIDevice> dummyDev;
        if (ok) {
            dummyDev = ObjectRegistryDatabase::Instance()->Find("dummyDev");
            ok &= dummyDev.IsValid();
        }
        ok &= SafeMath::IsEqual(myMap->SetDevice(1, dummyDev), true);
        //we do need to renew at least the timestamp for the PollForNewPacket method to try to scale new values and run into the error
        newTimestamp = 0x02;
        memcpy(memoryInput, &newTimestamp, 1*4);
        ok &= SafeMath::IsEqual(myMap->PollForNewPacket(errorCode), false);
        ok &= SafeMath::IsEqual(errorCode, ERROR);

    }
    if (ok){
        god->Purge();
    }

    return ok;
}