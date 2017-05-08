/**
 * @file RealTimeThreadSynchronisationTest.cpp
 * @brief Source file for class RealTimeThreadSynchronisationTest
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

 * @details This source file contains the definition of all the methods for
 * the class RealTimeThreadSynchronisationTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "Directory.h"
#include "DirectoryScanner.h"
#include "RealTimeThreadSynchronisation.h"
#include "RealTimeThreadSynchronisationTest.h"

#include "GAM.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief GAM which is capable of reading and writing signals from/to a given RealTimeThreadSynchronisation
 */
class RealTimeThreadSynchronisationGAMTestHelper: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()RealTimeThreadSynchronisationGAMTestHelper() {
        uint32Signal = NULL;
        uint16Signal = NULL;
        uint64Signal = NULL;
        int32Signal = NULL;
        uint16SignalElements = 0u;
        uint32SignalElements = 0u;
        uint64SignalElements = 0u;
        int32SignalElements = 0u;
        uint16SignalSamples = 0u;
        uint32SignalSamples = 0u;
        uint64SignalSamples = 0u;
        int32SignalSamples = 0u;
    }

    virtual ~RealTimeThreadSynchronisationGAMTestHelper() {
        using namespace MARTe;
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        uint32 n;
        uint32 numberOfSignals = GetNumberOfOutputSignals();
        for(n=0; n<numberOfSignals; n++) {
            if (GetSignalType(OutputSignals, n) == UnsignedInteger16Bit) {
                uint16Signal = reinterpret_cast<uint16 *>(GetOutputSignalMemory(n));
                GetSignalNumberOfElements(OutputSignals, n, uint16SignalElements);
                GetSignalNumberOfSamples(OutputSignals, n, uint16SignalSamples);
            }
            else if (GetSignalType(OutputSignals, n) == UnsignedInteger32Bit) {
                uint32Signal = reinterpret_cast<uint32 *>(GetOutputSignalMemory(n));
                GetSignalNumberOfElements(OutputSignals, n, uint32SignalElements);
                GetSignalNumberOfSamples(OutputSignals, n, uint32SignalSamples);
            }
            else if (GetSignalType(OutputSignals, n) == UnsignedInteger64Bit) {
                uint64Signal = reinterpret_cast<uint64 *>(GetOutputSignalMemory(n));
                GetSignalNumberOfElements(OutputSignals, n, uint64SignalElements);
                GetSignalNumberOfSamples(OutputSignals, n, uint64SignalSamples);
            }
            else if (GetSignalType(OutputSignals, n) == SignedInteger32Bit) {
                int32Signal = reinterpret_cast<int32 *>(GetOutputSignalMemory(n));
                GetSignalNumberOfElements(OutputSignals, n, int32SignalElements);
                GetSignalNumberOfSamples(OutputSignals, n, int32SignalSamples);
            }
        }
        numberOfSignals = GetNumberOfInputSignals();
        for(n=0; n<numberOfSignals; n++) {
            if (GetSignalType(InputSignals, n) == UnsignedInteger16Bit) {
                uint16Signal = reinterpret_cast<uint16 *>(GetInputSignalMemory(n));
                GetSignalNumberOfElements(InputSignals, n, uint16SignalElements);
                GetSignalNumberOfSamples(InputSignals, n, uint16SignalSamples);
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger32Bit) {
                uint32Signal = reinterpret_cast<uint32 *>(GetInputSignalMemory(n));
                GetSignalNumberOfElements(InputSignals, n, uint32SignalElements);
                GetSignalNumberOfSamples(InputSignals, n, uint32SignalSamples);
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger64Bit) {
                uint64Signal = reinterpret_cast<uint64 *>(GetInputSignalMemory(n));
                GetSignalNumberOfElements(InputSignals, n, uint64SignalElements);
                GetSignalNumberOfSamples(InputSignals, n, uint64SignalSamples);
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger32Bit) {
                int32Signal = reinterpret_cast<int32 *>(GetInputSignalMemory(n));
                GetSignalNumberOfElements(InputSignals, n, int32SignalElements);
                GetSignalNumberOfSamples(InputSignals, n, int32SignalSamples);
            }
        }

        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        return true;
    }

    MARTe::uint16 *uint16Signal;
    MARTe::uint32 *uint32Signal;
    MARTe::uint64 *uint64Signal;
    MARTe::int32 *int32Signal;
    MARTe::uint32 uint16SignalElements;
    MARTe::uint32 uint32SignalElements;
    MARTe::uint32 uint64SignalElements;
    MARTe::uint32 int32SignalElements;
    MARTe::uint32 uint16SignalSamples;
    MARTe::uint32 uint32SignalSamples;
    MARTe::uint32 uint64SignalSamples;
    MARTe::uint32 int32SignalSamples;
};
CLASS_REGISTER(RealTimeThreadSynchronisationGAMTestHelper, "1.0")

