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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class EventConditionMessageGAMTest: public GAM, public MessageI, public StatefulI {
public:

    CLASS_REGISTER_DECLARATION()
    EventConditionMessageGAMTest();
    virtual ~EventConditionMessageGAMTest();
    virtual bool Initialise(StructuredDataI &data);
    virtual bool Setup();
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);
    virtual bool Execute();
    uint32 GetNumberOfCommands() const ;
    uint32 GetNumberOfEvents() const ;
    virtual void Purge(ReferenceContainer &purgeList);

protected:
    virtual bool IsChanged(const uint32 cIdx) const;
    uint32 numberOfFields;
    SignalMetadata *signalMetadata;
    ReferenceT<ReferenceContainer> events;
    uint32 numberOfCommands;
    uint32 numberOfEvents;
    uint32 *cntTrigger;
    uint8 *currentValue;
    uint32 *commandIndex;
    uint8 *previousValue;
    uint8 trigOnChange;
    bool firstTime;
};

EventConditionMessageGAMTest::EventConditionMessageGAMTest() :
        GAM(),
        MessageI() {
    numberOfFields = 0u;
    signalMetadata = NULL_PTR(SignalMetadata*);
    numberOfCommands = 0u;
    numberOfEvents = 0u;
    cntTrigger = NULL_PTR(uint32*);
    currentValue = NULL_PTR(uint8*);
    previousValue = NULL_PTR(uint8*);
    commandIndex = NULL_PTR(uint32*);
    trigOnChange = 1u;
    firstTime = true;
}

EventConditionMessageGAMTest::~EventConditionMessageGAMTest() {
    if (signalMetadata != NULL_PTR(SignalMetadata*)) {
        delete[] signalMetadata;
    }
    if (previousValue != NULL_PTR(uint8*)) {
        delete[] previousValue;
    }
    if (commandIndex != NULL_PTR(uint32*)) {
        delete[] commandIndex;
    }
    cntTrigger = NULL_PTR(uint32*);
    currentValue = NULL_PTR(uint8*);
}

bool EventConditionMessageGAMTest::Initialise(StructuredDataI &data) {
    bool ret = GAM::Initialise(data);
    if (ret) {
        if (!data.Read("TriggerOnChange", trigOnChange)) {
            trigOnChange = 1u;
        }
    }
    return ret;
}

