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
/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include "xseries-lib.h"
/*lint -D__cplusplus */

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/*lint -e551 -e1923 -e767 -e91 allow macros and large lines for comments*/
/**
 * Number of DAC channels
 */
const uint32 NI6368DAC_MAX_CHANNELS = 4u;
/**
 * @brief A DataSource which provides an analogue output interface to the NI6368 boards.
 *
 * @details It is possible to define one optional signal which must be declared before the channels.
 *   - Signal 0 (Status): the board status.
 * The board status is a uint32 bitmask. Follows the error description for each bit:
 *   0: ADC Overrun detected in STC3 (Intimer)
 *   1: AI FIFO overflow detected in STC3 (Intimer)
 *   2: AI FIFO overflow detected in Simultaneous Control
 *   4: AI DMA channel error happened, the channel shall be restarted
 *   5: DI Overrun error happened
 *   6: DI Overflow error happened
 *   8: DI DMA channel error happend, the channel shall be restarted
 *   10: AO Overrun error detected
 *   11: AO Underflow error detected
 *   12: AO write too fast error detected
 *   13: AO DMA channel error happend, the channel shall be restarted
 *   14: DO Overrun error detected
 *   15: DO Underflow error detected
 *   16: DO Write too fast detected
 *   17: DO DMA channel error happend, the channel shall be restarted
 *   20: GPC0 DMA channel error happend, the channel shall be restarted
 *   21: GPC1 DMA channel error happend, the channel shall be restarted
 *   22: GPC2 DMA channel error happend, the channel shall be restarted
 *   23: GPC3 DMA channel error happend, the channel shall be restarted
 *
 * @details The configuration syntax is (names are only given as an example):
 *
 * <pre>
 * +NI6368_0_DAC = {
 *
 *     // Basic Configuration
 *
 *     Class = NI6368::NI6368DAC
 *     DeviceName = "/dev/pxie-6363"
 *     BoardId = 0
 *     IsMultiplexed = 1 //Default is 0 for 6368. Set to 1 if using 6363.
 *     SamplingFrequency = 1000
 *
 *     // Advanced Configuration
 *
 *     BaseSampleClockFrequency = 100000000 //Default is 100 MHz
 *     StartTriggerSource = "SW_PULSE" //Mandatory. Start trigger source. Possible values: SW_PULSE, PFI0, ..., PFI15, RTSI0, ..., RTSI7, AI_START1, AI_START2, STAR_TRIGGER, PXIE_DSTARA, PXIE_DSTARB, ANALOG_TRIGGER, LOW, G0_OUT, ..., G3_OUT, DIO_CHGDETECT, DI_START1, DI_START2, DO_START1, INTTRIGGERA0, ..., INTTRIGGERA7, FIFOCONDITION
 *     BaseClockUpdateSource (or UpdateCounterSource) = "UI_TC" //Optional (default UI_TC). Possible values: UI_TC, PFI0, PFI1, PFI2, PFI3, PFI4, PFI5, PFI6, PFI7, PFI8, PFI9, RTSI0, RTSI1, RTSI2, RTSI3, RTSI4, RTSI5, RTSI6, G0_OUT, G1_OUT, STAR_TRIGGER, PFI10, PFI11, PFI12, PFI13, PFI14, PFI15, RTSI7, G2_OUT, G3_OUT, ANALOG_TRIGGER, LOW, PXIE_DSTARA, PXIE_DSTARB, DIO_CHGDETECT, G0_SAMPLECLK, G1_SAMPLECLK, G2_SAMPLECLK, G3_SAMPLECLK, AI_CONVERT, AI_START, DI_CONVERT, DO_UPDATE, INTTRIGGERA0, INTTRIGGERA1, INTTRIGGERA2, INTTRIGGERA3, INTTRIGGERA4, INTTRIGGERA5, INTTRIGGERA6, INTTRIGGERA7, AUTOUPDATE
 *     BaseClockUpdatePolarity (or UpdateCounterPolarity) = "RISING_EDGE" //Optional (default RISING_EDGE). Update counter polarity.  RISING_EDGE, FALLING_EDGE
 *     ClockUpdateSource (or UpdateIntervalCounterSource) = "TB3" //Optional (default TB3). Possible values:TB3, PFI0, PFI1, PFI2, PFI3, PFI4, PFI5, PFI6, PFI7, PFI8, PFI9, RTSI0, RTSI1, RTSI2, RTSI3, RTSI4, RTSI5, RTSI6, DSTARA, TB2, STAR_TRIGGER, PFI10, PFI11, PFI12, PFI13, PFI14, PFI15, RTSI7, TB1, PXI_CLK10, ANALOG_TRIGGER, DSTARB
 *     ClockUpdatePolarity (or UpdateIntervalCounterPolarity) = "RISING_EDGE" //Optional (default RISING_EDGE). Possible values: RISING_EDGE, FALLING_EDGE
 *     ClockUpdateDivisor (or UpdateIntervalCounterDivisor) = 100000 //Optional divisor of BaseSampleClockFrequency. To be defined as alternative of SamplingFrequency
 *     ClockUpdateDelay (or UpdateIntervalCounterDelay) = 2 //Mandatory > 0
 *     Signals = {
 *         Status = { //Optional. The board status
 *             Type = uint32
 *         }
 *         DAC0_0 = {
 *             Type = float32 //Mandatory. Only type that is supported.
 *             ChannelId = 0 //Mandatory. The channel number.
 *             OutputRange = 10 //Mandatory. Possible values: 10, 5, APFI0, APFI1
 *         }
 *     }
 * }
 * </pre>
 * Note that at least one of the GAMs writing to this DataSource must have set one of the signals with Trigger=1 (which forces the writing of all the signals to the DAC).
 */
