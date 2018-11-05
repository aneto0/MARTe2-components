/**
 * @file CRCGAMTest.cpp
 * @brief Source file for class CRCGAMTest
 * @date Oct 30, 2018 TODO Verify the value and format of the date
 * @author root TODO Verify the name and format of the author
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
 * the class CRCGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CRCGAMTest.h"
#include "ConfigurationDatabase.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "DataSourceI.h"
#include "StructuredDataI.h"
#include "AdvancedErrorManagement.h"
#include "RealTimeApplication.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#define MAX_UINT8    256
#define MAX_UINT16   65536
#define MAX_UINT32   4294967296


namespace MARTe {

class CRCTestHelper : public CRCGAM {
public:
    CLASS_REGISTER_DECLARATION()

    CRCTestHelper(uint32 pol = 0, uint32 initCRC = 0, uint8 inv = 0) {
        polynomial = pol;
        initialCRCValue = initCRC;
        isReflected = inv;
    }

    bool InitialiseHelper() {
        bool ok;
        ok = config.Write("Polynomial", polynomial);
        ok = config.Write("InitialValue", initialCRCValue);
        ok = config.Write("Inverted", isReflected);
        return ok;
    }

    void *GetOutputSignalMemory(const uint32 signalIdx) {
        return CRCGAM::GetOutputSignalMemory(signalIdx);
    }

    ConfigurationDatabase config;

private:
    uint32 polynomial;
    uint32 initialCRCValue;
    uint8 isReflected;
};
CLASS_REGISTER(CRCTestHelper, "1.0")
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool CRCGAMTest::TestConstructor() {
    using namespace MARTe;
    CRCGAM test;
    bool ok = (test.GetNumberOfInputSignals() == 0u) && (test.GetNumberOfOutputSignals() == 0u);
    return ok;
}

bool CRCGAMTest::TestInitialise() {
    using namespace MARTe;
    CRCTestHelper gam;
    bool ok = gam.InitialiseHelper();
    ok = gam.Initialise(gam.config);
    return ok;
}

bool CRCGAMTest::TestInitialiseMissingParameters() {
    using namespace MARTe;
    CRCGAM gam;
    ConfigurationDatabase config;
    bool ok;
    ok = !gam.Initialise(config);
    return ok;
}

bool CRCGAMTest::TestInitialiseMissingPolynomial() {
    using namespace MARTe;
    CRCGAM gam;
    bool ok;
    ConfigurationDatabase config;
    uint32 initCRC = 0;
    uint8 inverted = 0;
    config.Write("InitialValue", initCRC);
    config.Write("Inverted", inverted);
    ok = !gam.Initialise(config);
    return ok;
}

bool CRCGAMTest::TestInitialiseMissingInitialCRCValue() {
    using namespace MARTe;
    CRCGAM gam;
    bool ok;
    ConfigurationDatabase config;
    uint32 pol = 0;
    uint8 inverted = 0;
    config.Write("Polynomial", pol);
    config.Write("Inverted", inverted);
    ok = !gam.Initialise(config);
    return ok;
}

bool CRCGAMTest::TestInitialiseMissingInverted() {
    using namespace MARTe;
    CRCGAM gam;
    bool ok;
    ConfigurationDatabase config;
    uint32 pol = 0;
    uint32 initCRC = 0;
    config.Write("Polynomial", pol);
    config.Write("InitialValue", initCRC);
    ok = !gam.Initialise(config);
    return ok;
}

bool CRCGAMTest::TestSetup() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();

    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        cdb.MoveToRoot();
        ord->Purge();
        ok = ord->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = ord->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    ord->Purge();
    return ok;
}

bool CRCGAMTest::TestSetupWrongNumberOfInput() {
    using namespace MARTe;
    CRCTestHelper gam;
    bool ok = gam.InitialiseHelper();
    ok = gam.Initialise(gam.config);
    ok = !gam.Setup();
    return ok;
}

bool CRCGAMTest::TestSetupWrongNumberOfOutput() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1");
    }
    if (ok) {
        ok = cdb.Delete("OutputSignals");
    }
    if(ok) {
        ok = cdb.MoveToAncestor(1u);
    }
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        cdb.MoveToRoot();
        ord->Purge();
        ok = ord->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = ord->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = !application->ConfigureApplication();
    }

    ord->Purge();
    return ok;
}

bool CRCGAMTest::TestSetupWrongOutputType() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.OutputSignals");
    }
    if (ok) {
        StreamString signalName = "Signal2";
        ok = cdb.MoveRelative(signalName.Buffer());
        if (ok) {
            ok = cdb.Delete("Type");
        }
        if (ok) {
            ok = cdb.Write("Type", "uint64");
        }
        if (ok) {
            ok = cdb.MoveToAncestor(1u);
        }
    }
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        cdb.MoveToRoot();
        ord->Purge();
        ok = ord->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = ord->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = !application->ConfigureApplication();
    }
    ord->Purge();
    return ok;
}

bool CRCGAMTest::TestExecuteUint8() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();

    if(ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.OutputSignals.Signal2");
    }
    if(ok) {
        ok = cdb.Delete("Type");
    }
    if(ok) {
        ok = cdb.Write("Type", "uint8");
    }
    if(ok) {
        ok = cdb.MoveToAncestor(1u);
    }
    if(ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1");
    }
    if(ok) {
        ok = cdb.Delete("Polynomial");
    }
    if(ok) {
        ok = cdb.Write("Polynomial", 0x7);
    }
    if(ok) {
        ok = cdb.MoveToAncestor(1u);
    }

    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if(ok) {
        cdb.MoveToRoot();
        ord->Purge();
        ok = ord->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if(ok) {
        application = ord->Find("Test");
        ok = application.IsValid();
    }
    if(ok) {
        ok = application->ConfigureApplication();
    }
    if(ok) {
        ok = application->PrepareNextState("State1");
    }
    if(ok) {
        ok = application->StartNextStateExecution();
    }

    ReferenceT<CRCTestHelper> gam = ord->Find("Test.Functions.GAM1");

    uint8 * output = reinterpret_cast<uint8*>(gam->GetOutputSignalMemory(0u));

    if(*output != 0 && *output < MAX_UINT8) {
        ok = true;
    } else {
        ok = false;
    }

    application->StopCurrentStateExecution();

    return ok;
}

bool CRCGAMTest::TestExecuteUint16() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();

    if(ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.OutputSignals.Signal2");
    }
    if(ok) {
        ok = cdb.Delete("Type");
    }
    if(ok) {
        ok = cdb.Write("Type", "uint8");
    }
    if(ok) {
        ok = cdb.MoveToAncestor(1u);
    }
    if(ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1");
    }
    if(ok) {
        ok = cdb.Delete("Polynomial");
    }
    if(ok) {
        ok = cdb.Write("Polynomial", 0x1021);
    }
    if(ok) {
        ok = cdb.MoveToAncestor(1u);
    }

    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if(ok) {
        cdb.MoveToRoot();
        ord->Purge();
        ok = ord->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if(ok) {
        application = ord->Find("Test");
        ok = application.IsValid();
    }
    if(ok) {
        ok = application->ConfigureApplication();
    }
    if(ok) {
        ok = application->PrepareNextState("State1");
    }
    if(ok) {
        ok = application->StartNextStateExecution();
    }

    ReferenceT<CRCTestHelper> gam = ord->Find("Test.Functions.GAM1");

    uint16 * output = reinterpret_cast<uint16*>(gam->GetOutputSignalMemory(0u));

    if(*output != 0 && *output < MAX_UINT16) {
        ok = true;
    } else {
        ok = false;
    }

    application->StopCurrentStateExecution();

    return ok;
}

bool CRCGAMTest::TestExecuteUint32() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = configFromBasicTypeTemplate;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ok = parser.Parse();

    if(ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1.OutputSignals.Signal2");
    }
    if(ok) {
        ok = cdb.Delete("Type");
    }
    if(ok) {
        ok = cdb.Write("Type", "uint8");
    }
    if(ok) {
        ok = cdb.MoveToAncestor(1u);
    }
    if(ok) {
        ok = cdb.MoveAbsolute("$Test.+Functions.+GAM1");
    }
    if(ok) {
        ok = cdb.Delete("Polynomial");
    }
    if(ok) {
        ok = cdb.Write("Polynomial", 0x1021);
    }
    if(ok) {
        ok = cdb.MoveToAncestor(1u);
    }

    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if(ok) {
        cdb.MoveToRoot();
        ord->Purge();
        ok = ord->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if(ok) {
        application = ord->Find("Test");
        ok = application.IsValid();
    }
    if(ok) {
        ok = application->ConfigureApplication();
    }
    if(ok) {
        ok = application->PrepareNextState("State1");
    }
    if(ok) {
        ok = application->StartNextStateExecution();
    }

    ReferenceT<CRCTestHelper> gam = ord->Find("Test.Functions.GAM1");

    uint32 * output = reinterpret_cast<uint32*>(gam->GetOutputSignalMemory(0u));

    if(*output != 0 && *output < MAX_UINT32) {
        ok = true;
    } else {
        ok = false;
    }

    application->StopCurrentStateExecution();

    return ok;
}
	
