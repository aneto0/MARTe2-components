/**
 * @file EPICSPVTest.cpp
 * @brief Source file for class EPICSPVTest
 * @date 25/03/2017
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
 * the class EPICSPVTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "CLASSMETHODREGISTER.h"
#include "EPICSPV.h"
#include "EPICSPVTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * Helper class that reacts to messages received from the EPICSPV
 */
class EPICSPVTestHelper : public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()
    EPICSPVTestHelper() : Object(), MessageI() {
        uint32Value = 0u;
    }

    MARTe::ErrorManagement::ErrorType HandleInt32(const MARTe::uint32 newValue) {
        uint32Value = newValue;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::uint32 uint32Value;
};
CLASS_REGISTER(EPICSPVTestHelper, "1.0")
CLASS_METHOD_REGISTER(EPICSPVTestHelper, HandleInt32)
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool EPICSPVTest::TestConstructor() {
    using namespace MARTe;
    EPICSPV pv;
    bool ok = (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "");
    ok &= (pv.GetFunction() == "");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().asUint8 == 0u);
    ok &= (pv.GetPVName() == "");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_INT);
    ok &= (pv.GetTimeout() == 5.0);
    return ok;
}

bool EPICSPVTest::TestInitialise_NoEvent_UInt32() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONEU");
    cdb.Write("PVType", "uint32");

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "");
    ok &= (pv.GetFunction() == "");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().notSet);
    ok &= (pv.GetPVName() == "PVONEU");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_INT);
    ok &= (pv.GetTimeout() == 5.0);

    return ok;
}

bool EPICSPVTest::TestInitialise_NoEvent_Int32() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONE");
    cdb.Write("PVType", "int32");

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "");
    ok &= (pv.GetFunction() == "");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().notSet);
    ok &= (pv.GetPVName() == "PVONE");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_INT);
    ok &= (pv.GetTimeout() == 5.0);
    return ok;
}

bool EPICSPVTest::TestInitialise_NoEvent_Float32() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONEF");
    cdb.Write("PVType", "float32");

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "");
    ok &= (pv.GetFunction() == "");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().notSet);
    ok &= (pv.GetPVName() == "PVONEF");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_FLOAT);
    ok &= (pv.GetTimeout() == 5.0);
    return ok;
}

bool EPICSPVTest::TestInitialise_NoEvent_Float64() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONED");
    cdb.Write("PVType", "float64");

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "");
    ok &= (pv.GetFunction() == "");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().notSet);
    ok &= (pv.GetPVName() == "PVONED");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_DOUBLE);
    ok &= (pv.GetTimeout() == 5.0);
    return ok;
}

bool EPICSPVTest::TestInitialise_NoEvent_String() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "");
    ok &= (pv.GetFunction() == "");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().notSet);
    ok &= (pv.GetPVName() == "PVONES");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_STRING);
    ok &= (pv.GetTimeout() == 5.0);
    return ok;
}

bool EPICSPVTest::TestInitialise_NoEvent_Timeout() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.Write("Timeout", 3);

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "");
    ok &= (pv.GetFunction() == "");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().notSet);
    ok &= (pv.GetPVName() == "PVONES");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_STRING);
    ok &= (pv.GetTimeout() == 3);
    return ok;
}

bool EPICSPVTest::TestInitialise_False_PVName() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVType", "string");
    cdb.Write("Timeout", 3);

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestInitialise_False_PVType() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("Timeout", 3);

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestInitialise_False_ZeroTimeout() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.Write("Timeout", 0);

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestInitialise_Event_Function() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("PVValue", "Function");
    cdb.Write("Function", "AFunction");
    cdb.MoveToRoot();

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "AnObject");
    ok &= (pv.GetFunction() == "AFunction");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().function);
    ok &= (pv.GetPVName() == "PVONES");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_STRING);
    ok &= (pv.GetTimeout() == 5.0);
    return ok;
}

bool EPICSPVTest::TestInitialise_Event_FunctionMap() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("PVValue", "Function");
    Matrix<StreamString> functionMap(3, 2);
    functionMap[0][0] = "AA";
    functionMap[1][0] = "BB";
    functionMap[2][0] = "CC";
    functionMap[0][1] = "AFunction";
    functionMap[1][1] = "BFunction";
    functionMap[2][1] = "CFunction";
    cdb.Write("FunctionMap", functionMap);
    cdb.MoveToRoot();

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "AnObject");
    ok &= (pv.GetFunction() == "");
    ok &= (pv.GetFunctionFromMap("AA") == "AFunction");
    ok &= (pv.GetFunctionFromMap("BB") == "BFunction");
    ok &= (pv.GetFunctionFromMap("CC") == "CFunction");
    ok &= (pv.GetMode().function);
    ok &= (pv.GetPVName() == "PVONES");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_STRING);
    ok &= (pv.GetTimeout() == 5.0);
    return ok;
}

