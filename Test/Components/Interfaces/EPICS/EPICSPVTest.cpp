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
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * Helper class that reacts to messages received from the EPICSPV
 */
class EPICSPVTestHelper: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()EPICSPVTestHelper() : Object(), MessageI() {
        using namespace MARTe;
        uint32Value = 0u;
        int32Value = 0;
        float32Value = 0;
        float64Value = 0;
        stringValue = "";
        noParameterFunctionCalled = false;
        ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetDestination(this);
        ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
        if (!ret.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
        }
    }

    MARTe::ErrorManagement::ErrorType HandleNoParameter() {
        noParameterFunctionCalled = true;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType HandleUInt32(const MARTe::uint32 newValue) {
        uint32Value = newValue;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType HandleInt32(const MARTe::int32 newValue) {
        int32Value = newValue;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType HandleFloat32(const MARTe::float32 newValue) {
        float32Value = newValue;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType HandleFloat64(const MARTe::float64 newValue) {
        float64Value = newValue;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType HandleString(MARTe::StreamString newValue) {
        stringValue = newValue;
        stringValue.Seek(0);
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType HandleInt32ParameterName(MARTe::StreamString parameterNameIn, const MARTe::int32 newValue) {
        int32Value = newValue;
        parameterName = parameterNameIn;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::uint32 uint32Value;
    MARTe::int32 int32Value;
    MARTe::float32 float32Value;
    MARTe::float64 float64Value;
    MARTe::StreamString stringValue;
    MARTe::StreamString parameterName;
    bool noParameterFunctionCalled;
};
CLASS_REGISTER(EPICSPVTestHelper, "1.0")
CLASS_METHOD_REGISTER(EPICSPVTestHelper, HandleNoParameter)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, HandleInt32)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, HandleUInt32)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, HandleFloat32)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, HandleFloat64)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, HandleString)
CLASS_METHOD_REGISTER(EPICSPVTestHelper, HandleInt32ParameterName)

/**
 * Helper class to test the CAPut
 */
class EPICSPVTestCAPut: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()EPICSPVTestCAPut() : Object(), MessageI() {
        using namespace MARTe;
    }

    MARTe::ErrorManagement::ErrorType TestCAPutUInt32() {
        using namespace MARTe;
        uint32 testValue = 7u;
        ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase cdbMsg;
        cdbMsg.Write("Destination", "EPICSCA.PV_1");
        cdbMsg.Write("Function", "CAPut");
        cdbMsg.CreateRelative("+Parameter");
        cdbMsg.Write("Class", "ConfigurationDatabase");
        cdbMsg.Write("param1", testValue);
        cdbMsg.MoveToAncestor(1u);
        bool ok = msg->Initialise(cdbMsg);
        if (ok) {
            ok = (MessageI::SendMessage(msg, this) == ErrorManagement::NoError);
        }
        if (ok) {
            ReferenceT<Message> msgReply(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ConfigurationDatabase cdbMsg;
            cdbMsg.Write("Destination", "EPICSCA.PV_1");
            cdbMsg.Write("Function", "CAGet");
            cdbMsg.Write("Mode", "ExpectsReply");
            cdbMsg.CreateRelative("+Parameter");
            cdbMsg.Write("Class", "ConfigurationDatabase");
            cdbMsg.MoveToAncestor(1u);
            msgReply->Initialise(cdbMsg);
            ok = MessageI::SendMessage(msgReply, this);
            if (ok) {
                ReferenceT<ConfigurationDatabase> replyCDB = msgReply->Get(0);
                uint32 replyValue = 0u;
                replyCDB->Read("param1", replyValue);
                ok = (replyValue == testValue);
            }
        }
        return ok ? MARTe::ErrorManagement::NoError : MARTe::ErrorManagement::FatalError;
    }

    MARTe::ErrorManagement::ErrorType TestCAPutInt32() {
        using namespace MARTe;
        int32 testValue = -7;
        ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase cdbMsg;
        cdbMsg.Write("Destination", "EPICSCA.PV_1");
        cdbMsg.Write("Function", "CAPut");
        cdbMsg.CreateRelative("+Parameter");
        cdbMsg.Write("Class", "ConfigurationDatabase");
        cdbMsg.Write("param1", testValue);
        cdbMsg.MoveToAncestor(1u);
        bool ok = msg->Initialise(cdbMsg);
        if (ok) {
            ok = (MessageI::SendMessage(msg, this) == ErrorManagement::NoError);
        }
        if (ok) {
            ReferenceT<Message> msgReply(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ConfigurationDatabase cdbMsg;
            cdbMsg.Write("Destination", "EPICSCA.PV_1");
            cdbMsg.Write("Function", "CAGet");
            cdbMsg.Write("Mode", "ExpectsReply");
            cdbMsg.CreateRelative("+Parameter");
            cdbMsg.Write("Class", "ConfigurationDatabase");
            cdbMsg.MoveToAncestor(1u);
            msgReply->Initialise(cdbMsg);
            ok = MessageI::SendMessage(msgReply, this);
            if (ok) {
                ReferenceT<ConfigurationDatabase> replyCDB = msgReply->Get(0);
                int32 replyValue = 0;
                replyCDB->Read("param1", replyValue);
                ok = (replyValue == testValue);
            }
        }
        return ok ? MARTe::ErrorManagement::NoError : MARTe::ErrorManagement::FatalError;
    }

    MARTe::ErrorManagement::ErrorType TestCAPutFloat32() {
        using namespace MARTe;
        float32 testValue = -8.0;
        ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase cdbMsg;
        cdbMsg.Write("Destination", "EPICSCA.PV_1");
        cdbMsg.Write("Function", "CAPut");
        cdbMsg.CreateRelative("+Parameter");
        cdbMsg.Write("Class", "ConfigurationDatabase");
        cdbMsg.Write("param1", testValue);
        cdbMsg.MoveToAncestor(1u);
        bool ok = msg->Initialise(cdbMsg);
        if (ok) {
            ok = (MessageI::SendMessage(msg, this) == ErrorManagement::NoError);
        }
        if (ok) {
            ReferenceT<Message> msgReply(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ConfigurationDatabase cdbMsg;
            cdbMsg.Write("Destination", "EPICSCA.PV_1");
            cdbMsg.Write("Function", "CAGet");
            cdbMsg.Write("Mode", "ExpectsReply");
            cdbMsg.CreateRelative("+Parameter");
            cdbMsg.Write("Class", "ConfigurationDatabase");
            cdbMsg.MoveToAncestor(1u);
            msgReply->Initialise(cdbMsg);
            ok = MessageI::SendMessage(msgReply, this);
            if (ok) {
                ReferenceT<ConfigurationDatabase> replyCDB = msgReply->Get(0);
                float32 replyValue = 0;
                replyCDB->Read("param1", replyValue);
                ok = (replyValue == testValue);
            }
        }
        return ok ? MARTe::ErrorManagement::NoError : MARTe::ErrorManagement::FatalError;
    }

    MARTe::ErrorManagement::ErrorType TestCAPutFloat64() {
        using namespace MARTe;
        float64 testValue = -9.0;
        ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase cdbMsg;
        cdbMsg.Write("Destination", "EPICSCA.PV_1");
        cdbMsg.Write("Function", "CAPut");
        cdbMsg.CreateRelative("+Parameter");
        cdbMsg.Write("Class", "ConfigurationDatabase");
        cdbMsg.Write("param1", testValue);
        cdbMsg.MoveToAncestor(1u);
        bool ok = msg->Initialise(cdbMsg);
        if (ok) {
            ok = (MessageI::SendMessage(msg, this) == ErrorManagement::NoError);
        }
        if (ok) {
            ReferenceT<Message> msgReply(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ConfigurationDatabase cdbMsg;
            cdbMsg.Write("Destination", "EPICSCA.PV_1");
            cdbMsg.Write("Function", "CAGet");
            cdbMsg.Write("Mode", "ExpectsReply");
            cdbMsg.CreateRelative("+Parameter");
            cdbMsg.Write("Class", "ConfigurationDatabase");
            cdbMsg.MoveToAncestor(1u);
            msgReply->Initialise(cdbMsg);
            ok = MessageI::SendMessage(msgReply, this);
            if (ok) {
                ReferenceT<ConfigurationDatabase> replyCDB = msgReply->Get(0);
                float64 replyValue = 0;
                replyCDB->Read("param1", replyValue);
                ok = (replyValue == testValue);
            }
        }
        return ok ? MARTe::ErrorManagement::NoError : MARTe::ErrorManagement::FatalError;
    }

    MARTe::ErrorManagement::ErrorType TestCAPutString() {
        using namespace MARTe;
        StreamString testValue = "OK!!";
        ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase cdbMsg;
        cdbMsg.Write("Destination", "EPICSCA.PV_1");
        cdbMsg.Write("Function", "CAPut");
        cdbMsg.CreateRelative("+Parameter");
        cdbMsg.Write("Class", "ConfigurationDatabase");
        cdbMsg.Write("param1", testValue);
        cdbMsg.MoveToAncestor(1u);
        bool ok = msg->Initialise(cdbMsg);
        if (ok) {
            ok = (MessageI::SendMessage(msg, this) == ErrorManagement::NoError);
        }
        if (ok) {
            ReferenceT<Message> msgReply(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ConfigurationDatabase cdbMsg;
            cdbMsg.Write("Destination", "EPICSCA.PV_1");
            cdbMsg.Write("Function", "CAGet");
            cdbMsg.Write("Mode", "ExpectsReply");
            cdbMsg.CreateRelative("+Parameter");
            cdbMsg.Write("Class", "ConfigurationDatabase");
            cdbMsg.MoveToAncestor(1u);
            msgReply->Initialise(cdbMsg);
            ok = MessageI::SendMessage(msgReply, this);
            if (ok) {
                ReferenceT<ConfigurationDatabase> replyCDB = msgReply->Get(0);
                StreamString replyValue = 0;
                replyCDB->Read("param1", replyValue);
                ok = (replyValue == testValue);
            }
        }
        return ok ? MARTe::ErrorManagement::NoError : MARTe::ErrorManagement::FatalError;
    }

};
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

bool EPICSPVTest::TestGetTimeout() {
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
        aPV->HandlePVEvent(reinterpret_cast<const void *>(value));
        //Call twice to trigger change
        aPV->HandlePVEvent(reinterpret_cast<const void *>(value));
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
        aPV->HandlePVEvent(reinterpret_cast<const void *>(value));
        //Call twice to trigger change
        aPV->HandlePVEvent(reinterpret_cast<const void *>(value));
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
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        //Call twice to trigger change
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
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
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        //Call twice to trigger change
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        ok = !(anObject->noParameterFunctionCalled);
    }
    ord->Purge();
    return ok;

}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_Int() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = int32"
            "    Event = {"
            "        PVValue = Parameter"
            "        Destination = AnObject"
            "        Function = HandleInt32"
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
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        //Call twice to trigger change
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        ok = (anObject->int32Value == value);
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_UInt() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = uint32"
            "    Event = {"
            "        PVValue = Parameter"
            "        Destination = AnObject"
            "        Function = HandleUInt32"
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
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        //Call twice to trigger change
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        ok = (anObject->uint32Value == value);
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_Float32() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = float32"
            "    Event = {"
            "        PVValue = Parameter"
            "        Destination = AnObject"
            "        Function = HandleFloat32"
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
        float32 value = 8;
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        //Call twice to trigger change
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        ok = (anObject->float32Value == value);
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestHandlePVEvent_Function_Parameter_Float64() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    PVType = float64"
            "    Event = {"
            "        PVValue = Parameter"
            "        Destination = AnObject"
            "        Function = HandleFloat64"
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
        float64 value = 9;
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        //Call twice to trigger change
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        ok = (anObject->float64Value == value);
    }
    ord->Purge();
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
            "        Function = HandleString"
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
        aPV->HandlePVEvent(reinterpret_cast<const void *>(value.Buffer()));
        //Call twice to trigger change
        aPV->HandlePVEvent(reinterpret_cast<const void *>(value.Buffer()));
        ok = (anObject->stringValue == value);
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
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        //Call twice to trigger change
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
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
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        //Call twice to trigger change
        aPV->HandlePVEvent(reinterpret_cast<const void *>(&value));
        ok = (anObject->noParameterFunctionCalled);
    }
    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestCAPut_Int32() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+EPICSCA = {"
            "    Class = EPICSCAClient"
            "    +PV_1 = {"
            "        Class = EPICSPV"
            "        PVName = MARTe2::EPICSCA::Test::Int32"
            "        PVType = int32"
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
            ok = (testCAPut->TestCAPutInt32() == ErrorManagement::NoError);
            if (!ok) {
                Sleep::Sec(0.1);
                i++;
            }
        }
    }

    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestCAPut_UInt32() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+EPICSCA = {"
            "    Class = EPICSCAClient"
            "    +PV_1 = {"
            "        Class = EPICSPV"
            "        PVName = MARTe2::EPICSCA::Test::UInt32"
            "        PVType = uint32"
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
            ok = (testCAPut->TestCAPutUInt32() == ErrorManagement::NoError);
            if (!ok) {
                Sleep::Sec(0.1);
                i++;
            }
        }
    }

    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestCAPut_Float32() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+EPICSCA = {"
            "    Class = EPICSCAClient"
            "    +PV_1 = {"
            "        Class = EPICSPV"
            "        PVName = MARTe2::EPICSCA::Test::Float32"
            "        PVType = float32"
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
            ok = (testCAPut->TestCAPutFloat32() == ErrorManagement::NoError);
            if (!ok) {
                Sleep::Sec(0.1);
                i++;
            }
        }
    }

    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestCAPut_Float64() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+EPICSCA = {"
            "    Class = EPICSCAClient"
            "    +PV_1 = {"
            "        Class = EPICSPV"
            "        PVName = MARTe2::EPICSCA::Test::Float64"
            "        PVType = float64"
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
            ok = (testCAPut->TestCAPutFloat64() == ErrorManagement::NoError);
            if (!ok) {
                Sleep::Sec(0.1);
                i++;
            }
        }
    }

    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestCAPut_String() {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+EPICSCA = {"
            "    Class = EPICSCAClient"
            "    +PV_1 = {"
            "        Class = EPICSPV"
            "        PVName = MARTe2::EPICSCA::Test::String"
            "        PVType = string"
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
            ok = (testCAPut->TestCAPutString() == ErrorManagement::NoError);
            if (!ok) {
                Sleep::Sec(0.1);
                i++;
            }
        }
    }

    ord->Purge();
    return ok;
}

bool EPICSPVTest::TestCAGet() {
    return TestCAPut_String();
}
