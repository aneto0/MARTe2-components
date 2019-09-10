/**
 * @file OPCUAClient.cpp
 * @brief Source file for class OPCUAClient
 * @date 05/04/2019
 * @authors Chiara Piron, Luca Boncagni
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
 * the class OPCUAClient (public, protected, and private). Be aware that some
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
#include "OPCUAClient.h"
#include "RegisteredMethodsMessageFilter.h"
#include "OPCUAVariable.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

OPCUAClient::OPCUAClient() :
        ReferenceContainer(),
        EmbeddedServiceMethodBinderI(),
        MessageI(),
        executor(*this) {
    stackSize = THREADS_DEFAULT_STACKSIZE * 4u;
    cpuMask = 0xffu;

    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
//    config = UA_ClientConfig_default;
//    opcuaClient = UA_Client_new(config);
    opcuaClient = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(opcuaClient));
    serverAddress.Seek(0LLU);
    // monitoredItemsContainer=this;
    OPCUAVariable::CreateLock();
    OPCUAVariable::SetContainer(this);
}

OPCUAClient::~OPCUAClient() {
    UA_StatusCode code = UA_Client_disconnect(opcuaClient);
    if (code == 0x00U) { /* UA_STATUSCODE_GOOD */
        UA_Client_delete(opcuaClient);
    }
}

void OPCUAClient::Purge(ReferenceContainer &purgeList) {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    ReferenceContainer::Purge(purgeList);
}

bool OPCUAClient::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "OPCUAClient: Client is initilized"); // <<< CP
        if (!data.Read("CPUs", cpuMask)) {
            REPORT_ERROR(ErrorManagement::Information, "No CPUs defined. Using default = %d", cpuMask);
        }
        if (!data.Read("StackSize", stackSize)) {
            REPORT_ERROR(ErrorManagement::Information, "No StackSize defined. Using default = %d", stackSize);
        }
        ok = data.Read("Address", serverAddress);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the Address attribute");
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "CPUs = %d", cpuMask); // <<< CP
            REPORT_ERROR(ErrorManagement::Information, "StackSize = %d", stackSize); // <<< CP
            executor.SetStackSize(stackSize);
            executor.SetCPUMask(cpuMask);
            uint32 autoStart = 1u;
            (void) (data.Read("AutoStart", autoStart));
            if (autoStart == 1u) {
                ok = (Start() == ErrorManagement::NoError);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "OPCUAClient: Client is NOT initilised"); // <<< CP
    }
    return ok;
}

bool OPCUAClient::Connect() {
    UA_StatusCode retval = UA_Client_connect(opcuaClient, const_cast<char8*>(serverAddress.Buffer()));
    return (retval == 0x00U); /* UA_STATUSCODE_GOOD */
}

ErrorManagement::ErrorType OPCUAClient::Start() {
    ErrorManagement::ErrorType err = executor.Start();
    return err;
}

EmbeddedThreadI::States OPCUAClient::GetStatus() {
    return executor.GetStatus();
}

ErrorManagement::ErrorType OPCUAClient::Execute(ExecutionInfo& info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() == ExecutionInfo::StartupStage) {
        //REPORT_ERROR(ErrorManagement::Information, "OPCUAClient: start up");

        bool ok = Connect();
        if (ok) {
            //REPORT_ERROR(ErrorManagement::Information, "The connection with the OPCUA Server has been established successfully!");
            uint32 j;
            for (j = 0u; j < Size(); j++) {
                ReferenceT<OPCUAVariable> child = Get(j);
                if (child.IsValid()) {
                    ok = child->PostInit(opcuaClient);
                    if (!ok)
                        err = ErrorManagement::FatalError;
                }
            }
        }
        else {
            err = ErrorManagement::FatalError;
            REPORT_ERROR(err, "Connection failed");
        }

    }
    else if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        Sleep::Sec(0.999F);
        OPCUAVariable::Lock();
//        UA_Client_runAsync(opcuaClient,100);//changed from run_iterate
        UA_Client_run_iterate(opcuaClient, 100);
        OPCUAVariable::UnLock();
    }
    else {

    }

    return err;
}

uint32 OPCUAClient::GetStackSize() const {
    return stackSize;
}

uint32 OPCUAClient::GetCPUMask() const {
    return cpuMask;
}

CLASS_REGISTER(OPCUAClient, "1.0")
CLASS_METHOD_REGISTER(OPCUAClient, Start)
}

