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
    headPointer = NULL_PTR(uint8*);
    writePointer = NULL_PTR(uint8*);
    readPointer = NULL_PTR(uint8*);
    timestampRequired = false;
    nOfBuffers = 0;
    singleBufferLength = 0;
    runawayZoneLength = 0;
    bufferLength = 0;
    fullBufferLength = 0;
    nChannels = 0;
    sizeOfSamples = 0;
    samplesPerExternalWrite = 0;
    samplesPerExternalRead = 0;
    readPointerList = NULL_PTR(UEIBufferPointer*);
    writePointerList = NULL_PTR(UEIBufferPointer*);
}

UEICircularBuffer::~UEICircularBuffer(){
    //Just in case
    writePointer = NULL_PTR(uint8*);

    if (headPointer != (NULL_PTR(uint8*))){
        free(headPointer);
    }
    if (readPointerList != NULL_PTR(UEIBufferPointer*)){
        delete [] readPointerList;
    }
}


bool UEICircularBuffer::InitialiseBuffer(uint32 numberOfBuffers, uint32 channels, uint32 SamplesPerExternalWrite, uint8 SizeOfSamples, uint32 SamplesPerExternalRead, bool tStampRequired){
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
        ok = (SamplesPerExternalWrite > 0u);
        samplesPerExternalWrite = SamplesPerExternalWrite;
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "At least one sample per channel must be supplied by the Map packet");
        }
    }
    //Check the desired number of samples to be retrieved by MARTe DataSource from this buffer, defines de size of the buffer (at least one sample
    //must be retrieved)
    if (ok){
        ok = (SamplesPerExternalRead > 0u);
        samplesPerExternalRead = SamplesPerExternalRead;
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid number of samples to be read from this buffer");
        }
    }
    //Check the size of the samples to be logged into the buffer. A UEICircularBuffer is intended to save data from a single hardware layer, therefore
    // only having a single datatype (uint32, uint16 or uint8) for all its channels. The timestamp channel length is hardcoded into the buffer.
    if (ok){
        ok = (SizeOfSamples > 0 && SizeOfSamples <= sizeof(uint32));  //UEIDAQ only supports uint32 or below sample size
        sizeOfSamples = SizeOfSamples;
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
        singleBufferLength = (sizeOfSamples*nChannels+sizeof(uint32)*timestampRequired)*samplesPerExternalRead;
        //The runaway zone is needed to write partial data into the the buffer when the last single buffer is full, data is written
        // into this runaway zone and later on copied into the first buffer of the list if empty
        runawayZoneLength = (sizeOfSamples*nChannels+sizeof(uint32)*timestampRequired)*samplesPerExternalWrite;
        //The total length of the buffer is determined
        bufferLength = singleBufferLength*numberOfBuffers; 
        fullBufferLength = singleBufferLength*numberOfBuffers + runawayZoneLength; 
        headPointer = (uint8*)malloc(fullBufferLength);
        ok = (headPointer != NULL_PTR(uint8*));
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to initialise memory for UEICircularBuffer");
        }
    }
    //Place correctly the writePointer for initial configuration
    if (ok){
        writePointer = (uint8*)headPointer;
        readPointer = (uint8*)headPointer;
    }
    if (ok){
        //Allocate timestamp UEIBufferPointer object
        // As the timestamp is always the first channel retrieved the values are fixed, offset 0 bytes, gain and number of samples
        timestampList.SetPointerCharacteristics(0u, (nChannels*sizeOfSamples) + sizeof(uint32), samplesPerExternalRead);
        //Try to allocate memory for the array of pointers
        readPointerList  = new UEIBufferPointer[nChannels]; 
        writePointerList = new UEIBufferPointer[nChannels]; 
        ok &= (readPointerList != NULL_PTR(UEIBufferPointer*));
        ok &= (writePointerList != NULL_PTR(UEIBufferPointer*));
        //Assign the different UEIBufferPointer objects accordingly
        for (uint32 i = 0; i < nChannels && ok; i++){
            uint32 offset = i*sizeOfSamples + sizeof(uint32)*timestampRequired;
            uint32 pointerGain = nChannels*sizeOfSamples + sizeof(uint32)*timestampRequired;
            uint32 maxLength = samplesPerExternalRead;
            ok &= readPointerList[i].SetPointerCharacteristics(offset, pointerGain, maxLength);
            ok &= writePointerList[i].SetPointerCharacteristics(offset, pointerGain, maxLength);
        }
    }
    return ok;
}

