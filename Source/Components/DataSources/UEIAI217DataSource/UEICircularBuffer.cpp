/**
 * @file UEICircularBuffer.cpp
 * @brief Source file for class UEICircularBuffer
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
#include <iostream>


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
    bufferLength = 0u;
    currentBufferLocation = 0;
}

UEICircularBuffer::~UEICircularBuffer(){
    if (headPointer != (NULL_PTR(uint8*))){
        free(headPointer);
    }
}


bool UEICircularBuffer::InitialiseBuffer(uint32 maxSamplesStored, uint32 channels_, uint32 VMapSamplesPerChannel_, uint8 sizeOfSamples_, uint32 readSamples_){
    bool ok = true;
    if (ok){
        ok = (channels_ > 1u);      //At least a channel + timestamp must be provided
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "At least two channels must be supplied for a UEICircularBuffer (data channel + timestamp)");
        }else{
            channels = channels_;
        }
    }
    if (ok){
        ok = (VMapSamplesPerChannel_ > 0u);
        samplesPerChannelVMap = VMapSamplesPerChannel_;
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "At least one sample per channel must be supplied for the ");
        }
    }
    if (ok){
        ok = (readSamples_ > 0u && readSamples_ < maxSamplesStored);
        readSamples = readSamples_;
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid number of samples to be read into destination buffer");
        }
    }
    if (ok){
        ok = (sizeOfSamples_ > 0 && sizeOfSamples_ <= sizeof(uint32));
        sizeOfSamples = sizeOfSamples_;
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Unsupported data type to write into the buffer");
        }
    }
    if(ok){
        ok = (maxSamplesStored > 0);
        bufferLength = maxSamplesStored*sizeOfSamples*channels;  
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "CircularBuffer size is too big.");
        }
    }
    if (ok){
        headPointer = (uint8*)malloc(bufferLength);
        ok = (headPointer != NULL_PTR(uint8*));
        if (!ok){
            REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to initialise memory for UEICircularBuffer");
        }
    }
    //Place correctly the writePointer
    if (ok){
        writePointer = (uint8*)headPointer;
    }
    return ok;
}

//Check if there's available space on the buffer for a new read from the UEIDAQ VMap packet
bool UEICircularBuffer::CheckAvailableSpace(){
    bool spaceAvailable = true;
    //Check if there's space in te buffer for a new batch of samples from VMap
    if ((bufferLength-currentBufferLocation) < channels*samplesPerChannelVMap*sizeOfSamples){
        spaceAvailable = false;
    }
    return spaceAvailable;
}

//Check if there's enough samples in the buffer to be read by the DataSource
bool UEICircularBuffer::CheckReadReady(){
    bool ReadReady = true;
    //Check if there's space in te buffer for a new batch of samples from VMap
    if (currentBufferLocation < channels*readSamples*sizeOfSamples){
        ReadReady = false;
    }
    return ReadReady;
}

//Advance the index on the buffer corresponding with the last written bytes into the buffer
bool UEICircularBuffer::AdvanceBufferIndex(uint32 writtenBytes){
    //Function to advance the current index in the buffer
    bool ok = true;
    if ((bufferLength-currentBufferLocation) >= writtenBytes){
        ok = true;
        currentBufferLocation += writtenBytes;
        writePointer = (uint8*)(&headPointer[currentBufferLocation]);
    }else{
        ok = false;
        REPORT_ERROR(ErrorManagement::CommunicationError, "Invalid index advancement in the UEICircularBuffer");
    }
    return ok;
}

bool UEICircularBuffer::ReadBuffer(uint8* destinationMemory, uint32* timestampArray, bool timestampInMap){
    //By definition in this Interface, the timestamp channel is always the first in the map.
    bool ok = CheckReadReady();
    if (ok){
        ok = CopyChannels(headPointer, timestampArray, destinationMemory, timestampInMap && (timestampArray != NULL));
        if (ok){        
            //Prepare the memory to leave space for the next packet/s
            uint32 lastReadIndex = channels*readSamples*sizeOfSamples;
            uint32 lastWrittenIndex = bufferLength-currentBufferLocation;
            //Then move the content of the buffer upwards to the start of the buffer
            memcpy(headPointer, &headPointer[lastReadIndex], currentBufferLocation-lastReadIndex);
            //Update the write index and write pointer
            currentBufferLocation -= lastReadIndex;
            writePointer = (uint8*)(&headPointer[currentBufferLocation]);
        }else{
        REPORT_ERROR(ErrorManagement::CommunicationError, "Channel copy into destination failed");	
        }	
    }
    return ok;
}

//This function copies interleaved channel memory into flat memory for input signal output
//it also extracts the timestamp channel value to be used for synchronisation purposes or 
//as separate input signal (Timestamp channel is always the first channel received)
bool UEICircularBuffer::CopyChannels(uint8* sourceMemory, uint32* timestampMemory, uint8* destinationMemory, bool timestampPresent){
    //Timestamp channel is always the first channel in a member input map
	uint8 offset = timestampPresent ? 1u:0u;
    for (uint32 j = 0; j < readSamples; j++){
        if (timestampPresent){
            timestampMemory[j] = reinterpret_cast<uint32*>(sourceMemory)[(j*channels)];
        }
		for (uint32 i = offset; i < channels; i++){	
			reinterpret_cast<uint32*>(destinationMemory)[((i-offset)*readSamples+j)] = reinterpret_cast<uint32*>(sourceMemory)[(j*channels+i)];				
		}	
	}        
    return true;
}

CLASS_REGISTER(UEICircularBuffer, "1.0")
}
