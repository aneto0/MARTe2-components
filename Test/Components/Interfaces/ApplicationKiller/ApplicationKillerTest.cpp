/**
 * @file ApplicationKillerTest.cpp
 * @brief Source file for class ApplicationKillerTest
 * @date 30/04/2025
 * @author Llorenc Capella
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
 * the class ApplicationKillerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "signal.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ApplicationKillerTest.h"
#include "ApplicationKiller.h"
#include "AdvancedErrorManagement.h"
#include "Sleep.h"

#include "ConfigurationDatabase.h"
#include "StandardParser.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StateMachine.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
static bool signalCaught = 0;

void SignalHandler(int signal) {
    if (signal == SIGUSR1) {
        signalCaught = true;
    }
}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ApplicationKillerTest::ApplicationKillerTest() {
}

ApplicationKillerTest::~ApplicationKillerTest() {
//Auto-generated destructor stub for ApplicationKillerTest

//TODO Verify if manual additions are needed here
}

static bool InitialiseEnviroment(const char8 *const config) {
    using namespace MARTe;
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    //if (ok) {
    //    ok = application->ConfigureApplication();
    //}
    return ok;
}

const MARTe::char8 *const cfg = ""
        "+ApplicationKiller = {"
        "    Class = ApplicationKiller"
        "}"
        "+StateMachine = {"
        "    Class = StateMachine"
        "    +INITIAL = {"
        "         Class = ReferenceContainer"
        "         +TestKill = {"
        "             Class = StateMachineEvent"
        "             NextState = INITIAL"
        "             NextStateError = INITIAL"
        "             Timeout = 0"
        "             +KillAppMsg = {"
        "                 Class = Message"
        "                 Destination = ApplicationKiller"
        "                 Function = Kill"
        "                 Mode = ExpectsReply"
        "             }"
        "         }"
        "     }"
        "}";

bool ApplicationKillerTest::Test_KillOnSucceed() {
    struct sigaction sa;
    ApplicationKiller myKiller;
    Sleep mySleep;
    bool ret;
    signalCaught = false;
    sa.sa_handler = SignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    ret = sigaction(SIGUSR1, &sa, NULL_PTR(struct sigaction*)) == 0;
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to install signal handler");
    }
    if (ret) {
        ret = signalCaught == false; // check that signal is not yet triggered.
    }
    if (ret) {
        myKiller.Kill();
        mySleep.MSec(1u); // give time to catch signal and update signal Caught
    }
    if (ret) {
        ret = signalCaught == true; //Check that the signal is triggered
    }
    return ret;
}

bool ApplicationKillerTest::Test_KillOnSucceedMARTe() {
    signalCaught = false;
    bool ret = InitialiseEnviroment(cfg);
    ReferenceT<ApplicationKiller> myKiller;
    ReferenceT<StateMachine> myStateMachine;
    ReferenceT<StateMachineEvent> myStateMachineEvent;
    if (ret) {
        myKiller = MARTe::ObjectRegistryDatabase::Instance()->Find("ApplicationKiller");
        ret = myKiller.IsValid();
    }
    if (ret) {
        myStateMachine = MARTe::ObjectRegistryDatabase::Instance()->Find("StateMachine");
        ret = myStateMachine.IsValid();
    }
    if (ret) {
        myStateMachineEvent = ObjectRegistryDatabase::Instance()->Find("StateMachine.INITIAL.TestKill");
        ret = myStateMachineEvent.IsValid();
    }
    if (ret) {
        myStateMachine->EventTriggered(myStateMachineEvent);
    }
    if (ret) {
        ret = signalCaught == true;
    }
    return ret;
}

}

