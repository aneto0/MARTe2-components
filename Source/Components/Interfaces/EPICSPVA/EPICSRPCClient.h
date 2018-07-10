/**
 * @file EPICSRPCClient.h
 * @brief Header file for class EPICSRPCClient
 * @date 25/06/2018
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

 * @details This header file contains the declaration of the class EPICSRPCClient
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSRPCCLIENT_H_
#define EPICSPVA_EPICSRPCCLIENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pv/rpcService.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "QueuedMessageI.h"
#include "ReferenceT.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief EPICS PVA RPC client.
 * @details Messages sent to this component are relayed as PVStructure through an epics::pvAccess::RPCClient.
 *
 * The name of the Message is expected to be the destination to be set on the epics::pvAccess::RPCClient.
 *
 * The first element of the Message is expected to be a StructuredDataI with the structure to be serialised a PVStructure.
 *
 * The configuration syntax is (names are only given as an example):
 * <pre>
 * +EPICSRPCClient = {
 *   Class = EPICSPVA::EPICSRPCClient
 * }
 * </pre>
 */
namespace MARTe {
class EPICSRPCClient: public Object, public QueuedMessageI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief TODO
     */
    EPICSRPCClient();

    /**
     * @brief TODO
     */
    virtual ~EPICSRPCClient();


};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSRPCCLIENT_H_ */
