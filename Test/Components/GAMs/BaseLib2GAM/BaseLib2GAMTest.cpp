/**
 * @file BaseLib2GAMTest.cpp
 * @brief Source file for class BaseLib2GAMTest
 * @date 2/12/2016
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
 * the class BaseLib2GAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BaseLib2GAM.h"
#include "BaseLib2GAMTest.h"
#include "DataSourceI.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * Gives access to the BaseLib2GAM memory for the Execute test
 */
class BaseLib2GAMHelper: public MARTe::BaseLib2GAM {
public:
    CLASS_REGISTER_DECLARATION()BaseLib2GAMHelper() : MARTe::BaseLib2GAM() {
    }

    virtual ~BaseLib2GAMHelper() {

    }

    void *GetInputSignalsMemory() {
        return BaseLib2GAM::GetInputSignalsMemory();
    }

    void *GetOutputSignalsMemory() {
        return BaseLib2GAM::GetOutputSignalsMemory();
    }

};
CLASS_REGISTER(BaseLib2GAMHelper, "1.0");

/**
 * Configures a MARTe application that uses this GAM instance.
 */
static bool TestIntegratedInApplication(const MARTe::char8 * const config,
                                        const MARTe::char8 * const firstState) {
    using namespace MARTe;

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
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = ord->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        ok = application->PrepareNextState(firstState);
    }

    return ok;
}

/**
 * Very basic scheduler to execute a single cycle
 */
class BaseLib2GAMTestScheduler: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

BaseLib2GAMTestScheduler    () : MARTe::GAMSchedulerI() {
        scheduledStates = NULL_PTR(MARTe::ScheduledState * const *);
    }

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    void ExecuteThreadCycle(MARTe::uint32 threadId) {
        MARTe::ReferenceT<MARTe::RealTimeApplication> realTimeAppT = realTimeApp;
        ExecuteSingleCycle(scheduledStates[realTimeAppT->GetIndex()]->threads[threadId].executables,
                scheduledStates[realTimeAppT->GetIndex()]->threads[threadId].numberOfExecutables);
    }

    virtual bool ConfigureScheduler(MARTe::Reference realTimeApp) {
        bool ret = GAMSchedulerI::ConfigureScheduler(realTimeApp);
        if (ret) {
            scheduledStates = GetSchedulableStates();
        }
        return ret;
    }

    virtual void CustomPrepareNextState() {

    }

