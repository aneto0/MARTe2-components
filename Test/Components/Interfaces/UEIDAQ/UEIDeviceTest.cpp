/**
 * @file UEIDeviceTest.cpp
 * @brief Source file for class UEIDeviceTest
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
 * the class UEIDeviceTest (public, protected, and private). Be aware that some 
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
#include "UEIDeviceTest.h"
#include "UEIDevice.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


class UEITestDeviceHL : public UEIDevice{ 
    public:
        /**
        * @brief Constructs the interface
        */
        UEITestDeviceHL();
        
        /**
        * @brief Destroys the interface
        */
        ~UEITestDeviceHL();
        bool CheckChannelAndDirection(uint32 channelNumber, SignalDirection direction);
        uint32 GetNInputChannelsHL()                                    {return nInputChannels;}
        uint32* GetInputChannelListHL()                                 {return inputChannelList;}
        uint32 GetNOutputChannelsHL()                                   {return nOutputChannels;}
        uint32* GetOutputChannelListHL()                                {return outputChannelList;}      
        uint32 GetDeviceChannels(SignalDirection direction)             {return 3;}  
        bool AnyTypeToBooleanHL(uint32 nSamples, bool* booleanSignal, void* inputSignal, TypeDescriptor signalType) {return AnyTypeToBoolean(nSamples, booleanSignal, inputSignal, signalType);}
        int32 FindChannelIndexHL(uint32 channelNumber, SignalDirection direction) {return FindChannelIndex(channelNumber, direction);}
};

bool UEITestDeviceHL::CheckChannelAndDirection(uint32 channelNumber, SignalDirection direction){
    //For this test, set the accepted direction to only InputSignals
    // and the accepted channels to be 0, 1, 2 and 3
    bool ok = false;
    if (direction == InputSignals){
        if (channelNumber == 0u ||
            channelNumber == 1u ||
            channelNumber == 2u ||
            channelNumber == 3u){
                ok = true;
        }
    }
    return ok;
}

UEITestDeviceHL::UEITestDeviceHL() : UEIDevice() {
        name = "UEITestDeviceHL";
}

UEITestDeviceHL::~UEITestDeviceHL(){
}

const MARTe::char8 * const configUEIDevice = ""
    "+Device = {"
    "    Class = UEIDevice"
    "    Devn = 0"
    "    SamplingFrequency = 1000.0"
    "}";

UEIDeviceTest::UEIDeviceTest(){
}

UEIDeviceTest::~UEIDeviceTest(){
}

bool UEIDeviceTest::TestConstructor() {
    bool ok = true;
    MARTe::UEIDevice device;
    if (ok){
        ok = SafeMath::IsEqual(device.GetDevN(), (uint8) 0xFFu);        
    }
    if (ok){
        ok = SafeMath::IsEqual(device.GetSamplingFrequency(), (float32) 0.0);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.GetHardwareCorrespondence(), false);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.GetMapAssignment(), false);
    }
    return ok;
}

bool UEIDeviceTest::TestBaseMethods() {
    bool ok = true;
    MARTe::UEIDevice device;
    if (ok){
        ok = SafeMath::IsEqual(device.GetModel(), (int32) 0x00);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.GetType(), NO_LAYER);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.GetSampleSize(), (uint8) 0u);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.GetDeviceChannels(InputSignals), (uint32) 0u);
        ok = SafeMath::IsEqual(device.GetDeviceChannels(OutputSignals), (uint32) 0u);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.CheckChannelAndDirection(0u, InputSignals), false);
    }
    if (ok){
        uint32 dummy;
        ok = SafeMath::IsEqual(device.ConfigureChannels(InputSignals, NULL_PTR(uint32**), dummy, NO_MAP), false);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.AcceptedSignalType(TypeDescriptor(UnsignedInteger8Bit)), false);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.GetChannelStatus(0, NULL_PTR(uint32*), NULL_PTR(uint32*)), false);
    }
    if (ok){
        UEIBufferPointer mypointer;
        ok = SafeMath::IsEqual(device.ScaleSignal(0, 0, mypointer, NULL_PTR(void*), Float32Bit), false);
    }
    if (ok){
        UEIBufferPointer myPointer;
        ok = SafeMath::IsEqual(device.ScaleSignal(0, 0, myPointer, NULL_PTR(void*), Float32Bit), false);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.ConfigureDevice(0),false);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.RetrieveInputSignal(0, 0, NULL_PTR(void*), UnsignedInteger8Bit),false);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.SetOutputSignal(0, 0, NULL_PTR(void*), UnsignedInteger8Bit),false);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.InitBuffer(InputSignals, 0, 0, 0), false);
    }
    if (ok){
        uint32 nChannels;
        ok &= SafeMath::IsEqual(device.GetHardwareChannels(InputSignals, nChannels), false);
    }
    return ok;
}

