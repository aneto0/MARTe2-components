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
#include <pv/pvaClient.h>
#include <pva/client.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "CLASSREGISTER.h"
#include "EPICSRPCClient.h"
#include "EPICSPVADatabase.h"
#include "EPICSPVADatabaseTest.h"
#include "Introspection.h"
#include "IntrospectionT.h"
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
            "+Types = {"
            "   Class = ReferenceContainer"
            "   +EPICSPVADatabaseTestT1 = {"
            "       Class = IntrospectionStructure"
            "       ElementB1 = {"
            "           Type = uint8"
            "           NumberOfElements = 100"
            "       }"
            "       ElementB2 = {"
            "           Type = float32"
            "           NumberOfElements = 5"
            "       }"
            "   }"
            "   +EPICSPVADatabaseTestT2 = {"
            "       Class = IntrospectionStructure"
            "       Element1 = {"
            "           Type = uint32"
            "           NumberOfElements = 10"
            "       }"
            "       Element2 = {"
            "           Type = float32"
            "       }"
            "       ElementsB = {"
            "           Type = EPICSPVADatabaseTestT1"
            "       }"
            "   }"
            "}"
            "+EPICSPVADatabase1 = {"
            "    Class = EPICSPVADatabase"
            "    +Record1 = {"
            "        Class = EPICSPVA::EPICSPVARecord"
            "        Structure = {"
            "            ElementsA = {"
            "                Type = EPICSPVADatabaseTestT2"
            "                NumberOfElements = 1"
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
        ok = (pvDatabase->GetServerContext() ? true : false);
    }
    ord->Purge();
    return ok;
}

bool EPICSPVADatabaseTest::TestInitialise_NoDefaults() {
    using namespace MARTe;

    StreamString config = ""
            "+Types = {"
            "   Class = ReferenceContainer"
            "   +EPICSPVADatabaseTestT1 = {"
            "       Class = IntrospectionStructure"
            "       ElementB1 = {"
            "           Type = uint8"
            "           NumberOfElements = 100"
            "       }"
            "       ElementB2 = {"
            "           Type = float32"
            "           NumberOfElements = 5"
            "       }"
            "   }"
            "   +EPICSPVADatabaseTestT2 = {"
            "       Class = IntrospectionStructure"
            "       Element1 = {"
            "           Type = uint32"
            "           NumberOfElements = 10"
            "       }"
            "       Element2 = {"
            "           Type = float32"
            "       }"
            "       ElementsB = {"
            "           Type = EPICSPVADatabaseTestT1"
            "       }"
            "   }"
            "}"
            "+EPICSPVADatabase1 = {"
            "    Class = EPICSPVADatabase"
            "    CPUs = 0x1"
            "    StackSize = 2000000"
            "    AutoStart = 0"
            "    +Record1 = {"
            "        Class = EPICSPVA::EPICSPVARecord"
            "        Structure = {"
            "            ElementsA = {"
            "                Type = EPICSPVADatabaseTestT2"
            "                NumberOfElements = 1"
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
            "+Types = {"
            "   Class = ReferenceContainer"
            "   +EPICSPVADatabaseTestT1 = {"
            "       Class = IntrospectionStructure"
            "       ElementB1 = {"
            "           Type = uint8"
            "           NumberOfElements = 100"
            "       }"
            "       ElementB2 = {"
            "           Type = float32"
            "           NumberOfElements = 5"
            "       }"
            "       ElementB3 = {"
            "           Type = uint32"
            "       }"
            "   }"
            "   +EPICSPVADatabaseTestT2 = {"
            "       Class = IntrospectionStructure"
            "       Element1 = {"
            "           Type = uint32"
            "           NumberOfElements = 10"
            "       }"
            "       Element2 = {"
            "           Type = float32"
            "       }"
            "       ElementsB = {"
            "           Type = EPICSPVADatabaseTestT1"
            "       }"
            "   }"
            "}"
            "+EPICSPVADatabase1 = {"
            "    Class = EPICSPVADatabase"
            "    +Record1 = {"
            "        Class = EPICSPVA::EPICSPVARecord"
            "        Structure = {"
            "            value = {"
            "               Type = EPICSPVADatabaseTestT2"
            "               NumberOfElements = 1"
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
        ok = (pvDatabase->GetServerContext() ? true : false);
    }
    pvac::ClientProvider provider("pva");
    {
        pvac::ClientChannel channel(provider.connect("Record2"));
        float32 value = 9;

        channel.put().set("Element1", value).exec();
        epics::pvData::PVStructure::const_shared_pointer getStruct = channel.get();
        std::shared_ptr<const epics::pvData::PVDouble> rvalue = getStruct->getSubField<epics::pvData::PVDouble>("Element1");
        ok = (rvalue ? true : false);
        if (ok) {
            ok = (rvalue->get() == value);
        }
    }
    if (ok) {
        pvac::ClientChannel channel(provider.connect("Record1"));
        uint32 value = 99;

        channel.put().set("value.ElementsB.ElementB3", value).exec();
        epics::pvData::PVStructure::const_shared_pointer getStruct = channel.get();
        std::shared_ptr<const epics::pvData::PVUInt> rvalue = getStruct->getSubField<epics::pvData::PVUInt>("value.ElementsB.ElementB3");
        ok = (rvalue ? true : false);
        if (ok) {
            ok = (rvalue->get() == value);
        }
    }
    ord->Purge();
    return ok;
}

