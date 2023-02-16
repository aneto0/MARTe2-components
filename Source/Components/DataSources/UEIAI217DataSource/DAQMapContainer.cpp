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
    mapid = 0u;
    inputMapPtr = NULL_PTR(uint32*);
    nInputChannels = 0u;
    nOutputChannels = 0u;
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
}
bool DAQMapContainer::CleanupMap(int32 DAQ_handle){
    bool ok = true;
    if(mapid){
        ok &= (DqRtDmapStop(DAQ_handle, mapid) >= 0u);
        ok &= (DqRtDmapClose(DAQ_handle, mapid) >= 0u);
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
        if (direction == INPUT_CHANNEL){
            *nChannels = members[devn].Inputs.nChannels;
            valid = true;
        }else if (direction == OUTPUT_CHANNEL){
            *nChannels = members[devn].Outputs.nChannels;
            valid = true;
        }
    }
    return valid;
}

bool DAQMapContainer::GetChannelOfMember(uint32 devn, uint8 direction, uint32 channelIdx, uint32* channelNumber){
    bool valid = false;
    if (devn<MAX_IO_SLOTS){
        if (direction == INPUT_CHANNEL && (channelIdx<members[devn].Inputs.nChannels)){
            *channelNumber = members[devn].Inputs.channels[channelIdx];
            valid = true;
        }else if (direction == OUTPUT_CHANNEL && (channelIdx<members[devn].Outputs.nChannels)){
            *channelNumber = members[devn].Outputs.channels[channelIdx];
            valid = true;
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

bool DAQMapContainer::StartMap(int32 DAQ_handle){
    bool ok = true;
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
        if (mapType == RTDMAP){
            ok = (DqRtDmapStart(DAQ_handle, mapid) >= 0);
        }else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "Map type still not supported on Map %s", name.Buffer());
        }
    }
    return ok;
}


CLASS_REGISTER(DAQMapContainer, "1.0")
}