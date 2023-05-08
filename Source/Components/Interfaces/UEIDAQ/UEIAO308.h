/**
 * @file UEIAO308.h
 * @brief Header file for class UEIAO308
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

 * @details This header file contains the declaration of the class UEIAO308
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIAO308_H_
#define UEIAO308_H_

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
//Interface specific includes
#include "UEIDevice.h"
#include "UEIDefinitions.h"
//PowerDNA library includes
#include "PDNA.h"
#include "powerdna.h"

namespace MARTe {

#define MAX_FIR_TAPS_803 512u
#define FIR_BANK_NUMBER 4u
#define CHANNEL_NUMBER 16u

/**
 * @brief Device class for UEIDAQ AO-308 hardware layer.
 *
 * @details This device-specific class inherits from UEIDevice and implements layer-specific methods for UEI AO-308 hardware layer.
 * No additional configuration parameters are required for this hardware layer.
 *
 * A sample configuration for the AO-308 is shown below:
 * <pre>
 *  + dev0={
 *      Class               = UEIAO308
 *      Devn                = 0
 *      SamplingFrequency   = 1000.0
 *  }
 * </pre>
 */

class UEIAO308 : public UEIDevice {
    public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details NOOP.
     */
    UEIAO308();

    /**
     * @brief Default destructor.
     * @details NOOP.
     */
    virtual ~UEIAO308();

    /**
     * @brief Initialise the Object from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity.
     * @return true if every parameter has been read correctly and validated.
     */
    bool Initialise(StructuredDataI &data);
    
    /**
     * @brief Method to scale an array of samples retrieved from the hardware layer into its real value.
     * @details This method returns an array of scaled samples from the raw value of the data retrieved from
     * IOM by performing the proper conversions. 
     *
     * For this specific implementation this method always returns false, as the AO-308 hardware layer does not provide support for input
     * channels, only output channels.
     * @param[in] channelNumber channel identifier within the device channel list (first channel is idnexed as 0)
     * @param[in] listLength length of the array of raw data fed into the method. Developer must ensure
     * it is properly sized.
     * @param[in] rawData UEIBufferPointer to the virtual array on a UEICircularBuffer in which the raw data is stored.
     * @param[out] scaledData pointer to the output array of scaled values.
     * @param[out] outputType type of the output data to be retrieved to the destination memory area.
     * @return false, this hardware layer does not support input signals.
     */
    bool ScaleSignal(uint32 channelNumber, uint32 listLength, UEIBufferPointer rawData, void* scaledData, TypeDescriptor outputType);

    /**
     * @brief Returns the model number of this specific hardware layer.
     * @return 0x308.
     */
    int32 GetModel();

    /**
     * @brief Returns the kind of data to be retrieved from this hardware layer.
     * @return HARDWARE_LAYER_ANALOG_O.
     */
    IOLayerType GetType();

    /**
     * @brief Returns the sample size in bytes of the data returned by a query to this model of hardware layer.
     * @return sizeof(uint16).
     */
    uint8 GetSampleSize();
    
    /**
     * @brief Returns the number of channels available on this hardware layer model.
     * @return 8 if direction is OutputSignals, 0 otherwise.
     */
    uint32 GetDeviceChannels(SignalDirection direction);
    
    /**
     * @brief Method to check if a channel is a valid to be used in a certain direction (input or output channel).
     * @details This method checks if channel number provided (first channel is indexed as 0) is valid within this device
     * in the specified direction (input or output channel).
     * @param[in] channelNumber channel index (first channel indexes to 0).
     * @param[in] direction direction for the specified channel query  
     * @return true if the specified channel is valid within the layer with the direction provided.
     */
    bool CheckChannelAndDirection(uint32 channelNumber, SignalDirection direction);

    /**
     * @brief Method to configure a channel in the specific configuration scheme of the device.
     * @details This method returns the configuration bitfield for the specified channel in the device. The bitfield is used as the configuration word to set
     * the required parameters in the IOM.
     *
     * For this specific implementation, the number of configurationBitFileds always is equal to the number of channels in the specified direction.
     * @param[in] channelNumber channel index (first channel indexes to 0).
     * @param[out] channelConfiguration pointer to channel configuration bitfield for the specified channel in the device. 
     * @return true if configuration process succeeds, false otherwise.
     */
    bool ConfigureChannels(SignalDirection direction, uint32** configurationBitfields, uint32& nConfigurationBitfields, MapType mapType);
   
