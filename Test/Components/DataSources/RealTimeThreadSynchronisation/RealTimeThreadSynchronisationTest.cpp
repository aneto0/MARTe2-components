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

//As config1 with WaitForNext
static const MARTe::char8 * const config1b = ""
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
        "            WaitForNext = 1"
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


//Configuration with no GAMs producing/consuming data from the RealTimeThreadSynchronisation which is OK
static const MARTe::char8 * const config2 = ""
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
        "        +RealTimeThreadSynchronisationTestEmpty = {"
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

//Configuration with one GAMs producing but no GAM consuming (which is OK)
static const MARTe::char8 * const config3 = ""
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
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = RealTimeThreadSynchronisationSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Configuration with no GAMs producing consuming data from the RealTimeThreadSynchronisation which is wrong!
static const MARTe::char8 * const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
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

//Configuration with more than one producer
static const MARTe::char8 * const config5 = ""
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
        "            }"
        "        }"
        "        +GAM1Thread1B = {"
        "            Class = RealTimeThreadSynchronisationGAMTestHelper"
        "            OutputSignals = {"
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
        "                    Functions = {GAM1Thread1 GAM1Thread1B}"
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

//Configuration where not all signals are written
static const MARTe::char8 * const config6 = ""
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
        "            Signals = {"
        "                SignalUInt32 = {"
        "                    Type = uint32"
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

//Configuration with more than one output sample
static const MARTe::char8 * const config7 = ""
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
        "                    Samples = 2"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    Samples = 2"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 5"
        "                    Samples = 2"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = RealTimeThreadSynchronisationTest"
        "                    Samples = 2"
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

//Configuration reading a different number of samples from the same GAM
static const MARTe::char8 * const config8 = ""
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
        "                    Samples = 4"
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
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool RealTimeThreadSynchronisationTest::TestConstructor() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
    return true;
}

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
    test.Initialise(cdb);
    void *ptr = NULL;
    bool ok = !test.GetSignalMemoryBuffer(0, 0, ptr);
    if (ok) {
        ok = TestIntegratedInApplication(config1, true);
    }
    return ok;
}

bool RealTimeThreadSynchronisationTest::TestGetBrokerName() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, InputSignals), "RealTimeThreadSynchBroker") == 0);
    ok &= (StringHelper::Compare(test.GetBrokerName(cdb, OutputSignals), "MemoryMapSynchronisedOutputBroker") == 0);
    return ok;
}

bool RealTimeThreadSynchronisationTest::TestGetInputBrokers() {
    return TestIntegratedInApplication(config1, true);
}

bool RealTimeThreadSynchronisationTest::TestGetOutputBrokers() {
    return TestIntegratedInApplication(config1, true);
}

bool RealTimeThreadSynchronisationTest::TestPrepareNextState() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
    return test.PrepareNextState("", "");
}

bool RealTimeThreadSynchronisationTest::TestInitialise() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
    ConfigurationDatabase cdb;
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (test.GetSynchroniseTimeout() == 1000);
    }

    return ok;
}

bool RealTimeThreadSynchronisationTest::TestInitialise_Timeout() {
    using namespace MARTe;
    RealTimeThreadSynchronisation test;
    ConfigurationDatabase cdb;
    uint32 timeoutToTest = 100;
    cdb.Write("Timeout", timeoutToTest);
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (test.GetSynchroniseTimeout() == timeoutToTest);
    }

    return ok;
}

bool RealTimeThreadSynchronisationTest::TestGetSynchroniseTimeout() {
    return TestInitialise_Timeout();
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication(config1, true);
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase_WaitForNext() {
    return TestIntegratedInApplication(config1b, true);
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase_NoInteraction() {
    return TestIntegratedInApplication(config2, true);
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase_ProducerOnly() {
    return TestIntegratedInApplication(config3, true);
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase_False_NoProducer() {
    return !TestIntegratedInApplication(config4, true);
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase_False_MoreThanOneProducer() {
    return !TestIntegratedInApplication(config5, true);
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase_Warning_NotAllSignals() {
    return TestIntegratedInApplication(config6, true);
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase_False_MoreThanOneOutputSample() {
    return !TestIntegratedInApplication(config7, true);
}

bool RealTimeThreadSynchronisationTest::TestSetConfiguredDatabase_False_DifferentInputSamplesFromSameFunction() {
    return !TestIntegratedInApplication(config8, true);
}

bool RealTimeThreadSynchronisationTest::TestSynchronise() {
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
