/**
 * @file EPICSPVAChannelWrapperTest.cpp
 * @brief Source file for class EPICSPVAChannelWrapperTest
 * @date 23/10/2018
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
 * the class EPICSPVAChannelWrapperTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Directory.h"
#include "DirectoryScanner.h"
#include "EPICSPVAChannelWrapper.h"
#include "EPICSPVAChannelWrapperTest.h"
#include "EPICSPVAInputTest.h"
#include "EPICSPVAOutputTest.h"
#include "GAM.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool EPICSPVAChannelWrapperTest::TestConstructor() {
    using namespace MARTe;
    EPICSPVAChannelWrapper test;
    StreamString chName = test.GetChannelName();
    return (chName.Size() == 0u);
}

bool EPICSPVAChannelWrapperTest::TestSetAliasAndField() {
    using namespace MARTe;
    using namespace MARTe;
    bool ok = false;
    {
        EPICSPVAChannelWrapper test;
        ConfigurationDatabase cdb;
        cdb.CreateAbsolute("Record1");
        ok = test.SetAliasAndField(cdb);
        if (ok) {
            StreamString chName = test.GetChannelName();
            ok = (chName == "Record1");
        }
        if (ok) {
            StreamString fieldName = test.GetFieldName();
            ok = (fieldName == "value");
        }
    }
    {
        EPICSPVAChannelWrapper test;
        ConfigurationDatabase cdb;
        cdb.CreateAbsolute("Record1");
        cdb.Write("Alias", "A:B:C");
        cdb.Write("Field", "Test");
        ok = test.SetAliasAndField(cdb);
        if (ok) {
            StreamString chName = test.GetChannelName();
            ok = (chName == "A:B:C");
        }
        if (ok) {
            StreamString fieldName = test.GetFieldName();
            ok = (fieldName == "Test");
        }
    }
    return ok;
}

bool EPICSPVAChannelWrapperTest::TestSetup() {
    using namespace MARTe;
    return TestMonitor();
}

bool EPICSPVAChannelWrapperTest::TestPut() {
    EPICSPVAOutputTest test;
    return test.TestSynchronise();
}

bool EPICSPVAChannelWrapperTest::TestMonitor() {
    EPICSPVAInputTest test;
    return test.TestExecute();
}

bool EPICSPVAChannelWrapperTest::TestGetChannelName() {
    return TestSetAliasAndField();
}

bool EPICSPVAChannelWrapperTest::TestGetFieldName() {
    return TestSetAliasAndField();
}
