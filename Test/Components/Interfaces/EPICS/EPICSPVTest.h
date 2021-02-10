/**
 * @file EPICSPVTest.h
 * @brief Header file for class EPICSPVTest
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
 * @details This header file contains the declaration of the class EPICSPVTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVTEST_H_
#define EPICSPVTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "CLASSMETHODREGISTER.h"
#include "ConfigurationDatabase.h"
#include "EPICSPV.h"
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests the EPICSPV public methods.
 */
class EPICSPVTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method with no event and uint16 PVType.
     */
    bool TestInitialise_NoEvent_UInt16();

    /**
     * @brief Tests the Initialise method with no event and int16 PVType.
     */
    bool TestInitialise_NoEvent_Int16();

    /**
     * @brief Tests the Initialise method with no event and uint32 PVType.
     */
    bool TestInitialise_NoEvent_UInt32();

    /**
     * @brief Tests the Initialise method with no event and int32 PVType.
     */
    bool TestInitialise_NoEvent_Int32();

    /**
     * @brief Tests the Initialise method with no event and float32 PVType.
     */
    bool TestInitialise_NoEvent_Float32();

    /**
     * @brief Tests the Initialise method with no event and float64 PVType.
     */
    bool TestInitialise_NoEvent_Float64();

    /**
     * @brief Tests the Initialise method with no event for an array.
     */
    bool TestInitialise_NoEvent_Array();

    /**
     * @brief Tests the Initialise method with no event and string PVType.
     */
    bool TestInitialise_NoEvent_String();

    /**
     * @brief Tests the Initialise method with no event and with a non-default timeout.
     */
    bool TestInitialise_NoEvent_Timeout();

    /**
     * @brief Tests the Initialise method without specifying the PV name.
     */
    bool TestInitialise_False_PVName();

    /**
     * @brief Tests the Initialise method without specifying the PV type.
     */
    bool TestInitialise_False_PVType();

    /**
     * @brief Tests the Initialise method specifying an invalid PV type.
     */
    bool TestInitialise_False_PVType_Invalid();

    /**
     * @brief Tests the Initialise method specifying a zero timeout.
     */
    bool TestInitialise_False_ZeroTimeout();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Function.
     */
    bool TestInitialise_Event_Function();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=FunctionMap.
     */
    bool TestInitialise_Event_FunctionMap();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Parameter.
     */
    bool TestInitialise_Event_Parameter();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Parameter.
     */
    bool TestInitialise_Event_ParameterName();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Ignore.
     */
    bool TestInitialise_Event_Ignore();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Message.
     */
    bool TestInitialise_Event_Message();

    /**
     * @brief Tests the Initialise method with an Event and without specifying an invalid PVValue.
     */
    bool TestInitialise_Event_False_PVValue();

    /**
     * @brief Tests the Initialise method with an Event and specifying an invalid PVValue.
     */
    bool TestInitialise_Event_False_PVValue_Invalid();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Function and specifying the function.
     */
    bool TestInitialise_Event_False_Function_AndFunctionName();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Parameter and without specifying the function.
     */
    bool TestInitialise_Event_False_Parameter_NoFunctionName();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Ignore and without specifying the function.
     */
    bool TestInitialise_Event_False_Ignore_NoFunctionName();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Function specifying Function and a FunctionMap.
     */
    bool TestInitialise_Event_False_Function_And_FunctionMap();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Function specifying a FunctionMap that is not a matrix.
     */
    bool TestInitialise_Event_False_FunctionMap_NotMatrix();

    /**
     * @brief Tests the Initialise method with an Event and PVValue=Function specifying a FunctionMap that is a matrix with more than two columns.
     */
    bool TestInitialise_Event_False_FunctionMap_MatrixNotTwoColumns();

    /**
     * @brief Tests the Initialise method with an Event and PVValue!=Function specifying FunctionMap.
     */
    bool TestInitialise_Event_False_NotFunction_And_FunctionMap();

    /**
     * @brief Tests the Initialise method with an Event without specifying the Destination.
     */
    bool TestInitialise_Event_False_NoDestination();

    /**
     * @brief Tests the SetContext method.
     */
    bool TestSetContext();

    /**
     * @brief Tests the GetContext method.
     */
    bool TestGetContext();

    /**
     * @brief Tests the HandlePVEvent method calling a function with no parameter.
     */
    bool TestHandlePVEvent_Function_NoParameter();

    /**
     * @brief Tests the HandlePVEvent method calling a function with no parameter to a destination that does not exits.
     */
    bool TestHandlePVEvent_Function_NoParameter_False();

    /**
     * @brief Tests the HandlePVEvent method calling a function with a function map.
     */
    bool TestHandlePVEvent_FunctionMap();

    /**
     * @brief Tests the HandlePVEvent method calling a function with a function map with a key that does not exist.
     */
    bool TestHandlePVEvent_FunctionMap_NoKey();

    /**
     * @brief Tests the HandlePVEvent method calling a function with an int16 parameter.
     */
    bool TestHandlePVEvent_Function_Parameter_Int16();

    /**
     * @brief Tests the HandlePVEvent method calling a function with an uint16 parameter.
     */
    bool TestHandlePVEvent_Function_Parameter_UInt16();

    /**
     * @brief Tests the HandlePVEvent method calling a function with an int32 parameter.
     */
    bool TestHandlePVEvent_Function_Parameter_Int32();

    /**
     * @brief Tests the HandlePVEvent method calling a function with an uint32 parameter.
     */
    bool TestHandlePVEvent_Function_Parameter_UInt32();

    /**
     * @brief Tests the HandlePVEvent method calling a function with a float32 parameter.
     */
    bool TestHandlePVEvent_Function_Parameter_Float32();

    /**
     * @brief Tests the HandlePVEvent method calling a function with a float64 parameter.
     */
    bool TestHandlePVEvent_Function_Parameter_Float64();

    /**
     * @brief Tests the HandlePVEvent method calling a function with a string parameter.
     */
    bool TestHandlePVEvent_Function_Parameter_String();

    /**
     * @brief Tests the HandlePVEvent method calling a function with an array.
     */
    bool TestHandlePVEvent_Function_Parameter_Array();

    /**
     * @brief Tests the HandlePVEvent method calling a function with a string array.
     */
    bool TestHandlePVEvent_Function_Parameter_String_Array();

    /**
     * @brief Tests the HandlePVEvent method calling a function with an int parameter.
     */
    bool TestHandlePVEvent_Function_ParameterName_Int();

    /**
     * @brief Tests the HandlePVEvent method calling a function in Ignore mode.
     */
    bool TestHandlePVEvent_Function_Ignore();

    /**
     * @brief Tests the HandlePVEvent method calling a function in Message mode.
     */
    bool TestHandlePVEvent_Function_Message();

    /**
     * @brief Tests the HandlePVEvent method calling a function in Message mode, replacing PVName.
     */
    bool TestHandlePVEvent_Function_Message_PVName();

    /**
     * @brief Tests the HandlePVEvent method calling a function in Message mode, replacing PVValue.
     */
    bool TestHandlePVEvent_Function_Message_PVValue();

    /**
     * @brief Tests the GetPVName method.
     */
    bool TestGetPVName();

    /**
     * @brief Tests the GetPVChid method.
     */
    bool TestGetPVChid();

    /**
     * @brief Tests the SetPVChid method.
     */
    bool TestSetPVChid();

    /**
     * @brief Tests the GetPVEvid method.
     */
    bool TestGetPVEvid();

    /**
     * @brief Tests the SetPVEvid method.
     */
    bool TestSetPVEvid();

    /**
     * @brief Tests the GetPVType method.
     */
    bool TestGetPVType();

    /**
     * @brief Tests the GetMode method.
     */
    bool TestGetMode();

    /**
     * @brief Tests the GetCATimeout method.
     */
    bool TestGetCATimeout();

    /**
     * @brief Tests the GetDestination method.
     */
    bool TestGetDestination();

    /**
     * @brief Tests the GetFunction method.
     */
    bool TestGetFunction();

    /**
     * @brief Tests the GetFunctionFromMap method.
     */
    bool TestGetFunctionFromMap();

    /**
     * @brief Tests the CAPut (and CAGet) method with an int16.
     */
    bool TestCAPut_Int16();

    /**
     * @brief Tests the CAPut (and CAGet) method with an uint16.
     */
    bool TestCAPut_UInt16();

    /**
     * @brief Tests the CAPut (and CAGet) method with an int32.
     */
    bool TestCAPut_Int32();

    /**
     * @brief Tests the CAPut (and CAGet) method with an uint32.
     */
    bool TestCAPut_UInt32();

    /**
     * @brief Tests the CAPut (and CAGet) method with a float32.
     */
    bool TestCAPut_Float32();

    /**
     * @brief Tests the CAPut (and CAGet) method with a float64.
     */
    bool TestCAPut_Float64();

    /**
     * @brief Tests the CAPut (and CAGet) method with a string.
     */
    bool TestCAPut_String();

    /**
     * @brief Tests the CAPut (and CAGet) method with a string array.
     */
    bool TestCAPut_String_Arr();

    /**
     * @brief Tests the CAPut (and CAGet) method with an int16 array.
     */
    bool TestCAPut_Int16_Arr();

    /**
     * @brief Tests the CAPut (and CAGet) method with an uint16 array.
     */
    bool TestCAPut_UInt16_Arr();

    /**
     * @brief Tests the CAPut (and CAGet) method with an int32 array.
     */
    bool TestCAPut_Int32_Arr();

    /**
     * @brief Tests the CAPut (and CAGet) method with an uint32 array.
     */
    bool TestCAPut_UInt32_Arr();

    /**
     * @brief Tests the CAPut (and CAGet) method with a float32 array.
     */
    bool TestCAPut_Float32_Arr();

    /**
     * @brief Tests the CAPut (and CAGet) method with a float64 array.
     */
    bool TestCAPut_Float64_Arr();

    /**
     * @brief Tests the CAGet method.
     */
    bool TestCAGet();

    /**
     * @brief Tests the CAPutRaw method.
     */
    bool TestCAPutRaw();

    /**
     * @brief Tests the GetAnyType method.
     */
    bool TestGetAnyType();

