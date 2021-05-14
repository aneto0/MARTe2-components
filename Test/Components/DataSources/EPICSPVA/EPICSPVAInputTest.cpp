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
#include "CLASSREGISTER.h"
#include "Directory.h"
#include "DirectoryScanner.h"
#include "EPICSPVAInput.h"
#include "EPICSPVAInputTest.h"
#include "GAM.h"
#include "IntrospectionT.h"
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
public:CLASS_REGISTER_DECLARATION()EPICSPVAInputGAMTestHelper() {
        char8Signal = NULL;
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

    virtual bool Initialise(MARTe::StructuredDataI &data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        GetSignalNumberOfElements(InputSignals, 0, numberOfElements);
        uint32 n;
        uint32 numberOfSignals = GetNumberOfInputSignals();
        for (n = 0; n < numberOfSignals; n++) {
            if (GetSignalType(InputSignals, n) == Character8Bit) {
                char8Signal = reinterpret_cast<char8*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger8Bit) {
                uint8Signal = reinterpret_cast<uint8*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger8Bit) {
                int8Signal = reinterpret_cast<int8*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger16Bit) {
                uint16Signal = reinterpret_cast<uint16*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger16Bit) {
                int16Signal = reinterpret_cast<int16*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger32Bit) {
                uint32Signal = reinterpret_cast<uint32*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger32Bit) {
                int32Signal = reinterpret_cast<int32*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger64Bit) {
                uint64Signal = reinterpret_cast<uint64*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger64Bit) {
                int64Signal = reinterpret_cast<int64*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == Float32Bit) {
                float32Signal = reinterpret_cast<float32*>(GetInputSignalMemory(n));
            }
            else {
                float64Signal = reinterpret_cast<float64*>(GetInputSignalMemory(n));
            }
        }

        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        return true;
    }

    MARTe::char8 *char8Signal;
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

    EPICSPVAInputSchedulerTestHelper () : MARTe::GAMSchedulerI() {
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
        ReferenceT < RealTimeApplication > realTimeAppT = realTimeApp;
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

    MARTe::ScheduledState *const*scheduledStates;
};

CLASS_REGISTER(EPICSPVAInputSchedulerTestHelper, "1.0")

//An introspectable structure
struct EPICSPVAInputTestUInt {
    MARTe::uint8 UInt8;
    MARTe::uint16 UInt16;
    MARTe::uint32 UInt32;
    MARTe::uint64 UInt64;
};
struct EPICSPVAInputTestInt {
    MARTe::int8 Int8;
    MARTe::int16 Int16;
    MARTe::int32 Int32;
    MARTe::int64 Int64;
};
struct EPICSPVAInputTestFloat {
    MARTe::float32 Float32;
    MARTe::float64 Float64;
};
struct EPICSPVADatabaseTestInputTypesS {
    struct EPICSPVAInputTestUInt UInts;
    struct EPICSPVAInputTestInt Ints;
    struct EPICSPVAInputTestFloat Floats;
};
//The strategy is identical to the class registration
DECLARE_CLASS_MEMBER(EPICSPVAInputTestUInt, UInt8, uint8, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestUInt, UInt16, uint16, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestUInt, UInt32, uint32, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestUInt, UInt64, uint64, "", "");
static const MARTe::IntrospectionEntry *EPICSPVAInputTestUIntStructEntries[] =
        { &EPICSPVAInputTestUInt_UInt8_introspectionEntry, &EPICSPVAInputTestUInt_UInt16_introspectionEntry, &EPICSPVAInputTestUInt_UInt32_introspectionEntry,
                &EPICSPVAInputTestUInt_UInt64_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAInputTestUInt, EPICSPVAInputTestUIntStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVAInputTestInt, Int8, int8, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestInt, Int16, int16, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestInt, Int32, int32, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestInt, Int64, int64, "", "");
static const MARTe::IntrospectionEntry *EPICSPVAInputTestIntStructEntries[] = { &EPICSPVAInputTestInt_Int8_introspectionEntry,
        &EPICSPVAInputTestInt_Int16_introspectionEntry, &EPICSPVAInputTestInt_Int32_introspectionEntry, &EPICSPVAInputTestInt_Int64_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAInputTestInt, EPICSPVAInputTestIntStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVAInputTestFloat, Float32, float32, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestFloat, Float64, float64, "", "");
static const MARTe::IntrospectionEntry *EPICSPVAInputTestFloatStructEntries[] = { &EPICSPVAInputTestFloat_Float32_introspectionEntry,
        &EPICSPVAInputTestFloat_Float64_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAInputTestFloat, EPICSPVAInputTestFloatStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestInputTypesS, UInts, EPICSPVAInputTestUInt, "", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestInputTypesS, Ints, EPICSPVAInputTestInt, "", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestInputTypesS, Floats, EPICSPVAInputTestFloat, "", "");
static const MARTe::IntrospectionEntry *EPICSPVADatabaseTestInputTypesSStructEntries[] = { &EPICSPVADatabaseTestInputTypesS_UInts_introspectionEntry,
        &EPICSPVADatabaseTestInputTypesS_Ints_introspectionEntry, &EPICSPVADatabaseTestInputTypesS_Floats_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVADatabaseTestInputTypesS, EPICSPVADatabaseTestInputTypesSStructEntries)

struct EPICSPVAInputTestUIntA {
    MARTe::uint8 UInt8[4];
    MARTe::uint16 UInt16[4];
    MARTe::uint32 UInt32[4];
    MARTe::uint64 UInt64[4];
};
struct EPICSPVAInputTestIntA {
    MARTe::int8 Int8[4];
    MARTe::int16 Int16[4];
    MARTe::int32 Int32[4];
    MARTe::int64 Int64[4];
};
struct EPICSPVAInputTestFloatA {
    MARTe::float32 Float32[4];
    MARTe::float64 Float64[4];
};
struct EPICSPVADatabaseTestInputTypesSA {
    struct EPICSPVAInputTestUIntA UInts;
    struct EPICSPVAInputTestIntA Ints;
    struct EPICSPVAInputTestFloatA Floats;
};
//The strategy is identical to the class registration
DECLARE_CLASS_MEMBER(EPICSPVAInputTestUIntA, UInt8, uint8, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestUIntA, UInt16, uint16, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestUIntA, UInt32, uint32, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestUIntA, UInt64, uint64, "[4]", "");
static const MARTe::IntrospectionEntry *EPICSPVAInputTestUIntAStructEntries[] = { &EPICSPVAInputTestUIntA_UInt8_introspectionEntry,
        &EPICSPVAInputTestUIntA_UInt16_introspectionEntry, &EPICSPVAInputTestUIntA_UInt32_introspectionEntry, &EPICSPVAInputTestUIntA_UInt64_introspectionEntry,
        0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAInputTestUIntA, EPICSPVAInputTestUIntAStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVAInputTestIntA, Int8, int8, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestIntA, Int16, int16, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestIntA, Int32, int32, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestIntA, Int64, int64, "[4]", "");
static const MARTe::IntrospectionEntry *EPICSPVAInputTestIntAStructEntries[] = { &EPICSPVAInputTestIntA_Int8_introspectionEntry,
        &EPICSPVAInputTestIntA_Int16_introspectionEntry, &EPICSPVAInputTestIntA_Int32_introspectionEntry, &EPICSPVAInputTestIntA_Int64_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAInputTestIntA, EPICSPVAInputTestIntAStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVAInputTestFloatA, Float32, float32, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAInputTestFloatA, Float64, float64, "[4]", "");
static const MARTe::IntrospectionEntry *EPICSPVAInputTestFloatAStructEntries[] = { &EPICSPVAInputTestFloatA_Float32_introspectionEntry,
        &EPICSPVAInputTestFloatA_Float64_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAInputTestFloatA, EPICSPVAInputTestFloatAStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestInputTypesSA, UInts, EPICSPVAInputTestUIntA, "", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestInputTypesSA, Ints, EPICSPVAInputTestIntA, "", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestInputTypesSA, Floats, EPICSPVAInputTestFloatA, "", "");
static const MARTe::IntrospectionEntry *EPICSPVADatabaseTestInputTypesSAStructEntries[] = { &EPICSPVADatabaseTestInputTypesSA_UInts_introspectionEntry,
        &EPICSPVADatabaseTestInputTypesSA_Ints_introspectionEntry, &EPICSPVADatabaseTestInputTypesSA_Floats_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVADatabaseTestInputTypesSA, EPICSPVADatabaseTestInputTypesSAStructEntries)

/**
 * @brief GAM which writes signals into a given EPICSPVAInput
 */
class EPICSPVAInputGAMTestHelperS: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()EPICSPVAInputGAMTestHelperS() {
        testStruct = NULL;
    }

    virtual ~EPICSPVAInputGAMTestHelperS() {
        using namespace MARTe;
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        testStruct = reinterpret_cast<EPICSPVADatabaseTestInputTypesS *>(GetInputSignalMemory(0u));
        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        return true;
    }

    EPICSPVADatabaseTestInputTypesS *testStruct;
};
CLASS_REGISTER(EPICSPVAInputGAMTestHelperS, "1.0")

