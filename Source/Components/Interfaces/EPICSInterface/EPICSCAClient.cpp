/**
 * @file EPCISCAClient.cpp
 * @brief Source file for class EPCISCAClient
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
 * the class EPCISCAClient (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EPICSCAClient.h"

#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"

#include "EPICSPVBase.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
static FastPollingMutexSem eventCallbackFastMux;
void EPCISCAClientEventCallback(struct event_handler_args args) {
    eventCallbackFastMux.FastLock();
    EPCISCAClient *listener = static_cast<EPCISCAClient *>(args.usr);
    StreamString currentValue;
    if (listener != NULL_PTR(EPICSEventListener *)) {
        currentValue = static_cast<const char8 *>(args.dbr);
        bool found = false;
        uint32 j;
        for (j = 0u; (j < listener->Size()) && (!found); j++) {
            ReferenceT<EPICSPVEvent> pvEvent = listener->Get(j);
            StreamString pvName;
            if (pvEvent.IsValid()) {
                found = (pvEvent->GetPVChid() == args.chid);
                if (found) {
                    pvEvent->ValueChanged(currentValue);
                }
            }
        }
    }

    eventCallbackFastMux.FastUnLock();
}
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
EPCISCAClient::EPCISCAClient() :
        ReferenceContainer(), EmbeddedServiceMethodBinderI(), executor(*this) {
    stackSize = THREADS_DEFAULT_STACKSIZE;
    cpuMask = 0xff;
    timeout = 5.0;
}

EPCISCAClient::~EPCISCAClient() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
}

bool EPCISCAClient::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
    if (ok) {
        if (!data.Read("CPUs", cpuMask)) {
            REPORT_ERROR(ErrorManagement::Information, "No CPUs defined. Using default = %d", cpuMask);
        }
        if (!data.Read("StackSize", stackSize)) {
            REPORT_ERROR(ErrorManagement::Information, "No StackSize defined. Using default = %d", stackSize);
        }
        //This stack size should be sufficient?
        executor.SetStackSize(stackSize);
        executor.SetCPUMask(cpuMask);
        ok = (executor.Start() == ErrorManagement::NoError);
    }
    if (data.Read("Timeout", timeout)) {
        if (timeout == 0u) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Timeout shall be > 0");
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Timeout set to %f s", timeout);
        }
    }

    return ok;
}

ErrorManagement::ErrorType EPCISCAClient::Execute(const ExecutionInfo& info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() == ExecutionInfo::StartupStage) {
        //if (ca_context_create(ca_disable_preemptive_callback) != ECA_NORMAL) {
        if (ca_context_create(ca_enable_preemptive_callback) != ECA_NORMAL) {
            err = ErrorManagement::FatalError;
            REPORT_ERROR(err, "ca_enable_preemptive_callback failed");
        }
        uint32 j;
        for (j=0; j<Size(); j++) {
            ReferenceT<EPICSPVContext> child = Get(j);
            if (child.IsValid()) {
                child->SetContext(ca_current_context(), timeout);
            }
        }
    }
    else if (info.GetStage() != ExecutionInfo::TerminationStage) {
        Sleep::Sec(1.0);
    }
    else {
        (void) ca_context_destroy();
    }

    return err;
}
CLASS_REGISTER(EPCISCAClient, "1.0")

}

