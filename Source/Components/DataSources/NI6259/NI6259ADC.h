/**
 * @file NI6259ADC.h
 * @brief Header file for class NI6259ADC
 * @date 28/11/2016
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

 * @details This header file contains the declaration of the class NI6259ADC
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6259_NI6259ADC_H_
#define NI6259_NI6259ADC_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pxi-6259-lib.h"

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
/*lint -esym(551, MARTe::NI6259ADC_MAX_CHANNELS) the symbol is used to define the size of several arrays below*/
const uint32 NI6259ADC_MAX_CHANNELS = 32u;
/**
 * Header definition.
 */
const uint32 NI6259ADC_HEADER_SIZE = 2u;
/**
 * The number of buffers to synchronise with the DMA
 */
const uint32 NUMBER_OF_BUFFERS = 32u;
/**
 * @brief A DataSource which provides an input interface to the NI6259 boards.
 * @details Note that this is a multiplexed board and thus the sampling frequency is
 * shared between the number of enabled channels.
 *
 * @details It's useful to understand how this board works to configure it. The NI-6259 is a multiplexed ADC board which can acquire up to 32 channels.
 * - Sample clock. It is the main clock which drives the channels block acquisition.
 * - Convert clock. It is the clock of a single conversion, in other words the single channel is acquired on the edge of this clock.
 * Both clocks must divide 20 MHz base clock and the maximum is:
 * - 1.25 MHz when only one channel is acquired
 * - 1 MHz if multiple channels
 * The normal configuration is to set the sample clock at the desired frequency and then set the convert clock fast enough to convert all the channels
 * before the next sample clock edge. After the channels acquisition, the convert clock stops waiting for the next sample edge.
 * Another possible configuration is to set sample and convert clock at the same frequency. In this case it behaves as we have a single clock which acts
 * as the convert clock at the configured frequency. It means that the channels block is acquired at the configured frequency divided by the number
 * of channels. As this frequency must divide exactly the 20 MHz base clock, this configuration does not fit all the uses cases of the first way of configuring.
 * Imagine we want to acquire 32 channels at 6400 Hz. Set sample=convert clock frequency to 6400*32=204800 will not work because 204800 does not divide 20 Mhz.
 * We can instead set sample frequency to 6400 and convert frequency to for instance to 1 MHz and get the job done.
 * Other important parameters are:
 * - SampleDelayDivisor: Minimum is 3. It's the phase between the main clock and the sample clock. It is expressed as divisor of 20 MHz (20 means 1us)
 * - ConvertDelayDivisor: Minimum is 3 but it is recommended to set it bigger. It's the phase between the sample clock and the convert clock.
 *
 * @details Only one GAM is allowed to interface with this data source. It can interface in synchronised (Frequency>0 or Trigger>0) or non-synchronised more.
 * When synchronised, the GAM waits the declared number of samples for each channels to be acquired before proceed. In non-synchronised mode, the read returns
 * immediately and if the declared samples have not been acquired yet, the buffer will be partially full and the Counter signal (if ResetOnBufferChange == 1)
 * reports the number of samples in the buffer.
 *
 * @details There are two execution modes:
 * - IndependentThread: an internal thread manages the acquisition from DMA and copy to the data source internal buffers
 * - RealTimeThread: the read from DMA is performed upon GAM read.
 *
 * The configuration syntax is (names are only given as an example):
 * <pre>
 * +NI6259_0 = {
 *     Class = NI6259::NI6259ADC
 *     SamplingFrequency = 1000000 //]0, 1 MHZ]
 *     DeviceName = "/dev/pxi6259" //Mandatory
 *     BoardId = 0 //Mandatory
 *     SampleDelayDivisor = 3 //Optional. Default is 3
 *     ConvertDelayDivisor = 3 //Optional. Default is 3 but recommended to set to at least 20.
 *     ClockSampleSource = "SI_TC" //Optional (default is SI_TC). Sampling clock source. Possible values:SI_TC, PFI0, ..., PFI15, RTSI0, ..., RTSI7, PULSE, GPCRT0_OUT, STAR_TRIGGER, GPCTR1_OUT, SCXI_TRIG1, ANALOG_TRIGGER, LOW
 *     ClockSamplePolarity = "ACTIVE_HIGH_OR_RISING_EDGE" //Optional (default is ACTIVE_HIGH_OR_RISING_EDGE). Sampling clock polarity. Possible values: ACTIVE_HIGH_OR_RISING_EDGE, ACTIVE_LOW_OR_FALLING_EDGE
 *     ClockConvertSource = "SI2TC" //Optional (default is SI2TC). Convert clock source. Possible values:SI2TC, PFI0, ..., PFI15, RTSI0, ..., RTSI7, GPCRT0_OUT, STAR_TRIGGER, ANALOG_TRIGGER, LOW
 *     ClockConvertPolarity = "RISING_EDGE" //Optional (default is RISING_EDGE). Convert clock polarity. Possible values:RISING_EDGE, FALLING_EDGE
 *     ExecutionMode = IndependentThread //Optional (default is IndepenentThread). Possible values:IndependentThread, RealTimeThread
 *     CPUs = 0xf //CPUs where the thread which reads data from the board is allowed to run on.
 *     Convert = 0 //Optional (default is 0). If disabled the signals show raw acquired value (signal type must be int16). If enabled the samples already converted accordingly to the declared InputRange. In this case the signal type must be float32
 *
 *     Signals = {
 *          Counter = { //Mandatory. Number of buffers since last state change.
 *              Type = uint32 //int32 also supported.
 *              CountSamples = 1 //Optional (default is 0). If enabled, the signal shows the total number of samples acquired
 *              ResetOnBufferChange = 1 //Optional (default is 0). If enabled, the signal shows the number of samples in the current frame
 *          }
 *          Time = { //Mandatory. Elapsed time in micro-seconds since last state change.
 *               Type = uint32 //int32 also supported.
 *          }
 *          ADC0_0 = { //At least one ADC input shall be specified.
 *              InputRange = 10 //Optional. Possible values: 0.1, 0.2, 0.5, 1, 2, 5, 10. Default value 10.
 *              Type = int16 //Mandatory. It can be int16 (if Convert == 0) or float32 (if Convert == 1)
 *              ChannelId = 0 //Mandatory. The channel number.
 *              InputPolarity = Bipolar //Optional. Possible values: Bipolar, Unipolar. Default value Unipolar.
 *              InputMode = RSE //Optional. Possible values: Differential, RSE, NRSE. Default value RSE.
 *          }
 *          ADC1_0 = {
 *             ...
 *          }
 *          ...
 *     }
 * }
 * </pre>
 */
