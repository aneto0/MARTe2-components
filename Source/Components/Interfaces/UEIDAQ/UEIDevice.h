/**
 * @file UEIDevice.h
 * @brief Header file for class UEIDevice
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

 * @details This header file contains the declaration of the class UEIDevice
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIDevice_H_
#define UEIDevice_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
//MARTe includes
#include "Object.h"
#include "StreamString.h"
#include "StructuredDataI.h"
#include "StructuredDataIHelper.h"
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
//This include is just for the SignalDirection enum
#include "DataSourceI.h"
//Interface specific includes
#include "UEIDefinitions.h"
#include "UEIBufferPointer.h"
#include "UEICircularBuffer.h"
//PowerDNA library include
#include "PDNA.h"

namespace MARTe {

/**
 * @brief Base class for UEIDAQ hardware layer class implementation.
 *
 * @details This class provides base parameters and checks for any hardware layer class of UEIDAQ system. Any 
 * layer specific implementation class must derive from this base class as it does provide generic interface from
 * MapContainer and MasterObject perspective, generalising the access to different UEIDAQ hardware layers.
 * 
 * Some of the functions (virtual methods) contained in this class must be reimplemented in the child class with the appropriate methods
 * for the target hardware layer.
 *
 * The initialisation method of this class checks and acquires the parameters Devn and SamplingFrequency provided for 
 * a device child class derived from this one.
 *
 * This class should not be instantiated as a device object in MARTe but rather the derived classes from this base class.
 *
 */

class UEIDevice : public Object {
    public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details NOOP.
     */
    UEIDevice();

    /**
     * @brief Default destructor.
     * @details NOOP.
     */
    virtual ~UEIDevice();

    /**
     * @brief Initialise the base UEIDevice from a configuration file.
     * @details This implementation of the initialise provides the checks of the 
     * base parameters any UEIDAQ device should contain in a MARTe application.
     * Set as virtual method so the derived class (implementing the specific hardware layer)
     * can redefine its Initialise method. This method may be called within the derived class
     * Initialise method to initialise the base parameters of the hardware layer.
     * @return true if every parameter has been read correctly and validated.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Getter for the device model number.
     * @details This method returns the device model number.
     *
     * Implementation must be provided in each of the device-specific child classes for its specific model number.
     * @return 0.
     */
    virtual int32 GetModel();
    
    /**
     * @brief Getter for the device type (hardware layer interface type).
     * @details This method returns the device type.
     *
     * Implementation must be provided in each of the device-specific child classes for its specific device type.
     * @return NO_LAYER.
     */
    virtual IOLayerType GetType();
    
    /**
     * @brief Getter for the device sample size (sample size in bytes).
     * @details This method returns the device sample size in bytes.
     *
     * Implementation must be provided in each of the device-specific child classes for its specific sample size.
     * @return 0
     */
    virtual uint8 GetSampleSize();
    
    /**
     * @brief Getter for the number of channels of the device.
     * @details This method returns the number of channels of the device.
     *
     * Implementation must be provided in each of the device-specific child classes for its specific number of channels.
     * @return 0
     */
    virtual uint32 GetDeviceChannels(SignalDirection direction);
    
    /**
     * @brief Method to check if a channel is a valid to be used in a certain direction (input or output channel).
     * @details This method checks if channel number provided (first channel is indexed as 0) is valid within this device
     * in the specified direction (input or output channel).
     *
     * Implementation of this method must be provided in each of the device-specific child classes for their specific I/O characteristics.
     * @param[in] channelNumber channel index (first channel indexes to 0).
     * @param[in] direction direction for the specified channel query
     * @return false.
     */
    virtual bool CheckChannelAndDirection(uint32 channelNumber, SignalDirection direction);

    /**
     * @brief Method to check if a list of channels is a valid to be used in a certain direction (input or output channel).
     * @details This method checks if the channel numbers provided in a list (first channel is indexed as 0) are valid within this device
     * in the specified direction (input or output channel).
     *
     * Implementation of this method relies on CheckChannelAndDirection() method which must be re-defined in each specific hardware layer
     * child object.
     * @param[in] channelList pointer to an array of channel numbers of length listLength.
     * @param[in] listLength length of the array supplied in channelList.
     * @param[in] direction direction for the specified channels query
     * @return false.
     */
    bool CheckChannelListAndDirection(uint32* channelList, uint32 listLength, SignalDirection direction);
    
