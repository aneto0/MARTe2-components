/**
 * @file RealTimeThreadAsyncBridge.h
 * @brief Header file for class RealTimeThreadAsyncBridge
 * @date 13/04/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class RealTimeThreadAsyncBridge
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMETHREADASYNCBIDGE_H_
#define REALTIMETHREADASYNCBIDGE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "FastPollingMutexSem.h"
#include "MemoryDataSourceI.h"
#include "MessageI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Allows to share asynchronously signals among two or more real time threads.
 *
 * @details For each signal, the writer GAM finds the oldest available buffer written to write on and after the write operation
 * it sets it as the newest. The reader GAMs attempt to read on the newest written available buffer. The mutual access between readers and
 * writer is guaranteed by spinlock semaphores, one for each buffer of each signal. If no buffer is available for the writer, the GetOutputOffset
 * function returns false. The same happens if there is no buffer available for the reader (impossible if more than one buffer has been declared),
 * in this case the GetInputOffset returns false.
 *
 * The RPC method ResetSignalValue allows to reset all the signal values.
 *
  * <pre>
 * +ThisDataSourceIObjectName = {
 *    Class = RealTimeThreadAsyncBridge
 *    NumberOfBuffers = 3 //Optional but < 64. Default = 1. Each buffer contains a copy of each signal.
 *    HeapName = "Default" //Optional. Default = GlobalObjectsDatabase::Instance()->GetStandardHeap();
 *    BlockingMode = 0 //Optional. Default = 0. NumberOfBuffers will be set to 1 and a spinlock mutex is used for synchronization on the shared signals
 *    ResetMSecTimeout = 1 //Optional. Default = TTInfiniteWait. The TerminateOutputCopy function can block when the counter used to newest written buffer overflows and needs to be reset.
 *                                                               If the reader should wait for this counter to be properly reset then the ResetMSecTimeout should
 *                                                               be increased to a large number. If instead the reader does not mind to get oldest buffer (instead of the newest) while this reset operation
 *                                                               is being performed, the ResetMSecTimeout should be set to 0.
 *                                                               This overflow will occur every 2**32-1 writes, which at e.g. 1 kHz frequency, will occur every ~49 days).
 *    Signals = {
 *        +*NAME = {
 *            +Type = BasicType|StructuredType
 *            +NumberOfDimensions = 0|1|2
 *            +NumberOfElements = NUMBER>0
 *       }
 *    }
 * }
 * </pre>
 */
class RealTimeThreadAsyncBridge: public MemoryDataSourceI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
RealTimeThreadAsyncBridge    ();

    /**
     * @brief Destructor
     * @details Frees the memory
     */
    virtual ~RealTimeThreadAsyncBridge();

    /**
     * @see DataSourceI::Initialise
     * @details Checks that (NumberOfBuffers < 64) .
     *   NumberOfBuffers = N (<64)\n
     *   ResetMSecTimeout = msec (the TerminateOutputCopy can block when the counter used to denote the oldest and newest written buffer
     *   overflows and needs to be reset. If the read should wait for this counter to be properly reset then the ResetMSecTimeout should
     *   be increased to a large number. If instead the reader does not mind to get oldest buffer (instead of the newest) while this reset operation
     *    is being performed, the ResetMSecTimeout should be set to 0.
     *   This overflow will occur every 2**32-1 writes, which at e.g. 1 kHz frequency, will occur every ~49 days).
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Allocates the memory for the state variables.
     * @details Checks that the signals have not defined the "Samples" field, because this data source does not support samples.
     * @return true if for each signal "Samples" is not defined and if the memory is allocated with no errors for the state variables.
     * It returns false otherwse.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief NOOP.
     * @return true.
     */
    virtual bool Synchronise();

    /**
     * @brief Initialises the variables that depends by the application state.
     * @details In the case where signals are written more than one time by the writer GAM and/or signals with more than one range defined,
     * the TerminateOutputCopy function will not unlock the buffer until all the write operations have been completed. In this function
     * the \a writeOp variable is set as the sum of the write operations for each signal.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @see DataSourceI::GetInputOffset
     * @details Checks the last written available buffer and returns its offset. The atomic variable \a spinlocksRead, denoting the number
     * of readers on that buffer is incremented. The writer can not write on that buffer if this atomic variable is greater than zero.
     * @return false if no buffer is available. This happens only if only one buffer is defined and the writer is writing on it.
     */
    virtual bool GetInputOffset(const uint32 signalIdx,
            const uint32 numberOfSamples, uint32 &offset);

    /**
     * @see DataSourceI::GetOutputOffset
     * @details Checks the oldest written available buffer and returns its offset. the semaphore \a spinlocksWrite, is locked
     * on that buffer. The readers can not read on that buffer while the semaphore is locked.
     * @return false if no buffer is available because the readers are using all of them.
     */
    virtual bool GetOutputOffset(const uint32 signalIdx,
            const uint32 numberOfSamples, uint32 &offset);

    /**
     * @see DataSourceI::TerminateInputCopy
     * @details Decrements the atomic variable \a spinlocksRead for the buffer that has just been read.
     */
    virtual bool TerminateInputCopy(const uint32 signalIdx, const uint32 offset,
            const uint32 numberOfSamples);

    /**
     * @see DataSourceI::TerminateOutputCopy
     * @details If all the write operations for the signal \a signalIdx have been terminated, it increments the
     * \a whatIsNewestCounter setting the buffer as the last written. The spinlocksWrite variable is unlocked for the
     * buffer that have just been written.
     */
    virtual bool TerminateOutputCopy(const uint32 signalIdx,
            const uint32 offset, const uint32 numberOfSamples);

    /**
     * @see DataSourceI::GetBrokerName
     * @returns the broker name.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    /**
     * @brief RPC call to reset all the signal values.
     */
    ErrorManagement::ErrorType ResetSignalValue();

protected:

    /**
     * Denoted the current number of readers for each buffer.
     */
    volatile int32 *spinlocksRead;

    /**
     * A semaphore to lock the buffer that is going to be written.
     */
    FastPollingMutexSem *spinlocksWrite;

    /**
     * Used to denote the oldest and newest written buffers
     */
    uint32 *whatIsNewestCounter;

    /**
     * The counter incremented for each signal at the end of the write operation
     */
    uint32 *whatIsNewestGlobCounter;

    /**
     * The timeout in millisecs to lock the other \a spinlocksWrite semaphores
     * in case of overflow of \a whatIsNewestCounter (to avoid a priority inversion)
     */
    TimeoutType resetTimeout;

    /**
     * TODO
     */
    uint8 blockingMode;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMETHREADASYNCBIDGE_H_ */

