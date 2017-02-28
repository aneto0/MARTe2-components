/**
 * @file NI6368DIO.h
 * @brief Header file for class NI6368DIO
 * @date 30/01/2017
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

 * @details This header file contains the declaration of the class NI6368DIO
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6368_NI6368DIO_H_
#define NI6368_NI6368DIO_H_

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
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A DataSource which provides a digital input/output interface to the NI6368 boards.
 * @details The configuration syntax is (names are only given as an example):
 *
 * +NI6368_0_DIO = {
 *     Class = NI6368::NI6368DIO
 *     DeviceName = "/dev/pxi6368" //Mandatory
 *     BoardId = 0 //Mandatory
 *     ClockSampleSource = "INTERNAL" //Mandatory. Sampling clock source. Possible values: INTERNAL, PFI0, ..., PFI15, RTSI0, ..., RTSI6, DIO_CHGDETECT, G0_OUT, ..., G3_OUT, STAR_TRIGGER, LOW, PXIE_DSTARA, ATRIG, PXIE_DSTARB, G0_SAMPLECLK, ..., G3_SAMPLECLK, AI_CONVERT, AO_UPDATE, DO_UPDATE, INTTRIGGERA0, ..., INTTRIGGERA7
 *     ClockSamplePolarity = "ACTIVE_HIGH_OR_RISING_EDGE" //Mandatory. Sampling clock polarity. Possible values: ACTIVE_HIGH_OR_RISING_EDGE, ACTIVE_LOW_OR_FALLING_EDGE
 *     ClockConvertSource = "INTERNAL" //Mandatory. Convert clock source. Possible values: INTERNALTIMING, PFI0, ..., PFI15, RTSI0, ..., RTSI6, DIO_CHGDETECT, G0_OUT, ..., G3_OUT, STAR_TRIGGER, LOW, PXIE_DSTARA, ATRIG, PXIE_DSTARB, G0_SAMPLECLK, ..., G3_SAMPLECLK, AI_CONVERT, AO_UPDATE, DO_UPDATE, INTTRIGGERA0, ..., INTTRIGGERA7
 *     ScanIntervalCounterSource = "COUNTER_TB3" //Mandatory. Scan interval clock source. Possible values: COUNTER_TB1, COUNTER_TB2, COUNTER_TB3, PFI0, ..., PFI15, RTSI0, ..., RTSI6,PXI_CLK0, STAR_TRIGGER, ANALOG_TRIGGER, D_STARA, D_STARB, STAR_TRIGGER
 *     ScanIntervalCounterPeriod = 50 //Mandatory. Period of the scan interval.
 *     ScanIntervalCounterDelay = 2 //Mandatory. Minimum delay after the start trigger.
 *     StartTriggerSource = "SW_PULSE" //Mandatory. Start trigger source. Possible values: SW_PULSE, PFI0, ..., PFI15, RTSI0, ..., RTSI7, AI_START1, AI_START2, STAR_TRIGGER, PXIE_DSTARA, PXIE_DSTARB, ANALOG_TRIGGER, LOW, G0_OUT, ..., G3_OUT, DIO_CHGDETECT, DI_START1, DI_START2, INTTRIGGERA0, ..., INTTRIGGERA7, FIFOCONDITION
 *     UpdateCounterSource = "UI_TC" //Mandatory. Possible values: UI_TC, PFI0, PFI1, PFI2, PFI3, PFI4, PFI5, PFI6, PFI7, PFI8, PFI9, RTSI0, RTSI1, RTSI2, RTSI3, RTSI4, RTSI5, RTSI6, G0_OUT, G1_OUT, STAR_TRIGGER, PFI10, PFI11, PFI12, PFI13, PFI14, PFI15, RTSI7, G2_OUT, G3_OUT, ANALOG_TRIGGER, LOW, PXIE_DSTARA, PXIE_DSTARB, DIO_CHGDETECT, G0_SAMPLECLK, G1_SAMPLECLK, G2_SAMPLECLK, G3_SAMPLECLK, AI_CONVERT, AI_START, DI_CONVERT, INTTRIGGERA0, INTTRIGGERA1, INTTRIGGERA2, INTTRIGGERA3, INTTRIGGERA4, INTTRIGGERA5, INTTRIGGERA6, INTTRIGGERA7
 *     UpdateCounterPolarity = "RISING_EDGE" //Mandatory. Update counter polarity.  RISING_EDGE, FALLING_EDGE
 *     UpdateIntervalCounterSource = "TB3" //Mandatory. Possible values:TB3, PFI0, PFI1, PFI2, PFI3, PFI4, PFI5, PFI6, PFI7, PFI8, PFI9, RTSI0, RTSI1, RTSI2, RTSI3, RTSI4, RTSI5, RTSI6, DSTARA, TB2, STAR_TRIGGER, PFI10, PFI11, PFI12, PFI13, PFI14, PFI15, RTSI7, TB1, PXI_CLK10, ANALOG_TRIGGER, DSTARB
 *     UpdateIntervalCounterPolarity = "RISING_EDGE" //Mandatory. Possible values: RISING_EDGE, FALLING_EDGE
 *     UpdateIntervalCounterDivisor = 100000 //Mandatory > 0
 *     UpdateIntervalCounterDelay = 2 //Mandatory > 0
 *     Signals = {
 *         DIO0_0 = {
 *             Type = uint32 //Mandatory. Only type that is supported.
 *             InputPortMask = 0x1 //Mandatory. A mask where each bit defines if the pin is an input (bit=0) or an output (bit=1).
 *             OutputPortMask = 0x1 //Mandatory. A mask where each bit defines if the pin is an input (bit=0) or an output (bit=1).
 *         }
 *     }
 * }
 *
 */

