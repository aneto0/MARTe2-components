/**
 * @file DoubleHandshakeMasterGAMTest.cpp
 * @brief Source file for class DoubleHandshakeMasterGAMTest
 * @date 07/08/2019
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
 * the class DoubleHandshakeMasterGAMTest (public, protected, and private). Be aware that some 
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
#include "DataSourceI.h"
#include "DoubleHandshakeMasterGAMTest.h"
#include "GAMSchedulerI.h"
#include "MemoryDataSourceI.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "MessageGAM.h"
#include "MessageI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class DoubleHandShakeMasterGAMTestGAM: public DoubleHandshakeMasterGAM {
public:
    CLASS_REGISTER_DECLARATION()

    DoubleHandShakeMasterGAMTestGAM();

    virtual ~DoubleHandShakeMasterGAMTestGAM();

    void *GetInputMemoryX();
    void *GetOutputMemoryX();
};

DoubleHandShakeMasterGAMTestGAM::DoubleHandShakeMasterGAMTestGAM() {

}

DoubleHandShakeMasterGAMTestGAM::~DoubleHandShakeMasterGAMTestGAM() {

}

void *DoubleHandShakeMasterGAMTestGAM::GetInputMemoryX() {
    return GetInputSignalsMemory();
}
void *DoubleHandShakeMasterGAMTestGAM::GetOutputMemoryX() {
    return GetOutputSignalsMemory();
}

CLASS_REGISTER(DoubleHandShakeMasterGAMTestGAM, "1.0")

class CommandProviderDS: public MemoryDataSourceI, public MessageI {

public:
    CLASS_REGISTER_DECLARATION()
    CommandProviderDS();

    virtual ~CommandProviderDS();

    const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    virtual bool Synchronise();

    void SetCommand(uint32 index,
            void* command);

    ErrorManagement::ErrorType TrigFun1();
    ErrorManagement::ErrorType TrigFun2();
    ErrorManagement::ErrorType TrigFun3();

    void ResetFlag();
    uint8 GetFlag();

    void SetSleepTime(uint32 sleepTime);
private:
    uint8 callFunFlag;
    uint32 sleepTimeMSec;

};

CommandProviderDS::CommandProviderDS() {
    ReferenceT < RegisteredMethodsMessageFilter > filter = ReferenceT < RegisteredMethodsMessageFilter > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
    callFunFlag = 0u;
    sleepTimeMSec = 0u;
}

CommandProviderDS::~CommandProviderDS() {

}

void CommandProviderDS::SetSleepTime(uint32 sleepTime) {
    sleepTimeMSec = sleepTime;
}

const char8 *CommandProviderDS::GetBrokerName(StructuredDataI &data,
                                              const SignalDirection direction) {
    const char8* brokerName = NULL;

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
        if (brokerName == NULL) {
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
        if (brokerName == NULL) {
            brokerName = "MemoryMapOutputBroker";
        }
    }

    return brokerName;
}

bool CommandProviderDS::Synchronise() {
    return true;
}

void CommandProviderDS::SetCommand(uint32 index,
                                   void* command) {
    void* signalPtr = NULL;

    GetSignalMemoryBuffer(index, 0u, signalPtr);
    uint32 byteSize = 0u;
    GetSignalByteSize(index, byteSize);
    MemoryOperationsHelper::Copy(signalPtr, command, byteSize);

}

bool CommandProviderDS::PrepareNextState(const char8 * const currentStateName,
                                         const char8 * const nextStateName) {
    return true;
}

ErrorManagement::ErrorType CommandProviderDS::TrigFun1() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function1");
    callFunFlag |= 1;
    if (sleepTimeMSec > 0) {
        Sleep::MSec (sleepTimeMSec);
    }
    return ret;
}

ErrorManagement::ErrorType CommandProviderDS::TrigFun2() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function2");
    callFunFlag |= (1 << 1);
    if (sleepTimeMSec > 0) {
        Sleep::MSec (sleepTimeMSec);
    }
    return ret;
}

ErrorManagement::ErrorType CommandProviderDS::TrigFun3() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function3");
    callFunFlag |= (1 << 2);
    if (sleepTimeMSec > 0) {
        Sleep::MSec (sleepTimeMSec);
    }
    return ret;
}

void CommandProviderDS::ResetFlag() {
    callFunFlag = 0;
}

uint8 CommandProviderDS::GetFlag() {
    return callFunFlag;
}

CLASS_METHOD_REGISTER(CommandProviderDS, TrigFun1)
CLASS_METHOD_REGISTER(CommandProviderDS, TrigFun2)
CLASS_METHOD_REGISTER(CommandProviderDS, TrigFun3)

CLASS_REGISTER(CommandProviderDS, "1.0")

class PlcSimulatorMasterDS: public MemoryDataSourceI, public MessageI {

public:
    CLASS_REGISTER_DECLARATION()
    PlcSimulatorMasterDS();

    virtual ~PlcSimulatorMasterDS();

    virtual bool Initialise(StructuredDataI & data);

    const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);
    virtual bool Synchronise();

    ErrorManagement::ErrorType SetState(uint32 index,
            uint32 value);

    ErrorManagement::ErrorType SetSignal(uint32 index,
            uint32 value);

private:
    uint32 *counter;
    uint32 nCycles;
    uint32 numberOfInputs;
    uint32 *states;
    uint32 numberOfStates;
    uint8 manual;
};
CLASS_REGISTER(PlcSimulatorMasterDS, "1.0")

CLASS_METHOD_REGISTER(PlcSimulatorMasterDS, SetState)
CLASS_METHOD_REGISTER(PlcSimulatorMasterDS, SetSignal)

PlcSimulatorMasterDS::PlcSimulatorMasterDS() {
    ReferenceT < RegisteredMethodsMessageFilter > filter = ReferenceT < RegisteredMethodsMessageFilter > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }

    counter=NULL;
    nCycles=0u;
    numberOfInputs=0u;
    states=NULL;
    numberOfStates=0u;
    manual=0u;
}

PlcSimulatorMasterDS::~PlcSimulatorMasterDS() {
    if (counter != NULL) {
        delete[] counter;
    }
    if (states != NULL) {
        delete[] states;
    }
}

bool PlcSimulatorMasterDS::Initialise(StructuredDataI & data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        if (!data.Read("DelayCount", nCycles)) {
            nCycles = 10u;
        }
        if (!data.Read("NumberOfInputs", numberOfInputs)) {
            numberOfInputs = 0u;
        }
        if (!data.Read("Manual", manual)) {
            manual = 0u;
        }
    }
    return ret;
}

const char8 *PlcSimulatorMasterDS::GetBrokerName(StructuredDataI &data,
                                                 const SignalDirection direction) {
    const char8* brokerName = NULL;

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
        if (brokerName == NULL) {
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
        if (brokerName == NULL) {
            brokerName = "MemoryMapOutputBroker";
        }
    }

    return brokerName;
}

bool PlcSimulatorMasterDS::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = MemoryDataSourceI::SetConfiguredDatabase(data);

    if (ret) {
        counter = new uint32[numberOfInputs];
        for (uint32 i = 0u; i < numberOfInputs; i++) {
            counter[i] = 0u;
        }
        if (numberOfSignals > (2 * numberOfInputs)) {
            numberOfStates = (numberOfSignals - (2 * numberOfInputs));
            states = new uint32[numberOfStates];
        }
    }
    return ret;
}

bool PlcSimulatorMasterDS::PrepareNextState(const char8 * const currentStateName,
                                            const char8 * const nextStateName) {
    return true;
}
bool PlcSimulatorMasterDS::Synchronise() {
    if (manual == 0u) {
        for (uint32 i = 0u; i < numberOfInputs; i++) {
            counter[i]++;
            if (counter[i] >= nCycles) {
                counter[i] = 0u;
                void* signalIn = NULL;
                GetSignalMemoryBuffer(i, 0u, signalIn);
                void* signalOut = NULL;
                GetSignalMemoryBuffer(numberOfInputs + i, 0u, signalOut);
                uint32 byteSize;
                GetSignalByteSize(i, byteSize);
                MemoryOperationsHelper::Copy(signalOut, signalIn, byteSize);
            }
        }
    }
    uint32 *statePtr = NULL;
    GetSignalMemoryBuffer((2 * numberOfInputs), 0u, (void*&) statePtr);

    for (uint32 i = 0u; i < numberOfStates; i++) {
        statePtr[i] = states[i];
    }
    return true;
}

ErrorManagement::ErrorType PlcSimulatorMasterDS::SetState(uint32 index,
                                                          uint32 value) {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called SetState");

    states[index] = value;
    return ret;
}

ErrorManagement::ErrorType PlcSimulatorMasterDS::SetSignal(uint32 index,
                                                           uint32 value) {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called SetSignal");

    uint32 *signalPtr = NULL;
    GetSignalMemoryBuffer(index, 0u, (void*&) signalPtr);

    *signalPtr = value;

    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

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
    ReferenceT < RealTimeApplication > application;
    if (ok) {
        application = god->Find("Application");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}

DoubleHandshakeMasterGAMTest::DoubleHandshakeMasterGAMTest() {
    // Auto-generated constructor stub for DoubleHandshakeMasterGAMTest
    // TODO Verify if manual additions are needed
}

DoubleHandshakeMasterGAMTest::~DoubleHandshakeMasterGAMTest() {
    // Auto-generated destructor stub for DoubleHandshakeMasterGAMTest
    // TODO Verify if manual additions are needed
}

bool DoubleHandshakeMasterGAMTest::TestConstructor() {

    DoubleHandshakeMasterGAM *test = new DoubleHandshakeMasterGAM;
    bool ret = (test != NULL);
    delete test;
    return ret;
}

bool DoubleHandshakeMasterGAMTest::TestSetup() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeMasterGAMTest::TestSetup_UselessSignal() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               Ciaone = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
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
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

/* Commented out as it was not executed
bool DoubleHandshakeMasterGAMTest::TestSetup_NumberOfElements() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   NumberOfElements = 2"
            "                   Default = 0"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn = {"
            "                   Alias = Command"
            "                   NumberOfElements=2"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   NumberOfElements=2"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn = {"
            "                   NumberOfElements = 2"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    return true;
}
//Commented out as it was not executed and only returning true
*/

