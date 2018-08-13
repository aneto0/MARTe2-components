/**
 * @file TriggerOnChangeGAMTest.cpp
 * @brief Source file for class TriggerOnChangeGAMTest
 * @date 17/07/2018
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
 * the class TriggerOnChangeGAMTest (public, protected, and private). Be aware that some 
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
#include "MessageI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "TriggerOnChangeGAMTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class TriggerOnChangeGAMTestGAM: public TriggerOnChangeGAM {
public:
    CLASS_REGISTER_DECLARATION()

    TriggerOnChangeGAMTestGAM();
    ~TriggerOnChangeGAMTestGAM();

    uint8 *GetPreviousValue();
    uint32 *GetState();
    PacketField *GetPacketConfig();
    uint32 GetNumberOfFields();
    ReferenceT<ReferenceContainer> GetEvents();
    uint32 GetNumberOfEvents();
    uint32 *GetCommandIndex();
    uint32 GetNumberOfCommands();
    uint32 *GetCntReplied();
    uint32 *GetCntTrigger();
    uint8 *GetCurrentValue();
    uint8 *GetAckValue();

    void *GetInputSignalsMemory();

    void *GetOutputSignalsMemory();

};

TriggerOnChangeGAMTestGAM::TriggerOnChangeGAMTestGAM() {

}

TriggerOnChangeGAMTestGAM::~TriggerOnChangeGAMTestGAM() {

}

void *TriggerOnChangeGAMTestGAM::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void *TriggerOnChangeGAMTestGAM::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

uint8 *TriggerOnChangeGAMTestGAM::GetPreviousValue() {
    return previousValue;

}

uint32 *TriggerOnChangeGAMTestGAM::GetState() {
    return state;
}

PacketField *TriggerOnChangeGAMTestGAM::GetPacketConfig() {
    return packetConfig;
}

uint32 TriggerOnChangeGAMTestGAM::GetNumberOfFields() {
    return numberOfFields;
}

ReferenceT<ReferenceContainer> TriggerOnChangeGAMTestGAM::GetEvents() {
    return events;
}

uint32 TriggerOnChangeGAMTestGAM::GetNumberOfEvents() {
    return numberOfEvents;
}

uint32 *TriggerOnChangeGAMTestGAM::GetCommandIndex() {
    return commandIndex;
}

uint32 TriggerOnChangeGAMTestGAM::GetNumberOfCommands() {
    return numberOfCommands;
}

uint32 *TriggerOnChangeGAMTestGAM::GetCntReplied() {
    return cntReplied;
}

uint32 *TriggerOnChangeGAMTestGAM::GetCntTrigger() {
    return cntTrigger;
}
uint8 *TriggerOnChangeGAMTestGAM::GetCurrentValue() {
    return currentValue;
}
uint8 *TriggerOnChangeGAMTestGAM::GetAckValue() {
    return ackValue;
}

CLASS_REGISTER(TriggerOnChangeGAMTestGAM, "1.0")

class TriggerOnChangeGAMTestDS: public MemoryDataSourceI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    TriggerOnChangeGAMTestDS();

    virtual ~TriggerOnChangeGAMTestDS();

    const char8 *GetBrokerName(StructuredDataI &data,const SignalDirection direction);

    bool ChangeCommand(uint32 index, uint32 value);

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

TriggerOnChangeGAMTestDS::TriggerOnChangeGAMTestDS() :
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

TriggerOnChangeGAMTestDS::~TriggerOnChangeGAMTestDS() {

}

void TriggerOnChangeGAMTestDS::ResetFlag() {
    callFunFlag = 0;
}

uint8 TriggerOnChangeGAMTestDS::GetFlag() {
    return callFunFlag;
}

void TriggerOnChangeGAMTestDS::SetSleepTime(uint32 sleepTime) {
    sleepTimeMSec = sleepTime;
}

ErrorManagement::ErrorType TriggerOnChangeGAMTestDS::TrigFun1() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function1");
    callFunFlag |= 1;
    if (sleepTimeMSec > 0) {
        Sleep::MSec (sleepTimeMSec);
    }
    return ret;
}

ErrorManagement::ErrorType TriggerOnChangeGAMTestDS::TrigFun2() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function2");
    callFunFlag |= (1 << 1);
    if (sleepTimeMSec > 0) {
        Sleep::MSec (sleepTimeMSec);
    }
    return ret;
}

ErrorManagement::ErrorType TriggerOnChangeGAMTestDS::TrigFun3() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function3");
    callFunFlag |= (1 << 2);
    if (sleepTimeMSec > 0) {
        Sleep::MSec (sleepTimeMSec);
    }
    return ret;
}

bool TriggerOnChangeGAMTestDS::ChangeCommand(uint32 index,
                                             uint32 value) {
    uint32 *signalAddress = NULL;
    bool ret = GetSignalMemoryBuffer(0, 0, (void*&) signalAddress);
    if (ret) {
        signalAddress[index] = value;
    }
    return ret;
}

const char8 *TriggerOnChangeGAMTestDS::GetBrokerName(StructuredDataI &data,
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

bool TriggerOnChangeGAMTestDS::PrepareNextState(const char8 * const currentStateName,
                                                const char8 * const nextStateName) {
    return true;
}

bool TriggerOnChangeGAMTestDS::Synchronise() {
    return true;
}

CLASS_REGISTER(TriggerOnChangeGAMTestDS, "1.0")

CLASS_METHOD_REGISTER(TriggerOnChangeGAMTestDS, TrigFun1)
CLASS_METHOD_REGISTER(TriggerOnChangeGAMTestDS, TrigFun2)
CLASS_METHOD_REGISTER(TriggerOnChangeGAMTestDS, TrigFun3)
/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapInputBrokerEnviroment(const char8 * const config) {

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

TriggerOnChangeGAMTest::TriggerOnChangeGAMTest() {
    // Auto-generated constructor stub for TriggerOnChangeGAMTest
    // TODO Verify if manual additions are needed
}

TriggerOnChangeGAMTest::~TriggerOnChangeGAMTest() {
    // Auto-generated destructor stub for TriggerOnChangeGAMTest
    // TODO Verify if manual additions are needed
}

bool TriggerOnChangeGAMTest::TestConstructor() {
    TriggerOnChangeGAMTestGAM gam;
    bool ret = gam.GetPreviousValue() == NULL;
    ret &= gam.GetState() == NULL;
    ret &= gam.GetPacketConfig() == NULL;
    ret &= gam.GetNumberOfFields() == 0;
    ret &= !gam.GetEvents().IsValid();
    ret &= gam.GetNumberOfEvents() == 0;
    ret &= gam.GetCommandIndex() == NULL;
    ret &= gam.GetNumberOfCommands() == 0;
    ret &= gam.GetCntReplied() == NULL;
    ret &= gam.GetCntTrigger() == NULL;
    ret &= gam.GetCurrentValue() == NULL;
    ret &= gam.GetAckValue() == NULL;
    return ret;
}

bool TriggerOnChangeGAMTest::TestSetup() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                    }"
            "                    +StartStateMachine = {"
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
            "                    +IdleStateMachine = {"
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
            "                    +AcquireStateMachine = {"
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
            "             }"
            "             OutputSignals = {"
            "                 CommandAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     Trigger = 1"
            "                 }"
            "                 State = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     Trigger = 1"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    if (ret) {
        uint32* prevValue = (uint32*) gam->GetPreviousValue();
        ret &= prevValue[0] == 0;
        uint32* state = gam->GetState();
        ret &= state[0] == 0;
        PacketField *fields = gam->GetPacketConfig();
        ret &= fields[0].isCommand == true;
        ret &= fields[0].name == "Command";
        ret &= fields[0].offset == 0;
        ret &= fields[0].type == UnsignedInteger32Bit;
        ret &= gam->GetNumberOfFields() == 1;
        ReferenceT < ReferenceContainer > events = gam->GetEvents();
        ret &= events.IsValid();
        if (ret) {
            ret &= events->Size() == 3;
        }
        ret &= gam->GetNumberOfEvents() == 3;
        uint32 *commandIndex = gam->GetCommandIndex();
        ret &= commandIndex[0] == 0;
        ret &= gam->GetNumberOfCommands() == 1;
        uint32 *cntReplied = gam->GetCntReplied();
        ret &= cntReplied[0] == 0;
        uint32 *cntTrigger = gam->GetCntTrigger();
        ret &= cntTrigger[0] == 0;
        uint32 *currentValue = (uint32 *) gam->GetCurrentValue();
        ret &= currentValue[0] == 0;
        uint32 *ackValue = (uint32 *) gam->GetAckValue();
        ret &= ackValue[0] == 0;
    }
    god->Purge();
    return ret;
}

bool TriggerOnChangeGAMTest::TestSetup_MoreCommands() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 2"
            "                    }"
            "                    +IdleStateMachine = {"
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
            "                    +AcquireStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 Command2 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandsAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
            "                 }"
            "                 State = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    if (ret) {
        uint32* prevValue = (uint32*) gam->GetPreviousValue();
        ret &= prevValue[0] == 0;
        uint32* state = gam->GetState();
        ret &= state[0] == 0;
        PacketField *fields = gam->GetPacketConfig();
        ret &= fields[0].isCommand == true;
        ret &= fields[1].isCommand == true;
        ret &= fields[0].name == "Command1";
        ret &= fields[0].offset == 0;
        ret &= fields[0].type == UnsignedInteger32Bit;
        ret &= fields[1].name == "Command2";
        ret &= fields[1].offset == 4;
        ret &= fields[1].type == UnsignedInteger32Bit;
        ret &= gam->GetNumberOfFields() == 2;
        ReferenceT < ReferenceContainer > events = gam->GetEvents();
        ret &= events.IsValid();
        if (ret) {
            ret &= events->Size() == 3;
        }
        ret &= gam->GetNumberOfEvents() == 3;
        uint32 *commandIndex = gam->GetCommandIndex();
        ret &= commandIndex[0] == 0;
        ret &= commandIndex[1] == 1;
        ret &= gam->GetNumberOfCommands() == 2;
        uint32 *cntReplied = gam->GetCntReplied();
        ret &= cntReplied[0] == 0;
        ret &= cntReplied[1] == 0;
        uint32 *cntTrigger = gam->GetCntTrigger();
        ret &= cntTrigger[0] == 0;
        ret &= cntTrigger[1] == 0;
        uint32 *currentValue = (uint32 *) gam->GetCurrentValue();
        ret &= currentValue[0] == 0;
        ret &= currentValue[1] == 0;
        uint32 *ackValue = (uint32 *) gam->GetAckValue();
        ret &= ackValue[0] == 0;
        ret &= ackValue[1] == 0;
    }
    god->Purge();
    return ret;
}

bool TriggerOnChangeGAMTest::TestSetup_NotAllCommands() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 2"
            "                    }"
            "                    +IdleStateMachine = {"
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
            "                    +AcquireStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 StatePLC = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 Command2 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 StatePLC_DDB = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 CommandsAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
            "                 }"
            "                 State = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    if (ret) {
        uint32* prevValue = (uint32*) gam->GetPreviousValue();
        ret &= prevValue[0] == 0;
        uint32* state = gam->GetState();
        ret &= state[0] == 0;
        PacketField *fields = gam->GetPacketConfig();
        ret &= fields[0].isCommand == false;
        ret &= fields[1].isCommand == true;
        ret &= fields[2].isCommand == true;
        ret &= fields[0].name == "StatePLC";
        ret &= fields[0].offset == 0;
        ret &= fields[0].type == UnsignedInteger32Bit;
        ret &= fields[1].name == "Command1";
        ret &= fields[1].offset == 4;
        ret &= fields[1].type == UnsignedInteger32Bit;
        ret &= fields[2].name == "Command2";
        ret &= fields[2].offset == 8;
        ret &= fields[2].type == UnsignedInteger32Bit;
        ret &= gam->GetNumberOfFields() == 3;
        ReferenceT < ReferenceContainer > events = gam->GetEvents();
        ret &= events.IsValid();
        if (ret) {
            ret &= events->Size() == 3;
        }
        ret &= gam->GetNumberOfEvents() == 3;
        uint32 *commandIndex = gam->GetCommandIndex();
        ret &= commandIndex[0] == 1;
        ret &= commandIndex[1] == 2;
        ret &= gam->GetNumberOfCommands() == 2;
        uint32 *cntReplied = gam->GetCntReplied();
        ret &= cntReplied[0] == 0;
        ret &= cntReplied[1] == 0;
        uint32 *cntTrigger = gam->GetCntTrigger();
        ret &= cntTrigger[0] == 0;
        ret &= cntTrigger[1] == 0;
        uint32 *currentValue = (uint32 *) gam->GetCurrentValue();
        ret &= currentValue[0] == 0;
        ret &= currentValue[1] == 0;
        uint32 *ackValue = (uint32 *) gam->GetAckValue();
        ret &= ackValue[0] == 0;
        ret &= ackValue[1] == 0;
    }

    god->Purge();
    return ret;
}

bool TriggerOnChangeGAMTest::TestSetup_InputCommandArray() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command_0 = 1"
            "                    }"
            "                    +StartStateMachine = {"
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
            "                    +IdleStateMachine = {"
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
            "                    +AcquireStateMachine = {"
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
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandsAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
            "                 }"
            "                 State = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    if (ret) {
        uint32* prevValue = (uint32*) gam->GetPreviousValue();
        ret &= prevValue[0] == 0;
        uint32* state = gam->GetState();
        ret &= state[0] == 0;
        PacketField *fields = gam->GetPacketConfig();
        ret &= fields[0].isCommand == true;
        ret &= fields[1].isCommand == true;
        ret &= fields[0].name == "Command_0";
        ret &= fields[0].offset == 0;
        ret &= fields[0].type == UnsignedInteger32Bit;
        ret &= fields[1].name == "Command_1";
        ret &= fields[1].offset == 4;
        ret &= fields[1].type == UnsignedInteger32Bit;
        ret &= gam->GetNumberOfFields() == 2;
        ReferenceT < ReferenceContainer > events = gam->GetEvents();
        ret &= events.IsValid();
        if (ret) {
            ret &= events->Size() == 3;
        }
        ret &= gam->GetNumberOfEvents() == 3;
        uint32 *commandIndex = gam->GetCommandIndex();
        ret &= commandIndex[0] == 0;
        ret &= commandIndex[1] == 1;
        ret &= gam->GetNumberOfCommands() == 2;
        uint32 *cntReplied = gam->GetCntReplied();
        ret &= cntReplied[0] == 0;
        ret &= cntReplied[1] == 0;
        uint32 *cntTrigger = gam->GetCntTrigger();
        ret &= cntTrigger[0] == 0;
        ret &= cntTrigger[1] == 0;
        uint32 *currentValue = (uint32 *) gam->GetCurrentValue();
        ret &= currentValue[0] == 0;
        ret &= currentValue[1] == 0;
        uint32 *ackValue = (uint32 *) gam->GetAckValue();
        ret &= ackValue[0] == 0;
        ret &= ackValue[1] == 0;
    }
    god->Purge();
    return ret;
}

bool TriggerOnChangeGAMTest::TestSetup_False_NoCommands() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "             InputSignals = {"
            "                 StatePLC = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "                 State1 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 State2 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 StatePLC_DDB = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 OthStates = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
            "                 }"
            "                 State = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    god->Purge();
    return ret;
}

bool TriggerOnChangeGAMTest::TestSetup_False_WrongStateNElements() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 2"
            "                    }"
            "                    +IdleStateMachine = {"
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
            "                    +AcquireStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 Command2 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandsAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
            "                 }"
            "                 State = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 3"
            "                     Trigger = 1"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    god->Purge();
    return ret;

}

bool TriggerOnChangeGAMTest::TestSetup_False_WrongStateType() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 2"
            "                    }"
            "                    +IdleStateMachine = {"
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
            "                    +AcquireStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 Command2 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandsAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
            "                 }"
            "                 State = {"
            "                     DataSource = DDB1"
            "                     Type = uint16"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    god->Purge();
    return ret;

}

bool TriggerOnChangeGAMTest::TestSetup_False_WrongIOSizeMatch() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 2"
            "                    }"
            "                    +IdleStateMachine = {"
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
            "                    +AcquireStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 Command2 = {"
            "                     DataSource = Input"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandsAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
            "                 }"
            "                 Command2Ack = {"
            "                     DataSource = DDB1"
            "                     Type = uint8"
            "                     Trigger = 1"
            "                 }"
            "                 State = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Trigger = 1"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    god->Purge();
    return ret;

}

bool TriggerOnChangeGAMTest::TestExecute() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                    }"
            "                    +StartStateMachine = {"
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
            "                    +IdleStateMachine = {"
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
            "                    +AcquireStateMachine = {"
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
            "             }"
            "             OutputSignals = {"
            "                 CommandAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    ReferenceT < TriggerOnChangeGAMTestDS > ds;
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }
    //get the brokers
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        gam->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }
    if (ret) {
        gam->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }

    if (ret) {
        ret = ds->PrepareNextState("Idle", "Idle");
    }

    if (ret) {
        uint32 *mem = (uint32*) gam->GetOutputSignalsMemory();
        uint32 *state = gam->GetState();
        //execution
        uint32 cnt = 0u;
        while (ret) {
            //send msg
            if (*state == 0) {
                if (cnt > 1) {
                    uint8 flag = ds->GetFlag();
                    ret = (flag == (1 << (cnt - 2)));
                    ds->ResetFlag();
                    ret &= (*mem == 0);
                }
                if (cnt >= 4) {
                    break;
                }
                ds->ChangeCommand(0, cnt);
                cnt++;
            }
            if (*state == 1) {
                ret &= (*mem == 0);
            }
            //send clear
            if (*state == 2) {
                ret &= (*mem == (cnt - 1));
                ds->ChangeCommand(0, 0);
            }

            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
        }
    }
    god->Purge();
    return ret;
}

bool TriggerOnChangeGAMTest::TestExecute_MoreCommands() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command2 = 2"
            "                    }"
            "                    +IdleStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command2 = 3"
            "                    }"
            "                    +AcquireStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 Command2 = {"
            "                     DataSource = Input"
            "                     Trigger = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandAcks = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfElements = 2"
            "                     NumberOfDimensions = 1"
            "                 }"
            "                 State = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfElements = 2"
            "                     NumberOfDimensions = 1"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    ReferenceT < TriggerOnChangeGAMTestDS > ds;
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }
    //get the brokers
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        gam->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }
    if (ret) {
        gam->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }

    if (ret) {
        ret = ds->PrepareNextState("Idle", "Idle");
    }

    if (ret) {
        uint32 *state = gam->GetState();
        uint32 *mem = (uint32*) gam->GetOutputSignalsMemory();

        //execution
        uint32 cnt = 0u;
        while (ret) {
            //send msg
            if ((state[0] == 0) && (state[1] == 0)) {
                if (cnt > 1) {
                    uint8 flag = ds->GetFlag();
                    ret = (flag == (1 << (cnt - 2)));
                    ds->ResetFlag();

                }
                if (cnt >= 4) {
                    break;
                }
                ds->ChangeCommand(0, cnt);
                ds->ChangeCommand(1, cnt);
                cnt++;
            }
            //if both clear have been received
            if ((state[0] == 2) && (state[1] == 2)) {
                ds->ChangeCommand(0, 0);
                ds->ChangeCommand(1, 0);
            }

            if (state[0] == 0) {
                ret &= mem[0] == 0;
            }
            if (state[1] == 0) {
                ret &= mem[1] == 0;
            }

            if (state[0] == 1) {
                ret &= mem[0] == 0;
            }
            if (state[1] == 1) {
                ret &= mem[1] == 0;
            }

            if (state[0] == 2) {
                ret &= (mem[0] == (cnt - 1));
            }
            if (state[1] == 2) {
                ret &= (mem[1] == (cnt - 1));
            }

            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
        }
    }
    god->Purge();
    return ret;
}

bool TriggerOnChangeGAMTest::TestExecute_NotAllCommands() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                        StatePlc = 1"
            "                    }"
            "                    +StartStateMachine = {"
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
            "                        StatePlc = 2"
            "                    }"
            "                    +IdleStateMachine = {"
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
            "                        StatePlc = 3"
            "                    }"
            "                    +AcquireStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 StatePlc = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Trigger = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandAck = {"
            "                     DataSource = DDB1"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    ReferenceT < TriggerOnChangeGAMTestDS > ds;
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }
    //get the brokers
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        gam->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }
    if (ret) {
        gam->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }

    if (ret) {
        ret = ds->PrepareNextState("Idle", "Idle");
    }

    if (ret) {
        uint32 *state = gam->GetState();
        uint32 *mem = (uint32*) gam->GetOutputSignalsMemory();

        //mismatch... no signal to be sent
        if (state[0] == 0) {
            ds->ChangeCommand(0, 0);
            ds->ChangeCommand(1, 1);
        }

        while (ret) {
            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
            //if clear has been received
            if (state[0] == 2) {
                ds->ChangeCommand(1, 0);

                brokerIn->Execute();
                gam->Execute();
                brokerOut->Execute();
                Sleep::MSec(100);

                uint8 flag = ds->GetFlag();
                ret = (flag == 0);
                break;
            }
        }

        //execution
        uint32 cnt = 0u;
        while (ret) {
            //send msg
            if (state[0] == 0) {
                if (cnt > 1) {
                    uint8 flag = ds->GetFlag();
                    ret = (flag == (1 << (cnt - 2)));
                    ds->ResetFlag();
                }
                if (cnt >= 4) {
                    break;
                }
                ds->ChangeCommand(0, cnt);
                ds->ChangeCommand(1, cnt);
                cnt++;
            }

            //if clear has been received
            if (state[0] == 2) {
                ds->ChangeCommand(1, 0);
            }

            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);

            if (state[0] == 0) {
                ret &= mem[1] == 0;
                ret &= (mem[0] == (cnt - 1));
            }
            if (state[0] == 1) {
                ret &= mem[1] == 0;
            }
            if (state[0] == 2) {
                ret &= (mem[1] == (cnt - 1));
            }

        }
    }
    god->Purge();
    return ret;

}

bool TriggerOnChangeGAMTest::TestExecute_InputCommandArray(){

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command_0 = 1"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command_1 = 2"
            "                    }"
            "                    +IdleStateMachine = {"
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
            "                    +AcquireStateMachine = {"
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
            "                     NumberOfElements = 2"
            "                     NumberOfDimensions = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandAcks = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfElements = 2"
            "                     NumberOfDimensions = 1"
            "                 }"
            "                 State = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfElements = 2"
            "                     NumberOfDimensions = 1"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    ReferenceT < TriggerOnChangeGAMTestDS > ds;
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }
    //get the brokers
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        gam->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }
    if (ret) {
        gam->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }

    if (ret) {
        ret = ds->PrepareNextState("Idle", "Idle");
    }

    if (ret) {
        uint32 *state = gam->GetState();
        uint32 *mem = (uint32*) gam->GetOutputSignalsMemory();

        //execution
        uint32 cnt = 0u;
        while (ret) {
            //send msg
            if ((state[0] == 0) && (state[1] == 0)) {
                if (cnt > 1) {
                    uint8 flag = ds->GetFlag();
                    ret = (flag == (1 << (cnt - 2)));
                    ds->ResetFlag();

                }
                if (cnt >= 4) {
                    break;
                }
                ds->ChangeCommand(0, cnt);
                ds->ChangeCommand(1, cnt);
                cnt++;
            }
            //if both clear have been received
            if ((state[0] == 2) && (state[1] == 2)) {
                ds->ChangeCommand(0, 0);
                ds->ChangeCommand(1, 0);
            }

            if (state[0] == 0) {
                ret &= mem[0] == 0;
            }
            if (state[1] == 0) {
                ret &= mem[1] == 0;
            }

            if (state[0] == 1) {
                ret &= mem[0] == 0;
            }
            if (state[1] == 1) {
                ret &= mem[1] == 0;
            }

            if (state[0] == 2) {
                ret &= (mem[0] == (cnt - 1));
            }
            if (state[1] == 2) {
                ret &= (mem[1] == (cnt - 1));
            }

            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
        }
    }
    god->Purge();
    return ret;
}

bool TriggerOnChangeGAMTest::TestExecute_NotAllCommandsArray(){

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command_0 = 1"
            "                        PLC_State_0 = 1"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command_1 = 2"
            "                        PLC_State_1 = 2"
            "                    }"
            "                    +IdleStateMachine = {"
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
            "                        PLC_State_1 = 3"
            "                    }"
            "                    +AcquireStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 PLC_State = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     NumberOfElements = 2"
            "                     NumberOfDimensions = 1"
            "                     Type = uint32"
            "                 }"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Trigger = 1"
            "                     NumberOfElements = 2"
            "                     NumberOfDimensions = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 PLC_State_DDB = {"
            "                     DataSource = DDB1"
            "                     NumberOfElements = 2"
            "                     NumberOfDimensions = 1"
            "                     Type = uint32"
            "                 }"
            "                 CommandAcks = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfElements = 2"
            "                     NumberOfDimensions = 1"
            "                 }"
            "                 State = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfElements = 2"
            "                     NumberOfDimensions = 1"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    ReferenceT < TriggerOnChangeGAMTestDS > ds;
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }
    //get the brokers
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        gam->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }
    if (ret) {
        gam->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }

    if (ret) {
        ret = ds->PrepareNextState("Idle", "Idle");
    }

    if (ret) {
        uint32 *state = gam->GetState();
        uint32 *mem = (uint32*) gam->GetOutputSignalsMemory();

        //execution
        uint32 cnt = 0u;
        while (ret) {
            //send msg
            if ((state[0] == 0) && (state[1] == 0)) {
                if (cnt > 1) {
                    uint8 flag = ds->GetFlag();
                    ret = (flag == (1 << (cnt - 2)));
                    ds->ResetFlag();

                }
                if (cnt >= 4) {
                    break;
                }
                ds->ChangeCommand(0, cnt);
                ds->ChangeCommand(1, cnt);
                ds->ChangeCommand(2, cnt);
                ds->ChangeCommand(3, cnt);
                cnt++;
            }
            //if both clear have been received
            if ((state[0] == 2) && (state[1] == 2)) {
                ds->ChangeCommand(2, 0);
                ds->ChangeCommand(3, 0);
            }

            if (state[0] == 0) {
                ret &= mem[2] == 0;
            }
            if (state[1] == 0) {
                ret &= mem[3] == 0;
            }

            if (state[0] == 1) {
                ret &= mem[2] == 0;
            }
            if (state[1] == 1) {
                ret &= mem[3] == 0;
            }

            if (state[0] == 2) {
                ret &= (mem[2] == (cnt - 1));
            }
            if (state[1] == 2) {
                ret &= (mem[3] == (cnt - 1));
            }

            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
        }
    }
    god->Purge();
    return ret;
}


bool TriggerOnChangeGAMTest::TestExecute_WaitIndirectReply() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 2"
            "                    }"
            "                    +IdleStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 3"
            "                    }"
            "                    +AcquireStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    ReferenceT < TriggerOnChangeGAMTestDS > ds;
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }
    //get the brokers
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        gam->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }
    if (ret) {
        gam->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }

    if (ret) {
        ret = ds->PrepareNextState("Idle", "Idle");
    }

    if (ret) {
        uint32 *mem = (uint32*) gam->GetOutputSignalsMemory();
        uint32 *state = gam->GetState();
        //execution
        uint32 cnt = 0u;
        while (ret) {
            //send msg
            if (*state == 0) {
                if (cnt > 1) {
                    uint8 flag = ds->GetFlag();
                    ret = (flag == (1 << (cnt - 2)));
                    ds->ResetFlag();
                    ret &= (*mem == 0);
                }
                if (cnt >= 4) {
                    break;
                }
                ds->ChangeCommand(0, cnt);
                cnt++;
            }
            if (*state == 1) {
                ret &= (*mem == 0);
            }
            //send clear
            if (*state == 2) {
                ret &= (*mem == (cnt - 1));
                ds->ChangeCommand(0, 0);
            }

            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
        }
    }
    god->Purge();
    return ret;
}

bool TriggerOnChangeGAMTest::TestExecute_MoreCommandsWaitIndirectReply() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command2 = 2"
            "                    }"
            "                    +IdleStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command2 = 3"
            "                    }"
            "                    +AcquireStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 Command1 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 Command2 = {"
            "                     DataSource = Input"
            "                     Trigger = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandAcks = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfElements = 2"
            "                     NumberOfDimensions = 1"
            "                 }"
            "                 State = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfElements = 2"
            "                     NumberOfDimensions = 1"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    ReferenceT < TriggerOnChangeGAMTestDS > ds;
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }
    //get the brokers
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        gam->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }
    if (ret) {
        gam->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }

    if (ret) {
        ret = ds->PrepareNextState("Idle", "Idle");
    }

    if (ret) {
        uint32 *state = gam->GetState();
        uint32 *mem = (uint32*) gam->GetOutputSignalsMemory();

        //execution
        uint32 cnt = 0u;
        while (ret) {
            //send msg
            if ((state[0] == 0) && (state[1] == 0)) {
                if (cnt > 1) {
                    uint8 flag = ds->GetFlag();
                    ret = (flag == (1 << (cnt - 2)));
                    ds->ResetFlag();

                }
                if (cnt >= 4) {
                    break;
                }
                ds->ChangeCommand(0, cnt);
                ds->ChangeCommand(1, cnt);
                cnt++;
            }
            //if both clear have been received
            if ((state[0] == 2) && (state[1] == 2)) {
                ds->ChangeCommand(0, 0);
                ds->ChangeCommand(1, 0);
            }

            if (state[0] == 0) {
                ret &= mem[0] == 0;
            }
            if (state[1] == 0) {
                ret &= mem[1] == 0;
            }

            if (state[0] == 1) {
                ret &= mem[0] == 0;
            }
            if (state[1] == 1) {
                ret &= mem[1] == 0;
            }

            if (state[0] == 2) {
                ret &= (mem[0] == (cnt - 1));
            }
            if (state[1] == 2) {
                ret &= (mem[1] == (cnt - 1));
            }

            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
        }
    }
    god->Purge();
    return ret;
}

bool TriggerOnChangeGAMTest::TestExecute_NotAllCommandsWaitIndirectReply() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                        StatePlc = 1"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 2"
            "                        StatePlc = 2"
            "                    }"
            "                    +IdleStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 3"
            "                        StatePlc = 3"
            "                    }"
            "                    +AcquireStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "                 StatePlc = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Trigger = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandAck = {"
            "                     DataSource = DDB1"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    ReferenceT < TriggerOnChangeGAMTestDS > ds;
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }
    //get the brokers
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        gam->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }
    if (ret) {
        gam->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }

    if (ret) {
        ret = ds->PrepareNextState("Idle", "Idle");
    }

    if (ret) {
        uint32 *state = gam->GetState();
        uint32 *mem = (uint32*) gam->GetOutputSignalsMemory();

        //mismatch... no signal to be sent
        if (state[0] == 0) {
            ds->ChangeCommand(0, 0);
            ds->ChangeCommand(1, 1);
        }

        while (ret) {
            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
            //if clear has been received
            if (state[0] == 2) {
                ds->ChangeCommand(1, 0);

                brokerIn->Execute();
                gam->Execute();
                brokerOut->Execute();
                Sleep::MSec(100);

                uint8 flag = ds->GetFlag();
                ret = (flag == 0);
                break;
            }
        }

        //execution
        uint32 cnt = 0u;
        while (ret) {
            //send msg
            if (state[0] == 0) {
                if (cnt > 1) {
                    uint8 flag = ds->GetFlag();
                    ret = (flag == (1 << (cnt - 2)));
                    ds->ResetFlag();
                }
                if (cnt >= 4) {
                    break;
                }
                ds->ChangeCommand(0, cnt);
                ds->ChangeCommand(1, cnt);
                cnt++;
            }

            //if clear has been received
            if (state[0] == 2) {
                ds->ChangeCommand(1, 0);
            }

            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);

            if (state[0] == 0) {
                ret &= mem[1] == 0;
                ret &= (mem[0] == (cnt - 1));
            }
            if (state[0] == 1) {
                ret &= mem[1] == 0;
            }
            if (state[0] == 2) {
                ret &= (mem[1] == (cnt - 1));
            }

        }
    }
    god->Purge();
    return ret;

}

bool TriggerOnChangeGAMTest::TestExecute_ErrorState() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = TriggerOnChangeGAMTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                    }"
            "                    +StartStateMachine = {"
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
            "                    +IdleStateMachine = {"
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
            "                    +AcquireStateMachine = {"
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
            "             }"
            "             OutputSignals = {"
            "                 CommandAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 State = {"
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
            "            Class = TriggerOnChangeGAMTestDS"
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
            "                    CPUs = 2"
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

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<TriggerOnChangeGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    ReferenceT < TriggerOnChangeGAMTestDS > ds;
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }
    //get the brokers
    ReferenceContainer inputBrokers;
    ReferenceContainer outputBrokers;

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerIn;
    ReferenceT < MemoryMapOutputBroker > brokerOut;

    if (ret) {
        gam->GetInputBrokers(inputBrokers);
        brokerIn = inputBrokers.Get(0);
        ret = brokerIn.IsValid();
    }
    if (ret) {
        gam->GetOutputBrokers(outputBrokers);
        brokerOut = outputBrokers.Get(0);
        ret = brokerOut.IsValid();
    }

    if (ret) {
        ret = ds->PrepareNextState("Idle", "Idle");
    }

    if (ret) {
        uint32 *state = gam->GetState();
        const uint32 nMaxCycles = 20;
        ret = false;
        printf("\nFirst case");
        //first case: change command during DONE state
        for (uint32 i = 0u; i < nMaxCycles; i++) {
            if (*state == 0) {
                ds->ChangeCommand(0, 1);
            }
            if (*state == 2) {
                ds->ChangeCommand(0, 2);
            }
            if (*state == 3) {
                ret = true;
                break;
            }
            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
        }
        printf("\nReset first case");
        //recover the READY state
        ret = false;
        for (uint32 i = 0u; i < nMaxCycles; i++) {
            if (*state == 3) {
                ds->ChangeCommand(0, 0);
            }
            if (*state == 0) {
                ret = true;
                break;
            }
            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
        }
        printf("\nSecond case");
        //second case: change command during SENDING state
        if (ret) {
            ds->SetSleepTime(1000);
            const uint32 nMaxCycles = 20;
            ret = false;
            for (uint32 i = 0u; i < nMaxCycles; i++) {
                if (*state == 0) {
                    ds->ChangeCommand(0, 1);
                }
                if (*state == 1) {
                    ds->ChangeCommand(0, 2);
                }
                if (*state == 3) {
                    ret = true;
                    break;
                }
                brokerIn->Execute();
                gam->Execute();
                brokerOut->Execute();
                Sleep::MSec(100);
            }
        }
        printf("\nReset second case");
        //recover the READY state
        ret = false;
        for (uint32 i = 0u; i < nMaxCycles; i++) {
            if (*state == 3) {
                ds->ChangeCommand(0, 0);
            }
            if (*state == 0) {
                ret = true;
                break;
            }
            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
        }
        printf("\nThird case");
        //third case: reset command during SENDING state
        if (ret) {
            ds->SetSleepTime(1000);
            const uint32 nMaxCycles = 20;
            ret = false;
            for (uint32 i = 0u; i < nMaxCycles; i++) {
                if (*state == 0) {
                    ds->ChangeCommand(0, 1);
                }
                if (*state == 1) {
                    ds->ChangeCommand(0, 0);
                }
                if (*state == 3) {
                    ret = true;
                    break;
                }
                brokerIn->Execute();
                gam->Execute();
                brokerOut->Execute();
                Sleep::MSec(100);
            }
        }
        printf("\nReset third case");
        //recover the READY state
        ret = false;
        for (uint32 i = 0u; i < nMaxCycles; i++) {
            if (*state == 3) {
                //command + reset to reset
                ds->ChangeCommand(0, 1);
            }
            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
            if (*state == 3) {
                //command + reset to reset
                ds->ChangeCommand(0, 0);
            }
            brokerIn->Execute();
            gam->Execute();
            brokerOut->Execute();
            Sleep::MSec(100);
            if (*state == 0) {
                ret = true;
                break;
            }
        }

    }
    god->Purge();
    return ret;
}