bool EPICSPVADatabaseTest::TestExecute_False_NoRecord() {
    using namespace MARTe;

    StreamString config = ""
            "+EPICSPVADatabase1 = {"
            "    Class = EPICSPVADatabase"
            "    +Record1 = {"
            "        Class = ReferenceContainer"
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
        ok = !ord->Initialise(cdb);
    }
    Sleep::Sec(1.0);
    ord->Purge();
    return ok;
}

//An introspectable structure
struct EPICSPVADatabaseTestS2 {
    MARTe::uint32 ElementB1[8];
    MARTe::float32 ElementB2[10];
    MARTe::uint32 ElementB3;
    MARTe::char8 ElementB4[128];
    MARTe::char8 ElementB5[64];
};
struct EPICSPVADatabaseTestS1 {
    MARTe::uint32 Element1[10];
    MARTe::float32 Element2;
    struct EPICSPVADatabaseTestS2 ElementsB;
};

//The strategy is identical to the class registration
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestS2, ElementB1, uint32, "[8]", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestS2, ElementB2, float32, "[10]", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestS2, ElementB3, uint32, "", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestS2, ElementB4, string, "[128]", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestS2, ElementB5, string, "[64]", "");
static const MARTe::IntrospectionEntry* EPICSPVADatabaseTestS2StructEntries[] = { &EPICSPVADatabaseTestS2_ElementB1_introspectionEntry, &EPICSPVADatabaseTestS2_ElementB2_introspectionEntry,
        &EPICSPVADatabaseTestS2_ElementB3_introspectionEntry, &EPICSPVADatabaseTestS2_ElementB4_introspectionEntry, &EPICSPVADatabaseTestS2_ElementB5_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVADatabaseTestS2, EPICSPVADatabaseTestS2StructEntries)
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestS1, Element1, uint32, "[10]", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestS1, Element2, float32, "", "");
DECLARE_CLASS_MEMBER(EPICSPVADatabaseTestS1, ElementsB, EPICSPVADatabaseTestS2, "", "");
static const MARTe::IntrospectionEntry* EPICSPVADatabaseTestS1StructEntries[] = { &EPICSPVADatabaseTestS1_Element1_introspectionEntry, &EPICSPVADatabaseTestS1_Element2_introspectionEntry,
        &EPICSPVADatabaseTestS1_ElementsB_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(EPICSPVADatabaseTestS1, EPICSPVADatabaseTestS1StructEntries)

bool EPICSPVADatabaseTest::TestExecute_StructuredTypes() {
    using namespace MARTe;

    StreamString config = ""
            "+EPICSPVADatabase1 = {"
            "    Class = EPICSPVADatabase"
            "    +Record1S = {"
            "        Class = EPICSPVA::EPICSPVARecord"
            "        Structure = {"
            "            value = {"
            "               Type = EPICSPVADatabaseTestS1"
            "            }"
            "        }"
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
        ok = (pvDatabase->GetServerContext() ? true : false);
    }
    pvac::ClientProvider provider("pva");
    if (ok) {
        pvac::ClientChannel channel(provider.connect("Record1S"));
        uint32 value = 99;

        channel.put().set("value.ElementsB.ElementB3", value).exec();
        epics::pvData::PVStructure::const_shared_pointer getStruct = channel.get();
        std::shared_ptr<const epics::pvData::PVUInt> rvalue = getStruct->getSubField<epics::pvData::PVUInt>("value.ElementsB.ElementB3");
        ok = (rvalue ? true : false);
        if (ok) {
            ok = (rvalue->get() == value);
        }
    }
    if (ok) {
        pvac::ClientChannel channel(provider.connect("Record1S"));
        const char8 *value = "HELLOSTRINGS";

        channel.put().set("value.ElementsB.ElementB4", value).exec();
        epics::pvData::PVStructure::const_shared_pointer getStruct = channel.get();
        std::shared_ptr<const epics::pvData::PVString> rvalue = getStruct->getSubField<epics::pvData::PVString>("value.ElementsB.ElementB4");
        ok = (rvalue ? true : false);
        if (ok) {
            ok = (rvalue->get() == value);
        }
    }
    if (ok) {
        pvac::ClientChannel channel(provider.connect("Record1S"));
        const char8 *value = "HELLOSTRINGSSTRING";

        channel.put().set("value.ElementsB.ElementB5", value).exec();
        epics::pvData::PVStructure::const_shared_pointer getStruct = channel.get();
        std::shared_ptr<const epics::pvData::PVString> rvalue = getStruct->getSubField<epics::pvData::PVString>("value.ElementsB.ElementB5");
        ok = (rvalue ? true : false);
        if (ok) {
            ok = (rvalue->get() == value);
        }
    }
    ord->Purge();
    return ok;
}

