/**
 * @file UEIAO308.cpp
 * @brief Source file for class UEIAO308
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
 * the class UEIAO308 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEIAO308.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

UEIAO308::UEIAO308() : UEIDevice() {
}

UEIAO308::~UEIAO308(){
}

bool UEIAO308::Initialise(StructuredDataI &data){
    //Initialise the UEIDevice Object, this method will initialise and check base parameters for the device (devn, name and sampling frequency)
    bool ok = UEIDevice::Initialise(data);
    return ok;
}

bool UEIAO308::CheckChannelAndDirection(uint32 channelNumber, SignalDirection direction){
    //For AI-217-803 all the channels (0 to 15 as only 16 channels are available) are inputs
    bool validChannel = false;
    //Channel only valid if it is configured as an output
    if (direction == OutputSignals){
        //Channel only valid if within range [0,15]
        if (channelNumber>=0u && channelNumber<8u){
            validChannel = true;
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError,"AO-308 layer only accepts channels 0 to 7 as outputs.");
        }
    }else{
        REPORT_ERROR(ErrorManagement::ParametersError, "AO-308 layer only accepts output signals!");
    }
    return validChannel;
}

bool UEIAO308::ConfigureDevice(int32 DAQ_handle){
    //No configuration available for this layer at the moment
    return (DAQ_handle > 0);
}

int32 UEIAO308::GetModel(){
    return 0x0308; //217 in hexadecimal
}

IOLayerType UEIAO308::GetType(){
    return HARDWARE_LAYER_ANALOG_O;
}

uint32 UEIAO308::GetDeviceChannels(SignalDirection direction){
    switch(direction){
        case InputSignals:
            return 0u;
        break;
        case OutputSignals:
            return 8u;
        break;
        default:
            return 0u;
        break;
    }
}

uint8 UEIAO308::GetSampleSize(){
    return sizeof(uint16);
}

bool UEIAO308::ConfigureChannels(SignalDirection direction, uint32** configurationBitfields, uint32& nConfigurationBitfields, MapType mapType){
    bool ok = true;
    switch (direction){
        case OutputSignals:
            ok &= (nOutputChannels != 0u);
            if (ok){
                //No timestamp retrieving is allowed, since the layer only recieves data, does not provide it
                nConfigurationBitfields = nOutputChannels;
                *configurationBitfields = (uint32*)malloc(sizeof(uint32)*nOutputChannels);
                //The number of bitfields is equal to the number of channels supplied (if no errors are present)
                for (uint32 i = 0u; i < nOutputChannels && ok; i++){
                    (*configurationBitfields)[i] = outputChannelList[i];
                }
            }
        break;
        case InputSignals:
            ok = false;
        break;
        default:
            ok = false;
        break;
    }
    return ok;
}

bool UEIAO308::AcceptedSignalType(TypeDescriptor signalType){
    //Check the datatypes this device can input. Return true for the datatypes accepted as a valid output of this device
    bool accepted = false;
    if (signalType == Float32Bit ||
        signalType == Float64Bit ||
        signalType == UnsignedInteger16Bit||
        signalType == UnsignedInteger32Bit||
        signalType == UnsignedInteger64Bit){
        accepted = true;
    }
    return accepted;
}

bool UEIAO308::ScaleSignal(uint32 channelNumber, uint32 listLength, UEIBufferPointer rawData, void* scaledData, TypeDescriptor outputType){
    //This layer cannot scale the signals as it is only output
    return false;
}

bool UEIAO308::RetrieveInputSignal(uint32 channelIdx, uint32 nSamples, void* SignalPointer, TypeDescriptor signalType){
    //This layer cannot retrieve input signals as it is only output
    return false;
}

bool UEIAO308::SetOutputSignal(uint32 channelIdx, uint32 nSamples, void* SignalPointer, TypeDescriptor signalType){
    bool ok = (channelIdx < 8u);
    //Check if there's space to write the needed samples
    ok &= outputChannelsBuffer->CheckAvailableSpace(nSamples*GetSampleSize());
    if (ok){
        uint8* wPointer = outputChannelsBuffer->writePointer;
        ok &= (wPointer != NULL_PTR(uint8*));
        if (ok){
            int32 index = FindChannelIndex(channelIdx, OutputSignals);
            uint16* samplesBuffer = reinterpret_cast<uint16*>(wPointer);
            UEIBufferPointer writePointer;
            ok &= outputChannelsBuffer->GetWritePointer(channelIdx, writePointer);
            if (ok){
                if (signalType == UnsignedInteger64Bit){
                    uint64* outputSignal = reinterpret_cast<uint64*>(SignalPointer);
                    for (uint32 sample = 0; sample < nSamples; sample++){
                        uint16* thisSample = reinterpret_cast<uint16*>(writePointer.GetSample(sample));
                        *thisSample = (uint16)(outputSignal[sample]&0x0000FFFF);
                    }
                }else if (signalType == UnsignedInteger32Bit){
                    uint32* outputSignal = reinterpret_cast<uint32*>(SignalPointer);
                    for (uint32 sample = 0; sample < nSamples; sample++){
                        uint16* thisSample = reinterpret_cast<uint16*>(writePointer.GetSample(sample));
                        *thisSample = (uint16)(outputSignal[sample]&0x0000FFFF);
                    }
                }else if (signalType == UnsignedInteger16Bit){
                    uint16* outputSignal = reinterpret_cast<uint16*>(SignalPointer);
                    for (uint32 sample = 0; sample < nSamples; sample++){
                        uint16* thisSample = reinterpret_cast<uint16*>(writePointer.GetSample(sample));
                        *thisSample = outputSignal[sample];
                    }
                }else if (signalType == Float32Bit){
                    float32* outputSignal = reinterpret_cast<float32*>(SignalPointer);
                    for (uint32 sample = 0; sample < nSamples && ok; sample++){
                        uint16* thisSample = reinterpret_cast<uint16*>(writePointer.GetSample(sample));
                        if (outputSignal[sample] >= -10.0 && outputSignal[sample] <= 10.0){
                            *thisSample = ((uint16)((outputSignal[sample]+10.0)*(0x0000FFFF/20.0)) & 0xFFFF);
                        }else{
                            REPORT_ERROR(ErrorManagement::CommunicationError, "Invalid input voltage supplied for channel %d of device %s, voltage must be within the -10.0 and +10.0v range", channelIdx, name.Buffer());
                            ok = false;
                        } 
                    }
                }else if (signalType == Float64Bit){
                    float64* outputSignal = reinterpret_cast<float64*>(SignalPointer);
                    for (uint32 sample = 0; sample < nSamples && ok; sample++){
                        uint16* thisSample = reinterpret_cast<uint16*>(writePointer.GetSample(sample));
                        if (outputSignal[sample] >= -10.0 && outputSignal[sample] <= 10.0){
                            *thisSample = ((uint16)((outputSignal[sample]+10.0)*(0x0000FFFF/20.0)) & 0xFFFF);
                        }else{
                            REPORT_ERROR(ErrorManagement::CommunicationError, "Invalid input voltage supplied for channel %d of device %s, voltage must be within the -10.0 and +10.0v range", channelIdx, name.Buffer());
                            ok = false;
                        }    
                    }
                }else{
                    ok = false;
                }
            }
        }
    }
    return ok;
}

bool UEIAO308::InitBuffer(SignalDirection direction, uint32 nBuffers, uint32 writeSamples, uint32 readSammples){
    bool ok = true;
    switch (direction){
        case OutputSignals:
            ok &= outputChannelsBuffer->InitialiseBuffer(nBuffers, nOutputChannels, writeSamples, GetSampleSize(), readSammples, false);
        break;
        case InputSignals:
            ok = false;
        break;
        default:
            ok = false;
        break;
    }
    return ok;
}

bool UEIAO308::GetHardwareChannels(SignalDirection direction, uint32& nChannels){
    bool ok = true;
    switch(direction){
        case InputSignals:
            nChannels = 0u;
        break;
        case OutputSignals:
            nChannels = nOutputChannels;
        break;
        default:
            nChannels = 0u;
            ok = false;
        break;
    }
    return ok;
}
    


CLASS_REGISTER(UEIAO308, "1.0")
}