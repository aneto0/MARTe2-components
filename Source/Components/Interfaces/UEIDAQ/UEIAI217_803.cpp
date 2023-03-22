/**
 * @file UEIAI217_803.cpp
 * @brief Source file for class UEIAI217_803
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
 * the class UEIAI217_803 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEIAI217_803.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

UEIAI217_803::UEIAI217_803() : UEIDevice() {
}

UEIAI217_803::~UEIAI217_803(){
    
    //Destroy the FIRBanks strcutures
}

bool UEIAI217_803::Initialise(StructuredDataI &data){
    //Initialise the UEIDevice Object, this method will initialise and check base parameters for the device (devn, name and sampling frequency)
    bool ok = UEIDevice::Initialise(data);
    //Initialise helper for retriecing the input parameters
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Read and validate the gains
    if(ok){
        uint32 numberOfGains;
        bool found = helper.ReadArray("Gains", gains, numberOfGains);
        ok = (numberOfGains <= CHANNEL_NUMBER);
        if (ok){
            if (numberOfGains == 0u || !found){
                gains = (uint16*)malloc(2*CHANNEL_NUMBER);
                for (uint8 i = 0u; i < CHANNEL_NUMBER; i++){
                    gains[i] = 1u;
                }
                REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
                "No gain information was supplied for channels at device %s. All channels configured with gain 1.", name.Buffer());
            }else{
                ok = (numberOfGains == CHANNEL_NUMBER);
                if (ok){
                    for (uint8 i = 0u; i < CHANNEL_NUMBER && ok; i++){
                        if (!(  gains[i] == 1 || gains[i] == 2 ||
                                gains[i] == 4 || gains[i] == 8 ||
                                gains[i] == 16|| gains[i] == 32||
                                gains[i] == 64))
                        {
                            ok = false;
                            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                            "Invalid gain supplied for device %s. (Gain in position %d)", name.Buffer(), i);                               
                        }
                    }   
                }else{
                    REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                    "Incomplete gains information for device %s.", name.Buffer());
                }
            }
        }else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
            "Too many gains defined for channels at device %s. Maximum channel number = %d.", name.Buffer(), CHANNEL_NUMBER);
        }
    }
    //Read and choose ADC Mode (DEFAULT mode by default)
    if (ok){
        StreamString ADC_mode_string;
        bool adcModeFound = data.Read("ADC_mode", ADC_mode_string);
        if (adcModeFound){
            if (ADC_mode_string == StreamString("ENHANCED")){
                ADCMode = DQ_AI217_SET_ADC_ENH;
                REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
                "Set ADC_mode to ENHANCED for device %s.", name.Buffer());
            }else if (ADC_mode_string == StreamString("DEFAULT")){
                ADCMode = DQ_AI217_SET_ADC_DEFAULT;
                REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
                "Set ADC_mode to DEFAULT for device %s.", name.Buffer());
            }else{
                REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                "Invalid ADC_mode provided for device %s.", name.Buffer());
                ok = false;  
            }
        }else{
           ADCMode = DQ_AI217_SET_ADC_DEFAULT;
           REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
           "Set ADC_mode to DEFAULT for device %s.", name.Buffer());
        }
    }
    //Read and validate FIR taps (optional parameters)
    if (ok){
        bool requestedFilters = data.MoveRelative("Filters");
        if (!requestedFilters){
            REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
            "No FIR filters specified for device %s, banks A,B,C and D deactivated.", name.Buffer());
            //Set the FIR bank configuration structures to not enabled
            for (uint32 i = 0; i < FIR_BANK_NUMBER; i++){
                FIRBanks[i].bankState = BANK_NOT_ENABLED;
            }      
        }else{
            //Filters structure is found, go through the different banks settings
            StreamString banks [FIR_BANK_NUMBER] = {"A","B","C","D"};
            //loop over the four different FIR banks structure
            for (uint32 i = 0; i < FIR_BANK_NUMBER && ok; i++){
                //Loop to setup a single FIR bank (A,B,C or D)
                bool bankDefined = data.MoveRelative(banks[i].Buffer());
                if (bankDefined){
                    //This bank is defined, search the parameters to configure the bank
                    //Check wether we wish to configure it as default or custom configuration
                    bool defaultConfiguration = data.Read("Default_filter", FIRBanks[i].defaultBankSetting);
                    bool customConfiguration = helper.ReadArray("Taps", FIRBanks[i].taps, FIRBanks[i].nTaps);
                    ok = (defaultConfiguration != customConfiguration);
                    if (ok){
                        if (customConfiguration){
                            FIRBanks[i].bankState = CUSTOM_FIR_SETTING;
                            ok = FIRBanks[i].nTaps <= MAX_FIR_TAPS_803;
                            if (!ok){
                                REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                                "Invalid FIR bank %s configuration for device %s, the number of taps must be equal or lower than %d.", banks[i].Buffer() ,name.Buffer(), MAX_FIR_TAPS_803);     
                            }else{
                                REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
                                "FIR bank %s for device %s will be enabled as custom FIR filter.", banks[i].Buffer() ,name.Buffer());
                            }
                        }else if (defaultConfiguration){
                            FIRBanks[i].bankState = DEFAULT_FIR_SETTING;
                            ok = (FIRBanks[i].defaultBankSetting <= 9);     //Possible Default FIR settings as defined in documentation for this layer
                            if (!ok){
                                REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                                "Invalid FIR bank %s configuration for device %s, the default FIR configuration index must be in 0-9 range.", banks[i].Buffer() ,name.Buffer());     
                            }else{
                                REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
                                "FIR bank %s for device %s will be enabled as default FIR filter (setting %d).", banks[i].Buffer() ,name.Buffer(), FIRBanks[i].defaultBankSetting);
                            }                      
                        }
                    }else{
                       REPORT_ERROR(ErrorManagement::InitialisationError, "UEIAI217_803::Initialise - "
                        "Invalid FIR bank %s configuration for device %s, a bank must contain either a custom or default configuration.", banks[i].Buffer() ,name.Buffer()); 
                    }
                    ok &= data.MoveToAncestor(1u);
                }else{
                    FIRBanks[i].bankState = BANK_NOT_ENABLED;
                    REPORT_ERROR(ErrorManagement::Information, "UEIAI217_803::Initialise - "
                    "FIR bank %s not defined for device %s, this bank will be disabled.", banks[i].Buffer() ,name.Buffer());
                }
            }
            //Return to the device node
            ok &= data.MoveToAncestor(1u);
        }
    }
    return ok;
}

bool UEIAI217_803::CheckChannelAndDirection(uint32 channelIdx, uint8 direction){
    //For AI-217-803 all the channels (0 to 15 as only 16 channels are available) are inputs
    bool validChannel = false;
    //Channel only valid if it is configured as an output
    if (direction == OUTPUT_CHANNEL){
        //Channel only valid if within range [0,15]
        if (channelIdx>=0u && channelIdx<16){
            validChannel = true;
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError, "UEIMasterObject::Initialise - "
            "AI-217-803 layer only accepts channels 0 to 15.");
        }
    }else{
        REPORT_ERROR(ErrorManagement::ParametersError, "UEIMasterObject::Initialise - "
        "AI-217-803 layer only accepts output signals!");
    }
    return validChannel;
}

bool UEIAI217_803::ConfigureDevice(int32 DAQ_handle){
    bool ok = true;
    //Configure all the channels in the layer at the same time (for now no custom channel configuration is allowed)
    ok = (DqAdv217SetCfgLayer(DAQ_handle, deviceId, DQ_AI217_SETCFG_ALL_CHAN, DQ_AI217_SET_CFG_LAYER_ADC, ADCMode) >= 0);
    if (!ok){
        REPORT_ERROR(ErrorManagement::ParametersError, "UEIMasterObject::Initialise - "
        "Unable to configure ADC CfgLayer for Device %s.", name.Buffer());
    }
    //Configure the FIR filters as required
    if (ok){
        //As described in the API, set all the FIR banks to default state prior to custom initialisation
        ok = (DqAdv217SetFIR(DAQ_handle, deviceId, DQ_AI217_SEL_QFIR_ALL, DQ_AI217_FIR_SET_DEFAULT,0,0,NULL,NULL) >= 0);
        //All the FIR banks are initialised to their default value and enabled, no need to re enable them
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "UEIAI217_803::ConfigureDevice - "
            "Unable to set default FIR parameters for device %s.", name.Buffer());  
        }
    }
    if (ok){
        //Set custom configuration for ach FIR filter
        StreamString banks [FIR_BANK_NUMBER] = {"A","B","C","D"};
        for (uint8 i = 0u; i < FIR_BANK_NUMBER && ok; i++){
            int32 bank = (0x01<<i);
            int32 action = 0;
            int32 decrat = 0;
            int32 tapsize = 0;
            float64* taps = NULL;
            switch(FIRBanks[i].bankState){
                case BANK_NOT_ENABLED:
                    //Disable the FIR filter
                    action = DQ_AI217_FIR_DISABLE;                  //FIR Disable flag
                break;
                case DEFAULT_FIR_SETTING:
                    //Change the FIR filter setting to a new setting
                    action = DQ_AI217_FIR_ENABLE | DQ_AI217_FIR_SET_INDEX;           // FIR Enable flag + Set Index flag
                    tapsize = FIRBanks[i].defaultBankSetting;
                break;
                case CUSTOM_FIR_SETTING:
                    action = DQ_AI217_FIR_ENABLE | DQ_AI217_FIR_COEFF_LOAD;           // FIR Enable flag + Load Coeffs flag
                    tapsize = FIRBanks[i].nTaps;
                    taps = FIRBanks[i].taps;
                break;
                default:
                    ok = false;
                break;
            }
            int32 totalFilterResult;
            ok = (DqAdv217SetFIR(DAQ_handle, deviceId, bank, action, decrat, tapsize,&totalFilterResult,  taps) >= 0);
            if (!ok){
                REPORT_ERROR(ErrorManagement::CommunicationError, "UEIAI217_803::ConfigureDevice - "
                "Error trying to program FIR configuration for bank %s for device %s.", banks[i].Buffer() ,name.Buffer()); 
            }
            if (ok){
                if (totalFilterResult != 8388608){
                    REPORT_ERROR(ErrorManagement::Warning, "UEIAI217_803::ConfigureDevice - "
                    "Inaccurate return value for bank %s for device %s (expected 8388608, got %d).",banks[i].Buffer() ,name.Buffer(),totalFilterResult);  
                }
            }
        }
    }
    return ok;
}

bool UEIAI217_803::GetChannelStatus(int32 DAQ_handle, uint32* errorBitField, uint32* pgaStatusArray){
    bool ok = (DAQ_handle != 0); 
    if (!ok){
        REPORT_ERROR(ErrorManagement::ParametersError, "UEIMasterObject::GetChannelStatus - "
        "Unable to get Channel status for Device %s. Invalid IOM handle", name.Buffer());
    }
    if (ok){
        uint32 newdata;
        ok = (DqAdv217GetPgaStatus(DAQ_handle, deviceId, errorBitField, &newdata, pgaStatusArray) >= 0);
    }
    return ok;
}


int32 UEIAI217_803::GetModel(){
    return 535; //217 in hexadecimal
}

uint8 UEIAI217_803::GetType(){
    return HARDWARE_LAYER_ANALOG_I;
}

uint32 UEIAI217_803::GetDeviceChannels(){
    return CHANNEL_NUMBER;
}

uint8 UEIAI217_803::GetSampleSize(){
    return sizeof(uint32);
}

bool UEIAI217_803::ConfigureChannel(uint32 channelIdx, uint32* channelConfiguration){
    bool ok = (channelIdx < CHANNEL_NUMBER);
    uint32 gain = 0u;
    //TODO checks on channel
    //Check if the channel provided is within available channels for this device
    if (ok){
        switch(gains[channelIdx]){
            case 1:
                gain = DQ_AI217_GAIN_1;
                break;
            case 2:
                gain = DQ_AI217_GAIN_2;
                break;
            case 4:
                gain = DQ_AI217_GAIN_4;
                break;
            case 8:
                gain = DQ_AI217_GAIN_8;
                break;
            case 16:
                gain = DQ_AI217_GAIN_16;
                break;
            case 32:
                gain = DQ_AI217_GAIN_32;
                break;
            case 64:
                gain = DQ_AI217_GAIN_64;
                break;
            default:
                return false;
                break;
        }
        *channelConfiguration = (channelIdx | DQ_LNCL_GAIN(gain) | DQ_LNCL_DIFF); //AI-217 can only operate in differential mode
    }
    return ok;
}

bool UEIAI217_803::ConfigureChannel(uint32 channelIdx, int32* channelConfiguration){
    bool ok = ConfigureChannel(channelIdx, (uint32*)channelConfiguration); 
    return ok;
}

bool UEIAI217_803::AcceptedSignalType(TypeDescriptor signalType){
    //Check the datatypes this device can output. Return true for the datatypes accepted as a valid output of this device
    bool accepted = false;
    /*if (signalType == UnsignedInteger32Bit || signalType == Float64Bit){
        accepted = true;
    }*/
    if (signalType == Float32Bit){
        accepted = true;
    }
    return accepted;
}

