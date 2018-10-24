/**
 * @file EPICSPVAOutputTest.cpp
 * @brief Source file for class EPICSPVAOutputTest
 * @date 22/10/2018
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
 * the class EPICSPVAOutputTest (public, protected, and private). Be aware that some 
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
#include "EPICSPVAOutput.h"
#include "EPICSPVAOutputTest.h"
#include "GAM.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAM which writes signals into a given EPICSPVAOutput
 */
class EPICSPVAOutputGAMTestHelper: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()EPICSPVAOutputGAMTestHelper() {
        uint8Signal = NULL;
        int8Signal = NULL;
        uint16Signal = NULL;
        int16Signal = NULL;
        uint32Signal = NULL;
        int32Signal = NULL;
        uint64Signal = NULL;
        int64Signal = NULL;
        float32Signal = NULL;
        float64Signal = NULL;
        numberOfElements = 1u;
    }

    virtual ~EPICSPVAOutputGAMTestHelper() {
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
            if (GetSignalType(OutputSignals, n) == UnsignedInteger8Bit) {
                uint8Signal = reinterpret_cast<uint8 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == SignedInteger8Bit) {
                int8Signal = reinterpret_cast<int8 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == UnsignedInteger16Bit) {
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
            else if (GetSignalType(OutputSignals, n) == UnsignedInteger64Bit) {
                uint64Signal = reinterpret_cast<uint64 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == SignedInteger64Bit) {
                int64Signal = reinterpret_cast<int64 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == Float32Bit) {
                float32Signal = reinterpret_cast<float32 *>(GetOutputSignalMemory(n));
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

    MARTe::uint8 *uint8Signal;
    MARTe::int8 *int8Signal;
    MARTe::uint16 *uint16Signal;
    MARTe::int16 *int16Signal;
    MARTe::uint32 *uint32Signal;
    MARTe::int32 *int32Signal;
    MARTe::uint64 *uint64Signal;
    MARTe::int64 *int64Signal;
    MARTe::float32 *float32Signal;
    MARTe::float64 *float64Signal;
    MARTe::uint32 numberOfElements;
};
CLASS_REGISTER(EPICSPVAOutputGAMTestHelper, "1.0")

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

/**
 * @brief Manual scheduler to test the correct interface between the EPICSCAOutput and the GAMs
 */
class EPICSPVAOutputSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

EPICSPVAOutputSchedulerTestHelper    () : MARTe::GAMSchedulerI() {
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

CLASS_REGISTER(EPICSPVAOutputSchedulerTestHelper, "1.0")
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool EPICSPVAOutputTest::TestConstructor() {
    using namespace MARTe;
    EPICSPVAOutput test;
    return (test.NumberOfReferences() == 0);
}

bool EPICSPVAOutputTest::TestAllocateMemory() {
    using namespace MARTe;
    EPICSPVAOutput test;
    return test.AllocateMemory();
}

bool EPICSPVAOutputTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    EPICSPVAOutput test;
    return (test.GetNumberOfMemoryBuffers() == 1);
}

bool EPICSPVAOutputTest::TestGetSignalMemoryBuffer() {
    return TestSetConfiguredDatabase();
}

bool EPICSPVAOutputTest::TestGetBrokerName() {
    using namespace MARTe;
    EPICSPVAOutput test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, OutputSignals), "MemoryMapAsyncOutputBroker") == 0);

    return ok;
}

bool EPICSPVAOutputTest::TestGetBrokerName_InputSignals() {
    using namespace MARTe;
    EPICSPVAOutput test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, InputSignals), "") == 0);

    return ok;
}

bool EPICSPVAOutputTest::TestGetOutputBrokers() {
    return TestSetConfiguredDatabase();
}

