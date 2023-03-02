/**
 * @file UEICircularBuffer.h
 * @brief Header file for class UEICircularBuffer
 * @date 08/02/2023
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

 * @details This header file contains the declaration of the class UEICircularBuffer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEICircularBuffer_H_
#define UEICircularBuffer_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class UEICircularBuffer : public Object {
    public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details NOOP.
     */
    UEICircularBuffer();

    /**
     * @brief Default destructor.
     * @details NOOP.
     */
    virtual ~UEICircularBuffer();

    bool InitialiseBuffer(uint32 maxSamplesStored, uint32 channels_, uint32 VMapSamplesPerChannel_, uint8 sizeOfSamples_, uint32 readSamples_);

    uint8* writePointer;

    bool CheckAvailableSpace();
    bool CheckReadReady();
    bool AdvanceBufferIndex(uint32 writtenBytes);
    bool ReadBuffer(uint8* destinationMemory);

private:
    bool CopyChannels(uint8* sourceMemory, uint32* timestampMemory, uint8* destinationMemory);
    /*
    *   Variable depicting if this UEICircularBuffer is provided with a timestamp channel to perform data synchronisation
    *   among UEICircularBuffers
    */
    bool timestampProvided;
    
    /*
    *   Variable depicting which is the channel index containing the timestamp channel to perform synchronisation
    *   Only one timestamp channel is allowed per buffer.
    */
    uint8 timestampChannel;
    /*
    *   Pointer to the start position of the buffer (HEAD). This pointer is uint32 as the bus width of the
    *   UEIDAQ bus is 32 bit. 
    */
    uint8* headPointer;
    
    /*
    *   Length of the buffer in bytes.
    */
    uint32 bufferLength;

    /*
    *   Index of the first position available for the buffer to write safely
    *
    */
    uint32 currentBufferLocation;

    uint32 samplesPerChannelVMap;
    uint32 channels;
    uint8 sizeOfSamples;
    
    /*
    *   Samples to be read for each channel on DataSource sycnhronize
    */
    uint32 readSamples;

    uint32* packetMemberSize;
    uint32 packetByteSize;
    uint32 numberOfPacketMembers;
    uint32 numberOfSamples;
};
}
#endif /* UEICircularBuffer_H_ */