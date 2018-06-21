/**
 * @file EPICSPVAMessageI.cpp
 * @brief Source file for class EPICSPVAMessageI
 * @date 18/06/2018
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
 * the class EPICSPVAMessageI (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "pv/pvData.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EPICSPVAMessageI.h"
#include "EPICSPVStructureDataI.h"
#include "Message.h"
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSPVAMessageI::EPICSPVAMessageI() :
        Object(), MessageI() {
    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
    ReferenceContainer::AddBuildToken('_');
}

EPICSPVAMessageI::~EPICSPVAMessageI() {

}

epics::pvData::PVStructurePtr EPICSPVAMessageI::request(epics::pvData::PVStructure::shared_pointer const & args)
        throw (epics::pvAccess::RPCRequestException) {

    EPICSPVStructureDataI config;
    config.InitStructure();
    config.SetStructure(args);
    ConfigurationDatabase cdbMsg;
    ReferenceT<ReferenceContainer> rc;
    ReferenceT<Message> msg;
    bool ok = config.Copy(cdbMsg);
    if (ok) {
        //TODO
        rc = ReferenceT<ReferenceContainer>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        //In theory I could pass the PVStructure config directly to the Initialise
        ok = rc->Initialise(cdbMsg);
    }
    if (ok) {
        msg = rc;
        ok = rc.IsValid();
    }
    bool expectsReply = false;
    if (ok) {
        expectsReply = msg->ExpectsReply();
        ErrorManagement::ErrorType err = SendMessage(msg);
        ok = (err.ErrorsCleared());
        if (!ok) {
            REPORT_ERROR(err, "Could not send the message");
        }
    }

    if (ok) {
        if (expectsReply) {
//...TODO serialise the reply
        }
    }
    epics::pvData::FieldBuilderPtr fieldBuilder = epics::pvData::getFieldCreate()->createFieldBuilder();
    epics::pvData::StructureConstPtr topStructure = fieldBuilder->createStructure();
    epics::pvData::PVStructurePtr result(epics::pvData::getPVDataCreate()->createPVStructure(topStructure));
    args->dumpValue(std::cout);
    return result;
}

CLASS_REGISTER(EPICSPVAMessageI, "1.0")
}
