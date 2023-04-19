/**
 * @file UEIDIO404.h
 * @brief Header file for class UEIDIO404
 * @date 17/04/2023
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

 * @details This header file contains the declaration of the class UEIDIO404
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIDIO404_H_
#define UEIDIO404_H_

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

#define CHANNEL_NUMBER 24u

/**
 * @brief Device class for UEIDAQ DIO-404 hardware layer.
 *
 * @details This device-specific class inherits from UEIDevice and implements layer-specific methods for UEI AI-217-803 hardware layer.
 * This class adds support for analog gains on the hardware layer under the parameter Gains and configuration of the FIR filtering
 * capabilities of the layer through the configuration on the Filters block.
 *
 * The layer's ADCs can be configured (all of them) in two different operation modes : DEFAULT or ENHANCED modes, refer to UEI documentation on AI-217-803
 * for details on each operating mode. This mode can be configured through the ADC_mode parameter by setting a string of value "DEFAULT" or "ENHANCED".
 * This parameter is optional and set to DEFAULT mode if not provided.
 *
 * This object expects a Gains parametr to be a 16 number array stating the amplifier gain for each channel (ordered). The allowed gain values
 * for each channel are 1, 2, 4, 8, 16, 32 or 64, any other gain provided in this array will cause the initialisation to fail.
 *
 * This hardware layer retrieves data in 24-bit format (24 bit ADCs) so the only valid I/O signal types are uint32, uint64, float32 and float64
 * in order to avoid precision/resolution loss on the data retrieved in this layer.
 *
 * As the DIO-404 hardware layer is provided with a quad FIR filter stage, this capability can be configured by properly setting the Filters
 * block expected on the object configuration. The hardware layer provides 4 FIR filtering banks with 512 taps each, dividing the channels as:
 * - Bank A : channels 0 to 3
 * - Bank B : channels 4 to 7
 * - Bank C : channles 8 to 11
 * - Bank D : channels 12 to 15
 * 
 * The Filters block allows for configuration of the FIR banks separatedly, but if the block is not provided all the FIR banks are automatically
 * disabled and no FIR filtering is provided. In the case the Filters block is provided, if the bank (identified by the bank letter) is not defined,
 * the filter is disabled on such bank, otherwise the bank is enabled and configured depending on the supplied parameters:
 * - Taps parameter supplied for the bank : An array of 512 or less FIR taps is provided in the Taps paramter to configure a custom filter by
 * its taps.
 * - Default_filter parameter supplied for the bank : The Default_filter parameter defines the index of the defautl FIR configuration as provided in
 * the AI-217-803 layer documentation by UEI. The bank is enabled with such configuration.
 *
 * User must only enable one configuration scheme for each FIR filter bank, otherwise the initialisation procedure will fail.
 *
 * See below an example configuration for this specific hardware layer:
 * <pre>
 *  + dev0={
 *      Class               = UEIDIO404
 *      Devn                = 0
 *      SamplingFrequency   = 1000.0
 *      Filters = {
 *         A = {
 *             Taps = {0,1,0,2,0,3,0,4,0,5,6}
 *          }
 *          B = {
 *              Default_filter = 0
 *          }
 *          C = {
 *              Default_filter = 1
 *          }
 *          D = {
 *              Default_filter = 2
 *          }
 *      }
 *  }
 * </pre>
 */
class UEIDIO404 : public UEIDevice {
    public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details NOOP.
     */
    UEIDIO404();

    /**
     * @brief Default destructor.
     * @details NOOP.
     */
    virtual ~UEIDIO404();

    /**
     * @brief Initialise the Object from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity.
     * @return true if every parameter has been read correctly and validated.
     */
    bool Initialise(StructuredDataI &data);
    
    /**
     * @brief Scales a set of samples obtained from the UEIDAQ as raw data into the desired type.
     * @details This method allows a block on raw data from the UEIDAQ hardware layer to be processed into the specified
     * type for later usage.
     * @param[in] channelNumber Index of the channel to which the provided samples correspond (first channel is indexed as 0)
     * @param[in] listLength Length (in samples) of the provided array of input data to be processed
     * @param[in] rawData Array of raw data to be processed as listLenght number of samples.
     * @param[out] scaledData Pointer to the destination memory area where the scaled result is to be stored. The developer is responsible to
     * allocate such space taking into account the size of the desired output type and the number of samples to be scaled.
     * @param[in] type Type to which the input raw data is to be scaled to. The allowed types are checked according to AcceptedSignalType() method
     * @return true if the scaling process was successful, false otherwise.
     */
    bool ScaleSignal(uint32 channelNumber, uint32 listLength, void* rawData, void* scaledData, TypeDescriptor type);
    