/**
 * @brief GAM which writes signals into a given EPICSPVAInput
 */
class EPICSPVAInputGAMTestHelperSA: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()EPICSPVAInputGAMTestHelperSA() {
        testStruct = NULL;
    }

    virtual ~EPICSPVAInputGAMTestHelperSA() {
        using namespace MARTe;
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        testStruct = reinterpret_cast<EPICSPVADatabaseTestInputTypesSA *>(GetInputSignalMemory(0u));
        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        return true;
    }

    EPICSPVADatabaseTestInputTypesSA *testStruct;
};
CLASS_REGISTER(EPICSPVAInputGAMTestHelperSA, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
EPICSPVAInputTest::~EPICSPVAInputTest() {
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
}

bool EPICSPVAInputTest::TestConstructor() {
    using namespace MARTe;
    EPICSPVAInput test;
    return (test.NumberOfReferences() == 0);
}

bool EPICSPVAInputTest::TestExecute() {
    using namespace MARTe;
    StreamString config = ""
            "+Types = {\n"
            "    Class = ReferenceContainer"
            "    +UnsignedIntegers = {\n"
            "        Class = IntrospectionStructure"
            "        UInt8 = {\n"
            "            Type = uint8\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        UInt16 = {\n"
            "            Type = uint16\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        UInt32 = {\n"
            "            Type = uint32\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        UInt64 = {\n"
            "            Type = uint64\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "    }\n"
            "    +SignedIntegers = {\n"
            "        Class = IntrospectionStructure"
            "        Int8 = {\n"
            "            Type = int8\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        Int16 = {\n"
            "            Type = int16\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        Int32 = {\n"
            "            Type = int32\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        Int64 = {\n"
            "            Type = int64\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordIn1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Alias = \"TEST_RECORD1\"\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  Type = UnsignedIntegers\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordIn2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Type = SignedIntegers\n"
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
            "    +RecordIn6 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            ElementString = {\n"
            "                Type = char8\n"
            "                NumberOfElements = 12\n"
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
            "                    Alias = RecordIn1.UInt8\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int32\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn3\n"
            "                }\n"
            "                SignalString = {\n"
            "                    Type = char8\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn6\n"
            "                    NumberOfElements = 12\n"
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
            "                    Alias = \"TEST_RECORD1\"\n"
            "                    Field = UnsignedIntegers"
            "                    Type = UnsignedIntegers"
            "                }\n"
            "                RecordIn2 = {\n"
            "                    Field = SignedIntegers"
            "                    Type = SignedIntegers"
            "                }\n"
            "                RecordIn3 = {\n"
            "                    Field = Element1"
            "                    Type = float32\n"
            "                    NumberOfElements = 1\n"
            "                }\n"
            "                RecordIn4 = {\n"
            "                    Field = Element1"
            "                    Type = float64\n"
            "                    NumberOfElements = 1\n"
            "                }\n"
            "                RecordIn6 = {\n"
            "                    Field = ElementString"
            "                    Type = char8\n"
            "                    NumberOfElements = 12\n"
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
    ReferenceT < RealTimeApplication > application;

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
        pvac::ClientChannel record1(provider.connect("TEST_RECORD1"));
        pvac::ClientChannel record2(provider.connect("RecordIn2"));
        pvac::ClientChannel record3(provider.connect("RecordIn3"));
        pvac::ClientChannel record4(provider.connect("RecordIn4"));
        pvac::ClientChannel record6(provider.connect("RecordIn6"));
        pvac::detail::PutBuilder putBuilder1 = record1.put();
        pvac::detail::PutBuilder putBuilder2 = record2.put();
        pvac::detail::PutBuilder putBuilder3 = record3.put();
        pvac::detail::PutBuilder putBuilder4 = record4.put();
        pvac::detail::PutBuilder putBuilder6 = record6.put();

        while ((!ok) && (timeOutCounts != 0u)) {
            putBuilder1.set("UnsignedIntegers.UInt8", 1);
            putBuilder1.set("UnsignedIntegers.UInt16", 2);
            putBuilder1.set("UnsignedIntegers.UInt32", 3);
            putBuilder1.set("UnsignedIntegers.UInt64", 4);
            putBuilder2.set("SignedIntegers.Int8", -1);
            putBuilder2.set("SignedIntegers.Int16", -2);
            putBuilder2.set("SignedIntegers.Int32", -3);
            putBuilder2.set("SignedIntegers.Int64", -4);
            putBuilder3.set("Element1", 32);
            putBuilder4.set("Element1", 64);

            const char8 *stringValueOut = "HELLOSTRING;HELLOSTRING,HELLOSTRING";
            const char8 *stringValueExpected = "HELLOSTRING;";
            putBuilder6.set("ElementString", stringValueOut);

            putBuilder1.exec();
            putBuilder2.exec();
            putBuilder3.exec();
            putBuilder4.exec();
            putBuilder6.exec();

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
            StreamString tmpString = gam1->char8Signal;
            ok &= (tmpString == stringValueExpected);

            Sleep::Sec(0.1);
            timeOutCounts--;
        }
    }
    godb->Purge();

    return ok;
}

