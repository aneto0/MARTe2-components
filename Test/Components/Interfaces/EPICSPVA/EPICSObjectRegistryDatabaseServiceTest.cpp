/**
 * @file EPICSObjectRegistryDatabaseServiceTest.cpp
 * @brief Source file for class EPICSObjectRegistryDatabaseServiceTest
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
 * the class EPICSObjectRegistryDatabaseServiceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "EPICSRPCClient.h"
#include "EPICSObjectRegistryDatabaseService.h"
#include "EPICSObjectRegistryDatabaseServiceTest.h"
#include "EPICSRPCServer.h"
#include "ObjectRegistryDatabase.h"
#include "MessageI.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool EPICSObjectRegistryDatabaseServiceTest::TestConstructor() {
    using namespace MARTe;
    EPICSObjectRegistryDatabaseService test;
    return (test.NumberOfReferences() == 0);
}

bool EPICSObjectRegistryDatabaseServiceTest::Testrequest() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSRPCServer = {"
            "    Class = EPICSPVA::EPICSRPCServer"
            "    +EPICSObjectRegistryDatabaseService = {"
            "        Class = EPICSPVA::EPICSObjectRegistryDatabaseService"
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
    if (ok) {
        ConfigurationDatabase msgConfig;
        msg->SetName("EPICSObjectRegistryDatabaseService");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msg->Initialise(msgConfig);
        msg->SetExpectsReply(true);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msg, &notReallyUsed);
    }
    ReferenceT<StructuredDataI> replyStruct;
    if (ok) {
        replyStruct = msg->Get(0u);
        ok = replyStruct.IsValid();
    }
    if (ok) {
        ok = replyStruct->MoveAbsolute("EPICSRPCServer.EPICSObjectRegistryDatabaseService");
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}


	
