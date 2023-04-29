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
        ok = SafeMath::IsEqual(device.ConfigureChannel(0u, dummy), false);
    }
    if (ok){
        int32 dummy;
        ok = SafeMath::IsEqual(device.ConfigureChannel(0u, dummy), false);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.ConfigureDevice(0u), false);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.AcceptedSignalType(TypeDescriptor(UnsignedInteger8Bit)), false);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.GetChannelStatus(0, NULL_PTR(uint32*), NULL_PTR(uint32*)), false);
    }
    if (ok){
        ok = SafeMath::IsEqual(device.ScaleSignal(0, 0, NULL_PTR(void*), NULL_PTR(void*), Float32Bit), false);
    }
    if (ok){
        UEIBufferPointer myPointer;
        ok = SafeMath::IsEqual(device.ScaleSignal(0, 0, myPointer, NULL_PTR(void*), Float32Bit), false);
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