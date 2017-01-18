/**
 * @file NI6368ADC.h
 * @brief Header file for class NI6368ADC
 * @date 12/01/2017
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

 * @details This header file contains the declaration of the class NI6368ADC
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6368_NI6368ADC_H_
#define NI6368_NI6368ADC_H_

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
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
//Number of ADC channels
const uint32 NI6368ADC_MAX_CHANNELS = 16u;
//Counter and timer
const uint32 NI6368ADC_HEADER_SIZE = 2u;
//Sampling frequency
const uint32 NI6368ADC_SAMPLING_FREQUENCY = 2000000u;
/**
 * @brief A DataSource which provides an input interface to the NI6368 boards.
 *
 * The configuration syntax is (names are only given as an example):
 * +NI6368_0 = {
 *     Class = NI6368::NI6368ADC
 *     DeviceName = "/dev/pxie-6368" //Mandatory
 *     BoardId = 0 //Mandatory
 *     DMABufferSize = 1000 //Mandatory. DMA size in bytes > 0.
 *     ClockSampleSource = "INTERNALTIMING" //Mandatory. Sampling clock source. Possible values: INTERNALTIMING, PFI0, ..., PFI15, RTSI0, ..., RTSI6, DIO_CHGDETECT, G0_OUT, ..., G3_OUT, STAR_TRIGGER, SCXI_TRIG1, LOW, PXIE_DSTARA, ATRIG, PXIE_DSTARB, G0_SAMPLECLK, ..., G3_SAMPLECLK, DI_CONVERT, AO_UPDATE, DO_UPDATE, INTTRIGGERA0, ..., INTTRIGGERA7
 *     ClockSamplePolarity = "ACTIVE_HIGH_OR_RISING_EDGE" //Mandatory. Sampling clock polarity. Possible values: ACTIVE_HIGH_OR_RISING_EDGE, ACTIVE_LOW_OR_FALLING_EDGE
 *     ClockConvertSource = "INTERNALTIMING" //Mandatory. Convert clock source. Possible values: INTERNALTIMING, PFI0, ..., PFI15, RTSI0, ..., RTSI6, DIO_CHGDETECT, G0_OUT, ..., G3_OUT, STAR_TRIGGER, SCXI_TRIG1, LOW, PXIE_DSTARA, ATRIG, PXIE_DSTARB, G0_SAMPLECLK, ..., G3_SAMPLECLK, DI_CONVERT, AO_UPDATE, DO_UPDATE, INTTRIGGERA0, ..., INTTRIGGERA7
 *     ClockConvertPolarity = "ACTIVE_HIGH_OR_RISING_EDGE" //Mandatory. Convert clock polarity. Possible values: ACTIVE_HIGH_OR_RISING_EDGE, ACTIVE_LOW_OR_FALLING_EDGE
 *     ScanIntervalCounterSource = "COUNTER_TB3" //Mandatory. Scan interval clock source. Possible values: COUNTER_TB1, COUNTER_TB2, COUNTER_TB3, PFI0, ..., PFI15, RTSI0, ..., RTSI6,PXI_CLK0, STAR_TRIGGER, ANALOG_TRIGGER, D_STARA, D_STARB, STAR_TRIGGER
 *     ScanIntervalCounterPolarity = "RISING_EDGE" //Mandatory. Convert clock polarity. Possible values: RISING_EDGE, FALLING_EDGE
 *     ScanIntervalCounterPeriod = 50 //Mandatory. Period of the scan interval.
 *     ScanIntervalCounterDelay = 2 //Mandatory. Minimum delay after the start trigger.
 *     CPUs = 0xf //CPUs where the thread which reads data from the board is allowed to run on.
 *     Signals = {
 *          Counter = { //Mandatory. Number of ticks since last state change.
 *              Type = uint32 //int32 also supported.
 *          }
 *          Time = { //Mandatory. Elapsed time in micro-seconds since last state change.
 *               Type = uint32 //int32 also supported.
 *          }
 *          ADC0_0 = { //At least one ADC input shall be specified.
 *              InputRange = 10 //Optional. Possible values: 0.1, 0.2, 0.5, 1, 2, 5, 10. Default value 10.
 *              Type = uint16 //Mandatory. Only the uint16 type is supported.
 *              ChannelId = 0 //Mandatory. The channel number.
 *              InputType = RSE //Optional. Possible values: Differential, RSE, NRSE, Loopback, Internal. Default value RSE.
 *          }
 *          ADC1_0 = {
 *             ...
 *          }
 *          ...
 *     }
 * }
 */
class NI6368ADC: public DataSourceI, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor.
     * @details Initialises all the optional parameters as described in the class description.
     */
