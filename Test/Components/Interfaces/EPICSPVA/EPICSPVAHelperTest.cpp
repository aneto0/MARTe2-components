/**
 * @file EPICSPVAHelperTest.cpp
 * @brief Source file for class EPICSPVAHelperTest
 * @date 21/11/2018
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
 * the class EPICSPVAHelperTest (public, protected, and private). Be aware that some 
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
#include "EPICSPVAHelper.h"
#include "EPICSPVAHelperTest.h"
#include "IntrospectionT.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool EPICSPVAHelperTest::TestGetStructure() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = ""
            "+Types = {\n"
            "    Class = ReferenceContainer"
            "    +EPICSPVAHelperTestT1 = {\n"
            "        Class = IntrospectionStructure"
            "        UInt8 = {\n"
            "            Type = uint8\n"
            "            NumberOfElements = 32\n"
            "        }\n"
            "        UInt16 = {\n"
            "            Type = uint16\n"
            "            NumberOfElements = {3}\n"
            "        }\n"
            "        UInt32 = {\n"
            "            Type = uint32\n"
            "            NumberOfElements = {2}\n"
            "        }\n"
            "    }\n"
            "    +EPICSPVAHelperTestT2 = {\n"
            "        Class = IntrospectionStructure"
            "        E1 = {\n"
            "            Type = EPICSPVAHelperTestT1\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        E2 = {\n"
            "            Type = EPICSPVAHelperTestT1\n"
            "            NumberOfElements = {3}\n"
            "        }\n"
            "        Int64 = {\n"
            "            Type = int64\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "    }\n"
            "}\n"
            "";
    configStream.Seek(0);
    StreamString err;
    StandardParser parser(configStream, cdb, &err);

    bool ok = parser.Parse();
    if (ok) {
        (void) cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "%s", err.Buffer());
    }
    const ClassRegistryItem *cri;
    const Introspection *intro;
    if (ok) {
        cri = ClassRegistryDatabase::Instance()->Find("EPICSPVAHelperTestT2");
        ok = (cri != NULL);
    }
    if (ok) {
        intro = cri->GetIntrospection();
        ok = (intro != NULL);
    }
    epics::pvData::StructureConstPtr strPtr;
    if (ok) {
        strPtr = EPICSPVAHelper::GetStructure(intro, "EPICSPVAHelperTestT2");
        ok = (strPtr ? true : false);
    }
    if (ok) {
        epics::pvData::FieldConstPtr e1Ptr = strPtr->getField("E1");
        ok = (e1Ptr ? true : false);
    }
    if (ok) {
        epics::pvData::FieldConstPtr e2Ptr = strPtr->getField("E2");
        ok = (e2Ptr ? true : false);
    }
    if (ok) {
        epics::pvData::FieldConstPtr e3Ptr = strPtr->getField("Int64");
        ok = (e3Ptr ? true : false);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool EPICSPVAHelperTest::TestInitStructure() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = ""
            "+Types = {\n"
            "    Class = ReferenceContainer"
            "    +EPICSPVAHelperTestT1 = {\n"
            "        Class = IntrospectionStructure"
            "        UInt8 = {\n"
            "            Type = uint8\n"
            "            NumberOfElements = 32\n"
            "        }\n"
            "        UInt16 = {\n"
            "            Type = uint16\n"
            "            NumberOfElements = {3}\n"
            "        }\n"
            "        UInt32 = {\n"
            "            Type = uint32\n"
            "            NumberOfElements = {2}\n"
            "        }\n"
            "    }\n"
            "    +EPICSPVAHelperTestT2 = {\n"
            "        Class = IntrospectionStructure"
            "        E1 = {\n"
            "            Type = EPICSPVAHelperTestT1\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        E2 = {\n"
            "            Type = EPICSPVAHelperTestT1\n"
            "            NumberOfElements = {3}\n"
            "        }\n"
            "        Int64 = {\n"
            "            Type = int64\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "    }\n"
            "}\n"
            "";
    configStream.Seek(0);
    StreamString err;
    StandardParser parser(configStream, cdb, &err);

    bool ok = parser.Parse();
    if (ok) {
        (void) cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "%s", err.Buffer());
    }
    const ClassRegistryItem *cri;
    const Introspection *intro;
    if (ok) {
        cri = ClassRegistryDatabase::Instance()->Find("EPICSPVAHelperTestT2");
        ok = (cri != NULL);
    }
    if (ok) {
        intro = cri->GetIntrospection();
        ok = (intro != NULL);
    }
    epics::pvData::StructureConstPtr strPtr;
    if (ok) {
        strPtr = EPICSPVAHelper::GetStructure(intro, "EPICSPVAHelperTestT2");
        ok = (strPtr ? true : false);
    }
    epics::pvData::PVStructurePtr pvStructure;
    if (ok) {
        pvStructure = epics::pvData::getPVDataCreate()->createPVStructure(strPtr);
    }
    if (ok) {
        ok = (pvStructure ? true : false);
    }
    if (ok) {
        ok = EPICSPVAHelper::InitStructure(intro, pvStructure);
        std::cout << pvStructure << std::endl;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool EPICSPVAHelperTest::TestReplaceStructureArray() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = ""
            "+Types = {\n"
            "    Class = ReferenceContainer"
            "    +EPICSPVAHelperTestT1 = {\n"
            "        Class = IntrospectionStructure"
            "        UInt8 = {\n"
            "            Type = uint8\n"
            "            NumberOfElements = 32\n"
            "        }\n"
            "        UInt16 = {\n"
            "            Type = uint16\n"
            "            NumberOfElements = {3}\n"
            "        }\n"
            "        UInt32 = {\n"
            "            Type = uint32\n"
            "            NumberOfElements = {2}\n"
            "        }\n"
            "    }\n"
            "    +EPICSPVAHelperTestT2 = {\n"
            "        Class = IntrospectionStructure"
            "        E1 = {\n"
            "            Type = EPICSPVAHelperTestT1\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "        E2 = {\n"
            "            Type = EPICSPVAHelperTestT1\n"
            "            NumberOfElements = {3}\n"
            "        }\n"
            "        Int64 = {\n"
            "            Type = int64\n"
            "            NumberOfElements = 1\n"
            "        }\n"
            "    }\n"
            "}\n"
            "";
    configStream.Seek(0);
    StreamString err;
    StandardParser parser(configStream, cdb, &err);

    bool ok = parser.Parse();
    if (ok) {
        (void) cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "%s", err.Buffer());
    }
    const ClassRegistryItem *cri;
    const Introspection *intro;
    if (ok) {
        cri = ClassRegistryDatabase::Instance()->Find("EPICSPVAHelperTestT2");
        ok = (cri != NULL);
    }
    if (ok) {
        intro = cri->GetIntrospection();
        ok = (intro != NULL);
    }
    uint32 numberOfElements = 2u;
    const char8* const typeName = "EPICSPVAHelperTestT2";
    epics::pvData::StructureConstPtr strPtr;
    if (ok) {
        epics::pvData::FieldCreatePtr fieldCreate = epics::pvData::getFieldCreate();
        epics::pvData::FieldBuilderPtr fieldBuilder = fieldCreate->createFieldBuilder();
        fieldBuilder = fieldBuilder->addArray("value", EPICSPVAHelper::GetStructure(intro, typeName));
        strPtr = fieldBuilder->createStructure();
        ok = (strPtr ? true : false);
    }
    epics::pvData::PVStructureArrayPtr pvStructureArray;
    epics::pvData::PVStructurePtr pvStructure;
    if (ok) {
        pvStructure = epics::pvData::getPVDataCreate()->createPVStructure(strPtr);
        pvStructureArray = pvStructure->getSubField<epics::pvData::PVStructureArray> ("value");
    }
    if (ok) {
        ok = (pvStructureArray ? true : false);
    }
    if (ok) {
        ok = EPICSPVAHelper::ReplaceStructureArray(intro, pvStructureArray, numberOfElements, typeName);
        std::cout << pvStructure << std::endl;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

