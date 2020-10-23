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
/*lint -efile(766,EPICSRPCServer.cpp) EPICSRPCService.h, EPICSRPCServiceAdapter.h and RegisteredMethodsMessageFilter.h are used in this file.*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "EPICSRPCServer.h"
#include "EPICSRPCService.h"
#include "EPICSRPCServiceAdapter.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSRPCServer::EPICSRPCServer() :
        ReferenceContainer(), MessageI(), executor(*this) {
    stackSize = THREADS_DEFAULT_STACKSIZE * 4u;
    cpuMask = 0xffu;
    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }

    //This is required in order to be able to call rpcServer->destroy() in the destructor.
    std::shared_ptr<epics::pvAccess::RPCServer> rpcServerShared(new epics::pvAccess::RPCServer());
    rpcServer = rpcServerShared;
}

EPICSRPCServer::~EPICSRPCServer() {

}

void EPICSRPCServer::Purge(ReferenceContainer &purgeList) {
    if (rpcServer) {
        rpcServer->destroy();
    }
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
    executor.SetName(GetName());
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
            ReferenceT<EPICSRPCService> service = Get(i);
            ReferenceT<Object> serviceObj = Get(i);
            ok = service.IsValid();
            if (ok) {
                ok = serviceObj.IsValid();
            }
            if (ok) {
                const char8 * const serviceName = serviceObj->GetName();
                std::shared_ptr<EPICSRPCServiceAdapter> rpcService(new EPICSRPCServiceAdapter());
                ok = (rpcService ? true : false);
                if (ok) {
                    rpcService->SetHandler(service);
                    REPORT_ERROR(ErrorManagement::Information, "Registered service with name %s", serviceName);
                    rpcServer->registerService(serviceName, rpcService);
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "Service %s is not an epics::pvAccess::RPCService", serviceName);
                }
            }
        }
        if (ok) {
            rpcServer->printInfo();
            //This is a blocking call and it will run forever!
            try {
                REPORT_ERROR(ErrorManagement::Debug, "Running the rpcServer");
                rpcServer->run();
                REPORT_ERROR(ErrorManagement::Debug, "Stopped the rpcServer");
            }
            catch (epics::pvData::detail::ExceptionMixed<epics::pvData::BaseException> &ignored) {
                std::cout << ignored.show() << std::endl;
            }
        }
        err = !ok;
    }
    else if (info.GetStage() == ExecutionInfo::MainStage) {
        Sleep::Sec(0.1);
    }
    else {
    }

    return err;
}

EmbeddedThreadI::States EPICSRPCServer::GetStatus() {
    return executor.GetStatus();
}

uint32 EPICSRPCServer::GetStackSize() const {
    return stackSize;
}

uint32 EPICSRPCServer::GetCPUMask() const {
    return cpuMask;
}

CLASS_REGISTER(EPICSRPCServer, "")
CLASS_METHOD_REGISTER(EPICSRPCServer, Start)

}
