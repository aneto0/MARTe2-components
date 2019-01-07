/**
 * @file LinkDataSourceTest.cpp
 * @brief Source file for class LinkDataSourceTest
 * @date 23/07/2018
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
 * the class LinkDataSourceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "IOGAM.h"
#include "LinkDataSourceTest.h"
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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class LinkDataSourceTestDS: public LinkDataSource {
public:
    CLASS_REGISTER_DECLARATION()

    LinkDataSourceTestDS();
    virtual ~LinkDataSourceTestDS();
    uint8 GetIsWriter();

    StreamString GetLinkPath();
    ReferenceT<MemoryGate> GetLink();

    uint32 GetTotalMemorySize();
};

LinkDataSourceTestDS::LinkDataSourceTestDS() {

}

LinkDataSourceTestDS::~LinkDataSourceTestDS() {

}

uint8 LinkDataSourceTestDS::GetIsWriter() {
    return isWriter;
}


StreamString LinkDataSourceTestDS::GetLinkPath() {
    return linkPath;
}
ReferenceT<MemoryGate> LinkDataSourceTestDS::GetLink() {
    return link;
}
uint32 LinkDataSourceTestDS::GetTotalMemorySize() {
    return totalMemorySize;
}

CLASS_REGISTER(LinkDataSourceTestDS, "1.0")

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
    ReferenceT<RealTimeApplication> application1;
    if (ok) {
        application1 = god->Find("Application1");
        ok = application1.IsValid();
    }
    if (ok) {
        ok = application1->ConfigureApplication();
    }
    ReferenceT<RealTimeApplication> application2;
    if (ok) {
        application2 = god->Find("Application2");
        ok = application2.IsValid();
    }
    if (ok) {
        ok = application2->ConfigureApplication();
    }
    return ok;
}

LinkDataSourceTest::LinkDataSourceTest() {
}

LinkDataSourceTest::~LinkDataSourceTest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool LinkDataSourceTest::TestConstructor() {
    LinkDataSourceTestDS test;

    bool ret = (test.GetIsWriter() == 0u);
    ret &= (test.GetLinkPath() == "");
    ReferenceT<MemoryGate> ref = test.GetLink();
    ret &= (!ref.IsValid());

    return ret;
}

bool LinkDataSourceTest::TestInitialise() {

    const char8 *config = ""
            "+ExternalComponent1 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "+ExternalComponent2 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "$Application1 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputA = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint32"
            "                     Trigger = 1"
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
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 1"
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
            "}"
            "$Application2 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputA = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint32"
            "                     Trigger = 1"
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
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "            IsWriter = 1"
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

    return ok;
}

bool LinkDataSourceTest::TestInitialise_FalseNoIsWriter() {

    const char8 *config = ""
            "+ExternalComponent1 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "+ExternalComponent2 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "$Application1 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputA = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint32"
            "                     Trigger = 1"
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
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 1"
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
            "}"
            "$Application2 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputA = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint32"
            "                     Trigger = 1"
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
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "            IsWriter = 1"
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
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = !god->Initialise(cdb);
    }

    return ok;
}

bool LinkDataSourceTest::TestInitialise_FalseNoLink() {

    const char8 *config = ""
            "+ExternalComponent1 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "+ExternalComponent2 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "$Application1 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputA = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint32"
            "                     Trigger = 1"
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
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 1"
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
            "}"
            "$Application2 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputA = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint32"
            "                     Trigger = 1"
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
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "            IsWriter = 1"
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
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = !god->Initialise(cdb);
    }

    return ok;
}
bool LinkDataSourceTest::TestInitialise_FalseInvalidLink() {

    const char8 *config = ""
            "+ExternalComponent1 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "+ExternalComponent2 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "$Application1 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputA = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint32"
            "                     Trigger = 1"
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
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 1"
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
            "}"
            "$Application2 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputA = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint32"
            "                     Trigger = 1"
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
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent3"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "            IsWriter = 1"
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
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = !god->Initialise(cdb);
    }

    return ok;
}

bool LinkDataSourceTest::TestAllocateMemory() {

    const char8 *config = ""
            "+ExternalComponent1 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "+ExternalComponent2 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "$Application1 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 InputB = {"
            "                     DataSource = InputGAM1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputA = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint32"
            "                     Trigger = 1"
            "                 }"
            "                 OutputB = {"
            "                     DataSource = DDB1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 1"
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
            "}"
            "$Application2 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 InputB = {"
            "                     DataSource = Timings"
            "                     Type = uint32"
            "                     Alias = GAM1.AbsoluteTimeUsec"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputB = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint64"
            "                     Trigger = 1"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "            IsWriter = 1"
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
    ReferenceT<LinkDataSourceTestDS> ds1;
    ReferenceT<LinkDataSourceTestDS> ds2;

    ReferenceT<LinkDataSourceTestDS> ds3;
    ReferenceT<LinkDataSourceTestDS> ds4;

    if (ret) {
        ds1 = god->Find("Application1.Data.InputGAM1");
        ret = ds1.IsValid();
    }

    if (ret) {
        ds2 = god->Find("Application1.Data.OutputGAM1");
        ret = ds2.IsValid();
    }

    if (ret) {
        ds3 = god->Find("Application2.Data.InputGAM1");
        ret = ds3.IsValid();
    }

    if (ret) {
        ds4 = god->Find("Application2.Data.OutputGAM1");
        ret = ds4.IsValid();
    }
    if (ret) {
        ret = ds1->GetTotalMemorySize() == 8;

    }
    if (ret) {
        ret = ds2->GetTotalMemorySize() == 4;

    }
    if (ret) {
        ret = ds3->GetTotalMemorySize() == 4;

    }
    if (ret) {
        ret = ds4->GetTotalMemorySize() == 8;
    }

    return ret;

}

bool LinkDataSourceTest::TestAllocateMemory_FalseDifferentSizes() {

    const char8 *config = ""
            "+ExternalComponent1 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "+ExternalComponent2 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "$Application1 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "                 InputB = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint16"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputA = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint32"
            "                     Trigger = 1"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 1"
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
            "}"
            "$Application2 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputB = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint64"
            "                     Trigger = 1"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "            IsWriter = 1"
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
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);

    return ret;
}

bool LinkDataSourceTest::TestPrepareNextState() {
    LinkDataSource link;
    return link.PrepareNextState("ciao", "hola");
}

bool LinkDataSourceTest::TestGetBrokerName() {
    LinkDataSource link;
    ConfigurationDatabase cdb;
    StreamString brokerName = link.GetBrokerName(cdb, InputSignals);
    bool ok = (brokerName == "MemoryMapSynchronisedInputBroker");
    brokerName = link.GetBrokerName(cdb, OutputSignals);
    ok &= (brokerName == "MemoryMapSynchronisedOutputBroker");
    return ok;
}

bool LinkDataSourceTest::TestSynchronise() {

    const char8 *config = ""
            "+ExternalComponent1 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "+ExternalComponent2 = {"
            "    Class = MemoryGate"
            "    NumberOfBuffers = 2"
            "}"
            "$Application1 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputA = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint32"
            "                     Trigger = 1"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 1"
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
            "}"
            "$Application2 = {"
            "   Class = RealTimeApplication"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "             Class = IOGAM"
            "             InputSignals = {"
            "                 InputA = {"
            "                     DataSource = InputGAM1"
            "                     Frequency = 1"
            "                     Type = uint32"
            "                 }"
            "             }"
            "             OutputSignals = {"
            "                 OutputB = {"
            "                     DataSource = OutputGAM1"
            "                     Type = uint32"
            "                     Trigger = 1"
            "                 }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +InputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent1"
            "            IsWriter = 0"
            "        }"
            "        +OutputGAM1 = {"
            "            Class = LinkDataSourceTestDS"
            "            Link = ExternalComponent2"
            "            IsWriter = 1"
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
    ReferenceT<LinkDataSourceTestDS> ds1;
    ReferenceT<LinkDataSourceTestDS> ds2;

    ReferenceT<LinkDataSourceTestDS> ds3;
    ReferenceT<LinkDataSourceTestDS> ds4;

    if (ret) {
        ds1 = god->Find("Application1.Data.InputGAM1");
        ret = ds1.IsValid();
    }

    if (ret) {
        ds2 = god->Find("Application1.Data.OutputGAM1");
        ret = ds2.IsValid();
    }

    if (ret) {
        ds3 = god->Find("Application2.Data.InputGAM1");
        ret = ds3.IsValid();
    }

    if (ret) {
        ds4 = god->Find("Application2.Data.OutputGAM1");
        ret = ds4.IsValid();
    }

    uint32 *data1 = NULL;
    if (ret) {
        ret = ds1->GetSignalMemoryBuffer(0, 0, (void*&) data1);
    }

    uint32 *data2 = NULL;
    if (ret) {
        ret = ds2->GetSignalMemoryBuffer(0, 0, (void*&) data2);
    }

    uint32 *data3 = NULL;
    if (ret) {
        ret = ds3->GetSignalMemoryBuffer(0, 0, (void*&) data3);
    }

    uint32 *data4 = NULL;
    if (ret) {
        ret = ds4->GetSignalMemoryBuffer(0, 0, (void*&) data4);
    }

    if (ret) {
        *data2 = 1;
        *data4 = 2;

        ret &= ds2->Synchronise();
        ret &= ds4->Synchronise();

    }

    if(ret){
        ret &= ds1->Synchronise();
        ret &= ds3->Synchronise();

        ret&=(*data1==2);
        ret&=(*data3==1);

    }

    return ret;
}

