/**
 * @file EPICSPVAInput.h
 * @brief Header file for class EPICSPVAInput
 * @date 11/06/2018
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

 * @details This header file contains the declaration of the class EPICSPVAInput
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVAINPUT_H_
#define EPICSPVAINPUT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EPICSPVAChannelWrapper.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"
#include "MemoryDataSourceI.h"
#include "MultiThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief A DataSource which allows to retrieve data from any number of records using the EPICS PVA protocol.
 * Data is asynchronously retrieved using pvac::MonitorSync (see EPICAPVAChannelWrapper::Monitor).
 *
 * One thread will be created for each record being monitored (using a MultiThreadService)
 *
 * Each signal root name defines the name of the record (signal).
 * The configuration syntax is (names are only given as an example):
 *
 * Note that strings shall be specified with Type = string (also inside structured types).
 *
 * The DataSource can be used as a synchronisation point (with at most one channel). The signal must have the property
 * 'Frequency' setup to the value at which the values are expected to be produced.
 *
 * <pre>
 * +EPICSPVAInput_1 = {
 *     Class = EPICSPVADataSource::EPICSPVAInput
 *     StackSize = 1048576 //Optional the EmbeddedThread stack size. Default value is THREADS_DEFAULT_STACKSIZE * 4u
 *     CPUs = 0xff //Optional the affinity of the EmbeddedThread which actually performs the PVA monitoring.
 *     Signals = {
 *         RecordIn1Value = {//Record name if the Alias field is not set
 *             Alias = "alternative::channel::name"
 *             Field = "value" //If not set "value" is assumed
 *             Type = MyStruct1 //See e.g. InstrospectionStructure
 *         }
 *         RecordIn1Status = {//Record name if the Alias field is not set
 *             Alias = "alternative::channel::name" //Note that this is the same record as RecordIn1Value
 *             Field = "value"
 *             Type = uint32
 *         }
 *         RecordIn2 = {
 *             Field = "test"
 *             Type = MyStruct3
 *         }
 *         ...
 *     }
 * }
 *
 * </pre>
 */
class EPICSPVAInput: public MemoryDataSourceI, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP.
     */
EPICSPVAInput    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EPICSPVAInput();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @details Only InputSignals are supported.
     * @return MemoryMapInputBroker.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

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
     * @brief Final verification of all the parameters. Setup of the memory required to hold all the signals.
     * @details This method verifies that all the parameters requested by the GAMs interacting with this DataSource
     *  are valid and consistent with the parameters set during the initialisation phase.
     * In particular the following conditions shall be met:
     * - All the signals have one and only one sample.
     * @return true if all the parameters are valid and the conditions above are met.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Calls EPICSPVAChannelWrapper::Setup and start the threading service.
     * @details see MemoryDataSourceI::AllocateMemory
     * @return true if the EPICSPVAChannelWrapper::Setup was successful and the service started.
     */
    virtual bool AllocateMemory();

    /**
     * @brief Gets the affinity of the thread which is going to be used to asynchronously read data from the pvac::MonitorSync.
     * @return the the affinity of the thread which is going to be used to asynchronously read data from the pvac::MonitorSync.
     */
    uint32 GetCPUMask() const;

    /**
     * @brief Gets the stack size of the thread which is going to be used to asynchronously read data from the pvac::MonitorSync.
     * @return the stack size of the thread which is going to be used to asynchronously read data from the pvac::MonitorSync.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Provides the context to execute pvac::MonitorSync::wait.
     * @return ErrorManagement::NoError if the pvac::MonitorSync::wait and pvac::MonitorSync::poll calls return without any error.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @brief See DataSourceI::Synchronise.
     * @return true if being used as a synchronisation point and if the real-time thread successfully waits on the semaphore.
     */
    virtual bool Synchronise();

private:

    /**
     * One channel per signal at the root level.
     */
    EPICSPVAChannelWrapper *channelList;

    /**
     * Number of records (channels).
     */
    uint32 numberOfChannels;

    /**
     * The CPU mask for the executor
     */
    uint32 cpuMask;

    /**
     * The stack size
     */
    uint32 stackSize;

    /**
     * The EmbeddedThread where the monitor is executed (one thread per record).
     */
    MultiThreadService executor;

    /**
     * @brief The semaphore for the synchronisation between the EmbeddedThread and the Synchronise method.
     */
    EventSem synchSem;


};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVADATASOURCE_H_ */

