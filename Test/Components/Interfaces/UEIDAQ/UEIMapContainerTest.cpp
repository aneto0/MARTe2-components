/**
 * @file UEIMapContaineTestr.cpp
 * @brief Source file for class UEIMapContainerTest
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
 * the class UEIMapContainerTest (public, protected, and private). Be aware that some 
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
#include "UEIMapContainerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class UEIMapContainerHL : public UEIMapContainer{ 
    public:
        CLASS_REGISTER_DECLARATION()
        /**
        * @brief Constructs the interface
        */
        UEIMapContainerHL();
        
        /**
        * @brief Destroys the interface
        */
        ~UEIMapContainerHL();

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
        uint64* GetTimestampAddrHL()            {return TimestampAddr;}
        bool GetTimestampHL(uint32 inputTimestamp, uint64 &outputTimestamp);
        bool GetTimestampHL(uint32* inputTimestamp, uint32 timestampListLength ,uint64* outputTimestamp);
        bool GetTimestampHL(UEIBufferPointer inputTimestamp, uint32 timestampListLength ,uint64* outputTimestamp);
        void SetnInputChannelsHL(uint32 nChannels)            {nInputChannels = nChannels;}
};

bool UEIMapContainerHL::GetTimestampHL(uint32 inputTimestamp, uint64 &outputTimestamp){
    return GetTimestamp(inputTimestamp, outputTimestamp);
}

bool UEIMapContainerHL::GetTimestampHL(uint32* inputTimestamp, uint32 timestampListLength ,uint64* outputTimestamp){
    return GetTimestamp(inputTimestamp,timestampListLength ,outputTimestamp);
}
bool UEIMapContainerHL::GetTimestampHL(UEIBufferPointer inputTimestamp, uint32 timestampListLength ,uint64* outputTimestamp){
    return GetTimestamp(inputTimestamp, timestampListLength ,outputTimestamp);
}

UEIMapContainerHL::UEIMapContainerHL() : UEIMapContainer() {
}

UEIMapContainerHL::~UEIMapContainerHL(){
}

CLASS_REGISTER(UEIMapContainerHL, "1.0")

class UEITestDevice : public UEIDevice{ 
    public:
        CLASS_REGISTER_DECLARATION()
        /**
        * @brief Constructs the interface
        */
        UEITestDevice();
        
        /**
        * @brief Destroys the interface
        */
        ~UEITestDevice();

        bool    AcceptedSignalType(TypeDescriptor signalType);
        void    SetSamplingFrequency (float32 freq);
        bool    CheckChannelAndDirection(uint32 channelNumber, SignalDirection direction);
};

bool UEITestDevice::CheckChannelAndDirection(uint32 channelNumber, SignalDirection direction){
    //Test function for the inner call to CheckChannelAndDirection during CheckMapCoherency tests
    // for the UEITestDevice class, devices in both directions are allowed if channel number is below 20.
    bool ok = false;
    if (channelNumber <= 20u){
        ok = true;
    }
    return ok;
}

bool UEITestDevice::AcceptedSignalType(TypeDescriptor signalType){
    if (signalType == UnsignedInteger8Bit){
        return true;
    }else{
        return false;
    }
}

void UEITestDevice::SetSamplingFrequency(float32 freq){
    samplingFrequency = freq;
}

UEITestDevice::UEITestDevice() : UEIDevice() {
    name = "UEITestDevice";
}

UEITestDevice::~UEITestDevice(){
}
CLASS_REGISTER(UEITestDevice, "1.0")

const MARTe::char8 * const configUEIMapContainer = ""
    "+Map = {"
    "    Class = UEIMapContainerHL"
    "    Outputs = {"
    "        dev0 = {"
    "            Devn = 0"
    "            Channels = {0, 1, 2, 3}"
    "        }"
    "        dev1 = {"
    "            Devn = 1"
    "            Channels = {0, 1}"
    "        }"
    "        dev5 = {"
    "            Devn = 5"
    "            Channels = {0, 16}"
    "        }"
    "    }"
    "    Inputs = {"
    "        dev3 = {"
    "            Devn = 3"
    "            Channels = {4, 5, 6, 7}"
    "        }"
    "        dev4 = {"
    "            Devn = 1"
    "            Channels = {8, 9, 10, 11}"
    "        }"
    "        dev6 = {"
    "            Devn = 7"
    "            Channels = {12, 13}"
    "        }"
    "    }"
    "}";

const MARTe::char8 * const configUEIMapContainerBaseClass = ""
    "+Map = {"
    "    Class = UEIMapContainer"
    "    Outputs = {"
    "        dev0 = {"
    "            Devn = 0"
    "            Channels = {0, 1, 2, 3}"
    "        }"
    "        dev1 = {"
    "            Devn = 1"
    "            Channels = {0, 1}"
    "        }"
    "        dev5 = {"
    "            Devn = 5"
    "            Channels = {0, 16}"
    "        }"
    "    }"
    "    Inputs = {"
    "        dev3 = {"
    "            Devn = 3"
    "            Channels = {4, 5, 6, 7}"
    "        }"
    "        dev4 = {"
    "            Devn = 1"
    "            Channels = {8, 9, 10, 11}"
    "        }"
    "        dev6 = {"
    "            Devn = 7"
    "            Channels = {12, 13}"
    "        }"
    "    }"
    "}";

UEIMapContainerTest::UEIMapContainerTest(){
}

UEIMapContainerTest::~UEIMapContainerTest(){
}

bool UEIMapContainerTest::TestConstructor() {
    bool ok = true;
    UEIMapContainerHL mapContainer;
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetsampleRate(), (float32) 0.0);
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetoutputAssignedToDSHL(), false);
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetinputAssignedToDSHL(), false);
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetMapIdHL(), (int32) 0);
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetnInputChannelsHL(), (uint32) 0);
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetnOutputChannelsHL(), (uint32) 0);
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetDAQ_handleHL(), (int32) 0);
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetfragmentedMapHL(), false);
    }
    if (ok){
        MapMember* thisMembers = mapContainer.GetMembersHL();
        for (uint32 i = 0u; i < MAX_IO_SLOTS; i++){
            ok &= SafeMath::IsEqual(thisMembers[i].defined, false);
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.defined, false);
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.nChannels, 0u);
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.channels, NULL_PTR(uint32*));
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.requestSize, 0u);
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.timestampRequired, false);
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.defined, false);
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.nChannels, 0u);
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.channels, NULL_PTR(uint32*));
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.requestSize, 0u);
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.timestampRequired, false);
        }
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetoutputMembersOrderedHL(), NULL_PTR(MapMember**));
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetinputMembersOrderedHL(), NULL_PTR(MapMember**));
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetnInputMembersHL(), (uint32) 0 );
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetnOutputMembersHL(), (uint32) 0 );
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetsampleNumberHL(), (uint32) 0 );
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GettimestampCorrectorHL(), (uint32) 0 );
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetlastTimestampHL(), (uint32) 0 );
    }
    if(ok){
        ok &= SafeMath::IsEqual(mapContainer.GetinputSignalAddressesHL(), NULL_PTR(uint8**));
        ok &= SafeMath::IsEqual(mapContainer.GetinputSignalTypesHL(), NULL_PTR(TypeDescriptor*));
        ok &= SafeMath::IsEqual(mapContainer.GetTimestampAddrHL(), NULL_PTR(uint64*));
    }
    return ok;
}

