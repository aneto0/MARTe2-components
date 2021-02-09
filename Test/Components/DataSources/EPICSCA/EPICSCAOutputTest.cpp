/**
 * @file EPICSCAOutputTest.cpp
 * @brief Source file for class EPICSCAOutputTest
 * @date 04/02/2021
 * @author Andre Neto
 * @author Pedro Lourenco
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
 * the class EPICSCAOutputTest (public, protected, and private). Be aware that some
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
#include "EPICSCAOutput.h"
#include "EPICSCAOutputTest.h"
#include "GAM.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief GAM which writes signals into a given EPICSCAOutput
 */
class EPICSCAOutputGAMTestHelper: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()EPICSCAOutputGAMTestHelper() {
        stringSignal = NULL;
        char8Signal = NULL;
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

    virtual ~EPICSCAOutputGAMTestHelper() {
        using namespace MARTe;
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        GetSignalNumberOfElements(OutputSignals, 0, numberOfElements);
        uint32 n;
        uint32 numberOfSignals = GetNumberOfOutputSignals();
        for(n=0; n<numberOfSignals; n++) {
            if (GetSignalType(OutputSignals, n) == UnsignedInteger16Bit) {
                uint16Signal = reinterpret_cast<uint16 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == SignedInteger16Bit) {
                int16Signal = reinterpret_cast<int16 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == UnsignedInteger32Bit) {
                uint32Signal = reinterpret_cast<uint32 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == SignedInteger32Bit) {
                int32Signal = reinterpret_cast<int32 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == Float32Bit) {
                float32Signal = reinterpret_cast<float32 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == Character8Bit) {
                char8Signal = reinterpret_cast<char8 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == CharString) {
                stringSignal = reinterpret_cast<char8 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == UnsignedInteger8Bit) {
                uint8Signal = reinterpret_cast<uint8 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == SignedInteger8Bit) {
                int8Signal = reinterpret_cast<int8 *>(GetOutputSignalMemory(n));
            }
            else {
                float64Signal = reinterpret_cast<float64 *>(GetOutputSignalMemory(n));
            }
        }

        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        return true;
    }

    MARTe::char8 *stringSignal;
    MARTe::char8 *char8Signal;
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
CLASS_REGISTER(EPICSCAOutputGAMTestHelper, "1.0")

/**
 * @brief Manual scheduler to test the correct interface between the EPICSCAOutput and the GAMs
 */
class EPICSCAOutputSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

