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
/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include "xseries-lib.h"
/*lint -D__cplusplus*/
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
 * Number of ADC channels
 */
/*lint -esym(551, MARTe::NI6368ADC_MAX_CHANNELS) the symbol is used to define the size of several array below*/
const uint32 NI6363ADC_MAX_CHANNELS = 32u;

const uint32 NI6368ADC_MAX_CHANNELS = 16u;
/**
 * Counter and timer
 */
/*lint -esym(551, MARTe::NI6368ADC_HEADER_SIZE) the symbol is used to define the size of one array below*/
const uint32 NI6368ADC_HEADER_SIZE = 2u;
/**
 * Sampling frequency
 */
const uint32 NI6368ADC_SAMPLING_FREQUENCY = 2000000u;
/**
 * The number of buffers to synchronise with the DMA
 */
const uint32 NUMBER_OF_BUFFERS = 32u;
//If the execution mode is RealTimeThread the synchronisation is performed in the scope of the real-time thread. Otherwise if the mode is IndependentThread then a thread is spawned in order to synchronised with the CPU
/**
 * Execute in the context of the real-time thread.
 */
const uint32 NI6368ADC_EXEC_RTTHREAD = 1u;
/**
 * Execute in the context of a spawned thread.
 */
const uint32 NI6368ADC_EXEC_SPAWNED = 2u;

