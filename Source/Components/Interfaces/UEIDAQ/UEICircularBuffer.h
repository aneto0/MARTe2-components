/**
 * @file UEICircularBuffer.h
 * @brief Header file for class UEICircularBuffer
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

/**
 * @brief Class defining a specific circular buffer implementation to be used for UEIDAQ hardware layers sample retrieving.
 * @details This specific implementation of a circular buffer is tailored to the needs and specificities of UEIDAQ hardware layer
 * sample retrieving format.
 * 
 * Upon a refreshment of a buffered map packet, the samples retrieved may not match the number of samples needed by the UEIDataSource to be delivered
 * in MARTe, therefore the retrieved samples must be stored in a buffer until the desired number of samples is retrieved and can be read by the DataSource.
 *
 * Additionally, when a UEIDAQ buffered map is retrieved from hardware layers it may have different number of retrieved samples for each hardware layer
 * even if the same sampling frequency is configured for such layers. Therefore this buffer implementation presents a method to check if the required number
 * of samples is achieved for this specific buffer so it can be externally checked if all the buffers are ready to read at a moment.
 * 
 * This buffer implementation is intended to be used for a single hardware layer on a UEIDAQ, therefore each hardware layer must have a circular buffer associated
 * when working in buffered map mode. Upon reception of new samples for a hardware layer they are stored in the associated circular buffer and allt eh buffers
 * checked for readiness to supply samples to DataSource.
 *
 * No initialisation directly via configuration file is foreseen for this class.
 *
 */
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

    /**
     * @brief Initialisation method for the buffer object.
     * @details This method must be called to initialise the buffer with adequate variables prior to usage of the buffer.
     * It also performs checks on the supplied parameters to ensure they're valid.
     * @param[in] maxSamplesStored maximum number of samples per channel to be stored in the buffer (this number must be
     * greater than the number of samples per channel retrieved on a Map refresh).
     * @param[in] channels_ number of channels in the map associated with this buffer.
     * @param[in] VMapSamplesPerChannel_ maximum number of samples per channel to be retrieved on a map refresh operation.
     * @param[in] sizeOfSamples_ size (in bytes) of the samples to be retrieved in the map refresh request.
     * @param[in] readSamples_ number of samples per channel to be read by the DataSource during a Synchronise call.
     * @returns true if the parameters supplied to the method are valid, false otherwise.
     */
    bool InitialiseBuffer(uint32 maxSamplesStored, uint32 channels_, uint32 VMapSamplesPerChannel_, uint8 sizeOfSamples_, uint32 readSamples_);

    /**
     * @brief Method which checks the availalbe space on the buffer.
     * @details This method checks if there is space available in the buffer for a new packet coming from the IOM for this map member.
     * The minimum space needed on the buffer to write new data is the maximum size of the IOM packet (number of channels * number of samples per channel * 
     * bytes per sample).
     *
     * This method must be called prior to a write on writePointer to check if enough space is available. If no space is available the developer must not write
     * new data into the buffer, otherwise behavior of the buffer is not granted.
     * @returns true if the buffer has space for an additional packet write, false otherwise.
     */
    bool CheckAvailableSpace();
   
    /**
     * @brief Method which checks if the buffer contains the needed ammount of samples to be delivered into the DataSource.
     * @details This method checks if the amount of samples stored in the buffer is enough to deliver a full packet to the DataSource.
     * The amount of samples needed by the DataSource is the one configured in InitialiseBuffer method.
     *
     * @returns true if the buffer has enough samples to deliver to the DataSource, false otherwise.
     */
    bool CheckReadReady();
    
    /**
     * @brief Method which updates the buffer indexes and pointers after a data write into the buffer with data from IOM.
     * @details This method must be called after each write to writePointer location providing the amount of bytes written in the buffer. The method
     * internally updates the indexes and pointers of the buffer to allow correct operation of the buffer.
     * @param[in] writtenBytes number of bytes written into the buffer on the last write operation to writePointer.
     * @returns true if the buffer is updated correctly, false otherwise.
     */
    bool AdvanceBufferIndex(uint32 writtenBytes);

    /**
     * @brief Method which allows the data to be retrieved from the buffer into a destination memory area (provided by the DataSource).
     * @details This method performs the data extraction from the circular buffer to a destination memory area. This method performs the conversion
     * from interleaved memory arrangement retrieved from IOM packet into the flat memory arrangement desired by MARTe and the DataSource.
     *
     * If the map contained a tiemstamp channel, then the method also returns a separated array containing the samples of the timestamp channel for
     * flexibility. Note that by internal convention if a Map contains a timestamp channel, this is always the first channel retrieved in the map.
     *
     * This method performs no scaling of the data or any other operation other than interleaved to flat order conversion
     *
     * @param[out] destinationMemory pointer to the memory area where the samples are to be copied in flat memory order.
     * @param[out] timestampArray pointer to the memory area where the timestamp channel (if present) samples are to be copied in flat memory order.
     * @param[in] timestampInMap flag signaling if a timestamp channel is expected for data in this buffer.
     * @returns true if read operation succeeded, false otherwise.
     */
    bool ReadBuffer(uint8* destinationMemory, uint32* timestampArray, bool timestampInMap);
    /**
     * @brief Method to reset the buffer status.
     * @details This method allows the circular buffer to be reset to initial condition (without any data overwritting). The purpose of this method
     * is to allow a fresh buffer to be used after a map restart event, in which data store in the circular buffer is no longer valid and differences
     * among different buffers filling may lead to errors.
     *
     * This method does not overwrite any data on the buffer, just sets the buffer to initial state so data already written is overwritten during 
     * write operations in the buffer.
     * @returns always true.
     */
    bool ResetBuffer();

    /*
    *   Public pointer allowing write access into the buffer. When new data is recieved from IOM it must be written into this location on the buffer.
    *   Before writting into such location the developer is responsible to check if enough space is available in the buffer by calling the CheckAvailableSpace method
    *   After writting into the buffer the developer is responisble to advance the pointer by calling the AdvanceBufferIndex method. The developer must not
    *   perform any actions other than writting on this pointer.
    */
    uint8* writePointer;
