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
#include "MemoryOperationsHelper.h"

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
     * @param[in] SamplesPerExternalWrite Maximum number of samples to be written into the buffer in a single operation.
     * @param[in] sOfSamples Size (in bytes) of the samples to be retrieved in the map refresh request.
     * @param[in] SamplesPerExternalRead Maximum number of samples to be read from the buffer in a single operation.
     * @param[in] tStampRequired Flag stating if this circular buffer will contain a channel for timestamp, which must mandatorily be the first
     * channel retrieved from the hardware layer through the DAQ packet.
     * @returns true if the parameters supplied to the method are valid, false otherwise.
     */
    bool InitialiseBuffer(uint32 numberOfBuffers, uint32 channels, uint32 SamplesPerExternalWrite, uint8 SizeOfSamples, uint32 SamplesPerExternalRead, bool tStampRequired);

    /**
     * @brief Method which checks the availalbe space on the buffer.
     * @details This method checks if there is space available in the buffer for a full write operation (SamplesPerExternalWrite samples per channel).
     *
     * This method must be called prior to a write on writePointer to check if enough space is available. If no space is available the developer must not write
     * new data into the buffer, otherwise behavior of the buffer is not granted.
     * @returns true if the buffer has space for an additional packet write, false otherwise.
     */
    bool CheckAvailableSpace();

    /**
     * @brief Method which checks the availalbe space on the buffer.
     * @details This method checks if there is space available in the buffer for a write operation of a certain ammount of bytes.
     * @param[in] writtenBytes Number of bytes to be written into the buffer
     * @returns true if the buffer has space for the specified number of bytes, false otherwise.
     */
    bool CheckAvailableSpace(uint32 writtenBytes);
    
    /**
     * @brief Method which checks if the buffer contains the needed ammount of samples to be delivered into the DataSource.
     * @details This method checks if the amount of samples stored in the buffer is enough to deliver a full read in a single operation 
     * (SamplesPerExternalRead samples per channel)
     *
     * @returns true if the buffer has enough samples to deliver to the DataSource, false otherwise.
     */
    bool CheckReadReady();

    /**
     * @brief Method which checks if the buffer contains the needed ammount of samples to be delivered into the DataSource.
     * @details This method checks if the amount of samples stored in the buffer is enough to deliver a read of a certain ammount of bytes 
     * in a single operation 
     *
     * @returns true if the buffer has enough samples to deliver to the DataSource, false otherwise.
     */
    bool CheckReadReady(uint32 readBytes);
    
    /**
     * @brief Method which updates the buffer indexes and pointers after a data write into the buffer.
     * @details This method must be called after each write to writePointer location providing the amount of bytes written in the buffer. The method
     * internally updates the indexes and pointers of the buffer to allow correct operation of the buffer.
     * @param[in] writtenBytes number of bytes written into the buffer on the last write operation to writePointer.
     * @returns true if the buffer is updated correctly, false otherwise.
     */
    bool AdvanceBufferIndex(uint32 writtenBytes);

    /**
     * @brief Method which allows the data to be retrieved from the buffer.
     * @details The implementation of this method is done through the usage of UEIBufferPointers, which serve as virtual arrays effectively containing
     * the samples for each of the retrieved channels in this circularBuffer. This method retrieves the memory of all the configured channels (minus the timestamp)
     * in the buffer as an array of UEIBufferPointers (sorted by the channel setting order in the buffer)
     * @param[out] ok Flag stating if the read operation is valid by setting to true, or false otherwise.
     * @returns pointer to an array of UEIBufferPointer objects to access the virtual arrays for each of the channels. The channels are retrieved in the
     * same order as they're retrieved by the UEIDAQ hardware layer packet and therefore the configured order during map enabling.
     */
    UEIBufferPointer* ReadBuffer(bool& ok);

    /**
     * @brief Method which allows the data from a single channel to be retrieved from the buffer.
     * @details The implementation of this method is done through the usage of UEIBufferPointers, which serve as virtual arrays effectively containing
     * the samples for each of the retrieved channels in this circularBuffer. This method retrieves a single channel by channelIdx (index of the channel within
     * the configured channels in configuration order).
     * @param[in] chanelIdx Channel idx (index within the configured channels).
     * @param[out] pointer UEIBufferPointer instance pointing to the data of the requested channel.
     * @returns true if the read of the channel succeeded, false otherwise.
     */
    bool ReadChannel(uint32 chanelIdx, UEIBufferPointer& pointer);

    /**
     * @brief Method which allows the timestamp channel data to be retrieved from the buffer.
     * @details The implementation of this method is done through the usage of UEIBufferPointers, which serve as virtual arrays effectively containing
     * the samples for each of the retrieved channels in this circularBuffer.
     * @param[out] ok Flag stating if the read operation is valid by setting to true (and the timestamp channel is configured in this buffer),
     * or false otherwise.
     * @returns Pointer to a UEIBufferPointer containing the access to the timestamp channel retrieved from the hardware layer in this buffer
     * as a virtual array.
     */
    bool ReadTimestamp(UEIBufferPointer& pointer);
    
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

    /**
     * @brief Method to mark the data on the buffer as read and allow new data to come into the buffer without overflow.
     * @details This method marks the next SamplesPerExternalRead samples per channel to be read as read, and frees the memory to be used in subsequent
     * writes into the buffer. This method must be called upon a full read to free the buffer and allow the next samples to be read/written.
     * @returns true if the operation was successful, false otherwise.
     */
    bool CheckoutBuffer();

    /**
     * @brief Method which advances the read pointer in the buffer 1 sample per channel.
     * @details This method advances the read pointer in the buffer 1 sample per channel, effectively marking 1 sample per channel as read.
     * This method is of special usefulness in DMap mode, where the data is read/written in a 1 sample basis.
     * @returns true if the operation was successful, false otherwise.
     */
    bool AdvanceBufferReadOneSample();
    
    /**
     * @brief Method which reads a certain ammount of bytes and copies them in a destination memory location.
     * @details This method reads a certain amount of bytes into a destination location. It also may advance the read pointer a readBytes ammount if requested,
     * effectively freeing the desired ammount of bytes for write operations.
     * @param[in] readBytes number of bytes to read from the buffer into the destination location.
     * @param[in] destination pointer to the destination area that will hold the requested read bytes.
     * @param[in] advancePointer flag signaling if the readPointer should be advanced readBytes after copying the contents of the buffer into the 
     * destination location.
     * @returns true if the operation was successful, false otherwise.
     */
    bool ReadBytes(uint32 readBytes, uint8* destination, bool advancePointer);
    
    /**
     * @brief Method which advances a certain ammount of bytes the read pointer.
     * @details This method advances the read pointer the read bytes if requested,
     * effectively freeing the desired ammount of bytes for write operations.
     * @param[in] readBytes number of bytes to read from the buffer into the destination location.
     * @returns true if the operation was successful, false otherwise.
     */
    bool AdvanceReadPointer(uint32 readBytes);

    /**
     * @brief Method which returns the number of bytes available to be read from the buffer.
     * @returns number of bytes available for reading from the buffer.
     */
    uint32 GetAvailableBytesToRead();

    /**
     * @brief Method which sets a certain ammount of bytes (from the writePointer) to 0.
     * @details This method sets the next bytesToWrite bytes to 0 at the writePointer location, effectively clearing any previous values.
     * This method is specially useful when the write into the buffer depends on the current value of the sample and that needs a reset at some point.
     * @param[in] bytesToWrite number of bytes to set to 0 from the writePointer location.
     * @returns true if the operation whas successfull, false otherwise.
     */
    bool ZeroNextBytes(uint32 bytesToWrite);
    
    /**
     * @brief Method to get a list of UEIBufferPointers pointing to the write locations for each of the configured channels.
     * @details This method returns a list of UEIBufferPointers with the write locations of the next samples to be written into the buffer.
     * @param[out] ok flag signaling if the operation was performed successfully.
     * @returns a list of UEIBufferPointer pointing to the write locations for the configured channels.
     */
    UEIBufferPointer* GetBufferWritePointers(bool& ok);

    /**
     * @brief Method to get a UEIBufferPointer pointing to the write location for the requested channel.
     * @details This method returns a UEIBufferPointer pointing to the next write location for the requested channel.
     * @param[in] chanelIdx channel index within the configured channel list to be retrieved.
     * @param[out] pointer returned UEIBufferPointer poiting to the write locations of the requested channel.
     * @returns true if the operation is performed successfully.
     */
    bool GetWritePointer(uint32 chanelIdx, UEIBufferPointer& pointer);

    /*
    *   Public pointer allowing write access into the buffer. When new data is recieved from IOM it must be written into this location on the buffer.
    *   Before writting into such location the developer is responsible to check if enough space is available in the buffer by calling the CheckAvailableSpace method
    *   After writting into the buffer the developer is responisble to advance the pointer by calling the AdvanceBufferIndex method. The developer must not
    *   perform any actions other than writting on this pointer.
    */
    uint8* writePointer;