//Check if there's available space on the buffer for a given ammount of bytes written
bool UEICircularBuffer::CheckAvailableSpace(uint32 writtenBytes){
    bool spaceAvailable = false;
    //Check if there's space in te buffer for a new batch of samples from VMap
    if (bufferLength > 0){
        if (writePointer < readPointer){
            //This means the writting pointer is behind, only allow the write if there's enough space for the whole MVap reading
            spaceAvailable = ((uint32)(readPointer-writePointer) >= (writtenBytes+1));
        }else{
            uint32 bytesAvailable = ((headPointer+bufferLength)-writePointer) + (readPointer - headPointer);
            spaceAvailable = (bytesAvailable >= (writtenBytes+1));
            //Check also that the runawayZone can acomodate the write
            spaceAvailable &= ((uint32)((headPointer+bufferLength+runawayZoneLength)-writePointer) >= writtenBytes);
        }
    }
    return spaceAvailable;
}

//Check if there's enough space for an additional VMap write
bool UEICircularBuffer::CheckAvailableSpace(){
    return CheckAvailableSpace(runawayZoneLength);
}

//Check if there's available space on the buffer for a given ammount of bytes written
bool UEICircularBuffer::CheckReadReady(uint32 readBytes){
    bool ReadReady = false;
    //Check if there's space in the buffer for a new batch of samples from VMap
    if (singleBufferLength > 0){
        uint32 readSpaceAvailable = GetAvailableBytesToRead();
        ReadReady = (readSpaceAvailable >= readBytes);
    }
    return ReadReady;
}

//Check if there's enough space for a single buffer read
bool UEICircularBuffer::CheckReadReady(){
    return CheckReadReady(singleBufferLength);
}

//Advance the index on the buffer corresponding with the last written bytes into the buffer
bool UEICircularBuffer::AdvanceBufferIndex(uint32 writtenBytes){
    //Function to advance the current index in the buffer
    bool ok = CheckAvailableSpace(writtenBytes);
    if (ok){
        if(writePointer+writtenBytes >= headPointer+bufferLength){
            uint32 overflownBytes = (writePointer+writtenBytes)-(headPointer+bufferLength);
            if (overflownBytes != 0u){
                memcpy(headPointer, headPointer+bufferLength, overflownBytes);
            }
            writePointer = headPointer + overflownBytes;
        }else{
            writePointer += writtenBytes;
        }
    }
    return ok;
}

UEIBufferPointer* UEICircularBuffer::ReadBuffer(bool& ok){
    ok = CheckReadReady();
    if (ok){
        for (uint32 i = 0; i < nChannels; i++){
            readPointerList[i].SetHead(readPointer);
        }
    }
    return readPointerList;
}

UEIBufferPointer* UEICircularBuffer::GetBufferWritePointers(bool& ok){
    ok = CheckAvailableSpace(singleBufferLength);
    if (ok){
        for (uint32 i = 0; i < nChannels; i++){
            writePointerList[i].SetHead(writePointer);
        }
    }
    return writePointerList;
}

bool UEICircularBuffer::ReadChannel(uint32 chanelIdx, UEIBufferPointer& pointer){
    bool ok = CheckReadReady();
    ok &= (chanelIdx < nChannels);
    if (ok){
        readPointerList[chanelIdx].SetHead(readPointer);
        pointer=readPointerList[chanelIdx];
    }
    return ok;
}

