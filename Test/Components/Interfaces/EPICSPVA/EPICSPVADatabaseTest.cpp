/**
 * @file EPICSPVADatabaseTest.cpp
 * @brief Source file for class EPICSPVADatabaseTest
 * @date 25/09/2018
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
 * the class EPICSPVADatabaseTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "EPICSRPCClient.h"
#include "EPICSPVADatabase.h"
#include "EPICSPVADatabaseTest.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool EPICSPVADatabaseTest::TestConstructor() {
    using namespace MARTe;
    EPICSPVADatabase pvaDatabase;
    return (pvaDatabase.GetStatus() == EmbeddedThreadI::OffState);
}

bool EPICSPVADatabaseTest::TestInitialise_Defaults() {
    using namespace MARTe;

    StreamString config = ""
            "+EPICSPVADatabase1 = {"
            "    Class = EPICSPVADatabase"
            "    +Record1 = {"
            "        Class = EPICSPVA::EPICSPVARecord"
            "        Structure = {"
            "            ElementsA = {"
            "                Element1 = {"
            "                   Type = uint32"
            "                   NumberOfElements = 10"
            "               }"
            "               Element2 = {"
            "                   Type = float32"
            "               }"
            "               ElementsB = {"
            "                   ElementB1 = {"
            "                       Type = uint8"
            "                       NumberOfElements = 100"
            "                   }"
            "                   ElementB2 = {"
            "                       Type = float32"
            "                       NumberOfElements = 5"
            "                   }"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Record2 = {"
            "        Class = EPICSPVA::EPICSPVARecord"
            "        Structure = {"
            "            Element1 = {"
            "                Type = float64"
            "                NumberOfElements = 1"
            "            }"
            "       }"
            "    }"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVADatabase> pvDatabase;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        pvDatabase = ord->Find("EPICSPVADatabase1");
        ok = pvDatabase.IsValid();
    }
    if (ok) {
        ok = (pvDatabase->GetCPUMask() == 0xFF);
    }
    if (ok) {
        ok = (pvDatabase->GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 4u));
    }
    if (ok) {
        uint32 timeout = 50;
        while (!pvDatabase->GetServerContext()) {
            Sleep::Sec(0.1);
            timeout--;
            if (timeout == 0) {
                break;
            }
        }
        ok = (pvDatabase->GetServerContext());
    }
    ord->Purge();
    return ok;
}

bool EPICSPVADatabaseTest::TestInitialise_NoDefaults() {
    using namespace MARTe;

    StreamString config = ""
            "+EPICSPVADatabase1 = {"
            "    Class = EPICSPVADatabase"
            "    CPUs = 0x1"
            "    StackSize = 2000000"
            "    AutoStart = 0"
            "    +Record1 = {"
            "        Class = EPICSPVA::EPICSPVARecord"
            "        Structure = {"
            "            ElementsA = {"
            "                Element1 = {"
            "                   Type = uint32"
            "                   NumberOfElements = 10"
            "               }"
            "               Element2 = {"
            "                   Type = float32"
            "               }"
            "               ElementsB = {"
            "                   ElementB1 = {"
            "                       Type = uint8"
            "                       NumberOfElements = 100"
            "                   }"
            "                   ElementB2 = {"
            "                       Type = float32"
            "                       NumberOfElements = 5"
            "                   }"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Record2 = {"
            "        Class = EPICSPVA::EPICSPVARecord"
            "        Structure = {"
            "            Element1 = {"
            "                Type = float64"
            "                NumberOfElements = 1"
            "            }"
            "       }"
            "    }"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVADatabase> pvDatabase;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        pvDatabase = ord->Find("EPICSPVADatabase1");
        ok = pvDatabase.IsValid();
    }
    if (ok) {
        ok = (pvDatabase->GetCPUMask() == 0x1);
    }
    if (ok) {
        ok = (pvDatabase->GetStackSize() == (2000000));
    }
    if (ok) {
        ok = (pvDatabase->GetStatus() == EmbeddedThreadI::OffState);
    }
    if (ok) {
        ok = (!pvDatabase->GetServerContext());
    }
    ord->Purge();
    return ok;
}

bool EPICSPVADatabaseTest::TestExecute() {
    using namespace MARTe;

    StreamString config = ""
            "+EPICSPVADatabase1 = {"
            "    Class = EPICSPVADatabase"
            "    +Record1 = {"
            "        Class = EPICSPVA::EPICSPVARecord"
            "        Structure = {"
            "            ElementsA = {"
            "                Element1 = {"
            "                   Type = uint32"
            "                   NumberOfElements = 10"
            "               }"
            "               Element2 = {"
            "                   Type = float32"
            "               }"
            "               ElementsB = {"
            "                   ElementB1 = {"
            "                       Type = uint8"
            "                       NumberOfElements = 100"
            "                   }"
            "                   ElementB2 = {"
            "                       Type = float32"
            "                       NumberOfElements = 5"
            "                   }"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Record2 = {"
            "        Class = EPICSPVA::EPICSPVARecord"
            "        Structure = {"
            "            Element1 = {"
            "                Type = float64"
            "                NumberOfElements = 1"
            "            }"
            "       }"
            "    }"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVADatabase> pvDatabase;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        pvDatabase = ord->Find("EPICSPVADatabase1");
        ok = pvDatabase.IsValid();
    }
    if (ok) {
        ok = (pvDatabase->GetCPUMask() == 0xFF);
    }
    if (ok) {
        ok = (pvDatabase->GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 4u));
    }
    if (ok) {
        uint32 timeout = 50;
        while (!pvDatabase->GetServerContext()) {
            Sleep::Sec(0.1);
            timeout--;
            if (timeout == 0) {
                break;
            }
        }
        ok = (pvDatabase->GetServerContext());
    }
    ord->Purge();

    //TODO FINISH
    return false;
}

bool EPICSPVADatabaseTest::TestStart() {
    return TestInitialise_NoDefaults();
}

bool EPICSPVADatabaseTest::TestGetStackSize() {
    return TestInitialise_NoDefaults();
}

bool EPICSPVADatabaseTest::TestGetCPUMask() {
    return TestInitialise_NoDefaults();
}

bool EPICSPVADatabaseTest::TestGetStatus() {
    return TestInitialise_NoDefaults();
}

bool EPICSPVADatabaseTest::TestGetServerContext() {
    return TestInitialise_NoDefaults();
}
