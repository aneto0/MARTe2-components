/**
 * @file DAQMasterObject.cpp
 * @brief Source file for class DAQMasterObject
 * @date 07/02/2023
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
 * the class DAQMasterObject (public, protected, and private). Be aware that some
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
#include "DAQMasterObject.h"
//Device objects include
#include "UEIAI217_803.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

DAQMasterObject::DAQMasterObject() : ReferenceContainer() {
    DAQ_handle = 0u;
    port = 0u;
    ip_string = NULL_PTR(char8*);  
}

DAQMasterObject::~DAQMasterObject(){
    //Clean the defined DAQ maps for this UEIDAQ device
    for (uint32 i = 0u; i < nMaps; i++){
        bool mapClenaupOk = true;
        if (maps[i].IsValid()){
            mapClenaupOk &= maps[i]->CleanupMap();
            if (!mapClenaupOk){
                REPORT_ERROR(ErrorManagement::CommunicationError, "DAQMasterObject::Destructor - "
                "Could not clean properly map %s!", name.Buffer());
            }
        }
    }
    //Clean the IOM structures
    bool ok = true;
    if (DAQ_handle != 0){
        ok &= (DqCloseIOM(DAQ_handle) >= 0);
    }
    if (!ok){
        REPORT_ERROR(ErrorManagement::CommunicationError, "DAQMasterObject::Destructor - "
        "Device %s could not close properly the IOM!", name.Buffer());
    }
    //Clean the PDNA library structures
    DqCleanUpDAQLib();
    REPORT_ERROR(ErrorManagement::CommunicationError, "DAQMasterObject::Destructor - "
    "Destroying %s!", name.Buffer());    
}

bool DAQMasterObject::Initialise(StructuredDataI &data){
    bool ok = ReferenceContainer::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Check the name of the Object
    if (ok) {
        name = data.GetName();
        ok = (name.Size() != 0ull);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                "Could not retrieve DAQ Master Object Name.");
        }
    }
    //Get connection timeout for the IOM if defiened, if not set default
    if (ok){
        bool set = helper.Read("ConnectionTimeOut",IOMTimeOut,(uint32)DEFAULT_IOM_CONNECTION_TIMEOUT);
        if (!set){
            REPORT_ERROR(ErrorManagement::Information, "DAQMasterObject::Initialise - "
                "IOM connection timout set to default %dms for device %s.", IOMTimeOut, name.Buffer());
        }
    }
    //Check Ip and port of the UEIDAQ - 127.0.0.1 is recognized as local device
    if (ok) {
        uint32 ip_length;
        ok = data.Read("Ip", ip);
        if (ok){
            if (ip[0] == 127u &&ip[1] == 0u && ip[2] == 0u && ip[3] == 1u && ok){
                REPORT_ERROR(ErrorManagement::Information, "DAQMasterObject::Initialise - "
                "UEIDAQ %s set as local device.", name.Buffer());
            }else if (ip[0] <= 255u && ip[1] <= 255u && ip[2] <= 255 && ip[3] <= 255u){
                ok = data.Read("Port", port);
                if (ok) {
                    //Store the ip as a string for later IOM initialization
                    uint16 ip_string_size = snprintf(NULL, 0, "%d.%d.%d.%d",ip[0], ip[1], ip[2], ip[3]);
                    ip_string = (char8*)malloc(ip_string_size);
                    snprintf(ip_string, ip_string_size+1, "%d.%d.%d.%d",ip[0], ip[1], ip[2], ip[3]);
                    REPORT_ERROR(ErrorManagement::Information, "DAQMasterObject::Initialise - "
                    "UEIDAQ %s set at address %s:%d.", name.Buffer(), ip_string, port);
                }else{
                    REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                    "Could not find Port for remote UEIDAQ %s.", name.Buffer());
                }
            }else{
                ok = false;
                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                "UEIDAQ %s IP is ill-formatted (%d.%d.%d.%d:%d).", name.Buffer(), ip[0], ip[1], ip[2], ip[3]);                
            }
        }else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
            "No Ip parameter provided for UEIDAQ device %s.", name.Buffer());
        }
    }
    //Check the configuration of the listed device
    if (ok){
        //Try to retrieve the Devices ReferenceContainer
        devicesContainer = (ReferenceT<ReferenceContainer>)this->Find("Devices");
        //ok = ReferenceContainer.isContainer(devicesContainer);
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
            "Could not retrive Devices for UEIDAQ device %s", name.Buffer());
        }
        if (ok){
            nDevices = devicesContainer->Size();
            if (nDevices == 0u){
                ok = false;
                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                "No devices found for UEIDAQ device %s", name.Buffer());
            }else if (nDevices >= MAX_IO_SLOTS){
                ok = false;
                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                "Too many devices found for UEIDAQ device %s (Max number of devices %d)",name.Buffer(), MAX_IO_SLOTS);
            }else{
                REPORT_ERROR(ErrorManagement::Information, "DAQMasterObject::Initialise - "
                "%d devices found for UEIDAQ device %s", nDevices, name.Buffer());
                if (ok){
                    for (uint8 i = 0u; i < nDevices && ok; i++){
                        ReferenceT<UEIAI217_803>dev_ = devicesContainer->Get(i);
                        uint8 devn_ = dev_->GetDevN();
                        ok = (dev_.IsValid() && devn_ < MAX_IO_SLOTS);
                        if (ok){
                            ok = !devices[devn_].IsValid();
                            if (ok){
                                devices[i] = dev_;
                            }else{
                                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                                "Devn %d is repeated among devices on %s.", devn_, name.Buffer());
                            }                         
                        }else{
                            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                            "Device %d is not valid", i);
                        }
                    }
                }
            }
        }
    }
    //Check the configuration of the listed DAQ Maps (if there are any defined)
    if (ok){
        mapsContainer = (ReferenceT<ReferenceContainer>)this->Find("Maps");
        bool maps_present = mapsContainer->IsReferenceContainer();
        if (maps_present){
            //The maps container is defined
            //Get the number of maps defined in it
            nMaps = mapsContainer->Size();
            REPORT_ERROR(ErrorManagement::Information, "DAQMasterObject::Initialise - "
            "Found %d DAQ maps for UEIDAQ device %s.", nMaps, name.Buffer());
            maps = new ReferenceT<DAQMapContainer>[nMaps];
            ok = (maps != NULL_PTR(ReferenceT<DAQMapContainer>*));
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                "Unable to allocate memory for DAQMapContainer pointers for UEIDAQ device %s.", name.Buffer());  
            }
            if (ok){
                //Assign DAQ Map pointers to the pointer array
                //Map checks are performed during Map object initialisation
                for (uint32 i = 0u; i < nMaps && ok; i++){
                    maps[i] = mapsContainer->Get(i);
                    ok = maps[i].IsValid();
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                        "Invalid DAQ Map (%d) for UEIDAQ device %s.",i , name.Buffer());  
                    }
                }
            }
        }else{
            //No maps are defined, its ok
            REPORT_ERROR(ErrorManagement::Information, "DAQMasterObject::Initialise - "
            "No maps found for UEIDAQ device %s.", name.Buffer());
        }
    }
    //Check coherency for the Maps (devices cannot be in two memory maps at the same time and scan frequency should be lower
    //than device sampling rate, otherwise a warning is thrown).
    if (ok){
        //Traverse the list of maps defiend for this UEIDAQ object.
        for (uint32 i = 0u; i<nMaps && ok; i++){
            //For each map, traverse the list of devices (or map members) defined for it.
            for (uint32 devn = 0u; devn < MAX_IO_SLOTS && ok; devn++){
                //For each of the possible devn identifiers (0 to MAX_IO_SLOTS) Check if the selected map needs the device (devn)
                bool neededDev = maps[i]->GetDevDefined(devn); //Check if the device is defined in the Map Members
                if (neededDev){     //If and only if the device (devn) is needed, check if it can actually be assigned to the map
                    //First check if the device requested in the map member is actually defined in the configuration for the UEIDAQ
                    ok = devices[devn].IsValid();    //IsValid will return False if the device is not set in configuration
                    if (ok){
                        ok = !(devices[devn]->GetMapAssignment());    //Check if the device has already been assigned to a map
                        if (!ok){
                            //The needed device for this map member is already in use, this is an error
                            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                            "Device dev% is requested for multiple DAQ Maps in UEIDAQ device %s.", devn, name.Buffer()); 
                        }else{
                            //The device is still free to assign to the map, we do so. Once set, the device can only be exclusively used by this map
                            devices[devn]->SetMapAssignment();
                            //Since this member seems to be correct, set the reference of the device to the member for later use (the map needs access to each device).
                            maps[i]->SetDevReference(devn, devices[devn]);
                        }
                    }else{
                        REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                        "Device dev%d requested in Map %d is not configured for UEIDAQ device %s.", devn, i, name.Buffer()); //TODO put map name instead of index
                    }
                    //Check that the channels requested by the member for the map are actually available on the required device and the required direction (Input or Output)
                    if (ok){
                        //Check if output signals are defined for this member
                        if (maps[i]->GetDevDefined(devn, OUTPUT_CHANNEL)){
                            //Output signals are defined for this map member, lets check if the device configured in the UEIDAQ device can accept
                            //such channels in such direction
                            uint32 channelsForThisMember = 0u;
                            //Try to get the number of channels for this member in this direction
                            ok = maps[i]->GetNumberOfChannels(devn, OUTPUT_CHANNEL, &channelsForThisMember);
                            if (ok){
                                for (uint32 j = 0u; j < channelsForThisMember && ok; j++){
                                    //try to get the signal for this map and its member and direction at position j
                                    uint32 channelNumber = 0u;
                                    ok = maps[i]->GetChannelOfMember(devn, OUTPUT_CHANNEL, j, &channelNumber); 
                                    //ok will check if the channel requested is part of the member
                                    if (!ok){
                                        REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                                        "Error requesting output signal %d for dev%d in Map %d for UEIDAQ device %s.", j, devn, name.Buffer()); 
                                    }
                                    if (ok){
                                        //Check if this channel number and direction can be accepted for the required layer
                                        //no need to check if the device exists in the UEIDAQ device as it has already been checked in previous if condition
                                        ok = devices[devn]->CheckChannelAndDirection(channelNumber, OUTPUT_CHANNEL);
                                        if (!ok){
                                            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                                            "Invalid output signal %d for dev%d in Map %d for UEIDAQ device %s.", channelNumber, devn, name.Buffer());  
                                        }
                                        //if this returns ok = true the channel is fit to use in this layer in this direction for this map
                                    }
                                }
                            }else{
                                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                                "Error getting numberOfChannels for output Device dev%d requested in Map %d for UEIDAQ device %s.", devn, i, name.Buffer());
                            }
                        }
                        //Check if input signals are defined for this member
                        if (maps[i]->GetDevDefined(devn, INPUT_CHANNEL)){
                            //Input signals are defined for this map member, lets check if the device configured in the UEIDAQ device can accept
                            //such channels in such direction
                            uint32 channelsForThisMember = 0u;
                            //Try to get the number of channels for this member in this direction
                            ok = maps[i]->GetNumberOfChannels(devn, INPUT_CHANNEL, &channelsForThisMember);
                            if (ok){
                                for (uint32 j = 0u; j < channelsForThisMember && ok; j++){
                                    //try to get the signal for this map and its member and direction at position j
                                    uint32 channelNumber = 0u;
                                    ok = maps[i]->GetChannelOfMember(devn, INPUT_CHANNEL, j, &channelNumber); 
                                    //ok will check if the channel requested is part of the member
                                    if (!ok){
                                        REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                                        "Error requesting input signal %d for dev%d in Map %d for UEIDAQ device %s.", j, devn, name.Buffer()); 
                                    }
                                    if (ok){
                                        //Check if this channel number and direction can be accepted for the required layer
                                        //no need to check if the device exists in the UEIDAQ device as it has already been checked in previous if condition
                                        ok = devices[devn]->CheckChannelAndDirection(channelNumber, INPUT_CHANNEL);
                                        //if this returns ok = true the channel is fit to use in this layer in this direction for this map
                                        if (!ok){
                                            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                                            "Invalid input signal %d for dev%d in Map %d for UEIDAQ device %s.", channelNumber, devn, name.Buffer());  
                                        }
                                    }
                                }
                            }else{
                                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                                "Error getting numberOfChannels for input Device dev%d requested in Map %d for UEIDAQ device %s.", devn, i, name.Buffer());
                            }
                        }
                    }
                    //Check that the scan rate for the map is lower than each of the required devices
                    //Scan rate (frequency in Hz) of a map must be at least half of that of the devices to which it interfaces to allow for stable data acquisition
                    if (ok){
                        float scanRate = maps[i]->GetScanRate();                //Retrieve the scan rate for the map
                        float sampleRate = devices[devn]->GetSamplingFrequency();   //Retrieve the sampling frequency of the device
                        ok = (2*scanRate <= sampleRate);                            //Check, for now it must be lower or EQUAL (check this) TODO
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMasterObject::Initialise - "
                            "UEIDAQ device %s's Map %d scan frequency is too high (higher than half sampling rate from device dev%s)",name.Buffer(), i, devn);
                        }
                    }
                }
            }
        }
    }
    //Message congratulating you if Maps are coherent and devices well set
    if (ok){
        REPORT_ERROR(ErrorManagement::Information, "DAQMasterObject::Initialise - "
        "Maps correctly initialised for UEIDAQ device %s.", name.Buffer());
    }
    /*
    //Perfrom the initialisation for the IOM structure for the PDNA library
    if (ok){
        ok = (DqInitDAQLib() == DQ_SUCCESS);
        if (!ok){
           REPORT_ERROR(ErrorManagement::ParametersError, "Unable to initialise DAQ Lib."); 
        }
    }
    //Initialise the handle to the configured IOM
    if (ok){
        ok = (DqOpenIOM(ip_string, port, IOMTimeOut, &DAQ_handle, NULL) >= 0);
        if(!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to contact IOM for Device %s", name.Buffer());
        }
    }
    //Check that the configured devices correspond to the IOM hardware configuration
    if(ok){
        uint32 devn;
        uint32 serial;
        uint32 address;
        uint16 model;
        //First list the actual hardware configuration for reference
        REPORT_ERROR(ErrorManagement::Information, "UEIDAQ device %s actual IOM configuration.", name.Buffer());
        for (uint32 i = 0u; i < MAX_IO_SLOTS && ok; i++){
            //Set devn and model to 0 to avoid errors
            devn = 0;
            model = 0;
            int32 queryResult = DqGetDevnBySlot(DAQ_handle, i, &devn, &serial, &address, &model);
            ok = (queryResult != DQ_ILLEGAL_HANDLE);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to query IOM for layer information.");   
            }else{
                if (queryResult == DQ_BAD_PARAMETER){
                    //The queried slot is empty or non-existent. Check that no device is configured on that slot.
                    REPORT_ERROR(ErrorManagement::Information, "Slot %2d on %s is EMPTY.", i, name.Buffer());
                }else{
                    //The queried slot is populated by a layer
                    REPORT_ERROR(ErrorManagement::Information, "Slot %2d on %s is dev%2d, layer model %x.", i, name.Buffer(),  devn, model); 
                }
            }
        }
        //Go over the same procedure again to check the match of the configured devices vs the real configuration
        for (uint32 i = 0u; i < MAX_IO_SLOTS && ok; i++){
            //Set devn and model to 0 to avoid errors
            devn = 0;
            model = 0;
            int32 queryResult = DqGetDevnBySlot(DAQ_handle, i, &devn, &serial, &address, &model);
            ok = (queryResult != DQ_ILLEGAL_HANDLE);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to query IOM for layer information.");   
            }else{
                //Check if the query did not return void and moreover the device at such position is defined in our device (if it is not defined there's
                // no problem, but the application cannot use it).
                if (queryResult == DQ_SUCCESS && devices[devn].IsValid()){
                    //The queried slot is not empty -> Check that the configured device is the same model at the given devn
                    ok = (model == devices[devn]->GetModel());
                    if (ok){
                        //The model of the hardware layer corresponds to the one configured, mark this device as correctly set
                        devices[devn]->SetHardwareCorrespondence();
                    }else{
                        //The model of the actual hardware layer does not match the configured device at devn.
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Device configured at dev%d in device %s does not match hardware layer model.", devn, name.Buffer());
                    }
                }
            }
        }
        //If no device in the configured set is mismatched with the hardware layers we still need to check if some configured device
        //is not installed in the chassis.
        if (ok){
            for (uint8 i = 0u; i < MAX_IO_SLOTS && ok; i++){
                if (devices[i].IsValid()){
                    //The device is configured in MARTe
                    ok = (devices[i]->GetHardwareCorrespondence());
                    if (!ok){
                        //The device is configured but has not been validated to have hardware correspondece, meaning it is not installed or
                        //recognized by the IOM.
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Device configured at dev%d in UEIDAQ device %s is not installed/recognised in IOM.", i, name.Buffer());
                    }
                }
            }
        }
    }
    //Start the DAQ Maps
    if (ok){
        for (uint32 i = 0u; i < nMaps && ok; i++){
            ok = maps[i]->StartMap(DAQ_handle);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to start Map %s for UEIDAQ Device %s", maps[i]->GetName(), name.Buffer());
            }
        }
    }
    // At this point, if ok is valid we've checked connection to the IOM, hardware configuration matching and device configuration
    */
    return ok;
}
/*
bool DAQMasterObject::ConfigureDevice(uint8 devn){
    
}

bool DAQMasterObject::ConfigureMap(uint8 map){

}
*/
CLASS_REGISTER(DAQMasterObject, "1.0")
}