bool EPICSPVAInputTest::TestExecute_StructuredType() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordIn1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Alias = \"TEST_RECORD1\"\n"
            "        Structure = {\n"
            "             SignalTypes = {\n"
            "                  Type = EPICSPVADatabaseTestInputTypesS\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAInputGAMTestHelperS\n"
            "            InputSignals = {\n"
            "                SignalTypes = {\n"
            "                    Type = EPICSPVADatabaseTestInputTypesS\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = \"RecordIn1\""
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
            "                    Alias = \"TEST_RECORD1\"\n"
            "                    Field = SignalTypes\n"
            "                    Type = EPICSPVADatabaseTestInputTypesS\n"
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

    ReferenceT < EPICSPVAInputGAMTestHelperS > gam1;
    ReferenceT < RealTimeApplication > application;

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
        gam1->testStruct->UInts.UInt8 = 0;
        gam1->testStruct->UInts.UInt16 = 0;
        gam1->testStruct->UInts.UInt32 = 0;
        gam1->testStruct->UInts.UInt64 = 0;
        gam1->testStruct->Ints.Int8 = 0;
        gam1->testStruct->Ints.Int16 = 0;
        gam1->testStruct->Ints.Int32 = 0;
        gam1->testStruct->Ints.Int64 = 0;
        gam1->testStruct->Floats.Float32 = 0;
        gam1->testStruct->Floats.Float64 = 0;

        pvac::ClientProvider provider("pva");
        uint32 timeOutCounts = 50;
        ok = false;
        pvac::ClientChannel record1(provider.connect("TEST_RECORD1"));
        pvac::detail::PutBuilder putBuilder1 = record1.put();

        while ((!ok) && (timeOutCounts != 0u)) {
            putBuilder1.set("SignalTypes.UInts.UInt8", 1);
            putBuilder1.set("SignalTypes.UInts.UInt16", 2);
            putBuilder1.set("SignalTypes.UInts.UInt32", 3);
            putBuilder1.set("SignalTypes.UInts.UInt64", 4);
            putBuilder1.set("SignalTypes.Ints.Int8", -1);
            putBuilder1.set("SignalTypes.Ints.Int16", -2);
            putBuilder1.set("SignalTypes.Ints.Int32", -3);
            putBuilder1.set("SignalTypes.Ints.Int64", -4);
            putBuilder1.set("SignalTypes.Floats.Float32", 32);
            putBuilder1.set("SignalTypes.Floats.Float64", 64);
            putBuilder1.exec();
            scheduler->ExecuteThreadCycle(0u);
            ok = (1 == gam1->testStruct->UInts.UInt8);
            ok &= (2 == gam1->testStruct->UInts.UInt16);
            ok &= (3 == gam1->testStruct->UInts.UInt32);
            ok &= (4 == gam1->testStruct->UInts.UInt64);
            ok &= (-1 == gam1->testStruct->Ints.Int8);
            ok &= (-2 == gam1->testStruct->Ints.Int16);
            ok &= (-3 == gam1->testStruct->Ints.Int32);
            ok &= (-4 == gam1->testStruct->Ints.Int64);
            ok &= (32 == gam1->testStruct->Floats.Float32);
            ok &= (64 == gam1->testStruct->Floats.Float64);
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
            "+Types = {\n"
            "    Class = ReferenceContainer"
            "    +UnsignedIntegers = {\n"
            "        Class = IntrospectionStructure"
            "        UInt8 = {\n"
            "            Type = uint8\n"
            "            NumberOfElements = 4\n"
            "        }\n"
            "        UInt16 = {\n"
            "            Type = uint16\n"
            "            NumberOfElements = 4\n"
            "        }\n"
            "        UInt32 = {\n"
            "            Type = uint32\n"
            "            NumberOfElements = 4\n"
            "        }\n"
            "        UInt64 = {\n"
            "            Type = uint64\n"
            "            NumberOfElements = 4\n"
            "        }\n"
            "    }\n"
            "    +SignedIntegers = {\n"
            "        Class = IntrospectionStructure"
            "        Int8 = {\n"
            "            Type = int8\n"
            "            NumberOfElements = 4\n"
            "        }\n"
            "        Int16 = {\n"
            "            Type = int16\n"
            "            NumberOfElements = 4\n"
            "        }\n"
            "        Int32 = {\n"
            "            Type = int32\n"
            "            NumberOfElements = 4\n"
            "        }\n"
            "        Int64 = {\n"
            "            Type = int64\n"
            "            NumberOfElements = 4\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordIn1Arr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  Type = UnsignedIntegers\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordIn2Arr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                 Type = SignedIntegers\n"
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
            "                    Alias = RecordIn1Arr.UInt8\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1Arr.UInt16\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1Arr.UInt32\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1Arr.UInt64\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn4Arr\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2Arr.Int8\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2Arr.Int16\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2Arr.Int32\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2Arr.Int64\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn3Arr\n"
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
            "                    Field = UnsignedIntegers"
            "                    Type = UnsignedIntegers"
            "                }\n"
            "                RecordIn2Arr = {\n"
            "                    Field = SignedIntegers"
            "                    Type = SignedIntegers"
            "                }\n"
            "                RecordIn3Arr = {\n"
            "                     Field = Element1"
            "                     Type = float32\n"
            "                     NumberOfElements = 4\n"
            "                }\n"
            "                RecordIn4Arr = {\n"
            "                    Field = Element1"
            "                    Type = float64\n"
            "                    NumberOfElements = 4\n"
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
    ReferenceT < RealTimeApplication > application;

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
        epics::pvData::shared_vector < uint8 > outUInt8;
        epics::pvData::shared_vector < uint16 > outUInt16;
        epics::pvData::shared_vector < uint32 > outUInt32;
        epics::pvData::shared_vector<unsigned long int> outUInt64;
        epics::pvData::shared_vector < int8 > outInt8;
        epics::pvData::shared_vector < int16 > outInt16;
        epics::pvData::shared_vector < int32 > outInt32;
        epics::pvData::shared_vector<long int> outInt64;
        epics::pvData::shared_vector < float32 > outFloat32;
        epics::pvData::shared_vector < float64 > outFloat64;

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
        epics::pvData::shared_vector<const uint8> outUInt8F = freeze(outUInt8);
        epics::pvData::shared_vector<const uint16> outUInt16F = freeze(outUInt16);
        epics::pvData::shared_vector<const uint32> outUInt32F = freeze(outUInt32);
        epics::pvData::shared_vector<const unsigned long int> outUInt64F = freeze(outUInt64);
        epics::pvData::shared_vector<const int8> outInt8F = freeze(outInt8);
        epics::pvData::shared_vector<const int16> outInt16F = freeze(outInt16);
        epics::pvData::shared_vector<const int32> outInt32F = freeze(outInt32);
        epics::pvData::shared_vector<const long int> outInt64F = freeze(outInt64);
        epics::pvData::shared_vector<const float32> outFloat32F = freeze(outFloat32);
        epics::pvData::shared_vector<const float64> outFloat64F = freeze(outFloat64);

        pvac::ClientProvider provider("pva");
        uint32 timeOutCounts = 50;
        ok = false;
        pvac::ClientChannel record1(provider.connect("RecordIn1Arr"));
        pvac::ClientChannel record2(provider.connect("RecordIn2Arr"));
        pvac::ClientChannel record3(provider.connect("RecordIn3Arr"));
        pvac::ClientChannel record4(provider.connect("RecordIn4Arr"));

        pvac::detail::PutBuilder putBuilder1 = record1.put();
        pvac::detail::PutBuilder putBuilder2 = record2.put();
        pvac::detail::PutBuilder putBuilder3 = record3.put();
        pvac::detail::PutBuilder putBuilder4 = record4.put();

        while ((!ok) && (timeOutCounts != 0u)) {

            putBuilder1.set("UnsignedIntegers.UInt8", outUInt8F);
            putBuilder1.set("UnsignedIntegers.UInt16", outUInt16F);
            putBuilder1.set("UnsignedIntegers.UInt32", outUInt32F);
            putBuilder1.set("UnsignedIntegers.UInt64", outUInt64F);

            putBuilder2.set("SignedIntegers.Int8", outInt8F);
            putBuilder2.set("SignedIntegers.Int16", outInt16F);
            putBuilder2.set("SignedIntegers.Int32", outInt32F);
            putBuilder2.set("SignedIntegers.Int64", outInt64F);

            putBuilder3.set("Element1", outFloat32F);
            putBuilder4.set("Element1", outFloat64F);

            putBuilder1.exec();
            putBuilder2.exec();
            putBuilder3.exec();
            putBuilder4.exec();

            scheduler->ExecuteThreadCycle(0u);
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

bool EPICSPVAInputTest::TestExecute_StructuredType_Arrays() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordIn1SArr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignalTypes = {\n"
            "                 Type = EPICSPVADatabaseTestInputTypesSA\n"
            "                 NumberOfElements = 1\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAInputGAMTestHelperSA\n"
            "            InputSignals = {\n"
            "                RecordIn1SArr = {\n"
            "                    Type = EPICSPVADatabaseTestInputTypesSA\n"
            "                    DataSource = EPICSPVAInputTest\n"
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
            "                RecordIn1SArr = {\n"
            "                    Field = SignalTypes"
            "                    Type = EPICSPVADatabaseTestInputTypesSA\n"
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

    ReferenceT < EPICSPVAInputGAMTestHelperSA > gam1;
    ReferenceT < RealTimeApplication > application;

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
        epics::pvData::shared_vector < uint8 > outUInt8;
        epics::pvData::shared_vector < uint16 > outUInt16;
        epics::pvData::shared_vector < uint32 > outUInt32;
        epics::pvData::shared_vector<unsigned long int> outUInt64;
        epics::pvData::shared_vector < int8 > outInt8;
        epics::pvData::shared_vector < int16 > outInt16;
        epics::pvData::shared_vector < int32 > outInt32;
        epics::pvData::shared_vector<long int> outInt64;
        epics::pvData::shared_vector < float32 > outFloat32;
        epics::pvData::shared_vector < float64 > outFloat64;

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
        epics::pvData::shared_vector<const uint8> outUInt8F = freeze(outUInt8);
        epics::pvData::shared_vector<const uint16> outUInt16F = freeze(outUInt16);
        epics::pvData::shared_vector<const uint32> outUInt32F = freeze(outUInt32);
        epics::pvData::shared_vector<const unsigned long int> outUInt64F = freeze(outUInt64);
        epics::pvData::shared_vector<const int8> outInt8F = freeze(outInt8);
        epics::pvData::shared_vector<const int16> outInt16F = freeze(outInt16);
        epics::pvData::shared_vector<const int32> outInt32F = freeze(outInt32);
        epics::pvData::shared_vector<const long int> outInt64F = freeze(outInt64);
        epics::pvData::shared_vector<const float32> outFloat32F = freeze(outFloat32);
        epics::pvData::shared_vector<const float64> outFloat64F = freeze(outFloat64);

        pvac::ClientProvider provider("pva");
        uint32 timeOutCounts = 50;
        ok = false;
        pvac::ClientChannel record1(provider.connect("RecordIn1SArr"));
        pvac::detail::PutBuilder putBuilder1 = record1.put();

        while ((!ok) && (timeOutCounts != 0u)) {
            putBuilder1.set("SignalTypes.UInts.UInt8", outUInt8F);
            putBuilder1.set("SignalTypes.UInts.UInt16", outUInt16F);
            putBuilder1.set("SignalTypes.UInts.UInt32", outUInt32F);
            putBuilder1.set("SignalTypes.UInts.UInt64", outUInt64F);
            putBuilder1.set("SignalTypes.Ints.Int8", outInt8F);
            putBuilder1.set("SignalTypes.Ints.Int16", outInt16F);
            putBuilder1.set("SignalTypes.Ints.Int32", outInt32F);
            putBuilder1.set("SignalTypes.Ints.Int64", outInt64F);
            putBuilder1.set("SignalTypes.Floats.Float32", outFloat32F);
            putBuilder1.set("SignalTypes.Floats.Float64", outFloat64F);
            putBuilder1.exec();

            scheduler->ExecuteThreadCycle(0u);
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
            for (n = 0u; (n < nOfElements); n++) {
                ok = (outUInt8[n] == gam1->testStruct->UInts.UInt8[n]);
                ok &= (outUInt16[n] == gam1->testStruct->UInts.UInt16[n]);
                ok &= (outUInt32[n] == gam1->testStruct->UInts.UInt32[n]);
                ok &= (outUInt64[n] == gam1->testStruct->UInts.UInt64[n]);
                ok &= (outInt8[n] == gam1->testStruct->Ints.Int8[n]);
                ok &= (outInt16[n] == gam1->testStruct->Ints.Int16[n]);
                ok &= (outInt32[n] == gam1->testStruct->Ints.Int32[n]);
                ok &= (outInt64[n] == gam1->testStruct->Ints.Int64[n]);
                ok &= (outFloat32[n] == gam1->testStruct->Floats.Float32[n]);
                ok &= (outFloat64[n] == gam1->testStruct->Floats.Float64[n]);
            }
        }
        Sleep::Sec(0.1);
        timeOutCounts--;
    }
    godb->Purge();
    return ok;
}

bool EPICSPVAInputTest::TestExecute_False_CharString() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordIn6 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            ElementString = {\n"
            "                Type = char8\n"
            "                NumberOfElements = 12\n"
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
            "                SignalString = {\n"
            "                    Type = string\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn6\n"
            "                    NumberOfElements = 1\n"
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
            "                RecordIn6 = {\n"
            "                    Field = ElementString"
            "                    Type = string\n"
            "                    NumberOfElements = 1\n"
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

    ReferenceT<EPICSPVAInput> ds1;
    ReferenceT < RealTimeApplication > application;

    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ds1 = godb->Find("Test.Data.EPICSPVAInputTest");
        ok = ds1.IsValid();
    }
    //Just to wait to increase coverage
    Sleep::Sec(0.5);
    godb->Purge();

    return ok;
}