bool UEIDeviceTest::TestHardwareCorrespondence() {
    bool ok = true;
    MARTe::UEIDevice device;
    if (ok){
        //Initially hardware correspondence is not set
        ok = SafeMath::IsEqual(device.GetHardwareCorrespondence(), false);
    }
    if (ok){
        //Set the hardware correspondence
        ok &= SafeMath::IsEqual(device.SetHardwareCorrespondence(), true);
    }
    if (ok){
        //Check the hardware correspondence now
        ok &= SafeMath::IsEqual(device.GetHardwareCorrespondence(), true);
    }
    if (ok){
        //Try to set the hardware correspondence now
        ok &= SafeMath::IsEqual(device.SetHardwareCorrespondence(), false);
    }
    if (ok){
        //Check the hardware correspondence now
        ok &= SafeMath::IsEqual(device.GetHardwareCorrespondence(), true);
    }
    return ok;
}

bool UEIDeviceTest::TestMapAssignment() {
    bool ok = true;
    MARTe::UEIDevice device;
    if (ok){
        //Initially hardware correspondence is not set
        ok = SafeMath::IsEqual(device.GetMapAssignment(), false);
    }
    if (ok){
        //Set the hardware correspondence
        ok &= SafeMath::IsEqual(device.SetMapAssignment(), true);
    }
    if (ok){
        //Check the hardware correspondence now
        ok &= SafeMath::IsEqual(device.GetMapAssignment(), true);
    }
    if (ok){
        //Try to set the hardware correspondence now
        ok &= SafeMath::IsEqual(device.SetMapAssignment(), false);
    }
    if (ok){
        //Check the hardware correspondence now
        ok &= SafeMath::IsEqual(device.GetMapAssignment(), true);
    }
    return ok;
}

bool UEIDeviceTest::TestInitialise(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIDevice> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    return ok;
}

bool UEIDeviceTest::TestInitialise_NoDevn(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        //Remove Devn parameter from device
        ok = cdb.MoveAbsolute("+Device");
        ok &= cdb.Delete("Devn");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = (!god->Initialise(cdb));
    }
    return ok;
}

bool UEIDeviceTest::TestInitialise_InvalidDevn(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        //Remove Devn parameter from device
        ok = cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("Devn", 15);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = (!god->Initialise(cdb));
    }
    return ok;
}

bool UEIDeviceTest::TestInitialise_NoFrequency(){
    bool ok = true;
    //Test initialisation
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        //Remove Devn parameter from device
        ok = cdb.MoveAbsolute("+Device");
        ok &= cdb.Delete("SamplingFrequency");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = (!god->Initialise(cdb));
    }
    return ok;
}

bool UEIDeviceTest::TestCheckChannelAndDirection_List(){
    bool ok = true;
    UEITestDeviceHL testDevice;
    uint32 testChannels [4] = {0,1,2,3};
    ok &= SafeMath::IsEqual(testDevice.CheckChannelListAndDirection(testChannels, 4u, InputSignals), true);
    //Now check for error condition in which list is null pointer
    ok &= SafeMath::IsEqual(testDevice.CheckChannelListAndDirection(NULL_PTR(uint32*), 4u, InputSignals), false);
    //test on invalid direction
    ok &= SafeMath::IsEqual(testDevice.CheckChannelListAndDirection(testChannels, 4u, OutputSignals), false);
    //Check for behavior on invalid channel list
    uint32 failTestChannels [4] = {10,11,12,13};
    ok &= SafeMath::IsEqual(testDevice.CheckChannelListAndDirection(failTestChannels, 4u, InputSignals), false);
    return ok;
}

