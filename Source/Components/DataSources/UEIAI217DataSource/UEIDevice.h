/**
 * @file UEIDevice.h
 * @brief Header file for class UEIDevice
 * @date 09/02/2023
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
//Interface specific includes
#include "UEIDefinitions.h"
//PowerDNA library include
#include "PDNA.h"

namespace MARTe {

/**
 * @brief Base class for UEIDAQ hardware layer class implementation.
 *
 * @details This class provides base parameters and checks for any hardware layer class of UEIDAQ system. Any 
 * layer specific implmementation clas must derive from this base class as it does provide generic interface from
 * MapContainer and MasterObject perspective, generalising the access to different UEIDAQ hardware layers.
 * 
 * The functions contained in this class must be reimplemented in the child class with the appropriate methods
 * for the target hardware layer.
 *
 * This class should not be instantiated as a device object but rather the derived classes from this base class.
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
     * Set as virtual so the child class (implementing the specific hardware layer)
     * can call its Initialise method. This method should be called within the child class
     * Initialise method.
     * @return true if every parameter has been read correctly and validated.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Getter for the device model number.
     * @details This method returns the device model number. Implementation must be provided in each
     * of the device-specific child classes for its specific model number.
     * @return 0x00 (invalid model number).
     */
    virtual int32 GetModel();
    
    /**
     * @brief Getter for the device type (hardware layer interface type).
     * @details This method returns the device type. Implementation must be provided in each
     * of the device-specific child classes for its specific device type.
     * @return HARDWARE_LAYER_UNDEFINED.
     */
    virtual uint8 GetType();
    
    /**
     * @brief Getter for the device sample size (sample size in bytes).
     * @details This method returns the device sample size. Implementation must be provided in each
     * of the device-specific child classes for its specific sample size.
     * @return 0
     */
    virtual uint8 GetSampleSize();
    
    /**
     * @brief Getter for the number of channels of the device.
     * @details This method returns the number of channels of the device. Implementation must be provided in each
     * of the device-specific child classes for its specific number of channels.
     * @return 0
     */
    virtual uint32 GetDeviceChannels();
    
    /**
     * @brief Method to check if a channel is a valid to be used in a certain direction (input or output channel).
     * @details This method checks if channel number provided (first channel is indexed to 0) is valid within this device
     * in the specified direction (input or output channel).
     * Implementation of this method must be provided in each of the device-specific child classes for their specific I/O characteristics.
     * @param[in] channelNumber channel index (first channel indexes to 0).
     * @param[in] direction direction for the specified channel query (see UEIDefinitions.h for reference)
     * @return false. (Must return true if specified channel and direction are valid, false otherwise).
     */
    virtual bool CheckChannelAndDirection(uint32 channelIdx, uint8 direction);
    
    /**
     * @brief Method to configure a channel in the specific configuration scheme of the device.
     * @details This method returns the configuration bitfield for the specified channel in the device.
     * Implementation of this method must be provided in each of the device-specific child classes for their specific configuration flags.
     * @param[in] channelIdx channel index (first channel indexes to 0).
     * @param[out] channelConfiguration pointer to channel configuration bitfield for the specified channel in the device. 
     * @return false (true if the channel is valid and information for configuration can be retrieved, false otherwise).
     */
    virtual bool ConfigureChannel(uint32 channelIdx, uint32* channelConfiguration);
    
    /**
     * @brief Method to configure a channel in the specific configuration scheme of the device.
     * @details This method returns the configuration bitfield for the specified channel in the device.
     * Implementation of this method must be provided in each of the device-specific child classes for their specific configuration flags.
     * @param[in] channelIdx channel index (first channel indexes to 0).
     * @param[out] channelConfiguration pointer to channel configuration bitfield for the specified channel in the device. 
     * @return false (true if the channel is valid and information for configuration can be retrieved, false otherwise).
     */
    virtual bool ConfigureChannel(uint32 channelIdx, int32* channelConfiguration);
    
    /**
     * @brief Method to configure the device.
     * @details This method performs the required configuration and initialisation procedures for the specific device (hardware layer).
     * Implementation of this method must be provided in each of the device-specific child classes for their specific configuration procedures.
     * @param[in] DAQ_handle IOM handle provided by UEIMasterObject to issue initialisation commands to the device.
     * @return false (true if configuration procedure for the device succeeded, false otherwise).
     */
    virtual bool ConfigureDevice(int32 DAQ_handle);

    /**
     * @brief Method to check if a signal type is accepted as input/output type.
     * @details This method checks if the signal type provided is a valid type for input/output operations for the specific device (hardware layer).
     * Implementation of this method must be provided in each of the device-specific child classes for their specific type characteristics.
     * @param[in] signalType the type which is being queried as input/output type.
     * @return false (true if the signal type provided is accepted as input/output type, false otherwise).
     */
    virtual bool AcceptedSignalType(TypeDescriptor signalType);

    /**
     * @brief Method to check channel status.
     * @details This method performs checks on the channels defined for the device (if available by hardware) and reports its status.
     * Implementation of this method must be provided in each of the device-specific child classes for their specific channel diagnostics capabilities.
     * @param[in] DAQ_handle IOM handle for the current device UEIDAQ system.
     * @param[out] errorBitField pointer to array of bit fields stating the current status of each of the channels on the device (refer to
     * device specific documentation for interpretation on such bitfield values).
     * @param[out] pgaStatusArray TODO.
     * @return false (true if the diagnostics checks were performed successfully, false otherwise).
     */
    virtual bool GetChannelStatus(int32 DAQ_handle, uint32* errorBitField, uint32* pgaStatusArray);

    virtual bool ScaleSignal(uint32 channelIdx, uint32 listLength, uint32* rawData, float32* scaledData);
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
     * Implementation must be only done in this base class and not in child classes, as this behavior is 
     * not device-dependant.
     * @return true if the hardwareCorrespondence flag was previously false and therefore the device has not been previously set, false otherwise.
     */
    bool SetHardwareCorrespondence();
    
    /**
     * @brief Getter for hardwareCorrespondence flag.
     * @details This method returns the value of the hardware correspondence flag.
     * Implementation must be only done in this base class and not in child classes, as this behavior is 
     * not device-dependant.
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

protected:
    /**
    *   Variable holding the device name
    */
    StreamString name;

    /**
    *   Variable the device Id (devn)
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
};

}
#endif /*UEIDevice_H_*/