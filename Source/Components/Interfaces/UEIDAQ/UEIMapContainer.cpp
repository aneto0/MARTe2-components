/**
 * @file UEIMapContainer.cpp
 * @brief Source file for class UEIMapContainer
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
 * the class UEIMapContainer (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEIMapContainer.h"
#include "TypeDescriptor.h"


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
    outputAssignedToDS  = false;
    inputAssignedToDS   = false;
    mapid = 0u;
    nInputChannels = 0u;
    nOutputChannels = 0u;
    DAQ_handle = 0;
    fragmentedMap = false;
    //Set the members of the map to a non-set state for safety
    for (uint32 i = 0u; i < MAX_IO_SLOTS; i++){
        //Set the member as not defined
        members[i].defined = false;
        //Set the inputs of the member as not defined
        members[i].Inputs.defined = false;
        members[i].Inputs.nChannels = 0u;
        members[i].Inputs.channels = NULL_PTR(uint32*);
        members[i].Inputs.requestSize = 0u;
        members[i].Inputs.timestampRequired = false;
        //Set the outputs of the memeber as not defined
        members[i].Outputs.defined = false;
        members[i].Outputs.nChannels = 0u;
        members[i].Outputs.channels = NULL_PTR(uint32*);
        members[i].Outputs.requestSize = 0u;
        members[i].Outputs.timestampRequired = false;
    }
    outputMembersOrdered = NULL_PTR(MapMember**);
    inputMembersOrdered = NULL_PTR(MapMember**);
    nInputMembers = 0u;
    nOutputMembers = 0u;
    sampleNumber = 0u;
    timestampCorrector = 0u;
    lastTimestamp = 0u;
    inputSignalAddresses = NULL_PTR(uint8**);
    inputSignalTypes = NULL_PTR(TypeDescriptor*);
    TimestampAddr = NULL_PTR(uint64*);
    firstPckt = true;
    mapStarted = false;
    signalsConfigured = false;
    mapCoherent = false;
}

UEIMapContainer::~UEIMapContainer(){
    //Clean up the different data structures
    if (outputMembersOrdered != NULL_PTR(MapMember**)){
        delete[] outputMembersOrdered;
    }
    if (inputMembersOrdered != NULL_PTR(MapMember**)){
        delete[] inputMembersOrdered;
    }
    if (inputSignalAddresses != NULL_PTR(uint8**)){
        delete[] inputSignalAddresses;
    }
    if(inputSignalTypes != NULL_PTR(TypeDescriptor*)){
        delete[] inputSignalTypes;
    }
    if (TimestampAddr != NULL_PTR(uint64*)){
        TimestampAddr = NULL_PTR(uint64*);
    }   
}

//VIRTUAL METHODS TO BE REIMPLMENTED IN CHILD CLASSES

bool UEIMapContainer::StopMap(){
    return false;
}

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
                REPORT_ERROR(ErrorManagement::InitialisationError,"Invalid Output device number for map %s.", name.Buffer());    
            }
            //Assign the array of pointers to the members array for ordered configuration usage
            if (ok){
                //Assign memory
                outputMembersOrdered = new MapMember*[nOutputMembers];
                ok = (outputMembersOrdered != NULL_PTR(MapMember**));
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError,"Could not allocate memory for outputMembersOrdered for map %s.", name.Buffer());     
                }
            }
            if (ok){
                //Perform the checks on the data supplied on Inputs/Outputs blocks on configuration of the object
                ok = ParseIODevices(data, OutputSignals, nOutputMembers, outputMembersOrdered);
                //Move back to "Map" leaf
                ok &= data.MoveToAncestor(1u);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Detected error during Outputs block check");
                }
            }        
        }else{
            REPORT_ERROR(ErrorManagement::Information,"No output signals defined for map %s.", name.Buffer());
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
                REPORT_ERROR(ErrorManagement::InitialisationError,"Invalid Input device number for map %s.", name.Buffer());    
            }
            //Assign the array of pointers to the members array for ordered configuration usage
            if (ok){
                //Assign memory
                inputMembersOrdered = new MapMember*[nInputMembers];
                ok = (inputMembersOrdered != NULL_PTR(MapMember**));
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Could not allocate memory for inputMembersOrdered for map %s.", name.Buffer());     
                }
            }     
            if (ok){
                //Perform the checks on the data supplied on Inputs/Outputs blocks on configuration of the object
                ok = ParseIODevices(data, InputSignals, nInputMembers, inputMembersOrdered);
                //Move back to "Map" leaf
                ok &= data.MoveToAncestor(1u);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Detected error during Inputs block check");
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

bool UEIMapContainer::StartMap(){
    return false;
}

bool UEIMapContainer::PollForNewPacket(MapReturnCode& outputCode){
    return false; 
}

MapType UEIMapContainer::GetType(){
    //Base implementation of this function which returns NOMAP by default
    //Reimplementation of this function is responsibility of each of the child classes
    return NO_MAP;
}

bool UEIMapContainer::ConfigureInputsForDataSource(uint32 nSamples, uint32 nChannels, uint64* inputTimestampAddress, uint8** signalAddresses, TypeDescriptor* signalTypes){
    bool ok = (signalAddresses != NULL_PTR(uint8**));
    ok &= (signalTypes != NULL_PTR(TypeDescriptor*));
    ok &= (inputTimestampAddress != NULL_PTR(uint64*));
    ok &= (nChannels == nInputChannels);
    //Set the memory address for the TimestampChannel;
    TimestampAddr = inputTimestampAddress;
    //Copy the memory locations for the input signals
    if (ok){
        if (inputSignalAddresses != NULL_PTR(uint8**)) delete[] inputSignalAddresses;
        inputSignalAddresses = new uint8*[nChannels];
        ok &= (inputSignalAddresses != NULL_PTR(uint8**));
        if (ok){
            for (uint32 i = 0; i < nChannels; i++){
                inputSignalAddresses[i] = signalAddresses[i];
            }
        }
    }
    //Copy the types for the different input signals
    if (ok){
        if (inputSignalTypes != NULL_PTR(TypeDescriptor*)) delete[] inputSignalTypes;
        inputSignalTypes = new TypeDescriptor[nChannels];
        ok &= (inputSignalTypes != NULL_PTR(TypeDescriptor*));
        if (ok){
            for (uint32 i = 0; i < nChannels; i++){
                inputSignalTypes[i] = signalTypes[i];
            }
        }
    }
    return ok;
}

//BASE CLASS METHODS

bool UEIMapContainer::GetDevDefined(uint32 devn){
    bool defined = false;
    if (devn< MAX_IO_SLOTS){
        defined = members[devn].defined;
    }
    return defined;
}

bool UEIMapContainer::GetDevDefined(uint32 devn, SignalDirection direction){
    bool defined = false;
    if (devn<MAX_IO_SLOTS){
        if (direction == InputSignals){
            defined = members[devn].Inputs.defined;
        }else if (direction == OutputSignals){
            defined = members[devn].Outputs.defined;
        }else{
            defined = false;
        }
    }
    return defined;
}

bool UEIMapContainer::GetNumberOfChannels(uint32 devn, SignalDirection direction, uint32* nChannels){
    bool valid = false;
    if (devn<MAX_IO_SLOTS){
        if (members[devn].defined == true){
            valid = true;
            if (direction == InputSignals){
                *nChannels = members[devn].Inputs.nChannels;
            }else if (direction == OutputSignals){
                *nChannels = members[devn].Outputs.nChannels;
            }else{
                valid = false;
            }
        }
    }
    return valid;
}

bool UEIMapContainer::GetChannelOfMember(uint32 devn, SignalDirection direction, uint32 channelIdx, uint32 &channelNumber){
    bool valid = false;
    if (devn<MAX_IO_SLOTS){
        if (members[devn].defined){
            if (direction == InputSignals && (channelIdx<members[devn].Inputs.nChannels)){
                channelNumber = members[devn].Inputs.channels[channelIdx];
                valid = true;
            }else if (direction == OutputSignals && (channelIdx<members[devn].Outputs.nChannels)){
                channelNumber = members[devn].Outputs.channels[channelIdx];
                valid = true;
            }
        }
    }
    return valid;
}

bool  UEIMapContainer::SetDevice(uint8 devn, ReferenceT<UEIDevice> reference){
    bool ok = (devn < MAX_IO_SLOTS);
    ok &= (members[devn].defined);
    if (ok){
        //We need to set the reference to the device in this member
        //Check first that the reference for this device is valid
        ok &= (reference.IsValid());
        if(ok){
            //If the reference is valid, assign it to the member
            members[devn].reference = reference;
        }else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to recover reference for dev%d in Map %s while setting Device references", devn, name.Buffer());
        }
    }
    return ok;
}

bool UEIMapContainer::SetDevices(ReferenceT<UEIDevice>* referenceList){
    bool ok = true;
    if (ok){
        //traverse safely the provided list now that we know it is of MAX_IO_SLOTS length
        for (uint32 i = 0; i < MAX_IO_SLOTS && ok; i++){
            //Only set the device if it is needed by the member
            if (members[i].defined){
                ok &= (SetDevice(i, referenceList[i]));
            }
        }
    }
    return ok;
}

bool UEIMapContainer::CheckMapCoherency(){
    bool ok = true;
    bool firstDeviceVisited = false;
    //Check that all the devices needed by this map are free to use (not registered by any other map)
    // and have the same sampling frequency (DESIGN DECISION)
    //Traverse the list of possible members for this map
    for (uint32 i = 0; i < MAX_IO_SLOTS && ok; i++){
        //Check if the device (member) is needed
        if (members[i].defined){
            ReferenceT<UEIDevice> devReference = members[i].reference;
            ok = devReference.IsValid();
            if (ok){
                //Then check if the device is avaialble
                ok &= (!devReference->GetMapAssignment());//This will return false if the device is available for usage in this map, true otherwise
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Device dev%d must not be configured for more than one Map", i);
                }else{
                    devReference->SetMapAssignment();   //Set the device as requested by this map
                }
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
                }
                //Now check that the defined channels for each direction in the map are actually valid in the devices
                if (ok){
                    if (members[i].Inputs.defined){
                        ok &= devReference->CheckChannelListAndDirection(members[i].Inputs.channels,members[i].Inputs.nChannels, InputSignals);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "Channels supplied as inputs for device %s in map %s are not valid", devReference->GetName(), name.Buffer());
                        }
                    }
                    if (members[i].Outputs.defined){
                        ok &= devReference->CheckChannelListAndDirection(members[i].Outputs.channels,members[i].Outputs.nChannels, OutputSignals);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "Channels supplied as outputs for device %s in map %s are not valid", devReference->GetName(), name.Buffer());
                        }
                    }
                }
            }else{
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve device reference during map coherency check");
            }
        }
    }
    mapCoherent = ok;
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
        break;
    }
    return ok;
}

bool UEIMapContainer::IsSignalAllowed(TypeDescriptor signalType, SignalDirection direction){
    bool ok = true;
    //Check for each of the memebers of the map if the signal type and direction are allowed
    if (direction == OutputSignals){
        //Traverse the outputMembers in an ordered fashion
        for (uint32 i = 0u; i < nOutputMembers && ok; i++){
            ok &= outputMembersOrdered[i]->reference.IsValid();
            if (ok){
                //Check if the member is needed for the supplied signal
                ok &= outputMembersOrdered[i]->reference->AcceptedSignalType(signalType);
            }
        }
    }else if (direction == InputSignals){
        //Traverse the outputMembers in an ordered fashion
        for (uint32 i = 0u; i < nInputMembers && ok; i++){
            ok &= inputMembersOrdered[i]->reference.IsValid();
            if (ok){
                //Check if the member is needed for the supplied signal
                ok &= inputMembersOrdered[i]->reference->AcceptedSignalType(signalType);
            }
        }
    }else{
        ok = false;
    }
    return ok;
}

bool UEIMapContainer::GetNumberOfChannels(SignalDirection direction, uint32 &nChannels){
    bool ok = true;
    //Getter for the number of channels in the map regarding its direction
    nChannels = 0;
    switch(direction){
        case OutputSignals:
            nChannels = nOutputChannels;
        break;
        case InputSignals:
            nChannels = nInputChannels;
        break;
        default:
            ok = false;
        break;
    }
    return ok;
}

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
    bool ok = (timestampListLength >= 0u);
    ok &= (inputTimestamp != NULL_PTR(uint32*));
    ok &= (outputTimestamp != NULL_PTR(uint64*));
    if (ok){
        //The lengths of the arrays are ok to accomodate the timestamps.
        for (uint32 i = 0u; i < timestampListLength && ok; i++){
            ok = (GetTimestamp(inputTimestamp[i], outputTimestamp[i]));
        }
    }
    return ok;
}

bool UEIMapContainer::GetTimestamp(UEIBufferPointer inputTimestamp, uint32 timestampListLength ,uint64* outputTimestamp){
    //Check length of the different destination/source arrays
    bool ok = (timestampListLength >= 0u);
    ok &= (outputTimestamp != NULL_PTR(uint64*));
    if (ok){
        //The lengths of the arrays are ok to accomodate the timestamps.
        for (uint32 i = 0u; i < timestampListLength && ok; i++){
            uint32 thisTimestamp = *(reinterpret_cast<uint32*>(inputTimestamp.GetSample(i)));
            ok = (GetTimestamp(thisTimestamp, outputTimestamp[i]));
        }
    }
    return ok;
}

bool UEIMapContainer::SetDAQHandle(int32 DAQ_handle_){
    bool ok = true;
    if (DAQ_handle_ > 0){
        DAQ_handle = DAQ_handle_;
    }else{
        ok = false;
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error, invalid DAQ handle supplied to Map %s", name.Buffer());
    }
    return ok;
}

bool UEIMapContainer::ParseIODevices(StructuredDataI &data, SignalDirection direction, uint32 nMembers, MapMember** orderedMembers){
    bool ok = (nMembers == data.GetNumberOfChildren()); //Check just in case
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    StreamString directionName = "";
    //Set the name of the device block direction for error report
    if (direction == InputSignals){
        directionName = "Inputs";
    }else if (direction == OutputSignals){
        directionName = "Outputs";
    }
    //Traverse each of the devices leaf to list the device used and the channels within such device
    for (uint32 i = 0u; i < nMembers && ok; i++){
        //Move to the device node
        ok = data.MoveToChild(i);
        StreamString node_name;
        int8 devn = 0;
        if (ok){
            //get node name
            node_name = data.GetName();
        }
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError,"Could not retrieve device %s at %s block for map %s.", node_name.Buffer(), directionName.Buffer(), name.Buffer());                
        }
        if (ok){
            //Get and check the device identifier for this map member
            ok = data.Read("Devn", devn);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError,"Could not retrieve Devn parameter for device %s in map %s.", node_name.Buffer(), name.Buffer());  
            }
            if (ok){
                ok &= (devn >= 0 && devn < MAX_IO_SLOTS);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError,"Invalid Devn for device %s in map %s.", node_name.Buffer(), name.Buffer());  
                }
            }
        }
        //Reference to the IOMapMember we're working on in this iteration, for convinience
        IOMapMember* thisMember;
        if (direction == InputSignals){
            thisMember = &(members[devn].Inputs);
        }else if (direction == OutputSignals){
            thisMember = &(members[devn].Outputs);
        }
        //Check that this devn is not already used for a member of this map
        if (ok){
            ok = (!thisMember->defined);       //Check it the device has already been assigned within this map.
            if (!ok){
                //The device devn is already defined for another member of this map
                REPORT_ERROR(ErrorManagement::InitialisationError,"Devn %d is repeated within map %s (at device %s in %s block).", devn, name.Buffer(), node_name.Buffer(), directionName.Buffer()); 
            }
        }
        if (ok){
            //Read the data for this device
            //Mark this device (member) as needed by the map
            members[devn].defined = true;
            //Mark this device (member) as needing output signals by the map
            thisMember->defined = true;
            //Set the device number for this member
            members[devn].devn = (uint8) devn;
            //Save the pointer to this member in ordered fashion
            orderedMembers[i] = &members[devn];
            //Write the requested channels for this map member, check on such information is delegate to the UEIDevice
            //As hardware layer-type is not provided to MapContainer Object.
           
            int32* channelList;
            uint32 channelNumber;
            ok = helper.ReadArray("Channels", channelList, channelNumber);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve Channels for device %s for map %s.", node_name.Buffer() , name.Buffer());
            }
            if (ok){
                //Check the type of the Channels parameter to not be float (negative channels are sorted in the next lines)
                ok = (helper.GetType("Channels").GetTypeDescriptor() != Float32Bit && helper.GetType("Channels").GetTypeDescriptor() != Float64Bit);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Channels supplied for device %s in map %s must be positive integers.", node_name.Buffer(), name.Buffer());
                }
            }
            if (ok){
                //Check the order of the channels assigned to the member, channels must be set in ascending order by convention
                //Also check that no channel repetition is allowed
                int32 lastChannel = -1;
                for (uint32 i = 0; i < channelNumber && ok; i++){
                    ok &= (channelList[i] > lastChannel);   //This condition also ensures no channel repetition
                    if(!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid channels for device %s in map %s. Channels must be supplied in ascending order and must be positive non-repeating integers.", node_name.Buffer(), name.Buffer());
                    }
                    lastChannel = channelList[i];
                }
            }
            if (ok){
                //Once the supplied channel list is validated, assign it to the member
                thisMember->nChannels = channelNumber;
                thisMember->channels = new uint32[channelNumber];
                for (uint32 i = 0; i < channelNumber; i++){
                    thisMember->channels[i] = (uint32)channelList[i];
                }
                //free the channelList structure
                if (channelList != NULL_PTR(int32*)){
                    delete[] channelList;
                }
                //Log the number of I/O channels in this whole map
                if (direction == InputSignals){
                    nInputChannels += thisMember->nChannels;
                }else{
                    nOutputChannels += thisMember->nChannels;
                }
            }
        }
        //Move back to "Direction" block
        ok &= data.MoveToAncestor(1u);
    }
    return ok;
}

CLASS_REGISTER(UEIMapContainer, "1.0")
}