/**
 * @file UARTDataSourceTest.cpp
 * @brief Source file for class UARTDataSourceTest
 * @date 07/08/2018
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
 * the class UARTDataSourceTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "UARTDataSource.h"
#include "BasicUART.h"
#include "GAM.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "TimestampProvider.h"
#include "UARTDataSourceTest.h"
#include "UARTDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*class UARTDataSourceTimestampProvider: public MARTe::TimestampProvider {
 public:CLASS_REGISTER_DECLARATION()

 UARTDataSourceTimestampProvider() : MARTe::TimestampProvider() {

 }

 virtual ~UARTDataSourceTimestampProvider() {

 }

 virtual uint64 Timestamp() {
 return 10000u;
 }
 };
 CLASS_REGISTER(UARTDataSourceTimestampProvider, "1.0")*/

class UARTDataSourceTestGAM: public MARTe::GAM {
public:CLASS_REGISTER_DECLARATION()

    UARTDataSourceTestGAM () {
        expectedPatterns = NULL_PTR(MARTe::uint8 **);
        numberOfExpectedPatterns = 0u;
        numberOfPatternsFound = 0u;
        currentPattern = 0u;
        patternSize = 0u;
        executedAtLeastOnce = false;
    }

    virtual ~UARTDataSourceTestGAM() {
        if (expectedPatterns != NULL_PTR(MARTe::uint8 **)) {
            MARTe::uint32 i;
            for (i = 0; i < numberOfExpectedPatterns; i++) {
                delete[] expectedPatterns[i];
            }
            delete[] expectedPatterns;
        }
    }

    bool Initialise(MARTe::StructuredDataI &data) {
        using namespace MARTe;
        bool ok = GAM::Initialise(data);
        AnyType arrayDescription = data.GetType("ExpectedPatterns");
        ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
        uint32 numberOfDimensions = 0u;
        if (ok) {
            numberOfDimensions = arrayDescription.GetNumberOfDimensions();
            ok = (numberOfDimensions == 2u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "ExpectedPatterns is not a Matrix");
            }
        }
        if (ok) {
            numberOfExpectedPatterns = arrayDescription.GetNumberOfElements(1u);
            patternSize = arrayDescription.GetNumberOfElements(0u);
            ok = (numberOfExpectedPatterns > 0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "No rows defined in the matrix");
            }
        }
        if (ok) {
            ok = (patternSize > 0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "No columns defined in the matrix");
            }
        }
        if (ok) {
            expectedPatterns = new uint8*[numberOfExpectedPatterns];
            MARTe::uint32 i;
            for (i = 0; i < numberOfExpectedPatterns; i++) {
                expectedPatterns[i] = new uint8[patternSize];
            }
            Matrix < uint8 > expectedPatternsMatrix(expectedPatterns, numberOfExpectedPatterns, patternSize);
            ok = data.Read("ExpectedPatterns", expectedPatternsMatrix);
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "expectedPatternsMatrix set to %d", expectedPatternsMatrix);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the ExpectedPatterns");
            }
        }

        return ok;
    }

    bool Execute() {
        MARTe::uint8 *validSignal = reinterpret_cast<MARTe::uint8*>(GetInputSignalMemory(0));
        MARTe::uint8 *dataSignal = reinterpret_cast<MARTe::uint8*>(GetInputSignalMemory(1));
        if (*validSignal) {
            if (currentPattern < numberOfExpectedPatterns) {
                MARTe::uint32 i;
                bool ok = true;
                for (i = 0u; (i < patternSize) && (ok); i++) {
                    ok = (dataSignal[i] == expectedPatterns[currentPattern][i]);
                }
                if (ok) {
                    MARTe::Vector < MARTe::uint8 > vecTemp(dataSignal, patternSize);
                    REPORT_ERROR(MARTe::ErrorManagement::Information, "Read %d %d ...", *validSignal, vecTemp);
                    currentPattern++;
                }
            }
        }
        executedAtLeastOnce = true;
        return true;
    }

    bool Setup() {
        bool ok = (GetNumberOfInputSignals() == 2);
        return ok;
    }

    MARTe::uint8 **expectedPatterns;
    MARTe::uint8 numberOfExpectedPatterns;
    MARTe::uint8 numberOfPatternsFound;
    MARTe::uint8 currentPattern;
    MARTe::uint8 patternSize;
    bool executedAtLeastOnce;
};
CLASS_REGISTER(UARTDataSourceTestGAM, "1.0")