bool DoubleHandshakeMasterGAMTest::TestSetup_FalseNInCommandsDiffThanNInAcks() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
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

bool DoubleHandshakeMasterGAMTest::TestSetup_FalseNInCommandsDiffThanClearTrigs() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
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

bool DoubleHandshakeMasterGAMTest::TestSetup_FalseClearTrigsWrongType() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint16"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint16"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
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

bool DoubleHandshakeMasterGAMTest::TestSetup_FalseInternalStateWrongType() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
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

bool DoubleHandshakeMasterGAMTest::TestSetup_FalseNInCommandsDiffThanNOutCommands() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
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

bool DoubleHandshakeMasterGAMTest::TestSetup_FalseNInCommandsDiffThanNOutStatus() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
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

bool DoubleHandshakeMasterGAMTest::TestSetup_FalseSizeInCommandDiffThanSizeInAck() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint16"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint16"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
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

bool DoubleHandshakeMasterGAMTest::TestSetup_FalseSizeInCommandDiffThanSizeOutCommand() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint16"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint16"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC }"
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

bool DoubleHandshakeMasterGAMTest::TestExecute() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +FC2PLC = {"
            "           Class = DoubleHandShakeMasterGAMTestGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = CommandProviderDS"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = CommandProviderDS"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8" 
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +MessageGAM = {"
            "           Class = MessageGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        CommandInAck0 = 1"
            "                        State0 = 2"
            "                    }"
            "                    +Message1 = {"
            "                        Class = Message"
            "                        Destination = \"Application.Data.CommandProviderDS\""
            "                        Function = TrigFun1"
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "             }"
            "             InputSignals = {"
            "               CommandInAck0 = {"
            "                   Alias = AckIn0"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandInAck1 = {"
            "                   Alias = AckIn1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "             }"
            "             OutputSignals = {"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
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
            "        +CommandProviderDS = {"
            "            Class = CommandProviderDS"
            "            Signals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   Default = 0"
            "               }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 5"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { FC2PLC, MessageGAM }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { FC2PLC, MessageGAM }"
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
    ReferenceT<DoubleHandShakeMasterGAMTestGAM> dhGAM;
    if (ret) {
        dhGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.FC2PLC");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer dhInputBrokers;
    ReferenceContainer dhOutputBrokers;

    if (ret) {
        dhGAM->GetInputBrokers(dhInputBrokers);
        dhGAM->GetOutputBrokers(dhOutputBrokers);
    }

    ReferenceT<MessageGAM> messGAM;
    if (ret) {
        messGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.MessageGAM");
        ret = messGAM.IsValid();
    }

    ReferenceContainer messInputBrokers;
    ReferenceContainer messOutputBrokers;

    if (ret) {
        messGAM->GetInputBrokers(messInputBrokers);
        messGAM->GetOutputBrokers(messOutputBrokers);
    }

    ReferenceT < CommandProviderDS > cmdProvDS;
    if (ret) {
        cmdProvDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.CommandProviderDS");
        ret = (cmdProvDS.IsValid());
    }

    ReferenceT < PlcSimulatorMasterDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorMasterDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalDhGamState0;
    if (ret) {
        internalDhGamState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
    }

    if (ret) {
        uint32 command0 = 1;
        cmdProvDS->SetCommand(0u, &command0);
        uint32 state0 = 2;
        plcSimDS->SetState(0u, state0);
    }

    //first cycles
    ret = false;
    for (uint32 j = 0u; j < 13u; j++) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
        //TODO: Was commented

        if (ret && (*internalDhGamState0 == 3u)) {
            uint32 command0 = 0;
            cmdProvDS->SetCommand(0u, &command0);
        }

        for (uint32 i = 0u; i < 10u; i++) {
            if (cmdProvDS->GetFlag() != 0x1u) {
                Sleep::MSec(100u);
            }
            else {
                ret = true;
                break;
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

const char8 *configErrorTests = ""
        "$Application = {"
        "   Class = RealTimeApplication"
        "   +Functions = {"
        "       Class = ReferenceContainer"
        "       +FC2PLC = {"
        "           Class = DoubleHandShakeMasterGAMTestGAM"
        "           InputSignals = {"
        "               CommandIn0 = {"
        "                   Alias = Command0"
        "                   Type = uint32"
        "                   DataSource = CommandProviderDS"
        "               }"
        "               CommandIn1 = {"
        "                   Alias = Command1"
        "                   Type = uint32"
        "                   DataSource = CommandProviderDS"
        "               }"
        "               AckIn0 = {"
        "                   Type = uint32"
        "                   DataSource = CommandProviderDS"
        "               }"
        "               AckIn1 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorMasterDS"
        "               }"
        "               ClearIn0 = {"
        "                   Type = uint32"
        "                   DataSource = DDB1"
        "               }"
        "               ClearIn1 = {"
        "                   Type = uint32"
        "                   DataSource = DDB1"
        "               }"
        "           }"
        "           OutputSignals = {"
        "               CommandOut0 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorMasterDS"
        "               }"
        "               CommandOut1 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorMasterDS"
        "               }"
        "               InternalState0 = {"
        "                   Type = uint8"
        "                   DataSource = DDB1"
        "               }"
        "               InternalState1 = {"
        "                   Type = uint8"
        "                   DataSource = DDB1"
        "               }"
        "           }"
        "       }"
        "       +MessageGAM = {"
        "           Class = MessageGAM"
        "            +Events = {"
        "                Class = ReferenceContainer"
        "                +Event1 = {"
        "                    Class = EventConditionTrigger"
        "                    EventTrigger = {"
        "                        CommandInAck0 = 1"
        "                        State0 = 2"
        "                    }"
        "                    +Message1 = {"
        "                        Class = Message"
        "                        Destination = \"Application.Data.CommandProviderDS\""
        "                        Function = TrigFun1"
        "                        Mode = ExpectsReply"
        "                    }"
        "                }"
        "             }"
        "             InputSignals = {"
        "               CommandInAck0 = {"
        "                   Alias = AckIn0"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorMasterDS"
        "               }"
        "               CommandInAck1 = {"
        "                   Alias = AckIn1"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorMasterDS"
        "               }"
        "               State0 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorMasterDS"
        "               }"
        "             }"
        "             OutputSignals = {"
        "               ClearIn0 = {"
        "                   Type = uint32"
        "                   DataSource = DDB1"
        "               }"
        "               ClearIn1 = {"
        "                   Type = uint32"
        "                   DataSource = DDB1"
        "               }"
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
        "        +CommandProviderDS = {"
        "            Class = CommandProviderDS"
        "            Signals = {"
        "               Command0 = {"
        "                   Type = uint32"
        "                   Default = 0"
        "               }"
        "               Command1 = {"
        "                   Type = uint32"
        "                   Default = 0"
        "               }"
        "               AckIn0 = {"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +PlcSimulatorMasterDS = {"
        "            Class = PlcSimulatorMasterDS"
        "            DelayCount = 5"
        "            NumberOfInputs = 2"
        "            Signals = {"
        "               CommandOut0 = {"
        "                   Type = uint32"
        "               }"
        "               CommandOut1 = {"
        "                   Type = uint32"
        "               }"
        "               AckIn1 = {"
        "                   Type = uint32"
        "               }"
        "               State0 = {"
        "                   Type = uint32"
        "               }"
        "            }"
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
        "                    Functions = { FC2PLC, MessageGAM }"
        "                }"
        "            }"
        "         }"
        "        +Acquiring = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = { FC2PLC, MessageGAM }"
        "                }"
        "            }"
        "         }"
        "     }"
        "     +Scheduler = {"
        "         Class = GAMScheduler"
        "         TimingDataSource = Timings"
        "     }"
        "}";

bool DoubleHandshakeMasterGAMTest::TestExecute_ErrorFromReady() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configErrorTests);
    ReferenceT<DoubleHandShakeMasterGAMTestGAM> dhGAM;
    if (ret) {
        dhGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.FC2PLC");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer dhInputBrokers;
    ReferenceContainer dhOutputBrokers;

    ReferenceT < MemoryMapInputBroker > dhBrokerIn;
    ReferenceT < MemoryMapInputBroker > dhBrokerIn1;
    ReferenceT < MemoryMapOutputBroker > dhBrokerOut;

    if (ret) {
        dhGAM->GetInputBrokers(dhInputBrokers);
        dhBrokerIn = dhInputBrokers.Get(0);
        ret = dhBrokerIn.IsValid();
    }
    if (ret) {
        dhBrokerIn1 = dhInputBrokers.Get(1);
        ret = dhBrokerIn1.IsValid();
    }
    if (ret) {
        dhGAM->GetOutputBrokers(dhOutputBrokers);
        dhBrokerOut = dhOutputBrokers.Get(0);
        ret = dhBrokerOut.IsValid();
    }

    ReferenceT<MessageGAM> messGAM;
    if (ret) {
        messGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.MessageGAM");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer messInputBrokers;
    ReferenceContainer messOutputBrokers;

    ReferenceT < MemoryMapInputBroker > messBrokerIn;
    ReferenceT < MemoryMapOutputBroker > messBrokerOut;

    if (ret) {
        messGAM->GetInputBrokers(messInputBrokers);
        messBrokerIn = dhInputBrokers.Get(0);
        ret = messBrokerIn.IsValid();
    }
    if (ret) {
        messGAM->GetOutputBrokers(messOutputBrokers);
        messBrokerOut = messOutputBrokers.Get(0);
        ret = messBrokerOut.IsValid();
    }

    ReferenceT < CommandProviderDS > cmdProvDS;
    if (ret) {
        cmdProvDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.CommandProviderDS");
        ret = (cmdProvDS.IsValid());
    }

    messGAM->PrepareNextState("Idle", "Idle");

    //first cycle
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }

    ReferenceT < PlcSimulatorMasterDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorMasterDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalDhGamState0;
    if (ret) {
        internalDhGamState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
    }

    //first cycle
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 0u);
    }

    if (ret) {
        uint32 ack0 = 1;
        cmdProvDS->SetCommand(2u, &ack0);
    }
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 4u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeMasterGAMTest::TestExecute_ErrorFromSending() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configErrorTests);
    ReferenceT<DoubleHandShakeMasterGAMTestGAM> dhGAM;
    if (ret) {
        dhGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.FC2PLC");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer dhInputBrokers;
    ReferenceContainer dhOutputBrokers;

    ReferenceT < MemoryMapInputBroker > dhBrokerIn;
    ReferenceT < MemoryMapInputBroker > dhBrokerIn1;
    ReferenceT < MemoryMapOutputBroker > dhBrokerOut;

    if (ret) {
        dhGAM->GetInputBrokers(dhInputBrokers);
        dhBrokerIn = dhInputBrokers.Get(0);
        ret = dhBrokerIn.IsValid();
    }
    if (ret) {
        dhBrokerIn1 = dhInputBrokers.Get(1);
        ret = dhBrokerIn1.IsValid();
    }
    if (ret) {
        dhGAM->GetOutputBrokers(dhOutputBrokers);
        dhBrokerOut = dhOutputBrokers.Get(0);
        ret = dhBrokerOut.IsValid();
    }

    ReferenceT<MessageGAM> messGAM;
    if (ret) {
        messGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.MessageGAM");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer messInputBrokers;
    ReferenceContainer messOutputBrokers;

    ReferenceT < MemoryMapInputBroker > messBrokerIn;
    ReferenceT < MemoryMapOutputBroker > messBrokerOut;

    if (ret) {
        messGAM->GetInputBrokers(messInputBrokers);
        messBrokerIn = dhInputBrokers.Get(0);
        ret = messBrokerIn.IsValid();
    }
    if (ret) {
        messGAM->GetOutputBrokers(messOutputBrokers);
        messBrokerOut = messOutputBrokers.Get(0);
        ret = messBrokerOut.IsValid();
    }

    ReferenceT < CommandProviderDS > cmdProvDS;
    if (ret) {
        cmdProvDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.CommandProviderDS");
        ret = (cmdProvDS.IsValid());
    }

    //first cycle
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }

    ReferenceT < PlcSimulatorMasterDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorMasterDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalDhGamState0;
    if (ret) {
        internalDhGamState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
    }
    if (ret) {
        uint32 command0 = 1;
        cmdProvDS->SetCommand(0u, &command0);
        uint32 state0 = 2;
        plcSimDS->SetState(0u, state0);
    }
    //first cycle... go in sending mode
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 1u);
    }

    if (ret) {
        //ack different than the command!
        uint32 ack0 = 2;
        cmdProvDS->SetCommand(2u, &ack0);
    }
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 4u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeMasterGAMTest::TestExecute_ErrorFromClear() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configErrorTests);
    ReferenceT<DoubleHandShakeMasterGAMTestGAM> dhGAM;
    if (ret) {
        dhGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.FC2PLC");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer dhInputBrokers;
    ReferenceContainer dhOutputBrokers;

    ReferenceT < MemoryMapInputBroker > dhBrokerIn;
    ReferenceT < MemoryMapInputBroker > dhBrokerIn1;
    ReferenceT < MemoryMapOutputBroker > dhBrokerOut;

    if (ret) {
        dhGAM->GetInputBrokers(dhInputBrokers);
        dhBrokerIn = dhInputBrokers.Get(0);
        ret = dhBrokerIn.IsValid();
    }
    if (ret) {
        dhBrokerIn1 = dhInputBrokers.Get(1);
        ret = dhBrokerIn1.IsValid();
    }
    if (ret) {
        dhGAM->GetOutputBrokers(dhOutputBrokers);
        dhBrokerOut = dhOutputBrokers.Get(0);
        ret = dhBrokerOut.IsValid();
    }

    ReferenceT<MessageGAM> messGAM;
    if (ret) {
        messGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.MessageGAM");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer messInputBrokers;
    ReferenceContainer messOutputBrokers;

    ReferenceT < MemoryMapInputBroker > messBrokerIn;
    ReferenceT < MemoryMapOutputBroker > messBrokerOut;

    if (ret) {
        messGAM->GetInputBrokers(messInputBrokers);
        messBrokerIn = dhInputBrokers.Get(0);
        ret = messBrokerIn.IsValid();
    }
    if (ret) {
        messGAM->GetOutputBrokers(messOutputBrokers);
        messBrokerOut = messOutputBrokers.Get(0);
        ret = messBrokerOut.IsValid();
    }

    ReferenceT < CommandProviderDS > cmdProvDS;
    if (ret) {
        cmdProvDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.CommandProviderDS");
        ret = (cmdProvDS.IsValid());
    }

    //first cycle
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }

    ReferenceT < PlcSimulatorMasterDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorMasterDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalDhGamState0;
    if (ret) {
        internalDhGamState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
    }
    if (ret) {
        uint32 command0 = 1;
        cmdProvDS->SetCommand(0u, &command0);
        uint32 state0 = 2;
        plcSimDS->SetState(0u, state0);
    }
    //first cycle... go in sending mode
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 1u);
    }

    //go in clear mode
    if (ret) {
        uint32 ack0 = 1;
        cmdProvDS->SetCommand(2u, &ack0);
    }
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 2u);
    }

    //change the ack
    if (ret) {
        uint32 ack0 = 0;
        cmdProvDS->SetCommand(2u, &ack0);
    }
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 4u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeMasterGAMTest::TestExecute_ErrorFromDone() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configErrorTests);
    ReferenceT<DoubleHandShakeMasterGAMTestGAM> dhGAM;
    if (ret) {
        dhGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.FC2PLC");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer dhInputBrokers;
    ReferenceContainer dhOutputBrokers;

    ReferenceT < MemoryMapInputBroker > dhBrokerIn;
    ReferenceT < MemoryMapInputBroker > dhBrokerIn1;
    ReferenceT < MemoryMapOutputBroker > dhBrokerOut;

    if (ret) {
        dhGAM->GetInputBrokers(dhInputBrokers);
        dhBrokerIn = dhInputBrokers.Get(0);
        ret = dhBrokerIn.IsValid();
    }
    if (ret) {
        dhBrokerIn1 = dhInputBrokers.Get(1);
        ret = dhBrokerIn1.IsValid();
    }
    if (ret) {
        dhGAM->GetOutputBrokers(dhOutputBrokers);
        dhBrokerOut = dhOutputBrokers.Get(0);
        ret = dhBrokerOut.IsValid();
    }

    ReferenceT<MessageGAM> messGAM;
    if (ret) {
        messGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.MessageGAM");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer messInputBrokers;
    ReferenceContainer messOutputBrokers;

    ReferenceT < MemoryMapInputBroker > messBrokerIn;
    ReferenceT < MemoryMapOutputBroker > messBrokerOut;

    if (ret) {
        messGAM->GetInputBrokers(messInputBrokers);
        messBrokerIn = dhInputBrokers.Get(0);
        ret = messBrokerIn.IsValid();
    }
    if (ret) {
        messGAM->GetOutputBrokers(messOutputBrokers);
        messBrokerOut = messOutputBrokers.Get(0);
        ret = messBrokerOut.IsValid();
    }

    ReferenceT < CommandProviderDS > cmdProvDS;
    if (ret) {
        cmdProvDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.CommandProviderDS");
        ret = (cmdProvDS.IsValid());
    }

    //first cycle
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }

    ReferenceT < PlcSimulatorMasterDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorMasterDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalDhGamState0;
    if (ret) {
        internalDhGamState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
    }
    if (ret) {
        uint32 command0 = 1;
        cmdProvDS->SetCommand(0u, &command0);
        uint32 state0 = 2;
        plcSimDS->SetState(0u, state0);
    }
    //first cycle... go in sending mode
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 1u);
    }

    //go in clear mode
    if (ret) {
        uint32 ack0 = 1;
        cmdProvDS->SetCommand(2u, &ack0);
    }
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 2u);
    }

    //a cycle more to go to done
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 3u);
    }

    //send something different than the command to go to error
    if (ret) {
        uint32 ack0 = 2;
        cmdProvDS->SetCommand(2u, &ack0);
    }
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 4u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