bool EPICSPVTest::TestInitialise_Event_Parameter() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("PVValue", "Parameter");
    cdb.Write("Function", "AFunction");
    cdb.MoveToRoot();

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "AnObject");
    ok &= (pv.GetFunction() == "AFunction");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().parameter);
    ok &= (pv.GetPVName() == "PVONES");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_STRING);
    ok &= (pv.GetTimeout() == 5.0);
    return ok;
}

bool EPICSPVTest::TestInitialise_Event_Ignore() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("Function", "AFunction");
    cdb.Write("PVValue", "Ignore");
    cdb.MoveToRoot();

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "AnObject");
    ok &= (pv.GetFunction() == "AFunction");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().ignore);
    ok &= (pv.GetPVName() == "PVONES");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_STRING);
    ok &= (pv.GetTimeout() == 5.0);
    return ok;
}

bool EPICSPVTest::TestInitialise_Event_False_PVValue() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.MoveToRoot();

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestInitialise_Event_False_PVValue_Invalid() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("PVValue", "Invalid");
    cdb.MoveToRoot();

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestInitialise_Event_False_Function_NoFunctionName() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("PVValue", "Function");
    cdb.MoveToRoot();

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestInitialise_Event_False_Parameter_NoFunctionName() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("PVValue", "Parameter");
    cdb.MoveToRoot();

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestInitialise_Event_False_Ignore_NoFunctionName() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("PVValue", "Ignore");
    cdb.MoveToRoot();

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestInitialise_Event_False_Function_And_FunctionMap() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("PVValue", "Function");
    Matrix<StreamString> functionMap(3, 2);
    functionMap[0][0] = "AA";
    functionMap[1][0] = "BB";
    functionMap[2][0] = "CC";
    functionMap[0][1] = "AFunction";
    functionMap[1][1] = "BFunction";
    functionMap[2][1] = "CFunction";
    cdb.Write("FunctionMap", functionMap);
    cdb.Write("Function", "AFunction");
    cdb.MoveToRoot();

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestInitialise_Event_False_FunctionMap_NotMatrix() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("PVValue", "Function");
    Vector<StreamString> functionMap(3);
    functionMap[0] = "AA";
    functionMap[1] = "BB";
    functionMap[2] = "CC";
    cdb.Write("FunctionMap", functionMap);
    cdb.MoveToRoot();

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestInitialise_Event_False_FunctionMap_MatrixNotTwoColumns() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("PVValue", "Function");
    Matrix<StreamString> functionMap(3, 3);
    functionMap[0][0] = "AA";
    functionMap[1][0] = "BB";
    functionMap[2][0] = "CC";
    functionMap[0][1] = "AFunction";
    functionMap[1][1] = "BFunction";
    functionMap[2][1] = "CFunction";
    functionMap[0][2] = "AFunction";
    functionMap[1][2] = "BFunction";
    functionMap[2][2] = "CFunction";
    cdb.Write("FunctionMap", functionMap);
    cdb.MoveToRoot();

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestInitialise_Event_False_NotFunction_And_FunctionMap() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("PVValue", "Parameter");
    Matrix<StreamString> functionMap(3, 2);
    functionMap[0][0] = "AA";
    functionMap[1][0] = "BB";
    functionMap[2][0] = "CC";
    functionMap[0][1] = "AFunction";
    functionMap[1][1] = "BFunction";
    functionMap[2][1] = "CFunction";
    cdb.Write("FunctionMap", functionMap);
    cdb.MoveToRoot();

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestInitialise_False_PVType_Invalid() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "strangetype");

    return !(pv.Initialise(cdb));
}

bool EPICSPVTest::TestSetContext() {
    using namespace MARTe;
    EPICSPV pv;
    ca_client_context *cx = (ca_client_context *)0xAABBCCDD;
    pv.SetContext(cx);
    return (pv.GetContext() == cx);
}

bool EPICSPVTest::TestGetContext() {
    return TestSetContext();
}

bool EPICSPVTest::TestGetPVName() {
    return TestInitialise_NoEvent_Int32();
}

bool EPICSPVTest::TestGetPVChid() {
    return TestInitialise_NoEvent_Int32();
}

bool EPICSPVTest::TestSetPVChid() {
    using namespace MARTe;
    EPICSPV pv;
    chid c = (chid)0xFFFFAABB;
    pv.SetPVChid(c);
    return (pv.GetPVChid() == c);
}

bool EPICSPVTest::TestGetPVType() {
    return TestInitialise_NoEvent_Int32();
}

bool EPICSPVTest::TestGetMode() {
    return TestInitialise_Event_Function();
}

bool EPICSPVTest::TestGetTimeout() {
    return TestInitialise_NoEvent_Timeout();
}

bool EPICSPVTest::TestGetDestination() {
    return TestInitialise_Event_Function();
}

bool EPICSPVTest::TestGetFunction() {
    return TestInitialise_Event_Function();
}

bool EPICSPVTest::TestGetFunctionFromMap() {
    return TestInitialise_Event_FunctionMap();
}

bool EPICSPVTest::TestCAPut() {
    return true;
}

bool EPICSPVTest::TestCAGet() {
    return true;
}

bool EPICSPVTest::TestHandlePVEvent() {
    return true;
}