bool EPICSPVAOutputTest::TestInitialise() {
    using namespace MARTe;
    EPICSPVAOutput test;
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

bool EPICSPVAOutputTest::TestInitialise_Defaults() {
    using namespace MARTe;
    EPICSPVAOutput test;
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

bool EPICSPVAOutputTest::TestInitialise_False_Signals() {
    using namespace MARTe;
    EPICSPVAOutput test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 1);
    cdb.Write("StackSize", 100000);
    cdb.Write("NumberOfBuffers", 11);
    return !test.Initialise(cdb);
}

bool EPICSPVAOutputTest::TestInitialise_False_NumberOfBuffers() {
    using namespace MARTe;
    EPICSPVAOutput test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 1);
    cdb.Write("StackSize", 100000);
    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool EPICSPVAOutputTest::TestSetConfiguredDatabase() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordOut1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  UInt8 = {\n"
            "                      Type = uint8\n"
            "                      NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt16 = {\n"
            "                       Type = uint16\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt32 = {\n"
            "                       Type = uint32\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt64 = {\n"
            "                       Type = uint64\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Int8 = {\n"
            "                      Type = int8\n"
            "                      NumberOfElements = 1\n"
            "                  }\n"
            "                  Int16 = {\n"
            "                       Type = int16\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  Int32 = {\n"
            "                       Type = int32\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  Int64 = {\n"
            "                       Type = int64\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut3 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float32\n"
            "                NumberOfElements = 1\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "    +RecordOut4 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float64\n"
            "                NumberOfElements = 1\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAOutputGAMTestHelper\n"
            "            OutputSignals = {\n"
            "                SignalUInt8 = {\n"
            "                    Type = uint8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt8\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut4.Element1\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int32\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut3.Element1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +EPICSPVAOutputTest = {\n"
            "            Class = EPICSPVAOutput\n"
            "            CPUMask = 15\n"
            "            StackSize = 10000000\n"
            "            NumberOfBuffers = 2\n"
            "            Signals = {\n"
            "                RecordOut1 = {\n"
            "                    UnsignedIntegers = {\n"
            "                        UInt8 = {\n"
            "                            Type = uint8\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt16 = {\n"
            "                            Type = uint16\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt32 = {\n"
            "                            Type = uint32\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt64 = {\n"
            "                            Type = uint64\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "                RecordOut2 = {\n"
            "                    SignedIntegers = {\n"
            "                        Int8 = {\n"
            "                            Type = int8\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int16 = {\n"
            "                            Type = int16\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int32 = {\n"
            "                            Type = int32\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int64 = {\n"
            "                            Type = int64\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "                RecordOut3 = {\n"
            "                     Element1 = {\n"
            "                         Type = float32\n"
            "                         NumberOfElements = 1\n"
            "                     }\n"
            "                }\n"
            "                RecordOut4 = {\n"
            "                    Element1 = {\n"
            "                        Type = float64\n"
            "                        NumberOfElements = 1\n"
            "                    }\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAM1}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = EPICSPVAOutputSchedulerTestHelper\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";

    bool ok = TestIntegratedInApplication(config.Buffer(), true);
    return ok;
}

bool EPICSPVAOutputTest::TestSetConfiguredDatabase_False_NoSignals() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordOut1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  UInt8 = {\n"
            "                      Type = uint8\n"
            "                      NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt16 = {\n"
            "                       Type = uint16\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt32 = {\n"
            "                       Type = uint32\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt64 = {\n"
            "                       Type = uint64\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Int8 = {\n"
            "                      Type = int8\n"
            "                      NumberOfElements = 1\n"
            "                  }\n"
            "                  Int16 = {\n"
            "                       Type = int16\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  Int32 = {\n"
            "                       Type = int32\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  Int64 = {\n"
            "                       Type = int64\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut3 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float32\n"
            "                NumberOfElements = 1\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "    +RecordOut4 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float64\n"
            "                NumberOfElements = 1\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAOutputGAMTestHelper\n"
            "            OutputSignals = {\n"
            "                SignalUInt8 = {\n"
            "                    Type = uint8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt8\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut4.Element1\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int32\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut3.Element1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +EPICSPVAOutputTest = {\n"
            "            Class = EPICSPVAOutput\n"
            "            CPUMask = 15\n"
            "            StackSize = 10000000\n"
            "            NumberOfBuffers = 2\n"
            "            Signals = {\n"
            "                RecordOut1 = {\n"
            "                    UnsignedIntegers = {\n"
            "                        UInt8 = {\n"
            "                            Type = uint8\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt16 = {\n"
            "                            Type = uint16\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt32 = {\n"
            "                            Type = uint32\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt64 = {\n"
            "                            Type = uint64\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "                RecordOut2 = {\n"
            "                    SignedIntegers = {\n"
            "                        Int8 = {\n"
            "                            Type = int8\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int16 = {\n"
            "                            Type = int16\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int32 = {\n"
            "                            Type = int32\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int64 = {\n"
            "                            Type = int64\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "                RecordOut3 = {\n"
            "                     Element1 = {\n"
            "                         Type = float32\n"
            "                         NumberOfElements = 1\n"
            "                     }\n"
            "                }\n"
            "                RecordOut4 = {\n"
            "                    Element1 = {\n"
            "                        Type = float64\n"
            "                        NumberOfElements = 1\n"
            "                    }\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +EPICSPVAOutputTest2 = {\n"
            "            Class = EPICSPVAOutput\n"
            "            CPUMask = 15\n"
            "            StackSize = 10000000\n"
            "            NumberOfBuffers = 2\n"
            "            Signals = {\n"
            "                Locked = 0\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAM1}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = EPICSPVAOutputSchedulerTestHelper\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";

    bool ok = !TestIntegratedInApplication(config.Buffer(), true);
    return ok;
}

bool EPICSPVAOutputTest::TestSetConfiguredDatabase_False_Samples() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordOut1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  UInt8 = {\n"
            "                      Type = uint8\n"
            "                      NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt16 = {\n"
            "                       Type = uint16\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt32 = {\n"
            "                       Type = uint32\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt64 = {\n"
            "                       Type = uint64\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Int8 = {\n"
            "                      Type = int8\n"
            "                      NumberOfElements = 1\n"
            "                  }\n"
            "                  Int16 = {\n"
            "                       Type = int16\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  Int32 = {\n"
            "                       Type = int32\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  Int64 = {\n"
            "                       Type = int64\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut3 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float32\n"
            "                NumberOfElements = 1\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "    +RecordOut4 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float64\n"
            "                NumberOfElements = 1\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAOutputGAMTestHelper\n"
            "            OutputSignals = {\n"
            "                SignalUInt8 = {\n"
            "                    Type = uint8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt8\n"
            "                    Samples = 4\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut4.Element1\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int32\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut3.Element1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +EPICSPVAOutputTest = {\n"
            "            Class = EPICSPVAOutput\n"
            "            CPUMask = 15\n"
            "            StackSize = 10000000\n"
            "            NumberOfBuffers = 2\n"
            "            Signals = {\n"
            "                RecordOut1 = {\n"
            "                    UnsignedIntegers = {\n"
            "                        UInt8 = {\n"
            "                            Type = uint8\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt16 = {\n"
            "                            Type = uint16\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt32 = {\n"
            "                            Type = uint32\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt64 = {\n"
            "                            Type = uint64\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "                RecordOut2 = {\n"
            "                    SignedIntegers = {\n"
            "                        Int8 = {\n"
            "                            Type = int8\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int16 = {\n"
            "                            Type = int16\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int32 = {\n"
            "                            Type = int32\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int64 = {\n"
            "                            Type = int64\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "                RecordOut3 = {\n"
            "                     Element1 = {\n"
            "                         Type = float32\n"
            "                         NumberOfElements = 1\n"
            "                     }\n"
            "                }\n"
            "                RecordOut4 = {\n"
            "                    Element1 = {\n"
            "                        Type = float64\n"
            "                        NumberOfElements = 1\n"
            "                    }\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAM1}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = EPICSPVAOutputSchedulerTestHelper\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";

    bool ok = !TestIntegratedInApplication(config.Buffer(), true);
    return ok;
}

bool EPICSPVAOutputTest::TestSetConfiguredDatabase_False_MoreThanOneGAM() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordOut1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  UInt8 = {\n"
            "                      Type = uint8\n"
            "                      NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt16 = {\n"
            "                       Type = uint16\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt32 = {\n"
            "                       Type = uint32\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt64 = {\n"
            "                       Type = uint64\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Int8 = {\n"
            "                      Type = int8\n"
            "                      NumberOfElements = 1\n"
            "                  }\n"
            "                  Int16 = {\n"
            "                       Type = int16\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  Int32 = {\n"
            "                       Type = int32\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  Int64 = {\n"
            "                       Type = int64\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut3 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float32\n"
            "                NumberOfElements = 1\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "    +RecordOut4 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float64\n"
            "                NumberOfElements = 1\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM0 = {\n"
            "            Class = EPICSPVAOutputGAMTestHelper\n"
            "            OutputSignals = {\n"
            "                SignalUInt8 = {\n"
            "                    Type = uint8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt8\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAOutputGAMTestHelper\n"
            "            OutputSignals = {\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut4.Element1\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int32\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut3.Element1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +EPICSPVAOutputTest = {\n"
            "            Class = EPICSPVAOutput\n"
            "            CPUMask = 15\n"
            "            StackSize = 10000000\n"
            "            NumberOfBuffers = 2\n"
            "            Signals = {\n"
            "                RecordOut1 = {\n"
            "                    UnsignedIntegers = {\n"
            "                        UInt8 = {\n"
            "                            Type = uint8\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt16 = {\n"
            "                            Type = uint16\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt32 = {\n"
            "                            Type = uint32\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt64 = {\n"
            "                            Type = uint64\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "                RecordOut2 = {\n"
            "                    SignedIntegers = {\n"
            "                        Int8 = {\n"
            "                            Type = int8\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int16 = {\n"
            "                            Type = int16\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int32 = {\n"
            "                            Type = int32\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int64 = {\n"
            "                            Type = int64\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "                RecordOut3 = {\n"
            "                     Element1 = {\n"
            "                         Type = float32\n"
            "                         NumberOfElements = 1\n"
            "                     }\n"
            "                }\n"
            "                RecordOut4 = {\n"
            "                    Element1 = {\n"
            "                        Type = float64\n"
            "                        NumberOfElements = 1\n"
            "                    }\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAM0 GAM1}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = EPICSPVAOutputSchedulerTestHelper\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";

    bool ok = !TestIntegratedInApplication(config.Buffer(), true);
    return ok;
}

bool EPICSPVAOutputTest::TestGetCPUMask() {
    return TestInitialise();
}

bool EPICSPVAOutputTest::TestGetStackSize() {
    return TestInitialise();
}

bool EPICSPVAOutputTest::TestGetNumberOfBuffers() {
    return TestInitialise();
}

bool EPICSPVAOutputTest::TestIsIgnoringBufferOverrun() {
    return TestInitialise();
}

bool EPICSPVAOutputTest::TestSynchronise() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordOut1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  UInt8 = {\n"
            "                      Type = uint8\n"
            "                      NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt16 = {\n"
            "                       Type = uint16\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt32 = {\n"
            "                       Type = uint32\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  UInt64 = {\n"
            "                       Type = uint64\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Alias = \"TEST::RECORDOUT2\"\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Int8 = {\n"
            "                      Type = int8\n"
            "                      NumberOfElements = 1\n"
            "                  }\n"
            "                  Int16 = {\n"
            "                       Type = int16\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  Int32 = {\n"
            "                       Type = int32\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "                  Int64 = {\n"
            "                       Type = int64\n"
            "                       NumberOfElements = 1\n"
            "                  }\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut3 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float32\n"
            "                NumberOfElements = 1\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "    +RecordOut4 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float64\n"
            "                NumberOfElements = 1\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAOutputGAMTestHelper\n"
            "            OutputSignals = {\n"
            "                SignalUInt8 = {\n"
            "                    Type = uint8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt8\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UnsignedIntegers.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut4.Element1\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int32\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.SignedIntegers.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut3.Element1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +EPICSPVAOutputTest = {\n"
            "            Class = EPICSPVAOutput\n"
            "            CPUMask = 15\n"
            "            StackSize = 10000000\n"
            "            NumberOfBuffers = 2\n"
            "            Signals = {\n"
            "                RecordOut1 = {\n"
            "                    UnsignedIntegers = {\n"
            "                        UInt8 = {\n"
            "                            Type = uint8\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt16 = {\n"
            "                            Type = uint16\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt32 = {\n"
            "                            Type = uint32\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        UInt64 = {\n"
            "                            Type = uint64\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "                RecordOut2 = {\n"
            "                    Alias = \"TEST::RECORDOUT2\"\n"
            "                    SignedIntegers = {\n"
            "                        Int8 = {\n"
            "                            Type = int8\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int16 = {\n"
            "                            Type = int16\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int32 = {\n"
            "                            Type = int32\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                        Int64 = {\n"
            "                            Type = int64\n"
            "                            NumberOfElements = 1\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "                RecordOut3 = {\n"
            "                     Element1 = {\n"
            "                         Type = float32\n"
            "                         NumberOfElements = 1\n"
            "                     }\n"
            "                }\n"
            "                RecordOut4 = {\n"
            "                    Element1 = {\n"
            "                        Type = float64\n"
            "                        NumberOfElements = 1\n"
            "                    }\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAM1}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = EPICSPVAOutputSchedulerTestHelper\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";

    bool ok = TestIntegratedInApplication(config.Buffer(), false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<EPICSPVAOutputGAMTestHelper> gam1;
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
        ok = application->PrepareNextState("State1");
    }
    ReferenceT<EPICSPVAOutputSchedulerTestHelper> scheduler;
    if (ok) {
        scheduler = godb->Find("Test.Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }
    if (ok) {
        *gam1->uint8Signal = 1;
        *gam1->uint16Signal = 2;
        *gam1->uint32Signal = 3;
        *gam1->uint64Signal = 4;
        *gam1->int8Signal = -1;
        *gam1->int16Signal = -2;
        *gam1->int32Signal = -3;
        *gam1->int64Signal = -4;
        *gam1->float32Signal = 32;
        *gam1->float64Signal = 64;

        scheduler->ExecuteThreadCycle(0u);
        pvac::ClientProvider provider("pva");
        uint32 timeOutCounts = 50;
        ok = false;
        while ((!ok) && (timeOutCounts != 0u)) {
            {
                pvac::ClientChannel record1(provider.connect("RecordOut1"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record1.get();
                std::shared_ptr<const epics::pvData::PVUByte> uint8Value = getStruct->getSubField<epics::pvData::PVUByte>("UnsignedIntegers.UInt8");
                std::shared_ptr<const epics::pvData::PVUShort> uint16Value = getStruct->getSubField<epics::pvData::PVUShort>("UnsignedIntegers.UInt16");
                std::shared_ptr<const epics::pvData::PVUInt> uint32Value = getStruct->getSubField<epics::pvData::PVUInt>("UnsignedIntegers.UInt32");
                std::shared_ptr<const epics::pvData::PVULong> uint64Value = getStruct->getSubField<epics::pvData::PVULong>("UnsignedIntegers.UInt64");
                ok = (uint8Value ? true : false);
                if (ok) {
                    ok = (uint8Value->get() == *gam1->uint8Signal);
                    ok &= (uint16Value->get() == *gam1->uint16Signal);
                    ok &= (uint32Value->get() == *gam1->uint32Signal);
                    ok &= (uint64Value->get() == *gam1->uint64Signal);
                }
            }
            {
                pvac::ClientChannel record2(provider.connect("TEST::RECORDOUT2"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record2.get();
                std::shared_ptr<const epics::pvData::PVByte> int8Value = getStruct->getSubField<epics::pvData::PVByte>("SignedIntegers.Int8");
                std::shared_ptr<const epics::pvData::PVShort> int16Value = getStruct->getSubField<epics::pvData::PVShort>("SignedIntegers.Int16");
                std::shared_ptr<const epics::pvData::PVInt> int32Value = getStruct->getSubField<epics::pvData::PVInt>("SignedIntegers.Int32");
                std::shared_ptr<const epics::pvData::PVLong> int64Value = getStruct->getSubField<epics::pvData::PVLong>("SignedIntegers.Int64");
                ok &= (int8Value ? true : false);
                if (ok) {
                    ok = (int8Value->get() == *gam1->int8Signal);
                    ok &= (int16Value->get() == *gam1->int16Signal);
                    ok &= (int32Value->get() == *gam1->int32Signal);
                    ok &= (int64Value->get() == *gam1->int64Signal);
                }
            }
            {
                pvac::ClientChannel record3(provider.connect("RecordOut3"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record3.get();
                std::shared_ptr<const epics::pvData::PVFloat> float32Value = getStruct->getSubField<epics::pvData::PVFloat>("Element1");
                ok &= (float32Value ? true : false);
                if (ok) {
                    ok = (float32Value->get() == *gam1->float32Signal);
                }
            }
            {
                pvac::ClientChannel record4(provider.connect("RecordOut4"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record4.get();
                std::shared_ptr<const epics::pvData::PVDouble> float64Value = getStruct->getSubField<epics::pvData::PVDouble>("Element1");
                ok &= (float64Value ? true : false);
                if (ok) {
                    ok = (float64Value->get() == *gam1->float64Signal);
                }
            }
            Sleep::Sec(0.1);
            timeOutCounts--;
        }
    }
    godb->Purge();

    return ok;
}

bool EPICSPVAOutputTest::TestSynchronise_Arrays() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordOut1Arr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  UInt8 = {\n"
            "                      Type = uint8\n"
            "                      NumberOfElements = 4\n"
            "                  }\n"
            "                  UInt16 = {\n"
            "                       Type = uint16\n"
            "                       NumberOfElements = 4\n"
            "                  }\n"
            "                  UInt32 = {\n"
            "                       Type = uint32\n"
            "                       NumberOfElements = 4\n"
            "                  }\n"
            "                  UInt64 = {\n"
            "                       Type = uint64\n"
            "                       NumberOfElements = 4\n"
            "                  }\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut2Arr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Int8 = {\n"
            "                      Type = int8\n"
            "                      NumberOfElements = 4\n"
            "                  }\n"
            "                  Int16 = {\n"
            "                       Type = int16\n"
            "                       NumberOfElements = 4\n"
            "                  }\n"
            "                  Int32 = {\n"
            "                       Type = int32\n"
            "                       NumberOfElements = 4\n"
            "                  }\n"
            "                  Int64 = {\n"
            "                       Type = int64\n"
            "                       NumberOfElements = 4\n"
            "                  }\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut3Arr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float32\n"
            "                NumberOfElements = 4\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "    +RecordOut4Arr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float64\n"
            "                NumberOfElements = 4\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAOutputGAMTestHelper\n"
            "            OutputSignals = {\n"
            "                SignalUInt8 = {\n"
            "                    Type = uint8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1Arr.UnsignedIntegers.UInt8\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1Arr.UnsignedIntegers.UInt16\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1Arr.UnsignedIntegers.UInt32\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1Arr.UnsignedIntegers.UInt64\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut4Arr.Element1\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2Arr.SignedIntegers.Int8\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2Arr.SignedIntegers.Int16\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2Arr.SignedIntegers.Int32\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2Arr.SignedIntegers.Int64\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut3Arr.Element1\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +EPICSPVAOutputTest = {\n"
            "            Class = EPICSPVAOutput\n"
            "            CPUMask = 15\n"
            "            StackSize = 10000000\n"
            "            NumberOfBuffers = 2\n"
            "            Signals = {\n"
            "                RecordOut1Arr = {\n"
            "                    UnsignedIntegers = {\n"
            "                        UInt8 = {\n"
            "                            Type = uint8\n"
            "                            NumberOfElements = 4\n"
            "                        }\n"
            "                        UInt16 = {\n"
            "                            Type = uint16\n"
            "                            NumberOfElements = 4\n"
            "                        }\n"
            "                        UInt32 = {\n"
            "                            Type = uint32\n"
            "                            NumberOfElements = 4\n"
            "                        }\n"
            "                        UInt64 = {\n"
            "                            Type = uint64\n"
            "                            NumberOfElements = 4\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "                RecordOut2Arr = {\n"
            "                    SignedIntegers = {\n"
            "                        Int8 = {\n"
            "                            Type = int8\n"
            "                            NumberOfElements = 4\n"
            "                        }\n"
            "                        Int16 = {\n"
            "                            Type = int16\n"
            "                            NumberOfElements = 4\n"
            "                        }\n"
            "                        Int32 = {\n"
            "                            Type = int32\n"
            "                            NumberOfElements = 4\n"
            "                        }\n"
            "                        Int64 = {\n"
            "                            Type = int64\n"
            "                            NumberOfElements = 4\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "                RecordOut3Arr = {\n"
            "                     Element1 = {\n"
            "                         Type = float32\n"
            "                         NumberOfElements = 4\n"
            "                     }\n"
            "                }\n"
            "                RecordOut4Arr = {\n"
            "                    Element1 = {\n"
            "                        Type = float64\n"
            "                        NumberOfElements = 4\n"
            "                    }\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAM1}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = EPICSPVAOutputSchedulerTestHelper\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";

    bool ok = TestIntegratedInApplication(config.Buffer(), false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<EPICSPVAOutputGAMTestHelper> gam1;
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
        ok = application->PrepareNextState("State1");
    }
    ReferenceT<EPICSPVAOutputSchedulerTestHelper> scheduler;
    if (ok) {
        scheduler = godb->Find("Test.Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }
    if (ok) {
        uint32 n;
        uint32 nOfElements = 4;
        for (n = 0u; n < nOfElements; n++) {
            gam1->uint8Signal[n] = n;
            gam1->uint16Signal[n] = 2 * n;
            gam1->uint32Signal[n] = 3 * n;
            gam1->uint64Signal[n] = 4 * n;
            gam1->int8Signal[n] = -1 * n;
            gam1->int16Signal[n] = -2 * n;
            gam1->int32Signal[n] = -3 * n;
            gam1->int64Signal[n] = -4 * n;
            gam1->float32Signal[n] = 32 * n;
            gam1->float64Signal[n] = 64 * n;
        }
        scheduler->ExecuteThreadCycle(0u);
        pvac::ClientProvider provider("pva");
        uint32 timeOutCounts = 50;
        ok = false;
        while ((!ok) && (timeOutCounts != 0u)) {
            {
                pvac::ClientChannel record1(provider.connect("RecordOut1Arr"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record1.get();
                std::shared_ptr<const epics::pvData::PVUByteArray> uint8Value = getStruct->getSubField<epics::pvData::PVUByteArray>("UnsignedIntegers.UInt8");
                std::shared_ptr<const epics::pvData::PVUShortArray> uint16Value = getStruct->getSubField<epics::pvData::PVUShortArray>("UnsignedIntegers.UInt16");
                std::shared_ptr<const epics::pvData::PVUIntArray> uint32Value = getStruct->getSubField<epics::pvData::PVUIntArray>("UnsignedIntegers.UInt32");
                std::shared_ptr<const epics::pvData::PVULongArray> uint64Value = getStruct->getSubField<epics::pvData::PVULongArray>("UnsignedIntegers.UInt64");
                ok = (uint8Value ? true : false);
                epics::pvData::shared_vector<const uint8> outUInt8;
                epics::pvData::shared_vector<const uint16> outUInt16;
                epics::pvData::shared_vector<const uint32> outUInt32;
                epics::pvData::shared_vector<const unsigned long int> outUInt64;
                outUInt8.resize(nOfElements);
                outUInt16.resize(nOfElements);
                outUInt32.resize(nOfElements);
                outUInt64.resize(nOfElements);
                if (ok) {
                    uint8Value->getAs < uint8 > (outUInt8);
                    uint16Value->getAs < uint16 > (outUInt16);
                    uint32Value->getAs < uint32 > (outUInt32);
                    uint64Value->getAs<unsigned long int>(outUInt64);
                }
                for (n = 0u; (n < nOfElements) && (ok); n++) {
                    ok = (outUInt8[n] == gam1->uint8Signal[n]);
                    ok &= (outUInt16[n] == gam1->uint16Signal[n]);
                    ok &= (outUInt32[n] == gam1->uint32Signal[n]);
                    ok &= (outUInt64[n] == gam1->uint64Signal[n]);
                }
            }
            {
                pvac::ClientChannel record2(provider.connect("RecordOut2Arr"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record2.get();
                std::shared_ptr<const epics::pvData::PVByteArray> int8Value = getStruct->getSubField<epics::pvData::PVByteArray>("SignedIntegers.Int8");
                std::shared_ptr<const epics::pvData::PVShortArray> int16Value = getStruct->getSubField<epics::pvData::PVShortArray>("SignedIntegers.Int16");
                std::shared_ptr<const epics::pvData::PVIntArray> int32Value = getStruct->getSubField<epics::pvData::PVIntArray>("SignedIntegers.Int32");
                std::shared_ptr<const epics::pvData::PVLongArray> int64Value = getStruct->getSubField<epics::pvData::PVLongArray>("SignedIntegers.Int64");
                ok = (int8Value ? true : false);
                epics::pvData::shared_vector<const int8> outInt8;
                epics::pvData::shared_vector<const int16> outInt16;
                epics::pvData::shared_vector<const int32> outInt32;
                epics::pvData::shared_vector<const long int> outInt64;
                outInt8.resize(nOfElements);
                outInt16.resize(nOfElements);
                outInt32.resize(nOfElements);
                outInt64.resize(nOfElements);
                if (ok) {
                    int8Value->getAs < int8 > (outInt8);
                    int16Value->getAs < int16 > (outInt16);
                    int32Value->getAs < int32 > (outInt32);
                    int64Value->getAs<long int>(outInt64);
                }
                for (n = 0u; (n < nOfElements) && (ok); n++) {
                    ok = (outInt8[n] == gam1->int8Signal[n]);
                    ok &= (outInt16[n] == gam1->int16Signal[n]);
                    ok &= (outInt32[n] == gam1->int32Signal[n]);
                    ok &= (outInt64[n] == gam1->int64Signal[n]);
                }
            }
            {
                pvac::ClientChannel record3(provider.connect("RecordOut3Arr"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record3.get();
                std::shared_ptr<const epics::pvData::PVFloatArray> float32Value = getStruct->getSubField<epics::pvData::PVFloatArray>("Element1");
                ok = (float32Value ? true : false);
                epics::pvData::shared_vector<const float32> outFloat32;
                outFloat32.resize(nOfElements);
                if (ok) {
                    float32Value->getAs < float32 > (outFloat32);
                }
                for (n = 0u; (n < nOfElements) && (ok); n++) {
                    ok = (outFloat32[n] == gam1->float32Signal[n]);
                }
            }
            {
                pvac::ClientChannel record4(provider.connect("RecordOut4Arr"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record4.get();
                std::shared_ptr<const epics::pvData::PVDoubleArray> float64Value = getStruct->getSubField<epics::pvData::PVDoubleArray>("Element1");
                ok = (float64Value ? true : false);
                epics::pvData::shared_vector<const float64> outFloat64;
                outFloat64.resize(nOfElements);
                if (ok) {
                    float64Value->getAs < float64 > (outFloat64);
                }
                for (n = 0u; (n < nOfElements) && (ok); n++) {
                    ok = (outFloat64[n] == gam1->float64Signal[n]);
                }
            }
            Sleep::Sec(0.1);
            timeOutCounts--;
        }
    }
    godb->Purge();
    return ok;
}
