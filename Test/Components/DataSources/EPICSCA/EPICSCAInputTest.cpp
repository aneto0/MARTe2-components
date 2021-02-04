/**
 * @file EPICSCAInputTest.cpp
 * @brief Source file for class EPICSCAInputTest
 * @date 21/04/2017
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
 *
 * @details This source file contains the definition of all the methods for
 * the class EPICSCAInputTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <cadef.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "Directory.h"
#include "DirectoryScanner.h"
#include "EPICSCAInput.h"
#include "EPICSCAInputTest.h"
#include "GAM.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief GAM which reads signals from a given EPICSCAInput
 */
class EPICSCAInputGAMTestHelper: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()EPICSCAInputGAMTestHelper() {
        char8Signal = NULL;
        stringSignal = NULL;
        uint8Signal = NULL;
        int8Signal = NULL;
        uint16Signal = NULL;
        int16Signal = NULL;
        uint32Signal = NULL;
        int32Signal = NULL;
        float32Signal = NULL;
        float64Signal = NULL;
        numberOfElements = 1u;
    }

    virtual ~EPICSCAInputGAMTestHelper() {
        using namespace MARTe;
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        uint32 i = 0u;
        if (GetSignalType(InputSignals, i) == CharString) {
            stringSignal = reinterpret_cast<char8 *>(GetInputSignalMemory(i));
            i++;
        }
        else if (GetSignalType(InputSignals, i) == Character8Bit) {
            char8Signal = reinterpret_cast<char8 *>(GetInputSignalMemory(i));
            i++;
        }
        GetSignalNumberOfElements(InputSignals, i, numberOfElements);
        if (GetSignalType(InputSignals, i) == UnsignedInteger8Bit) {
            uint8Signal = reinterpret_cast<uint8 *>(GetInputSignalMemory(i));
        }
        else {
            int8Signal = reinterpret_cast<int8 *>(GetInputSignalMemory(i));
        }
        i++;
        if (GetSignalType(InputSignals, i) == UnsignedInteger16Bit) {
            uint16Signal = reinterpret_cast<uint16 *>(GetInputSignalMemory(i));
        }
        else {
            int16Signal = reinterpret_cast<int16 *>(GetInputSignalMemory(i));
        }
        i++;
        if (GetSignalType(InputSignals, i) == UnsignedInteger32Bit) {
            uint32Signal = reinterpret_cast<uint32 *>(GetInputSignalMemory(i));
        }
        else {
            int32Signal = reinterpret_cast<int32 *>(GetInputSignalMemory(i));
        }
        i++;
        if (GetSignalType(InputSignals, i) == Float32Bit) {
            float32Signal = reinterpret_cast<float32 *>(GetInputSignalMemory(i));
        }
        else {
            float64Signal = reinterpret_cast<float64 *>(GetInputSignalMemory(i));
        }

        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        return true;
    }
    const MARTe::char8 *char8Signal;
    const MARTe::char8 *stringSignal;
    MARTe::uint8 *uint8Signal;
    MARTe::int8 *int8Signal;
    MARTe::uint16 *uint16Signal;
    MARTe::int16 *int16Signal;
    MARTe::uint32 *uint32Signal;
    MARTe::int32 *int32Signal;
    MARTe::float32 *float32Signal;
    MARTe::float64 *float64Signal;
    MARTe::uint32 numberOfElements;
};
CLASS_REGISTER(EPICSCAInputGAMTestHelper, "1.0")

/**
 * @brief Manual scheduler to test the correct interface between the EPICSCAInput and the GAMs
 */
class EPICSCAInputSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

EPICSCAInputSchedulerTestHelper    () : MARTe::GAMSchedulerI() {
        scheduledStates = NULL;
    }

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    void ExecuteThreadCycle(MARTe::uint32 threadId) {
        using namespace MARTe;
        ReferenceT<RealTimeApplication> realTimeAppT = realTimeApp;
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

CLASS_REGISTER(EPICSCAInputSchedulerTestHelper, "1.0")

static bool TestIntegratedInApplication(const MARTe::char8 * const config, bool destroy) {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StreamString err;
    StandardParser parser(configStream, cdb, &err);

    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "%s", err.Buffer());
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }

    if (destroy) {
        god->Purge();
    }
    return ok;
}

