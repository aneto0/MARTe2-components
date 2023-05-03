/**
 * @file UEIDataSourceI.cpp
 * @brief Source file for class UEIDataSourceI
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
 * the class UEIDataSourceI (public, protected, and private). Be aware that some
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
#include "UEIDataSourceI.h"

namespace MARTe {

UEIDataSourceI::UEIDataSourceI() : MemoryDataSourceI() {
        PollSleepPeriod = 100;
        deviceName = StreamString("");
        mapName = StreamString("");
        firstSync = true;
}

UEIDataSourceI::~UEIDataSourceI() {
}

bool UEIDataSourceI::Initialise(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Get the name of the DataSource
    name = data.GetName();
    ok = data.Read("Device", deviceName);
    if (!ok){
        REPORT_ERROR(ErrorManagement::ParametersError, "No device specified for UEIDataSourceI %s", name.Buffer());        
    }
    if (ok){
        ok = data.Read("Map", mapName);
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "No map specified for UEIDataSourceI %s", name.Buffer());        
        }
    }
    //Retrieve the reference to the UEIDAQ device
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok){
        device = ord->Find(deviceName.Buffer());
        ok = device.IsValid();
        if (ok){
           REPORT_ERROR(ErrorManagement::Information, "Device %s found (at UEIDataSourceI %s)", deviceName.Buffer(), name.Buffer());
        }else{
           REPORT_ERROR(ErrorManagement::ParametersError, "Unable to find device %s (at UEIDataSourceI %s)", deviceName.Buffer(), name.Buffer());
        }
    }
    //Retrieve the reference to the selected map within the UEIDAQ device
    if (ok){
        StreamString mapPath = StreamString("");
        mapPath += deviceName;
        mapPath += ".Maps.";
        mapPath += mapName;
        map = ord->Find(mapPath.Buffer());
        ok = map.IsValid();
        if (ok){
            REPORT_ERROR(ErrorManagement::Information, "Map %s found (at UEIDataSourceI %s, path: %s)", mapName.Buffer(), name.Buffer(), mapPath.Buffer());
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError, "Unable to find map %s (at UEIDataSourceI %s, path: %s)", mapName.Buffer(), name.Buffer(), mapPath.Buffer());
        }
    }
    
    return ok;
}

bool UEIDataSourceI::SetConfiguredDatabase(){
    //This method must be implemented by the derived classes
    return false;
}

bool UEIDataSourceI::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples){
    return true;
}

bool UEIDataSourceI::AllocateMemory(SignalDirection direction){
    uint32 nOfSignals = GetNumberOfSignals();
    bool ret = true;
    if (memory == NULL_PTR(uint8*)){    
        if (ret) {
            if (nOfSignals > 0u) {
                signalOffsets = new uint32[nOfSignals];
                ret = (signalOffsets != NULL_PTR(uint32*));
                if (ret) {
                    signalSize = new uint32[nOfSignals];
                    ret = (signalSize != NULL_PTR(uint32*));
                }
            }
        }
        stateMemorySize = 0u;
        for (uint32 s = 0u; (s < nOfSignals) && (ret); s++) {
            uint32 thisSignalMemorySize;
            ret = GetSignalByteSize(s, thisSignalMemorySize);
            if (ret) {
                if (signalOffsets != NULL_PTR(uint32 *)) {
                    signalOffsets[s] = stateMemorySize;
                }
            }
            if (ret) {
                ret = (thisSignalMemorySize > 0u);
            }
            if (ret) {
                uint32 thisSignalSampleN;
                ret = GetFunctionSignalSamples(direction,0,s,thisSignalSampleN);
                stateMemorySize += (thisSignalMemorySize * numberOfBuffers * thisSignalSampleN);
                signalSize[s] = thisSignalMemorySize * thisSignalSampleN;
            }
            if (!ret) REPORT_ERROR(ErrorManagement::InitialisationError, "Could not obtain information for signal number %d on UEIDataSourceI %s", s, name.Buffer());
        }
        uint32 numberOfStateBuffers = GetNumberOfStatefulMemoryBuffers();
        if (ret) {
            ret = (numberOfStateBuffers > 0u);
        }
        if (ret) {
            totalMemorySize = stateMemorySize * numberOfStateBuffers;
            if (memoryHeap != NULL_PTR(HeapI *)) {
                memory = reinterpret_cast<uint8 *>(memoryHeap->Malloc(totalMemorySize));
            }
            ret = MemoryOperationsHelper::Set(memory, '\0', totalMemorySize);
        }
    }
    return ret;
}

bool UEIDataSourceI::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName){
    //To terminate an Input copy, stop the DAQ Map on the device and set the device as not synched yet to allow new
    //map start/enable procedure
    return true;
}

const char8* UEIDataSourceI::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    //This is a virtual method that must be implemented by the DataSources derived from this class    
    return "";
}

bool UEIDataSourceI::Synchronise() {
    //This is a virtual method that must be implemented by the DataSources derived from this class
    return false;
}

CLASS_REGISTER(UEIDataSourceI, "1.0")
}
