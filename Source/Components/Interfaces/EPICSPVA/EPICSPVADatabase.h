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
#include "pv/channelProviderLocal.h"
#include "pv/pvDatabase.h"
#include "pv/serverContext.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"
#include "MessageI.h"
#include "MutexSem.h"
#include "ReferenceContainer.h"
#include "SingleThreadService.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A server of EPICSPVARecord elements.
 * @details A ReferenceContainer which offers the threading context to register any number EPICSPVARecord elements on an epics::pvDatabase::PVDatabase.
 *
 * The configuration syntax is  (names are only given as an example):
 * <pre>
 * +EPICSPVADatabase1 = {
 *   Class = EPICSPVA::EPICSPVADatabase
 *   StackSize = 1048576 //Optional the EmbeddedThread stack size. Default value is THREADS_DEFAULT_STACKSIZE * 4u
 *   CPUs = 0xff //Optional the affinity of the EmbeddedThread (where the EPICS context is attached).
 *   AutoStart = 0 //Optional. Default = 1. If false the service will only be started after receiving a Start message (see Start method).
 *   +Record1 = {
 *     Class = EPICSPVA::EPICSPVARecord
 *     Alias = "f4e::falcon::Fast::Record1" //Optional. If set this will be the record name (as opposed to the Object name).
 *     Structure = {//Use structured types for nested structures.
 *       A = {
 *         Type = uint32
 *       }
 *       B = {
 *         Type = AStructuredType //e.g. registered with IntrospectionStructure
 *       }
 *     }
 *   }
 *   +Record2 = {
 *     Class = EPICSPVA::EPICSPVARecord
 *     Structure = {
 *       value = {
 *         Type = AStructuredType2 //e.g. another structured type
 *         NumberOfElements = 1
 *       }
 *     }
 *   }
 * }
 * </pre>
 */
class EPICSPVADatabase: public ReferenceContainer, public EmbeddedServiceMethodBinderI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     * @post
     *    GetStackSize() = THREADS_DEFAULT_STACKSIZE * 4u;
     *    GetCPUMask() = 0xff;
     */
    EPICSPVADatabase ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EPICSPVADatabase();

    /**
     * @brief Initialises the ReferenceContainer and reads the thread parameters (see class documentation above).
     * @details If the AutoStart parameter is set to 1 the Start() method is called.
     * @return true if the ReferenceContainer and thread parameters are successfully initialised.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Provides the context to execute all the EPICS relevant calls (epics::pvAccess::startPVAServer, epics::pvDatabase::PVDatabase::getMaster(), CreatePVRecord).
     * @return ErrorManagement::NoError if all the EPICSPVA calls return without any error.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @brief Start the embedded service it wasn't already started.
     * @return ErrorManagement::NoError if the service wasn't already started.
     */
    ErrorManagement::ErrorType Start();

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
     * @brief Gets the pva server. This is required for the tests.
     * @return the pva server;
     */
    epics::pvAccess::ServerContext::shared_pointer GetServerContext() const;

    /**
     * @brief If the Stop was to be called from the destructor the Size() would already be == 0 and as consequence it would not
     * be possible to clean the EPICS resources when the state is BadTerminationStage
     * @details See ReferenceContainer::Purge
     * @param[in] purgeList See ReferenceContainer::Purge
     */
    virtual void Purge(ReferenceContainer &purgeList);

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

    /**
     * The PVDatabase
     */
    epics::pvDatabase::PVDatabasePtr master;

    /**
     * The channel provider
     */
    epics::pvDatabase::ChannelProviderLocalPtr channelProvider;

    /**
     * To avoid racing conditions at shutdown
     */
    MutexSem mux;

    /**
     * EPICS Variables needed to be added in the thread context.
     */
    bool allVariablesAdded;

    /**
     * Check if there was an error while adding the variables.
     */
    bool threadError;

    /**
     * True if the server is to be shutdown
     */
    bool shutdown;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSPVADATABASE_H_ */