/**
 * @brief Manual scheduler to test the correct interface between the RealTimeThreadSynchronisation and the GAMs
 */
class RealTimeThreadSynchronisationSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

RealTimeThreadSynchronisationSchedulerTestHelper    () : MARTe::GAMSchedulerI() {
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
        ExecuteSingleCycle(scheduledStates[RealTimeApplication::GetIndex()]->threads[threadId].executables,
                scheduledStates[RealTimeApplication::GetIndex()]->threads[threadId].numberOfExecutables);
    }

    virtual bool ConfigureScheduler() {

        bool ret = GAMSchedulerI::ConfigureScheduler();
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

CLASS_REGISTER(RealTimeThreadSynchronisationSchedulerTestHelper, "1.0")

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
        "        +GAM1Thread1 = {"
        "            Class = RealTimeThreadSynchronisationGAMTestHelper"
        "            OutputSignals = {"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 3"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 5"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                }"
        "            }"
        "        }"
        "        +GAM1Thread2 = {"
        "            Class = RealTimeThreadSynchronisationGAMTestHelper"
        "            InputSignals = {"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 3"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 5"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                }"
        "            }"
        "        }"
        "        +GAM1Thread3 = {"
        "            Class = RealTimeThreadSynchronisationGAMTestHelper"
        "            InputSignals = {"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    Samples = 2"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 3"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    Samples = 2"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    Samples = 2"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 5"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    Samples = 2"
        "                }"
        "            }"
        "        }"
        "        +GAM1Thread4 = {"
        "            Class = RealTimeThreadSynchronisationGAMTestHelper"
        "            InputSignals = {"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    Samples = 4"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 3"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    Samples = 4"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    Samples = 4"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 5"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    Samples = 4"
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
        "        +RealTimeThreadSynchronisationTest = {"
        "            Class = RealTimeThreadSynchronisation"
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
        "                    Functions = {GAM1Thread1}"
        "                }"
        "                +Thread2 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1Thread2}"
        "                }"
        "                +Thread3 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1Thread3}"
        "                }"
        "                +Thread4 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1Thread4}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = RealTimeThreadSynchronisationSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

