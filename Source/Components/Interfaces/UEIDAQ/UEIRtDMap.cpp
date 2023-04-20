/**
 * @file UEIRtDMap.cpp
 * @brief Source file for class UEIRtDMap
 * @date 14/02/2023
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
 * the class UEIRtDMap (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CLASSMETHODREGISTER.h"
#include "UEIRtDMap.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UEIRtDMap::UEIRtDMap() : UEIMapContainer() {
    inputMap = NULL_PTR(uint8*);
    scanRate = 0;
    previousTimestamp = 0;
    previousSyncTime = 0;
}

UEIRtDMap::~UEIRtDMap(){
    if (inputMap != NULL_PTR(uint8*)){
        inputMap = NULL_PTR(uint8*);
    }
    //try to clean the maps in case it was not already done beforehand
    CleanupMap();
}

bool UEIRtDMap::StopMap(){
    bool ok = true;
    if (DAQ_handle != 0 && mapid != 0){
        ok &= (DqRtDmapStop(DAQ_handle, mapid) >= 0);
    }
    return ok;
}

bool UEIRtDMap::CleanupMap(){
    bool ok = true;
    if (DAQ_handle != 0){
        if(mapid){
            ok &= StopMap();
            ok &= (DqRtDmapClose(DAQ_handle, mapid) >= 0);
        }
        if (!ok){
            REPORT_ERROR(ErrorManagement::CommunicationError, "Map %s could not be correctly cleaned!", name.Buffer());
        }
        DAQ_handle = 0;
    }else{
        REPORT_ERROR(ErrorManagement::Information, "Map %s was not started, skipping cleanup!", name.Buffer());
    }
    return ok;
}

bool UEIRtDMap::Initialise(StructuredDataI &data){
    bool ok = UEIMapContainer::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //For the RtDMap we do need a ScanRate parameter which defines at which frequency the IOM refreshes the 
    //RtDMap internally
    if (ok){
        ok = data.Read("ScanRate", scanRate);
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve ScanRate for map %s.", name.Buffer());            
        }
    }
    //Check that 
    if (ok && nInputChannels > 0u){
        //For the RtDMap, the first channel of the first device is always going to be the map timestamp
        inputMembersOrdered[0]->Inputs.timestampRequired = true;
    }
    return ok;
}

bool UEIRtDMap::ConfigureInputsForDataSource(uint32 nSamples, uint32 nChannels, uint64* inputTimestampAddress, uint8** signalAddresses, TypeDescriptor* signalTypes){
    bool ok = UEIMapContainer::ConfigureInputsForDataSource(nSamples, nChannels, inputTimestampAddress, signalAddresses, signalTypes);
    if (ok){
        if (nSamples != 1u){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid sample number, RtDMap only supports sample number 1");
            ok = false;
        }
    }
    //Flag signaling the signals are well configured into the Map
    signalsConfigured = ok;
    return ok;
}

bool UEIRtDMap::StartMap(){
    bool ok = true;
    ok = (DqRtDmapInit(DAQ_handle, &mapid, scanRate) >= 0);
    if (!ok){
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error on Initialising Map %s", name.Buffer());
    }
    if (ok){
        //First after creating the Dmap structure is setting the sampling rate for each of the devices,
        // as the function DqRtDmapInit sets all the sampling rates to that of the retrieval of the Map
        for (uint32 dev = 0u; dev < MAX_IO_SLOTS && ok; dev++){
            if (members[dev].defined){
                ok = (members[dev].reference.IsValid());
                if (ok){
                    ReferenceT<UEIDevice> device = members[dev].reference;
                    ok &= (SetDevSamplingFreqRtDmap(DAQ_handle, mapid, dev, device->GetSamplingFrequency()) >= 0);
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Could not set sampling rate on dev%d for Map %s", dev, name.Buffer());
                    }
                }else{
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve the device reference for dev%d on map %s", dev, name.Buffer());
                }
            }
        }
    }
    //Configure the devices with channels to be retrieved information
    if (ok){
        for (uint32 i = 0u; i < nInputMembers && ok; i++){
            ReferenceT<UEIDevice> reference = inputMembersOrdered[i]->reference;
            ok &= reference->SetInputChannelList(inputMembersOrdered[i]->Inputs.channels, inputMembersOrdered[i]->Inputs.nChannels);
            inputMembersOrdered[i]->reference->timestampRequired = inputMembersOrdered[i]->Inputs.timestampRequired;
            //Init the buffers, only one sample retrieved and read back
            ok &= reference->InitBuffer(InputSignals, 2u, 1u, 1u);
        }
        for (uint32 i = 0u; i < nOutputMembers && ok; i++){
            ReferenceT<UEIDevice> reference = inputMembersOrdered[i]->reference;
            ok &= reference->SetOutputChannelList(inputMembersOrdered[i]->Inputs.channels, inputMembersOrdered[i]->Inputs.nChannels);
            //Init the buffers, only one sample retrieved and read back
            ok &= reference->InitBuffer(OutputSignals, 2u, 1u, 1u);
        }
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Device channel information setting failed");
        }
    }
    if (ok){
        //Once the map is initialized correctly, the I/O channels need to be checked into the map itself
        //First the input channels are checked. The inputMembersOrdered pointer array is traversed in order,
        // yielding the ordered list of channels to sequence into the input map
        //Now go ahead and register all the needed channels in the configured order
        for (uint32 i = 0u; i < nInputMembers && ok; i++){
            uint8 devn;
            ReferenceT<UEIDevice> devReference = inputMembersOrdered[i]->reference;
            ok = (devReference.IsValid());  //This should not be necessary, but it is implemented for precaution
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Found invalid device reference on inputMember %i (devn%d) on Map %s", i, inputMembersOrdered[i]->devn, name.Buffer());
            }
            //Retrieve the configuration bitfields for this device and the selected channels
            if (ok){
                uint32* configurationBitfields = NULL_PTR(uint32*);
                uint32 nConfigurationBitfields = 0;
                devn = inputMembersOrdered[i]->devn;
                ok &= (devReference->ConfigureChannels(InputSignals, &configurationBitfields, nConfigurationBitfields));
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error configuring input channels for dev%d on Map %s", devn, name.Buffer());
                }
                for (uint32 j = 0; j < nConfigurationBitfields && ok; j++){
                    ok = (DqRtDmapAddChannel(DAQ_handle, mapid, devn, DQ_SS0IN, &configurationBitfields[j], 1) >= 0);
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Error adding input channels in IOM for dev%d on Map %s", devn, name.Buffer());
                    }
                }
                if (configurationBitfields != NULL_PTR(uint32*)){
                    free(configurationBitfields);
                }
            }
        }        
    }
    if (ok){
        //Once the map is initialized correctly, the I/O channels need to be checked into the map itself
        //First the output channels are checked. The outputMembersOrdered pointer array is traversed in order,
        // yielding the ordered list of channels to sequence into the output map
        for (uint32 i = 0u; i < nOutputMembers && ok; i++){
            ReferenceT<UEIDevice> devReference = outputMembersOrdered[i]->reference;
            ok = (devReference.IsValid());  //This should not be necessary, but it is implemented for precaution
            uint8 devn;
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Found invalid device reference on outputMember %i (devn%d) on Map %s", i, outputMembersOrdered[i]->devn,  name.Buffer());
            }
            //Retrieve the configuration bitfields for this device and the selected channels
            if (ok){
                uint32* configurationBitfields = NULL_PTR(uint32*);
                uint32 nConfigurationBitfields = 0;
                devn = outputMembersOrdered[i]->devn;
                ok &= (devReference->ConfigureChannels(OutputSignals, &configurationBitfields, nConfigurationBitfields));
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error configuring output channels for dev%d on Map %s", devn, name.Buffer());
                }
                for (uint32 j = 0; j < nConfigurationBitfields && ok; j++){
                    ok = (DqRtDmapAddChannel(DAQ_handle, mapid, devn, DQ_SS0OUT, &configurationBitfields[j], 1) >= 0);
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Error adding output channels in IOM for dev%d on Map %s", devn, name.Buffer());
                    }
                }
                if (configurationBitfields != NULL_PTR(uint32*)){
                    free(configurationBitfields);
                }
            }
        }
    }
    //If we reach here with ok=true, then the channels are correctly placed into the V/Dmap sctructure
    //Let's start the layers to start acquiring data
    if (ok){
        ok = (DqRtDmapStart(DAQ_handle, mapid) >= 0);
        if (!ok){
            REPORT_ERROR(ErrorManagement::CommunicationError, "Could not start Map %s", name.Buffer());
        }
        if (ok){
            //Try to acquire the memory map pointers
            ok = GetMapPointers();
            if (!ok){
                REPORT_ERROR(ErrorManagement::CommunicationError, "Could not retrieve the memory map pointers for Map %s", name.Buffer());
            }
        }
    }
    //Flag signaling the map is ready for action
    mapStarted = ok;
    return ok;
}

bool UEIRtDMap::GetMapPointers(){
    //Function just valid for input signals (the ones enternig into IOM) TODO extended to output maps
    unsigned char* map_pointer = NULL;
    uint16 counter = 0u;
    bool ok = (DAQ_handle > 0);
    if (ok){
        while((((uint8*) map_pointer) == NULL)&& ok){
            counter++;
            ok = (DqRtDmapRefresh(DAQ_handle, mapid) >= 0);
            if (ok){
                Sleep::MSec(10);
                ok = (DqRtDmapGetInputMap(DAQ_handle, mapid, 0, &(map_pointer)) >=0);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::CommunicationError, "GetOutputMap failed during the GetMapAddr method for Map %s", name.Buffer());   
                }
            }else{
                REPORT_ERROR(ErrorManagement::CommunicationError, "Refresh failed during the GetMapPointers method for Map %s", name.Buffer());
            }
            if (counter > 20) break; //If NULL map is still present during 2s abort the initialisation of the DS
        }
        if (((uint8*) map_pointer) != NULL){
            inputMap = ((uint8*)map_pointer);
            ok &=  true;
        }else{
            ok = false;
            REPORT_ERROR(ErrorManagement::CommunicationError, "RtDMap pointer getter method timed out for Map %s", name.Buffer());
        }
    }else{
        REPORT_ERROR(ErrorManagement::CommunicationError, "Error while getting map pointers for Map %s due to DAQ_handle being not set", name.Buffer());
    }
    return ok;
}

bool UEIRtDMap::PollForNewPacket(MapReturnCode& outputCode){
    bool ok = (mapStarted && signalsConfigured && mapCoherent);
    //Check first if the map is ready for action
    if (!ok){
        outputCode = ERROR;
        REPORT_ERROR(ErrorManagement::CommunicationError, "Map %s is not ready for data acquisition", name.Buffer());
    }
    if (ok){
        //Calculate the period since the last sync in ms
        float32 periodSinceLastSync =  (HighResolutionTimer::Counter() - previousSyncTime)/1000000;
        while (periodSinceLastSync < ((1000/(scanRate))-1)){
            periodSinceLastSync = (HighResolutionTimer::Counter() - previousSyncTime)/1000000;
            int32 timeToSleep = (1000/(scanRate))-periodSinceLastSync;
            if (timeToSleep > 0){
                Sleep::MSec(timeToSleep);
            }else{
                break;
            }
        }
        //Set the next time for DMap refresh, the last sync time + the period of the Dmap, dont calculate it with the
        // current time to avoid accumulative error.
        previousSyncTime = (HighResolutionTimer::Counter());            
    }
    //Poll for next packet from UEIDAQ
    if (ok){
        ok = (DqRtDmapRefresh(DAQ_handle, mapid) >= 0);
        if (!ok){
            outputCode = ERROR;
            REPORT_ERROR(ErrorManagement::CommunicationError, "Refresh for Map %s failed", name.Buffer());
        }
    }
    //If Poll request succeeded, then start the process to acquire the new data
    if (ok){
        //Check if the response is a new packet or a rerequest.
        //been previously requested.
        //The first channel on the map is always the timestamp, check its value to see if it is different than the last received
        firstPckt = false;
        //The recived packet is a newly converted one not requested yet.
        //Make the signals available to the DataSource.
        uint32 signalIdx = 0u;
        for (uint32 i = 0; i < nInputMembers && ok; i++){
            uint32 nOfchannel = inputMembersOrdered[i]->Inputs.nChannels;
            ReferenceT<UEIDevice> thisDevice = inputMembersOrdered[i]->reference;
            ok &= (thisDevice.IsValid());
            //The channels requested have already been copied, stop the loop
            if (!ok){
                outputCode = ERROR;
                REPORT_ERROR(ErrorManagement::CommunicationError, "Error while accessing a device reference (devn%s) on Map %s.", inputMembersOrdered[i]->devn, name.Buffer());
            }
            //Read the data from this device in the Dmap into the device 
            if (ok){
                void* writePointer = reinterpret_cast<void*>(thisDevice->inputChannelsBuffer.writePointer);
                //We try to read all the data avilable for this device, the 1000u number of channels is just to ensure all channels are read.
                ok &= (DqRtDmapReadRawData(DAQ_handle, mapid, inputMembersOrdered[i]->devn, writePointer, 1000u) >= 0);
                if (ok){
                    //if read was successful, then mark the buffer as ready to write by advancing the pointer
                    ok &= thisDevice->inputChannelsBuffer.AdvanceBufferReadOneSample();
                }
            }
            if (ok){               
                uint8 byteSize = thisDevice->GetSampleSize();
                for (uint32 j = 0; j < nOfchannel && ok; j++){
                    uint32 channelIdx = inputMembersOrdered[i]->Inputs.channels[j];
                    void* scaledData = reinterpret_cast<void*>(inputSignalAddresses[signalIdx]);
                    TypeDescriptor outputType = inputSignalTypes[signalIdx];
                    ok &= thisDevice->RetrieveInputSignal(channelIdx, 1u, scaledData, outputType);
                    signalIdx ++;
                }
                //Compute the timestamp, which occupies the first position of input memory on the datasource as uint64
                //By calling this method in this position, the already written float64 on the first position is overwritten by this uint64 timestamp (better alternative)
                if (ok){
                    ok &= GetTimestamp(reinterpret_cast<uint32*>(inputMap)[0], *reinterpret_cast<uint64*>(TimestampAddr));
                    if (!ok){
                        outputCode = ERROR;
                        REPORT_ERROR(ErrorManagement::CommunicationError, "Could not process correctly the timestamp value on RtDMap %s", name.Buffer());
                    }
                }
                if (ok){
                    outputCode = NEW_DATA_AVAILABLE;
                }
                thisDevice->inputChannelsBuffer.CheckoutBuffer();
                //The channels requested have already been copied, stop the loop
                if (!ok){
                    outputCode = ERROR;
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Error while translating the channels to scaled values on Map %s.", name.Buffer());
                }  
            }
        }
        
    }
    return ok;
}

MapType UEIRtDMap::GetType(){
    return RTDMAP;
}

bool UEIRtDMap::CheckMapCoherency(){
    bool ok = UEIMapContainer::CheckMapCoherency();
    if (ok){
        ok = (scanRate <= GetsampleRate());
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "ScanRate for RtDMap %s must not be higher than the sampling rate of its configured devices (%.02f > %.02f Hz)", name.Buffer(), scanRate, GetsampleRate());
        }
    }
    mapCoherent = ok;
    return ok;
}

CLASS_REGISTER(UEIRtDMap, "1.0")
}