private:
    template<typename T>
    bool TestHandlePVEvent_Function_Parameter(T testValue);

    template<typename T>
    bool TestCAPutT(T testValue);

    template<typename T>
    bool TestCAPutArrT(T testValue);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 * Helper class to test the CAPut
 */
class EPICSPVTestCAPut: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()EPICSPVTestCAPut() : Object(), MessageI() {
        using namespace MARTe;
    }

    template<typename T>
    MARTe::ErrorManagement::ErrorType TestCAPut(T testValue) {
        using namespace MARTe;
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
                T replyValue;
                replyCDB->Read("param1", replyValue);
                ok = (replyValue == testValue);
            }
        }
        return ok ? MARTe::ErrorManagement::NoError : MARTe::ErrorManagement::FatalError;
    }

    template<typename T>
    MARTe::ErrorManagement::ErrorType TestCAPutRaw(T testValue) {
        using namespace MARTe;
        ReferenceT<EPICSPV> epicsPV = ObjectRegistryDatabase::Instance()->Find("EPICSCA.PV_1");
        bool ok = epicsPV.IsValid();
        if (ok) {
            AnyType at = epicsPV->GetAnyType();
            *static_cast<T *>(at.GetDataPointer()) = testValue;
            epicsPV->CAPutRaw();
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
                T replyValue;
                replyCDB->Read("param1", replyValue);
                ok = (replyValue == testValue);
            }
        }
        return ok ? MARTe::ErrorManagement::NoError : MARTe::ErrorManagement::FatalError;
    }

    MARTe::ErrorManagement::ErrorType TestCAPutStringArr() {
        using namespace MARTe;
        uint32 nElements = 10u;
        StreamString *testValue = new StreamString[nElements];
        uint32 n;
        for (n=0u; n<nElements; n++) {
            testValue[n].Printf("%d", n);
        }
        ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase cdbMsg;
        cdbMsg.Write("Destination", "EPICSCA.PV_1");
        cdbMsg.Write("Function", "CAPut");
        cdbMsg.CreateRelative("+Parameter");
        cdbMsg.Write("Class", "ConfigurationDatabase");
        Vector<StreamString> vecTestValue(testValue, nElements);
        cdbMsg.Write("param1", vecTestValue);
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
                Vector<StreamString> vecReplyValue(nElements);
                ok = replyCDB->Read("param1", vecReplyValue);
                if (ok) {
                    for (n=0u; (n<nElements) && (ok); n++) {
                        StreamString testToken;
                        testToken.Printf("%d", n);
                        testToken.Seek(0LLU);
                        REPORT_ERROR(ErrorManagement::Information, "%s", testToken.Buffer());
                        ok = (testToken == vecReplyValue[n].Buffer());
                    }
                }
            }
        }
        delete [] testValue;
        return ok ? MARTe::ErrorManagement::NoError : MARTe::ErrorManagement::FatalError;
    }

    template<typename T>
    MARTe::ErrorManagement::ErrorType TestCAPutArrT(T startValue) {
        using namespace MARTe;
        uint32 nElements = 10u;
        T *testValue = new T [nElements];
        uint32 n;
        for (n=0u; n<nElements; n++) {
            testValue[n] = n + startValue;
        }
        ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase cdbMsg;
        cdbMsg.Write("Destination", "EPICSCA.PV_1");
        cdbMsg.Write("Function", "CAPut");
        cdbMsg.CreateRelative("+Parameter");
        cdbMsg.Write("Class", "ConfigurationDatabase");
        Vector<T> vecTestValue(testValue, nElements);
        cdbMsg.Write("param1", vecTestValue);
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
                Vector<T> vecReplyValue(nElements);
                ok = replyCDB->Read("param1", vecReplyValue);
                for (n=0u; (n<nElements) && (ok); n++) {
                    ok = (vecReplyValue[n] == testValue[n]);
                }
            }
        }
        delete [] testValue;
        return ok ? MARTe::ErrorManagement::NoError : MARTe::ErrorManagement::FatalError;
    }

};

