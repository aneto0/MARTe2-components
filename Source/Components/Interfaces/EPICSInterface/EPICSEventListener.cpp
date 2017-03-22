/**
 * @file EPICSEventListener.cpp
 * @brief Source file for class EPICSEventListener
 * @date 21/03/2017
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
 * the class EPICSEventListener (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Message.h"

#include "EPICSEventListener.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
EPICSEventListener::EPICSEventListener() :
        ReferenceContainer() {

}

EPICSEventListener::~EPICSEventListener() {

}

bool EPICSEventListener::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
    StreamString eventName;
    if (ok) {
        ok = data.Read("EventName", eventName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "EventName must be specified");
        }
    }
    TimeoutType timeout;
    if (ok) {
        uint32 timeoutMSec = 0u;
        if (data.Read("Timeout", timeoutMSec)) {
            timeout = timeoutMSec;
            REPORT_ERROR(ErrorManagement::ParametersError, "Timeout set to %u ms", timeoutMSec);
        }
        else {
            timeout = TTInfiniteWait;
            REPORT_ERROR(ErrorManagement::ParametersError, "Timeout set to infinite");
        }

    }
    if (ok) {
        mdsEvent = ReferenceT<EPICSEventWrapper>(new (NULL) EPICSEventWrapper(eventName.Buffer(), timeout));
    }


    return ok;
}

CLASS_REGISTER(EPICSEventListener, "1.0")
}

