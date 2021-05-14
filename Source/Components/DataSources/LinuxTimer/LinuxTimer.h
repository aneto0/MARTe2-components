/**
 * @file LinuxTimer.h
 * @brief Header file for class LinuxTimer
 * @date 24/10/2016
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

 * @details This header file contains the declaration of the class LinuxTimer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_LINUXTIMER_H_
#define SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_LINUXTIMER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"
#include "RealTimeApplication.h"
#include "SingleThreadService.h"
#include "TimeProvider.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief A DataSource which provides a timing source for a MARTe application.
 * @details The LinuxTimer provides a timing generation facility where generators can be conveniently plugged in.
 * The LinuxTimer itself comes with a default provider which is based on internal HighResolutionTimer.
 * The Timer can be configured with two, three or four signals and it shall
 * always have a frequency set in one of the signals.

 * The configuration syntax is (names are only given as an example):
 * <pre>
 * +Timer = {
 *     Class = LinuxTimer
 *     SleepNature = Busy|Default//If SleepNature is not specified then Default is set
 *     SleepPercentage = 0 //Only meaningful if SleepNature = Busy. The percentage of time to sleep using the OS sleep.
 *     ExecutionMode = IndependentThread //Optional. If not set ExecutionMode = IndependentThread. If ExecutionMode == IndependentThread a thread is spawned to generate the time events. ExecutionMode == RealTimeThread the time is generated in the context of the real-time thread.
 *     Phase = 1 //Optional, sets the phase of the timing generation, defaults to MAX_PHASE = 1000000u
 *     CPUMask = 0x8 //Optional and only relevant if ExecutionMode=IndependentThread
 *     +TimeProvider = { //Optional, if omitted defaults to HighResolutionTimeProvider
 *         //Can be any of the implementing types for the TimeProvider interface
           //Please refer to the specific time provider interface for configuration details
 *         //Please refer to the TimeProvider.h header for implementation details
 *         Class = HighResolutionTimeProvider
 *     }
 *     Signals = {
 *         Counter = {
 *             Type = uint32 //int32 also supported
 *         }
 *         Time = {
 *             Type = uint32 //int32 also supported
 *             Frequency = 1000
 *         }
 *           AbsoluteTime = { //Optional, can be omitted
 *               Type = uint64 //Only type supported
 *           }
 *           DeltaTime = { //Optional, can be omitted
 *               Type = uint64 //Only type supported
 *           }
 *     }
 * }
 * </pre>
 *
 * If the SleepNature=Busy a Sleep::Busy will be used to wait for the 1/Frequency period to elapse, otherwise
 *  a Sleep::NoMore will be used.
 */
