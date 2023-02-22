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
}

bool UEIDAQDataSource::Initialise(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //Get the name of the DataSource
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
    //Retrieve PollSleepPeriod (optional)
    if (ok){
        helper.Read("PollSleepPeriod", poll_sleep_period, 100);
    }
    //Retrieve the reference to the UEIDAQ device
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
    //Retrieve the reference to the selected map within the UEIDAQ device
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
    //Only one DataSource can be associated with a map, check that the map is not being used by another DataSource
    if (ok){
        ok = (!map->RegisterDS(OutputSignals));     //Check if the Map has already been assigned to a DS (and assign it if not)
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "The map %s requested by DataSource %s is already assigned to another DataSource!", mapName.Buffer(), name.Buffer());
        }
    }
    return ok;
}

bool UEIDAQDataSource::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::SetConfiguredDatabase(data);
    //Check the number of signals into the datasource
    if (ok){
        ok = (numberOfSignals > 0);
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "At least one signal must be provided for DataSource %s", name.Buffer());
        }
    }
    //All signals within a DataSource must be of the same type (simplification condition). Therefore, no mismatch in the 
    // type of different signals is allowed
    if (ok){
        //Get the type of the first signal (we know we do have at least 1 signal)
        signalType = GetSignalType(0);
        ok = (signalType == Float64Bit); //TODO, for now we only support scaled float64 data, we'll update on this later
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Signal type inconsistency in DataSource %s (all signals must be float64 bit for now)", name.Buffer());        
        }
    }
    if(ok){
        //Traverse all the signals defined for this DataSource
        for (uint32 i = 0; i < numberOfSignals && ok; i++){
            //Check that all the signals do have the same type as the selected one (the first signal)
            ok = (signalType == GetSignalType(i));
        }
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Signal type inconsistency in DataSource %s (all signals must be of same type)", name.Buffer());
        }
    }
    //Check that all the devices of the map assigned to this DS support the selected signal Type
    if (ok){
        //Check if the map does support the signal type specified
        ok = map->IsSignalAllowed(signalType,OUTPUT_CHANNEL); //TODO as the current DS only supports OUTPUT signals
        if(!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Signal type provided for DataSource %s is incompatible with specified map", name.Buffer());
        }
    }
    //Check the number of elements of all the signals combined, which cannot be larger than the number of channels in the map
    if (ok){
        uint32 nChan = 0;
        for (uint32 i = 0u; i < numberOfSignals && ok; i++){
            uint32 nElements;
            ok = GetSignalNumberOfElements(i, nElements);
            nOutputChannels += nElements;
        }
        if (ok){ 
            ok = (map->GetNumberOfChannels(OUTPUT_CHANNEL, nChan));
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve the number of channels in map for DataSource %s", name.Buffer());
            }
        }
        if (ok){
            ok = (nOutputChannels <= nChan);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "DataSource %s cannot have more signal elements than channels read from IOM", name.Buffer());
            }
        }
    }
    //Check NumberOfSamples for all the signals to be equal and 1 if the Map is XDMap or coherent with XVMap.
    //TODO more checks
    return ok;
}

bool UEIDAQDataSource::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples){
    return true;
}

bool UEIDAQDataSource::Synchronise() {
    //Start to poll for next packet to the Map. The memory access is handled by the Map Container
    bool ok = false;
    while(!ok){
        ok = (map->PollForNewPacket(reinterpret_cast<float64*>(memory), nOutputChannels));
        if (!ok){
            Sleep::MSec(poll_sleep_period);    //To change
        }
    }
    return true;
}

const char8* UEIDAQDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    //The Datasource is synchronous to the Reception of data from UEIDAQ device
    if (direction == InputSignals){
        return "MemoryMapSynchronisedInputBroker";
    }else{
        //Only input signals are supported for this DataSource
        REPORT_ERROR(ErrorManagement::InternalSetupError, "UEIDAQDataSource::GetBrokerName - DataSource %s only supports input signals", name.Buffer());
        return "";
    }
}

bool UEIDAQDataSource::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName){
    return true;
}

CLASS_REGISTER(UEIDAQDataSource, "1.0")
}