/**
 * Starts a MARTe application that uses this driver instance.
 */
static bool TestIntegratedInApplication(const MARTe::char8 * const config, MARTe::TimeoutType maxTimeout, MARTe::uint32 forceErrorIdx = 100000u) {
    using namespace MARTe;

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
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        application->StartNextStateExecution();
    }

    ReferenceT<UARTDataSourceTestGAM> gam;
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    BasicUART uart;
    if (ok) {
        uint32 speed = 115200;
        ok = uart.SetSpeed(speed);
    }
    if (ok) {
        ok = uart.Open("/dev/ttyLoopWr");
    }
    if (ok) {
        while (!gam->executedAtLeastOnce) {
            Sleep::Sec(0.1);
        }
        uint32 i = 0u;
        bool done = false;
        uint64 timeoutTicks = HighResolutionTimer::Counter();
        uint64 maxTimeoutTicks = (timeoutTicks + maxTimeout.HighResolutionTimerTicks());
        while ((timeoutTicks < maxTimeoutTicks) && (!done) && (ok)) {
            done = (gam->currentPattern == gam->numberOfExpectedPatterns);
            timeoutTicks = HighResolutionTimer::Counter();
            Sleep::Sec(0.1);
            if (i < gam->numberOfExpectedPatterns) {
                if (i == forceErrorIdx) {
                    uart.Write(reinterpret_cast<char8 *>(gam->expectedPatterns[i]), gam->patternSize-1);
                    Sleep::Sec(0.1);
                    uart.Write(reinterpret_cast<char8 *>(gam->expectedPatterns[i]), gam->patternSize-1);
                    Sleep::Sec(1.0);
                    forceErrorIdx = 100000;
                }
                else {
                    uart.Write(reinterpret_cast<char8 *>(gam->expectedPatterns[i]), gam->patternSize);
                    MARTe::Vector<MARTe::uint8> vecTemp(gam->expectedPatterns[i], gam->patternSize);
                    REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Wrote %d", vecTemp);
                    i++;
                }
            }
        }
        ok = done;
    }

    if (ok) {
        application->StopCurrentStateExecution();
    }

    god->Purge();
    return ok;
}

