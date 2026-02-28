/**
 * @file OPCUADSOutputTest.cpp
 * @brief Source file for class OPCUADSOutputTest
 * @date 15/03/2019
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
 * the class OPCUADSOutputTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "open62541.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "File.h"
#include "ObjectRegistryDatabase.h"
#include "OPCUADSOutputTest.h"
#include "OPCUADSOutput.h"
#include "OPCUAClientWriteTest.h"
#include "MessageI.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class OPCUADSOutputTestReader {
public:
    OPCUADSOutputTestReader() {
        client = NULL_PTR(UA_Client *);
    }

    bool Connect() {
        using namespace MARTe;
        bool connected = false;
        uint32 retries = 5;
        while(retries > 0 && !connected) {
            client = UA_Client_new();
            UA_ClientConfig_setDefault(UA_Client_getConfig(client));

            UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
            connected = (retval == UA_STATUSCODE_GOOD);
            if (!connected) {
                UA_Client_disconnect(client);
                UA_Client_delete(client);
                retries--;
                Sleep::Sec(1.0);
            }
        }
        return connected;
    }

    UA_StatusCode FindChildNodeId(const MARTe::char8 *targetName, UA_NodeId *outNodeId, UA_NodeId parent = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER)) {
        using namespace MARTe;
        UA_BrowseRequest bReq;
        UA_BrowseRequest_init(&bReq);

        bReq.nodesToBrowse = UA_BrowseDescription_new();
        bReq.nodesToBrowseSize = 1;

        bReq.nodesToBrowse[0].nodeId = parent;
        bReq.nodesToBrowse[0].browseDirection = UA_BROWSEDIRECTION_FORWARD;
        bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;

        UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);

        UA_StatusCode rc = UA_STATUSCODE_BADNOTFOUND;
        bool found = false;
        uint32 targetNameLen = StringHelper::Length(targetName);
        for(uint32 i = 0; (i < bResp.resultsSize) && (!found); i++) {
            for(uint32 j = 0; (j < bResp.results[i].referencesSize) && (!found); j++) {
                UA_ReferenceDescription *ref = &bResp.results[i].references[j];
                found = (targetNameLen == ref->browseName.name.length);
                if (found) {
                    found = (StringHelper::CompareN((char8*)ref->browseName.name.data, targetName, ref->browseName.name.length) == 0);
                }
                if (found) {
                    if (UA_NodeId_copy(&ref->nodeId.nodeId, outNodeId) == UA_STATUSCODE_GOOD) {
                        rc = UA_STATUSCODE_GOOD;
                    }
                }
            }
        }

        UA_BrowseResponse_clear(&bResp);
        UA_BrowseRequest_clear(&bReq);
        return rc;
    }

    UA_StatusCode ReadValue(UA_NodeId nodeId, const UA_DataType *expectedType, MARTe::uint32 nElements, UA_Variant *out) {

        UA_Variant_clear(out);

        UA_StatusCode rc = UA_Client_readValueAttribute(client, nodeId, out);
        if(rc == UA_STATUSCODE_GOOD) {
            if (nElements == 1u) {
                if(!UA_Variant_hasScalarType(out, expectedType)) {
                    rc = UA_STATUSCODE_BADTYPEMISMATCH;
                }
            }
            else {
                if(!UA_Variant_hasArrayType(out, expectedType)) {
                    rc = UA_STATUSCODE_BADTYPEMISMATCH;
                }
            }
        }
        if (rc != UA_STATUSCODE_GOOD) {
            UA_Variant_clear(out);
        }

        return rc;
    }

    bool WaitForValue(UA_NodeId nodeId, MARTe::uint8 *expectedValue, const UA_DataType *expectedType, MARTe::uint32 typeLen, MARTe::uint32 nElements, MARTe::uint32 retries = 5) {
        using namespace MARTe;
        UA_Variant v;
        UA_Variant_init(&v);
        UA_StatusCode rc = ReadValue(nodeId, expectedType, nElements, &v);
        bool ok = false;
        while(retries > 0u && !ok) {
            if(rc == UA_STATUSCODE_GOOD) {
                bool valueOk = true;
                if (nElements > 1u) {
                    valueOk = (v.arrayLength == nElements);
                }
                uint8 *readValue = static_cast<uint8 *>(v.data);
                typeLen *= nElements;
                for (uint32 k=0; (k<typeLen) && (valueOk); k++) {
                    valueOk = (readValue[k] == expectedValue[k]);
                    //printf("Checking readValue[%d] == expectedValue[%d]: %d == %d?\n", k, k, readValue[k], expectedValue[k]);
                }
                ok = valueOk;
            }
            if (!ok) {
                printf("Read failed: %s\n", UA_StatusCode_name(rc));
                retries--;
                Sleep::Sec(1.0);
                rc = ReadValue(nodeId, expectedType, nElements, &v);
            }
        }
        UA_Variant_clear(&v);
        return ok;
    }

    bool CheckTimestamp(UA_NodeId nodeId, MARTe::uint64 expectedTimestampValue, bool expectTimestamp) {
        using namespace MARTe;
        UA_ReadRequest request;
        UA_ReadRequest_init(&request);

        request.nodesToRead = UA_ReadValueId_new();
        request.nodesToReadSize = 1;
        request.nodesToRead[0].nodeId = nodeId;
        request.nodesToRead[0].attributeId = UA_ATTRIBUTEID_VALUE;
        request.timestampsToReturn = UA_TIMESTAMPSTORETURN_BOTH;

        UA_ReadResponse response = UA_Client_Service_read(client, request);
        bool ok = (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD);
        if (ok) {
            ok = (response.resultsSize > 0);
        }   
        UA_DataValue *dv = NULL_PTR(UA_DataValue *);
        if (ok) {
            dv = &response.results[0];
        }
        if (ok) {
            if (expectTimestamp) {
                ok = dv->hasSourceTimestamp;
            }
            else {
                if (dv->hasServerTimestamp) {
                    ok = (dv->sourceTimestamp == dv->serverTimestamp);
                }
            }
        }
        if (ok) {
            if (expectTimestamp) {
                ok = dv->hasSourcePicoseconds;
            }
        }
        if (ok) {
            if (expectTimestamp) {
                uint64 readTimestamp100Ns = static_cast<uint64>(dv->sourceTimestamp);
                uint64 readTimestampPicos = static_cast<uint64>(dv->sourcePicoseconds);
                uint64 readTimestamp = readTimestamp100Ns - UA_DATETIME_UNIX_EPOCH;
                readTimestamp *= 100; //back to full ns
                readTimestamp += readTimestampPicos / 100; //stored as multiple of 10 ps
                ok = (readTimestamp == expectedTimestampValue);
            }
        }

        UA_ReadResponse_clear(&response);
        UA_ReadRequest_clear(&request);

        return ok;
    }

    bool WaitForExtObjValue(UA_NodeId nodeId, MARTe::uint8 *expectedValue, MARTe::uint32 valueLen, MARTe::uint32 retries = 5) {
        using namespace MARTe;
        UA_Variant v;
        UA_Variant_init(&v);

        UA_StatusCode rc = ReadValue(nodeId, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT], 1u, &v);
        bool ok = false;
        while(retries > 0u && !ok) {
            if(rc == UA_STATUSCODE_GOOD) {
                bool valueOk = true;
                UA_ExtensionObject *eo = (UA_ExtensionObject*) v.data;
                uint8 *readValue = static_cast<uint8 *>(eo->content.encoded.body.data);
                for (uint32 k=0; (k<valueLen) && (valueOk); k++) {
                    valueOk = (readValue[k] == expectedValue[k]);
                    //printf("Checking readValue[%d] == expectedValue[%d]: %d == %d?\n", k, k, readValue[k], expectedValue[k]);
                }
                ok = valueOk;
            }
            if (!ok) {
                printf("Read failed: %s\n", UA_StatusCode_name(rc));
                retries--;
                Sleep::Sec(1.0);
                rc = ReadValue(nodeId, &UA_TYPES[UA_TYPES_EXTENSIONOBJECT], 1u, &v);
            }
        }
        UA_Variant_clear(&v);
        return ok;

    }

    void Disconnect() {
        UA_Client_disconnect(client);
        UA_Client_delete(client);
    }


private:
    UA_Client *client;


};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

class OPCUADSOutputSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

OPCUADSOutputSchedulerTestHelper    () : MARTe::GAMSchedulerI() {
        scheduledStates = NULL;
    }

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    void ExecuteThreadCycle(MARTe::uint32 threadId) {
        using namespace MARTe;
        ReferenceT<RealTimeApplication> realTimeAppT = realTimeApp;
        ExecuteSingleCycle(scheduledStates[realTimeAppT->GetIndex()]->threads[threadId].executables,
                scheduledStates[realTimeAppT->GetIndex()]->threads[threadId].numberOfExecutables);
    }

    virtual bool ConfigureScheduler(MARTe::Reference realTimeApp) {

        bool ret = GAMSchedulerI::ConfigureScheduler(realTimeApp);
        if (ret) {
            scheduledStates = GetSchedulableStates();
        }
        return ret;
    }

    virtual void CustomPrepareNextState() {
    }

private:

    MARTe::ScheduledState * const * scheduledStates;
};

CLASS_REGISTER(OPCUADSOutputSchedulerTestHelper, "1.0")

/**
 * @brief GAM which writes signals into a given OPCUADSOutput
 */
class OPCUADSOutputGAMTestHelper: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()OPCUADSOutputGAMTestHelper() {
        char8Signal = NULL;
        uint8Signal = NULL;
        int8Signal = NULL;
        uint16Signal = NULL;
        int16Signal = NULL;
        uint32Signal = NULL;
        int32Signal = NULL;
        uint64Signal = NULL;
        int64Signal = NULL;
        float32Signal = NULL;
        float64Signal = NULL;
        boolSignal = NULL;
        numberOfElements = 1u;
    }

    virtual ~OPCUADSOutputGAMTestHelper() {
        using namespace MARTe;
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        GetSignalNumberOfElements(OutputSignals, 0, numberOfElements);
        uint32 n;
        uint32 numberOfSignals = GetNumberOfOutputSignals();
        for(n=0; n<numberOfSignals; n++) {
            if (GetSignalType(OutputSignals, n) == Character8Bit) {
                char8Signal = reinterpret_cast<char8 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == UnsignedInteger8Bit) {
                uint8Signal = reinterpret_cast<uint8 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == SignedInteger8Bit) {
                int8Signal = reinterpret_cast<int8 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == UnsignedInteger16Bit) {
                uint16Signal = reinterpret_cast<uint16 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == SignedInteger16Bit) {
                int16Signal = reinterpret_cast<int16 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == UnsignedInteger32Bit) {
                uint32Signal = reinterpret_cast<uint32 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == SignedInteger32Bit) {
                int32Signal = reinterpret_cast<int32 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == UnsignedInteger64Bit) {
                uint64Signal = reinterpret_cast<uint64 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == SignedInteger64Bit) {
                int64Signal = reinterpret_cast<int64 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == Float32Bit) {
                float32Signal = reinterpret_cast<float32 *>(GetOutputSignalMemory(n));
            }
            else if (GetSignalType(OutputSignals, n) == BooleanType) {
                boolSignal = reinterpret_cast<bool *>(GetOutputSignalMemory(n));
                *boolSignal = false;
            }
            else {
                float64Signal = reinterpret_cast<float64 *>(GetOutputSignalMemory(n));
            }
        }

        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        return true;
    }

    MARTe::char8 *char8Signal;
    MARTe::uint8 *uint8Signal;
    MARTe::int8 *int8Signal;
    MARTe::uint16 *uint16Signal;
    MARTe::int16 *int16Signal;
    MARTe::uint32 *uint32Signal;
    MARTe::int32 *int32Signal;
    MARTe::uint64 *uint64Signal;
    MARTe::int64 *int64Signal;
    MARTe::float32 *float32Signal;
    MARTe::float64 *float64Signal;
    bool *boolSignal;
    MARTe::uint32 numberOfElements;
};
CLASS_REGISTER(OPCUADSOutputGAMTestHelper, "1.0")