    /**
     * @brief Method to configure the device.
     * @details This method performs the required configuration and initialisation procedures for the specific device (hardware layer).
     * For this specific implementation no configuration is performed on the IOM.
     * @param[in] DAQ_handle IOM handle provided by UEIMasterObject to issue initialisation commands to the device.
     * @return true if the configuration process succeeds, false otherwise.
     */
    bool ConfigureDevice(int32 DAQ_handle);
    
    /**
     * @brief Method to check if a signal type is accepted as input/output type.
     * @details This method checks if the signal type provided is a valid type for input/output operations for the specific device (hardware layer).
     * Accpeted signal typed for this device are float32, float64, uint16, uint32 and uint64.
     * @param[in] signalType the type which is being queried as input/output type.
     * @return true if the supplied signal type is within the accepted types.
     */
    bool AcceptedSignalType(TypeDescriptor signalType);
    
    /**
     * @brief Method to retrieve the input signal from IOM from Device memory bufffer.
     * @details This method is used to retrieve the input signal data contained in the inputChannelsBuffer memory buffer on this object and copy the properly
     * scaled signal into a destination supplied by the developer. It is the preferred method to retrieve data from the input packets from IOM, both DMap and VMap.
     * @param[in] channelIdx channel index of the channel to be retrieved (first channel is indexed as 0). The developer must ensure this parameter corresponds
     * to a valid an configured channel, otherwise the method call will fail.
     * @param[in] nSamples number of samples to be retrieved from the device buffer into the destination memory.
     * @param[in] SignalPointer pointer to the destination memory location, where the scaled samples will be stored after method call. Take into account the
     * desired type of the samples and number of samples to grant enough space for the destination memory area.
     * @param[in] signalType type of the signal to be retrieved, the type must be an accepted type for the specific hardware layer.
     * @return false, as this hardware layer does not support input signals.
     */
    bool RetrieveInputSignal(uint32 channelIdx, uint32 nSamples, void* SignalPointer, TypeDescriptor signalType);

    /**
     * @brief Method to retrieve the set an output signal for a channel of the hardware layer to be pushed into the IOM.
     * @details This method is used to push output signal data contained into the outputChannelsBuffer memory buffer on this object to be later pushed into the IOM.
     * It is the preferred method to set output data for the output packets to IOM, both DMap and VMap. Data to be pushed is supplied from a memory area (where MARTe output 
     * signals are copied) with an specific type which must be accepted by the device object.
     * @param[in] channelIdx channel index of the channel to be set with new data (first channel is indexed as 0). The developer must ensure this parameter corresponds
     * to a valid an configured channel, otherwise the method call will fail.
     * @param[in] nSamples number of samples to be pushed to the device buffer.
     * @param[in] SignalPointer pointer to the source memory location, where the scaled samples are stored during method call.
     * @param[in] signalType type of the signal to be retrieved from source memory area, the type must be an accepted type for the specific hardware layer.
     * @return true if the output signal setting process is successful.
     */
    bool SetOutputSignal(uint32 channelIdx, uint32 nSamples, void* SignalPointer, TypeDescriptor signalType);

    /**
     * @brief Method to set and initialise the internal buffers for the device to hold the retrieved/pushed data into the IOM.
     * @details This method performs the initialisation procedure for the input and output memory buffers on the device which hold the data 
     * retrieved / to be pushed from the IOM. The buffers must be initialised through this method before their usage.
     * @param[in] direction direction of the buffer to be initialised (for input or output signals).
     * @param[in] nBuffers number of buffers to configure for the specific buffer direction.
     * @param[in] writeSamples maximum number of samples to be written into the buffer in a single operation.
     * @param[in] readSammples maximum number of samples to be read from the buffer in a single operation.
     * @return true if the buffer initialisation process succeeds.
     */
    bool InitBuffer(SignalDirection direction, uint32 nBuffers, uint32 writeSamples, uint32 readSammples);

    /**
     * @brief Method to get the number of "channels" understood as "FIFO channels" in the specific hardware layer.
     * @details This method returns the number of "FIFO channels" to retrieve/set in the IOM. The definition needs to be done independently
     * for each hardware layer as some hardware layers combine the readings of multiple channels in a single "FIFO channels" (such as DIO404) and others
     * use a single "FIFO channels" for each physical channel
     * 
     * This specific implementation of the method always return the same number of configured channels for the direction, as the layer sets 1 FIFO channel
     * per physical channel.
     * @param[in] direction direction for which to get the number of "FIFO channels" to retrieve.
     * @param[out] nChannels number of "FIFO channels" to set/retrieve for this device and direction.
     * @return true.
     */
    bool GetHardwareChannels(SignalDirection direction, uint32& nChannels);
};

}
#endif /*UEIAO308_H_*/