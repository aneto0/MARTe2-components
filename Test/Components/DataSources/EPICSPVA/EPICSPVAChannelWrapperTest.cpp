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

bool EPICSPVAChannelWrapperTest::TestSetup() {
    using namespace MARTe;
    EPICSPVAChannelWrapper test;
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("Record1.value");
    cdb.Write("Type", "uint32");
    cdb.Write("NumberOfElements", 1);
    cdb.CreateAbsolute("Record1.NodeA.NodeB.Element1");
    cdb.Write("Type", "float64");
    cdb.Write("NumberOfElements", 24);
    cdb.MoveAbsolute("Record1");
    bool ok = test.Setup(cdb);
    if (ok) {
        StreamString chName = test.GetChannelName();
        ok = (chName == "Record1");
    }
    if (ok) {
        void *mem = NULL;
        test.GetSignalMemory("value", mem);
        ok = (mem != NULL);
    }
    if (ok) {
        void *mem = NULL;
        test.GetSignalMemory("NodeA.NodeB.Element1", mem);
        ok = (mem != NULL);
    }
    if (ok) {
        void *mem = NULL;
        test.GetSignalMemory("NodeA.NodeB.DoesNotExist", mem);
        ok = (mem == NULL);
    }
    return ok;
}

bool EPICSPVAChannelWrapperTest::TestSetup_False_Dimensions() {
    using namespace MARTe;
    EPICSPVAChannelWrapper test;
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("Record1.value");
    cdb.Write("Type", "uint32");
    cdb.Write("NumberOfElements", 10);
    cdb.Write("NumberOfDimensions", 2);
    cdb.MoveAbsolute("Record1");
    bool ok = !test.Setup(cdb);
    return ok;
}

bool EPICSPVAChannelWrapperTest::TestSetup_False_InvalidLeaf() {
    using namespace MARTe;
    EPICSPVAChannelWrapper test;
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("Record1.value");
    cdb.Write("Type", "uint32");
    cdb.Write("NumberOfElements", 10);
    cdb.Write("NumberOfDimensions", 1);
    cdb.CreateAbsolute("Record1.value2");
    cdb.MoveAbsolute("Record1");
    bool ok = !test.Setup(cdb);
    return ok;
}

bool EPICSPVAChannelWrapperTest::TestGetSignalMemory() {
    return TestSetup();
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
    using namespace MARTe;
    bool ok = false;
    {
        EPICSPVAChannelWrapper test;
        ConfigurationDatabase cdb;
        cdb.CreateAbsolute("Record1.value");
        cdb.Write("Type", "uint32");
        cdb.Write("NumberOfElements", 1);
        cdb.CreateAbsolute("Record1.NodeA.NodeB.Element1");
        cdb.Write("Type", "float64");
        cdb.Write("NumberOfElements", 24);
        cdb.MoveAbsolute("Record1");
        ok = test.Setup(cdb);
        if (ok) {
            StreamString chName = test.GetChannelName();
            ok = (chName == "Record1");
        }
        if (ok) {
            StreamString chName = test.GetChannelUnaliasedName();
            ok = (chName == "Record1");
        }
    }
    if (ok) {
        EPICSPVAChannelWrapper test;
        ConfigurationDatabase cdb;
        cdb.CreateAbsolute("Record1.value");
        cdb.Write("Type", "uint32");
        cdb.Write("NumberOfElements", 1);
        cdb.CreateAbsolute("Record1.NodeA.NodeB.Element1");
        cdb.Write("Type", "float64");
        cdb.Write("NumberOfElements", 24);
        cdb.MoveAbsolute("Record1");
        cdb.Write("Alias", "Test::Rec");
        ok = test.Setup(cdb);
        if (ok) {
            StreamString chName = test.GetChannelName();
            ok = (chName == "Test::Rec");
        }
        if (ok) {
            StreamString chName = test.GetChannelUnaliasedName();
            ok = (chName == "Record1");
        }
    }
    return ok;
}

bool EPICSPVAChannelWrapperTest::TestGetChannelUnaliasedName() {
    return TestGetChannelName();
}
