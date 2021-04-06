/**
 * @file MessageGAMTest.cpp
 * @brief Source file for class MessageGAMTest
 * @date 02/07/2019
 * @author Giuseppe Ferrò
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
 * the class MessageGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "MemoryDataSourceI.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "MessageGAMTest.h"
#include "MessageI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class MessageGAMTestGAM: public MessageGAM {
public:
    CLASS_REGISTER_DECLARATION()

    void *GetInputMemoryX();

    void *GetOutputMemoryX();
};

void *MessageGAMTestGAM::GetInputMemoryX() {
    return GetInputSignalsMemory();
}

void *MessageGAMTestGAM::GetOutputMemoryX() {
    return GetOutputSignalsMemory();
}

CLASS_REGISTER(MessageGAMTestGAM, "1.0")

class MessageGAMTestDS: public MemoryDataSourceI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    MessageGAMTestDS();

    virtual ~MessageGAMTestDS();

    const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    bool ChangeCommand(uint32 index,
            uint32 value);

    ErrorManagement::ErrorType TrigFun1();
    ErrorManagement::ErrorType TrigFun2();
    ErrorManagement::ErrorType TrigFun3();

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);
    virtual bool Synchronise();

    void ResetFlag();
    uint8 GetFlag();

    void SetSleepTime(uint32 sleepTime);

private:
    uint8 callFunFlag;
    uint32 sleepTimeMSec;
};

MessageGAMTestDS::MessageGAMTestDS() :
        MessageI() {
    ReferenceT < RegisteredMethodsMessageFilter > filter = ReferenceT < RegisteredMethodsMessageFilter > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
    callFunFlag = 0;
    sleepTimeMSec = 0u;
}

MessageGAMTestDS::~MessageGAMTestDS() {

}

void MessageGAMTestDS::ResetFlag() {
    callFunFlag = 0;
}

uint8 MessageGAMTestDS::GetFlag() {
    return callFunFlag;
}

void MessageGAMTestDS::SetSleepTime(uint32 sleepTime) {
    sleepTimeMSec = sleepTime;
}

ErrorManagement::ErrorType MessageGAMTestDS::TrigFun1() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function1");
    callFunFlag |= 1;
    if (sleepTimeMSec > 0) {
        Sleep::MSec (sleepTimeMSec);
    }
    return ret;
}

ErrorManagement::ErrorType MessageGAMTestDS::TrigFun2() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function2");
    callFunFlag |= (1 << 1);
    if (sleepTimeMSec > 0) {
        Sleep::MSec (sleepTimeMSec);
    }
    return ret;
}

ErrorManagement::ErrorType MessageGAMTestDS::TrigFun3() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function3");
    callFunFlag |= (1 << 2);
    if (sleepTimeMSec > 0) {
        Sleep::MSec (sleepTimeMSec);
    }
    return ret;
}

bool MessageGAMTestDS::ChangeCommand(uint32 index,
                                     uint32 value) {
    uint32 *signalAddress = NULL;
    bool ret = GetSignalMemoryBuffer(0, 0, (void*&) signalAddress);
    if (ret) {
        signalAddress[index] = value;
    }
    return ret;
}

const char8 *MessageGAMTestDS::GetBrokerName(StructuredDataI &data,
                                             const SignalDirection direction) {

    const char8* brokerName = NULL_PTR(const char8 *);

    if (direction == InputSignals) {

        float32 frequency = 0.F;
        if (data.Read("Frequency", frequency)) {
            if (frequency > 0.) {
                brokerName = "MemoryMapSynchronisedInputBroker";
            }
        }
        uint32 trigger = 0u;
        if (data.Read("Trigger", trigger)) {
            if (trigger > 0u) {
                brokerName = "MemoryMapSynchronisedInputBroker";
            }
        }
        if (brokerName == NULL_PTR(const char8 *)) {
            brokerName = "MemoryMapInputBroker";
        }
    }

    if (direction == OutputSignals) {

        float32 frequency = 0.F;
        if (data.Read("Frequency", frequency)) {
            if (frequency > 0.) {
                brokerName = "MemoryMapSynchronisedOutputBroker";
            }
        }
        uint32 trigger = 0u;
        if (data.Read("Trigger", trigger)) {
            if (trigger > 0u) {
                brokerName = "MemoryMapSynchronisedOutputBroker";
            }
        }
        if (brokerName == NULL_PTR(const char8 *)) {
            brokerName = "MemoryMapOutputBroker";
        }
    }

    return brokerName;
}

bool MessageGAMTestDS::PrepareNextState(const char8 * const currentStateName,
                                        const char8 * const nextStateName) {
    return true;
}

bool MessageGAMTestDS::Synchronise() {
    return true;
}

CLASS_REGISTER(MessageGAMTestDS, "1.0")

CLASS_METHOD_REGISTER(MessageGAMTestDS, TrigFun1)
CLASS_METHOD_REGISTER(MessageGAMTestDS, TrigFun2)
CLASS_METHOD_REGISTER(MessageGAMTestDS, TrigFun3)

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapInputBrokerEnviroment(const char8 * const config) {

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
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Application");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MessageGAMTest::MessageGAMTest() {
}

MessageGAMTest::~MessageGAMTest() {
}

bool MessageGAMTest::TestConstructor() {
    MessageGAMTestGAM test;

    bool ret = (test.GetNumberOfCommands() == 0u);
    ret &= (test.GetNumberOfEvents() == 0u);

    return ret;
}

bool MessageGAMTest::TestSetup() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = MessageGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                    }"
            "                    +Fun1Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 2"
            "                    }"
            "                    +Fun2Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 3"
            "                    }"
            "                    +Fun3Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 ReplyState = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 ReplyState1 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = MessageGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<MessageGAMTestGAM> test;
    if (ret) {
        test = ObjectRegistryDatabase::Instance()->Find("Application.Functions.GAM1");
        ret = test.IsValid();
    }
    if (ret) {
        ret = (test->GetNumberOfCommands() == 2u);
    }
    if (ret) {
        ret = (test->GetNumberOfEvents() == 3u);
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool MessageGAMTest::TestSetupNumberOfElements() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = MessageGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command_0 = 1"
            "                    }"
            "                    +Fun1Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command_0 = 2"
            "                    }"
            "                    +Fun2Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command_1 = 3"
            "                    }"
            "                    +Fun3Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                     NumberOfElements=2"
            "                 }"
            "                 State = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 ReplyState = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 ReplyState1 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = MessageGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<MessageGAMTestGAM> test;
    if (ret) {
        test = ObjectRegistryDatabase::Instance()->Find("Application.Functions.GAM1");
        ret = test.IsValid();
    }
    if (ret) {
        ret = (test->GetNumberOfCommands() == 2u);
    }
    if (ret) {
        ret = (test->GetNumberOfEvents() == 3u);
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool MessageGAMTest::TestSetup_FalseNoCommands() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = MessageGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                    }"
            "                    +Fun1Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 2"
            "                    }"
            "                    +Fun2Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 3"
            "                    }"
            "                    +Fun3Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 State = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = MessageGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool MessageGAMTest::TestSetup_FalseInOutMismatch() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = MessageGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        State1 = 1"
            "                    }"
            "                    +Fun1Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        State1 = 2"
            "                    }"
            "                    +Fun2Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        State1 = 3"
            "                    }"
            "                    +Fun3Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "                 State1 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 ReplyState = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 ReplyState1 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = MessageGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool MessageGAMTest::TestSetup_FalseStateBadType() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = MessageGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                    }"
            "                    +Fun1Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 2"
            "                    }"
            "                    +Fun2Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 3"
            "                    }"
            "                    +Fun3Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 ReplyState = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 ReplyState1 = {"
            "                     DataSource = DDB1"
            "                     Type = uint64"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = MessageGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;
}

bool MessageGAMTest::TestExecute() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = MessageGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                    }"
            "                    +Fun1Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 2"
            "                    }"
            "                    +Fun2Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 3"
            "                    }"
            "                    +Fun3Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 ReplyState = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 ReplyState1 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = MessageGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<MessageGAMTestGAM> test;
    if (ret) {
        test = ObjectRegistryDatabase::Instance()->Find("Application.Functions.GAM1");
        ret = test.IsValid();
    }
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapInputBroker > brokerIn1;

    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        test->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after InputBrokers(0) IsValid");
    }

    if (ret) {
        brokerIn1 = inputBrokers.Get(1);
        ret = brokerIn1.IsValid();
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after InputBrokers(1) IsValid");
    }

    if (ret) {
        test->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }
    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after OutputBrokers IsValid");
    }

    ReferenceT < MessageGAMTestDS > ds;
    if (ret) {
        ds = ObjectRegistryDatabase::Instance()->Find("Application.Data.Input");
        ret = ds.IsValid();
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after MessageGAMDS IsValid");
    }

    uint32 *outMem = NULL;
    if (ret) {
        ds->PrepareNextState("Idle", "Idle");
        test->PrepareNextState("Idle", "Idle");

        outMem = (uint32*) test->GetOutputMemoryX();
    }
    bool retBrokerIn, retBrokerIn1, retTest, retBrokerOut;

    if (ret) {
        ds->ChangeCommand(0u, 10u);
        retBrokerIn = brokerIn->Execute();
        retBrokerIn1 = brokerIn1->Execute();
        retTest = test->Execute();
        
        retBrokerOut = brokerOut->Execute();
    }

    uint32 iterationCount = 0u;
        
    if (ret) {
        ret = false;
        ds->ChangeCommand(0u, 1u);

        while(!ret && (iterationCount++ < 20)) {
            retBrokerIn = brokerIn->Execute();
            retBrokerIn1 = brokerIn1->Execute();
            retTest = test->Execute();
            
            retBrokerOut = brokerOut->Execute();
            REPORT_ERROR_STATIC(ErrorManagement::Information, "outMem %d - %d", outMem[0], outMem[1]);
            ret = ((outMem[0] == 1) && (outMem[1] == 0));
            Sleep::MSec(250);
        }
    }
    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after change command 0, 1 %d %d %d %d %d %d", outMem[0], outMem[1], retBrokerIn, retBrokerIn1, retTest, retBrokerOut);
    }

    ds->ChangeCommand(0u, 2u);

    if(ret) {
        ret = false;
        while(!ret && (iterationCount++ < 20)) {
            brokerIn->Execute();
            brokerIn1->Execute();
            test->Execute();
            brokerOut->Execute();
            ret = ((outMem[0] >= 1) && (outMem[1] == 0));
            REPORT_ERROR_STATIC(ErrorManagement::Information, "outMem %d - %d", outMem[0], outMem[1]);
            Sleep::MSec(250);
        }
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after change command 0, 2");
    }

    iterationCount = 0u;
    ds->ChangeCommand(0u, 3u);
        
    if (ret) {
        ret = false;
        while(!ret && (iterationCount++ < 20)) {
            brokerIn->Execute();
            brokerIn1->Execute();
            test->Execute();
            brokerOut->Execute();
            ret = (outMem[0] >= 1);
            REPORT_ERROR_STATIC(ErrorManagement::Information, "outMem %d - %d", outMem[0], outMem[1]);
            Sleep::MSec(250);
        }
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after change command 0, 3");
    }

    if (ret) {
        ret = false;
        iterationCount = 0u;

        while(!ret && (iterationCount++ < 20)) {
            ret = (ds->GetFlag() == 0x7);
            Sleep::MSec(250);
        }
    }
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Purging...");

    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool MessageGAMTest::TestPrepareNextState() {
    return TestExecute();
}

bool MessageGAMTest::TestPurge() {
    return TestExecute();
}

bool MessageGAMTest::TestExecute_MoreCommands() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = MessageGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                    }"
            "                    +Fun1Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 2"
            "                    }"
            "                    +Fun2Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 3"
            "                    }"
            "                    +Fun3Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 ReplyState = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 ReplyState1 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = MessageGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<MessageGAMTestGAM> test;
    if (ret) {
        test = ObjectRegistryDatabase::Instance()->Find("Application.Functions.GAM1");
        ret = test.IsValid();
    }
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapInputBroker > brokerIn1;

    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        test->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after InputBrokers(0) IsValid");
    }

    if (ret) {
        brokerIn1 = inputBrokers.Get(1);
        ret = brokerIn1.IsValid();
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after InputBrokers(1) IsValid");
    }

    if (ret) {
        test->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after OutputBrokers IsValid");
    }

    ReferenceT < MessageGAMTestDS > ds;
    if (ret) {
        ds = ObjectRegistryDatabase::Instance()->Find("Application.Data.Input");
        ret = ds.IsValid();
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after MessageGAMTestDS IsValid");
    }

    uint32 *outMem = NULL;
    if (ret) {
        ds->PrepareNextState("Idle", "Idle");
        outMem = (uint32*) test->GetOutputMemoryX();
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after PrepareNextState check");
    }

    if (ret) {
        ds->ChangeCommand(0u, 10u);
        brokerIn->Execute();
        brokerIn1->Execute();
        test->Execute();
      
        brokerOut->Execute();
    }

    uint32 iterationCount = 0u;

    if(ret) {
        ret = false;
        iterationCount = 0u;
        ds->ChangeCommand(0u, 1u);
        while(!ret && (iterationCount++ < 20)) {
            brokerIn->Execute();
            brokerIn1->Execute();
            test->Execute();
            brokerOut->Execute();
            ret = ((outMem[0] >= 1) && (outMem[1] == 0));
            Sleep::MSec(250);
        }
    }

    if(ret) {
        ret = false;
        iterationCount = 0u;
        ds->ChangeCommand(0u, 2u);
        while(!ret && (iterationCount++ < 20)) {
            brokerIn->Execute();
            brokerIn1->Execute();
            test->Execute();
            brokerOut->Execute();
            ret = ((outMem[0] >= 1) && (outMem[1] == 0));
            Sleep::MSec(250);
        }
    }

    if(ret) {
        ret = false;
        iterationCount = 0u;
        ds->ChangeCommand(2u, 3u);
        while(!ret && (iterationCount++ < 20)) {
            brokerIn->Execute();
            brokerIn1->Execute();
            test->Execute();
            brokerOut->Execute();
            ret = (outMem[1] == 1);
            Sleep::MSec(250);
        }
    }

    if (ret) {
        Sleep::Sec(2.0);
        ret = (ds->GetFlag() == 0x7);
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after 4th check");
    }

    ObjectRegistryDatabase::Instance()->Purge();

    return ret;

}

bool MessageGAMTest::TestExecute_CommandsAndStates() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = MessageGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                        State = 1"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 2"
            "                        State = 2"
            "                        State1 = 4"
            "                    }"
            "                    +Fun2Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                    +Fun1Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 3"
            "                        State = 5"
            "                        State1 = 1"
            "                    }"
            "                    +Fun3Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "                 State1 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 ReplyState = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 ReplyState1 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = MessageGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<MessageGAMTestGAM> test;
    if (ret) {
        test = ObjectRegistryDatabase::Instance()->Find("Application.Functions.GAM1");
        ret = test.IsValid();
    }
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapInputBroker > brokerIn1;

    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        test->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }
    if (ret) {
        brokerIn1 = inputBrokers.Get(1);
        ret = brokerIn1.IsValid();
    }
    if (ret) {
        test->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }

    ReferenceT < MessageGAMTestDS > ds;
    if (ret) {
        ds = ObjectRegistryDatabase::Instance()->Find("Application.Data.Input");
        ret = ds.IsValid();
    }

    uint32 *outMem = NULL;
    if (ret) {
        ds->PrepareNextState("Idle", "Idle");
        outMem = (uint32*) test->GetOutputMemoryX();
    }

    if (ret) {
        ds->ChangeCommand(0u, 1u);
        ds->ChangeCommand(1u, 1u);
        brokerIn->Execute();
        brokerIn1->Execute();
        test->Execute();
        ret = ((outMem[0] == 0) && (outMem[1] == 0));
    }
    if (ret) {
        brokerOut->Execute();
        ds->ChangeCommand(0u, 2u);
        ds->ChangeCommand(1u, 2u);
        ds->ChangeCommand(2u, 4u);
        brokerIn->Execute();
        brokerIn1->Execute();
        test->Execute();
        ret = ((outMem[0] >= 1) && (outMem[1] == 0));
    }
    if (ret) {
        brokerOut->Execute();
        ds->ChangeCommand(3u, 3u);
        ds->ChangeCommand(1u, 5u);
        ds->ChangeCommand(2u, 1u);
        brokerIn->Execute();
        brokerIn1->Execute();
        test->Execute();
        brokerOut->Execute();
        ret = (outMem[1] == 1);
    }
    if (ret) {
        Sleep::Sec(2);
        ret = (ds->GetFlag() == 0x7);
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;

}

bool MessageGAMTest::TestExecute_Commands() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = MessageGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                        Command1 = 1"
            "                        State = 1"
            "                    }"
            "                    +Fun1Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "             }"
            "             InputSignals = {"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "                 State1 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 ReplyState = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 ReplyState1 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = MessageGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<MessageGAMTestGAM> test;
    if (ret) {
        test = ObjectRegistryDatabase::Instance()->Find("Application.Functions.GAM1");
        ret = test.IsValid();
    }
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapInputBroker > brokerIn1;

    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        test->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }
    if (ret) {
        brokerIn1 = inputBrokers.Get(1);
        ret = brokerIn1.IsValid();
    }
    if (ret) {
        test->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }

    ReferenceT < MessageGAMTestDS > ds;
    if (ret) {
        ds = ObjectRegistryDatabase::Instance()->Find("Application.Data.Input");
        ret = ds.IsValid();
    }

    uint32 *outMem = NULL;
    if (ret) {
        ds->PrepareNextState("Idle", "Idle");
        outMem = (uint32*) test->GetOutputMemoryX();
    }

    if (ret) {
        ds->ChangeCommand(0u, 10u);
        brokerIn->Execute();
        brokerIn1->Execute();
        test->Execute();
        
        brokerOut->Execute();
    }

    if (ret) {
        ds->ChangeCommand(0u, 1u);
        ds->ChangeCommand(1u, 1u);
        ds->ChangeCommand(3u, 1u);

        brokerIn->Execute();
        brokerIn1->Execute();
        test->Execute();
        ret = (outMem[0] == 1) && (outMem[1] == 1);
    }
    if (ret) {
        Sleep::Sec(2);
        ret = (ds->GetFlag() == 0x1);
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;
}

bool MessageGAMTest::TestExecute_WithoutTriggerOnChange() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = MessageGAMTestGAM"
            "           TriggerOnChange = 0"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                    }"
            "                    +Fun1Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 2"
            "                    }"
            "                    +Fun2Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 3"
            "                    }"
            "                    +Fun3Mess = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 ReplyState = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 ReplyState1 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = MessageGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<MessageGAMTestGAM> test;
    if (ret) {
        test = ObjectRegistryDatabase::Instance()->Find("Application.Functions.GAM1");
        ret = test.IsValid();
    }
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapInputBroker > brokerIn1;

    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        test->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after InputBrokers(0) IsValid");
    }

    if (ret) {
        brokerIn1 = inputBrokers.Get(1);
        ret = brokerIn1.IsValid();
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after InputBrokers(1) IsValid");
    }

    if (ret) {
        test->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }
    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after OutputBrokers IsValid");
    }

    ReferenceT < MessageGAMTestDS > ds;
    if (ret) {
        ds = ObjectRegistryDatabase::Instance()->Find("Application.Data.Input");
        ret = ds.IsValid();
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failing after MessageGAMDS IsValid");
    }

    uint32 *outMem = NULL;
    if (ret) {
        ds->PrepareNextState("Idle", "Idle");
        test->PrepareNextState("Idle", "Idle");

        outMem = (uint32*) test->GetOutputMemoryX();
    }
    bool retBrokerIn, retBrokerIn1, retTest, retBrokerOut;

    if (ret) {
        ds->ChangeCommand(0u, 1u);
        brokerIn->Execute();
        brokerIn1->Execute();
        test->Execute();
        
        brokerOut->Execute();
        ret = ((outMem[0] == 1) && (outMem[1] == 0));
    }

    if (ret) {
        Sleep::Sec(2);
        ret = (ds->GetFlag() == 0x1);
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ret;

}


bool MessageGAMTest::TestGetNumberOfCommands() {
    return TestSetup();
}

bool MessageGAMTest::TestGetNumberOfEvents() {
    return TestSetup();
}