//Standard configuration
static const MARTe::char8 * const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalString = {"
        "                    Type = string"
        "                    NumberOfElements = 40"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalChar8 = {"
        "                    Type = char8"
        "                    NumberOfElements = 40"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +EPICSCAInputTest = {"
        "            Class = EPICSCAInput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            Signals = {"
        "                SignalChar8 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Char8\""
        "                }"
        "                SignalString = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::String\""
        "                }"
        "                SignalUInt8 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt8\""
        "                }"
        "                SignalInt8 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int8\""
        "                }"
        "                SignalUInt16 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt16\""
        "                }"
        "                SignalInt16 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int16\""
        "                }"
        "                SignalUInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt32\""
        "                }"
        "                SignalInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int32\""
        "                }"
        "                SignalFloat32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float32\""
        "                }"
        "                SignalFloat64 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float64\""
        "                }"
        "            }"
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
        "        Class = EPICSCAInputSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Standard configuration no signals
static const MARTe::char8 * const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +EPICSCAInputTest = {"
        "            Class = EPICSCAInput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            Signals = {"
        "                SignalUInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt32\""
        "                }"
        "                SignalInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int32\""
        "                }"
        "                SignalFloat32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float32\""
        "                }"
        "                SignalFloat64 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float64\""
        "                }"
        "            }"
        "        }"
        "        +EPICSCAInputTestNoSignals = {"
        "            Class = EPICSCAInput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            Signals = {"
        "                Locked = 0"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = EPICSCAInputSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Standard configuration with arrays
static const MARTe::char8 * const config3 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +EPICSCAInputTest = {"
        "            Class = EPICSCAInput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            Signals = {"
        "                SignalUInt8 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt8Arr\""
        "                }"
        "                SignalUInt16 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt16Arr\""
        "                }"
        "                SignalInt8 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int8Arr\""
        "                }"
        "                SignalInt16 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int16Arr\""
        "                }"
        "                SignalUInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt32Arr\""
        "                }"
        "                SignalInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int32Arr\""
        "                }"
        "                SignalFloat32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float32Arr\""
        "                }"
        "                SignalFloat64 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float64Arr\""
        "                }"
        "            }"
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
        "        Class = EPICSCAInputSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Standard configuration no PVName
static const MARTe::char8 * const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +EPICSCAInputTest = {"
        "            Class = EPICSCAInput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            Signals = {"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                }"
        "                SignalInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int32\""
        "                }"
        "                SignalFloat32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float32\""
        "                }"
        "                SignalFloat64 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float64\""
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = EPICSCAInputSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Standard configuration with samples
static const MARTe::char8 * const config5 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    Samples = 2"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +EPICSCAInputTest = {"
        "            Class = EPICSCAInput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            Signals = {"
        "                SignalUInt16 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt16\""
        "                }"
        "                SignalInt16 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int16\""
        "                }"
        "                SignalUInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt32\""
        "                }"
        "                SignalInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int32\""
        "                }"
        "                SignalFloat32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float32\""
        "                }"
        "                SignalFloat64 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float64\""
        "                }"
        "            }"
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
        "        Class = EPICSCAInputSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Standard configuration with an unsupported type
static const MARTe::char8 * const config6 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +EPICSCAInputTest = {"
        "            Class = EPICSCAInput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            Signals = {"
        "                SignalUInt64 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt64\""
        "                }"
        "                SignalInt16 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int16\""
        "                }"
        "                SignalUInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt32\""
        "                }"
        "                SignalInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int32\""
        "                }"
        "                SignalFloat32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float32\""
        "                }"
        "                SignalFloat64 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float64\""
        "                }"
        "            }"
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
        "        Class = EPICSCAInputSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Standard configuration with a string with more than 40 bytes
static const MARTe::char8 * const config7 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalChar8 = {"
        "                    Type = string"
        "                    NumberOfElements = 39"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = EPICSCAInputGAMTestHelper"
        "            InputSignals = {"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAInputTest"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +EPICSCAInputTest = {"
        "            Class = EPICSCAInput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            Signals = {"
        "                SignalChar8 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Char8\""
        "                }"
        "                SignalUInt8 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt8\""
        "                }"
        "                SignalInt8 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int8\""
        "                }"
        "                SignalUInt16 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt16\""
        "                }"
        "                SignalInt16 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int16\""
        "                }"
        "                SignalUInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt32\""
        "                }"
        "                SignalInt32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int32\""
        "                }"
        "                SignalFloat32 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float32\""
        "                }"
        "                SignalFloat64 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Float64\""
        "                }"
        "            }"
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
        "        Class = EPICSCAInputSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool EPICSCAInputTest::TestConstructor() {
    using namespace MARTe;
    EPICSCAInput test;
    bool ok = (test.GetCPUMask() == 0xff);
    ok &= (test.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 4u));
    return ok;
}

bool EPICSCAInputTest::TestAllocateMemory() {
    using namespace MARTe;
    EPICSCAInput test;
    return test.AllocateMemory();
}

bool EPICSCAInputTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    EPICSCAInput test;
    return (test.GetNumberOfMemoryBuffers() == 1);
}

bool EPICSCAInputTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    EPICSCAInput test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 1);
    cdb.Write("StackSize", 100000);
    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();
    test.Initialise(cdb);
    void *ptr = NULL;
    bool ok = !test.GetSignalMemoryBuffer(0, 0, ptr);
    if (ok) {
        ok = TestIntegratedInApplication(config1, true);
    }
    return ok;
}

bool EPICSCAInputTest::TestGetBrokerName_OutputSignals() {
    using namespace MARTe;
    EPICSCAInput test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, OutputSignals), "") == 0);

    return ok;
}