private:

    /**
     * @brief Private method to perform interleaved to flat memory copy operation.
     * @details This method is used internally by ReadBuffer method to copy the memory from interleaved to flat order. It also allows timestamp
     * channel extraction from the packet structure into a separate timestamp array.
     * @param[in] sourceMemory pointer to the source memory to perform the copy operation.
     * @param[out] timestampMemory pointer to destination memory for the timestamp channel samples array.
     * @param[out] destinationMemory pointer to the destination memory for the data channels samples.
     * @param[in] timestampPresent falg indicating if a timestamp channel is to be retrieved from the buffer memory.
     * @returns true if operation was successfull.
     */
    bool CopyChannels(uint8* sourceMemory, uint32* timestampMemory, uint8* destinationMemory, bool timestampPresent);

    /*
    *   Flag stating if this UEICircularBuffer is provided with a timestamp channel
    */
    bool timestampProvided;
    
    /*
    *   Pointer to the start position of the buffer (HEAD). This pointer is uint8 as the bus width of the
    *   UEIDAQ bus is 32 bit but the width of the samples may change for each hardware layer. 
    */
    uint8* headPointer;
    
    /*
    *   Length of the buffer in bytes.
    */
    uint32 bufferLength;

    /*
    *   Index of the first position available for the buffer to write safely.
    */
    uint32 currentBufferLocation;

    /*
    *   Variable holding the maximum number of samples per channels to be retrieved in a VMap packet request.
    */
    uint32 samplesPerChannelVMap;

    /*
    *   Variable holding the number of channels defined for the VMap assigned to this circular buffer.
    */
    uint32 channels;

    /*
    *   Variable holding the width (in bytes) of the samples for the channels on this circular buffer.
    */
    uint8 sizeOfSamples;
    
    /*
    *   Samples to be read for each channel on DataSource sycnhronize call.
    */
    uint32 readSamples;
};
}
#endif /* UEICircularBuffer_H_ */