bool UEIMapContainerTest::TestBaseImplementations_Default() {
    bool ok = true;
    UEIMapContainerHL mapContainer;

    if (ok){
        ok = SafeMath::IsEqual(mapContainer.StartMap(), false);
    }
    if (ok){
        MapReturnCode code;
        ok = SafeMath::IsEqual(mapContainer.PollForNewPacket(code), false);
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.GetType(), NO_MAP);
    }    
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.CleanupMap(), false);
    }
    if (ok){
        ok = SafeMath::IsEqual(mapContainer.StopMap(), false);
    }
    return ok;
}

bool UEIMapContainerTest::TestConfigurationSetter() {
    bool ok = true;
    UEIMapContainerHL mapContainer;
    //Check that the pointers are still unassigned
    if(ok){
        ok &= SafeMath::IsEqual(mapContainer.GetinputSignalAddressesHL(), NULL_PTR(uint8**));
        ok &= SafeMath::IsEqual(mapContainer.GetinputSignalTypesHL(), NULL_PTR(TypeDescriptor*));
        ok &= SafeMath::IsEqual(mapContainer.GetTimestampAddrHL(), NULL_PTR(uint64*));
    }
    //Set the input channels for this map to the following number (to avoid all the hassle of initialisation)
    mapContainer.SetnInputChannelsHL(3u);
    uint32 nSamples = 10;
    uint32 nChannels = 3u;
    uint64* Timestamps = new uint64 [10];
    uint8* signalAddresses[3u];
    TypeDescriptor signalTypes[3u];
    if (ok){     
        //Try to perform a correct initialisation
        ok &= mapContainer.ConfigureInputsForDataSource(nSamples, nChannels, Timestamps, signalAddresses, signalTypes);
    }
    if (ok){
        //Check error condition due to null pointer to signalAddresses
        ok &= !mapContainer.ConfigureInputsForDataSource(nSamples, nChannels, Timestamps, NULL_PTR(uint8**), signalTypes);
    }
    if (ok){
        //Check error condition due to null pointer to signalTypes
        ok &= !mapContainer.ConfigureInputsForDataSource(nSamples, nChannels, Timestamps, signalAddresses, NULL_PTR(TypeDescriptor*));
    }
    if (ok){
        //Check error condition due to null pointer to Timestamp pointer
        ok &= !mapContainer.ConfigureInputsForDataSource(nSamples, nChannels, NULL_PTR(uint64*), signalAddresses, signalTypes);
    }
    if (ok){
        //Check error condition due to invalid nChannels
        ok &= !mapContainer.ConfigureInputsForDataSource(nSamples, nChannels-1, NULL_PTR(uint64*), signalAddresses, signalTypes);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainerBaseClass;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMapContainer> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        const char8* myMapName = "+Map";
        ok &= SafeMath::IsEqual(*(myMap->GetName()), *myMapName);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_General(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMapContainerHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        const char8* myMapName = "+Map";
        ok &= SafeMath::IsEqual(*(myMap->GetName()), *myMapName);
    }
    //Check if Inputs and outputs are detected properly
    if (ok){
        ok &= SafeMath::IsEqual(myMap->GetnOutputMembersHL(), (uint32) 3);
        ok &= SafeMath::IsEqual(myMap->GetnInputMembersHL(),  (uint32) 3);
    }
    //Check that the devices are properly configured in the map structures
    //First we check if the defined devices are defined in their respective directions.
    bool DefinedDevices[12]     =   {true , true, false, true , false, true , false, true , false, false, false, false};
    bool DefinedAsInput[12]     =   {false, true, false, true , false, false, false, true , false, false, false, false};
    bool DefinedAsOutput[12]    =   {true , true, false, false, false, true , false, false, false, false, false, false};
    uint32 InputChanneln[12]    =   { 0   ,  4  ,  0   ,   4  ,   0  ,   0  ,   0  ,   2  ,   0  ,   0  ,   0  ,   0  };
    uint32 OutputChanneln[12]   =   { 4   ,  2  ,  0   ,   0  ,   0  ,   2  ,   0  ,   0  ,   0  ,   0  ,   0  ,   0  };
    if (ok){
        //First get the list of members so we can check
        MapMember* thisMembers = myMap->GetMembersHL();
        for (uint32 i = 0; i < MAX_IO_SLOTS; i++){
            //Check if the configuration matches to the provided parameters
            //Check if the device is appointed as defined directly from the member variable
            ok &= SafeMath::IsEqual(thisMembers[i].defined, DefinedDevices[i]);
            //Check if the device is appointed as defined from the method provides by the map
            ok &= SafeMath::IsEqual(myMap->GetDevDefined(i), DefinedDevices[i]);

            //INPUTS CHECKS
            //Check the Input IOMapMember structure for this member
            //Check the device is defined in the required direction directly from map member variable
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.defined,   DefinedAsInput[i]);
            //Check the device is defined in the required direction from GetDevDefined method
            ok &= SafeMath::IsEqual(myMap->GetDevDefined(i, InputSignals), DefinedAsInput[i]);
            //Check the number of channels defined as inputs for this member directly from member variables
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.nChannels, InputChanneln[i]);
            //Check the number of channels defined as inputs for this member from GetNumberOfChannels method
            uint32 nChannelsInput = 0u;
            bool inputChannelNAvailable = myMap->GetNumberOfChannels(i, InputSignals, &nChannelsInput);
            ok &= SafeMath::IsEqual(inputChannelNAvailable, DefinedDevices[i]);
            if (inputChannelNAvailable){
                ok &= SafeMath::IsEqual(nChannelsInput, InputChanneln[i]);
            }
            //Buffer reference must be not assigned
            ok &= (thisMembers[i].Inputs.buffer.IsValid() == false);
            //requestSize must not be assigned
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.requestSize, (uint32) 0);
            
            //OUTPUTS CHECKS
            //Check the Output IOMapMember structure for this member
            //Check the device is defined in the required direction directly from map member variable
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.defined,   DefinedAsOutput[i]);
            //Check the device is defined in the required direction from GetDevDefined method
            ok &= SafeMath::IsEqual(myMap->GetDevDefined(i, OutputSignals), DefinedAsOutput[i]);
            //Check the number of channels defined as outputs for this member directly from member variables
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.nChannels, OutputChanneln[i]);            
            //Check the number of channels defined as outputs for this member from GetNumberOfChannels method
            uint32 nChannelsOutput = 0u;
            bool outputChannelNAvailable = myMap->GetNumberOfChannels(i, OutputSignals, &nChannelsOutput);
            ok &= SafeMath::IsEqual(outputChannelNAvailable, DefinedDevices[i]);
            if (outputChannelNAvailable){
                ok &= SafeMath::IsEqual(nChannelsOutput, OutputChanneln[i]);
            }
            //Buffer reference must be not assigned
            ok &= (thisMembers[i].Outputs.buffer.IsValid() == false);
            //requestSize must not be assigned
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.requestSize, (uint32) 0);
            //Check that the device number is correct
            if (DefinedDevices[i]){
                ok &= SafeMath::IsEqual(thisMembers[i].devn, (uint8) i);    //The devn is only set in case the device is used by the map
            }
            //Check that the reference to the device is still uninitialised (as it must just after initialisation)
            ok &= (thisMembers[i].reference.IsValid() == false);
            //Check that the IOMap members for each of the members are well configured
        }
    }
    //Now we do check that the MapMembers are put in the required order in the outputMembersOrdered and inputMembersOrdered lists
    //Also try to access channel list by idx, convinient as we do have the device numbers defined
    if (ok){
        //Input devices
        uint8 inputDevices_devn [3] = {3, 1, 7};
        uint32 nInputChannels = 3;
        uint32 inputDevices_nChannels[3] = {4, 4, 2};
        uint32* inputDevices_Channels[3];
        uint32 dev3InputChannels [4] = {4, 5, 6, 7}; 
        uint32 dev1InputChannels [4] = {8, 9, 10, 11}; 
        uint32 dev7InputChannels [2] = {12, 13}; 
        inputDevices_Channels[0] = dev3InputChannels;
        inputDevices_Channels[1] = dev1InputChannels;
        inputDevices_Channels[2] = dev7InputChannels;
        //Output devices
        uint8 outputDevices_devn[3] = {0, 1, 5};
        uint32 nOutputChannels = 3;
        uint32 outputDevices_nChannels[3] = {4, 2, 2};
        uint32* outputDevices_Channels[3];
        uint32 dev0OutputChannels [4] = {0, 1, 2, 3}; 
        uint32 dev1OutputChannels [2] = {0, 1};
        uint32 dev5OutputChannels [2] = {0, 16}; 
        outputDevices_Channels[0] = dev0OutputChannels;
        outputDevices_Channels[1] = dev1OutputChannels;
        outputDevices_Channels[2] = dev5OutputChannels;
        //Let's test the ordered access to the map members
        MapMember* thisMembers = myMap->GetMembersHL();
        MapMember** thisOutputMembersOrdered = myMap->GetoutputMembersOrderedHL();
        for (uint8 i = 0; i < 3; i++){
            ok &= SafeMath::IsEqual(thisOutputMembersOrdered[i]->devn, outputDevices_devn[i]);
            //Check that it is actually the pointer to were we think it points, if that is ok, the previous checks certify that
            //the map member will work as presumed
            ok &= SafeMath::IsEqual(thisOutputMembersOrdered[i], &(thisMembers[outputDevices_devn[i]]));
        }
        MapMember** thisInputMembersOrdered = myMap->GetinputMembersOrderedHL();
        for (uint8 i = 0; i < 3; i++){
            ok &= SafeMath::IsEqual(thisInputMembersOrdered[i]->devn, inputDevices_devn[i]);
            //Check that it is actually the pointer to were we think it points, if that is ok, the previous checks certify that
            //the map member will work as presumed
            ok &= SafeMath::IsEqual(thisInputMembersOrdered[i], &(thisMembers[inputDevices_devn[i]]));
        }
        //Lets try to retrieve the channels from the initialised devices in order
        //First try the input channels
        for (uint32 i = 0; i < nInputChannels && ok; i++){
            uint32 thisChannel;
            for (uint32 j = 0; j < inputDevices_nChannels[i] && ok; j++){
                ok &= SafeMath::IsEqual(myMap->GetChannelOfMember(inputDevices_devn[i], InputSignals, j, thisChannel), true);
                if (ok){
                    ok &= SafeMath::IsEqual(thisChannel, (uint32) inputDevices_Channels[i][j]);
                }
            }
        }
        //Try for output channels
        for (uint32 i = 0; i < nOutputChannels && ok; i++){
            uint32 thisChannel;
            for (uint32 j = 0; j < outputDevices_nChannels[i] && ok; j++){
                ok &= SafeMath::IsEqual(myMap->GetChannelOfMember(outputDevices_devn[i], OutputSignals, j, thisChannel), true);
                if (ok){
                    ok &= SafeMath::IsEqual(thisChannel, (uint32) outputDevices_Channels[i][j]);
                }
            }
        }
        //Try the error results for GetChannelOfMember method
        if (ok){
            uint32 thisChannel;
            //Try to access a device outside the maximum allowed device number
            ok &= SafeMath::IsEqual(myMap->GetChannelOfMember(13u, InputSignals, 0, thisChannel), false);
            //Try to acces a device with invalid channel direction;
            ok &= SafeMath::IsEqual(myMap->GetChannelOfMember(0, None, 0, thisChannel), false);
            //Try to access an undefined member (a device not configured for this map)
            ok &= SafeMath::IsEqual(myMap->GetChannelOfMember(2u, InputSignals, 0, thisChannel), false);
        }
    }
    //Check if the I/O channels are assigned to any DataSource
    if (ok){
        ok = SafeMath::IsEqual(myMap->GetoutputAssignedToDSHL(), false);
        ok &= SafeMath::IsEqual(myMap->GetinputAssignedToDSHL(), false);
    }
    //Check the error condition on GetDevDefined() method for a devn outside the maximum allowed
    if (ok){
        ok &= SafeMath::IsEqual(myMap->GetDevDefined(MAX_IO_SLOTS+1), false);
    }
    //Check error condition for GetDevDefined() method for invalid direction/devn
    if (ok){
        //Return false in case wrong direction is provided
        ok &= SafeMath::IsEqual(myMap->GetDevDefined(0, None), false);
        //Return false in case wrong devn is provided
        ok &= SafeMath::IsEqual(myMap->GetDevDefined(MAX_IO_SLOTS+1, InputSignals), false);
    }
    //Check the error condition for GetNumberOfChannels() method
    if (ok){
        uint32 nChannelsTest = 0u;
        ok &= SafeMath::IsEqual(myMap->GetNumberOfChannels(MAX_IO_SLOTS+1, InputSignals, &nChannelsTest), false);
        ok &= SafeMath::IsEqual(nChannelsTest, (uint32) 0);
        ok &= SafeMath::IsEqual(myMap->GetNumberOfChannels(0, None, &nChannelsTest), false);
        ok &= SafeMath::IsEqual(nChannelsTest, (uint32) 0);
    }
    if (ok){
        //Check the ammount of channels in each different direction is ok
        uint32 nChannels;
        ok &= SafeMath::IsEqual(myMap->GetNumberOfChannels(InputSignals, nChannels), true);
        ok &= SafeMath::IsEqual(nChannels, (uint32) 10u);
        ok &= SafeMath::IsEqual(myMap->GetNumberOfChannels(OutputSignals, nChannels), true);
        ok &= SafeMath::IsEqual(nChannels, (uint32) 8u);
        //Check error condition
        ok &= SafeMath::IsEqual(myMap->GetNumberOfChannels(None, nChannels), false);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_NoOutputs(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map");
        ok &= cdb.Delete("Outputs");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMapContainerHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        const char8* myMapName = "+Map";
        ok &= SafeMath::IsEqual(*(myMap->GetName()), *myMapName);
    }
    if (ok){
        ok &= SafeMath::IsEqual(myMap->GetnOutputMembersHL(), (uint32) 0);
        ok &= SafeMath::IsEqual(myMap->GetnInputMembersHL(),  (uint32) 3);
    }
    //Check that the devices are properly configured in the map structures
    //First we check if the defined devices are defined in their respective directions.
    bool DefinedDevices[12]     =   {false , true , false, true , false, false , false, true , false, false, false, false};
    bool DefinedAsInput[12]     =   {false , true , false, true , false, false , false, true , false, false, false, false};
    bool DefinedAsOutput[12]    =   {false , false, false, false, false, false , false, false, false, false, false, false};
    uint32 InputChanneln[12]    =   { 0   ,  4  ,  0   ,   4  ,   0  ,   0  ,   0  ,   2  ,   0  ,   0  ,   0  ,   0  };
    uint32 OutputChanneln[12]   =   { 0   ,  0  ,  0   ,   0  ,   0  ,   0  ,   0  ,   0  ,   0  ,   0  ,   0  ,   0  };
    if (ok){
        //First get the list of members so we can check
        MapMember* thisMembers = myMap->GetMembersHL();
        for (uint32 i = 0; i < MAX_IO_SLOTS; i++){
            //Check if the configuration matches to the provided parameters
            ok &= SafeMath::IsEqual(thisMembers[i].defined, DefinedDevices[i]);
            //Check the Input IOMapMember structure for this member
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.defined,   DefinedAsInput[i]);
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.nChannels, InputChanneln[i]);
            ok &= (thisMembers[i].Inputs.buffer.IsValid() == false);            //Buffer reference must be not assigned
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.requestSize, (uint32) 0); //requestSize must not be assigned
            //Check the Output IOMapMember structure for this member
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.defined,   DefinedAsOutput[i]);
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.nChannels, OutputChanneln[i]);
            ok &= (thisMembers[i].Outputs.buffer.IsValid() == false);    //Buffer reference must be not assigned
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.requestSize, (uint32) 0); //requestSize must not be assigned
            //Check that the device number is correct
            ok &= SafeMath::IsEqual(thisMembers[i].devn, (uint8) i);
            //Check that the reference to the device is still uninitialised (as it must just after initialisation)
            ok = (thisMembers[i].reference.IsValid() == false);
            //Check that the IOMap members for each of the members are well configured
        }
    }
    //Now we do check that the MapMembers are put in the required order in the outputMembersOrdered and inputMembersOrdered lists
    if (ok){
        uint8 inputDevices_devn [3] = {3, 1, 7};
        MapMember* thisMembers = myMap->GetMembersHL();
        MapMember** thisInputMembersOrdered = myMap->GetinputMembersOrderedHL();
        for (uint8 i = 0; i < 3; i++){
            ok &= SafeMath::IsEqual(thisInputMembersOrdered[i]->devn, inputDevices_devn[i]);
            //Check that it is actually the pointer to were we think it points, if that is ok, the previous checks certify that
            //the map member will work as presumed
            ok &= SafeMath::IsEqual(thisInputMembersOrdered[i], &(thisMembers[inputDevices_devn[i]]));
        }
    }
    //Check if the I/O channels are assigned to any DataSource
    if (ok){
        ok = SafeMath::IsEqual(myMap->GetoutputAssignedToDSHL(), true);
        ok &= SafeMath::IsEqual(myMap->GetinputAssignedToDSHL(), false);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_NoInputs(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map");
        ok &= cdb.Delete("Inputs");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMapContainerHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        const char8* myMapName = "+Map";
        ok &= SafeMath::IsEqual(*(myMap->GetName()), *myMapName);
    }
    if (ok){
        ok &= SafeMath::IsEqual(myMap->GetnOutputMembersHL(), (uint32) 3);
        ok &= SafeMath::IsEqual(myMap->GetnInputMembersHL(),  (uint32) 0);
    }
    //Check that the devices are properly configured in the map structures
    //First we check if the defined devices are defined in their respective directions.
    bool DefinedDevices[12]     =   {true , true , false, false, false, true , false, false, false, false, false, false};
    bool DefinedAsInput[12]     =   {false, false, false, false, false, false, false, false, false, false, false, false};
    bool DefinedAsOutput[12]    =   {true , true , false, false, false, true , false, false, false, false, false, false};
    uint32 InputChanneln[12]    =   { 0   ,  0  ,  0   ,   0  ,   0  ,   0  ,   0  ,   0  ,   0  ,   0  ,   0  ,   0  };
    uint32 OutputChanneln[12]   =   { 4   ,  2  ,  0   ,   0  ,   0  ,   2  ,   0  ,   0  ,   0  ,   0  ,   0  ,   0  };
    if (ok){
        //First get the list of members so we can check
        MapMember* thisMembers = myMap->GetMembersHL();
        for (uint32 i = 0; i < MAX_IO_SLOTS; i++){
            //Check if the configuration matches to the provided parameters
            ok &= SafeMath::IsEqual(thisMembers[i].defined, DefinedDevices[i]);
            //Check the Input IOMapMember structure for this member
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.defined,   DefinedAsInput[i]);

            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.nChannels, InputChanneln[i]);
            ok &= (thisMembers[i].Inputs.buffer.IsValid() == false);            //Buffer reference must be not assigned
            ok &= SafeMath::IsEqual(thisMembers[i].Inputs.requestSize, (uint32) 0); //requestSize must not be assigned
            //Check the Output IOMapMember structure for this member
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.defined,   DefinedAsOutput[i]);
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.nChannels, OutputChanneln[i]);            
            ok &= (thisMembers[i].Outputs.buffer.IsValid() == false);    //Buffer reference must be not assigned
            ok &= SafeMath::IsEqual(thisMembers[i].Outputs.requestSize, (uint32) 0); //requestSize must not be assigned
            //Check that the device number is correct
            if (DefinedDevices[i]){
                ok &= SafeMath::IsEqual(thisMembers[i].devn, (uint8) i);    //The devn is only set in case the device is used by the map
            }
            //Check that the reference to the device is still uninitialised (as it must just after initialisation)
            ok &= (thisMembers[i].reference.IsValid() == false);
            //Check that the IOMap members for each of the members are well configured
        }
    }
    //Now we do check that the MapMembers are put in the required order in the outputMembersOrdered and inputMembersOrdered lists
    if (ok){
        uint8 outputDevices_devn[3] = {0, 1, 5};
        MapMember* thisMembers = myMap->GetMembersHL();
        MapMember** thisOutputMembersOrdered = myMap->GetoutputMembersOrderedHL();
        for (uint8 i = 0; i < 3; i++){
            ok &= SafeMath::IsEqual(thisOutputMembersOrdered[i]->devn, outputDevices_devn[i]);
            //Check that it is actually the pointer to were we think it points, if that is ok, the previous checks certify that
            //the map member will work as presumed
            ok &= SafeMath::IsEqual(thisOutputMembersOrdered[i], &(thisMembers[outputDevices_devn[i]]));
        }
    }
    //Check if the I/O channels are assigned to any DataSource
    if (ok){
        ok = SafeMath::IsEqual(myMap->GetoutputAssignedToDSHL(), false);
        ok &= SafeMath::IsEqual(myMap->GetinputAssignedToDSHL(), true);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_NoDevices(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map");
        ok &= cdb.Delete("Inputs");
        ok &= cdb.Delete("Outputs");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed without the devices configured
        ok = !god->Initialise(cdb);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_NoDevn(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Outputs.dev0");
        ok &= cdb.Delete("Devn");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed without the devices configured
        ok = !god->Initialise(cdb);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_NegativeDevn(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Outputs.dev0");
        ok &= cdb.Write("Devn", -1);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed without the devices configured
        ok = !god->Initialise(cdb);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_InvalidDevn(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Outputs.dev0");
        ok &= cdb.Write("Devn", 12);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed without the devices configured
        ok = !god->Initialise(cdb);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_RepeatedDevn(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Outputs.dev0");
        ok &= cdb.Write("Devn", 1);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        ok = !god->Initialise(cdb);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_NoChannels(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Inputs.dev3");
        ok &= cdb.Delete("Channels");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        ok = !god->Initialise(cdb);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_NegativeChannels(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Inputs.dev3");
        int32 channels [3] =  {-2,-1,0};
        ok &= cdb.Write("Channels", channels);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        ok = !god->Initialise(cdb);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_DescendingChannels(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Inputs.dev3");
        int32 channels [3] =  {10,5,0};
        ok &= cdb.Write("Channels", channels);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        ok = !god->Initialise(cdb);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_InvalidChannelType(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Inputs.dev3");
        float32 channels [3] = {1.1, 2.3, 4.5};
        ok &= cdb.Write("Channels", channels);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        ok = !god->Initialise(cdb);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_EmptyBlock(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Outputs");
        ok &= cdb.Delete("dev0");
        ok &= cdb.Delete("dev1");
        ok &= cdb.Delete("dev5");
        ok &= cdb.Write("test_param", 0);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        ok = !god->Initialise(cdb);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_TooManyOutputDevices(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Outputs");
        ok &= cdb.Delete("dev0");
        ok &= cdb.Delete("dev1");
        ok &= cdb.Delete("dev5");
        ok &= cdb.Write("test_param1", 0);
        ok &= cdb.Write("test_param2", 0);
        ok &= cdb.Write("test_param3", 0);
        ok &= cdb.Write("test_param4", 0);
        ok &= cdb.Write("test_param5", 0);
        ok &= cdb.Write("test_param6", 0);
        ok &= cdb.Write("test_param7", 0);
        ok &= cdb.Write("test_param8", 0);
        ok &= cdb.Write("test_param9", 0);
        ok &= cdb.Write("test_param10", 0);
        ok &= cdb.Write("test_param11", 0);
        ok &= cdb.Write("test_param12", 0);
        ok &= cdb.Write("test_param13", 0);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        ok = !god->Initialise(cdb);
    }
    return ok;
}

bool UEIMapContainerTest::TestInitialise_TooManyInputDevices(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Inputs");
        ok &= cdb.Delete("dev3");
        ok &= cdb.Delete("dev4");
        ok &= cdb.Delete("dev6");
        ok &= cdb.Write("test_param1", 0);
        ok &= cdb.Write("test_param2", 0);
        ok &= cdb.Write("test_param3", 0);
        ok &= cdb.Write("test_param4", 0);
        ok &= cdb.Write("test_param5", 0);
        ok &= cdb.Write("test_param6", 0);
        ok &= cdb.Write("test_param7", 0);
        ok &= cdb.Write("test_param8", 0);
        ok &= cdb.Write("test_param9", 0);
        ok &= cdb.Write("test_param10", 0);
        ok &= cdb.Write("test_param11", 0);
        ok &= cdb.Write("test_param12", 0);
        ok &= cdb.Write("test_param13", 0);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        ok = !god->Initialise(cdb);
    }
    return ok;
}

bool UEIMapContainerTest::TestDAQHandleSetter() {
    bool ok = true;
    UEIMapContainerHL mapContainer;
    ok &= SafeMath::IsEqual(mapContainer.SetDAQHandle(0), false);
    ok &= SafeMath::IsEqual(mapContainer.SetDAQHandle(-1), false);
    //Check that a valid Handle Id is correctly set
    ok &= SafeMath::IsEqual(mapContainer.SetDAQHandle(1), true);
    ok &= SafeMath::IsEqual(mapContainer.GetDAQ_handleHL(), (int32) 1);
    return ok;
}

bool UEIMapContainerTest::TestTimestampMethods() {
    bool ok = true;
    UEIMapContainerHL mapContainer;
    if (ok){
        //Test the single timestamp method for correct behavior on overflowing counter
        uint64 resultTimestamp;
        uint32 inputTimestamps [6]      = {0x00000100, 0x00000200, 0xFF000000, 0x00000100,  0xFFFFFFFF , 0x00000100 };
        uint64 resultingTimestamps [6]  = {0x00000100, 0x00000200, 0xFF000000, 0x100000100, 0x1FFFFFFFF, 0x200000100};
        uint32 expectedCorrectors [6]   = {      0x00,       0x00,       0x00,        0x01,        0x01,        0x02};
        for (uint32 i = 0; i < 6; i++){
            ok &= SafeMath::IsEqual(mapContainer.GetTimestampHL(inputTimestamps[i] ,resultTimestamp), true);
            ok &= SafeMath::IsEqual(resultingTimestamps[i] , (uint64) resultTimestamp);
            ok &= SafeMath::IsEqual(expectedCorrectors[i] ,  (uint32)mapContainer.GettimestampCorrectorHL());
            ok &= SafeMath::IsEqual(inputTimestamps[i] ,     (uint32)mapContainer.GetlastTimestampHL());
        }
    }
    if (ok){
        //Test the array-input/output implementation for this method
        UEIMapContainerHL mapContainer_arrayTest;
        uint32 timestampArraylength = 6;
        uint64* resultTimestampArray = new uint64[6];
        uint32 inputTimestamps_1     [6]  = {0x00000100,  0x00000200,  0xFF000000,  0x00000100,  0xFFFFFFFF , 0x00000100 };
        uint64 resultingTimestamps_1 [6]  = {0x00000100,  0x00000200,  0xFF000000,  0x100000100, 0x1FFFFFFFF, 0x200000100};
        uint32 inputTimestamps_2     [6]  = {0x00000100,  0x00000200,  0xFF000000,  0x00000100,  0xFFFFFFFF , 0x00000100 };
        uint64 resultingTimestamps_2 [6]  = {0x200000100, 0x200000200, 0x2FF000000, 0x300000100, 0x3FFFFFFFF, 0x400000100};
        uint32 inputTimestamps_3     [6]  = {0x00000100,  0x00000200,  0xFF000000,  0x00000100,  0xFFFFFFFF , 0x00000100 };
        uint64 resultingTimestamps_3 [6]  = {0x400000100, 0x400000200, 0x4FF000000, 0x500000100, 0x5FFFFFFFF, 0x600000100};
        uint32* inputTimestamps [3];
        inputTimestamps[0] = inputTimestamps_1;
        inputTimestamps[1] = inputTimestamps_2;
        inputTimestamps[2] = inputTimestamps_3;
        uint64* resultingTimestamps [3];
        resultingTimestamps[0] = resultingTimestamps_1;
        resultingTimestamps[1] = resultingTimestamps_2;
        resultingTimestamps[2] = resultingTimestamps_3;
        uint32 expectedCorrectors [3] = {0x02, 0x04, 0x06};
        for (uint32 i = 0; i < 3 && ok; i++){
            ok &= SafeMath::IsEqual(mapContainer_arrayTest.GetTimestampHL(inputTimestamps[i], timestampArraylength, resultTimestampArray), true);
            for (uint32 j = 0; j < timestampArraylength; j++){
                ok &= SafeMath::IsEqual(resultTimestampArray[j], resultingTimestamps[i][j]);
            }
            ok &= SafeMath::IsEqual(expectedCorrectors[i], (uint32)mapContainer_arrayTest.GettimestampCorrectorHL()); 
            ok &= SafeMath::IsEqual(inputTimestamps[i][timestampArraylength-1], (uint32)mapContainer_arrayTest.GetlastTimestampHL());
        }

        //Test the array-input/output implementation for this method using UEIBufferPointer
        UEIMapContainerHL mapContainer_arrayTest_pointer;
        UEIBufferPointer myPointer;
        myPointer.SetPointerCharacteristics(0u, sizeof(uint32), timestampArraylength);
        for (uint32 i = 0; i < 3 && ok; i++){
            myPointer.SetHead(reinterpret_cast<uint8*>(inputTimestamps[i]));
            ok &= SafeMath::IsEqual(mapContainer_arrayTest_pointer.GetTimestampHL(myPointer, timestampArraylength, resultTimestampArray), true);
            for (uint32 j = 0; j < timestampArraylength; j++){
                ok &= SafeMath::IsEqual(resultTimestampArray[j], resultingTimestamps[i][j]);
            }
            ok &= SafeMath::IsEqual(expectedCorrectors[i], (uint32)mapContainer_arrayTest_pointer.GettimestampCorrectorHL()); 
            ok &= SafeMath::IsEqual(inputTimestamps[i][timestampArraylength-1], (uint32)mapContainer_arrayTest_pointer.GetlastTimestampHL());
        }
    }
    return ok;
}

bool UEIMapContainerTest::TestDSRegistration() {
    bool ok = true;
    UEIMapContainerHL mapContainer;
    //Check error condition for invalid direction
    ok &= SafeMath::IsEqual(mapContainer.RegisterDS(None), false);
    //Check if the Map is registered into any DS
    ok &= SafeMath::IsEqual(mapContainer.GetoutputAssignedToDSHL(), false);
    ok &= SafeMath::IsEqual(mapContainer.GetinputAssignedToDSHL(), false);
    //Register the Map into a DS as input DS
    ok &= SafeMath::IsEqual(mapContainer.RegisterDS(InputSignals), true);
    ok &= SafeMath::IsEqual(mapContainer.GetinputAssignedToDSHL(), true);
    //Try to re-register inputs for a DS, it must fail
    ok &= SafeMath::IsEqual(mapContainer.RegisterDS(InputSignals), false);
    ok &= SafeMath::IsEqual(mapContainer.GetinputAssignedToDSHL(), true);
    //Perform the same procedure for output signals
    //Register the Map into a DS as input DS
        ok &= SafeMath::IsEqual(mapContainer.GetoutputAssignedToDSHL(), false);
    ok &= SafeMath::IsEqual(mapContainer.RegisterDS(OutputSignals), true);
    ok &= SafeMath::IsEqual(mapContainer.GetoutputAssignedToDSHL(), true);
    //Try to re-register inputs for a DS, it must fail
    ok &= SafeMath::IsEqual(mapContainer.RegisterDS(OutputSignals), false);
    ok &= SafeMath::IsEqual(mapContainer.GetoutputAssignedToDSHL(), true);
    
    return ok;
}

bool UEIMapContainerTest::TestSetDevice() {
    bool ok = true;
    //We do need an initialised MapContainer object to test
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMapContainerHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        const char8* myMapName = "+Map";
        ok &= SafeMath::IsEqual(*(myMap->GetName()), *myMapName);
    }
    //Once initialised, check that the references to the devices are all invalid as no reference linking
    //has been done yet
    MapMember* thisMapMembers;
    if (ok){
        thisMapMembers = myMap->GetMembersHL();
        ok = (thisMapMembers != NULL_PTR(MapMember*));
        //Check that initially the references are not set
        if (ok){
            for (uint32 dev = 0; dev < MAX_IO_SLOTS; dev++){
                ok &= SafeMath::IsEqual(thisMapMembers[dev].reference.IsValid(), false);
            }
        }
    }
    if (ok){
        bool DefinedDevices[12]     =   {true , true, false, true , false, true , false, true , false, false, false, false};
        for (uint32 dev = 0; dev < MAX_IO_SLOTS && ok; dev++){
            ReferenceT<UEIDevice> thisDeviceRef = ReferenceT<UEIDevice>(new UEIDevice);
            ok &= thisDeviceRef.IsValid();
            if (DefinedDevices[dev]){
                //If the device is defined, try to set a reference to a new UEIDevice object by using the SetDevice method
                ok &= myMap->SetDevice(dev, thisDeviceRef);
                //Check that the reference is set and actually points to the same object
                ok &= SafeMath::IsEqual(thisMapMembers[dev].reference.IsValid(), true);
                ok &= (thisMapMembers[dev].reference == thisDeviceRef);
            }else{
                //If the device is not defined in the members then try to set it, but the method must return false to signal an error
                ok &= (!myMap->SetDevice(dev, thisDeviceRef));
                //Check that the reference is not set
                ok &= SafeMath::IsEqual(thisMapMembers[dev].reference.IsValid(), false);
            }
        }
    }
    //Check failure conditions for SetDevice method
    if (ok){
        ReferenceT<UEIDevice> thisDeviceRef = ReferenceT<UEIDevice>(new UEIDevice);
        //at an invalid devn
        ok &= (!myMap->SetDevice(MAX_IO_SLOTS+1, thisDeviceRef));
        //at an invalid devn
        ReferenceT<UEIDevice> invalidRef;
        ok &= SafeMath::IsEqual(invalidRef.IsValid(), false);
        ok &= (!myMap->SetDevice(0, invalidRef));
    }
    return ok;
}

bool UEIMapContainerTest::TestSetDevices() {
    bool ok = true;
    //We do need an initialised MapContainer object to test
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMapContainerHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        const char8* myMapName = "+Map";
        ok &= SafeMath::IsEqual(*(myMap->GetName()), *myMapName);
    }
    //Once initialised, check that the references to the devices are all invalid as no reference linking
    //has been done yet
    MapMember* thisMapMembers;
    if (ok){
        thisMapMembers = myMap->GetMembersHL();
        ok = (thisMapMembers != NULL_PTR(MapMember*));
        //Check that initially the references are not set
        if (ok){
            for (uint32 dev = 0; dev < MAX_IO_SLOTS; dev++){
                ok &= SafeMath::IsEqual(thisMapMembers[dev].reference.IsValid(), false);
            }
        }
    }
    
    if (ok){
        bool DefinedDevices[12]     =   {true , true, false, true , false, true , false, true , false, false, false, false};
        ReferenceT<UEIDevice> referenceList [12];
        //First populate the referenceList with valid (but "dummy" UEIDevice references)
        for (uint32 i = 0; i < 12 && ok; i++){
            if (DefinedDevices[i]){
                referenceList[i] = ReferenceT<UEIDevice>(new UEIDevice);
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), true);
            }else{
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), false);
            }
        }
        //Then set all the references in the produced list into the members of the map
        if (ok){
            ok &= SafeMath::IsEqual(myMap->SetDevices(referenceList), true);
        }
        //Check that the device references have been correctly set
        if (ok){
            for (uint32 dev = 0; dev < 12 && ok; dev++){
                if (thisMapMembers[dev].defined){
                    ok &= SafeMath::IsEqual(thisMapMembers[dev].reference.IsValid(), true);
                    ok &= (thisMapMembers[dev].reference == referenceList[dev]);
                }else{
                    ok &= SafeMath::IsEqual(thisMapMembers[dev].reference.IsValid(), false);
                }
            }
        }
    }
    return ok;
}

bool UEIMapContainerTest::TestSignalAllowed() {
    bool ok = true;
    //We do need an initialised MapContainer object to test
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMapContainerHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        const char8* myMapName = "+Map";
        ok &= SafeMath::IsEqual(*(myMap->GetName()), *myMapName);
    }
    //Once initialised, check that the references to the devices are all invalid as no reference linking
    //has been done yet
    MapMember* thisMapMembers;
    if (ok){
        thisMapMembers = myMap->GetMembersHL();
        ok = (thisMapMembers != NULL_PTR(MapMember*));
        //Check that initially the references are not set
        if (ok){
            for (uint32 dev = 0; dev < MAX_IO_SLOTS; dev++){
                ok &= SafeMath::IsEqual(thisMapMembers[dev].reference.IsValid(), false);
            }
        }
    }
    //At this point test the IsSignalAllowed method. It must fail as no valid references are set for the members of the map to their device object
    if (ok){
        ok &= SafeMath::IsEqual(myMap->IsSignalAllowed(UnsignedInteger8Bit, InputSignals), false);
        ok &= SafeMath::IsEqual(myMap->IsSignalAllowed(UnsignedInteger8Bit, OutputSignals), false);
    }
    //First set the devices references in the MapContainer. For this test a specific device class is implemented as UEITestDevice
    //to avoid relying on a specific hardware device class, therefore UEITestDevice class is used and implemented in this file
    //so uint8 signals are the only allowed type in any direction.
    if (ok){
        bool DefinedDevices[12]     =   {true , true, false, true , false, true , false, true , false, false, false, false};
        ReferenceT<UEIDevice> referenceList [12];
        //First populate the referenceList with valid (but "dummy" UEITestDevice references)
        for (uint32 i = 0; i < 12 && ok; i++){
            if (DefinedDevices[i]){
                referenceList[i] = ReferenceT<UEIDevice>(new UEITestDevice);
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), true);
            }else{
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), false);
            }
        }
        //Then set all the references in the produced list into the members of the map
        if (ok){
            ok &= SafeMath::IsEqual(myMap->SetDevices(referenceList), true);
        }
    }
    //With the devices appropriately try to query for different types of signals
    TypeDescriptor typeList [14]    = {CharString, SignedInteger8Bit, SignedInteger16Bit, SignedInteger32Bit, SignedInteger64Bit, UnsignedInteger8Bit, UnsignedInteger16Bit, UnsignedInteger32Bit, UnsignedInteger64Bit, BooleanType, Float32Bit, Float64Bit, Character8Bit, VoidType}; 
    bool acceptedtype [14]          = {false,false,false,false,false,true,false,false,false,false,false,false,false,false};
    if (ok){
        //Test all the possible types in the two directions, only UnsignedInteger8Bit must succeed in both directions
        // the rest must fail independent of the direction as defined in UEITestDevice
        for (uint32 i = 0; i < 14 && ok; i++){
            ok &= SafeMath::IsEqual(myMap->IsSignalAllowed(typeList[i], InputSignals), acceptedtype[i]);
            ok &= SafeMath::IsEqual(myMap->IsSignalAllowed(typeList[i], OutputSignals), acceptedtype[i]);
        }
    }
    //Finally, test an invalid direction parameter for the error condition
    if (ok){
        ok &= SafeMath::IsEqual(myMap->IsSignalAllowed(UnsignedInteger8Bit, None), false);
    }
    return ok;
}

