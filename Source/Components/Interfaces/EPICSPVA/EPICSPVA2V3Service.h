/**
 * @file EPICSPVA2V3Service.h
 * @brief Header file for class EPICSPVA2V3Service
 * @date 18/06/2018
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

 * @details This header file contains the declaration of the class EPICSPVA2V3Service
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVAMESSAGEI_H_
#define EPICSPVAMESSAGEI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pv/rpcService.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EPICSCAClient.h"
#include "EPICSPVAStructureDataI.h"
#include "EPICSRPCService.h"
#include "MessageI.h"
#include "Object.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief TODO
 *
 * To be used with an EPICSRPCServer.
 * The configuration syntax is  (names are only given as an example):
 * <pre>
 * +EPICSPVARPC = {
 *   Class = EPICSPVA::EPICSRPCServer
 *   StackSize = 1048576 //Optional the EmbeddedThread stack size. Default value is THREADS_DEFAULT_STACKSIZE * 4u
 *   CPUs = 0xff //Optional the affinity of the EmbeddedThread (where the EPICS context is attached).
 *   AutoStart = 0 //Optional. Default = 1. If false the service will only be started after receiving a Start message (see Start method).
 *   +Service1 = {
 TODO
 *   }
 * }
 * </pre>
 */
namespace MARTe {
class EPICSPVA2V3Service: public EPICSRPCService, public ReferenceContainer, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    EPICSPVA2V3Service();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EPICSPVA2V3Service();

    /**
     * @brief TODO
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief TODO
     */
    epics::pvData::PVStructurePtr request(epics::pvData::PVStructure::shared_pointer const & args);
private:

    /**
     * @brief TODO
     */
    bool RegisterPVs(ReferenceT<ReferenceContainer> rc, ReferenceT<EPICSCAClient> caClient);

    /**
     * @brief TODO
     */
    bool CAPutAll(StructuredDataI &pvStruct, StreamString currentNodeName, bool ignoreRootName = false);

    /**
     * @brief TODO
     */
    bool ComputeCRC(StructuredDataI &pvStruct, StreamString currentNodeName, uint32 &chksum, bool ignoreRootName = false);

    /**
     * @brief TODO
     */
    ReferenceT<ReferenceContainer> structureContainer;

    /**
     * @brief TODO
     */
    ReferenceT<EPICSPVAStructureDataI> epicsPVAStructure;

    /**
     * @brief TODO
     */
    uint32 seed;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVAMESSAGEI_H_ */
