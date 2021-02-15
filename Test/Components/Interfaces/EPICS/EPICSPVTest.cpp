/**
 * @file EPICSPVTest.cpp
 * @brief Source file for class EPICSPVTest
 * @date 04/02/2021
 * @author Andre Neto
 * @author Pedro Lourenco
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
 *
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
#include "EPICSPVTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

CLASS_REGISTER(EPICSPVTestHelper, "1.0")
CLASS_METHOD_REGISTER(EPICSPVTestHelper, HandleMessage)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, HandleNoParameter)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, Handle_int16)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, Handle_uint16)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, Handle_int32)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, Handle_uint32)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, Handle_float32)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, Handle_float64)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, Handle_string)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, Handle_vec)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, Handle_str_vec)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, HandleInt32ParameterName)

CLASS_REGISTER(EPICSPVTestCAPut, "1.0")
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
    ok &= (pv.GetPVType() == DBR_LONG);
    ok &= (pv.GetCATimeout() == 5.0);
    return ok;
}

bool EPICSPVTest::TestInitialise_NoEvent_UInt16() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONEU");
    cdb.Write("PVType", "uint16");

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "");
    ok &= (pv.GetFunction() == "");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().notSet);
    ok &= (pv.GetPVName() == "PVONEU");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_SHORT);
    ok &= (pv.GetCATimeout() == 5.0);
    ok &= (pv.GetAnyType().GetTypeDescriptor() == UnsignedInteger16Bit);

    return ok;
}

bool EPICSPVTest::TestInitialise_NoEvent_Int16() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONE");
    cdb.Write("PVType", "int16");

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "");
    ok &= (pv.GetFunction() == "");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().notSet);
    ok &= (pv.GetPVName() == "PVONE");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_SHORT);
    ok &= (pv.GetCATimeout() == 5.0);
    ok &= (pv.GetAnyType().GetTypeDescriptor() == SignedInteger16Bit);

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
    ok &= (pv.GetPVType() == DBR_LONG);
    ok &= (pv.GetCATimeout() == 5.0);
    ok &= (pv.GetAnyType().GetTypeDescriptor() == UnsignedInteger32Bit);
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
    ok &= (pv.GetPVType() == DBR_LONG);
    ok &= (pv.GetCATimeout() == 5.0);
    ok &= (pv.GetAnyType().GetTypeDescriptor() == SignedInteger32Bit);
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
    ok &= (pv.GetCATimeout() == 5.0);
    ok &= (pv.GetAnyType().GetTypeDescriptor() == Float32Bit);
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
    ok &= (pv.GetCATimeout() == 5.0);
    ok &= (pv.GetAnyType().GetTypeDescriptor() == Float64Bit);
    return ok;
}

bool EPICSPVTest::TestInitialise_NoEvent_Array() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONED");
    cdb.Write("PVType", "float64");
    cdb.Write("NumberOfElements", 101);

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "");
    ok &= (pv.GetFunction() == "");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().notSet);
    ok &= (pv.GetPVName() == "PVONED");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_DOUBLE);
    ok &= (pv.GetCATimeout() == 5.0);
    ok &= (pv.GetAnyType().GetTypeDescriptor() == Float64Bit);
    ok &= (pv.GetAnyType().GetNumberOfElements(0u) == 101);
    ok &= (pv.GetAnyType().GetNumberOfDimensions() == 1);
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
    ok &= (pv.GetCATimeout() == 5.0);
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
    ok &= (pv.GetCATimeout() == 3);
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
    cdb.MoveToRoot();

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "AnObject");
    ok &= (pv.GetFunction() == "");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().function);
    ok &= (pv.GetPVName() == "PVONES");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_STRING);
    ok &= (pv.GetCATimeout() == 5.0);
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
    ok &= (pv.GetCATimeout() == 5.0);
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
    ok &= (pv.GetCATimeout() == 5.0);
    return ok;
}

bool EPICSPVTest::TestInitialise_Event_ParameterName() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("PVValue", "ParameterName");
    cdb.Write("Function", "AFunction");
    cdb.MoveToRoot();

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "AnObject");
    ok &= (pv.GetFunction() == "AFunction");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().parameterName);
    ok &= (pv.GetPVName() == "PVONES");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_STRING);
    ok &= (pv.GetCATimeout() == 5.0);
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
    ok &= (pv.GetCATimeout() == 5.0);
    return ok;
}

bool EPICSPVTest::TestInitialise_Event_Message() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Destination", "AnObject");
    cdb.Write("Function", "AFunction");
    cdb.Write("PVValue", "Message");
    cdb.MoveToRoot();

    bool ok = (pv.Initialise(cdb));
    ok &= (pv.GetContext() == NULL);
    ok &= (pv.GetDestination() == "");
    ok &= (pv.GetFunction() == "AFunction");
    ok &= (pv.GetFunctionFromMap("ANYKEY") == "");
    ok &= (pv.GetMode().message);
    ok &= (pv.GetPVName() == "PVONES");
    ok &= (pv.GetPVChid() == 0u);
    ok &= (pv.GetPVType() == DBR_STRING);
    ok &= (pv.GetCATimeout() == 5.0);
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

bool EPICSPVTest::TestInitialise_Event_False_Function_AndFunctionName() {
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

bool EPICSPVTest::TestInitialise_Event_False_NoDestination() {
    using namespace MARTe;
    EPICSPV pv;
    ConfigurationDatabase cdb;
    cdb.Write("PVName", "PVONES");
    cdb.Write("PVType", "string");
    cdb.CreateRelative("Event");
    cdb.Write("Function", "AFunction");
    cdb.Write("PVValue", "Ignore");
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
    ca_client_context *cx = (ca_client_context *) 0xAABBCCDD;
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
    return TestSetPVChid();
}

bool EPICSPVTest::TestSetPVChid() {
    using namespace MARTe;
    EPICSPV pv;
    chid c = (chid) 0xFFFFAABB;
    pv.SetPVChid(c);
    return (pv.GetPVChid() == c);
}

bool EPICSPVTest::TestGetPVEvid() {
    return TestSetPVEvid();
}

bool EPICSPVTest::TestSetPVEvid() {
    using namespace MARTe;
    EPICSPV pv;
    evid c = (evid) 0xFFFFAABB;
    pv.SetPVEvid(c);
    return (pv.GetPVEvid() == c);
}

bool EPICSPVTest::TestGetPVType() {
    return TestInitialise_NoEvent_Int32();
}

bool EPICSPVTest::TestGetMode() {
    return TestInitialise_Event_Function();
}

bool EPICSPVTest::TestGetCATimeout() {
    return TestInitialise_NoEvent_Timeout();
}

bool EPICSPVTest::TestGetDestination() {
    return TestInitialise_Event_Function();
}

bool EPICSPVTest::TestGetFunction() {
    return TestInitialise_Event_Parameter();
}

bool EPICSPVTest::TestGetFunctionFromMap() {
    return TestInitialise_Event_FunctionMap();
}

bool EPICSPVTest::TestHandlePVEvent_Function_NoParameter() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = string"
            "    Event = {"
            "        PVValue = Function"
            "        Destination = AnObject"
            "    }"
            "}"
            "+AnObject = {"
            "    Class = EPICSPVTestHelper"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVTestHelper> anObject;
    ReferenceT<EPICSPV> aPV;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        anObject = ord->Find("AnObject");
        ok = anObject.IsValid();
    }
    if (ok) {
        const char8 * const value = "HandleNoParameter";
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(value);
        args.count = 1;
        aPV->HandlePVEvent(args);
        //Call twice to trigger change
        aPV->HandlePVEvent(args);
        ok = (anObject->noParameterFunctionCalled);
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_NoParameter_False() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = string"
            "    Event = {"
            "        PVValue = Function"
            "        Destination = AnObjecta"
            "    }"
            "}"
            "+AnObject = {"
            "    Class = EPICSPVTestHelper"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVTestHelper> anObject;
    ReferenceT<EPICSPV> aPV;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        anObject = ord->Find("AnObject");
        ok = anObject.IsValid();
    }
    if (ok) {
        const char8 * const value = "HandleNoParameter";
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(value);
        args.count = 1;
        aPV->HandlePVEvent(args);
        //Call twice to trigger change
        aPV->HandlePVEvent(args);
        ok = !(anObject->noParameterFunctionCalled);
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_FunctionMap() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = uint32"
            "    Event = {"
            "        PVValue = Function"
            "        Destination = AnObject"
            "        FunctionMap = {{\"7\", \"HandleNoParameter\"}}"
            "    }"
            "}"
            "+AnObject = {"
            "    Class = EPICSPVTestHelper"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVTestHelper> anObject;
    ReferenceT<EPICSPV> aPV;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        anObject = ord->Find("AnObject");
        ok = anObject.IsValid();
    }
    if (ok) {
        uint32 value = 7;
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(&value);
        args.count = 1;
        aPV->HandlePVEvent(args);
        //Call twice to trigger change
        aPV->HandlePVEvent(args);
        ok = (anObject->noParameterFunctionCalled);
    }
    ord->Purge();
    return ok;

}

bool EPICSPVTest::TestHandlePVEvent_FunctionMap_NoKey() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = uint32"
            "    Event = {"
            "        PVValue = Function"
            "        Destination = AnObject"
            "        FunctionMap = {{\"7\", \"HandleNoParameter\"}}"
            "    }"
            "}"
            "+AnObject = {"
            "    Class = EPICSPVTestHelper"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVTestHelper> anObject;
    ReferenceT<EPICSPV> aPV;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        anObject = ord->Find("AnObject");
        ok = anObject.IsValid();
    }
    if (ok) {
        uint32 value = 6;
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(&value);
        args.count = 1;
        aPV->HandlePVEvent(args);
        //Call twice to trigger change
        aPV->HandlePVEvent(args);
        ok = !(anObject->noParameterFunctionCalled);
    }
    ord->Purge();
    return ok;

}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_Int16() {
    using namespace MARTe;
    int16 value = -7;
    bool ok = TestHandlePVEvent_Function_Parameter(value);
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_UInt16() {
    using namespace MARTe;
    uint16 value = 7;
    bool ok = TestHandlePVEvent_Function_Parameter(value);
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_Int32() {
    using namespace MARTe;
    int32 value = -7;
    bool ok = TestHandlePVEvent_Function_Parameter(value);
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_UInt32() {
    using namespace MARTe;
    uint32 value = 7;
    bool ok = TestHandlePVEvent_Function_Parameter(value);
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_Float32() {
    using namespace MARTe;
    float32 value = -7;
    bool ok = TestHandlePVEvent_Function_Parameter(value);
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_Float64() {
    using namespace MARTe;
    float64 value = -7;
    bool ok = TestHandlePVEvent_Function_Parameter(value);
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_String() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = string"
            "    Event = {"
            "        PVValue = Parameter"
            "        Destination = AnObject"
            "        Function = Handle_string"
            "    }"
            "}"
            "+AnObject = {"
            "    Class = EPICSPVTestHelper"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVTestHelper> anObject;
    ReferenceT<EPICSPV> aPV;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        anObject = ord->Find("AnObject");
        ok = anObject.IsValid();
    }
    if (ok) {
        StreamString value = "OK!";
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(value.Buffer());
        args.count = 1;
        aPV->HandlePVEvent(args);
        //Call twice to trigger change
        aPV->HandlePVEvent(args);
        ok = (anObject->stringValue == value);
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_Array() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = int32"
            "    NumberOfElements = 10"
            "    Event = {"
            "        PVValue = Parameter"
            "        Destination = AnObject"
            "        Function = Handle_vec"
            "    }"
            "}"
            "+AnObject = {"
            "    Class = EPICSPVTestHelper"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVTestHelper> anObject;
    ReferenceT<EPICSPV> aPV;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        anObject = ord->Find("AnObject");
        ok = anObject.IsValid();
    }
    if (ok) {
        int32 *vec = new int32[10u];
        uint32 n;
        for (n = 0u; n < 10; n++) {
            vec[n] = n;
        }
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(&vec[0]);
        args.count = 10;
        aPV->HandlePVEvent(args);
        //Call twice to trigger change
        aPV->HandlePVEvent(args);
        for (n = 0u; (n < 10) && (ok); n++) {
            ok = (anObject->int32Arr[n] == vec[n]);
        }
        delete vec;
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_String_Array() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = string"
            "    NumberOfElements = 10"
            "    Event = {"
            "        PVValue = Parameter"
            "        Destination = AnObject"
            "        Function = Handle_str_vec"
            "    }"
            "}"
            "+AnObject = {"
            "    Class = EPICSPVTestHelper"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVTestHelper> anObject;
    ReferenceT<EPICSPV> aPV;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        anObject = ord->Find("AnObject");
        ok = anObject.IsValid();
    }
    if (ok) {
        char8 *vec = new char8[40u * 10u];
        uint32 n;
        for (n = 0u; n < 10; n++) {
            anObject->strArr[n] = "";
            vec[n * 40u] = ('0' + n);
            vec[n * 40u + 1] = '\0';
        }
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(&vec[0]);
        args.count = 10;
        aPV->HandlePVEvent(args);

        //Call twice to trigger change
        for (n = 0u; n < 10; n++) {
            anObject->strArr[n] = "";
        }
        //Call twice to trigger change
        aPV->HandlePVEvent(args);
        for (n = 0u; (n < 10) && (ok); n++) {
            StreamString test;
            test.Printf("%d", n);
            ok = (anObject->strArr[n] == test);
        }
        delete vec;
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_ParameterName_Int() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = int32"
            "    Event = {"
            "        PVValue = ParameterName"
            "        Destination = AnObject"
            "        Function = HandleInt32ParameterName"
            "    }"
            "}"
            "+AnObject = {"
            "    Class = EPICSPVTestHelper"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVTestHelper> anObject;
    ReferenceT<EPICSPV> aPV;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        anObject = ord->Find("AnObject");
        ok = anObject.IsValid();
    }
    if (ok) {
        int32 value = 7;
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(&value);
        args.count = 1;
        aPV->HandlePVEvent(args);
        //Call twice to trigger change
        aPV->HandlePVEvent(args);
        ok = (anObject->int32Value == value);
        if (ok) {
            ok = (anObject->parameterName == "PV_1");
        }
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Ignore() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = int32"
            "    Event = {"
            "        PVValue = Ignore"
            "        Destination = AnObject"
            "        Function = HandleNoParameter"
            "    }"
            "}"
            "+AnObject = {"
            "    Class = EPICSPVTestHelper"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVTestHelper> anObject;
    ReferenceT<EPICSPV> aPV;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        anObject = ord->Find("AnObject");
        ok = anObject.IsValid();
    }
    if (ok) {
        int32 value = 7;
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(&value);
        args.count = 1;
        aPV->HandlePVEvent(args);
        //Call twice to trigger change
        aPV->HandlePVEvent(args);
        ok = (anObject->noParameterFunctionCalled);
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Message() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = int32"
            "    Event = {"
            "        PVValue = Message"
            "    }"
            "    +AMessage = {"
            "        Class = Message"
            "        Destination = AnObject"
            "        Function = HandleMessage"
            "        +Parameters = {"
            "            Class = ConfigurationDatabase"
            "            param1 = APARAM"
            "            param2 = 777"
            "        }"
            "    }"
            "}"
            "+AnObject = {"
            "    Class = EPICSPVTestHelper"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVTestHelper> anObject;
    ReferenceT<EPICSPV> aPV;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        anObject = ord->Find("AnObject");
        ok = anObject.IsValid();
    }
    if (ok) {
        int32 value = 7;
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(&value);
        args.count = 1;
        aPV->HandlePVEvent(args);
        //Call twice to trigger change
        aPV->HandlePVEvent(args);
        ok = (anObject->messageReceived);
        if (ok) {
            ok = (anObject->parameterName == "APARAM");
        }
        if (ok) {
            ok = (anObject->int32Value == 777);
        }
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Message_PVName() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = int32"
            "    Event = {"
            "        PVValue = Message"
            "    }"
            "    +AMessage = {"
            "        Class = Message"
            "        Destination = AnObject"
            "        Function = HandleMessage"
            "        +Parameters = {"
            "            Class = ConfigurationDatabase"
            "            param1 = $PVName"
            "            param2 = 777"
            "        }"
            "    }"
            "}"
            "+AnObject = {"
            "    Class = EPICSPVTestHelper"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVTestHelper> anObject;
    ReferenceT<EPICSPV> aPV;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        anObject = ord->Find("AnObject");
        ok = anObject.IsValid();
    }
    if (ok) {
        int32 value = 7;
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(&value);
        args.count = 1;
        aPV->HandlePVEvent(args);
        //Call twice to trigger change
        aPV->HandlePVEvent(args);
        ok = (anObject->messageReceived);
        if (ok) {
            ok = (anObject->parameterName == "PV_1");
        }
        if (ok) {
            ok = (anObject->int32Value == 777);
        }
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Message_PVValue() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = int32"
            "    Event = {"
            "        PVValue = Message"
            "    }"
            "    +AMessage = {"
            "        Class = Message"
            "        Destination = AnObject"
            "        Function = HandleMessage"
            "        +Parameters = {"
            "            Class = ConfigurationDatabase"
            "            param1 = APARAM"
            "            param2 = $PVValue"
            "        }"
            "    }"
            "}"
            "+AnObject = {"
            "    Class = EPICSPVTestHelper"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPVTestHelper> anObject;
    ReferenceT<EPICSPV> aPV;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        anObject = ord->Find("AnObject");
        ok = anObject.IsValid();
    }
    if (ok) {
        int32 value = 7;
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(&value);
        args.count = 1;
        aPV->HandlePVEvent(args);
        //Call twice more to trigger change
        aPV->HandlePVEvent(args);
        aPV->HandlePVEvent(args);
        ok = (anObject->messageReceived);
        if (ok) {
            ok = (anObject->parameterName == "APARAM");
        }
        if (ok) {
            ok = (anObject->int32Value == value);
        }
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestCAPut_Int16() {
    using namespace MARTe;
    int16 testValue = -7;
    bool ok = TestCAPutT(testValue);
    return ok;
}

bool EPICSPVTest::TestCAPut_UInt16() {
    using namespace MARTe;
    uint16 testValue = 7;
    bool ok = TestCAPutT(testValue);
    return ok;
}

bool EPICSPVTest::TestCAPut_Int32() {
    using namespace MARTe;
    int32 testValue = -7;
    bool ok = TestCAPutT(testValue);
    return ok;
}

bool EPICSPVTest::TestCAPut_UInt32() {
    using namespace MARTe;
    uint32 testValue = 7;
    bool ok = TestCAPutT(testValue);
    return ok;
}

bool EPICSPVTest::TestCAPut_Float32() {
    using namespace MARTe;
    float32 testValue = -7;
    bool ok = TestCAPutT(testValue);
    return ok;
}

bool EPICSPVTest::TestCAPut_Float64() {
    using namespace MARTe;
    float64 testValue = -7;
    bool ok = TestCAPutT(testValue);
    return ok;
}

bool EPICSPVTest::TestCAPut_String() {
    using namespace MARTe;
    StreamString testValue = "OK!!!";
    bool ok = TestCAPutT(testValue);
    return ok;
}

bool EPICSPVTest::TestCAPut_String_Arr() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+EPICSCA = {"
            "    Class = EPICSCAClient"
            "    +PV_1 = {"
            "        Class = EPICSPV"
            "        PVName = MARTe2::EPICSCA::Test::stringArr"
            "        PVType = string"
            "        NumberOfElements = 10"
            "    }"
            "}"
            "+TestCAPut = {"
            "    Class = EPICSPVTestCAPut"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPV> aPV;
    ReferenceT<EPICSPVTestCAPut> testCAPut;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("EPICSCA.PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        testCAPut = ord->Find("TestCAPut");
        ok = testCAPut.IsValid();
    }
    if (ok) {
        uint32 timeoutCounts = 50;
        uint32 i = 0u;
        ok = false;
        while ((i < timeoutCounts) && (!ok)) {
            ok = (testCAPut->TestCAPutStringArr() == ErrorManagement::NoError);
            if (!ok) {
                Sleep::Sec(0.1);
                i++;
            }
        }
    }

    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestCAPut_Int16_Arr() {
    using namespace MARTe;
    int16 initialValue = -7;
    bool ok = TestCAPutArrT(initialValue);
    return ok;
}

bool EPICSPVTest::TestCAPut_UInt16_Arr() {
    using namespace MARTe;
    uint16 initialValue = 7;
    bool ok = TestCAPutArrT(initialValue);
    return ok;
}

bool EPICSPVTest::TestCAPut_Int32_Arr() {
    using namespace MARTe;
    int32 initialValue = -7;
    bool ok = TestCAPutArrT(initialValue);
    return ok;
}

bool EPICSPVTest::TestCAPut_UInt32_Arr() {
    using namespace MARTe;
    uint32 initialValue = 7;
    bool ok = TestCAPutArrT(initialValue);
    return ok;
}

bool EPICSPVTest::TestCAPut_Float32_Arr() {
    using namespace MARTe;
    float32 initialValue = -7;
    bool ok = TestCAPutArrT(initialValue);
    return ok;
}

bool EPICSPVTest::TestCAPut_Float64_Arr() {
    using namespace MARTe;
    float64 initialValue = -7;
    bool ok = TestCAPutArrT(initialValue);
    return ok;
}

bool EPICSPVTest::TestCAPutRaw() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+EPICSCA = {"
            "    Class = EPICSCAClient"
            "    +PV_1 = {"
            "        Class = EPICSPV"
            "        PVType = uint32"
            "        PVName = MARTe2::EPICSCA::Test::uint32"
            "    }"
            "}"
            "+TestCAPut = {"
            "    Class = EPICSPVTestCAPut"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    ReferenceT<EPICSPV> aPV;
    ReferenceT<EPICSPVTestCAPut> testCAPut;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        aPV = ord->Find("EPICSCA.PV_1");
        ok = aPV.IsValid();
    }
    if (ok) {
        testCAPut = ord->Find("TestCAPut");
        ok = testCAPut.IsValid();
    }
    if (ok) {
        uint32 timeoutCounts = 50;
        uint32 i = 0u;
        uint32 testValue = 87u;
        ok = false;
        while ((i < timeoutCounts) && (!ok)) {
            ok = (testCAPut->TestCAPutRaw(testValue) == ErrorManagement::NoError);
            if (!ok) {
                Sleep::Sec(0.1);
                i++;
            }
        }
    }

    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestGetAnyType() {
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
    ok &= (pv.GetCATimeout() == 5.0);
    ok &= (pv.GetAnyType().GetTypeDescriptor() == Float64Bit);
    return ok;
}

bool EPICSPVTest::TestCAGet() {
    return TestCAPut_String();
}
