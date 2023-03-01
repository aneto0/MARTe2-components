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


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UEIRtVMap::UEIRtVMap() : UEIMapContainer() {
}

UEIRtVMap::~UEIRtVMap(){
    //try to clean the maps in case it was not already done beforehand
    CleanupMap();
}

bool UEIRtVMap::CleanupMap(){
    bool ok = true;
    if (DAQ_handle != 0){
        REPORT_ERROR(ErrorManagement::Information, "UEIRtVMap::CleanupMap - "
        "Cleaning Map %s", name.Buffer());
        if(mapid){
            ok &= (DqRtVmapStop(DAQ_handle, mapid) >= 0);
            ok &= (DqRtVmapClose(DAQ_handle, mapid) >= 0);
        }
        DAQ_handle = 0;
    }else{
        REPORT_ERROR(ErrorManagement::Information, "UEIRtVMap::CleanupMap - "
        "Map %s was not started, skipping cleanup!", name.Buffer());
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
    //For the VMap, the circular buffers must be initialised prior to any usage
    if (ok){
        //Create a reference T to the new circular buffer for each device
        ClassRegistryDatabase *crdSingleton;
        const ClassRegistryItem *classRegistryItem;
        const ObjectBuilder* builder;
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Could not allocate memory for VMap Circular Buffer objects");
        }
        if (ok){
            crdSingleton = ClassRegistryDatabase::Instance();
            classRegistryItem = crdSingleton->Find("UEICircularBuffer");
            ok = (classRegistryItem != NULL); 
        }
        if (ok) {
            //Get the object builder (which knows how to build classes of this type).
            builder = classRegistryItem->GetObjectBuilder();
            ok = (builder != NULL);
        }
        if (ok){
            //Assign the CircularBuffer references to the Input members only TODO
            for (uint8 i = 0; i < nInputMembers && ok; i++){
                //Instantiate the objects and save the referenceT to them
                Object *obj = builder->Build(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                inputMembersOrdered[i]->Inputs.buffer = ReferenceT<UEICircularBuffer>(dynamic_cast<UEICircularBuffer*>(obj));
                ok = (inputMembersOrdered[i]->Inputs.buffer.IsValid());
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error while assigning the UEICircularBuffer object to Map members of Map %s", name.Buffer());
                }
            }
            //TODO buffering for Output signals
        }
    }    
    //Acknowledge the successful initialisation of the Object
    if (ok){
        REPORT_ERROR(ErrorManagement::Information, "UEIRtVMap::Initialise - "
        "Successful initialisation of map %s.", name.Buffer());
    }
    return ok;
}

