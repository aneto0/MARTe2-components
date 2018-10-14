/**
 * @file EPICSPVADatabase.cpp
 * @brief Source file for class EPICSPVADatabase
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
 * the class EPICSPVADatabase (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "EPICSPVADatabase.h"

#include "pv/channelProviderLocal.h"
#include "pv/pvAccess.h"
#include "pv/pvaConstants.h"
#include "pv/pvData.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "EPICSPVARecord.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSPVADatabase::EPICSPVADatabase() :
        ReferenceContainer(), MessageI(), executor(*this) {
    stackSize = THREADS_DEFAULT_STACKSIZE * 4u;
    cpuMask = 0xffu;
    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

EPICSPVADatabase::~EPICSPVADatabase() {

}

void EPICSPVADatabase::Purge(ReferenceContainer &purgeList) {
    if (master) {
        uint32 n;
        uint32 nElements = Size();
        for (n = 0u; n < nElements; n++) {
            ReferenceT<EPICSPVARecord> pvaRecord = Get(n);
            if (pvaRecord.IsValid()) {
                epics::pvDatabase::PVRecordPtr pvRecord = master->findRecord(pvaRecord->GetName());
                REPORT_ERROR(ErrorManagement::Information, "Removing record %s", pvaRecord->GetName());
                if (pvRecord) {
                    if(master->removeRecord(pvRecord)) {
                        REPORT_ERROR(ErrorManagement::Information, "Removed record %s", pvaRecord->GetName());
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "Failed to remove record %s", pvaRecord->GetName());
                    }
                }
            }
        }
    }
    if (serverContext) {
        serverContext->shutdown();
    }
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    ReferenceContainer::Purge(purgeList);
}

bool EPICSPVADatabase::Initialise(StructuredDataI & data) {
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

ErrorManagement::ErrorType EPICSPVADatabase::Start() {
    ErrorManagement::ErrorType err = executor.Start();
    return err;
}

ErrorManagement::ErrorType EPICSPVADatabase::Execute(ExecutionInfo& info) {

    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() == ExecutionInfo::StartupStage) {
        uint32 i;
        uint32 nOfRecords = Size();
        bool ok = true;
        epics::pvDatabase::ChannelProviderLocalPtr channelProvider = epics::pvDatabase::getChannelProviderLocal();
        master = epics::pvDatabase::PVDatabase::getMaster();
        for (i = 0u; (i < nOfRecords) && (ok); i++) {
            ReferenceT<EPICSPVARecord> record = Get(i);
            if (record.IsValid()) {
                epics::pvDatabase::PVRecordPtr pvRecordPtr = record->CreatePVRecord();
                ok = (pvRecordPtr ? true : false);
                if (ok) {
                    master->addRecord(pvRecordPtr);
                    REPORT_ERROR(ErrorManagement::Information, "Registered record with name %s", record->GetName());
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "Record %s failed to CreatePVRecord", record->GetName());
                }
            }
            else {
                ReferenceT<Object> recordObj = Get(i);
                REPORT_ERROR(ErrorManagement::FatalError, "Record %s is not an EPICSPVARecord", recordObj->GetName());
            }
        }
        if (ok) {
            //This is a blocking call and it will run forever!
            try {
                //serverContext = epics::pvAccess::startPVAServer(epics::pvAccess::PVACCESS_ALL_PROVIDERS, 0, false, true);
                serverContext = epics::pvAccess::ServerContext::create();
                serverContext->printInfo();
                serverContext->run(0);
            }
            catch (epics::pvData::detail::ExceptionMixed<epics::pvData::BaseException> &ignored) {
            }
        }
        err = !ok;
    }
    else if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        Sleep::Sec(1.0);
    }
    else {
    }

    return err;
}

EmbeddedThreadI::States EPICSPVADatabase::GetStatus() {
    return executor.GetStatus();
}

uint32 EPICSPVADatabase::GetStackSize() const {
    return stackSize;
}

uint32 EPICSPVADatabase::GetCPUMask() const {
    return cpuMask;
}

epics::pvAccess::ServerContext::shared_pointer EPICSPVADatabase::GetServerContext() const {
    return serverContext;
}

CLASS_REGISTER(EPICSPVADatabase, "")
CLASS_METHOD_REGISTER(EPICSPVADatabase, Start)

}