bool UEIDeviceTest::TestSetChannelList(){
    bool ok = true;
    UEITestDeviceHL myDevice;
    ok &= SafeMath::IsEqual(myDevice.GetNInputChannelsHL(), 0u);
    ok &= SafeMath::IsEqual(myDevice.GetNOutputChannelsHL(), 0u);
    ok &= SafeMath::IsEqual(myDevice.GetInputChannelListHL(), NULL_PTR(uint32*));
    ok &= SafeMath::IsEqual(myDevice.GetOutputChannelListHL(), NULL_PTR(uint32*));
    //Try first to set a well formatted channel list
    uint32 faultyChannels [3] = {0,1,5};
    ok &= SafeMath::IsEqual(myDevice.SetInputChannelList(faultyChannels, 3u), false);
    ok &= SafeMath::IsEqual(myDevice.GetNInputChannelsHL(), 0u);
    ok &= SafeMath::IsEqual(myDevice.GetInputChannelListHL(), NULL_PTR(uint32*));
    //Try a successful channel list
    uint32 channels [3] = {0,1,2};
    ok &= SafeMath::IsEqual(myDevice.SetInputChannelList(channels, 3u), true);
    ok &= SafeMath::IsEqual(myDevice.GetNInputChannelsHL(), 3u);
    ok &= (myDevice.GetInputChannelListHL() != NULL_PTR(uint32*));
    //Try an unordered channel list
    uint32 unorderedChannels [3] = {0,2,1};
    ok &= SafeMath::IsEqual(myDevice.SetInputChannelList(unorderedChannels, 3u), false);
    ok &= SafeMath::IsEqual(myDevice.GetNInputChannelsHL(), 0u);
    ok &= SafeMath::IsEqual(myDevice.GetInputChannelListHL(), NULL_PTR(uint32*));
    //Try an repeated channel list
    uint32 repeatedChannels [3] = {0,1,1};
    ok &= SafeMath::IsEqual(myDevice.SetInputChannelList(repeatedChannels, 3u), false);
    ok &= SafeMath::IsEqual(myDevice.GetNInputChannelsHL(), 0u);
    ok &= SafeMath::IsEqual(myDevice.GetInputChannelListHL(), NULL_PTR(uint32*));
    
    //Now try the same with the output channel method
    //Try first to set a well formatted channel list
    uint32 OutfaultyChannels [3] = {0,1,5};
    ok &= SafeMath::IsEqual(myDevice.SetOutputChannelList(OutfaultyChannels, 3u), false);
    ok &= SafeMath::IsEqual(myDevice.GetNOutputChannelsHL(), 0u);
    ok &= SafeMath::IsEqual(myDevice.GetOutputChannelListHL(), NULL_PTR(uint32*));
    //Try a successful channel list
    uint32 Outchannels [3] = {0,1,2};
    ok &= SafeMath::IsEqual(myDevice.SetOutputChannelList(Outchannels, 3u), true);
    ok &= SafeMath::IsEqual(myDevice.GetNOutputChannelsHL(), 3u);
    ok &= (myDevice.GetOutputChannelListHL() != NULL_PTR(uint32*));
    //Try an unordered channel list
    uint32 OutunorderedChannels [3] = {0,2,1};
    ok &= SafeMath::IsEqual(myDevice.SetOutputChannelList(OutunorderedChannels, 3u), false);
    ok &= SafeMath::IsEqual(myDevice.GetNOutputChannelsHL(), 0u);
    ok &= SafeMath::IsEqual(myDevice.GetOutputChannelListHL(), NULL_PTR(uint32*));
    //Try an repeated channel list
    uint32 OutrepeatedChannels [3] = {0,1,1};
    ok &= SafeMath::IsEqual(myDevice.SetOutputChannelList(OutrepeatedChannels, 3u), false);
    ok &= SafeMath::IsEqual(myDevice.GetNOutputChannelsHL(), 0u);
    ok &= SafeMath::IsEqual(myDevice.GetOutputChannelListHL(), NULL_PTR(uint32*));
    return ok;
}

