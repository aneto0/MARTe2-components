/**
 * @file MARTeIocServerTest.cpp
 * @brief Source file for class MARTeIocServerTest
 * @date 12/07/2019
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
 * the class MARTeIocServerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <epicsStdlib.h>
#include <cadef.h>
#include <epicsGetopt.h>
//#include "tool_lib.h"
#include <string.h>
#include "casCtx.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTeIocServerTest.h"
#include "MARTeCommonPv.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MARTeIocServerTest::MARTeIocServerTest() {
    // Auto-generated constructor stub for MARTeIocServerTest
    // TODO Verify if manual additions are needed
}

MARTeIocServerTest::~MARTeIocServerTest() {
    // Auto-generated destructor stub for MARTeIocServerTest
    // TODO Verify if manual additions are needed
}

bool MARTeIocServerTest::TestInitialise() {

    const char8 *config = ""
            "    Class = MARTeIocServer"
            "    ScanOn = true"
            "    AsyncScan = true"
            "    AsyncDelay = 0.1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"jane\""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "        }"
            "        +albert = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"albert\""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 0.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "            SCAN = 0.5"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    return ok;
}

bool MARTeIocServerTest::TestCreateTimer() {

    const char8 *config = ""
            "    Class = MARTeIocServer"
            "    ScanOn = true"
            "    AsyncScan = true"
            "    AsyncDelay = 0.1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"jane\""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    epicsTimer *timer = NULL;
    if (ok) {
        timer = &ioc.createTimer();
        ok = timer != NULL;
    }
    class MARTeIocServerTestTimerTest: public epicsTimerNotify {
    public:
        MARTeIocServerTestTimerTest(bool testCheck) {
            ok = &testCheck;
            *ok = false;
        }
    private:
        bool *ok;
        expireStatus expire(const epicsTime & currentTime) {
            *ok = true;
            return noRestart;
        }
    };

    MARTeIocServerTestTimerTest timerTest(ok);
    if (ok) {
        timer->start(timerTest, 1.0);
    }
    if (ok) {
        Sleep::Sec(1.1);
    }

    return ok;
}

bool MARTeIocServerTest::TestPvExistTest() {
    const char8 *config = ""
            "    Class = MARTeIocServer"
            "    ScanOn = true"
            "    AsyncScan = true"
            "    AsyncDelay = 0.1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"jane\""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    if (ok) {
        casCtx *ctxIn = new casCtx;
        caNetAddr *client = new caNetAddr;
        pvExistReturn ret = ioc.pvExistTest(*ctxIn, *client, "jane");
        ok = (ret.getStatus() == pverExistsHere);
        if (ok) {
            ret = ioc.pvExistTest(*ctxIn, *client, "albert");
            ok = (ret.getStatus() == pverDoesNotExistHere);
        }
        delete ctxIn;
        delete client;
    }
    return ok;
}

bool MARTeIocServerTest::TestPvAttach() {
    const char8 *config = ""
            "    Class = MARTeIocServer"
            "    ScanOn = true"
            "    AsyncScan = true"
            "    AsyncDelay = 0.1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"jane\""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    if (ok) {
        casCtx *ctxIn = new casCtx;
        pvAttachReturn ret = ioc.pvAttach(*ctxIn, "jane");
        ok = (ret.getStatus() == S_casApp_success);
        if (ok) {
            ReferenceT<MARTePv> pv = ioc.Find("jane");
            if (pv.IsValid()) {
                ok = (ret.getPV() == pv.operator->());
            }
        }
        if (ok) {
            ret = ioc.pvAttach(*ctxIn, "albert");
            ok = (ret.getStatus() == S_casApp_pvNotFound);
        }
        delete ctxIn;
    }
    return ok;
}

bool MARTeIocServerTest::TestExecute() {
    return true;
}

bool MARTeIocServerTest::TestIocAddPv() {
    const char8 *config = ""
            "    Class = MARTeIocServer"
            "    ScanOn = true"
            "    AsyncScan = true"
            "    AsyncDelay = 0.1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"jane\""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    if (ok) {
        ok = (ioc.Size() == 1u);
    }
    if (ok) {
        ReferenceT<MARTeCommonPv> ref(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase cdb;
        cdb.Write("TYPE", "aitEnumFloat64");
        cdb.Write("NELM", 1);
        cdb.Write("PREC", 4);
        cdb.Write("SYNC", "excasIoSync");
        cdb.Write("EGU", "volt");
        cdb.Write("HOPR", 10.0);
        cdb.Write("LOPR", 0.0);
        cdb.Write("HIHI", 9.0);
        cdb.Write("HIGH", 8.0);
        cdb.Write("LOW", 0.5);
        cdb.Write("LOLO", 0.1);
        cdb.Write("HHSV", "MAJOR");
        cdb.Write("HSV", "MINOR");
        cdb.Write("LSV", "MINOR");
        cdb.Write("LLSV", "MAJOR");
        cdb.Write("HYST", "0.0001");
        cdb.Write("ADEL", "0.0001");
        cdb.Write("MDEL", "0.01");
        cdb.Write("SCAN", "0.5");
        cdb.MoveToRoot();
        ref->Initialise(cdb);
        ref->SetName("albert");
        ioc.IocAddPv(ref);
        ok = (ioc.Size() == 2u);
    }
    if (ok) {
        ReferenceT<MARTePv> newPv = ioc.Find("albert");
        ok = newPv.IsValid();
    }

    return ok;
}

bool MARTeIocServerTest::TestIocRemovePv() {

    const char8 *config = ""
            "    Class = MARTeIocServer"
            "    ScanOn = true"
            "    AsyncScan = true"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"jane\""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "        }"
            "        +albert = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"albert\""
            "            TYPE = aitEnumFloat64"
            "            SYNC = excasIoAsync"
            "            SCAN = 0.1"
            "            NELM = 1"
            "            PREC = 4"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "            SCAN = 0.5"
            "        }";
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    if (ok) {
        ok = (ioc.Size() == 2u);
    }
    if (ok) {
        ok = ioc.IocRemovePv("albert");
    }
    if (ok) {
        ok = (ioc.Size() == 1u);
    }
    if (ok) {
        StreamString name = ioc.Get(0)->GetName();
        ok = (name == "jane");
    }
    return ok;
}

bool MARTeIocServerTest::TestGetNumberOfPVs() {

    const char8 *config = ""
            "    Class = MARTeIocServer"
            "    ScanOn = true"
            "    AsyncScan = true"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"jane\""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "        }"
            "        +albert = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"albert\""
            "            TYPE = aitEnumFloat64"
            "            SYNC = excasIoAsync"
            "            SCAN = 0.1"
            "            NELM = 1"
            "            PREC = 4"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "            SCAN = 0.5"
            "        }";
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    if (ok) {
        ok = (ioc.GetNumberOfPVs() == 2u);
    }
    if (ok) {
        ok = ioc.IocRemovePv("albert");
    }
    if (ok) {
        ok = (ioc.GetNumberOfPVs() == 1u);
    }
    if (ok) {
        StreamString name = ioc.Get(0)->GetName();
        ok = (name == "jane");
    }
    if (ok) {
        ReferenceT<MARTeCommonPv> ref(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase cdb;
        cdb.Write("TYPE", "aitEnumFloat64");
        cdb.Write("NELM", 1);
        cdb.Write("PREC", 4);
        cdb.Write("SYNC", "excasIoSync");
        cdb.Write("EGU", "volt");
        cdb.Write("HOPR", 10.0);
        cdb.Write("LOPR", 0.0);
        cdb.Write("HIHI", 9.0);
        cdb.Write("HIGH", 8.0);
        cdb.Write("LOW", 0.5);
        cdb.Write("LOLO", 0.1);
        cdb.Write("HHSV", "MAJOR");
        cdb.Write("HSV", "MINOR");
        cdb.Write("LSV", "MINOR");
        cdb.Write("LLSV", "MAJOR");
        cdb.Write("HYST", "0.0001");
        cdb.Write("ADEL", "0.0001");
        cdb.Write("MDEL", "0.01");
        cdb.Write("SCAN", "0.5");
        cdb.MoveToRoot();
        ref->Initialise(cdb);
        ioc.IocAddPv(ref);
        ok = (ioc.GetNumberOfPVs() == 2u);
    }

    return ok;
}

bool MARTeIocServerTest::TestInteractive() {

    const char8 *config = ""
            "+EPICSLib = {"
            "    Class = MARTeIocServer"
            "    DebugLevel = 0"
            "    ScanOn = true"
            "    AsyncScan = true"
            "    AsyncDelay = 0"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"jane\""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "        }"
            "        +albert = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"albert\""
            "            TYPE = aitEnumFloat64"
            "            NELM = 5"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 0.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "            SCAN = 0.5"
            "        }"
            "        +genoveffa = {"
            "            Class = MARTeCommonPv"
            "            SCAN = 0.1"
            "            NAME = \"genoveffa\""
            "            TYPE = aitEnumString"
            "            SYNC = excasIoSync"
            "            NELM = 1"
            "        }"
            "        +moseppe = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"genoveffa\""
            "            TYPE = aitEnumString"
            "            SYNC = excasIoSync"
            "            NELM = 5"
            "        }"
            "}"
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = LinuxTimerTestGAM"
            "            InputSignals = {"
            "                Counter = {"
            "                    DataSource = Timer"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = Timer"
            "                    Type = uint32"
            "                    Frequency = 1000"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +Timer = {"
            "            Class = LinuxTimer"
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
    ReferenceT<MARTeIocServer> ioc = god->Find("EPICSLib");
    ReferenceT<MARTeCommonPv> info = god->Find("EPICSLib.jane");
    /*
     uint32 x = 4;
     ioc->IocWrite("jane", x);

     float32 y = 0.;
     ioc->IocRead("jane", y);
     printf("Read from Ioc %f\n", y);
     */