    /**
     * @brief Method to configure a channel in the specific configuration scheme of the device.
     * @details This method returns the configuration bitfield for the specified channel in the device. The bitfield is used as the configuration word to set
     * the required parameters in the IOM.
     *
     * Implementation of this method must be provided in each of the device-specific child classes for their specific configuration flags.
     * @param[in] channelNumber channel index (first channel indexes to 0).
     * @param[out] channelConfiguration pointer to channel configuration bitfield for the specified channel in the device. 
     * @return false.
     */
    virtual bool ConfigureChannels(SignalDirection direction, uint32** configurationBitfields, uint32& nConfigurationBitfields, MapType mapType);
    
    /**
     * @brief Method to configure the device.
     * @details This method performs the required configuration and initialisation procedures for the specific device (hardware layer).
     *
     * Implementation of this method must be provided in each of the device-specific child classes for their specific configuration procedures.
     * @param[in] DAQ_handle IOM handle provided by UEIMasterObject to issue initialisation commands to the device.
     * @return false.
     */
    virtual bool ConfigureDevice(int32 DAQ_handle);

    /**
     * @brief Method to check if a signal type is accepted as input/output type.
     * @details This method checks if the signal type provided is a valid type for input/output operations for the specific device (hardware layer).
     *
     * Implementation of this method must be provided in each of the device-specific child classes for their specific type characteristics.
     * @param[in] signalType the type which is being queried as input/output type.
     * @return false.
     */
    virtual bool AcceptedSignalType(TypeDescriptor signalType);

    /**
     * @brief Method to scale an array of samples retrieved from the hardware layer into its real value.
     * @details This method returns an array of scaled samples from the raw value of the data retrieved from
     * IOM by performing the proper conversions. 
     *
     * This base class implementation is generic for all the UEIDAQ devices and should not be redefined in child classes.
     * @param[in] channelNumber channel identifier within the device channel list (first channel is idnexed as 0)
     * @param[in] listLength length of the array of raw data fed into the method. Developer must ensure
     * it is properly sized.
     * @param[in] rawData UEIBufferPointer to the virtual array on a UEICircularBuffer in which the raw data is stored.
     * @param[out] scaledData pointer to the output array of scaled values.
     * @param[out] outputType type of the output data to be retrieved to the destination memory area.
     * @return false.
     */
    virtual bool ScaleSignal(uint32 channelNumber, uint32 listLength, UEIBufferPointer rawData, void* scaledData, TypeDescriptor outputType);

    /**
     * @brief Getter for device Id (devn in PowerDNA API).
     * @details This method returns the device id (devn) for the device. This base class implementation
     * is generic for all the UEIDAQ devices and should not be redefined in child classes.
     * @return devn configured for this device, 0xFF if the device has not been initialised.
     */
    uint8 GetDevN();

    /**
     * @brief Setter for hardwareCorrespondence flag.
     * @details This method sets the hardware correspondence flag (which cannot be unset externally).
     * @return true if the hardwareCorrespondence flag was previously false and therefore the device has not been previously set, false otherwise.
     */
    bool SetHardwareCorrespondence();
    
    /**
     * @brief Getter for hardwareCorrespondence flag.
     * @details This method returns the value of the hardware correspondence flag.
     * @return value of hardwareCorrespondence flag.
     */
    bool GetHardwareCorrespondence();

    /**
     * @brief Setter for assignedToMap flag.
     * @details This method sets the map assignment flag (which cannot be unset externally).
     * Implementation must be only done in this base class and not in child classes, as this behavior is 
     * not device-dependant.
     * @return true if the assignedToMap flag was previously false and therefore the device has not been previously assigned to a map, false otherwise.
     */
    bool SetMapAssignment();

    /**
     * @brief Getter for assignedToMap flag.
     * @details This method returns the value of the map assignment flag (which cannot be unset externally).
     * Implementation must be only done in this base class and not in child classes, as this behavior is 
     * not device-dependant.
     * @return value of assignedToMap.
     */
    bool GetMapAssignment();