private:

    MARTe::ScheduledState * const * scheduledStates;
};
CLASS_REGISTER(BaseLib2GAMTestScheduler, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool BaseLib2GAMTest::TestConstructor() {
    MARTe::BaseLib2GAM gam;
    return true;
}

bool BaseLib2GAMTest::TestInitialise() {
    using namespace MARTe;
    const char8 * const config = ""
            "+GAM1 = {"
            "    Class = BaseLib2GAMHelper"
            "    InputSignals = {"
            "        Signal1 = {"
            "            DataSource = DDB1"
            "            Type = float32"
            "            NumberOfElements = 1"
            "            Default = 2.5"
            "        }"
            "    }"
            "    OutputSignals = {"
            "        Signal2 = {"
            "            DataSource = DDB1"
            "            Type = int32"
            "            NumberOfElements = 1"
            "        }"
            "    }"
            "    GAMName = \"TypeConvert1\""
            "    GAMFunctionNumber = 0x00010000"
            "    BaseLib2Config = \""
            "        +TypeConvert1 = {"
            "            Class = TypeConvertGAM"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    SignalName = Signal1"
            "                    SignalType = float"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    SignalName = Signal2"
            "                    SignalType = int32"
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

bool BaseLib2GAMTest::TestInitialise_False_NoGAMName() {
    using namespace MARTe;
    const char8 * const config = ""
            "+GAM1 = {"
            "    Class = BaseLib2GAMHelper"
            "    InputSignals = {"
            "        Signal1 = {"
            "            DataSource = DDB1"
            "            Type = float32"
            "            NumberOfElements = 1"
            "            Default = 2.5"
            "        }"
            "    }"
            "    OutputSignals = {"
            "        Signal2 = {"
            "            DataSource = DDB1"
            "            Type = int32"
            "            NumberOfElements = 1"
            "        }"
            "    }"
            "    GAMFunctionNumber = 0x00010000"
            "    BaseLib2Config = \""
            "        +TypeConvert1 = {"
            "            Class = TypeConvertGAM"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    SignalName = Signal1"
            "                    SignalType = float"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    SignalName = Signal2"
            "                    SignalType = int32"
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

bool BaseLib2GAMTest::TestInitialise_False_WrongGAMName() {
    using namespace MARTe;
    const char8 * const config = ""
            "+GAM1 = {"
            "    Class = BaseLib2GAMHelper"
            "    InputSignals = {"
            "        Signal1 = {"
            "            DataSource = DDB1"
            "            Type = float32"
            "            NumberOfElements = 1"
            "            Default = 2.5"
            "        }"
            "    }"
            "    OutputSignals = {"
            "        Signal2 = {"
            "            DataSource = DDB1"
            "            Type = int32"
            "            NumberOfElements = 1"
            "        }"
            "    }"
            "    GAMName = \"TypeConvert11\""
            "    GAMFunctionNumber = 0x00010000"
            "    BaseLib2Config = \""
            "        +TypeConvert1 = {"
            "            Class = TypeConvertGAM"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    SignalName = Signal1"
            "                    SignalType = float"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    SignalName = Signal2"
            "                    SignalType = int32"
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

bool BaseLib2GAMTest::TestInitialise_False_NoGAMFunctionNumber() {
    using namespace MARTe;
    const char8 * const config = ""
            "+GAM1 = {"
            "    Class = BaseLib2GAMHelper"
            "    InputSignals = {"
            "        Signal1 = {"
            "            DataSource = DDB1"
            "            Type = float32"
            "            NumberOfElements = 1"
            "            Default = 2.5"
            "        }"
            "    }"
            "    OutputSignals = {"
            "        Signal2 = {"
            "            DataSource = DDB1"
            "            Type = int32"
            "            NumberOfElements = 1"
            "        }"
            "    }"
            "    GAMName = \"TypeConvert1\""
            "    BaseLib2Config = \""
            "        +TypeConvert1 = {"
            "            Class = TypeConvertGAM"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    SignalName = Signal1"
            "                    SignalType = float"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    SignalName = Signal2"
            "                    SignalType = int32"
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

bool BaseLib2GAMTest::TestInitialise_False_NoBaseLib2Config() {
    using namespace MARTe;
    const char8 * const config = ""
            "+GAM1 = {"
            "    Class = BaseLib2GAMHelper"
            "    InputSignals = {"
            "        Signal1 = {"
            "            DataSource = DDB1"
            "            Type = float32"
            "            NumberOfElements = 1"
            "            Default = 2.5"
            "        }"
            "    }"
            "    OutputSignals = {"
            "        Signal2 = {"
            "            DataSource = DDB1"
            "            Type = int32"
            "            NumberOfElements = 1"
            "        }"
            "    }"
            "    GAMName = \"TypeConvert1\""
            "    GAMFunctionNumber = 0x00010000"
            "    BaseLib2Configs = \""
            "        +TypeConvert1 = {"
            "            Class = TypeConvertGAM"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    SignalName = Signal1"
            "                    SignalType = float"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    SignalName = Signal2"
            "                    SignalType = int32"
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

bool BaseLib2GAMTest::TestInitialise_False_WrongBaseLib2Config() {
    using namespace MARTe;
    const char8 * const config = ""
            "+GAM1 = {"
            "    Class = BaseLib2GAMHelper"
            "    InputSignals = {"
            "        Signal1 = {"
            "            DataSource = DDB1"
            "            Type = float32"
            "            NumberOfElements = 1"
            "            Default = 2.5"
            "        }"
            "    }"
            "    OutputSignals = {"
            "        Signal2 = {"
            "            DataSource = DDB1"
            "            Type = int32"
            "            NumberOfElements = 1"
            "        }"
            "    }"
            "    GAMName = \"TypeConvert1\""
            "    GAMFunctionNumber = 0x00010000"
            "    BaseLib2Config = \""
            "        +TypeConvert1 = {"
            "            Classs = TypeConvertGAM"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    SignalName = Signal1"
            "                    SignalType = float"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    SignalName = Signal2"
            "                    SignalType = int32"
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

bool BaseLib2GAMTest::TestSetup() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = BaseLib2GAMHelper"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 1"
            "                   Default = 2.5"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal2 = {"
            "                   DataSource = DDB1"
            "                   Type = int32"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            GAMName = \"TypeConvert1\""
            "            GAMFunctionNumber = 0x00010000"
            "            BaseLib2Config = \""
            "                +TypeConvert1 = {"
            "                    Class = TypeConvertGAM"
            "                    InputSignals = {"
            "                        Signal1 = {"
            "                            SignalName = Signal1"
            "                            SignalType = float"
            "                        }"
            "                    }"
            "                    OutputSignals = {"
            "                        Signal2 = {"
            "                            SignalName = Signal2"
            "                            SignalType = int32"
            "                        }"
            "                    }"
            "                }"
            "            \""
            "        }"
            "        +GAM2 = {"
            "            Class = BaseLib2GAMHelper"
            "            InputSignals = {"
            "               Signal2 = {"
            "                   DataSource = DDB1"
            "                   Type = int32"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            GAMName = \"TypeConvert2\""
            "            GAMFunctionNumber = 0x00010000"
            "            BaseLib2Config = \""
            "                +TypeConvert2 = {"
            "                    Class = TypeConvertGAM"
            "                    InputSignals = {"
            "                        Signal2 = {"
            "                            SignalName = Signal2"
            "                            SignalType = int32"
            "                        }"
            "                    }"
            "                    OutputSignals = {"
            "                        Signal1 = {"
            "                            SignalName = Signal1"
            "                            SignalType = float"
            "                        }"
            "                    }"
            "                }"
            "            \""
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
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAM1 GAM2}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = BaseLib2GAMTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = TestIntegratedInApplication(config1, "State1");

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool BaseLib2GAMTest::TestSetup_InputSignals_Mismatch() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = BaseLib2GAMHelper"
            "            InputSignals = {"
            "               Signal3 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 1"
            "                   Default = 2.5"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal2 = {"
            "                   DataSource = DDB1"
            "                   Type = int32"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            GAMName = \"TypeConvert1\""
            "            GAMFunctionNumber = 0x00010000"
            "            BaseLib2Config = \""
            "                +TypeConvert1 = {"
            "                    Class = TypeConvertGAM"
            "                    InputSignals = {"
            "                        Signal1 = {"
            "                            SignalName = Signal1"
            "                            SignalType = float"
            "                        }"
            "                    }"
            "                    OutputSignals = {"
            "                        Signal2 = {"
            "                            SignalName = Signal2"
            "                            SignalType = int32"
            "                        }"
            "                    }"
            "                }"
            "            \""
            "        }"
            "        +GAM2 = {"
            "            Class = BaseLib2GAMHelper"
            "            InputSignals = {"
            "               Signal2 = {"
            "                   DataSource = DDB1"
            "                   Type = int32"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            GAMName = \"TypeConvert2\""
            "            GAMFunctionNumber = 0x00010000"
            "            BaseLib2Config = \""
            "                +TypeConvert2 = {"
            "                    Class = TypeConvertGAM"
            "                    InputSignals = {"
            "                        Signal2 = {"
            "                            SignalName = Signal2"
            "                            SignalType = int32"
            "                        }"
            "                    }"
            "                    OutputSignals = {"
            "                        Signal1 = {"
            "                            SignalName = Signal1"
            "                            SignalType = float"
            "                        }"
            "                    }"
            "                }"
            "            \""
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
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAM1 GAM2}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = BaseLib2GAMTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = TestIntegratedInApplication(config1, "State1");

    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool BaseLib2GAMTest::TestSetup_OutputSignals_Mismatch() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = BaseLib2GAMHelper"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 1"
            "                   Default = 2.5"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal3 = {"
            "                   DataSource = DDB1"
            "                   Type = int32"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            GAMName = \"TypeConvert1\""
            "            GAMFunctionNumber = 0x00010000"
            "            BaseLib2Config = \""
            "                +TypeConvert1 = {"
            "                    Class = TypeConvertGAM"
            "                    InputSignals = {"
            "                        Signal1 = {"
            "                            SignalName = Signal1"
            "                            SignalType = float"
            "                        }"
            "                    }"
            "                    OutputSignals = {"
            "                        Signal2 = {"
            "                            SignalName = Signal2"
            "                            SignalType = int32"
            "                        }"
            "                    }"
            "                }"
            "            \""
            "        }"
            "        +GAM2 = {"
            "            Class = BaseLib2GAMHelper"
            "            InputSignals = {"
            "               Signal2 = {"
            "                   DataSource = DDB1"
            "                   Type = int32"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            GAMName = \"TypeConvert2\""
            "            GAMFunctionNumber = 0x00010000"
            "            BaseLib2Config = \""
            "                +TypeConvert2 = {"
            "                    Class = TypeConvertGAM"
            "                    InputSignals = {"
            "                        Signal2 = {"
            "                            SignalName = Signal2"
            "                            SignalType = int32"
            "                        }"
            "                    }"
            "                    OutputSignals = {"
            "                        Signal1 = {"
            "                            SignalName = Signal1"
            "                            SignalType = float"
            "                        }"
            "                    }"
            "                }"
            "            \""
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
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAM1 GAM2}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = BaseLib2GAMTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = TestIntegratedInApplication(config1, "State1");

    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool BaseLib2GAMTest::TestExecute() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = BaseLib2GAMHelper"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 1"
            "                   Default = 2.5"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal2 = {"
            "                   DataSource = DDB1"
            "                   Type = int32"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            GAMName = \"TypeConvert1\""
            "            GAMFunctionNumber = 0x00010000"
            "            BaseLib2Config = \""
            "                +TypeConvert1 = {"
            "                    Class = TypeConvertGAM"
            "                    InputSignals = {"
            "                        Signal1 = {"
            "                            SignalName = Signal1"
            "                            SignalType = float"
            "                        }"
            "                    }"
            "                    OutputSignals = {"
            "                        Signal2 = {"
            "                            SignalName = Signal2"
            "                            SignalType = int32"
            "                        }"
            "                    }"
            "                }"
            "            \""
            "        }"
            "        +GAM2 = {"
            "            Class = BaseLib2GAMHelper"
            "            InputSignals = {"
            "               Signal2 = {"
            "                   DataSource = DDB1"
            "                   Type = int32"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            GAMName = \"TypeConvert2\""
            "            GAMFunctionNumber = 0x00010000"
            "            BaseLib2Config = \""
            "                +TypeConvert2 = {"
            "                    Class = TypeConvertGAM"
            "                    InputSignals = {"
            "                        Signal2 = {"
            "                            SignalName = Signal2"
            "                            SignalType = int32"
            "                        }"
            "                    }"
            "                    OutputSignals = {"
            "                        Signal1 = {"
            "                            SignalName = Signal1"
            "                            SignalType = float"
            "                        }"
            "                    }"
            "                }"
            "            \""
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
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAM1 GAM2}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = BaseLib2GAMTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = TestIntegratedInApplication(config1, "State1");

    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<BaseLib2GAMHelper> gam1 = ord->Find("Test.Functions.GAM1");
    ReferenceT<BaseLib2GAMHelper> gam2 = ord->Find("Test.Functions.GAM2");
    ReferenceT<BaseLib2GAMTestScheduler> scheduler = ord->Find("Test.Scheduler");
    ReferenceT<RealTimeApplication> application = ord->Find("Test");
    if (ok) {
        ok = application->StartNextStateExecution();
    }
    if (ok) {
        ok = gam1.IsValid();
    }
    if (ok) {
        ok = gam2.IsValid();
    }
    if (ok) {
        ok = scheduler.IsValid();
    }
    float32 *inMemGam1 = NULL;
    uint32 *outMemGam1 = NULL;
    uint32 *inMemGam2 = NULL;
    float32 *outMemGam2 = NULL;

    if (ok) {
        inMemGam1 = static_cast<float32 *>(gam1->GetInputSignalsMemory());
        outMemGam1 = static_cast<uint32 *>(gam1->GetOutputSignalsMemory());
        inMemGam2 = static_cast<uint32 *>(gam2->GetInputSignalsMemory());
        outMemGam2 = static_cast<float32 *>(gam2->GetOutputSignalsMemory());
        scheduler->ExecuteThreadCycle(0);
    }
    if (ok) {
        ok &= (*inMemGam1 == 2.5);
        ok &= (*outMemGam1 == 2);
        ok &= (*inMemGam2 == 2);
        ok &= (*outMemGam2 == 2.0);
        scheduler->ExecuteThreadCycle(0);
    }
    if (ok) {
        ok &= (*inMemGam1 == 2.0);
        ok &= (*outMemGam1 == 2);
        ok &= (*inMemGam2 == 2);
        ok &= (*outMemGam2 == 2.0);
    }
    application->StopCurrentStateExecution();
    ord->Purge();
    return ok;
}

bool BaseLib2GAMTest::TestExecute_Arrays() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = BaseLib2GAMHelper"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "                   Default = {3.5 1.5 2.5}"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal2 = {"
            "                   DataSource = DDB1"
            "                   Type = int32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "            }"
            "            GAMName = \"TypeConvert1\""
            "            GAMFunctionNumber = 0x00010000"
            "        }"
            "        +GAM2 = {"
            "            Class = BaseLib2GAMHelper"
            "            InputSignals = {"
            "               Signal2 = {"
            "                   DataSource = DDB1"
            "                   Type = int32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "            }"
            "            GAMName = \"TypeConvert2\""
            "            GAMFunctionNumber = 0x00010000"
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
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAM1 GAM2}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = BaseLib2GAMTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    //Have to do it this way because commas inside commas of a RHS string will terminate the string...
    //Using an external file should not pose this problem.
    StreamString baseLib2GAMConfig1 = ""
            "+TypeConvert1 = {\n"
            "    Class = TypeConvertGAM\n"
            "    InputSignals = {\n"
            "        Signal1 = {\n"
            "            SignalName = \"Signal1[3]\"\n"
            "            SignalType = float\n"
            "        }\n"
            "    }\n"
            "    OutputSignals = {\n"
            "        Signal2 = {\n"
            "            SignalName = \"Signal2[3]\"\n"
            "            SignalType = int32\n"
            "        }\n"
            "    }\n"
            "}";

    StreamString baseLib2GAMConfig2 = ""
            "+TypeConvert2 = {\n"
            "    Class = TypeConvertGAM\n"
            "    InputSignals = {\n"
            "        Signal2 = {\n"
            "            SignalName = \"Signal2[3]\"\n"
            "            SignalType = int32\n"
            "        }\n"
            "    }\n"
            "    OutputSignals = {\n"
            "        Signal1 = {\n"
            "            SignalName = \"Signal1[3]\"\n"
            "            SignalType = float\n"
            "        }\n"
            "    }\n"
            "}";

    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1");
    }
    if (ok) {
        ok = cdb.Write("BaseLib2Config", baseLib2GAMConfig1.Buffer());
    }
    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM2");
    }
    if (ok) {
        ok = cdb.Write("BaseLib2Config", baseLib2GAMConfig2.Buffer());
    }
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();

    if (ok) {
        ord->Purge();
        ok = ord->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = ord->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }

    ReferenceT<BaseLib2GAMHelper> gam1 = ord->Find("Test.Functions.GAM1");
    ReferenceT<BaseLib2GAMHelper> gam2 = ord->Find("Test.Functions.GAM2");
    ReferenceT<BaseLib2GAMTestScheduler> scheduler = ord->Find("Test.Scheduler");

    if (ok) {
        ok = gam1.IsValid();
    }
    if (ok) {
        ok = gam2.IsValid();
    }
    if (ok) {
        ok = scheduler.IsValid();
    }
    float32 *inMemGam1 = NULL;
    uint32 *outMemGam1 = NULL;
    uint32 *inMemGam2 = NULL;
    float32 *outMemGam2 = NULL;

    if (ok) {
        inMemGam1 = static_cast<float32 *>(gam1->GetInputSignalsMemory());
        outMemGam1 = static_cast<uint32 *>(gam1->GetOutputSignalsMemory());
        inMemGam2 = static_cast<uint32 *>(gam2->GetInputSignalsMemory());
        outMemGam2 = static_cast<float32 *>(gam2->GetOutputSignalsMemory());
        scheduler->ExecuteThreadCycle(0);
    }
    if (ok) {
        ok &= (inMemGam1[0] == 3.5);
        ok &= (inMemGam1[1] == 1.5);
        ok &= (inMemGam1[2] == 2.5);
        ok &= (outMemGam1[0] == 3);
        ok &= (outMemGam1[1] == 1);
        ok &= (outMemGam1[2] == 2);
        ok &= (inMemGam2[0] == 3);
        ok &= (inMemGam2[1] == 1);
        ok &= (inMemGam2[2] == 2);
        ok &= (outMemGam2[0] == 3.0);
        ok &= (outMemGam2[1] == 1.0);
        ok &= (outMemGam2[2] == 2.0);
        scheduler->ExecuteThreadCycle(0);
    }
    if (ok) {
        ok &= (inMemGam1[0] == 3.0);
        ok &= (inMemGam1[1] == 1.0);
        ok &= (inMemGam1[2] == 2.0);
        ok &= (outMemGam1[0] == 3);
        ok &= (outMemGam1[1] == 1);
        ok &= (outMemGam1[2] == 2);
        ok &= (inMemGam2[0] == 3);
        ok &= (inMemGam2[1] == 1);
        ok &= (inMemGam2[2] == 2);
        ok &= (outMemGam2[0] == 3.0);
        ok &= (outMemGam2[1] == 1.0);
        ok &= (outMemGam2[2] == 2.0);
    }
    application->StopCurrentStateExecution();
    ord->Purge();
    return ok;
}