bool UEIDeviceTest::TestAnyTypeToBoolean(){
    bool ok = true;
    UEITestDeviceHL myDevice;
    //Uint8 test
    if (ok){
        uint8 U8BitSamples [10] = {0,1,2,3,0,4,0,5,6,0};
        bool U8BitSamples_solution [10] = {false, true, true, true, false, true, false, true, true, false};
        bool* result = new bool [10];
        ok &= (myDevice.AnyTypeToBooleanHL(10u, result, reinterpret_cast<void*>(U8BitSamples), UnsignedInteger8Bit), true);
        ok &= (result != NULL_PTR(bool*));
        if (ok){
            for (uint32 i = 0u; i < 10u; i++){
                ok &= SafeMath::IsEqual(result[i], U8BitSamples_solution[i]);
            }
        }
    }
    //Uint16 test
    if (ok){
        uint16 U16BitSamples [10] = {0,1,2,3,0,4,0,5,6,0};
        bool U16BitSamples_solution [10] = {false, true, true, true, false, true, false, true, true, false};
        bool* result = new bool [10];
        ok &= (myDevice.AnyTypeToBooleanHL(10u, result, reinterpret_cast<void*>(U16BitSamples), UnsignedInteger16Bit), true);
        ok &= (result != NULL_PTR(bool*));
        if (ok){
            for (uint32 i = 0u; i < 10u; i++){
                ok &= SafeMath::IsEqual(result[i], U16BitSamples_solution[i]);
            }
        }
    }
    //Uint32 test
    if (ok){
        uint32 U32BitSamples [10] = {0,1,2,3,0,4,0,5,6,0};
        bool U32BitSamples_solution [10] = {false, true, true, true, false, true, false, true, true, false};
        bool* result = new bool [10];
        ok &= (myDevice.AnyTypeToBooleanHL(10u, result, reinterpret_cast<void*>(U32BitSamples), UnsignedInteger32Bit), true);
        ok &= (result != NULL_PTR(bool*));
        if (ok){
            for (uint32 i = 0u; i < 10u; i++){
                ok &= SafeMath::IsEqual(result[i], U32BitSamples_solution[i]);
            }
        }
    }
    //Uint64 test
    if (ok){
        uint64 U64BitSamples [10] = {0,1,2,3,0,4,0,5,6,0};
        bool U64BitSamples_solution [10] = {false, true, true, true, false, true, false, true, true, false};
        bool* result = new bool [10];
        ok &= (myDevice.AnyTypeToBooleanHL(10u, result, reinterpret_cast<void*>(U64BitSamples), UnsignedInteger64Bit), true);
        ok &= (result != NULL_PTR(bool*));
        if (ok){
            for (uint32 i = 0u; i < 10u; i++){
                ok &= SafeMath::IsEqual(result[i], U64BitSamples_solution[i]);
            }
        }
    }
    //Int8 test
    if (ok){
        int8 I8BitSamples [10] = {0,1,2,3,-1,4,0,5,6,-30};
        bool I8BitSamples_solution [10] = {false, true, true, true, false, true, false, true, true, false};
        bool* result = new bool [10];
        ok &= (myDevice.AnyTypeToBooleanHL(10u, result, reinterpret_cast<void*>(I8BitSamples), SignedInteger8Bit), true);
        ok &= (result != NULL_PTR(bool*));
        if (ok){
            for (uint32 i = 0u; i < 10u; i++){
                ok &= SafeMath::IsEqual(result[i], I8BitSamples_solution[i]);
            }
        }
    }
    //Int16 test
    if (ok){
        int16 I16BitSamples [10] = {0,1,2,3,-1,4,0,5,6,-30};
        bool I16BitSamples_solution [10] = {false, true, true, true, false, true, false, true, true, false};
        bool* result = new bool [10];
        ok &= (myDevice.AnyTypeToBooleanHL(10u, result, reinterpret_cast<void*>(I16BitSamples), SignedInteger16Bit), true);
        ok &= (result != NULL_PTR(bool*));
        if (ok){
            for (uint32 i = 0u; i < 10u; i++){
                ok &= SafeMath::IsEqual(result[i], I16BitSamples_solution[i]);
            }
        }
    }
    //Int32 test
    if (ok){
        int32 I32BitSamples [10] = {0,1,2,3,-1,4,0,5,6,-30};
        bool I32BitSamples_solution [10] = {false, true, true, true, false, true, false, true, true, false};
        bool* result = new bool [10];
        ok &= (myDevice.AnyTypeToBooleanHL(10u, result, reinterpret_cast<void*>(I32BitSamples), SignedInteger32Bit), true);
        ok &= (result != NULL_PTR(bool*));
        if (ok){
            for (uint32 i = 0u; i < 10u; i++){
                ok &= SafeMath::IsEqual(result[i], I32BitSamples_solution[i]);
            }
        }
    }
    //Int64 test
    if (ok){
        int64_t I64BitSamples [10] = {0,1,2,3,-1,4,0,5,6,-30};
        bool I64BitSamples_solution [10] = {false, true, true, true, false, true, false, true, true, false};
        bool* result = new bool [10];
        ok &= (myDevice.AnyTypeToBooleanHL(10u, result, reinterpret_cast<void*>(I64BitSamples), SignedInteger64Bit), true);
        ok &= (result != NULL_PTR(bool*));
        if (ok){
            for (uint32 i = 0u; i < 10u; i++){
                ok &= SafeMath::IsEqual(result[i], I64BitSamples_solution[i]);
            }
        }
    }
    //Float32 test
    if (ok){
        float32 F32BitSamples [10] = {-0.1,1.5,2.7,3.3,-5.0,4.1,-0.001,5.2,6.0,-15.0};
        bool F32BitSamples_solution [10] = {false, true, true, true, false, true, false, true, true, false};
        bool* result = new bool [10];
        ok &= (myDevice.AnyTypeToBooleanHL(10u, result, reinterpret_cast<void*>(F32BitSamples), Float32Bit), true);
        ok &= (result != NULL_PTR(bool*));
        if (ok){
            for (uint32 i = 0u; i < 10u; i++){
                ok &= SafeMath::IsEqual(result[i], F32BitSamples_solution[i]);
            }
        }
    }
    //Float64 test
    if (ok){
        float64 F64BitSamples [10] = {-0.1,1.5,2.7,3.3,-5.0,4.1,-0.001,5.2,6.0,-15.0};
        bool F64BitSamples_solution [10] = {false, true, true, true, false, true, false, true, true, false};
        bool* result = new bool [10];
        ok &= (myDevice.AnyTypeToBooleanHL(10u, result, reinterpret_cast<void*>(F64BitSamples), Float64Bit), true);
        ok &= (result != NULL_PTR(bool*));
        if (ok){
            for (uint32 i = 0u; i < 10u; i++){
                ok &= SafeMath::IsEqual(result[i], F64BitSamples_solution[i]);
            }
        }
    }
    ok &= (myDevice.AnyTypeToBooleanHL(10u, NULL_PTR(bool*), NULL_PTR(void*), BooleanType), false);
    return ok;
}