    /**
     * @brief Getter for samplingFrequency flag.
     * @details This method returns the value of samplingFrequency.
     * Implementation must be only done in this base class and not in child classes, as this behavior is 
     * not device-dependant.
     * @return value of samplingFrequency.
     */
    float GetSamplingFrequency();

    /**
     * @brief Getter for the name of the device.
     * @details This method returns the name of the device (node name).
     * @return the name of the device.
     */
    char8* GetName();

    /**
     * @brief Method to retrieve the input signal from IOM from Device memory bufffer.
     * @details This method is used to retrieve the input signal data contained in the inputChannelsBuffer memory buffer on this object and copy the properly
     * scaled signal into a destination supplied by the developer. It is the preferred method to retrieve data from the input packets from IOM, both DMap and VMap.
     *
     * This base class implementation is generic for all the UEIDAQ devices and should not be redefined in child classes.
     * @param[in] channelIdx channel index of the channel to be retrieved (first channel is indexed as 0). The developer must ensure this parameter corresponds
     * to a valid an configured channel, otherwise the method call will fail.
     * @param[in] nSamples number of samples to be retrieved from the device buffer into the destination memory.
     * @param[in] SignalPointer pointer to the destination memory location, where the scaled samples will be stored after method call. Take into account the
     * desired type of the samples and number of samples to grant enough space for the destination memory area.
     * @param[in] signalType type of the signal to be retrieved, the type must be an accepted type for the specific hardware layer.
     * @return false
     */
    virtual bool RetrieveInputSignal(uint32 channelIdx, uint32 nSamples, void* SignalPointer, TypeDescriptor signalType);
    
    /**
     * @brief Method to retrieve the set an output signal for a channel of the hardware layer to be pushed into the IOM.
     * @details This method is used to push output signal data contained into the outputChannelsBuffer memory buffer on this object to be later pushed into the IOM.
     * It is the preferred method to set output data for the output packets to IOM, both DMap and VMap. Data to be pushed is supplied from a memory area (where MARTe output 
     * signals are copied) with an specific type which must be accepted by the device object.
     *
     * This base class implementation is generic for all the UEIDAQ devices and should not be redefined in child classes.
     * @param[in] channelIdx channel index of the channel to be set with new data (first channel is indexed as 0). The developer must ensure this parameter corresponds
     * to a valid an configured channel, otherwise the method call will fail.
     * @param[in] nSamples number of samples to be pushed to the device buffer.
     * @param[in] SignalPointer pointer to the source memory location, where the scaled samples are stored during method call.
     * @param[in] signalType type of the signal to be retrieved from source memory area, the type must be an accepted type for the specific hardware layer.
     * @return false
     */
    virtual bool SetOutputSignal(uint32 channelIdx, uint32 nSamples, void* SignalPointer, TypeDescriptor signalType);
    
    /**
     * @brief Method to set the list of channels configured as inputs for this specific device.
     * @details This method checks for validity in the configured input channel list for the device, checking that the channels are within the valid channels
     * for the device in input direction and the channel list is supplied in ascending order with no repeated channels.
     * @param[in] channelList pointer to an array containing the indexes of the channels to be used as inputs for this device.
     * @param[in] nChannels length or number of channels of the array in channelList.
     * @return true if the supplied channel list is valid.
     */
    bool SetInputChannelList (uint32* channelList, uint32 nChannels);

    /**
     * @brief Method to set the list of channels configured as outputs for this specific device.
     * @details This method checks for validity in the configured output channel list for the device, checking that the channels are within the valid channels
     * for the device in output direction and the channel list is supplied in ascending order with no repeated channels.
     * @param[in] channelList pointer to an array containing the indexes of the channels to be used as outputs for this device.
     * @param[in] nChannels length or number of channels of the array in channelList.
     * @return true if the supplied channel list is valid.
     */
    bool SetOutputChannelList (uint32* channelList, uint32 nChannels);

