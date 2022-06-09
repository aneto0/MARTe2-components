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
#include "IntrospectionT.h"
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
        boolSignal = NULL;
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
            if (GetSignalType(OutputSignals, n) == Character8Bit) {
                char8Signal = reinterpret_cast<char8 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == UnsignedInteger8Bit) {
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
            else if (GetSignalType(OutputSignals, n) == BooleanType) {
                boolSignal = reinterpret_cast<bool *>(GetOutputSignalMemory(n));
                *boolSignal = false;
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
    bool *boolSignal;
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

//An introspectable structure
struct EPICSPVAOutputTestUInt {
    MARTe::uint8 UInt8;
    MARTe::uint16 UInt16;
    MARTe::uint32 UInt32;
    MARTe::uint64 UInt64;
};
struct EPICSPVAOutputTestInt {
    MARTe::int8 Int8;
    MARTe::int16 Int16;
    MARTe::int32 Int32;
    MARTe::int64 Int64;
};
struct EPICSPVAOutputTestFloat {
    MARTe::float32 Float32;
    MARTe::float64 Float64;
};
struct EPICSPVAOutputTestString {
    MARTe::char8 Char8[256];
    MARTe::char8 SString[128];
};
struct EPICSPVADatabaseTestOutputTypesS {
    struct EPICSPVAOutputTestUInt UInts;
    struct EPICSPVAOutputTestInt Ints;
    struct EPICSPVAOutputTestFloat Floats;
    struct EPICSPVAOutputTestString Strings;
    bool BooleanValue;
};

//The strategy is identical to the class registration
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestUInt, UInt8, uint8, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestUInt, UInt16, uint16, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestUInt, UInt32, uint32, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestUInt, UInt64, uint64, "", "");
static const MARTe::IntrospectionEntry* EPICSPVAOutputTestUIntStructEntries[] = { &EPICSPVAOutputTestUInt_UInt8_introspectionEntry, &EPICSPVAOutputTestUInt_UInt16_introspectionEntry,
        &EPICSPVAOutputTestUInt_UInt32_introspectionEntry, &EPICSPVAOutputTestUInt_UInt64_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAOutputTestUInt, EPICSPVAOutputTestUIntStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVAOutputTestInt, Int8, int8, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestInt, Int16, int16, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestInt, Int32, int32, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestInt, Int64, int64, "", "");
static const MARTe::IntrospectionEntry* EPICSPVAOutputTestIntStructEntries[] = { &EPICSPVAOutputTestInt_Int8_introspectionEntry, &EPICSPVAOutputTestInt_Int16_introspectionEntry,
        &EPICSPVAOutputTestInt_Int32_introspectionEntry, &EPICSPVAOutputTestInt_Int64_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAOutputTestInt, EPICSPVAOutputTestIntStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVAOutputTestFloat, Float32, float32, "", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestFloat, Float64, float64, "", "");
static const MARTe::IntrospectionEntry* EPICSPVAOutputTestFloatStructEntries[] = { &EPICSPVAOutputTestFloat_Float32_introspectionEntry, &EPICSPVAOutputTestFloat_Float64_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAOutputTestFloat, EPICSPVAOutputTestFloatStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVAOutputTestString, Char8, char8, "[256]", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestString, SString, char8, "[128]", "");
static const MARTe::IntrospectionEntry* EPICSPVAOutputTestStringStructEntries[] = { &EPICSPVAOutputTestString_Char8_introspectionEntry, &EPICSPVAOutputTestString_SString_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAOutputTestString, EPICSPVAOutputTestStringStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestOutputTypesS, UInts, EPICSPVAOutputTestUInt, "", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestOutputTypesS, Ints, EPICSPVAOutputTestInt, "", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestOutputTypesS, Floats, EPICSPVAOutputTestFloat, "", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestOutputTypesS, Strings, EPICSPVAOutputTestString, "", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestOutputTypesS, BooleanValue, bool, "", "");
static const MARTe::IntrospectionEntry* EPICSPVADatabaseTestOutputTypesSStructEntries[] = { &EPICSPVADatabaseTestOutputTypesS_UInts_introspectionEntry,
        &EPICSPVADatabaseTestOutputTypesS_Ints_introspectionEntry, &EPICSPVADatabaseTestOutputTypesS_Floats_introspectionEntry, &EPICSPVADatabaseTestOutputTypesS_Strings_introspectionEntry, &EPICSPVADatabaseTestOutputTypesS_BooleanValue_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVADatabaseTestOutputTypesS, EPICSPVADatabaseTestOutputTypesSStructEntries)

struct EPICSPVAOutputTestUIntA {
    MARTe::uint8 UInt8[4];
    MARTe::uint16 UInt16[4];
    MARTe::uint32 UInt32[4];
    MARTe::uint64 UInt64[4];
};
struct EPICSPVAOutputTestIntA {
    MARTe::int8 Int8[4];
    MARTe::int16 Int16[4];
    MARTe::int32 Int32[4];
    MARTe::int64 Int64[4];
};
struct EPICSPVAOutputTestFloatA {
    MARTe::float32 Float32[4];
    MARTe::float64 Float64[4];
};
struct EPICSPVADatabaseTestOutputTypesSA {
    struct EPICSPVAOutputTestUIntA UInts;
    struct EPICSPVAOutputTestIntA Ints;
    struct EPICSPVAOutputTestFloatA Floats[2];
};
//The strategy is identical to the class registration
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestUIntA, UInt8, uint8, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestUIntA, UInt16, uint16, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestUIntA, UInt32, uint32, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestUIntA, UInt64, uint64, "[4]", "");
static const MARTe::IntrospectionEntry* EPICSPVAOutputTestUIntAStructEntries[] = { &EPICSPVAOutputTestUIntA_UInt8_introspectionEntry, &EPICSPVAOutputTestUIntA_UInt16_introspectionEntry,
        &EPICSPVAOutputTestUIntA_UInt32_introspectionEntry, &EPICSPVAOutputTestUIntA_UInt64_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAOutputTestUIntA, EPICSPVAOutputTestUIntAStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVAOutputTestIntA, Int8, int8, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestIntA, Int16, int16, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestIntA, Int32, int32, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestIntA, Int64, int64, "[4]", "");
