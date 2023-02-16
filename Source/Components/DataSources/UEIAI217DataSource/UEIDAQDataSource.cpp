/**
 * @file UEIDAQDataSource.cpp
 * @brief Source file for class UEIDAQDataSource
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
 * the class UEIDAQDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
#include "UEIDAQDataSource.h"

namespace MARTe {

UEIDAQDataSource::UEIDAQDataSource() : MemoryDataSourceI() {
        poll_sleep_period = 100;
        deviceName = StreamString("");
        mapName = StreamString("");
}

UEIDAQDataSource::~UEIDAQDataSource() {
/*    if (mapid != 0){
       DqRtDmapStop(hd, mapid);
       DqRtDmapClose(hd, mapid);
    }
    if(hd){
        DqCloseIOM(hd);
    }
    DqCleanUpDAQLib();
*/
}

bool UEIDAQDataSource::Initialise(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::Initialise(data);
    name = data.GetName();
    ok = data.Read("Device", deviceName);
    if (!ok){
        REPORT_ERROR(ErrorManagement::ParametersError, "No device specified for UEIDAQDataSource %s", name.Buffer());        
    }
    if (ok){
        ok = data.Read("Map", mapName);
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "No map specified for UEIDAQDataSource %s", name.Buffer());        
        }
    }
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok){
        device = ord->Find(deviceName.Buffer());
        ok = device.IsValid();
        if (ok){
           REPORT_ERROR(ErrorManagement::Information, "Device %s found (at UEIDAQDataSource %s)", deviceName.Buffer(), name.Buffer());
        }else{
           REPORT_ERROR(ErrorManagement::ParametersError, "Unable to find device %s (at UEIDAQDataSource %s)", deviceName.Buffer(), name.Buffer());
        }
    }
    if (ok){
        StreamString mapPath = StreamString("");
        mapPath += deviceName;
        mapPath += ".Maps.";
        mapPath += mapName;
        map = ord->Find(mapPath.Buffer());
        ok = map.IsValid();
        if (ok){
            REPORT_ERROR(ErrorManagement::Information, "Map %s found (at UEIDAQDataSource %s, path: %s)", mapName.Buffer(), name.Buffer(), mapPath.Buffer());
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError, "Unable to find map %s (at UEIDAQDataSource %s, path: %s)", mapName.Buffer(), name.Buffer(), mapPath.Buffer());
        }
    }
    return ok;
}

bool UEIDAQDataSource::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::SetConfiguredDatabase(data);
    /*if (ok) {
        ok = (GetNumberOfSignals() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly one signal must be defined (OutputData)");
        }
    }
    if (ok) {
        ok = (GetSignalType(0u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The type of the first signal (Trigger) must be UnsignedInteger8");
        }
    }
    if (ok) {
        uint32 elements;
        ok = GetSignalNumberOfElements(0u, elements);
//        if (ok) {
//            ok = (elements == 3u);
//        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The first signal (Trigger) must have 10 elements");
        }
    }*/

    return ok;
}

bool UEIDAQDataSource::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples){
    return true;
}

bool UEIDAQDataSource::Synchronise() {
    //Start to poll for next packet to the Map. The memory access is handled by the Map Container
    bool ok = false;
    while(!ok){
        ok = (map->PollForNewPacket((uint32*)&memory));
        if (!ok){
            Sleep::MSec(10);    //To change
        }
    }
    return true;
}

const char8* UEIDAQDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
        return "MemoryMapSynchronisedInputBroker";
}

bool UEIDAQDataSource::GetMapAddr(){
    return true;
}

bool UEIDAQDataSource::PollForNextPacket(){
    bool next_packet = false;
    bool ok = true;
    return ok;
}

bool UEIDAQDataSource::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName){
    return true;
}

CLASS_REGISTER(UEIDAQDataSource, "1.0")
}
