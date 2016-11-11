/**
 * @file LoggerDataSourceTest.cpp
 * @brief Source file for class LoggerDataSourceTest
 * @date Nov 9, 2016 TODO Verify the value and format of the date
 * @author aneto TODO Verify the name and format of the author
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
#include "LoggerDataSourceTest.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

bool LoggerDataSourceTest::TestConstructor() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = BufferGAM::BufferGAM"
            "            InputSignals = {"
            "                Counter = {"
            "                    DataSource = Timer"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = Timer"
            "                    Type = uint32"
            "                    Frequency = 10"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = BufferGAM"
            "            InputSignals = {"
            "                Counter = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Counter = {"
            "                    DataSource = LoggerDS"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = LoggerDS"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timer = {"
            "            Class = LinuxTimer"
            "        }"
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
            "                    Functions = {GAMA GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    ClassRegistryDatabase *crd = ClassRegistryDatabase::Instance();
    uint32 numberOfItems = crd->GetSize();
    uint32 n;
    for (n = 0u; n < numberOfItems; n++) {
        const ClassRegistryItem *cri = crd->Peek(n);
        uint16 classUid = cri->GetClassProperties()->GetUniqueIdentifier();
        printf("ClassName = %s %u\n", cri->GetClassProperties()->GetName(), classUid);
    }

    ConfigurationDatabase cdb;
    StreamString configStream = config1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->CleanUp();
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
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        application->StartExecution();
    }
    while (1) {
        Sleep::Sec(1.0);
    }
    return true;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

