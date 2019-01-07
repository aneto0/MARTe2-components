/**
 * @file EPICSRPCClient.cpp
 * @brief Source file for class EPICSRPCClient
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

 * @details This source file contains the definition of all the methods for
 * the class EPICSRPCClient (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
/*lint -efile(766,EPICSRPCClient.cpp) RegisteredMethodsMessageFilter.h is used in this file.*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "EPICSRPCClient.h"
#include "EPICSRPCClientMessageFilter.h"
#include "Message.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSRPCClient::EPICSRPCClient() :
        Object(), QueuedMessageI() {

    ReferenceT<RegisteredMethodsMessageFilter> filterRPC = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filterRPC->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filterRPC);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install RPC message filter");
    }
    if (ret.ErrorsCleared()) {
        filter = ReferenceT<EPICSRPCClientMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ErrorManagement::ErrorType ret = QueuedMessageI::InstallMessageFilter(filter);
        if (!ret.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
        }

    }
}

EPICSRPCClient::~EPICSRPCClient() {
    if (!Stop()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not Stop the QueuedMessageI");
    }
}

bool EPICSRPCClient::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        float64 timeout = 10.0;
        if (data.Read("Timeout", timeout)) {
            if (filter.IsValid()) {
                filter->SetTimeout(timeout);
            }
        }
    }
    return ok;
}


CLASS_REGISTER(EPICSRPCClient, "1.0")
CLASS_METHOD_REGISTER(EPICSRPCClient, Start)

}
