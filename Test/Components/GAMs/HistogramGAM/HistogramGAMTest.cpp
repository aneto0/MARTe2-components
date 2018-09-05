/**
 * @file HistogramGAMTest.cpp
 * @brief Source file for class HistogramGAMTest
 * @date 30/08/2018
 * @author Giuseppe Ferro
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
 * the class HistogramGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "MemoryDataSourceI.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "MessageI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "HistogramGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class HistogramGAMTestGAM: public HistogramGAM {
public:

    CLASS_REGISTER_DECLARATION()

    HistogramGAMTestGAM();
    virtual ~HistogramGAMTestGAM();

    HistogramComparator ** GetComps();

    uint32 GetBeginCycle();

    uint32 GetCycleCounter();

    void *GetInputSignalsMemory();

    void *GetOutputSignalsMemory();

};

HistogramGAMTestGAM::HistogramGAMTestGAM() {

}

HistogramGAMTestGAM::~HistogramGAMTestGAM() {

}

HistogramComparator ** HistogramGAMTestGAM::GetComps() {
    return comps;
}

uint32 HistogramGAMTestGAM::GetBeginCycle() {
    return beginCycle;
}

uint32 HistogramGAMTestGAM::GetCycleCounter() {
    return cycleCounter;
}

void *HistogramGAMTestGAM::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void *HistogramGAMTestGAM::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(HistogramGAMTestGAM, "1.0")

class HistogramGAMTestDS: public MemoryDataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    HistogramGAMTestDS();

    virtual ~HistogramGAMTestDS();

    const char8 *GetBrokerName(StructuredDataI &data,const SignalDirection direction);

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    virtual bool Synchronise();

private:
    bool firstTime;
};

HistogramGAMTestDS::HistogramGAMTestDS() {
    firstTime = true;
}

HistogramGAMTestDS::~HistogramGAMTestDS() {

}

const char8 *HistogramGAMTestDS::GetBrokerName(StructuredDataI &data,
                                               const SignalDirection direction) {

    const char8* brokerName = NULL_PTR(const char8 *);

    if (direction == InputSignals) {

        float32 frequency = 0.F;
        if (data.Read("Frequency", frequency)) {
            if (frequency > 0.) {
                brokerName = "MemoryMapSynchronisedInputBroker";
            }
        }
        uint32 trigger = 0u;
        if (data.Read("Trigger", trigger)) {
            if (trigger > 0u) {
                brokerName = "MemoryMapSynchronisedInputBroker";
            }
        }
        if (brokerName == NULL_PTR(const char8 *)) {
            brokerName = "MemoryMapInputBroker";
        }
    }

    if (direction == OutputSignals) {

        float32 frequency = 0.F;
        if (data.Read("Frequency", frequency)) {
            if (frequency > 0.) {
                brokerName = "MemoryMapSynchronisedOutputBroker";
            }
        }
        uint32 trigger = 0u;
        if (data.Read("Trigger", trigger)) {
            if (trigger > 0u) {
                brokerName = "MemoryMapSynchronisedOutputBroker";
            }
        }
        if (brokerName == NULL_PTR(const char8 *)) {
            brokerName = "MemoryMapOutputBroker";
        }
    }

    return brokerName;
}

bool HistogramGAMTestDS::PrepareNextState(const char8 * const currentStateName,
                                          const char8 * const nextStateName) {

    MemoryOperationsHelper::Set(memory, 0, totalMemorySize);

    return true;
}

bool HistogramGAMTestDS::Synchronise() {
    if (!firstTime) {
        (*(uint8*) (memory)) = (*(uint8*) (memory)) + 1;
        (*(int8*) (memory + 1)) = (*(int8*) (memory + 1)) + 1;
        (*(uint16*) (memory + 2)) = (*(uint16*) (memory + 2)) + 1;
        (*(int16*) (memory + 4)) = (*(int16*) (memory + 4)) + 1;
        (*(uint32*) (memory + 6)) = (*(uint32*) (memory + 6)) + 1;
        (*(int32*) (memory + 10)) = (*(int32*) (memory + 10)) + 1;
        (*(uint64*) (memory + 14)) = (*(uint64*) (memory + 14)) + 1;
        (*(int64*) (memory + 22)) = (*(int64*) (memory + 22)) + 1;
        (*(float32*) (memory + 30)) = (*(float32*) (memory + 30)) + 1.;
        (*(float64*) (memory + 34)) = (*(float64*) (memory + 34)) + 1.;
    }
    else {
        firstTime = false;
    }

    return true;
}

CLASS_REGISTER(HistogramGAMTestDS, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapInputBrokerEnviroment(const char8 * const config) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
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
        application = god->Find("Application");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}

HistogramGAMTest::HistogramGAMTest() {

}

HistogramGAMTest::~HistogramGAMTest() {
}

bool HistogramGAMTest::TestConstructor() {
    HistogramGAMTestGAM test;
    bool ret = test.GetComps() == NULL;
    ret &= test.GetBeginCycle() == 0u;
    ret &= test.GetCycleCounter() == 0u;
    return ret;
}

bool HistogramGAMTest::TestInitialise() {
    ConfigurationDatabase cdb;
    uint32 x = 10u;
    cdb.Write("BeginCycleNumber", x);

    HistogramGAMTestGAM test;

    bool ret = test.Initialise(cdb);

    ret &= test.GetBeginCycle() == 10u;
    ret &= test.GetComps() == NULL;
    ret &= test.GetCycleCounter() == 0u;
    return ret;
}

bool HistogramGAMTest::TestSetup() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HistogramGAMTestGAM"
            "             BeginCycleNumber = 10"
            "             InputSignals = {"
            "                 Source_U8 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint8"
            "                 }"
            "                 Source_I8 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int8"
            "                 }"
            "                 Source_U16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint16"
            "                 }"
            "                 Source_I16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int16"
            "                 }"
            "                 Source_U32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint32"
            "                 }"
            "                 Source_i32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int32"
            "                 }"
            "                 Source_U64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint64"
            "                 }"
            "                 Source_I64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int64"
            "                 }"
            "                 Source_F32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float32"
            "                 }"
            "                 Source_F64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float64"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 Statistics_U8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "             }"
            "        }"
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
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = HistogramGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<HistogramGAMTestGAM> gam;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    if (ret) {
        HistogramComparator **comps = gam->GetComps();
        ret &= dynamic_cast<HistogramComparatorT<uint8>*>(comps[0]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<int8>*>(comps[1]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<uint16>*>(comps[2]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<int16>*>(comps[3]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<uint32>*>(comps[4]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<int32>*>(comps[5]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<uint64>*>(comps[6]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<int64>*>(comps[7]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<float32>*>(comps[8]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<float64>*>(comps[9]) != NULL;
    }
    god->Purge();
    return ret;
}

bool HistogramGAMTest::TestSetup_DifferentInputOutputNSignals() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HistogramGAMTestGAM"
            "             BeginCycleNumber = 10"
            "             InputSignals = {"
            "                 Source_U8 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint8"
            "                 }"
            "                 Source_I8 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int8"
            "                 }"
            "                 Source_U16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint16"
            "                 }"
            "                 Source_I16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int16"
            "                 }"
            "                 Source_U32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint32"
            "                 }"
            "                 Source_i32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int32"
            "                 }"
            "                 Source_U64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint64"
            "                 }"
            "                 Source_I64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int64"
            "                 }"
            "                 Source_F32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float32"
            "                 }"
            "                 Source_F64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float64"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 Statistics_U8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "             }"
            "        }"
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
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = HistogramGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    god->Purge();
    return ret;
}

bool HistogramGAMTest::TestSetup_FalseInputNotScalar() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HistogramGAMTestGAM"
            "             BeginCycleNumber = 10"
            "             InputSignals = {"
            "                 Source_U8 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint8"
            "                 }"
            "                 Source_I8 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int8"
            "                 }"
            "                 Source_U16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint16"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Source_I16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int16"
            "                 }"
            "                 Source_U32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint32"
            "                 }"
            "                 Source_i32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int32"
            "                 }"
            "                 Source_U64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint64"
            "                 }"
            "                 Source_I64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int64"
            "                 }"
            "                 Source_F32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float32"
            "                 }"
            "                 Source_F64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float64"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 Statistics_U8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "             }"
            "        }"
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
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = HistogramGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    god->Purge();
    return ret;
}

bool HistogramGAMTest::TestSetup_FalseSamplesNotOne() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HistogramGAMTestGAM"
            "             BeginCycleNumber = 10"
            "             InputSignals = {"
            "                 Source_U8 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint8"
            "                 }"
            "                 Source_I8 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int8"
            "                     Samples = 2"
            "                 }"
            "                 Source_U16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint16"
            "                 }"
            "                 Source_I16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int16"
            "                 }"
            "                 Source_U32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint32"
            "                 }"
            "                 Source_i32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int32"
            "                 }"
            "                 Source_U64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint64"
            "                 }"
            "                 Source_I64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int64"
            "                 }"
            "                 Source_F32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float32"
            "                 }"
            "                 Source_F64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float64"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 Statistics_U8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "             }"
            "        }"
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
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = HistogramGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    god->Purge();
    return ret;
}

bool HistogramGAMTest::TestSetup_UndefinedMaxLim() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HistogramGAMTestGAM"
            "             BeginCycleNumber = 10"
            "             InputSignals = {"
            "                 Source_U8 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint8"
            "                 }"
            "                 Source_I8 = {"
            "                     DataSource = Input"
            "                     MinLim = 0"
            "                     Type = int8"
            "                 }"
            "                 Source_U16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint16"
            "                 }"
            "                 Source_I16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int16"
            "                 }"
            "                 Source_U32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint32"
            "                 }"
            "                 Source_i32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int32"
            "                 }"
            "                 Source_U64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint64"
            "                 }"
            "                 Source_I64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int64"
            "                 }"
            "                 Source_F32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float32"
            "                 }"
            "                 Source_F64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float64"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 Statistics_U8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "             }"
            "        }"
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
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = HistogramGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    god->Purge();
    return ret;
}

bool HistogramGAMTest::TestSetup_UndefinedMinLim() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HistogramGAMTestGAM"
            "             BeginCycleNumber = 10"
            "             InputSignals = {"
            "                 Source_U8 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint8"
            "                 }"
            "                 Source_I8 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int8"
            "                 }"
            "                 Source_U16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint16"
            "                 }"
            "                 Source_I16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int16"
            "                 }"
            "                 Source_U32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     Type = uint32"
            "                 }"
            "                 Source_i32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int32"
            "                 }"
            "                 Source_U64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint64"
            "                 }"
            "                 Source_I64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int64"
            "                 }"
            "                 Source_F32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float32"
            "                 }"
            "                 Source_F64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float64"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 Statistics_U8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "             }"
            "        }"
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
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = HistogramGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    god->Purge();
    return ret;
}

bool HistogramGAMTest::TestSetup_NBindMinorThan3() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HistogramGAMTestGAM"
            "             BeginCycleNumber = 10"
            "             InputSignals = {"
            "                 Source_U8 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint8"
            "                 }"
            "                 Source_I8 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int8"
            "                 }"
            "                 Source_U16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint16"
            "                 }"
            "                 Source_I16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int16"
            "                 }"
            "                 Source_U32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint32"
            "                 }"
            "                 Source_i32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int32"
            "                 }"
            "                 Source_U64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint64"
            "                 }"
            "                 Source_I64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int64"
            "                 }"
            "                 Source_F32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float32"
            "                 }"
            "                 Source_F64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float64"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 Statistics_U8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 2"
            "                 }"
            "                 Statistics_I64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "             }"
            "        }"
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
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = HistogramGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    god->Purge();
    return ret;
}

bool HistogramGAMTest::TestSetup_OutputTypeNotUint32() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HistogramGAMTestGAM"
            "             BeginCycleNumber = 10"
            "             InputSignals = {"
            "                 Source_U8 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint8"
            "                 }"
            "                 Source_I8 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int8"
            "                 }"
            "                 Source_U16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint16"
            "                 }"
            "                 Source_I16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int16"
            "                 }"
            "                 Source_U32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint32"
            "                 }"
            "                 Source_i32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int32"
            "                 }"
            "                 Source_U64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint64"
            "                 }"
            "                 Source_I64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = int64"
            "                 }"
            "                 Source_F32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float32"
            "                 }"
            "                 Source_F64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = float64"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 Statistics_U8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U16 = {"
            "                     DataSource = DDB1"
            "                     Type = int32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "             }"
            "        }"
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
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = HistogramGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    god->Purge();
    return ret;

}

bool HistogramGAMTest::TestExecute() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HistogramGAMTestGAM"
            "             InputSignals = {"
            "                 Source_U8 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint8"
            "                 }"
            "                 Source_I8 = {"
            "                     DataSource = Input"
            "                     MaxLim = 11"
            "                     MinLim = 1"
            "                     Type = int8"
            "                 }"
            "                 Source_U16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 12"
            "                     MinLim = 2"
            "                     Type = uint16"
            "                 }"
            "                 Source_I16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 13"
            "                     MinLim = 3"
            "                     Type = int16"
            "                 }"
            "                 Source_U32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 14"
            "                     MinLim = 4"
            "                     Type = uint32"
            "                 }"
            "                 Source_i32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 15"
            "                     MinLim = 5"
            "                     Type = int32"
            "                 }"
            "                 Source_U64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 16"
            "                     MinLim = 6"
            "                     Type = uint64"
            "                 }"
            "                 Source_I64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 17"
            "                     MinLim = 7"
            "                     Type = int64"
            "                 }"
            "                 Source_F32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 18"
            "                     MinLim = 8"
            "                     Type = float32"
            "                 }"
            "                 Source_F64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 19"
            "                     MinLim = 9"
            "                     Type = float64"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 Statistics_U8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "             }"
            "        }"
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
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = HistogramGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<HistogramGAMTestGAM> gam;
    ReferenceT < HistogramGAMTestDS > ds;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }
    if (ret) {
        HistogramComparator **comps = gam->GetComps();
        ret &= dynamic_cast<HistogramComparatorT<uint8>*>(comps[0]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<int8>*>(comps[1]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<uint16>*>(comps[2]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<int16>*>(comps[3]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<uint32>*>(comps[4]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<int32>*>(comps[5]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<uint64>*>(comps[6]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<int64>*>(comps[7]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<float32>*>(comps[8]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<float64>*>(comps[9]) != NULL;
    }
    ReferenceT < MemoryMapSynchronisedInputBroker > brokerSync;
    ReferenceT < MemoryMapInputBroker > broker;

    if (ret) {
        ReferenceContainer inputBrokers;
        gam->GetInputBrokers(inputBrokers);
        brokerSync = inputBrokers.Get(0);
        ret = brokerSync.IsValid();
        if (ret) {
            broker = inputBrokers.Get(1);
            ret = broker.IsValid();
        }
    }
    if (ret) {
        ret = ds->PrepareNextState("State1", "State1");
    }

    uint32 *outMemU8 = (uint32*) gam->GetOutputSignalsMemory();
    uint32 *outMemI8 = (outMemU8 + 12);
    uint32 *outMemU16 = (outMemI8 + 12);
    uint32 *outMemI16 = (outMemU16 + 12);
    uint32 *outMemU32 = (outMemI16 + 12);
    uint32 *outMemI32 = (outMemU32 + 12);
    uint32 *outMemU64 = (outMemI32 + 12);
    uint32 *outMemI64 = (outMemU64 + 12);
    uint32 *outMemF32 = (outMemI64 + 12);
    uint32 *outMemF64 = (outMemF32 + 12);
    if (ret) {
        for (uint32 i = 0u; (i < 20u) && (ret); i++) {
            brokerSync->Execute();
            broker->Execute();
            gam->Execute();
            for (uint32 j = 0u; (j < 12u) && (ret); j++) {
                if (i >= 10) {
                    ret &= (outMemU8[11] == (uint32)(i - 9));
                }
                else {
                    if (j > 0 && j <= (i + 1)) {
                        ret &= (outMemU8[j] == (uint32) 1);
                    }
                    else {
                        ret &= (outMemU8[j] == (uint32) 0);
                    }
                }

                if (i >= 11) {
                    ret &= (outMemI8[11] == (uint32)(i - 10));
                }
                else if (i < 1) {
                    ret &= (outMemI8[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i)) {
                        ret &= (outMemI8[j] == (uint32) 1);
                    }
                    else if (j > i) {
                        ret &= (outMemI8[j] == (uint32) 0);
                    }
                }

                if (i >= 12) {
                    ret &= (outMemU16[11] == (uint32)(i - 11));
                }
                else if (i < 2) {
                    ret &= (outMemU16[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 1)) {
                        ret &= (outMemU16[j] == (uint32) 1);
                    }
                    else if (j > (i - 1)) {
                        ret &= (outMemU16[j] == (uint32) 0);
                    }
                }

                if (i >= 13) {
                    ret &= (outMemI16[11] == (uint32)(i - 12));
                }
                else if (i < 3) {
                    ret &= (outMemI16[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 2)) {
                        ret &= (outMemI16[j] == (uint32) 1);
                    }
                    else if (j > (i - 2)) {
                        ret &= (outMemI16[j] == (uint32) 0);
                    }
                }

                if (i >= 14) {
                    ret &= (outMemU32[11] == (uint32)(i - 13));
                }
                else if (i < 4) {
                    ret &= (outMemU32[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 3)) {
                        ret &= (outMemU32[j] == (uint32) 1);
                    }
                    else if (j > (i - 3)) {
                        ret &= (outMemU32[j] == (uint32) 0);
                    }
                }

                if (i >= 15) {
                    ret &= (outMemI32[11] == (uint32)(i - 14));
                }
                else if (i < 5) {
                    ret &= (outMemI32[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 4)) {
                        ret &= (outMemI32[j] == (uint32) 1);
                    }
                    else if (j > (i - 4)) {
                        ret &= (outMemI32[j] == (uint32) 0);
                    }
                }

                if (i >= 16) {
                    ret &= (outMemU64[11] == (uint32)(i - 15));
                }
                else if (i < 6) {
                    ret &= (outMemU64[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 5)) {
                        ret &= (outMemU64[j] == (uint32) 1);
                    }
                    else if (j > (i - 5)) {
                        ret &= (outMemU64[j] == (uint32) 0);
                    }
                }

                if (i >= 17) {
                    ret &= (outMemI64[11] == (uint32)(i - 16));
                }
                else if (i < 7) {
                    ret &= (outMemI64[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 6)) {
                        ret &= (outMemI64[j] == (uint32) 1);
                    }
                    else if (j > (i - 6)) {
                        ret &= (outMemI64[j] == (uint32) 0);
                    }
                }

                if (i >= 18) {
                    ret &= (outMemF32[11] == (uint32)(i - 17));
                }
                else if (i < 8) {
                    ret &= (outMemF32[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 7)) {
                        ret &= (outMemF32[j] == (uint32) 1);
                    }
                    else if (j > (i - 7)) {
                        ret &= (outMemF32[j] == (uint32) 0);
                    }
                }

                if (i >= 19) {
                    ret &= (outMemF64[11] == (uint32)(i - 18));
                }
                else if (i < 9) {
                    ret &= (outMemF64[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 8)) {
                        ret &= (outMemF64[j] == (uint32) 1);
                    }
                    else if (j > (i - 8)) {
                        ret &= (outMemF64[j] == (uint32) 0);
                    }
                }
            }
        }
    }

    god->Purge();
    return ret;
}
bool HistogramGAMTest::TestExecute_BeginCycleNumber() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HistogramGAMTestGAM"
            "           BeginCycleNumber = 5"
            "             InputSignals = {"
            "                 Source_U8 = {"
            "                     DataSource = Input"
            "                     Frequency = 1"
            "                     MaxLim = 10"
            "                     MinLim = 0"
            "                     Type = uint8"
            "                 }"
            "                 Source_I8 = {"
            "                     DataSource = Input"
            "                     MaxLim = 11"
            "                     MinLim = 1"
            "                     Type = int8"
            "                 }"
            "                 Source_U16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 12"
            "                     MinLim = 2"
            "                     Type = uint16"
            "                 }"
            "                 Source_I16 = {"
            "                     DataSource = Input"
            "                     MaxLim = 13"
            "                     MinLim = 3"
            "                     Type = int16"
            "                 }"
            "                 Source_U32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 14"
            "                     MinLim = 4"
            "                     Type = uint32"
            "                 }"
            "                 Source_i32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 15"
            "                     MinLim = 5"
            "                     Type = int32"
            "                 }"
            "                 Source_U64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 16"
            "                     MinLim = 6"
            "                     Type = uint64"
            "                 }"
            "                 Source_I64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 17"
            "                     MinLim = 7"
            "                     Type = int64"
            "                 }"
            "                 Source_F32 = {"
            "                     DataSource = Input"
            "                     MaxLim = 18"
            "                     MinLim = 8"
            "                     Type = float32"
            "                 }"
            "                 Source_F64 = {"
            "                     DataSource = Input"
            "                     MaxLim = 19"
            "                     MinLim = 9"
            "                     Type = float64"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 Statistics_U8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I8 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I16 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_U64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_I64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F32 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "                 Statistics_F64 = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                     NumberOfDimensions = 1"
            "                     NumberOfElements = 12"
            "                 }"
            "             }"
            "        }"
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
            "        +LoggerDataSource = {"
            "            Class = LoggerDataSource"
            "        }"
            "        +Input = {"
            "            Class = HistogramGAMTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<HistogramGAMTestGAM> gam;
    ReferenceT < HistogramGAMTestDS > ds;
    if (ret) {
        gam = god->Find("Application.Functions.GAM1");
        ret = gam.IsValid();
    }
    if (ret) {
        ds = god->Find("Application.Data.Input");
        ret = ds.IsValid();
    }
    if (ret) {
        HistogramComparator **comps = gam->GetComps();
        ret &= dynamic_cast<HistogramComparatorT<uint8>*>(comps[0]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<int8>*>(comps[1]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<uint16>*>(comps[2]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<int16>*>(comps[3]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<uint32>*>(comps[4]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<int32>*>(comps[5]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<uint64>*>(comps[6]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<int64>*>(comps[7]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<float32>*>(comps[8]) != NULL;
        ret &= dynamic_cast<HistogramComparatorT<float64>*>(comps[9]) != NULL;
    }
    ReferenceT < MemoryMapSynchronisedInputBroker > brokerSync;
    ReferenceT < MemoryMapInputBroker > broker;

    if (ret) {
        ReferenceContainer inputBrokers;
        gam->GetInputBrokers(inputBrokers);
        brokerSync = inputBrokers.Get(0);
        ret = brokerSync.IsValid();
        if (ret) {
            broker = inputBrokers.Get(1);
            ret = broker.IsValid();
        }
    }
    if (ret) {
        ret = ds->PrepareNextState("State1", "State1");
    }

    uint32 *outMemU8 = (uint32*) gam->GetOutputSignalsMemory();
    uint32 *outMemI8 = (outMemU8 + 12);
    uint32 *outMemU16 = (outMemI8 + 12);
    uint32 *outMemI16 = (outMemU16 + 12);
    uint32 *outMemU32 = (outMemI16 + 12);
    uint32 *outMemI32 = (outMemU32 + 12);
    uint32 *outMemU64 = (outMemI32 + 12);
    uint32 *outMemI64 = (outMemU64 + 12);
    uint32 *outMemF32 = (outMemI64 + 12);
    uint32 *outMemF64 = (outMemF32 + 12);
    if (ret) {
        //Force the BeginCycleNumber
        for (uint32 i = 0u; (i < 5u) && (ret); i++) {
            gam->Execute();
        }

        for (uint32 i = 0u; (i < 20u) && (ret); i++) {
            brokerSync->Execute();
            broker->Execute();
            gam->Execute();
            for (uint32 j = 0u; (j < 12u) && (ret); j++) {
                if (i >= 10) {
                    ret &= (outMemU8[11] == (uint32)(i - 9));
                }
                else {
                    if (j > 0 && j <= (i + 1)) {
                        ret &= (outMemU8[j] == (uint32) 1);
                    }
                    else {
                        ret &= (outMemU8[j] == (uint32) 0);
                    }
                }

                if (i >= 11) {
                    ret &= (outMemI8[11] == (uint32)(i - 10));
                }
                else if (i < 1) {
                    ret &= (outMemI8[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i)) {
                        ret &= (outMemI8[j] == (uint32) 1);
                    }
                    else if (j > i) {
                        ret &= (outMemI8[j] == (uint32) 0);
                    }
                }

                if (i >= 12) {
                    ret &= (outMemU16[11] == (uint32)(i - 11));
                }
                else if (i < 2) {
                    ret &= (outMemU16[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 1)) {
                        ret &= (outMemU16[j] == (uint32) 1);
                    }
                    else if (j > (i - 1)) {
                        ret &= (outMemU16[j] == (uint32) 0);
                    }
                }

                if (i >= 13) {
                    ret &= (outMemI16[11] == (uint32)(i - 12));
                }
                else if (i < 3) {
                    ret &= (outMemI16[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 2)) {
                        ret &= (outMemI16[j] == (uint32) 1);
                    }
                    else if (j > (i - 2)) {
                        ret &= (outMemI16[j] == (uint32) 0);
                    }
                }

                if (i >= 14) {
                    ret &= (outMemU32[11] == (uint32)(i - 13));
                }
                else if (i < 4) {
                    ret &= (outMemU32[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 3)) {
                        ret &= (outMemU32[j] == (uint32) 1);
                    }
                    else if (j > (i - 3)) {
                        ret &= (outMemU32[j] == (uint32) 0);
                    }
                }

                if (i >= 15) {
                    ret &= (outMemI32[11] == (uint32)(i - 14));
                }
                else if (i < 5) {
                    ret &= (outMemI32[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 4)) {
                        ret &= (outMemI32[j] == (uint32) 1);
                    }
                    else if (j > (i - 4)) {
                        ret &= (outMemI32[j] == (uint32) 0);
                    }
                }

                if (i >= 16) {
                    ret &= (outMemU64[11] == (uint32)(i - 15));
                }
                else if (i < 6) {
                    ret &= (outMemU64[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 5)) {
                        ret &= (outMemU64[j] == (uint32) 1);
                    }
                    else if (j > (i - 5)) {
                        ret &= (outMemU64[j] == (uint32) 0);
                    }
                }

                if (i >= 17) {
                    ret &= (outMemI64[11] == (uint32)(i - 16));
                }
                else if (i < 7) {
                    ret &= (outMemI64[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 6)) {
                        ret &= (outMemI64[j] == (uint32) 1);
                    }
                    else if (j > (i - 6)) {
                        ret &= (outMemI64[j] == (uint32) 0);
                    }
                }

                if (i >= 18) {
                    ret &= (outMemF32[11] == (uint32)(i - 17));
                }
                else if (i < 8) {
                    ret &= (outMemF32[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 7)) {
                        ret &= (outMemF32[j] == (uint32) 1);
                    }
                    else if (j > (i - 7)) {
                        ret &= (outMemF32[j] == (uint32) 0);
                    }
                }

                if (i >= 19) {
                    ret &= (outMemF64[11] == (uint32)(i - 18));
                }
                else if (i < 9) {
                    ret &= (outMemF64[0] == (uint32)(i + 1));
                }
                else {
                    if (j > 0 && j <= (i - 8)) {
                        ret &= (outMemF64[j] == (uint32) 1);
                    }
                    else if (j > (i - 8)) {
                        ret &= (outMemF64[j] == (uint32) 0);
                    }
                }
            }
        }
    }

    god->Purge();
    return ret;
}

