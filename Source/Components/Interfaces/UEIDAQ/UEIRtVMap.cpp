/**
 * @file UEIRtVMap.cpp
 * @brief Source file for class UEIRtVMap
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
 * the class UEIRtVMap (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CLASSMETHODREGISTER.h"
#include "UEIRtVMap.h"
#include <iostream>     //Provisional for testing


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UEIRtVMap::UEIRtVMap() : UEIMapContainer() {
    nBuffers = 0;
    nReadSamples = 0;
}

UEIRtVMap::~UEIRtVMap(){
    //try to clean the maps in case it was not already done beforehand
    CleanupMap();
}

bool UEIRtVMap::StopMap(){
    if (DAQ_handle != 0 && mapid != 0){
        return (DqRtVmapStop(DAQ_handle, mapid) >= 0);
    }else{
        return true;
    }
}

bool UEIRtVMap::CleanupMap(){
    bool ok = true;
    if (DAQ_handle != 0){
        if(mapid){
            ok &= StopMap();
            ok &= (DqRtVmapClose(DAQ_handle, mapid) >= 0);
        }
        DAQ_handle = 0;
    }else{
        REPORT_ERROR(ErrorManagement::Information,"Map %s was not started, skipping cleanup!", name.Buffer());
    }
    return ok;
}

bool UEIRtVMap::Initialise(StructuredDataI &data){
    bool ok = UEIMapContainer::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //In the VMap mode we do need to provide a parameter "Samples" which states
    //the maximum number to be retrieved from the devices FIFOs for each VMap query
    //This parameter is important for VMap as it plays a very important role in system performance and FIFO overflow avoidance
    if (ok){
        ok = data.Read("Samples", sampleNumber);
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "Samples parameter not provided in Map %s, required for selected VMap mode.", name.Buffer());
        }
    }
    if (ok){
        ok = data.Read("NumberOfBuffers", nBuffers);
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfBuffers (Number of sub buffers for UEICircularBuffer) parameter not provided in Map %s, required for selected VMap mode.", name.Buffer());
        }
    }
    //For the VMap, the circular buffers must be initialised prior to any usage
    if (ok){
        //Create a reference T to the new circular buffer for each device
        ClassRegistryDatabase *crdSingleton;
        const ClassRegistryItem *classRegistryItem;
        const ObjectBuilder* builder;
        crdSingleton = ClassRegistryDatabase::Instance();
        classRegistryItem = crdSingleton->Find("UEICircularBuffer");
        ok = (classRegistryItem != NULL); 
        if (!ok) REPORT_ERROR(ErrorManagement::InitialisationError, "Could not find UEICircularBuffer class");
        if (ok) {
            //Get the object builder (which knows how to build classes of this type).
            builder = classRegistryItem->GetObjectBuilder();
            ok = (builder != NULL);
            if (!ok) REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve ObjectBuilder");
        }
        if (ok){
            //Assign the CircularBuffer references to the Input members
            for (uint8 i = 0; i < nInputMembers && ok; i++){
                //Instantiate the objects and save the referenceT to them
                Object *obj = builder->Build(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                inputMembersOrdered[i]->Inputs.buffer = ReferenceT<UEICircularBuffer>(dynamic_cast<UEICircularBuffer*>(obj));
                ok = (inputMembersOrdered[i]->Inputs.buffer.IsValid());
                if (!ok) REPORT_ERROR(ErrorManagement::InitialisationError, "Error while allocating UEICircularBuffer objects on Map members of Map %s", name.Buffer());
            }
            //TODO buffering for Output signals
        }
    }    
    //TODO for now we only retrieve the timestamp of the first member defined in inputs
    if (ok && nInputChannels > 0u){
        //For the RtVMap, the first channel of the first device is always going to be the map timestamp.
        inputMembersOrdered[0]->Inputs.timestampRequired = true;
    }
    return ok;
}

bool UEIRtVMap::ConfigureInputsForDataSource(uint32 nSamples, uint32 nChannels, uint64* inputTimestampAddress, uint8** signalAddresses, TypeDescriptor* signalTypes){
    bool ok = UEIMapContainer::ConfigureInputsForDataSource(nSamples, nChannels, inputTimestampAddress, signalAddresses, signalTypes);
    if (ok){
        ok = (nSamples != 0u);
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid provided sample number to be read from Map %s", name.Buffer());
        }
        nReadSamples = nSamples;
    }
    signalsConfigured = ok;
    return ok;
}

bool UEIRtVMap::StartMap(){
    bool ok = true;
    //Now that the map is to be started it is valuable to make a copy of the DAQ handle reference for deallocating the map upon
    //destruction of this object.
    //First of all, set all the UEICircularBuffer objects for each of the VMap queried channels
    if (ok){
        //Traverse the input channel list to initialise each UEICircularBuffer object
        for (uint32 i = 0; i < nInputMembers && ok; i++){
            uint32 thisMemberChannelN = inputMembersOrdered[i]->Inputs.nChannels;
            ReferenceT<UEIDevice> thisDev = inputMembersOrdered[i]->reference;
            ok = thisDev.IsValid();
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve reference for device devn%d in Map %s", i, name.Buffer());
            }
            if (ok){
                ok = (inputMembersOrdered[i]->Inputs.buffer->InitialiseBuffer(nBuffers, thisMemberChannelN, sampleNumber, thisDev->GetSampleSize(), nReadSamples, inputMembersOrdered[i]->Inputs.timestampRequired));
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "The initilisation of DAQCiruclarBuffer failed for Map %s", name.Buffer());
                }
            }
        }
    }
    //Initialise the map always to retrieve fragmented packets, has no impact in data retrieving except if not used
    if (ok){
        DQ_RTMAP_PARAM vmapparam = {11500, 1518, 1000};  //This parameters are the maximum allowed
        ok = (DqRtVmapInitEx(DAQ_handle, &mapid, &vmapparam) >= 0); //Extended version of the VMapInit method for fragmented maps
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error on Initialising Map %s", name.Buffer());
        }
    }
    if (ok){
        //Once the map is initialized correctly, the I/O channels need to be checked into the map itself
        //First the input channels are checked. The inputMembersOrdered pointer array is traversed in order,
        // yielding the ordered list of channels to sequence into the input map
        for (uint32 i = 0u; i < nInputMembers && ok; i++){
            ReferenceT<UEIDevice> devReference = inputMembersOrdered[i]->reference;
            //The first channel of the first device in the map must be the timestamp
            uint32 nChannels_ = inputMembersOrdered[i]->Inputs.nChannels;
            nChannels_ = nChannels_;
            uint8 offset = 0u;
            if (inputMembersOrdered[i]->Inputs.timestampRequired){
                nChannels_ += 1;  
            }                
            int32 channels [nChannels_];
            int32 flags [nChannels_];
            uint8 devn = inputMembersOrdered[i]->devn;
            //if the timestamp is required, set the first channel and its flags
            if (inputMembersOrdered[i]->Inputs.timestampRequired){
                channels[0] = DQ_LNCL_TIMESTAMP;
                flags[0] = DQ_VMAP_FIFO_STATUS;
                offset = 1u;    //Set the offset for the channel-confgiruation loop
            }
            for (uint32 j = offset ; j < nChannels_ && ok; j++){
                channels[j] = (int32)(inputMembersOrdered[i]->Inputs.channels[j-offset]);
                flags[j] = DQ_VMAP_FIFO_STATUS;
                int32 channel = channels[j];
                ok = (devReference->ConfigureChannel(channel, channels[j]));
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Could not configure channels in inputMember %i on Map %s", i, name.Buffer());
                }
            }
            if (ok){
                ok = (DqRtVmapAddChannel(DAQ_handle, mapid, devn, DQ_SS0IN, channels, flags, 1) >= 0);   //TODO add support for FIFO functions
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Could not add channels from inputMember %i on Map %s", i, name.Buffer());
                }
            }
            if (ok){
                //In the case of AI, AO, AIO or DIO, the channels are interleaved into a signle FIFO, we need to check the number of channels into
                //the map
                if (devReference->GetType() == HARDWARE_LAYER_ANALOG_I  || devReference->GetType() == HARDWARE_LAYER_ANALOG_O  ||
                    devReference->GetType() == HARDWARE_LAYER_ANALOG_IO || devReference->GetType() == HARDWARE_LAYER_DIGITAL_IO){
                    //Check th channel number into the map
                    ok = (DqRtVmapSetChannelList(DAQ_handle, mapid, devn, DQ_SS0IN, channels, nChannels_) >=0);
                    //Set the scan rate for the device on this channel
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Could set channel list in inputMember %i on Map %s", i, name.Buffer());
                    }
                    //Set the scan rate of the hardware layer to the sampling rate of the device
                    if (ok){
                        ok = (DqRtVmapSetScanRate(DAQ_handle, mapid, devn, DQ_SS0IN, sampleRate) >= 0);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "Could not set scan rate in inputMember %i on Map %s", i, name.Buffer());
                        }
                    }    
                }
            }
        }
    }
    //TODO -> Output signals for VMAP
    //Let's start the layers to start acquiring data
    if (ok){
        ok = EnableMap();
    }    
    return ok;
}

bool UEIRtVMap::PollForNewPacket(MapReturnCode& outputCode){
    bool ok = (mapStarted && signalsConfigured);
    if (!ok){
        REPORT_ERROR(ErrorManagement::InitialisationError, "VMap %s must be enabled and input signals configured before polling", name.Buffer());
        outputCode = ERROR;
    }
    if (ok){
        //Refresh the VMap
        int32 refreshReturn = 0;
        refreshReturn = DqRtVmapRefresh(DAQ_handle, mapid, 0);
        ok = (refreshReturn == DQ_SUCCESS);
        if (ok){
            //If the map was correclty refreshed we proceed to reading the samples contained in the VMap structure
            //In this case, since we received an update on the VMap content, feed it to each of the UEICircularBuffers
            for (uint32 i = 0; i < nInputMembers; i++){
                //For each of the input memebrs (in order of assignment) feed the new data into the buffers.
                //Retrieve the input address for the circularBuffer to write new data
                uint8* bufferWriteAddress = inputMembersOrdered[i]->Inputs.buffer->writePointer;
                //Prior to any write, check that the buffer has enough space to accept the new data
                if (inputMembersOrdered[i]->Inputs.buffer->CheckAvailableSpace()){
                    int32 dataWritten = 0;
                    int32 avl_size = 0;
                    uint32 requestedPacketSize = inputMembersOrdered[i]->Inputs.requestSize;
                    ok = (DqRtVmapGetInputData(DAQ_handle, mapid, i, requestedPacketSize, &dataWritten, &avl_size, bufferWriteAddress) >= 0);
                    if (ok){
                        inputMembersOrdered[i]->Inputs.buffer->AdvanceBufferIndex(dataWritten);
                    }else{
                        REPORT_ERROR(ErrorManagement::CommunicationError, "Error while writting data to the circular buffer");
                    }
                }else{ 
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Error while writting to circular buffer, not enough space available");
                }
            }
        }else{
            if (refreshReturn == DQ_FIFO_OVERFLOW){
                outputCode = FIFO_OVERFLOW; //Signal that an error occurred during map refresh due to FIFO overflow
                //Upon FIFO overflow for the map, the map must be restarted prior to getting any new samples, otherwise the
                //refresh method will not return appropriately
                REPORT_ERROR(ErrorManagement::CommunicationError, "Device FIFO overflow for Map %s VMAP, restarting the map", name.Buffer());
            }else{
                outputCode = ERROR; //Signal that an error occurred during map refresh due to other kind of errors
                //The refresh method failed for other reasons, report the error and carry on.
                REPORT_ERROR(ErrorManagement::CommunicationError, "Refresh of VMap for Map %s failed, restarting the map", name.Buffer());
            }
            bool restartOk = (ResetVMap());    //Reset the map for the next iteration
            if (!restartOk){
                REPORT_ERROR(ErrorManagement::CommunicationError, "Could not restart the Map %s succesfully", name.Buffer());
            }else{
                //Id the Map refresh is successful, then the circular buffers must be reset, all the stored samples are discarded
                //If not done subsequent FIFO overflows could force a circular buffer to run out of space and throw errors or samples
                for (uint8 i = 0; i < nInputMembers && restartOk; i++){
                    //in this loop, if ok = false, then at least one of the buffers is not reaUEIRtVMapdy yet
                    restartOk = (inputMembersOrdered[i]->Inputs.buffer->ResetBuffer());
                }
                if (!restartOk){
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Error while resetting circular buffers on Map %s", name.Buffer());
                }
            }
        }
    }
    //Once the refresh is done and data written into the circularBuffer Structure, check if all the buffers are ready to be read, if not
    //we need to wait for next refresh to fill the ones not ready (next poll operation).
    if (ok){
        //Check if all the buffers are ready
        bool newData = true;
        for (uint8 i = 0; i < nInputMembers && newData; i++){
            //in this loop, if ok = false, then at least one of the buffers is not reaUEIRtVMapdy yet
            newData &= (inputMembersOrdered[i]->Inputs.buffer->CheckReadReady());
        }
        //if ok, then all the buffers are ready to deliver the samples needed
        if (newData){
            outputCode = NEW_DATA_AVAILABLE;    //flag meaning that a complete packet is ready, deliver it
            //The destinationMemory (pointer) points to the memory region where the samples must be written
            //It is mandatory that the first MARTe signal is a Timestamp signal with 64 bit precision, therefore,
            //the destination memory (data channels) does indeed start padded a total of nReadSamples*8 bytes from the
            //DataSource memory location.
            //Flag signaling if the timestamp has already been recived from the buffer (typically only the first member
            // should contain a timestamp).
            bool timestampAcquired = false;
            uint32 signalIdx = 0;
            UEIBufferPointer timestampPointer;
            for (uint8 i = 0; i < nInputMembers && ok; i++){
                if (inputMembersOrdered[i]->Inputs.timestampRequired && !timestampAcquired){
                    timestampPointer = inputMembersOrdered[i]->Inputs.buffer->ReadTimestamp(ok);
                    timestampAcquired = true;
                }
                UEIBufferPointer* BufferPointersInMember = inputMembersOrdered[i]->Inputs.buffer->ReadBuffer(ok);
                if (ok){
                    //First check out the current buffer
                    ok &= inputMembersOrdered[i]->Inputs.buffer->CheckoutBuffer();
                    //Scale the obtained data
                    for (uint32 j = 0u; j < inputMembersOrdered[i]->Inputs.nChannels; j++){
                        //Extract the configuration for this member data scale.
                        uint32 channel = inputMembersOrdered[i]->Inputs.channels[j];
                        UEIBufferPointer RawPointer = BufferPointersInMember[j];
                        void* scaledDataPointer = reinterpret_cast<void*>(inputSignalAddresses[signalIdx]);
                        TypeDescriptor thisOutputType = inputSignalTypes[signalIdx];
                        ok = (inputMembersOrdered[i]->reference->ScaleSignal(channel, nReadSamples, RawPointer, scaledDataPointer, thisOutputType));
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::CommunicationError, "The scaling process failed in Map %s", name.Buffer());
                        }
                    }
                    signalIdx++;
                }
                if (!ok){
                    outputCode = ERROR;
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Could not retrieve data from CircualrBuffer in Map %s", name.Buffer());
                }
            }
            //Correct and set in memory the timestamps
            if (ok){
                ok = (timestampAcquired);
                if (ok){
                    ok = GetTimestamp(timestampPointer, nReadSamples, TimestampAddr);
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::CommunicationError, "Could not correct appropiately the timestamps retrieved in Map %s", name.Buffer());
                    }
                }else{
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Could not retrieve timestamp in Map %s", name.Buffer());
                } 
            }
        }else{
            outputCode = NO_NEW_DATA_AVAILABLE;
        }
    }
    return ok;
}

MapType UEIRtVMap::GetType(){
    return RTVMAP;
}



bool UEIRtVMap::ResetVMap(){
    bool ok = StopMap();
    //Stop the map (don't call cleanup map, as it would completely close it)
    ok &= EnableMap();
    if (!ok){
        REPORT_ERROR(ErrorManagement::CommunicationError, "Could not re-start Map %s during map reset request", name.Buffer());
    }
    return ok;
}

bool UEIRtVMap::EnableMap(){
    bool ok = true;
    if (ok){
        ok = (DqRtVmapStart(DAQ_handle, mapid) >= 0);
        if (!ok){
            REPORT_ERROR(ErrorManagement::CommunicationError, "Could not start Map %s", name.Buffer());
        }
    }
    //Now set how many samples do we wish to recieve for this devices channels (this needs to be done AFTER map start, otherwise, you guessed it -> segfault)
    if (ok){
        for (uint32 i = 0u; i < nInputMembers && ok; i++){
            ReferenceT<UEIDevice> devReference = inputMembersOrdered[i]->reference;
            ok = (devReference.IsValid());  //This should not be necessary, but it is implemented for precaution
            if (ok){
                uint32 nChannels_ = inputMembersOrdered[i]->Inputs.nChannels;
                uint32 byteSize = devReference->GetSampleSize();
                uint32 requestSize = nChannels_*sampleNumber*byteSize;
                if (inputMembersOrdered[i]->Inputs.timestampRequired){
                    requestSize += 4u*sampleNumber;   //Add a fictional channel for timestamp
                }
                //Assign the requested number of bytes on this member for later access during Refresh
                inputMembersOrdered[i]->Inputs.requestSize = requestSize;
                int32 act_size;
                //With this method we set the ammount of samples we want to obtain from this member's device
                ok = (DqRtVmapRqInputDataSz(DAQ_handle, mapid, i, requestSize , &act_size, NULL) >= 0);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error setting the VMap size on member %d in Map %s", i, name.Buffer());
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Requested size was %d bytes, but only %d bytes available", requestSize, act_size);
                }
            }else{
                REPORT_ERROR(ErrorManagement::InitialisationError, "could not retrieve devReference for device %d on Map %s while setting sample number", i, name.Buffer());
            }
        }
    }
    mapStarted = ok;
    return ok;
}

CLASS_REGISTER(UEIRtVMap, "1.0")
}