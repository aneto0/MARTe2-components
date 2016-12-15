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
/**
 * @brief A DataSource which provides an input interface to the NI6259 boards.
 * @details The DataSource shall always be ...
 *
 * The configuration syntax is (names are only given as an example):
 * +NI6259_0 = {
 *     Class = NI6259::NI6259ADC
 *     TODO
 *     Signals = {
 *         TODO
 *     }
 * }
 */

namespace MARTe {
//Number of ADC channels
const uint32 NI6259ADC_MAX_CHANNELS = 32u;
//Counter and timer
const uint32 NI6259ADC_HEADER_SIZE = 2u;
/**
 * @brief TODO
 */
class NI6259ADC: public DataSourceI, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief TODO
     */
NI6259ADC    ();

    /**
     * @brief TODO
     */
    virtual ~NI6259ADC();

    /**
     * @brief See DataSourceI::AllocateMemory.
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
     * @return false.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief Waits on an EventSem for the requested number of samples to be acquired.
     * @return true if the semaphore is successfully posted.
     */
    virtual bool Synchronise();

    /**
     * @brief Callback function for the EmbeddedThread that reads data from this ADC board.
     * @details TODO
     * @return TODO.
     */
    virtual ErrorManagement::ErrorType Execute(const ExecutionInfo & info);

    /**
     * @brief TODO
     * @details See StatefulI::PrepareNextState. Starts the EmbeddedThread (if it was not already started) and loops
     * on the ExecuteMethod.
     * @return true if the EmbeddedThread can be successfully started.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief TODO
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Verifies that 32, and only 32, signal are set with the correct type.
     * @details TODO
     * @return TODO.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

private:

    /**
     * The counter value
     */
    uint32 counter;

    /**
     * The time value
     */
    uint32 time;

    /**
     * The EmbeddedThread where the Execute method waits for the ADC data to be available.
     */
    SingleThreadService executor;

    /**
     * Index of the function which has the signal that synchronises on this DataSourceI.
     */
    uint32 synchronisingFunctionIdx;

    /**
     * Number of samples to read
     */
    uint32 numberOfSamples;

    /**
     * The sampling frequency.
     */
    uint32 samplingFrequency;

    /**
     * The sampling period in micro-seconds.
     */
    uint32 samplingPeriodMicroSeconds;

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
    uint32 inputRange[NI6259ADC_MAX_CHANNELS];

    /**
     * The board individual channel polarities
     */
    ai_polarity_t inputPolarity[NI6259ADC_MAX_CHANNELS];

    /**
     * The board individual channel modes (differential, ...)
     */
    ai_channel_type_t inputMode[NI6259ADC_MAX_CHANNELS];

    /**
     * The ADC clock source;
     */
    ai_sample_select_t clockSource;

    /**
     * The ADC clock source;
     */
    ai_sample_polarity_t clockPolarity;

    /**
     * The clock delay divisior
     */
    uint32 delayDivisor;

    /**
     * The board file descriptor
     */
    int32 boardFileDescriptor;

    /**
     * The channel file descriptors
     */
    int32 channelsFileDescriptors[NI6259ADC_MAX_CHANNELS];

    /**
     * The signal memory
     */
    float32 *channelsMemory[NI6259ADC_MAX_CHANNELS];

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

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6259_NI6259ADC_H_ */

