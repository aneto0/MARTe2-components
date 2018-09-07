/**
 * @file EPICSObjectRegistryDatabaseService.h
 * @brief Header file for class EPICSObjectRegistryDatabaseService
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

 * @details This header file contains the declaration of the class EPICSObjectRegistryDatabaseService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSOBJECTREGISTRYDATABASESERVICE_H_
#define EPICSPVA_EPICSOBJECTREGISTRYDATABASESERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pv/rpcService.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "ReferenceT.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Query the contents of the ObjectRegistryDatabase using an epics::pvAccess::RPCService.
 * @details To be used with an EPICSRPCServer.
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
 *   }
 * }
 * </pre>
 */
namespace MARTe {
class epicsShareClass EPICSObjectRegistryDatabaseService: public virtual epics::pvAccess::RPCService, public Object {
public:
    POINTER_DEFINITIONS(EPICSObjectRegistryDatabaseService);
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    EPICSObjectRegistryDatabaseService();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EPICSObjectRegistryDatabaseService();

    /**
     * @brief Returns the ObjectRegistryDatabase tree as a PVStructure.
     * @param[in] args not used.
     * @return a snapshot of the ObjectRegistryDatabase tree as a PVStructure.
     */
    epics::pvData::PVStructurePtr request(epics::pvData::PVStructure::shared_pointer const & args) throw (epics::pvAccess::RPCRequestException);

private:
    /**
     * @brief Helper method which recursively creates an epics::pvData::PVStructure from the ObjectRegistryDatabase.
     * @param[in] fieldBuilder the FieldBuilder that is used to populate the structure.
     * @param[in] rc a reference to the node being currently populated (the ObjectRegistryDatabase root in the first call).
     */
    void GetEPICSStructure(epics::pvData::FieldBuilderPtr & fieldBuilder, ReferenceContainer &rc);

    /**
     * @brief Helper method which recursively populates an epics::pvData::PVStructure (generated with GetEPICSStructure) from the ObjectRegistryDatabase.
     * @param[in] pvStructure the PVStructure created with GetEPICSStructure.
     * @param[in] rc a reference to the node being currently populated (the ObjectRegistryDatabase root in the first call).
     * @param[in] fullName the name, including the full path, to the current node.
     */
    void FillEPICSStructure(epics::pvData::PVStructurePtr & pvStructure, ReferenceContainer &rc, StreamString fullName);

};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSOBJECTREGISTRYDATABASESERVICE_H_ */
