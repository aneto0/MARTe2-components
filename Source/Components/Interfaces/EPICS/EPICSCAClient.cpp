/**
 * @file EPICSCAClient.cpp
 * @brief Source file for class EPICSCAClient
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
 * the class EPICSCAClient (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "../EPICS/EPICSCAClient.h"

#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"

#include "../EPICS/EPICSPV.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
static FastPollingMutexSem eventCallbackFastMux;
void EPICSCAClientEventCallback(struct event_handler_args args) {
    eventCallbackFastMux.FastLock();
    EPICSCAClient *listener = static_cast<EPICSCAClient *>(args.usr);
    if (listener != NULL_PTR(EPICSCAClient *)) {
        bool found = false;
        uint32 j;
        for (j = 0u; (j < listener->Size()) && (!found); j++) {
            ReferenceT<EPICSPV> pvEvent = listener->Get(j);
            if (pvEvent.IsValid()) {
                found = (pvEvent->GetPVChid() == args.chid);
                if (found) {
                    pvEvent->HandlePVEvent(args.dbr);
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
EPICSCAClient::EPICSCAClient() :
        ReferenceContainer(), EmbeddedServiceMethodBinderI(), executor(*this) {
    stackSize = THREADS_DEFAULT_STACKSIZE;
    cpuMask = 0xff;
    timeout = 5.0;
}

EPICSCAClient::~EPICSCAClient() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
}

bool EPICSCAClient::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
    if (data.Read("Timeout", timeout)) {
        if (timeout == 0u) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Timeout shall be > 0");
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Timeout set to %f s", timeout);
        }
    }
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

    return ok;
}

ErrorManagement::ErrorType EPICSCAClient::Execute(const ExecutionInfo& info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() == ExecutionInfo::StartupStage) {
        //if (ca_context_create(ca_disable_preemptive_callback) != ECA_NORMAL) {
        if (ca_context_create(ca_enable_preemptive_callback) != ECA_NORMAL) {
            err = ErrorManagement::FatalError;
            REPORT_ERROR(err, "ca_enable_preemptive_callback failed");
        }
        uint32 j;
        for (j = 0; j < Size(); j++) {
            ReferenceT<EPICSPV> child = Get(j);
            if (child.IsValid()) {
                chid pvChid;
                StreamString pvName = child->GetPVName();
                (void) pvName.Seek(0LLU);
                if (err.ErrorsCleared()) {
                    child->SetContext(ca_current_context());
                }
                if (ca_create_channel(pvName.Buffer(), NULL, NULL, 20, &pvChid) != ECA_NORMAL) {
                    err = ErrorManagement::FatalError;
                    REPORT_ERROR(err, "ca_create_channel failed for PV with name %s", pvName.Buffer());
                }
                if (err.ErrorsCleared()) {
                    child->SetPVChid(pvChid);
                    evid ignore;
                    if (ca_create_subscription(child->GetPVType(), 1u, pvChid, DBE_VALUE, EPICSCAClientEventCallback, this, &ignore) != ECA_NORMAL) {
                        err = ErrorManagement::FatalError;
                        REPORT_ERROR(err, "ca_create_subscription failed for PV %s", pvName.Buffer());
                    }
                }
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
CLASS_REGISTER(EPICSCAClient, "1.0")

}

