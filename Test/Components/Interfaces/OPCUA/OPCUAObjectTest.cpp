/**
 * @file OPCUAObjectTest.cpp
 * @brief Source file for class OPCUAObjectTest
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
 * the class OPCUAObjectTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "OPCUAObjectTest.h"
#include "OPCUAObject.h"
#include "StandardParser.h"
#include "ObjectRegistryDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

bool OPCUAObjectTest::TestGetOPCObject_First() {
    using namespace MARTe;
    StreamString config = ""
            "+OPCUATypes = {"
            "     Class = ReferenceContainer"
            "     +MasterSet = {"
            "         Class = IntrospectionStructure"
            "         Sensor1 = {"
            "             Type = float64"
            "             NumberOfElements = 1"
            "         }"
            "     }"
            "}"
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         TestStructure = {"
            "             Type = MasterSet"
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

bool OPCUAObjectTest::TestGetOPCObject_NotFirst() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "+OPCUATypes = {"
            "     Class = ReferenceContainer"
            "     +SensorPackage = {"
            "         Class = IntrospectionStructure"
            "         Sensor1 = {"
            "             Type = float64"
            "             NumberOfElements = 2"
            "             NumberOfDimensions = 2"
            "         }"
            "     }"
            "     +MasterSet = {"
            "         Class = IntrospectionStructure"
            "         SensorPackage1 = {"
            "             Type = SensorPackage"
            "             NumberOfElements = 1"
            "         }"
            "     }"
            "}"
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         TestStructure = {"
            "             Type = MasterSet"
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
    return !ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

