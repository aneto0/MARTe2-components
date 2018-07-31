/**
 * @file TimeCorrectionGAMTest.cpp
 * @brief Source file for class TimeCorrectionGAMTest
 * @date 07/06/2018
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
 * the class TimeCorrectionGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "MemoryDataSourceI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "TimeCorrectionGAMTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class TimeCorrectionGAMTestGAM: public TimeCorrectionGAM {
public:
    CLASS_REGISTER_DECLARATION()

    uint64 GetExpectedDelta();
    uint64 GetDeltaTolerance();
    float GetFilterGain();
    uint64 GetEstimatedDelta();
    uint64 *GetInputTime();
    uint64 *GetCorrectedTime();
    uint8* GetCorrected();

    uint64 GetLastValidTime();

    void *GetInputSignalsMemory();

    void *GetOutputSignalsMemory();

};

uint64 lastValidTime;
uint64 TimeCorrectionGAMTestGAM::GetExpectedDelta() {
    return expectedDelta;
}

uint64 TimeCorrectionGAMTestGAM::GetDeltaTolerance() {
    return deltaTolerance;
}

float TimeCorrectionGAMTestGAM::GetFilterGain() {
    return filterGain;
}

uint64 TimeCorrectionGAMTestGAM::GetEstimatedDelta() {
    return estimatedDelta;
}

uint64 *TimeCorrectionGAMTestGAM::GetInputTime() {
    return inputTime;
}

uint64 *TimeCorrectionGAMTestGAM::GetCorrectedTime() {
    return correctedTime;
}

uint8* TimeCorrectionGAMTestGAM::GetCorrected() {
    return corrected;
}

uint64 TimeCorrectionGAMTestGAM::GetLastValidTime() {
    return lastValidTime;
}

void *TimeCorrectionGAMTestGAM::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void *TimeCorrectionGAMTestGAM::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(TimeCorrectionGAMTestGAM, "1.0")

class TimeCorrectionGAMTestDS: public MemoryDataSourceI {
public:
    CLASS_REGISTER_DECLARATION()
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    virtual bool Synchronise();

    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);
};
CLASS_REGISTER(TimeCorrectionGAMTestDS, "1.0")

bool TimeCorrectionGAMTestDS::PrepareNextState(const char8 * const currentStateName,
        const char8 * const nextStateName) {
    return true;
}

bool TimeCorrectionGAMTestDS::Synchronise() {
    return true;
}

const char8 *TimeCorrectionGAMTestDS::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    return "MemoryMapInputBroker";
}

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
    ReferenceT < RealTimeApplication > application;
    if (ok) {
        application = god->Find("Application1");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TimeCorrectionGAMTest::TimeCorrectionGAMTest() {
}

TimeCorrectionGAMTest::~TimeCorrectionGAMTest() {
}

bool TimeCorrectionGAMTest::TestConstructor() {
    TimeCorrectionGAMTestGAM gam;
    bool ret = gam.GetExpectedDelta() == 0ull;
    ret &= gam.GetDeltaTolerance() == 0ull;
    ret &= gam.GetFilterGain() == 0.f;
    ret &= gam.GetEstimatedDelta() == 0ull;
    ret &= gam.GetInputTime() == NULL;
    ret &= gam.GetCorrectedTime() == NULL;
    ret &= gam.GetCorrected() == NULL;

    ret &= gam.GetLastValidTime() == 0ull;

    return ret;
}

bool TimeCorrectionGAMTest::TestInitialise() {
    const char8* config = "ExpectedDelta=1000000\n "
            "DeltaTolerance=20\n "
            "FilterGain=0.9\n ";

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();
    if (ret) {
        TimeCorrectionGAMTestGAM gam;
        cdb.MoveToRoot();
        ret = gam.Initialise(cdb);

        if (ret) {
            bool ret = gam.GetExpectedDelta() == 1000000ull;
            ret &= gam.GetDeltaTolerance() == 20ull;
            ret &= gam.GetFilterGain() == 0.9f;
            ret &= gam.GetEstimatedDelta() == 0ull;
            ret &= gam.GetInputTime() == NULL;
            ret &= gam.GetCorrectedTime() == NULL;
            ret &= gam.GetCorrected() == NULL;
        }
    }
    return ret;
}

bool TimeCorrectionGAMTest::TestInitialise_FalseNoExpectedDelta() {
    const char8* config = "DeltaTolerance=20\n "
            "FilterGain=0.9\n ";

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();
    if (ret) {
        TimeCorrectionGAMTestGAM gam;
        cdb.MoveToRoot();
        ret = !gam.Initialise(cdb);
    }
    return ret;
}

bool TimeCorrectionGAMTest::TestInitialise_FalseNoDeltaTolerance() {
    const char8* config = "ExpectedDelta=1000000\n "
            "FilterGain=0.9\n ";

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();
    if (ret) {
        TimeCorrectionGAMTestGAM gam;
        cdb.MoveToRoot();
        ret = !gam.Initialise(cdb);
    }
    return ret;
}

bool TimeCorrectionGAMTest::TestInitialise_FalseNoFilterGain() {
    const char8* config = "ExpectedDelta=1000000\n "
            "DeltaTolerance=20\n ";

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();
    if (ret) {
        TimeCorrectionGAMTestGAM gam;
        cdb.MoveToRoot();
        ret = !gam.Initialise(cdb);
    }
    return ret;
}

bool TimeCorrectionGAMTest::TestSetup() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "               }"
            "               IsCorrected = {"
            "                   DataSource = DDB"
            "                   Type = uint8"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;

}

bool TimeCorrectionGAMTest::TestSetup_OneOutputSignal() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;

}

bool TimeCorrectionGAMTest::TestSetup_FalseNumberOfInputSignals() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                }"
            "                InputTime2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;
}

bool TimeCorrectionGAMTest::TestSetup_FalseNumberOfOutputSignals() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "               }"
            "               IsCorrected = {"
            "                   DataSource = DDB"
            "                   Type = uint8"
            "               }"
            "               IsCorrected2 = {"
            "                   DataSource = DDB"
            "                   Type = uint8"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;
}

bool TimeCorrectionGAMTest::TestSetup_FalseBadInputType() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Frequency = 0"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "               }"
            "               IsCorrected = {"
            "                   DataSource = DDB"
            "                   Type = uint8"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;
}

bool TimeCorrectionGAMTest::TestSetup_FalseBadOutputType1() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint32"
            "               }"
            "               IsCorrected = {"
            "                   DataSource = DDB"
            "                   Type = uint8"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;
}

bool TimeCorrectionGAMTest::TestSetup_FalseBadOutputType2() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "               }"
            "               IsCorrected = {"
            "                   DataSource = DDB"
            "                   Type = uint16"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;
}

bool TimeCorrectionGAMTest::TestSetup_FalseBadInputNElements() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "               }"
            "               IsCorrected = {"
            "                   DataSource = DDB"
            "                   Type = uint8"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;
}

bool TimeCorrectionGAMTest::TestSetup_FalseBadOutputNElements1() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "               IsCorrected = {"
            "                   DataSource = DDB"
            "                   Type = uint8"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;
}

bool TimeCorrectionGAMTest::TestSetup_FalseBadOutputNElements2() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "               }"
            "               IsCorrected = {"
            "                   DataSource = DDB"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;
}

bool TimeCorrectionGAMTest::TestExecute() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "               }"
            "               IsCorrected = {"
            "                   DataSource = DDB"
            "                   Type = uint8"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<TimeCorrectionGAMTestGAM> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    uint64* input = NULL;
    uint64* output = NULL;
    if (ret) {
        input = (uint64*) gam->GetInputSignalsMemory();
        output = (uint64*) gam->GetOutputSignalsMemory();
        ret = (input != NULL) && (output != NULL);

    }

    if (ret) {
        uint64 fakeTime = 0u;
        uint32 nIterations = 1000;
        for (uint32 i = 0u; (i < nIterations) && (ret); i++) {

            *input = fakeTime;
            fakeTime += 1000000;
            gam->Execute();
            ret = (*output == *input);
            if (ret) {
                ret = *(uint8*) (&output[1]) == 0;
            }
        }
    }
    return ret;
}

bool TimeCorrectionGAMTest::TestExecute_OneOutputSignal(){

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<TimeCorrectionGAMTestGAM> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    uint64* input = NULL;
    uint64* output = NULL;
    if (ret) {
        input = (uint64*) gam->GetInputSignalsMemory();
        output = (uint64*) gam->GetOutputSignalsMemory();
        ret = (input != NULL) && (output != NULL);

    }

    if (ret) {
        uint64 fakeTime = 0u;
        uint32 nIterations = 1000;
        for (uint32 i = 0u; (i < nIterations) && (ret); i++) {

            *input = fakeTime;
            fakeTime += 1000000;
            gam->Execute();
            ret = (*output == *input);
        }
    }
    return ret;
}


bool TimeCorrectionGAMTest::TestExecute_EstimationConsecutiveJumps() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.9"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "               }"
            "               IsCorrected = {"
            "                   DataSource = DDB"
            "                   Type = uint8"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<TimeCorrectionGAMTestGAM> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    uint64* input = NULL;
    uint64* output = NULL;
    if (ret) {
        input = (uint64*) gam->GetInputSignalsMemory();
        output = (uint64*) gam->GetOutputSignalsMemory();
        ret = (input != NULL) && (output != NULL);

    }

    if (ret) {
        uint64 fakeTime = 0u;
        uint32 nIterations = 1000;
        uint32 beginJumpIndex = 500;
        uint32 expectedJump = 1000000;

        for (uint32 i = 0u; (i < nIterations) && (ret); i++) {
            if (i > beginJumpIndex) {
                if ((i % 2) == 0) {
                    fakeTime += 2 * expectedJump;
                }
            }
            else {
                fakeTime += expectedJump;
            }
            *input = fakeTime;
            gam->Execute();
            ret = (*output == (i + 1) * expectedJump);

            if (ret) {
                if (i > beginJumpIndex) {
                    if ((i % 2) == 0) {
                        ret = *(uint8*) (&output[1]) == 0;
                    }
                    else {
                        ret = *(uint8*) (&output[1]) == 1;
                    }
                }
                else {
                    ret = *(uint8*) (&output[1]) == 0;
                }
            }
        }
    }
    return ret;
}

bool TimeCorrectionGAMTest::TestExecute_EstimationSlowChange() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = TimeCorrectionGAMTestGAM"
            "           ExpectedDelta=1000000"
            "           DeltaTolerance=20"
            "           FilterGain=0.1"
            "            InputSignals = {"
            "                InputTime = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Frequency = 0"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               CorrectedTime = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "               }"
            "               IsCorrected = {"
            "                   DataSource = DDB"
            "                   Type = uint8"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = TimeCorrectionGAMTestDS"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<TimeCorrectionGAMTestGAM> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    uint64* input = NULL;
    uint64* output = NULL;
    if (ret) {
        input = (uint64*) gam->GetInputSignalsMemory();
        output = (uint64*) gam->GetOutputSignalsMemory();
        ret = (input != NULL) && (output != NULL);

    }

    if (ret) {
        uint64 fakeTime = 0u;
        uint32 nIterations = 1000;
        uint32 expectedJump = 1000000;

        float32 decayGain = 0.999999;

        uint32 goOutIdx = 20;
        for (uint32 i = 0u; (i < nIterations) && (ret); i++) {
            fakeTime += expectedJump;

            *input = fakeTime;
            gam->Execute();
            expectedJump *= decayGain;
            if (ret) {
                if (i >= goOutIdx) {
                    ret = *(uint8*) (&output[1]) == 1;
                }
                else {
                    ret = (*output == *input);
                    ret = *(uint8*) (&output[1]) == 0;
                }
            }
        }
    }
    return ret;
}

