/**
 * @file DAQDevice.h
 * @brief Header file for class DAQDevice
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

 * @details This header file contains the declaration of the class DAQDevice
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DAQDevice_H_
#define DAQDevice_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "StreamString.h"
#include "StructuredDataI.h"
#include "UEIDefinitions.h"
#include "PDNA.h"

namespace MARTe {

class DAQDevice : public Object {
    public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details NOOP.
     */
    DAQDevice();

    /**
     * @brief Default destructor.
     * @details NOOP.
     */
    virtual ~DAQDevice();

    /**
     * @brief Initialise the Object from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity.
     * @return true if every parameter has been read correctly and validated.
     */

    /**
     * @brief Initialise the Object from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity.
     * @return true if every parameter has been read correctly and validated.
     */
    virtual bool Initialise(StructuredDataI &data);

    virtual uint8 GetDevN();
    virtual int32 GetModel();
    virtual void SetHardwareCorrespondence();
    virtual bool GetHardwareCorrespondence();
    virtual void SetMapAssignment();
    virtual bool GetMapAssignment();
    virtual uint32 GetDeviceChannels();
    
    /**
     * @brief Initialise the Object from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity.
     * @param[in] channelNumber the channel number to be checked. First channel of a layer is always 0
     * @param[in] direction the channel direction, definition according to UEIDefinitions.h (INPUT_CHANNEL and OUTPUT_CHANNEL)
     * @return true the specified channel is valid within AI-217-803 layer with the direction provided.
     */
    virtual bool CheckChannelAndDirection(uint32 channelNumber, uint8 direction);
    virtual bool ConfigureChannel(uint32* channel);
    virtual bool ConfigureDevice(int32 DAQ_handle);
    virtual float GetSamplingFrequency();
    virtual bool AcceptedSignalType(TypeDescriptor signalType);
    virtual bool GetChannelStatus(int32 DAQ_handle, uint32* errorBitField, uint32* pgaStatusArray);

private:
    StreamString name;              //name of the device object
    uint8 deviceId;                 //devn
    float samplingFrequency;        //Sets the sampling frequency of the device (only one per device)
    bool hardwareCorrespondence;    //Signals if this device has been identified to an installed layer
    bool assignedToMap;             //Signals if this device has been assigned to a DAQ map
};

}
#endif /*DAQDevice_H_*/