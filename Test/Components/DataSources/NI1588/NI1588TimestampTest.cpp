/**
 * @file NI1588TimestampTest.cpp
 * @brief Source file for class NI1588TimestampTest
 * @date 15/05/2018
 * @author Giuseppe Ferrò
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
 * the class NI1588TimestampTest (public, protected, and private). Be aware that some
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
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "NI1588TimestampTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class NI1588TimestampTestDS: public NI1588Timestamp {
public:
    CLASS_REGISTER_DECLARATION()

int32    GetPollTimeout();

    int32 GetDevNum();
    uint8 *GetCapturedEventType();
    uint32 *GetSignalIndexes();
    uint64 *GetSignalTs();

};

int32 NI1588TimestampTestDS::GetPollTimeout() {
    return pollTimeout;
}

int32 NI1588TimestampTestDS::GetDevNum() {
    return devNum;
}

uint8 *NI1588TimestampTestDS::GetCapturedEventType() {
    return capturedEventType;
}
uint32 *NI1588TimestampTestDS::GetSignalIndexes() {
    return signalIndexes;
}

uint64 *NI1588TimestampTestDS::GetSignalTs() {
    return signalTs;
}

CLASS_REGISTER(NI1588TimestampTestDS, "1.0")

class NI1588TimestampTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

NI1588TimestampTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();
    void *GetInputMemoryBuffer();

    void *GetOutputMemoryBuffer();
private:
    uint32 totalSize;
};

NI1588TimestampTestGAM1::NI1588TimestampTestGAM1() :
        GAM() {
    totalSize = 0u;
}

bool NI1588TimestampTestGAM1::Setup() {

    return true;
}

bool NI1588TimestampTestGAM1::Execute() {
    return true;
}

void *NI1588TimestampTestGAM1::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

void *NI1588TimestampTestGAM1::GetOutputMemoryBuffer() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(NI1588TimestampTestGAM1, "1.0")

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
        application = god->Find("Application1");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}

NI1588TimestampTest::NI1588TimestampTest() {
    // Auto-generated constructor stub for NI1588TimestampTest
    // TODO Verify if manual additions are needed
}

NI1588TimestampTest::~NI1588TimestampTest() {
    // Auto-generated destructor stub for NI1588TimestampTest
    // TODO Verify if manual additions are needed
}

bool NI1588TimestampTest::TestConstructor() {

    NI1588TimestampTestDS dataSource;

    bool ret = dataSource.GetPollTimeout() == -1;
    uint8* eventType = dataSource.GetCapturedEventType();

    uint32 *signalIndexes = dataSource.GetSignalIndexes();
    uint64 *ts = dataSource.GetSignalTs();
    for (uint32 i = 0u; i < 6u; i++) {
        ret &= signalIndexes[i] == 0xFFFFFFFFu;
        if (i < 3u) {
            ret &= (eventType[i] == 0u);
            ret &= (ts[i] == 0ull);
        }
    }
    ret &= dataSource.GetDevNum() == -1;

    return ret;
}

bool NI1588TimestampTest::TestInitialise() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "            InputSignals = {"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI1 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI2 = {"
            "                   Type = uint64"
            "               }"
            "            }"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }

    ReferenceT<NI1588TimestampTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = dataSource->GetPollTimeout() == 1000;
    }

    return ret;
}

bool NI1588TimestampTest::TestInitialise_DefaultPollTimeout() {
    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "            TerminalPFI0 = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI1 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI2 = {"
            "                   Type = uint64"
            "               }"
            "            }"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }

    ReferenceT<NI1588TimestampTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = dataSource->GetPollTimeout() == -1;
    }

    return ret;

}

bool NI1588TimestampTest::TestSetConfiguredDatabase() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "               }"
            "            }"
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
    ReferenceT<NI1588TimestampTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = dataSource->GetNumberOfSignals() == 1;
    }
    if (ret) {
        uint32 *signalIndexes = dataSource->GetSignalIndexes();

        for (uint32 i = 1u; (i < 6u) && (ret); i++) {
            ret = signalIndexes[i] == 0xFFFFFFFFu;
        }

        if (ret) {
            ret = signalIndexes[0] == 0;
        }
    }

    return ret;
}

bool NI1588TimestampTest::TestSetConfiguredDatabase_AllSignals() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               EventPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "               TerminalPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               EventPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
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
    ReferenceT<NI1588TimestampTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = dataSource->GetNumberOfSignals() == 8;
    }
    if (ret) {
        uint32 *signalIndexes = dataSource->GetSignalIndexes();
        if (ret) {
            ret &= signalIndexes[0] == 4;
            ret &= signalIndexes[1] == 2;
            ret &= signalIndexes[2] == 5;
            ret &= signalIndexes[3] == 6;
            ret &= signalIndexes[4] == 0;
            ret &= signalIndexes[5] == 7;
        }
    }

    return ret;
}

bool NI1588TimestampTest::TestSetConfiguredDatabase_False_InvalidTcnTimestamp_PFI0_Type() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "               TerminalPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               EventPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
            "            Signals = {"
            "               TerminalPFI1 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI2 = {"
            "                   Type = uint64"
            "               }"
            "            }"
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
    return !InitialiseMemoryMapInputBrokerEnviroment(config);
}

bool NI1588TimestampTest::TestSetConfiguredDatabase_False_InvalidTcnTimestamp_PFI1_Type() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "               TerminalPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               EventPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI2 = {"
            "                   Type = uint64"
            "               }"
            "            }"
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
    return !InitialiseMemoryMapInputBrokerEnviroment(config);
}

bool NI1588TimestampTest::TestSetConfiguredDatabase_False_InvalidTcnTimestamp_PFI2_Type() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "               TerminalPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 1"
            "               }"
            "               EventPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI1 = {"
            "                   Type = uint64"
            "               }"
            "            }"
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
    return !InitialiseMemoryMapInputBrokerEnviroment(config);
}

bool NI1588TimestampTest::TestSetConfiguredDatabase_False_InvalidEventType_PFI0_Type() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "               TerminalPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               EventPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   Samples = 1"
            "               }"
            "               EventPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI1 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI2 = {"
            "                   Type = uint64"
            "               }"
            "            }"
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
    return !InitialiseMemoryMapInputBrokerEnviroment(config);
}

bool NI1588TimestampTest::TestSetConfiguredDatabase_False_InvalidEventType_PFI1_Type() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "               TerminalPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               EventPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   Samples = 1"
            "               }"
            "               EventPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI1 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI2 = {"
            "                   Type = uint64"
            "               }"
            "            }"
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
    return !InitialiseMemoryMapInputBrokerEnviroment(config);
}

bool NI1588TimestampTest::TestSetConfiguredDatabase_False_InvalidEventType_PFI2_Type() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "               TerminalPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               EventPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI1 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI2 = {"
            "                   Type = uint64"
            "               }"
            "            }"
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
    return !InitialiseMemoryMapInputBrokerEnviroment(config);
}

bool NI1588TimestampTest::TestSetConfiguredDatabase_False_InvalidTcnTimestamp_Nelements() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfElements = 2"
            "                   NumberOfDimensions = 1"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "               TerminalPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               EventPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI1 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI2 = {"
            "                   Type = uint64"
            "               }"
            "            }"
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

    return !InitialiseMemoryMapInputBrokerEnviroment(config);

}

bool NI1588TimestampTest::TestSetConfiguredDatabase_False_InvalidEventType_Nelements() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "               TerminalPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               EventPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfElements = 2"
            "                   NumberOfDimensions = 1"
            "                   Samples = 1"
            "               }"
            "               EventPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI1 = {"
            "                   Type = uint64"
            "               }"
            "               TerminalPFI2 = {"
            "                   Type = uint64"
            "               }"
            "            }"
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
    return !InitialiseMemoryMapInputBrokerEnviroment(config);

}

bool NI1588TimestampTest::TestSetConfiguredDatabase_False_InvalidSignal() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "               SignalBoh = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
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
    return !InitialiseMemoryMapInputBrokerEnviroment(config);
}

bool NI1588TimestampTest::TestSetConfiguredDatabase_False_CaptureEvent() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
            "            Signals = {"
            "                TerminalPFI0 = {"
            "                   Type = uint64"
            "                   CaptureEvent = INVALID"
            "                }"
            "            }"
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
    return !InitialiseMemoryMapInputBrokerEnviroment(config);
}

bool NI1588TimestampTest::TestDriverRead() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = 1000"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "               }"
            "            }"
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
    ReferenceT<NI1588TimestampTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = dataSource->GetNumberOfSignals() == 1;
    }
    if (ret) {
        uint32 *signalIndexes = dataSource->GetSignalIndexes();

        for (uint32 i = 1u; (i < 3u) && (ret); i++) {
            ret = signalIndexes[i] == 0xFFFFFFFFu;
        }

        if (ret) {
            ret = signalIndexes[0] == 0;
        }
    }
    //find the gam
    ReferenceT<NI1588TimestampTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> brokerSync;
    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
        if (ret) {
            brokerSync = inputBrokers.Get(0);
            ret = (brokerSync.IsValid());
        }
    }

    uint64* mem = NULL;
    if (ret) {
        mem = (uint64 *) gam->GetInputMemoryBuffer();
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    uint32 nReads = 10;
    uint64 store;
    int64 tol = 4000;
    const uint64 expected = 250000000;
    for (uint32 i = 0u; i < nReads && (ret); i++) {

        brokerSync->Execute();
        uint64 delta = *mem - store;
        if (i > 0) {
            ret = (((int64) (delta - expected)) < tol) || (((int64) (delta - expected)) > -tol);
        }
        store = *mem;
    }

    return ret;
}

bool NI1588TimestampTest::TestDriverRead_AllSignals(uint64 expected, int64 tol, uint8 expectedTerminal) {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 6"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "               TerminalPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               EventPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "               EventPFI2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = -1"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "                   CaptureEvent = NISYNC_EDGE_FALLING"
            "               }"
            "               TerminalPFI1 = {"
            "                   Type = uint64"
            "                   CaptureEvent = NISYNC_EDGE_RISING"
            "               }"
            "               TerminalPFI2 = {"
            "                   Type = uint64"
            "                   CaptureEvent = NISYNC_EDGE_FALLING"
            "               }"
            "            }"
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
    ReferenceT<NI1588TimestampTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = dataSource->GetNumberOfSignals() == 8;
    }

    //find the gam
    ReferenceT<NI1588TimestampTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> brokerSync;
    ReferenceT<MemoryMapMultiBufferInputBroker> broker;
    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);

        if (ret) {
            brokerSync = inputBrokers.Get(0);
            ret = (brokerSync.IsValid());
        }
        if (ret) {
            broker = inputBrokers.Get(1);
            ret = (broker.IsValid());
        }
    }

    uint8* mem = NULL;
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
        mem = (uint8 *) gam->GetInputMemoryBuffer();
    }
    uint32 nReads = 10;
    uint64 store, storeIn;

    for (uint32 i = 0u; i < nReads && (ret); i++) {

        brokerSync->Execute();
        broker->Execute();
        uint8 *omem = mem;
        uint64 delta = *((uint64 *) (mem + 72)) - store;

        for (uint32 j = 0u; j < 6u; j++) {
            omem += 8;
        }
        for (uint32 j = 0u; j < 6u; j++) {
            uint32 errorCheck = *(uint32*) omem;
            ret &= errorCheck == 0;
            omem += 4;
        }
        if (i > 0) {
            ret &= (((int64) (delta - expected)) < tol) || (((int64) (delta - expected)) > -tol);
            uint8 *event = (omem + 24);
            ret &= (*event == 1);
        }
        store = *((uint64*) (mem + 72));
        storeIn = *((uint64*) mem);
    }

    return ret;
}

bool NI1588TimestampTest::TestDriverRead_MoreSamples(uint64 expected, int64 tol, uint8 expectedTerminal) {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = -1"
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
    ReferenceT<NI1588TimestampTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    //find the gam
    ReferenceT<NI1588TimestampTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> brokerSync;
    ReferenceT<MemoryMapMultiBufferInputBroker> broker;
    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);

        if (ret) {
            brokerSync = inputBrokers.Get(0);
            ret = (brokerSync.IsValid());
        }
        if (ret) {
            broker = inputBrokers.Get(1);
            ret = (broker.IsValid());
        }
    }

    uint8* mem = NULL;
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
        mem = (uint8 *) gam->GetInputMemoryBuffer();
    }
    const uint32 nReads = 3;

    uint64 store, storeIn;
    for (uint32 i = 0u; i < nReads && (ret); i++) {

        brokerSync->Execute();
        broker->Execute();
        uint8 *omem = mem;

        omem += 8;
        uint32 errorCheck = *(uint32*) omem;
        ret &= errorCheck == 0;
        omem += 4;
        for (uint32 j = 0u; j < 5u; j++) {
            uint64 delta = *((uint64 *) (omem)) - store;
            if (i > 0) {
                ret &= (((int64) (delta - expected)) < tol) || (((int64) (delta - expected)) > -tol);
            }
            store = *((uint64*) (omem));
            omem += 8;
        }

        storeIn = *((uint64*) mem);
    }

    return ret;
}

bool NI1588TimestampTest::TestDriverRead_NoSync(uint32 expected, int32 tol) {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI1588TimestampTestGAM1"
            "             InputSignals = {"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "                   Samples = 1"
            "               }"
            "               TerminalPFI0 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI1588TimestampTestDS"
            "            NumberOfBuffers = 10"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
            "            PollMsecTimeout = -1"
            "            Signals = {"
            "               TerminalPFI0 = {"
            "                   Type = uint64"
            "                   CaptureEvent = NISYNC_EDGE_RISING"
            "               }"
            "               TerminalPFI1 = {"
            "                   Type = uint64"
            "                   CaptureEvent = NISYNC_EDGE_RISING"
            "               }"
            "               TerminalPFI2 = {"
            "                   Type = uint64"
            "                   CaptureEvent = NISYNC_EDGE_FALLING"
            "               }"
            "            }"
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
    ReferenceT<NI1588TimestampTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    //find the gam
    ReferenceT<NI1588TimestampTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    ReferenceT<MemoryMapMultiBufferInputBroker> broker;
    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);

        if (ret) {
            broker = inputBrokers.Get(0);
            ret = (broker.IsValid());
        }
    }

    uint8* mem = NULL;
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
        mem = (uint8 *) gam->GetInputMemoryBuffer();
    }
    //uint32 nReads = 50;
    uint64 store, storeIn;

    uint64 tic = HighResolutionTimer::Counter();

    uint32 cnt = 0;
    while (cnt < 10u) {

        broker->Execute();
        uint8 *omem = mem;
        uint64 delta = *((uint64 *) (mem + 36)) - store;

        for (uint32 j = 0u; j < 3u; j++) {
            omem += 8;
        }

        omem++;
        omem++;
        for (uint32 j = 0u; j < 3u; j++) {
            omem += 4;
        }
        if (delta > 0) {
            if (cnt > 1) {
                uint64 elapsed = HighResolutionTimer::Counter() - tic;
                tic = HighResolutionTimer::Counter();
                uint32 test = elapsed * HighResolutionTimer::Period() * 1e6;
                ret &= ((((int32) (test - expected)) < tol) || (((int32) (test - expected)) > -tol));
            }
            cnt++;
        }
        store = *((uint64*) (mem + 36));
        storeIn = *((uint64*) mem);
        Sleep::MSec(1);
    }

    return ret;
}

bool NI1588TimestampTest::TestDriverRead_TimeoutExpired(const char8* config, uint64 expected, int64 tol, uint8 expectedTerminal, uint32 nErrorSamples) {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<NI1588TimestampTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    //find the gam
    ReferenceT<NI1588TimestampTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> brokerSync;
    ReferenceT<MemoryMapMultiBufferInputBroker> broker;
    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);

        if (ret) {
            brokerSync = inputBrokers.Get(0);
            ret = (brokerSync.IsValid());
        }
        if (ret) {
            broker = inputBrokers.Get(1);
            ret = (broker.IsValid());
        }
    }

    uint8* mem = NULL;
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
        mem = (uint8 *) gam->GetInputMemoryBuffer();
    }
    uint32 nReads = 10;
    uint64 store, storeIn;
    for (uint32 i = 0u; i < nReads && (ret); i++) {

        brokerSync->Execute();
        broker->Execute();
        uint8 *omem = mem;
        uint64 delta = *((uint64 *) (mem + 28)) - store;

        omem += 8;
        for (uint32 n = 0u; n < nErrorSamples; n++) {
            uint32 errorCheck = *(uint32*) omem;
            omem += 4;
            if ((n < (nErrorSamples - 1))) {
                //timeout expired error
                ret &= errorCheck == 4;
            }
            else {
                ret &= errorCheck == 0;
            }
        }
        if (i > 0) {
            ret &= (((int64) (delta - expected)) < tol) || (((int64) (delta - expected)) > -tol);
        }
        store = *((uint64*) (mem + 28));
        storeIn = *((uint64*) mem);
    }

    return ret;
}

bool NI1588TimestampTest::TestDriverRead_Pattern(const char8* config, uint64 *expectedDeltas, int64 tolerance, uint32 numberOfCycles) {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<NI1588TimestampTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    //find the gam
    ReferenceT<NI1588TimestampTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    ReferenceT<MemoryMapSynchronisedMultiBufferInputBroker> brokerSync;
    //ReferenceT < MemoryMapMultiBufferInputBroker > broker;
    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);

        if (ret) {
            brokerSync = inputBrokers.Get(0);
            ret = (brokerSync.IsValid());
        }
        /*
         if (ret) {
         broker = inputBrokers.Get(1);
         ret = (broker.IsValid());
         }*/
    }

    uint8* mem = NULL;
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
        mem = (uint8 *) gam->GetInputMemoryBuffer();
    }

    uint64 store;
    for (uint32 i = 0u; i < numberOfCycles && (ret); i++) {

        brokerSync->Execute();
        //broker->Execute();
        uint8 *omem = mem;

        if (i > 0) {
            uint64 delta = *(uint64*) (omem) - store;
            ret = (((int64) (delta - expectedDeltas[i])) < tolerance) || ((int64) ((delta - expectedDeltas[i])) > -tolerance);
            if (!ret) {
                printf("Exit 1\n");
            }
        }
        store = *(uint64*) (mem);
    }

    return ret;
}