class NI6259ADC: public DataSourceI, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor.
     * @details Initialises all the optional parameters as described in the class description.
     */
NI6259ADC    ();

    /**
     * @brief Destructor.
     * @details Closes all the file descriptors associated to the board, including any opened channels.
     * Stops the embedded thread which is reading from this board.
     */
    virtual ~NI6259ADC();

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
     * @details Only InputSignals are supported.
     * @return MemoryMapSynchronisedInputBroker if frequency > 0, MemoryMapInputBroker otherwise.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details If the functionName is the one synchronising it adds a MemoryMapSynchronisedInputBroker instance to
     *  the inputBrokers, otherwise it adds a MemoryMapInputBroker instance to the inputBrokers.
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
     * @details Reads data from all the configured ADC channels and posts the synchronisation semaphore.
     * @return false if the synchronisation semaphore cannot be posted. Note that failure to read from the ADC will not
     * return an error as the reading operation will be retried forever.
     * @warning This method sleeps for 100 us. This is needed as otherwise it gets stuck on the function pxi6259_read_ai.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

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
     * - All the ADC channels have type float32.
     * - The number of samples of all the ADC channels is the same.
     * - For synchronising boards (i.e. where a Frequency was defined for a given channel):
     *  - The single ADC frequency (SamplingFrequency/NumberOfChannels) > Frequency * Samples
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
    bool ReadAIConfiguration(pxi6259_ai_conf_t *conf) const;

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
     * @brief Lock the buffer in use
     * @param[in] idx: the buffer index to lock
     */
    void LockB(const uint32 idx);

    /**
     * @brief Unlock the buffer in use
     * @param[in] idx: the buffer index to unlock
     */
    void UnLockB(const uint32 idx);

    /**
     * @brief Calls the ADC read if ExecutionMode == RealTimeThread
     */
    bool NonSynchronise();