#if 0
    if (ok) {
        ok = (ca_context_create(ca_enable_preemptive_callback) == ECA_NORMAL);
    }
    Sleep::Sec(5);

    PvDes pvs[2];
    MemoryOperationsHelper::Set(pvs[0].pvName, 0, PV_NAME_MAX_SIZE);
    MemoryOperationsHelper::Set(pvs[1].pvName, 0, PV_NAME_MAX_SIZE);

    StringHelper::Copy(pvs[0].pvName, "jane");
    StringHelper::Copy(pvs[1].pvName, "albert");
    float64 memFloat = 100;

    if (ok) {
        ok = (ca_create_channel(pvs[0].pvName, NULL, NULL, 20u, &pvs[0].pvChid) == ECA_NORMAL);
        ca_pend_io(0.1);
//        ca_create_channel(pvs[0].pvName, NULL, NULL, 20u, &pvs[0].pvChid);
//        ca_create_channel(pvs[1].pvName, NULL, NULL, 20u, &pvs[1].pvChid);
    }
    if (ok) {

        pvs[0].memory = &memFloat;
        pvs[0].pvType = ca_field_type(pvs[0].pvChid);
        pvs[0].numberOfElements = ca_element_count(pvs[0].pvChid);
        if (ca_state(pvs[0].pvChid) == cs_conn) {
            ok = ca_array_get(pvs[0].pvType, pvs[0].numberOfElements, pvs[0].pvChid, pvs[0].memory);
            ca_pend_io(0.1);
            printf("get value before %f\n", memFloat);
        }
    }

    if (ca_create_subscription(pvs[0].pvType, pvs[0].numberOfElements, pvs[0].pvChid, DBE_VALUE, &GetValueCallback, &pvs[0], &pvs[0].pvEvid) != ECA_NORMAL) {
        ok = false;
    }
    ca_pend_io(0.1);

    //use caget and caput
