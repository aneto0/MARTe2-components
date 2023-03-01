/**
 * @file UEIRtDMap.cpp
 * @brief Source file for class UEIRtDMap
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
 * the class UEIRtDMap (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CLASSMETHODREGISTER.h"
#include "UEIRtDMap.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UEIRtDMap::UEIRtDMap() : UEIMapContainer() {
    inputMap = NULL_PTR(uint32*);
}

UEIRtDMap::~UEIRtDMap(){
    if (inputMap != NULL_PTR(uint32*)){
        inputMap = NULL_PTR(uint32*);
    }
    //try to clean the maps in case it was not already done beforehand
    CleanupMap();
}

bool UEIRtDMap::CleanupMap(){
    bool ok = true;
    if (DAQ_handle != 0){
        REPORT_ERROR(ErrorManagement::Information, "UEIRtDMap::CleanupMap - "
        "Cleaning Map %s", name.Buffer());
        if(mapid){
            ok &= (DqRtDmapStop(DAQ_handle, mapid) >= 0);
            ok &= (DqRtDmapClose(DAQ_handle, mapid) >= 0);
        }
        DAQ_handle = 0;
    }else{
        REPORT_ERROR(ErrorManagement::Information, "UEIRtDMap::CleanupMap - "
        "Map %s was not started, skipping cleanup!", name.Buffer());
    }
    return ok;
}

bool UEIRtDMap::Initialise(StructuredDataI &data){
    bool ok = UEIMapContainer::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //For the RtDMap we do need a ScanRate parameter which defines at which frequency the IOM refreshes the 
    //RtDMap internally
    if (ok){
        ok = data.Read("ScanRate", scanRate);
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "UEIRtDMap::Initialise - "
            "Could not retrieve ScanRate for map %s.", name.Buffer());            
        }
    }
    if (ok){
        REPORT_ERROR(ErrorManagement::Information, "UEIRtDMap::Initialise - "
        "Successful initialisation of map %s.", name.Buffer());
    }
    return ok;
}

bool UEIRtDMap::StartMap(int32 DAQ_handle_){
    bool ok = true;
    //Now that the map is to be started it is valuable to make a copy of the DAQ handle reference for deallocating the map upon
    //destruction of this object.
    DAQ_handle = DAQ_handle_;
    ok = (DqRtDmapInit(DAQ_handle, &mapid, scanRate) >= 0);
    if (!ok){
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error on Initialising Map %s", name.Buffer());
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
            for (uint32 j = 0; j < inputMembersOrdered[i]->Inputs.nChannels && ok; j++){
                uint32 channel = inputMembersOrdered[i]->Inputs.channels[j];
                uint8 devn = inputMembersOrdered[i]->devn;
                uint32 channelData = 0u;
                ok = (devReference->ConfigureChannel(channel, &channelData));
                if (ok){
                    ok = (DqRtDmapAddChannel(DAQ_handle, mapid, devn, DQ_SS0IN, &channelData, 1));
                }
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error adding input channels for dev%d on Map %s", devn, name.Buffer());
                }
            }
        }
        
    }
    if (ok){
        //Once the map is initialized correctly, the I/O channels need to be checked into the map itself
        //First the output channels are checked. The outputMembersOrdered pointer array is traversed in order,
        // yielding the ordered list of channels to sequence into the output map
        for (uint32 i = 0u; i < nOutputMembers && ok; i++){
            ReferenceT<UEIDevice> devReference = outputMembersOrdered[i]->reference;
            ok = (devReference.IsValid());  //This should not be necessary, but it is implemented for precaution
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Found invalid device reference on outputMember %i on Map %s", i, name.Buffer());
            }
            for (uint32 j = 0; j < outputMembersOrdered[i]->Outputs.nChannels && ok; j++){
                uint32 channel = outputMembersOrdered[i]->Outputs.channels[j];
                uint8 devn = outputMembersOrdered[i]->devn;
                uint32 channelData = 0u;
                ok = (devReference->ConfigureChannel(channel, &channelData));
                if (ok){
                    ok = (DqRtDmapAddChannel(DAQ_handle, mapid, devn, DQ_SS0OUT, &channelData, 1));
                }
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error adding output channels for dev%d on Map %s", devn, name.Buffer());
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
    return ok;
}

bool UEIRtDMap::GetMapPointers(){
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
        inputMap = ((uint32*)map_pointer);
        return true;
    }else{
        return false;
    }
    return ok;
}

bool UEIRtDMap::PollForNewPacket(float64* destinationAddr){
    bool next_packet = false;
    bool ok = true;
    //Poll for next packet from UEIDAQ
    ok = (DqRtDmapRefresh(DAQ_handle, mapid) >= 0);
    if (ok){
        //Check if the response is a new packet or a rerequest.
        //The 0x80000000 bit on the recived samples lets us know if the packet has
        //been previously requested.
        if (*((uint32*)inputMap) & 0x80000000){ 
            uint32 iterator = 0; 
            //The recived packet is a newly converted one not requested yet.
            //Make the signals available to the broker.
            //TODO implement this using memcopy
            for (uint32 mem = 0; mem < nInputMembers && ok; mem++){
                //Copy the scaled values obtained in the hardware layer into the destination buffer
                ok = (DqRtDmapReadScaledData(DAQ_handle, mapid, inputMembersOrdered[mem]->devn, &destinationAddr[iterator], inputMembersOrdered[mem]->Inputs.nChannels) >= 0);
                iterator += inputMembersOrdered[mem]->Inputs.nChannels;
                //The channels requested have already been copied, stop the loop
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

uint8 UEIRtDMap::GetType(){
    return RTDMAP;
}

CLASS_REGISTER(UEIRtDMap, "1.0")
}