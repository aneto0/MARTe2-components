/**
 * @file DANSource.h
 * @brief Header file for class DANSource
 * @date 04/04/2017
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

 * @details This header file contains the declaration of the class DANSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DAN_DANSOURCE_H_
#define DAN_DANSOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DANStream.h"
#include "DataSourceI.h"
#include "MemoryMapAsyncOutputBroker.h"
#include "MemoryMapAsyncTriggerOutputBroker.h"
#include "MessageI.h"
#include "ProcessorType.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief A DataSourceI which allows to store signals in an ITER DAN database.
 *
 * @details Data written into this data source is temporarily stored in a circular buffer and
 * asynchronously flushed to the DAN database in the context of a separate thread.
 * This circular buffer can either be continuously stored or stored only when a given event occurs (see StoreOnTrigger below).
 *
 * This DataSourceI has the functions OpenStream and CloseStream registered as an RPC.
 *
 * The configuration syntax is (names are only given as an example):
 * <pre>
 * +DANSource_0 = {
 *     Class = DAN::DANSource
 *     NumberOfBuffers = 10 //Compulsory. Number of buffers in the circular buffer defined above. Each buffer is capable of holding a copy of all the DataSourceI signals.
 *     CPUMask = 15 //Compulsory. Affinity assigned to the threads responsible for asynchronously flush data into the DAN database.
 *     StackSize = 10000000 //Compulsory. Stack size of the thread above.
 *     DanBufferMultiplier = 4 //Compulsory. Number of buffers that the dan_publisher_publishSource_withDAQBuffer will use to buffer the data written by the DataSourceI
 *     StoreOnTrigger = 1 //Compulsory. If 0 all the data in the circular buffer is continuously stored. If 1 data is stored when the Trigger signal is 1 (see below).
 *     NumberOfPreTriggers = 2 //Compulsory iff StoreOnTrigger = 1.  Number of cycles to store before the trigger.
 *     NumberOfPostTriggers = 1 //Compulsory iff StoreOnTrigger = 1.  Number of cycles to store after the trigger.
 *     ICProgName = "MARTeApp.ex" //Optional. If set it will call dan_initLibrary_icprog with the specified name.
 *
 *     Signals = {
 *         Trigger = { //Compulsory when StoreOnTrigger = 1. Must be set in index 0 of the Signals node. When the value of this signal is 1 data will be stored into the DAN database. Shall not be added if StoreOnTrigger = 0.
 *             Type = 'uint8" //Type must be uint8
 *         }
 *         Time = { //Compulsory when StoreOnTrigger = 1. Can be stored in any index, but TimeSignal must be set = 1. Shall not be added if StoreOnTrigger = 0.
 *             Type = "uint32" //Type must be uint32, int32, uint64 or int64
 *             TimeSignal = 1 //When set, this signal will be considered as the time source against which all signals will be stored.
 *                            //When not set (i.e. when no TimeSignal is defined) the time will computed as the absolute time at the latest state change (given by tcn_get_time) + the number of cycles multiplied by the signal Period (or 1/SamplingFrequency)
 *             AbsoluteTime = 0 //Optional. If 1 the Type shall be uint64 or int64 and the absolute time given by this signal will be used to time stamp the samples.
 *                              //          If 0 and TimeSignal = 1 the time will computed as the absolute time at the latest state change (given by tcn_get_time) + the time given by this time signal (which is assumed to be relative).
 *         }
 *         SignalUInt16F = { //As many as required.
 *             Period = 1e-3 //Compulsory if SamplingFrequency not set. Period between signal samples.
 *             SamplingFrequency = 1000 //Compulsory if Period not set. 1/Period between signal samples.
 *         }
 *         ...
 *     }
 * }
 * </pre>
 * A DANStream instance will be created for every different signal_type/sampling_frequency pair. Signals must be listed in the same order as they appear on the DAN xml.
 */
class DANSource: public DataSourceI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor.
     * @details Initialises all the optional parameters as described in the class description.
     * Registers the RPC OpenStream and CloseStream callback functions.
     */
