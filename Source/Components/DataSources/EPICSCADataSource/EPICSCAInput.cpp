/**
 * @file EPICSCADataSource.cpp
 * @brief Source file for class EPICSCADataSource
 * @date 20/04/2017
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
 * the class EPICSCADataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <cadef.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EPICSCAInput.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Callback function for the ca_create_subscription. Single point of access which
 * delegates the events to the corresponding EPICSPV instance.
 */
static FastPollingMutexSem eventCallbackFastMux;
/*lint -e{1746} function must match required prototype and thus cannot be changed to constant reference.*/
void EPICSCAClientEventCallback(struct event_handler_args const args) {
    (void) eventCallbackFastMux.FastLock();
    EPICSCAInput::PVWrapper *pv = static_cast<EPICSCAInput::PVWrapper *>(args.usr);
    if (pv != NULL_PTR(EPICSCAInput::PVWrapper *)) {
        if (pv->pvType == DBR_INT) {
            *(reinterpret_cast<uint32 *>(pv->memory)) = *(reinterpret_cast<const uint32 *>(args.dbr));
        }
        else if (pv->pvType == DBR_FLOAT) {
            *(reinterpret_cast<float32 *>(pv->memory)) = *(reinterpret_cast<const float32 *>(args.dbr));
        }
        else if (pv->pvType == DBR_DOUBLE) {
            *(reinterpret_cast<float64 *>(pv->memory)) = *(reinterpret_cast<const float64 *>(args.dbr));
        }
        else {
            //Should never enter here...
        }
    }
    eventCallbackFastMux.FastUnLock();
}
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
EPICSCAInput::EPICSCAInput() :
        DataSourceI(), EmbeddedServiceMethodBinderI(), MessageI(), executor(*this) {
    pvs = NULL_PTR(PVWrapper *);
    stackSize = THREADS_DEFAULT_STACKSIZE * 4u;
    cpuMask = 0xffu;
    eventCallbackFastMux.Create();
}

EPICSCAInput::~EPICSCAInput() {

}

bool EPICSCAInput::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
    if (ok) {
        if (!data.Read("CPUs", cpuMask)) {
            REPORT_ERROR(ErrorManagement::Information, "No CPUs defined. Using default = %d", cpuMask);
        }
        if (!data.Read("StackSize", stackSize)) {
            REPORT_ERROR(ErrorManagement::Information, "No StackSize defined. Using default = %d", stackSize);
        }
        executor.SetStackSize(stackSize);
        executor.SetCPUMask(cpuMask);
    }
    if (ok) {
        ok = data.MoveRelative("Signals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }
        if (ok) {
            //Do not allow to add signals in run-time
            ok = data.Write("Locked", 1);
        }
        if (ok) {
            ok = data.Copy(originalSignalInformation);
        }
        if (ok) {
            ok = originalSignalInformation.MoveToRoot();
        }
    }
    if (ok) {
        ok = data.MoveToAncestor(1u);
    }
    return ok;
}

bool EPICSCAInput::SetConfiguredDatabase(StructuredDataI & data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {

    }

    if (ok) {
        ok = (executor.Start() == ErrorManagement::NoError);
    }
    return ok;
}

}

