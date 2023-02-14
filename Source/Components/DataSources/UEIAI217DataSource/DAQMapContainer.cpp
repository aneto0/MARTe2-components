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
    //Read devices information
    if (ok){
        ok = data.MoveRelative("Devices");
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
            "Could not retrieve Devices container for map %s.", name.Buffer());    
        }
        if (ok){
            nDevices = data.GetNumberOfChildren();
            ok = (nDevices > 0u && nDevices < MAX_IO_SLOTS);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                "Invalid device number for map %s.", name.Buffer());    
            }
            if (ok){
                members = new mapMember[nDevices];
                ok = (members!=NULL_PTR(mapMember*));
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                    "Error allocating memory for pointer array of mapMembers for map %s.", name.Buffer());    
                }
            }
        }
        if (ok){
            //Traverse each of the devices leaf to list the device used and the channels within such device
            for (uint32 i = 0u; i < nDevices && ok; i++){
                ok = data.MoveToChild(i);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                    "Could not retrieve device number %d for map %s.", i, name.Buffer());                
                }
                if (ok){
                    ok = data.Read("Devn", members[i].devn);
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                        "Could not retrieve Devn for device number %d for map %s.", i, name.Buffer());  
                    }
                }
                if (ok){
                    ok = helper.ReadArray("Channels", members[i].channels, members[i].nChannels);
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "DAQMapContainer::Initialise - "
                        "Could not retrieve Channels for device number %d for map %s.", i, name.Buffer());
                    }
                }
                ok &= data.MoveToAncestor(1u);
            }
        }
        ok &= data.MoveToAncestor(1u);
        if (ok){
            REPORT_ERROR(ErrorManagement::Information, "DAQMapContainer::Initialise - "
            "%s map initialised with %d devices.", name.Buffer(), nDevices);
        }
    }
    //Acknowledge the successful initialisation of the Object
    if (ok){
        REPORT_ERROR(ErrorManagement::Information, "DAQMapContainer::Initialise - "
        "Successful initialisation of map %s.", name.Buffer());
    }
    return ok;
}

uint32 DAQMapContainer::GetNMembers(){
    return nDevices;
}

uint8 DAQMapContainer::GetDevInMember(uint32 MemberIdx){
    return members[MemberIdx].devn;
}

CLASS_REGISTER(DAQMapContainer, "1.0")
}