DANSource    ();

    /**
     * @brief Destructor.
     * @details Closes all the DANStream instances.
     */
    virtual ~DANSource();

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
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @details Only OutputSignals are supported.
     * @return MemoryMapAsyncOutputBroker if storeOnTrigger == 0, MemoryMapAsyncTriggerOutputBroker otherwise.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @return false.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @details If storeOnTrigger == 0 it adds a MemoryMapAsyncOutputBroker instance to
     *  the inputBrokers, otherwise it adds a MemoryMapAsyncTriggerOutputBroker instance to the outputBrokers.
     * @pre
     *   GetNumberOfFunctions() == 1u
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief Calls PutData on all the DANStream instances.
     * @return true if the DANStream::PutData returns true on all the streams.
     */
    virtual bool Synchronise();

    /**
     * @brief Calls DANStream::Reset on all the DANStream instances.
     * @details If AbsoluteTime = 0 in the configuration entry, calls DANStream::SetAbsoluteStartTime with the current time given by tcn_get_time.
     * @return ok if tcn_get_time succeeds.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Loads and verifies the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Final verification of all the parameters and setup of the DANStream instances.
     * @details This method verifies that all the parameters (e.g. number of samples) requested by the GAMs interacting with this DataSource
     *  are valid and consistent with the parameters set during the initialisation phase.
     * In particular the following conditions shall be met:
     * - If relevant, the Trigger signal shall have type uint8
     * - If relevant, the Time signal shall have type uint32, int32, uint64 or int64
     * - The number of samples of all the DAN signals is one.
     * - For every signal either the Period or the SamplingFrequency is specified.
     * - At least one DAN signal (apart from the eventual Trigger and Time signal) is set.
     *
     * A DANStream instance will be created for every different signal_type/sampling_frequency pair.
     * @return true if all the parameters are valid.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Calls dan_publisher_openStream on every DANStream.
     * @return true if all dan_publisher_openStream return >= 0.
     */
    ErrorManagement::ErrorType OpenStream();

    /**
     * @brief Calls dan_publisher_closeStream on every DANStream.
     * @return true if all dan_publisher_closeStream return 0.
     */
    ErrorManagement::ErrorType CloseStream();

    /**
     * @brief Gets the affinity of the thread which is going to be used to asynchronously store the data in the DAN database.
     * @return the affinity of the thread which is going to be used to asynchronously store the data in the DAN database.
     */
    const ProcessorType& GetCPUMask() const;

    /**
     * @brief Gets the number of buffers in the circular buffer.
     * @return the number of buffers in the circular buffer.
     */
    uint32 GetNumberOfBuffers() const;

    /**
     * @brief Gets the number of post configured buffers in the circular buffer.
     * @return the number of post configured buffers in the circular buffer.
     */
    uint32 GetNumberOfPostTriggers() const;

    /**
     * @brief Gets the number of pre configured buffers in the circular buffer.
     * @return the number of pre configured buffers in the circular buffer.
     */
    uint32 GetNumberOfPreTriggers() const;

    /**
     * @brief Gets the stack size of the thread which is going to be used to asynchronously store the data in the DAN database.
     * @return the stack size of the thread which is going to be used to asynchronously store the data in the DAN database.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Returns true if the data is going to be stored in DAN is based on the occurrence of an external trigger.
     * @return true if the data is going to be stored in DAN is based on a trigger event.
     */
    bool IsStoreOnTrigger() const;

    /**
     * @brief Checks is AbsoluteTime = 1 was set in the configuration entry.
     * @return true if AbsoluteTime = 1 was set in the TimeSignal of the configuration entry.
     */
    bool IsAbsoluteTime() const;

    /**
     * @brief Returns the index of the signal which is going to provide the time if the data is based on an external trigger.
     * @return the index of the signal which is going to provide the time if the data is based on an external trigger.
     */
    int32 GetTimeSignalIdx() const;

    /**
     * @brief Gets the latest set absolute start time in nano-seconds (see PrepareNextState)
     * @return the latest set absolute start time in nano-seconds
     */
    uint64 GetAbsoluteStartTime() const;

    /**
     * @brief Gets the number of buffers that the dan_publisher_publishSource_withDAQBuffer will use to buffer the data written by the DataSourceI
     * @return the number of buffers that the dan_publisher_publishSource_withDAQBuffer will use .
     */
    uint32 GetDANBufferMultiplier() const;

    /**
     * @see DataSourceI::Purge()
     */
    virtual void Purge(ReferenceContainer &purgeList);

private:

    /**
     * True if the data is only to be stored in DAN following a trigger.
     */
    bool storeOnTrigger;

    /**
     * Number of pre buffers when StoreOnTrigger == 1.
     */
    uint32 numberOfPreTriggers;

    /**
     * Number of post buffers when StoreOnTrigger == 1.
     */
    uint32 numberOfPostTriggers;

    /**
     * Number of buffers in the circular buffer.
     */
    uint32 numberOfBuffers;

    /**
     * The index of the time signal in the signal list.
     */
    int32 timeSignalIdx;

    /**
     * The affinity of the thread that asynchronously flushes data into MDSplus.
     */
    ProcessorType cpuMask;

    /**
     * The size of the stack of the thread that asynchronously flushes data into DAN.
     */
    uint32 stackSize;

    /**
     * Stores the configuration information received at Initialise.
     */
    ConfigurationDatabase originalSignalInformation;

    /**
     * Filter to receive the RPC which allows to change the pulse number.
     */
    ReferenceT<RegisteredMethodsMessageFilter> filter;

    /**
     * Memory map asynchronous broker.
     */
    ReferenceT<MemoryMapAsyncOutputBroker> brokerAsyncOutput;

    /**
     * Memory map asynchronously triggered broker.
     */
    ReferenceT<MemoryMapAsyncTriggerOutputBroker> brokerAsyncTrigger;

    /**
     * dan_DataCore singleton that is required by all the DANStream instances.
     */
    //static dan_DataCore danDataCore;

    /**
     * Number of internal buffers (number of signal copies) that the DAN library should allocate.
     */
    uint32 danBufferMultiplier;

    /**
     * The signal sampling rate.
     */
    float64 samplingRate;

    /**
     * List of DANStream instances (one for each type/samplingRate pair).
     */
    DANStream **danStreams;

    /**
     * Number of of DANStream instances (one for each type/samplingRate pair).
     */
    uint32 nOfDANStreams;

    /**
     * True if the time stamps should be marked against an absolute time.
     */
    bool useAbsoluteTime;

    /**
     * Holds the time as a uint32. This variable is shared by the DANStream instances.
     */
    uint32 timeUInt32;

    /**
     * Holds the time as a uint64. This variable is shared by the DANStream instances.
     */
    uint64 timeUInt64;

    /**
     * Holds the trigger to be used by the MemoryMapAsyncTriggerOutputBroker.
     */
    uint8 trigger;

    /**
     * The latest set absolute start time
     */
    uint64 absoluteStartTime;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DAN_DANSOURCE_H_ */