bool UEIMapContainerTest::TestMapCoherency() {
    bool ok = true;
    //We do need an initialised MapContainer object to test
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMapContainerHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        const char8* myMapName = "+Map";
        ok &= SafeMath::IsEqual(*(myMap->GetName()), *myMapName);
    }
    //First set the devices references in the MapContainer. For this test a specific device class is implemented as UEITestDevice
    //to avoid relying on a specific hardware device class, therefore UEITestDevice class is used and implemented in this file
    //so uint8 signals are the only allowed type in any direction.
    ReferenceT<UEIDevice> referenceList [12]; //in this test this list is "global" as we need to reconfigure the devices directly.
    bool DefinedDevices[12]     =   {true , true, false, true , false, true , false, true , false, false, false, false};
    if (ok){
        //First populate the referenceList with valid (but "dummy" UEITestDevice references)
        for (uint32 i = 0; i < 12 && ok; i++){
            if (DefinedDevices[i]){
                UEITestDevice* thisDevice = new UEITestDevice();
                thisDevice->SetSamplingFrequency(1000.0);
                referenceList[i] = ReferenceT<UEIDevice>(thisDevice);
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), true);
                ok &= SafeMath::IsEqual(referenceList[i]->GetMapAssignment(), false);
            }else{
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), false);
            }
        }
        //Then set all the references in the produced list into the members of the map
        if (ok){
            ok &= SafeMath::IsEqual(myMap->SetDevices(referenceList), true);
        }
    }
    //Check the map coherency, it must return true since all the devices do have the same frequency, references set
    if (ok){
        ok &= SafeMath::IsEqual(myMap->CheckMapCoherency(), true);
    }
    //Now change the frequency of one of the map member reference objects and assign it to devn 0
    if (ok){
        //The references to the device objects must be reinitialised to allow for new coherency check (the current ones are already assigned to the
        //map structure).
        for (uint32 i = 0; i < 12 && ok; i++){
            if (DefinedDevices[i]){
                UEITestDevice* thisDevice = new UEITestDevice();
                thisDevice->SetSamplingFrequency(1000.0);
                referenceList[i] = ReferenceT<UEIDevice>(thisDevice);
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), true);
                ok &= SafeMath::IsEqual(referenceList[i]->GetMapAssignment(), false);
            }else{
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), false);
            }
        }
        if (ok){
            ok &= SafeMath::IsEqual(myMap->SetDevices(referenceList), true);
        }
        //The device 0 will be redefined with a different frequency
        UEITestDevice* thisDevice = new UEITestDevice();
        thisDevice->SetSamplingFrequency(100.0);
        ReferenceT<UEIDevice> faultyReference  = ReferenceT<UEIDevice>(thisDevice);
        //The references to the objects must be 
        ok &= SafeMath::IsEqual(myMap->SetDevice(0 ,faultyReference), true);
        ok &= SafeMath::IsEqual(myMap->CheckMapCoherency(), false);
    }
    //Now check for error condition when a device is already checked by another map object
    if (ok){
        //The references to the device objects must be reinitialised to allow for new coherency check (the current ones are already assigned to the
        //map structure).
        for (uint32 i = 0; i < 12 && ok; i++){
            if (DefinedDevices[i]){
                UEITestDevice* thisDevice = new UEITestDevice();
                thisDevice->SetSamplingFrequency(1000.0);
                referenceList[i] = ReferenceT<UEIDevice>(thisDevice);
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), true);
                ok &= SafeMath::IsEqual(referenceList[i]->GetMapAssignment(), false);
            }else{
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), false);
            }
        }
        if (ok){
            ok &= SafeMath::IsEqual(myMap->SetDevices(referenceList), true);
        }
        //The device 0 will be checked by another map object
        referenceList[0]->SetMapAssignment();
        ok &= SafeMath::IsEqual(myMap->CheckMapCoherency(), false);
    }
    //Now check for error condition when a reference to a device is invalid (not set, empty ReferenceT). To do so we do need to 
    //overwrite the reference directly, as SetDevice and SetDevices methods check for such error condition and will prevent the test
    // from properly set an invalid ReferenceT for an object and checking via CheckMapCoherency
    if (ok){
        //The references to the device objects must be reinitialised to allow for new coherency check (the current ones are already assigned to the
        //map structure).
        for (uint32 i = 0; i < 12 && ok; i++){
            if (DefinedDevices[i]){
                UEITestDevice* thisDevice = new UEITestDevice();
                thisDevice->SetSamplingFrequency(1000.0);
                referenceList[i] = ReferenceT<UEIDevice>(thisDevice);
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), true);
                ok &= SafeMath::IsEqual(referenceList[i]->GetMapAssignment(), false);
            }else{
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), false);
            }
        }
        if (ok){
            ok &= SafeMath::IsEqual(myMap->SetDevices(referenceList), true);
        }
        //The device 0 reference is set to an invalid reference directly from the members access
        MapMember* thisMembers = myMap->GetMembersHL();
        thisMembers[0].reference = ReferenceT<UEIDevice>();
        ok &= SafeMath::IsEqual(myMap->CheckMapCoherency(), false);
    }

    return ok;
}

