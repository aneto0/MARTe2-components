/**
 * @file EPICSCAClient.h
 * @brief Header file for class EPICSCAClient
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
 * @details This header file contains the declaration of the class EPICSCAClient
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSCACLIENT_H_
#define EPICSCACLIENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <cadef.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EmbeddedServiceI.h"
#include "EventSem.h"
#include "FastPollingMutexSem.h"
#include "MessageI.h"
#include "ReferenceContainer.h"
#include "SingleThreadService.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief A container of EPICSPV variables. Provides the threading context for the EPICS CA interface.
 * @details The configuration syntax is (names are only given as an example):
 *
 * <pre>
 * +EPICS_CA = {
 *   Class = EPICSInterface::EPICSCAClient
 *   StackSize = 1048576 //Optional the EmbeddedThread stack size. Default value is THREADS_DEFAULT_STACKSIZE * 4u
 *   CPUs = 0xff //Optional the affinity of the EmbeddedThread (where the EPICS context is attached).
 *   AutoStart = 0 //Optional. Default = 1. If false the service will only be started after receiving a Start message (see Start method).
 *   +PV_1 = {
 *      Class = EPICSPV //See class documentation of EPICSPV
 *      ...
 *   }
 *   +PV_2 = {
 *      Class = EPICSPV
 *      ...
 *   }
 * }
 * </pre>
 */
class EPICSCAClient: public ReferenceContainer, public EmbeddedServiceMethodBinderI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    EPICSCAClient();

    /**
     * @brief Destructor. Stops the embedded thread.
     */
    virtual ~EPICSCAClient();

    /**
     * @brief Initialises the ReferenceContainer and reads the thread parameters.
     * @return true if the ReferenceContainer and thread parameters are successfully initialised.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Provides the context to execute all the EPICS relevant calls.
     * @details Executes in the context of a spawned thread the following EPICS calls:
     * ca_context_create, ca_create_channel, ca_create_subscription, ca_clear_subscription,
     * ca_clear_event, ca_clear_channel, ca_detach_context and ca_context_destroy
     * @return ErrorManagement::NoError if all the EPICS calls return without any error.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @brief Registered as the ca_create_subscription callback function.
     * It calls HandlePVEvent every time a value is updated on a registered PV.
     */
    friend void EPICSCAClientEventCallback(struct event_handler_args args);

    /**
     * @brief Gets the thread stack size.
     * @return the thread stack size.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Gets the thread affinity.
     * @return the thread affinity.
     */
    uint32 GetCPUMask() const;

    /**
     * @brief Gets the embedded thread state.
     * @return the embedded thread state.
     */
    EmbeddedThreadI::States GetStatus();

    /**
     * @brief If the Stop was to be called from the destructor the Size() would already be == 0 and as consequence it would not
     * be possible to clean the EPICS resources when the state is BadTerminationStage
     * @details See ReferenceContainer::Purge
     */
    virtual void Purge(ReferenceContainer &purgeList);

    /**
     * @brief Start the embedded service it wasn't already started.
     * @return ErrorManagement::NoError if the service wasn't already started.
     */
    ErrorManagement::ErrorType Start();

private:

    /**
     * The EmbeddedThread where the ca_pend_event is executed.
     */
    SingleThreadService executor;

    /**
     * The CPU mask for the executor
     */
    uint32 cpuMask;

    /**
     * The stack size
     */
    uint32 stackSize;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSCACLIENT_H_ */
