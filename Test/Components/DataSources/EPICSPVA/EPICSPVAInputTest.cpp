/**
 * @file EPICSPVAInputTest.cpp
 * @brief Source file for class EPICSPVAInputTest
 * @date 23/10/2018
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
 * the class EPICSPVAInputTest (public, protected, and private). Be aware that some
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
#include "EPICSPVAInput.h"
#include "EPICSPVAInputTest.h"
#include "GAM.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAM which writes signals into a given EPICSPVAInput
 */
class EPICSPVAInputGAMTestHelper: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()EPICSPVAInputGAMTestHelper() {
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

    virtual ~EPICSPVAInputGAMTestHelper() {
        using namespace MARTe;
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        GetSignalNumberOfElements(InputSignals, 0, numberOfElements);
        uint32 n;
        uint32 numberOfSignals = GetNumberOfInputSignals();
        for(n=0; n<numberOfSignals; n++) {
            if (GetSignalType(InputSignals, n) == UnsignedInteger8Bit) {
                uint8Signal = reinterpret_cast<uint8 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger8Bit) {
                int8Signal = reinterpret_cast<int8 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger16Bit) {
                uint16Signal = reinterpret_cast<uint16 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger16Bit) {
                int16Signal = reinterpret_cast<int16 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger32Bit) {
                uint32Signal = reinterpret_cast<uint32 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger32Bit) {
                int32Signal = reinterpret_cast<int32 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger64Bit) {
                uint64Signal = reinterpret_cast<uint64 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger64Bit) {
                int64Signal = reinterpret_cast<int64 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == Float32Bit) {
                float32Signal = reinterpret_cast<float32 *>(GetInputSignalMemory(n));
            }
            else {
                float64Signal = reinterpret_cast<float64 *>(GetInputSignalMemory(n));
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
CLASS_REGISTER(EPICSPVAInputGAMTestHelper, "1.0")

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
 * @brief Manual scheduler to test the correct interface between the EPICSCAInput and the GAMs
 */
class EPICSPVAInputSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

EPICSPVAInputSchedulerTestHelper    () : MARTe::GAMSchedulerI() {
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

CLASS_REGISTER(EPICSPVAInputSchedulerTestHelper, "1.0")
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool EPICSPVAInputTest::TestConstructor() {
    return true;
}

bool EPICSPVAInputTest::TestExecute() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordIn1 = {\n"
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
            "    +RecordIn2 = {\n"
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
            "    +RecordIn3 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float32\n"
            "                NumberOfElements = 1\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "    +RecordIn4 = {\n"
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
            "            Class = EPICSPVAInputGAMTestHelper\n"
            "            InputSignals = {\n"
            "                SignalUInt8 = {\n"
            "                    Type = uint8\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UnsignedIntegers.UInt8\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UnsignedIntegers.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UnsignedIntegers.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UnsignedIntegers.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn4.Element1\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.SignedIntegers.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.SignedIntegers.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.SignedIntegers.Int32\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.SignedIntegers.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn3.Element1\n"
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
            "        +EPICSPVAInputTest = {\n"
            "            Class = EPICSPVAInput\n"
            "            CPUMask = 15\n"
            "            StackSize = 10000000\n"
            "            NumberOfBuffers = 2\n"
            "            Signals = {\n"
            "                RecordIn1 = {\n"
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
            "                RecordIn2 = {\n"
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
            "                RecordIn3 = {\n"
            "                     Element1 = {\n"
            "                         Type = float32\n"
            "                         NumberOfElements = 1\n"
            "                     }\n"
            "                }\n"
            "                RecordIn4 = {\n"
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
            "        Class = EPICSPVAInputSchedulerTestHelper\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";

    bool ok = TestIntegratedInApplication(config.Buffer(), false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<EPICSPVAInputGAMTestHelper> gam1;
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
    ReferenceT<EPICSPVAInputSchedulerTestHelper> scheduler;
    if (ok) {
        scheduler = godb->Find("Test.Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }
    if (ok) {
        *gam1->uint8Signal = 0;
        *gam1->uint16Signal = 0;
        *gam1->uint32Signal = 0;
        *gam1->uint64Signal = 0;
        *gam1->int8Signal = 0;
        *gam1->int16Signal = 0;
        *gam1->int32Signal = 0;
        *gam1->int64Signal = 0;
        *gam1->float32Signal = 0;
        *gam1->float64Signal = 0;

        pvac::ClientProvider provider("pva");
        uint32 timeOutCounts = 50;
        ok = false;
        while ((!ok) && (timeOutCounts != 0u)) {
            pvac::ClientChannel record1(provider.connect("RecordIn1"));
            record1.put().set("UnsignedIntegers.UInt8", 1).exec();
            record1.put().set("UnsignedIntegers.UInt16", 2).exec();
            record1.put().set("UnsignedIntegers.UInt32", 3).exec();
            record1.put().set("UnsignedIntegers.UInt64", 4).exec();
            pvac::ClientChannel record2(provider.connect("RecordIn2"));
            record2.put().set("SignedIntegers.Int8", -1).exec();
            record2.put().set("SignedIntegers.Int16", -2).exec();
            record2.put().set("SignedIntegers.Int32", -3).exec();
            record2.put().set("SignedIntegers.Int64", -4).exec();
            pvac::ClientChannel record3(provider.connect("RecordIn3"));
            record3.put().set("Element1", 32).exec();
            pvac::ClientChannel record4(provider.connect("RecordIn4"));
            record4.put().set("Element1", 64).exec();
            scheduler->ExecuteThreadCycle(0u);
            ok = (1 == *gam1->uint8Signal);
            ok &= (2 == *gam1->uint16Signal);
            ok &= (3 == *gam1->uint32Signal);
            ok &= (4 == *gam1->uint64Signal);
            ok &= (-1 == *gam1->int8Signal);
            ok &= (-2 == *gam1->int16Signal);
            ok &= (-3 == *gam1->int32Signal);
            ok &= (-4 == *gam1->int64Signal);
            ok &= (32 == *gam1->float32Signal);
            ok &= (64 == *gam1->float64Signal);
            Sleep::Sec(0.1);
            timeOutCounts--;
        }
    }
    godb->Purge();

    return ok;
}

bool EPICSPVAInputTest::TestExecute_Arrays() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordIn1Arr = {\n"
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
            "    +RecordIn2Arr = {\n"
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
            "    +RecordIn3Arr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            Element1 = {\n"
            "                Type = float32\n"
            "                NumberOfElements = 4\n"
            "            }\n"
            "       }\n"
            "    }\n"
            "    +RecordIn4Arr = {\n"
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
            "            Class = EPICSPVAInputGAMTestHelper\n"
            "            InputSignals = {\n"
            "                SignalUInt8 = {\n"
            "                    Type = uint8\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1Arr.UnsignedIntegers.UInt8\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1Arr.UnsignedIntegers.UInt16\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1Arr.UnsignedIntegers.UInt32\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1Arr.UnsignedIntegers.UInt64\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn4Arr.Element1\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2Arr.SignedIntegers.Int8\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2Arr.SignedIntegers.Int16\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2Arr.SignedIntegers.Int32\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2Arr.SignedIntegers.Int64\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn3Arr.Element1\n"
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
            "        +EPICSPVAInputTest = {\n"
            "            Class = EPICSPVAInput\n"
            "            CPUMask = 15\n"
            "            StackSize = 10000000\n"
            "            NumberOfBuffers = 2\n"
            "            Signals = {\n"
            "                RecordIn1Arr = {\n"
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
            "                RecordIn2Arr = {\n"
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
            "                RecordIn3Arr = {\n"
            "                     Element1 = {\n"
            "                         Type = float32\n"
            "                         NumberOfElements = 4\n"
            "                     }\n"
            "                }\n"
            "                RecordIn4Arr = {\n"
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
            "        Class = EPICSPVAInputSchedulerTestHelper\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";

    bool ok = TestIntegratedInApplication(config.Buffer(), false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<EPICSPVAInputGAMTestHelper> gam1;
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
    ReferenceT<EPICSPVAInputSchedulerTestHelper> scheduler;
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
        epics::pvData::shared_vector<uint8> outUInt8;
        epics::pvData::shared_vector<uint16> outUInt16;
        epics::pvData::shared_vector<uint32> outUInt32;
        epics::pvData::shared_vector<unsigned long int> outUInt64;
        epics::pvData::shared_vector<int8> outInt8;
        epics::pvData::shared_vector<int16> outInt16;
        epics::pvData::shared_vector<int32> outInt32;
        epics::pvData::shared_vector<long int> outInt64;
        epics::pvData::shared_vector<float32> outFloat32;
        epics::pvData::shared_vector<float64> outFloat64;

        outUInt8.resize(nOfElements);
        outUInt16.resize(nOfElements);
        outUInt32.resize(nOfElements);
        outUInt64.resize(nOfElements);
        outInt8.resize(nOfElements);
        outInt16.resize(nOfElements);
        outInt32.resize(nOfElements);
        outInt64.resize(nOfElements);
        outFloat32.resize(nOfElements);
        outFloat64.resize(nOfElements);

        for (n = 0u; n < nOfElements; n++) {
            outUInt8[n] = n;
            outUInt16[n] = 2 * n;
            outUInt32[n] = 3 * n;
            outUInt64[n] = 4 * n;
            outInt8[n] = -1 * n;
            outInt16[n] = -2 * n;
            outInt32[n] = -3 * n;
            outInt64[n] = -4 * n;
            outFloat32[n] = 32 * n;
            outFloat64[n] = 64 * n;
        }

        pvac::ClientProvider provider("pva");
        uint32 timeOutCounts = 50;
        ok = false;
        while ((!ok) && (timeOutCounts != 0u)) {
            pvac::ClientChannel record1(provider.connect("RecordIn1Arr"));
            record1.put().set("UnsignedIntegers.UInt8", freeze(outUInt8)).exec();
            record1.put().set("UnsignedIntegers.UInt16", freeze(outUInt16)).exec();
            record1.put().set("UnsignedIntegers.UInt32", freeze(outUInt32)).exec();
            record1.put().set("UnsignedIntegers.UInt64", freeze(outUInt64)).exec();
            pvac::ClientChannel record2(provider.connect("RecordIn2Arr"));
            record2.put().set("SignedIntegers.Int8", freeze(outInt8)).exec();
            record2.put().set("SignedIntegers.Int16", freeze(outInt16)).exec();
            record2.put().set("SignedIntegers.Int32", freeze(outInt32)).exec();
            record2.put().set("SignedIntegers.Int64", freeze(outInt64)).exec();
            pvac::ClientChannel record3(provider.connect("RecordIn3Arr"));
            record3.put().set("Element1", freeze(outFloat32)).exec();
            pvac::ClientChannel record4(provider.connect("RecordIn4Arr"));
            record4.put().set("Element1", freeze(outFloat64)).exec();
            scheduler->ExecuteThreadCycle(0u);
            for (n = 0u; (n < nOfElements); n++) {
                ok = (outUInt8[n] == gam1->uint8Signal[n]);
                ok &= (outUInt16[n] == gam1->uint16Signal[n]);
                ok &= (outUInt32[n] == gam1->uint32Signal[n]);
                ok &= (outUInt64[n] == gam1->uint64Signal[n]);
                ok &= (outInt8[n] == gam1->int8Signal[n]);
                ok &= (outInt16[n] == gam1->int16Signal[n]);
                ok &= (outInt32[n] == gam1->int32Signal[n]);
                ok &= (outInt64[n] == gam1->int64Signal[n]);
                ok &= (outFloat32[n] == gam1->float32Signal[n]);
                ok &= (outFloat64[n] == gam1->float64Signal[n]);
            }
        }
        Sleep::Sec(0.1);
        timeOutCounts--;
    }
    godb->Purge();
    return ok;
}

