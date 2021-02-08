/**
 * @file EPICSCAOutput.h
 * @brief Header file for class EPICSCAOutput
 * @date 04/02/2021
 * @author Andre Neto
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class EPICSCAOutput
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSCAOUTPUT_H_
#define EPICSCAOUTPUT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <cadef.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EPICSCAInput.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"
#include "FastPollingMutexSem.h"
#include "MessageI.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
//Maximum size that a PV name may have

/**
 * @brief A DataSource which allows to output data into any number of PVs using the EPICS channel access client protocol.
 * Data is asynchronously ca_put in the context of a different thread (w.r.t. to the real-time thread).
 *
 * The configuration syntax is (names are only given as an example):
 *
 * <pre>
 * +EPICSCAOutput_1 = {
 *     Class = EPICSCA::EPICSCAOutput
 *     StackSize = 1048576 //Optional the EmbeddedThread stack size. Default value is THREADS_DEFAULT_STACKSIZE * 4u
 *     CPUs = 0xff //Optional the affinity of the EmbeddedThread (where the EPICS context is attached).
 *     IgnoreBufferOverrun = 1 //Optional. If true no error will be triggered when the thread that writes into EPICS does not consume the data fast enough.
 *     NumberOfBuffers = 10 //Compulsory. Number of buffers in a circular buffer that asynchronously writes the PV values. Each buffer is capable of holding a copy of all the DataSourceI signals.
 *     Signals = {
 *          PV1 = { //At least one shall be defined
 *             PVName = My::PV1 //Compulsory. Name of the PV.
 *             Type = uint32 //Compulsory. Supported types are char8[40], string[40], uint8, int8, uint16, int16, int32, uint32, float32 and float64
 *          }
 *          ...
 *     }
 * }
 *
 * </pre>
 */
class EPICSCAOutput: public DataSourceI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP.
     */
    EPICSCAOutput();

    /**
     * @brief Destructor.
     * @details Calls the ca_clear_channel method on the pvs, calls Free on the pvs and delete on the signalFlag.
     */
    virtual ~EPICSCAOutput();

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
     * @return MemoryMapAsyncOutputBroker.
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
     * @details adds a memory MemoryMapOutputBroker instance to the outputBrokers
     * @return true.
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
     * @brief Loads and verifies the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Final verification of all the parameters. Setup of the memory required to hold all the signals.
     * @details This method verifies that all the parameters requested by the GAMs interacting with this DataSource
     *  are valid and consistent with the parameters set during the initialisation phase.
     * In particular the following conditions shall be met:
     * - All the signals have the PVName defined
     * - All the signals have one of the following types: uint32, int32, float32 or float64.
     * @return true if all the parameters are valid and the conditions above are met.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Gets the affinity of the thread which is going to be used to asynchronously write data with ca_put.
     * @return the affinity of the thread which is going to be used to asynchronously write data with ca_put.
     */
    uint32 GetCPUMask() const;

    /**
     * @brief Gets the stack size of the thread which is going to be used to asynchronously write data with ca_put.
     * @return the stack size of the thread which is going to be used to asynchronously write data with ca_put.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Gets the number of buffers in the circular buffer that asynchronously writes the PV values.
     * @return the number of buffers in the circular buffer that asynchronously writes the PV values.
     */
    uint32 GetNumberOfBuffers() const;

    /**
     * @brief Provides the context to execute all the EPICS ca_put calls.
     * @details Executes in the context of the MemoryMapAsyncOutputBroker thread the following EPICS calls:
     * ca_context_create, ca_create_channel, ca_create_subscription, ca_clear_subscription,
     * ca_clear_event, ca_clear_channel, ca_detach_context and ca_context_destroy
     * @return true if all the EPICS calls return without any error.
     */
    virtual bool Synchronise();

    /**
     * @brief Gets if buffer overruns is being ignored (i.e. the consumer thread which writes into EPICS is not consuming the data fast enough).
     * @return if true no error is to be triggered when there is a buffer overrun.
     */
    bool IsIgnoringBufferOverrun() const;

    /**
     * @brief Asynchronous Channel Access Put.
     * @details Performs the following EPICS calls: ca_create_channel and then, depending on pvType, 
     * ca_put or ca_array_put. Occuring errors are reported using MARTe2 REPORT_ERROR facility.
     * @return The error err if occurred in any of the described calls.
     */
    ErrorManagement::ErrorType AsyncCaPut(StreamString pvName, StreamString pvVal);

private:
    /**
     * List of PVs.
     */
    PVWrapper *pvs;

    /**
     * The CPU mask for the executor
     */
    uint32 cpuMask;

    /**
     * The stack size
     */
    uint32 stackSize;

    /**
     * Stores the configuration information received at Initialise.
     */
    ConfigurationDatabase originalSignalInformation;

    /**
     * The number of buffers for the circular buffer that flushes data into EPICS
     */
    uint32 numberOfBuffers;

    /**
     * True once the epics thread context is set
     */
    bool threadContextSet;

    /**
     * If true no error will be triggered when the data cannot be consumed by the thread doing the caputs.
     */
    uint32 ignoreBufferOverrun;
    
    /**
     * Flags to the signals.
     */
    uint8 *signalFlag;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSCAOUTPUT_H_ */