bool EventConditionMessageGAMTest::Setup() {

    numberOfFields = 0u;
    bool ret = true;

    uint32 numberOfSignals = GetNumberOfInputSignals();

    for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
        uint32 numberOfElements = 0u;
        ret = GetSignalNumberOfElements(InputSignals, i, numberOfElements);
        if (ret) {
            numberOfFields += numberOfElements;
        }
    }

    uint32 metadataSize = 0u;
    if (ret) {
        /*lint -e{613} NULL pointer checked.*/
        signalMetadata = new SignalMetadata[numberOfFields];
        ret = (signalMetadata != NULL_PTR(SignalMetadata*));

        uint32 n = 0u;

        /*lint -e{850} the loop variable is not modified within the loop*/
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            StreamString signalName;
            ret = GetSignalName(InputSignals, i, signalName);
            /*lint -e{613} NULL pointer checked.*/
            signalMetadata[n].name = signalName;
            if (ret) {
                TypeDescriptor type = GetSignalType(InputSignals, i);
                /*lint -e{613} NULL pointer checked.*/
                signalMetadata[n].type = type;
                bool isCommand = (StringHelper::CompareN(signalName.Buffer(), "Command", StringHelper::Length("Command")) == 0);
                /*lint -e{613} NULL pointer checked.*/
                signalMetadata[n].isCommand = isCommand;
                /*lint -e{613} NULL pointer checked.*/
                if (signalMetadata[n].isCommand) {
                    numberOfCommands++;
                }

                /*lint -e{613} NULL pointer checked.*/
                uint32 typeSize = (static_cast<uint32>(signalMetadata[i].type.numberOfBits) / 8u);
                /*lint -e{613} NULL pointer checked.*/
                signalMetadata[n].offset = metadataSize;
                metadataSize += typeSize;

                uint32 numberOfElements = 0u;
                ret = GetSignalNumberOfElements(InputSignals, i, numberOfElements);
                if (ret) {
                    if (numberOfElements > 1u) {
                        for (uint32 j = 0u; (j < numberOfElements) && (ret); j++) {
                            StreamString newSignalName = signalName;
                            ret = newSignalName.Printf("_%d", j);
                            if (ret) {
                                /*lint -e{613} NULL pointer checked.*/
                                signalMetadata[n].name = newSignalName;
                                if (j > 0u) {
                                    /*lint -e{613} NULL pointer checked.*/
                                    signalMetadata[n].type = type;
                                    /*lint -e{613} NULL pointer checked.*/
                                    signalMetadata[n].isCommand = isCommand;
                                    /*lint -e{613} NULL pointer checked.*/
                                    if (signalMetadata[n].isCommand) {
                                        numberOfCommands++;
                                    }
                                    /*lint -e{613} NULL pointer checked.*/
                                    signalMetadata[n].offset = metadataSize;
                                    metadataSize += typeSize;
                                }
                            }
                            n++;
                        }
                    }
                    else {
                        n++;
                    }
                }
            }
        }
    }

    if (ret) {
        ret = (numberOfCommands > 0u);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The number of commands is 0");
        }
    }

    if (ret) {
        previousValue = new uint8[metadataSize];
        (void) MemoryOperationsHelper::Set(previousValue, '\0', metadataSize);
        commandIndex = new uint32[numberOfCommands];

        uint32 c = 0u;
        for (uint32 i = 0u; i < numberOfFields; i++) {
            /*lint -e{613} NULL pointer checked.*/
            if (signalMetadata[i].isCommand) {
                commandIndex[c] = i;
                c++;
            }
        }

//build the lookup table
        events = this->Find("Events");
        ret = events.IsValid();
        if (ret) {
            numberOfEvents = events->Size();
            for (uint32 i = 0u; i < numberOfEvents; i++) {
                ReferenceT<EventConditionTrigger> eventCondition = events->Get(i);
                ret = eventCondition.IsValid();
                if (ret) {
                    ret = eventCondition->SetMetadataConfig(signalMetadata, numberOfFields);
                }
            }
        }

    }

    if (ret) {
//check that the state signal has siz #elements
        uint32 totalNumberOfOutputs = 0u;
        for (uint32 i = 0u; (i < numberOfOutputSignals) && (ret); i++) {
            TypeDescriptor td = GetSignalType(OutputSignals, i);
            ret = (td == UnsignedInteger32Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The type of the output signal (reply state) must be uint32");
            }
            else {
                uint32 numberOfElements;
                ret = GetSignalNumberOfElements(OutputSignals, 0u, numberOfElements);
                totalNumberOfOutputs += numberOfElements;
            }

        }
        if (ret) {
            ret = (totalNumberOfOutputs == numberOfCommands);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The number of outputs %d must match the number of commands %d", totalNumberOfOutputs,
                             numberOfCommands);
            }
        }
    }

    if (ret) {
        currentValue = reinterpret_cast<uint8*>(GetInputSignalsMemory());
        cntTrigger = reinterpret_cast<uint32*>(GetOutputSignalsMemory());
    }

    return ret;
}

bool EventConditionMessageGAMTest::PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName) {

    firstTime = trigOnChange;
    REPORT_ERROR(ErrorManagement::Information, "\nCalled MessageGAM::PrepareNextState\n");

    return true;
}

