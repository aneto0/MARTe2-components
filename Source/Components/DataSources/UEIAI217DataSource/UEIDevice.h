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
 * <pre>
 *
 * <\pre>
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



    virtual uint8 GetDevN();
    virtual int32 GetModel();
    virtual uint8 GetType();
    //Returns the number of bytes a single sample of this device occupies
    virtual uint8 GetSampleSize();
    virtual uint32 GetDeviceChannels();
    virtual bool CheckChannelAndDirection(uint32 channelNumber, uint8 direction);
    virtual bool ConfigureChannel(uint32* channel);
    virtual bool ConfigureChannel(int32* channel);
    virtual bool ConfigureDevice(int32 DAQ_handle);    
    virtual bool AcceptedSignalType(TypeDescriptor signalType);
    virtual bool GetChannelStatus(int32 DAQ_handle, uint32* errorBitField, uint32* pgaStatusArray);

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