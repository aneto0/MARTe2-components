/**
 * @file UEIMasterObject.cpp
 * @brief Source file for class UEIMasterObject
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
 * the class UEIMasterObject (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEIMasterObject.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UEIMasterObject::UEIMasterObject() : ReferenceContainer() {
    DAQ_handle = 0u;
    port = 0u;
    ip_string = NULL_PTR(char8*);  
    nDevices = 0;
    ip[0] = 0;
    ip[1] = 0;
    ip[2] = 0;
    ip[3] = 0;
    name = "";
    nMaps = 0;
    IOMTimeOut = 0;
}

UEIMasterObject::~UEIMasterObject(){
    //Clean the defined DAQ maps for this UEIDAQ device
    for (uint32 i = 0u; i < nMaps; i++){
        bool mapClenaupOk = true;
        if (maps[i].IsValid()){
            mapClenaupOk &= maps[i]->CleanupMap();
            if (!mapClenaupOk){
                REPORT_ERROR(ErrorManagement::CommunicationError, "Could not clean properly map %s!", name.Buffer());
            }
        }
    }
    delete [] maps;
    //Clean the IOM structures
    bool ok = true;
    if (DAQ_handle != 0){
        ok &= (DqCloseIOM(DAQ_handle) >= 0);
    }
    if (!ok){
        REPORT_ERROR(ErrorManagement::CommunicationError, "Device %s could not close properly the IOM!", name.Buffer());
    }
    //Clean the PDNA library structures
    DqCleanUpDAQLib();
}

bool UEIMasterObject::Initialise(StructuredDataI &data){
    bool ok = ReferenceContainer::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Check the name of the Object
    if (ok) {
        name = data.GetName();
        ok = (name.Size() != 0ull);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIMasterObject::Initialise - "
                "Could not retrieve DAQ Master Object Name.");
        }
    }
    //Get connection timeout for the IOM if defiened, if not set default
    if (ok){
        helper.Read("ConnectionTimeOut",IOMTimeOut,(uint32)DEFAULT_IOM_CONNECTION_TIMEOUT);
    }
    //Check Ip and port of the UEIDAQ - 127.0.0.1 is recognized as local device
    if (ok) {
        uint32 ip_length;
        ok = data.Read("Ip", ip);
        if (ok){
            if (ip[0] == 127u &&ip[1] == 0u && ip[2] == 0u && ip[3] == 1u && ok){
                REPORT_ERROR(ErrorManagement::Information, "UEIDAQ %s set as local device.", name.Buffer());
                ip_string = new char8[9];
                snprintf(ip_string, 10, "127.0.0.1");
            }else if (ip[0] <= 255u && ip[1] <= 255u && ip[2] <= 255 && ip[3] <= 255u){
                ok = data.Read("Port", port);
                if (ok) {
                    //Store the ip as a string for later IOM initialization
                    uint16 ip_string_size = snprintf(NULL, 0, "%d.%d.%d.%d",ip[0], ip[1], ip[2], ip[3]);
                    ip_string = (char8*)malloc(ip_string_size);
                    snprintf(ip_string, ip_string_size+1, "%d.%d.%d.%d",ip[0], ip[1], ip[2], ip[3]);
                    REPORT_ERROR(ErrorManagement::Information, "UEIDAQ %s set at address %s:%d.", name.Buffer(), ip_string, port);
                }else{
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Could not find Port for remote UEIDAQ %s.", name.Buffer());
                }
            }else{
                ok = false;
                REPORT_ERROR(ErrorManagement::InitialisationError, "UEIDAQ %s IP is ill-formatted (%d.%d.%d.%d:%d).", name.Buffer(), ip[0], ip[1], ip[2], ip[3]);                
            }
        }else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "No Ip parameter provided for UEIDAQ device %s.", name.Buffer());
        }
    }
    //Check the configuration of the listed device
    if (ok){
        //Try to retrieve the Devices ReferenceContainer
        ReferenceT<ReferenceContainer> devicesContainer = (ReferenceT<ReferenceContainer>)this->Find("Devices");
        ok = devicesContainer.IsValid();
        if (ok) ok &= devicesContainer->IsReferenceContainer();
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrive Devices for UEIDAQ device %s", name.Buffer());
        }
        if (ok){
            nDevices = devicesContainer->Size();
            if (nDevices == 0u){
                ok = false;
                REPORT_ERROR(ErrorManagement::InitialisationError, "No devices found for UEIDAQ device %s", name.Buffer());
            }else if (nDevices >= MAX_IO_SLOTS){
                ok = false;
                REPORT_ERROR(ErrorManagement::InitialisationError, "Too many devices found for UEIDAQ device %s (Max number of devices %d)",name.Buffer(), MAX_IO_SLOTS);
            }else{
                REPORT_ERROR(ErrorManagement::Information, "%d devices found for UEIDAQ device %s", nDevices, name.Buffer());
                if (ok){
                    for (uint8 i = 0u; i < nDevices && ok; i++){
                        ReferenceT<UEIDevice>dev_ = devicesContainer->Get(i);
                        uint8 devn_ = dev_->GetDevN();
                        ok = (dev_.IsValid() && devn_ < MAX_IO_SLOTS);
                        if (ok){
                            ok = !devices[devn_].IsValid();
                            if (ok){
                                devices[devn_] = dev_;
                            }else{
                                REPORT_ERROR(ErrorManagement::InitialisationError, "Devn %d is repeated among devices on %s.", devn_, name.Buffer());
                            }                         
                        }else{
                            REPORT_ERROR(ErrorManagement::InitialisationError, "Device %d is not valid", i);
                        }
                    }
                }
            }
        }
    }
    //Check the configuration of the listed DAQ Maps (if there are any defined)
    if (ok){
        ReferenceT<ReferenceContainer> mapsContainer = (ReferenceT<ReferenceContainer>)this->Find("Maps");
        bool maps_present = mapsContainer.IsValid();
        if (maps_present) maps_present &= mapsContainer->IsReferenceContainer();
        if (maps_present){
            //The maps container is defined
            //Get the number of maps defined in it
            nMaps = mapsContainer->Size();
            REPORT_ERROR(ErrorManagement::Information, "Found %d DAQ maps for UEIDAQ device %s.", nMaps, name.Buffer());
            maps = new ReferenceT<UEIMapContainer>[nMaps];
            ok = (maps != NULL_PTR(ReferenceT<UEIMapContainer>*));
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to allocate memory for UEIMapContainer pointers for UEIDAQ device %s.", name.Buffer());  
            }
            if (ok){
                //Assign DAQ Map pointers to the pointer array
                //Map checks are performed during Map object initialisation
                for (uint32 i = 0u; i < nMaps && ok; i++){
                    maps[i] = mapsContainer->Get(i);
                    ok = maps[i].IsValid();
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid DAQ Map (%d) for UEIDAQ device %s.",i , name.Buffer());  
                    }
                }
            }
        }else{
            //No maps are defined, its ok
            REPORT_ERROR(ErrorManagement::Information, "No maps found for UEIDAQ device %s.", name.Buffer());
        }
    }
    //Set devices references for Maps and check coherency of them
    if (ok){
        //Traverse the list of maps defiend for this UEIDAQ object.
        for (uint32 i = 0u; i<nMaps && ok; i++){
            //Give access to the devices objects to the mapContainers, and let them check for coherency
            //This operation is responsability of the mapContainer, since the coherency of the map is
            //something between the map and device
            
            //First make the device list avaialble for the MapContainer
            ok = (maps[i]->SetDevices(devices));
            if (ok){
                //Then ask the map to check for coherency
                ok = (maps[i]->CheckMapCoherency());
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Inconsistency detected in Map %s", maps[i]->GetName());
                }
            }else{
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not assign devices list for Map %s", i, maps[i]->GetName());
            }
        }
    }
    //Message congratulating you if Maps are coherent and devices well set
    if (ok){
        REPORT_ERROR(ErrorManagement::Information, "Maps correctly initialised for UEIDAQ device %s.", name.Buffer());
    }
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
    //Check if the IOM handle is valid, if it is 0 it will not be valid
    if (ok){
        ok = (DAQ_handle != 0);
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to initialise IOM handle for Device %s", name.Buffer());
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
    //Configure the different devices
    if (ok){
        for (uint8 i = 0u; i < MAX_IO_SLOTS && ok; i++){
            if (devices[i].IsValid()){
                //The device is configured in MARTe and its validity has been checked
                ok = (devices[i]->ConfigureDevice(DAQ_handle));
                if (!ok){
                    //The device cannot be correctly configured, report error
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Device configured at dev%d in UEIDAQ device %s cannot be correctly configured in IOM.", i, name.Buffer());
                }
            }
        }
    }
    //Prepare the Maps for usage by setting the IOM handle to them
    if (ok){
        for (uint32 i = 0; i < nMaps && ok; i++){
            ok = (maps[i]->SetDAQHandle(DAQ_handle));
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not set DAQ handle for Map %s", maps[i]->GetName());
            }
        }
    }
    // At this point, if ok is valid we've checked connection to the IOM, hardware configuration matching and device configuration
    return ok;
}

CLASS_REGISTER(UEIMasterObject, "1.0")
}