static const MARTe::IntrospectionEntry* EPICSPVAOutputTestIntAStructEntries[] = { &EPICSPVAOutputTestIntA_Int8_introspectionEntry, &EPICSPVAOutputTestIntA_Int16_introspectionEntry,
        &EPICSPVAOutputTestIntA_Int32_introspectionEntry, &EPICSPVAOutputTestIntA_Int64_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAOutputTestIntA, EPICSPVAOutputTestIntAStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVAOutputTestFloatA, Float32, float32, "[4]", "");
DECLARE_CLASS_MEMBER(EPICSPVAOutputTestFloatA, Float64, float64, "[4]", "");
static const MARTe::IntrospectionEntry* EPICSPVAOutputTestFloatAStructEntries[] = { &EPICSPVAOutputTestFloatA_Float32_introspectionEntry, &EPICSPVAOutputTestFloatA_Float64_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVAOutputTestFloatA, EPICSPVAOutputTestFloatAStructEntries)

DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestOutputTypesSA, UInts, EPICSPVAOutputTestUIntA, "", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestOutputTypesSA, Ints, EPICSPVAOutputTestIntA, "", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestOutputTypesSA, Floats, EPICSPVAOutputTestFloatA, "[2]", "");
static const MARTe::IntrospectionEntry* EPICSPVADatabaseTestOutputTypesSAStructEntries[] = { &EPICSPVADatabaseTestOutputTypesSA_UInts_introspectionEntry,
        &EPICSPVADatabaseTestOutputTypesSA_Ints_introspectionEntry, &EPICSPVADatabaseTestOutputTypesSA_Floats_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVADatabaseTestOutputTypesSA, EPICSPVADatabaseTestOutputTypesSAStructEntries)

/**
 * @brief GAM which writes signals into a given EPICSPVAOutput
 */
class EPICSPVAOutputGAMTestHelperS: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()EPICSPVAOutputGAMTestHelperS() {
        testStruct = NULL;
    }

    virtual ~EPICSPVAOutputGAMTestHelperS() {
        using namespace MARTe;
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        testStruct = reinterpret_cast<EPICSPVADatabaseTestOutputTypesS *>(GetOutputSignalMemory(0u));
        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        return true;
    }

    EPICSPVADatabaseTestOutputTypesS *testStruct;
};
CLASS_REGISTER(EPICSPVAOutputGAMTestHelperS, "1.0")

/**
 * @brief GAM which writes signals into a given EPICSPVAOutput
 */
