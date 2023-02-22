/**
 * @file DAQMapContainer.cpp
 * @brief Source file for class DAQMapContainer
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
 * the class DAQMapContainer (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "RegisteredMethodsMessageFilter.h"
#include "DAQMapContainer.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

DAQMapContainer::DAQMapContainer() : ReferenceContainer() {
    mapType = 0u;
    outputAssignedToDS  = false;
    inputAssignedToDS   = false;
    mapid = 0u;
    inputMapPtr = NULL_PTR(uint32*);
    nInputChannels = 0u;
    nOutputChannels = 0u;
    DAQ_handle = 0;
    poll_sleep_period = 200u;
    //Set the members of the map to a non-set state for safety
    for (uint32 i = 0u; i < MAX_IO_SLOTS; i++){
        //Set the member as not defined
        members[i].defined = false;
        //Set the inputs of the member as not defined
        members[i].Inputs.defined = false;
        members[i].Inputs.nChannels = 0u;
        members[i].Inputs.channels = NULL_PTR(uint32*);
        //Set the outputs of the memeber as not defined
        members[i].Outputs.defined = false;
        members[i].Outputs.nChannels = 0u;
        members[i].Outputs.channels = NULL_PTR(uint32*);
    }
}

DAQMapContainer::~DAQMapContainer(){
    if (inputMapPtr != NULL_PTR(uint32*)){
        free(inputMapPtr);
    }
    //try to clean the maps in case it was not already done beforehand
    CleanupMap();
}

bool DAQMapContainer::CleanupMap(){
    bool ok = true;
    if (DAQ_handle != 0){
        REPORT_ERROR(ErrorManagement::Information, "DAQMapContainer::CleanupMap - "
        "Cleaning Map %s", name.Buffer());
        switch (mapType){
            case RTDMAP:
                if(mapid){
                    ok &= (DqRtDmapStop(DAQ_handle, mapid) >= 0);
                    ok &= (DqRtDmapClose(DAQ_handle, mapid) >= 0);
                }
            break;
            case RTVMAP:
                if(mapid){
                    ok &= (DqRtVmapStop(DAQ_handle, mapid) >= 0);
                    ok &= (DqRtVmapClose(DAQ_handle, mapid) >= 0);
                }
            break;
        }
        DAQ_handle = 0;
    }else{
        REPORT_ERROR(ErrorManagement::Information, "DAQMapContainer::CleanupMap - "
        "Map %s was not started, skipping cleanup!", name.Buffer());
    }
    return ok;
}

bool DAQMapContainer::Initialise(StructuredDataI &data){
    bool ok = ReferenceContainer::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Check the name of the Object
    if (ok) {
        name = data.GetName();
        ok = (name.Size() != 0ull);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                "Could not retrieve DAQ Map Container Name.");
        }
    }
    //Check Map Type
    StreamString MapType;
    if (ok) {
        ok = data.Read("Type", MapType);
        if(ok){
            //Type of the map is defined
            StreamString rtDMap = StreamString("RtDMap");
            StreamString rtVMap = StreamString("RtVMap");
            if(MapType == rtDMap){
                mapType = RTDMAP;
            }else if (MapType == rtVMap){
                mapType = RTVMAP;
            }else{
                ok = false;
                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                "Unrecognized/unsupported map type for map %s.", name.Buffer());
            }
        }else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
            "Could not retrieve DAQ Map Type for map %s.", name.Buffer());
        }
    }
    //Read Scan Rate
    if (ok){
        ok = data.Read("ScanRate", scanRate);
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
            "Could not retrieve ScanRate for map %s.", name.Buffer());            
        }
    }
    //Variables to control wether the Inputs and/or Outputs blocks have been defined
    //at least one of such blocks must be defined for a valid MapContainer
    bool outputSignalsDefined   = false;
    bool inputSignalsDefined    = false;
    //Read devices information for output signals (the ones coming from IOM)
    //Check if any output signals are defined
    if (ok){
        //Check if there's Outputs section in the configuration
        outputSignalsDefined = data.MoveRelative("Outputs");
        if (outputSignalsDefined){
            //Get and check the number of devices defined into the Devices subsection
            nDevices = data.GetNumberOfChildren();
            ok = (nDevices > 0u && nDevices < MAX_IO_SLOTS);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                "Invalid device number for map %s.", name.Buffer());    
            }
            //Assign the array of pointers to the members array for ordered configuration usage
            if (ok){
                //Save the number of output members (= nDevices)
                nOutputMembers = nDevices;
                //Assign memory
                outputMembersOrdered = new mapMember*[nDevices];
                ok = (outputMembersOrdered != NULL_PTR(mapMember**));
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                    "Could not allocate memory for outputMembersOrdered for map %s.", name.Buffer());     
                }
            }
            if (ok){
                //Traverse each of the devices leaf to list the device used and the channels within such device
                for (uint32 i = 0u; i < nDevices && ok; i++){
                    //Move to the device node
                    ok = data.MoveToChild(i);
                    StreamString node_name;
                    uint8 devn_ = 0u;
                    if (ok){
                        //get node name
                        node_name = data.GetName();
                    }
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                        "Could not retrieve device %s at Outputs block for map %s.", i, name.Buffer());                
                    }
                    if (ok){
                        //Get and check the device identifier for this map member
                        ok = data.Read("Devn", devn_);
                        ok &= (devn_>=0u && devn_<MAX_IO_SLOTS);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                            "Could not retrieve Devn for device %s  for map %s.", node_name.Buffer(), name.Buffer());  
                        }
                        //Check that this devn is not already used for a member of this map
                        if (ok){
                            ok = (!members[devn_].Outputs.defined);       //Check it the device has already been assigned within this map.
                            //used_devn = used_devn&(~(0x0001<<(devn_)));  //Set the device at bit (devn) to 0 to signal it has been assigned in this map.
                            if (!ok){
                                //The device devn is already defined for another member of this map
                                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                                "devn %d is repeated within map %s (at device %s in Outputs block).", devn_, name.Buffer(), node_name.Buffer()); 
                            }
                            if (ok){
                                //Device is not repeated, check if it is within allowed devn limits
                                ok = (devn_ < MAX_IO_SLOTS);
                                if (ok){
                                    //Mark this device (member) as needed by the map
                                    members[devn_].defined = true;
                                    //Mark this device (member) as needing output signals by the map
                                    members[devn_].Outputs.defined = true;
                                    //Save the pointer to this member in ordered fashion
                                    outputMembersOrdered[i] = &members[devn_];
                                    members[devn_].devn = devn_;
                                }
                                if (!ok){
                                    REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                                    "Invalid devn %d in map %s (at device %s in Outputs block).", devn_, name.Buffer(), node_name.Buffer());
                                }
                            }
                        }
                    }
                    if (ok){
                        //Write the requested channels for this map member, check on such information is delegate to the DAQMasterObject
                        //As hardware layer-type is not provided to MapContainer Object.
                        ok = helper.ReadArray("Channels", members[devn_].Outputs.channels, members[devn_].Outputs.nChannels);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                            "Could not retrieve Channels for device number %d for map %s.", i, name.Buffer());
                        }
                        if (ok){
                            nOutputChannels += members[devn_].Outputs.nChannels;
                        }
                    }
                    //Move back to "Outputs" leaf
                    ok &= data.MoveToAncestor(1u);
                }     
            }
            //Move back to "Map" leaf
            ok &= data.MoveToAncestor(1u);
            if (ok){
                REPORT_ERROR(ErrorManagement::Information, "DAQMapContainer::Initialise - "
                "%s map initialised with %d devices.", name.Buffer(), nDevices);
            }
        }

        if (!outputSignalsDefined){
            REPORT_ERROR(ErrorManagement::Information, "DAQMapContainer::Initialise - "
            "No output signals defined for map %s.", name.Buffer());
        }

    }
    //Check if any input signals are defined
    if (ok){
        //Check if there's Inputs section in the configuration
        inputSignalsDefined = data.MoveRelative("Inputs");
        if (inputSignalsDefined){
            //Get and check the number of devices defined into the Devices subsection
            nDevices = data.GetNumberOfChildren();
            ok = (nDevices > 0u && nDevices < MAX_IO_SLOTS);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                "Invalid device number for map %s.", name.Buffer());    
            }
            //Assign the array of pointers to the members array for ordered configuration usage
            if (ok){
                //Save the number of input members (= nDevices)
                nInputMembers = nDevices;
                inputMembersOrdered = new mapMember*[nDevices];
                ok = (inputMembersOrdered != NULL_PTR(mapMember**));
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                    "Could not allocate memory for inputMembersOrdered for map %s.", name.Buffer());     
                }
            }
            if (ok){
                //Traverse each of the devices leaf to list the device used and the channels within such device
                for (uint32 i = 0u; i < nDevices && ok; i++){
                    //Move to the device node
                    ok = data.MoveToChild(i);
                    StreamString node_name;
                    uint8 devn_ = 0u;
                    if (ok){
                        //get node name
                        node_name = data.GetName();
                    }
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                        "Could not retrieve device %s at Inputs block for map %s.", i, name.Buffer());                
                    }
                    if (ok){
                        //Get and check the device identifier for this map member
                        ok = data.Read("Devn", devn_);
                        ok &= (devn_>=0u && devn_<MAX_IO_SLOTS);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                            "Could not retrieve Devn for device %s for map %s.", node_name.Buffer(), name.Buffer());  
                        }
                        //Check that this devn is not already used for a member of this map
                        if (ok){
                            ok = (!members[devn_].Inputs.defined);       //Check it the device has already been assigned within this map.
                            if (!ok){
                                //The device devn is already defined for another member of this map
                                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                                "devn %d is repeated within map %s (at device %s in Inputs block).", devn_, name.Buffer(), node_name.Buffer()); 
                            }
                            if (ok){
                                //Device is not repeated, chec if it is within allowed devn limits
                                ok = (devn_ < MAX_IO_SLOTS);
                                if (ok){
                                    //Mark this device (member) as needed by the map
                                    members[devn_].defined = true;
                                    //Mark this device (member) as needing output signals by the map
                                    members[devn_].Inputs.defined = true;
                                    //Save the pointer to this member in ordered fashion
                                    inputMembersOrdered[i] = &members[devn_];
                                    members[devn_].devn = devn_;
                                }
                                if (!ok){
                                    REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                                    "Invalid devn %d in map %s (at device %s in Inputs block).", devn_, name.Buffer(), node_name.Buffer());
                                }
                            }
                        }
                    }
                    if (ok){
                        //Write the requested channels for this map member, check on such information is delegate to the DAQMasterObject
                        //As hardware layer-type is not provided to MapContainer Object.
                        ok = helper.ReadArray("Channels", members[devn_].Inputs.channels, members[devn_].Inputs.nChannels);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                            "Could not retrieve Channels for device number %d for map %s.", i, name.Buffer());
                        }
                        if (ok){
                            nInputChannels += members[devn_].Inputs.nChannels;
                        }
                    }
                    //Move back to "Inputs" leaf
                    ok &= data.MoveToAncestor(1u);
                }
            }
            if (ok){
                REPORT_ERROR(ErrorManagement::Information, "DAQMapContainer::Initialise - "
                "%s map initialised with %d devices.", name.Buffer(), nDevices);
            }
            //Move back to "Map" node
            ok &= data.MoveToAncestor(1u);
        }
        if (!inputSignalsDefined){
            REPORT_ERROR(ErrorManagement::Information, "DAQMapContainer::Initialise - "
            "No input signals defined for map %s.", name.Buffer());
        }
    }
    //Check if the Inputs/Outputs configuration blocks are defined.
    if (ok){
        ok = (outputSignalsDefined || inputSignalsDefined);
        if (!ok){
            //No Inputs and Outputs signal blocks are defined. The MapContainer cannot be empty
            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
            "No Inputs or Outputs blocks defined for map %s.", name.Buffer());
        }
        //Set the input/outputAssignedToDs variables to the value of input/outputSignalsDefined value.
        inputAssignedToDS = inputSignalsDefined;
        outputAssignedToDS = outputSignalsDefined;
    }

    //Acknowledge the successful initialisation of the Object
    if (ok){
        REPORT_ERROR(ErrorManagement::Information, "DAQMapContainer::Initialise - "
        "Successful initialisation of map %s.", name.Buffer());
    }
    return ok;
}

bool DAQMapContainer::GetDevDefined(uint32 devn){
    bool defined = false;
    if (devn< MAX_IO_SLOTS){
        defined = members[devn].defined;
    }
    return defined;
}

bool DAQMapContainer::GetDevDefined(uint32 devn, uint8 direction){
    bool defined = false;
    if (devn<MAX_IO_SLOTS){
        if (direction == INPUT_CHANNEL){
            defined = members[devn].Inputs.defined;
        }else if (direction == OUTPUT_CHANNEL){
            defined = members[devn].Outputs.defined;
        }else{
            defined = false;
        }
    }
    return defined;
}

bool DAQMapContainer::GetNumberOfChannels(uint32 devn, uint8 direction, uint32* nChannels){
    bool valid = false;
    if (devn<MAX_IO_SLOTS){
        if (members[devn].defined == true){
            if (direction == INPUT_CHANNEL){
                *nChannels = members[devn].Inputs.nChannels;
                valid = true;
            }else if (direction == OUTPUT_CHANNEL){
                *nChannels = members[devn].Outputs.nChannels;
                valid = true;
            }
        }
    }
    return valid;
}

bool DAQMapContainer::GetChannelOfMember(uint32 devn, uint8 direction, uint32 channelIdx, uint32* channelNumber){
    bool valid = false;
    if (devn<MAX_IO_SLOTS){
        if (members[devn].defined){
            if (direction == INPUT_CHANNEL && (channelIdx<members[devn].Inputs.nChannels)){
                *channelNumber = members[devn].Inputs.channels[channelIdx];
                valid = true;
            }else if (direction == OUTPUT_CHANNEL && (channelIdx<members[devn].Outputs.nChannels)){
                *channelNumber = members[devn].Outputs.channels[channelIdx];
                valid = true;
            }
        }
    }
    return valid;
}

bool DAQMapContainer::SetDevReference(uint32 devn, ReferenceT<UEIAI217_803> reference){ //TODO
    bool ok = false;
    if (members[devn].defined){
        if (reference.IsValid()){
            members[devn].reference = reference;
            ok = true;
        }
    }
    return ok;
}

float DAQMapContainer::GetScanRate(){
    return scanRate;
}

char8* DAQMapContainer::GetName(){
    return (char8*) name.Buffer();
}

bool DAQMapContainer::StartMap(int32 DAQ_handle_){
    bool ok = true;
    //Now that the map is to be started it is valuable to make a copy of the DAQ handle reference for deallocating the map upon
    //destruction of this object.
    DAQ_handle = DAQ_handle_;
    switch(mapType){
        case RTDMAP:
            ok = (DqRtDmapInit(DAQ_handle, &mapid, scanRate) >= 0);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error on Initialising Map %s", name.Buffer());
            }
            if (ok){
                //Once the map is initialized correctly, the I/O channels need to be checked into the map itself
                //First the output channels are checked. The outputMembersOrdered pointer array is traversed in order,
                // yielding the ordered list of channels to sequence into the output map
                for (uint32 i = 0u; i < nOutputMembers && ok; i++){
                    ReferenceT<UEIAI217_803> devReference = outputMembersOrdered[i]->reference;
                    ok = (devReference.IsValid());  //This should not be necessary, but it is implemented for precaution
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Found invalid device reference on outputMember %i on Map %s", i, name.Buffer());
                    }
                    if (ok){
                        for (uint32 j = 0; j < outputMembersOrdered[i]->Outputs.nChannels && ok; j++){
                            uint32 channel = outputMembersOrdered[i]->Outputs.channels[j];
                            uint8 devn = outputMembersOrdered[i]->devn;
                            ok = (devReference->ConfigureChannel(&channel));
                            if (ok){
                                ok = (DqRtDmapAddChannel(DAQ_handle, mapid, devn, DQ_SS0IN, &channel, 1));
                            }
                            if (!ok){
                                REPORT_ERROR(ErrorManagement::InitialisationError, "Error adding output channels for dev%d on Map %s", devn, name.Buffer());
                            }
                        }
                    }
                }
                
            }
            if (ok){
                //Once the map is initialized correctly, the I/O channels need to be checked into the map itself
                //First the input channels are checked. The inputMembersOrdered pointer array is traversed in order,
                // yielding the ordered list of channels to sequence into the input map
                for (uint32 i = 0u; i < nInputMembers && ok; i++){
                    ReferenceT<UEIAI217_803> devReference = inputMembersOrdered[i]->reference;
                    ok = (devReference.IsValid());  //This should not be necessary, but it is implemented for precaution
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Found invalid device reference on inputMember %i on Map %s", i, name.Buffer());
                    }
                    if (ok){
                        for (uint32 j = 0; j < inputMembersOrdered[i]->Inputs.nChannels && ok; j++){
                            uint32 channel = inputMembersOrdered[i]->Inputs.channels[j];
                            uint8 devn = inputMembersOrdered[i]->devn;
                            ok = (devReference->ConfigureChannel(&channel));
                            if (ok){
                                ok = (DqRtDmapAddChannel(DAQ_handle, mapid, devn, DQ_SS0OUT, &channel, 1));
                            }
                            if (!ok){
                                REPORT_ERROR(ErrorManagement::InitialisationError, "Error adding input channels for dev%d on Map %s", devn, name.Buffer());
                            }
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
        break;
        case RTVMAP:
            ok = (DqRtVmapInit(DAQ_handle, &mapid, scanRate) >= 0);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error on Initialising Map %s", name.Buffer());
            }
            if (ok){
                //Once the map is initialized correctly, the I/O channels need to be checked into the map itself
                //First the output channels are checked. The outputMembersOrdered pointer array is traversed in order,
                // yielding the ordered list of channels to sequence into the output map
                for (uint32 i = 0u; i < nOutputMembers && ok; i++){
                    ReferenceT<UEIAI217_803> devReference = outputMembersOrdered[i]->reference;
                    ok = (devReference.IsValid());  //This should not be necessary, but it is implemented for precaution
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Found invalid device reference on outputMember %i on Map %s", i, name.Buffer());
                    }
                    if (ok){
                        int32 channels [outputMembersOrdered[i]->Outputs.nChannels];
                        uint8 devn = outputMembersOrdered[i]->devn;
                        for (uint32 j = 0; j < outputMembersOrdered[i]->Outputs.nChannels && ok; j++){
                            channels[j] = (int32)(outputMembersOrdered[i]->Outputs.channels[j]);
                            ok = (devReference->ConfigureChannel(&channels[j]));
                        }
                        if (ok){
                            ok = (DqRtVmapAddChannel(DAQ_handle, mapid, devn, DQ_SS0IN, channels, 0, 1));   //TODO add support for FIFO functions
                        }
                        if (ok){
                            //In the case of AI, AO, AIO or DIO, the channels are interleaved into a signle FIFO, we need to check the number of channels into
                            //the map
                            if (devReference->GetType() == HARDWARE_LAYER_ANALOG_I  ||
                                devReference->GetType() == HARDWARE_LAYER_ANALOG_O  ||
                                devReference->GetType() == HARDWARE_LAYER_ANALOG_IO ||
                                devReference->GetType() == HARDWARE_LAYER_DIGITAL_IO){
                                //Check th channel number into the map
                                int32 mapNumberOfChannels = (int32)outputMembersOrdered[i]->Outputs.nChannels;
                                ok = (DqRtVmapSetChannelList(DAQ_handle, mapid, devn,DQ_SS0IN, channels, mapNumberOfChannels) >=0);
                                //Set the scan rate for the device on this channel
                                if (ok){
                                    ok = DqRtVmapSetScanRate(DAQ_handle, mapid, devn, DQ_SS0IN, scanRate);
                                }    
                            }
                        }
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "Error adding output channels for dev%d on Map %s", devn, name.Buffer());
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
                if (ok){
                    //Try to acquire the memory map pointers
                //    ok = GetMapPointers();
                //    if (!ok){
                //        REPORT_ERROR(ErrorManagement::CommunicationError, "Could not retrieve the memory map pointers for Map %s", name.Buffer());
                //    }
                }
            }
        break;
        default:
            ok = false;
            REPORT_ERROR(ErrorManagement::InitialisationError, "Map type still not supported on Map %s", name.Buffer());
        break;
    }
    return ok;
}

bool DAQMapContainer::GetMapPointers(){
    //Function just valid for output signals (the ones enternig into IOM) TODO extended to input maps
    unsigned char* map_pointer = NULL;
    uint16 counter = 0u;
    bool ok = true;
    while((((uint32*) map_pointer) == NULL)&& ok){
        counter++;
        ok = (DqRtDmapRefresh(DAQ_handle, mapid) >= 0);
        if (ok){
            Sleep::MSec(10);
            ok = (DqRtDmapGetInputMap(DAQ_handle, mapid, 0, &(map_pointer)) >=0);
            if (!ok){
                REPORT_ERROR(ErrorManagement::CommunicationError, "GetOutputMap failed during the GetMapAddr method");   
            }
        }else{
            REPORT_ERROR(ErrorManagement::CommunicationError, "Refresh failed during the GetMapAddr method");
        }
        if (counter > 200) break; //If NULL map is still present during 2s abort the initialisation of the DS
    }
    if (((uint32*) map_pointer) != NULL){
        outputMap = ((uint32*)map_pointer);
        return true;
    }else{
        return false;
    }
    return ok;
}

bool DAQMapContainer::PollForNewPacket(float64* destinationAddr, uint32 nChannels){
    bool next_packet = false;
    bool ok = true;
    bool copy_done = false;
    uint32 iterator = 0u;
    uint32 nSamples = nOutputChannels; //For now, to be changed
    //Poll for next packet from UEIDAQ
    ok = (DqRtDmapRefresh(DAQ_handle, mapid) >= 0);
    if (ok){
        //Check if the response is a new packet or a rerequest.
        //The 0x80000000 bit on the recived samples lets us know if the packet has
        //been previously requested.
        if (*((uint32*)outputMap) & 0x80000000){ 
            uint32 mask = 0x00FFFFFF; 
            //The recived packet is a newly converted one not requested yet.
            //Make the signals available to the broker.
            //TODO implement this using memcopy
            for (uint32 mem = 0; mem < nOutputMembers && ok && !copy_done; mem++){
                //min function
                uint32 samples_to_copy = !(nChannels<outputMembersOrdered[mem]->Outputs.nChannels)?outputMembersOrdered[mem]->Outputs.nChannels:nChannels;
                //Copy the scaled values obtained in the hardware layer into the destination buffer
                ok = (DqRtDmapReadScaledData(DAQ_handle, mapid, outputMembersOrdered[mem]->devn, &destinationAddr[iterator], samples_to_copy) >= 0);
                nChannels -= samples_to_copy;
                iterator += samples_to_copy;
                //The channels requested have already been copied, stop the loop
                if (nChannels == 0u){
                    copy_done = true;
                }
                if (!ok){
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Error while translating the channels to scaled values on Map %s.", name.Buffer());
                }
            }
            next_packet = true;
        }
    }else{
        REPORT_ERROR(ErrorManagement::ParametersError, "Refresh failed during Poll for conversion in Map %s", name.Buffer());
    }
    return next_packet;
}

bool DAQMapContainer::RegisterDS(SignalDirection direction){
    //
    bool ok = false;
    switch(direction){
        case OutputSignals:
            if (!outputAssignedToDS){
                outputAssignedToDS = true;
                ok = true;
            }
        break;
        case InputSignals:
            if (!inputAssignedToDS){
                inputAssignedToDS = true;
                ok = true;
            }
        break;
        default:
            ok = false;
        break;
    }
    return ok;
}

bool DAQMapContainer::IsSignalAllowed(TypeDescriptor signalType, uint8 direction){
    bool ok = true;
    if (direction == OUTPUT_CHANNEL){
        //Traverse the outputMembers in an ordered fashion
        for (uint32 i = 0u; i < nOutputMembers && ok; i++){
            //Check if the member is needed for the supplied signal
            ok = outputMembersOrdered[i]->reference->AcceptedSignalType(signalType);
        }
    }else{
        ok = false;
    }
    return ok;
}

bool DAQMapContainer::GetNumberOfChannels(uint8 direction, uint32 &nChannels){
    bool ok = true;
    nChannels = 0;
    switch(direction){
        case OUTPUT_CHANNEL:
            nChannels = nOutputChannels;
        break;
        case INPUT_CHANNEL:
            nChannels = nInputChannels;
        break;
        default:
            ok = false;
        break;
    }
    return ok;
}

CLASS_REGISTER(DAQMapContainer, "1.0")
}