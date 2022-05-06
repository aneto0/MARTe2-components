/**
 * @file FlattenedStructIOGAMTest.cpp
 * @brief Source file for class FlattenedStructIOGAMTest
 * @date 25/03/2022
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
 * the class FlattenedStructIOGAMTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "FlattenedStructIOGAM.h"
#include "FlattenedStructIOGAMTest.h"
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
 * Gives access to the FlattenedStructIOGAM memory for the Execute test
 */
class FlattenedStructIOGAMHelper: public MARTe::FlattenedStructIOGAM {
public:
    CLASS_REGISTER_DECLARATION()FlattenedStructIOGAMHelper() : MARTe::FlattenedStructIOGAM() {
    }

    virtual ~FlattenedStructIOGAMHelper() {

    }

    void *GetInputSignalsMemory() {
        return FlattenedStructIOGAM::GetInputSignalsMemory();
    }

    void *GetOutputSignalMemory(MARTe::uint32 idx) {
        return FlattenedStructIOGAM::GetOutputSignalMemory(idx);
    }
};
CLASS_REGISTER(FlattenedStructIOGAMHelper, "1.0");

/**
 * NOOP support GAM 
 */
class FlattenedStructIOGAMHelper2: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()FlattenedStructIOGAMHelper2() : MARTe::GAM() {
    }

    virtual ~FlattenedStructIOGAMHelper2() {

    }

    bool Setup() {
        using namespace MARTe;
        bool ret = true;
        uint32 n;
        uint32 inTotalSignalsByteSize = 0u;
        for (n = 0u; (n < GetNumberOfInputSignals()) && (ret); n++) {
            uint32 inByteSize = 0u;
            uint32 inSamples = 1u;
            ret = GetSignalByteSize(InputSignals, n, inByteSize);
            if (ret) {
                ret = GetSignalNumberOfSamples(InputSignals, n, inSamples);
            }
            if (ret) {
                inByteSize *= inSamples;
                inTotalSignalsByteSize += inByteSize;
            }
            StreamString tmp;
            GetSignalName(InputSignals, n, tmp);
        }
        uint32 outTotalSignalsByteSize = 0u;
        for (n = 0u; (n < GetNumberOfOutputSignals()) && (ret); n++) {
            uint32 outByteSize = 0u;
            uint32 outSamples = 1u;
            ret = GetSignalByteSize(OutputSignals, n, outByteSize);
            if (ret) {
                ret = GetSignalNumberOfSamples(OutputSignals, n, outSamples);
            }
            if (ret) {
                outByteSize *= outSamples;
                outTotalSignalsByteSize += outByteSize;
            }
        }
        if (ret) {
            ret = (inTotalSignalsByteSize == outTotalSignalsByteSize);
        }
        if (ret) {
            totalSignalsByteSize = outTotalSignalsByteSize;
        }

        return ret;
    }

    bool Execute() {
        using namespace MARTe;
        return MemoryOperationsHelper::Copy(GetOutputSignalsMemory(), GetInputSignalsMemory(), totalSignalsByteSize);
    }

private:
    MARTe::uint32 totalSignalsByteSize;
};
CLASS_REGISTER(FlattenedStructIOGAMHelper2, "1.0");

struct __attribute__((__packed__)) FlattenedStructIOGAMTest_MyType0 {
    MARTe::uint16 MyUInt16;
};

struct __attribute__((__packed__)) FlattenedStructIOGAMTest_MyType1 {
    MARTe::uint32 MyUInt32;
    MARTe::float32 MyFloat32Array[8];
    FlattenedStructIOGAMTest_MyType0 MyType0Array[2];
};

struct __attribute__((__packed__)) FlattenedStructIOGAMTest_MyType2 {
    MARTe::float64 MyFloat64;
    FlattenedStructIOGAMTest_MyType1 MyType1Array [2];
};

/**
 * A dummy DataSource which can be used to test different configuration interfaces
 *  to the FlattenedStructIOGAM
 */
class FlattenedStructIOGAMDataSourceHelper: public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