template<typename T>
bool EPICSPVTest::TestCAPutT(T testValue) {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+EPICSCA = {"
            "    Class = EPICSCAClient"
            "    +PV_1 = {"
            "        Class = EPICSPV"
            "    }"
            "}"
            "+TestCAPut = {"
            "    Class = EPICSPVTestCAPut"
            "}";

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveAbsolute("+EPICSCA.+PV_1");
    AnyType at(testValue);
    StreamString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(at.GetTypeDescriptor());
    if (at.GetTypeDescriptor().type == SString) {
        typeName = "string";
    }
    cdb.Write("PVType", typeName.Buffer());
    StreamString pvName;
    pvName.Printf("MARTe2::EPICSCA::Test::%s", typeName.Buffer());
    cdb.Write("PVName", pvName.Buffer());
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
            ok = (testCAPut->TestCAPut(testValue) == ErrorManagement::NoError);
            if (!ok) {
                Sleep::Sec(0.1);
                i++;
            }
        }
    }

    ord->Purge();
    return ok;
}

template<typename T>
bool EPICSPVTest::TestCAPutArrT(T initialValue) {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+EPICSCA = {"
            "    Class = EPICSCAClient"
            "    +PV_1 = {"
            "        Class = EPICSPV"
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
    AnyType at(initialValue);
    cdb.MoveAbsolute("+EPICSCA.+PV_1");
    StreamString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(at.GetTypeDescriptor());
    if (at.GetTypeDescriptor().type == SString) {
        typeName = "string";
    }
    cdb.Write("PVType", typeName.Buffer());
    StreamString pvName;
    pvName.Printf("MARTe2::EPICSCA::Test::%sArr", TypeDescriptor::GetTypeNameFromTypeDescriptor(at.GetTypeDescriptor()));
    cdb.Write("PVName", pvName.Buffer());
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
            ok = (testCAPut->TestCAPutArrT(initialValue) == ErrorManagement::NoError);
            if (!ok) {
                Sleep::Sec(0.1);
                i++;
            }
        }
    }

    ord->Purge();
    return ok;
}

