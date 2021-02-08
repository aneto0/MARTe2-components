/**
 * @file EPICSCAInput.h
 * @brief Header file for class EPICSCAInput
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
 * @details This header file contains the declaration of the class EPICSCAInput
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSCAINPUT_H_
#define EPICSCAINPUT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <cadef.h>

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
 * Maximum size that a PV name may have
 */
/*lint -esym(551, MARTe::PV_NAME_MAX_SIZE) the symbol is used to define the size of PVWrapper below*/
const uint32 PV_NAME_MAX_SIZE = 64u;

/**
 * Wraps a PV
 */
struct PVWrapper {

    /**
     * The channel identifier
     */
    chid pvChid;

    /**
     * The event identifier
     */
    evid pvEvid;

    /**
     * The PV type
     */
    chtype pvType;

    /**
     * The memory of the signal associated to this channel
     */
    void *memory;

    /**
     * The number of elements > 0
     */
    uint32 numberOfElements;

    /**
     * The memory size
     */
    uint32 memorySize;

    /**
     * The PV name
     */
    char8 pvName[PV_NAME_MAX_SIZE];

    /**
     * The type descriptor
     */
    TypeDescriptor td;
};

/**
 * @brief A DataSource which allows to retrieved data from any number of PVs using the EPICS channel access client protocol.
 * Data is asynchronously retrieved using ca_create_subscriptions in the context of a different thread (w.r.t. to the real-time thread).
 *
 * The configuration syntax is (names are only given as an example):
 *
 * <pre>
 * +EPICSCAInput_1 = {
 *     Class = EPICSCA::EPICSCAInput
 *     StackSize = 1048576 //Optional the EmbeddedThread stack size. Default value is THREADS_DEFAULT_STACKSIZE * 4u
 *     CPUs = 0xff //Optional the affinity of the EmbeddedThread (where the EPICS context is attached).
 *     Signals = {
 *          PV1 = { //At least one shall be defined
 *             PVName = My::PV1 //Compulsory. Name of the PV.
 *             Type = uint32 //Compulsory. Supported types are char8[40], string[40], uint8, int8, uint16, int16, int32, uint32, uint64, int64, float32 and float64
 *             NumberOfElements = 1 //Arrays also supported
 *          }
 *          ...
 *     }
 * }
 *
 * </pre>
 */
class EPICSCAInput: public DataSourceI, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP.
     */
    EPICSCAInput();

    /**
     * @brief Destructor.
     * @details Stops the thread executor, calls Free on the pvs memory and delete on pvs.
     */
    virtual ~EPICSCAInput();

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
     * @details Only InputSignals are supported.
     * @return MemoryMapInputBroker.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details adds a memory MemoryMapInputBroker instance to the inputBrokers
     * @return true.
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
     * @brief Gets the affinity of the thread which is going to be used to asynchronously read data from the ca_create_subscription.
     * @return the the affinity of the thread which is going to be used to asynchronously read data from the ca_create_subscription.
     */
    uint32 GetCPUMask() const;

    /**
     * @brief Gets the stack size of the thread which is going to be used to asynchronously read data from the ca_create_subscription.
     * @return the stack size of the thread which is going to be used to asynchronously read data from the ca_create_subscription.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Provides the context to execute all the EPICS relevant calls.
     * @details Executes in the context of a spawned thread the following EPICS calls:
     * ca_context_create, ca_create_channel, ca_create_subscription, ca_clear_subscription,
     * ca_clear_event, ca_clear_channel, ca_detach_context and ca_context_destroy
     * @return ErrorManagement::NoError if all the EPICS calls return without any error.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @brief See DataSourceI::Synchronise.
     * @return false.
     */
    virtual bool Synchronise();

    /**
     * @brief Registered as the ca_create_subscription callback function.
     * It calls updates the memory of the corresponding PV variable.
     */
    friend void EPICSCAInputEventCallback(struct event_handler_args args);

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
     * The EmbeddedThread where the ca_pend_event is executed.
     */
    SingleThreadService executor;

    /**
     * Stores the configuration information received at Initialise.
     */
    ConfigurationDatabase originalSignalInformation;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSCAINPUT_H_ */

