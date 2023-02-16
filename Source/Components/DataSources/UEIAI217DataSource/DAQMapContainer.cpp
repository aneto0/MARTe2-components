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
            //if there's Outputs section access the Devices subsection
            ok = data.MoveRelative("Devices");
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                "Could not retrieve output devices container for map %s.", name.Buffer());    
            }
            if (ok){
                //Get and check the number of devices defined into the Devices subsection
                nDevices = data.GetNumberOfChildren();
                ok = (nDevices > 0u && nDevices < MAX_IO_SLOTS);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                    "Invalid device number for map %s.", name.Buffer());    
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
                        "Could not retrieve device %s for map %s.", node_name.Buffer(), name.Buffer());                
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
                                    members[devn_].Outputs.defined = true;
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
                    //Move back to "Devices" leaf
                    ok &= data.MoveToAncestor(1u);
                }
            }
            //Move back to "Outputs" leaf
            ok &= data.MoveToAncestor(1u);
            if (ok){
                REPORT_ERROR(ErrorManagement::Information, "DAQMapContainer::Initialise - "
                "%s map initialised with %d devices.", name.Buffer(), nDevices);
            }
            //Move back to "Map" node
            ok &= data.MoveToAncestor(1u);
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
            //if there's Inputs section access the Devices subsection
            ok = data.MoveRelative("Devices");
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                "Could not retrieve input devices container for map %s.", name.Buffer());    
            }
            if (ok){
                //Get and check the number of devices defined into the Devices subsection
                nDevices = data.GetNumberOfChildren();
                ok = (nDevices > 0u && nDevices < MAX_IO_SLOTS);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                    "Invalid device number for map %s.", name.Buffer());    
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
                        "Could not retrieve device %s for map %s.", node_name.Buffer(), name.Buffer());                
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
                                    members[devn_].Inputs.defined = true;
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
                    //Move back to "Devices" leaf
                    ok &= data.MoveToAncestor(1u);
                }
            }
            //Move back to "Inputs" leaf
            ok &= data.MoveToAncestor(1u);
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

uint8 DAQMapContainer::GetDevDefined(uint32 devn){
    return members[devn].defined;
}

CLASS_REGISTER(DAQMapContainer, "1.0")
}