class EPICSPVAOutputGAMTestHelperSA: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()EPICSPVAOutputGAMTestHelperSA() {
        testStruct = NULL;
    }

    virtual ~EPICSPVAOutputGAMTestHelperSA() {
        using namespace MARTe;
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        testStruct = reinterpret_cast<EPICSPVADatabaseTestOutputTypesSA *>(GetOutputSignalMemory(0u));
        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        return true;
    }

    EPICSPVADatabaseTestOutputTypesSA *testStruct;
};
CLASS_REGISTER(EPICSPVAOutputGAMTestHelperSA, "1.0")

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
    return test.Synchronise();
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
        ok &= (test.GetNumberOfMemoryBuffers() == 1);
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
        ok &= (test.GetNumberOfMemoryBuffers() == 1);
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

bool EPICSPVAOutputTest::TestSetConfiguredDatabase() {
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
            "    +RecordOut1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  Type = UnsignedIntegers\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Type = SignedIntegers\n"
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
            "                    Alias = RecordOut1.UInt8\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int32\n"
            "                }\n"
            "                SignalInt64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut3\n"
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
            "                    Type = UnsignedIntegers\n"
            "                    Field = UnsignedIntegers\n"
            "                }\n"
            "                RecordOut2 = {\n"
            "                    Type = SignedIntegers\n"
            "                    Field = SignedIntegers\n"
            "                }\n"
            "                RecordOut3 = {\n"
            "                     Field = Element1\n"
            "                     Type = float32\n"
            "                     NumberOfElements = 1\n"
            "                }\n"
            "                RecordOut4 = {\n"
            "                    Field = Element1\n"
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
            "    +RecordOut1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  Type = UnsignedIntegers\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Type = SignedIntegers\n"
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
            "                    Alias = RecordOut1.UInt8\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int32\n"
            "                }\n"
            "                SignalInt64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut3\n"
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
            "                    Type = UnsignedIntegers\n"
            "                    Field = UnsignedIntegers\n"
            "                }\n"
            "                RecordOut2 = {\n"
            "                    Type = SignedIntegers\n"
            "                    Field = SignedIntegers\n"
            "                }\n"
            "                RecordOut3 = {\n"
            "                     Field = Element1\n"
            "                     Type = float32\n"
            "                     NumberOfElements = 1\n"
            "                }\n"
            "                RecordOut4 = {\n"
            "                    Field = Element1\n"
            "                    Type = float64\n"
            "                    NumberOfElements = 1\n"
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
            "    +RecordOut1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  Type = UnsignedIntegers\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Type = SignedIntegers\n"
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
            "                    Alias = RecordOut1.UInt8\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int32\n"
            "                }\n"
            "                SignalInt64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut3\n"
            "                    Samples = 2"
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
            "                    Type = UnsignedIntegers\n"
            "                    Field = UnsignedIntegers\n"
            "                }\n"
            "                RecordOut2 = {\n"
            "                    Type = SignedIntegers\n"
            "                    Field = SignedIntegers\n"
            "                }\n"
            "                RecordOut3 = {\n"
            "                     Field = Element1\n"
            "                     Type = float32\n"
            "                     NumberOfElements = 1\n"
            "                }\n"
            "                RecordOut4 = {\n"
            "                    Field = Element1\n"
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
            "    +RecordOut1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  Type = UnsignedIntegers\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Type = SignedIntegers\n"
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
            "                    Alias = RecordOut1.UInt8\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAOutputGAMTestHelper\n"
            "            OutputSignals = {\n"
            "                SignalUInt8 = {\n"
            "                    Type = uint8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt8\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int32\n"
            "                }\n"
            "                SignalInt64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut3\n"
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
            "                    Type = UnsignedIntegers\n"
            "                    Field = UnsignedIntegers\n"
            "                }\n"
            "                RecordOut2 = {\n"
            "                    Type = SignedIntegers\n"
            "                    Field = SignedIntegers\n"
            "                }\n"
            "                RecordOut3 = {\n"
            "                     Field = Element1\n"
            "                     Type = float32\n"
            "                     NumberOfElements = 1\n"
            "                }\n"
            "                RecordOut4 = {\n"
            "                    Field = Element1\n"
            "                    Type = float64\n"
            "                    NumberOfElements = 1\n"
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
            "    +RecordOut1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  Type = UnsignedIntegers\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut2 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Alias = \"TEST::RECORDOUT2\"\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Type = SignedIntegers\n"
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
            "    +RecordOut5 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Alias = \"TEST::RECORDOUT5\"\n"
            "        Structure = {\n"
            "             Strings = {\n"
            "                  Type = EPICSPVAOutputTestString\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut6 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Alias = \"TEST::RECORDOUT6\"\n"
            "        Structure = {\n"
            "             Boolean = {\n"
            "                  Type = bool\n"
            "             }\n"
            "        }\n"
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
            "                    Alias = RecordOut1.UInt8\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt16\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt32\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1.UInt64\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int8\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int16\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int32\n"
            "                }\n"
            "                SignalInt64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2.Int64\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut3\n"
            "                }\n"
            "                SignalString = {\n"
            "                    Type = char8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    NumberOfElements = 128\n"
            "                    Alias = RecordOut5.SString\n"
            "                }\n"
            "                Boolean = {\n"
            "                    Type = bool\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    NumberOfElements = 1\n"
            "                    Alias = RecordOut6\n"
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
            "                    Type = UnsignedIntegers\n"
            "                    Field = UnsignedIntegers\n"
            "                }\n"
            "                RecordOut2 = {\n"
            "                    Alias = \"TEST::RECORDOUT2\""
            "                    Type = SignedIntegers\n"
            "                    Field = SignedIntegers\n"
            "                }\n"
            "                RecordOut3 = {\n"
            "                     Field = Element1\n"
            "                     Type = float32\n"
            "                     NumberOfElements = 1\n"
            "                }\n"
            "                RecordOut4 = {\n"
            "                    Field = Element1\n"
            "                    Type = float64\n"
            "                    NumberOfElements = 1\n"
            "                }\n"
            "                RecordOut5 = {\n"
            "                    Alias = \"TEST::RECORDOUT5\""
            "                    Field = Strings\n"
            "                    Type = EPICSPVAOutputTestString\n"
            "                }\n"
            "                RecordOut6 = {\n"
            "                    Alias = \"TEST::RECORDOUT6\""
            "                    Field = Boolean\n"
            "                    Type = bool\n"
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
        *gam1->boolSignal = true;
        StreamString expectedStringValue = "STRINGSIGNAL";

        StringHelper::CopyN(&gam1->char8Signal[0], expectedStringValue.Buffer(), expectedStringValue.Size());

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
            {
                pvac::ClientChannel record5(provider.connect("TEST::RECORDOUT5"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record5.get();
                std::shared_ptr<const epics::pvData::PVString> stringValue = getStruct->getSubField<epics::pvData::PVString>("Strings.SString");

                ok &= (stringValue ? true : false);
                if (ok) {
                    std::string val = stringValue->get();
                    ok = (expectedStringValue == val.c_str());
                }
            }
            {
                pvac::ClientChannel record6(provider.connect("TEST::RECORDOUT6"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record6.get();
                std::shared_ptr<const epics::pvData::PVBoolean> boolValue = getStruct->getSubField<epics::pvData::PVBoolean>("Boolean");

                ok &= (boolValue ? true : false);
                if (ok) {
                    ok = boolValue->get();
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
            "    +RecordOut1Arr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers = {\n"
            "                  Type = UnsignedIntegers\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "    +RecordOut2Arr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignedIntegers = {\n"
            "                  Type = SignedIntegers\n"
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
            "                    Alias = RecordOut1Arr.UInt8\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalUInt16 = {\n"
            "                    Type = uint16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1Arr.UInt16\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalUInt32 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1Arr.UInt32\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalUInt64 = {\n"
            "                    Type = uint64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1Arr.UInt64\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalFloat64 = {\n"
            "                    Type = float64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut4Arr\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt8 = {\n"
            "                    Type = int8\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2Arr.Int8\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt16 = {\n"
            "                    Type = int16\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2Arr.Int16\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt32 = {\n"
            "                    Type = int32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2Arr.Int32\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalInt64 = {\n"
            "                    Type = int64\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut2Arr.Int64\n"
            "                    NumberOfElements = 4\n"
            "                }\n"
            "                SignalFloat32 = {\n"
            "                    Type = float32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut3Arr\n"
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
            "                    Field = UnsignedIntegers\n"
            "                    Type = UnsignedIntegers\n"
            "                }\n"
            "                RecordOut2Arr = {\n"
            "                    Field = SignedIntegers\n"
            "                    Type = SignedIntegers\n"
            "                }\n"
            "                RecordOut3Arr = {\n"
            "                     Field = Element1\n"
            "                     Type = float32\n"
            "                     NumberOfElements = 4\n"
            "                }\n"
            "                RecordOut4Arr = {\n"
            "                    Field = Element1\n"
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

bool EPICSPVAOutputTest::TestSynchronise_StructuredType() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordOut1S = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "            SignalTypes = {\n"
            "                Type = EPICSPVADatabaseTestOutputTypesS\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAOutputGAMTestHelperS\n"
            "            OutputSignals = {\n"
            "                SignalTypes = {\n"
            "                    Type = EPICSPVADatabaseTestOutputTypesS\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1S\n"
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
            "                RecordOut1S = {\n"
            "                    Field = SignalTypes\n"
            "                    Type = EPICSPVADatabaseTestOutputTypesS\n"
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

    ReferenceT<EPICSPVAOutputGAMTestHelperS> gam1;
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
    gam1->testStruct->BooleanValue = false;
    if (ok) {
        gam1->testStruct->UInts.UInt8 = 1;
        gam1->testStruct->UInts.UInt16 = 2;
        gam1->testStruct->UInts.UInt32 = 3;
        gam1->testStruct->UInts.UInt64 = 4;
        gam1->testStruct->Ints.Int8 = -1;
        gam1->testStruct->Ints.Int16 = -2;
        gam1->testStruct->Ints.Int32 = -3;
        gam1->testStruct->Ints.Int64 = -4;
        gam1->testStruct->Floats.Float32 = 32;
        gam1->testStruct->Floats.Float64 = 64;
        gam1->testStruct->BooleanValue = true;

        scheduler->ExecuteThreadCycle(0u);
        pvac::ClientProvider provider("pva");
        uint32 timeOutCounts = 50;
        ok = false;
        while ((!ok) && (timeOutCounts != 0u)) {
            {
                pvac::ClientChannel record1(provider.connect("RecordOut1S"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record1.get();
                std::shared_ptr<const epics::pvData::PVUByte> uint8Value = getStruct->getSubField<epics::pvData::PVUByte>("SignalTypes.UInts.UInt8");
                std::shared_ptr<const epics::pvData::PVUShort> uint16Value = getStruct->getSubField<epics::pvData::PVUShort>("SignalTypes.UInts.UInt16");
                std::shared_ptr<const epics::pvData::PVUInt> uint32Value = getStruct->getSubField<epics::pvData::PVUInt>("SignalTypes.UInts.UInt32");
                std::shared_ptr<const epics::pvData::PVULong> uint64Value = getStruct->getSubField<epics::pvData::PVULong>("SignalTypes.UInts.UInt64");
                std::shared_ptr<const epics::pvData::PVByte> int8Value = getStruct->getSubField<epics::pvData::PVByte>("SignalTypes.Ints.Int8");
                std::shared_ptr<const epics::pvData::PVShort> int16Value = getStruct->getSubField<epics::pvData::PVShort>("SignalTypes.Ints.Int16");
                std::shared_ptr<const epics::pvData::PVInt> int32Value = getStruct->getSubField<epics::pvData::PVInt>("SignalTypes.Ints.Int32");
                std::shared_ptr<const epics::pvData::PVLong> int64Value = getStruct->getSubField<epics::pvData::PVLong>("SignalTypes.Ints.Int64");
                std::shared_ptr<const epics::pvData::PVFloat> float32Value = getStruct->getSubField<epics::pvData::PVFloat>("SignalTypes.Floats.Float32");
                std::shared_ptr<const epics::pvData::PVDouble> float64Value = getStruct->getSubField<epics::pvData::PVDouble>("SignalTypes.Floats.Float64");
                std::shared_ptr<const epics::pvData::PVBoolean> boolValue = getStruct->getSubField<epics::pvData::PVBoolean>("SignalTypes.BooleanValue");

                ok = (uint8Value ? true : false);
                if (ok) {
                    ok = (uint8Value->get() == gam1->testStruct->UInts.UInt8);
                    ok &= (uint16Value->get() == gam1->testStruct->UInts.UInt16);
                    ok &= (uint32Value->get() == gam1->testStruct->UInts.UInt32);
                    ok &= (uint64Value->get() == gam1->testStruct->UInts.UInt64);
                    ok &= (int8Value->get() == gam1->testStruct->Ints.Int8);
                    ok &= (int16Value->get() == gam1->testStruct->Ints.Int16);
                    ok &= (int32Value->get() == gam1->testStruct->Ints.Int32);
                    ok &= (int64Value->get() == gam1->testStruct->Ints.Int64);
                    ok &= (float32Value->get() == gam1->testStruct->Floats.Float32);
                    ok &= (float64Value->get() == gam1->testStruct->Floats.Float64);
                    ok &= (boolValue->get() == gam1->testStruct->BooleanValue);
                }
            }
            Sleep::Sec(0.1);
            timeOutCounts--;
        }
    }
    godb->Purge();

    return ok;
}

bool EPICSPVAOutputTest::TestSynchronise_Arrays_StructuredType() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordOut1SArr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignalTypes = {\n"
            "                  Type = EPICSPVADatabaseTestOutputTypesSA\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAOutputGAMTestHelperSA\n"
            "            OutputSignals = {\n"
            "                SignalTypes = {\n"
            "                    Type = EPICSPVADatabaseTestOutputTypesSA\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1SArr\n"
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
            "                RecordOut1SArr = {\n"
            "                    Field = SignalTypes\n"
            "                    Type = EPICSPVADatabaseTestOutputTypesSA\n"
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

    ReferenceT<EPICSPVAOutputGAMTestHelperSA> gam1;
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
            gam1->testStruct->UInts.UInt8[n] = n;
            gam1->testStruct->UInts.UInt16[n] = 2 * n;
            gam1->testStruct->UInts.UInt32[n] = 3 * n;
            gam1->testStruct->UInts.UInt64[n] = 4 * n;
            gam1->testStruct->Ints.Int8[n] = -1 * n;
            gam1->testStruct->Ints.Int16[n] = -2 * n;
            gam1->testStruct->Ints.Int32[n] = -3 * n;
            gam1->testStruct->Ints.Int64[n] = -4 * n;
            gam1->testStruct->Floats[0].Float32[n] = 32 * n;
            gam1->testStruct->Floats[0].Float64[n] = 64 * n;
            gam1->testStruct->Floats[1].Float32[n] = -32 * n;
            gam1->testStruct->Floats[1].Float64[n] = -64 * n;
        }
        scheduler->ExecuteThreadCycle(0u);
        pvac::ClientProvider provider("pva");
        uint32 timeOutCounts = 50;
        ok = false;
        while ((!ok) && (timeOutCounts != 0u)) {
            {
                pvac::ClientChannel record1(provider.connect("RecordOut1SArr"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record1.get();
                std::shared_ptr<const epics::pvData::PVUByteArray> uint8Value = getStruct->getSubField<epics::pvData::PVUByteArray>("SignalTypes.UInts.UInt8");
                std::shared_ptr<const epics::pvData::PVUShortArray> uint16Value = getStruct->getSubField<epics::pvData::PVUShortArray>("SignalTypes.UInts.UInt16");
                std::shared_ptr<const epics::pvData::PVUIntArray> uint32Value = getStruct->getSubField<epics::pvData::PVUIntArray>("SignalTypes.UInts.UInt32");
                std::shared_ptr<const epics::pvData::PVULongArray> uint64Value = getStruct->getSubField<epics::pvData::PVULongArray>("SignalTypes.UInts.UInt64");
                std::shared_ptr<const epics::pvData::PVByteArray> int8Value = getStruct->getSubField<epics::pvData::PVByteArray>("SignalTypes.Ints.Int8");
                std::shared_ptr<const epics::pvData::PVShortArray> int16Value = getStruct->getSubField<epics::pvData::PVShortArray>("SignalTypes.Ints.Int16");
                std::shared_ptr<const epics::pvData::PVIntArray> int32Value = getStruct->getSubField<epics::pvData::PVIntArray>("SignalTypes.Ints.Int32");
                std::shared_ptr<const epics::pvData::PVLongArray> int64Value = getStruct->getSubField<epics::pvData::PVLongArray>("SignalTypes.Ints.Int64");

                std::shared_ptr<const epics::pvData::PVStructureArray> pvStructValue = getStruct->getSubField<epics::pvData::PVStructureArray>("SignalTypes.Floats");
                epics::pvData::PVStructureArray::const_svector arr(static_cast<const epics::pvData::PVStructureArray*>(pvStructValue.operator ->())->view());

                std::shared_ptr<const epics::pvData::PVFloatArray> float32Value0 = arr[0].get()->getSubField<epics::pvData::PVFloatArray>("Float32");
                std::shared_ptr<const epics::pvData::PVDoubleArray> float64Value0 = arr[0].get()->getSubField<epics::pvData::PVDoubleArray>("Float64");
                std::shared_ptr<const epics::pvData::PVFloatArray> float32Value1 = arr[1].get()->getSubField<epics::pvData::PVFloatArray>("Float32");
                std::shared_ptr<const epics::pvData::PVDoubleArray> float64Value1 = arr[1].get()->getSubField<epics::pvData::PVDoubleArray>("Float64");

                ok = (uint8Value ? true : false);
                epics::pvData::shared_vector<const uint8> outUInt8;
                epics::pvData::shared_vector<const uint16> outUInt16;
                epics::pvData::shared_vector<const uint32> outUInt32;
                epics::pvData::shared_vector<const unsigned long int> outUInt64;
                epics::pvData::shared_vector<const int8> outInt8;
                epics::pvData::shared_vector<const int16> outInt16;
                epics::pvData::shared_vector<const int32> outInt32;
                epics::pvData::shared_vector<const long int> outInt64;
                epics::pvData::shared_vector<const float32> outFloat320;
                epics::pvData::shared_vector<const float64> outFloat640;
                epics::pvData::shared_vector<const float32> outFloat321;
                epics::pvData::shared_vector<const float64> outFloat641;

                outUInt8.resize(nOfElements);
                outUInt16.resize(nOfElements);
                outUInt32.resize(nOfElements);
                outUInt64.resize(nOfElements);
                outInt8.resize(nOfElements);
                outInt16.resize(nOfElements);
                outInt32.resize(nOfElements);
                outInt64.resize(nOfElements);
                outFloat320.resize(nOfElements);
                outFloat640.resize(nOfElements);
                outFloat321.resize(nOfElements);
                outFloat641.resize(nOfElements);

                if (ok) {
                    uint8Value->getAs < uint8 > (outUInt8);
                    uint16Value->getAs < uint16 > (outUInt16);
                    uint32Value->getAs < uint32 > (outUInt32);
                    uint64Value->getAs<unsigned long int>(outUInt64);
                    int8Value->getAs < int8 > (outInt8);
                    int16Value->getAs < int16 > (outInt16);
                    int32Value->getAs < int32 > (outInt32);
                    int64Value->getAs<long int>(outInt64);
                    float32Value0->getAs < float32 > (outFloat320);
                    float64Value0->getAs < float64 > (outFloat640);
                    float32Value1->getAs < float32 > (outFloat321);
                    float64Value1->getAs < float64 > (outFloat641);
                }
                for (n = 0u; (n < nOfElements) && (ok); n++) {
                    ok = (outUInt8[n] == gam1->testStruct->UInts.UInt8[n]);
                    ok &= (outUInt16[n] == gam1->testStruct->UInts.UInt16[n]);
                    ok &= (outUInt32[n] == gam1->testStruct->UInts.UInt32[n]);
                    ok &= (outUInt64[n] == gam1->testStruct->UInts.UInt64[n]);
                    ok &= (outInt8[n] == gam1->testStruct->Ints.Int8[n]);
                    ok &= (outInt16[n] == gam1->testStruct->Ints.Int16[n]);
                    ok &= (outInt32[n] == gam1->testStruct->Ints.Int32[n]);
                    ok &= (outInt64[n] == gam1->testStruct->Ints.Int64[n]);
                    ok &= (outFloat320[n] == gam1->testStruct->Floats[0].Float32[n]);
                    ok &= (outFloat640[n] == gam1->testStruct->Floats[0].Float64[n]);
                    ok &= (outFloat321[n] == gam1->testStruct->Floats[1].Float32[n]);
                    ok &= (outFloat641[n] == gam1->testStruct->Floats[1].Float64[n]);
                }
            }
            Sleep::Sec(0.1);
            timeOutCounts--;
        }
    }
    godb->Purge();
    return ok;
}

bool EPICSPVAOutputTest::TestSynchronise_False_CharString() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordOut1SArr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignalCharString = {\n"
            "                  Type = char8\n"
            "                  NumberOfElements = 64"
            "             }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAOutputGAMTestHelperS\n"
            "            OutputSignals = {\n"
            "                RecordOut1SArr = {\n"
            "                    Type = string\n"
            "                    DataSource = EPICSPVAOutputTest\n"
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
            "                RecordOut1SArr = {\n"
            "                    Field = SignalCharString\n"
            "                    Type = string\n"
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

    ReferenceT<RealTimeApplication> application;
    ReferenceT<EPICSPVAOutput> ds1;

    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ds1 = godb->Find("Test.Data.EPICSPVAOutputTest");
        ok = ds1.IsValid();
    }
    if (ok) {
        ok = !ds1->Synchronise();
    }

    godb->Purge();

    return ok;
}

bool EPICSPVAOutputTest::TestSynchronise_Arrays_Elements() {
    using namespace MARTe;
    StreamString config = ""
            "+Types = {\n"
            "    Class = ReferenceContainer"
            "    +UnsignedIntegers1 = {\n"
            "        Class = IntrospectionStructure"
            "        UInt32 = {\n"
            "            Type = uint32\n"
            "            NumberOfElements = 4\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordOut1Arr = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             UnsignedIntegers1 = {\n"
            "                  Type = UnsignedIntegers1\n"
            "                  NumberOfElements = 2\n"
            "             }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAM1 = {\n"
            "            Class = EPICSPVAOutputGAMTestHelper\n"
            "            OutputSignals = {\n"
            "                SignalUInt32 = {\n"
            "                    Type = UnsignedIntegers1\n"
            "                    DataSource = EPICSPVAOutputTest\n"
            "                    Alias = RecordOut1Arr[0]\n"
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
            "        +EPICSPVAOutputTest = {\n"
            "            Class = EPICSPVAOutput\n"
            "            CPUMask = 15\n"
            "            StackSize = 10000000\n"
            "            NumberOfBuffers = 2\n"
            "            Signals = {\n"
            "                RecordOut1Arr = {\n"
            "                    Field = UnsignedIntegers1\n"
            "                    Type = UnsignedIntegers1\n"
            "                    NumberOfElements = 2\n"
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
            gam1->uint32Signal[n] = 3 * n;
        }
        scheduler->ExecuteThreadCycle(0u);
        pvac::ClientProvider provider("pva");
        uint32 timeOutCounts = 50;
        ok = false;
        while ((!ok) && (timeOutCounts != 0u)) {
            {
                pvac::ClientChannel record1(provider.connect("RecordOut1Arr"));
                epics::pvData::PVStructure::const_shared_pointer getStruct = record1.get();

                std::shared_ptr<const epics::pvData::PVStructureArray> pvStructValue = getStruct->getSubField<epics::pvData::PVStructureArray>("UnsignedIntegers1");
                epics::pvData::PVStructureArray::const_svector arr(static_cast<const epics::pvData::PVStructureArray*>(pvStructValue.operator ->())->view());
                std::shared_ptr<const epics::pvData::PVUIntArray> uint32Value = arr[0].get()->getSubField<epics::pvData::PVUIntArray>("UInt32");
                ok = (uint32Value ? true : false);
                epics::pvData::shared_vector<const uint32> outUInt32;
                outUInt32.resize(nOfElements);
                if (ok) {
                    uint32Value->getAs < uint32 > (outUInt32);
                }
                for (n = 0u; (n < nOfElements) && (ok); n++) {
                    ok &= (outUInt32[n] == gam1->uint32Signal[n]);
                }
            }
            Sleep::Sec(0.1);
            timeOutCounts--;
        }
    }
    godb->Purge();
    return ok;
}

bool EPICSPVAOutputTest::TestSynchronise_False_BadSignal() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSPVADatabase1 = {\n"
            "    Class = EPICSPVADatabase\n"
            "    +RecordOut1 = {\n"
            "        Class = EPICSPVA::EPICSPVARecord\n"
            "        Structure = {\n"
            "             SignalUInt = {\n"
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
            "            Class = EPICSPVAOutputGAMTestHelper\n"
            "            OutputSignals = {\n"
            "                RecordOut1 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = EPICSPVAOutputTest\n"
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
            "                    Field = SignalUIntB\n"
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
            "        Class = EPICSPVAOutputSchedulerTestHelper\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";

    bool ok = TestIntegratedInApplication(config.Buffer(), false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<RealTimeApplication> application;
    ReferenceT<EPICSPVAOutput> ds1;

    if (ok) {
        application = godb->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ds1 = godb->Find("Test.Data.EPICSPVAOutputTest");
        ok = ds1.IsValid();
    }
    if (ok) {
        ok = !ds1->Synchronise();
    }

    godb->Purge();

    return ok;
}