//    if (ca_array_put((pvs[n].pvType), pvs[n].numberOfElements, pvs[n].pvChid, memory + pvs[n].offset) != ECA_NORMAL) {
//
//    }

    //while (1) {
    Sleep::Sec(30);
    if (ok) {
        ok = ca_array_get(pvs[0].pvType, pvs[0].numberOfElements, pvs[0].pvChid, pvs[0].memory);
        ca_pend_io(0.1);
        printf("get after %f\n", memFloat);
    }
#endif
    uint32 k = 0u;

    uint32 albertVec[] = { 1, 2, 3, 4, 5 };

    Vector < StreamString > stringVec(5);
    while (1) {
        Sleep::Sec(1);

        //ioc->IocWrite("jane", k);
        StreamString s;
        for (uint32 i = 0u; i < 5u; i++) {
            albertVec[i]++;
            stringVec[i] = "";
            stringVec[i].Printf("%d", k);
        }
        ioc->IocWrite("albert", albertVec);
        StreamString y;
        y.Printf("%d", k);
        //ioc->IocWrite("genoveffa", y.Buffer());
        ioc->IocWrite("moseppe", stringVec);

        k++;
    }

    /*
     uint32 input;
     printf("input:\n");
     scanf("%d", &input);
     if (input == 1) {
     printf("Remove jane!\n");
     ioc->RemovePvRuntime("jane");
     }
     if (input == 0) {
     printf("Add jane!\n");
     ioc->AddPvRuntime(info, "jane");

     }*/
    //}
    application->StopCurrentStateExecution();

    return ok;
}