    EPICSCAOutputSchedulerTestHelper () : MARTe::GAMSchedulerI() {
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

CLASS_REGISTER(EPICSCAOutputSchedulerTestHelper, "1.0")

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
        "            Class = EPICSCAOutputGAMTestHelper"
        "            OutputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalChar8 = {"
        "                    Type = char8"
        "                    DataSource = EPICSCAOutputTest"
        "                    NumberOfElements = 40"
        "                }"
        "                SignalString = {"
        "                    Type = string"
        "                    DataSource = EPICSCAOutputTest"
        "                    NumberOfElements = 40"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAOutputTest"
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
        "        +EPICSCAOutputTest = {"
        "            Class = EPICSCAOutput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            NumberOfBuffers = 8"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = EPICSCAOutputSchedulerTestHelper"
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
        "            Class = EPICSCAOutputGAMTestHelper"
        "            OutputSignals = {"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAOutputTest"
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
        "        +EPICSCAOutputTest = {"
        "            Class = EPICSCAOutput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            NumberOfBuffers = 8"
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
        "        +EPICSCAOutputTestNoSignals = {"
        "            Class = EPICSCAOutput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            NumberOfBuffers = 8"
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
        "        Class = EPICSCAOutputSchedulerTestHelper"
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
        "            Class = EPICSCAOutputGAMTestHelper"
        "            OutputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    DataSource = EPICSCAOutputTest"
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
        "        +EPICSCAOutputTest = {"
        "            Class = EPICSCAOutput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            NumberOfBuffers = 8"
        "            Signals = {"
        "                SignalUInt8 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt8Arr\""
        "                }"
        "                SignalInt8 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::Int8Arr\""
        "                }"
        "                SignalUInt16 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt16Arr\""
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = EPICSCAOutputSchedulerTestHelper"
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
        "            Class = EPICSCAOutputGAMTestHelper"
        "            OutputSignals = {"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAOutputTest"
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
        "        +EPICSCAOutputTest = {"
        "            Class = EPICSCAOutput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            NumberOfBuffers = 8"
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
        "        Class = EPICSCAOutputSchedulerTestHelper"
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
        "            Class = EPICSCAOutputGAMTestHelper"
        "            OutputSignals = {"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    Samples = 2"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAOutputTest"
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
        "        +EPICSCAOutputTest = {"
        "            Class = EPICSCAOutput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            NumberOfBuffers = 8"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = EPICSCAOutputSchedulerTestHelper"
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
        "            Class = EPICSCAOutputGAMTestHelper"
        "            OutputSignals = {"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAOutputTest"
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
        "        +EPICSCAOutputTest = {"
        "            Class = EPICSCAOutput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            NumberOfBuffers = 8"
        "            Signals = {"
        "                SignalUInt64 = {"
        "                    PVName = \"MARTe2::EPICSCAInput::Test::UInt8\""
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = EPICSCAOutputSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Standard configuration with more than one GAM interacting with the data source.
static const MARTe::char8 * const config7 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = EPICSCAOutputGAMTestHelper"
        "            OutputSignals = {"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = EPICSCAOutputGAMTestHelper"
        "            OutputSignals = {"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = EPICSCAOutputTest"
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
        "        +EPICSCAOutputTest = {"
        "            Class = EPICSCAOutput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            NumberOfBuffers = 8"
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
        "        Class = EPICSCAOutputSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Standard configuration
static const MARTe::char8 * const config8 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = EPICSCAOutputGAMTestHelper"
        "            OutputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalChar8 = {"
        "                    Type = char8"
        "                    DataSource = EPICSCAOutputTest"
        "                    NumberOfElements = 39"
        "                }"
        "                SignalString = {"
        "                    Type = string"
        "                    DataSource = EPICSCAOutputTest"
        "                    NumberOfElements = 40"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat64 = {"
        "                    Type = float64"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = EPICSCAOutputTest"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = EPICSCAOutputTest"
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
        "        +EPICSCAOutputTest = {"
        "            Class = EPICSCAOutput"
        "            CPUMask = 15"
        "            StackSize = 10000000"
        "            NumberOfBuffers = 8"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = EPICSCAOutputSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool EPICSCAOutputTest::TestConstructor() {
    using namespace MARTe;
    EPICSCAOutput test;
    bool ok = (test.GetCPUMask() == 0xff);
    ok &= (test.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 4u));
    return ok;
}

bool EPICSCAOutputTest::TestAllocateMemory() {
    using namespace MARTe;
    EPICSCAOutput test;
    return test.AllocateMemory();
}

bool EPICSCAOutputTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    EPICSCAOutput test;
    return (test.GetNumberOfMemoryBuffers() == 1);
}

bool EPICSCAOutputTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    EPICSCAOutput test;
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

bool EPICSCAOutputTest::TestGetBrokerName_InputSignals() {
    using namespace MARTe;
    EPICSCAOutput test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, InputSignals), "") == 0);

    return ok;
}

bool EPICSCAOutputTest::TestGetBrokerName() {
    using namespace MARTe;
    EPICSCAOutput test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, OutputSignals), "MemoryMapAsyncOutputBroker") == 0);

    return ok;
}

bool EPICSCAOutputTest::TestGetInputBrokers() {
    using namespace MARTe;
    EPICSCAOutput test;
    ReferenceContainer rc;
    return !test.GetInputBrokers(rc, "", NULL);
}

bool EPICSCAOutputTest::TestGetOutputBrokers() {
    return TestIntegratedInApplication(config1, true);
}

bool EPICSCAOutputTest::TestInitialise() {
    using namespace MARTe;
    EPICSCAOutput test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 1);
    cdb.Write("StackSize", 100000);
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("IgnoreBufferOverrun", 0);
    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (test.GetCPUMask() == 1);
        ok &= (test.GetStackSize() == 100000);
        ok &= (test.GetNumberOfBuffers() == 10);
        ok &= (!test.IsIgnoringBufferOverrun());
    }
    return ok;
}