bool EPICSPVADatabaseTest::TestExecute_Array() {
    using namespace MARTe;

    StreamString config = ""
            "+Types = {"
            "   Class = ReferenceContainer"
            "   +EPICSPVADatabaseTestT1 = {"
            "       Class = IntrospectionStructure"
            "       ElementB1 = {"
            "           Type = uint32"
            "           NumberOfElements = 10"
            "       }"
            "       ElementB2 = {"
            "           Type = float32"
            "           NumberOfElements = 10"
            "       }"
            "   }"
            "   +EPICSPVADatabaseTestT2 = {"
            "       Class = IntrospectionStructure"
            "       Element1 = {"
            "           Type = uint32"
            "           NumberOfElements = 10"
            "       }"
            "       Element2 = {"
            "           Type = float32"
            "       }"
            "       Element3 = {"
            "           Type = char8"
            "           NumberOfElements = {3, 64}"
            "       }"
            "       ElementsB = {"
            "           Type = EPICSPVADatabaseTestT1"
            "           NumberOfElements = 1"
            "       }"
            "   }"
            "}"
            "+EPICSPVADatabase1 = {"
            "    Class = EPICSPVADatabase"
            "    +Rec = {"
            "        Class = EPICSPVA::EPICSPVARecord"
            "        Alias = Record1"
            "        Structure = {"
            "            value = {"
            "                Type = EPICSPVADatabaseTestT2"
            "                NumberOfElements = 1"
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
        ok = (pvDatabase->GetServerContext() ? true : false);
    }
    pvac::ClientProvider provider("pva");
    {
        pvac::ClientChannel channel(provider.connect("Record1"));

        uint32 numberOfElements = 3u;
        uint32 n;
        epics::pvData::shared_vector<std::string> outt;
        outt.resize(numberOfElements);
        outt[0] = "ELEMENT0";
        outt[1] = "ELEMENT1";
        outt[2] = "ELEMENT2";
        epics::pvData::shared_vector<const std::string> outF = freeze(outt);
        const char8 * const field = "value.Element3";
        channel.put().set(field, outF).exec();
        if (ok) {
            epics::pvData::PVStructure::const_shared_pointer getStruct = channel.get();
            std::shared_ptr<const epics::pvData::PVStringArray> rvalue = getStruct->getSubField<epics::pvData::PVStringArray>(field);
            epics::pvData::shared_vector<const std::string> out;
            out.resize(numberOfElements);
            rvalue->getAs < std::string > (out);
            for (n = 0u; (n < numberOfElements) && (ok); n++) {
                ok = (out[n] == outF[n]);
            }
        }
    }
    {
        pvac::ClientChannel channel(provider.connect("Record1"));

        uint32 numberOfElements = 10u;
        uint32 n;
        epics::pvData::shared_vector<uint32> outt;
        outt.resize(numberOfElements);
        for (n = 0u; n < numberOfElements; n++) {
            outt[n] = n + 1;
        }
        epics::pvData::shared_vector<const uint32> outF = freeze(outt);
        const char8 * const field = "value.ElementsB.ElementB1";
        channel.put().set(field, outF).exec();
        if (ok) {
            epics::pvData::PVStructure::const_shared_pointer getStruct = channel.get();
            std::shared_ptr<const epics::pvData::PVUIntArray> rvalue = getStruct->getSubField<epics::pvData::PVUIntArray>(field);
            epics::pvData::shared_vector<const uint32> out;
            out.resize(numberOfElements);
            rvalue->getAs < uint32 > (out);
            for (n = 0u; (n < numberOfElements) && (ok); n++) {
                ok = (out[n] == (n + 1));
            }
        }
    }
    ord->Purge();
    return ok;
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
