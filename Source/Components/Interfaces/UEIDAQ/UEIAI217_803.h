/**
 * @file UEIAI217_803.h
 * @brief Header file for class UEIAI217_803
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

//FIR bank state description
#define BANK_NOT_ENABLED        0u
#define DEFAULT_FIR_SETTING     1u
#define CUSTOM_FIR_SETTING      2u

/**
*   @struct AI217_803FIRBank
*   @brief Structure to store the configuration of the FIR Banks on AI-217-803 hardware layers.
*/
typedef struct{
    /**
    *   Flag indicating the status of the FIR bank, can be set to BANK_NOT_ENABLED, DEFAULT_FIR_SETTING or CUSTOM_FIR_SETTING as defined in UEIAI217_803.h.
    */
    uint8 bankState;
    /**
    *   Variable holding the default fir configuration table index in case the bank is configured in DEFAULT_FIR_SETTING mode.
    */
    uint32 defaultBankSetting;
    /**
    *   Variable holding the number of taps configured for this FIR bank in case the bank is configured in CUSTOM_FIR_SETTING mode.
    */
    uint32 nTaps;
    /**
    *   Array holding the taps for the FIR bank in case it is configured in CUSTOM_FIR_SETTING mode.
    */
    float64* taps;
}AI217_803FIRBank;

/**
 * @brief Device class for UEIDAQ AI-217-803 hardware layer.
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
 * As the AI-217-803 hardware layer is provided with a quad FIR filter stage, this capability can be configured by properly setting the Filters
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
 *      Class               = UEIAI217_803
 *      Devn                = 0
 *      SamplingFrequency  = 1000.0
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
class UEIAI217_803 : public UEIDevice {
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
    bool Initialise(StructuredDataI &data);
    bool ScaleSignal(uint32 channelIdx, uint32 listLength, uint32* rawData, float32* scaledData);
    int32 GetModel();
    uint8 GetType();
    //Returns the number of bytes a single sample of this device occupies
    uint8 GetSampleSize();
    uint32 GetDeviceChannels();
    
    /**
     * @brief Initialise the Object from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity.
     * @param[in] channelNumber the channel number to be checked. First channel of a layer is always 0
     * @param[in] direction the channel direction, definition according to UEIDefinitions.h (INPUT_CHANNEL and OUTPUT_CHANNEL)
     * @return true the specified channel is valid within AI-217-803 layer with the direction provided.
     */
    bool CheckChannelAndDirection(uint32 channelIdx, uint8 direction);
    bool ConfigureChannel(uint32 channelIdx, uint32* channelConfiguration);
    bool ConfigureChannel(uint32 channelIdx, int32* channelConfiguration);
    bool ConfigureDevice(int32 DAQ_handle);
    bool AcceptedSignalType(TypeDescriptor signalType);
    bool GetChannelStatus(int32 DAQ_handle, uint32* errorBitField, uint32* pgaStatusArray);
    
private:
    uint16* gains;                  //Sets the sampling frequency of the device (only one per device)
    int32 ADCMode;                  //Stores the advanced configuration for the ADC on the AI-217-803, can be set to ENHANCED or DEFAULT
    AI217_803FIRBank FIRBanks [FIR_BANK_NUMBER];  //Stores the structure defining the FIR settings for each of the four banks (A,B,C or D, identified by the index)
};

}
#endif /*UEIAI217_803_H_*/