/**
 * @file UEIRtVMap.cpp
 * @brief Source file for class UEIRtVMap
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
 * the class UEIRtVMap (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CLASSMETHODREGISTER.h"
#include "UEIRtVMap.h"
#include <iostream>     //Provisional for testing


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UEIRtVMap::UEIRtVMap() : UEIMapContainer(),
    EmbeddedServiceMethodBinderT<UEIRtVMap>(*this, &UEIRtVMap::IndependentThreadCallback),
    executor(*this) {
    nBuffers = 0;
    nReadSamples = 0u;
    nWriteSamples = 0u;
    nSamplesMapExchange = 0;
}

UEIRtVMap::~UEIRtVMap(){
    //try to clean the maps in case it was not already done beforehand
    CleanupMap();
    executor.Stop();
}

bool UEIRtVMap::StopMap(){
    bool ok = true;
    //First of all, we need to acquire the mutex to avoid issuing commands to the IOM while other
    // parts of the code access the IOM
    bool mutexAcquired = false;
    //The map mutex always needs to be locked in this call to prevent possible access to IOM by the other DataSource
    // independently of the execution mode of the Map
    ErrorManagement::ErrorType err;
    err = mapMutex.Lock();
    ok &= (err == ErrorManagement::NoError);
    mutexAcquired = ok;
    //First, stop the Independent thread if necessary
    if (ok && executionMode == UEIMapIndependentThreadExecutionMode){
        //Since this call does indeed have the mutex locked, we know for sure that the 
        executor.Stop();
    }
    if (ok){
        if (DAQ_handle != 0 && mapid != 0){
            ok &= (DqRtVmapStop(DAQ_handle, mapid) >= 0);
        }
        if (ok){
            //Set to flag to signal that this map is not active
            mapStarted = false;
        }
    }
    //Release the mutex
    if (mutexAcquired){
        mapMutex.UnLock();
    }
    return ok;
}

bool UEIRtVMap::CleanupMap(){
    bool ok = true;
    if (DAQ_handle != 0){
        if(mapid){
            ok &= StopMap();
            ok &= (DqRtVmapClose(DAQ_handle, mapid) >= 0);
        }
        DAQ_handle = 0;
    }else{
        REPORT_ERROR(ErrorManagement::Information,"Map %s was not started, skipping cleanup!", name.Buffer());
    }
    return ok;
}

bool UEIRtVMap::Initialise(StructuredDataI &data){
    bool ok = UEIMapContainer::Initialise(data);
    StructuredDataIHelper helper = StructuredDataIHelper(data, this);
    //In the VMap mode we do need to provide a parameter "Samples" which states
    //the maximum number to be retrieved from the devices FIFOs for each VMap query
    //This parameter is important for VMap as it plays a very important role in system performance and FIFO overflow avoidance
    if (ok){
        ok = data.Read("Samples", nSamplesMapExchange);
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "Samples parameter not provided in Map %s, required for selected VMap mode.", name.Buffer());
        }
    }
    if (ok){
        ok = data.Read("NumberOfBuffers", nBuffers);
        if (!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfBuffers (Number of sub buffers for UEICircularBuffer) parameter not provided in Map %s, required for selected VMap mode.", name.Buffer());
        }
    }   
    //TODO for now we only retrieve the timestamp of the first member defined in inputs
    if (ok && nInputChannels > 0u){
        //For the RtVMap, the first channel of the first device is always going to be the map timestamp.
        inputMembersOrdered[0]->Inputs.timestampRequired = true;
    }
    //Check that the DMap is operated in RealTimeThread Mode, fail otherwise, this map does not support multithreading
    if (ok){
        if (executionMode == UEIMapIndependentThreadExecutionMode){
            REPORT_ERROR(ErrorManagement::Information, "Initialising VMap %s in IndependetThread mode", name.Buffer());
        }else if (executionMode == UEIMapRealTimeThreadExecutionMode){
            REPORT_ERROR(ErrorManagement::Information, "Initialising VMap %s in RealTimeThread mode", name.Buffer());
        }
    }
    return ok;
}

bool UEIRtVMap::ConfigureInputsForDataSource(uint32 nSamples, uint32 nChannels, uint64* inputTimestampAddress, uint8** signalAddresses, TypeDescriptor* signalTypes){
    bool ok = UEIMapContainer::ConfigureInputsForDataSource(nSamples, nChannels, inputTimestampAddress, signalAddresses, signalTypes);
    if (ok){
        ok = (nSamples != 0u);
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid provided sample number to be read from Map %s", name.Buffer());
        }
        nReadSamples = nSamples;
    }
    inputSignalsConfigured = ok;
    return ok;
}

bool UEIRtVMap::ConfigureOutputsForDataSource(uint32 nSamples, uint32 nChannels, uint8** signalAddresses, TypeDescriptor* signalTypes){
    bool ok = UEIMapContainer::ConfigureOutputsForDataSource(nSamples, nChannels, signalAddresses, signalTypes);
    if (ok){
        ok = (nSamples != 0u);
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid provided sample number to be write into Map %s", name.Buffer());
        }
        nWriteSamples = nSamples;
    }
    outputSignalsConfigured = ok;
    return ok;
}

bool UEIRtVMap::StartMap(){
    if (mapStarted) return true;
    bool ok = true;
    //Now that the map is to be started it is valuable to make a copy of the DAQ handle reference for deallocating the map upon
    //destruction of this object.
    //First of all, set all the UEICircularBuffer objects for each of the VMap queried channels
    //Configure the devices with channels to be retrieved information
    if (ok){
        for (uint32 i = 0u; i < nInputMembers && ok; i++){
            ReferenceT<UEIDevice> reference = inputMembersOrdered[i]->reference;
            ok &= reference.IsValid();
            if (ok){
                ok &= reference->SetInputChannelList(inputMembersOrdered[i]->Inputs.channels, inputMembersOrdered[i]->Inputs.nChannels);
                reference->timestampRequired = inputMembersOrdered[i]->Inputs.timestampRequired;
                //Init the buffers, only one sample retrieved and read back
                ok &= reference->InitBuffer(InputSignals, nBuffers, nSamplesMapExchange, nReadSamples);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Detected error while setting input channel list and input channel buffer in device %s", name.Buffer());
                }
            }else{
                REPORT_ERROR(ErrorManagement::CommunicationError, "Invalid reference retrieved for input member in Map %s upon start procedure", name.Buffer());
            }
        }
        for (uint32 i = 0u; i < nOutputMembers && ok; i++){
            ReferenceT<UEIDevice> reference = outputMembersOrdered[i]->reference;
            ok &= reference.IsValid();
            if (ok){
                ok &= reference->SetOutputChannelList(outputMembersOrdered[i]->Outputs.channels, outputMembersOrdered[i]->Outputs.nChannels);
                //Init the buffers, only one sample retrieved and read back
                ok &= reference->InitBuffer(OutputSignals, nBuffers, nWriteSamples, nSamplesMapExchange);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Detected error while setting input channel list and output channel buffer in device %s", name.Buffer());
                }
            }else{
                REPORT_ERROR(ErrorManagement::CommunicationError, "Invalid reference retrieved for input member in Map %s upon start procedure", name.Buffer());
            }
        }
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Device channel information setting failed");
        }
    }
    //Initialise the map always to retrieve fragmented packets, has no impact in data retrieving except if not used
    if (ok){
        DQ_RTMAP_PARAM vmapparam = {11500, 1518, 1000};  //This parameters are the maximum allowed
        ok = (DqRtVmapInitEx(DAQ_handle, &mapid, &vmapparam) >= 0); //Extended version of the VMapInit method for fragmented maps
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error on Initialising Map %s", name.Buffer());
        }
    }
    //Configure input channels
    if (ok){
        //Once the map is initialized correctly, the I/O channels need to be checked into the map itself
        //First the input channels are checked. The inputMembersOrdered pointer array is traversed in order,
        // yielding the ordered list of channels to sequence into the input map
        for (uint32 i = 0u; i < nInputMembers && ok; i++){
            uint8 devn;
            ReferenceT<UEIDevice> devReference = inputMembersOrdered[i]->reference;
            ok = (devReference.IsValid());  //This should not be necessary, but it is implemented for precaution
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Found invalid device reference on inputMember %i (devn%d) on Map %s", i, inputMembersOrdered[i]->devn, name.Buffer());
            }
            //Retrieve the configuration bitfields for this device and the selected channels
            if (ok){
                uint32* configurationBitfields = NULL_PTR(uint32*);
                uint32 nConfigurationBitfields = 0;
                devn = inputMembersOrdered[i]->devn;
                ok &= (devReference->ConfigureChannels(InputSignals, &configurationBitfields, nConfigurationBitfields, RTVMAP));
                int32 flag = DQ_VMAP_FIFO_STATUS;
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error configuring input channels for dev%d on Map %s", devn, name.Buffer());
                }
                if (ok){
                    ok &= (DqRtVmapAddChannel(DAQ_handle, mapid, devn, DQ_SS0IN, (int32*) configurationBitfields, &flag, 1) >= 0);
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Error adding input channels in IOM for dev%d on Map %s", devn, name.Buffer());
                    }
                }
                if (ok){
                    //In the case of AI, AO, AIO or DIO, the channels are interleaved into a signle FIFO, we need to check the number of channels into
                    //the map
                    if (devReference->GetType() == HARDWARE_LAYER_ANALOG_I  || devReference->GetType() == HARDWARE_LAYER_ANALOG_O  ||
                        devReference->GetType() == HARDWARE_LAYER_ANALOG_IO || devReference->GetType() == HARDWARE_LAYER_DIGITAL_IO){
                        //Check th channel number into the map
                        ok = (DqRtVmapSetChannelList(DAQ_handle, mapid, devn, DQ_SS0IN, (int32*) configurationBitfields, nConfigurationBitfields) >=0);
                        //Set the scan rate for the device on this channel
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "Could not set channel list in inputMember %i on Map %s", i, name.Buffer());
                        }
                        //Set the scan rate of the hardware layer to the sampling rate of the device
                        if (ok){
                            ok = (DqRtVmapSetScanRate(DAQ_handle, mapid, devn, DQ_SS0IN, sampleRate) >= 0);
                            if (!ok){
                                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not set scan rate in inputMember %i on Map %s", i, name.Buffer());
                            }
                        }    
                    }
                }
                if (configurationBitfields != NULL_PTR(uint32*)){
                    free(configurationBitfields);
                }
            }
        }
    }
    //Configure output channels
    if (ok){
        //Once the map is initialized correctly, the I/O channels need to be checked into the map itself
        //First the input channels are checked. The inputMembersOrdered pointer array is traversed in order,
        // yielding the ordered list of channels to sequence into the input map
        for (uint32 i = 0u; i < nOutputMembers && ok; i++){
            uint8 devn;
            ReferenceT<UEIDevice> devReference = outputMembersOrdered[i]->reference;
            ok = (devReference.IsValid());  //This should not be necessary, but it is implemented for precaution
            if (!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "Found invalid device reference on outputMember %i (devn%d) on Map %s", i, outputMembersOrdered[i]->devn, name.Buffer());
            }
            //Retrieve the configuration bitfields for this device and the selected channels
            if (ok){
                uint32* configurationBitfields = NULL_PTR(uint32*);
                uint32 nConfigurationBitfields = 0;
                devn = outputMembersOrdered[i]->devn;
                ok &= (devReference->ConfigureChannels(OutputSignals, &configurationBitfields, nConfigurationBitfields, RTVMAP));
                int32 flag = DQ_VMAP_FIFO_STATUS;
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error configuring output channels for dev%d on Map %s", devn, name.Buffer());
                }
                if (ok){
                    ok &= (DqRtVmapAddChannel(DAQ_handle, mapid, devn, DQ_SS0OUT, (int32*) configurationBitfields, &flag, 1) >= 0);
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Error adding output channels in IOM for dev%d on Map %s", devn, name.Buffer());
                    }
                }
                if (configurationBitfields != NULL_PTR(uint32*)){
                    free(configurationBitfields);
                }
                if (ok){
                    //In the case of AO, AIO or DIO, the channels are interleaved into a signle FIFO, we need to check the number of channels into
                    //the map
                    if (devReference->GetType() == HARDWARE_LAYER_ANALOG_O  ||
                        devReference->GetType() == HARDWARE_LAYER_ANALOG_IO || devReference->GetType() == HARDWARE_LAYER_DIGITAL_IO){
                        //Check th channel number into the map
                        ok = (DqRtVmapSetChannelList(DAQ_handle, mapid, devn, DQ_SS0OUT, (int32*) configurationBitfields, nConfigurationBitfields) >=0);
                        //Set the scan rate for the device on this channel
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::InitialisationError, "Could set channel list in outputMember %i on Map %s", i, name.Buffer());
                        }
                        //Set the scan rate of the hardware layer to the sampling rate of the device
                        if (ok){
                            ok = (DqRtVmapSetScanRate(DAQ_handle, mapid, devn, DQ_SS0OUT, sampleRate) >= 0);
                            if (!ok){
                                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not set scan rate in outputMember %i on Map %s", i, name.Buffer());
                            }
                        }    
                    }
                }
            }
        }
    }
    //Let's start the layers to start acquiring data
    if (ok){
        ok = EnableMap();
    }
    //Now, if the map is set as executionMode IndependentThread mode, start the thread which will provide the data by refre
    // refreshing the map
    if (ok && executionMode == UEIMapIndependentThreadExecutionMode){
        executor.Start();
    }    
    return ok;
}

bool UEIRtVMap::ExchangeMap(MapReturnCode& outputCode){
    bool ok = (mapStarted && mapCoherent);
    bool mutexAcquired = false;
    if (ok && (nOutputMembers > 0)) ok &= (outputSignalsConfigured);
    if (ok && (nInputMembers  > 0)) ok &= (inputSignalsConfigured);
    if (!ok){
        REPORT_ERROR(ErrorManagement::InitialisationError, "VMap %s must be enabled and input signals configured before polling", name.Buffer());
        outputCode = ERROR;
    }
    //Before any operation with the IOM, check the execution mode of the Map, if it is IndependentThreadMode we do need to 
    //acquire the mutex first to allow for thread-safe operation
    if (ok && (executionMode == UEIMapIndependentThreadExecutionMode)){
        //Acquire the mutex
        ErrorManagement::ErrorType err;
        err = mapMutex.Lock();
        ok &= (err == ErrorManagement::NoError);
        mutexAcquired = ok;
    }
    //Before the map exchange, set the outputs into the map (if any)
    if (ok){
        for (uint32 i = 0u; i < nOutputMembers && ok; i++){
            ReferenceT<UEIDevice> thisDevice = outputMembersOrdered[i]->reference;
            int32 updatesAccepted = 0;
            uint32 maxRequestedSize = outputMembersOrdered[i]->Outputs.requestSize;
            uint32 maxBytesInBuffer = thisDevice->outputChannelsBuffer->GetAvailableBytesToRead();
            uint32 requestSize = std::min(maxRequestedSize, maxBytesInBuffer);
            uint8* bufferPointer = new uint8[requestSize];
            ok &= (bufferPointer != NULL_PTR(uint8*));
            if(!ok){
                REPORT_ERROR(ErrorManagement::CommunicationError, "Error while allocating output temporal buffer");
            }
            if (ok){
                ok &= thisDevice->outputChannelsBuffer->ReadBytes(requestSize, bufferPointer, false);            
                if (!ok){
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Error while reading samples from output buffer on %s in Map %s", thisDevice->GetName(), name.Buffer());
                }
            }
            if (ok){
                ok &= (DqRtVmapAddOutputData(DAQ_handle, mapid, i, requestSize, &updatesAccepted, bufferPointer) >= 0);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Error while copying output samples into Map %s", name.Buffer());
                }
            }
        }
    }
    if (ok){
        //Refresh the VMap
        int32 refreshReturn = 0;
        refreshReturn = DqRtVmapRefresh(DAQ_handle, mapid, 0);
        ok = (refreshReturn == DQ_SUCCESS);
        if (ok){
            //If the map was correclty refreshed we proceed to reading the samples contained in the VMap structure
            //In this case, since we received an update on the VMap content, feed it to each of the UEICircularBuffers
            for (uint32 i = 0; i < nInputMembers && ok; i++){
                ReferenceT<UEIDevice> thisDevice = inputMembersOrdered[i]->reference;
                uint8 devn = inputMembersOrdered[i]->devn;
                ok &= thisDevice.IsValid();
                if (ok){
                    //For each of the input memebrs (in order of assignment) feed the new data into the buffers.
                    //Retrieve the input address for the circularBuffer to write new data
                    uint8* bufferWriteAddress = thisDevice->inputChannelsBuffer->writePointer;
                    //Prior to any write, check that the buffer has enough space to accept the new data
                    if (thisDevice->inputChannelsBuffer->CheckAvailableSpace()){
                        int32 dataWritten = 0;
                        int32 avl_size = 0;
                        uint32 requestedPacketSize = inputMembersOrdered[i]->Inputs.requestSize;
                        ok = (DqRtVmapGetInputData(DAQ_handle, mapid, i, requestedPacketSize, &dataWritten, &avl_size, bufferWriteAddress) >= 0);
                        if (ok){
                            thisDevice->inputChannelsBuffer->AdvanceBufferIndex(dataWritten);
                        }else{
                            REPORT_ERROR(ErrorManagement::CommunicationError, "Error while writting data to the circular buffer");
                        }
                    }else{ 
                        REPORT_ERROR(ErrorManagement::CommunicationError, "Error while writting to circular buffer, not enough space available");
                    }
                }
            }
            //If the map was correctly refreshed, then see how much of the output data was actually set into the output FIFOs
            //and update the output buffers accordingly
            for (uint32 i = 0; i < nOutputMembers && ok; i++){
                ReferenceT<UEIDevice> thisDevice = outputMembersOrdered[i]->reference;
                uint8 devn = outputMembersOrdered[i]->devn;
                ok &= thisDevice.IsValid();
                if (ok){
                    //For each of the output memebrs (in order of assignment) advance the buffer the number 
                    // of written bytes into the FIFOs
                    int32 avl_size = 0;
                    int32 transmittedBytes = 0;
                    ok &= (DqRtVmapGetOutputDataSz(DAQ_handle, mapid, i, &transmittedBytes, &avl_size) >= 0);
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::CommunicationError, "Error during output written size in Map %s refresh", name.Buffer());
                    }
                    if (ok){
                        ok &= thisDevice->outputChannelsBuffer->AdvanceReadPointer((uint32)transmittedBytes);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::CommunicationError, "Error during readPointer advancement in Map %s refresh", name.Buffer());
                        }
                    }
                }
            }
        }else{
            if (refreshReturn == DQ_FIFO_OVERFLOW){
                outputCode = FIFO_OVERFLOW; //Signal that an error occurred during map refresh due to FIFO overflow
                //Upon FIFO overflow for the map, the map must be restarted prior to getting any new samples, otherwise the
                //refresh method will not return appropriately
                REPORT_ERROR(ErrorManagement::CommunicationError, "Device FIFO overflow for Map %s VMAP, restarting the map", name.Buffer());
            }else{
                outputCode = ERROR; //Signal that an error occurred during map refresh due to other kind of errors
                //The refresh method failed for other reasons, report the error and carry on.
                REPORT_ERROR(ErrorManagement::CommunicationError, "Refresh of VMap for Map %s failed, restarting the map", name.Buffer());
            }
            bool restartOk = (ResetVMap());    //Reset the map for the next iteration
            if (!restartOk){
                REPORT_ERROR(ErrorManagement::CommunicationError, "Could not restart the Map %s succesfully", name.Buffer());
            }else{
                //Id the Map refresh is successful, then the circular buffers must be reset, all the stored samples are discarded
                //If not done subsequent FIFO overflows could force a circular buffer to run out of space and throw errors or samples
                for (uint8 i = 0; i < nInputMembers && restartOk; i++){
                    //in this loop, if ok = false, then at least one of the buffers is not reaUEIRtVMapdy yet
                    restartOk = (inputMembersOrdered[i]->reference->inputChannelsBuffer->ResetBuffer());
                }
                if (!restartOk){
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Error while resetting circular buffers on Map %s", name.Buffer());
                }
            }
        }
    }
    //Before returning from this call, if the mutex is active and we're working in IndependentThreadMode release the mutex
    if (mutexAcquired && (executionMode == UEIMapIndependentThreadExecutionMode)){
        //Release the mutex
        ok &= mapMutex.UnLock();
    }
    return ok;
}

bool UEIRtVMap::GetInputs(MapReturnCode& outputCode){
    bool ok = (inputSignalsConfigured && mapStarted && mapCoherent);
    if (!ok){
        if (!inputSignalsConfigured) REPORT_ERROR(ErrorManagement::CommunicationError, "Map %s does not have any configured input signal", name.Buffer());
        if (!mapStarted) REPORT_ERROR(ErrorManagement::CommunicationError, "Map %s is not started", name.Buffer());
        if (!mapCoherent) REPORT_ERROR(ErrorManagement::CommunicationError, "Map %s is not coherent", name.Buffer());
    }
    bool mutexAcquired = false;    
    if (ok){
        //The map mutex always needs to be locked in this call to prevent possible access to IOM by the other DataSource
        // independently of the execution mode of the Map
        ErrorManagement::ErrorType err;
        err = mapMutex.Lock();
        ok &= (err == ErrorManagement::NoError);
        mutexAcquired = ok;
    }
    if (ok){
        if(executionMode == UEIMapRealTimeThreadExecutionMode){
            //If the Map is executed in RealTimeThread mode, perform one map refresh as it will not refresh on itself
            //The refresh procedure must not cause a deadlock on the mutex as the execution mode prvents such on the implementation
            // of the ExchangeMap call
            ok &= ExchangeMap(outputCode);
        }
    }
    //If the ExchangeMap call returns ok check that we have enough data in the buffers to acquire the samples needed.
    //In the event of a FIFO Overflow, the buffers are re-set to zero
    //Once the refresh is done and data written into the circularBuffer Structure, check if all the buffers are ready to be read, if not
    //we need to wait for next refresh to fill the ones not ready (next poll operation).
    if (ok){
        //Check if all the buffers are ready
        bool newData = (nInputMembers>0u) ? true : false;
        for (uint8 i = 0; i < nInputMembers && newData; i++){
            //in this loop, if ok = false, then at least one of the buffers is not reaUEIRtVMapdy yet
            newData &= (inputMembersOrdered[i]->reference->inputChannelsBuffer->CheckReadReady());
        }
        //if ok, then all the buffers are ready to deliver the samples needed
        if (newData){
            outputCode = NEW_DATA_AVAILABLE;    //flag meaning that a complete packet is ready, deliver it
            //The destinationMemory (pointer) points to the memory region where the samples must be written
            //It is mandatory that the first MARTe signal is a Timestamp signal with 64 bit precision, therefore,
            //the destination memory (data channels) does indeed start padded a total of nReadSamples*8 bytes from the
            //DataSource memory location.
            //Flag signaling if the timestamp has already been recived from the buffer (typically only the first member
            // should contain a timestamp).
            bool timestampAcquired = false;
            uint32 signalIdx = 0;
            UEIBufferPointer timestampPointer;
            for (uint8 i = 0; i < nInputMembers && ok; i++){
                ReferenceT<UEIDevice> devReference = inputMembersOrdered[i]->reference;
                if (inputMembersOrdered[i]->Inputs.timestampRequired && !timestampAcquired){
                    ok &= devReference->inputChannelsBuffer->ReadTimestamp(timestampPointer);
                    timestampAcquired = true;
                }
                UEIBufferPointer* BufferPointersInMember = devReference->inputChannelsBuffer->ReadBuffer(ok);
                if (ok){
                    //Scale the obtained data
                    for (uint32 j = 0u; j < inputMembersOrdered[i]->Inputs.nChannels; j++){
                        //Extract the configuration for this member data scale.
                        uint32 channel = inputMembersOrdered[i]->Inputs.channels[j];
                        UEIBufferPointer RawPointer = BufferPointersInMember[j];
                        void* scaledDataPointer = reinterpret_cast<void*>(inputSignalAddresses[signalIdx]);
                        TypeDescriptor thisOutputType = inputSignalTypes[signalIdx];
                        ok &= devReference->RetrieveInputSignal(channel, nReadSamples, scaledDataPointer, thisOutputType);
                        if (!ok){
                            REPORT_ERROR(ErrorManagement::CommunicationError, "The scaling process failed in Map %s", name.Buffer());
                        }
                        signalIdx++;
                    }
                    //Check out the current buffer
                    ok &= devReference->inputChannelsBuffer->CheckoutBuffer();
                }
                if (!ok){
                    outputCode = ERROR;
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Could not retrieve data from CircualrBuffer in Map %s", name.Buffer());
                }
            }
            //Correct and set in memory the timestamps
            if (ok){
                ok = (timestampAcquired);
                if (ok){
                    ok = GetTimestamp(timestampPointer, nReadSamples, TimestampAddr);
                    if (!ok){
                        REPORT_ERROR(ErrorManagement::CommunicationError, "Could not correct appropiately the timestamps retrieved in Map %s", name.Buffer());
                    }
                }else{
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Could not retrieve timestamp in Map %s", name.Buffer());
                } 
            }
        }else{
            outputCode = NO_NEW_DATA_AVAILABLE;
        }
    }
    //If the mutex was previously acquired, release it so the IOM can be accessed by another thread
    if ((mutexAcquired)){
        //Release the mutex
        mapMutex.UnLock();
    }
    return ok;
}

bool UEIRtVMap::SetOutputs(MapReturnCode& outputCode){
    bool ok = (outputSignalsConfigured && mapStarted && mapCoherent);
    if (!ok){
        if (!outputSignalsConfigured) REPORT_ERROR(ErrorManagement::CommunicationError, "Map %s does not have any configured output signal", name.Buffer());
        if (!mapStarted) REPORT_ERROR(ErrorManagement::CommunicationError, "Map %s is not started", name.Buffer());
        if (!mapCoherent) REPORT_ERROR(ErrorManagement::CommunicationError, "Map %s is not coherent", name.Buffer());
    }
    bool mutexAcquired = false;
    uint32 signalIdx = 0u;
    //First of all acquire the Map mutex to allow for safe access to the IOM without overwriting any buffer
    if (ok){
        //Acquire the mutex
        ErrorManagement::ErrorType err;
        err = mapMutex.Lock();
        ok &= (err == ErrorManagement::NoError);
        mutexAcquired = ok;
    }
    //When an output is to be set for a VMap, first set the values of the output to that of the output signals
    //The types and pointers to the VMap output signals must be configured in the Map prior to this call
    //Traverse all the output members and channels in order (the same order as output signals)
    for (uint32 mem = 0u; mem < nOutputMembers && ok; mem++){
        ReferenceT<UEIDevice> thisDevice = outputMembersOrdered[mem]->reference;
        ok &= thisDevice.IsValid();
        if (!ok){
            REPORT_ERROR(ErrorManagement::CommunicationError, "Error while getting the reference of output device %s in Map %s", thisDevice->GetName(), name.Buffer());
        }
        uint32 thisChannels = thisDevice->GetHardwareChannels(OutputSignals, outputMembersOrdered[mem]->Outputs.nChannels);
        uint32 bytesToWrite = nSamplesMapExchange*thisDevice->GetSampleSize()*thisChannels;
        //First of all, check if there's space left in the buffer for the output samples we'd like to write into it
        if (ok){
            ok &= thisDevice->outputChannelsBuffer->CheckAvailableSpace(bytesToWrite);
            if (!ok){
                REPORT_ERROR(ErrorManagement::CommunicationError, "Output buffer for device %s in Map %s is full, write operation failed", thisDevice->GetName(), name.Buffer());
            }
        }
        if (ok && (thisDevice->GetType() == HARDWARE_LAYER_DIGITAL_IO)){
            //If everything is fine, then prepare the output buffer to accept the next samples (this is required
            // as some hardware layers do not completely overwrite the existant data in the buffer bu rather modify
            // some bits, by performing this call we do ensure a clean canvas for the device to write into the buffer)
            thisDevice->outputChannelsBuffer->ZeroNextBytes(bytesToWrite);
        }
        for(uint32 chan = 0u; chan < outputMembersOrdered[mem]->Outputs.nChannels && ok; chan++){
            uint32 channelIdx = outputMembersOrdered[mem]->Outputs.channels[chan];
            uint32 nSamples = nWriteSamples;
            void* SignalPointer = reinterpret_cast<void*>(outputSignalAddresses[signalIdx]);
            TypeDescriptor signalType = outputSignalTypes[signalIdx];
            //With this call, the outputBuffers for each device should be set with the required information
            //from each of the output signals
            ok &= thisDevice->SetOutputSignal(channelIdx, nSamples, SignalPointer, signalType);
            if (!ok){
                REPORT_ERROR(ErrorManagement::CommunicationError, "Output signal parsing for device %s in Map %s failed", thisDevice->GetName(), name.Buffer());
            }
            signalIdx++;
        }
        if (ok){
            //Once all the samples are written into the output buffer, close it and mark it as ready to be sent into the
            //IOM
            ok &= thisDevice->outputChannelsBuffer->AdvanceBufferIndex(bytesToWrite);
        }
        if (!ok){
            REPORT_ERROR(ErrorManagement::CommunicationError, "Writting of the output samples int to output buffer failed for device %s in Map %s", thisDevice->GetName(), name.Buffer());
        }
    }
    //If the map is set to run in RealTimeThreadExecutionMode, just perform a MAP exchange with the IOM as the refresh
    //would not be performed otherwise
    if (ok && (executionMode == UEIMapRealTimeThreadExecutionMode)){
        ok &= ExchangeMap(outputCode);
    }
    //If we previously acquired the mutex, now that the read is performed, release the mutex for further operations.
    if ((mutexAcquired)){
        //Release the mutex
        mapMutex.UnLock();
    }
    return ok;
}

MapType UEIRtVMap::GetType(){
    return RTVMAP;
}

bool UEIRtVMap::ResetVMap(){
    bool ok = StopMap();
    //Stop the map (don't call cleanup map, as it would completely close it)
    ok &= EnableMap();
    if (!ok){
        REPORT_ERROR(ErrorManagement::CommunicationError, "Could not re-start Map %s during map reset request", name.Buffer());
    }
    return ok;
}

bool UEIRtVMap::EnableMap(){
    bool ok = true;
    if (ok){
        ok = (DqRtVmapStart(DAQ_handle, mapid) >= 0);
        if (!ok){
            REPORT_ERROR(ErrorManagement::CommunicationError, "Could not start Map %s", name.Buffer());
        }
    }
    //Now set how many samples do we wish to recieve for this devices channels (this needs to be done AFTER map start, otherwise, you guessed it -> segfault)
    if (ok){
        for (uint32 i = 0u; i < nInputMembers && ok; i++){
            ReferenceT<UEIDevice> devReference = inputMembersOrdered[i]->reference;
            ok = (devReference.IsValid());  //This should not be necessary, but it is implemented for precaution
            if (ok){
                uint32 nChannels_;
                ok &= devReference->GetHardwareChannels(InputSignals, nChannels_);
                uint32 byteSize = devReference->GetSampleSize();
                uint32 requestSize = nChannels_*nSamplesMapExchange*byteSize;
                if (inputMembersOrdered[i]->Inputs.timestampRequired){
                    requestSize += sizeof(uint32)*nSamplesMapExchange;   //Add a fictional channel for timestamp
                }
                //Assign the requested number of bytes on this member for later access during Refresh
                inputMembersOrdered[i]->Inputs.requestSize = requestSize;
                int32 act_size;
                //With this method we set the ammount of samples we want to obtain from this member's device
                ok = (DqRtVmapRqInputDataSz(DAQ_handle, mapid, i, requestSize , &act_size, NULL) >= 0);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error setting the VMap size on member %d in Map %s", i, name.Buffer());
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Requested size was %d bytes, but only %d bytes available", requestSize, act_size);
                }
            }else{
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve devReference for device %d on Map %s while setting sample number", i, name.Buffer());
            }
        }
    }
    //for output signals only calcualte the requestPacketSize, size request is performed during refresh
    if (ok){
        for (uint32 i = 0u; i < nOutputMembers && ok; i++){
            ReferenceT<UEIDevice> devReference = outputMembersOrdered[i]->reference;
            ok = (devReference.IsValid());  //This should not be necessary, but it is implemented for precaution
            if (ok){
                uint32 nChannels_;
                ok &= devReference->GetHardwareChannels(OutputSignals, nChannels_);
                uint32 byteSize = devReference->GetSampleSize();
                uint32 requestSize = nChannels_*nSamplesMapExchange*byteSize;
                //Assign the requested number of bytes on this member for later access during Refresh
                outputMembersOrdered[i]->Outputs.requestSize = requestSize;
            }else{
                REPORT_ERROR(ErrorManagement::InitialisationError, "Could not retrieve devReference for device %d on Map %s while setting sample number", i, name.Buffer());
            }
        }
    }
    mapStarted = ok;
    return ok;
}

bool UEIRtVMap::CheckMapCoherency(){
    bool ok = UEIMapContainer::CheckMapCoherency();
    mapCoherent = ok;
    return ok;
}


ErrorManagement::ErrorType UEIRtVMap::IndependentThreadCallback(ExecutionInfo &info) {
    MapReturnCode returnCode;
    //ExchangeMap method can be called right away, mutex and thread safety are managed inside the call
    // no need for a call to the mutex lock prior to this method call.
    ExchangeMap(returnCode);
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    //TODO management of errors on the exchange packet method.
    Sleep::MSec(1);
    return err;
}


CLASS_REGISTER(UEIRtVMap, "1.0")
}