class NI6368DIO: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor.
     * @details Initialises all the parameters described in the class description.
     */
NI6368DIO    ();

    /**
     * @brief Destructor.
     * @details Closes all the file descriptors associated to the board, including any opened ports.
     */
    virtual ~NI6368DIO();

    /**
     * @brief See DataSourceI::AllocateMemory.
     * @return true.
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief See DataSourceI::GetBrokerName.
     * @return For OutputSignals: MemoryMapSynchronisedOutputBroker.
     * For InputSignals: MemoryMapSynchronisedInputBroker.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details Adds a MemoryMapSynchronisedInputBroker instance to the inputBrokers.
     * @param[out] inputBrokers where the BrokerI instances have to be added to.
     * @param[in] functionName name of the function being queried.
     * @param[in] gamMemPtr the GAM memory where the signals will be read from.
     * @return true if the inputBrokers can be successfully configured.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @details Adds a MemoryMapSynchronisedOutputBroker instance to the inputBrokers.
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
     * @return true if all the mandatory parameters are correctly specified.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Final verification of all the parameters and setup of the board configuration.
     * @details This method verifies that all the parameters (e.g. number of samples) requested by the GAMs interacting with this DataSource
     *  are valid and consistent with the board parameters set during the initialisation phase.
     * In particular the following conditions shall be met:
     * - The DIO channel has type uint32.
     * - The number of samples of the DIO channel is exactly one.
     * - The number of elements of the DIO channel is exactly one.
     * @return true if all the parameters are valid and consistent with the board parameters and if the board can be successfully configured with
     *  these parameters.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @details Writes the value of all the DIO channels to the board, followed by a read of all the DIO channels from the board.
     * @return true if the reading and writing of all the channels is successful.
     */
    virtual bool Synchronise();

    /**
     * @brief Helper function to test the class.
     * @details This method is meant to be used by the unit-test class in order to verify the correct setting of the board parameters.
     * No real added value on making getters for all the structure elements, just for the sake of testing.
     * @return true if the board was opened (i.e. if SetConfiguredDatabase was successful).
     */
    bool ReadDIOConfiguration(xseries_di_conf_t *confDI, xseries_do_conf_t *confDO) const;
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
     * The board file descriptor
     */
    int32 boardFileDescriptor;

    /**
     * The input mask of the DIO port
     */
    uint32 inputPortMask;

    /**
     * The output mask of the DIO port
     */
    uint32 outputPortMask;

    /**
     * The current port value.
     */
    uint32 portValue;

    /**
     * True if this board is used to write digital values.
     */
    bool outputsEnabled;

    /**
     * True if this board is used to read digital values.
     */
    bool inputsEnabled;

    /**
     * The DI sampling clock source;
     */
    xseries_di_sample_convert_clock_t clockSampleSource;

    /**
     * The DI sampling clock polarity;
     */
    xseries_di_polarity_t clockSamplePolarity;

    /**
     * The DI scan interval counter source
     */
    xseries_scan_interval_counter_t scanIntervalCounterSource;

    /**
     * The DI interval counter period
     */
    uint32 scanIntervalCounterPeriod;

    /**
     * The DI interval counter delay from the trigger
     */
    uint32 scanIntervalCounterDelay;

    /**
     * The start trigger source
     */
    xseries_do_start_trigger_t startTriggerSource;

    /**
     * The start trigger polarity
     */
    xseries_do_polarity_t startTriggerPolarity;

    /**
     * The update counter source
     */
    xseries_do_update_counter_t updateCounterSource;

    /**
     * The update counter polarity
     */
    xseries_do_polarity_t updateCounterPolarity;

    /**
     * The update interval counter source
     */
    xseries_outtimer_update_interval_counter_t updateIntervalCounterSource;

    /**
     * The update interval counter source
     */
    xseries_outtimer_polarity_t updateIntervalCounterPolarity;

    /**
     * The update interval counter period divisor
     */
    uint32 updateIntervalCounterPeriodDivisor;

    /**
     * The update interval counter delay
     */
    uint8 updateIntervalCounterDelay;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6368_NI6368DIO_H_ */

