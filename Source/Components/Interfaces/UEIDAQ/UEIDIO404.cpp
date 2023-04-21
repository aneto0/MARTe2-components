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
        upperHysteresisProvided = helper.Read("UpperHysteresys", upperHysteresisThreshold);
        if (!upperHysteresisProvided){
            REPORT_ERROR(ErrorManagement::Information, "Upper hysteresis parameter for device %s not provided, the default value is used", name.Buffer());
        }
    }
    if (ok){
        ok &= (upperHysteresisProvided == lowerHysteresisProvided);
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
            ok &= (DqAdv404SetHyst(DAQ_handle, deviceId, voltageReference, &lowerHysteresisThreshold, &upperHysteresisThreshold) >= 0);
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
    return sizeof(uint32); //TODO
}

bool UEIDIO404::ConfigureChannels(SignalDirection direction, uint32** configurationBitfields, uint32& nConfigurationBitfields){
    bool ok = true;
    switch (direction){
        case InputSignals:
            ok &= (nInputChannels != 0u);
            if (ok){
                nConfigurationBitfields = 1;
                if (timestampRequired) nConfigurationBitfields += 1;
                *configurationBitfields = (uint32*)malloc(sizeof(uint32)*nConfigurationBitfields);
                if (timestampRequired) *configurationBitfields[0u] = DQ_LNCL_TIMESTAMP;               
                //The number of bitfields is equal to the number of channels supplied (if no errors are present)
                uint32 destinationIndex = timestampRequired ? 1:0;
                (*configurationBitfields)[destinationIndex] = 0;
            }
        break;
        case OutputSignals:
            ok = false;
        break;
        default:
            ok = false;
        break;
    }
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
    //In this version of the method, the UEIBufferPointer points to the returned 32-bit output word, the first 12 bits
    bool ok = (channelNumber < 12u);
    if (ok){
        uint32* castedRawData;
        if(outputType == Float32Bit){
            float32* castedDestination = reinterpret_cast<float32*>(scaledData);
            for (uint32 i = 0; i < listLength; i++){   
                castedRawData = reinterpret_cast<uint32*>(rawData.GetSample(i)); 
                castedDestination[i] = (float32)(0x01&(*castedRawData>>channelNumber));
            }
        }else if (outputType == Float64Bit){
            float64* castedDestination = reinterpret_cast<float64*>(scaledData);
            for (uint32 i = 0; i < listLength; i++){   
                castedRawData = reinterpret_cast<uint32*>(rawData.GetSample(i)); 
                castedDestination[i] = (float64)(0x01&(*castedRawData>>channelNumber));
            }
        }else if (outputType == UnsignedInteger32Bit){
            uint32* castedDestination = reinterpret_cast<uint32*>(scaledData);
            for (uint32 i = 0; i < listLength; i++){   
                castedRawData = reinterpret_cast<uint32*>(rawData.GetSample(i)); 
                castedDestination[i] = (uint32)(0x01&(*castedRawData>>channelNumber));
            }
        }else if (outputType == UnsignedInteger64Bit){
            uint64* castedDestination = reinterpret_cast<uint64*>(scaledData);
            for (uint32 i = 0; i < listLength; i++){   
                castedRawData = reinterpret_cast<uint32*>(rawData.GetSample(i)); 
                castedDestination[i] = (uint64)(0x01&(*castedRawData>>channelNumber));
            }
        }else if (outputType == UnsignedInteger16Bit){
            uint16* castedDestination = reinterpret_cast<uint16*>(scaledData);
            for (uint32 i = 0; i < listLength; i++){   
                castedRawData = reinterpret_cast<uint32*>(rawData.GetSample(i)); 
                castedDestination[i] = (uint16)(0x01&(*castedRawData>>channelNumber));
            }
        }else if (outputType == UnsignedInteger8Bit){
            uint8* castedDestination = reinterpret_cast<uint8*>(scaledData);
            for (uint32 i = 0; i < listLength; i++){   
                castedRawData = reinterpret_cast<uint32*>(rawData.GetSample(i)); 
                castedDestination[i] = (uint8)(0x01&(*castedRawData>>channelNumber));
            }
        }else{
            ok = false;
        }
    }
    return ok;
}

bool UEIDIO404::RetrieveInputSignal(uint32 channelIdx, uint32 nSamples, void* SignalPointer, TypeDescriptor signalType){
    //Check if the input signals buffer is ready for reading
    bool ok = inputChannelsBuffer.CheckReadReady();
    if (ok){
        UEIBufferPointer rawData = inputChannelsBuffer.ReadChannel(0, ok);
        if (ok){
            ok &= ScaleSignal(channelIdx, nSamples, rawData, SignalPointer, signalType);
        }
    }
    return ok;
}

bool UEIDIO404::SetOutputSignal(uint32 channelIdx, uint32 nSamples, void* SignalPointer, TypeDescriptor signalType){
    //This implementation of the method always return false as this hardware layer can never accept output signals
    return false;
}

bool UEIDIO404::InitBuffer(SignalDirection direction, uint32 nBuffers, uint32 retrievedSamples, uint32 readSammples){
    bool ok = true;
    switch (direction){
        case InputSignals:
            //The retrieved input is a 32-bit wide word containing the status of all the I/O channels
            ok &= inputChannelsBuffer.InitialiseBuffer(nBuffers, 1u, retrievedSamples, sizeof(uint32), readSammples, timestampRequired);
        break;
        case OutputSignals:
            ok = false;
        break;
        default:
            ok = false;
        break;
    }
    return ok;
}

CLASS_REGISTER(UEIDIO404, "1.0")
}