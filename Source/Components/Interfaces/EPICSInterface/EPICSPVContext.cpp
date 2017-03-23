/**
 * @file EPICSCAContext.cpp
 * @brief Source file for class EPICSCAContext
 * @date 23/03/2017
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
 * the class EPICSCAContext (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EPICSPVContext.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

EPICSPVContext::EPICSPVContext() {
    context = NULL_PTR(struct ca_client_context *);
    timeout = 5.0F;
    pvName = "";
    pvChid = 0;

}

EPICSPVContext::~EPICSPVContext() {

}

bool EPICSPVContext::Initialise(StructuredDataI & data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Read("PVName", pvName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "PVName must be specified");
        }
    }
    return ok;
}

ErrorManagement::ErrorType EPICSPVContext::SetContext(struct ca_client_context * contextIn, float32 timeoutIn) {
    ErrorManagement::ErrorType err;
    context = contextIn;
    timeout = timeoutIn;
    if (ca_create_channel(pvName.Buffer(), NULL, NULL, 20, &pvChid) != ECA_NORMAL) {
        err = ErrorManagement::FatalError;
        REPORT_ERROR(err, "ca_create_channel failed");
    }
    return err;
}

}