#if 0
//Standard configuration no signals
static const MARTe::char8 * const config2 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = RealTimeThreadSynchronisationGAMTestHelper"
"            InputSignals = {"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                    DataSource = RealTimeThreadSynchronisationTest"
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
"        +RealTimeThreadSynchronisationTest = {"
"            Class = RealTimeThreadSynchronisation"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Signals = {"
"                SignalUInt32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::UInt32\""
"                }"
"                SignalInt32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Int32\""
"                }"
"                SignalFloat32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Float32\""
"                }"
"                SignalFloat64 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Float64\""
"                }"
"            }"
"        }"
"        +RealTimeThreadSynchronisationTestNoSignals = {"
"            Class = RealTimeThreadSynchronisation"
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
"        Class = RealTimeThreadSynchronisationSchedulerTestHelper"
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
"            Class = RealTimeThreadSynchronisationGAMTestHelper"
"            InputSignals = {"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    NumberOfDimensions = 1"
"                    NumberOfElements = 10"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    NumberOfDimensions = 1"
"                    NumberOfElements = 10"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                    NumberOfDimensions = 1"
"                    NumberOfElements = 10"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"            }"
"        }"
"        +GAM2 = {"
"            Class = RealTimeThreadSynchronisationGAMTestHelper"
"            InputSignals = {"
"                SignalInt16 = {"
"                    Type = int16"
"                    NumberOfDimensions = 1"
"                    NumberOfElements = 10"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                    NumberOfDimensions = 1"
"                    NumberOfElements = 10"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                    NumberOfDimensions = 1"
"                    NumberOfElements = 10"
"                    DataSource = RealTimeThreadSynchronisationTest"
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
"        +RealTimeThreadSynchronisationTest = {"
"            Class = RealTimeThreadSynchronisation"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Signals = {"
"                SignalUInt16 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::UInt16Arr\""
"                }"
"                SignalInt16 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Int16Arr\""
"                }"
"                SignalUInt32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::UInt32Arr\""
"                }"
"                SignalInt32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Int32Arr\""
"                }"
"                SignalFloat32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Float32Arr\""
"                }"
"                SignalFloat64 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Float64Arr\""
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
"        Class = RealTimeThreadSynchronisationSchedulerTestHelper"
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
"            Class = RealTimeThreadSynchronisationGAMTestHelper"
"            InputSignals = {"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                    DataSource = RealTimeThreadSynchronisationTest"
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
"        +RealTimeThreadSynchronisationTest = {"
"            Class = RealTimeThreadSynchronisation"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Signals = {"
"                SignalUInt32 = {"
"                    Type = uint32"
"                }"
"                SignalInt32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Int32\""
"                }"
"                SignalFloat32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Float32\""
"                }"
"                SignalFloat64 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Float64\""
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
"        Class = RealTimeThreadSynchronisationSchedulerTestHelper"
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
"            Class = RealTimeThreadSynchronisationGAMTestHelper"
"            InputSignals = {"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    Samples = 2"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"            }"
"        }"
"        +GAM2 = {"
"            Class = RealTimeThreadSynchronisationGAMTestHelper"
"            InputSignals = {"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                    DataSource = RealTimeThreadSynchronisationTest"
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
"        +RealTimeThreadSynchronisationTest = {"
"            Class = RealTimeThreadSynchronisation"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Signals = {"
"                SignalUInt16 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::UInt16\""
"                }"
"                SignalInt16 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Int16\""
"                }"
"                SignalUInt32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::UInt32\""
"                }"
"                SignalInt32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Int32\""
"                }"
"                SignalFloat32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Float32\""
"                }"
"                SignalFloat64 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Float64\""
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
"        Class = RealTimeThreadSynchronisationSchedulerTestHelper"
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
"            Class = RealTimeThreadSynchronisationGAMTestHelper"
"            InputSignals = {"
"                SignalUInt8 = {"
"                    Type = uint8"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"            }"
"        }"
"        +GAM2 = {"
"            Class = RealTimeThreadSynchronisationGAMTestHelper"
"            InputSignals = {"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                    DataSource = RealTimeThreadSynchronisationTest"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                    DataSource = RealTimeThreadSynchronisationTest"
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
"        +RealTimeThreadSynchronisationTest = {"
"            Class = RealTimeThreadSynchronisation"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Signals = {"
"                SignalUInt8 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::UInt8\""
"                }"
"                SignalInt16 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Int16\""
"                }"
"                SignalUInt32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::UInt32\""
"                }"
"                SignalInt32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Int32\""
"                }"
"                SignalFloat32 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Float32\""
"                }"
"                SignalFloat64 = {"
"                    PVName = \"MARTe2::RealTimeThreadSynchronisation::Test::Float64\""
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
"        Class = RealTimeThreadSynchronisationSchedulerTestHelper"
"        TimingDataSource = Timings"
"    }"
"}";
#endif

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool RealTimeThreadSynchronisationTest::TestConstructor() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
    /*bool ok = (test.GetCPUMask() == 0xff);
     ok &= (test.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 4u));*/
    return true;
}

#if 0
bool RealTimeThreadSynchronisationTest::TestAllocateMemory() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
    return test.AllocateMemory();
}

bool RealTimeThreadSynchronisationTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
    return (test.GetNumberOfMemoryBuffers() == 1);
}

bool RealTimeThreadSynchronisationTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
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

bool RealTimeThreadSynchronisationTest::TestGetBrokerName_OutputSignals() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, OutputSignals), "") == 0);

    return ok;
}

bool RealTimeThreadSynchronisationTest::TestGetBrokerName() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, InputSignals), "MemoryMapInputBroker") == 0);

    return ok;
}

bool RealTimeThreadSynchronisationTest::TestGetInputBrokers() {
    return TestIntegratedInApplication(config1, true);
}

bool RealTimeThreadSynchronisationTest::TestGetOutputBrokers() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
    ReferenceContainer rc;
    return !test.GetOutputBrokers(rc, "", NULL);
}

bool RealTimeThreadSynchronisationTest::TestInitialise() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
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

bool RealTimeThreadSynchronisationTest::TestInitialise_Defaults() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
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

bool RealTimeThreadSynchronisationTest::TestInitialise_False_Signals() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 1);
    cdb.Write("StackSize", 100000);
    return !test.Initialise(cdb);
}

bool RealTimeThreadSynchronisationTest::TestGetStackSize() {
    return TestInitialise();
}