bool EPICSCAOutputTest::TestInitialise_Defaults() {
    using namespace MARTe;
    EPICSCAOutput test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 11);
    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (test.GetCPUMask() == 0xff);
        ok &= (test.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 4u));
        ok &= (test.GetNumberOfBuffers() == 11);
        ok &= (test.IsIgnoringBufferOverrun());
    }
    return ok;
}

bool EPICSCAOutputTest::TestInitialise_False_Signals() {
    using namespace MARTe;
    EPICSCAOutput test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 1);
    cdb.Write("StackSize", 100000);
    cdb.Write("NumberOfBuffers", 11);
    return !test.Initialise(cdb);
}

bool EPICSCAOutputTest::TestInitialise_False_NumberOfBuffers() {
    using namespace MARTe;
    EPICSCAOutput test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 1);
    cdb.Write("StackSize", 100000);
    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool EPICSCAOutputTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config1, true);
}

bool EPICSCAOutputTest::TestSetConfiguredDatabase_False_NoSignals() {
    return !TestIntegratedInApplication(config2, true);
}

bool EPICSCAOutputTest::TestSetConfiguredDatabase_False_PVName() {
    return !TestIntegratedInApplication(config4, true);
}

bool EPICSCAOutputTest::TestSetConfiguredDatabase_False_Samples() {
    return !TestIntegratedInApplication(config5, true);
}

bool EPICSCAOutputTest::TestSetConfiguredDatabase_False_UnsupportedType() {
    return !TestIntegratedInApplication(config6, true);
}

bool EPICSCAOutputTest::TestSetConfiguredDatabase_False_MoreThanOneGAM() {
    return !TestIntegratedInApplication(config7, true);
}

bool EPICSCAOutputTest::TestSetConfiguredDatabase_False_WrongStringSize() {
    return !TestIntegratedInApplication(config8, true);
}

bool EPICSCAOutputTest::TestGetCPUMask() {
    return TestInitialise();
}

bool EPICSCAOutputTest::TestGetStackSize() {
    return TestInitialise();
}

bool EPICSCAOutputTest::TestGetNumberOfBuffers() {
    return TestInitialise();
}

bool EPICSCAOutputTest::TestIsIgnoringBufferOverrun() {
    return TestInitialise();
}

