/**
 * @file EPICSPVARecordTest.cpp
 * @brief Source file for class EPICSPVARecordTest
 * @date 13/10/2018
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
 * the class EPICSPVARecordTest (public, protected, and private). Be aware that some 
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
#include "EPICSPVARecord.h"
#include "EPICSPVARecordTest.h"
#include "IntrospectionStructure.h"
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
#include "MessageI.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool EPICSPVARecordTest::TestConstructor() {
    using namespace MARTe;
    EPICSPVARecord pvaRecord;
    return (pvaRecord.NumberOfReferences() == 0u);
}

bool EPICSPVARecordTest::TestCreatePVRecord() {
    using namespace MARTe;
    ConfigurationDatabase cdbType;
    cdbType.CreateAbsolute("+EPICSPVARecordTestS1");
    cdbType.CreateRelative("A");
    cdbType.Write("Type", "uint32");
    cdbType.Write("NumberOfElements", 3);
    cdbType.CreateAbsolute("+EPICSPVARecordTestS2");
    cdbType.CreateRelative("B");
    cdbType.Write("Type", "EPICSPVARecordTestS1");
    cdbType.Write("NumberOfElements", 2);

    cdbType.MoveAbsolute("+EPICSPVARecordTestS1");
    IntrospectionStructure is1;
    is1.SetName("EPICSPVARecordTestS1");
    is1.Initialise(cdbType);
    IntrospectionStructure is2;
    cdbType.MoveAbsolute("+EPICSPVARecordTestS2");
    is2.SetName("EPICSPVARecordTestS2");
    is2.Initialise(cdbType);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("Structure.B");
    cdb.Write("Type", "uint32");
    cdb.Write("NumberOfElements", 1);
    cdb.CreateAbsolute("Structure.C");
    cdb.Write("Type", "uint32");
    cdb.Write("NumberOfElements", 10);
    cdb.CreateAbsolute("Structure.D");
    cdb.Write("Type", "uint8");
    cdb.Write("NumberOfElements", 64);
    cdb.CreateAbsolute("Structure.E");
    cdb.Write("Type", "EPICSPVARecordTestS2");
    cdb.Write("NumberOfElements", 1);
    cdb.MoveToRoot();
    EPICSPVARecord pvaRecord;
    pvaRecord.SetName("EPICSPVARecordTestTestCreatePVRecord");
    bool ok = pvaRecord.Initialise(cdb);
    if (ok) {
        ok = (pvaRecord.CreatePVRecord() ? true : false);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool EPICSPVARecordTest::TestInitialise() {
    return TestCreatePVRecord();
}

bool EPICSPVARecordTest::TestInitialise_False_NoStructure() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("EPICSPVARecordTest.B");
    cdb.Write("Type", "uint32");
    cdb.Write("NumberOfElements", 1);
    cdb.MoveToRoot();
    EPICSPVARecord pvaRecord;
    pvaRecord.SetName("EPICSPVARecordTestTestInitialise_False_NoStructure");
    bool ok = !pvaRecord.Initialise(cdb);
    return ok;
}

bool EPICSPVARecordTest::TestInitialise_False_InvalidType() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("Structure.C");
    cdb.Write("Type", "uint4");
    cdb.Write("NumberOfElements", 1);
    cdb.MoveToRoot();
    EPICSPVARecord pvaRecord;
    pvaRecord.SetName("EPICSPVARecordTestTestInitialise_False_InvalidType");
    bool ok = pvaRecord.Initialise(cdb);
    if (ok) {
        ok = (pvaRecord.CreatePVRecord() ? false : true);
    }
    return ok;
}

bool EPICSPVARecordTest::TestInitialise_False_InvalidType_Char8() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("Structure.C");
    cdb.Write("Type", "char8");
    cdb.Write("NumberOfElements", 64);
    cdb.MoveToRoot();
    EPICSPVARecord pvaRecord;
    pvaRecord.SetName("EPICSPVARecordTestTestInitialise_False_InvalidType");
    bool ok = pvaRecord.Initialise(cdb);
    if (ok) {
        ok = (pvaRecord.CreatePVRecord() ? false : true);
    }
    return ok;
}

bool EPICSPVARecordTest::TestInitialise_False_NoType() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("Structure.StA.C");
    cdb.Write("Type", "uint32");
    cdb.Write("NumberOfElements", 1);
    cdb.MoveToRoot();
    EPICSPVARecord pvaRecord;
    pvaRecord.SetName("EPICSPVARecordTestTestInitialise_False_InvalidType");
    bool ok = pvaRecord.Initialise(cdb);
    if (ok) {
        ok = (pvaRecord.CreatePVRecord() ? false : true);
    }
    return ok;
}

bool EPICSPVARecordTest::TestGetRecordName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("Structure.B");
    cdb.Write("Type", "uint32");
    cdb.Write("NumberOfElements", 1);
    cdb.CreateAbsolute("Structure.C");
    cdb.Write("Type", "uint32");
    cdb.Write("NumberOfElements", 10);
    cdb.MoveToRoot();
    EPICSPVARecord pvaRecord;
    pvaRecord.SetName("EPICSPVARecordTestTestCreatePVRecord");
    bool ok = pvaRecord.Initialise(cdb);
    if (ok) {
        StreamString recordName;
        pvaRecord.GetRecordName(recordName);
        ok = (recordName == "EPICSPVARecordTestTestCreatePVRecord");
    }
    if (ok) {
        EPICSPVARecord pvaRecord2;
        cdb.MoveToRoot();
        cdb.Write("Alias", "A::Different::Name");
        ok = pvaRecord2.Initialise(cdb);
        if (ok) {
            StreamString recordName;
            pvaRecord2.GetRecordName(recordName);
            ok = (recordName == "A::Different::Name");
        }
    }

    return ok;
}

