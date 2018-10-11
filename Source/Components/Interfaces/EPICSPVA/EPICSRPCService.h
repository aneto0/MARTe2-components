/**
 * @file EPICSRPCService.h
 * @brief Header file for class EPICSRPCService
 * @date 10/10/2018
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

 * @details This header file contains the declaration of the class EPICSRPCService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSRPCSERVICE_H_
#define EPICSPVA_EPICSRPCSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pv/rpcService.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief A class cannot inherit both from RPCService and Object as otherwise the
 *  smart pointer mechanisms will conflict and potentially call the destructor twice!
 */
class EPICSRPCService {
public:
    /**
     * @brief NOOP.
     */
    EPICSRPCService();

    /**
     * @brief NOOP.
     */
    virtual ~EPICSRPCService();

    /**
     * @see the epics::pvAccess::RPCService::request.
     */
    virtual epics::pvData::PVStructurePtr request(epics::pvData::PVStructure::shared_pointer const & args) = 0;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSRPCSERVICE_H_ */

