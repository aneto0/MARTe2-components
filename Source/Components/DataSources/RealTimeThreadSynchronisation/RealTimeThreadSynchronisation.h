/**
 * @file RealTimeThreadSynchronisation.h
 * @brief Header file for class RealTimeThreadSynchronisation
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

 * @details This header file contains the declaration of the class RealTimeThreadSynchronisation
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMETHREADSYNCHRONISATION_H_
#define REALTIMETHREADSYNCHRONISATION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EventSem.h"
#include "RealTimeThreadSynchBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A DataSource which allows to synchronise multiple real-time threads.
 * @details A GAM will write into this DataSource on a given thread. One or more GAMs (each on its thread) will read and
 *  synchronise against this DataSource. If the number of samples read from the GAM is great than one the DataSource will
 *  block until the specified number of samples is available. This feature allows to run and synchronise threads at a lower frequency.
 *
 * Only one GAM is allowed to write into this DataSource. More than one GAM may read in the same thread, but it should be noted that the
 *  reading is blocking and this will force multiple synchronisation points (which has an unspecified behaviour and thus should be avoided).
 *
 * The number of samples of the signals of the GAM writing into this DataSource shall be exactly 1.
 *
 * The number of samples for all the signals of any given GAM reading from this DataSource shall be the same.
 *
 * If the parameter WaitForNext is set to 1, the synchronisation thread will first reset and then wait on the event semaphore. This can be 
 * useful if cycles were lost and the thread should wait for the next synchronisation cycle. The default behaviour (WaitForNext=0) is to 
 * first wait and then reset the semaphore and, as a consequence, if the semaphore had already been posted, it will not wait.
 *
 * The configuration syntax is (names are only given as an example):
 * <pre>
 * +Functions = {"
 *   +GAM1 = {
 *     Class = AGAM
 *     OutputSignals = {
 *       Signal1 = {
 *         SignalType = uint32
 *         NumberOfDimensions = 1
 *         NumberOfElements = 4
 *         DataSource = RealTimeThreadSynch"
 *       }
 *       Signal2 = {
 *         SignalType = float64
 *         NumberOfDimensions = 1
 *         NumberOfElements = 1
 *         DataSource = RealTimeThreadSynch"
 *       }
 *     }
 *   }
 *   +GAM2 = {
 *     Class = AnotherGAM
 *     InputSignals = {
 *       Signal1 = {
 *         SignalType = uint32
 *         NumberOfDimensions = 1
 *         NumberOfElements = 4
 *         DataSource = RealTimeThreadSynch"
 *         Samples = 2 //Will run at half the frequency
 *       }
 *       Signal2 = {
 *         SignalType = float64
 *         NumberOfDimensions = 1
 *         NumberOfElements = 1
 *         DataSource = RealTimeThreadSynch"
 *         Samples = 2 //Will run at half the frequency
 *       }
 *     }
 *   }
 *   +GAM3 = {
 *     Class = AnotherGAM
 *     InputSignals = {
 *       Signal1 = {
 *         SignalType = uint32
 *         NumberOfDimensions = 1
 *         NumberOfElements = 4
 *         DataSource = RealTimeThreadSynch"
 *         Samples = 4 //Will run at a quarter of the frequency
 *       }
 *       Signal2 = {
 *         SignalType = float64
 *         NumberOfDimensions = 1
 *         NumberOfElements = 1
 *         DataSource = RealTimeThreadSynch"
 *         Samples = 4 //Will run at a quarter of the frequency
 *       }
 *     }
 *   }
 * }
 * +States = {
 *   Class = ReferenceContainer
 *   +State1 = {
 *     Class = RealTimeState
 *     +Threads = {
 *       Class = ReferenceContainer
 *       +Thread1 = {
 *         Class = RealTimeThread
 *         Functions = {GAM1Thread1}
 *       }
 *       +Thread2 = {
 *         Class = RealTimeThread
 *         Functions = {GAM1Thread2}
 *       }
 *       +Thread3 = {
 *         Class = RealTimeThread
 *         Functions = {GAM1Thread3}
 *       }
 *     }
 *   }
 * }
 * +Data = {
 *   +RealTimeSynch = {
 *     Class = RealTimeThreadSynchronisation
 *     Timeout = 1000 //Timeout in ms to wait for the thread to cycle.
 *                    //If this parameter is not set it will wait forever to be triggered and might lock a state change.
 *                    //Default is 1000
 *   }
 * }
 * </pre>
 */
class RealTimeThreadSynchronisation: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP
     */
RealTimeThreadSynchronisation    ();

    /**
     * @brief Destructor. Frees the DataSource memory (allocated to store a copy of all the signals).
     */
    virtual ~RealTimeThreadSynchronisation();

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
     * @pre
     *   SetConfiguredDatabase
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief See DataSourceI::GetBrokerName.
     * @return RealTimeThreadSynchBroker if direction == InputSignals, MemoryMapSynchronisedOutputBroker otherwise.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details Creates a new instance of a RealTimeThreadSynchBroker and inserts it into the \a inputBrokers list.
     * @return true if RealTimeThreadSynchBroker::Init initialises correctly.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @details Creates a new instance of a MemoryMapSynchronisedOutputBroker and inserts it into the \a outputBrokers list.
     * @return true if MemoryMapSynchronisedOutputBroker::Init initialises correctly.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::PrepareNextState. NOOP.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Calls DataSourceI::Initialise.
     * @return see DataSourceI::Initialise.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Final verification of all the parameters. Setup of the memory required to hold all the signals.
     * @details This method verifies that all the parameters requested by the GAMs interacting with this DataSource
     *  are valid and consistent the following rules:
     * - Only one GAM is writing signals into this DataSourceI instance.
     * - The number of written samples is exactly one.
     * - The number of read samples is constant for all the signals of any given GAM (but may different between GAMs).
     * - If there is a GAM reading from this DataSourceI, then there must be a GAM writing into this DataSourceI.
     * @return true if all the parameters are valid and the conditions above are met.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Calls RealTimeThreadSynchBroker::AddSample on all the brokers.
     * @return true if all the AddSample calls return true.
     */
    virtual bool Synchronise();

    /**
     * @brief Gets the synchronisation timeout.
     * @return the synchronisation timeout.
     */
    TimeoutType GetSynchroniseTimeout() const;

private:
    /**
     * List of input brokers. One for each GAM reading from this DataSourceI.
     */
    ReferenceContainer synchInputBrokersContainer;

    /**
     * Accelerator for the list of input brokers. One for each GAM reading from this DataSourceI.
     */
    RealTimeThreadSynchBroker **synchInputBrokers;

    /**
     * Number of GAMs reading from this DataSourceI.
     */
    uint32 numberOfSyncGAMs;

    /**
     * Memory which holds the latest values of the signals written into this DataSourceI.
     */
    char8 *memory;

    /**
     * Size required to hold the latest values of the signals written into this DataSourceI.
     */
    uint32 memorySize;

    /**
     * Offsets of the signals in the memory area.
     */
    uint32 *memoryOffsets;

    /**
     * Allows to correctly synchronise the GetInputBrokers->RealTimeThreadSynchBroker::Init with the GetSignalMemoryBuffer function.
     */
    int32 currentInitBrokerIndex;

    /**
     * Synchronisation timeout
     */
    TimeoutType timeout;

    /**
     * If 1 => first reset and then wait at the synchronisation point.
     */
    uint8 waitForNext;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMETHREADSYNCHRONISATION_H_ */