private:

    /**
     * @brief Copies from the DMA memory into the broker memory.
     * @details The DMA memory is organised in a different way (see xseries-lib.h) w.r.t. to the broker memory.
     * This function maps the DMA memory into the broker memory.
     * @param numberOfSamplesFromDMA number of samples to copy between memories.
     * @return ErrorManagement::FatalError if the semaphore cannot be posted.
     */
    ErrorManagement::ErrorType CopyFromDMA(size_t numberOfSamplesFromDMA);

    /**
     * The number of samples written to the current buffer.
     */
    uint32 currentBufferOffset;

    /**
     * The counter value
     */
    uint64 counter;

    /**
     * The time value
     */
    uint64 timeValue[NUMBER_OF_BUFFERS];

    /**
     * The last time value (for error checking)
     */
    uint64 lastTimeValue;

    /**
     * The counter value. Different from counter to avoid synchronisation issues.
     */
    uint64 counterValue[NUMBER_OF_BUFFERS];

    /**
     * The EmbeddedThread where the Execute method waits for the ADC data to be available.
     */
    SingleThreadService executor;

    /**
     * Number of samples to read
     */
    uint32 numberOfSamples;

    /**
     * The sampling frequency.
     */
    uint32 samplingFrequency;

    /**
     * The convert frequency.
     */
    uint32 convertFrequency;

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
    uint8 inputRange[NI6259ADC_MAX_CHANNELS];

    /**
     * The board individual channel polarities
     */
    ai_polarity_t inputPolarity[NI6259ADC_MAX_CHANNELS];

    /**
     * The board individual channel modes (differential, ...)
     */
    ai_channel_type_t inputMode[NI6259ADC_MAX_CHANNELS];

    /**
     * The ADC sampling clock source;
     */
    ai_sample_select_t clockSampleSource;

    /**
     * The ADC sampling clock polarity;
     */
    ai_sample_polarity_t clockSamplePolarity;

    /**
     * The ADC convert clock source;
     */
    ai_convert_select_t clockConvertSource;

    /**
     * The ADC convert clock polarity;
     */
    ai_convert_polarity_t clockConvertPolarity;

    /**
     * The sample clock delay divisor
     */
    uint32 sampleDelayDivisor;

    /**
     * The sample clock delay divisor
     */
    uint32 convertDelayDivisor;

    /**
     * The board file descriptor
     */
    int32 boardFileDescriptor;

    /**
     * The signals memory
     */
    uint8 *channelsMemory[NUMBER_OF_BUFFERS][NI6259ADC_MAX_CHANNELS];

    /**
     * The memory DMA offset
     */
    size_t dmaOffset;

    /**
     * The memory DMA
     */
    struct pxi6259_dma *dma;

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
     * The ADCs that are enabled
     */
    bool adcEnabled[NI6259ADC_MAX_CHANNELS];

    /**
     * The number of enabled adcs
     */
    uint32 numberOfADCsEnabled;

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
     * The thread CPUs mask.
     */
    uint32 cpuMask;

    /**
     * Semaphore associated with the counter reset
     */
    FastPollingMutexSem counterResetFastMux;

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
     * Semaphore to manage the buffer indexes.
     */
    FastPollingMutexSem fastBufMux;


    /**
     * Sleep for the fastMux. Default if RealTimeMode == 0 or 0 if RealTimeMode == 1
     */
    float32 fastMuxSleepTime;

    /**
     * The sampling frequency of the single ADC.
     */                
    float64 singleADCFrequency;

    /**
     * Additional attributes.
     */
    ConfigurationDatabase attributes;

    /**
     * The synchronisation type. If real-time thread or independent thread
     */
    uint8 synchType;

    /**
     * Use calibration to get the final value
     */
    bool calibrate;


    ai_scaling_coefficients_t ai_coefs_all;


    uint32 sampleSize;

    bool countSamples;

    bool resetOnBufferChange;

    bool changeBuffer;

    uint64 lastCounter;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6259_NI6259ADC_H_ */