bool EPICSPVAInputTest::TestAllocateMemory() {
    using namespace MARTe;
    EPICSPVAInput test;
    return test.AllocateMemory();
}

bool EPICSPVAInputTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    EPICSPVAInput test;
    return (test.GetNumberOfMemoryBuffers() == 0);
}

bool EPICSPVAInputTest::TestGetSignalMemoryBuffer() {
    return TestSetConfiguredDatabase();
}

bool EPICSPVAInputTest::TestGetBrokerName() {
    using namespace MARTe;
    EPICSPVAInput test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, InputSignals), "MemoryMapInputBroker") == 0);

    return ok;
}

bool EPICSPVAInputTest::TestGetBrokerName_OutputSignals() {
    using namespace MARTe;
    EPICSPVAInput test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, OutputSignals), "") == 0);

    return ok;
}

bool EPICSPVAInputTest::TestInitialise() {
    using namespace MARTe;
    EPICSPVAInput test;
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

bool EPICSPVAInputTest::TestInitialise_Defaults() {
    using namespace MARTe;
    EPICSPVAInput test;
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

bool EPICSPVAInputTest::TestInitialise_False_Signals() {
    using namespace MARTe;
    EPICSPVAInput test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 1);
    cdb.Write("StackSize", 100000);
    return !test.Initialise(cdb);
}

bool EPICSPVAInputTest::TestGetCPUMask() {
    return TestInitialise();
}

bool EPICSPVAInputTest::TestGetStackSize() {
    return TestInitialise();
}

bool EPICSPVAInputTest::TestSetConfiguredDatabase() {
    using namespace MARTe;
    StreamString config = ""
            "+Types = {\n"
            "    Class = ReferenceContainer"
            "    +UnsignedIntegers = {\n"
            "        Class = IntrospectionStructure"
            "        UInt8 = {\n"
            "            Type = uint8\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        UInt16 = {\n"
            "            Type = uint16\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        UInt32 = {\n"
            "            Type = uint32\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        UInt64 = {\n"
            "            Type = uint64\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "    }\n"
            "    +SignedIntegers = {\n"
            "        Class = IntrospectionStructure"
            "        Int8 = {\n"
            "            Type = int8\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        Int16 = {\n"
            "            Type = int16\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        Int32 = {\n"
            "            Type = int32\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        Int64 = {\n"
            "            Type = int64\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordIn1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Alias = \"TEST_RECORD1\"\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  Type = UnsignedIntegers\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordIn2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Type = SignedIntegers\n"
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
            "                    Alias = RecordIn1.UInt8\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int32\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn3\n"
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
            "                    Alias = \"TEST_RECORD1\"\n"
            "                    Field = UnsignedIntegers"
            "                    Type = UnsignedIntegers"
            "                }\n"
            "                RecordIn2 = {\n"
            "                    Field = SignedIntegers"
            "                    Type = SignedIntegers"
            "                }\n"
            "                RecordIn3 = {\n"
            "                    Field = Element1"
            "                    Type = float32\n"
            "                    NumberOfElements = 1\n"
            "                }\n"
            "                RecordIn4 = {\n"
            "                    Field = Element1"
            "                    Type = float64\n"
            "                    NumberOfElements = 1\n"
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

    bool ok = TestIntegratedInApplication(config.Buffer(), true);
    return ok;
}