static bool TestTranslateSignalNameHelper(MARTe::TypeDescriptor typeDesc,
                                          MARTe::StreamString expected) {
    using namespace MARTe;
    BaseLib2GAM gam;
    StreamString signalName;
    bool ok = gam.TranslateSignalName(typeDesc, signalName);
    ok &= (signalName == expected);
    return ok;
}

bool BaseLib2GAMTest::TestTranslateSignalName() {
    using namespace MARTe;

    bool ok = TestTranslateSignalNameHelper(UnsignedInteger8Bit, "uint8");
    ok &= TestTranslateSignalNameHelper(UnsignedInteger16Bit, "uint16");
    ok &= TestTranslateSignalNameHelper(UnsignedInteger32Bit, "uint32");
    ok &= TestTranslateSignalNameHelper(UnsignedInteger64Bit, "uint64");
    ok &= TestTranslateSignalNameHelper(SignedInteger8Bit, "int8");
    ok &= TestTranslateSignalNameHelper(SignedInteger16Bit, "int16");
    ok &= TestTranslateSignalNameHelper(SignedInteger32Bit, "int32");
    ok &= TestTranslateSignalNameHelper(SignedInteger64Bit, "int64");
    ok &= TestTranslateSignalNameHelper(Float32Bit, "float");
    ok &= TestTranslateSignalNameHelper(Float64Bit, "double");
    ok &= !TestTranslateSignalNameHelper(Float128Bit, "");
    ok &= !TestTranslateSignalNameHelper(Character8Bit, "");
    return ok;
}
