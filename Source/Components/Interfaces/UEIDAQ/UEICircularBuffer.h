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
#include "UEIBufferPointer.h"

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
     * @param[in] numberOfBuffers Number of individual buffers to be used in this circularbuffer implementation. Each individual buffer
     * can hold the required number of samples to be read into MARTe through a DataSource.
     * @param[in] channels Number of channels in the map associated with this buffer.
     * @param[in] samplesPerMapRequest Maximum number of samples per channel to be retrieved on a map refresh operation.
     * @param[in] sOfSamples Size (in bytes) of the samples to be retrieved in the map refresh request.
     * @param[in] nReadSamples Number of samples per channel to be read by the DataSource during a Synchronise call.
     * @param[in] tStampRequired Flag stating if this circular buffer will contain a channel for timestamp, which must mandatorily be the first
     * channel retrieved from the hardware layer through the DAQ packet.
     * @returns true if the parameters supplied to the method are valid, false otherwise.
     */
    bool InitialiseBuffer(uint32 numberOfBuffers, uint32 channels, uint32 samplesPerMapRequest, uint8 sOfSamples, uint32 nReadSamples, bool tStampRequired);

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
     * @brief Method which checks the availalbe space on the buffer.
     * @details Variation of the CheckAvailableSpace() method which computes if a certain ammount of bytes may be 
     * set into the buffer, therefore if enough space is still available on the buffer
     * @param[in] writtenBytes Number of bytes to be written into the buffer
     * @returns true if the buffer has space for the specified number of bytes, false otherwise.
     */
    bool CheckAvailableSpace(uint32 writtenBytes);
    
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
     * @brief Method which allows the data to be retrieved from the buffer.
     * @details The implementation of this method is done through the usage of UEIBufferPointers, which serve as virtual arrays effectively containing
     * the samples for each of the retrieved channels in this circularBuffer.
     * @param[out] ok Flag stating if the read operation is valid by setting to true, or false otherwise.
     * @returns pointer to an array of UEIBufferPointer objects to access the virtual arrays for each of the channels. The channels are retrieved in the
     * same order as they're retrieved by the UEIDAQ hardware layer packet and therefore the configured order during map enabling.
     */
    UEIBufferPointer* ReadBuffer(bool& ok);

    UEIBufferPointer ReadChannel(uint32 chanelIdx, bool& ok);
    /**
     * @brief Method which allows the timestamp channel data to be retrieved from the buffer.
     * @details The implementation of this method is done through the usage of UEIBufferPointers, which serve as virtual arrays effectively containing
     * the samples for each of the retrieved channels in this circularBuffer.
     * @param[out] ok Flag stating if the read operation is valid by setting to true (and the timestamp channel is configured in this buffer),
     * or false otherwise.
     * @returns Pointer to a UEIBufferPointer containing the access to the timestamp channel retrieved from the hardware layer in this buffer
     * as a virtual array.
     */
    UEIBufferPointer ReadTimestamp(bool& ok);
    
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
    
    /**
     * @brief Method to mark the data on the buffer as read and allow new data to come into the buffer without overflow.
     * @details This method must be called once the latest available batch of data on the buffer is no longer needed. This method
     * allows further data to be written into the buffer and therefore prevent overflow.
     * @returns true if the operation was successful, false otherwise.
     */
    bool CheckoutBuffer();
    bool AdvanceBufferReadOneSample();
    
protected:

    /**
    *   Flag stating if the buffer is properly configured before operation
    */
    bool bufferSet;

    /**
    *   Flag stating if this UEICircularBuffer is provided with a timestamp channel
    */
    bool timestampRequired;
    
    /**
    *   Pointer to the start position of the buffer (HEAD). This pointer is uint8 as the bus width of the
    *   UEIDAQ bus is 32 bit but the width of the samples may change for each hardware layer. 
    */
    uint8* headPointer;
    
    /**
    *   Variable holding the number of individual buffers that this buffer can contain
    */
    uint32 nOfBuffers;
    
    /**
    *   Variable holding the length in bytes of an individual buffer in this circular buffer
    */
    uint32 singleBufferLength;
    
    /**
    *   Variable holding the length in bytes of the zone after the latest individual buffer to store the mismatched samples from the last write.
    */
    uint32 runawayZoneLength;

    /**
    *   Index of the next buffer to be retrieved by a read call.
    */
    uint32 currentActiveBuffer;
    
    /**
    *   Length of the whole buffer in bytes.
    */
    uint32 bufferLength;

    /**
    *   Variable holding the maximum number of samples per channels to be retrieved in a VMap packet request.
    */
    uint32 samplesInMapRequest;

    /**
    *   Variable holding the number of channels defined for the VMap assigned to this circular buffer.
    */
    uint32 nChannels;

    /**
    *   Variable holding the width (in bytes) of the samples for the channels on this circular buffer.
    */
    uint8 sizeOfSamples;
    
    /**
    *   Samples to be read for each channel on DataSource sycnhronize call.
    */
    uint32 readSamples;
    
    /**
    *   List of UEIBufferPointer objects to access the internal channel list of he buffer
    */
    UEIBufferPointer* pointerList;
    
    /**
    *   UEIBufferPointer object to access the timestamp
    */
    UEIBufferPointer timestampList;
    
    /**
    *   Flag indicating if the writePointer has lapped the buffer. This flag is true if the write pointer returned to headPointer
    *   before the readBuffer. TODO explain this better.
    */
    bool writePointerLapped;

    /**
    *   Index pointing the location in which the next write will take place on the buffer.
    */
    uint32 currentBufferLocation;
};
}
#endif /* UEICircularBuffer_H_ */