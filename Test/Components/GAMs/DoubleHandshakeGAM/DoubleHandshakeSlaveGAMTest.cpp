/**
 * @file DoubleHandshakeSlaveGAMTest.cpp
 * @brief Source file for class DoubleHandshakeSlaveGAMTest
 * @date 13/08/2019
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
 * the class DoubleHandshakeSlaveGAMTest (public, protected, and private). Be aware that some 
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
#include "DoubleHandshakeSlaveGAMTest.h"
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

class DoubleHandShakeSlaveGAMTestGAM: public DoubleHandshakeSlaveGAM {
public:
    CLASS_REGISTER_DECLARATION()

    DoubleHandShakeSlaveGAMTestGAM();

    virtual ~DoubleHandShakeSlaveGAMTestGAM();

    void *GetInputMemoryX();
    void *GetOutputMemoryX();
};

DoubleHandShakeSlaveGAMTestGAM::DoubleHandShakeSlaveGAMTestGAM() {

}

DoubleHandShakeSlaveGAMTestGAM::~DoubleHandShakeSlaveGAMTestGAM() {

}

void *DoubleHandShakeSlaveGAMTestGAM::GetInputMemoryX() {
    return GetInputSignalsMemory();
}
void *DoubleHandShakeSlaveGAMTestGAM::GetOutputMemoryX() {
    return GetOutputSignalsMemory();
}

CLASS_REGISTER(DoubleHandShakeSlaveGAMTestGAM, "1.0")

class PlcSimulatorSlaveDS: public MemoryDataSourceI, public MessageI {

public:
    CLASS_REGISTER_DECLARATION()
    PlcSimulatorSlaveDS();

    virtual ~PlcSimulatorSlaveDS();

    virtual bool Initialise(StructuredDataI & data);

    const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);
    virtual bool Synchronise();

    ErrorManagement::ErrorType SetSignal(uint32 index,
            uint32 value);

    ErrorManagement::ErrorType TrigFun1();
    ErrorManagement::ErrorType TrigFun2();
    ErrorManagement::ErrorType TrigFun3();

    void ResetFlag();
    uint8 GetFlag();
    void SetSleepTime(uint32 sleepTimeIn);

private:
    uint32 numberOfInputs;
    uint8 callFunFlag;
    uint32 sleepTimeMSec;

};
CLASS_REGISTER(PlcSimulatorSlaveDS, "1.0")

CLASS_METHOD_REGISTER(PlcSimulatorSlaveDS, SetSignal)
CLASS_METHOD_REGISTER(PlcSimulatorSlaveDS, TrigFun1)
CLASS_METHOD_REGISTER(PlcSimulatorSlaveDS, TrigFun2)
CLASS_METHOD_REGISTER(PlcSimulatorSlaveDS, TrigFun3)

PlcSimulatorSlaveDS::PlcSimulatorSlaveDS() {
    ReferenceT < RegisteredMethodsMessageFilter > filter = ReferenceT < RegisteredMethodsMessageFilter > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }

    numberOfInputs=0u;
    callFunFlag=0u;
    sleepTimeMSec=0u;
}

PlcSimulatorSlaveDS::~PlcSimulatorSlaveDS() {
}

bool PlcSimulatorSlaveDS::Initialise(StructuredDataI & data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        if (!data.Read("NumberOfInputs", numberOfInputs)) {
            numberOfInputs = 0u;
        }
    }
    return ret;
}

const char8 *PlcSimulatorSlaveDS::GetBrokerName(StructuredDataI &data,
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

bool PlcSimulatorSlaveDS::PrepareNextState(const char8 * const currentStateName,
                                           const char8 * const nextStateName) {
    return true;
}
bool PlcSimulatorSlaveDS::Synchronise() {
    return true;
}

ErrorManagement::ErrorType PlcSimulatorSlaveDS::SetSignal(uint32 index,
                                                          uint32 value) {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called SetSignal");

    uint32 *signalPtr = NULL;
    GetSignalMemoryBuffer(index, 0u, (void*&) signalPtr);

    *signalPtr = value;
    return ret;
}

ErrorManagement::ErrorType PlcSimulatorSlaveDS::TrigFun1() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function1");
    callFunFlag |= 1;
    if (sleepTimeMSec > 0) {
        Sleep::MSec (sleepTimeMSec);
    }
    return ret;
}

ErrorManagement::ErrorType PlcSimulatorSlaveDS::TrigFun2() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function2");
    callFunFlag |= (1 << 1);
    if (sleepTimeMSec > 0) {
        Sleep::MSec (sleepTimeMSec);
    }
    return ret;
}

ErrorManagement::ErrorType PlcSimulatorSlaveDS::TrigFun3() {
    ErrorManagement::ErrorType ret;
    REPORT_ERROR(ErrorManagement::Information, "Called Function3");
    callFunFlag |= (1 << 2);
    if (sleepTimeMSec > 0) {
        Sleep::MSec (sleepTimeMSec);
    }
    return ret;
}

void PlcSimulatorSlaveDS::ResetFlag() {
    callFunFlag = 0;
}

uint8 PlcSimulatorSlaveDS::GetFlag() {
    return callFunFlag;
}

void PlcSimulatorSlaveDS::SetSleepTime(uint32 sleepTimeIn) {
    sleepTimeMSec = sleepTimeIn;
}

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
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

DoubleHandshakeSlaveGAMTest::DoubleHandshakeSlaveGAMTest() {
}

DoubleHandshakeSlaveGAMTest::~DoubleHandshakeSlaveGAMTest() {
}

bool DoubleHandshakeSlaveGAMTest::TestConstructor() {
    DoubleHandshakeSlaveGAM *test = new DoubleHandshakeSlaveGAM;
    bool ret = (test != NULL);
    return ret;
}

bool DoubleHandshakeSlaveGAMTest::TestSetup() {
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
            "           Class = DoubleHandshakeGAM::DoubleHandshakeSlaveGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               CommandIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "               AckOut0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               AckOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "        +PlcSimulatorSlaveDS = {"
            "            Class = PlcSimulatorSlaveDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut0 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut1 = {"
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

bool DoubleHandshakeSlaveGAMTest::TestSetup_UselessSignal() {
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
            "           Class = DoubleHandshakeGAM::DoubleHandshakeSlaveGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               CommandIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               Ciao = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "               AckOut0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               AckOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "        +PlcSimulatorSlaveDS = {"
            "            Class = PlcSimulatorSlaveDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut0 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut1 = {"
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

bool DoubleHandshakeSlaveGAMTest::TestSetup_NumberOfElements() {
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
            "           Class = DoubleHandshakeGAM::DoubleHandshakeSlaveGAM"
            "           InputSignals = {"
            "               CommandIn = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "                   NumberOfElements = 2"
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
            "               AckOut = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   NumberOfElements = 2"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "        +PlcSimulatorSlaveDS = {"
            "            Class = PlcSimulatorSlaveDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandIn = {"
            "                   Type = uint32"
            "               }"
            "               AckOut = {"
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

bool DoubleHandshakeSlaveGAMTest::TestSetup_FalseNCommansDiffThanNAcks() {
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
            "           Class = DoubleHandshakeGAM::DoubleHandshakeSlaveGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               CommandIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "               AckOut0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               AckOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               AckOut2 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "        +PlcSimulatorSlaveDS = {"
            "            Class = PlcSimulatorSlaveDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut0 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut1 = {"
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

bool DoubleHandshakeSlaveGAMTest::TestSetup_FalseNCommansDiffThanNClears() {
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
            "           Class = DoubleHandshakeGAM::DoubleHandshakeSlaveGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               CommandIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               AckOut0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               AckOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "        +PlcSimulatorSlaveDS = {"
            "            Class = PlcSimulatorSlaveDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut0 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut1 = {"
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

bool DoubleHandshakeSlaveGAMTest::TestSetup_FalseNCommansDiffThanNStatuses() {
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
            "           Class = DoubleHandshakeGAM::DoubleHandshakeSlaveGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               CommandIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "               AckOut0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               AckOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState1 = {"
            "                   Type = uint8"
            "                   DataSource = DDB1"
            "               }"
            "               InternalState2 = {"
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
            "        +PlcSimulatorSlaveDS = {"
            "            Class = PlcSimulatorSlaveDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut0 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut1 = {"
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

bool DoubleHandshakeSlaveGAMTest::TestSetup_FalseClearTrigsWrongType() {
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
            "               ClearIn0 = {"
            "                   Type = uint16"
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
            "           Class = DoubleHandshakeGAM::DoubleHandshakeSlaveGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               CommandIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint16"
            "                   DataSource = DDB1"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               AckOut0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               AckOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "        +PlcSimulatorSlaveDS = {"
            "            Class = PlcSimulatorSlaveDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut0 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut1 = {"
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

bool DoubleHandshakeSlaveGAMTest::TestSetup_FalseInternalStateWrongType() {
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
            "           Class = DoubleHandshakeGAM::DoubleHandshakeSlaveGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               CommandIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "               AckOut0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               AckOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               InternalState0 = {"
            "                   Type = uint64"
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
            "        +PlcSimulatorSlaveDS = {"
            "            Class = PlcSimulatorSlaveDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut0 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut1 = {"
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

bool DoubleHandshakeSlaveGAMTest::TestSetup_FalseCommandAndAckMismatch() {
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
            "           Class = DoubleHandshakeGAM::DoubleHandshakeSlaveGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               ClearIn0 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "               CommandIn1 = {"
            "                   Type = uint8"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               ClearIn1 = {"
            "                   Type = uint32"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               AckOut0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               AckOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "        +PlcSimulatorSlaveDS = {"
            "            Class = PlcSimulatorSlaveDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandOut0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandOut1 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut0 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut1 = {"
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

const char8 *configExecute = ""
        "$Application = {"
        "   Class = RealTimeApplication"
        "   +Functions = {"
        "       Class = ReferenceContainer"
        "       +FC2PLC = {"
        "           Class = DoubleHandShakeSlaveGAMTestGAM"
        "           InputSignals = {"
        "               CommandIn0 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
        "               }"
        "               CommandIn1 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
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
        "               AckOut0 = {"
        "                   Trigger = 1"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
        "               }"
        "               AckOut1 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
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
        "                        CommandIn0 = 1"
        "                        State0 = 2"
        "                    }"
        "                    +Message1 = {"
        "                        Class = Message"
        "                        Destination = Application.Data.PlcSimulatorSlaveDS"
        "                        Function = TrigFun1"
        "                        Mode = ExpectsReply"
        "                    }"
        "                }"
        "                +Event2 = {"
        "                    Class = EventConditionTrigger"
        "                    EventTrigger = {"
        "                        CommandIn0 = 2"
        "                        State0 = 3"
        "                    }"
        "                    +Message2 = {"
        "                        Class = Message"
        "                        Destination = Application.Data.PlcSimulatorSlaveDS"
        "                        Function = TrigFun2"
        "                        Mode = ExpectsReply"
        "                    }"
        "                }"
        "                +Event3 = {"
        "                    Class = EventConditionTrigger"
        "                    EventTrigger = {"
        "                        CommandIn1 = 3"
        "                        State0 = 3"
        "                    }"
        "                    +Message3 = {"
        "                        Class = Message"
        "                        Destination = Application.Data.PlcSimulatorSlaveDS"
        "                        Function = TrigFun3"
        "                        Mode = ExpectsRIdleeply"
        "                    }"
        "                 }"
        "             }"
        "             InputSignals = {"
        "               CommandIn0 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
        "               }"
        "               CommandIn1 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
        "               }"
        "               State0 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
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
        "        +PlcSimulatorSlaveDS = {"
        "            Class = PlcSimulatorSlaveDS"
        "            DelayCount = 10"
        "            NumberOfInputs = 2"
        "            Signals = {"
        "               CommandIn0 = {"
        "                   Type = uint32"
        "               }"
        "               CommandIn1 = {"
        "                   Type = uint32"
        "               }"
        "               AckOut0 = {"
        "                   Type = uint32"
        "               }"
        "               AckOut1 = {"
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

bool DoubleHandshakeSlaveGAMTest::TestExecute() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configExecute);
    ReferenceT<DoubleHandShakeSlaveGAMTestGAM> dhGAM;
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

    ReferenceT < PlcSimulatorSlaveDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorSlaveDS");
        ret = (plcSimDS.IsValid());
    }

    //Warm up
    //for (uint32 i = 0u; i < messInputBrokers.Size(); i++) {
    //    ReferenceT < MemoryMapInputBroker > inBroker = messInputBrokers.Get(i);
    //    inBroker->Execute();
    //}
    //messGAM->Execute();
    //for (uint32 i = 0u; i < messOutputBrokers.Size(); i++) {
    //    ReferenceT < MemoryMapOutputBroker > outBroker = messOutputBrokers.Get(i);
    //    outBroker->Execute();
    //}
    //End of warm up

    messGAM->PrepareNextState("Idle", "Idle");

    if (ret) {
        uint32 command0 = 1;
        plcSimDS->SetSignal(0u, command0);
        uint32 state0 = 2;
        plcSimDS->SetSignal(4u, state0);
    }

    uint8 *internalState0;
    if (ret) {
        //get the internal state
        internalState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
        ret = (*internalState0 == 0u);
    }

    ret = false;
    //first cycles
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
        if (ret && (*internalState0==2u)) {
            uint32 command0 = 0;
            plcSimDS->SetSignal(0u, command0);
        }
        else {
            for (uint32 i = 0u; i < 10u; i++) {
                if (plcSimDS->GetFlag() != 0x1u) {
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

bool DoubleHandshakeSlaveGAMTest::TestExecute_ErrorFromSending() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configExecute);
    ReferenceT<DoubleHandShakeSlaveGAMTestGAM> dhGAM;
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

    ReferenceT < PlcSimulatorSlaveDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorSlaveDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalState0;
    if (ret) {
        //get the internal state
        internalState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
        ret = (*internalState0 == 0u);
    }

    if (ret) {
        uint32 command0 = 1;
        plcSimDS->SetSignal(0u, command0);
        uint32 state0 = 2;
        plcSimDS->SetSignal(4u, state0);
    }

    messGAM->PrepareNextState("Idle", "Idle");

    //first cycles
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
        ret = (*internalState0 == 1u);
    }

    if (ret) {
        uint32 command0 = 2;
        plcSimDS->SetSignal(0u, command0);
        uint32 state0 = 2;
        plcSimDS->SetSignal(4u, state0);
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
        ret = (*internalState0 == 3u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool DoubleHandshakeSlaveGAMTest::TestExecute_ErrorFromDone() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configExecute);
    ReferenceT<DoubleHandShakeSlaveGAMTestGAM> dhGAM;
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

    ReferenceT < PlcSimulatorSlaveDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorSlaveDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalState0;
    if (ret) {
        //get the internal state
        internalState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
        ret = (*internalState0 == 0u);
    }

    if (ret) {
        uint32 command0 = 1;
        plcSimDS->SetSignal(0u, command0);
        uint32 state0 = 2;
        plcSimDS->SetSignal(4u, state0);
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
        ret = (*internalState0 == 1u);
    }

    //run until the message is sent
    const uint32 maxCycles = 20u;
    if (ret) {
        for (uint32 i = 0u; i < maxCycles; i++) {
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
            if ((*internalState0) == 2u) {
                break;
            }
        }
    }

    if (ret) {
        ret = ((*internalState0) == 2u);
    }

    if (ret) {
        uint32 command0 = 2;
        plcSimDS->SetSignal(0u, command0);
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
        ret = (*internalState0 == 3u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

const char8 *configTimeout = ""
        "$Application = {"
        "   Class = RealTimeApplication"
        "   +Functions = {"
        "       Class = ReferenceContainer"
        "       +FC2PLC = {"
        "           Class = DoubleHandShakeSlaveGAMTestGAM"
        "           Timeout = 1000"
        "           InputSignals = {"
        "               CommandIn0 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
        "               }"
        "               CommandIn1 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
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
        "               AckOut0 = {"
        "                   Trigger = 1"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
        "               }"
        "               AckOut1 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
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
        "                        CommandIn0 = 1"
        "                        State0 = 2"
        "                    }"
        "                    +Message1 = {"
        "                        Class = Message"
        "                        Destination = Application.Data.PlcSimulatorSlaveDS"
        "                        Function = TrigFun1"
        "                        Mode = ExpectsReply"
        "                    }"
        "                }"
        "                +Event2 = {"
        "                    Class = EventConditionTrigger"
        "                    EventTrigger = {"
        "                        CommandIn0 = 2"
        "                        State0 = 3"
        "                    }"
        "                    +Message2 = {"
        "                        Class = Message"
        "                        Destination = Application.Data.PlcSimulatorSlaveDS"
        "                        Function = TrigFun2"
        "                        Mode = ExpectsReply"
        "                    }"
        "                }"
        "                +Event3 = {"
        "                    Class = EventConditionTrigger"
        "                    EventTrigger = {"
        "                        CommandIn1 = 3"
        "                        State0 = 3"
        "                    }"
        "                    +Message3 = {"
        "                        Class = Message"
        "                        Destination = Application.Data.PlcSimulatorSlaveDS"
        "                        Function = TrigFun3"
        "                        Mode = ExpectsRIdleeply"
        "                    }"
        "                 }"
        "             }"
        "             InputSignals = {"
        "               CommandIn0 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
        "               }"
        "               CommandIn1 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
        "               }"
        "               State0 = {"
        "                   Type = uint32"
        "                   DataSource = PlcSimulatorSlaveDS"
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
        "        +PlcSimulatorSlaveDS = {"
        "            Class = PlcSimulatorSlaveDS"
        "            DelayCount = 10"
        "            NumberOfInputs = 2"
        "            Signals = {"
        "               CommandIn0 = {"
        "                   Type = uint32"
        "               }"
        "               CommandIn1 = {"
        "                   Type = uint32"
        "               }"
        "               AckOut0 = {"
        "                   Type = uint32"
        "               }"
        "               AckOut1 = {"
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
bool DoubleHandshakeSlaveGAMTest::TestExecute_ErrorFromSendingTimeout() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configTimeout);
    ReferenceT<DoubleHandShakeSlaveGAMTestGAM> dhGAM;
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

    ReferenceT < PlcSimulatorSlaveDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorSlaveDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalState0;
    if (ret) {
        //get the internal state
        internalState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
        ret = (*internalState0 == 0u);
    }

    if (ret) {
        uint32 command0 = 1;
        plcSimDS->SetSignal(0u, command0);
        uint32 state0 = 2;
        plcSimDS->SetSignal(4u, state0);
    }

    //first cycles
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
        ret = (*internalState0 == 1u);
    }

    Sleep::MSec(1010u);

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
        ret = (*internalState0 == 3u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeSlaveGAMTest::TestExecute_ErrorFromDoneTimeout() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configTimeout);
    ReferenceT<DoubleHandShakeSlaveGAMTestGAM> dhGAM;
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

    ReferenceT < PlcSimulatorSlaveDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorSlaveDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalState0;
    if (ret) {
        //get the internal state
        internalState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
        ret = (*internalState0 == 0u);
    }

    if (ret) {
        uint32 command0 = 1;
        plcSimDS->SetSignal(0u, command0);
        uint32 state0 = 2;
        plcSimDS->SetSignal(4u, state0);
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
        ret = (*internalState0 == 1u);
    }

    //run until the message is sent
    const uint32 maxCycles = 20u;
    if (ret) {
        for (uint32 i = 0u; i < maxCycles; i++) {
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
            if ((*internalState0) == 2u) {
                break;
            }
        }
    }

    if (ret) {
        ret = ((*internalState0) == 2u);
    }

    Sleep::MSec(1010u);

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
        ret = (*internalState0 == 3u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeSlaveGAMTest::TestExecute_RecoverFromError() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(configExecute);
    ReferenceT<DoubleHandShakeSlaveGAMTestGAM> dhGAM;
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

    ReferenceT < PlcSimulatorSlaveDS > plcSimDS;
    if (ret) {
        plcSimDS = ObjectRegistryDatabase::Instance()->Find("Application.Data.PlcSimulatorSlaveDS");
        ret = (plcSimDS.IsValid());
    }

    uint8 *internalState0;
    if (ret) {
        //get the internal state
        internalState0 = (((uint8*) dhGAM->GetOutputMemoryX()) + 8u);
        ret = (*internalState0 == 0u);
    }

    messGAM->PrepareNextState("Idle", "Idle");

    if (ret) {
        uint32 command0 = 1;
        plcSimDS->SetSignal(0u, command0);
        uint32 state0 = 2;
        plcSimDS->SetSignal(4u, state0);
    }

    //first cycles
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
        ret = (*internalState0 == 1u);
    }

    if (ret) {
        uint32 command0 = 2;
        plcSimDS->SetSignal(0u, command0);
        uint32 state0 = 2;
        plcSimDS->SetSignal(4u, state0);
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
        ret = (*internalState0 == 3u);
    }

    //return to the ready state
    if (ret) {
        uint32 command0 = 0;
        plcSimDS->SetSignal(0u, command0);
        uint32 state0 = 2;
        plcSimDS->SetSignal(4u, state0);
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
        ret = (*internalState0 == 0u);
    }

    Sleep::Sec(5.0);

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool DoubleHandshakeSlaveGAMTest::TestExecute_Interactive() {
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
            "            Destination = Application.Data.PlcSimulatorSlaveDS"
            "            Function = SetSignal"
            "            +Parameters = {"
            "                Class = ConfigurationDatabase"
            "                param1 = 0"
            "                param2 = \"$PVValue\""
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
            "            Destination = Application.Data.PlcSimulatorSlaveDS"
            "            Function = SetSignal"
            "            +Parameters = {"
            "                Class = ConfigurationDatabase"
            "                param1 = 1"
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
            "            Destination = Application.Data.PlcSimulatorSlaveDS"
            "            Function = SetSignal"
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
            "       +FC2PLC = {"
            "           Class = DoubleHandshakeGAM::DoubleHandshakeSlaveGAM"
            "           InputSignals = {"
            "               CommandIn0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               CommandIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "               AckOut0 = {"
            "                   Trigger = 1"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               AckOut1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "                        CommandIn0 = 1"
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
            "                        CommandIn0 = 2"
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
            "                        CommandIn1 = 3"
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
            "               CommandIn0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               CommandIn1 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
            "               }"
            "               State0 = {"
            "                   Type = uint32"
            "                   DataSource = PlcSimulatorSlaveDS"
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
            "        +PlcSimulatorSlaveDS = {"
            "            Class = PlcSimulatorSlaveDS"
            "            DelayCount = 10"
            "            NumberOfInputs = 2"
            "            Signals = {"
            "               CommandIn0 = {"
            "                   Type = uint32"
            "               }"
            "               CommandIn1 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut0 = {"
            "                   Type = uint32"
            "               }"
            "               AckOut1 = {"
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
            "                    Functions = { Sync, FC2PLC, MessageGAM }"
            "                }"
            "            }"
            "         }"
            "        +Acquiring = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = { Sync, FC2PLC, MessageGAM }"
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

bool DoubleHandshakeSlaveGAMTest::TestInitialise() {
    return TestExecute();
}

