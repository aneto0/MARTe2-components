/**
 * @file UEIReader.cpp
 * @brief Source file for class UEIReader
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
 * the class UEIReader (public, protected, and private). Be aware that some
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
#include "UEIReader.h"

namespace MARTe {

UEIReader::UEIReader() : UEIDataSourceI() {
        PollSleepPeriod = 100;
        deviceName = StreamString("");
        mapName = StreamString("");
        firstSync = true;
}

UEIReader::~UEIReader() {
}

bool UEIReader::Initialise(StructuredDataI &data) {
    bool ok = UEIDataSourceI::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);

    //Retrieve PollSleepPeriod, the time to wait between Poll requests, only meaningful for RtVMap (optional)
    if (ok){
        helper.Read("PollSleepPeriod", PollSleepPeriod, 100);
    }
    //Only one DataSource can be associated with a map, check that the map is not being used by another DataSource
    if (ok){
        ok = (map->RegisterDS(InputSignals));     //Check if the Map has already been assigned to a DS (and assign it if not)
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "The map %s requested by DataSource %s is already assigned to another DataSource for input signals", mapName.Buffer(), name.Buffer());
        }
    }
    return ok;
}

bool UEIReader::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::SetConfiguredDatabase(data);
    ok &= AllocateMemory();
    //Check the number of signals into the datasource
    uint32 expectedSignalN = 0u;
    if (ok){
        uint32 expectedSignalN = 0u;
        ok = map->GetNumberOfChannels(InputSignals, expectedSignalN); //TODO, check for Output channels
        if (!ok) REPORT_ERROR(ErrorManagement::InitialisationError, "Could not get number of Input channels for Map %s", map->GetName());
        if(ok){
            expectedSignalN += 2u;  //Add 2 channel for DAQ status and timestamp
            ok = (numberOfSignals == expectedSignalN);  
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Number of signals in DataSource %s must match signal number in Map %s, (status signal + timestamp signal + %d channel signals)", name.Buffer(), map->GetName(), expectedSignalN-2);
            }
        }
    }
    //First Signal from this DataSource must be a Status uint32 signal
    if (ok){
        StreamString statusSignalName;
        ok = (GetSignalName(0, statusSignalName));
        if (ok){
            ok = (statusSignalName == StreamString("Status"));
            ok &= (GetSignalType(0) == UnsignedInteger32Bit);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Signal 0 for this DataSource must be a \"Status\" uint32 signal");
            }
        }
    }
    //Second Signal from this DataSource must be a Timestamp uint64 signal
    if (ok){
        StreamString timestampSignalName;
        ok = (GetSignalName(1, timestampSignalName));
        if (ok){
            ok = (timestampSignalName == StreamString("Timestamp"));
            ok &= (GetSignalType(1) == UnsignedInteger64Bit);
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Signal 1 for this DataSource must be a \"Timestamp\" uint64 signal");
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
    //Check that all the signals have NumberOfDimensions 1 (1 signal = 1 physical channel) and register the number of signals
    if (ok){
        for (uint32 i = 0; i < numberOfSignals && ok; i++){
            uint8 nOfDimensions = 0u;
            ok = (GetSignalNumberOfDimensions(i, nOfDimensions));
            if (!ok) REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve number of Dimensions for signal %d in DataSource %s", i, name.Buffer());
            if (ok){
                ok = (nOfDimensions <= 1u);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Signals in DataSource %s must have only 1 dimension", name.Buffer());
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
        uint32 samples = 0;
        ok = (GetFunctionSignalSamples(InputSignals,0,0,samples));
        ok &= (samples == 1u);
        if (!ok){
                REPORT_ERROR(ErrorManagement::ParametersError, "Status for DataSource %s must have 1 sample only", name.Buffer());
        }
    }
    if (ok){
        bool firstSignalVisited = false;
        for (uint32 i = 1u; i < numberOfSignals && ok; i++){
            uint32 samples = 0;
            ok &= (GetFunctionSignalSamples(InputSignals,0,i,samples));
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
                    if (!ok) REPORT_ERROR(ErrorManagement::InitialisationError, "Map %s for DataSource %s only supports signals with 1 sample (RtDMap)", map->GetName(), name.Buffer());
                break;
                case RTVMAP:
                    //For RtVMap, only signals with 1 sample or more are allowed
                    ok = (nSamples > 1u);
                    if (!ok) REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid sample number for signals on Map %s for DataSource %s (RtVMap needs more than 1 sample to be retrieved)", map->GetName(), name.Buffer());
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
        TypeDescriptor* signalTypes     = new TypeDescriptor[numberOfSignals-2];
        //Define an array to signal memory addresses
        uint8** signalAddresses         = new uint8*[numberOfSignals-2];
        uint64* timestampSignalAddr     =  reinterpret_cast<uint64*>(memory+(signalOffsets[1]));
        //Traverse all the signals defined for this DataSource
        for (uint32 i = 0; i < numberOfSignals-2 && ok; i++){
            signalTypes[i] = GetSignalType(i+2);
        }
        if (ok){
            //get the signal memory addresses for the input signals on the DataSource
            for (uint32 i = 0; i < numberOfSignals-2 && ok; i++){
                signalAddresses[i] = reinterpret_cast<uint8*>(memory)+(signalOffsets[i+2]);
            }
        }
        if (ok){
            //Sent the configuration of the DS to the map object, with this configuration, the object now has the information
            //needed to share the data obtained to the DataSource in MARTe
            ok = map->ConfigureInputsForDataSource(nSamples, numberOfSignals-2, timestampSignalAddr, signalAddresses, signalTypes);
            if (!ok) REPORT_ERROR(ErrorManagement::InitialisationError, "Could not set number of MARTe signal samples in DataSource %s", name.Buffer());
        }
    }
    return ok;

}

bool UEIReader::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples){
    return true;
}

bool UEIReader::Synchronise() {
    bool ok = true;
    //Start the DAQ Map
    if (firstSync){
        //This delay is a measure to ensure MARTe is completely set up prior to starting the maps
        //If the map is started before MARTe is completely set up and the MARTe loop is not executed fast enough
        //The FIFOs on VMap could overflow
        Sleep::MSec(1000);  
        if (ok && !map->GetMapStatus()){
            ok = map->StartMap();
            firstSync = !ok;
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not start Map %s in DataSource %s", map->GetName(), name.Buffer());
            }else{
                REPORT_ERROR(ErrorManagement::Information, "Map %s started!", map->GetName());
	    }
        }
    }
    //Start to poll for next packet to the Map. The memory access is handled by the Map Container
    uint32* statusMemory = reinterpret_cast<uint32*>(memory);
    //Counters and flags to register errors
    uint16 localErrorCounter = 0u;
    uint8 FIFOError = 0u;
    uint8 GeneralError = 0u;
    uint8 ValidData = 0u;
    //Acquisition loop
    bool continueLoop = true;
    MapReturnCode outputCode;
    *statusMemory = 0;
    uint32 counter = 0;
    while(continueLoop && ok){
        ok &= (map->GetInputs(outputCode));
        switch (outputCode){
            case ERROR:
                GeneralError = 1;
                localErrorCounter ++;
            break;
            case FIFO_OVERFLOW:
                FIFOError = 1;
                localErrorCounter ++;
            break;
            case NEW_DATA_AVAILABLE:
                continueLoop = false;
            break;
            case NO_NEW_DATA_AVAILABLE:
                continueLoop = true;
            break;
        }
        if (localErrorCounter > 100) {
            //Maximum numbers of errors achieved
            ValidData = 1; //Mark the data as invalid 
            continueLoop = false;          
        }
        if (continueLoop) Sleep::MSec(PollSleepPeriod);
        if (counter > 100) break;
        counter++;
        
    }
    *statusMemory = (localErrorCounter | ValidData << 16 | FIFOError << 17 | GeneralError << 18);
    return ok;
}

const char8* UEIReader::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    //The Datasource is synchronous to the Reception of data from UEIDAQ device
    if (direction == InputSignals){
            return "MemoryMapSynchronisedInputBroker";
    }else{
        //Only input signals are supported for this DataSource
        REPORT_ERROR(ErrorManagement::InternalSetupError, "DataSource %s only supports input signals (UEIReader)", name.Buffer());
        return "";
    }
}

bool UEIReader::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName){
    //To terminate an Input copy, stop the DAQ Map on the device and set the device as not synched yet to allow new
    //map start/enable procedure
    if (map.IsValid()){
        map->StopMap();
    }
    firstSync = true;
    return true;
}

bool UEIReader::AllocateMemory(){
    return UEIDataSourceI::AllocateMemory(InputSignals);
}

CLASS_REGISTER(UEIReader, "1.0")
}
