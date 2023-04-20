/**
 * @file UEIDIO404.cpp
 * @brief Source file for class UEIDIO404
 * @date 17/04/2023
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
 * the class UEIDIO404 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEIDIO404.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

UEIDIO404::UEIDIO404() : UEIDevice() {
    referenceVoltage = 0.0;
    hysteresisConfigured = false;
}

UEIDIO404::~UEIDIO404(){
}

bool UEIDIO404::Initialise(StructuredDataI &data){
    //Initialise the UEIDevice Object, this method will initialise and check base parameters for the device (devn, name and sampling frequency)
    bool ok = UEIDevice::Initialise(data);
    //Initialise helper for retriecing the input parameters
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Read and validate the gains
    if(ok){
        ok &= helper.Read("VoltageReference", referenceVoltage);
        ok &= (referenceVoltage == 3.3  || referenceVoltage == 5.0  ||
               referenceVoltage == 12.0 || referenceVoltage == 24.0 ||
               referenceVoltage == 36.0);
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "VoltageReference parameter must be supplied for device %s (3.3, 5, 12, 24, 36 volts values accepted)", name.Buffer());
        }
    }
    bool upperHysteresisProvided = false;
    bool lowerHysteresisProvided = false;
    if(ok){
        lowerHysteresisProvided = helper.Read("LowerHysteresys", lowerHysteresisThreshold);
        if (!lowerHysteresisProvided){
            REPORT_ERROR(ErrorManagement::Information, "Lower hysteresis parameter for device %s not provided, the default value is used", name.Buffer());
        }
    }
    if(ok){
        upperHysteresisProvided = helper.ReadArray("UpperHysteresys", upperHysteresisThreshold);
        if (!upperHysteresisProvided){
            REPORT_ERROR(ErrorManagement::Information, "Upper hysteresis parameter for device %s not provided, the default value is used", name.Buffer());
        }
    }
    if (ok){
        ok &= (upperHysteresisProvided != lowerHysteresisProvided);
        hysteresisConfigured = ok;
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Hysteresis must be provided for both upper and lower thresholds or not be provided (default values are used) for device %s", name.Buffer());
        }
    }
    return ok;
}

bool UEIDIO404::CheckChannelAndDirection(uint32 channelNumber, SignalDirection direction){
    //For DIO-404 all the channels (0 to 11 as only 12 channels are available for each direction) can be both inputs and outputs
    bool validChannel = false;
    //Channel only valid if it is configured as an output
    if (direction == InputSignals){
        //Channel only valid if within range [0,15]
        if (channelNumber>=0u && channelNumber<12){
            validChannel = true;
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError,"DIO-404 layer only accepts channels 0 to 11 as inputs.");
        }
    }else{
        if (channelNumber>=0u && channelNumber<12){
            validChannel = true;
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError,"DIO-404 layer only accepts channels 0 to 11 as outputs.");
        }
    }
    return validChannel;
}

bool UEIDIO404::ConfigureDevice(int32 DAQ_handle){
    bool ok = (DAQ_handle > 0);
    if (!ok){
        REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid IOM handle supplied to DIO-404 %s", name.Buffer());
    }
    //Configure hysteresis into the device
    if (ok && hysteresisConfigured){
        int32 voltageReference;
        //Set the appropriate voltage reference
        if      (referenceVoltage == 3.3 ) voltageReference = DQ_DIO404_REF_3_3V;
        else if (referenceVoltage == 5.0 ) voltageReference = DQ_DIO404_REF_5V;
        else if (referenceVoltage == 12.0) voltageReference = DQ_DIO404_REF_12V;
        else if (referenceVoltage == 24.0) voltageReference = DQ_DIO404_REF_24V;
        else if (referenceVoltage == 36.0) voltageReference = DQ_DIO404_REF_36V;
        else ok = false;
        if (ok){
            ok &= (DqAdv404SetHyst(DAQ_handle, deviceId, voltageReference, lowerHysteresisThreshold, upperHysteresisThreshold) >= 0);
        }
    }
    return ok;
}

int32 UEIDIO404::GetModel(){
    return 0x404; //404 in hexadecimal
}

IOLayerType UEIDIO404::GetType(){
    return HARDWARE_LAYER_DIGITAL_IO;
}

uint32 UEIDIO404::GetDeviceChannels(SignalDirection direction){
    switch (direction){
        case InputSignals:
            return 12u;
        break;
        case OutputSignals:
            return 12u;
        break;
        default:
            return 0u;
        break;
    }
}

uint8 UEIDIO404::GetSampleSize(){
    return sizeof(uint32);
}

bool UEIDIO404::ConfigureChannel(uint32* channelIdxs, uint32 channelNumber, uint32* configurationBitfields, uint32& nConfigurationBitfields, SignalDirection direction){
    //When configuring this device, irrespective of the direction and the number of channels, the number of configuration bitfields is just an empty one
    bool ok = (channelNumber <= 12u);
    nConfigurationBitfields = 1;
    configurationBitfields = (uint32*)malloc (sizeof(uint32));
    configurationBitfields[0] = 0;
    return ok;
}

bool UEIDIO404::AcceptedSignalType(TypeDescriptor signalType){
    //Check the datatypes this device can output. Return true for the datatypes accepted as a valid output of this device
    bool accepted = false;
    if (signalType == Float32Bit ||
        signalType == Float64Bit ||
        signalType == UnsignedInteger8Bit||
        signalType == UnsignedInteger16Bit||
        signalType == UnsignedInteger32Bit||
        signalType == UnsignedInteger64Bit){
        accepted = true;
    }
    return accepted;
}


bool UEIDIO404::ScaleSignal(uint32 channelNumber, uint32 listLength, UEIBufferPointer rawData, void* scaledData, TypeDescriptor outputType){
    float64 step = 0;
    float64 offset = 0;
    bool ok = (channelNumber < CHANNEL_NUMBER);
    if (!ok){
        REPORT_ERROR(ErrorManagement::CommunicationError, "Invalid channel number supplied for scaling");
    }
    if (ok){
        ok &= AcceptedSignalType(outputType);
        if (!ok){
            REPORT_ERROR(ErrorManagement::CommunicationError, "Invalid output data format speficied for AI217-803");
        }
    }
    if (ok){
        ok &= (scaledData != NULL_PTR(void*));
        ok &= (rawData.CheckPointer());
        ok &= (rawData.GetSample(0) != scaledData);
        if (!ok){
            REPORT_ERROR(ErrorManagement::CommunicationError, "Invalid pointers supplied");
        }
    }
    if (ok){
        switch (gains[channelNumber]) {
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
                REPORT_ERROR(ErrorManagement::CommunicationError, "Forbbidden gain detected on channel %d", channelNumber);
                break;
        }
        uint32* castedRawData;
        if(outputType == Float32Bit){
            float32* castedDestination = reinterpret_cast<float32*>(scaledData);
            for (uint32 i = 0; i < listLength; i++){   
                castedRawData = reinterpret_cast<uint32*>(rawData.GetSample(i)); 
                castedDestination[i] = (float32)((*castedRawData & 0xFFFFFF) * step - offset);
            }
        }else if (outputType == Float64Bit){
            float64* castedDestination = reinterpret_cast<float64*>(scaledData);
            for (uint32 i = 0; i < listLength; i++){   
                castedRawData = reinterpret_cast<uint32*>(rawData.GetSample(i)); 
                castedDestination[i] = (float64)((*castedRawData & 0xFFFFFF) * step - offset);
            }
        }else if (outputType == UnsignedInteger32Bit){
            uint32* castedDestination = reinterpret_cast<uint32*>(scaledData);
            for (uint32 i = 0; i < listLength; i++){   
                castedRawData = reinterpret_cast<uint32*>(rawData.GetSample(i)); 
                castedDestination[i] = (uint32)(*castedRawData & 0xFFFFFF);
            }
        }else if (outputType == UnsignedInteger64Bit){
            uint64* castedDestination = reinterpret_cast<uint64*>(scaledData);
            for (uint32 i = 0; i < listLength; i++){   
                castedRawData = reinterpret_cast<uint32*>(rawData.GetSample(i)); 
                castedDestination[i] = (uint64)(*castedRawData & 0xFFFFFF);
            }
        }else{
            ok = false;
        }
    }
    if (!ok){
        REPORT_ERROR(ErrorManagement::CommunicationError, "Could not scale channel samples");
    }
    return ok;
}

CLASS_REGISTER(UEIDIO404, "1.0")
}