/*lint +e551 +e1923 +e767 +e91*/
class NI6368DAC: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor.
     * @details Initialises all the optional parameters as described in the class description.
     */
NI6368DAC    ();

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
     * - The number of samples of all the DAC channels is the same.
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
     * The start trigger source
     */
    xseries_ao_start_trigger_t startTriggerSource;

    /**
     * The start trigger polarity
     */
    xseries_ao_polarity_t startTriggerPolarity;

    /**
     * The update counter source
     */
    xseries_ao_update_counter_t updateCounterSource;

    /**
     * The update counter polarity
     */
    xseries_ao_polarity_t updateCounterPolarity;

    /**
     * The update interval counter source
     */
    xseries_outtimer_update_interval_counter_t clockUpdateSource;

    /**
     * The update interval counter source
     */
    xseries_outtimer_polarity_t clockUpdatePolarity;

    /**
     * The base sample clock frequency
     */
    uint32 baseSampleClockFrequency;

    /**
     * The update interval counter period divisor
     */
    uint32 clockUpdateDivisor;

    /**
     * The update interval counter delay
     */
    uint8 clockUpdateDelay;

    /**
     * The board individual channel output ranges
     */
    xseries_output_range_t outputRange[NI6368DAC_MAX_CHANNELS];

    /**
     * The output gain
     */
    float32 outputGain[NI6368DAC_MAX_CHANNELS];

    /**
     * The output offset
     */
    float32 outputOffset[NI6368DAC_MAX_CHANNELS];

    /**
     * The board file descriptor
     */
    int32 boardFileDescriptor;

    /**
     * The board status file descriptor
     */
    int32 rootFileDescriptor;

    /**
     * The channel file descriptors
     */
    int32 channelsFileDescriptors[NI6368DAC_MAX_CHANNELS];

    /**
     * The signal memory
     */
    float32 *channelsMemory[NI6368DAC_MAX_CHANNELS];

    /**
     * The number of elements to write
     */
    uint32 numberOfSamplesT;

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

    /**
     * If multiplexed
     */
    uint8 multiplexed;

    /**
     * The number of signals in header
     */
    uint32 headerSize;

    /**
     * The board status
     */
    uint32 boardStatus;


};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6368_NI6368DAC_H_ */

