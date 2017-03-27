/**
 * @file EPICSCAClient.h
 * @brief Header file for class EPICSCAClient
 * @date 23/03/2017
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
 * +EPICS_CA = {
 *   Class = EPICSInterface::EPICSCAClient
 *   StackSize = 1048576 //Optional the EmbeddedThread stack size. Default value is THREADS_DEFAULT_STACKSIZE * 4u
 *   CPUs = 0xff //Optional the affinity of the EmbeddedThread (where the EPICS context is attached).
 *   +PV_1 = {
 *      Class = EPICSPV //See class documentation of EPICSPV
 *      ...
 *   }
 *   +PV_2 = {
 *      Class = EPICSPV
 *      ...
 *   }
 * }
 */
class EPICSCAClient: public ReferenceContainer, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
EPICSCAClient    ();

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
     * @brief TODO
     */
    virtual ErrorManagement::ErrorType Execute(const ExecutionInfo & info);

    /**
     * @brief TODO
     */
    friend void EPICSCAClientEventCallback(struct event_handler_args args);

    /**
     * @brief TODO
     */
    uint32 GetStackSize() const;

    /**
     * @brief TODO
     */
    uint32 GetCPUMask() const;
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