bool UEIMapContainerTest::TestMapCoherency_InvalidInputChannels() {
    bool ok = true;
    //We do need an initialised MapContainer object to test
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Inputs.dev3");
        uint32 invalidInputChannels [4] = {4, 5, 6, 21};
        ok &= cdb.Write("Channels", invalidInputChannels);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMapContainerHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        const char8* myMapName = "+Map";
        ok &= SafeMath::IsEqual(*(myMap->GetName()), *myMapName);
    }
    //First set the devices references in the MapContainer. For this test a specific device class is implemented as UEITestDevice
    //to avoid relying on a specific hardware device class, therefore UEITestDevice class is used and implemented in this file
    //so uint8 signals are the only allowed type in any direction.
    ReferenceT<UEIDevice> referenceList [12]; //in this test this list is "global" as we need to reconfigure the devices directly.
    bool DefinedDevices[12]     =   {true , true, false, true , false, true , false, true , false, false, false, false};
    if (ok){
        //First populate the referenceList with valid (but "dummy" UEITestDevice references)
        for (uint32 i = 0; i < 12 && ok; i++){
            if (DefinedDevices[i]){
                UEITestDevice* thisDevice = new UEITestDevice();
                thisDevice->SetSamplingFrequency(1000.0);
                referenceList[i] = ReferenceT<UEIDevice>(thisDevice);
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), true);
                ok &= SafeMath::IsEqual(referenceList[i]->GetMapAssignment(), false);
            }else{
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), false);
            }
        }
        //Then set all the references in the produced list into the members of the map
        if (ok){
            ok &= SafeMath::IsEqual(myMap->SetDevices(referenceList), true);
        }
    }
    //Check the map coherency, it must fail due to channels configured in the map being invalid for the UEITestDevice
    //Channels as inputs and above 20 channel number.
    if (ok){
        ok &= SafeMath::IsEqual(myMap->CheckMapCoherency(), false);
    }
    return ok;
}