class LinuxTimer: public DataSourceI, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor
     * @post
     *   Counter = 0
     *   Time = 0
     */
    LinuxTimer ();

    /**
     * @brief Destructor. Stops the EmbeddedThread.
     */
    virtual ~LinuxTimer();

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
     * @brief Waits on an EventSem for the period given by 1/Frequency to elapse on Execute.
     * @return true if the semaphore is successfully posted.
     */
    virtual bool Synchronise();

    /**
     * @brief Callback function for an EmbeddedThread.
     * @details Sleeps (Busy or Default) for the period given by 1/Frequency and post an EventSem which is waiting on
     *  the Synchronise method.
     * @param[in] info not used.
     * @return NoError if the EventSem can be successfully posted.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @brief Resets the counter and the timer to zero and starts the EmbeddedThread.
     * @details See StatefulI::PrepareNextState. Starts the EmbeddedThread (if it was not already started) and loops
     * on the ExecuteMethod.
     * @return true if the EmbeddedThread can be successfully started.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief Initialises the LinuxTimer
     * @param[in] data configuration in the form:
     * +Timer = {
     *     Class = LinuxTimer
     *     SleepNature = Busy|Default//If SleepNature is not specified then Default is set
     *     Phase = 1000000u
     *     Signals = {
     *         Counter = {
     *             Type = uint32 //int32 also supported
     *         }
     *         Time = {
     *             Type = uint32 //int32 also supported
     *             Frequency = 1000
     *         }
     *           AbsoluteTime = { //Optional
     *               Type = uint64 //Only uint64 supported
     *           }
     *           DeltaTime = { //Optional
     *               Type = uint64 //Only uint64 supported
     *           }
     *     }
     * }
     * If the SleepNature=Busy a Sleep::Busy will be used to wait for the 1/Frequency period to elapse, otherwise
     *  a Sleep::NoMore will be used.
     * @return true if SleepNature=Busy or SleepNature=Default
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Verifies that two, and only two, signal are set with the correct type.
     * @details Verifies that two, and only two, signal are set; that the signals are
     * 32 bits in size with a SignedInteger or UnsignedInteger type and that a Frequency > 0 was set in one of the two signals.
     * @param[in] data see DataSourceI::SetConfiguredDatabase
     * @return true if the rules above are met.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Gets the affinity of the thread which is going to be used to asynchronously wait for the time to elapse.
     * @return the affinity of the thread which is going to be used to asynchronously wait for the time to elapse.
     */
    const ProcessorType& GetCPUMask() const;

    /**
     * @brief Gets the stack size of the thread which is going to be used to asynchronously wait for the time to elapse.
     * @return the stack size of the thread which is going to be used to asynchronously wait for the time to elapse.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Gets the percentage of the time to sleep using the OS sleep (i.e. the non-busy Sleep).
     * @return the percentage of the time to sleep using the OS sleep (i.e. the non-busy Sleep).
     */
    uint32 GetSleepPercentage() const;

    /**
    * @brief Purges the DataSource
    */
    virtual void Purge(ReferenceContainer &purgeList);

private:

    /**
     * @brief The two supported sleep natures.
     */
    enum LinuxTimerSleepNature {
        Default = 0, Busy = 1
    };

    /**
     * @brief The non-busy sleep percentage. Valid only if LinuxTimerSleepNature == Busy
     */
    uint32 sleepPercentage;

    /**
     * @brief The selected sleep nature.
     */
    LinuxTimerSleepNature sleepNature;

    /**
     * @brief Current counter and timer
     */
    uint32 counterAndTimer[2];

    /**
     * @brief The semaphore for the synchronisation between the EmbeddedThread and the Synchronise method.
     */
    EventSem synchSem;

    /**
     * @brief The EmbeddedThread where the Execute method waits for the period to elapse.
     */
    SingleThreadService executor;

    /**
    *  @brief Number of ticks at the start of the cycle
    */
    uint64 startTimeTicks;

    /**
     * @brief Sleeping period in units of ticks.
     */
    uint64 sleepTimeTicks[2];

    /**
     * @brief Sleeping period.
     */
    uint32 timerPeriodUsecTime[2];

    /**
     * @brief Index of the function which has the signal that synchronises on this DataSourceI.
     */
    uint32 synchronisingFunctionIdx;

    /**
     * @brief True if this a synchronising data source
     */
    bool synchronising;

    /**
     * @brief The affinity of the thread that asynchronously generates the time.
     */
    ProcessorType cpuMask;

    /**
     * @brief The size of the stack of the thread that asynchronously generates the time.
     */
    uint32 stackSize;

    /**
     * @brief The execution mode.
     */
    uint32 executionMode;

    /**
     * @brief Reference to the time provider plugin
     */
    ReferenceT<TimeProvider> timeProvider;

    /**
     * @brief The absolute time
     */    
    uint64 absoluteTime;

    /**
     * @brief The delta time
     */
    uint64 deltaTime;

    /**
     * @brief The absolute time
     */
    uint64 absoluteTime_1;

    /**
     * @brief Number of ticks occuring in a us
     */
    float64 ticksPerUs;

    /**
     * @brief Phase of the generating signal
     */
    uint32 phase;

    /**
    * @brief Keeps old phase value
    */
    uint32 phaseBackup;

    /**
     * @brief Frequency of the currentState and nextState
     */
    float32 frequency[2];

    /**
     * @brief The RealTime application reference
     */
    ReferenceT<RealTimeApplication> rtApp;

    /**
     * @brief Rephase triggering signals.
     */
    uint8 trigRephase;
    uint8 trigRephase_1;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_LINUXTIMER_H_ */

