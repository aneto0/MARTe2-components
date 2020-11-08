/**
 * @file OPCUAClientWriteTest.h
 * @brief Header file for class OPCUAClientWriteTest
 * @date 29/10/2019
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

 * @details This header file contains the declaration of the class OPCUAClientWriteTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRITETEST_H_
#define TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRITETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "custom_datatype.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "SingleThreadService.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

static const MARTe::char8 * const SET_WRITE_REQUEST_CONFIG_TEMPLATE = ""
"+ServerTest = {\n"
"  Class = OPCUA::OPCUAServer\n"
"  CPUMask = 0x2\n"
"  AddressSpace = {\n"
"    MyNode = {\n"
"      Type = %s\n"
"      NumberOfElements = %u\n"
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
"           DataSource = DDB1\n"
"           Type = uint32\n"
"         }\n"
"         Time = {\n"
"           DataSource = DDB1\n"
"           Type = uint32\n"
"         }\n"
"       }\n"
"     }\n"
"     +GAMValue = {\n"
"       Class = OPCUATestHelperGam\n"
"       OutputSignals = {\n"
"         MyNode = {\n"
"           DataSource = OPCUAOut\n"
"           Type = %s\n"
"           NumberOfElements = %u\n"
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
"      Signals = {\n"
"        MyNode = {\n"
"          NamespaceIndex = 1\n"
"          Path = MyNode\n"
"          NumberOfElements = %u\n"
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
"          Functions = {GAMTimer GAMValue}\n"
"        }\n"
"      }\n"
"    }\n"
"  }\n"
"  +Scheduler = {\n"
"    Class = GAMScheduler\n"
"    TimingDataSource = Timings\n"
"  }\n"
"}\n";



class OPCUAClientWriteTest {
public:
    /**
     * @brief Tests the constructor. NOOP
     */
    bool TestConstructor();

    bool Test_SetServiceRequest();

    bool Test_GetExtensionObjectByteString();

    bool Test_SetExtensionObject();

    /**
     * @brief Tests the SetWriteRequest method
     */
    bool Test_SetWriteRequest(const MARTe::char8 *typeUT, MARTe::uint8 numberOfElementsUT);

    /**
     * @brief Tests the SetServiceRequest method with a wrong string identifier that doesn't match any variable on the server.
     */
    bool Test_WrongNodeId();

    bool Test_Write_ExtensionObject();

    /**
     * Tested with Siemens PLC S7-1500 - Waiting for the next version of
     * open62541 to test it with a standalone server.
     */
    bool Test_Write_ExtensionObject_Complex();

    /**
     * Tested with Siemens PLC S7-1500 - Waiting for the next version of
     * open62541 to test it with a standalone server.
     */
    bool Test_Write_ExtensionObject_Array();

