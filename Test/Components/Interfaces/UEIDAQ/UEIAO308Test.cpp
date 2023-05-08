/**
 * @file UEIAO308Test.cpp
 * @brief Source file for class UEIAO308Test
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
 * the class UEIAO308Test (public, protected, and private). Be aware that some 
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
#include "UEIAO308Test.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class UEIAO308HL : public UEIAO308 {
public:
    CLASS_REGISTER_DECLARATION()
    UEIAO308HL();
    ~UEIAO308HL();
    bool ConfigureChannelHL(uint32 channelNumber, uint32& channelConfiguration) {return ConfigureChannel(channelNumber, channelConfiguration);}
};
UEIAO308HL::UEIAO308HL() : UEIAO308(){

}
UEIAO308HL::~UEIAO308HL(){

}
CLASS_REGISTER(UEIAO308HL, "1.0")

const MARTe::char8 * const configUEIDevice = ""
"+Device = {"
"    Class = UEIAO308HL"
"    Devn = 0"
"    SamplingFrequency = 1000.0"
"}";

UEIAO308Test::UEIAO308Test(){
}

UEIAO308Test::~UEIAO308Test(){
}

bool UEIAO308Test::TestConstructor() {
    bool ok = true;
    UEIAO308HL testDevice;
    return ok;
}

bool UEIAO308Test::TestFixedParameters() {
    bool ok = true;
    UEIAO308 testDevice;
    ok &= SafeMath::IsEqual(testDevice.GetModel(), (int32) 0x308);
    ok &= SafeMath::IsEqual(testDevice.GetType(), HARDWARE_LAYER_ANALOG_O);
    ok &= SafeMath::IsEqual(testDevice.GetDeviceChannels(InputSignals), (uint32) 0u);
    ok &= SafeMath::IsEqual(testDevice.GetDeviceChannels(OutputSignals), (uint32) 8u); 
    ok &= SafeMath::IsEqual(testDevice.GetDeviceChannels(None), 0u); 
    ok &= SafeMath::IsEqual(testDevice.GetSampleSize(), (uint8) sizeof(uint16));
    uint32* configurationBitfields;
    uint32 nConfigurationBitfields;
    ok &= SafeMath::IsEqual(testDevice.ConfigureChannels(InputSignals, &configurationBitfields, nConfigurationBitfields, RTDMAP), false);
    ok &= SafeMath::IsEqual(testDevice.ConfigureChannels(None, &configurationBitfields, nConfigurationBitfields, RTDMAP), false);
    UEIBufferPointer emptyPointer;
    ok &= SafeMath::IsEqual(testDevice.ScaleSignal(0u, 1u, emptyPointer, NULL_PTR(void*), BooleanType), false);
    ok &= SafeMath::IsEqual(testDevice.RetrieveInputSignal(0u, 1u, NULL_PTR(void*), BooleanType), false);
    ok &= SafeMath::IsEqual(testDevice.InitBuffer(InputSignals, 2u, 1u, 1u), false);
    ok &= SafeMath::IsEqual(testDevice.InitBuffer(None, 2u, 1u, 1u), false);
    uint32 nChannels = 1u;
    ok &= SafeMath::IsEqual(testDevice.GetHardwareChannels(None, nChannels), false);
    ok &= SafeMath::IsEqual(nChannels, 0u);
    ok &= SafeMath::IsEqual(testDevice.GetHardwareChannels(InputSignals, nChannels), true);
    ok &= SafeMath::IsEqual(nChannels, 0u);    
    return ok;
}

bool UEIAO308Test::TestAcceptedSignalType() {
    bool ok = true;
    UEIAO308 testDevice;
    TypeDescriptor possibleTypes [14] =   {CharString, SignedInteger8Bit, SignedInteger16Bit, SignedInteger32Bit, 
                                        SignedInteger64Bit, UnsignedInteger8Bit, UnsignedInteger16Bit, UnsignedInteger32Bit, 
                                        UnsignedInteger64Bit, BooleanType, Float32Bit, Float64Bit, Character8Bit, VoidType};
    bool acceptedType [14] = {false, false, false, false, false, false, true, true, true, false, true, true, false, false};
    for (uint32 i = 0; i < 14u && ok; i++){
        ok &= SafeMath::IsEqual(testDevice.AcceptedSignalType(possibleTypes[i]), acceptedType[i]);
    }
    return ok;
}

bool UEIAO308Test::TestInitialise() {
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
    ReferenceT<UEIAO308HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //Initialisation must succedd and now we must check that the values entered are the correct ones
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

bool UEIAO308Test::TestInitialise_class() {
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
        ok &= cdb.Write("Class", "UEIAO308");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIAO308> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIAO308Test::TestCheckChannelAndDirection() {
    bool ok = true;
    UEIAO308 device;
    //Check if output signals are allowed for this device
    for (uint32 i = 0; i < 8u && ok; i++){
        ok &= SafeMath::IsEqual(device.CheckChannelAndDirection(i, OutputSignals), true);
    }
    //Check if input signals are allowed
    for (uint32 i = 0; i < 8u && ok; i++){
        ok &= SafeMath::IsEqual(device.CheckChannelAndDirection(i, InputSignals), false);
    }
    //Check if channel numbers above the maximum are valid
    for (uint32 i = 8u; i < 8u+10 && ok; i++){
        ok &= SafeMath::IsEqual(device.CheckChannelAndDirection(i, OutputSignals), false);
    }
    return ok;
}

bool UEIAO308Test::TestConfigureChannel() {
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
    ReferenceT<UEIAO308HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //Check error condition when no channel is configured to be retrieved by the device
    if (ok){
        uint32* configurationBitfields = NULL_PTR(uint32*);
        uint32 nConfigurationBitfields = 0;
        ok &= !myDevice->ConfigureChannels(OutputSignals, &configurationBitfields, nConfigurationBitfields, RTDMAP);
        ok &= (configurationBitfields == NULL_PTR(uint32*));
        ok &= (nConfigurationBitfields == 0u);
        if (ok && configurationBitfields == NULL_PTR(uint32*)){
            free(configurationBitfields);
        }
    }
    if (ok){
        //Test the channel configuration functions by trying to set some channels and see the response
        //Gains are set to 1,2,4,8,16,32 and 64 by the default configuration in this test 
        uint32 channels [7] = {0,1,2,3,4,5,6};
        uint32 channelConfigurations [7] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
        uint32* configurationBitfields = NULL_PTR(uint32*);
        uint32 nConfigurationBitfields = 0;
        //First set the channels we want to retrieve for this device
        ok &= myDevice->SetOutputChannelList(channels, 7u);
        //Check the number of channels actually configured in hardware
        uint32 nChannels;
        ok &= myDevice->GetHardwareChannels(OutputSignals, nChannels);
        ok &= SafeMath::IsEqual(nChannels, 7u);
        ok &= myDevice->GetHardwareChannels(InputSignals, nChannels);
        ok &= SafeMath::IsEqual(nChannels, 0u);

        ok &= myDevice->ConfigureChannels(OutputSignals, &configurationBitfields, nConfigurationBitfields, RTDMAP);
        ok &= (configurationBitfields != NULL_PTR(uint32*));
        ok &= (nConfigurationBitfields == 7u);
        for(uint32 i = 0; i < 7 && ok; i++){
            ok &= SafeMath::IsEqual(configurationBitfields[i], (uint32)channelConfigurations[i]);
        }
        if (ok && configurationBitfields == NULL_PTR(uint32*)){
            free (configurationBitfields);
        }
    }
    //Test for no channel admitted on direction
    if (ok){
        //Test the channel configuration functions by trying to set some channels and see the response
        uint32* configurationBitfields = NULL_PTR(uint32*);
        uint32 nConfigurationBitfields = 0;
        ok &= !myDevice->ConfigureChannels(InputSignals, &configurationBitfields, nConfigurationBitfields, RTDMAP);
        ok &= (configurationBitfields == NULL_PTR(uint32*));
        ok &= (nConfigurationBitfields == 0u);
        if (ok && configurationBitfields == NULL_PTR(uint32*)){
            free(configurationBitfields);
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
        if (ok && configurationBitfields == NULL_PTR(uint32*)){
            free(configurationBitfields);
        }
    }
    return ok;
}

bool UEIAO308Test::TestConfigureDevice() {
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
    ReferenceT<UEIAO308HL> myDevice;
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
        ok &= !(myDevice->ConfigureDevice(-1));
    }   
    return ok;
}

bool UEIAO308Test::TestSetOutputSignal() {
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
    ReferenceT<UEIAO308HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    if (ok){
        //In order to set some signals into the output buffer, first we need to initialise the buffer and
        //set some channels
        uint32 channelList [3] = {0,1,2}; 
        ok &= myDevice->SetOutputChannelList (channelList, 3u);
        if (ok){
            //Initialise the buffer
            ok &= myDevice->InitBuffer(OutputSignals, 2u, 5u, 5u);
            ok &= myDevice->outputChannelsBuffer->CheckAvailableSpace();
        }
        //Now the buffer is ready to be used, there's no need to zero the buffer content as this layer
        //overrides the contents on the buffer after each write
    }
    //Try to set the contents by a uint16 input signal, one for every channel
    if (ok){
        uint16 channel0 [5] = {0x4BD3, 0x8179, 0x1F31, 0x149B, 0xE009};
        uint16 channel1 [5] = {0xD3AB, 0xFD0A, 0x2ED8, 0xE6BA, 0xC5DE};
        uint16 channel2 [5] = {0xFABC, 0xB189, 0x0054, 0x56AB, 0x1234};
        uint16* channels[3];
        channels[0] = channel0;
        channels[1] = channel1;
        channels[2] = channel2;
        for (uint32 i = 0u; i < 3u && ok; i++){
            ok &= myDevice->SetOutputSignal(i, 5u, reinterpret_cast<void*>(channels[i]), UnsignedInteger16Bit);
        }
        //After the buffer has been written into, we do need to signal that we have done so.
        ok &= myDevice->outputChannelsBuffer->AdvanceBufferIndex(3*5*sizeof(uint16));
        ok &= myDevice->outputChannelsBuffer->CheckReadReady();
        //Read the contents of the outputChannelsBuffer to see if it is the same as the input values
        uint8 retrievedData [3*5*sizeof(uint16)];
        if (ok){
            ok &= myDevice->outputChannelsBuffer->ReadBytes(3*5*sizeof(uint16), retrievedData, false);
        }
        //Look at the data retrieved and compare if with the written data
        if (ok){
            uint16* dataU16 = reinterpret_cast<uint16*>(retrievedData);
            for (uint32 sample = 0u; sample < 5u && ok; sample++){
                for (uint32 chan = 0u; chan < 3u && ok; chan++){
                    ok &= SafeMath::IsEqual(dataU16[3*sample+chan], channels[chan][sample]);
                }
            }
        }
        //Reset the buffer to input new data
        ok &= myDevice->outputChannelsBuffer->ResetBuffer();
        ok &= myDevice->outputChannelsBuffer->CheckAvailableSpace();
    }
    //Try to set the contents by a uint32 input signal, one for every channel
    if (ok){
        uint32 channel0 [5] = {0xFABC, 0xB189, 0x0054, 0x56AB, 0x1234};
        uint32 channel1 [5] = {0xD3AB, 0xFD0A, 0x2ED8, 0xE6BA, 0xC5DE};
        uint32 channel2 [5] = {0x4BD3, 0x8179, 0x1F31, 0x149B, 0xE009};
        uint32* channels[3];
        channels[0] = channel0;
        channels[1] = channel1;
        channels[2] = channel2;
        for (uint32 i = 0u; i < 3u && ok; i++){
            ok &= myDevice->SetOutputSignal(i, 5u, reinterpret_cast<void*>(channels[i]), UnsignedInteger32Bit);
        }
        //After the buffer has been written into, we do need to signal that we have done so.
        ok &= myDevice->outputChannelsBuffer->AdvanceBufferIndex(3*5*sizeof(uint16));
        ok &= myDevice->outputChannelsBuffer->CheckReadReady();
        //Read the contents of the outputChannelsBuffer to see if it is the same as the input values
        uint8 retrievedData [3*5*sizeof(uint16)];
        if (ok){
            ok &= myDevice->outputChannelsBuffer->ReadBytes(3*5*sizeof(uint16), retrievedData, false);
        }
        //Look at the data retrieved and compare if with the written data
        if (ok){
            uint16* dataU16 = reinterpret_cast<uint16*>(retrievedData);
            for (uint32 sample = 0u; sample < 5u && ok; sample++){
                for (uint32 chan = 0u; chan < 3u && ok; chan++){
                    ok &= SafeMath::IsEqual(dataU16[3*sample+chan], (uint16) channels[chan][sample]);
                }
            }
        }
        //Reset the buffer to input new data
        ok &= myDevice->outputChannelsBuffer->ResetBuffer();
        ok &= myDevice->outputChannelsBuffer->CheckAvailableSpace();
    }
    //Try to set the contents by a uint64 input signal, one for every channel
    if (ok){
        uint64 channel0 [5] = {0xD3AB, 0xFD0A, 0x2ED8, 0xE6BA, 0xC5DE};
        uint64 channel1 [5] = {0xFABC, 0xB189, 0x0054, 0x56AB, 0x1234};
        uint64 channel2 [5] = {0x4BD3, 0x8179, 0x1F31, 0x149B, 0xE009};
        uint64* channels[3];
        channels[0] = channel0;
        channels[1] = channel1;
        channels[2] = channel2;
        for (uint32 i = 0u; i < 3u && ok; i++){
            ok &= myDevice->SetOutputSignal(i, 5u, reinterpret_cast<void*>(channels[i]), UnsignedInteger64Bit);
        }
        //After the buffer has been written into, we do need to signal that we have done so.
        ok &= myDevice->outputChannelsBuffer->AdvanceBufferIndex(3*5*sizeof(uint16));
        ok &= myDevice->outputChannelsBuffer->CheckReadReady();
        //Read the contents of the outputChannelsBuffer to see if it is the same as the input values
        uint8 retrievedData [3*5*sizeof(uint16)];
        if (ok){
            ok &= myDevice->outputChannelsBuffer->ReadBytes(3*5*sizeof(uint16), retrievedData, false);
        }
        //Look at the data retrieved and compare if with the written data
        if (ok){
            uint16* dataU16 = reinterpret_cast<uint16*>(retrievedData);
            for (uint32 sample = 0u; sample < 5u && ok; sample++){
                for (uint32 chan = 0u; chan < 3u && ok; chan++){
                    ok &= SafeMath::IsEqual(dataU16[3*sample+chan], (uint16) channels[chan][sample]);
                }
            }
        }
        //Reset the buffer to input new data
        ok &= myDevice->outputChannelsBuffer->ResetBuffer();
        ok &= myDevice->outputChannelsBuffer->CheckAvailableSpace();
    }
    //Try to set the contents by a float32 input signal, one for every channel
    if (ok){
        float32 channel0 [5] = {5.0 ,   3.2,    0.86,   -3.1,   -0.23};
        float32 channel1 [5] = {1.2 ,   4.1,    -6.5,   -5.6,    0.24};
        float32 channel2 [5] = {8.6 ,   0.8,    -3.4,    2.8,     9.6};
        float32* channels[3];
        channels[0] = channel0;
        channels[1] = channel1;
        channels[2] = channel2;
        for (uint32 i = 0u; i < 3u && ok; i++){
            ok &= myDevice->SetOutputSignal(i, 5u, reinterpret_cast<void*>(channels[i]), Float32Bit);
        }
        //After the buffer has been written into, we do need to signal that we have done so.
        ok &= myDevice->outputChannelsBuffer->AdvanceBufferIndex(3*5*sizeof(uint16));
        ok &= myDevice->outputChannelsBuffer->CheckReadReady();
        //Read the contents of the outputChannelsBuffer to see if it is the same as the input values
        uint8 retrievedData [3*5*sizeof(uint16)];
        if (ok){
            ok &= myDevice->outputChannelsBuffer->ReadBytes(3*5*sizeof(uint16), retrievedData, false);
        }
        //Look at the data retrieved and compare if with the written data
        if (ok){
            uint16* dataU16 = reinterpret_cast<uint16*>(retrievedData);
            for (uint32 sample = 0u; sample < 5u && ok; sample++){
                for (uint32 chan = 0u; chan < 3u && ok; chan++){
                    ok &= SafeMath::IsEqual(dataU16[3*sample+chan], (uint16) ((uint16)(((channels[chan][sample])+10)*(0xFFFF/20.0)) & 0xFFFF));
                }
            }
        }
        //Reset the buffer to input new data
        ok &= myDevice->outputChannelsBuffer->ResetBuffer();
        ok &= myDevice->outputChannelsBuffer->CheckAvailableSpace();
    }
    //Try to set the contents by a float64 input signal, one for every channel
    if (ok){
        float64 channel0 [5] = {8.6 ,   0.8,    -3.4,    2.8,     9.6};
        float64 channel1 [5] = {5.0 ,   3.2,    0.86,   -3.1,   -0.23};
        float64 channel2 [5] = {1.2 ,   4.1,    -6.5,   -5.6,    0.24};
        float64* channels[3];
        channels[0] = channel0;
        channels[1] = channel1;
        channels[2] = channel2;
        for (uint32 i = 0u; i < 3u && ok; i++){
            ok &= myDevice->SetOutputSignal(i, 5u, reinterpret_cast<void*>(channels[i]), Float64Bit);
        }
        //After the buffer has been written into, we do need to signal that we have done so.
        ok &= myDevice->outputChannelsBuffer->AdvanceBufferIndex(3*5*sizeof(uint16));
        ok &= myDevice->outputChannelsBuffer->CheckReadReady();
        //Read the contents of the outputChannelsBuffer to see if it is the same as the input values
        uint8 retrievedData [3*5*sizeof(uint16)];
        if (ok){
            ok &= myDevice->outputChannelsBuffer->ReadBytes(3*5*sizeof(uint16), retrievedData, false);
        }
        //Look at the data retrieved and compare if with the written data
        if (ok){
            uint16* dataU16 = reinterpret_cast<uint16*>(retrievedData);
            for (uint32 sample = 0u; sample < 5u && ok; sample++){
                for (uint32 chan = 0u; chan < 3u && ok; chan++){
                    ok &= SafeMath::IsEqual(dataU16[3*sample+chan], (uint16) ((uint16)(((channels[chan][sample])+10)*(0xFFFF/20.0)) & 0xFFFF));
                }
            }
        }
        //Reset the buffer to input new data
        ok &= myDevice->outputChannelsBuffer->ResetBuffer();
        ok &= myDevice->outputChannelsBuffer->CheckAvailableSpace();
    }
    //Test failure of the method due to invalid signal type
    if (ok){
        bool failureChannels [5] = {true, true, false, true, false};
        ok &= !myDevice->SetOutputSignal(0u, 5u, reinterpret_cast<void*>(failureChannels), BooleanType);
    }
    //Test failure of the method due to invalid output voltage supplied
    if (ok){
        float64 channel0 [5] = {8.6 ,   -12.0,    -3.4,    2.8,     9.6};
        ok &= !myDevice->SetOutputSignal(0u, 5u, reinterpret_cast<void*>(channel0), Float64Bit);
        float32 channel1 [5] = {8.6 ,   21.0,    -3.4,    2.8,     9.6};
        ok &= !myDevice->SetOutputSignal(0u, 5u, reinterpret_cast<void*>(channel1), Float32Bit);
    }
    return ok;
}