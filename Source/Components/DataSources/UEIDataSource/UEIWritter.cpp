/**
 * @file UEIWritter.cpp
 * @brief Source file for class UEIWritter
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
 * the class UEIWritter (public, protected, and private). Be aware that some
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
#include "UEIWritter.h"

namespace MARTe {

UEIWritter::UEIWritter() : MemoryDataSourceI() {
        deviceName = StreamString("");
        mapName = StreamString("");
        signalTypes = NULL_PTR(TypeDescriptor*);
        signalAddresses = NULL_PTR(uint8**);
}

UEIWritter::~UEIWritter() {
    if (signalTypes != NULL_PTR(TypeDescriptor*)){
        delete [] signalTypes;
    }
    if (signalAddresses != NULL_PTR(uint8**)){
        delete [] signalAddresses;
    }
}

bool UEIWritter::Initialise(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Get the name of the DataSource
    name = data.GetName();
    ok = data.Read("Device", deviceName);
    if (!ok){
        REPORT_ERROR(ErrorManagement::ParametersError, "No device specified for UEIWritter %s", name.Buffer());        
    }
    if (ok){
        ok = data.Read("Map", mapName);
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "No map specified for UEIWritter %s", name.Buffer());        
        }
    }
    //Retrieve the reference to the UEIDAQ device
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok){
        device = ord->Find(deviceName.Buffer());
        ok = device.IsValid();
        if (ok){
           REPORT_ERROR(ErrorManagement::Information, "Device %s found (at UEIWritter %s)", deviceName.Buffer(), name.Buffer());
        }else{
           REPORT_ERROR(ErrorManagement::ParametersError, "Unable to find device %s (at UEIWritter %s)", deviceName.Buffer(), name.Buffer());
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
            REPORT_ERROR(ErrorManagement::Information, "Map %s found (at UEIWritter %s, path: %s)", mapName.Buffer(), name.Buffer(), mapPath.Buffer());
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError, "Unable to find map %s (at UEIWritter %s, path: %s)", mapName.Buffer(), name.Buffer(), mapPath.Buffer());
        }
    }
    //Only one DataSource can be associated with a map, check that the map is not being used by another DataSource
    if (ok){
        ok = (map->RegisterDS(OutputSignals));     //Check if the Map has already been assigned to a DS (and assign it if not)
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "The map %s requested by DataSource %s is already assigned to another DataSource for output signals", mapName.Buffer(), name.Buffer());
        }
    }
    return ok;
}

bool UEIWritter::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::SetConfiguredDatabase(data);
    ok &= AllocateMemory();
    //Check the number of signals into the datasource
    if (ok){
        uint32 expectedSignalN = 0u;    //For outputs to the UEI, no status or timestamp signals are expected
        ok = map->GetNumberOfChannels(OutputSignals, expectedSignalN);
        if (!ok) REPORT_ERROR(ErrorManagement::InitialisationError, "Could not get number of Output channels for Map %s", map->GetName());
        if(ok){
            ok = (numberOfSignals == expectedSignalN);  
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Number of signals in DataSource %s must match signal number in Map %s, (%d channel signals)", name.Buffer(), map->GetName(), expectedSignalN);
            }
        }
    }
    //Check that all the signals have NumberOfElements 1 (1 signal = 1 physical channel) and register the number of signals
    if (ok){
        for (uint32 i = 0; i < numberOfSignals && ok; i++){
            uint32 nOfElements = 0u;
            ok = (GetSignalNumberOfElements(i, nOfElements));
            if (!ok) REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve number of elements for signal %d in DataSource %s", i, name.Buffer());
            if (ok){
                ok = (nOfElements == 1u);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Signals in DataSource %s must have only 1 element", name.Buffer());
                }
            }
        }
    }
    //DESIGN DECISION : One DataSource can only interact with a single GAM, multiple gams would cause the map to re-refresh on the same cycle
    //the operation of the DS would be unpredictable. Therefore only one GAM is supposed to interact with this DS
    if (ok){
        ok = (GetNumberOfFunctions() <= 1u);  //0 GAMS is also ok, you can leave the DataSource "unconnected"
        if (!ok) REPORT_ERROR(ErrorManagement::InitialisationError, "This DataSource can only interact to a single GAM, currently %d GAMs are configured to interface this DataSource", GetNumberOfFunctions());
    }
    //Check NumberOfSamples for all the signals to be equal and 1 if the Map is XDMap or equal for all signals in XVMap.
    //The first signal (Status) must only have 1 sample
    if (ok){
        bool firstSignalVisited = false;
        for (uint32 i = 0u; i < numberOfSignals && ok; i++){
            uint32 samples = 0;
            ok &= (GetFunctionSignalSamples(OutputSignals,0,i,samples));
            if (!firstSignalVisited){
                nSamples = samples;
                firstSignalVisited = true;
            }
            if (firstSignalVisited){
                ok &= (samples == nSamples);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "All signals for DataSource %s must have the same number of samples", name.Buffer());
                }
            }
        }
        //If the sample number is >= 1 and equal for all signals check what to do depending on the
        //assigned map, this is responsibility of the datasource, not the map
        if (ok){
            switch(map->GetType()){
                case RTDMAP:
                    //For RtDMap, only signals with 1 sample are allowed
                    ok = (nSamples == 1u);
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Map %s for DataSource %s only supports signals with 1 sample (RtDMap)", map->GetName(), name.Buffer());
                    }
                break;
                case RTVMAP:
                    //For RtVMap, only signals with more than 1 sample are allowed
                    ok = (nSamples > 1u);
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid sample number for signals on Map %s for DataSource %s (RtVMap needs more than 1 sample to be retrieved)", map->GetName(), name.Buffer());
                    }
                break;
                default:
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid Map type supplied to DataSource %s", name.Buffer());
                    ok = false;
                break;
            }
        }
    }
    //Configure the Map to DataSource interface
    if (ok){
        //Define an array to signal types
        TypeDescriptor* signalTypes     = new TypeDescriptor[numberOfSignals];
        //Define an array to signal memory addresses
        uint8** signalAddresses         = new uint8*[numberOfSignals];
        //Traverse all the signals defined for this DataSource
        for (uint32 i = 0; i < numberOfSignals && ok; i++){
            signalTypes[i] = GetSignalType(i);
        }
        if (ok){
            //get the signal memory addresses for the output signals on the DataSource
            for (uint32 i = 0; i < numberOfSignals && ok; i++){
                signalAddresses[i] = reinterpret_cast<uint8*>(memory)+(signalOffsets[i]);
            }
        }
        if (ok){
            //Sent the configuration of the DS to the map object, with this configuration, the object now has the information
            //needed to share the data obtained to the DataSource in MARTe
            ok = map->ConfigureOutputsForDataSource(nSamples, numberOfSignals, signalAddresses, signalTypes);
            if (!ok) REPORT_ERROR(ErrorManagement::InitialisationError, "Could not set number of MARTe signal samples in DataSource %s", name.Buffer());
        }
    }
    return ok;
}

bool UEIWritter::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples){
    return true;
}

bool UEIWritter::Synchronise() {
    bool ok = true;
    uint8** sigs = new uint8*[2];
    sigs[0] = reinterpret_cast<uint8*>(memory)+(signalOffsets[0]);
    sigs[1] = reinterpret_cast<uint8*>(memory)+(signalOffsets[1]);
    if (!map->GetMapStatus()){
        ok &= map->StartMap();
        if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not start Map %s in DataSource %s", map->GetName(), name.Buffer());
        }
    }
    MapReturnCode outputCode;
    ok = map->WriteOutputs(outputCode);
    return ok;
}

bool UEIWritter::AllocateMemory(){
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
                ret = GetFunctionSignalSamples(OutputSignals,0,s,thisSignalSampleN);
                stateMemorySize += (thisSignalMemorySize * numberOfBuffers * thisSignalSampleN);
                signalSize[s] = thisSignalMemorySize * thisSignalSampleN;
            }
            if (!ret) REPORT_ERROR(ErrorManagement::InitialisationError, "Could not obtain information for signal number %d on UEIWritter %s", s, name.Buffer());
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

const char8* UEIWritter::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    //The Datasource is synchronous to the Reception of data from UEIDAQ device
    if (direction == InputSignals){
        //Only input signals are supported for this DataSource
        REPORT_ERROR(ErrorManagement::InternalSetupError, "DataSource %s only supports output signals (UEIWritter)", name.Buffer());
        return "";
    }else{
        return "MemoryMapSynchronisedOutputBroker";
    }
}

bool UEIWritter::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName){
    //To terminate an Input copy, stop the DAQ Map on the device and set the device as not synched yet to allow new
    //map start/enable procedure
    map->StopMap();
    return true;
}

CLASS_REGISTER(UEIWritter, "1.0")
}