bool UEIMapContainerTest::TestMapCoherency_InvalidOutputChannels() {
    bool ok = true;
    //We do need an initialised MapContainer object to test
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIMapContainer;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Map.Outputs.dev0");
        uint32 invalidInputChannels [4] = {4, 5, 6, 21};
        ok &= cdb.Write("Channels", invalidInputChannels);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMapContainerHL> myMap;
    if (ok) {
        myMap = ObjectRegistryDatabase::Instance()->Find("Map");
        ok &= myMap.IsValid();
    }
    if (ok){
        const char8* myMapName = "+Map";
        ok &= SafeMath::IsEqual(*(myMap->GetName()), *myMapName);
    }
    //First set the devices references in the MapContainer. For this test a specific device class is implemented as UEITestDevice
    //to avoid relying on a specific hardware device class, therefore UEITestDevice class is used and implemented in this file
    //so uint8 signals are the only allowed type in any direction.
    ReferenceT<UEIDevice> referenceList [12]; //in this test this list is "global" as we need to reconfigure the devices directly.
    bool DefinedDevices[12]     =   {true , true, false, true , false, true , false, true , false, false, false, false};
    if (ok){
        //First populate the referenceList with valid (but "dummy" UEITestDevice references)
        for (uint32 i = 0; i < 12 && ok; i++){
            if (DefinedDevices[i]){
                UEITestDevice* thisDevice = new UEITestDevice();
                thisDevice->SetSamplingFrequency(1000.0);
                referenceList[i] = ReferenceT<UEIDevice>(thisDevice);
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), true);
                ok &= SafeMath::IsEqual(referenceList[i]->GetMapAssignment(), false);
            }else{
                ok &= SafeMath::IsEqual(referenceList[i].IsValid(), false);
            }
        }
        //Then set all the references in the produced list into the members of the map
        if (ok){
            ok &= SafeMath::IsEqual(myMap->SetDevices(referenceList), true);
        }
    }
    //Check the map coherency, it must fail due to channels configured in the map being invalid for the UEITestDevice
    //Channels as inputs and above 20 channel number.
    if (ok){
        ok &= SafeMath::IsEqual(myMap->CheckMapCoherency(), false);
    }
    return ok;
}