bool UEICircularBuffer::GetWritePointer(uint32 chanelIdx, UEIBufferPointer& pointer){
    bool ok = CheckAvailableSpace(singleBufferLength);
    ok &= (chanelIdx < nChannels);
    if (ok){
        writePointerList[chanelIdx].SetHead(writePointer);
        pointer=writePointerList[chanelIdx];
    }
    return ok;
}


bool UEICircularBuffer::ReadTimestamp(UEIBufferPointer& pointer){
    bool ok = CheckReadReady() && timestampRequired;
    if (ok){
        timestampList.SetHead(readPointer);
        pointer = timestampList;
    }else{
        REPORT_ERROR(ErrorManagement::CommunicationError, "UEICircularBuffer cannot retrieve timestamp channel, as the subbuffer is not ready yet");
    }
    return ok;
}

bool UEICircularBuffer::CheckoutBuffer(){
    //Move the read pointer to the next buffer in the circularbuffer
    //To do so, we must ensure that the actual pointing buffer is ready to be read, otherwise we cannot advance to next buffer
    if (CheckReadReady()){
        readPointer += singleBufferLength;
        if (readPointer >= (headPointer + bufferLength)){
            //If we've reached the last buffer, then just go back to the first one.
            readPointer = headPointer;
        }
        return true;
    }else{
        return false;
    }
}

bool UEICircularBuffer::ResetBuffer(){
    writePointer = (uint8*)headPointer;
    readPointer = (uint8*) headPointer;
    return true;
}

bool UEICircularBuffer::AdvanceBufferReadOneSample(){
    return AdvanceBufferIndex(sizeOfSamples*nChannels+sizeof(uint32)*timestampRequired);
}

bool UEICircularBuffer::ReadBytes(uint32 readBytes, uint8* destination, bool advancePointer){
    bool ok = CheckReadReady(readBytes);
    if (ok){
        if ((uint32)((headPointer+bufferLength)-readPointer) > readBytes){
            memcpy(destination, readPointer, readBytes);
        }else{
            uint32 overflownBytes = readBytes-((headPointer+bufferLength)-readPointer);
            memcpy(destination, readPointer, readBytes-overflownBytes);
            if (overflownBytes != 0u){
                memcpy(destination+(readBytes-overflownBytes), headPointer, overflownBytes);
            }
        }
        if (advancePointer){
            ok &= AdvanceReadPointer(readBytes);
        }
    }
    return ok;
}

bool UEICircularBuffer::AdvanceReadPointer(uint32 readBytes){
    bool ok = CheckReadReady(readBytes);
    if (ok){
        if ((uint32)((headPointer+bufferLength)-readPointer) > readBytes){
            readPointer += readBytes;
        }else{
            uint32 overflownBytes = readBytes-((headPointer+bufferLength)-readPointer);
            readPointer = headPointer + overflownBytes;
        }
    }
    return ok;
}

uint32 UEICircularBuffer::GetAvailableBytesToRead(){
    uint32 readSpaceAvailable = 0u;
    if (singleBufferLength > 0){
        if (writePointer < readPointer){
            readSpaceAvailable = ((headPointer+bufferLength)-readPointer)+(writePointer-headPointer);
        }else{
            readSpaceAvailable = (writePointer-readPointer); 
        }
    }
    return readSpaceAvailable;
}

bool UEICircularBuffer::ZeroNextBytes(uint32 bytesToWrite){
    bool ok = CheckAvailableSpace(bytesToWrite);
    if (ok){
        //By default set the bytes to be written to 0;
        ok &= MemoryOperationsHelper::Set(reinterpret_cast<void*>(writePointer), 0x00, bytesToWrite);
    }
    return ok;
}

CLASS_REGISTER(UEICircularBuffer, "1.0")
}
