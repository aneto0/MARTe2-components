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

#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "EPICSCAClient.h"
#include "EPICSPV.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Callback function for the ca_create_subscription. Single point of access which
 * delegates the events to the corresponding EPICSPV instance.
 */
static FastPollingMutexSem eventCallbackFastMux;
static EPICSCAClient *currentListener = NULL_PTR(EPICSCAClient *);
/*lint -e{1746} function must match required prototype and thus cannot be changed to constant reference.*/
void EPICSCAClientEventCallback(struct event_handler_args const args) {
    (void) eventCallbackFastMux.FastLock();
    EPICSCAClient *listener = static_cast<EPICSCAClient *>(args.usr);
    if (listener != NULL_PTR(EPICSCAClient *)) {
        if (listener == currentListener) {
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
    stackSize = THREADS_DEFAULT_STACKSIZE * 4u;
    cpuMask = 0xffu;
    eventCallbackFastMux.Create();
}

/*lint -e{1551} The destructor is responsible for stopping the embedded thread.*/
EPICSCAClient::~EPICSCAClient() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
}

bool EPICSCAClient::Initialise(StructuredDataI & data) {
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

    return ok;
}

ErrorManagement::ErrorType EPICSCAClient::Execute(const ExecutionInfo& info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() == ExecutionInfo::StartupStage) {
        (void) eventCallbackFastMux.FastLock();
        /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
        if (ca_context_create(ca_enable_preemptive_callback) != ECA_NORMAL) {
            err = ErrorManagement::FatalError;
            REPORT_ERROR(err, "ca_enable_preemptive_callback failed");
        }
        uint32 j;
        for (j = 0u; j < Size(); j++) {
            ReferenceT<EPICSPV> child = Get(j);
            if (child.IsValid()) {
                chid pvChid;
                StreamString pvName = child->GetPVName();
                (void) pvName.Seek(0LLU);
                /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
                if (ca_create_channel(pvName.Buffer(), NULL_PTR(caCh *), NULL_PTR(void *), 20u, &pvChid) != ECA_NORMAL) {
                    err = ErrorManagement::FatalError;
                    REPORT_ERROR(err, "ca_create_channel failed for PV with name %s", pvName.Buffer());
                }
                if (err.ErrorsCleared()) {
                    child->SetContext(ca_current_context());
                }
                if (err.ErrorsCleared()) {

                    child->SetPVChid(pvChid);
                    evid pvEvid;
                    /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
                    if (ca_create_subscription(child->GetPVType(), 1u, pvChid, DBE_VALUE, &EPICSCAClientEventCallback, this, &pvEvid) != ECA_NORMAL) {
                        err = ErrorManagement::FatalError;
                        REPORT_ERROR(err, "ca_create_subscription failed for PV %s", pvName.Buffer());
                    }
                    else {
                        child->SetPVEvid(pvEvid);
                    }
                }
            }
        }
        currentListener = this;
        eventCallbackFastMux.FastUnLock();
    }
    else if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        Sleep::Sec(1.0);
    }
    else {
        (void) eventCallbackFastMux.FastLock();
        uint32 j;
        for (j = 0u; j < Size(); j++) {
            ReferenceT<EPICSPV> pv = Get(j);
            if (pv.IsValid()) {
                (void) ca_clear_subscription(pv->GetPVEvid());
                (void) ca_clear_event(pv->GetPVEvid());
                (void) ca_clear_channel(pv->GetPVChid());
            }
        }
        ca_detach_context();
        ca_context_destroy();
        eventCallbackFastMux.FastUnLock();
    }

    return err;
}
CLASS_REGISTER(EPICSCAClient, "1.0")

}

