/**
 * @file UEIMapContainer.cpp
 * @brief Source file for class UEIMapContainer
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
 * the class UEIMapContainer (public, protected, and private). Be aware that some
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
#include "UEIMapContainer.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UEIMapContainer::UEIMapContainer() : ReferenceContainer() {
    name = "";
    sampleRate = 0.0;
    scanRate = 0.0;
    outputAssignedToDS  = false;
    inputAssignedToDS   = false;
    mapid = 0u;
    nInputChannels = 0u;
    nOutputChannels = 0u;
    DAQ_handle = 0;
    //Set the members of the map to a non-set state for safety
    for (uint32 i = 0u; i < MAX_IO_SLOTS; i++){
        //Set the member as not defined
        members[i].defined = false;
        //Set the inputs of the member as not defined
        members[i].Inputs.defined = false;
        members[i].Inputs.nChannels = 0u;
        members[i].Inputs.channels = NULL_PTR(uint32*);
        members[i].Inputs.correctionIndexes = NULL_PTR(int8*);
        members[i].Inputs.requestSize = 0u;
        members[i].Inputs.timestampRequired = false;
        //Set the outputs of the memeber as not defined
        members[i].Outputs.defined = false;
        members[i].Outputs.nChannels = 0u;
        members[i].Outputs.channels = NULL_PTR(uint32*);
        members[i].Outputs.correctionIndexes = NULL_PTR(int8*);
        members[i].Outputs.requestSize = 0u;
        members[i].Outputs.timestampRequired = false;
    }
    outputMembersOrdered = NULL_PTR(mapMember**);
    inputMembersOrdered = NULL_PTR(mapMember**);
    nInputMembers = 0u;
    nOutputMembers = 0u;
    sampleNumber = 0u;
    timestampCorrector = 0u;
    lastTimestamp = 0u;
}

UEIMapContainer::~UEIMapContainer(){
    //Clean up the different data structures
    if (outputMembersOrdered != NULL_PTR(mapMember**)){
        outputMembersOrdered = NULL_PTR(mapMember**);
    }
    if (inputMembersOrdered != NULL_PTR(mapMember**)){
        inputMembersOrdered = NULL_PTR(mapMember**);
    }
}

//VIRTUAL METHODS TO BE REIMPLMENTED IN CHILD CLASSES

bool UEIMapContainer::CleanupMap(){
    return false;
}

bool UEIMapContainer::Initialise(StructuredDataI &data){
    bool ok = ReferenceContainer::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Check the name of the Object
    if (ok) {
        name = data.GetName();
        ok = (name.Size() != 0ull);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                "Could not retrieve DAQ Map Container Name.");
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
            nOutputMembers = data.GetNumberOfChildren();
            ok = (nOutputMembers > 0u && nOutputMembers < MAX_IO_SLOTS);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                "Invalid device number for map %s.", name.Buffer());    
            }
            //Assign the array of pointers to the members array for ordered configuration usage
            if (ok){
                //Assign memory
                outputMembersOrdered = new mapMember*[nOutputMembers];
                ok = (outputMembersOrdered != NULL_PTR(mapMember**));
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                    "Could not allocate memory for outputMembersOrdered for map %s.", name.Buffer());     
                }
            }
            if (ok){
                //Traverse each of the devices leaf to list the device used and the channels within such device
                for (uint32 i = 0u; i < nOutputMembers && ok; i++){
                    //Move to the device node
                    ok = data.MoveToChild(i);
                    StreamString node_name;
                    uint8 devn = 0u;
                    if (ok){
                        //get node name
                        node_name = data.GetName();
                    }
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                        "Could not retrieve device %s at Outputs block for map %s.", i, name.Buffer());                
                    }
                    if (ok){
                        //Get and check the device identifier for this map member
                        ok = data.Read("Devn", devn);
                        ok &= (devn>=0u && devn < MAX_IO_SLOTS);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                            "Could not retrieve Devn for device %s  for map %s.", node_name.Buffer(), name.Buffer());  
                        }
                    }
                    //Check that this devn is not already used for a member of this map
                    if (ok){
                        ok = (!members[devn].Outputs.defined);       //Check it the device has already been assigned within this map.
                        if (!ok){
                            //The device devn is already defined for another member of this map
                            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                            "devn %d is repeated within map %s (at device %s in Outputs block).", devn, name.Buffer(), node_name.Buffer()); 
                        }
                    }
                    if (ok){
                        //Device is not repeated, check if it is within allowed devn limits
                        ok = (devn < MAX_IO_SLOTS);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                            "Invalid devn %d in map %s (at device %s in Outputs block).", devn, name.Buffer(), node_name.Buffer());
                        }
                    }
                    if (ok){
                        //Read the data for this device
                        //Mark this device (member) as needed by the map
                        members[devn].defined = true;
                        //Mark this device (member) as needing output signals by the map
                        members[devn].Outputs.defined = true;
                        //Set the device number for this member
                        members[devn].devn = devn;
                        //Save the pointer to this member in ordered fashion
                        outputMembersOrdered[i] = &members[devn];
                        //Write the requested channels for this map member, check on such information is delegate to the UEIDevice
                        //As hardware layer-type is not provided to MapContainer Object.
                        ok = helper.ReadArray("Channels", members[devn].Outputs.channels, members[devn].Outputs.nChannels);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                            "Could not retrieve Channels for device number %d for map %s.", i, name.Buffer());
                        }
                        if (ok){
                            nOutputChannels += members[devn].Outputs.nChannels;
                        }
                    }
                    //Move back to "Outputs" leaf
                    ok &= data.MoveToAncestor(1u);
                }     
                //Move back to "Map" leaf
                ok &= data.MoveToAncestor(1u);
                if (ok){
                    REPORT_ERROR(ErrorManagement::Information, "UEIMapContainer::Initialise - "
                    "%s map initialised with %d input devices.", name.Buffer(), nOutputMembers);
                }
            }
        }else{
            REPORT_ERROR(ErrorManagement::Information, "UEIMapContainer::Initialise - "
            "No output signals defined for map %s.", name.Buffer());
        }
    }
    //Check if any input signals are defined
    if (ok){
        //Check if there's Inputs section in the configuration
        inputSignalsDefined = data.MoveRelative("Inputs");
        if (inputSignalsDefined){
            //Get and check the number of devices defined into the Devices subsection
            nInputMembers = data.GetNumberOfChildren();
            ok = (nInputMembers > 0u && nInputMembers < MAX_IO_SLOTS);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                "Invalid device number for map %s.", name.Buffer());    
            }
            //Assign the array of pointers to the members array for ordered configuration usage
            if (ok){
                //Assign memory
                inputMembersOrdered = new mapMember*[nInputMembers];
                ok = (inputMembersOrdered != NULL_PTR(mapMember**));
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                    "Could not allocate memory for inputMembersOrdered for map %s.", name.Buffer());     
                }
            }
            if (ok){
                //Traverse each of the devices leaf to list the device used and the channels within such device
                for (uint32 i = 0u; i < nInputMembers && ok; i++){
                    //Move to the device node
                    ok = data.MoveToChild(i);
                    StreamString node_name;
                    uint8 devn = 0u;
                    if (ok){
                        //get node name
                        node_name = data.GetName();
                    }
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                        "Could not retrieve device %s at Inputs block for map %s.", i, name.Buffer());                
                    }
                    if (ok){
                        //Get and check the device identifier for this map member
                        ok = data.Read("Devn", devn);
                        ok &= (devn>=0u && devn < MAX_IO_SLOTS);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                            "Could not retrieve Devn for device %s  for map %s.", node_name.Buffer(), name.Buffer());  
                        }
                    }
                    //Check that this devn is not already used for a member of this map
                    if (ok){
                        ok = (!members[devn].Inputs.defined);       //Check it the device has already been assigned within this map.
                        if (!ok){
                            //The device devn is already defined for another member of this map
                            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                            "devn %d is repeated within map %s (at device %s in Inputs block).", devn, name.Buffer(), node_name.Buffer()); 
                        }
                    }
                    if (ok){
                        //Device is not repeated, check if it is within allowed devn limits
                        ok = (devn < MAX_IO_SLOTS);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                            "Invalid devn %d in map %s (at device %s in Inputs block).", devn, name.Buffer(), node_name.Buffer());
                        }
                    }
                    if (ok){
                        //Read the data for this device
                        //Mark this device (member) as needed by the map
                        members[devn].defined = true;
                        //Mark this device (member) as needing input signals by the map
                        members[devn].Inputs.defined = true;
                        //Set the device number for this member
                        members[devn].devn = devn;
                        //Save the pointer to this member in ordered fashion
                        inputMembersOrdered[i] = &members[devn];
                        //Write the requested channels for this map member, check on such information is delegate to the UEIDevice
                        //As hardware layer-type is not provided to MapContainer Object.
                        ok = helper.ReadArray("Channels", members[devn].Inputs.channels, members[devn].Inputs.nChannels);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
                            "Could not retrieve Channels for device number %d for map %s.", i, name.Buffer());
                        }
                        if (ok){
                            nInputChannels += members[devn].Inputs.nChannels;
                        }
                    }
                    //Move back to "Inputs" leaf
                    ok &= data.MoveToAncestor(1u);
                }     
                //Move back to "Map" leaf
                ok &= data.MoveToAncestor(1u);
                if (ok){
                    REPORT_ERROR(ErrorManagement::Information, "UEIMapContainer::Initialise - "
                    "%s map initialised with %d input devices.", name.Buffer(), nInputMembers);
                }
            }
        }else{
            REPORT_ERROR(ErrorManagement::Information, "UEIMapContainer::Initialise - "
            "No input signals defined for map %s.", name.Buffer());
        }
    }
    //Check if the Inputs/Outputs configuration blocks are defined.
    if (ok){
        ok = (outputSignalsDefined || inputSignalsDefined);
        if (!ok){
            //No Inputs and Outputs signal blocks are defined. The MapContainer cannot be empty
            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMapContainer::Initialise - "
            "No Inputs or Outputs blocks defined for map %s.", name.Buffer());
        }
        //Set the input/outputAssignedToDs variables to the value of input/outputSignalsDefined value.
        inputAssignedToDS = !inputSignalsDefined;
        outputAssignedToDS = !outputSignalsDefined;
    }
    return ok;
}

bool UEIMapContainer::StartMap(int32 DAQ_handle_){
    return false;
}

bool UEIMapContainer::PollForNewPacket(float64* destinationAddr){
    return false;
}

uint8 UEIMapContainer::GetType(){
    //Base implementation of this function which returns NOMAP by default
    //Reimplementation of this function is responsibility of each of the child classes
    return NOMAP;
}

//BASE CLASS METHODS

bool UEIMapContainer::GetDevDefined(uint32 devn){
    bool defined = false;
    if (devn< MAX_IO_SLOTS){
        defined = members[devn].defined;
    }
    return defined;
}

bool UEIMapContainer::GetNumberOfChannels(uint32 devn, uint8 direction, uint32* nChannels){
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

bool UEIMapContainer::GetDevDefined(uint32 devn, uint8 direction){
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

bool UEIMapContainer::GetChannelOfMember(uint32 devn, uint8 direction, uint32 channelIdx, uint32* channelNumber){
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

bool UEIMapContainer::SetDevices(ReferenceT<UEIDevice>* referenceList){
    bool ok = true;
    for (uint32 i = 0; i < MAX_IO_SLOTS && ok; i++){
        //First of all check if the device is needed in this map
        if (members[i].defined){
            //We need to set the reference to the device in this member
            //Check first that the reference for this device is valid
            ok = (referenceList[i].IsValid());
            if(ok){
                //If the reference is valid, assign it to the member
                members[i].reference = referenceList[i];
            }else{
                REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to recover reference for dev%d in Map %s", i, name.Buffer());
            }
        }
    }
    return ok;
}

bool UEIMapContainer::CheckMapCoherency(){
    bool ok = true;
    //First check that all the devices needed by this map are free to use (not registered by any other map)
    if (ok){
        //Traverse the list of possible members for this map
        for (uint32 i = 0; i < MAX_IO_SLOTS && ok; i++){
            //Check if the device (member) is needed
            if (members[i].defined){
                //Then check if the device is avaialble
                ok = (!members[i].reference->GetMapAssignment());//This will return false if the device is available for usage in this map, true otherwise
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Device dev%d must not be configured for more than one Map", i);
                }else{
                    members[i].reference->SetMapAssignment();   //Set the device as requested by this map
                }
            }
        }
    }
    //DESIGN DECISION
    //Check all possible devices in the map members
    bool firstDeviceVisited = false;
    for (uint32 i = 0; i <MAX_IO_SLOTS && ok; i++){
        if (members[i].defined){
            //The device is part of the map, check its sampling frequency
            ReferenceT<UEIDevice> devReference = members[i].reference;
            ok = devReference.IsValid();
            if (ok){
                //Assign the sampling rate of the map to the one of the first device defined for this map and compare
                if (!firstDeviceVisited){
                    sampleRate = devReference->GetSamplingFrequency();
                    firstDeviceVisited = true;
                }
                //Compare the sampling rate of the first device (the one of the map) with all the defined devices
                ok = (sampleRate == devReference->GetSamplingFrequency());
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Device sampling frequencies do not match for devices in Map %s", name.Buffer());
                }
            }else{
                REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to retrieve reference for device dev%d in Map %s",i , name.Buffer());
            }
        }
    }
    return ok;
}

float UEIMapContainer::GetsampleRate(){
    return sampleRate;
}

char8* UEIMapContainer::GetName(){
    return (char8*) name.Buffer();
}

bool UEIMapContainer::RegisterDS(SignalDirection direction){
    bool ok = false;
    //Check if the direction has already been assigned for this Map
    //if it was already assigned, set the flag and return true
    //otherwise return false
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

bool UEIMapContainer::IsSignalAllowed(TypeDescriptor signalType, uint8 direction){
    bool ok = true;
    //Check for each of the memebers of the map if the signal type and direction are allowed
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

bool UEIMapContainer::GetNumberOfChannels(uint8 direction, uint32 &nChannels){
    bool ok = true;
    //Getter for the number of channels in the map regarding its direction
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

//This function does not check for repeated channel numbers TODO
/*bool UEIMapContainer::CalculateCorrectionIndexes(uint32* configuredChannelList, int8* correctionCoefficientsList){
    bool ok = true;
    uint32 arrayLength = sizeof(configuredChannelList)/sizeof(uint32);
    //Create an array to store the ordered channel numbers
    uint32* orderedList = new uint32[arrayLength];
    memcpy(orderedList, configuredChannelList, arrayLength);
    //Sort the orderedList array
    std::sort(orderedList, orderedList+arrayLength);
    //With the array sorted, traverse the user-configured order and find the location of the indexes on the sorted array
    for (uint32 i = 0; i < arrayLength && ok; i++){
        uint32 configuredChannel = configuredChannelList[i];
        ok = false;
        for (uint32 j = 0; j < arrayLength; j++){
            if (orderedList[j] == configuredChannel){
                //We found the channel in the ordered array, calculate the correction factor
                correctionCoefficientsList[i] = (int8) (j-i);
                ok = true;
            }
        }
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Could not sort channel array in Map %s", name.Buffer());
        }
    }
    return ok;
}
*/

bool UEIMapContainer::GetTimestamp(uint32 inputTimestamp, uint64 &outputTimestamp){
    //Simple implementation of the 32-bit to 64-bit translation mechanism
    //employed for the timestamp to improve timestamp range.
    if (inputTimestamp < lastTimestamp){
        //Detect a timestamp overflow condition
        timestampCorrector += 1u;
    }
    lastTimestamp = inputTimestamp;
    outputTimestamp = (uint64)timestampCorrector << 32 | inputTimestamp;
    return true;
}

bool UEIMapContainer::GetTimestamp(uint32* inputTimestamp, uint32 timestampListLength ,uint64* outputTimestamp){
    //Check length of the different destination/source arrays
    bool ok = (timestampListLength <= (sizeof(inputTimestamp)/sizeof(inputTimestamp[0])));
    ok &= (timestampListLength <= (sizeof(outputTimestamp)/sizeof(outputTimestamp[0])));
    if (ok){
        //The lengths of the arrays are ok to accomodate the timestamps.
        for (uint32 i = 0u; i < timestampListLength && ok; i++){
            ok = (GetTimestamp(inputTimestamp[i], outputTimestamp[i]));
        }
    }
    return ok;
}

CLASS_REGISTER(UEIMapContainer, "1.0")
}