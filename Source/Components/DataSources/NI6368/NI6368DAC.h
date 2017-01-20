/**
 * @file NI6368DAC.h
 * @brief Header file for class NI6368DAC
 * @date 20/01/2017
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class NI6368DAC
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6368_NI6368DAC_H_
#define NI6368_NI6368DAC_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "xseries-lib.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
//Number of DAC channels
const uint32 NI6368DAC_MAX_CHANNELS = 4u;
/**
 * TODO check configuration
 * @brief A DataSource which provides an analogue output interface to the NI6368 boards.
 * @details The configuration syntax is (names are only given as an example):
 * +NI6368_0_DAC = {
 *     Class = NI6368::NI6368DAC
 *     DeviceName = "/dev/pxi6368" //Mandatory
 *     BoardId = 0 //Mandatory
 *     ClockUpdateSource = "UI_TC" //Mandatory. Update clock source. Possible values:UI_TC, PFI0, ..., PFI15, RTSI0, ..., RTSI7, GPCRT0_OUT, STAR_TRIGGER, GPCTR1_OUT, ANALOG_TRIGGER, LOW
 *     ClockUpdatePolarity = "RISING_EDGE" //Mandatory. Possible values: RISING_EDGE, FALLING_EDGE
 *     ClockUpdateDivisor = 10 //Optional. Default value = 10. Only meaningful if ClockUpdateSource == UI_TC.
 *     Signals = {
 *         DAC0_0 = {
 *             Type = float32 //Mandatory. Only type that is supported.
 *             ChannelId = 0 //Mandatory. The channel number.
 *             OutputPolarity = Bipolar //Optional. Possible values: Bipolar, Unipolar. Default value Unipolar.
 *         }
 *     }
 * }
 *
 * Note that at least one of the GAMs writing to this DataSource must have set one of the signals with Trigger=1 (which forces the writing of all the signals to the DAC).
 * The clock configuration is fixed to AO_UPDATE_SOURCE_SELECT_UI_TC and AO_UPDATE_SOURCE_POLARITY_RISING_EDGE.
 */
class NI6368DAC: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor.
     * @details Initialises all the optional parameters as described in the class description.
     */
    NI6368DAC();

    /**
     * @brief Destructor.
     * @details Closes all the file descriptors associated to the board, including any opened channels.
     */
    virtual ~NI6368DAC();

    /**
     * @brief See DataSourceI::AllocateMemory. NOOP.
     * @return true.
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetSignalMemoryBuffer.
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @details Only OutputSignals are supported.
     * @return MemoryMapSynchronisedOutputBroker if Trigger == 1 for any of the signals, MemoryMapOutputBroker otherwise.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @return false.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @details If the functionName is one of the functions which requested a Trigger,
     * it adds a MemoryMapSynchronisedOutputBroker instance to the outputBrokers,
     * otherwise it adds a MemoryMapOutputBroker instance to the outputBrokers.
     * @param[out] outputBrokers where the BrokerI instances have to be added to.
     * @param[in] functionName name of the function being queried.
     * @param[in] gamMemPtr the GAM memory where the signals will be read from.
     * @return true if the outputBrokers can be successfully configured.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See StatefulI::PrepareNextState.
     * @details NOOP.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Loads and verifies the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Final verification of all the parameters and setup of the board configuration.
     * @details This method verifies that all the parameters (e.g. number of samples) requested by the GAMs interacting with this DataSource
     *  are valid and consistent with the board parameters set during the initialisation phase.
     * In particular the following conditions shall be met:
     * - At least one triggering signal was requested by a GAM (with the property Trigger = 1)
     * - All the DAC channels have type float32.
     * - The number of samples of all the DAC channels is exactly one.
     * @return true if all the parameters are valid and consistent with the board parameters and if the board can be successfully configured with
     *  these parameters.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @details Writes the value of all the DAC channels to the board.
     * @return true if the writing of all the channels is successful.
     */
    virtual bool Synchronise();

    /**
     * @brief Helper function to test the class.
     * @details This method is meant to be used by the unit-test class in order to verify the correct setting of the board parameters.
     * No real added value on making getters for all the structure elements, just for the sake of testing.
     * @return true if the board was opened (i.e. if SetConfiguredDatabase was successful).
     */
    bool ReadAOConfiguration(xseries_ao_conf_t *conf) const;

private:
    /**
     * The board identifier
     */
    uint32 boardId;

    /**
     * The board device name
     */
    StreamString deviceName;

    /**
     * The DAC update clock source;
     */
    //ao_update_source_select_t clockUpdateSource;

    /**
     * The DAC update clock polarity;
     */
    //ao_update_source_polarity_t clockUpdatePolarity;

    /**
     * The DAC clock update divisor
     */
    //uint32 clockUpdateDivisor;

    /**
     * The board individual channel polarities
     */
    //ao_dac_polarity_t outputPolarity[NI6368DAC_MAX_CHANNELS];

    /**
     * The board individual channel output ranges
     */
    xseries_output_range_t outputRange[NI6368DAC_MAX_CHANNELS];

    /**
     * The number of samples
     */
    uint32 numberOfSamples;

    /**
     * The board file descriptor
     */
    int32 boardFileDescriptor;

    /**
     * The channel file descriptors
     */
    int32 channelsFileDescriptors[NI6368DAC_MAX_CHANNELS];

    /**
     * The signal memory
     */
    float32 *channelsMemory[NI6368DAC_MAX_CHANNELS];

    /**
     * The DACs that are enabled
     */
    bool dacEnabled[NI6368DAC_MAX_CHANNELS];

    /**
     * The number of enabled DACs
     */
    uint32 numberOfDACsEnabled;

    /**
     * True if at least one trigger was set.
     */
    bool triggerSet;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6368_NI6368DAC_H_ */