    /**
     * @brief Method to set and initialise the internal buffers for the device to hold the retrieved/pushed data into the IOM.
     * @details This method performs the initialisation procedure for the input and output memory buffers on the device which hold the data 
     * retrieved / to be pushed from the IOM. The buffers must be initialised through this method before their usage.
     *
     * This base class implementation is generic for all the UEIDAQ devices and should not be redefined in child classes.
     * @param[in] direction direction of the buffer to be initialised (for input or output signals).
     * @param[in] nBuffers number of buffers to configure for the specific buffer direction.
     * @param[in] writeSamples maximum number of samples to be written into the buffer in a single operation.
     * @param[in] readSammples maximum number of samples to be read from the buffer in a single operation.
     * @return false.
     */
    virtual bool InitBuffer(SignalDirection direction, uint32 nBuffers, uint32 writeSamples, uint32 readSammples);
    
    /**
     * @brief Method to get the number of "channels" understood as "FIFO channels" in the specific hardware layer.
     * @details This method returns the number of "FIFO channels" to retrieve/set in the IOM. The definition needs to be done independently
     * for each hardware layer as some hardware layers combine the readings of multiple channels in a single "FIFO channels" (such as DIO404) and others
     * use a single "FIFO channels" for each physical channel
     *
     * This base class implementation is generic for all the UEIDAQ devices and should not be redefined in child classes.
     * @param[in] direction direction for which to get the number of "FIFO channels" to retrieve.
     * @param[out] nChannels number of "FIFO channels" to set/retrieve for this device and direction.
     * @return false.
     */
    virtual bool GetHardwareChannels(SignalDirection direction, uint32& nChannels);

    /**
    *   Flag signaling if this device is set to retrieve a timestamp channel as an input channel
    */
    bool timestampRequired;
    
    /**
    *   Reference to the buffer holding input channels samples
    */
    ReferenceT<UEICircularBuffer> inputChannelsBuffer;

    /**
    *   Reference to the buffer holding output channels samples
    */
    ReferenceT<UEICircularBuffer> outputChannelsBuffer;
    
protected:
    /**
     * @brief Protected helper method to convert ant input signal type into a boolean signal.
     * @details This method is used mainly by digital I/O hardware layers to retrieve the boolean signals from any output signals.
     * @param[in] nSamples number of samples in the supplied signal.
     * @param[in] booleanSignal pointer to an array where the destination boolean signal is to be stored.
     * @param[in] inputSignal pointer to the input signal to convert into boolean signal.
     * @param[in] signalType type of the input signal into the method call.
     * @return true if the signal type is accepted and the boolean conversion is performed successfully.
     */
    bool AnyTypeToBoolean(uint32 nSamples, bool* booleanSignal, void* inputSignal, TypeDescriptor signalType);

    /**
     * @brief Protected helper method to find the index of a channel number in the configured channel list for the direction.
     * @details This helper method finds the index of a supplied channel in the configured input or output channel list.
     * @param[in] channelNumber index of the channel to find in the configured channel list (first channel is indexed as 0).
     * @param[in] direction direction in which to search for the channel id.
     * @return index within the configured channel list if present, -1 otherwise.
     */
    int32 FindChannelIndex(uint32 channelNumber, SignalDirection direction);

    /**
    *   Variable holding the device name
    */
    StreamString name;

    /**
    *   Variable holding the device Id (devn)
    */
    uint8 deviceId;                 

    /**
    *   Variable holding the sampling frequency set for this device
    */
    float samplingFrequency;
    
    /**
    *   Flag identifying if this device is recognized by the IOM as configured in MARTe (checks devn and model)
    */
    bool hardwareCorrespondence;
    
    /**
    *   Flag identifying if this device has been assigned to a MapContainer object (true if alerady set for a map, false otherwise)
    */
    bool assignedToMap;
    
    /**
    *   Variable holding the number of input channels configured for this device
    */
    uint32 nInputChannels;

    /**
    *   Array holding the list of configured input channels (channels idx)
    */
    uint32* inputChannelList;

    /**
    *   Variable holding the number of output channels configured for this device
    */
    uint32 nOutputChannels;

    /**
    *   Array holding the list of configured output channels (channels idx)
    */
    uint32* outputChannelList;
};

}
#endif /*UEIDevice_H_*/