bool EPICSPVAInputTest::TestSetConfiguredDatabase_False_NoSignals() {
    using namespace MARTe;
    StreamString config = ""
            "+Types = {\n"
            "    Class = ReferenceContainer"
            "    +UnsignedIntegers = {\n"
            "        Class = IntrospectionStructure"
            "        UInt8 = {\n"
            "            Type = uint8\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        UInt16 = {\n"
            "            Type = uint16\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        UInt32 = {\n"
            "            Type = uint32\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        UInt64 = {\n"
            "            Type = uint64\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "    }\n"
            "    +SignedIntegers = {\n"
            "        Class = IntrospectionStructure"
            "        Int8 = {\n"
            "            Type = int8\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        Int16 = {\n"
            "            Type = int16\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        Int32 = {\n"
            "            Type = int32\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        Int64 = {\n"
            "            Type = int64\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordIn1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Alias = \"TEST_RECORD1\"\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  Type = UnsignedIntegers\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordIn2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Type = SignedIntegers\n"
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
            "                    Alias = RecordIn1.UInt8\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int32\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn3\n"
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
            "                    Alias = \"TEST_RECORD1\"\n"
            "                    Field = UnsignedIntegers"
            "                    Type = UnsignedIntegers"
            "                }\n"
            "                RecordIn2 = {\n"
            "                    Field = SignedIntegers"
            "                    Type = SignedIntegers"
            "                }\n"
            "                RecordIn3 = {\n"
            "                    Field = Element1"
            "                    Type = float32\n"
            "                    NumberOfElements = 1\n"
            "                }\n"
            "                RecordIn4 = {\n"
            "                    Field = Element1"
            "                    Type = float64\n"
            "                    NumberOfElements = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +EPICSPVAInputTestNoSignals = {\n"
            "            Class = EPICSPVAInput\n"
            "            CPUMask = 15\n"
            "            StackSize = 10000000\n"
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
            "        Class = EPICSPVAInputSchedulerTestHelper\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";

    bool ok = TestIntegratedInApplication(config.Buffer(), true);
    return !ok;
}

