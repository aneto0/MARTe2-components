/**
 * @file EPICSCAClientTest.cpp
 * @brief Source file for class EPICSCAClientTest
 * @date 04/02/2021
 * @author Andre Neto
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class EPICSCAClientTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "EPICSCAClient.h"
#include "EPICSCAClientTest.h"
#include "EPICSPV.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool EPICSCAClientTest::TestConstructor() {
    using namespace MARTe;
    EPICSCAClient test;
    return (test.Size() == 0u);
}

bool EPICSCAClientTest::TestInitialise_Defaults() {
    using namespace MARTe;

    StreamString config = ""
            "+EPICSCA = {"
            "    Class = EPICSCAClient"
            "    +PV_1 = {"
            "        Class = EPICSPV"
            "        PVName = \"MARTe2::EPICSCA::Test::String\""
            "        PVType = string"
            "    }"
            "    +PV_2 = {"
            "        Class = EPICSPV"
            "        PVName = \"MARTe2::EPICSCA::Test::Int32\""
            "        PVType = int32"
            "    }"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPV> sPV;
    ReferenceT<EPICSPV> iPV;
    ReferenceT<EPICSCAClient> client;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        sPV = ord->Find("EPICSCA.PV_1");
        ok = sPV.IsValid();
    }
    if (ok) {
        iPV = ord->Find("EPICSCA.PV_2");
        ok = iPV.IsValid();
    }
    if (ok) {
        client = ord->Find("EPICSCA");
        ok = client.IsValid();
    }
    if (ok) {
        ok = (client->GetCPUMask() == 0xFF);
    }
    if (ok) {
        ok = (client->GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 4u));
    }
    ord->Purge();
    return ok;
}

bool EPICSCAClientTest::TestInitialise_NoDefaults() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+EPICSCA = {"
            "    Class = EPICSCAClient"
            "    CPUs = 0x1"
            "    StackSize = 327680"
            "    +PV_1 = {"
            "        Class = EPICSPV"
            "        PVName = \"MARTe2::EPICSCA::Test::String\""
            "        PVType = string"
            "    }"
            "    +PV_2 = {"
            "        Class = EPICSPV"
            "        PVName = \"MARTe2::EPICSCA::Test::Int32\""
            "        PVType = int32"
            "    }"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPV> sPV;
    ReferenceT<EPICSPV> iPV;
    ReferenceT<EPICSCAClient> client;

    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        sPV = ord->Find("EPICSCA.PV_1");
        ok = sPV.IsValid();
    }
    if (ok) {
        iPV = ord->Find("EPICSCA.PV_2");
        ok = iPV.IsValid();
    }
    if (ok) {
        client = ord->Find("EPICSCA");
        ok = client.IsValid();
    }
    if (ok) {
        ok = (client->GetCPUMask() == 0x1);
    }
    if (ok) {
        ok = (client->GetStackSize() == 327680);
    }
    ord->Purge();
    return ok;
}

bool EPICSCAClientTest::TestGetCPUMask() {
    return TestInitialise_NoDefaults();
}

bool EPICSCAClientTest::TestGetStackSize() {
    return TestInitialise_NoDefaults();
}

bool EPICSCAClientTest::TestExecute() {
    return TestInitialise_NoDefaults();
}

bool EPICSCAClientTest::TestStart() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+EPICSCA = {"
            "    Class = EPICSCAClient"
            "    CPUs = 0x1"
            "    StackSize = 327680"
            "    AutoStart = 0"
            "    +PV_1 = {"
            "        Class = EPICSPV"
            "        PVName = \"MARTe2::EPICSCA::Test::String\""
            "        PVType = string"
            "    }"
            "    +PV_2 = {"
            "        Class = EPICSPV"
            "        PVName = \"MARTe2::EPICSCA::Test::Int32\""
            "        PVType = int32"
            "    }"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPV> sPV;
    ReferenceT<EPICSPV> iPV;
    ReferenceT<EPICSCAClient> client;

    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        sPV = ord->Find("EPICSCA.PV_1");
        ok = sPV.IsValid();
    }
    if (ok) {
        iPV = ord->Find("EPICSCA.PV_2");
        ok = iPV.IsValid();
    }
    if (ok) {
        client = ord->Find("EPICSCA");
        ok = client.IsValid();
    }
    if (ok) {
        ok = (client->GetCPUMask() == 0x1);
    }
    if (ok) {
        ok = (client->GetStackSize() == 327680);
    }
    if (ok) {
        ok = (client->GetStatus() == EmbeddedThreadI::OffState);
    }
    if (ok) {
        ok = (client->Start() == ErrorManagement::NoError);
    }
    if (ok) {
        ok = false;
        uint32 timeoutCounts = 50;
        uint32 i = 0;
        while ((!ok) && (i < timeoutCounts)) {
            ok = (client->GetStatus() != EmbeddedThreadI::OffState);
            i++;
            if (!ok) {
                Sleep::Sec(0.1);
            }
        }
    }
    ord->Purge();
    return ok;
}

bool EPICSCAClientTest::TestGetStatus() {
    return TestStart();
}
