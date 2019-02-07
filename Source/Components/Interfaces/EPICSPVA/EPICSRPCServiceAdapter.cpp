
/**
 * @file EPICSRPCServiceAdapter.cpp
 * @brief Source file for class EPICSRPCServiceAdapter
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

 * @details This source file contains the definition of all the methods for
 * the class EPICSRPCServiceAdapter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EPICSRPCServiceAdapter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSRPCServiceAdapter::EPICSRPCServiceAdapter() : epics::pvAccess::RPCService() {
}

EPICSRPCServiceAdapter::~EPICSRPCServiceAdapter() {
}

void EPICSRPCServiceAdapter::SetHandler(ReferenceT<EPICSRPCService> handlerIn) {
    handler = handlerIn;
}

epics::pvData::PVStructurePtr EPICSRPCServiceAdapter::request(epics::pvData::PVStructure::shared_pointer const & args) {
    return handler->request(args);
}


}