private:
    class OPCUATestServer: public MARTe::EmbeddedServiceMethodBinderI {
    public:
        OPCUATestServer() :
                EmbeddedServiceMethodBinderI(),
                service(*this) {
            running = true;
        }

        ~OPCUATestServer() {
            running = false;
        }

        MARTe::ErrorManagement::ErrorType Execute(MARTe::ExecutionInfo &info) {
            using namespace MARTe;
            ErrorManagement::ErrorType err = ErrorManagement::NoError;
            UA_Server *server = UA_Server_new();
            UA_ServerConfig *config = UA_Server_getConfig(server);
            UA_ServerConfig_setDefault(config);

            /* Make the custom datatype known to the stack */
            UA_DataType *types = (UA_DataType*) UA_malloc(sizeof(UA_DataType) * 11);

            UA_DataTypeMember *pointMembers = (UA_DataTypeMember*) UA_malloc(sizeof(UA_DataTypeMember) * 3);
            pointMembers[0] = Point_members[0];
            pointMembers[1] = Point_members[1];
            pointMembers[2] = Point_members[2];
            types[0] = PointType;
            types[0].members = pointMembers;

            UA_DataTypeMember *pointSByteMembers = (UA_DataTypeMember*) UA_malloc(sizeof(UA_DataTypeMember) * 3);
            pointSByteMembers[0] = PointArraySByte_members[0];
            pointSByteMembers[1] = PointArraySByte_members[1];
            pointSByteMembers[2] = PointArraySByte_members[2];
            types[1] = PointArraySByteType;
            types[1].members = pointSByteMembers;

            UA_DataTypeMember *pointByteMembers = (UA_DataTypeMember*) UA_malloc(sizeof(UA_DataTypeMember) * 3);
            pointByteMembers[0] = PointArrayByte_members[0];
            pointByteMembers[1] = PointArrayByte_members[1];
            pointByteMembers[2] = PointArrayByte_members[2];
            types[2] = PointArrayByteType;
            types[2].members = pointByteMembers;

            UA_DataTypeMember *pointInt16Members = (UA_DataTypeMember*) UA_malloc(sizeof(UA_DataTypeMember) * 3);
            pointInt16Members[0] = PointArrayInt16_members[0];
            pointInt16Members[1] = PointArrayInt16_members[1];
            pointInt16Members[2] = PointArrayInt16_members[2];
            types[3] = PointArrayInt16Type;
            types[3].members = pointInt16Members;

            UA_DataTypeMember *pointUInt16Members = (UA_DataTypeMember*) UA_malloc(sizeof(UA_DataTypeMember) * 3);
            pointUInt16Members[0] = PointArrayUInt16_members[0];
            pointUInt16Members[1] = PointArrayUInt16_members[1];
            pointUInt16Members[2] = PointArrayUInt16_members[2];
            types[4] = PointArrayUInt16Type;
            types[4].members = pointUInt16Members;

            UA_DataTypeMember *pointInt32Members = (UA_DataTypeMember*) UA_malloc(sizeof(UA_DataTypeMember) * 3);
            pointInt32Members[0] = PointArrayInt32_members[0];
            pointInt32Members[1] = PointArrayInt32_members[1];
            pointInt32Members[2] = PointArrayInt32_members[2];
            types[5] = PointArrayInt32Type;
            types[5].members = pointInt32Members;

            UA_DataTypeMember *pointUInt32Members = (UA_DataTypeMember*) UA_malloc(sizeof(UA_DataTypeMember) * 3);
            pointUInt32Members[0] = PointArrayUInt32_members[0];
            pointUInt32Members[1] = PointArrayUInt32_members[1];
            pointUInt32Members[2] = PointArrayUInt32_members[2];
            types[6] = PointArrayUInt32Type;
            types[6].members = pointUInt32Members;

            UA_DataTypeMember *pointInt64Members = (UA_DataTypeMember*) UA_malloc(sizeof(UA_DataTypeMember) * 3);
            pointInt64Members[0] = PointArrayInt64_members[0];
            pointInt64Members[1] = PointArrayInt64_members[1];
            pointInt64Members[2] = PointArrayInt64_members[2];
            types[7] = PointArrayInt64Type;
            types[7].members = pointInt64Members;

            UA_DataTypeMember *pointUInt64Members = (UA_DataTypeMember*) UA_malloc(sizeof(UA_DataTypeMember) * 3);
            pointUInt64Members[0] = PointArrayUInt64_members[0];
            pointUInt64Members[1] = PointArrayUInt64_members[1];
            pointUInt64Members[2] = PointArrayUInt64_members[2];
            types[8] = PointArrayUInt64Type;
            types[8].members = pointUInt64Members;

            UA_DataTypeMember *pointFloatMembers = (UA_DataTypeMember*) UA_malloc(sizeof(UA_DataTypeMember) * 3);
            pointFloatMembers[0] = PointArrayFloat_members[0];
            pointFloatMembers[1] = PointArrayFloat_members[1];
            pointFloatMembers[2] = PointArrayFloat_members[2];
            types[9] = PointArrayFloatType;
            types[9].members = pointFloatMembers;

            UA_DataTypeMember *pointDoubleMembers = (UA_DataTypeMember*) UA_malloc(sizeof(UA_DataTypeMember) * 3);
            pointDoubleMembers[0] = PointArrayDouble_members[0];
            pointDoubleMembers[1] = PointArrayDouble_members[1];
            pointDoubleMembers[2] = PointArrayDouble_members[2];
            types[10] = PointArrayDoubleType;
            types[10].members = pointDoubleMembers;

            /* Initialising Point Arrays */
            PointArraySByte pSByte;
            UA_SByte *zArraySByte = (UA_SByte*) UA_Array_new(2, &UA_TYPES[UA_TYPES_SBYTE]);
            pSByte.zLength = 2;
            pSByte.z = zArraySByte;

            PointArrayByte pByte;
            UA_Byte *zArrayByte = (UA_Byte*) UA_Array_new(2, &UA_TYPES[UA_TYPES_BYTE]);
            pByte.zLength = 2;
            pByte.z = zArrayByte;

            PointArrayInt16 pInt16;
            UA_Int16 *zArrayInt16 = (UA_Int16*) UA_Array_new(2, &UA_TYPES[UA_TYPES_INT16]);
            pInt16.zLength = 2;
            pInt16.z = zArrayInt16;

            PointArrayUInt16 pUInt16;
            UA_UInt16 *zArrayUInt16 = (UA_UInt16*) UA_Array_new(2, &UA_TYPES[UA_TYPES_UINT16]);
            pUInt16.zLength = 2;
            pUInt16.z = zArrayUInt16;

            PointArrayInt32 pInt32;
            UA_Int32 *zArrayInt32 = (UA_Int32*) UA_Array_new(2, &UA_TYPES[UA_TYPES_INT32]);
            pInt32.zLength = 2;
            pInt32.z = zArrayInt32;

            PointArrayUInt32 pUInt32;
            UA_UInt32 *zArrayUInt32 = (UA_UInt32*) UA_Array_new(2, &UA_TYPES[UA_TYPES_UINT32]);
            pUInt32.zLength = 2;
            pUInt32.z = zArrayUInt32;

            PointArrayInt64 pInt64;
            UA_Int64 *zArrayInt64 = (UA_Int64*) UA_Array_new(2, &UA_TYPES[UA_TYPES_INT64]);
            pInt64.zLength = 2;
            pInt64.z = zArrayInt64;

            PointArrayUInt64 pUInt64;
            UA_UInt64 *zArrayUInt64 = (UA_UInt64*) UA_Array_new(2, &UA_TYPES[UA_TYPES_UINT64]);
            pUInt64.zLength = 2;
            pUInt64.z = zArrayUInt64;

            PointArrayFloat pFloat;
            UA_Float *zArrayFloat = (UA_Float*) UA_Array_new(2, &UA_TYPES[UA_TYPES_FLOAT]);
            pFloat.zLength = 2;
            pFloat.z = zArrayFloat;

            PointArrayDouble pDouble;
            UA_Double *zArrayDouble = (UA_Double*) UA_Array_new(2, &UA_TYPES[UA_TYPES_DOUBLE]);
            pDouble.zLength = 2;
            pDouble.z = zArrayDouble;

            /* Attention! Here the custom datatypes are allocated on the stack. So they
             * cannot be accessed from parallel (worker) threads. */
            UA_DataTypeArray customDataTypes = { config->customDataTypes, 1, types };
            config->customDataTypes = &customDataTypes;

            add3PointDataType(server);
            add3DPointVariable(server);

            add3PointArraySByteDataType(server, pSByte);
            add3DPointArraySByteVariable(server, pSByte);

            add3PointArrayByteDataType(server, pByte);
            add3DPointArrayByteVariable(server, pByte);

            add3PointArrayInt16DataType(server, pInt16);
            add3DPointArrayInt16Variable(server, pInt16);

            add3PointArrayUInt16DataType(server, pUInt16);
            add3DPointArrayUInt16Variable(server, pUInt16);

            add3PointArrayInt32DataType(server, pInt32);
            add3DPointArrayInt32Variable(server, pInt32);

            add3PointArrayUInt32DataType(server, pUInt32);
            add3DPointArrayUInt32Variable(server, pUInt32);

            add3PointArrayInt64DataType(server, pInt64);
            add3DPointArrayInt64Variable(server, pInt64);

            add3PointArrayUInt64DataType(server, pUInt64);
            add3DPointArrayUInt64Variable(server, pUInt64);

            add3PointArrayFloatDataType(server, pFloat);
            add3DPointArrayFloatVariable(server, pFloat);

            add3PointArrayDoubleDataType(server, pDouble);
            add3DPointArrayDoubleVariable(server, pDouble);
            if (info.GetStage() == ExecutionInfo::StartupStage) {
                UA_Server_run(server, &running);
            }
            else {
                UA_Server_delete(server);
                /* Deleting Arrays */
                UA_free(zArrayDouble);
                UA_free(zArrayFloat);
                UA_free(zArrayUInt64);
                UA_free(zArrayInt64);
                UA_free(zArrayUInt32);
                UA_free(zArrayInt32);
                UA_free(zArrayUInt16);
                UA_free(zArrayInt16);
                UA_free(zArrayByte);
                UA_free(zArraySByte);

                UA_free(pointDoubleMembers);
                UA_free(pointFloatMembers);
                UA_free(pointUInt64Members);
                UA_free(pointInt64Members);
                UA_free(pointUInt32Members);
                UA_free(pointInt32Members);
                UA_free(pointUInt16Members);
                UA_free(pointInt16Members);
                UA_free(pointByteMembers);
                UA_free(pointSByteMembers);
                UA_free(pointMembers);
                UA_free(types);
            }
            return err;
        }

        void SetRunning(bool value) {
            running = value;
        }

        MARTe::SingleThreadService service;

        bool running;

    private:
        void add3PointDataType(UA_Server *server) {
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("Point"));
            dattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("Point"));
            dattr.dataType = PointType.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;

            Point p;
            p.x = 0.0;
            p.y = 0.0;
            p.z = 0.0;
            UA_Variant_setScalar(&dattr.value, &p, &PointType);

            UA_Server_addVariableTypeNode(server, PointType.typeId, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("Point")),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), dattr, NULL, NULL);

        }

        void add3PointArraySByteDataType(UA_Server *server,
                                         PointArraySByte p) {
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArraySByte"));
            dattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArraySByte"));
            dattr.dataType = PointArraySByteType.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;

            p.x = 0;
            p.y = 0;
            p.z[0] = 0;
            p.z[1] = 0;
            UA_Variant_setScalar(&dattr.value, &p, &PointArraySByteType);

            UA_Server_addVariableTypeNode(server, PointArraySByteType.typeId, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArraySByte")),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), dattr, NULL, NULL);

        }

        void add3PointArrayByteDataType(UA_Server *server,
                                        PointArrayByte p) {
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayByte"));
            dattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayByte"));
            dattr.dataType = PointArrayByteType.typeId;
            dattr.valueRank = UA_VALUERANK_ANY;

            p.x = 0;
            p.y = 0;
            p.z[0] = 0;
            p.z[1] = 0;
            UA_Variant_setScalar(&dattr.value, &p, &PointArrayByteType);

            UA_Server_addVariableTypeNode(server, PointArrayByteType.typeId, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayByte")),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), dattr, NULL, NULL);

        }

        void add3PointArrayInt16DataType(UA_Server *server,
                                         PointArrayInt16 p) {
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayInt16"));
            dattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayInt16"));
            dattr.dataType = PointArrayInt16Type.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;

            p.x = 0;
            p.y = 0;
            p.z[0] = 0;
            p.z[1] = 0;
            UA_Variant_setScalar(&dattr.value, &p, &PointArrayInt16Type);

            UA_Server_addVariableTypeNode(server, PointArrayInt16Type.typeId, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayInt16")),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), dattr, NULL, NULL);

        }

        void add3PointArrayUInt16DataType(UA_Server *server,
                                          PointArrayUInt16 p) {
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayUInt16"));
            dattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayUInt16"));
            dattr.dataType = PointArrayUInt16Type.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;

            p.x = 0;
            p.y = 0;
            p.z[0] = 0;
            p.z[1] = 0;
            UA_Variant_setScalar(&dattr.value, &p, &PointArrayUInt16Type);

            UA_Server_addVariableTypeNode(server, PointArrayUInt16Type.typeId, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayUInt16")),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), dattr, NULL, NULL);

        }

        void add3PointArrayInt32DataType(UA_Server *server,
                                         PointArrayInt32 p) {
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayInt32"));
            dattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayInt32"));
            dattr.dataType = PointArrayInt32Type.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;

            p.x = 0;
            p.y = 0;
            p.z[0] = 0;
            p.z[1] = 0;
            UA_Variant_setScalar(&dattr.value, &p, &PointArrayInt32Type);

            UA_Server_addVariableTypeNode(server, PointArrayInt32Type.typeId, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayInt32")),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), dattr, NULL, NULL);

        }

        void add3PointArrayUInt32DataType(UA_Server *server,
                                          PointArrayUInt32 p) {
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayUInt32"));
            dattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayUInt32"));
            dattr.dataType = PointArrayUInt32Type.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;

            p.x = 0;
            p.y = 0;
            p.z[0] = 0;
            p.z[1] = 0;
            UA_Variant_setScalar(&dattr.value, &p, &PointArrayUInt32Type);

            UA_Server_addVariableTypeNode(server, PointArrayUInt32Type.typeId, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayUInt32")),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), dattr, NULL, NULL);

        }

        void add3PointArrayInt64DataType(UA_Server *server,
                                         PointArrayInt64 p) {
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayInt64"));
            dattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayInt64"));
            dattr.dataType = PointArrayInt64Type.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;

            p.x = 0;
            p.y = 0;
            p.z[0] = 0;
            p.z[1] = 0;
            UA_Variant_setScalar(&dattr.value, &p, &PointArrayInt64Type);

            UA_Server_addVariableTypeNode(server, PointArrayInt64Type.typeId, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayInt64")),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), dattr, NULL, NULL);

        }

        void add3PointArrayUInt64DataType(UA_Server *server,
                                          PointArrayUInt64 p) {
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayUInt64"));
            dattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayUInt64"));
            dattr.dataType = PointArrayUInt64Type.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;

            p.x = 0;
            p.y = 0;
            p.z[0] = 0;
            p.z[1] = 0;
            UA_Variant_setScalar(&dattr.value, &p, &PointArrayUInt64Type);

            UA_Server_addVariableTypeNode(server, PointArrayUInt64Type.typeId, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayUInt64")),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), dattr, NULL, NULL);

        }

        void add3PointArrayFloatDataType(UA_Server *server,
                                         PointArrayFloat p) {
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayFloat"));
            dattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayFloat"));
            dattr.dataType = PointArrayFloatType.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;

            p.x = 0.0;
            p.y = 0.0;
            p.z[0] = 0.0;
            p.z[1] = 0.0;
            UA_Variant_setScalar(&dattr.value, &p, &PointArrayFloatType);

            UA_Server_addVariableTypeNode(server, PointArrayFloatType.typeId, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayFloat")),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), dattr, NULL, NULL);

        }

        void add3PointArrayDoubleDataType(UA_Server *server,
                                          PointArrayDouble p) {
            UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
            dattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayDouble"));
            dattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayDouble"));
            dattr.dataType = PointArrayDoubleType.typeId;
            dattr.valueRank = UA_VALUERANK_SCALAR;

            p.x = 0.0;
            p.y = 0.0;
            p.z[0] = 0.0;
            p.z[1] = 0.0;
            UA_Variant_setScalar(&dattr.value, &p, &PointArrayDoubleType);

            UA_Server_addVariableTypeNode(server, PointArrayDoubleType.typeId, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayDouble")),
                                          UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), dattr, NULL, NULL);

        }

        /* Adding Variables */

        void add3DPointVariable(UA_Server *server) {
            Point p;
            p.x = 1.0;
            p.y = 2.0;
            p.z = 3.0;
            UA_VariableAttributes vattr = UA_VariableAttributes_default;
            vattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("Point"));
            vattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("Point"));
            vattr.dataType = PointType.typeId;
            vattr.valueRank = UA_VALUERANK_SCALAR;
            vattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            UA_Variant_setScalar(&vattr.value, &p, &PointType);

            UA_Server_addVariableNode(server, UA_NODEID_STRING(1, const_cast<MARTe::char8*>("Point")), UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("Point")), PointType.typeId,
                                      vattr, NULL, NULL);
        }

        void add3DPointArraySByteVariable(UA_Server *server,
                                          PointArraySByte p) {
            p.x = 1;
            p.y = 2;
            p.z[0] = 3;
            p.z[1] = 3;
            UA_VariableAttributes vattr = UA_VariableAttributes_default;
            vattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArraySByte"));
            vattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArraySByte"));
            vattr.dataType = PointArraySByteType.typeId;
            vattr.valueRank = UA_VALUERANK_SCALAR;
            vattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            UA_Variant_setScalar(&vattr.value, &p, &PointArraySByteType);

            UA_Server_addVariableNode(server, UA_NODEID_STRING(1, const_cast<MARTe::char8*>("PointArraySByte")), UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArraySByte")),
                                      PointArraySByteType.typeId, vattr, NULL, NULL);
        }

        void add3DPointArrayByteVariable(UA_Server *server,
                                         PointArrayByte p) {
            p.x = 1;
            p.y = 2;
            p.z[0] = 3;
            p.z[1] = 3;
            UA_VariableAttributes vattr = UA_VariableAttributes_default;
            vattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayByte"));
            vattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayByte"));
            vattr.dataType = PointArrayByteType.typeId;
            vattr.valueRank = UA_VALUERANK_ANY;
            vattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            UA_Variant_setScalar(&vattr.value, &p, &PointArrayByteType);

            UA_Server_addVariableNode(server, UA_NODEID_STRING(1, const_cast<MARTe::char8*>("PointArrayByte")), UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayByte")),
                                      PointArrayByteType.typeId, vattr, NULL, NULL);
        }

        void add3DPointArrayInt16Variable(UA_Server *server,
                                          PointArrayInt16 p) {
            p.x = 1;
            p.y = 2;
            p.z[0] = 3;
            p.z[1] = 3;
            UA_VariableAttributes vattr = UA_VariableAttributes_default;
            vattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayInt16"));
            vattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayInt16"));
            vattr.dataType = PointArrayInt16Type.typeId;
            vattr.valueRank = UA_VALUERANK_SCALAR;
            vattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            UA_Variant_setScalar(&vattr.value, &p, &PointArrayInt16Type);

            UA_Server_addVariableNode(server, UA_NODEID_STRING(1, const_cast<MARTe::char8*>("PointArrayInt16")), UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayInt16")),
                                      PointArrayInt16Type.typeId, vattr, NULL, NULL);
        }

        void add3DPointArrayUInt16Variable(UA_Server *server,
                                           PointArrayUInt16 p) {
            p.x = 1;
            p.y = 2;
            p.z[0] = 3;
            p.z[1] = 3;
            UA_VariableAttributes vattr = UA_VariableAttributes_default;
            vattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayUInt16"));
            vattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayUInt16"));
            vattr.dataType = PointArrayUInt16Type.typeId;
            vattr.valueRank = UA_VALUERANK_SCALAR;
            vattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            UA_Variant_setScalar(&vattr.value, &p, &PointArrayUInt16Type);

            UA_Server_addVariableNode(server, UA_NODEID_STRING(1, const_cast<MARTe::char8*>("PointArrayUInt16")), UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayUInt16")),
                                      PointArrayUInt16Type.typeId, vattr, NULL, NULL);
        }

        void add3DPointArrayInt32Variable(UA_Server *server,
                                          PointArrayInt32 p) {
            p.x = 1;
            p.y = 2;
            p.z[0] = 3;
            p.z[1] = 3;
            UA_VariableAttributes vattr = UA_VariableAttributes_default;
            vattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayInt32"));
            vattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayInt32"));
            vattr.dataType = PointArrayInt32Type.typeId;
            vattr.valueRank = UA_VALUERANK_SCALAR;
            vattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            UA_Variant_setScalar(&vattr.value, &p, &PointArrayInt32Type);

            UA_Server_addVariableNode(server, UA_NODEID_STRING(1, const_cast<MARTe::char8*>("PointArrayInt32")), UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayInt32")),
                                      PointArrayInt32Type.typeId, vattr, NULL, NULL);
        }

        void add3DPointArrayUInt32Variable(UA_Server *server,
                                           PointArrayUInt32 p) {
            p.x = 1;
            p.y = 2;
            p.z[0] = 3;
            p.z[1] = 3;
            UA_VariableAttributes vattr = UA_VariableAttributes_default;
            vattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayUInt32"));
            vattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayUInt32"));
            vattr.dataType = PointArrayUInt32Type.typeId;
            vattr.valueRank = UA_VALUERANK_SCALAR;
            vattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            UA_Variant_setScalar(&vattr.value, &p, &PointArrayUInt32Type);

            UA_Server_addVariableNode(server, UA_NODEID_STRING(1, const_cast<MARTe::char8*>("PointArrayUInt32")), UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayUInt32")),
                                      PointArrayUInt32Type.typeId, vattr, NULL, NULL);
        }

        void add3DPointArrayInt64Variable(UA_Server *server,
                                          PointArrayInt64 p) {
            p.x = 1;
            p.y = 2;
            p.z[0] = 3;
            p.z[1] = 3;
            UA_VariableAttributes vattr = UA_VariableAttributes_default;
            vattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayInt64"));
            vattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayInt64"));
            vattr.dataType = PointArrayInt64Type.typeId;
            vattr.valueRank = UA_VALUERANK_SCALAR;
            vattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            UA_Variant_setScalar(&vattr.value, &p, &PointArrayInt64Type);

            UA_Server_addVariableNode(server, UA_NODEID_STRING(1, const_cast<MARTe::char8*>("PointArrayInt64")), UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayInt64")),
                                      PointArrayInt64Type.typeId, vattr, NULL, NULL);
        }

        void add3DPointArrayUInt64Variable(UA_Server *server,
                                           PointArrayUInt64 p) {
            p.x = 1;
            p.y = 2;
            p.z[0] = 3;
            p.z[1] = 3;
            UA_VariableAttributes vattr = UA_VariableAttributes_default;
            vattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayUInt64"));
            vattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayUInt64"));
            vattr.dataType = PointArrayUInt64Type.typeId;
            vattr.valueRank = UA_VALUERANK_SCALAR;
            vattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            UA_Variant_setScalar(&vattr.value, &p, &PointArrayUInt64Type);

            UA_Server_addVariableNode(server, UA_NODEID_STRING(1, const_cast<MARTe::char8*>("PointArrayUInt64")), UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayUInt64")),
                                      PointArrayUInt64Type.typeId, vattr, NULL, NULL);
        }

        void add3DPointArrayFloatVariable(UA_Server *server,
                                          PointArrayFloat p) {
            p.x = 1;
            p.y = 2;
            p.z[0] = 3;
            p.z[1] = 3;
            UA_VariableAttributes vattr = UA_VariableAttributes_default;
            vattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayFloat"));
            vattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayFloat"));
            vattr.dataType = PointArrayFloatType.typeId;
            vattr.valueRank = UA_VALUERANK_SCALAR;
            vattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            UA_Variant_setScalar(&vattr.value, &p, &PointArrayFloatType);

            UA_Server_addVariableNode(server, UA_NODEID_STRING(1, const_cast<MARTe::char8*>("PointArrayFloat")), UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayFloat")),
                                      PointArrayFloatType.typeId, vattr, NULL, NULL);
        }

        void add3DPointArrayDoubleVariable(UA_Server *server,
                                           PointArrayDouble p) {
            p.x = 1;
            p.y = 2;
            p.z[0] = 3;
            p.z[1] = 3;
            UA_VariableAttributes vattr = UA_VariableAttributes_default;
            vattr.description = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayDouble"));
            vattr.displayName = UA_LOCALIZEDTEXT(const_cast<MARTe::char8*>("en-US"), const_cast<MARTe::char8*>("PointArrayDouble"));
            vattr.dataType = PointArrayDoubleType.typeId;
            vattr.valueRank = UA_VALUERANK_SCALAR;
            vattr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
            UA_Variant_setScalar(&vattr.value, &p, &PointArrayDoubleType);

            UA_Server_addVariableNode(server, UA_NODEID_STRING(1, const_cast<MARTe::char8*>("PointArrayDouble")), UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, const_cast<MARTe::char8*>("PointArrayDouble")),
                                      PointArrayDoubleType.typeId, vattr, NULL, NULL);
        }

    };
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRITETEST_H_ */