/**
 * @brief A DataSource which provides an input interface to the NI6363 (multiplexed) and NI6368 (simultaneous) boards.
 *
 * @details It's useful to understand how these boards works to configure it.
 * The NI-6363 is a multiplexed ADC board which can acquire up to 32 channels.
 * - Sample clock. It is the main clock which drives the channels block acquisition.
 * - Convert clock. It is the clock of a single conversion, in other words the single channel is acquired on the edge of this clock.
 * Both clocks must divide 100 MHz base clock and the maximum is:
 * - 2 MHz when only one channel is acquired
 * - 1 MHz if multiple channels
 * The normal configuration is to set the sample clock at the desired frequency and then set the convert clock fast enough to convert all the channels
 * before the next sample clock edge. After the channels acquisition, the convert clock stops waiting for the next sample edge.
 * Another possible configuration is to set sample and convert clock at the same frequency. In this case it behaves as we have a single clock which acts
 * as the convert clock at the configured frequency. It means that the channels block is acquired at the configured frequency divided by the number
 * of channels. As this frequency must divide exactly the 100 MHz base clock, this configuration does not fit all the uses cases of the first way of configuring.
 * Imagine we want to acquire 32 channels at 6400 Hz. Set sample=convert clock frequency to 6400*32=204800 will not work because 204800 does not divide 100 Mhz.
 * We can instead set sample frequency to 6400 and convert frequency to for instance to 1 MHz and get the job done.
 * Other important parameters are:
 * - SampleDelayDivisor: Minimum is 2. It's the phase between the main clock and the sample clock. It is expressed as divisor of 100 MHz (100 means 1us)
 * - ConvertDelayDivisor: Minimum is 2 but it is recommended to set it bigger. It's the phase between the sample clock and the convert clock.
 *
 * The NI6368 is instead a simultaneous ADC board. It can acquire up to 16 channels at 2 MHz maximum speed. In this case only the sampling frequency has to be
 * configured.
 *
 * @details Only one GAM is allowed to interface with this data source. It can interface in synchronised (Frequency>0 or Trigger>0) or non-synchronised mode.
 * When synchronised, the GAM waits the declared number of samples for each channels to be acquired before proceed. In non-synchronised mode, the read returns
 * immediately and if the declared samples have not been acquired yet, the buffer will be partially full and the Counter signal (if ResetOnBufferChange == 1)
 * reports the number of samples in the buffer.
 *
 * @details There are two execution modes:
 * - IndependentThread: an internal thread manages the acquisition from DMA and copy to the data source internal buffers
 * - RealTimeThread: the read from DMA is performed upon GAM read.
 *
 * <pre>
 * +NI6368_0 = {
 *     Class = NI6368::NI6368ADC
 *     DeviceName = "/dev/pxie-6368" //Mandatory
 *     BoardId = 0 //Mandatory
 *     IsMultiplexed = 0 //Default is 0 for 6368. Set to 1 if using 6363.
 *     SamplingFrequency = 1000000 //[0, 2 MHZ] //Mandatory (can be optional but in that case ScanIntervalCounterPeriod is mandatory)
 *     ConvertFrequency = 1000000 //[0, 1 MHZ] //Optional. Relevant only if using 6363. Set by default to SamplingFrequency if not specified.
 *     ExecutionMode = IndependentThread //Optional. If not specified ExecutionMode=IndependentThread. If the execution mode is RealTimeThread the synchronisation is performed in the scope of the real-time thread. Otherwise if the mode is IndependentThread then a thread is spawned in order to synchronised with the CPU
 *     DMABufferSize = 1000 //Optional. Default is 1000.
 *     ClockSampleSource = "INTERNALTIMING" //Optional. Default is INTERNALTIMING. Possible values: INTERNALTIMING, PFI0, ..., PFI15, RTSI0, ..., RTSI6, DIO_CHGDETECT, G0_OUT, ..., G3_OUT, STAR_TRIGGER, SCXI_TRIG1, LOW, PXIE_DSTARA, ATRIG, PXIE_DSTARB, G0_SAMPLECLK, ..., G3_SAMPLECLK, DI_CONVERT, AO_UPDATE, DO_UPDATE, INTTRIGGERA0, ..., INTTRIGGERA7
 *     ClockSamplePolarity = "ACTIVE_HIGH_OR_RISING_EDGE" //Optional. Default is ACTIVE_HIGH_OR_RISING_EDGE.Possible values: ACTIVE_HIGH_OR_RISING_EDGE, ACTIVE_LOW_OR_FALLING_EDGE
 *     ClockConvertSource = "INTERNALTIMING" //Optional. Default is INTERNALTIMING. Possible values: INTERNALTIMING, PFI0, ..., PFI15, RTSI0, ..., RTSI6, DIO_CHGDETECT, G0_OUT, ..., G3_OUT, STAR_TRIGGER, SCXI_TRIG1, LOW, PXIE_DSTARA, ATRIG, PXIE_DSTARB, G0_SAMPLECLK, ..., G3_SAMPLECLK, DI_CONVERT, AO_UPDATE, DO_UPDATE, INTTRIGGERA0, ..., INTTRIGGERA7
 *     ClockConvertPolarity = "ACTIVE_HIGH_OR_RISING_EDGE" //Optional. Default is ACTIVE_HIGH_OR_RISING_EDGE. Possible values: ACTIVE_HIGH_OR_RISING_EDGE, ACTIVE_LOW_OR_FALLING_EDGE
 *     ScanIntervalCounterSource = "COUNTER_TB3" //Optional. Default is COUNTER_TB3. Possible values: COUNTER_TB1, COUNTER_TB2, COUNTER_TB3, PFI0, ..., PFI15, RTSI0, ..., RTSI6,PXI_CLK0, STAR_TRIGGER, ANALOG_TRIGGER, D_STARA, D_STARB, STAR_TRIGGER
 *     ScanIntervalCounterPolarity = "RISING_EDGE" //Optional. Default is RISING_EDGE. Possible values: RISING_EDGE, FALLING_EDGE
 *     ScanIntervalCounterPeriod = 50 //Mandatory if SamplingFrequency not defined. Period of the scan interval expressed as divisor of 100 MHz.
 *     ScanIntervalCounterDelay = 2 //Optional. Default is 2. Minimum delay after the start trigger expressed as divisor of 2 MHz.
 *     CPUs = 0xf //Optional and only relevant if ExecutionMode==IndependentThread. CPU affinity for the thread which reads data from the board.
 *     RealTimeMode = 0 //Optional and onle relevant if ExecutionMode==IndependentThread. If 1 it will busy sleep on the synchronisation semaphores.
 *     Signals = {
 *          Counter = { //Mandatory. Number of ticks since last state change.
 *              Type = uint64 //uint32 also supported.
 *              CountSamples = 1 //Optional (default is 0). If enabled, the signal shows the total number of samples acquired
 *              ResetOnBufferChange = 1 //Optional (default is 0). If enabled, the signal shows the number of samples in the current frame
 *          }
 *          Time = { //Mandatory. Elapsed time in micro-seconds since last state change.
 *              Type = uint64 //uint32 also supported.
 *          }
 *          ADC0_0 = { //At least one ADC input shall be specified.
 *              InputRange = 10 //Optional. Possible values: 1, 2, 5, 10. Default value 10.
 *              Type = uint16 //Mandatory. Only the uint16 type is supported.
 *              ChannelId = 0 //Mandatory. The channel number.
 *              InputMode = Differential //Optional (default is Differential). Relevant only if 6363 as the 6368 only supports differential input. Possible values: Internal, Loopback, Differential, NRSE, RSE.
 *          }
 *          ADC1_0 = {
 *             ...
 *          }
 *          ...
 *     }
 * }
 * </pre>
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
     * @return NUMBER_OF_BUFFERS.
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
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @brief Gets the last index written by the DMA.
     * @return the last index written by the DMA.
     */
    uint8 GetLastBufferIdx();

    /**
     * @brief Returns true if there is one GAM synchronising on this board.
     * @return true if there is one GAM synchronising on this board.
     */
    bool IsSynchronising() const;

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


    /**
     * @brief Lock the buffer in use
     * @param[in] idx: the buffer index to lock
     */
    void Lock(const uint32 idx);

    /**
     * @brief Unlock the buffer in use
     * @param[in] idx: the buffer index to unlock
     */
    void UnLock(const uint32 idx);

    /**
     * @brief Calls the ADC read if ExecutionMode == RealTimeThread
     */
    bool NonSynchronise();