bool RealTimeThreadSynchronisationTest::TestGetCPUMask() {
    return TestInitialise();
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config1, true);
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase_False_NoSignals() {
    return !TestIntegratedInApplication(config2, true);
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase_False_PVName() {
    return !TestIntegratedInApplication(config4, true);
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase_False_Samples() {
    return !TestIntegratedInApplication(config5, true);
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase_False_UnsupportedType() {
    return !TestIntegratedInApplication(config6, true);
}
#endif
#include <stdio.h>
bool RealTimeThreadSynchronisationTest::TestExecute() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config1, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<RealTimeThreadSynchronisationGAMTestHelper> gam1Thread1;
    ReferenceT<RealTimeThreadSynchronisationGAMTestHelper> gam1Thread2;
    ReferenceT<RealTimeThreadSynchronisationGAMTestHelper> gam1Thread3;
    ReferenceT<RealTimeThreadSynchronisationGAMTestHelper> gam1Thread4;
    ReferenceT<RealTimeThreadSynchronisationSchedulerTestHelper> scheduler;
    ReferenceT<RealTimeApplication> application;

    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        gam1Thread1 = godb->Find("Test.Functions.GAM1Thread1");
        ok = gam1Thread1.IsValid();
    }
    if (ok) {
        gam1Thread2 = godb->Find("Test.Functions.GAM1Thread2");
        ok = gam1Thread2.IsValid();
    }
    if (ok) {
        gam1Thread3 = godb->Find("Test.Functions.GAM1Thread3");
        ok = gam1Thread3.IsValid();
    }
    if (ok) {
        gam1Thread4 = godb->Find("Test.Functions.GAM1Thread4");
        ok = gam1Thread4.IsValid();
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

    const uint32 numberOfExecutions = 12u;
    uint32 j;
    for (j = 0u; (j < numberOfExecutions) && (ok); j++) {
        uint32 e;
        for (e = 0u; (e < gam1Thread1->uint16SignalElements); e++) {
            gam1Thread1->uint16Signal[e] = (j + e);
        }
        for (e = 0u; (e < gam1Thread1->uint32SignalElements); e++) {
            gam1Thread1->uint32Signal[e] = (j + e);
        }
        for (e = 0u; (e < gam1Thread1->uint64SignalElements); e++) {
            gam1Thread1->uint64Signal[e] = (j + e);
        }
        for (e = 0u; (e < gam1Thread1->int32SignalElements); e++) {
            gam1Thread1->int32Signal[e] = (j + e);
        }
        scheduler->ExecuteThreadCycle(0);
        scheduler->ExecuteThreadCycle(1);

        //Thread 2 should always have the same values of thread 1
        for (e = 0u; (e < gam1Thread1->uint16SignalElements) && (ok); e++) {
            ok = (gam1Thread1->uint16Signal[e] == gam1Thread2->uint16Signal[e]);
        }
        for (e = 0u; (e < gam1Thread1->uint32SignalElements) && (ok); e++) {
            ok = (gam1Thread1->uint32Signal[e] == gam1Thread2->uint32Signal[e]);
        }
        for (e = 0u; (e < gam1Thread1->uint64SignalElements) && (ok); e++) {
            ok = (gam1Thread1->uint64Signal[e] == gam1Thread2->uint64Signal[e]);
        }
        for (e = 0u; (e < gam1Thread1->int32SignalElements) && (ok); e++) {
            ok = (gam1Thread1->int32Signal[e] == gam1Thread2->int32Signal[e]);
        }
        //Thread 3 should store 2 samples of each signal
        if (((j + 1) % 2) == 0) {
            scheduler->ExecuteThreadCycle(2);
            uint32 s;
            for (s = 0; (s < gam1Thread3->uint16SignalSamples) && (ok); s++) {
                for (e = 0u; (e < gam1Thread3->uint16SignalElements) && (ok); e++) {
                    ok = (gam1Thread3->uint16Signal[s * gam1Thread3->uint16SignalElements + e] == (j + s - (gam1Thread3->uint16SignalSamples - 1) + e));
                }
            }
            for (s = 0; (s < gam1Thread3->uint32SignalSamples) && (ok); s++) {
                for (e = 0u; (e < gam1Thread3->uint32SignalElements) && (ok); e++) {
                    ok = (gam1Thread3->uint32Signal[s * gam1Thread3->uint32SignalElements + e] == (j + s - (gam1Thread3->uint32SignalSamples - 1) + e));
                }
            }
            for (s = 0; (s < gam1Thread3->uint64SignalSamples) && (ok); s++) {
                for (e = 0u; (e < gam1Thread3->uint64SignalElements) && (ok); e++) {
                    ok = (gam1Thread3->uint64Signal[s * gam1Thread3->uint64SignalElements + e] == (j + s - (gam1Thread3->uint64SignalSamples - 1) + e));
                }
            }
            for (s = 0; (s < gam1Thread3->int32SignalSamples) && (ok); s++) {
                for (e = 0u; (e < gam1Thread3->int32SignalElements) && (ok); e++) {
                    ok = (gam1Thread3->int32Signal[s * gam1Thread3->int32SignalElements + e] == static_cast<int32>(j + s - (gam1Thread3->int32SignalSamples - 1) + e));
                }
            }
        }
        //Thread 4 should store 4 samples of each signal
        if (((j + 1) % 4) == 0) {
            scheduler->ExecuteThreadCycle(3);
            uint32 s;
            for (s = 0; (s < gam1Thread4->uint16SignalSamples) && (ok); s++) {
                for (e = 0u; (e < gam1Thread4->uint16SignalElements) && (ok); e++) {
                    ok = (gam1Thread4->uint16Signal[s * gam1Thread4->uint16SignalElements + e] == (j + s - (gam1Thread4->uint16SignalSamples - 1) + e));
                }
            }
            for (s = 0; (s < gam1Thread4->uint32SignalSamples) && (ok); s++) {
                for (e = 0u; (e < gam1Thread4->uint32SignalElements) && (ok); e++) {
                    ok = (gam1Thread4->uint32Signal[s * gam1Thread4->uint32SignalElements + e] == (j + s - (gam1Thread4->uint32SignalSamples - 1) + e));
                }
            }
            for (s = 0; (s < gam1Thread4->uint64SignalSamples) && (ok); s++) {
                for (e = 0u; (e < gam1Thread4->uint64SignalElements) && (ok); e++) {
                    ok = (gam1Thread4->uint64Signal[s * gam1Thread4->uint64SignalElements + e] == (j + s - (gam1Thread4->uint64SignalSamples - 1) + e));
                }
            }
            for (s = 0; (s < gam1Thread4->int32SignalSamples) && (ok); s++) {
                for (e = 0u; (e < gam1Thread4->int32SignalElements) && (ok); e++) {
                    ok = (gam1Thread4->int32Signal[s * gam1Thread4->int32SignalElements + e] == static_cast<int32>(j + s - (gam1Thread4->int32SignalSamples - 1) + e));
                }
            }
        }
    }

    godb->Purge();
    return ok;
}

#if 0
bool RealTimeThreadSynchronisationTest::TestExecute_Arrays() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config3, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<RealTimeThreadSynchronisationGAMTestHelper> gam1;
    ReferenceT<RealTimeThreadSynchronisationGAMTestHelper> gam2;
    ReferenceT<RealTimeThreadSynchronisationSchedulerTestHelper> scheduler;
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
    const uint32 NUMBER_OF_PVS = 6u;
    chid pvChids[NUMBER_OF_PVS];
    chtype pvTypes[] = {DBR_SHORT, DBR_SHORT, DBR_LONG, DBR_LONG, DBR_FLOAT, DBR_DOUBLE};
    const char8 *pvNames[] = {"MARTe2::RealTimeThreadSynchronisation::Test::UInt16Arr", "MARTe2::RealTimeThreadSynchronisation::Test::Int16Arr", "MARTe2::RealTimeThreadSynchronisation::Test::UInt32Arr", "MARTe2::RealTimeThreadSynchronisation::Test::Int32Arr",
        "MARTe2::RealTimeThreadSynchronisation::Test::Float32Arr", "MARTe2::RealTimeThreadSynchronisation::Test::Float64Arr"};

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
        float32Values = new float32[numberOfElements];
        float64Values = new float64[numberOfElements];
        uint32 n;
        for (n = 0u; (n < numberOfElements); n++) {
            uint16Values[n] = 2 * n + 1;
            int16Values[n] = 3 * n + 1;
            uint32Values[n] = 4 * n + 1;
            int32Values[n] = 5 * n + 1;
            float32Values[n] = 6 * n + 1;
            float64Values[n] = 7 * n + 1;
        }
    }
    void *pvMemory[] = {uint16Values, int16Values, uint32Values, int32Values, float32Values, float64Values};

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
                done &= (gam1->uint16Signal[n] == uint16Values[n]);
            }
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam1->uint32Signal[n] == uint32Values[n]);
            }
            for (n = 0u; (n < numberOfElements); n++) {
                done &= (gam1->float64Signal[n] == float64Values[n]);
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
#endif