FlattenedStructIOGAMDataSourceHelper    () : MARTe::DataSourceI() {
    }

    virtual ~FlattenedStructIOGAMDataSourceHelper() {

    }

    virtual bool AllocateMemory() {
        return true;
    }

    virtual MARTe::uint32 GetNumberOfMemoryBuffers() {
        return 1;
    }

    virtual bool GetSignalMemoryBuffer(const MARTe::uint32 signalIdx,
            const MARTe::uint32 bufferIdx,
            void *&signalAddress) {
        signalAddress = &dataSourceMemory;
        return true;
    }

    virtual const MARTe::char8 *GetBrokerName(MARTe::StructuredDataI &data,
            const MARTe::SignalDirection direction) {
        if (direction == MARTe::InputSignals) {
            return "MemoryMapInputBroker";
        }
        return "MemoryMapOutputBroker";
    }

    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName,
            const MARTe::char8 * const nextStateName) {
        return true;
    }

    virtual bool Synchronise() {
        return true;
    }

    FlattenedStructIOGAMTest_MyType2 dataSourceMemory;
};
CLASS_REGISTER(FlattenedStructIOGAMDataSourceHelper, "1.0");

class FlattenedStructIOGAMScheduler: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

    FlattenedStructIOGAMScheduler() :
            MARTe::GAMSchedulerI() {
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
        ReferenceT<RealTimeApplication> rtAppT = realTimeApp;
        ExecuteSingleCycle(scheduledStates[rtAppT->GetIndex()]->threads[threadId].executables,
                scheduledStates[rtAppT->GetIndex()]->threads[threadId].numberOfExecutables);
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
CLASS_REGISTER(FlattenedStructIOGAMScheduler, "1.0");

/**
 * Starts a MARTe application that uses this GAM instance.
 */
static bool TestIntegratedInApplication(const MARTe::char8 * const config,
                                        bool destroy = true) {
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

    if (destroy) {
        god->Purge();
    }
    else {
        ok = application->PrepareNextState("State1");
        if (ok) {
            ok = application->StartNextStateExecution();
        }
    }

    return ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool FlattenedStructIOGAMTest::TestConstructor() {
    using namespace MARTe;
    FlattenedStructIOGAM gam;
    return (gam.GetNumberOfInputSignals() == 0u) && (gam.GetNumberOfOutputSignals() == 0u);
}

bool FlattenedStructIOGAMTest::TestInitialise() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "+Types = {"
            "    Class = ReferenceContainer"
            "    +MyType0 = {"
            "        Class = IntrospectionStructure"
            "        MyUInt16 = {"
            "            Type = uint16"
            "            NumberOfElements = 1"
            "        }"
            "    }"
            "    +MyType1 = {"
            "        Class = IntrospectionStructure"
            "        MyUInt32 = {"
            "            Type = uint32"
            "                NumberOfElements = 1"
            "        }"
            "        MyFloat32Array = {"
            "            Type = float32"
            "            NumberOfElements = 8"
            "        }"
            "        MyType0Array = {"
            "            Type = MyType0"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "    +MyType2 = {"
            "        Class = IntrospectionStructure"
            "        MyFloat64 = {"
            "            Type = float64"
            "            NumberOfElements = 1"
            "        }"
            "        MyType1Array= {"
            "            Type = MyType1"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "}"
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAMP = {"
            "            Class = FlattenedStructIOGAMHelper2"
            "            InputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = Drv1"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = DDB1"
            "               }"
            "            }"
            "       }"
            "       +FlatIOGAM = {"
            "           Class = FlattenedStructIOGAM"
            "           InputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = DDB1"
            "               } "
            "           }"
            "           OutputSignals = {"
            "               StructArrayType_MyFloat64 = {"
            "                   Type = float64"
            "                   NumberOfElements = 1"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyUInt32s = {"
            "                   Type = uint32"
            "                   NumberOfElements = 2"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyFloat32s = {"
            "                   Type = float32"
            "                   NumberOfElements = 16"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyType0Array_MyUInt16s = {"
            "                   Type = uint16"
            "                   NumberOfElements = 4"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +Drv1 = {"
            "            Class = FlattenedStructIOGAMDataSourceHelper"
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
            "                    Functions = {GAMP FlatIOGAM}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    bool ok = TestIntegratedInApplication(config1);
    return ok;
}

bool FlattenedStructIOGAMTest::TestSetup() {
    return TestInitialise();
}

bool FlattenedStructIOGAMTest::TestInitialise_MoreThanOneInput() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "+Types = {"
            "    Class = ReferenceContainer"
            "    +MyType0 = {"
            "        Class = IntrospectionStructure"
            "        MyUInt16 = {"
            "            Type = uint16"
            "            NumberOfElements = 1"
            "        }"
            "    }"
            "    +MyType1 = {"
            "        Class = IntrospectionStructure"
            "        MyUInt32 = {"
            "            Type = uint32"
            "                NumberOfElements = 1"
            "        }"
            "        MyFloat32Array = {"
            "            Type = float32"
            "            NumberOfElements = 8"
            "        }"
            "        MyType0Array = {"
            "            Type = MyType0"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "    +MyType2 = {"
            "        Class = IntrospectionStructure"
            "        MyFloat64 = {"
            "            Type = float64"
            "            NumberOfElements = 1"
            "        }"
            "        MyType1Array= {"
            "            Type = MyType1"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "}"
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAMP = {"
            "            Class = FlattenedStructIOGAMHelper2"
            "            InputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = Drv1"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = DDB1"
            "               }"
            "            }"
            "       }"
            "       +FlatIOGAM = {"
            "           Class = FlattenedStructIOGAM"
            "           InputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType2 = {"
            "                   Type = MyType2"
            "                   DataSource = Drv1"
            "               }"
            "           }"
            "           OutputSignals = {"
            "               StructArrayType_MyFloat64 = {"
            "                   Type = float64"
            "                   NumberOfElements = 1"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyUInt32s = {"
            "                   Type = uint32"
            "                   NumberOfElements = 2"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyFloat32s = {"
            "                   Type = float32"
            "                   NumberOfElements = 16"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyType0Array_MyUInt16s = {"
            "                   Type = uint16"
            "                   NumberOfElements = 4"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +Drv1 = {"
            "            Class = FlattenedStructIOGAMDataSourceHelper"
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
            "                    Functions = {GAMP FlatIOGAM}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    bool ok = !TestIntegratedInApplication(config1);
    return ok;
}

bool FlattenedStructIOGAMTest::TestInitialise_NoType() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "+Types = {"
            "    Class = ReferenceContainer"
            "    +MyType0 = {"
            "        Class = IntrospectionStructure"
            "        MyUInt16 = {"
            "            Type = uint16"
            "            NumberOfElements = 1"
            "        }"
            "    }"
            "    +MyType1 = {"
            "        Class = IntrospectionStructure"
            "        MyUInt32 = {"
            "            Type = uint32"
            "                NumberOfElements = 1"
            "        }"
            "        MyFloat32Array = {"
            "            Type = float32"
            "            NumberOfElements = 8"
            "        }"
            "        MyType0Array = {"
            "            Type = MyType0"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "    +MyType2 = {"
            "        Class = IntrospectionStructure"
            "        MyFloat64 = {"
            "            Type = float64"
            "            NumberOfElements = 1"
            "        }"
            "        MyType1Array= {"
            "            Type = MyType1"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "}"
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAMP = {"
            "            Class = FlattenedStructIOGAMHelper2"
            "            InputSignals = {"
            "               StructArrayType = {"
            "                   DataSource = Drv1"
            "                   Type = MyType2"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = DDB1"
            "               }"
            "            }"
            "       }"
            "       +FlatIOGAM = {"
            "           Class = FlattenedStructIOGAM"
            "           InputSignals = {"
            "               StructArrayType = {"
            "                   DataSource = DDB1"
            "               } "
            "           }"
            "           OutputSignals = {"
            "               StructArrayType_MyFloat64 = {"
            "                   Type = float64"
            "                   NumberOfElements = 1"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyUInt32s = {"
            "                   Type = uint32"
            "                   NumberOfElements = 2"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyFloat32s = {"
            "                   Type = float32"
            "                   NumberOfElements = 16"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyType0Array_MyUInt16s = {"
            "                   Type = uint16"
            "                   NumberOfElements = 4"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +Drv1 = {"
            "            Class = FlattenedStructIOGAMDataSourceHelper"
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
            "                    Functions = {GAMP FlatIOGAM}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    bool ok = !TestIntegratedInApplication(config1);
    return ok;
}

bool FlattenedStructIOGAMTest::TestInitialise_NoDataSource() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "+Types = {"
            "    Class = ReferenceContainer"
            "    +MyType0 = {"
            "        Class = IntrospectionStructure"
            "        MyUInt16 = {"
            "            Type = uint16"
            "            NumberOfElements = 1"
            "        }"
            "    }"
            "    +MyType1 = {"
            "        Class = IntrospectionStructure"
            "        MyUInt32 = {"
            "            Type = uint32"
            "                NumberOfElements = 1"
            "        }"
            "        MyFloat32Array = {"
            "            Type = float32"
            "            NumberOfElements = 8"
            "        }"
            "        MyType0Array = {"
            "            Type = MyType0"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "    +MyType2 = {"
            "        Class = IntrospectionStructure"
            "        MyFloat64 = {"
            "            Type = float64"
            "            NumberOfElements = 1"
            "        }"
            "        MyType1Array= {"
            "            Type = MyType1"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "}"
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAMP = {"
            "            Class = FlattenedStructIOGAMHelper2"
            "            InputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = Drv1"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = DDB1"
            "               }"
            "            }"
            "       }"
            "       +FlatIOGAM = {"
            "           Class = FlattenedStructIOGAM"
            "           InputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "               } "
            "           }"
            "           OutputSignals = {"
            "               StructArrayType_MyFloat64 = {"
            "                   Type = float64"
            "                   NumberOfElements = 1"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyUInt32s = {"
            "                   Type = uint32"
            "                   NumberOfElements = 2"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyFloat32s = {"
            "                   Type = float32"
            "                   NumberOfElements = 16"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyType0Array_MyUInt16s = {"
            "                   Type = uint16"
            "                   NumberOfElements = 4"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +Drv1 = {"
            "            Class = FlattenedStructIOGAMDataSourceHelper"
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
            "                    Functions = {GAMP FlatIOGAM}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    bool ok = !TestIntegratedInApplication(config1);
    return ok;
}

bool FlattenedStructIOGAMTest::TestSetup_False_DifferentSize() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "+Types = {"
            "    Class = ReferenceContainer"
            "    +MyType0 = {"
            "        Class = IntrospectionStructure"
            "        MyUInt16 = {"
            "            Type = uint16"
            "            NumberOfElements = 1"
            "        }"
            "    }"
            "    +MyType1 = {"
            "        Class = IntrospectionStructure"
            "        MyUInt32 = {"
            "            Type = uint32"
            "                NumberOfElements = 1"
            "        }"
            "        MyFloat32Array = {"
            "            Type = float32"
            "            NumberOfElements = 8"
            "        }"
            "        MyType0Array = {"
            "            Type = MyType0"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "    +MyType2 = {"
            "        Class = IntrospectionStructure"
            "        MyFloat64 = {"
            "            Type = float64"
            "            NumberOfElements = 1"
            "        }"
            "        MyType1Array= {"
            "            Type = MyType1"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "}"
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAMP = {"
            "            Class = FlattenedStructIOGAMHelper2"
            "            InputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = Drv1"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = DDB1"
            "               }"
            "            }"
            "       }"
            "       +FlatIOGAM = {"
            "           Class = FlattenedStructIOGAM"
            "           InputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = DDB1"
            "               } "
            "           }"
            "           OutputSignals = {"
            "               StructArrayType_MyFloat64 = {"
            "                   Type = float64"
            "                   NumberOfElements = 1"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyUInt32s = {"
            "                   Type = uint32"
            "                   NumberOfElements = 2"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyFloat32s = {"
            "                   Type = float32"
            "                   NumberOfElements = 16"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +Drv1 = {"
            "            Class = FlattenedStructIOGAMDataSourceHelper"
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
            "                    Functions = {GAMP FlatIOGAM}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    bool ok = TestIntegratedInApplication(config1);
    return !ok;
}

bool FlattenedStructIOGAMTest::TestExecute() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "+Types = {"
            "    Class = ReferenceContainer"
            "    +MyType0 = {"
            "        Class = IntrospectionStructure"
            "        MyUInt16 = {"
            "            Type = uint16"
            "            NumberOfElements = 1"
            "        }"
            "    }"
            "    +MyType1 = {"
            "        Class = IntrospectionStructure"
            "        MyUInt32 = {"
            "            Type = uint32"
            "                NumberOfElements = 1"
            "        }"
            "        MyFloat32Array = {"
            "            Type = float32"
            "            NumberOfElements = 8"
            "        }"
            "        MyType0Array = {"
            "            Type = MyType0"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "    +MyType2 = {"
            "        Class = IntrospectionStructure"
            "        MyFloat64 = {"
            "            Type = float64"
            "            NumberOfElements = 1"
            "        }"
            "        MyType1Array= {"
            "            Type = MyType1"
            "            NumberOfElements = 2"
            "        }"
            "    }"
            "}"
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAMP = {"
            "            Class = FlattenedStructIOGAMHelper2"
            "            InputSignals = {"
            "               StructArrayType = {"
            "                   Type = uint8"
            "                   DataSource = Drv1"
            "                   NumberOfElements = 88"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = DDB1"
            "               }"
            "            }"
            "       }"
            "       +FlatIOGAM = {"
            "           Class = FlattenedStructIOGAMHelper"
            "           InputSignals = {"
            "               StructArrayType = {"
            "                   Type = MyType2"
            "                   DataSource = DDB1"
            "               } "
            "           }"
            "           OutputSignals = {"
            "               StructArrayType_MyFloat64 = {"
            "                   Type = float64"
            "                   NumberOfElements = 1"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyUInt32s = {"
            "                   Type = uint32"
            "                   NumberOfElements = 2"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyFloat32s = {"
            "                   Type = float32"
            "                   NumberOfElements = 16"
            "                   DataSource = DDB1"
            "               }"
            "               StructArrayType_MyType1Array_MyType0Array_MyUInt16s = {"
            "                   Type = uint16"
            "                   NumberOfElements = 4"
            "                   DataSource = DDB1"
            "               }"
            "           }"
            "       }"
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
            "        +Drv1 = {"
            "            Class = FlattenedStructIOGAMDataSourceHelper"
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
            "                    Functions = {GAMP FlatIOGAM}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = FlattenedStructIOGAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = TestIntegratedInApplication(config1, false);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<FlattenedStructIOGAMHelper> gamT = god->Find("Test.Functions.FlatIOGAM");
    if (ok) {
        ok = gamT.IsValid();
    }

    ReferenceT<FlattenedStructIOGAMDataSourceHelper> drv1 = god->Find("Test.Data.Drv1");
    if (ok) {
        ok = drv1.IsValid();
    }

    FlattenedStructIOGAMTest_MyType2 *inMem = &drv1->dataSourceMemory;
    float64 *s1 = static_cast<float64 *>(gamT->GetOutputSignalMemory(0));
    uint32 *s2 = static_cast<uint32 *>(gamT->GetOutputSignalMemory(1));
    float32 *s3 = static_cast<float32 *>(gamT->GetOutputSignalMemory(2));
    uint16 *s4 = static_cast<uint16 *>(gamT->GetOutputSignalMemory(3));

    inMem->MyFloat64 = 10.0;
    for (uint k=0; k<2; k++) {
        inMem->MyType1Array[k].MyUInt32 = k + 2;
    }
    for (uint k=0; k<2; k++) {
        for (uint n=0; n<8; n++) {
            inMem->MyType1Array[k].MyFloat32Array[n] = static_cast<float32>(k + 1) * (n + 3);
        }
    }
    for (uint k=0; k<2; k++) {
        for (uint n=0; n<2; n++) {
            inMem->MyType1Array[k].MyType0Array[n].MyUInt16 = static_cast<uint16>(k + 2) * (n + 4);
        }
    }

    ReferenceT<FlattenedStructIOGAMScheduler> schedT = god->Find("Test.Scheduler");
    if (ok) {
        ok = schedT.IsValid();
    }
    if (ok) {
        schedT->ExecuteThreadCycle(0);
    }
    if (ok) {
        ok = (*s1 == 10.0);
    }
    for (uint k=0; (k<2) && (ok); k++) {
        ok = (s2[k] == (k + 2));
    }
    for (uint k=0; (k<2) && (ok); k++) {
        for (uint n=0; (n<8) && (ok); n++) {
            ok = (s3[k * 8 + n] == static_cast<float32>(k + 1) * (n + 3));
        }
    }
    for (uint k=0; (k<2) && (ok); k++) {
        for (uint n=0; (n<2) && (ok); n++) {
            ok = (s4[k * 2 + n] == static_cast<uint16>(k + 2) * (n + 4));
        }
    }

    god->Purge();
    return ok;

}