    /**
     * @brief Scales a set of samples obtained from the UEIDAQ as raw data into the desired type.
     * @details Variant of the ScaleSignal() method in which the raw input data is provided as a UEIBufferPointer special iterator instance,
     * allowing to reduce the amount of memory copy operations to read directly from the UEIBuffer.
     * @param[in] channelNumber Index of the channel to which the provided samples correspond (first channel is indexed as 0)
     * @param[in] listLength Length (in samples) of the provided array of input data to be processed
     * @param[in] rawData UEIBufferPointer instance configured to point to the samples retrieved from a UEICircularBuffer.
     * @param[out] scaledData Pointer to the destination memory area where the scaled result is to be stored. The developer is responsible to
     * allocate such space taking into account the size of the desired output type and the number of samples to be scaled.
     * @param[in] type Type to which the input raw data is to be scaled to. The allowed types are checked according to AcceptedSignalType() method
     * @return true if the scaling process was successful, false otherwise.
     */
    bool ScaleSignal(uint32 channelNumber, uint32 listLength, UEIBufferPointer rawData, void* scaledData, TypeDescriptor outputType);

    /**
     * @brief Returns the model number of this specific hardware layer.
     * @return the model number in hexadecimal of this hardware layer.
     */
    int32 GetModel();

    /**
     * @brief Returns the kind of data to be retrieved from this hardware layer.
     * @return the kind of hardware layer.
     */
    IOLayerType GetType();

    /**
     * @brief Returns the sample size in bytes of the data returned by a query to this model of hardware layer.
     * @return the data width of a sample of this hardware layer.
     */
    uint8 GetSampleSize();
    
    /**
     * @brief Returns the number of channels available on this hardware layer model.
     * @return the number of physical channels available on this hardware layer.
     */
    uint32 GetDeviceChannels();
    
    /**
     * @brief Checks the validity of a specified physical channel to be used on a certain direction.
     * @param[in] channelNumber the channel number to be checked. First channel of a layer is always indexed to 0
     * @param[in] direction the channel direction.
     * @return true if the specified channel is valid within the layer with the direction provided.
     */
    bool CheckChannelAndDirection(uint32 channelNumber, SignalDirection direction);

    /**
     * @brief Getter for the configuration bitfield for a specific channel on this hardware layer.
     * @details This method returns the appropriately formatted bitfield to be used during hardware layer 
     * configuration for a specific channel.
     * @param[in] channelNumber the channel number to be configured. First channel of a layer is always indexed to 0
     * @param[in] channelConfiguration location to where the configuration bitfield must be written.
     * @return true if the specified channel is valid and can be correctly configured into the bitfield.
     */
    bool ConfigureChannel(uint32 channelNumber, uint32 &channelConfiguration);
    
    /**
     * @brief Getter for the configuration bitfield for a specific channel on this hardware layer.
     * @details This method returns the appropriately formatted bitfield to be used during hardware layer 
     * configuration for a specific channel.
     * @param[in] channelNumber the channel number to be configured. First channel of a layer is always indexed to 0
     * @param[in] channelConfiguration location to where the configuration bitfield must be written.
     * @return true if the specified channel is valid and can be correctly configured into the bitfield.
     */
    bool ConfigureChannel(uint32 channelNumber, int32 &channelConfiguration);
    
    /**
     * @brief Configuration method which provides layer-spefcific configuration capabilities.
     * @details This method is used to configure specific functions or parameters on the hardware layer, which are not
     * common to other hardware layers and therefore must be specifically performed for a certain hardware layer
     * @param[in] DAQ_handle handler to the IOM where the device is installed.
     * @return true if the configuration procedure was successful, false otherwise.
     */
    bool ConfigureDevice(int32 DAQ_handle);
    
    /**
     * @brief Checker method to query if a certain signal type is allowed as I/O signal for this hardware layer.
     * @param[in] signalType Type of the I/O signal to be queried.
     * @return true if the type provided is valid to be used as I/O signal type, false otherwise.
     */
    bool AcceptedSignalType(TypeDescriptor signalType);

protected:
    /** 
     * Array containing the gain values for the different channels on this hardware layer
     */
    uint16* gains;
    
    /** 
     * Flag stating the ADC mode to be configured in this specific hardware layer
     */
    int32 ADCMode;
    
    /** 
     * Array of AI217_803FIRBank configuration structures holding the configuration parameters for the different FIR banks
     * for this specific hardware layer.
     */
    AI217_803FIRBank FIRBanks [FIR_BANK_NUMBER];
};

}
#endif /*UEIDIO404_H_*/