/**
 * @file EPICSPVAMessageI.h
 * @brief Header file for class EPICSPVAMessageI
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

 * @details This header file contains the declaration of the class EPICSPVAMessageI
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
#include "EPICSRPCService.h"
#include "MessageI.h"
#include "Object.h"
#include "ReferenceT.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Message interface through an epics::pvAccess::RPCService.
 * @details The PVStructure received from pvaccess shall encode a valid MARTe message configuration, which may contain any arbitrary payload.
 * The message will be sent using the standard MARTe messaging mechanism. The + symbol in the configuration nodes should be replaced by the _ character (+ is not supporte in EPICSPVA).
 *
 * If a reply is requested (only directly reply currently supported), a PVStructure will be returned with the contents of the StructuredDataI provided by Message::Get(0).
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
 *      Class = EPICSPVA::EPICSPVAMessageI
 *   }
 * }
 * </pre>
 */
namespace MARTe {
class EPICSPVAMessageI: public EPICSRPCService, public Object, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    EPICSPVAMessageI();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EPICSPVAMessageI();

    /**
     * @brief Returns the ObjectRegistryDatabase tree as a PVStructure.
     * @param[in] args the PVStructure that will be loaded as a MARTe Message.
     * @return if reply is expected a PVStructure will be returned with the contents of the StructuredDataI provided by Message::Get(0)..
     */
    virtual epics::pvData::PVStructurePtr request(epics::pvData::PVStructure::shared_pointer const & args);

};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVAMESSAGEI_H_ */