//Standard configuration
const MARTe::char8 *const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = UARTDataSourceTestGAM"
        "            ExpectedPatterns = {"
        "                {2   3  3  3  3  3  3  6  3  3  3  3  3  3  2}"
        "                {4   5  5  5  5  5  5  8  5  5  5  5  5  5  4}"
        "                {6   7  7  7  7  7  7  9  7  7  7  7  7  7  6}"
        "                {8   9  9  9  9  9  9 11  9  9  9  9  9  9  8}"
        "                {10 11 11 11 11 11 11 13 11 11 11 11 11 11 10}"
        "                {12 13 13 13 13 13 13 15 13 13 13 13 13 13 12}"
        "                {14 15 15 15 15 15 15 17 15 15 15 15 15 15 14}"
        "            }"
        "            InputSignals = {"
        "               DataOK = {"
        "                   DataSource = CRIOUART"
        "                   Type = uint8"
        "                   NumberOfElements = 1"
        "               }"
        "               Packet = {"
        "                   DataSource = CRIOUART"
        "                   Type = uint8"
        "                   NumberOfElements = 15"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +CRIOUART = {"
        "            Class = UARTDataSource"
        "            NumberOfBuffers = 3"
        "            PortName = \"/dev/ttyLoopRd\""
        "            BaudRate = 115200"
        "            Timeout = 200000"
        "            SerialTimeout = 100000"
        "            CPUMask = 8"
        "            Signals = {"
        "                DataOK = {"
        "                    Type = uint8"
        "                    NumberOfElements = 1"
        "                }"
        "                TimeStamp = {"
        "                    Type = uint64"
        "                    NumberOfElements = 1"
        "                }"
        "                Packet = {"
        "                    Type = uint8"
        "                    NumberOfElements = 15"
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
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Standard configuration
const MARTe::char8 *const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = UARTDataSourceTestGAM"
        "            ExpectedPatterns = {"
        "                {2   3  3  3  3  3  3  6  3  3  3  3  3  3  2}"
        "            }"
        "            InputSignals = {"
        "               DataOK = {"
        "                   DataSource = CRIOUART"
        "                   Type = uint8"
        "                   NumberOfElements = 1"
        "               }"
        "               Packet = {"
        "                   DataSource = CRIOUART"
        "                   Type = uint8"
        "                   NumberOfElements = 15"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +CRIOUART = {"
        "            Class = UARTDataSource"
        "            NumberOfBuffers = 3"
        "            PortName = \"/dev/ttyLoopRd\""
        "            BaudRate = 115200"
        "            Timeout = 200000"
        "            SerialTimeout = 100000"
        "            CPUMask = 8"
        "            Signals = {"
        "                DataOK = {"
        "                    Type = uint8"
        "                    NumberOfElements = 1"
        "                }"
        "                TimeStamp = {"
        "                    Type = uint64"
        "                    NumberOfElements = 1"
        "                }"
        "                Packet = {"
        "                    Type = uint8"
        "                    NumberOfElements = 15"
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
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//Too many plugins
const MARTe::char8 *const config3 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = UARTDataSourceTestGAM"
        "            ExpectedPatterns = {"
        "                {2   3  3  3  3  3  3  6  3  3  3  3  3  3  2}"
        "            }"
        "            InputSignals = {"
        "               DataOK = {"
        "                   DataSource = CRIOUART"
        "                   Type = uint8"
        "                   NumberOfElements = 1"
        "               }"
        "               Packet = {"
        "                   DataSource = CRIOUART"
        "                   Type = uint8"
        "                   NumberOfElements = 15"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +CRIOUART = {"
        "            Class = UARTDataSource"
        "            NumberOfBuffers = 3"
        "            PortName = \"/dev/ttyLoopRd\""
        "            BaudRate = 115200"
        "            Timeout = 200000"
        "            SerialTimeout = 100000"
        "            CPUMask = 8"
        "            +TheGoodOne = {"
        "                Class = HighResolutionTimestampProvider"
        "            }"
        "            +TheExcessOne = {"
        "                Class = HighResolutionTimestampProvider"
        "            }"
        "            Signals = {"
        "                DataOK = {"
        "                    Type = uint8"
        "                    NumberOfElements = 1"
        "                }"
        "                TimeStamp = {"
        "                    Type = uint64"
        "                    NumberOfElements = 1"
        "                }"
        "                Packet = {"
        "                    Type = uint8"
        "                    NumberOfElements = 15"
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
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//TimestampProvider specified
const MARTe::char8 *const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = UARTDataSourceTestGAM"
        "            ExpectedPatterns = {"
        "                {2   3  3  3  3  3  3  6  3  3  3  3  3  3  2}"
        "            }"
        "            InputSignals = {"
        "               DataOK = {"
        "                   DataSource = CRIOUART"
        "                   Type = uint8"
        "                   NumberOfElements = 1"
        "               }"
        "               Packet = {"
        "                   DataSource = CRIOUART"
        "                   Type = uint8"
        "                   NumberOfElements = 15"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +CRIOUART = {"
        "            Class = UARTDataSource"
        "            NumberOfBuffers = 3"
        "            PortName = \"/dev/ttyLoopRd\""
        "            BaudRate = 115200"
        "            Timeout = 200000"
        "            SerialTimeout = 100000"
        "            CPUMask = 8"
        "            +TheGoodOne = {"
        "                Class = HighResolutionTimestampProvider"
        "            }"
        "            Signals = {"
        "                DataOK = {"
        "                    Type = uint8"
        "                    NumberOfElements = 1"
        "                }"
        "                TimeStamp = {"
        "                    Type = uint64"
        "                    NumberOfElements = 1"
        "                }"
        "                Packet = {"
        "                    Type = uint8"
        "                    NumberOfElements = 15"
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
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool UARTDataSourceTest::TestConstructor() {
    MARTe::UARTDataSource ds;
    return (ds.GetNumberOfSignals() == 0);
}

bool UARTDataSourceTest::TestInitialise() {
    using namespace MARTe;
    UARTDataSource ds;
    ConfigurationDatabase cdb;
    cdb.Write("PortName", "/dev/ttyLoopRd");
    cdb.Write("BaudRate", 9600);
    cdb.Write("Timeout", 5000);
    cdb.Write("SerialTimeout", 100000);

    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();

    bool ok = ds.Initialise(cdb);
    return ok;
}

bool UARTDataSourceTest::TestInitialise_False_PortName() {
    using namespace MARTe;
    UARTDataSource ds;
    ConfigurationDatabase cdb;
    cdb.Write("BaudRate", 9600);
    cdb.Write("Timeout", 5000);

    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();

    bool ok = !ds.Initialise(cdb);
    return ok;
}

bool UARTDataSourceTest::TestInitialise_False_BaudRate() {
    using namespace MARTe;
    UARTDataSource ds;
    ConfigurationDatabase cdb;
    cdb.Write("PortName", "/dev/ttyLoopRd");
    cdb.Write("Timeout", 5000);

    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();

    bool ok = !ds.Initialise(cdb);
    return ok;
}

bool UARTDataSourceTest::TestInitialise_False_Timeout() {
    using namespace MARTe;
    UARTDataSource ds;
    ConfigurationDatabase cdb;
    cdb.Write("PortName", "/dev/ttyLoopRd");
    cdb.Write("BaudRate", 9600);
    cdb.Write("SerialTimeout", 960000);

    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();

    bool ok = !ds.Initialise(cdb);
    return ok;
}

bool UARTDataSourceTest::TestInitialise_False_SerialTimeout() {
    using namespace MARTe;
    UARTDataSource ds;
    ConfigurationDatabase cdb;
    cdb.Write("PortName", "/dev/ttyLoopRd");
    cdb.Write("BaudRate", 9600);

    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();

    bool ok = !ds.Initialise(cdb);
    return ok;
}

bool UARTDataSourceTest::TestInitialise_False_Port() {
    using namespace MARTe;
    UARTDataSource ds;
    ConfigurationDatabase cdb;
    cdb.Write("PortName", "/devsdsds");
    cdb.Write("BaudRate", 9600);
    cdb.Write("SerialTimeout", 960000);
    cdb.Write("Timeout", 5000);
    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();

    bool ok = !ds.Initialise(cdb);
    return ok;
}

bool UARTDataSourceTest::TestInitialise_False_Baud() {
    using namespace MARTe;
    UARTDataSource ds;
    ConfigurationDatabase cdb;
    cdb.Write("PortName", "/dev/ttyLoopRd");
    cdb.Write("BaudRate", 9601);
    cdb.Write("SerialTimeout", 960000);
    cdb.Write("Timeout", 5000);

    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();

    bool ok = !ds.Initialise(cdb);
    return ok;
}

bool UARTDataSourceTest::TestInitialise_TimestampProvider() {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config4;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }

    return ok;
}

bool UARTDataSourceTest::TestInitialise_TooManyTimestampProviders() {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config3;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }

    return !ok;
}

bool UARTDataSourceTest::TestSetConfiguredDatabase() {
    MARTe::TimeoutType timeout(2000);
    return TestIntegratedInApplication(config2, timeout);
}

bool UARTDataSourceTest::TestGetInputOffset() {
    return TestSetConfiguredDatabase();
}

bool UARTDataSourceTest::TestPrepareInputOffsets() {
    return TestSetConfiguredDatabase();
}

bool UARTDataSourceTest::TestTerminateInputCopy() {
    return TestSetConfiguredDatabase();
}

bool UARTDataSourceTest::TestCRIOThreadCallback() {
    MARTe::TimeoutType timeout(5000);
    return TestIntegratedInApplication(config1, timeout);
}

bool UARTDataSourceTest::TestCRIOThreadCallback_FailedRead() {
    MARTe::TimeoutType timeout(5000);
    return TestIntegratedInApplication(config1, timeout, 3);
}

bool UARTDataSourceTest::TestGetBrokerName() {
    using namespace MARTe;
    UARTDataSource ds;
    ConfigurationDatabase cdb;
    StreamString brokerName = ds.GetBrokerName(cdb, InputSignals);
    bool ok = (brokerName == "MemoryMapMultiBufferInputBroker");
    return ok;
}

bool UARTDataSourceTest::TestSynchronise() {
    using namespace MARTe;
    UARTDataSource ds;
    return ds.Synchronise();
}

bool UARTDataSourceTest::TestSetConfiguredDatabase_False_3_Signals() {
    MARTe::TimeoutType timeout(2000);

    //Standard configuration
    const MARTe::char8 *const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = UARTDataSourceTestGAM"
            "            ExpectedPatterns = {"
            "                {2   3  3  3  3  3  3  6  3  3  3  3  3  3  2}"
            "            }"
            "            InputSignals = {"
            "               DataOK = {"
            "                   DataSource = CRIOUART"
            "                   Type = uint8"
            "                   NumberOfElements = 1"
            "               }"
            "               Packet = {"
            "                   DataSource = CRIOUART"
            "                   Type = uint8"
            "                   NumberOfElements = 15"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +CRIOUART = {"
            "            Class = UARTDataSource"
            "            NumberOfBuffers = 3"
            "            PortName = \"/dev/ttyLoopRd\""
            "            BaudRate = 115200"
            "            SerialTimeout = 200000"
            "            Timeout = 200000"
            "            CPUMask = 8"
            "            Signals = {"
            "               DataOK = {"
            "                   Type = uint8"
            "                   NumberOfElements = 1"
            "               }"
            "                Packet = {"
            "                    Type = uint8"
            "                    NumberOfElements = 15"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    return !TestIntegratedInApplication(config, timeout);
}

bool UARTDataSourceTest::TestSetConfiguredDatabase_False_Signal1_Not_UInt8() {
    MARTe::TimeoutType timeout(2000);

    //Standard configuration
    const MARTe::char8 *const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = UARTDataSourceTestGAM"
            "            ExpectedPatterns = {"
            "                {2   3  3  3  3  3  3  6  3  3  3  3  3  3  2}"
            "            }"
            "            InputSignals = {"
            "               DataOK = {"
            "                   DataSource = CRIOUART"
            "                   Type = uint16"
            "                   NumberOfElements = 1"
            "               }"
            "               Packet = {"
            "                   DataSource = CRIOUART"
            "                   Type = uint8"
            "                   NumberOfElements = 15"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +CRIOUART = {"
            "            Class = UARTDataSource"
            "            NumberOfBuffers = 3"
            "            PortName = \"/dev/ttyLoopRd\""
            "            BaudRate = 115200"
            "            SerialTimeout = 200000"
            "            Timeout = 200000"
            "            CPUMask = 8"
            "            Signals = {"
            "                DataOK = {"
            "                    Type = uint16"
            "                    NumberOfElements = 1"
            "                }"
            "                TimeStamp = {"
            "                    Type = uint64"
            "                    NumberOfElements = 1"
            "                }"
            "                Packet = {"
            "                    Type = uint8"
            "                    NumberOfElements = 15"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    return !TestIntegratedInApplication(config, timeout);
}

bool UARTDataSourceTest::TestSetConfiguredDatabase_False_Signal1_Not_1_Element() {
    MARTe::TimeoutType timeout(2000);

    //Standard configuration
    const MARTe::char8 *const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = UARTDataSourceTestGAM"
            "            ExpectedPatterns = {"
            "                {2   3  3  3  3  3  3  6  3  3  3  3  3  3  2}"
            "            }"
            "            InputSignals = {"
            "               DataOK = {"
            "                   DataSource = CRIOUART"
            "                   Type = uint8"
            "                   NumberOfElements = 2"
            "               }"
            "               Packet = {"
            "                   DataSource = CRIOUART"
            "                   Type = uint8"
            "                   NumberOfElements = 15"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +CRIOUART = {"
            "            Class = UARTDataSource"
            "            NumberOfBuffers = 3"
            "            PortName = \"/dev/ttyLoopRd\""
            "            BaudRate = 115200"
            "            SerialTimeout = 200000"
            "            Timeout = 200000"
            "            CPUMask = 8"
            "            Signals = {"
            "                DataOK = {"
            "                    Type = uint8"
            "                    NumberOfElements = 2"
            "                }"
            "                TimeStamp = {"
            "                    Type = uint64"
            "                    NumberOfElements = 1"
            "                }"
            "                Packet = {"
            "                    Type = uint8"
            "                    NumberOfElements = 15"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    return !TestIntegratedInApplication(config, timeout);
}

bool UARTDataSourceTest::TestSetConfiguredDatabase_False_Signal2_Not_UInt64() {
    MARTe::TimeoutType timeout(2000);

    //Standard configuration
    const MARTe::char8 *const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = UARTDataSourceTestGAM"
            "            ExpectedPatterns = {"
            "                {2   3  3  3  3  3  3  6  3  3  3  3  3  3  2}"
            "            }"
            "            InputSignals = {"
            "               DataOK = {"
            "                   DataSource = CRIOUART"
            "                   Type = uint8"
            "                   NumberOfElements = 1"
            "               }"
            "               Packet = {"
            "                   DataSource = CRIOUART"
            "                   Type = uint8"
            "                   NumberOfElements = 15"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +CRIOUART = {"
            "            Class = UARTDataSource"
            "            NumberOfBuffers = 3"
            "            PortName = \"/dev/ttyLoopRd\""
            "            BaudRate = 115200"
            "            SerialTimeout = 200000"
            "            Timeout = 200000"
            "            CPUMask = 8"
            "            Signals = {"
            "                DataOK = {"
            "                    Type = uint8"
            "                    NumberOfElements = 1"
            "                }"
            "                TimeStamp = {"
            "                    Type = uint16"
            "                    NumberOfElements = 1"
            "                }"
            "                Packet = {"
            "                    Type = uint8"
            "                    NumberOfElements = 15"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    return !TestIntegratedInApplication(config, timeout);
}

bool UARTDataSourceTest::TestPrepareNextState() {
    return TestSetConfiguredDatabase();
}

bool UARTDataSourceTest::TestStopAcquisition() {
    using namespace MARTe;

    TimeoutType maxTimeout(2000);
    uint32 forceErrorIdx = 100000u;

    ConfigurationDatabase cdb;
    StreamString configStream = config1;
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
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    ReferenceT<UARTDataSource> ds;
    if (ok) {
        ds = application->Find("Data.CRIOUART");
        ok = ds.IsValid();
    }
    ErrorManagement::ErrorType err;
    if (ok) {
        err = ds->StopAcquisition();
        ok = err == ErrorManagement::NoError;
    }

    if (ok) {
        god->Purge();
    }


    return ok;
}
