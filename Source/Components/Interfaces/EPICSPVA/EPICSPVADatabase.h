/**
 * @file EPICSPVADatabase.h
 * @brief Header file for class EPICSPVADatabase
 * @date 12/06/2018
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

 * @details This header file contains the declaration of the class EPICSPVADatabase
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSPVADATABASE_H_
#define EPICSPVA_EPICSPVADATABASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <pv/serverContext.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EmbeddedServiceMethodBinderI.h"
#include "MessageI.h"
#include "ReferenceContainer.h"
#include "SingleThreadService.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief TODO
 */
class EPICSPVADatabase: public ReferenceContainer, public EmbeddedServiceMethodBinderI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief TODO
     */
    EPICSPVADatabase();

    /**
     * @brief TODO
     */
    virtual ~EPICSPVADatabase();

    /**
     * @brief TODO
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief If the Stop was to be called from the destructor the Size() would already be == 0 and as consequence it would not
     * be possible to clean the EPICS resources when the state is BadTerminationStage
     * @details See ReferenceContainer::Purge
     */
    virtual void Purge(ReferenceContainer &purgeList);

    /**
     * @brief Provides the context to execute all the EPICS relevant calls
     * TODO.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

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

    /**
     * The EPICS server context
     */
    epics::pvAccess::ServerContext::shared_pointer serverContext;

};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSPVADATABASE_H_ */