bool EPICSCAOutputTest::TestAsyncCaPut() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString err;
    ObjectRegistryDatabase *godb;
    ReferenceT<RealTimeApplication> application;
    ReferenceT<EPICSCAOutputSchedulerTestHelper> scheduler;
    ReferenceT<EPICSCAOutput> testData;
    StreamString returnedValue;

    const uint32 numberOfPVs = 10;
    StreamString pvName [numberOfPVs] = { "MARTe2::EPICSCAInput::Test::Char8", "MARTe2::EPICSCAInput::Test::String",
            "MARTe2::EPICSCAInput::Test::UInt8", "MARTe2::EPICSCAInput::Test::Int8", "MARTe2::EPICSCAInput::Test::UInt16",
            "MARTe2::EPICSCAInput::Test::Int16", "MARTe2::EPICSCAInput::Test::UInt32", "MARTe2::EPICSCAInput::Test::Int32",
            "MARTe2::EPICSCAInput::Test::Float32", "MARTe2::EPICSCAInput::Test::Float64" };
    StreamString pvValue [numberOfPVs] = { "F", "SECOND",
            "128", "-64", "32768",
            "-16384", "2147483648", "-1073741824",
            "-1.2E+8", "-1.2E+16" };
    chtype pvTypes[numberOfPVs] = { DBR_STRING, DBR_STRING,
            DBR_CHAR, DBR_CHAR, DBR_SHORT,
            DBR_SHORT, DBR_LONG, DBR_LONG,
            DBR_FLOAT, DBR_DOUBLE };
    chid pvChids[numberOfPVs];
    char8 char8Value[40];
    char8 stringValue[40];
    uint8 uint8Value = 0;
    int8 int8Value = 0;
    uint16 uint16Value = 0;
    int16 int16Value = 0;
    uint32 uint32Value = 0;
    int32 int32Value = 0;
    float32 float32Value = 0;
    float64 float64Value = 0;
    void *pvMemory[numberOfPVs] = { &char8Value[0], &stringValue[0],
            &uint8Value, &int8Value, &uint16Value,
            &int16Value, &uint32Value, &int32Value,
            &float32Value, &float64Value };

    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb, &err);

    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "%s", err.Buffer());
    }
    godb = ObjectRegistryDatabase::Instance();
    if (ok) {
        godb->Purge();
        ok = godb->Initialise(cdb);
    }
    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        scheduler = godb->Find("Test.Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        testData = godb->Find("Test.Data.EPICSCAOutputTest");
        ok = testData.IsValid();
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
    if (ok) {
        scheduler->ExecuteThreadCycle(0);
        //AsyncCaPut
        for (uint32 idx = 0u; idx < numberOfPVs; idx++) {
            if (ok) {
                Sleep::Sec(0.1);
                ok = (testData->AsyncCaPut(pvName[idx], pvValue[idx]) ==  ErrorManagement::NoError);
                REPORT_ERROR_STATIC(ErrorManagement::Information, "AsyncCaPut(%s, %s) %s", pvName[idx].Buffer(), 
                    pvValue[idx].Buffer(), (ok) ? "OK" : "ERROR");
                Sleep::Sec(0.1);
            }
        }
        //Check the values
        if (ok) {
            for (uint32 idx = 0u; idx < numberOfPVs; idx++) {
                if (ok) {
                    ok = (ca_create_channel(pvName[idx].Buffer(), NULL_PTR(caCh *), NULL_PTR(void *), 20u, &pvChids[idx]) == ECA_NORMAL);
                    Sleep::Sec(0.1);
                }
                if (ok) {
                    ok = (ca_get(pvTypes[idx], pvChids[idx], pvMemory[idx]) == ECA_NORMAL);
                }
                if (ok) {
                    (void) ca_pend_io(1.0);
                    Sleep::Sec(0.1);
                }
                if (ok) {
                    returnedValue.Seek(0ul);
                    returnedValue.SetSize(0ul);
                    switch (idx) {
                        case 0 :
                            returnedValue.Printf("%!", char8Value);
                            break;
                        case 1 :
                            returnedValue.Printf("%!", stringValue);
                            break;
                        case 2 :
                            returnedValue.Printf("%!", uint8Value);
                            break;
                        case 3 :
                            returnedValue.Printf("%!", int8Value);
                            break;
                        case 4 :
                            returnedValue.Printf("%!", uint16Value);
                            break;
                        case 5 :
                            returnedValue.Printf("%!", int16Value);
                            break;
                        case 6 :
                            returnedValue.Printf("%!", uint32Value);
                            break;
                        case 7 :
                            returnedValue.Printf("%!", int32Value);
                            break;
                        case 8 :
                            returnedValue.Printf("%.2e", float32Value);
                            break;
                        case 9 :
                            returnedValue.Printf("%.2e", float64Value);
                            break;
                        default:
                            returnedValue.Printf("%!", stringValue);
                    }
                    ok = ( pvValue[idx] == returnedValue);
                    REPORT_ERROR_STATIC(ErrorManagement::Information, "ca_get(%s) = %s, match %s ", pvName[idx].Buffer(),
                        returnedValue.Buffer(), (ok) ? "OK" : "ERROR" );
                }
            }
        }
    }
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (ok) {
        for (uint32 idx = 0u; idx < numberOfPVs; idx++) {
            ca_clear_channel(pvChids[idx]);
        }
    }
    ca_detach_context();
    ca_context_destroy();
    godb->Purge();
    return ok;
}