/**
 * Helper class that reacts to messages received from the EPICSPV
 */
class EPICSPVTestHelper: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()EPICSPVTestHelper() : Object(), MessageI() {
        using namespace MARTe;
        uint16Value = 0u;
        int16Value = 0;
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
        messageReceived = false;
        int32Arr = new int32[10];
        strArr = new StreamString[10];
    }

    virtual ~EPICSPVTestHelper() {
        delete [] int32Arr;
        delete [] strArr;
    }

    MARTe::ErrorManagement::ErrorType HandleMessage(MARTe::StreamString parameterNameIn, const MARTe::int32 newValue) {
        using namespace MARTe;
        messageReceived = true;
        parameterName = parameterNameIn;
        int32Value = newValue;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType HandleNoParameter() {
        noParameterFunctionCalled = true;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Handle_uint16(const MARTe::uint16 newValue) {
        uint16Value = newValue;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Handle_int16(const MARTe::int16 newValue) {
        int16Value = newValue;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Handle_uint32(const MARTe::uint32 newValue) {
        uint32Value = newValue;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Handle_int32(const MARTe::int32 newValue) {
        int32Value = newValue;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Handle_float32(const MARTe::float32 newValue) {
        float32Value = newValue;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Handle_float64(const MARTe::float64 newValue) {
        float64Value = newValue;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Handle_string(MARTe::StreamString newValue) {
        stringValue = newValue;
        stringValue.Seek(0);
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType HandleInt32ParameterName(MARTe::StreamString parameterNameIn, const MARTe::int32 newValue) {
        int32Value = newValue;
        parameterName = parameterNameIn;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Handle_vec(MARTe::StructuredDataI &newValue) {
        using namespace MARTe;
        Vector<int32> readArr(int32Arr, 10u);
        newValue.Read("param1", readArr);
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Handle_str_vec(MARTe::StructuredDataI &newValue) {
        using namespace MARTe;
        Vector<StreamString> readArr(strArr, 10u);
        newValue.Read("param1", readArr);
        return MARTe::ErrorManagement::NoError;
    }

    template<typename T>
    bool TestValue(T testValue) {
        using namespace MARTe;
        bool ok = false;
        AnyType at(testValue);
        if (at.GetTypeDescriptor() == UnsignedInteger16Bit) {
            ok = (static_cast<uint16>(testValue) == uint16Value);
        }
        else if (at.GetTypeDescriptor() == SignedInteger16Bit) {
            ok = (static_cast<int16>(testValue) == int16Value);
        }
        else if (at.GetTypeDescriptor() == UnsignedInteger32Bit) {
            ok = (static_cast<uint32>(testValue) == uint32Value);
        }
        else if (at.GetTypeDescriptor() == SignedInteger32Bit) {
            ok = (static_cast<int32>(testValue) == int32Value);
        }
        else if (at.GetTypeDescriptor() == Float32Bit) {
            ok = (static_cast<float32>(testValue) == float32Value);
        }
        else if (at.GetTypeDescriptor() == Float64Bit) {
            ok = (static_cast<float64>(testValue) == float64Value);
        }
        return ok;
    }

    MARTe::StreamString *strArr;
    MARTe::int32 *int32Arr;
    MARTe::uint16 uint16Value;
    MARTe::int16 int16Value;
    MARTe::uint32 uint32Value;
    MARTe::int32 int32Value;
    MARTe::float32 float32Value;
    MARTe::float64 float64Value;
    MARTe::StreamString stringValue;
    MARTe::StreamString parameterName;
    bool noParameterFunctionCalled;
    bool messageReceived;
};

template<typename T>
bool EPICSPVTest::TestHandlePVEvent_Function_Parameter(T testValue) {
    using namespace MARTe;
    EPICSPV pv;
    StreamString config = ""
            "+PV_1 = {"
            "    Class = EPICSPV"
            "    PVName = PVS::PV1"
            "    Event = {"
            "        PVValue = Parameter"
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
    cdb.MoveAbsolute("+PV_1");
    AnyType at(testValue);
    StreamString typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(at.GetTypeDescriptor());
    if (at.GetTypeDescriptor().type == SString) {
        typeName = "string";
    }
    cdb.Write("PVType", typeName.Buffer());
    cdb.MoveAbsolute("+PV_1.Event");
    StreamString funName;
    funName.Printf("Handle_%s", typeName.Buffer());
    cdb.Write("Function", funName.Buffer());
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
        struct event_handler_args args;
        args.dbr = reinterpret_cast<const void *>(&testValue);
        args.count = 1;
        aPV->HandlePVEvent(args);
        //Call twice to trigger change
        aPV->HandlePVEvent(args);
        ok = (anObject->TestValue(testValue));
    }
    ord->Purge();
    return ok;
}

#endif /* EPICSPVTEST_H_ */

