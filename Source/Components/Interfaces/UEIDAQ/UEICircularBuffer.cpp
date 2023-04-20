/**
 * @file UEICircularBuffer.cpp
 * @brief Source file for class UEICircularBuffer
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
 * the class UEICircularBuffer (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEICircularBuffer.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UEICircularBuffer::UEICircularBuffer() : Object() {
    writePointer = NULL_PTR(uint8*);
    timestampRequired = false;
    headPointer = NULL_PTR(uint8*);
    nOfBuffers = 0;
    singleBufferLength = 0;
    runawayZoneLength = 0;
    currentActiveBuffer = 0;
    bufferLength = 0;
    samplesInMapRequest = 0;
    nChannels = 0;
    sizeOfSamples = 0;
    readSamples = 0;
    pointerList = NULL_PTR(UEIBufferPointer*);
    bufferSet = false;
    writePointerLapped = false;
    currentBufferLocation = 0;
}

UEICircularBuffer::~UEICircularBuffer(){
    if (headPointer != (NULL_PTR(uint8*))){
        free(headPointer);
    }
    if (pointerList != NULL_PTR(UEIBufferPointer*)){
        delete [] pointerList;
    }
}


bool UEICircularBuffer::InitialiseBuffer(uint32 numberOfBuffers, uint32 channels, uint32 samplesPerMapRequest, uint8 sOfSamples, uint32 nReadSamples, bool tStampRequired){
    bool ok = true;
    //Check the number of channels the buffer must hold (at least 1 channel)
    if (ok){
        ok = (channels > 0);
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "At least one channel must be supplied for a UEICircularBuffer");
        }else{
            nChannels = channels;
        }
    }
    //Check if the timestamp channel is needed in this buffer (if so, it must be supplied as the first channel retrieved from the Map)
    if (ok){
        timestampRequired = tStampRequired;
    }
    //Check the maximum number of samples expected from a map refresh call (set by the user in the UEIMap configuration)
    if (ok){
        ok = (samplesPerMapRequest > 0u);
        samplesInMapRequest = samplesPerMapRequest;
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "At least one sample per channel must be supplied by the Map packet");
        }
    }
    //Check the desired number of samples to be retrieved by MARTe DataSource from this buffer, defines de size of the buffer (at least one sample
    //must be retrieved)
    if (ok){
        ok = (nReadSamples > 0u);
        readSamples = nReadSamples;
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid number of samples to be read from this buffer");
        }
    }
    //Check the size of the samples to be logged into the buffer. A UEICircularBuffer is intended to save data from a single hardware layer, therefore
    // only having a single datatype (uint32, uint16 or uint8) for all its channels. The timestamp channel length is hardcoded into the buffer.
    if (ok){
        ok = (sOfSamples > 0 && sOfSamples <= sizeof(uint32));  //UEIDAQ only supports uint32 or below sample size
        sizeOfSamples = sOfSamples;
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "UEICircularBuffer only supports 4-byte or less channel samples");
        }
    }
    //Check the desired number of buffers for the Circular buffer, at least 2 buffers must be specified, otherwise it does not behave as a circular buffer
    if(ok){
        ok = (numberOfBuffers > 1);
        nOfBuffers = numberOfBuffers;       
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "CircularBuffer must contain at least two buffers");
        }
    }
    //Calculate the size of the buffer and allocate them
    if (ok){
        //A single buffer must contain all the channels plus a timestamp channel (fixed uint32 channel) if needed with the ammount 
        //of samples requested in a datasource read
        singleBufferLength = (sizeOfSamples*nChannels+sizeof(uint32)*timestampRequired)*readSamples;
        //The runaway zone is needed to write partial data into the the buffer when the last single buffer is full, data is written
        // into this runaway zone and later on copied into the first buffer of the list if empty
        runawayZoneLength = (sizeOfSamples*nChannels+sizeof(uint32)*timestampRequired)*samplesInMapRequest;
        //The total length of the buffer is determined
        bufferLength = singleBufferLength*numberOfBuffers + runawayZoneLength; 
        headPointer = (uint8*)malloc(bufferLength);
        ok = (headPointer != NULL_PTR(uint8*));
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to initialise memory for UEICircularBuffer");
        }
        currentActiveBuffer = 0u;
    }
    //Place correctly the writePointer for initial configuration
    if (ok){
        writePointer = (uint8*)headPointer;
    }
    if (ok){
        //Allocate timestamp UEIBufferPointer object
        // As the timestamp is always the first channel retrieved the values are fixed, offset 0 bytes, gain and number of samples
        timestampList.SetPointerCharacteristics(0u, (nChannels*sizeOfSamples) + sizeof(uint32), readSamples);
        //Try to allocate memory for the array of pointers
        pointerList = new UEIBufferPointer[nChannels]; 
        ok &= (pointerList != NULL_PTR(UEIBufferPointer*));
        //Assign the different UEIBufferPointer objects accordingly
        for (uint32 i = 0; i < nChannels && ok; i++){
            uint32 offset = i*sizeOfSamples + sizeof(uint32)*timestampRequired;
            uint32 pointerGain = nChannels*sizeOfSamples + sizeof(uint32)*timestampRequired;
            uint32 maxLength = readSamples;
            ok &= pointerList[i].SetPointerCharacteristics(offset, pointerGain, maxLength);
        }
    }
    bufferSet = ok;
    return ok;
}

//Check if there's available space on the buffer for a new read from the UEIDAQ VMap packet
bool UEICircularBuffer::CheckAvailableSpace(){
    bool spaceAvailable = false;
    //Check if there's space in te buffer for a new batch of samples from VMap
    if (bufferLength > 0){
        if ((bufferLength-currentBufferLocation) > runawayZoneLength){
            spaceAvailable = true;
        }
    }
    return spaceAvailable;

}

//Check if there's available space on the buffer for a given ammount of bytes written
bool UEICircularBuffer::CheckAvailableSpace(uint32 writtenBytes){
    bool spaceAvailable = false;
    //Check if there's space in te buffer for a new batch of samples from VMap
    if (bufferLength > 0){
        if ((bufferLength-currentBufferLocation-writtenBytes) >= runawayZoneLength){
            spaceAvailable = true;
        }
    }
    return spaceAvailable;
}

//Check if there's enough samples in the buffer to be read by the DataSource
bool UEICircularBuffer::CheckReadReady(){
    bool ReadReady = false;
    //Check if there's space in the buffer for a new batch of samples from VMap
    if (singleBufferLength > 0){
        if (currentBufferLocation >= singleBufferLength){
            ReadReady = true;
        }
    }
    return ReadReady;
}

//Advance the index on the buffer corresponding with the last written bytes into the buffer
bool UEICircularBuffer::AdvanceBufferIndex(uint32 writtenBytes){
    //Function to advance the current index in the buffer
    bool ok = false;
    if (bufferLength > 0){
        if (CheckAvailableSpace(writtenBytes)){
            ok = true;
            currentBufferLocation += writtenBytes;
            writePointer = (uint8*)(&headPointer[currentBufferLocation]);
        }else{
            ok = false;
            REPORT_ERROR(ErrorManagement::CommunicationError, "Invalid index advancement in the UEICircularBuffer");
        }
    }
    return ok;
}

UEIBufferPointer* UEICircularBuffer::ReadBuffer(bool& ok){
    ok = CheckReadReady();
    if (ok){
        for (uint32 i = 0; i < nChannels; i++){
            pointerList[i].SetHead(headPointer);
        }
    }
    return pointerList;
}

UEIBufferPointer UEICircularBuffer::ReadChannel(uint32 chanelIdx, bool& ok){
    ok = CheckReadReady();
    if (ok){
        for (uint32 i = 0; i < nChannels; i++){
            pointerList[i].SetHead(headPointer);
        }
    }
    return pointerList[chanelIdx];
}

UEIBufferPointer UEICircularBuffer::ReadTimestamp(bool& ok){
    ok = CheckReadReady() && timestampRequired;
    if (ok){
        timestampList.SetHead(headPointer+singleBufferLength*currentActiveBuffer);
        return timestampList;
    }else{
        REPORT_ERROR(ErrorManagement::CommunicationError, "UEICircularBuffer cannot retrieve timestamp channel, as the subbuffer is not ready yet");
        return timestampList;
    }
}

bool UEICircularBuffer::CheckoutBuffer(){
    //Move the content of the buffer upwards to the start of the buffer
    if (CheckReadReady()){
        memcpy(headPointer, &headPointer[singleBufferLength], currentBufferLocation-singleBufferLength);
        //Update the write index and write pointer
        currentBufferLocation -= singleBufferLength;
        writePointer = (uint8*)(&headPointer[currentBufferLocation]);
        return true;
    }else{
        return false;
    }
}

bool UEICircularBuffer::ResetBuffer(){
    currentBufferLocation = 0;
    writePointer = (uint8*)headPointer;
    return true;
}

bool UEICircularBuffer::AdvanceBufferReadOneSample(){
    return AdvanceBufferIndex(sizeOfSamples*nChannels+sizeof(uint32)*timestampRequired);
}

CLASS_REGISTER(UEICircularBuffer, "1.0")
}
