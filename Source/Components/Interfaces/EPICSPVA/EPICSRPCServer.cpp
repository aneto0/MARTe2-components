/**
 * @file EPICSRPCServer.cpp
 * @brief Source file for class EPICSRPCServer
 * @date 12/06/2018
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
 * the class EPICSRPCServer (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "pv/channelProviderLocal.h"
#include "pv/pvAccess.h"
#include "pv/pvData.h"
#include "pv/pvDatabase.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "EPICSObjectRegistryDatabaseService.h"
#include "EPICSRPCServer.h"
#include "RegisteredMethodsMessageFilter.h"
#include "MARTe2RPC.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSRPCServer::EPICSRPCServer() :
        ReferenceContainer(),
        MessageI(),
        executor(*this) {
    stackSize = THREADS_DEFAULT_STACKSIZE * 4u;
    cpuMask = 0xffu;
    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

EPICSRPCServer::~EPICSRPCServer() {

}

void EPICSRPCServer::Purge(ReferenceContainer &purgeList) {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    ReferenceContainer::Purge(purgeList);
}

bool EPICSRPCServer::Initialise(StructuredDataI & data) {
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
        uint32 autoStart = 1u;
        (void) (data.Read("AutoStart", autoStart));
        if (autoStart == 1u) {
            ok = (Start() == ErrorManagement::NoError);
        }
    }
    return ok;
}

ErrorManagement::ErrorType EPICSRPCServer::Start() {
    ErrorManagement::ErrorType err = executor.Start();
    return err;
}

ErrorManagement::ErrorType EPICSRPCServer::Execute(ExecutionInfo& info) {

    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() == ExecutionInfo::StartupStage) {
        uint32 i;
        uint32 nOfServices = Size();
        bool ok = true;
        for (i = 0u; (i < nOfServices) && (ok); i++) {
            // register our service as "helloService"
            ReferenceT<Object> service = Get(i);
            if (service.IsValid()) {
                const char8 * const serviceName = service->GetName();
                epics::pvAccess::RPCService *rpcService =
                        dynamic_cast<epics::pvAccess::RPCService *>(service.operator ->());
                ok = (rpcService != NULL_PTR(epics::pvAccess::RPCService *));
                if (ok) {
                    REPORT_ERROR(ErrorManagement::Information, "Registered service with name %s", serviceName);
                    rpcServer.registerService(serviceName, epics::pvAccess::RPCService::shared_pointer(rpcService));
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "Service %s is not an epics::pvAccess::RPCService", serviceName);
                }
            }
        }
        if (ok) {
            rpcServer.printInfo();
            rpcServer.run();
        }
        err = !ok;
    }
    else if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        Sleep::Sec(1.0);
    }
    else {
        rpcServer.destroy();
    }

    return err;
}

CLASS_REGISTER(EPICSRPCServer, "")
CLASS_METHOD_REGISTER(EPICSRPCServer, Start)

}
