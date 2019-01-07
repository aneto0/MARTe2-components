/**
 * @file EPICSRPCServer.h
 * @brief Header file for class EPICSRPCServer
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

 * @details This header file contains the declaration of the class EPICSRPCServer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSRPCSERVER_H_
#define EPICSPVA_EPICSRPCSERVER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pv/rpcServer.h"

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
 * @brief Register under the same threading context a list of epics::pvAccess::RPCService .
 * @details A ReferenceContainer of MARTe Objects that also implement the epics::pvAccess::RPCService interface.
 *
 * The configuration syntax is  (names are only given as an example):
 * <pre>
 * +EPICSPVARPC = {
 *   Class = EPICSPVA::EPICSRPCServer
 *   StackSize = 1048576 //Optional the EmbeddedThread stack size. Default value is THREADS_DEFAULT_STACKSIZE * 4u
 *   CPUs = 0xff //Optional the affinity of the EmbeddedThread (where the EPICS context is attached).
 *   AutoStart = 0 //Optional. Default = 1. If false the service will only be started after receiving a Start message (see Start method).
 *   +Service1 = {
 *      Class = EPICSPVA::EPICSObjectRegistryDatabaseService
 *      ...
 *   }
 *   +PV_2 = {
 *      Class = ...//Inherits from EPICSRPCService
 *      ...
 *   }
 * }
 * </pre>
 */
class EPICSRPCServer: public ReferenceContainer, public EmbeddedServiceMethodBinderI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     * @post
     *    GetStackSize() = THREADS_DEFAULT_STACKSIZE * 4u;
     *    GetCPUMask() = 0xff;
     */
    EPICSRPCServer();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EPICSRPCServer();

    /**
     * @brief Initialises the ReferenceContainer and reads the thread parameters (see class documentation above).
     * @details If the AutoStart parameter is set to 1 the Start() method is called.
     * @return true if the ReferenceContainer and thread parameters are successfully initialised.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief If the Stop was to be called from the destructor the Size() would already be == 0 and as consequence it would not
     * be possible to clean the EPICSPVA resources when the state is BadTerminationStage
     * @details See ReferenceContainer::Purge
     */
    virtual void Purge(ReferenceContainer &purgeList);

    /**
     * @brief Provides the context to execute all the EPICSPVA relevant calls
     * @details Executes in the context of a spawned thread the EPICSPVA related to the epics::pvAccess::RPCServer (registerService, run, ...).
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
    std::shared_ptr<epics::pvAccess::RPCServer> rpcServer;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSRPCSERVER_H_ */
