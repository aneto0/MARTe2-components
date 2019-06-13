/**
 * @file OPCUANodeTest.cpp
 * @brief Source file for class OPCUANodeTest
 * @date 12/03/2019
 * @author Luca Porzio
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
 * the class OPCUANodeTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "OPCUANodeTest.h"
#include "OPCUANode.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool OPCUANodeTest::TestConstructor() {
    using namespace MARTe;
    OPCUANode test;
    return test.IsNode();
}

bool OPCUANodeTest::Test_IsNode() {
    using namespace MARTe;
    OPCUANode test;
    return test.IsNode();
}

bool OPCUANodeTest::Test_InitArray() {
    return TestGetOPCVariable_array_uint8();
}

bool OPCUANodeTest::TestGetOPCVariable_uint8() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint8"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_uint16() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint16"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_uint32() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_uint64() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint64"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_float32() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = float32"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_float64() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = float64"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_int8() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = int8"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_int16() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = int16"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_int32() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = int32"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_int64() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = int64"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_array_uint8() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint8"
            "             NumberOfElements = 2"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_array_uint16() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint16"
            "             NumberOfElements = 2"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_array_uint32() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint32"
            "             NumberOfElements = 2"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_array_uint64() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint64"
            "             NumberOfElements = 2"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_array_float32() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = float32"
            "             NumberOfElements = 2"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_array_float64() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = float64"
            "             NumberOfElements = 2"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_array_int8() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = int8"
            "             NumberOfElements = 2"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_array_int16() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = int16"
            "             NumberOfElements = 2"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_array_int32() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = int32"
            "             NumberOfElements = 2"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

bool OPCUANodeTest::TestGetOPCVariable_array_int64() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = int64"
            "             NumberOfElements = 2"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(100);
    ord->Purge();
    return ok;
}