NI6368ADC    ();

    /**
     * @brief Destructor.
     * @details Closes all the file descriptors associated to the board, including any opened channels.
     * Stops the embedded thread which is reading from this board.
     */
    virtual ~NI6368ADC();

    /**
     * @brief See DataSourceI::AllocateMemory. NOOP.
     * @return true.
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 2.
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
     * @details Only InputSignals are supported.
     * @return "NI6368ADCInputBroker".
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details Adds a NI6368ADCInputBroker instance to the inputBrokers.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @return false.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief Waits on an EventSem for the requested number of samples to be acquired for all the channels.
     * @return true if the semaphore is successfully posted.
     */
    virtual bool Synchronise();

    /**
     * @brief Callback function for the EmbeddedThread that reads data from this ADC board.
     * @details Reads data from all the configured DMA channels and posts the synchronisation semaphore.
     * @return false if the synchronisation semaphore cannot be posted.
     */
    virtual ErrorManagement::ErrorType Execute(const ExecutionInfo & info);

    /**
     * @brief Gets the last index written by the DMA (can be either 0 or 1).
     * @return the last index written by the DMA.
     */
    uint8 GetLastBufferIdx();

    /**
     * @brief Returns true if there is one GAM synchronising on this board.
     * @return true if there is one GAM synchronising on this board.
     */
    bool IsSynchronising();



    /**
     * @brief Starts the EmbeddedThread and sets the counter and the time to zero.
     * @details See StatefulI::PrepareNextState. Starts the EmbeddedThread (if it was not already started) that reads from the ADC.
     * Sets the counter and the time to zero.
     * @return true if the EmbeddedThread can be successfully started.
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
     * - The type of the counter and of the time shall be 32 bit (un)signed integers.
     * - All the ADC channels have type uint16.
     * - The number of samples for all the ADC channels is the same.
     * @return true if all the parameters are valid and consistent with the board parameters and if the board can be successfully configured with
     *  these parameters.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Helper function to test the class.
     * @details This method is meant to be used by the unit-test class in order to verify the correct setting of the board parameters.
     * No real added value on making getters for all the structure elements, just for the sake of testing.
     * @return true if the board was opened (i.e. if SetConfiguredDatabase was successful).
     */
    bool ReadAIConfiguration(xseries_ai_conf_t *conf) const;

private:

    /**
     * @brief Copies from the DMA memory into the broker memory.
     * @details The DMA memory is organised in a different way (see xseries-lib.h) w.r.t. to the broker memory.
     * This function maps the DMA memory into the broker memory.
     * @param numberOfSamples number of samples to copy between memories.
     * @return ErrorManagement::FatalError if the semaphore cannot be posted.
     */
    ErrorManagement::ErrorType CopyFromDMA(uint32 numberOfSamples);



    /**
     * The counter value
     */
    uint32 counter;

    /**
     * The time value
     */
    uint32 timeValue;

    /**
     * The EmbeddedThread where the Execute method waits for the ADC data to be available.
     */
    SingleThreadService executor;

    /**
     * Number of samples to read
     */
    uint32 numberOfSamples;

    /**
     * The board identifier
     */
    uint32 boardId;

    /**
     * The board device name
     */
    StreamString deviceName;

    /**
     * The board individual channel gains
     */
    xseries_input_range_t inputRange[NI6368ADC_MAX_CHANNELS];

    /**
     * The board individual channel modes (differential, ...)
     */
    xseries_ai_channel_type_t inputType[NI6368ADC_MAX_CHANNELS];

    /**
     * The ADC sampling clock source;
     */
    xseries_ai_sample_convert_clock_t clockSampleSource;

    /**
     * The ADC sampling clock polarity;
     */
    xseries_ai_polarity_t clockSamplePolarity;

    /**
     * The ADC convert clock source;
     */
    xseries_ai_sample_convert_clock_t clockConvertSource;

    /**
     * The ADC convert clock polarity;
     */
    xseries_ai_polarity_t clockConvertPolarity;

    /**
     * The ADC scan interval counter source
     */
    xseries_scan_interval_counter_t scanIntervalCounterSource;

    /**
     * The ADC scan interval counter polarity
     */
    xseries_scan_interval_counter_polarity_t scanIntervalCounterPolarity;

    /**
     * The scan interval counter period
     */
    uint32 scanIntervalCounterPeriod;

    /**
     * The scan interval counter delay from the trigger
     */
    uint32 scanIntervalCounterDelay;

    /**
     * The board file descriptor
     */
    int32 boardFileDescriptor;

    /**
     * The channel file descriptors
     */
    int32 channelsFileDescriptors[NI6368ADC_MAX_CHANNELS];

    /**
     * The signals memory
     */
    uint16 *channelsMemory[2][NI6368ADC_MAX_CHANNELS];

    /**
     * The memory DMA
     */
    struct xseries_dma *dma;

    /**
     * The memory DMA offset
     */
    uint32 dmaOffset;

    /**
     * The memory where the DMA is copied to.
     */
    int16 *dmaReadBuffer;

    /**
     * Total number of DMA bytes from the beginning
     */
    uint32 nBytesInDMAFromStart;

    /**
     * The last written buffer
     */
    uint8 currentBufferIdx;

    /**
     * The number of samples written to the current buffer.
     */
    uint32 currentBufferOffset;

    /**
     * The ADCs that are enabled
     */
    bool adcEnabled[NI6368ADC_MAX_CHANNELS];

    /**
     * The number of enabled adcs
     */
    uint32 numberOfADCsEnabled;

    /**
     * The DMA buffer size
     */
    uint32 dmaBufferSize;

    /**
     * The semaphore for the synchronisation between the EmbeddedThread and the Synchronise method.
     */
    EventSem synchSem;

    /**
     * True while running
     */
    bool keepRunning;

    /**
     * True if this a synchronising data source
     */
    bool synchronising;

    /**
     * The requested cycle frequency for an application that synchronises on this board.
     */
    float32 cycleFrequency;

    /**
     * The thread CPUs mask.
     */
    uint32 cpuMask;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6368_NI6368ADC_H_ */

