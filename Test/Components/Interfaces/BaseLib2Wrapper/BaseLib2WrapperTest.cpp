/**
 * @file BaseLib2WrapperTest.cpp
 * @brief Source file for class BaseLib2WrapperTest
 * @date 05/12/2016
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
 * the class BaseLib2WrapperTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BaseLib2Wrapper.h"
#include "BaseLib2WrapperTest.h"
#include "ConfigurationDatabase.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "StateMachine.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool BaseLib2WrapperTest::TestConstructor() {
    MARTe::BaseLib2Wrapper bl2Wrapper;
    return true;
}

bool BaseLib2WrapperTest::TestInitialise() {
    using namespace MARTe;
    const char8 * const config = ""
            "+BaseLib2Wrapper = {"
            "    Class = BaseLib2Wrapper"
            "    BaseLib2Config = \""
            "        +StateMachine = {"
            "            Class = StateMachine"
            "            ErrorStateName = IDLE"
            "            +INITIAL = {"
            "                Class = StateMachineState"
            "                StateCode = 0x0"
            "                +START = {"
            "                    Class = StateMachineEvent"
            "                    Code = 0x1"
            "                    NextState = IDLE"
            "                    ErrorState = IDLE"
            "                }"
            "            }"
            "            +IDLE = {"
            "                Class = StateMachineState"
            "                StateCode = 0x1"
            "                +START = {"
            "                    Class = StateMachineEvent"
            "                    Code = 0x2"
            "                    NextState = RUN"
            "                    ErrorState = IDLE"
            "                }"
            "            }"
            "            +RUN = {"
            "                Class = StateMachineState"
            "                StateCode = 0x2"
            "                +STOP = {"
            "                    Class = StateMachineEvent"
            "                    Code = 0x1"
            "                    NextState = IDLE"
            "                    ErrorState = IDLE"
            "                }"
            "            }"
            "        }"
            "    \""
            "}";
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();

    if (ok) {
        ord->Purge();
        ok = ord->Initialise(cdb);
    }
    ord->Purge();
    return ok;
}

bool BaseLib2WrapperTest::TestInitialise_False_NoBaseLib2Config() {
    using namespace MARTe;
    const char8 * const config = ""
            "+BaseLib2Wrapper = {"
            "    Class = BaseLib2Wrapper"
            "}";
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();

    if (ok) {
        ord->Purge();
        ok = ord->Initialise(cdb);
    }
    ord->Purge();
    return !ok;
}

bool BaseLib2WrapperTest::TestInitialise_False_BadBaseLib2Config() {
    using namespace MARTe;
    const char8 * const config = ""
            "+BaseLib2Wrapper = {"
            "    Class = BaseLib2Wrapper"
            "    BaseLib2Config = \""
            "        +StateMachine = {"
            "            Class = StateMachine"
            "            ErrorStateName = IDLE"
            "            +INITIAL = {"
            "                Class = StateMachineState"
            "                +START = {"
            "                    Class = StateMachineEvent"
            "                    NextState = IDLE"
            "                    ErrorState = IDLE"
            "                }"
            "            }"
            "            +IDLE = {"
            "                Class = StateMachineState"
            "                StateCode = 0x1"
            "                +START = {"
            "                    Class = StateMachineEvent"
            "                    Code = 0x2"
            "                    NextState = RUN"
            "                    ErrorState = IDLE"
            "                }"
            "            }"
            "            +RUN = {"
            "                Class = StateMachineState"
            "                StateCode = 0x2"
            "                +STOP = {"
            "                    Class = StateMachineEvent"
            "                    Code = 0x1"
            "                    NextState = IDLE"
            "                    ErrorState = IDLE"
            "                }"
            "            }"
            "        }"
            "    \""
            "}";
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();

    if (ok) {
        ord->Purge();
        ok = ord->Initialise(cdb);
    }
    ord->Purge();
    return !ok;
}

bool BaseLib2WrapperTest::TestHandleBaseLib2Message() {
    using namespace MARTe;
    const char8 * const config = ""
            "+StateMachine = {"
            "    Class = StateMachine"
            "    +A = {"
            "        Class = ReferenceContainer"
            "        +E1 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"B\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "            +TriggerBaseLib2StateMachineRun = {"
            "                Class = Message"
            "                Destination = \"BaseLib2Wrapper\""
            "                Function = \"StateMachine::RUN\""
            "            }"
            "        }"
            "    }"
            "    +B = {"
            "        Class = ReferenceContainer"
            "        +E2 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"C\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "    +C = {"
            "        Class = ReferenceContainer"
            "        +E3 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "    +E = {"
            "        Class = ReferenceContainer"
            "        +E4 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "}"
            "+TriggerMsg = {"
            "    Class = Message"
            "    Destination = StateMachine"
            "    Function = E1"
            "}"
            "+BaseLib2Wrapper = {"
            "    Class = BaseLib2Wrapper"
            "    BaseLib2Config = \""
            "        +AdapterMessageHandler = {"
            "            Class = AdapterMessageHandler"
            "        }"
            "        +StateMachine = {"
            "            Class = StateMachine"
            "            ErrorStateName = IDLE"
            "            +IDLE = {"
            "                Class = StateMachineState"
            "                StateCode = 0x1"
            "                +RUN = {"
            "                    Class = StateMachineEvent"
            "                    Value = RUN"
            "                    NextState = RUN"
            "                    ErrorState = IDLE"
            "                    +NOTIFY = {"
            "                        Class = MessageEnvelope"
            "                        Sender = StateMachine"
            "                        Destination = AdapterMessageHandler"
            "                        +MESSAGE = {"
            "                            Class = Message"
            "                            Content = StateMachine::E2"
            "                        }"
            "                    }"
            "                }"
            "            }"
            "            +RUN = {"
            "                Class = StateMachineState"
            "                StateCode = 0x2"
            "                +STOP = {"
            "                    Class = StateMachineEvent"
            "                    Value = STOP"
            "                    NextState = IDLE"
            "                    ErrorState = IDLE"
            "                }"
            "            }"
            "        }"
            "    \""
            "}";
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();

    if (ok) {
        ord->Purge();
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        ReferenceT<Message> msg = ord->Find("TriggerMsg");
        ok = msg.IsValid();
        if (ok) {
            ok = MessageI::SendMessage(msg, NULL);
        }
    }
    ReferenceT<StateMachine> sm;
    ReferenceT<ReferenceContainer> stateC;
    if (ok) {
        sm = ord->Find("StateMachine");
        stateC = ord->Find("StateMachine.C");
    }
    if (ok) {
        ok = sm.IsValid();
    }
    if (ok) {
        ok = stateC.IsValid();
    }
    if (ok) {
        uint32 counter = 0u;
        ok = false;
        while ((counter < 50) && (!ok)) {
            ok = (sm->GetCurrentState() == stateC);
            counter++;
            if (!ok) {
                Sleep::Sec(0.1);
            }
        }
    }
    ord->Purge();
    return ok;
}