bool EPICSCAInputTest::TestGetBrokerName() {
    using namespace MARTe;
    EPICSCAInput test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, InputSignals), "MemoryMapInputBroker") == 0);

    return ok;
}

bool EPICSCAInputTest::TestGetInputBrokers() {
    return TestIntegratedInApplication(config1, true);
}

bool EPICSCAInputTest::TestGetOutputBrokers() {
    using namespace MARTe;
    EPICSCAInput test;
    ReferenceContainer rc;
    return !test.GetOutputBrokers(rc, "", NULL);
}

bool EPICSCAInputTest::TestInitialise() {
    using namespace MARTe;
    EPICSCAInput test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 1);
    cdb.Write("StackSize", 100000);
    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (test.GetCPUMask() == 1);
        ok &= (test.GetStackSize() == 100000);
    }
    return ok;
}

bool EPICSCAInputTest::TestInitialise_Defaults() {
    using namespace MARTe;
    EPICSCAInput test;
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (test.GetCPUMask() == 0xff);
        ok &= (test.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 4u));
    }
    return ok;
}

bool EPICSCAInputTest::TestInitialise_False_Signals() {
    using namespace MARTe;
    EPICSCAInput test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 1);
    cdb.Write("StackSize", 100000);
    return !test.Initialise(cdb);
}

bool EPICSCAInputTest::TestGetStackSize() {
    return TestInitialise();
}

bool EPICSCAInputTest::TestGetCPUMask() {
    return TestInitialise();
}

bool EPICSCAInputTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config1, true);
}

bool EPICSCAInputTest::TestSetConfiguredDatabase_False_NoSignals() {
    return !TestIntegratedInApplication(config2, true);
}

bool EPICSCAInputTest::TestSetConfiguredDatabase_False_PVName() {
    return !TestIntegratedInApplication(config4, true);
}

bool EPICSCAInputTest::TestSetConfiguredDatabase_False_Samples() {
    return !TestIntegratedInApplication(config5, true);
}

bool EPICSCAInputTest::TestSetConfiguredDatabase_False_UnsupportedType() {
    return !TestIntegratedInApplication(config6, true);
}

bool EPICSCAInputTest::TestSetConfiguredDatabase_False_WrongStringSize() {
    return !TestIntegratedInApplication(config7, true);
}

