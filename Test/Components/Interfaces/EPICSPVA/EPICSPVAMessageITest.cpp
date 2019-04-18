/**
 * @file EPICSPVAMessageITest.cpp
 * @brief Source file for class EPICSPVAMessageITest
 * @date 07/09/2018
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
 * the class EPICSPVAMessageITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "CLASSMETHODREGISTER.h"
#include "EPICSRPCClient.h"
#include "EPICSPVAMessageI.h"
#include "EPICSPVAMessageITest.h"
#include "EPICSRPCServer.h"
#include "EPICSPVAStructureDataI.h"
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
#include "MessageI.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class EPICSPVAMessageITestObject: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()
    EPICSPVAMessageITestObject() : Object(), MessageI() {
        using namespace MARTe;
        ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetDestination(this);
        ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
        if (!ret.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
        }
    }

    virtual ~EPICSPVAMessageITestObject() {

    }

    MARTe::ErrorManagement::ErrorType HandleMessage(MARTe::uint32 &value) {
        value *= 10;
        return MARTe::ErrorManagement::NoError;
    }
};
CLASS_REGISTER(EPICSPVAMessageITestObject, "")
CLASS_METHOD_REGISTER(EPICSPVAMessageITestObject, HandleMessage)


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool EPICSPVAMessageITest::TestConstructor() {
    using namespace MARTe;
    EPICSPVAMessageI test;
    return (test.NumberOfReferences() == 0);
}

bool EPICSPVAMessageITest::Testrequest() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVAMessageITestObject = {"
            "    Class = EPICSPVAMessageITestObject"
            "}"
            "+EPICSRPCServer = {"
            "    Class = EPICSPVA::EPICSRPCServer"
            "    +EPICSPVAMessageI = {"
            "        Class = EPICSPVA::EPICSPVAMessageI"
            "    }"
            "}"
            "+EPICSRPCClient = {"
            "    Class = EPICSPVA::EPICSRPCClient"
            "}";

    ConfigurationDatabase cdb;
    config.Seek(0LLU);
    StandardParser parser(config, cdb);

    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ok) {
        ReferenceT<Message> msgStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgStart->SetName("EPICSRPCServerStart");
        msgConfig.Write("Destination", "EPICSRPCServer");
        msgConfig.Write("Function", "Start");
        msgStart->Initialise(msgConfig);
        MessageI::SendMessage(msgStart);
    }
    if (ok) {
        ReferenceT<Message> msgStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgStart->SetName("EPICSRPCClientStart");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "Start");
        msgStart->Initialise(msgConfig);
        MessageI::SendMessage(msgStart);
    }
    ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ConfigurationDatabase> payload(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ok) {
        ConfigurationDatabase msgConfig;
        msg->SetName("EPICSPVAMessageI");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msg->Initialise(msgConfig);
        payload->Write("Destination", "EPICSPVAMessageITestObject");
        payload->Write("Function", "HandleMessage");
        payload->Write("Mode", "ExpectsReply");
        payload->CreateAbsolute("_Parameters");
        payload->Write("Class", "ConfigurationDatabase");
        payload->Write("param1", 5);
        payload->MoveToRoot();

        msg->SetExpectsReply(true);
        msg->Insert(payload);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msg, &notReallyUsed);
    }
    ReferenceT<StructuredDataI> payloadReply;
    if (ok) {
        payloadReply = msg->Get(1);
        ok = payloadReply.IsValid();
    }
    uint32 val;
    if (ok) {
        ok = payloadReply->Read("param1", val);
    }
    if (ok) {
        ok = (val == 50);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}


bool EPICSPVAMessageITest::Testrequest_EPICSPVAStructuredDataI() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVAMessageITestObject = {"
            "    Class = EPICSPVAMessageITestObject"
            "}"
            "+EPICSRPCServer = {"
            "    Class = EPICSPVA::EPICSRPCServer"
            "    +EPICSPVAMessageI = {"
            "        Class = EPICSPVA::EPICSPVAMessageI"
            "    }"
            "}"
            "+EPICSRPCClient = {"
            "    Class = EPICSPVA::EPICSRPCClient"
            "}";

    ConfigurationDatabase cdb;
    config.Seek(0LLU);
    StandardParser parser(config, cdb);

    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ok) {
        ReferenceT<Message> msgStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgStart->SetName("EPICSRPCServerStart");
        msgConfig.Write("Destination", "EPICSRPCServer");
        msgConfig.Write("Function", "Start");
        msgStart->Initialise(msgConfig);
        MessageI::SendMessage(msgStart);
    }
    if (ok) {
        ReferenceT<Message> msgStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgStart->SetName("EPICSRPCClientStart");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "Start");
        msgStart->Initialise(msgConfig);
        MessageI::SendMessage(msgStart);
    }
    ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<EPICSPVAStructureDataI> payload(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    payload->InitStructure();
    if (ok) {
        ConfigurationDatabase msgConfig;
        msg->SetName("EPICSPVAMessageI");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msg->Initialise(msgConfig);
        payload->Write("Destination", "EPICSPVAMessageITestObject");
        payload->Write("Function", "HandleMessage");
        payload->Write("Mode", "ExpectsReply");
        payload->CreateAbsolute("_Parameters");
        payload->Write("Class", "ConfigurationDatabase");
        payload->Write("param1", 5);
        payload->FinaliseStructure();
        payload->MoveToRoot();
        msg->SetExpectsReply(true);
        msg->Insert(payload);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msg, &notReallyUsed);
    }
    ReferenceT<StructuredDataI> payloadReply;
    if (ok) {
        payloadReply = msg->Get(1);
        ok = payloadReply.IsValid();
    }
    uint32 val;
    if (ok) {
        ok = payloadReply->Read("param1", val);
    }
    if (ok) {
        ok = (val == 50);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