bool EventConditionMessageGAMTest::Execute() {
    if (firstTime) {
        for (uint32 i = 0u; i < numberOfCommands; i++) {

            (void) MemoryOperationsHelper::Copy(&previousValue[signalMetadata[commandIndex[i]].offset], &currentValue[signalMetadata[commandIndex[i]].offset],
                                                (static_cast<uint32>(signalMetadata[commandIndex[i]].type.numberOfBits) / 8u));

            REPORT_ERROR(ErrorManagement::Information, "\nFirst Time %s u-->%u\n", signalMetadata[commandIndex[i]].name.Buffer(), currentValue[signalMetadata[commandIndex[i]].offset],
                         previousValue[signalMetadata[commandIndex[i]].offset]);
        }

        firstTime = false;
    }
    else {

        for (uint32 i = 0u; i < numberOfCommands; i++) {

            bool check = (cntTrigger[i] == 0u);

            /*lint -e{613} NULL pointer checked.*/
            if (cntTrigger[i] > 0u) {
                for (uint32 j = 0u; j < numberOfEvents; j++) {
                    ReferenceT<EventConditionTrigger> eventCondition = events->Get(j);
                    if (eventCondition.IsValid()) {
                        uint32 nReplies = eventCondition->Replied(&signalMetadata[commandIndex[i]], cntTrigger[i]);
                        /*lint -e{613} NULL pointer checked.*/
                        cntTrigger[i] -= nReplies;
                    }
                }
            }

            bool trigEvent = false;
            if (trigOnChange == 0u) {
                trigEvent = check;
            }
            else {
                trigEvent = IsChanged(i);
            }

            if (trigEvent) {
                //rising edge, send the message associated to the code
                for (uint32 j = 0u; j < numberOfEvents; j++) {
                    ReferenceT<EventConditionTrigger> eventCondition = events->Get(j);
                    if (eventCondition.IsValid()) {
                        /*lint -e{613} NULL pointer checked.*/
                        if (eventCondition->Check(currentValue, &signalMetadata[commandIndex[i]])) {
                            uint32 nMessages = eventCondition->Size();
                            //trigger all the messages of that event
                            /*lint -e{613} NULL pointer checked.*/
                            cntTrigger[i] += nMessages;
                        }
                    }
                }
                /*lint -e{613} NULL pointer checked.*/
                (void) MemoryOperationsHelper::Copy(&previousValue[signalMetadata[commandIndex[i]].offset], &currentValue[signalMetadata[commandIndex[i]].offset],
                                                    (static_cast<uint32>(signalMetadata[commandIndex[i]].type.numberOfBits) / 8u));
            }
        }
    }
    return true;
}

bool EventConditionMessageGAMTest::IsChanged(const uint32 cIdx) const {
    //check if the command i had a rising event
    /*lint -e{613} NULL pointer checked.*/
    uint32 commandOffset = signalMetadata[commandIndex[cIdx]].offset;
    /*lint -e{613} NULL pointer checked.*/
    uint32 commandSize = static_cast<uint32>(signalMetadata[commandIndex[cIdx]].type.numberOfBits) / 8u;
    /*lint -e{613} NULL pointer checked.*/
    return (MemoryOperationsHelper::Compare(&currentValue[commandOffset], &previousValue[commandOffset], commandSize) != 0);
}

uint32 EventConditionMessageGAMTest::GetNumberOfCommands() const {
    return numberOfCommands;
}

uint32 EventConditionMessageGAMTest::GetNumberOfEvents() const {
    return numberOfEvents;

}

void EventConditionMessageGAMTest::Purge(ReferenceContainer &purgeList) {
    if (events.IsValid()) {
        events->Purge(purgeList);
    }
    ReferenceContainer::Purge(purgeList);
}

CLASS_REGISTER(EventConditionMessageGAMTest, "1.0")


class EventConditionTriggerTestGAM: public EventConditionMessageGAMTest {
public:
    CLASS_REGISTER_DECLARATION()

EventConditionTriggerTestGAM    ();
    ~EventConditionTriggerTestGAM();