bool EPICSCAInputTest::TestExecute() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config1, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<EPICSCAInputGAMTestHelper> gam1;
    ReferenceT<EPICSCAInputGAMTestHelper> gam2;
    ReferenceT<EPICSCAInputSchedulerTestHelper> scheduler;
    ReferenceT<RealTimeApplication> application;

    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        gam1 = godb->Find("Test.Functions.GAM1");
        ok = gam1.IsValid();
    }
    if (ok) {
        gam2 = godb->Find("Test.Functions.GAM2");
        ok = gam2.IsValid();
    }
    if (ok) {
        scheduler = godb->Find("Test.Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }

    if (ok) {
        ok = (ca_context_create(ca_enable_preemptive_callback));
    }
    const uint32 NUMBER_OF_PVS = 10u;
    chid pvChids[NUMBER_OF_PVS];
    chtype pvTypes[] = { DBR_STRING, DBR_STRING, DBR_CHAR, DBR_CHAR, DBR_SHORT, DBR_SHORT, DBR_LONG, DBR_LONG, DBR_FLOAT, DBR_DOUBLE };
    const char8 *pvNames[] = { "MARTe2::EPICSCAInput::Test::String", "MARTe2::EPICSCAInput::Test::Char8", "MARTe2::EPICSCAInput::Test::UInt8", "MARTe2::EPICSCAInput::Test::Int8",
            "MARTe2::EPICSCAInput::Test::UInt16", "MARTe2::EPICSCAInput::Test::Int16", "MARTe2::EPICSCAInput::Test::UInt32",
            "MARTe2::EPICSCAInput::Test::Int32", "MARTe2::EPICSCAInput::Test::Float32", "MARTe2::EPICSCAInput::Test::Float64" };
    char8 char8Value[40];
    char8 stringValue[40];
    StringHelper::Copy(&char8Value[0], "EPICSCATEST");
    StringHelper::Copy(&stringValue[0], "EPICSCATESTSTR");
    uint8 uint8Value = 11u;
    int8 int8Value = 12;
    uint16 uint16Value = 1u;
    int16 int16Value = 2;
    uint32 uint32Value = 3u;
    int32 int32Value = 5;
    float32 float32Value = 7.0;
    float64 float64Value = 9.0;
    void *pvMemory[] = { &stringValue[0], &char8Value[0], &uint8Value, &int8Value, &uint16Value, &int16Value, &uint32Value, &int32Value, &float32Value, &float64Value };

    if (ok) {
        uint32 n;
        for (n = 0u; (n < NUMBER_OF_PVS) && (ok); n++) {
            /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
            ok = (ca_create_channel(pvNames[n], NULL_PTR(caCh *), NULL_PTR(void *), 20u, &pvChids[n]) == ECA_NORMAL);
        }
    }

    if (ok) {
        uint32 n;
        bool done = false;
        uint32 doneC = 0;
        uint32 timeOutCounts = 50;
        while ((!done) && (timeOutCounts > 0u) && (ok)) {
            for (n = 0u; (n < NUMBER_OF_PVS) && (ok); n++) {
                ca_put(pvTypes[n], pvChids[n], pvMemory[n]);
            }
            if (ok) {
                ca_pend_io(1.0);
            }
            scheduler->ExecuteThreadCycle(0);

            StreamString tmpChar8 = char8Value;
            StreamString tmpString = stringValue;
            done = (tmpString == gam1->stringSignal);
            done &= (*gam1->uint8Signal == uint8Value);
            done &= (*gam1->uint16Signal == uint16Value);
            done &= (*gam1->uint32Signal == uint32Value);
            done &= (*gam1->float64Signal == float64Value);
            done &= (tmpChar8 == gam2->char8Signal);
            done &= (*gam2->int8Signal == int8Value);
            done &= (*gam2->int8Signal == int8Value);
            done &= (*gam2->int16Signal == int16Value);
            done &= (*gam2->int32Signal == int32Value);
            done &= (*gam2->float32Signal == float32Value);
            if (!done) {
                timeOutCounts--;
                Sleep::Sec(0.1);
            }
            if ((done) && (doneC == 0)) {
                StringHelper::Copy(&char8Value[0], "EPICSCATEST2");
                StringHelper::Copy(&stringValue[0], "EPICSCATESTSTR2");
                uint8Value *= 2;
                uint16Value *= 2;
                int8Value *= 2;
                int16Value *= 2;
                uint32Value *= 2;
                int32Value *= 2;
                float32Value *= 2;
                float64Value *= 2;
                done = false;
                doneC++;
            }
        }
        if (ok) {
            ok = done;
        }
    }

    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        uint32 n;
        for (n = 0u; (n < NUMBER_OF_PVS); n++) {
            ca_clear_channel(pvChids[n]);
        }
    }
    ca_detach_context();
    ca_context_destroy();
    godb->Purge();
    return ok;
}