bool EPICSPVAInputTest::TestSetConfiguredDatabase_False_Samples() {
    using namespace MARTe;
    StreamString config = ""
            "+Types = {\n"
            "    Class = ReferenceContainer"
            "    +UnsignedIntegers = {\n"
            "        Class = IntrospectionStructure"
            "        UInt8 = {\n"
            "            Type = uint8\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        UInt16 = {\n"
            "            Type = uint16\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        UInt32 = {\n"
            "            Type = uint32\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        UInt64 = {\n"
            "            Type = uint64\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "    }\n"
            "    +SignedIntegers = {\n"
            "        Class = IntrospectionStructure"
            "        Int8 = {\n"
            "            Type = int8\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        Int16 = {\n"
            "            Type = int16\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        Int32 = {\n"
            "            Type = int32\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        Int64 = {\n"
            "            Type = int64\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordIn1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Alias = \"TEST_RECORD1\"\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  Type = UnsignedIntegers\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordIn2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Type = SignedIntegers\n"
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
            "                    Alias = RecordIn1.UInt8\n"
            "                    Samples = 2\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn1.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int32\n"
            "                }\n"
            "                SignalIn64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn2.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAInputTest\n"
            "                    Alias = RecordIn3\n"
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
            "                    Alias = \"TEST_RECORD1\"\n"
            "                    Field = UnsignedIntegers"
            "                    Type = UnsignedIntegers"
            "                }\n"
            "                RecordIn2 = {\n"
            "                    Field = SignedIntegers"
            "                    Type = SignedIntegers"
            "                }\n"
            "                RecordIn3 = {\n"
            "                    Field = Element1"
            "                    Type = float32\n"
            "                    NumberOfElements = 1\n"
            "                }\n"
            "                RecordIn4 = {\n"
            "                    Field = Element1"
            "                    Type = float64\n"
            "                    NumberOfElements = 1\n"
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

    bool ok = !TestIntegratedInApplication(config.Buffer(), true);
    return ok;
}