bool UEIRtVMap::StartMap(int32 DAQ_handle_){
    bool ok = true;
    //Now that the map is to be started it is valuable to make a copy of the DAQ handle reference for deallocating the map upon
    //destruction of this object.
    DAQ_handle = DAQ_handle_;
    //First of all, set all the UEICircularBuffer objects for each of the VMap queried channels
    if (ok){
        //Traverse the input channel list to initialise each UEICircularBuffer object
        for (uint32 i = 0; i < nInputMembers && ok; i++){
            uint32 thisMemberChannelN = inputMembersOrdered[i]->Inputs.nChannels;
            ok = (inputMembersOrdered[i]->Inputs.buffer->InitialiseBuffer(3*sampleNumber, thisMemberChannelN, sampleNumber, 4u, 10u));  //TODO
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "The initilisation of DAQCiruclarBuffer failed for Map %s", name.Buffer());
            }
        }
    }
    if (ok){
        ok = (DqRtVmapInit(DAQ_handle, &mapid, 0) >= 0); //This scan rate is the rate at which the IOM refreshes the version of VMap (Bullshit, this scan rate is not valid)
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
            ok = (devReference.IsValid());  //This should not be necessary, but it is implemented for precaution
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Found invalid device reference on inputMember %i on Map %s", i, name.Buffer());
            }
            if (ok){
                //The first channel of the first device in the map must be the timestamp
                uint8 offset = (i == 0) ? 1u:0u;
                uint32 nChannels_ = inputMembersOrdered[i]->Inputs.nChannels;
                nChannels_ = nChannels_+offset;
                int32 channels [nChannels_];
                int32 flags [nChannels_];
                uint8 devn = inputMembersOrdered[i]->devn;
                if (i == 0u){
                    channels[0] = DQ_LNCL_TIMESTAMP;
                    flags[0] = DQ_VMAP_FIFO_STATUS;
                }
                for (uint32 j = offset ; j < nChannels_ && ok; j++){
                    channels[j] = (int32)(inputMembersOrdered[i]->Inputs.channels[j-offset]);
                    flags[j] = DQ_VMAP_FIFO_STATUS;
                    int32 channel = channels[j];
                    ok = (devReference->ConfigureChannel(channel, &channels[j]));
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
                    if (devReference->GetType() == HARDWARE_LAYER_ANALOG_I  ||
                        devReference->GetType() == HARDWARE_LAYER_ANALOG_O  ||
                        devReference->GetType() == HARDWARE_LAYER_ANALOG_IO ||
                        devReference->GetType() == HARDWARE_LAYER_DIGITAL_IO){
                        //Check th channel number into the map
                        ok = (DqRtVmapSetChannelList(DAQ_handle, mapid, devn,DQ_SS0IN, channels, nChannels_) >=0);
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
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error adding input channels for dev%d on Map %s", devn, name.Buffer());
                }
            }
        }
        
    }
    //TODO -> Output signals for VMAP

    //If we reach here with ok=true, then the channels are correctly placed into the V/Dmap sctructure
    //Let's start the layers to start acquiring data
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
            uint32 nChannels_ = inputMembersOrdered[i]->Inputs.nChannels;
            uint32 byteSize = devReference->GetSampleSize();
            //Assign the requested number of bytes on this member for later access during Refresh
            inputMembersOrdered[i]->Inputs.requestSize = nChannels_*sampleNumber*byteSize;
            if (ok){
                //With this method we set the ammount of samples we want to obtain from this member's device
                int32 act_size;
                ok = (DqRtVmapRqInputDataSz(DAQ_handle, mapid, i, nChannels_*sampleNumber*byteSize , &act_size, NULL) >= 0);
            }else{
                REPORT_ERROR(ErrorManagement::InitialisationError, "could not retrieve devReference for device %d on Map %s while setting sample number", i, name.Buffer());
            }
        }
    }
    return ok;
}

bool UEIRtVMap::PollForNewPacket(float64* destinationAddr){
    bool next_packet = false;
    bool ok = true;
    bool copy_done = false;
    //Refresh the VMap
    int32 refreshReturn = DqRtVmapRefresh(DAQ_handle, mapid, 0);
    ok = (refreshReturn == DQ_SUCCESS);
    if (ok){
        //If the map was correclty refreshed we proceed to reading the samples contained in the VMap structure
        REPORT_ERROR(ErrorManagement::Information, "Great!");
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
            //Upon FIFO overflow for the map, the map must be restarted prior to getting any new samples, otherwise the
            //refresh method will not return appropriately
            REPORT_ERROR(ErrorManagement::CommunicationError, "Device FIFO overflow for Map %s VMAP", name.Buffer());
        }else{
            //The refresh method failed for other reasons, report the error and carry on.
            REPORT_ERROR(ErrorManagement::CommunicationError, "Refresh of VMap for Map %s failed", name.Buffer());
        }
    }
    //Once the refresh is done and data written into the circularBuffer Structure, check if all the buffers are ready to be read, if not
    //we need to wait for next refresh to fill the ones not ready (next poll operation).
    if (ok){
        //Check if all the buffers are ready
        for (uint8 i = 0; i < nInputMembers && ok; i++){
            //in this loop, if ok = false, then at least one of the buffers is not reaUEIRtVMapdy yet
            ok = (inputMembersOrdered[i]->Inputs.buffer->CheckReadReady());
        }
        //if ok, then all the buffers are ready to deliver the samples needed
        if (ok){
            next_packet = true;
            for (uint8 i = 0; i < nInputMembers && ok; i++){
                //Write the data from the circular buffers into the input region of the DataSource
                ok = (inputMembersOrdered[i]->Inputs.buffer->ReadBuffer(reinterpret_cast<uint8*>(destinationAddr)));
            } 
        }
    }
    return next_packet;
}

uint8 UEIRtVMap::GetType(){
    return RTVMAP;
}

CLASS_REGISTER(UEIRtVMap, "1.0")
}