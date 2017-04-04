/**
 * @file DANSource.h
 * @brief Header file for class DANSource
 * @date 04/04/2017
 * @author Andre Neto
 * @author Lana Abadie
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
#include "DataSourceI.h"
#include "ProcessorType.h"
#include "MemoryMapAsyncTriggerOutputBroker.h"
#include "MessageI.h"
#include "RegisteredMethodsMessageFilter.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * TODO
 */
namespace MARTe {
class DANSource: public DataSourceI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief TODO.
     */
DANSource    ();

    /**
     * @brief Destructor.
     * @details TODO.
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
     * @brief TODO.
     */
    virtual bool Synchronise();

    /**
     * @brief See DataSourceI::PrepareNextState. NOOP.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Loads and verifies the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief TODO.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief TODO.
     */
    ErrorManagement::ErrorType OpenStream();

    /**
     * @brief TODO.
     */
    ErrorManagement::ErrorType CloseStream();

    /**
     * @brief Gets the affinity of the thread which is going to be used to asynchronously store the data in the MDS plus database.
     * @return the affinity of the thread which is going to be used to asynchronously store the data in the MDS database.
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
     * @brief Gets the stack size of the thread which is going to be used to asynchronously store the data in the MDS plus database.
     * @return the stack size of the thread which is going to be used to asynchronously store the data in the MDS plus database.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Returns true if the data is going to be stored in MDS plus based on the occurrence of an external trigger.
     * @return true if the data is going to be stored in MDS plus based on a trigger event.
     */
    bool IsStoreOnTrigger() const;

    /**
     * @brief Returns the index of the signal which is going to provide the time if the data is based on an external trigger.
     * @return the index of the signal which is going to provide the time if the data is based on an external trigger.
     */
    int32 GetTimeSignalIdx() const;

private:

    /**
     * The tree pulse number.
     */
    int32 pulseNumber;

    /**
     * True if the data is only to be stored in MDS plus following a trigger.
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
     * Offset of each signal in the dataSourceMemory
     */
    uint32 *offsets;

    /**
     * Memory holding all the signals that are to be stored, for each cycle, in MDSplus
     */
    char8 *dataSourceMemory;

    /**
     * The affinity of the thread that asynchronously flushes data into MDSplus.
     */
    ProcessorType cpuMask;

    /**
     * The size of the stack of the thread that asynchronously flushes data into MDSplus.
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
     * The asynchronous triggered broker that provides the interface between the GAMs and the MDS+ memory
     */
    MemoryMapAsyncTriggerOutputBroker *brokerAsyncTrigger;

    /**
     * TODO
     */
    static dan_DataCore danDataCore;

    /**
     * TODO
     */
    dan_Source danSource;

    /**
     * TODO
     */
    uint32 danBufferMultiplier;

    /**
     * TODO
     */
    float64 samplingRate;

    /**
     * TODO
     */
    uint32 totalSignalMemory;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_DAN_DANSOURCE_H_ */

