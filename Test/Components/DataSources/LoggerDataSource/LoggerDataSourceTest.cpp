/**
 * @file LoggerDataSourceTest.cpp
 * @brief Source file for class LoggerDataSourceTest
 * @date 9/11/2016
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
 * the class LoggerDataSourceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAM.h"
#include "LoggerDataSource.h"
#include "LoggerDataSourceTest.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * Empty GAM to "generate" signals for the LoggerDataSource
 */
class LoggerDataSourceTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

    LoggerDataSourceTestGAM    () : GAM() {
    }

    ~LoggerDataSourceTestGAM() {

    }

    bool Execute() {
        return true;
    }

    bool Setup() {
        return true;
    }

};
CLASS_REGISTER(LoggerDataSourceTestGAM, "1.0")



/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool LoggerDataSourceTest::TestConstructor() {
    using namespace MARTe;
    LoggerDataSource lds;
    return true;
}

bool LoggerDataSourceTest::TestSynchronise() {
    using namespace MARTe;
    LoggerDataSource lds;
    return !lds.Synchronise();
}

bool LoggerDataSourceTest::TestAllocateMemory() {
    using namespace MARTe;
    LoggerDataSource lds;
    return lds.AllocateMemory();
}

bool LoggerDataSourceTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    LoggerDataSource lds;
    return (lds.GetNumberOfMemoryBuffers() == 0u);
}

bool LoggerDataSourceTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    LoggerDataSource lds;
    uint32 signalIdx = 0u;
    uint32 bufferIdx = 0u;
    void *signalAddress = NULL;
    return lds.GetSignalMemoryBuffer(signalIdx, bufferIdx, signalAddress);
}

bool LoggerDataSourceTest::TestGetBrokerName() {
    using namespace MARTe;
    LoggerDataSource lds;
    ConfigurationDatabase cdb;
    return (StringHelper::Compare(lds.GetBrokerName(cdb, OutputSignals), "LoggerBroker") == 0u);
}

bool LoggerDataSourceTest::TestGetInputBrokers() {
    using namespace MARTe;
    LoggerDataSource lds;
    ReferenceContainer rc;
    return !lds.GetInputBrokers(rc, "", NULL);
}

bool LoggerDataSourceTest::TestGetOutputBrokers() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = LoggerDataSourceTestGAM"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = LoggerDS"
            "                    Type = uint32"
            "                }"
            "                Signal2 = {"
            "                    DataSource = LoggerDS"
            "                    Type = uint32"
            "                    NumberOfElements = 5"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +LoggerDS = {"
            "            Class = LoggerDataSource"
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
    StreamString configStream = config1;
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
    god->Purge();
    return ok;
}

bool LoggerDataSourceTest::TestPrepareNextState() {
    using namespace MARTe;
    LoggerDataSource lds;
    return lds.PrepareNextState("", "");
}

bool LoggerDataSourceTest::TestInitialise() {
    using namespace MARTe;
    LoggerDataSource lds;
    ConfigurationDatabase cdb;
    return lds.Initialise(cdb);
}