bool EPICSPVAInputTest::TestSynchronise() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +TestSignal = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Alias = \"TEST_RECORD1\"\n"
            "        Structure = {\n"
            "             Element = {\n"
            "                  Type = uint32\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAInputGAMTestHelper\n"
            "            InputSignals = {\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    Frequency = 1\n"
            "                    Alias = TestSignal"
            "                    DataSource = EPICSPVAInputTest\n"
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
            "            NumberOfBuffers = 1\n"
            "            Signals = {\n"
            "                TestSignal = {\n"
            "                    Alias = \"TEST_RECORD1\"\n"
            "                    Field = Element\n"
            "                    Type = uint32\n"
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
    ReferenceT < RealTimeApplication > application;

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
        *gam1->uint32Signal = 0;

        pvac::ClientProvider provider("pva");
        uint32 timeOutCounts = 50;
        ok = false;
        pvac::ClientChannel record1(provider.connect("TEST_RECORD1"));
        pvac::detail::PutBuilder putBuilder1 = record1.put();

        while (!ok) {
            putBuilder1.set("Element", 3);

            putBuilder1.exec();

            scheduler->ExecuteThreadCycle(0u);
            ok = (3 == *gam1->uint32Signal);

            Sleep::Sec(0.1);
        }
    }
    godb->Purge();

    return ok;
}