bool OPCUADSOutputTest::TestConstructor() {
    using namespace MARTe;
    OPCUADSOutput odo;
    StreamString s = odo.GetServerAddress();
    StreamString c = "";
    return (s == c);
}

bool OPCUADSOutputTest::TestInitialise() {
    using namespace MARTe;
    StreamString config = ""
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
            "            Authentication = None\n"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
            "                    Type = uint32\n"
            "                    Trigger = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSOutputTest::TestInitialise_Introspection() {
    using namespace MARTe;
    StreamString config = ""
            "+OPCUATypes = {\n"
            "     Class = ReferenceContainer\n"
            "     +SensorPackage = {\n"
            "         Class = IntrospectionStructure\n"
            "         Sensor1 = {\n"
            "             Type = uint32\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "     }\n"
            "     +MasterSet = {\n"
            "         Class = IntrospectionStructure\n"
            "         SensorPackage1 = {\n"
            "             Type = SensorPackage\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "     }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Alias = \"Sensor1\"\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUAOut\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUAOut = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            Authentication = None\n"
            "            Signals = {\n"
            "                Sensor1 = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MasterSet.SensorPackage1.Sensor1\n"
            "                    Type = uint32\n"
            "                    Trigger = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSOutputTest::TestInitialise_ExtensionObject() {
    using namespace MARTe;
    StreamString config = ""
            "+OPCUATypes = {\n"
            "     Class = ReferenceContainer\n"
            "     +SensorPackage = {\n"
            "         Class = IntrospectionStructure\n"
            "         Sensor1 = {\n"
            "             Type = uint32\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "     }\n"
            "     +MasterSet = {\n"
            "         Class = IntrospectionStructure\n"
            "         SensorPackage1 = {\n"
            "             Type = SensorPackage\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "     }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Alias = \"Sensor1\"\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                MasterSet = {\n"
            "                    Type = MasterSet\n"
            "                    DataSource = OPCUAIn\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                MasterSet = {\n"
            "                    Type = MasterSet\n"
            "                    DataSource = OPCUAOut\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "        +DDB1 = {\n"
            "            Class = GAMDataSource\n"
            "        }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUAIn = {\n"
            "            Class = OPCUADataSource::OPCUADSInput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            Authentication = None\n"
            "            Synchronise = \"yes\"\n"
            "            Signals = {\n"
            "                MasterSet = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MasterSet\n"
            "                    Type = MasterSet\n"
            "                    ExtensionObject = \"yes\"\n"
            "                    SynchSignal = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +OPCUAOut = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            Authentication = None\n"
            "            Signals = {\n"
            "                MasterSet = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MasterSet\n"
            "                    Type = MasterSet\n"
            "                    ExtensionObject = \"yes\"\n"
            "                    Trigger = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSOutputTest::TestInitialise_NoAddress() {
    using namespace MARTe;
    StreamString config = ""
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Authentication = None\n"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MyNode\n"
            "                    Type = uint32\n"
            "                    Trigger = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::TestInitialise_NoSignals() {
    using namespace MARTe;
    StreamString config = ""
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                    Trigger = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
            "            Authentication = None\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::TestInitialise_NoPath() {
    using namespace MARTe;
    StreamString config = ""
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
            "            Authentication = None\n"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Type = uint32\n"
            "                    Trigger = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::TestInitialise_NoNamespaceIndex() {
    using namespace MARTe;
    StreamString config = ""
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUA\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUA = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\""
            "            Authentication = None\n"
            "            Signals = {\n"
            "                MyNode = {\n"
            "                    Path = MyNode\n"
            "                    Type = uint32\n"
            "                    Trigger = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_NumberOfDimensionsGreater1() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  Authentication = None\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint32\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           Alias = \"MyNode\"\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Authentication = None\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode\n"
            "          Type = uint32\n"
            "          NumberOfDimensions = 2\n"
            "          Trigger = 1\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
            "}\n";
    config.Seek(0LLU);
    //HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_SetConfiguredDatabase_FailSetServiceRequest() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  CPUMask = 0x2\n"
            "  Authentication = None\n"
            "  AddressSpace = {\n"
            "    MyNode = {\n"
            "      Type = uint32\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "   +Functions = {\n"
            "     Class = ReferenceContainer\n"
            "     +GAMTimer = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         Counter = {\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           Frequency = 1\n"
            "           DataSource = Timer\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         Counter = {\n"
            "           Alias = \"MyNode\"\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "         Time = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "     +GAMDisplay = {\n"
            "       Class = IOGAM\n"
            "       InputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = DDB1\n"
            "           Type = uint32\n"
            "         }\n"
            "       }\n"
            "       OutputSignals = {\n"
            "         MyNode = {\n"
            "           DataSource = OPCUAOut\n"
            "           Type = uint32\n"
            "           Trigger = 1\n"
            "         }\n"
            "       }\n"
            "     }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Authentication = None\n"
            "      Signals = {\n"
            "        MyNode = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = MyNode1\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMTimer GAMDisplay}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = GAMScheduler\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    ReferenceT<OPCUADSOutput> odo;
    if (ok) {
        odo = ord->Find("TestApp.Data.OPCUAOut");
        ok = odo.IsValid();
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    Sleep::MSec(200);
    ObjectRegistryDatabase::Instance()->Purge();
    return !ok;
}

bool OPCUADSOutputTest::Test_SetConfiguredDatabase_ExtensionObject() {
    using namespace MARTe;
    StreamString config = ""
            "+OPCUATypes = {\n"
            "     Class = ReferenceContainer\n"
            "     +SensorPackage = {\n"
            "         Class = IntrospectionStructure\n"
            "         Sensor1 = {\n"
            "             Type = uint32\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "     }\n"
            "     +MasterSet = {\n"
            "         Class = IntrospectionStructure\n"
            "         SensorPackage1 = {\n"
            "             Type = SensorPackage\n"
            "             NumberOfElements = 1\n"
            "         }\n"
            "     }\n"
            "}\n"
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  Port = 4840\n"
            "  Authentication = None\n"
            "  AddressSpace = {\n"
            "    MasterSet = {\n"
            "      Type = MasterSet\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "                Time = {\n"
            "                    Frequency = 1\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Alias = \"Sensor1\"\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +GAMDisplay = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Sensor1 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = DDB1\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Sensor1 = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUAOut\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUAOut = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            Authentication = None\n"
            "            Signals = {\n"
            "                Sensor1 = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = MasterSet.SensorPackage1.Sensor1\n"
            "                    Type = uint32\n"
            "                    Trigger = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    +Timer = {\n"
            "      Class = LinuxTimer\n"
            "      SleepNature = \"Default\"\n"
            "      Signals = {\n"
            "        Counter = {\n"
            "          Type = uint32\n"
            "        }\n"
            "        Time = {\n"
            "          Type = uint32\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer GAMDisplay}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("Test");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSOutputTest::Test_Authentication() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "    Class = OPCUA::OPCUAServer\n"
            "    Port = 4840\n"
            "    Authentication = UserPassword\n"
            "    UserPasswordFile = /tmp/opcua_server_test_auth_keys.cfg\n"
            "    AddressSpace = {\n"
            "        Counter = {\n"
            "            Type = uint32\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUAOut\n"
            "                    Trigger = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUAOut = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            Authentication = UserPassword\n"
            "            UserPasswordFile = /tmp/opcua_client_test_auth_key.cfg\n"
            "            Signals = {\n"
            "                Counter = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = Counter\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +Timer = {\n"
            "            Class = LinuxTimer\n"
            "            SleepNature = \"Default\"\n"
            "            Signals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    StreamString serverAuthKeys = ""
            "AuthenticationKeys = {"
            "   KeyA = {"
            "       Username = user1"
            "       Password = password1"
            "   }"
            "   KeyB = {"
            "       Username = user2"
            "       Password = password2"
            "   }"
            "}\n";
    uint32 serverAuthKeysSize = serverAuthKeys.Size();
    File serverAuthKeysFile;
    bool ok = serverAuthKeysFile.Open("/tmp/opcua_server_test_auth_keys.cfg",
            BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_TRUNC | BasicFile::FLAG_APPEND);
    ok = ok && serverAuthKeysFile.Write(serverAuthKeys.Buffer(), serverAuthKeysSize);
    (void) serverAuthKeysFile.Close();

    StreamString clientAuthKey = ""
            "AuthenticationKey = {"
            "    Username = user2"
            "    Password = password2"
            "}\n";
    uint32 clientAuthKeySize = clientAuthKey.Size();
    File clientAuthKeyFile;
    ok = ok && clientAuthKeyFile.Open("/tmp/opcua_client_test_auth_key.cfg",
            BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_TRUNC | BasicFile::FLAG_APPEND);
    ok = ok && clientAuthKeyFile.Write(clientAuthKey.Buffer(), clientAuthKeySize);
    (void) clientAuthKeyFile.Close();

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    ok = ok && parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("Test");
        ok = app.IsValid();
    }
    if (ok) {
        // ConfigureApplication() calls SetConfiguredDatabase() on the DS
        // which uses a client to connect with authentication
        ok = app->ConfigureApplication();
    }
    (void) remove("/tmp/opcua_server_test_auth_keys.cfg");
    (void) remove("/tmp/opcua_client_test_auth_key.cfg");
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSOutputTest::Test_Authentication_NoCreds() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "    Class = OPCUA::OPCUAServer\n"
            "    Port = 4840\n"
            "    Authentication = UserPassword\n"
            "    UserPasswordFile = /tmp/opcua_server_test_auth_keys.cfg\n"
            "    AddressSpace = {\n"
            "        Counter = {\n"
            "            Type = uint32\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUAOut\n"
            "                    Trigger = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUAOut = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            Authentication = UserPassword\n"
            "            Signals = {\n"
            "                Counter = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = Counter\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +Timer = {\n"
            "            Class = LinuxTimer\n"
            "            SleepNature = \"Default\"\n"
            "            Signals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    StreamString serverAuthKeys = ""
            "AuthenticationKeys = {"
            "   KeyA = {"
            "       Username = user1"
            "       Password = password1"
            "   }"
            "   KeyB = {"
            "       Username = user2"
            "       Password = password2"
            "   }"
            "}\n";
    uint32 serverAuthKeysSize = serverAuthKeys.Size();
    File serverAuthKeysFile;
    bool ok = serverAuthKeysFile.Open("/tmp/opcua_server_test_auth_keys.cfg",
            BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_TRUNC | BasicFile::FLAG_APPEND);
    ok = ok && serverAuthKeysFile.Write(serverAuthKeys.Buffer(), serverAuthKeysSize);
    (void) serverAuthKeysFile.Close();

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    ok = ok && parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = !(ord->Initialise(cdb));
    }
    Sleep::MSec(200);
    (void) remove("/tmp/opcua_server_test_auth_keys.cfg");
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSOutputTest::Test_Authentication_BadCreds() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "    Class = OPCUA::OPCUAServer\n"
            "    Port = 4840\n"
            "    Authentication = UserPassword\n"
            "    UserPasswordFile = /tmp/opcua_server_test_auth_keys.cfg\n"
            "    AddressSpace = {\n"
            "        Counter = {\n"
            "            Type = uint32\n"
            "        }\n"
            "    }\n"
            "}\n"
            "$Test = {\n"
            "    Class = RealTimeApplication\n"
            "    +Functions = {\n"
            "        Class = ReferenceContainer\n"
            "        +GAMTimer = {\n"
            "            Class = IOGAM\n"
            "            InputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = Timer\n"
            "                }\n"
            "            }\n"
            "            OutputSignals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                    DataSource = OPCUAOut\n"
            "                    Trigger = 1\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Data = {\n"
            "        Class = ReferenceContainer\n"
            "        +Timings = {\n"
            "            Class = TimingDataSource\n"
            "        }\n"
            "        +OPCUAOut = {\n"
            "            Class = OPCUADataSource::OPCUADSOutput\n"
            "            Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "            Authentication = UserPassword\n"
            "            UserPasswordFile = /tmp/opcua_client_test_auth_key.cfg\n"
            "            Signals = {\n"
            "                Counter = {\n"
            "                    NamespaceIndex = 1\n"
            "                    Path = Counter\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "        +Timer = {\n"
            "            Class = LinuxTimer\n"
            "            SleepNature = \"Default\"\n"
            "            Signals = {\n"
            "                Counter = {\n"
            "                    Type = uint32\n"
            "                }\n"
            "                Time = {\n"
            "                    Type = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +States = {\n"
            "        Class = ReferenceContainer\n"
            "        +State1 = {\n"
            "            Class = RealTimeState\n"
            "            +Threads = {\n"
            "                Class = ReferenceContainer\n"
            "                +Thread1 = {\n"
            "                    Class = RealTimeThread\n"
            "                    Functions = {GAMTimer}\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +Scheduler = {\n"
            "        Class = GAMScheduler\n"
            "        TimingDataSource = Timings\n"
            "    }\n"
            "}\n";
    StreamString serverAuthKeys = ""
            "AuthenticationKeys = {"
            "   KeyA = {"
            "       Username = user1"
            "       Password = password1"
            "   }"
            "   KeyB = {"
            "       Username = user2"
            "       Password = password2"
            "   }"
            "}\n";
    uint32 serverAuthKeysSize = serverAuthKeys.Size();
    File serverAuthKeysFile;
    bool ok = serverAuthKeysFile.Open("/tmp/opcua_server_test_auth_keys.cfg",
        BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_TRUNC | BasicFile::FLAG_APPEND);
    ok = ok && serverAuthKeysFile.Write(serverAuthKeys.Buffer(), serverAuthKeysSize);
    (void) serverAuthKeysFile.Close();

    StreamString clientAuthKey = ""
            "AuthenticationKey = {"
            "    Username = user2"
            "    Password = password77"
            "}\n";
    uint32 clientAuthKeySize = clientAuthKey.Size();
    File clientAuthKeyFile;
    ok = ok && clientAuthKeyFile.Open("/tmp/opcua_client_test_auth_key.cfg",
        BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_TRUNC | BasicFile::FLAG_APPEND);
    ok = ok && clientAuthKeyFile.Write(clientAuthKey.Buffer(), clientAuthKeySize);
    (void) clientAuthKeyFile.Close();

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    ok = ok && parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("Test");
        ok = app.IsValid();
    }
    if (ok) {
        // ConfigureApplication() calls SetConfiguredDatabase() on the DS
        // which uses a client to connect with authentication
        ok = !(app->ConfigureApplication());
    }
    (void) remove("/tmp/opcua_server_test_auth_keys.cfg");
    (void) remove("/tmp/opcua_client_test_auth_key.cfg");
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSOutputTest::Test_Synchronise() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  Authentication = None\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    NodeU8 = {\n"
            "      Type = uint8\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "    NodeU16 = {\n"
            "      Type = uint16\n"
            "    }\n"
            "    NodeU32 = {\n"
            "      Type = uint32\n"
            "    }\n"
            "    NodeU64 = {\n"
            "      Type = uint64\n"
            "    }\n"
            "    NodeI8 = {\n"
            "      Type = int8\n"
            "    }\n"
            "    NodeI16 = {\n"
            "      Type = int16\n"
            "    }\n"
            "    NodeI32 = {\n"
            "      Type = int32\n"
            "    }\n"
            "    NodeI64 = {\n"
            "      Type = int64\n"
            "    }\n"
            "    NodeF32 = {\n"
            "      Type = float32\n"
            "    }\n"
            "    NodeF64 = {\n"
            "      Type = float32\n"
            "    }\n"
            "    NodeU8A = {\n"
            "      Type = uint8\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeU16A = {\n"
            "      Type = uint16\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeU32A = {\n"
            "      Type = uint32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeU64A = {\n"
            "      Type = uint64\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI8A = {\n"
            "      Type = int8\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI16A = {\n"
            "      Type = int16\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI32A = {\n"
            "      Type = int32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI64A = {\n"
            "      Type = int64\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeF32A = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeF64A = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "  +Functions = {\n"
            "    Class = ReferenceContainer\n"
            "    +GAMWriterScalar = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        SignalUInt8 = {\n"
            "          Type = uint8\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalUInt16 = {\n"
            "          Type = uint16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalUInt32 = {\n"
            "          Type = uint32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalUInt64 = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt8 = {\n"
            "          Type = int8\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt16 = {\n"
            "          Type = int16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt32 = {\n"
            "          Type = int32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt64 = {\n"
            "          Type = int64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalFloat32 = {\n"
            "          Type = float32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalFloat64 = {\n"
            "          Type = float64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMWriterArray = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        SignalUInt8A = {\n"
            "          Type = uint8\n"
            "          Trigger = 1\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt16A = {\n"
            "          Type = uint16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt32A = {\n"
            "          Type = uint32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt64A = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt8A = {\n"
            "          Type = int8\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt16A = {\n"
            "          Type = int16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt32A = {\n"
            "          Type = int32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt64A = {\n"
            "          Type = int64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat32A = {\n"
            "          Type = float32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat64A = {\n"
            "          Type = float64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUADSOutputTest = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Authentication = None\n"
            "      Signals = {\n"
            "        SignalUInt8 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU8\n"
            "          Type = uint8\n"
            "        }\n"
            "        SignalUInt16 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU16\n"
            "          Type = uint16\n"
            "        }\n"
            "        SignalUInt32 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU32\n"
            "          Type = uint32\n"
            "        }\n"
            "        SignalUInt64 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU64\n"
            "          Type = uint64\n"
            "        }\n"
            "        SignalInt8 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI8\n"
            "          Type = int8\n"
            "        }\n"
            "        SignalInt16 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI16\n"
            "          Type = int16\n"
            "        }\n"
            "        SignalInt32 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI32\n"
            "          Type = int32\n"
            "        }\n"
            "        SignalInt64 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI64\n"
            "          Type = int64\n"
            "        }\n"
            "        SignalFloat32 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF32\n"
            "          Type = float32\n"
            "        }\n"
            "        SignalFloat64 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF64\n"
            "          Type = float64\n"
            "        }\n"
            "        SignalUInt8A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU8A\n"
            "          Type = uint8\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt16A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU16A\n"
            "          Type = uint16\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt32A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU32A\n"
            "          Type = uint32\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt64A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU64A\n"
            "          Type = uint64\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt8A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI8A\n"
            "          Type = int8\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt16A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI16A\n"
            "          Type = int16\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt32A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI32A\n"
            "          Type = int32\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt64A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI64A\n"
            "          Type = int64\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat32A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF32A\n"
            "          Type = float32\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat64A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF64A\n"
            "          Type = float64\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMWriterScalar GAMWriterArray}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = OPCUADSOutputSchedulerTestHelper\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
            "}\n";
    config.Seek(0LLU);
    //HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterScalar;
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterArray;
    if (ok) {
        gamWriterScalar = ord->Find("TestApp.Functions.GAMWriterScalar");
        ok = gamWriterScalar.IsValid();
    }
    if (ok) {
        gamWriterArray = ord->Find("TestApp.Functions.GAMWriterArray");
        ok = gamWriterArray.IsValid();
    }
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    ReferenceT<OPCUADSOutputSchedulerTestHelper> scheduler;
    if (ok) {
        scheduler = ord->Find("TestApp.Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    
    OPCUADSOutputTestReader opcuaReader;
    const uint32 numberOfNodes = 20;
    const char8 *nodeNames[] = {"NodeU8", "NodeU16", "NodeU32", "NodeU64", "NodeI8", "NodeI16", "NodeI32", "NodeI64", "NodeF32", "NodeF64", "NodeU8A", "NodeU16A", "NodeU32A", "NodeU64A", "NodeI8A", "NodeI16A", "NodeI32A", "NodeI64A", "NodeF32A", "NodeF64A"};
    uint32 nodeNElements[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    uint32 nodeTypeSizes[] = {sizeof(uint8), sizeof(uint16), sizeof(uint32), sizeof(uint64), sizeof(int8), sizeof(int16), sizeof(int32), sizeof(int64), sizeof(float32), sizeof(float64), sizeof(uint8), sizeof(uint16), sizeof(uint32), sizeof(uint64), sizeof(int8), sizeof(int16), sizeof(int32), sizeof(int64), sizeof(float32), sizeof(float64)};
    UA_NodeId nodeIds[numberOfNodes];
    const UA_DataType *nodeDataTypes[] = {&UA_TYPES[UA_TYPES_BYTE], &UA_TYPES[UA_TYPES_UINT16], &UA_TYPES[UA_TYPES_UINT32], &UA_TYPES[UA_TYPES_UINT64], &UA_TYPES[UA_TYPES_SBYTE], &UA_TYPES[UA_TYPES_INT16], &UA_TYPES[UA_TYPES_INT32], &UA_TYPES[UA_TYPES_INT64], &UA_TYPES[UA_TYPES_FLOAT], &UA_TYPES[UA_TYPES_DOUBLE], &UA_TYPES[UA_TYPES_BYTE], &UA_TYPES[UA_TYPES_UINT16], &UA_TYPES[UA_TYPES_UINT32], &UA_TYPES[UA_TYPES_UINT64], &UA_TYPES[UA_TYPES_SBYTE], &UA_TYPES[UA_TYPES_INT16], &UA_TYPES[UA_TYPES_INT32], &UA_TYPES[UA_TYPES_INT64], &UA_TYPES[UA_TYPES_FLOAT], &UA_TYPES[UA_TYPES_DOUBLE]};
    void *expectedNodeValues[] = {gamWriterScalar->uint8Signal, gamWriterScalar->uint16Signal,gamWriterScalar->uint32Signal, gamWriterScalar->uint64Signal, gamWriterScalar->int8Signal, gamWriterScalar->int16Signal, gamWriterScalar->int32Signal, gamWriterScalar->int64Signal, gamWriterScalar->float32Signal, gamWriterScalar->float64Signal, gamWriterArray->uint8Signal, gamWriterArray->uint16Signal,gamWriterArray->uint32Signal, gamWriterArray->uint64Signal, gamWriterArray->int8Signal, gamWriterArray->int16Signal, gamWriterArray->int32Signal, gamWriterArray->int64Signal, gamWriterArray->float32Signal, gamWriterArray->float64Signal};

    if (ok) {
        ok = opcuaReader.Connect();
    }
    if (ok) {
        for (uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            ok = (opcuaReader.FindChildNodeId(nodeNames[k], &nodeIds[k]) == UA_STATUSCODE_GOOD);
        }
    }
    if (ok) {
        *gamWriterScalar->uint8Signal = 1u;
        *gamWriterScalar->uint16Signal = 2u;
        *gamWriterScalar->uint32Signal = 3u;
        *gamWriterScalar->uint64Signal = 4u;
        *gamWriterScalar->int8Signal = 1;
        *gamWriterScalar->int16Signal = 2;
        *gamWriterScalar->int32Signal = 3;
        *gamWriterScalar->int64Signal = 4;
        *gamWriterScalar->float32Signal = 3.2;
        *gamWriterScalar->float64Signal = 6.2;

        gamWriterArray->uint8Signal[0] = 3;
        gamWriterArray->uint8Signal[1] = 1;
        gamWriterArray->uint8Signal[2] = 2;
        gamWriterArray->uint16Signal[0] = 4;
        gamWriterArray->uint16Signal[1] = 2;
        gamWriterArray->uint16Signal[2] = 3;
        gamWriterArray->uint32Signal[0] = 5;
        gamWriterArray->uint32Signal[1] = 3;
        gamWriterArray->uint32Signal[2] = 4;
        gamWriterArray->uint64Signal[0] = 6;
        gamWriterArray->uint64Signal[1] = 4;
        gamWriterArray->uint64Signal[2] = 5;
        gamWriterArray->int8Signal[0] = -3;
        gamWriterArray->int8Signal[1] = 2;
        gamWriterArray->int8Signal[2] = -2;
        gamWriterArray->int16Signal[0] = -4;
        gamWriterArray->int16Signal[1] = 3;
        gamWriterArray->int16Signal[2] = -3;
        gamWriterArray->int32Signal[0] = -5;
        gamWriterArray->int32Signal[1] = 4;
        gamWriterArray->int32Signal[2] = -4;
        gamWriterArray->int64Signal[0] = -6;
        gamWriterArray->int64Signal[1] = 5;
        gamWriterArray->int64Signal[2] = -5;
        gamWriterArray->float32Signal[0] = 3.1;
        gamWriterArray->float32Signal[1] = -3.2;
        gamWriterArray->float32Signal[2] = 3.3;
        gamWriterArray->float64Signal[0] = -6.1;
        gamWriterArray->float64Signal[1] = 6.2;
        gamWriterArray->float64Signal[2] = -6.3;
        scheduler->ExecuteThreadCycle(0u);
        for(uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            //printf("Checking %d %s %d\n", k, nodeNames[k], nodeNElements[k]);
            ok = opcuaReader.WaitForValue(nodeIds[k], reinterpret_cast<uint8 *>(expectedNodeValues[k]), nodeDataTypes[k], nodeTypeSizes[k], nodeNElements[k]);
        }
    }
    if (ok) {
        *gamWriterScalar->uint8Signal = 2u;
        *gamWriterScalar->uint16Signal = 3u;
        *gamWriterScalar->uint32Signal = 4u;
        *gamWriterScalar->uint64Signal = 5u;
        *gamWriterScalar->int8Signal = 2;
        *gamWriterScalar->int16Signal = 3;
        *gamWriterScalar->int32Signal = 4;
        *gamWriterScalar->int64Signal = 5;
        *gamWriterScalar->float32Signal = 4.2;
        *gamWriterScalar->float64Signal = 7.2;

        gamWriterArray->uint8Signal[0] = 4;
        gamWriterArray->uint8Signal[1] = 2;
        gamWriterArray->uint8Signal[2] = 3;
        gamWriterArray->uint16Signal[0] = 5;
        gamWriterArray->uint16Signal[1] = 3;
        gamWriterArray->uint16Signal[2] = 4;
        gamWriterArray->uint32Signal[0] = 6;
        gamWriterArray->uint32Signal[1] = 4;
        gamWriterArray->uint32Signal[2] = 5;
        gamWriterArray->uint64Signal[0] = 7;
        gamWriterArray->uint64Signal[1] = 5;
        gamWriterArray->uint64Signal[2] = 6;
        gamWriterArray->int8Signal[0] = -2;
        gamWriterArray->int8Signal[1] = 3;
        gamWriterArray->int8Signal[2] = -1;
        gamWriterArray->int16Signal[0] = -3;
        gamWriterArray->int16Signal[1] = 4;
        gamWriterArray->int16Signal[2] = -2;
        gamWriterArray->int32Signal[0] = -4;
        gamWriterArray->int32Signal[1] = 5;
        gamWriterArray->int32Signal[2] = -3;
        gamWriterArray->int64Signal[0] = -5;
        gamWriterArray->int64Signal[1] = 6;
        gamWriterArray->int64Signal[2] = -4;
        gamWriterArray->float32Signal[0] = 4.1;
        gamWriterArray->float32Signal[1] = -2.2;
        gamWriterArray->float32Signal[2] = 4.3;
        gamWriterArray->float64Signal[0] = -5.1;
        gamWriterArray->float64Signal[1] = 7.2;
        gamWriterArray->float64Signal[2] = -6.3;

        scheduler->ExecuteThreadCycle(0u);
        for(uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            //printf("Checking %d %s %d\n", k, nodeNames[k], nodeNElements[k]);
            ok = opcuaReader.WaitForValue(nodeIds[k], reinterpret_cast<uint8 *>(expectedNodeValues[k]), nodeDataTypes[k], nodeTypeSizes[k], nodeNElements[k]);
        }

    }
    //Sleep::Sec(60);
    opcuaReader.Disconnect();
    ord->Purge();

    return ok;
}

bool OPCUADSOutputTest::Test_Synchronise_ExtensionObject() {
    using namespace MARTe;
    OPCUATestServer ots;
    ots.service.Start();
    StreamString config = ""
            "+OPCUATypes = {\n"
            "  Class = ReferenceContainer\n"
            "  +Point = {\n"
            "    Class = IntrospectionStructure\n"
            "    x = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "    y = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "    z = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "  }\n"
            "}\n"
            "+MessageX = {\n"
            "  Class = Message\n"
            "  Destination = \"TestApp.Functions.GAMXYZ\"\n"
            "  Function = \"SetOutput\"\n"
            "  +Parameters = {\n"
            "    Class = ConfigurationDatabase\n"
            "    SignalName = \"x\"\n"
            "    SignalValue = -6.7\n"
            "  }\n"
            "}\n"
            "+MessageY = {\n"
            "  Class = Message\n"
            "  Destination = \"TestApp.Functions.GAMXYZ\"\n"
            "  Function = \"SetOutput\"\n"
            "  +Parameters = {\n"
            "    Class = ConfigurationDatabase\n"
            "    SignalName = \"y\"\n"
            "    SignalValue = -8.9\n"
            "  }\n"
            "}\n"
            "+MessageZ = {\n"
            "  Class = Message\n"
            "  Destination = \"TestApp.Functions.GAMXYZ\"\n"
            "  Function = \"SetOutput\"\n"
            "  +Parameters = {\n"
            "    Class = ConfigurationDatabase\n"
            "    SignalName = \"z\"\n"
            "    SignalValue = 9.0\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "  +Functions = {\n"
            "    Class = ReferenceContainer\n"
            "    +GAMXYZ = {\n"
            "      Class = ConstantGAM\n"
            "      OutputSignals = {\n"
            "        x = {\n"
            "          DataSource = DDB1\n"
            "          Type = float32\n"
            "          Default = 1.2\n"
            "        }\n"
            "        y = {\n"
            "          DataSource = DDB1\n"
            "          Type = float32\n"
            "          Default = 3.4\n"
            "        }\n"
            "        z = {\n"
            "          DataSource = DDB1\n"
            "          Type = float32\n"
            "          Default = -5.6\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMWriter = {\n"
            "      Class = IOGAM\n"
            "      InputSignals = {\n"
            "        x = {\n"
            "          Type = float32\n"
            "          DataSource = DDB1\n"
            "        }\n"
            "        y = {\n"
            "          Type = float32\n"
            "          DataSource = DDB1\n"
            "        }\n"
            "        z = {\n"
            "          Type = float32\n"
            "          DataSource = DDB1\n"
            "        }\n"
            "      }\n"
            "      OutputSignals = {\n"
            "        Point = {\n"
            "          Type = Point\n"
            "          DataSource = OPCUAOut\n"
            "          Trigger = 1\n"
            "          TriggerSignal = \"Point.x\"\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +Timings = {\n"
            "      Class = TimingDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\""
            "      Signals = {\n"
            "        Point = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = Point\n"
            "          Type = Point\n"
            "          ExtensionObject = \"yes\"\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          Functions = {GAMXYZ GAMWriter}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = OPCUADSOutputSchedulerTestHelper\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    if (ok) {
        app->PrepareNextState("State1");
    }
    ReferenceT<OPCUADSOutputSchedulerTestHelper> scheduler;
    if (ok) {
        scheduler = ord->Find("TestApp.Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    if (ok) {
        scheduler->ExecuteThreadCycle(0u);
    }
    Sleep::Sec(0.2);
    OPCUADSOutputTestReader opcuaReader;
    if (ok) {
        ok = opcuaReader.Connect();
    }
    const char8 *nodeName = "Point";
    UA_NodeId nodeId;

    if (ok) {
        ok = (opcuaReader.FindChildNodeId(nodeName, &nodeId) == UA_STATUSCODE_GOOD);
    }
    if (ok) {
        float32 expectedValue[] = {1.2, 3.4, -5.6};
        ok = opcuaReader.WaitForExtObjValue(nodeId, reinterpret_cast<uint8 *>(&expectedValue[0]), sizeof(float32) * 3);
    }
    ReferenceT<Message> message;
    if (ok) {
        message = ord->Find("MessageX");
        MessageI::SendMessage(message, NULL);
    }
    if (ok) {
        message = ord->Find("MessageY");
        MessageI::SendMessage(message, NULL);
    }
    if (ok) {
        message = ord->Find("MessageZ");
        MessageI::SendMessage(message, NULL);
    }
    if (ok) {
        scheduler->ExecuteThreadCycle(0u);
    }
    Sleep::Sec(0.2);

    if (ok) {
        float32 expectedValue[] = {-6.7, -8.9, 9.0};
        ok = opcuaReader.WaitForExtObjValue(nodeId, reinterpret_cast<uint8 *>(&expectedValue[0]), sizeof(float32) * 3);
    }

    opcuaReader.Disconnect();
    ots.SetRunning(false);
    ots.service.Stop();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSOutputTest::Test_Synchronise_Timestamp() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  Authentication = None\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    NodeU8 = {\n"
            "      Type = uint8\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "    NodeU16 = {\n"
            "      Type = uint16\n"
            "    }\n"
            "    NodeU32 = {\n"
            "      Type = uint32\n"
            "    }\n"
            "    NodeU64 = {\n"
            "      Type = uint64\n"
            "    }\n"
            "    NodeI8 = {\n"
            "      Type = int8\n"
            "    }\n"
            "    NodeI16 = {\n"
            "      Type = int16\n"
            "    }\n"
            "    NodeI32 = {\n"
            "      Type = int32\n"
            "    }\n"
            "    NodeI64 = {\n"
            "      Type = int64\n"
            "    }\n"
            "    NodeF32 = {\n"
            "      Type = float32\n"
            "    }\n"
            "    NodeF64 = {\n"
            "      Type = float32\n"
            "    }\n"
            "    NodeU8A = {\n"
            "      Type = uint8\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeU16A = {\n"
            "      Type = uint16\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeU32A = {\n"
            "      Type = uint32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeU64A = {\n"
            "      Type = uint64\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI8A = {\n"
            "      Type = int8\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI16A = {\n"
            "      Type = int16\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI32A = {\n"
            "      Type = int32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI64A = {\n"
            "      Type = int64\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeF32A = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeF64A = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "  +Functions = {\n"
            "    Class = ReferenceContainer\n"
            "    +GAMWriterTimestamp1 = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        Timestamp1 = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMWriterTimestamp2 = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        Timestamp2 = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMWriterScalar = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        SignalUInt8 = {\n"
            "          Type = uint8\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalUInt16 = {\n"
            "          Type = uint16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalUInt32 = {\n"
            "          Type = uint32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalUInt64 = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt8 = {\n"
            "          Type = int8\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt16 = {\n"
            "          Type = int16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt32 = {\n"
            "          Type = int32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt64 = {\n"
            "          Type = int64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalFloat32 = {\n"
            "          Type = float32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalFloat64 = {\n"
            "          Type = float64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMWriterArray = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        SignalUInt8A = {\n"
            "          Type = uint8\n"
            "          Trigger = 1\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt16A = {\n"
            "          Type = uint16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt32A = {\n"
            "          Type = uint32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt64A = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt8A = {\n"
            "          Type = int8\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt16A = {\n"
            "          Type = int16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt32A = {\n"
            "          Type = int32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt64A = {\n"
            "          Type = int64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat32A = {\n"
            "          Type = float32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat64A = {\n"
            "          Type = float64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUADSOutputTest = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Authentication = None\n"
            "      Signals = {\n"
            "        Timestamp1 = {\n"
            "          Timestamp = {\"SignalUInt32\"}\n"
            "          Type = uint64\n"
            "        }\n"
            "        SignalUInt8 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU8\n"
            "          Type = uint8\n"
            "        }\n"
            "        SignalUInt16 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU16\n"
            "          Type = uint16\n"
            "        }\n"
            "        SignalUInt32 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU32\n"
            "          Type = uint32\n"
            "        }\n"
            "        SignalUInt64 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU64\n"
            "          Type = uint64\n"
            "        }\n"
            "        Timestamp2 = {\n"
            "          Timestamp = {\"SignalUInt64\" \"SignalFloat64A\"}\n"
            "          Type = uint64\n"
            "        }\n"
            "        SignalInt8 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI8\n"
            "          Type = int8\n"
            "        }\n"
            "        SignalInt16 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI16\n"
            "          Type = int16\n"
            "        }\n"
            "        SignalInt32 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI32\n"
            "          Type = int32\n"
            "        }\n"
            "        SignalInt64 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI64\n"
            "          Type = int64\n"
            "        }\n"
            "        SignalFloat32 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF32\n"
            "          Type = float32\n"
            "        }\n"
            "        SignalFloat64 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF64\n"
            "          Type = float64\n"
            "        }\n"
            "        SignalUInt8A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU8A\n"
            "          Type = uint8\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt16A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU16A\n"
            "          Type = uint16\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt32A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU32A\n"
            "          Type = uint32\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt64A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU64A\n"
            "          Type = uint64\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt8A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI8A\n"
            "          Type = int8\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt16A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI16A\n"
            "          Type = int16\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt32A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI32A\n"
            "          Type = int32\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt64A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI64A\n"
            "          Type = int64\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat32A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF32A\n"
            "          Type = float32\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat64A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF64A\n"
            "          Type = float64\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMWriterTimestamp1 GAMWriterTimestamp2 GAMWriterScalar GAMWriterArray}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = OPCUADSOutputSchedulerTestHelper\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
            "}\n";
    config.Seek(0LLU);
    //HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterScalar;
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterArray;
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterTimestamp1;
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterTimestamp2;
    if (ok) {
        gamWriterScalar = ord->Find("TestApp.Functions.GAMWriterScalar");
        ok = gamWriterScalar.IsValid();
    }
    if (ok) {
        gamWriterArray = ord->Find("TestApp.Functions.GAMWriterArray");
        ok = gamWriterArray.IsValid();
    }
    if (ok) {
        gamWriterTimestamp1 = ord->Find("TestApp.Functions.GAMWriterTimestamp1");
        ok = gamWriterTimestamp1.IsValid();
    }
    if (ok) {
        gamWriterTimestamp2 = ord->Find("TestApp.Functions.GAMWriterTimestamp2");
        ok = gamWriterTimestamp2.IsValid();
    }
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    ReferenceT<OPCUADSOutputSchedulerTestHelper> scheduler;
    if (ok) {
        scheduler = ord->Find("TestApp.Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }

    OPCUADSOutputTestReader opcuaReader;
    const uint32 numberOfNodes = 20;
    const char8 *nodeNames[] = {"NodeU8", "NodeU16", "NodeU32", "NodeU64", "NodeI8", "NodeI16", "NodeI32", "NodeI64", "NodeF32", "NodeF64", "NodeU8A", "NodeU16A", "NodeU32A", "NodeU64A", "NodeI8A", "NodeI16A", "NodeI32A", "NodeI64A", "NodeF32A", "NodeF64A"};
    uint32 nodeNElements[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    uint32 nodeTypeSizes[] = {sizeof(uint8), sizeof(uint16), sizeof(uint32), sizeof(uint64), sizeof(int8), sizeof(int16), sizeof(int32), sizeof(int64), sizeof(float32), sizeof(float64), sizeof(uint8), sizeof(uint16), sizeof(uint32), sizeof(uint64), sizeof(int8), sizeof(int16), sizeof(int32), sizeof(int64), sizeof(float32), sizeof(float64)};
    UA_NodeId nodeIds[numberOfNodes];
    const UA_DataType *nodeDataTypes[] = {&UA_TYPES[UA_TYPES_BYTE], &UA_TYPES[UA_TYPES_UINT16], &UA_TYPES[UA_TYPES_UINT32], &UA_TYPES[UA_TYPES_UINT64], &UA_TYPES[UA_TYPES_SBYTE], &UA_TYPES[UA_TYPES_INT16], &UA_TYPES[UA_TYPES_INT32], &UA_TYPES[UA_TYPES_INT64], &UA_TYPES[UA_TYPES_FLOAT], &UA_TYPES[UA_TYPES_DOUBLE], &UA_TYPES[UA_TYPES_BYTE], &UA_TYPES[UA_TYPES_UINT16], &UA_TYPES[UA_TYPES_UINT32], &UA_TYPES[UA_TYPES_UINT64], &UA_TYPES[UA_TYPES_SBYTE], &UA_TYPES[UA_TYPES_INT16], &UA_TYPES[UA_TYPES_INT32], &UA_TYPES[UA_TYPES_INT64], &UA_TYPES[UA_TYPES_FLOAT], &UA_TYPES[UA_TYPES_DOUBLE]};
    void *expectedNodeValues[] = {gamWriterScalar->uint8Signal, gamWriterScalar->uint16Signal,gamWriterScalar->uint32Signal, gamWriterScalar->uint64Signal, gamWriterScalar->int8Signal, gamWriterScalar->int16Signal, gamWriterScalar->int32Signal, gamWriterScalar->int64Signal, gamWriterScalar->float32Signal, gamWriterScalar->float64Signal, gamWriterArray->uint8Signal, gamWriterArray->uint16Signal,gamWriterArray->uint32Signal, gamWriterArray->uint64Signal, gamWriterArray->int8Signal, gamWriterArray->int16Signal, gamWriterArray->int32Signal, gamWriterArray->int64Signal, gamWriterArray->float32Signal, gamWriterArray->float64Signal};
    bool expectTimestamp[] = {false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true};
    uint64 *expectedTimestampValue[] = {NULL_PTR(uint64 *), NULL_PTR(uint64 *), gamWriterTimestamp1->uint64Signal, gamWriterTimestamp2->uint64Signal, NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), NULL_PTR(uint64 *), gamWriterTimestamp2->uint64Signal};

    if (ok) {
        ok = opcuaReader.Connect();
    }
    if (ok) {
        for (uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            ok = (opcuaReader.FindChildNodeId(nodeNames[k], &nodeIds[k]) == UA_STATUSCODE_GOOD);
        }
    }
    if (ok) {
        *gamWriterTimestamp1->uint64Signal = 1002003499;
        *gamWriterTimestamp2->uint64Signal = 2003004501;

        *gamWriterScalar->uint8Signal = 1u;
        *gamWriterScalar->uint16Signal = 2u;
        *gamWriterScalar->uint32Signal = 3u;
        *gamWriterScalar->uint64Signal = 4u;
        *gamWriterScalar->int8Signal = 1;
        *gamWriterScalar->int16Signal = 2;
        *gamWriterScalar->int32Signal = 3;
        *gamWriterScalar->int64Signal = 5;
        *gamWriterScalar->float32Signal = 3.2;
        *gamWriterScalar->float64Signal = 6.2;

        gamWriterArray->uint8Signal[0] = 3;
        gamWriterArray->uint8Signal[1] = 1;
        gamWriterArray->uint8Signal[2] = 2;
        gamWriterArray->uint16Signal[0] = 4;
        gamWriterArray->uint16Signal[1] = 2;
        gamWriterArray->uint16Signal[2] = 3;
        gamWriterArray->uint32Signal[0] = 5;
        gamWriterArray->uint32Signal[1] = 3;
        gamWriterArray->uint32Signal[2] = 4;
        gamWriterArray->uint64Signal[0] = 6;
        gamWriterArray->uint64Signal[1] = 4;
        gamWriterArray->uint64Signal[2] = 5;
        gamWriterArray->int8Signal[0] = -3;
        gamWriterArray->int8Signal[1] = 2;
        gamWriterArray->int8Signal[2] = -2;
        gamWriterArray->int16Signal[0] = -4;
        gamWriterArray->int16Signal[1] = 3;
        gamWriterArray->int16Signal[2] = -3;
        gamWriterArray->int32Signal[0] = -5;
        gamWriterArray->int32Signal[1] = 4;
        gamWriterArray->int32Signal[2] = -4;
        gamWriterArray->int64Signal[0] = -6;
        gamWriterArray->int64Signal[1] = 5;
        gamWriterArray->int64Signal[2] = -5;
        gamWriterArray->float32Signal[0] = 3.1;
        gamWriterArray->float32Signal[1] = -3.2;
        gamWriterArray->float32Signal[2] = 3.3;
        gamWriterArray->float64Signal[0] = -6.1;
        gamWriterArray->float64Signal[1] = 6.2;
        gamWriterArray->float64Signal[2] = -6.3;
        scheduler->ExecuteThreadCycle(0u);
        for(uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            //printf("Checking %d %s %d\n", k, nodeNames[k], nodeNElements[k]);
            ok = opcuaReader.WaitForValue(nodeIds[k], reinterpret_cast<uint8 *>(expectedNodeValues[k]), nodeDataTypes[k], nodeTypeSizes[k], nodeNElements[k]);
        }
        for(uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            uint64 timestampValue = 0;
            if (expectedTimestampValue[k] != NULL) {
                timestampValue = *expectedTimestampValue[k];
            }
            ok = opcuaReader.CheckTimestamp(nodeIds[k], timestampValue, expectTimestamp[k]);
        }
    }
    if (ok) {
        *gamWriterTimestamp1->uint64Signal = 1772206459535547039;
        *gamWriterTimestamp2->uint64Signal = 1193227200123456789;
        scheduler->ExecuteThreadCycle(0u);
        for(uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            uint64 timestampValue = 0;
            if (expectedTimestampValue[k] != NULL) {
                timestampValue = *expectedTimestampValue[k];
            }
            ok = opcuaReader.CheckTimestamp(nodeIds[k], timestampValue, expectTimestamp[k]);
        }
    }
    //Sleep::Sec(180);
    opcuaReader.Disconnect();
    ord->Purge();

    return ok;
}

bool OPCUADSOutputTest::Test_Synchronise_Timestamp_Default_Only() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  Authentication = None\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    NodeU8 = {\n"
            "      Type = uint8\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "    NodeU16 = {\n"
            "      Type = uint16\n"
            "    }\n"
            "    NodeU32 = {\n"
            "      Type = uint32\n"
            "    }\n"
            "    NodeU64 = {\n"
            "      Type = uint64\n"
            "    }\n"
            "    NodeI8 = {\n"
            "      Type = int8\n"
            "    }\n"
            "    NodeI16 = {\n"
            "      Type = int16\n"
            "    }\n"
            "    NodeI32 = {\n"
            "      Type = int32\n"
            "    }\n"
            "    NodeI64 = {\n"
            "      Type = int64\n"
            "    }\n"
            "    NodeF32 = {\n"
            "      Type = float32\n"
            "    }\n"
            "    NodeF64 = {\n"
            "      Type = float32\n"
            "    }\n"
            "    NodeU8A = {\n"
            "      Type = uint8\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeU16A = {\n"
            "      Type = uint16\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeU32A = {\n"
            "      Type = uint32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeU64A = {\n"
            "      Type = uint64\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI8A = {\n"
            "      Type = int8\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI16A = {\n"
            "      Type = int16\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI32A = {\n"
            "      Type = int32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI64A = {\n"
            "      Type = int64\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeF32A = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeF64A = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "  +Functions = {\n"
            "    Class = ReferenceContainer\n"
            "    +GAMWriterTimestamp1 = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        Timestamp1 = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMWriterScalar = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        SignalUInt8 = {\n"
            "          Type = uint8\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalUInt16 = {\n"
            "          Type = uint16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalUInt32 = {\n"
            "          Type = uint32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalUInt64 = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt8 = {\n"
            "          Type = int8\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt16 = {\n"
            "          Type = int16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt32 = {\n"
            "          Type = int32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt64 = {\n"
            "          Type = int64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalFloat32 = {\n"
            "          Type = float32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalFloat64 = {\n"
            "          Type = float64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMWriterArray = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        SignalUInt8A = {\n"
            "          Type = uint8\n"
            "          Trigger = 1\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt16A = {\n"
            "          Type = uint16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt32A = {\n"
            "          Type = uint32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt64A = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt8A = {\n"
            "          Type = int8\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt16A = {\n"
            "          Type = int16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt32A = {\n"
            "          Type = int32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt64A = {\n"
            "          Type = int64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat32A = {\n"
            "          Type = float32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat64A = {\n"
            "          Type = float64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUADSOutputTest = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Authentication = None\n"
            "      Signals = {\n"
            "        SignalUInt8 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU8\n"
            "          Type = uint8\n"
            "        }\n"
            "        SignalUInt16 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU16\n"
            "          Type = uint16\n"
            "        }\n"
            "        Timestamp1 = {\n"
            "          DefaultTimestampSignal = 1\n"
            "          Type = uint64\n"
            "        }\n"
            "        SignalUInt32 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU32\n"
            "          Type = uint32\n"
            "        }\n"
            "        SignalUInt64 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU64\n"
            "          Type = uint64\n"
            "        }\n"
            "        SignalInt8 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI8\n"
            "          Type = int8\n"
            "        }\n"
            "        SignalInt16 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI16\n"
            "          Type = int16\n"
            "        }\n"
            "        SignalInt32 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI32\n"
            "          Type = int32\n"
            "        }\n"
            "        SignalInt64 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI64\n"
            "          Type = int64\n"
            "        }\n"
            "        SignalFloat32 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF32\n"
            "          Type = float32\n"
            "        }\n"
            "        SignalFloat64 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF64\n"
            "          Type = float64\n"
            "        }\n"
            "        SignalUInt8A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU8A\n"
            "          Type = uint8\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt16A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU16A\n"
            "          Type = uint16\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt32A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU32A\n"
            "          Type = uint32\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt64A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU64A\n"
            "          Type = uint64\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt8A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI8A\n"
            "          Type = int8\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt16A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI16A\n"
            "          Type = int16\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt32A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI32A\n"
            "          Type = int32\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt64A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI64A\n"
            "          Type = int64\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat32A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF32A\n"
            "          Type = float32\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat64A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF64A\n"
            "          Type = float64\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMWriterTimestamp1 GAMWriterScalar GAMWriterArray}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = OPCUADSOutputSchedulerTestHelper\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
            "}\n";
    config.Seek(0LLU);
    //HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterScalar;
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterArray;
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterTimestamp1;
    if (ok) {
        gamWriterScalar = ord->Find("TestApp.Functions.GAMWriterScalar");
        ok = gamWriterScalar.IsValid();
    }
    if (ok) {
        gamWriterArray = ord->Find("TestApp.Functions.GAMWriterArray");
        ok = gamWriterArray.IsValid();
    }
    if (ok) {
        gamWriterTimestamp1 = ord->Find("TestApp.Functions.GAMWriterTimestamp1");
        ok = gamWriterTimestamp1.IsValid();
    }
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    ReferenceT<OPCUADSOutputSchedulerTestHelper> scheduler;
    if (ok) {
        scheduler = ord->Find("TestApp.Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }

    OPCUADSOutputTestReader opcuaReader;
    const uint32 numberOfNodes = 20;
    const char8 *nodeNames[] = {"NodeU8", "NodeU16", "NodeU32", "NodeU64", "NodeI8", "NodeI16", "NodeI32", "NodeI64", "NodeF32", "NodeF64", "NodeU8A", "NodeU16A", "NodeU32A", "NodeU64A", "NodeI8A", "NodeI16A", "NodeI32A", "NodeI64A", "NodeF32A", "NodeF64A"};
    uint32 nodeNElements[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    uint32 nodeTypeSizes[] = {sizeof(uint8), sizeof(uint16), sizeof(uint32), sizeof(uint64), sizeof(int8), sizeof(int16), sizeof(int32), sizeof(int64), sizeof(float32), sizeof(float64), sizeof(uint8), sizeof(uint16), sizeof(uint32), sizeof(uint64), sizeof(int8), sizeof(int16), sizeof(int32), sizeof(int64), sizeof(float32), sizeof(float64)};
    UA_NodeId nodeIds[numberOfNodes];
    const UA_DataType *nodeDataTypes[] = {&UA_TYPES[UA_TYPES_BYTE], &UA_TYPES[UA_TYPES_UINT16], &UA_TYPES[UA_TYPES_UINT32], &UA_TYPES[UA_TYPES_UINT64], &UA_TYPES[UA_TYPES_SBYTE], &UA_TYPES[UA_TYPES_INT16], &UA_TYPES[UA_TYPES_INT32], &UA_TYPES[UA_TYPES_INT64], &UA_TYPES[UA_TYPES_FLOAT], &UA_TYPES[UA_TYPES_DOUBLE], &UA_TYPES[UA_TYPES_BYTE], &UA_TYPES[UA_TYPES_UINT16], &UA_TYPES[UA_TYPES_UINT32], &UA_TYPES[UA_TYPES_UINT64], &UA_TYPES[UA_TYPES_SBYTE], &UA_TYPES[UA_TYPES_INT16], &UA_TYPES[UA_TYPES_INT32], &UA_TYPES[UA_TYPES_INT64], &UA_TYPES[UA_TYPES_FLOAT], &UA_TYPES[UA_TYPES_DOUBLE]};
    void *expectedNodeValues[] = {gamWriterScalar->uint8Signal, gamWriterScalar->uint16Signal,gamWriterScalar->uint32Signal, gamWriterScalar->uint64Signal, gamWriterScalar->int8Signal, gamWriterScalar->int16Signal, gamWriterScalar->int32Signal, gamWriterScalar->int64Signal, gamWriterScalar->float32Signal, gamWriterScalar->float64Signal, gamWriterArray->uint8Signal, gamWriterArray->uint16Signal,gamWriterArray->uint32Signal, gamWriterArray->uint64Signal, gamWriterArray->int8Signal, gamWriterArray->int16Signal, gamWriterArray->int32Signal, gamWriterArray->int64Signal, gamWriterArray->float32Signal, gamWriterArray->float64Signal};

    if (ok) {
        ok = opcuaReader.Connect();
    }
    if (ok) {
        for (uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            ok = (opcuaReader.FindChildNodeId(nodeNames[k], &nodeIds[k]) == UA_STATUSCODE_GOOD);
        }
    }
    if (ok) {
        *gamWriterTimestamp1->uint64Signal = 1002003499;

        *gamWriterScalar->uint8Signal = 1u;
        *gamWriterScalar->uint16Signal = 2u;
        *gamWriterScalar->uint32Signal = 3u;
        *gamWriterScalar->uint64Signal = 4u;
        *gamWriterScalar->int8Signal = 1;
        *gamWriterScalar->int16Signal = 2;
        *gamWriterScalar->int32Signal = 3;
        *gamWriterScalar->int64Signal = 5;
        *gamWriterScalar->float32Signal = 3.2;
        *gamWriterScalar->float64Signal = 6.2;

        gamWriterArray->uint8Signal[0] = 3;
        gamWriterArray->uint8Signal[1] = 1;
        gamWriterArray->uint8Signal[2] = 2;
        gamWriterArray->uint16Signal[0] = 4;
        gamWriterArray->uint16Signal[1] = 2;
        gamWriterArray->uint16Signal[2] = 3;
        gamWriterArray->uint32Signal[0] = 5;
        gamWriterArray->uint32Signal[1] = 3;
        gamWriterArray->uint32Signal[2] = 4;
        gamWriterArray->uint64Signal[0] = 6;
        gamWriterArray->uint64Signal[1] = 4;
        gamWriterArray->uint64Signal[2] = 5;
        gamWriterArray->int8Signal[0] = -3;
        gamWriterArray->int8Signal[1] = 2;
        gamWriterArray->int8Signal[2] = -2;
        gamWriterArray->int16Signal[0] = -4;
        gamWriterArray->int16Signal[1] = 3;
        gamWriterArray->int16Signal[2] = -3;
        gamWriterArray->int32Signal[0] = -5;
        gamWriterArray->int32Signal[1] = 4;
        gamWriterArray->int32Signal[2] = -4;
        gamWriterArray->int64Signal[0] = -6;
        gamWriterArray->int64Signal[1] = 5;
        gamWriterArray->int64Signal[2] = -5;
        gamWriterArray->float32Signal[0] = 3.1;
        gamWriterArray->float32Signal[1] = -3.2;
        gamWriterArray->float32Signal[2] = 3.3;
        gamWriterArray->float64Signal[0] = -6.1;
        gamWriterArray->float64Signal[1] = 6.2;
        gamWriterArray->float64Signal[2] = -6.3;
        scheduler->ExecuteThreadCycle(0u);
        for(uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            //printf("Checking %d %s %d\n", k, nodeNames[k], nodeNElements[k]);
            ok = opcuaReader.WaitForValue(nodeIds[k], reinterpret_cast<uint8 *>(expectedNodeValues[k]), nodeDataTypes[k], nodeTypeSizes[k], nodeNElements[k]);
        }
        for(uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            ok = opcuaReader.CheckTimestamp(nodeIds[k], *gamWriterTimestamp1->uint64Signal, true);
        }
    }
    if (ok) {
        *gamWriterTimestamp1->uint64Signal = 1772206459535547039;
        scheduler->ExecuteThreadCycle(0u);
        for(uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            ok = opcuaReader.CheckTimestamp(nodeIds[k], *gamWriterTimestamp1->uint64Signal, true);
        }
    }
    //Sleep::Sec(360);
    opcuaReader.Disconnect();
    ord->Purge();

    return ok;
}

bool OPCUADSOutputTest::Test_Synchronise_Timestamp_Default_Mix() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {\n"
            "  Class = OPCUA::OPCUAServer\n"
            "  Authentication = None\n"
            "  CPUMask = 0x2\n"
            "  AddressSpace = {\n"
            "    NodeU8 = {\n"
            "      Type = uint8\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "    NodeU16 = {\n"
            "      Type = uint16\n"
            "    }\n"
            "    NodeU32 = {\n"
            "      Type = uint32\n"
            "    }\n"
            "    NodeU64 = {\n"
            "      Type = uint64\n"
            "    }\n"
            "    NodeI8 = {\n"
            "      Type = int8\n"
            "    }\n"
            "    NodeI16 = {\n"
            "      Type = int16\n"
            "    }\n"
            "    NodeI32 = {\n"
            "      Type = int32\n"
            "    }\n"
            "    NodeI64 = {\n"
            "      Type = int64\n"
            "    }\n"
            "    NodeF32 = {\n"
            "      Type = float32\n"
            "    }\n"
            "    NodeF64 = {\n"
            "      Type = float32\n"
            "    }\n"
            "    NodeU8A = {\n"
            "      Type = uint8\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeU16A = {\n"
            "      Type = uint16\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeU32A = {\n"
            "      Type = uint32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeU64A = {\n"
            "      Type = uint64\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI8A = {\n"
            "      Type = int8\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI16A = {\n"
            "      Type = int16\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI32A = {\n"
            "      Type = int32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeI64A = {\n"
            "      Type = int64\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeF32A = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "    NodeF64A = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 3\n"
            "    }\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "  +Functions = {\n"
            "    Class = ReferenceContainer\n"
            "    +GAMWriterTimestamp1 = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        Timestamp1 = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMWriterTimestamp2 = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        Timestamp2 = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMWriterScalar = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        SignalUInt8 = {\n"
            "          Type = uint8\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalUInt16 = {\n"
            "          Type = uint16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalUInt32 = {\n"
            "          Type = uint32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalUInt64 = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt8 = {\n"
            "          Type = int8\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt16 = {\n"
            "          Type = int16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt32 = {\n"
            "          Type = int32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalInt64 = {\n"
            "          Type = int64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalFloat32 = {\n"
            "          Type = float32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "        SignalFloat64 = {\n"
            "          Type = float64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMWriterArray = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        SignalUInt8A = {\n"
            "          Type = uint8\n"
            "          Trigger = 1\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt16A = {\n"
            "          Type = uint16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt32A = {\n"
            "          Type = uint32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt64A = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt8A = {\n"
            "          Type = int8\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt16A = {\n"
            "          Type = int16\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt32A = {\n"
            "          Type = int32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt64A = {\n"
            "          Type = int64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat32A = {\n"
            "          Type = float32\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat64A = {\n"
            "          Type = float64\n"
            "          DataSource = OPCUADSOutputTest\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +LoggerDataSource = {\n"
            "      Class = LoggerDataSource\n"
            "    }\n"
            "    +OPCUADSOutputTest = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\"\n"
            "      Authentication = None\n"
            "      Signals = {\n"
            "        SignalUInt8 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU8\n"
            "          Type = uint8\n"
            "        }\n"
            "        SignalUInt16 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU16\n"
            "          Type = uint16\n"
            "        }\n"
            "        SignalUInt32 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU32\n"
            "          Type = uint32\n"
            "        }\n"
            "        SignalUInt64 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU64\n"
            "          Type = uint64\n"
            "        }\n"
            "        Timestamp2 = {\n"
            "          Timestamp = {\"SignalUInt8\" \"SignalUInt64\" \"SignalFloat64A\"}\n"
            "          Type = uint64\n"
            "        }\n"
            "        SignalInt8 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI8\n"
            "          Type = int8\n"
            "        }\n"
            "        SignalInt16 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI16\n"
            "          Type = int16\n"
            "        }\n"
            "        SignalInt32 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI32\n"
            "          Type = int32\n"
            "        }\n"
            "        SignalInt64 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI64\n"
            "          Type = int64\n"
            "        }\n"
            "        SignalFloat32 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF32\n"
            "          Type = float32\n"
            "        }\n"
            "        SignalFloat64 = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF64\n"
            "          Type = float64\n"
            "        }\n"
            "        SignalUInt8A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU8A\n"
            "          Type = uint8\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt16A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU16A\n"
            "          Type = uint16\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt32A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU32A\n"
            "          Type = uint32\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalUInt64A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeU64A\n"
            "          Type = uint64\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt8A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI8A\n"
            "          Type = int8\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt16A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI16A\n"
            "          Type = int16\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt32A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI32A\n"
            "          Type = int32\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalInt64A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeI64A\n"
            "          Type = int64\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat32A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF32A\n"
            "          Type = float32\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        SignalFloat64A = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = NodeF64A\n"
            "          Type = float64\n"
            "          NumberOfElements = 3\n"
            "        }\n"
            "        Timestamp1 = {\n"
            "          DefaultTimestampSignal = 1\n"
            "          Type = uint64\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +Timings = {\n"
            "       Class = TimingDataSource\n"
            "    }\n"
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          CPUs = 0x2\n"
            "          Functions = {GAMWriterTimestamp1 GAMWriterTimestamp2 GAMWriterScalar GAMWriterArray}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = OPCUADSOutputSchedulerTestHelper\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
            "}\n";
    config.Seek(0LLU);
    //HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    Sleep::MSec(200);
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterScalar;
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterArray;
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterTimestamp1;
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterTimestamp2;
    if (ok) {
        gamWriterScalar = ord->Find("TestApp.Functions.GAMWriterScalar");
        ok = gamWriterScalar.IsValid();
    }
    if (ok) {
        gamWriterArray = ord->Find("TestApp.Functions.GAMWriterArray");
        ok = gamWriterArray.IsValid();
    }
    if (ok) {
        gamWriterTimestamp1 = ord->Find("TestApp.Functions.GAMWriterTimestamp1");
        ok = gamWriterTimestamp1.IsValid();
    }
    if (ok) {
        gamWriterTimestamp2 = ord->Find("TestApp.Functions.GAMWriterTimestamp2");
        ok = gamWriterTimestamp2.IsValid();
    }
    if (ok) {
        ok = app->PrepareNextState("State1");
    }
    ReferenceT<OPCUADSOutputSchedulerTestHelper> scheduler;
    if (ok) {
        scheduler = ord->Find("TestApp.Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }

    OPCUADSOutputTestReader opcuaReader;
    const uint32 numberOfNodes = 20;
    const char8 *nodeNames[] = {"NodeU8", "NodeU16", "NodeU32", "NodeU64", "NodeI8", "NodeI16", "NodeI32", "NodeI64", "NodeF32", "NodeF64", "NodeU8A", "NodeU16A", "NodeU32A", "NodeU64A", "NodeI8A", "NodeI16A", "NodeI32A", "NodeI64A", "NodeF32A", "NodeF64A"};
    uint32 nodeNElements[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    uint32 nodeTypeSizes[] = {sizeof(uint8), sizeof(uint16), sizeof(uint32), sizeof(uint64), sizeof(int8), sizeof(int16), sizeof(int32), sizeof(int64), sizeof(float32), sizeof(float64), sizeof(uint8), sizeof(uint16), sizeof(uint32), sizeof(uint64), sizeof(int8), sizeof(int16), sizeof(int32), sizeof(int64), sizeof(float32), sizeof(float64)};
    UA_NodeId nodeIds[numberOfNodes];
    const UA_DataType *nodeDataTypes[] = {&UA_TYPES[UA_TYPES_BYTE], &UA_TYPES[UA_TYPES_UINT16], &UA_TYPES[UA_TYPES_UINT32], &UA_TYPES[UA_TYPES_UINT64], &UA_TYPES[UA_TYPES_SBYTE], &UA_TYPES[UA_TYPES_INT16], &UA_TYPES[UA_TYPES_INT32], &UA_TYPES[UA_TYPES_INT64], &UA_TYPES[UA_TYPES_FLOAT], &UA_TYPES[UA_TYPES_DOUBLE], &UA_TYPES[UA_TYPES_BYTE], &UA_TYPES[UA_TYPES_UINT16], &UA_TYPES[UA_TYPES_UINT32], &UA_TYPES[UA_TYPES_UINT64], &UA_TYPES[UA_TYPES_SBYTE], &UA_TYPES[UA_TYPES_INT16], &UA_TYPES[UA_TYPES_INT32], &UA_TYPES[UA_TYPES_INT64], &UA_TYPES[UA_TYPES_FLOAT], &UA_TYPES[UA_TYPES_DOUBLE]};
    void *expectedNodeValues[] = {gamWriterScalar->uint8Signal, gamWriterScalar->uint16Signal,gamWriterScalar->uint32Signal, gamWriterScalar->uint64Signal, gamWriterScalar->int8Signal, gamWriterScalar->int16Signal, gamWriterScalar->int32Signal, gamWriterScalar->int64Signal, gamWriterScalar->float32Signal, gamWriterScalar->float64Signal, gamWriterArray->uint8Signal, gamWriterArray->uint16Signal,gamWriterArray->uint32Signal, gamWriterArray->uint64Signal, gamWriterArray->int8Signal, gamWriterArray->int16Signal, gamWriterArray->int32Signal, gamWriterArray->int64Signal, gamWriterArray->float32Signal, gamWriterArray->float64Signal};

    if (ok) {
        ok = opcuaReader.Connect();
    }
    if (ok) {
        for (uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            ok = (opcuaReader.FindChildNodeId(nodeNames[k], &nodeIds[k]) == UA_STATUSCODE_GOOD);
        }
    }
    if (ok) {
        *gamWriterTimestamp1->uint64Signal = 1002003499;
        *gamWriterTimestamp2->uint64Signal = 2003004501;

        *gamWriterScalar->uint8Signal = 1u;
        *gamWriterScalar->uint16Signal = 2u;
        *gamWriterScalar->uint32Signal = 3u;
        *gamWriterScalar->uint64Signal = 4u;
        *gamWriterScalar->int8Signal = 1;
        *gamWriterScalar->int16Signal = 2;
        *gamWriterScalar->int32Signal = 3;
        *gamWriterScalar->int64Signal = 5;
        *gamWriterScalar->float32Signal = 3.2;
        *gamWriterScalar->float64Signal = 6.2;

        gamWriterArray->uint8Signal[0] = 3;
        gamWriterArray->uint8Signal[1] = 1;
        gamWriterArray->uint8Signal[2] = 2;
        gamWriterArray->uint16Signal[0] = 4;
        gamWriterArray->uint16Signal[1] = 2;
        gamWriterArray->uint16Signal[2] = 3;
        gamWriterArray->uint32Signal[0] = 5;
        gamWriterArray->uint32Signal[1] = 3;
        gamWriterArray->uint32Signal[2] = 4;
        gamWriterArray->uint64Signal[0] = 6;
        gamWriterArray->uint64Signal[1] = 4;
        gamWriterArray->uint64Signal[2] = 5;
        gamWriterArray->int8Signal[0] = -3;
        gamWriterArray->int8Signal[1] = 2;
        gamWriterArray->int8Signal[2] = -2;
        gamWriterArray->int16Signal[0] = -4;
        gamWriterArray->int16Signal[1] = 3;
        gamWriterArray->int16Signal[2] = -3;
        gamWriterArray->int32Signal[0] = -5;
        gamWriterArray->int32Signal[1] = 4;
        gamWriterArray->int32Signal[2] = -4;
        gamWriterArray->int64Signal[0] = -6;
        gamWriterArray->int64Signal[1] = 5;
        gamWriterArray->int64Signal[2] = -5;
        gamWriterArray->float32Signal[0] = 3.1;
        gamWriterArray->float32Signal[1] = -3.2;
        gamWriterArray->float32Signal[2] = 3.3;
        gamWriterArray->float64Signal[0] = -6.1;
        gamWriterArray->float64Signal[1] = 6.2;
        gamWriterArray->float64Signal[2] = -6.3;
        scheduler->ExecuteThreadCycle(0u);
        for(uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            //printf("Checking %d %s %d\n", k, nodeNames[k], nodeNElements[k]);
            ok = opcuaReader.WaitForValue(nodeIds[k], reinterpret_cast<uint8 *>(expectedNodeValues[k]), nodeDataTypes[k], nodeTypeSizes[k], nodeNElements[k]);
        }
        for(uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            if (k == 0 || k == 3 || k == 19) {
                ok = opcuaReader.CheckTimestamp(nodeIds[k], *gamWriterTimestamp2->uint64Signal, true);
            }
            else {
                ok = opcuaReader.CheckTimestamp(nodeIds[k], *gamWriterTimestamp1->uint64Signal, true);
            }
        }
    }
    if (ok) {
        *gamWriterTimestamp1->uint64Signal = 1772206459535547039;
        *gamWriterTimestamp2->uint64Signal = 1193227200123456789;
        scheduler->ExecuteThreadCycle(0u);
        for(uint32 k=0; (k<numberOfNodes) && (ok); k++) {
            if (k == 0 || k == 3 || k == 19) {
                ok = opcuaReader.CheckTimestamp(nodeIds[k], *gamWriterTimestamp2->uint64Signal, true);
            }
            else {
                ok = opcuaReader.CheckTimestamp(nodeIds[k], *gamWriterTimestamp1->uint64Signal, true);
            }
        }
    }

    //Sleep::Sec(360);
    opcuaReader.Disconnect();
    ord->Purge();

    return ok;
}

bool OPCUADSOutputTest::Test_Synchronise_ExtensionObject_Timestamp_Idx_0() {
    using namespace MARTe;
    OPCUATestServer ots;
    ots.service.Start();
    StreamString config = ""
            "+OPCUATypes = {\n"
            "  Class = ReferenceContainer\n"
            "  +Point = {\n"
            "    Class = IntrospectionStructure\n"
            "    x = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "    y = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "    z = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "  }\n"
            "}\n"
            "+MessageX = {\n"
            "  Class = Message\n"
            "  Destination = \"TestApp.Functions.GAMXYZ\"\n"
            "  Function = \"SetOutput\"\n"
            "  +Parameters = {\n"
            "    Class = ConfigurationDatabase\n"
            "    SignalName = \"x\"\n"
            "    SignalValue = -6.7\n"
            "  }\n"
            "}\n"
            "+MessageY = {\n"
            "  Class = Message\n"
            "  Destination = \"TestApp.Functions.GAMXYZ\"\n"
            "  Function = \"SetOutput\"\n"
            "  +Parameters = {\n"
            "    Class = ConfigurationDatabase\n"
            "    SignalName = \"y\"\n"
            "    SignalValue = -8.9\n"
            "  }\n"
            "}\n"
            "+MessageZ = {\n"
            "  Class = Message\n"
            "  Destination = \"TestApp.Functions.GAMXYZ\"\n"
            "  Function = \"SetOutput\"\n"
            "  +Parameters = {\n"
            "    Class = ConfigurationDatabase\n"
            "    SignalName = \"z\"\n"
            "    SignalValue = 9.0\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "  +Functions = {\n"
            "    Class = ReferenceContainer\n"
            "    +GAMWriterTimestamp1 = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        Timestamp1 = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUAOut\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMXYZ = {\n"
            "      Class = ConstantGAM\n"
            "      OutputSignals = {\n"
            "        x = {\n"
            "          DataSource = DDB1\n"
            "          Type = float32\n"
            "          Default = 1.2\n"
            "        }\n"
            "        y = {\n"
            "          DataSource = DDB1\n"
            "          Type = float32\n"
            "          Default = 3.4\n"
            "        }\n"
            "        z = {\n"
            "          DataSource = DDB1\n"
            "          Type = float32\n"
            "          Default = -5.6\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMWriter = {\n"
            "      Class = IOGAM\n"
            "      InputSignals = {\n"
            "        x = {\n"
            "          Type = float32\n"
            "          DataSource = DDB1\n"
            "        }\n"
            "        y = {\n"
            "          Type = float32\n"
            "          DataSource = DDB1\n"
            "        }\n"
            "        z = {\n"
            "          Type = float32\n"
            "          DataSource = DDB1\n"
            "        }\n"
            "      }\n"
            "      OutputSignals = {\n"
            "        Point = {\n"
            "          Type = Point\n"
            "          DataSource = OPCUAOut\n"
            "          Trigger = 1\n"
            "          TriggerSignal = \"Point.x\"\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +Timings = {\n"
            "      Class = TimingDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\""
            "      Signals = {\n"
            "        Timestamp1 = {\n"
            "          DefaultTimestampSignal = 1\n"
            "          Type = uint64\n"
            "        }\n"
            "        Point = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = Point\n"
            "          Type = Point\n"
            "          ExtensionObject = \"yes\"\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          Functions = {GAMXYZ GAMWriterTimestamp1 GAMWriter}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = OPCUADSOutputSchedulerTestHelper\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    if (ok) {
        app->PrepareNextState("State1");
    }
    ReferenceT<OPCUADSOutputSchedulerTestHelper> scheduler;
    if (ok) {
        scheduler = ord->Find("TestApp.Scheduler");
        ok = scheduler.IsValid();
    }
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterTimestamp1;
    if (ok) {
        gamWriterTimestamp1 = ord->Find("TestApp.Functions.GAMWriterTimestamp1");
        ok = gamWriterTimestamp1.IsValid();
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    if (ok) {
        *gamWriterTimestamp1->uint64Signal = 1002003499;
        scheduler->ExecuteThreadCycle(0u);
    }
    Sleep::Sec(0.2);
    OPCUADSOutputTestReader opcuaReader;
    if (ok) {
        ok = opcuaReader.Connect();
    }
    const char8 *nodeName = "Point";
    UA_NodeId nodeId;

    if (ok) {
        ok = (opcuaReader.FindChildNodeId(nodeName, &nodeId) == UA_STATUSCODE_GOOD);
    }
    if (ok) {
        float32 expectedValue[] = {1.2, 3.4, -5.6};
        ok = opcuaReader.WaitForExtObjValue(nodeId, reinterpret_cast<uint8 *>(&expectedValue[0]), sizeof(float32) * 3);
    }
    if (ok) {
        ok = opcuaReader.CheckTimestamp(nodeId, *gamWriterTimestamp1->uint64Signal, true);
    }
    ReferenceT<Message> message;
    if (ok) {
        message = ord->Find("MessageX");
        MessageI::SendMessage(message, NULL);
    }
    if (ok) {
        message = ord->Find("MessageY");
        MessageI::SendMessage(message, NULL);
    }
    if (ok) {
        message = ord->Find("MessageZ");
        MessageI::SendMessage(message, NULL);
    }
    if (ok) {
        *gamWriterTimestamp1->uint64Signal = 1193227200123456789;
        scheduler->ExecuteThreadCycle(0u);
    }
    Sleep::Sec(0.2);

    if (ok) {
        ok = (opcuaReader.FindChildNodeId(nodeName, &nodeId) == UA_STATUSCODE_GOOD);//Need to repeat because the CheckTimestamp is invalidating the nodeid
    }
    if (ok) {
        float32 expectedValue[] = {-6.7, -8.9, 9.0};
        ok = opcuaReader.WaitForExtObjValue(nodeId, reinterpret_cast<uint8 *>(&expectedValue[0]), sizeof(float32) * 3);
    }
    if (ok) {
        ok = opcuaReader.CheckTimestamp(nodeId, *gamWriterTimestamp1->uint64Signal, true);
    }
    opcuaReader.Disconnect();
    ots.SetRunning(false);
    ots.service.Stop();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool OPCUADSOutputTest::Test_Synchronise_ExtensionObject_Timestamp_Idx_Last() {
    using namespace MARTe;
    OPCUATestServer ots;
    ots.service.Start();
    StreamString config = ""
            "+OPCUATypes = {\n"
            "  Class = ReferenceContainer\n"
            "  +Point = {\n"
            "    Class = IntrospectionStructure\n"
            "    x = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "    y = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "    z = {\n"
            "      Type = float32\n"
            "      NumberOfElements = 1\n"
            "    }\n"
            "  }\n"
            "}\n"
            "+MessageX = {\n"
            "  Class = Message\n"
            "  Destination = \"TestApp.Functions.GAMXYZ\"\n"
            "  Function = \"SetOutput\"\n"
            "  +Parameters = {\n"
            "    Class = ConfigurationDatabase\n"
            "    SignalName = \"x\"\n"
            "    SignalValue = -6.7\n"
            "  }\n"
            "}\n"
            "+MessageY = {\n"
            "  Class = Message\n"
            "  Destination = \"TestApp.Functions.GAMXYZ\"\n"
            "  Function = \"SetOutput\"\n"
            "  +Parameters = {\n"
            "    Class = ConfigurationDatabase\n"
            "    SignalName = \"y\"\n"
            "    SignalValue = -8.9\n"
            "  }\n"
            "}\n"
            "+MessageZ = {\n"
            "  Class = Message\n"
            "  Destination = \"TestApp.Functions.GAMXYZ\"\n"
            "  Function = \"SetOutput\"\n"
            "  +Parameters = {\n"
            "    Class = ConfigurationDatabase\n"
            "    SignalName = \"z\"\n"
            "    SignalValue = 9.0\n"
            "  }\n"
            "}\n"
            "$TestApp = {\n"
            "  Class = RealTimeApplication\n"
            "  +Functions = {\n"
            "    Class = ReferenceContainer\n"
            "    +GAMWriterTimestamp1 = {\n"
            "      Class = OPCUADSOutputGAMTestHelper\n"
            "      OutputSignals = {\n"
            "        Timestamp1 = {\n"
            "          Type = uint64\n"
            "          DataSource = OPCUAOut\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMXYZ = {\n"
            "      Class = ConstantGAM\n"
            "      OutputSignals = {\n"
            "        x = {\n"
            "          DataSource = DDB1\n"
            "          Type = float32\n"
            "          Default = 1.2\n"
            "        }\n"
            "        y = {\n"
            "          DataSource = DDB1\n"
            "          Type = float32\n"
            "          Default = 3.4\n"
            "        }\n"
            "        z = {\n"
            "          DataSource = DDB1\n"
            "          Type = float32\n"
            "          Default = -5.6\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "    +GAMWriter = {\n"
            "      Class = IOGAM\n"
            "      InputSignals = {\n"
            "        x = {\n"
            "          Type = float32\n"
            "          DataSource = DDB1\n"
            "        }\n"
            "        y = {\n"
            "          Type = float32\n"
            "          DataSource = DDB1\n"
            "        }\n"
            "        z = {\n"
            "          Type = float32\n"
            "          DataSource = DDB1\n"
            "        }\n"
            "      }\n"
            "      OutputSignals = {\n"
            "        Point = {\n"
            "          Type = Point\n"
            "          DataSource = OPCUAOut\n"
            "          Trigger = 1\n"
            "          TriggerSignal = \"Point.x\"\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Data = {\n"
            "    Class = ReferenceContainer\n"
            "    DefaultDataSource = DDB1\n"
            "    +DDB1 = {\n"
            "      Class = GAMDataSource\n"
            "    }\n"
            "    +Timings = {\n"
            "      Class = TimingDataSource\n"
            "    }\n"
            "    +OPCUAOut = {\n"
            "      Class = OPCUADataSource::OPCUADSOutput\n"
            "      Address = \"opc.tcp://localhost.localdomain:4840\""
            "      Signals = {\n"
            "        Point = {\n"
            "          NamespaceIndex = 1\n"
            "          Path = Point\n"
            "          Type = Point\n"
            "          ExtensionObject = \"yes\"\n"
            "        }\n"
            "        Timestamp1 = {\n"
            "          DefaultTimestampSignal = 1\n"
            "          Type = uint64\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +States = {\n"
            "    Class = ReferenceContainer\n"
            "    +State1 = {\n"
            "      Class = RealTimeState\n"
            "      +Threads = {\n"
            "        Class = ReferenceContainer\n"
            "        +Thread1 = {\n"
            "          Class = RealTimeThread\n"
            "          Functions = {GAMXYZ GAMWriterTimestamp1 GAMWriter}\n"
            "        }\n"
            "      }\n"
            "    }\n"
            "  }\n"
            "  +Scheduler = {\n"
            "    Class = OPCUADSOutputSchedulerTestHelper\n"
            "    TimingDataSource = Timings\n"
            "  }\n"
            "}\n";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ord->Find("TestApp");
        ok = app.IsValid();
    }
    if (ok) {
        ok = app->ConfigureApplication();
    }
    if (ok) {
        app->PrepareNextState("State1");
    }
    ReferenceT<OPCUADSOutputSchedulerTestHelper> scheduler;
    if (ok) {
        scheduler = ord->Find("TestApp.Scheduler");
        ok = scheduler.IsValid();
    }
    ReferenceT<OPCUADSOutputGAMTestHelper> gamWriterTimestamp1;
    if (ok) {
        gamWriterTimestamp1 = ord->Find("TestApp.Functions.GAMWriterTimestamp1");
        ok = gamWriterTimestamp1.IsValid();
    }
    if (ok) {
        ok = app->StartNextStateExecution();
    }
    if (ok) {
        *gamWriterTimestamp1->uint64Signal = 1002003499;
        scheduler->ExecuteThreadCycle(0u);
    }
    Sleep::Sec(0.2);
    OPCUADSOutputTestReader opcuaReader;
    if (ok) {
        ok = opcuaReader.Connect();
    }
    const char8 *nodeName = "Point";
    UA_NodeId nodeId;

    if (ok) {
        ok = (opcuaReader.FindChildNodeId(nodeName, &nodeId) == UA_STATUSCODE_GOOD);
    }
    if (ok) {
        float32 expectedValue[] = {1.2, 3.4, -5.6};
        ok = opcuaReader.WaitForExtObjValue(nodeId, reinterpret_cast<uint8 *>(&expectedValue[0]), sizeof(float32) * 3);
    }
    if (ok) {
        ok = opcuaReader.CheckTimestamp(nodeId, *gamWriterTimestamp1->uint64Signal, true);
    }
    ReferenceT<Message> message;
    if (ok) {
        message = ord->Find("MessageX");
        MessageI::SendMessage(message, NULL);
    }
    if (ok) {
        message = ord->Find("MessageY");
        MessageI::SendMessage(message, NULL);
    }
    if (ok) {
        message = ord->Find("MessageZ");
        MessageI::SendMessage(message, NULL);
    }
    if (ok) {
        *gamWriterTimestamp1->uint64Signal = 1193227200123456789;
        scheduler->ExecuteThreadCycle(0u);
    }
    Sleep::Sec(0.2);

    if (ok) {
        ok = (opcuaReader.FindChildNodeId(nodeName, &nodeId) == UA_STATUSCODE_GOOD);//Need to repeat because the CheckTimestamp is invalidating the nodeid
    }
    if (ok) {
        float32 expectedValue[] = {-6.7, -8.9, 9.0};
        ok = opcuaReader.WaitForExtObjValue(nodeId, reinterpret_cast<uint8 *>(&expectedValue[0]), sizeof(float32) * 3);
    }
    if (ok) {
        ok = opcuaReader.CheckTimestamp(nodeId, *gamWriterTimestamp1->uint64Signal, true);
    }
    opcuaReader.Disconnect();
    ots.SetRunning(false);
    ots.service.Stop();
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}