bool UEIAI217_803::ScaleSignal(uint32 channelIdx, uint32 listLength, uint32* rawData, float32* scaledData){
    float64 step = 0;
    float64 offset = 0;
    bool ok = (channelIdx < CHANNEL_NUMBER);
    if (ok){
        switch (gains[channelIdx]) {
            case 1:
                step = DQ_AI217_STEP;
                offset = DQ_AI217_OFFSET;
                break;
            case 2:
                step = DQ_AI217_STEP_2 ;
                offset = DQ_AI217_OFFSET_2;
                break;
            case 4:
                step = DQ_AI217_STEP_4;
                offset = DQ_AI217_OFFSET_4;
                break;
            case 8:
                step = DQ_AI217_STEP_8;
                offset = DQ_AI217_OFFSET_8;
                break;
            case 16:
                step = DQ_AI217_STEP_16 ;
                offset = DQ_AI217_OFFSET_16;
                break;
            case 32:
                step = DQ_AI217_STEP_32;
                offset = DQ_AI217_OFFSET_32;
                break;
            case 64:
                step = DQ_AI217_STEP_64;
                offset = DQ_AI217_OFFSET_64;
                break;
            default:
                ok = false;
                break;
        }
        for (uint32 i = 0; i < listLength; i++){
            scaledData[i] = (float32)((rawData[i] & 0xFFFFFF) * step - offset);
        }
    }
    if (!ok){
        REPORT_ERROR(ErrorManagement::CommunicationError, "UEIAI217_803::ScaleSignal - Could not scale channel samples");
    }
    return ok;
}

CLASS_REGISTER(UEIAI217_803, "1.0")
}