    uint8 *GetPreviousValue();
    SignalMetadata *GetMetadataConfig();
    uint32 GetNumberOfFields();
    ReferenceT<ReferenceContainer> GetEvents();
    uint32 GetNumberOfEvents();
    uint32 *GetCommandIndex();
    uint32 GetNumberOfCommands();
    uint32 *GetCntTrigger();
    uint8 *GetCurrentValue();

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


SignalMetadata *EventConditionTriggerTestGAM::GetMetadataConfig() {
    return signalMetadata;
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


uint32 *EventConditionTriggerTestGAM::GetCntTrigger() {
    return cntTrigger;
}
uint8 *EventConditionTriggerTestGAM::GetCurrentValue() {
    return currentValue;
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

bool EventConditionTriggerTest::TestSetMetadataConfig() {

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

    SignalMetadata signalMetadata[5];
    signalMetadata[0].isCommand = true;
    signalMetadata[0].name = "Command1";
    signalMetadata[0].offset = 0;
    signalMetadata[0].type = UnsignedInteger64Bit;

    signalMetadata[1].isCommand = true;
    signalMetadata[1].name = "Command2";
    signalMetadata[1].offset = 8;
    signalMetadata[1].type = UnsignedInteger32Bit;

    signalMetadata[2].isCommand = true;
    signalMetadata[2].name = "Command3";
    signalMetadata[2].offset = 12;
    signalMetadata[2].type = UnsignedInteger16Bit;

    signalMetadata[3].isCommand = true;
    signalMetadata[3].name = "Command4";
    signalMetadata[3].offset = 14;
    signalMetadata[3].type = UnsignedInteger8Bit;

    signalMetadata[4].isCommand = false;
    signalMetadata[4].name = "State";
    signalMetadata[4].offset = 15;
    signalMetadata[4].type = SignedInteger32Bit;

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
        ret = comp.SetMetadataConfig(signalMetadata, 5);
    }

    if (ret) {
        EventConditionTriggerTestComp::EventConditionField *fields = comp.GetEventConditions();
        ret = (fields != NULL);
        if (ret) {
            ret = fields[0].signalMetadata == &signalMetadata[0];
        }
        if (ret) {
            ret = (fields[0].at.GetTypeDescriptor() == UnsignedInteger64Bit);
        }
        if (ret) {
            uint64 *data = (uint64*) (fields[0].at.GetDataPointer());
            ret = (*data == 1);
        }

        if (ret) {
            ret = fields[1].signalMetadata == &signalMetadata[1];
        }
        if (ret) {
            ret = (fields[1].at.GetTypeDescriptor() == UnsignedInteger32Bit);
        }
        if (ret) {
            uint32 *data = (uint32*) (fields[1].at.GetDataPointer());
            ret = (*data == 2);
        }

        if (ret) {
            ret = fields[2].signalMetadata == &signalMetadata[2];
        }
        if (ret) {
            ret = (fields[2].at.GetTypeDescriptor() == UnsignedInteger16Bit);
        }
        if (ret) {
            uint16 *data = (uint16*) (fields[2].at.GetDataPointer());
            ret = (*data == 3);
        }

        if (ret) {
            ret = fields[3].signalMetadata == &signalMetadata[3];
        }
        if (ret) {
            ret = (fields[3].at.GetTypeDescriptor() == UnsignedInteger8Bit);
        }
        if (ret) {
            uint8 *data = (uint8*) (fields[3].at.GetDataPointer());
            ret = (*data == 4);
        }

        if (ret) {
            ret = fields[4].signalMetadata == &signalMetadata[4];
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

bool EventConditionTriggerTest::TestSetMetadataConfig_FalseNoMetadataFieldMatch() {
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

    SignalMetadata signalMetadata[5];
    signalMetadata[0].isCommand = true;
    signalMetadata[0].name = "Command1";
    signalMetadata[0].offset = 0;
    signalMetadata[0].type = UnsignedInteger64Bit;

    signalMetadata[1].isCommand = true;
    signalMetadata[1].name = "Command2";
    signalMetadata[1].offset = 8;
    signalMetadata[1].type = UnsignedInteger32Bit;

    signalMetadata[2].isCommand = true;
    signalMetadata[2].name = "Command3";
    signalMetadata[2].offset = 12;
    signalMetadata[2].type = UnsignedInteger16Bit;

    signalMetadata[3].isCommand = true;
    signalMetadata[3].name = "Command4";
    signalMetadata[3].offset = 14;
    signalMetadata[3].type = UnsignedInteger8Bit;

    signalMetadata[4].isCommand = false;
    signalMetadata[4].name = "State";
    signalMetadata[4].offset = 15;
    signalMetadata[4].type = SignedInteger32Bit;

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
        ret = !comp.SetMetadataConfig(signalMetadata, 5);
    }

    return ret;

}

bool EventConditionTriggerTest::TestSetMetadataConfig_FalseReadFailedTypeMismatch() {
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

    SignalMetadata signalMetadata[5];
    signalMetadata[0].isCommand = true;
    signalMetadata[0].name = "Command1";
    signalMetadata[0].offset = 0;
    signalMetadata[0].type = UnsignedInteger64Bit;

    signalMetadata[1].isCommand = true;
    signalMetadata[1].name = "Command2";
    signalMetadata[1].offset = 8;
    signalMetadata[1].type = UnsignedInteger32Bit;

    signalMetadata[2].isCommand = true;
    signalMetadata[2].name = "Command3";
    signalMetadata[2].offset = 12;
    signalMetadata[2].type = UnsignedInteger16Bit;

    signalMetadata[3].isCommand = true;
    signalMetadata[3].name = "Command4";
    signalMetadata[3].offset = 14;
    signalMetadata[3].type = UnsignedInteger8Bit;

    signalMetadata[4].isCommand = false;
    signalMetadata[4].name = "State";
    signalMetadata[4].offset = 15;
    signalMetadata[4].type = SignedInteger32Bit;

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
        ret = !comp.SetMetadataConfig(signalMetadata, 5);
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

    SignalMetadata signalMetadata[5];
    signalMetadata[0].isCommand = true;
    signalMetadata[0].name = "Command1";
    signalMetadata[0].offset = 0;
    signalMetadata[0].type = UnsignedInteger64Bit;

    signalMetadata[1].isCommand = true;
    signalMetadata[1].name = "Command2";
    signalMetadata[1].offset = 8;
    signalMetadata[1].type = UnsignedInteger32Bit;

    signalMetadata[2].isCommand = true;
    signalMetadata[2].name = "Command3";
    signalMetadata[2].offset = 12;
    signalMetadata[2].type = UnsignedInteger16Bit;

    signalMetadata[3].isCommand = true;
    signalMetadata[3].name = "Command4";
    signalMetadata[3].offset = 14;
    signalMetadata[3].type = UnsignedInteger8Bit;

    signalMetadata[4].isCommand = false;
    signalMetadata[4].name = "State";
    signalMetadata[4].offset = 15;
    signalMetadata[4].type = SignedInteger32Bit;

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
        ret = comp.SetMetadataConfig(signalMetadata, 5);
    }

    if (ret) {
        uint8 mem[19];
        *(uint64*) mem = 1;
        *(uint32*) (mem + 8) = 2;
        *(uint16*) (mem + 12) = 3;
        *(uint8*) (mem + 14) = 4;
        *(int32*) (mem + 15) = -4;

        ret &= !comp.Check(mem, NULL);
        ret = comp.Check(mem, &signalMetadata[0]);
        ret &= comp.Check(mem, &signalMetadata[1]);
        ret &= !comp.Check(mem, &signalMetadata[2]);
        ret &= comp.Check(mem, &signalMetadata[3]);
        ret = !comp.Check(mem, &signalMetadata[4]);
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

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,"Find and IsValid Input");
    }

    ReferenceT<EventConditionTriggerTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,"Find and IsValid GAM1");
    }

