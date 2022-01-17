/**
 * @file MARTeIocServer.cpp
 * @brief Source file for class MARTeIocServer
 * @date 01/07/2019
 * @author Giuseppe Ferro
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
 * the class MARTeIocServer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
//#include "nciu.h"
#include "epicsRelease.h"
// EPICS FileDescriptor Manager
#include <fdmgr.h>
#include <fdManager.h>
// EPICS pCAS GDD stuff
#include "smartGDDPointer.h"
#include "aitTypes.h"
#include "epicsTimer.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTeIocServer.h"
#include "ConfigurationDatabase.h"
#include "StreamString.h"
#include "AdvancedErrorManagement.h"
#include "MARTePv.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
//#define NULL_PTR(x) NULL

const uint32 DEFAULT_EPICS_CPUMASK = 0xFFFFu;
const uint32 DEFAULT_EVENT_CPUMASK = 0xFFFFu;

MARTeIocServer::MARTeIocServer() :
        /*lint -e{1069} caServer is a base of MARTeIocServer*/
        caServer(),
        ReferenceContainer(),
        EmbeddedServiceMethodBinderI(),
        executor(*this)
/*lint -e{1928} caServer appears in the constructor initialiser list*/
{

    cpuMask = 0u;
    numberOfPVs = 0u;
}

/*lint -e{1551} No exception thrown.*/
MARTeIocServer::~MARTeIocServer() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
}

bool MARTeIocServer::Initialise(StructuredDataI &data) {
    bool ret = ReferenceContainer::Initialise(data);

    if (ret) {

        //PublishSubscriber Interface(TODO) configuration options
        if (!data.Read("RunOnCPU", cpuMask)) {
            cpuMask = DEFAULT_EPICS_CPUMASK;
        }
        executor.SetCPUMask(cpuMask);
        if (executor.GetStatus() == EmbeddedThreadI::OffState) {
            ret = executor.Start();
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Cannot create EpicsThreadCallback!");
            }
        }
    }

    if (ret) {
        for (uint32 i = 0u; i < Size(); i++) {
            ReferenceT<MARTePv> pvChild = Get(i);
            if (pvChild.IsValid()) {
                pvChild->SetIocServer(*this);
                numberOfPVs++;
            }
        }

        REPORT_ERROR(ErrorManagement::Information, "Loaded %d Process Variables.", numberOfPVs);
    }
    return ret;
}
/*lint -e{715} clientAddress not referenced*/
/*lint -e{952} pPVAliasName is not const to match the prototype of the external lib*/
/*lint -e{1039} pvExistTest is a member of MARTeIocServer*/
pvExistReturn MARTeIocServer::pvExistTest(const casCtx & ctx,
                                          const caNetAddr &clientAddress,
                                          const char8 * pPVAliasName) {

    pvExistReturn ret = pverDoesNotExistHere;
    uint32 len=StringHelper::Length(pPVAliasName);
    StreamString pvName=pPVAliasName;

    if(StringHelper::Compare(&pPVAliasName[len-4u], ".VAL")==0){
        /*lint -e{9123} needed cast to larger type*/
        (void)pvName.SetSize(static_cast<uint64>(len-4u));
    }
    ReferenceT<MARTePv> pv = Find(pvName.Buffer());
    if (pv.IsValid()) {
        ret = pverExistsHere;
    }
    else {
        ret = pverDoesNotExistHere;
    }
    return ret;
}

/*lint -e{715} ctx not referenced*/
/*lint -e{952} pPVAliasName is not const to match the prototype of the external lib*/
pvAttachReturn MARTeIocServer::pvAttach(const casCtx &ctx,
                                        const char8 * pPVAliasName) {

    /*lint -e{9130} depends on external library*/
    pvAttachReturn ret = static_cast<caStatus>(S_casApp_pvNotFound);

    uint32 len=StringHelper::Length(pPVAliasName);
    StreamString pvName=pPVAliasName;

    if(StringHelper::Compare(&pPVAliasName[len-4u], ".VAL")==0){
        /*lint -e{9123} needed cast to larger type*/
        (void)pvName.SetSize(static_cast<uint64>(len-4u));
    }
    ReferenceT<MARTePv> pv = Find(pvName.Buffer());
    if (pv.IsValid()) {
        /*lint -e{64} not a type mismatch error because pvAttachReturn has a
         * constructor from casPV
         */
        ret = pv.operator->();
    }
    return ret;
}

/*lint -e{715} info is not referenced*/
ErrorManagement::ErrorType MARTeIocServer::Execute(ExecutionInfo & info) {
    ErrorManagement::ErrorType err;
    fileDescriptorManager.process(0.1);
    return err;
}

caStatus MARTeIocServer::IocWrite(const char8 * const pvNameIn,
                                  const AnyType &value) {

    caStatus status = static_cast<caStatus>(S_casApp_success);
    ReferenceT<MARTePv> pv = Find(pvNameIn);
    if (pv.IsValid()) {
        status = pv->WriteDirect(value);
    }
    return status;
}

caStatus MARTeIocServer::IocRead(const char8 * const pvNameIn,
                                 const AnyType &value) {
    caStatus status = static_cast<caStatus>(S_casApp_success);
    ReferenceT<MARTePv> pv = Find(pvNameIn);
    if (pv.IsValid()) {
        status = pv->ReadDirect(value);
    }
    return status;
}

bool MARTeIocServer::IocAddPv(Reference newPvIn) {
    ReferenceT<MARTePv> newPv = newPvIn;
    bool ret = newPv.IsValid();
    if (ret) {
        newPv->SetIocServer(*this);
    }
    if (ret) {
        ret = ReferenceContainer::Insert(newPv);
    }
    if (ret) {
        numberOfPVs++;
    }
    return ret;
}

bool MARTeIocServer::IocRemovePv(const char8 * const pvName) {
    bool ret = ReferenceContainer::Delete(pvName);
    if (ret) {
        numberOfPVs--;
    }
    return ret;
}

uint32 MARTeIocServer::GetNumberOfPVs() const {
    return numberOfPVs;
}

CLASS_REGISTER(MARTeIocServer, "1.0")

}
