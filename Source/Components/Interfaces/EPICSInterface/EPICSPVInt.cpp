/**
 * @file EPICSPVInt.cpp
 * @brief Source file for class EPICSPVInt
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
 * the class EPICSPVInt (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include <EPICSPVInt.h>
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

EPICSPVInt::EPICSPVInt() :
        MessageI(), EPICSPVContext() {
    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

EPICSPVInt::~EPICSPVInt() {

}

ErrorManagement::ErrorType EPICSPVInt::CAPut(const int32 val) {
    ca_attach_context(context);
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (ca_put(DBR_INT, pvChid, &val) != ECA_NORMAL) {
        err = ErrorManagement::FatalError;
        REPORT_ERROR(err, "ca_put failed for PV: %s with value %d", pvName.Buffer(), val);
    }
    if (ca_pend_io(timeout) != ECA_NORMAL) {
        err = ErrorManagement::FatalError;
        REPORT_ERROR(err, "ca_pend_io failed for PV: %s", pvName.Buffer());
    }
    ca_detach_context();
    return err;
}

ErrorManagement::ErrorType EPICSPVInt::CAGet(int32 &val) {
    ca_attach_context(context);
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (ca_pend_io(timeout) != ECA_NORMAL) {
        err = ErrorManagement::FatalError;
        REPORT_ERROR(err, "ca_pend_io failed for PV: %s", pvName.Buffer());
    }
    if (ca_get(DBR_INT, pvChid, &val) != ECA_NORMAL) {
        err = ErrorManagement::FatalError;
        REPORT_ERROR(err, "ca_put failed for PV: %s with value %d", pvName.Buffer(), val);
    }
    if (ca_pend_io(timeout) != ECA_NORMAL) {
        err = ErrorManagement::FatalError;
        REPORT_ERROR(err, "ca_pend_io failed for PV: %s", pvName.Buffer());
    }
    ca_detach_context();
    return err;
}

CLASS_REGISTER(EPICSPVInt, "1.0")
CLASS_METHOD_REGISTER(EPICSPVInt, CAPut)
CLASS_METHOD_REGISTER(EPICSPVInt, CAGet)
}