    uint32 *mem = NULL;
    if (ret) {
        ret = ds->GetSignalMemoryBuffer(0u, 0u, (void*&) mem);

    }

    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,"Fail GetSignalMemoryBuffer");
    }

    if (ret) {
        mem[0] = 1;
        mem[1] = 2;
        SignalMetadata *signalMetadata = gam->GetMetadataConfig();
        ret = event1->Check((uint8*) mem, &signalMetadata[0]);
        if (ret) {
            uint8 flag = 0u;
            //give max 10 sec to execute the messages
            const uint32 nTries = 10u;
            ret = false;
            for (uint32 i = 0u; i < nTries; i++) {
                flag = ds->GetFlag();
                if (flag == 0x7) {
                    ret = (event1->Replied(&signalMetadata[0], 3) == 3);
                    break;
                }
                Sleep::Sec(1);
            }

        }
    }
    
    if(!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,"Fail Replied");
    }

    god->Purge();
    return ret;
}

bool EventConditionTriggerTest::TestPurge() {
    return TestExecute_ImmediateReply();
}

bool EventConditionTriggerTest::TestEventConditionField() {
    return TestExecute_ImmediateReply();
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
        SignalMetadata *signalMetadata = gam->GetMetadataConfig();
        ret = event1->Check((uint8*) mem, &signalMetadata[0]);
        if (ret) {
            uint8 flag = 0u;
            //give max 10 sec to execute the messages
            const uint32 nTries = 10u;
            ret = false;
            for (uint32 i = 0u; i < nTries; i++) {
                flag = ds->GetFlag();
                if (flag == 0x7) {
                    ret = (event1->Replied(&signalMetadata[0], 3) == 3);
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
        SignalMetadata *signalMetadata = gam->GetMetadataConfig();
        ret = event1->Check((uint8*) mem, &signalMetadata[0]);
        if (ret) {
            uint8 flag = 0u;
            //give max 10 sec to execute the messages
            const uint32 nTries = 10u;
            ret = false;
            for (uint32 i = 0u; i < nTries; i++) {
                flag = ds->GetFlag();
                if (flag == 0x7) {
                    ret = (event1->Replied(&signalMetadata[0], 3) == 3);
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