bool UEIDeviceTest::TestFindChannelIndex(){
    bool ok = true;
    UEITestDeviceHL myDevice;
    //Try a successful channel list
    uint32 channels [2] = {0,1};
    ok &= SafeMath::IsEqual(myDevice.SetInputChannelList(channels, 2u), true);
    ok &= SafeMath::IsEqual(myDevice.GetNInputChannelsHL(), 2u);
    ok &= (myDevice.GetInputChannelListHL() != NULL_PTR(uint32*));
    //Try a successful channel list
    uint32 Outchannels [2] = {0,2};
    ok &= SafeMath::IsEqual(myDevice.SetOutputChannelList(Outchannels, 2u), true);
    ok &= SafeMath::IsEqual(myDevice.GetNOutputChannelsHL(), 2u);
    ok &= (myDevice.GetOutputChannelListHL() != NULL_PTR(uint32*));
    //Try the FindChannelIndexHL method
    ok &= SafeMath::IsEqual(myDevice.FindChannelIndexHL(0, InputSignals), (int32) 0);
    ok &= SafeMath::IsEqual(myDevice.FindChannelIndexHL(1, InputSignals), (int32) 1);
    ok &= SafeMath::IsEqual(myDevice.FindChannelIndexHL(2, InputSignals), (int32) -1);
    ok &= SafeMath::IsEqual(myDevice.FindChannelIndexHL(0, OutputSignals), (int32) 0);
    ok &= SafeMath::IsEqual(myDevice.FindChannelIndexHL(1, OutputSignals), (int32) -1);
    ok &= SafeMath::IsEqual(myDevice.FindChannelIndexHL(2, OutputSignals), (int32) 1);
    ok &= SafeMath::IsEqual(myDevice.FindChannelIndexHL(1, None), (int32) -1);
    return ok;
}