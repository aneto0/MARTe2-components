/**
 * @file EventConditionTriggerTest.cpp
 * @brief Source file for class EventConditionTriggerTest
 * @date 19/07/2018
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
 * the class EventConditionTriggerTest (public, protected, and private). Be aware that some 
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
#include "EventConditionTriggerTest.h"
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
#include "TriggerOnChangeGAM.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class EventConditionTriggerTestGAM: public TriggerOnChangeGAM {
public:
    CLASS_REGISTER_DECLARATION()

EventConditionTriggerTestGAM    ();
    ~EventConditionTriggerTestGAM();

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

EventConditionTriggerTestGAM::EventConditionTriggerTestGAM() {

}

EventConditionTriggerTestGAM::~EventConditionTriggerTestGAM() {

}

void *EventConditionTriggerTestGAM::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void *EventConditionTriggerTestGAM::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

uint8 *EventConditionTriggerTestGAM::GetPreviousValue() {
    return previousValue;

}

uint32 *EventConditionTriggerTestGAM::GetState() {
    return state;
}

PacketField *EventConditionTriggerTestGAM::GetPacketConfig() {
    return packetConfig;
}

uint32 EventConditionTriggerTestGAM::GetNumberOfFields() {
    return numberOfFields;
}

ReferenceT<ReferenceContainer> EventConditionTriggerTestGAM::GetEvents() {
    return events;
}

uint32 EventConditionTriggerTestGAM::GetNumberOfEvents() {
    return numberOfEvents;
}

uint32 *EventConditionTriggerTestGAM::GetCommandIndex() {
    return commandIndex;
}

uint32 EventConditionTriggerTestGAM::GetNumberOfCommands() {
    return numberOfCommands;
}

uint32 *EventConditionTriggerTestGAM::GetCntReplied() {
    return cntReplied;
}

uint32 *EventConditionTriggerTestGAM::GetCntTrigger() {
    return cntTrigger;
}
uint8 *EventConditionTriggerTestGAM::GetCurrentValue() {
    return currentValue;
}
uint8 *EventConditionTriggerTestGAM::GetAckValue() {
    return ackValue;
}

CLASS_REGISTER(EventConditionTriggerTestGAM, "1.0")

class EventConditionTriggerTestComp: public EventConditionTrigger {
public:

    EventConditionTriggerTestComp();

    virtual ~EventConditionTriggerTestComp();

    EventConditionField *GetEventConditions();

    uint32 GetNumberOfConditions();

    ReferenceContainer *GetMessageQueue();

};

EventConditionTriggerTestComp::EventConditionTriggerTestComp() :
        EventConditionTrigger() {

}

EventConditionTriggerTestComp::~EventConditionTriggerTestComp() {
    ReferenceContainer rc;
    Purge(rc);
}

EventConditionTriggerTestComp::EventConditionField *EventConditionTriggerTestComp::GetEventConditions() {
    return eventConditions;
}

uint32 EventConditionTriggerTestComp::GetNumberOfConditions() {
    return numberOfConditions;
}

ReferenceContainer *EventConditionTriggerTestComp::GetMessageQueue() {
    return &messageQueue;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

class EventConditionTriggerTestDS: public MemoryDataSourceI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

EventConditionTriggerTestDS    ();

    virtual ~EventConditionTriggerTestDS();

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

EventConditionTriggerTestDS::EventConditionTriggerTestDS() :
        MessageI() {
    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
    callFunFlag = 0;
    sleepTimeMSec = 0u;
}

EventConditionTriggerTestDS::~EventConditionTriggerTestDS() {

}

void EventConditionTriggerTestDS::ResetFlag() {
    callFunFlag = 0;
}

uint8 EventConditionTriggerTestDS::GetFlag() {
    return callFunFlag;
}

void EventConditionTriggerTestDS::SetSleepTime(uint32 sleepTime) {
    sleepTimeMSec = sleepTime;
}

ErrorManagement::ErrorType EventConditionTriggerTestDS::TrigFun1() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function1");
    callFunFlag |= 1;
    if (sleepTimeMSec > 0) {
        Sleep::MSec(sleepTimeMSec);
    }
    return ret;
}

ErrorManagement::ErrorType EventConditionTriggerTestDS::TrigFun2() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function2");
    callFunFlag |= (1 << 1);
    if (sleepTimeMSec > 0) {
        Sleep::MSec(sleepTimeMSec);
    }
    return ret;
}

ErrorManagement::ErrorType EventConditionTriggerTestDS::TrigFun3() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function3");
    callFunFlag |= (1 << 2);
    if (sleepTimeMSec > 0) {
        Sleep::MSec(sleepTimeMSec);
    }
    return ret;
}

bool EventConditionTriggerTestDS::ChangeCommand(uint32 index, uint32 value) {
    uint32 *signalAddress = NULL;
    bool ret = GetSignalMemoryBuffer(index, 0, (void*&) signalAddress);
    if (ret) {
        *signalAddress = value;
    }
    return ret;
}

const char8 *EventConditionTriggerTestDS::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {

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

bool EventConditionTriggerTestDS::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    return true;
}

bool EventConditionTriggerTestDS::Synchronise() {
    return true;
}

CLASS_REGISTER(EventConditionTriggerTestDS, "1.0")

CLASS_METHOD_REGISTER(EventConditionTriggerTestDS, TrigFun1)
CLASS_METHOD_REGISTER(EventConditionTriggerTestDS, TrigFun2)
CLASS_METHOD_REGISTER(EventConditionTriggerTestDS, TrigFun3)

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

EventConditionTriggerTest::EventConditionTriggerTest() {
}

EventConditionTriggerTest::~EventConditionTriggerTest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool EventConditionTriggerTest::TestConstructor() {
    EventConditionTriggerTestComp comp;
    bool ret = comp.GetEventConditions() == NULL;
    ret &= comp.GetNumberOfConditions() == 0u;
    //ret &= comp.Replied() == 0u;
    return ret;
}

bool EventConditionTriggerTest::TestInitialise() {

    const char8 *config = ""
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                        Command2 = 2"
            "                        Command3 = 3"
            "                        Command4 = 4"
            "                        State = 4"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }";

    EventConditionTriggerTestComp comp;
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = comp.Initialise(cdb);

        if (ret) {
            ret = (comp.GetCPUMask() == 0xff);
        }
        if (ret) {
            ret = comp.GetNumberOfConditions() == 5;
        }
    }
    return ret;
}

bool EventConditionTriggerTest::TestInitialise_CPUMask() {

    const char8 *config = ""
            "                    Class = EventConditionTrigger"
            "                    CPUMask = 0x1"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                        Command2 = 2"
            "                        Command3 = 3"
            "                        Command4 = 4"
            "                        State = 4"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }";

    EventConditionTriggerTestComp comp;
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = comp.Initialise(cdb);

        if (ret) {
            ret = (comp.GetCPUMask() == 0x1);
        }
        if (ret) {
            ret = comp.GetNumberOfConditions() == 5;
        }
    }
    return ret;
}

bool EventConditionTriggerTest::TestInitialise_FalseNoEventTrigger() {

    const char8 *config = ""
            "                    Class = EventConditionTrigger"
            "                    EventTriggerX = {"
            "                        Command1 = 1"
            "                        Command2 = 2"
            "                        Command3 = 3"
            "                        Command4 = 4"
            "                        State = 4"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }";

    EventConditionTriggerTestComp comp;
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = !comp.Initialise(cdb);
    }
    return ret;

}

bool EventConditionTriggerTest::TestInitialise_FalseNotOnlyMessages() {

    const char8 *config = ""
            "                    Class = EventConditionTrigger"
            "                    CPUMask = 0x1"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                        Command2 = 2"
            "                        Command3 = 3"
            "                        Command4 = 4"
            "                        State = 4"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                    +StartStateMachine2 = {"
            "                        Class = Object"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }";

    EventConditionTriggerTestComp comp;
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = !comp.Initialise(cdb);
    }
    return ret;

}

bool EventConditionTriggerTest::TestSetPacketConfig() {

    const char8 *config = ""
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                        Command2 = 2"
            "                        Command3 = 3"
            "                        Command4 = 4"
            "                        State = -4"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }";

    PacketField packetField[5];
    packetField[0].isCommand = true;
    packetField[0].name = "Command1";
    packetField[0].offset = 0;
    packetField[0].type = UnsignedInteger64Bit;

    packetField[1].isCommand = true;
    packetField[1].name = "Command2";
    packetField[1].offset = 8;
    packetField[1].type = UnsignedInteger32Bit;

    packetField[2].isCommand = true;
    packetField[2].name = "Command3";
    packetField[2].offset = 12;
    packetField[2].type = UnsignedInteger16Bit;

    packetField[3].isCommand = true;
    packetField[3].name = "Command4";
    packetField[3].offset = 14;
    packetField[3].type = UnsignedInteger8Bit;

    packetField[4].isCommand = false;
    packetField[4].name = "State";
    packetField[4].offset = 15;
    packetField[4].type = SignedInteger32Bit;

    EventConditionTriggerTestComp comp;
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = comp.Initialise(cdb);

        if (ret) {
            ret = comp.GetNumberOfConditions() == 5;
        }
    }

    if (ret) {
        ret = comp.SetPacketConfig(packetField, 5);
    }

    if (ret) {
        EventConditionTriggerTestComp::EventConditionField *fields = comp.GetEventConditions();
        ret = (fields != NULL);
        if (ret) {
            ret = fields[0].packetField == &packetField[0];
        }
        if (ret) {
            ret = (fields[0].at.GetTypeDescriptor() == UnsignedInteger64Bit);
        }
        if (ret) {
            uint64 *data = (uint64*) (fields[0].at.GetDataPointer());
            ret = (*data == 1);
        }

        if (ret) {
            ret = fields[1].packetField == &packetField[1];
        }
        if (ret) {
            ret = (fields[1].at.GetTypeDescriptor() == UnsignedInteger32Bit);
        }
        if (ret) {
            uint32 *data = (uint32*) (fields[1].at.GetDataPointer());
            ret = (*data == 2);
        }

        if (ret) {
            ret = fields[2].packetField == &packetField[2];
        }
        if (ret) {
            ret = (fields[2].at.GetTypeDescriptor() == UnsignedInteger16Bit);
        }
        if (ret) {
            uint16 *data = (uint16*) (fields[2].at.GetDataPointer());
            ret = (*data == 3);
        }

        if (ret) {
            ret = fields[3].packetField == &packetField[3];
        }
        if (ret) {
            ret = (fields[3].at.GetTypeDescriptor() == UnsignedInteger8Bit);
        }
        if (ret) {
            uint8 *data = (uint8*) (fields[3].at.GetDataPointer());
            ret = (*data == 4);
        }

        if (ret) {
            ret = fields[4].packetField == &packetField[4];
        }
        if (ret) {
            ret = (fields[4].at.GetTypeDescriptor() == SignedInteger32Bit);
        }
        if (ret) {
            int32 *data = (int32*) (fields[4].at.GetDataPointer());
            ret = (*data == -4);
        }

    }

    return ret;

}

bool EventConditionTriggerTest::TestSetPacketConfig_FalseNoPacketFieldMatch() {
    const char8 *config = ""
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                        Command2 = 2"
            "                        Command5 = 3"
            "                        State = -4"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }";

    PacketField packetField[5];
    packetField[0].isCommand = true;
    packetField[0].name = "Command1";
    packetField[0].offset = 0;
    packetField[0].type = UnsignedInteger64Bit;

    packetField[1].isCommand = true;
    packetField[1].name = "Command2";
    packetField[1].offset = 8;
    packetField[1].type = UnsignedInteger32Bit;

    packetField[2].isCommand = true;
    packetField[2].name = "Command3";
    packetField[2].offset = 12;
    packetField[2].type = UnsignedInteger16Bit;

    packetField[3].isCommand = true;
    packetField[3].name = "Command4";
    packetField[3].offset = 14;
    packetField[3].type = UnsignedInteger8Bit;

    packetField[4].isCommand = false;
    packetField[4].name = "State";
    packetField[4].offset = 15;
    packetField[4].type = SignedInteger32Bit;

    EventConditionTriggerTestComp comp;
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = comp.Initialise(cdb);

        if (ret) {
            ret = comp.GetNumberOfConditions() == 4;
        }
    }

    if (ret) {
        ret = !comp.SetPacketConfig(packetField, 5);
    }

    return ret;

}

bool EventConditionTriggerTest::TestSetPacketConfig_FalseReadFailedTypeMismatch() {
    const char8 *config = ""
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                        Command2 = 2"
            "                        Command4 = ciao"
            "                        State = -4"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }";

    PacketField packetField[5];
    packetField[0].isCommand = true;
    packetField[0].name = "Command1";
    packetField[0].offset = 0;
    packetField[0].type = UnsignedInteger64Bit;

    packetField[1].isCommand = true;
    packetField[1].name = "Command2";
    packetField[1].offset = 8;
    packetField[1].type = UnsignedInteger32Bit;

    packetField[2].isCommand = true;
    packetField[2].name = "Command3";
    packetField[2].offset = 12;
    packetField[2].type = UnsignedInteger16Bit;

    packetField[3].isCommand = true;
    packetField[3].name = "Command4";
    packetField[3].offset = 14;
    packetField[3].type = UnsignedInteger8Bit;

    packetField[4].isCommand = false;
    packetField[4].name = "State";
    packetField[4].offset = 15;
    packetField[4].type = SignedInteger32Bit;

    EventConditionTriggerTestComp comp;
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = comp.Initialise(cdb);

        if (ret) {
            ret = comp.GetNumberOfConditions() == 4;
        }
    }

    if (ret) {
        ret = !comp.SetPacketConfig(packetField, 5);
    }

    return ret;

}

bool EventConditionTriggerTest::TestCheck() {
    const char8 *config = ""
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command1 = 1"
            "                        Command2 = 2"
            "                        Command4 = 4"
            "                        State = -4"
            "                    }"
            "                    +StartStateMachine = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }";

    PacketField packetField[5];
    packetField[0].isCommand = true;
    packetField[0].name = "Command1";
    packetField[0].offset = 0;
    packetField[0].type = UnsignedInteger64Bit;

    packetField[1].isCommand = true;
    packetField[1].name = "Command2";
    packetField[1].offset = 8;
    packetField[1].type = UnsignedInteger32Bit;

    packetField[2].isCommand = true;
    packetField[2].name = "Command3";
    packetField[2].offset = 12;
    packetField[2].type = UnsignedInteger16Bit;

    packetField[3].isCommand = true;
    packetField[3].name = "Command4";
    packetField[3].offset = 14;
    packetField[3].type = UnsignedInteger8Bit;

    packetField[4].isCommand = false;
    packetField[4].name = "State";
    packetField[4].offset = 15;
    packetField[4].type = SignedInteger32Bit;

    EventConditionTriggerTestComp comp;
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = comp.Initialise(cdb);

        if (ret) {
            ret = comp.GetNumberOfConditions() == 4;
        }
    }

    if (ret) {
        ret = comp.SetPacketConfig(packetField, 5);
    }

    if (ret) {
        uint8 mem[19];
        *(uint64*) mem = 1;
        *(uint32*) (mem + 8) = 2;
        *(uint16*) (mem + 12) = 3;
        *(uint8*) (mem + 14) = 4;
        *(int32*) (mem + 15) = -4;

        ret &= !comp.Check(mem, NULL);
        ret = comp.Check(mem, &packetField[0]);
        ret &= comp.Check(mem, &packetField[1]);
        ret &= !comp.Check(mem, &packetField[2]);
        ret &= comp.Check(mem, &packetField[3]);
        ret = !comp.Check(mem, &packetField[4]);
    }

    if (ret) {
        ReferenceContainer* queue = comp.GetMessageQueue();
        ret = queue->Size() == 3;

    }
    return ret;
}

bool EventConditionTriggerTest::TestExecute_ImmediateReply() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = EventConditionTriggerTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                        Status = 2"
            "                    }"
            "                    +TrigFun1 = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                    +TrigFun2 = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                    +TrigFun3 = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
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
            "                 Status = {"
            "                     DataSource = Input"
            "                     Trigger = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 StatusOut = {"
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
            "            Class = EventConditionTriggerTestDS"
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

    //initialise the whole environment
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    //get the event
    ReferenceT<EventConditionTrigger> event1;
    if (ret) {
        event1 = god->Find("Application.Functions.GAM1.Events.Event1");
        ret = event1.IsValid();
    }

    //get the ds memory

    ReferenceT<EventConditionTriggerTestDS> ds;
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }

    ReferenceT<EventConditionTriggerTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }

    uint32 *mem = NULL;
    if (ret) {
        ret = ds->GetSignalMemoryBuffer(0u, 0u, (void*&) mem);

    }

    if (ret) {
        mem[0] = 1;
        mem[1] = 2;
        PacketField *packetField = gam->GetPacketConfig();
        ret = event1->Check((uint8*) mem, &packetField[0]);
        if (ret) {
            uint8 flag = 0u;
            //give max 10 sec to execute the messages
            const uint32 nTries = 10u;
            ret = false;
            for (uint32 i = 0u; i < nTries; i++) {
                flag = ds->GetFlag();
                if (flag == 0x7) {
                    ret = (event1->Replied(&packetField[0]) == 3);
                    break;
                }
                Sleep::Sec(1);
            }

        }
    }

    god->Purge();
    return ret;
}

bool EventConditionTriggerTest::TestExecute_IndirectReply() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = EventConditionTriggerTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                        Status = 2"
            "                    }"
            "                    +TrigFun1 = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                    +TrigFun2 = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                    +TrigFun3 = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                }"
            "             }"
            "             InputSignals = {"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 Status = {"
            "                     DataSource = Input"
            "                     Trigger = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 StatusOut = {"
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
            "            Class = EventConditionTriggerTestDS"
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

    //initialise the whole environment
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    //get the event
    ReferenceT<EventConditionTrigger> event1;
    if (ret) {
        event1 = god->Find("Application.Functions.GAM1.Events.Event1");
        ret = event1.IsValid();
    }

    //get the ds memory

    ReferenceT<EventConditionTriggerTestDS> ds;
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }

    ReferenceT<EventConditionTriggerTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }

    uint32 *mem = NULL;
    if (ret) {
        ret = ds->GetSignalMemoryBuffer(0u, 0u, (void*&) mem);

    }

    if (ret) {
        mem[0] = 1;
        mem[1] = 2;
        PacketField *packetField = gam->GetPacketConfig();
        ret = event1->Check((uint8*) mem, &packetField[0]);
        if (ret) {
            uint8 flag = 0u;
            //give max 10 sec to execute the messages
            const uint32 nTries = 10u;
            ret = false;
            for (uint32 i = 0u; i < nTries; i++) {
                flag = ds->GetFlag();
                if (flag == 0x7) {
                    ret = (event1->Replied(&packetField[0]) == 3);
                    break;
                }
                Sleep::Sec(1);
            }

        }
    }

    god->Purge();
    return ret;
}

bool EventConditionTriggerTest::TestExecute_Mixed() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = EventConditionTriggerTestGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        Command = 1"
            "                        Status = 2"
            "                    }"
            "                    +TrigFun1 = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun1\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                    +TrigFun2 = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun2\""
            "                        Mode = ExpectsReply"
            "                    }"
            "                    +TrigFun3 = {"
            "                        Class = Message"
            "                        Destination = Application.Data.Input"
            "                        Function = \"TrigFun3\""
            "                        Mode = ExpectsIndirectReply"
            "                    }"
            "                }"
            "             }"
            "             InputSignals = {"
            "                 Command = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 Status = {"
            "                     DataSource = Input"
            "                     Trigger = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 CommandAck = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "                 StatusOut = {"
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
            "            Class = EventConditionTriggerTestDS"
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

    //initialise the whole environment
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    //get the event
    ReferenceT<EventConditionTrigger> event1;
    if (ret) {
        event1 = god->Find("Application.Functions.GAM1.Events.Event1");
        ret = event1.IsValid();
    }

    //get the ds memory

    ReferenceT<EventConditionTriggerTestDS> ds;
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }

    ReferenceT<EventConditionTriggerTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }

    uint32 *mem = NULL;
    if (ret) {
        ret = ds->GetSignalMemoryBuffer(0u, 0u, (void*&) mem);

    }

    if (ret) {
        mem[0] = 1;
        mem[1] = 2;
        PacketField *packetField = gam->GetPacketConfig();
        ret = event1->Check((uint8*) mem, &packetField[0]);
        if (ret) {
            uint8 flag = 0u;
            //give max 10 sec to execute the messages
            const uint32 nTries = 10u;
            ret = false;
            for (uint32 i = 0u; i < nTries; i++) {
                flag = ds->GetFlag();
                if (flag == 0x7) {
                    ret = (event1->Replied(&packetField[0]) == 3);
                    break;
                }
                Sleep::Sec(1);
            }

        }
    }

    god->Purge();
    return ret;
}

bool EventConditionTriggerTest::TestReplied_ImmediateReply() {
    return TestExecute_ImmediateReply();
}

bool EventConditionTriggerTest::TestReplied_IndirectReply() {
    return TestExecute_IndirectReply();
}

bool EventConditionTriggerTest::TestReplied_Mixed() {
    return TestExecute_Mixed();
}

bool EventConditionTriggerTest::TestGetCPUMask() {
    return TestInitialise_CPUMask();
}
