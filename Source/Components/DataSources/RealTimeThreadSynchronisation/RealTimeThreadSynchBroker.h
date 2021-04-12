/**
 * @file RealTimeThreadSynchBroker.h
 * @brief Header file for class RealTimeThreadSynchBroker
 * @date 24/04/2017
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

 * @details This header file contains the declaration of the class RealTimeThreadSynchBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMETHREADSYNCHBROKER_H_
#define REALTIMETHREADSYNCHBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EventSem.h"
#include "MemoryMapInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Input broker for the RealTimeThreadSynchronisation DataSourceI.
 * @details A MemoryMapInputBroker which will store in memory the required number of samples copies of the DataSourceI memory.
 * It will lock in Execute until the required number of samples are added by calling the AddSample method.
 */
class RealTimeThreadSynchBroker : public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
    RealTimeThreadSynchBroker();

    /**
     * @brief Destructor.
     */
    virtual ~RealTimeThreadSynchBroker();

    /**
     * @brief Sets the index of the function in the DataSourceI associated to this broker instance.
     * @param[in] dataSourceIn the RealTimeThreadSynchronisation DataSourceI instance using this broker.
     * @param[in] functionIdxIn the index of the function in the DataSourceI.
     * @param[in] timeoutIn the maximum time to wait for the expected number of samples to be available.
     * @param[in] waitForNextIn if 1 first reset and then wait at the synchronisation point.
     */
    void SetFunctionIndex(DataSourceI *dataSourceIn, uint32 functionIdxIn, const TimeoutType & timeoutIn, const uint8 waitForNextIn);

    /**
     * @brief Allocates memory to hold N copies of the dataSourceMemoryIn, where the N is the number of samples that are to be
     * stored by the signals allocated to this broker.
     * @param[in] dataSourceMemoryIn the RealTimeThreadSynchronisation DataSourceI memory holding the latest values.
     * @param[in] dataSourceMemoryOffsetsIn the signals offsets in the \a dataSourceMemoryIn.
     * @return true if the number of samples is the same for all signals and if the memory could be successfully allocated.
     */
    bool AllocateMemory(char8 *dataSourceMemoryIn, uint32 *dataSourceMemoryOffsetsIn);

    /**
     * @brief Proxy method to the DataSourceI::GetSignalMemoryBuffer
     */
    bool GetSignalMemoryBuffer(const uint32 signalIdx, void *&signalAddress) const;

    /**
     * @brief Adds a new sample of all the signals into the memory managed by this broker.
     * @return true if the memory could be successfully copied.
     * @pre
     *   All the memory must have been successfully allocated. For performance reasons the memory allocation is not checked at every iteration.
     */
    bool AddSample();

    /**
     * @brief Gets the name of the GAM interacting with the DataSourceI that uses this broker instance.
     * @return the name of the GAM interacting with the DataSourceI that uses this broker instance.
     */
    const char8 *const GetGAMName();

    /**
     * @brief Locks until the expected number of samples is written into this broker instance (see AddSample) and then copies the samples
     * using the MemoryMapInputBroker::Execute().
     * @return true if MemoryMapInputBroker::Execute().
     */
    virtual bool Execute();

private:
    /**
     * Number of signals in the DataSourceI (not all will necessarily be writing to this broker instance).
     */
    uint32 numberOfDataSourceSignals;

    /**
     * Memory to hold N samples of all the DataSourceI signals.
     */
    char8 **signalMemory;

    /**
     * Size of each signal to be copied.
     */
    uint32 *signalSize;

    /**
     * Pointer to the memory of the DataSourceI.
     */
    char8 *dataSourceMemory;

    /**
     * Offsets in the memory of the DataSourceI.
     */
    uint32 *dataSourceMemoryOffsets;

    /**
     * Index of the function in the DataSourceI associated to this broker instance.
     */
    uint32 functionIdx;

    /**
     * Number of samples to be read from this DataSourceI.
     */
    uint32 numberOfSamples;

    /**
     * The current samples being written.
     */
    uint32 currentSample;

    /**
     * Semaphore which will lock until all the samples are written.
     */
    EventSem synchSem;

    /**
     * The name of the GAM interacting with the DataSourceI that uses this broker instance.
     */
    StreamString gamName;

    /**
     * Synchronisation timeout
     */
    TimeoutType timeout;

    /**
     * If 1 => first reset and then wait at the synchronisation point.
     */
    uint8 waitForNext;

    /**
     * Protect the event sem
     */
    FastPollingMutexSem mux;
};
}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMETHREADSYNCHBROKER_H_ */
	