const char8 *configErrorTimeout = ""
        "$Application = {"
        "   Class = RealTimeApplication"
        "   +Functions = {"
        "       Class = ReferenceContainer"
        "       +FC2PLC = {"
        "           Class = DoubleHandShakeMasterGAMTestGAM"
        "           Timeout = 1000"
        "           InputSignals = {"
        "               CommandIn0 = {"
        "                   Alias = Command0"
        "                   Type = uint32"
        "                   DataSource = CommandProviderDS"
        "               }"
        "               CommandIn1 = {"
        "                   Alias = Command1"
        "                   Type = uint32"
        "                   DataSource = CommandProviderDS"
        "               }"
        "               AckIn0 = {"
        "                   Type = uint32"
        "                   DataSource = CommandProviderDS"
        "               }"
        "               AckIn1 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorMasterDS"
        "               }"
        "               ClearIn0 = {"
        "                   Type = uint32"
        "                   DataSource = DDB1"
        "               }"
        "               ClearIn1 = {"
        "                   Type = uint32"
        "                   DataSource = DDB1"
        "               }"
        "           }"
        "           OutputSignals = {"
        "               CommandOut0 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorMasterDS"
        "               }"
        "               CommandOut1 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorMasterDS"
        "               }"
        "               InternalState0 = {"
        "                   Type = uint8"
        "                   DataSource = DDB1"
        "               }"
        "               InternalState1 = {"
        "                   Type = uint8"
        "                   DataSource = DDB1"
        "               }"
        "           }"
        "       }"
        "       +MessageGAM = {"
        "           Class = MessageGAM"
        "            +Events = {"
        "                Class = ReferenceContainer"
        "                +Event1 = {"
        "                    Class = EventConditionTrigger"
        "                    EventTrigger = {"
        "                        CommandInAck0 = 1"
        "                        State0 = 2"
        "                    }"
        "                    +Message1 = {"
        "                        Class = Message"
        "                        Destination = \"Application.Data.CommandProviderD            Timeout = 1000S\""
        "                        Function = TrigFun1"
        "                        Mode = ExpectsReply"
        "                    }"
        "                }"
        "             }"
        "             InputSignals = {"
        "               CommandInAck0 = {"
        "                   Alias = AckIn0"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorMasterDS"
        "               }"
        "               CommandInAck1 = {"
        "                   Alias = AckIn1"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorMasterDS"
        "               }"
        "               State0 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorMasterDS"
        "               }"
        "             }"
        "             OutputSignals = {"
        "               ClearIn0 = {"
        "                   Type = uint32"
        "                   DataSource = DDB1"
        "               }"
        "               ClearIn1 = {"
        "                   Type = uint32"
        "                   DataSource = DDB1"
        "               }"
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
        "        +CommandProviderDS = {"
        "            Class = CommandProviderDS"
        "            Signals = {"
        "               Command0 = {"
        "                   Type = uint32"
        "                   Default = 0"
        "               }"
        "               Command1 = {"
        "                   Type = uint32"
        "                   Default = 0"
        "               }"
        "               AckIn0 = {"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +PlcSimulatorMasterDS = {"
        "            Class = PlcSimulatorMasterDS"
        "            DelayCount = 5"
        "            NumberOfInputs = 2"
        "            Signals = {"
        "               CommandOut0 = {"
        "                   Type = uint32"
        "               }"
        "               CommandOut1 = {"
        "                   Type = uint32"
        "               }"
        "               AckIn1 = {"
        "                   Type = uint32"
        "               }"
        "               State0 = {"
        "                   Type = uint32"
        "               }"
        "            }"
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
        "                    Functions = { FC2PLC, MessageGAM }"
        "                }"
        "            }"
        "         }"
        "        +Acquiring = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = { FC2PLC, MessageGAM }"
        "                }"
        "            }"
        "         }"
        "     }"
        "     +Scheduler = {"
        "         Class = GAMScheduler"
        "         TimingDataSource = Timings"
        "     }"
        "}";

bool DoubleHandshakeMasterGAMTest::TestExecute_ErrorFromSendingTimeout() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configErrorTimeout);
    ReferenceT<DoubleHandShakeMasterGAMTestGAM> dhGAM;
    if (ret) {
        dhGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.FC2PLC");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer dhInputBrokers;
    ReferenceContainer dhOutputBrokers;

    ReferenceT < MemoryMapInputBroker > dhBrokerIn;
    ReferenceT < MemoryMapInputBroker > dhBrokerIn1;
    ReferenceT < MemoryMapOutputBroker > dhBrokerOut;

    if (ret) {
        dhGAM->GetInputBrokers(dhInputBrokers);
        dhBrokerIn = dhInputBrokers.Get(0);
        ret = dhBrokerIn.IsValid();
    }
    if (ret) {
        dhBrokerIn1 = dhInputBrokers.Get(1);
        ret = dhBrokerIn1.IsValid();
    }
    if (ret) {
        dhGAM->GetOutputBrokers(dhOutputBrokers);
        dhBrokerOut = dhOutputBrokers.Get(0);
        ret = dhBrokerOut.IsValid();
    }

    ReferenceT<MessageGAM> messGAM;
    if (ret) {
        messGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.MessageGAM");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer messInputBrokers;
    ReferenceContainer messOutputBrokers;

    ReferenceT < MemoryMapInputBroker > messBrokerIn;
    ReferenceT < MemoryMapOutputBroker > messBrokerOut;

    if (ret) {
        messGAM->GetInputBrokers(messInputBrokers);
        messBrokerIn = dhInputBrokers.Get(0);
        ret = messBrokerIn.IsValid();
    }
    if (ret) {
        messGAM->GetOutputBrokers(messOutputBrokers);
        messBrokerOut = messOutputBrokers.Get(0);
        ret = messBrokerOut.IsValid();
    }

    ReferenceT < CommandProviderDS > cmdProvDS;
    if (ret) {
        cmdProvDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.CommandProviderDS");
        ret = (cmdProvDS.IsValid());
    }

    //first cycle
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }

    ReferenceT < PlcSimulatorMasterDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorMasterDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalDhGamState0;
    if (ret) {
        internalDhGamState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
    }
    if (ret) {
        uint32 command0 = 1;
        cmdProvDS->SetCommand(0u, &command0);
        uint32 state0 = 2;
        plcSimDS->SetState(0u, state0);
    }
    //first cycle... go in sending mode
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 1u);
        Sleep::MSec(1010u);
    }

    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 4u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeMasterGAMTest::TestExecute_ErrorFromClearTimeout() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configErrorTimeout);
    ReferenceT<DoubleHandShakeMasterGAMTestGAM> dhGAM;
    if (ret) {
        dhGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.FC2PLC");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer dhInputBrokers;
    ReferenceContainer dhOutputBrokers;

    ReferenceT < MemoryMapInputBroker > dhBrokerIn;
    ReferenceT < MemoryMapInputBroker > dhBrokerIn1;
    ReferenceT < MemoryMapOutputBroker > dhBrokerOut;

    if (ret) {
        dhGAM->GetInputBrokers(dhInputBrokers);
        dhBrokerIn = dhInputBrokers.Get(0);
        ret = dhBrokerIn.IsValid();
    }
    if (ret) {
        dhBrokerIn1 = dhInputBrokers.Get(1);
        ret = dhBrokerIn1.IsValid();
    }
    if (ret) {
        dhGAM->GetOutputBrokers(dhOutputBrokers);
        dhBrokerOut = dhOutputBrokers.Get(0);
        ret = dhBrokerOut.IsValid();
    }

    ReferenceT<MessageGAM> messGAM;
    if (ret) {
        messGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.MessageGAM");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer messInputBrokers;
    ReferenceContainer messOutputBrokers;

    ReferenceT < MemoryMapInputBroker > messBrokerIn;
    ReferenceT < MemoryMapOutputBroker > messBrokerOut;

    if (ret) {
        messGAM->GetInputBrokers(messInputBrokers);
        messBrokerIn = dhInputBrokers.Get(0);
        ret = messBrokerIn.IsValid();
    }
    if (ret) {
        messGAM->GetOutputBrokers(messOutputBrokers);
        messBrokerOut = messOutputBrokers.Get(0);
        ret = messBrokerOut.IsValid();
    }

    ReferenceT < CommandProviderDS > cmdProvDS;
    if (ret) {
        cmdProvDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.CommandProviderDS");
        ret = (cmdProvDS.IsValid());
    }

    //first cycle
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }

    ReferenceT < PlcSimulatorMasterDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorMasterDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalDhGamState0;
    if (ret) {
        internalDhGamState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
    }
    if (ret) {
        uint32 command0 = 1;
        cmdProvDS->SetCommand(0u, &command0);
        uint32 state0 = 2;
        plcSimDS->SetState(0u, state0);
    }
    //first cycle... go in sending mode
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 1u);
    }

    //go in clear mode
    if (ret) {
        uint32 ack0 = 1;
        cmdProvDS->SetCommand(2u, &ack0);
    }
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 2u);
        Sleep::MSec(1010u);
    }

    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 4u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeMasterGAMTest::TestExecute_ErrorFromDoneTimeout() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configErrorTimeout);
    ReferenceT<DoubleHandShakeMasterGAMTestGAM> dhGAM;
    if (ret) {
        dhGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.FC2PLC");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer dhInputBrokers;
    ReferenceContainer dhOutputBrokers;

    ReferenceT < MemoryMapInputBroker > dhBrokerIn;
    ReferenceT < MemoryMapInputBroker > dhBrokerIn1;
    ReferenceT < MemoryMapOutputBroker > dhBrokerOut;

    if (ret) {
        dhGAM->GetInputBrokers(dhInputBrokers);
        dhBrokerIn = dhInputBrokers.Get(0);
        ret = dhBrokerIn.IsValid();
    }
    if (ret) {
        dhBrokerIn1 = dhInputBrokers.Get(1);
        ret = dhBrokerIn1.IsValid();
    }
    if (ret) {
        dhGAM->GetOutputBrokers(dhOutputBrokers);
        dhBrokerOut = dhOutputBrokers.Get(0);
        ret = dhBrokerOut.IsValid();
    }

    ReferenceT<MessageGAM> messGAM;
    if (ret) {
        messGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.MessageGAM");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer messInputBrokers;
    ReferenceContainer messOutputBrokers;

    ReferenceT < MemoryMapInputBroker > messBrokerIn;
    ReferenceT < MemoryMapOutputBroker > messBrokerOut;

    if (ret) {
        messGAM->GetInputBrokers(messInputBrokers);
        messBrokerIn = dhInputBrokers.Get(0);
        ret = messBrokerIn.IsValid();
    }
    if (ret) {
        messGAM->GetOutputBrokers(messOutputBrokers);
        messBrokerOut = messOutputBrokers.Get(0);
        ret = messBrokerOut.IsValid();
    }

    ReferenceT < CommandProviderDS > cmdProvDS;
    if (ret) {
        cmdProvDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.CommandProviderDS");
        ret = (cmdProvDS.IsValid());
    }

    //first cycle
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }

    ReferenceT < PlcSimulatorMasterDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorMasterDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalDhGamState0;
    if (ret) {
        internalDhGamState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
    }
    if (ret) {
        uint32 command0 = 1;
        cmdProvDS->SetCommand(0u, &command0);
        uint32 state0 = 2;
        plcSimDS->SetState(0u, state0);
    }
    //first cycle... go in sending mode
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 1u);
    }

    //go in clear mode
    if (ret) {
        uint32 ack0 = 1;
        cmdProvDS->SetCommand(2u, &ack0);
    }
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 2u);
    }

    //a cycle more to go to done
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 3u);
    }

    //send something different than the command to go to error
    if (ret) {
        Sleep::MSec(1010u);
    }
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 4u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeMasterGAMTest::TestExecute_RecoverFromError() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configErrorTests);
    ReferenceT<DoubleHandShakeMasterGAMTestGAM> dhGAM;
    if (ret) {
        dhGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.FC2PLC");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer dhInputBrokers;
    ReferenceContainer dhOutputBrokers;

    ReferenceT < MemoryMapInputBroker > dhBrokerIn;
    ReferenceT < MemoryMapInputBroker > dhBrokerIn1;
    ReferenceT < MemoryMapOutputBroker > dhBrokerOut;

    if (ret) {
        dhGAM->GetInputBrokers(dhInputBrokers);
        dhBrokerIn = dhInputBrokers.Get(0);
        ret = dhBrokerIn.IsValid();
    }
    if (ret) {
        dhBrokerIn1 = dhInputBrokers.Get(1);
        ret = dhBrokerIn1.IsValid();
    }
    if (ret) {
        dhGAM->GetOutputBrokers(dhOutputBrokers);
        dhBrokerOut = dhOutputBrokers.Get(0);
        ret = dhBrokerOut.IsValid();
    }

    ReferenceT<MessageGAM> messGAM;
    if (ret) {
        messGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.MessageGAM");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer messInputBrokers;
    ReferenceContainer messOutputBrokers;

    ReferenceT < MemoryMapInputBroker > messBrokerIn;
    ReferenceT < MemoryMapOutputBroker > messBrokerOut;

    if (ret) {
        messGAM->GetInputBrokers(messInputBrokers);
        messBrokerIn = dhInputBrokers.Get(0);
        ret = messBrokerIn.IsValid();
    }
    if (ret) {
        messGAM->GetOutputBrokers(messOutputBrokers);
        messBrokerOut = messOutputBrokers.Get(0);
        ret = messBrokerOut.IsValid();
    }

    ReferenceT < CommandProviderDS > cmdProvDS;
    if (ret) {
        cmdProvDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.CommandProviderDS");
        ret = (cmdProvDS.IsValid());
    }

    ReferenceT < PlcSimulatorMasterDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorMasterDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalDhGamState0;
    if (ret) {
        internalDhGamState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
    }

    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 0u);
    }

    if (ret) {
        uint32 ack0 = 1;
        cmdProvDS->SetCommand(2u, &ack0);
    }
    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 4u);
    }

    //recover
    if (ret) {
        uint32 command0 = 1;
        cmdProvDS->SetCommand(0u, &command0);
    }

    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 4u);
    }

    //recover
    if (ret) {
        uint32 command0 = 0;
        cmdProvDS->SetCommand(0u, &command0);
    }

    if (ret) {
        for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
            inBroker->Execute();
        }
        dhGAM->Execute();
        for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
            outBroker->Execute();
        }

        for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
            ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
            inBroker->Execute();
        }
        messGAM->Execute();
        for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
            ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
            outBroker->Execute();
        }
    }
    if (ret) {
        ret = (*internalDhGamState0 == 0u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeMasterGAMTest::TestExecute_DoubleCycle() {
    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +FC2PLC = {"
            "           Class = DoubleHandShakeMasterGAMTestGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = CommandProviderDS"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = CommandProviderDS"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +MessageGAM = {"
            "           Class = MessageGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        CommandInAck0 = 1"
            "                        State0 = 2"
            "                    }"
            "                    +Message1 = {"
            "                        Class = Message"
            "                        Destination = \"Application.Data.CommandProviderDS\""
            "                        Function = TrigFun1"
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "             }"
            "             InputSignals = {"
            "               CommandInAck0 = {"
            "                   Alias = AckIn0"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandInAck1 = {"
            "                   Alias = AckIn1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "             }"
            "             OutputSignals = {"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
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
            "        +CommandProviderDS = {"
            "            Class = CommandProviderDS"
            "            Signals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   Default = 0"
            "               }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 5"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { FC2PLC, MessageGAM }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { FC2PLC, MessageGAM }"
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
    ReferenceT<DoubleHandShakeMasterGAMTestGAM> dhGAM;
    if (ret) {
        dhGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.FC2PLC");
        ret = dhGAM.IsValid();
    }

    ReferenceContainer dhInputBrokers;
    ReferenceContainer dhOutputBrokers;

    if (ret) {
        dhGAM->GetInputBrokers(dhInputBrokers);
        dhGAM->GetOutputBrokers(dhOutputBrokers);
    }

    ReferenceT<MessageGAM> messGAM;
    if (ret) {
        messGAM = ObjectRegistryDatabase::Instance()->Find("Application.Functions.MessageGAM");
        ret = messGAM.IsValid();
    }

    ReferenceContainer messInputBrokers;
    ReferenceContainer messOutputBrokers;

    if (ret) {
        messGAM->GetInputBrokers(messInputBrokers);
        messGAM->GetOutputBrokers(messOutputBrokers);
    }

    ReferenceT < CommandProviderDS > cmdProvDS;
    if (ret) {
        cmdProvDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.CommandProviderDS");
        ret = (cmdProvDS.IsValid());
    }

    ReferenceT < PlcSimulatorMasterDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorMasterDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalDhGamState0;
    if (ret) {
        internalDhGamState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
    }

    //first cycles
    for (uint32 k = 0u; (k < 2u) && (ret); k++) {
        if (ret) {
            uint32 command0 = k+1u;
            cmdProvDS->SetCommand(0u, &command0);
            uint32 state0 = 2;
            plcSimDS->SetState(0u, state0);
        }

        ret = false;
        for (uint32 j = 0u; j < 13u; j++) {
            for (uint32 i = 0u; i < dhInputBrokers.Size(); i++) {
                ReferenceT < MemoryMapInputBroker > inBroker = dhInputBrokers.Get(i);
                inBroker->Execute();
            }
            dhGAM->Execute();
            for (uint32 i = 0u; i < dhOutputBrokers.Size(); i++) {
                ReferenceT < MemoryMapOutputBroker > outBroker = dhOutputBrokers.Get(i);
                outBroker->Execute();
            }

            for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
                ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
                inBroker->Execute();
            }
            messGAM->Execute();
            for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
                ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
                outBroker->Execute();
            }
            //TODO: Was commented

            if (ret && (*internalDhGamState0 == 3u)) {
                uint32 command0 = 0;
                cmdProvDS->SetCommand(0u, &command0);
            }

            for (uint32 i = 0u; i < 10u; i++) {
                if (cmdProvDS->GetFlag() != 0x1u) {
                    Sleep::MSec(100u);
                }
                else {
                    ret = true;
                    break;
                }
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}
bool DoubleHandshakeMasterGAMTest::TestExecute_Interactive() {
    const char8 *config = ""
            "+EPICS_CA = {"
            "    Class = EPICS::EPICSCAClient"
            "    AutoStart = 1"
            "    +PV_COMMAND0 = {"
            "        Class = EPICS::EPICSPV"
            "        PVName = \"COMMAND0\""
            "        PVType = uint32"
            "        Event = {"
            "            PVValue = Message"
            "        }"
            "        +ChangeCommand0 = {"
            "            Class = Message"
            "            Destination = Application.Functions.CommandProvider"
            "            Function = SetOutput"
            "            +Parameters = {"
            "                Class = ConfigurationDatabase"
            "                SignalName = Command0"
            "                SignalValue = \"$PVValue\""
            "            }"
            "        }"
            "    }"
            "    +PV_COMMAND1 = {"
            "        Class = EPICS::EPICSPV"
            "        PVName = \"COMMAND1\""
            "        PVType = uint32"
            "        Event = {"
            "            PVValue = Message"
            "        }"
            "        +ChangeCommand1 = {"
            "            Class = Message"
            "            Destination = Application.Functions.CommandProvider"
            "            Function = SetOutput"
            "            +Parameters = {"
            "                Class = ConfigurationDatabase"
            "                SignalName = Command1"
            "                SignalValue = \"$PVValue\""
            "            }"
            "        }"
            "    }"
            "    +PV_STATE = {"
            "        Class = EPICS::EPICSPV"
            "        PVName = \"STATE\""
            "        PVType = uint32"
            "        Event = {"
            "            PVValue = Message"
            "        }"
            "        +ChangeState = {"
            "            Class = Message"
            "            Destination = Application.Data.PlcSimulatorMasterDS"
            "            Function = SetState"
            "            +Parameters = {"
            "                Class = ConfigurationDatabase"
            "                param1 = 0"
            "                param2 = \"$PVValue\""
            "            }"
            "        }"
            "    }"
            "}"
            "+StateMachine = {"
            "    Class = StateMachine"
            "    +IDLE = {"
            "        Class = ReferenceContainer"
            "        +ACQUIRE = {"
            "            Class = StateMachineEvent"
            "            NextState = \"ACQUIRING\""
            "            NextStateError = \"ACQUIRING\""
            "            Timeout = 0"
            "            +PrepareChangeToAcquiringMsg = {"
            "                Class = Message"
            "                Destination = Application"
            "                Mode = ExpectsReply"
            "                Function = PrepareNextState"
            "                +Parameters = {"
            "                    Class = ConfigurationDatabase"
            "                    param1 = Acquiring"
            "                }"
            "             }"
            "             +StopCurrentStateExecutionMsg = {"
            "                 Class = Message"
            "                 Destination = Application"
            "                 Function = StopCurrentStateExecution"
            "                 Mode = ExpectsReply"
            "             }"
            "             +StartNextStateExecutionMsg = {"
            "                 Class = Message"
            "                 Destination = Application"
            "                 Function = StartNextStateExecution"
            "                 Mode = ExpectsReply"
            "             }"
            "         }"
            "    }"
            "    +ACQUIRING = {"
            "        Class = ReferenceContainer"
            "        +STOP = {"
            "            Class = StateMachineEvent"
            "            NextState = \"IDLE\""
            "            NextStateError = \"IDLE\""
            "            Timeout = 0"
            "            +PrepareChangeToAcquiringMsg = {"
            "                Class = Message"
            "                Destination = Application"
            "                Mode = ExpectsReply"
            "                Function = PrepareNextState"
            "                +Parameters = {"
            "                    Class = ConfigurationDatabase"
            "                    param1 = Idle"
            "                }"
            "             }"
            "             +StopCurrentStateExecutionMsg = {"
            "                 Class = Message"
            "                 Destination = Application"
            "                 Function = StopCurrentStateExecution"
            "                 Mode = ExpectsReply"
            "             }"
            "             +StartNextStateExecutionMsg = {"
            "                 Class = Message"
            "                 Destination = Application"
            "                 Function = StartNextStateExecution"
            "                 Mode = ExpectsReply"
            "             }"
            "        }"
            "    }"
            "}"
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +MessageGAM = {"
            "           Class = MessageGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        CommandInAck0 = 1"
            "                        State0 = 2"
            "                    }"
            "                    +Message1 = {"
            "                        Class = Message"
            "                        Destination = StateMachine"
            "                        Function = ACQUIRE"
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        CommandInAck0 = 2"
            "                        State0 = 3"
            "                    }"
            "                    +Message2 = {"
            "                        Class = Message"
            "                        Destination = StateMachine"
            "                        Function = STOP"
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        CommandInAck1 = 3"
            "                        State0 = 3"
            "                    }"
            "                    +Message3 = {"
            "                        Class = Message"
            "                        Destination = StateMachine"
            "                        Function = ACQUIRE"
            "                        Mode = ExpectsRIdleeply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "               CommandInAck0 = {"
            "                   Alias = AckIn0"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandInAck1 = {"
            "                   Alias = AckIn1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "             }"
            "             OutputSignals = {"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC, MessageGAM }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC, MessageGAM }"
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

    ReferenceT < RealTimeApplication > app = ObjectRegistryDatabase::Instance()->Find("Application");
    if (ret) {
        ret = app->PrepareNextState("Idle");
    }
    if (ret) {
        ret = app->StartNextStateExecution();
    }

    if (ret) {
        while (1) {
            Sleep::Sec(1u);
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeMasterGAMTest::TestExecute_InteractiveManual() {
    const char8 *config = ""
            "+EPICS_CA = {"
            "    Class = EPICS::EPICSCAClient"
            "    AutoStart = 1"
            "    +PV_COMMAND0 = {"
            "        Class = EPICS::EPICSPV"
            "        PVName = \"COMMAND0\""
            "        PVType = uint32"
            "        Event = {"
            "            PVValue = Message"
            "        }"
            "        +ChangeCommand0 = {"
            "            Class = Message"
            "            Destination = Application.Functions.CommandProvider"
            "            Function = SetOutput"
            "            +Parameters = {"
            "                Class = ConfigurationDatabase"
            "                SignalName = Command0"
            "                SignalValue = \"$PVValue\""
            "            }"
            "        }"
            "    }"
            "    +PV_COMMAND1 = {"
            "        Class = EPICS::EPICSPV"
            "        PVName = \"COMMAND1\""
            "        PVType = uint32"
            "        Event = {"
            "            PVValue = Message"
            "        }"
            "        +ChangeCommand1 = {"
            "            Class = Message"
            "            Destination = Application.Functions.CommandProvider"
            "            Function = SetOutput"
            "            +Parameters = {"
            "                Class = ConfigurationDatabase"
            "                SignalName = Command1"
            "                SignalValue = \"$PVValue\""
            "            }"
            "        }"
            "    }"
            "    +PV_ACK0 = {"
            "        Class = EPICS::EPICSPV"
            "        PVName = \"ACK0\""
            "        PVType = uint32"
            "        Event = {"
            "            PVValue = Message"
            "        }"
            "        +ChangeCommand0 = {"
            "            Class = Message"
            "            Destination = Application.Data.PlcSimulatorMasterDS"
            "            Function = SetSignal"
            "            +Parameters = {"
            "                Class = ConfigurationDatabase"
            "                param1 = 2"
            "                param2 = \"$PVValue\""
            "            }"
            "        }"
            "    }"
            "    +PV_ACK1 = {"
            "        Class = EPICS::EPICSPV"
            "        PVName = \"ACK1\""
            "        PVType = uint32"
            "        Event = {"
            "            PVValue = Message"
            "        }"
            "        +ChangeCommand1 = {"
            "            Class = Message"
            "            Destination = Application.Data.PlcSimulatorMasterDS"
            "            Function = SetSignal"
            "            +Parameters = {"
            "                Class = ConfigurationDatabase"
            "                param1 = 3"
            "                param2 = \"$PVValue\""
            "            }"
            "        }"
            "    }"
            "    +PV_STATE = {"
            "        Class = EPICS::EPICSPV"
            "        PVName = \"STATE\""
            "        PVType = uint32"
            "        Event = {"
            "            PVValue = Message"
            "        }"
            "        +ChangeState = {"
            "            Class = Message"
            "            Destination = Application.Data.PlcSimulatorMasterDS"
            "            Function = SetState"
            "            +Parameters = {"
            "                Class = ConfigurationDatabase"
            "                param1 = 0"
            "                param2 = \"$PVValue\""
            "            }"
            "        }"
            "    }"
            "}"
            "+StateMachine = {"
            "    Class = StateMachine"
            "    +IDLE = {"
            "        Class = ReferenceContainer"
            "        +ACQUIRE = {"
            "            Class = StateMachineEvent"
            "            NextState = \"ACQUIRING\""
            "            NextStateError = \"ACQUIRING\""
            "            Timeout = 0"
            "            +PrepareChangeToAcquiringMsg = {"
            "                Class = Message"
            "                Destination = Application"
            "                Mode = ExpectsReply"
            "                Function = PrepareNextState"
            "                +Parameters = {"
            "                    Class = ConfigurationDatabase"
            "                    param1 = Acquiring"
            "                }"
            "             }"
            "             +StopCurrentStateExecutionMsg = {"
            "                 Class = Message"
            "                 Destination = Application"
            "                 Function = StopCurrentStateExecution"
            "                 Mode = ExpectsReply"
            "             }"
            "             +StartNextStateExecutionMsg = {"
            "                 Class = Message"
            "                 Destination = Application"
            "                 Function = StartNextStateExecution"
            "                 Mode = ExpectsReply"
            "             }"
            "         }"
            "    }"
            "    +ACQUIRING = {"
            "        Class = ReferenceContainer"
            "        +STOP = {"
            "            Class = StateMachineEvent"
            "            NextState = \"IDLE\""
            "            NextStateError = \"IDLE\""
            "            Timeout = 0"
            "            +PrepareChangeToAcquiringMsg = {"
            "                Class = Message"
            "                Destination = Application"
            "                Mode = ExpectsReply"
            "                Function = PrepareNextState"
            "                +Parameters = {"
            "                    Class = ConfigurationDatabase"
            "                    param1 = Idle"
            "                }"
            "             }"
            "             +StopCurrentStateExecutionMsg = {"
            "                 Class = Message"
            "                 Destination = Application"
            "                 Function = StopCurrentStateExecution"
            "                 Mode = ExpectsReply"
            "             }"
            "             +StartNextStateExecutionMsg = {"
            "                 Class = Message"
            "                 Destination = Application"
            "                 Function = StartNextStateExecution"
            "                 Mode = ExpectsReply"
            "             }"
            "        }"
            "    }"
            "}"
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +Sync = {"
            "           Class = IOGAM"
            "           InputSignals = {"
            "               Time = {"
            "                   Type = uint32"
            "                   DataSource = LinuxTimer"
            "                   Frequency = 10"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               TimeDDB = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +CommandProvider = {"
            "           Class = ConstantGAM"
            "           OutputSignals = {"
            "               Command0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "               Command1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "                   Default = 0"
            "               }"
            "           }"
            "       }"
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Alias = Command0"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               CommandIn1 = {"
            "                   Alias = Command1"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               AckIn0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
            "       +MessageGAM = {"
            "           Class = MessageGAM"
            "            +Events = {"
            "                Class = ReferenceContainer"
            "                +Event1 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        CommandInAck0 = 1"
            "                        State0 = 2"
            "                    }"
            "                    +Message1 = {"
            "                        Class = Message"
            "                        Destination = StateMachine"
            "                        Function = ACQUIRE"
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event2 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        CommandInAck0 = 2"
            "                        State0 = 3"
            "                    }"
            "                    +Message2 = {"
            "                        Class = Message"
            "                        Destination = StateMachine"
            "                        Function = STOP"
            "                        Mode = ExpectsReply"
            "                    }"
            "                }"
            "                +Event3 = {"
            "                    Class = EventConditionTrigger"
            "                    EventTrigger = {"
            "                        CommandInAck1 = 3"
            "                        State0 = 3"
            "                    }"
            "                    +Message3 = {"
            "                        Class = Message"
            "                        Destination = StateMachine"
            "                        Function = ACQUIRE"
            "                        Mode = ExpectsRIdleeply"
            "                    }"
            "                 }"
            "             }"
            "             InputSignals = {"
            "               CommandInAck0 = {"
            "                   Alias = AckIn0"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               CommandInAck1 = {"
            "                   Alias = AckIn1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorMasterDS"
            "               }"
            "             }"
            "             OutputSignals = {"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
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
            "        +LinuxTimer = {"
            "            Class = LinuxTimer"
            "            SleepNature = \"Default\""
            "            Signals = {"
            "                Counter = {"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +PlcSimulatorMasterDS = {"
            "            Class = PlcSimulatorMasterDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Manual = 1"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn0 = {"
            "                   Type = uint32"
            "               }"
            "               AckIn1 = {"
            "                   Type = uint32"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "               }"
            "            }"
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
            "                    Functions = { Sync, CommandProvider, FC2PLC, MessageGAM }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, CommandProvider, FC2PLC, MessageGAM }"
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

    ReferenceT < RealTimeApplication > app = ObjectRegistryDatabase::Instance()->Find("Application");
    if (ret) {
        ret = app->PrepareNextState("Idle");
    }
    if (ret) {
        ret = app->StartNextStateExecution();
    }

    if (ret) {
        while (1) {
            Sleep::Sec(1u);
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeMasterGAMTest::TestInitialise() {
    return TestExecute();
}
