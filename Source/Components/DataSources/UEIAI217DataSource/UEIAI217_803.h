/**
 * @file UEIAI217_803.h
 * @brief Header file for class UEIAI217_803
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

 * @details This header file contains the declaration of the class UEIAI217_803
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIAI217_803_H_
#define UEIAI217_803_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "StreamString.h"
#include "StructuredDataI.h"
#include "StructuredDataIHelper.h"
#include "PDNA.h"
#include "powerdna.h"


 /*
*       + dev0={
*           Class               = UEIAI217_803
*           Devn                = 0
*           Sampling_ferquency  = 1000.0     //in Hz
*           Filters = {         //This Structure is optional, only those banks specified will be activated (either with default FIR settings or
*                               // specified FIR by taps vector).
*               A = {
*                   Taps = {0,1,0,2,0,3,0,4,0,5,6}
*               }
*               B = {
*                   Default_filter = 0
*               }
*               C = {
*                   Default_filter = 1
*               }
*               D = {
*                   Default_filter = 2
*               }
*           }
*       }
 */

#define MAX_FIR_TAPS_803 512u
#define FIR_BANK_NUMBER 4u
#define CHANNEL_NUMBER 16u

//FIR bank state description
#define BANK_NOT_ENABLED        0u
#define DEFAULT_FIR_SETTING     1u
#define CUSTOM_FIR_SETTING      2u

namespace MARTe {

typedef struct{
    uint8 bankState;
    uint32 defaultBankSetting;
    uint32 nTaps;
    float64* taps;
}AI217_803FIRBank;

class UEIAI217_803 : public Object {
    public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details NOOP.
     */
    UEIAI217_803();

    /**
     * @brief Default destructor.
     * @details NOOP.
     */
    virtual ~UEIAI217_803();

    /**
     * @brief Initialise the Object from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity.
     * @return true if every parameter has been read correctly and validated.
     */
    virtual bool Initialise(StructuredDataI &data);

    uint8 GetDevN();
    int32 GetModel();
    void SetHardwareCorrespondence();
    bool GetHardwareCorrespondence();
    void SetMapAssignment();
    bool GetMapAssignment();
    uint32 GetDeviceChannels();
    
    /**
     * @brief Initialise the Object from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity.
     * @param[in] channelNumber the channel number to be checked. First channel of a layer is always 0
     * @param[in] direction the channel direction, definition according to UEIDefinitions.h (INPUT_CHANNEL and OUTPUT_CHANNEL)
     * @return true the specified channel is valid within AI-217-803 layer with the direction provided.
     */
    bool CheckChannelAndDirection(uint32 channelNumber, uint8 direction);
    bool ConfigureChannel(uint32* channel);
    bool ConfigureDevice(int32 DAQ_handle);
    float GetSamplingFrequency();
    bool AcceptedSignalType(TypeDescriptor signalType);
    bool GetChannelStatus(int32 DAQ_handle, uint32* errorBitField, uint32* pgaStatusArray);
    
private:
    StreamString name;              //name of the device object
    uint8 deviceId;                 //devn
    float samplingFrequency;        //Sets the sampling frequency of the device (only one per device)
    uint16* gains;                  //Sets the sampling frequency of the device (only one per device)
    bool hardwareCorrespondence;    //Signals if this device has been identified to an installed layer
    bool assignedToMap;             //Signals if this device has been assigned to a DAQ map
    int32 ADCMode;                  //Stores the advanced configuration for the ADC on the AI-217-803, can be set to ENHANCED or DEFAULT
    AI217_803FIRBank FIRBanks [FIR_BANK_NUMBER];  //Stores the structure defining the FIR settings for each of the four banks (A,B,C or D, identified by the index)
};

}
#endif /*UEIAI217_803_H_*/