private:

    /**
     * @brief Resets the DMA
     */
    void ResetDMA();

    /**
     * @brief Copies from the DMA memory into the broker memory.
     * @details The DMA memory is organised in a different way (see xseries-lib.h) w.r.t. to the broker memory.
     * This function maps the DMA memory into the broker memory.
     * @param numberOfSamplesFromDMA number of samples to copy between memories.
     * @return ErrorManagement::FatalError if the semaphore cannot be posted.
     */
    ErrorManagement::ErrorType CopyFromDMA(size_t numberOfSamplesFromDMA);

    /**
     * The sampling frequency.
     */
    uint32 samplingFrequency;

    /**
     * The convert frequency.
     */
    uint32 convertFrequency;

    /**
     * The counter value 
     */
    uint32 counter;

    /**
     * The counter signal value (one for each buffer index)
     */
    uint64 counterValue[NUMBER_OF_BUFFERS];

    /**
     * The time value (one for each buffer index)
     */
    uint64 timeValue[NUMBER_OF_BUFFERS];

    /**
     * The last time value (for error checking)
     */
    uint32 lastTimeValue;

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
    int32 boardId;

    /**
     * The board device name
     */
    StreamString deviceName;

    /**
     * The board individual channel gains
     */
    xseries_input_range_t inputRange[NI6363ADC_MAX_CHANNELS];

    /**
     * The board individual channel modes
     */
    xseries_ai_channel_type_t inputMode[NI6363ADC_MAX_CHANNELS];

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
     * The ADC sample interval counter source if multiplexed
     */
    xseries_sample_interval_counter_t sampleIntervalCounterSource;

    /**
     * The sample interval if multiplexed
     */
    uint32 sampleIntervalCounterDelay;

    /**
     * The sample interval counter period if multiplexed
     */
    uint32 sampleIntervalCounterPeriod;

    /**
     * The board file descriptor
     */
    int32 boardFileDescriptor;

    /**
     * The channel file descriptors
     */
    int32 channelsFileDescriptors[NI6363ADC_MAX_CHANNELS];


    xseries_ai_scaling_coef_t ai_coefs[NI6363ADC_MAX_CHANNELS];

    /**
     * The signals memory
     */
    uint8 *channelsMemory[NUMBER_OF_BUFFERS][NI6363ADC_MAX_CHANNELS];

    /**
     * Maps the signal index in the signal list to the channel id
     */
    uint32 signalIdxToChannelId[NI6363ADC_MAX_CHANNELS + NI6368ADC_HEADER_SIZE];

    /**
     * The memory DMA
     */
    struct xseries_dma *dma;

    /**
     * The memory DMA offset
     */
    size_t dmaOffset;

    /**
     * The memory where the DMA is copied to.
     */
    int16 *dmaReadBuffer;

    /**
     * The memory where the calibrated DMA samples are copied to.
     */
    float32 *dmaCalibBuffer;

    /**
     * The current DMA channel being copied.
     */
    size_t dmaChannel;

    /**
     * Total number of DMA bytes from the beginning
     */
    size_t nSamplesInDMAFromStart;

    /**
     * The last written buffer
     */
    uint8 currentBufferIdx;

    /**
     * The last read buffer index
     */
    uint8 lastBufferIdx;

    /**
     * Semaphore to manage the buffer indexes.
     */
    FastPollingMutexSem fastMux[NUMBER_OF_BUFFERS];

    /**
     * Semaphore to lock all the buffers
     */
    FastPollingMutexSem allBufMux;

    /**
     * The number of samples written to the current buffer.
     */
    uint32 currentBufferOffset;

    /**
     * The ADCs that are enabled
     */
    bool adcEnabled[NI6363ADC_MAX_CHANNELS];

    /**
     * The number of enabled adcs
     */
    size_t numberOfADCsEnabled;

    /**
     * The DMA buffer size
     */
    uint32 dmaBufferSize;

    /**
     * The semaphore for the synchronisation between the EmbeddedThread and the Synchronise method.
     */
    EventSem synchSem;

    /**
     * Semaphore to manage the buffer indexes.
     */
    FastPollingMutexSem fastBufMux;

    /**
     * Sleep for the fastMux. Default if RealTimeMode == 0 or 0 if RealTimeMode == 1
     */
    float32 fastMuxSleepTime;

    /**
     * Semaphore to guarantee synchronous reset of the counter.
     */
    FastPollingMutexSem counterResetFastMux;

    /**
     * True while running
     */
    bool keepRunning;

    /**
     * True if this a synchronising data source
     */
    bool synchronising;

    /**
     * The thread CPUs mask.
     */
    uint32 cpuMask;

    /**
     * The execution mode.
     */
    uint32 executionMode;

    bool countSamples;

    bool resetOnBufferChange;

    bool changeBuffer;

    uint64 lastCounter;

    /**
     * The sampling frequency of the single ADC.
     */
    float64 singleADCFrequency;

    uint8 multiplexed;

    bool samplingFrequencySet;

    uint32 maxNumberOfChannels;

    bool calibrate;

    uint32 sampleSize;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6368_NI6368ADC_H_ */

