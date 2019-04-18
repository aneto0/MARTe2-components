/**
 * @file EPICSRPCServerTest.cpp
 * @brief Source file for class EPICSRPCServerTest
 * @date 25/10/2018
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
 * the class EPICSRPCServerTest (public, protected, and private). Be aware that some 
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
#include "EPICSPVAStructureDataI.h"
#include "File.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "Vector.h"
#include "EPICSRPCServerTest.h"
#include "EPICSRPCServer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool EPICSRPCServerTest::TestConstructor() {
    using namespace MARTe;
    EPICSRPCServer rpcServer;

    return (rpcServer.NumberOfReferences() == 0u);
}

bool EPICSRPCServerTest::TestInitialise() {
    using namespace MARTe;
    EPICSRPCServer rpcServer;
    ConfigurationDatabase cdb;
    uint32 cpus = 1;
    uint32 stackSize = 1000000;
    uint32 autoStart = 0;

    cdb.Write("CPUs", cpus);
    cdb.Write("StackSize", stackSize);
    cdb.Write("AutoStart", autoStart);
    bool ok = rpcServer.Initialise(cdb);
    ok &= (rpcServer.GetCPUMask() == cpus);
    ok &= (rpcServer.GetStackSize() == stackSize);
    ok &= (rpcServer.GetStatus() == EmbeddedThread::OffState);
    Sleep::Sec(0.5);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool EPICSRPCServerTest::TestInitialise_Defaults() {
    using namespace MARTe;
    EPICSRPCServer rpcServer;
    ConfigurationDatabase cdb;
    bool ok = rpcServer.Initialise(cdb);
    ok &= (rpcServer.GetCPUMask() == 0xffu);
    ok &= (rpcServer.GetStackSize() == THREADS_DEFAULT_STACKSIZE * 4u);
    ok &= (rpcServer.GetStatus() != EmbeddedThread::OffState);
    Sleep::Sec(0.5);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool EPICSRPCServerTest::TestStart() {
    using namespace MARTe;
    EPICSRPCServer rpcServer;
    ConfigurationDatabase cdb;
    uint32 autoStart = 0;

    cdb.Write("AutoStart", autoStart);
    bool ok = rpcServer.Initialise(cdb);
    ok = (rpcServer.Start() == ErrorManagement::NoError);
    ok &= (rpcServer.GetStatus() != EmbeddedThread::OffState);
    Sleep::Sec(0.5);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool EPICSRPCServerTest::TestExecute() {
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
    Sleep::Sec(0.5);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool EPICSRPCServerTest::TestGetStatus() {
    return TestInitialise();
}

bool EPICSRPCServerTest::TestGetStackSize() {
    return TestInitialise();
}

bool EPICSRPCServerTest::TestGetCPUMask() {
    return TestInitialise();
}
