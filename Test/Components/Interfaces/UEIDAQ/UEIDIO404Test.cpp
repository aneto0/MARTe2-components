/**
 * @file UEIDIO404Test.cpp
 * @brief Source file for class UEIDIO404Test
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
 * the class UEIDIO404Test (public, protected, and private). Be aware that some 
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
#include "UEIDIO404Test.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class UEIDIO404HL : public UEIDIO404 {
public:
    CLASS_REGISTER_DECLARATION()
    UEIDIO404HL();
    ~UEIDIO404HL();
    float32 GetReferenceVoltageHL()             {return referenceVoltage;}
    float32 GetLowerHysteresisThresholdHL()     {return lowerHysteresisThreshold;}
    float32 GetUpperHysteresisThresholdHL()     {return upperHysteresisThreshold;}
    bool GetHysteresisConfiguredHL()            {return hysteresisConfigured;}
    void SetReferenceVoltageHL(float32 val)     {referenceVoltage = val;}
};
UEIDIO404HL::UEIDIO404HL() : UEIDIO404(){

}
UEIDIO404HL::~UEIDIO404HL(){

}
CLASS_REGISTER(UEIDIO404HL, "1.0")

const MARTe::char8 * const configUEIDevice = ""
"+Device = {"
"    Class = UEIDIO404HL"
"    Devn = 0"
"    VoltageReference = 5"
"    SamplingFrequency = 1000.0"
"}";

UEIDIO404Test::UEIDIO404Test(){
}

UEIDIO404Test::~UEIDIO404Test(){
}

bool UEIDIO404Test::TestConstructor() {
    bool ok = true;
    UEIDIO404HL testDevice;
    ok &= SafeMath::IsEqual(testDevice.GetReferenceVoltageHL(), (float32) 0.0);
    ok &= SafeMath::IsEqual(testDevice.GetLowerHysteresisThresholdHL(), (float32) 0.0);
    ok &= SafeMath::IsEqual(testDevice.GetUpperHysteresisThresholdHL(), (float32) 0.0);
    ok &= SafeMath::IsEqual(testDevice.GetHysteresisConfiguredHL(), false);
    return ok;
}

bool UEIDIO404Test::TestFixedParameters() {
    bool ok = true;
    UEIDIO404 testDevice;
    ok &= SafeMath::IsEqual(testDevice.GetModel(), (int32) 0x404);
    ok &= SafeMath::IsEqual(testDevice.GetType(), HARDWARE_LAYER_DIGITAL_IO);
    ok &= SafeMath::IsEqual(testDevice.GetDeviceChannels(InputSignals), (uint32) 12u);
    ok &= SafeMath::IsEqual(testDevice.GetDeviceChannels(OutputSignals), (uint32) 12u); 
    ok &= SafeMath::IsEqual(testDevice.GetDeviceChannels(None), (uint32) 0u); 
    ok &= SafeMath::IsEqual(testDevice.GetSampleSize(), (uint8) sizeof(uint32));
    uint32 nChannels = 0;
    ok &= SafeMath::IsEqual(testDevice.GetHardwareChannels(InputSignals, nChannels), true);
    ok &= SafeMath::IsEqual(nChannels, (uint32) 1u);
    nChannels = 0;
    ok &= SafeMath::IsEqual(testDevice.GetHardwareChannels(OutputSignals, nChannels), true);
    ok &= SafeMath::IsEqual(nChannels, (uint32) 1u);
    ok &= SafeMath::IsEqual(testDevice.GetHardwareChannels(None, nChannels), false);
    //Test failure of Buffer initialise due to invalid signal direction
    ok &= SafeMath::IsEqual(testDevice.InitBuffer(None, 2u, 1u, 1u), false);
    return ok;
}

bool UEIDIO404Test::TestAcceptedSignalType() {
    bool ok = true;
    UEIDIO404 testDevice;
    TypeDescriptor possibleTypes [14] =   {CharString, SignedInteger8Bit, SignedInteger16Bit, SignedInteger32Bit, 
                                        SignedInteger64Bit, UnsignedInteger8Bit, UnsignedInteger16Bit, UnsignedInteger32Bit, 
                                        UnsignedInteger64Bit, BooleanType, Float32Bit, Float64Bit, Character8Bit, VoidType};
    bool acceptedType [14] = {false, false, false, false, false, true, true, true, true, true, true, true, false, false};
    for (uint32 i = 0; i < 14u && ok; i++){
        ok &= SafeMath::IsEqual(testDevice.AcceptedSignalType(possibleTypes[i]), acceptedType[i]);
    }
    return ok;
}

bool UEIDIO404Test::TestInitialise() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
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
    ReferenceT<UEIDIO404HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //Initialisation must succedd and now we must check that the values entered are the correct ones
    if (ok){
        //Check the configuration values
        ok &= SafeMath::IsEqual(myDevice->GetHysteresisConfiguredHL(), false);
        ok &= SafeMath::IsEqual(myDevice->GetReferenceVoltageHL(), (float32) 5.0);
        ok &= SafeMath::IsEqual(myDevice->GetLowerHysteresisThresholdHL(), (float32) 0.0);
        ok &= SafeMath::IsEqual(myDevice->GetUpperHysteresisThresholdHL(), (float32) 0.0);
    }
    if (ok){
        //Check the sampling frequency is properly configured
        ok &= SafeMath::IsEqual(myDevice->GetSamplingFrequency(), (float32) 1000.0);
    }
    if (ok){
        //Check the device id number
        ok &= SafeMath::IsEqual(myDevice->GetDevN(), (uint8) 0);
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIDIO404Test::TestInitialise_NoVoltageReference() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Delete("VoltageReference");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIDIO404Test::TestInitialise_InvalidVoltageReference() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("VoltageReference", 10.0);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIDIO404Test::TestInitialise_IncompleteHysteresis() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("LowerHysteresys", 0.5);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIDIO404Test::TestInitialise_TooLowHysteresis() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("LowerHysteresys", -0.1);
        ok &= cdb.Write("UpperHysteresys", 3.5);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIDIO404Test::TestInitialise_TooHighHysteresis() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("LowerHysteresys", 0.5);
        ok &= cdb.Write("UpperHysteresys", 5.2);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIDIO404Test::TestInitialise_InvalidHysteresis() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("LowerHysteresys", 3.2);
        ok &= cdb.Write("UpperHysteresys", 1.5);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIDIO404Test::TestInitialise_ValidHysteresis() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("LowerHysteresys", 0.3);
        ok &= cdb.Write("UpperHysteresys", 3.4);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIDIO404HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //Initialisation must succedd and now we must check that the values entered are the correct ones
    if (ok){
        //Check the configuration values
        ok &= SafeMath::IsEqual(myDevice->GetHysteresisConfiguredHL(), true);
        ok &= SafeMath::IsEqual(myDevice->GetReferenceVoltageHL(), (float32) 5.0);
        ok &= SafeMath::IsEqual(myDevice->GetLowerHysteresisThresholdHL(), (float32) 0.3);
        ok &= SafeMath::IsEqual(myDevice->GetUpperHysteresisThresholdHL(), (float32) 3.4);
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIDIO404Test::TestInitialise_class() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("Class", "UEIDIO404");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIDIO404> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIDIO404Test::TestCheckChannelAndDirection() {
    bool ok = true;
    UEIDIO404 device;
    //Check if output signals are allowed for this device
    for (uint32 i = 0; i < 12u && ok; i++){
        ok &= SafeMath::IsEqual(device.CheckChannelAndDirection(i, OutputSignals), true);
    }
    //Check if input signals are allowed
    for (uint32 i = 0; i < 12u && ok; i++){
        ok &= SafeMath::IsEqual(device.CheckChannelAndDirection(i, InputSignals), true);
    }
    //Check if channel numbers above the maximum are valid
    for (uint32 i = 12u; i < 12u+10 && ok; i++){
        ok &= SafeMath::IsEqual(device.CheckChannelAndDirection(i, InputSignals), false);
        ok &= SafeMath::IsEqual(device.CheckChannelAndDirection(i, OutputSignals), false);
    }
    return ok;
}

bool UEIDIO404Test::TestConfigureChannel() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
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
    ReferenceT<UEIDIO404HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //Check error condition when no channel is configured to be retrieved by the device
    if (ok){
        uint32* configurationBitfields = NULL_PTR(uint32*);
        uint32 nConfigurationBitfields = 0;
        ok &= !myDevice->ConfigureChannels(InputSignals, &configurationBitfields, nConfigurationBitfields, RTDMAP);
        ok &= (configurationBitfields == NULL_PTR(uint32*));
        ok &= (nConfigurationBitfields == 0u);
        if (ok){
            free(configurationBitfields);
        }
    }
    //Repeat same procedure for output signals
    if (ok){
        uint32* configurationBitfields = NULL_PTR(uint32*);
        uint32 nConfigurationBitfields = 0;
        ok &= !myDevice->ConfigureChannels(OutputSignals, &configurationBitfields, nConfigurationBitfields, RTDMAP);
        ok &= (configurationBitfields == NULL_PTR(uint32*));
        ok &= (nConfigurationBitfields == 0u);
        if (ok){
            free(configurationBitfields);
        }
    }
    //Test the retrieved configuration bitfields for Input Channels configured
    if (ok){
        //Test the channel configuration functions by trying to set some channels and see the response
        //This device is initialised by setting a 0 for all the I/O channels
        uint32 channels [7] = {0,1,2,3,4,5,6};
        uint32 channelConfiguration = {0x00000000};
        uint32* configurationBitfields = NULL_PTR(uint32*);
        uint32 nConfigurationBitfields = 0;
        //First set the channels we want to retrieve for this device
        ok &= myDevice->SetInputChannelList(channels, 7u);
        ok &= myDevice->ConfigureChannels(InputSignals, &configurationBitfields, nConfigurationBitfields, RTDMAP);
        ok &= (configurationBitfields != NULL_PTR(uint32*));
        ok &= (nConfigurationBitfields == 1u);
        if (ok){
            ok &= SafeMath::IsEqual(configurationBitfields[0], (uint32)channelConfiguration);
        }
        if (ok){
            free (configurationBitfields);
        }
    }
    //Test the retrieved configuration bitfields for Output Channels configured
    if (ok){
        //Test the channel configuration functions by trying to set some channels and see the response
        //This device is initialised by setting a 0 for all the I/O channels
        uint32 channels [7] = {0,1,2,3,4,5,6};
        uint32 channelConfiguration = {0x00000000};
        uint32* configurationBitfields = NULL_PTR(uint32*);
        uint32 nConfigurationBitfields = 0;
        //First set the channels we want to retrieve for this device
        ok &= myDevice->SetOutputChannelList(channels, 7u);
        ok &= myDevice->ConfigureChannels(OutputSignals, &configurationBitfields, nConfigurationBitfields, RTDMAP);
        ok &= (configurationBitfields != NULL_PTR(uint32*));
        ok &= (nConfigurationBitfields == 1u);
        if (ok){
            ok &= SafeMath::IsEqual(configurationBitfields[0], (uint32)channelConfiguration);
        }
        if (ok){
            free (configurationBitfields);
        }
    }
    //Test for invalid direction
    if (ok){
        //Test the channel configuration functions by trying to set some channels and see the response
        //Gains are set to 1,2,4,8,16,32 and 64 by the default configuration in this test 
        uint32* configurationBitfields = NULL_PTR(uint32*);
        uint32 nConfigurationBitfields = 0;
        ok &= !myDevice->ConfigureChannels(None, &configurationBitfields, nConfigurationBitfields, RTDMAP);
        ok &= (configurationBitfields == NULL_PTR(uint32*));
        ok &= (nConfigurationBitfields == 0u);
        if (ok){
            free(configurationBitfields);
        }
    }
    return ok;
}

bool UEIDIO404Test::TestConfigureDevice() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("LowerHysteresys", 0.3);
        ok &= cdb.Write("UpperHysteresys", 3.4);
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIDIO404HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    if (ok){
        //Test that the base method ConfigureDevice works. The test is performed agains the UEIDAQ Mockup, which returns always
        // ok
        ok &= (myDevice->ConfigureDevice(1));
    } 
    if (ok){
        //Test for invalid DAQ handle supplied
        ok &= !(myDevice->ConfigureDevice(-1));
    }
    if (ok){
        //Test for error on DqAdv4040SetHyst call
        manager->SetErrorCode(DqAdv404SetHyst_ERROR);
        ok &= !(myDevice->ConfigureDevice(1));
        manager->resetManager();
    }
    if (ok){
        //Test for internal error on the voltage reference value
        myDevice->SetReferenceVoltageHL(6.0);
        ok &= !(myDevice->ConfigureDevice(1));
    }
    return ok;
}

bool UEIDIO404Test::TestSignalScaling() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
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
    ReferenceT<UEIDIO404HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //This test validates and tests the scale signal method of UEIDIO404 hardware layer software using
    // a UEIBufferPointer object
    //Try it for every channel number with different geins to check validity on gain effect on conversion.
    UEIBufferPointer myPointer;
    myPointer.SetPointerCharacteristics(0, sizeof(uint32), 10u);
    uint32 hardware_readings [6] = {0x65, 0x0B, 0x6A, 0x5C, 0x12, 0x7F}; 
    myPointer.SetHead(reinterpret_cast<uint8*>(hardware_readings));

    //Set the correct responses to each channel
    bool** channelReadings = new bool*[7];
    bool channel0 [6] = {true , true , false, false, false, true };
    bool channel1 [6] = {false, true , true , false, true , true };
    bool channel2 [6] = {true , false, false, true , false, true };
    bool channel3 [6] = {false, true , true , true , false, true };
    bool channel4 [6] = {false, false, false, true , true , true };
    bool channel5 [6] = {true , false, true , false, false, true };
    bool channel6 [6] = {true , false, true , true , false, true };
    channelReadings[0] = channel0;
    channelReadings[1] = channel1;
    channelReadings[2] = channel2;
    channelReadings[3] = channel3;
    channelReadings[4] = channel4;
    channelReadings[5] = channel5;
    channelReadings[6] = channel6;
    //Additionally, to test the RetrieveInputSignal call, we will also copy the data into the inputChannelsBuffer
    // in device memory
    myDevice->InitBuffer(InputSignals, 2u, 6u, 6u);
    ok &= myDevice->inputChannelsBuffer->CheckAvailableSpace(6*sizeof(uint32));
    if (ok){
        memcpy(myDevice->inputChannelsBuffer->writePointer, hardware_readings, 6*4u);
        ok &= myDevice->inputChannelsBuffer->AdvanceBufferIndex(6*4u);
        ok &= myDevice->inputChannelsBuffer->CheckReadReady();
    }
    //Test for uint8 values
    if (ok){
        uint8 destinationAddress [6];
        uint8 retrievedSignal [6];
        //Traverse all the channels
        for (uint32 i = 0; i < 7 && ok; i++){
            ok &= (myDevice->ScaleSignal(i, 6, myPointer, reinterpret_cast<void*>(destinationAddress), UnsignedInteger8Bit));
            ok &= (myDevice->RetrieveInputSignal(i, 6u, reinterpret_cast<void*>(retrievedSignal), UnsignedInteger8Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                ok &= SafeMath::IsEqual(destinationAddress[j], (uint8) channelReadings[i][j]);
                ok &= SafeMath::IsEqual(retrievedSignal[j], (uint8) channelReadings[i][j]);
            }
        }
    }
    
    //Test for uint16 values
    if (ok){
        uint16 destinationAddress [6];
        uint16 retrievedSignal [6];
        //Traverse all the channels
        for (uint32 i = 0; i < 7 && ok; i++){
            ok &= (myDevice->ScaleSignal(i, 6, myPointer, reinterpret_cast<void*>(destinationAddress), UnsignedInteger16Bit));
            ok &= (myDevice->RetrieveInputSignal(i, 6u, reinterpret_cast<void*>(retrievedSignal), UnsignedInteger16Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                ok &= SafeMath::IsEqual(destinationAddress[j], (uint16) channelReadings[i][j]);
                ok &= SafeMath::IsEqual(retrievedSignal[j], (uint16) channelReadings[i][j]);
            }
        }
    }
    
    //Test for uint32 values
    if (ok){
        uint32 destinationAddress [6];
        uint32 retrievedSignal [6];
        //Traverse all the channels
        for (uint32 i = 0; i < 7 && ok; i++){
            ok &= (myDevice->ScaleSignal(i, 6, myPointer, reinterpret_cast<void*>(destinationAddress), UnsignedInteger32Bit));
            ok &= (myDevice->RetrieveInputSignal(i, 6u, reinterpret_cast<void*>(retrievedSignal), UnsignedInteger32Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                ok &= SafeMath::IsEqual(destinationAddress[j], (uint32) channelReadings[i][j]);
                ok &= SafeMath::IsEqual(retrievedSignal[j], (uint32) channelReadings[i][j]);
            }
        }
    }

    //Test for uint64 values
    if (ok){
        uint64 destinationAddress [6];
        uint64 retrievedSignal [6];
        //Traverse all the channels
        for (uint32 i = 0; i < 7 && ok; i++){
            ok &= (myDevice->ScaleSignal(i, 6, myPointer, reinterpret_cast<void*>(destinationAddress), UnsignedInteger64Bit));
            ok &= (myDevice->RetrieveInputSignal(i, 6u, reinterpret_cast<void*>(retrievedSignal), UnsignedInteger64Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                ok &= SafeMath::IsEqual(destinationAddress[j], (uint64) channelReadings[i][j]);
                ok &= SafeMath::IsEqual(retrievedSignal[j], (uint64) channelReadings[i][j]);
            }
        }
    }

    //Test for float32 values
    if (ok){
        float32 destinationAddress [6];
        float32 retrievedSignal [6];
        //Traverse all the channels
        for (uint32 i = 0; i < 7 && ok; i++){
            ok &= (myDevice->ScaleSignal(i, 6, myPointer, reinterpret_cast<void*>(destinationAddress), Float32Bit));
            ok &= (myDevice->RetrieveInputSignal(i, 6u, reinterpret_cast<void*>(retrievedSignal), Float32Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                ok &= SafeMath::IsEqual(destinationAddress[j], (float32) channelReadings[i][j]);
                ok &= SafeMath::IsEqual(retrievedSignal[j], (float32) channelReadings[i][j]);
            }
        }
    }
    
    //Test for float64 values
    if (ok){
        float64 destinationAddress [6];
        float64 retrievedSignal [6];
        //Traverse all the channels
        for (uint32 i = 0; i < 7 && ok; i++){
            ok &= (myDevice->ScaleSignal(i, 6, myPointer, reinterpret_cast<void*>(destinationAddress), Float64Bit));
            ok &= (myDevice->RetrieveInputSignal(i, 6u, reinterpret_cast<void*>(retrievedSignal), Float64Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                ok &= SafeMath::IsEqual(destinationAddress[j], (float64) channelReadings[i][j]);
                ok &= SafeMath::IsEqual(retrievedSignal[j], (float64) channelReadings[i][j]);
            }
        }
    }

    //Test for bool values
    if (ok){
        bool destinationAddress [6];
        bool retrievedSignal [6];
        //Traverse all the channels
        for (uint32 i = 0; i < 7 && ok; i++){
            ok &= (myDevice->ScaleSignal(i, 6, myPointer, reinterpret_cast<void*>(destinationAddress), BooleanType));
            ok &= (myDevice->RetrieveInputSignal(i, 6u, reinterpret_cast<void*>(retrievedSignal), BooleanType));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                ok &= SafeMath::IsEqual(destinationAddress[j], (bool) channelReadings[i][j]);
                ok &= SafeMath::IsEqual(retrievedSignal[j], (bool) channelReadings[i][j]);
            }
        }
    }

    //Test signal scaling call failure due to invalid output type
    if (ok){
        bool retrievedSignal [6];
        ok &= !(myDevice->RetrieveInputSignal(0u, 6u, reinterpret_cast<void*>(retrievedSignal), VoidType));
    }
    return ok;
}

bool UEIDIO404Test::TestOutputSignalSetter() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
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
    ReferenceT<UEIDIO404HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //This test validates and tests the scale signal method of UEIDIO404 hardware layer software using
    // a UEIBufferPointer object
    //Try it for every channel number with different geins to check validity on gain effect on conversion.
    uint32 hardware_outputs [6] = {0xCC, 0x9D, 0xEA, 0xEB, 0x12, 0xC0}; 
    //Set the correct responses to each channel
    bool** channelWrite = new bool*[8];
    bool channel0 [6] = {false, true , false, true , false, false};
    bool channel1 [6] = {false, false, true , true , true , false};
    bool channel2 [6] = {true , true , false, false, false, false};
    bool channel3 [6] = {true , true , true , true , false, false};
    bool channel4 [6] = {false, true , false, false, true , false};
    bool channel5 [6] = {false, false, true , true , false, false};
    bool channel6 [6] = {true , false, true , true , false, true };
    bool channel7 [6] = {true , true , true , true , false, true };
    channelWrite [0] = channel0;
    channelWrite [1] = channel1;
    channelWrite [2] = channel2;
    channelWrite [3] = channel3;
    channelWrite [4] = channel4;
    channelWrite [5] = channel5;
    channelWrite [6] = channel6;
    channelWrite [7] = channel7;
    //Additionally, to test the RetrieveInputSignal call, we will also copy the data into the inputChannelsBuffer
    // in device memory
    ok &= myDevice->InitBuffer(OutputSignals, 2u, 6u, 6u);
    //This specific device needs the buffer to be initialised to 0s.
    ok &= myDevice->outputChannelsBuffer->ZeroNextBytes(6*4);
    //Test for uint8 values
    if (ok){
        uint8* inputSignal = new uint8 [6];
        for (uint32 chan = 0u; chan < 8 && ok; chan++){
            for (uint32 sample = 0; sample < 6 && ok; sample++){
                inputSignal[sample] = channelWrite[chan][sample] ? 1:0;
            }
            ok &= myDevice->SetOutputSignal(chan, 6u, reinterpret_cast<void*>(inputSignal), UnsignedInteger8Bit);
        }
        //With all the signals written into the device, check the contents of the buffer
        //First mark the samples as written into the buffer
        ok &= myDevice->outputChannelsBuffer->AdvanceBufferIndex(6*4);
        uint8 retrievedData [6*4];
        ok &= myDevice->outputChannelsBuffer->ReadBytes(6*4u, retrievedData, false);
        uint32* castedSamples = reinterpret_cast<uint32*>(retrievedData);
        for (uint32 i = 0u; i < 6u && ok; i++){
            ok &= SafeMath::IsEqual(hardware_outputs[i], castedSamples[i]);
        }
    }
    //Repeat the same procedure with different types of signal
    ok &= myDevice->outputChannelsBuffer->ResetBuffer();
    ok &= myDevice->outputChannelsBuffer->ZeroNextBytes(6*4);
    //Test for uint16 values
    if (ok){
        uint16* inputSignal = new uint16 [6];
        for (uint32 chan = 0u; chan < 8 && ok; chan++){
            for (uint32 sample = 0; sample < 6 && ok; sample++){
                inputSignal[sample] = channelWrite[chan][sample] ? 1:0;
            }
            ok &= myDevice->SetOutputSignal(chan, 6u, reinterpret_cast<void*>(inputSignal), UnsignedInteger16Bit);
        }
        //With all the signals written into the device, check the contents of the buffer
        //First mark the samples as written into the buffer
        ok &= myDevice->outputChannelsBuffer->AdvanceBufferIndex(6*4);
        uint8 retrievedData [6*4];
        ok &= myDevice->outputChannelsBuffer->ReadBytes(6*4u, retrievedData, false);
        uint32* castedSamples = reinterpret_cast<uint32*>(retrievedData);
        for (uint32 i = 0u; i < 6u && ok; i++){
            ok &= SafeMath::IsEqual(hardware_outputs[i], castedSamples[i]);
        }
    }
    ok &= myDevice->outputChannelsBuffer->ResetBuffer();
    ok &= myDevice->outputChannelsBuffer->ZeroNextBytes(6*4);
    //Test for uint32 values
    if (ok){
        uint32* inputSignal = new uint32 [6];
        for (uint32 chan = 0u; chan < 8 && ok; chan++){
            for (uint32 sample = 0; sample < 6 && ok; sample++){
                inputSignal[sample] = channelWrite[chan][sample] ? 1:0;
            }
            ok &= myDevice->SetOutputSignal(chan, 6u, reinterpret_cast<void*>(inputSignal), UnsignedInteger32Bit);
        }
        //With all the signals written into the device, check the contents of the buffer
        //First mark the samples as written into the buffer
        ok &= myDevice->outputChannelsBuffer->AdvanceBufferIndex(6*4);
        uint8 retrievedData [6*4];
        ok &= myDevice->outputChannelsBuffer->ReadBytes(6*4u, retrievedData, false);
        uint32* castedSamples = reinterpret_cast<uint32*>(retrievedData);
        for (uint32 i = 0u; i < 6u && ok; i++){
            ok &= SafeMath::IsEqual(hardware_outputs[i], castedSamples[i]);
        }
    }
    ok &= myDevice->outputChannelsBuffer->ResetBuffer();
    ok &= myDevice->outputChannelsBuffer->ZeroNextBytes(6*4);
    //Test for uint64 values
    if (ok){
        uint64* inputSignal = new uint64 [6];
        for (uint32 chan = 0u; chan < 8 && ok; chan++){
            for (uint32 sample = 0; sample < 6 && ok; sample++){
                inputSignal[sample] = channelWrite[chan][sample] ? 1:0;
            }
            ok &= myDevice->SetOutputSignal(chan, 6u, reinterpret_cast<void*>(inputSignal), UnsignedInteger64Bit);
        }
        //With all the signals written into the device, check the contents of the buffer
        //First mark the samples as written into the buffer
        ok &= myDevice->outputChannelsBuffer->AdvanceBufferIndex(6*4);
        uint8 retrievedData [6*4];
        ok &= myDevice->outputChannelsBuffer->ReadBytes(6*4u, retrievedData, false);
        uint32* castedSamples = reinterpret_cast<uint32*>(retrievedData);
        for (uint32 i = 0u; i < 6u && ok; i++){
            ok &= SafeMath::IsEqual(hardware_outputs[i], castedSamples[i]);
        }
    }
    ok &= myDevice->outputChannelsBuffer->ResetBuffer();
    ok &= myDevice->outputChannelsBuffer->ZeroNextBytes(6*4);
    //Test for float32 values
    if (ok){
        float32* inputSignal = new float32 [6];
        for (uint32 chan = 0u; chan < 8 && ok; chan++){
            for (uint32 sample = 0; sample < 6 && ok; sample++){
                inputSignal[sample] = channelWrite[chan][sample] ? 15.2:-2.0;
            }
            ok &= myDevice->SetOutputSignal(chan, 6u, reinterpret_cast<void*>(inputSignal), Float32Bit);
        }
        //With all the signals written into the device, check the contents of the buffer
        //First mark the samples as written into the buffer
        ok &= myDevice->outputChannelsBuffer->AdvanceBufferIndex(6*4);
        uint8 retrievedData [6*4];
        ok &= myDevice->outputChannelsBuffer->ReadBytes(6*4u, retrievedData, false);
        uint32* castedSamples = reinterpret_cast<uint32*>(retrievedData);
        for (uint32 i = 0u; i < 6u && ok; i++){
            ok &= SafeMath::IsEqual(hardware_outputs[i], castedSamples[i]);
        }
    }
    ok &= myDevice->outputChannelsBuffer->ResetBuffer();
    ok &= myDevice->outputChannelsBuffer->ZeroNextBytes(6*4);
    //Test for float64 values
    if (ok){
        float64* inputSignal = new float64 [6];
        for (uint32 chan = 0u; chan < 8 && ok; chan++){
            for (uint32 sample = 0; sample < 6 && ok; sample++){
                inputSignal[sample] = channelWrite[chan][sample] ? 15.2:-2.0;
            }
            ok &= myDevice->SetOutputSignal(chan, 6u, reinterpret_cast<void*>(inputSignal), Float64Bit);
        }
        //With all the signals written into the device, check the contents of the buffer
        //First mark the samples as written into the buffer
        ok &= myDevice->outputChannelsBuffer->AdvanceBufferIndex(6*4);
        uint8 retrievedData [6*4];
        ok &= myDevice->outputChannelsBuffer->ReadBytes(6*4u, retrievedData, false);
        uint32* castedSamples = reinterpret_cast<uint32*>(retrievedData);
        for (uint32 i = 0u; i < 6u && ok; i++){
            ok &= SafeMath::IsEqual(hardware_outputs[i], castedSamples[i]);
        }
    }
    if (ok){
        //With the buffer half full, try to perform a new write through this method, it should fail as there's no available space on the buffer
        float64* inputSignal = new float64 [6];
        ok &= !(myDevice->SetOutputSignal(0u, 6u, reinterpret_cast<void*>(inputSignal), Float64Bit));
        //If we check out the buffer, marking the last samples as read we should be able to write further into the buffer
        ok &= myDevice->outputChannelsBuffer->CheckoutBuffer();
        ok &= (myDevice->SetOutputSignal(0u, 6u, reinterpret_cast<void*>(inputSignal), Float64Bit));
        //Reset the error condition
        ok &= myDevice->outputChannelsBuffer->ResetBuffer();
        ok &= myDevice->outputChannelsBuffer->ZeroNextBytes(6*4);
        //Test failure due to the write of an invalid channel
        ok &= !(myDevice->SetOutputSignal(12u, 6u, reinterpret_cast<void*>(inputSignal), Float64Bit));
    }
    return ok;
}