bool EPICSCAOutputTest::TestExecute() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config1, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<EPICSCAOutputGAMTestHelper> gam1;
    ReferenceT<EPICSCAOutputSchedulerTestHelper> scheduler;
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
    const char8 *pvNames[] = { "MARTe2::EPICSCAInput::Test::String", "MARTe2::EPICSCAInput::Test::Char8", "MARTe2::EPICSCAInput::Test::UInt8",
            "MARTe2::EPICSCAInput::Test::Int8", "MARTe2::EPICSCAInput::Test::UInt16", "MARTe2::EPICSCAInput::Test::Int16", "MARTe2::EPICSCAInput::Test::UInt32",
            "MARTe2::EPICSCAInput::Test::Int32", "MARTe2::EPICSCAInput::Test::Float32", "MARTe2::EPICSCAInput::Test::Float64" };
    char8 char8Value[40];
    char8 stringValue[40];
    uint8 uint8Value = 0;
    int8 int8Value = 0;
    uint32 uint16Value = 0;
    int32 int16Value = 0;
    uint32 uint32Value = 0;
    int32 int32Value = 0;
    float32 float32Value = 0;
    float64 float64Value = 0;
    void *pvMemory[] = { &stringValue[0], &char8Value[0], &uint8Value, &int8Value, &uint16Value, &int16Value, &uint32Value, &int32Value, &float32Value,
            &float64Value };

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
        uint32 doneC = 0;
        uint32 timeOutCounts = 50;
        StringHelper::Copy(&gam1->char8Signal[0], "EPICSCATEST");
        StringHelper::Copy(&gam1->stringSignal[0], "EPICSCATESTSTR");
        *gam1->uint8Signal = 11;
        *gam1->int8Signal = 12;
        *gam1->uint16Signal = 1;
        *gam1->uint32Signal = 2;
        *gam1->float64Signal = 3;
        *gam1->int16Signal = 4;
        *gam1->int32Signal = 5;
        *gam1->float32Signal = 6;

        while ((!done) && (timeOutCounts > 0u) && (ok)) {
            scheduler->ExecuteThreadCycle(0);

            for (n = 0u; (n < NUMBER_OF_PVS) && (ok); n++) {
                ca_get(pvTypes[n], pvChids[n], pvMemory[n]);
            }
            if (ok) {
                ca_pend_io(1.0);
            }

            StreamString tmpChar8 = &char8Value[0];
            StreamString tmpString = &stringValue[0];
            done = (*gam1->uint16Signal == uint16Value);
            done &= (*gam1->uint32Signal == uint32Value);
            done &= (*gam1->float64Signal == float64Value);
            done &= (*gam1->int16Signal == int16Value);
            done &= (*gam1->int32Signal == int32Value);
            done &= (*gam1->float32Signal == float32Value);
            done &= (*gam1->int8Signal == int8Value);
            done &= (*gam1->uint8Signal == uint8Value);
            done &= (tmpChar8 == gam1->char8Signal);
            done &= (tmpString == gam1->stringSignal);

            if (!done) {
                timeOutCounts--;
                Sleep::Sec(0.1);
            }
            if ((done) && (doneC == 0)) {
                *gam1->uint16Signal *= 2;
                *gam1->uint32Signal *= 2;
                *gam1->float64Signal *= 2;
                *gam1->int16Signal *= 2;
                *gam1->int32Signal *= 2;
                *gam1->float32Signal *= 2;
                *gam1->int8Signal *= 2;
                *gam1->uint8Signal *= 2;
                gam1->char8Signal[0] = 'A';
                gam1->stringSignal[0] = 'B';
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

bool EPICSCAOutputTest::TestExecute_Arrays() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config3, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<EPICSCAOutputGAMTestHelper> gam1;
    ReferenceT<EPICSCAOutputSchedulerTestHelper> scheduler;
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
        uint32Values = new uint32[numberOfElements];
        int32Values = new int32[numberOfElements];
        uint16Values = new uint16[numberOfElements];
        int16Values = new int16[numberOfElements];
        uint8Values = new uint8[numberOfElements];
        int8Values = new int8[numberOfElements];
        float32Values = new float32[numberOfElements];
        float64Values = new float64[numberOfElements];
        uint32 n;
        for (n = 0u; (n < numberOfElements); n++) {
            gam1->uint16Signal[n] = 2 * n + 1;
            gam1->int16Signal[n] = 3 * n + 1;
            gam1->uint32Signal[n] = 4 * n + 1;
            gam1->int32Signal[n] = 5 * n + 1;
            gam1->float32Signal[n] = 6 * n + 1;
            gam1->float64Signal[n] = 7 * n + 1;
            gam1->uint8Signal[n] = 10 * n + 1;
            gam1->int8Signal[n] = 9 * n + 1;
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
                ca_array_get(pvTypes[n], numberOfElements, pvChids[n], pvMemory[n]);
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
                done &= (gam1->int8Signal[n] == int8Values[n]);
            }
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam1->int16Signal[n] == int16Values[n]);
            }
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam1->int32Signal[n] == int32Values[n]);
            }
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam1->float32Signal[n] == float32Values[n]);
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