protected:

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
    *   Pointer to the read position of the buffer. This pointer is uint8 as the bus width of the
    *   UEIDAQ bus is 32 bit but the width of the samples may change for each hardware layer. 
    */
    uint8* readPointer;

    /**
    *   Variable holding the number of individual buffers that this buffer can contain
    */
    uint32 nOfBuffers;
    
    /**
    *   Variable holding the maximum number of samples per channels to be retrieved in a VMap packet request.
    */
    uint32 samplesPerExternalWrite;

    /**
    *   Samples to be read for each channel on DataSource sycnhronize call.
    */
    uint32 samplesPerExternalRead;
    
    /**
    *   Variable holding the length in bytes of an individual buffer in this circular buffer
    */
    uint32 singleBufferLength;
    
    /**
    *   Variable holding the length in bytes of the zone after the latest individual buffer to store the mismatched samples from the last write.
    */
    uint32 runawayZoneLength;
    
    /**
    *   Length of the active buffer area in bytes.
    */
    uint32 bufferLength;

    /**
    *   Length of the full buffer area in bytes.
    */
    uint32 fullBufferLength;

    /**
    *   Variable holding the number of channels defined for the VMap assigned to this circular buffer.
    */
    uint32 nChannels;

    /**
    *   Variable holding the width (in bytes) of the samples for the channels on this circular buffer.
    */
    uint8 sizeOfSamples;
    
    /**
    *   List of UEIBufferPointer objects to access the internal channel list of the read buffer
    */
    UEIBufferPointer* readPointerList;
    
    /**
    *   List of UEIBufferPointer objects to access the internal channel list of the write buffer
    */
    UEIBufferPointer* writePointerList;

    /**
    *   UEIBufferPointer object to access the timestamp
    */
    UEIBufferPointer timestampList;
};
}
#endif /* UEICircularBuffer_H_ */