bool EPICSPVAInputTest::TestGetBrokerNameSynchronised_too_many_channels() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +TestSignal = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Alias = \"TEST_RECORD1\"\n"
            "        Structure = {\n"
            "             Element = {\n"
            "                  Type = uint32\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +TestSignal2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Alias = \"TEST_RECORD2\"\n"
            "        Structure = {\n"
            "             Element = {\n"
            "                  Type = uint64\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAInputGAMTestHelper\n"
            "            InputSignals = {\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    Frequency = 1\n"
            "                    Alias = TestSignal"
            "                    DataSource = EPICSPVAInputTest\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint32\n"
            "                    Alias = TestSignal2"
            "                    DataSource = EPICSPVAInputTest\n"
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
            "            NumberOfBuffers = 1\n"
            "            Signals = {\n"
            "                TestSignal = {\n"
            "                    Alias = \"TEST_RECORD1\"\n"
            "                    Field = Element\n"
            "                    Type = uint32\n"
            "                }\n"
            "                TestSignal2 = {\n"
            "                    Alias = \"TEST_RECORD2\"\n"
            "                    Field = Element\n"
            "                    Type = uint64\n"
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
    ReferenceT < RealTimeApplication > application;

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

    godb->Purge();

    return !ok;
}

bool EPICSPVAInputTest::TestPrepareNextState() {
    using namespace MARTe;
    EPICSPVAInput test;
    return (test.PrepareNextState("", ""));
}
