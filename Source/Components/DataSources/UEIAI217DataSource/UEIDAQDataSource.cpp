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
    //Check the output signals (the ones coming from UEIDAQ into the MARTe application)
    //The signal types for each signal must be in accordance to that of the device the channel
    //is being read from, e.g. a uint8 cannot be read from an analog device of 24 bit resolution
    //All the signals within a DataSource must be the same type
    if (ok){
        uint32 currentSignalElement = 0;
        uint32 numberOfSignals = GetNumberOfSignals();
        for (uint32 i = 0u; i < numberOfSignals && ok; i++){ //TODO change when input signals are allowed
            uint32 signalNOfElements = 0u;
            ok = (GetSignalNumberOfElements(i, signalNOfElements));
            if (!ok){
                //Could not get number of elements for signal
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not retrieve signal numberOfElements for signal %d in DataSource %s", i+1, name.Buffer());
            }
            if (ok){
                ok = (map->IsSignalAllowed(currentSignalElement, currentSignalElement+signalNOfElements, GetSignalType(i), OUTPUT_CHANNEL));
                currentSignalElement += signalNOfElements; //Update the counter taking in ming the already allowed (or not) signal
                if (!ok){
                    //Invalid signal type or range for signal regarding the IO signal type
                    REPORT_ERROR(ErrorManagement::ParametersError, "Mismatch in signal %d for DataSource %s (check type/signal length)", i+1, name.Buffer());
                }
            }
        }
    }
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
        ok = (map->PollForNewPacket(reinterpret_cast<float64*>(memory)));
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