bool EPICSCAInputTest::TestExecute_Arrays() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config3, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<EPICSCAInputGAMTestHelper> gam1;
    ReferenceT<EPICSCAInputGAMTestHelper> gam2;
    ReferenceT<EPICSCAInputSchedulerTestHelper> scheduler;
    ReferenceT<RealTimeApplication> application;

    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        gam1 = godb->Find("Test.Functions.GAM1");
        ok = gam1.IsValid();
    }
    if (ok) {
        gam2 = godb->Find("Test.Functions.GAM2");
        ok = gam2.IsValid();
    }
    if (ok) {
        scheduler = godb->Find("Test.Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }

    if (ok) {
        ok = (ca_context_create(ca_enable_preemptive_callback));
    }
    const uint32 NUMBER_OF_PVS = 8u;
    chid pvChids[NUMBER_OF_PVS];
    chtype pvTypes[] = { DBR_CHAR, DBR_CHAR, DBR_SHORT, DBR_SHORT, DBR_LONG, DBR_LONG, DBR_FLOAT, DBR_DOUBLE };
    const char8 *pvNames[] = { "MARTe2::EPICSCAInput::Test::UInt8Arr", "MARTe2::EPICSCAInput::Test::Int8Arr", "MARTe2::EPICSCAInput::Test::UInt16Arr",
            "MARTe2::EPICSCAInput::Test::Int16Arr", "MARTe2::EPICSCAInput::Test::UInt32Arr", "MARTe2::EPICSCAInput::Test::Int32Arr",
            "MARTe2::EPICSCAInput::Test::Float32Arr", "MARTe2::EPICSCAInput::Test::Float64Arr" };

    uint8 *uint8Values = NULL;
    int8 *int8Values = NULL;
    uint16 *uint16Values = NULL;
    int16 *int16Values = NULL;
    uint32 *uint32Values = NULL;
    int32 *int32Values = NULL;
    float32 *float32Values = NULL;
    float64 *float64Values = NULL;
    uint32 numberOfElements = 0;
    if (ok) {
        numberOfElements = gam1->numberOfElements;
        uint8Values = new uint8[numberOfElements];
        int8Values = new int8[numberOfElements];
        uint16Values = new uint16[numberOfElements];
        int16Values = new int16[numberOfElements];
        uint32Values = new uint32[numberOfElements];
        int32Values = new int32[numberOfElements];
        float32Values = new float32[numberOfElements];
        float64Values = new float64[numberOfElements];
        uint32 n;
        for (n = 0u; (n < numberOfElements); n++) {
            uint8Values[n] = 2 * n + 2;
            int8Values[n] = 3 * n + 2;
            uint16Values[n] = 2 * n + 1;
            int16Values[n] = 3 * n + 1;
            uint32Values[n] = 4 * n + 1;
            int32Values[n] = 5 * n + 1;
            float32Values[n] = 6 * n + 1;
            float64Values[n] = 7 * n + 1;
        }
    }
    void *pvMemory[] = { uint8Values, int8Values, uint16Values, int16Values, uint32Values, int32Values, float32Values, float64Values };

    if (ok) {
        uint32 n;
        for (n = 0u; (n < NUMBER_OF_PVS) && (ok); n++) {
            /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
            ok = (ca_create_channel(pvNames[n], NULL_PTR(caCh *), NULL_PTR(void *), 20u, &pvChids[n]) == ECA_NORMAL);
        }
    }

    if (ok) {
        uint32 n;
        bool done = 0;
        uint32 timeOutCounts = 50;
        while ((!done) && (timeOutCounts > 0u) && (ok)) {
            for (n = 0u; (n < NUMBER_OF_PVS) && (ok); n++) {
                ca_array_put(pvTypes[n], numberOfElements, pvChids[n], pvMemory[n]);
            }
            if (ok) {
                ca_pend_io(1.0);
            }
            scheduler->ExecuteThreadCycle(0);

            done = true;
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam1->uint8Signal[n] == uint8Values[n]);
            }
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam1->uint16Signal[n] == uint16Values[n]);
            }
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam1->uint32Signal[n] == uint32Values[n]);
            }
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam1->float64Signal[n] == float64Values[n]);
            }
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam2->int8Signal[n] == int8Values[n]);
            }
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam2->int16Signal[n] == int16Values[n]);
            }
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam2->int32Signal[n] == int32Values[n]);
            }
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam2->float32Signal[n] == float32Values[n]);
            }
            if (!done) {
                timeOutCounts--;
                Sleep::Sec(0.1);
            }
        }
        if (ok) {
            ok = done;
        }
    }

    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        uint32 n;
        for (n = 0u; (n < NUMBER_OF_PVS); n++) {
            ca_clear_channel(pvChids[n]);
        }
    }
    ca_detach_context();
    ca_context_destroy();
    if (ok) {
        delete[] uint8Values;
        delete[] int8Values;
        delete[] uint16Values;
        delete[] int16Values;
        delete[] uint32Values;
        delete[] int32Values;
        delete[] float32Values;
        delete[] float64Values;
    }
    godb->Purge();
    return ok;
}

