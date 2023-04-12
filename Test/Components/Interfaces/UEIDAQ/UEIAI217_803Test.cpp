/**
 * @file UEIAI217_803Test.cpp
 * @brief Source file for class UEIAI217_803Test
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
 * the class UEIAI217_803Test (public, protected, and private). Be aware that some 
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
#include "UEIAI217_803Test.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class UEIAI217_803HL : public UEIAI217_803 {
public:
    CLASS_REGISTER_DECLARATION()
    UEIAI217_803HL();
    ~UEIAI217_803HL();
    uint16* GetGainsHL() {return gains;}
    int32 GetADCModeHL() {return ADCMode;}
    AI217_803FIRBank GetFIRBankHL(uint32 Idx) {return FIRBanks[Idx];}
};
UEIAI217_803HL::UEIAI217_803HL() : UEIAI217_803(){

}
UEIAI217_803HL::~UEIAI217_803HL(){

}
CLASS_REGISTER(UEIAI217_803HL, "1.0")

const MARTe::char8 * const configUEIDevice = ""
"+Device = {"
"    Class = UEIAI217_803HL"
"    Devn = 0"
"    SamplingFrequency = 1000.0"
"    Gains = {1,2,4,8,16,32,64,1,1,1,1,1,1,1,1,1}"
"    ADCMode = \"DEFAULT\""
"    Filters = {"
"         A = {"
"             Taps = {0,1,0,2,0,3,0,4,0,5,6}"
"          }"
"          B = {"
"              DefaultFilter = 0"
"          }"
"          C = {"
"              DefaultFilter = 1"
"          }"
"          D = {"
"              DefaultFilter = 2"
"          }"
"      }"
"}";

UEIAI217_803Test::UEIAI217_803Test(){
}

UEIAI217_803Test::~UEIAI217_803Test(){
}

bool UEIAI217_803Test::TestConstructor() {
    bool ok = true;
    UEIAI217_803HL testDevice;
    ok &= SafeMath::IsEqual(testDevice.GetGainsHL(), NULL_PTR(uint16*));
    ok &= SafeMath::IsEqual(testDevice.GetADCModeHL(), (int32) DQ_AI217_SET_ADC_DEFAULT);
    for (uint32 i = 0; i < FIR_BANK_NUMBER && ok; i++){
        AI217_803FIRBank FIRBank = testDevice.GetFIRBankHL(i);
        ok &= SafeMath::IsEqual(FIRBank.bankState, BANK_NOT_ENABLED);
        ok &= SafeMath::IsEqual(FIRBank.defaultBankSetting, (uint32) 0);
        ok &= SafeMath::IsEqual(FIRBank.nTaps, (uint32) 0);
        ok &= SafeMath::IsEqual(FIRBank.taps, NULL_PTR(float64*));        
    }
    printf("%d", ok);
    return ok;
}

bool UEIAI217_803Test::TestFixedParameters() {
    bool ok = true;
    UEIAI217_803 testDevice;
    ok &= SafeMath::IsEqual(testDevice.GetModel(), (int32) 0x217);
    ok &= SafeMath::IsEqual(testDevice.GetType(), HARDWARE_LAYER_ANALOG_I);
    ok &= SafeMath::IsEqual(testDevice.GetDeviceChannels(), (uint32) 16u);
    ok &= SafeMath::IsEqual(testDevice.GetSampleSize(), (uint8) sizeof(uint32));
    return ok;
}

bool UEIAI217_803Test::TestAcceptedSignalType() {
    bool ok = true;
    UEIAI217_803 testDevice;
    TypeDescriptor possibleTypes [14] =   {CharString, SignedInteger8Bit, SignedInteger16Bit, SignedInteger32Bit, 
                                        SignedInteger64Bit, UnsignedInteger8Bit, UnsignedInteger16Bit, UnsignedInteger32Bit, 
                                        UnsignedInteger64Bit, BooleanType, Float32Bit, Float64Bit, Character8Bit, VoidType};
    bool acceptedType [14] = {false, false, false, false, false, false, false, true, true, false, true, true, false, false};
    for (uint32 i = 0; i < 14u && ok; i++){
        ok &= SafeMath::IsEqual(testDevice.AcceptedSignalType(possibleTypes[i]), acceptedType[i]);
    }
    return ok;
}

bool UEIAI217_803Test::TestInitialise() {
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
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //Initialisation must succedd and now we must check that the values entered are the correct ones
    if (ok){
        //Check gains entered into the device
        uint16* configuredGains = myDevice->GetGainsHL();
        uint16 gains [16] = {1,2,4,8,16,32,64,1,1,1,1,1,1,1,1,1};
        for (uint32 i = 0; i < 16u && ok; i++){
            ok &= SafeMath::IsEqual(configuredGains[i] ,gains[i]);
        }
    }
    if (ok){
        //Check the sampling frequency is properly configured
        ok &= SafeMath::IsEqual(myDevice->GetSamplingFrequency(), (float32) 1000.0);
    }
    if (ok){
        //Check the device id number
        ok &= SafeMath::IsEqual(myDevice->GetDevN(), (uint8) 0);
    }
    if (ok){
        //Check the ADC mode
        ok &= SafeMath::IsEqual(myDevice->GetADCModeHL(), DQ_AI217_SET_ADC_DEFAULT);
    }
    if (ok){
        //Check FIR banks configuration
        FirBankStatus states [4] = {CUSTOM_FIR_SETTING, DEFAULT_FIR_SETTING, DEFAULT_FIR_SETTING, DEFAULT_FIR_SETTING};
        uint32 defaultFilters [4] = {0,0,1,2};
        uint32 nTaps [4] = {11,0,0,0};
        float64 customTaps[11] = {0,1,0,2,0,3,0,4,0,5,6};
        for (uint32 i = 0; i < FIR_BANK_NUMBER && ok; i++){
            AI217_803FIRBank FIRBank = myDevice->GetFIRBankHL(i);
            ok &= SafeMath::IsEqual(FIRBank.bankState, states[i]);
            ok &= SafeMath::IsEqual(FIRBank.defaultBankSetting, defaultFilters[i]);
            ok &= SafeMath::IsEqual(FIRBank.nTaps, nTaps[i]);
            if (states[i] == CUSTOM_FIR_SETTING){
                ok &= (FIRBank.taps != NULL_PTR(float64*));  
                for (uint32 j = 0; j < nTaps[i] && ok; j++){
                    ok &= SafeMath::IsEqual(FIRBank.taps[j], customTaps[j]);      
                }
            }else{
                ok &= SafeMath::IsEqual(FIRBank.taps, NULL_PTR(float64*));  
            }
        }      
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIAI217_803Test::TestInitialise_class() {
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
        ok &= cdb.Write("Class", "UEIAI217_803");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIAI217_803> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIAI217_803Test::TestInitialise_NoGains() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Delete("Gains");
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

bool UEIAI217_803Test::TestInitialise_InvalidGainNumber() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        uint16 newGains [15] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("Gains", newGains);
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

bool UEIAI217_803Test::TestInitialise_InvalidGains() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        uint16 newGains [16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,35};
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("Gains", newGains);
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

bool UEIAI217_803Test::TestInitialise_ADCModeEnhanced() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("ADCMode", "ENHANCED");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //Initialisation must succedd and now we must check that the values entered are the correct ones
    if (ok){
       ok &= SafeMath::IsEqual(myDevice->GetADCModeHL(), (int32) DQ_AI217_SET_ADC_ENH);
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIAI217_803Test::TestInitialise_ADCModeDefault() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("ADCMode", "DEFAULT");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //Initialisation must succedd and now we must check that the values entered are the correct ones
    if (ok){
       ok &= SafeMath::IsEqual(myDevice->GetADCModeHL(), (int32) DQ_AI217_SET_ADC_DEFAULT);
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIAI217_803Test::TestInitialise_NoADCMode() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Delete("ADCMode");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //Initialisation must succedd and now we must check that the values entered are the correct ones
    if (ok){
       ok &= SafeMath::IsEqual(myDevice->GetADCModeHL(), (int32) DQ_AI217_SET_ADC_DEFAULT);
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIAI217_803Test::TestInitialise_InvalidADCMode() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Write("ADCMode", "OtherMode");
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

bool UEIAI217_803Test::TestInitialise_NoFIRBanks() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device");
        ok &= cdb.Delete("Filters");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //Initialisation must succeed and now we must check that the values entered are the correct ones
    if (ok){
        for (uint32 i = 0; i < FIR_BANK_NUMBER && ok; i++){
            AI217_803FIRBank myBank = myDevice->GetFIRBankHL(i);
            ok &= SafeMath::IsEqual(myBank.bankState, BANK_NOT_ENABLED);
            ok &= SafeMath::IsEqual(myBank.defaultBankSetting, 0u);
            ok &= SafeMath::IsEqual(myBank.nTaps, 0u);
            ok &= SafeMath::IsEqual(myBank.taps, NULL_PTR(float64*));
        } 
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIAI217_803Test::TestInitialise_NoFIRBankDefined() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device.Filters");
        ok &= cdb.Delete("A");
        ok &= cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //Initialisation must succeed and now we must check that the values entered are the correct ones
    if (ok){
        FirBankStatus bankState [4] = {BANK_NOT_ENABLED, DEFAULT_FIR_SETTING, DEFAULT_FIR_SETTING, DEFAULT_FIR_SETTING};
        for (uint32 i = 0; i < FIR_BANK_NUMBER && ok; i++){
            AI217_803FIRBank myBank = myDevice->GetFIRBankHL(i);
            ok &= SafeMath::IsEqual(myBank.bankState, bankState[i]);
        } 
    }
    if (ok) {
        god->Purge();
    }
    return ok;
}

bool UEIAI217_803Test::TestInitialise_ManyFIRTaps() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        uint16 newTaps [MAX_FIR_TAPS_803+1];
        ok &= cdb.MoveAbsolute("+Device.Filters.A");
        ok &= cdb.Write("Taps", newTaps);
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

bool UEIAI217_803Test::TestInitialise_InvalidDefaultSetting() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        ok &= cdb.MoveAbsolute("+Device.Filters.B");
        ok &= cdb.Write("DefaultFilter", 10u);
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

bool UEIAI217_803Test::TestInitialise_InvalidFIRtSetting_CustomAndDefault() {
    bool ok = true;
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = configUEIDevice;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    if (ok){
        uint16 newTaps [10];
        ok &= cdb.MoveAbsolute("+Device.Filters.B");
        ok &= cdb.Write("Taps", newTaps);
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

bool UEIAI217_803Test::TestCheckChannelAndDirection() {
    bool ok = true;
    UEIAI217_803 device;
    //Check if output signals are allowed for this device
    for (uint32 i = 0; i < CHANNEL_NUMBER && ok; i++){
        ok &= SafeMath::IsEqual(device.CheckChannelAndDirection(i, OutputSignals), false);
    }
    //Check if input signals are allowed
    for (uint32 i = 0; i < CHANNEL_NUMBER && ok; i++){
        ok &= SafeMath::IsEqual(device.CheckChannelAndDirection(i, InputSignals), true);
    }
    //Check if channel numbers above the maximum are valid
    for (uint32 i = CHANNEL_NUMBER; i < CHANNEL_NUMBER+10 && ok; i++){
        ok &= SafeMath::IsEqual(device.CheckChannelAndDirection(i, InputSignals), false);
    }
    return ok;
}

bool UEIAI217_803Test::TestConfigureChannel() {
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
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //Test the channel configuration functions by trying to set some channels and see the response
    //Gains are set to 1,2,4,8,16,32 and 64 by the default configuration in this test 
    uint32 channels [7] = {0,1,2,3,4,5,6};
    uint32 channelConfigurations [7] = {0x8000,0x8101, 0x8202, 0x8303, 0x8404, 0x8505, 0x8606};
    if(ok){
        for(uint32 i = 0; i < 7 && ok; i++){
            uint32 channelConfigUint;
            int32 channelConfigInt;
            ok &= (myDevice->ConfigureChannel(channels[i], channelConfigUint));
            ok &= (myDevice->ConfigureChannel(channels[i], channelConfigInt));
            ok &= SafeMath::IsEqual(channelConfigUint, (uint32)channelConfigurations[i]);
            ok &= SafeMath::IsEqual(channelConfigInt, (int32)channelConfigurations[i]);
        }
    }
    //Check error condition by setting invalid channel number or invalid gain parameter
    if (ok){
       uint32 channelConfigUint;
       ok &= SafeMath::IsEqual(myDevice->ConfigureChannel(CHANNEL_NUMBER+1, channelConfigUint), false); 
       //Set an invalid gain
       uint16* myGains =  myDevice->GetGainsHL();
       myGains[0] = 3; //Invalid gain for the device
       ok &= SafeMath::IsEqual(myDevice->ConfigureChannel(0u, channelConfigUint), false); 
    }

    return ok;
}

bool UEIAI217_803Test::TestSignalScaling() {
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
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    //This test validates and tests the scale signal method of UEIAI217_803 hardware layer software using
    // a UEIBufferPointer object
    uint32 channelN [7] = {0,1,2,3,4,5,6};
    float32 channelGain [7] = {1,2,4,8,16,32,64};
    //Try it for every channel number with different geins to check validity on gain effect on conversion.
    UEIBufferPointer myPointer;
    myPointer.SetPointerCharacteristics(0, sizeof(uint32), 10u);
    uint32 hardware_readings [6] = {0x11800000, 0x22000000, 0x33FFFFFF, 0x44801000, 0x55400230, 0x665F789F}; 
    myPointer.SetHead(reinterpret_cast<uint8*>(hardware_readings));
    //Test for uint32 values
    if (ok){
        uint32 destinationAddress [6];
        uint32 destinationAddress_ [6];
        //Traverse all the channels
        for (uint32 i = 0; i < 7 && ok; i++){
            ok &= (myDevice->ScaleSignal(i, 6, myPointer, reinterpret_cast<void*>(destinationAddress),UnsignedInteger32Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                ok &= SafeMath::IsEqual(destinationAddress[j], (hardware_readings[j]&0xFFFFFF));
            }
            ok &= (myDevice->ScaleSignal(i, 6, reinterpret_cast<void*>(hardware_readings), reinterpret_cast<void*>(destinationAddress_), UnsignedInteger32Bit));
            for (uint32 j = 0; j < 6 && ok; j++){
                ok &= SafeMath::IsEqual(destinationAddress_[j], (hardware_readings[j]&0xFFFFFF));
            }
        }
    }
    //Test for uint64 values
    if (ok){
        uint64 destinationAddress [6];
        uint64 destinationAddress_ [6];
        //Traverse all the channels
        for (uint32 i = 0; i < 7 && ok; i++){
            ok &= (myDevice->ScaleSignal(i, 6, myPointer, reinterpret_cast<void*>(destinationAddress), UnsignedInteger64Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                ok &= SafeMath::IsEqual(destinationAddress[j], (uint64) (hardware_readings[j]&0xFFFFFF));
            }
            ok &= (myDevice->ScaleSignal(i, 6, reinterpret_cast<void*>(hardware_readings), reinterpret_cast<void*>(destinationAddress_), UnsignedInteger64Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                ok &= SafeMath::IsEqual(destinationAddress_[j], (uint64) (hardware_readings[j]&0xFFFFFF));
            }
        }
    }
    //Test for float32 values
    if (ok){
        float32 destinationAddress [6];
        float32 destinationAddress_ [6];
        //Traverse all the channels
        for (uint32 i = 0; i < 7 && ok; i++){
            ok &= (myDevice->ScaleSignal(i, 6, myPointer, reinterpret_cast<void*>(destinationAddress), Float32Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                float32 expectedValue = ((20.0/channelGain[i])*((float32)(hardware_readings[j]&0xFFFFFF))/(float32)(0xFFFFFF))-((10.0/channelGain[i]));
                ok &= SafeMath::IsEqual(destinationAddress[j], expectedValue);
            }
            ok &= (myDevice->ScaleSignal(i, 6, reinterpret_cast<void*>(hardware_readings), reinterpret_cast<void*>(destinationAddress_), Float32Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                float32 expectedValue = ((20.0/channelGain[i])*((float32)(hardware_readings[j]&0xFFFFFF))/(float32)(0xFFFFFF))-((10.0/channelGain[i]));
                ok &= SafeMath::IsEqual(destinationAddress_[j], expectedValue);
            }
        }
    }
    //Test for float64 values
    if (ok){
        float64 destinationAddress [6];
        float64 destinationAddress_ [6];
        //Traverse all the channels
        for (uint32 i = 0; i < 7 && ok; i++){
            ok &= (myDevice->ScaleSignal(i, 6, myPointer, reinterpret_cast<void*>(destinationAddress), Float64Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                float64 expectedValue = ((20.0/channelGain[i])*((float64)(hardware_readings[j]&0xFFFFFF))/(float64)(0xFFFFFF))-((10.0/channelGain[i]));
                ok &= SafeMath::IsEqual(destinationAddress[j], expectedValue);
            }
            ok &= (myDevice->ScaleSignal(i, 6, reinterpret_cast<void*>(hardware_readings), reinterpret_cast<void*>(destinationAddress_), Float64Bit));
            //Check the validity of the retrieved data
            for (uint32 j = 0; j < 6 && ok; j++){
                float64 expectedValue = ((20.0/channelGain[i])*((float64)(hardware_readings[j]&0xFFFFFF))/(float64)(0xFFFFFF))-((10.0/channelGain[i]));
                ok &= SafeMath::IsEqual(destinationAddress_[j], expectedValue);
            }
        }
    }
    //Test fail cases
    if (ok){
        //NULL_PTR
        uint8 thisValue = 0;
        uint8* destinationAddress = &thisValue;
        UEIBufferPointer faultyPointer;
        ok &= SafeMath::IsEqual(myDevice->ScaleSignal(0, 1, faultyPointer, reinterpret_cast<void*>(destinationAddress), Float64Bit), false);
        ok &= SafeMath::IsEqual(myDevice->ScaleSignal(0, 1, myPointer, reinterpret_cast<void*>(NULL_PTR(uint8*)), Float64Bit), false);
        ok &= SafeMath::IsEqual(myDevice->ScaleSignal(0, 1, reinterpret_cast<void*>(NULL_PTR(uint8*)), reinterpret_cast<void*>(destinationAddress), Float64Bit),false);
        ok &= SafeMath::IsEqual(myDevice->ScaleSignal(0, 1, reinterpret_cast<void*>(destinationAddress), reinterpret_cast<void*>(NULL_PTR(uint8*)), Float64Bit),false);
        //The input and output pointers must not be equal
        ok &= SafeMath::IsEqual(myDevice->ScaleSignal(0, 1, reinterpret_cast<void*>(destinationAddress), reinterpret_cast<void*>(destinationAddress), Float64Bit),false);
        faultyPointer.SetHead(reinterpret_cast<uint8*>(destinationAddress));
        ok &= SafeMath::IsEqual(myDevice->ScaleSignal(0, 1, faultyPointer, reinterpret_cast<void*>(destinationAddress), Float64Bit),false);
        //Channel number must not be over maximum
        ok &= SafeMath::IsEqual(myDevice->ScaleSignal(CHANNEL_NUMBER+1, 1, myPointer, reinterpret_cast<void*>(destinationAddress), Float64Bit),false);
        uint8 thatValue = 0;
        uint8* sourceAddress = &thatValue;
        ok &= SafeMath::IsEqual(myDevice->ScaleSignal(CHANNEL_NUMBER+1, 1, reinterpret_cast<void*>(sourceAddress), reinterpret_cast<void*>(destinationAddress), Float64Bit),false);
        //Invalid output type supplied
        ok &= SafeMath::IsEqual(myDevice->ScaleSignal(0, 1, reinterpret_cast<void*>(sourceAddress), reinterpret_cast<void*>(destinationAddress), UnsignedInteger8Bit),false);
        ok &= SafeMath::IsEqual(myDevice->ScaleSignal(0, 1, myPointer, reinterpret_cast<void*>(destinationAddress), UnsignedInteger8Bit),false);
        //Test the case of an invalid gain supplied and filtered previously
        myDevice->GetGainsHL()[0] = 3;
        ok &= SafeMath::IsEqual(myDevice->ScaleSignal(0, 1, reinterpret_cast<void*>(sourceAddress), reinterpret_cast<void*>(destinationAddress), Float64Bit),false);
        ok &= SafeMath::IsEqual(myDevice->ScaleSignal(0, 1, myPointer, reinterpret_cast<void*>(destinationAddress), Float64Bit),false);
    }   
    return ok;
}

bool UEIAI217_803Test::TestConfigureDevice() {
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
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    if (ok){
        //Test that the base method ConfigureDevice works. The test is performed agains the UEIDAQ Mockup, which returns always
        // ok
        ok &= (myDevice->ConfigureDevice(1));
    }   
    return ok;
}

bool UEIAI217_803Test::TestConfigureDevice_SetCfgLayerError() {
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
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    if (ok){
        UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
        IOM* myIOM;
        ok &= (manager->GetIOM(1, &myIOM));
        if (ok){
            myIOM->SetErrorCode(DqAdv217SetCfgLayer_ERROR);
            //Test that the base method ConfigureDevice works. The test is performed agains the UEIDAQ Mockup, which returns always
            // ok
            ok &= !(myDevice->ConfigureDevice(1));
        }
    }   
    return ok;
}

bool UEIAI217_803Test::TestConfigureDevice_SetDefaultError() {
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
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    if (ok){
        UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
        IOM* myIOM;
        ok &= (manager->GetIOM(1, &myIOM));
        if (ok){
            myIOM->SetErrorCode(DqAdv217SetFIR_ERROR);
            //Test that the base method ConfigureDevice works. The test is performed agains the UEIDAQ Mockup, which returns always
            // ok
            ok &= !(myDevice->ConfigureDevice(1));
        }
    }   
    return ok;
}

bool UEIAI217_803Test::TestConfigureDevice_SetBank1Error() {
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
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    if (ok){
        UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
        IOM* myIOM;
        ok &= (manager->GetIOM(1, &myIOM));
        if (ok){
            myIOM->SetErrorCode(DqAdv217SetFIR_WrongBank1);
            //Test that the base method ConfigureDevice works. The test is performed agains the UEIDAQ Mockup, which returns always
            // ok
            ok &= !(myDevice->ConfigureDevice(1));
        }
    }   
    return ok;
}

bool UEIAI217_803Test::TestConfigureDevice_FIRTapsWarning() {
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
    ReferenceT<UEIAI217_803HL> myDevice;
    if (ok) {
        myDevice = ObjectRegistryDatabase::Instance()->Find("Device");
        ok &= myDevice.IsValid();
    }
    if (ok){
        UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
        IOM* myIOM;
        ok &= (manager->GetIOM(1, &myIOM));
        if (ok){
            myIOM->SetErrorCode(DqAdv217SetFIR_InvalidTaps);
            //Test that the base method ConfigureDevice works. The test is performed agains the UEIDAQ Mockup, which returns always
            // ok
            ok &= (myDevice->ConfigureDevice(1));
        }
    }   
    return ok;
}