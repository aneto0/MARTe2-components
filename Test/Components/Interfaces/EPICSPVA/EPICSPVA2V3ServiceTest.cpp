/**
 * @file EPICSPVA2V3ServiceTest.cpp
 * @brief Source file for class EPICSPVA2V3ServiceTest
 * @date 16/10/2018
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
 * the class EPICSPVA2V3ServiceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "EPICSRPCClient.h"
#include "EPICSPVA2V3Service.h"
#include "EPICSPVA2V3ServiceTest.h"
#include "EPICSRPCServer.h"
#include "ObjectRegistryDatabase.h"
#include "MessageI.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool EPICSPVA2V3ServiceTest::TestConstructor() {
    using namespace MARTe;
    EPICSPVA2V3Service test;
    return (test.NumberOfReferences() == 0);
}

bool EPICSPVA2V3ServiceTest::TestInitialise() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSRPCServer = {\n"
            "    Class = EPICSPVA::EPICSRPCServer\n"
            "    +EPICSPVA2V3 = {\n"
            "        Class = EPICSPVA::EPICSPVA2V3Service\n"
            "        Polynomial = 0x12345678 //32 bit polynomial to use in the CRC computation\n"
            "        CRCInitialValue = 0x0 //32 bit initial CRC value\n"
            "        CRCFinalXOR = 0xFFFFFFFF //32 bit value to XOR against the computed CRC\n"
            "        +Structure = { //Structure to be replied\n"
            "            Class = ReferenceContainer\n"
            "            +value1 = {//The leaf shall match against an existent EPICS3 PV.\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE1\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +value2 = {\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE2\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +valueS = {\n"
            "                Class = ReferenceContainer\n"
            "                +value3 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE3\"\n"
            "                    PVType = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSRPCClient = {\n"
            "    Class = EPICSPVA::EPICSRPCClient\n"
            "}";

    ConfigurationDatabase cdb;
    config.Seek(0LLU);
    StreamString err;
    StandardParser parser(config, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return (ok);
}

bool EPICSPVA2V3ServiceTest::TestInitialise_False_Polynomial() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSRPCServer = {\n"
            "    Class = EPICSPVA::EPICSRPCServer\n"
            "    +EPICSPVA2V3 = {\n"
            "        Class = EPICSPVA::EPICSPVA2V3Service\n"
            "        CRCInitialValue = 0x0 //32 bit initial CRC value\n"
            "        CRCFinalXOR = 0xFFFFFFFF //32 bit value to XOR against the computed CRC\n"
            "        +Structure = { //Structure to be replied\n"
            "            Class = ReferenceContainer\n"
            "            +value1 = {//The leaf shall match against an existent EPICS3 PV.\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE1\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +value2 = {\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE2\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +valueS = {\n"
            "                Class = ReferenceContainer\n"
            "                +value3 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE3\"\n"
            "                    PVType = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSRPCClient = {\n"
            "    Class = EPICSPVA::EPICSRPCClient\n"
            "}";

    ConfigurationDatabase cdb;
    config.Seek(0LLU);
    StreamString err;
    StandardParser parser(config, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return (!ok);
}

bool EPICSPVA2V3ServiceTest::TestInitialise_False_CRCInitialValue() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSRPCServer = {\n"
            "    Class = EPICSPVA::EPICSRPCServer\n"
            "    +EPICSPVA2V3 = {\n"
            "        Class = EPICSPVA::EPICSPVA2V3Service\n"
            "        Polynomial = 0x12345678 //32 bit polynomial to use in the CRC computation\n"
            "        CRCFinalXOR = 0xFFFFFFFF //32 bit value to XOR against the computed CRC\n"
            "        +Structure = { //Structure to be replied\n"
            "            Class = ReferenceContainer\n"
            "            +value1 = {//The leaf shall match against an existent EPICS3 PV.\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE1\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +value2 = {\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE2\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +valueS = {\n"
            "                Class = ReferenceContainer\n"
            "                +value3 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE3\"\n"
            "                    PVType = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSRPCClient = {\n"
            "    Class = EPICSPVA::EPICSRPCClient\n"
            "}";

    ConfigurationDatabase cdb;
    config.Seek(0LLU);
    StreamString err;
    StandardParser parser(config, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return (!ok);
}

bool EPICSPVA2V3ServiceTest::TestInitialise_False_CRCFinalXOR() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSRPCServer = {\n"
            "    Class = EPICSPVA::EPICSRPCServer\n"
            "    +EPICSPVA2V3 = {\n"
            "        Class = EPICSPVA::EPICSPVA2V3Service\n"
            "        Polynomial = 0x12345678 //32 bit polynomial to use in the CRC computation\n"
            "        CRCInitialValue = 0x0 //32 bit initial CRC value\n"
            "        +Structure = { //Structure to be replied\n"
            "            Class = ReferenceContainer\n"
            "            +value1 = {//The leaf shall match against an existent EPICS3 PV.\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE1\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +value2 = {\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE2\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +valueS = {\n"
            "                Class = ReferenceContainer\n"
            "                +value3 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE3\"\n"
            "                    PVType = uint32\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSRPCClient = {\n"
            "    Class = EPICSPVA::EPICSRPCClient\n"
            "}";

    ConfigurationDatabase cdb;
    config.Seek(0LLU);
    StreamString err;
    StandardParser parser(config, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return (!ok);
}

bool EPICSPVA2V3ServiceTest::TestInitialise_False_Structure() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSRPCServer = {\n"
            "    Class = EPICSPVA::EPICSRPCServer\n"
            "    +EPICSPVA2V3 = {\n"
            "        Class = EPICSPVA::EPICSPVA2V3Service\n"
            "        Polynomial = 0x12345678 //32 bit polynomial to use in the CRC computation\n"
            "        CRCInitialValue = 0x0 //32 bit initial CRC value\n"
            "        CRCFinalXOR = 0xFFFFFFFF //32 bit value to XOR against the computed CRC\n"
            "    }\n"
            "}\n"
            "+EPICSRPCClient = {\n"
            "    Class = EPICSPVA::EPICSRPCClient\n"
            "}";

    ConfigurationDatabase cdb;
    config.Seek(0LLU);
    StreamString err;
    StandardParser parser(config, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return (!ok);
}

bool EPICSPVA2V3ServiceTest::Testrequest() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSRPCServer = {\n"
            "    Class = EPICSPVA::EPICSRPCServer\n"
            "    +EPICSPVA2V3 = {\n"
            "        Class = EPICSPVA::EPICSPVA2V3Service\n"
            "        Polynomial = 0x12345678 //32 bit polynomial to use in the CRC computation\n"
            "        CRCInitialValue = 0x0 //32 bit initial CRC value\n"
            "        CRCFinalXOR = 0xFFFFFFFF //32 bit value to XOR against the computed CRC\n"
            "        +Structure = { //Structure to be replied\n"
            "            Class = ReferenceContainer\n"
            "            +value1 = {//The leaf shall match against an existent EPICS3 PV.\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE1\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +value2 = {\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE2\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +valueS = {\n"
            "                Class = ReferenceContainer\n"
            "                +value3 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE3\"\n"
            "                    PVType = uint32\n"
            "                }\n"
            "                +value4 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE4ARR\"\n"
            "                    PVType = uint32\n"
            "                    NumberOfElements = 5\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSRPCClient = {\n"
            "    Class = EPICSPVA::EPICSRPCClient\n"
            "}";

    ConfigurationDatabase cdb;
    config.Seek(0LLU);
    StreamString err;
    StandardParser parser(config, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ok) {
        ReferenceT<Message> msgInitStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgInitStart->SetName("EPICSRPCServerStart");
        msgConfig.Write("Destination", "EPICSRPCServer");
        msgConfig.Write("Function", "Start");
        msgInitStart->Initialise(msgConfig);
        msgInitStart->SetExpectsReply(true);
        //MessageI::SendMessage(msgInitStart);
        Object notReallyUsed;
        MessageI::SendMessageAndWaitReply(msgInitStart, &notReallyUsed);
    }
    if (ok) {
        ReferenceT<Message> msgInitStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgInitStart->SetName("EPICSRPCClientStart");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "Start");
        msgInitStart->Initialise(msgConfig);
        MessageI::SendMessage(msgInitStart);
    }
    ReferenceT<Message> msgInit(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    uint32 seed;
    uint32 status;

    if (ok) {
        ConfigurationDatabase msgConfig;
        msgInit->SetName("EPICSPVA2V3");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msgInit->Initialise(msgConfig);
        ReferenceT<ConfigurationDatabase> cdb(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cdb->Write("qualifier", "init");
        msgInit->Insert(cdb);
        msgInit->SetExpectsReply(true);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msgInit, &notReallyUsed);
    }
    if (ok) {
        ReferenceT<StructuredDataI> replyStruct;
        replyStruct = msgInit->Get(1u);
        ok = replyStruct.IsValid();

        if (ok) {
            ok = replyStruct->Read("status", status);
            if (ok) {
                ok = (status == 1u);
            }
        }
        if (ok) {
            ok = replyStruct->Read("value", seed);
        }
    }

    ReferenceT<Message> msgRead(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ConfigurationDatabase> serverStructure(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ok) {
        ConfigurationDatabase msgConfig;
        msgRead->SetName("EPICSPVA2V3");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msgRead->Initialise(msgConfig);
        ReferenceT<ConfigurationDatabase> cdb(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cdb->Write("qualifier", "read");
        msgRead->Insert(cdb);
        msgRead->SetExpectsReply(true);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msgRead, &notReallyUsed);
    }
    if (ok) {
        ReferenceT<StructuredDataI> replyStruct;
        replyStruct = msgRead->Get(1u);
        ok = replyStruct.IsValid();

        if (ok) {
            ok = replyStruct->Read("status", status);
            if (ok) {
                ok = (status == 1u);
            }
        }
        if (ok) {
            ok = replyStruct->MoveAbsolute("value.valueS");
            if (ok) {
                uint32 ignore;
                ok = replyStruct->Read("value3", ignore);
            }
            if (ok) {
                replyStruct->MoveAbsolute("value");
                ok = replyStruct->Copy(*serverStructure.operator ->());
            }
        }
    }
    if (ok) {
        //The caput might fail if the server is still not fully setup
        uint32 numberOfRetries = 10;
        while (numberOfRetries > 0) {
            ReferenceT<Message> msgLoad(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ConfigurationDatabase msgConfig;
            msgLoad->SetName("EPICSPVA2V3");
            msgConfig.Write("Destination", "EPICSRPCClient");
            msgConfig.Write("Function", "");
            msgLoad->Initialise(msgConfig);
            ReferenceT<ConfigurationDatabase> cdb(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            cdb->CreateAbsolute("value");
            uint32 values[] = { 1, 2, 3 };
            cdb->Write("value1", values[0]);
            cdb->Write("value2", values[1]);
            cdb->CreateAbsolute("value.valueS");
            cdb->Write("value3", values[2]);
            uint32 valuesArr[] = { 4, 5, 6, 7, 3 };
            Vector<uint32> valuesVec(valuesArr);
            cdb->Write("value4", valuesVec);
            cdb->MoveToRoot();
            CRC<uint32> crc;
            uint32 pol = 0x12345678;
            crc.ComputeTable(pol);
            uint32 crcInitialValue = 0x0;
            uint32 crcFinalXOR = 0xFFFFFFFF;
            uint32 hash = crc.Compute(reinterpret_cast<uint8 *>(&seed), sizeof(uint32), crcInitialValue, false);
            hash = crc.Compute(reinterpret_cast<uint8 *>(&values[0]), 3 * sizeof(uint32), hash, false);
            hash = crc.Compute(reinterpret_cast<uint8 *>(&valuesArr[0]), 5 * sizeof(uint32), hash, false);
            hash = hash ^ crcFinalXOR;

            cdb->Write("hash", hash);
            cdb->Write("qualifier", "load");
            msgLoad->Insert(cdb);
            msgLoad->SetExpectsReply(true);
            Object notReallyUsed;
            ok = MessageI::SendMessageAndWaitReply(msgLoad, &notReallyUsed);
            if (ok) {
                ReferenceT<StructuredDataI> replyStruct;
                replyStruct = msgLoad->Get(1u);
                ok = replyStruct.IsValid();

                if (ok) {
                    ok = replyStruct->Read("status", status);
                    if (ok) {
                        ok = (status == 1u);
                    }
                }
            }
            if (ok) {
                numberOfRetries = 0u;
            }
            else {
                numberOfRetries--;
                Sleep::Sec(0.5);
            }
        }
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool EPICSPVA2V3ServiceTest::Testrequest_False_qualifier() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSRPCServer = {\n"
            "    Class = EPICSPVA::EPICSRPCServer\n"
            "    +EPICSPVA2V3 = {\n"
            "        Class = EPICSPVA::EPICSPVA2V3Service\n"
            "        Polynomial = 0x12345678 //32 bit polynomial to use in the CRC computation\n"
            "        CRCInitialValue = 0x0 //32 bit initial CRC value\n"
            "        CRCFinalXOR = 0xFFFFFFFF //32 bit value to XOR against the computed CRC\n"
            "        +Structure = { //Structure to be replied\n"
            "            Class = ReferenceContainer\n"
            "            +value1 = {//The leaf shall match against an existent EPICS3 PV.\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE1\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +value2 = {\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE2\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +valueS = {\n"
            "                Class = ReferenceContainer\n"
            "                +value3 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE3\"\n"
            "                    PVType = uint32\n"
            "                }\n"
            "                +value4 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE4ARR\"\n"
            "                    PVType = uint32\n"
            "                    NumberOfElements = 5\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSRPCClient = {\n"
            "    Class = EPICSPVA::EPICSRPCClient\n"
            "}";

    ConfigurationDatabase cdb;
    config.Seek(0LLU);
    StreamString err;
    StandardParser parser(config, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ok) {
        ReferenceT<Message> msgInitStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgInitStart->SetName("EPICSRPCServerStart");
        msgConfig.Write("Destination", "EPICSRPCServer");
        msgConfig.Write("Function", "Start");
        msgInitStart->Initialise(msgConfig);
        msgInitStart->SetExpectsReply(true);
        //MessageI::SendMessage(msgInitStart);
        Object notReallyUsed;
        MessageI::SendMessageAndWaitReply(msgInitStart, &notReallyUsed);
    }
    if (ok) {
        ReferenceT<Message> msgInitStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgInitStart->SetName("EPICSRPCClientStart");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "Start");
        msgInitStart->Initialise(msgConfig);
        MessageI::SendMessage(msgInitStart);
    }
    ReferenceT<Message> msgInit(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    uint32 status;

    if (ok) {
        ConfigurationDatabase msgConfig;
        msgInit->SetName("EPICSPVA2V3");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msgInit->Initialise(msgConfig);
        ReferenceT<ConfigurationDatabase> cdb(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        msgInit->Insert(cdb);
        msgInit->SetExpectsReply(true);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msgInit, &notReallyUsed);
    }
    if (ok) {
        ReferenceT<StructuredDataI> replyStruct;
        replyStruct = msgInit->Get(1u);
        ok = replyStruct.IsValid();

        if (ok) {
            ok = replyStruct->Read("status", status);
            if (ok) {
                ok = (status == 0u);
            }
        }
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool EPICSPVA2V3ServiceTest::Testrequest_False_hash() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSRPCServer = {\n"
            "    Class = EPICSPVA::EPICSRPCServer\n"
            "    +EPICSPVA2V3 = {\n"
            "        Class = EPICSPVA::EPICSPVA2V3Service\n"
            "        Polynomial = 0x12345678 //32 bit polynomial to use in the CRC computation\n"
            "        CRCInitialValue = 0x0 //32 bit initial CRC value\n"
            "        CRCFinalXOR = 0xFFFFFFFF //32 bit value to XOR against the computed CRC\n"
            "        +Structure = { //Structure to be replied\n"
            "            Class = ReferenceContainer\n"
            "            +value1 = {//The leaf shall match against an existent EPICS3 PV.\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE1\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +value2 = {\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE2\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +valueS = {\n"
            "                Class = ReferenceContainer\n"
            "                +value3 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE3\"\n"
            "                    PVType = uint32\n"
            "                }\n"
            "                +value4 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE4ARR\"\n"
            "                    PVType = uint32\n"
            "                    NumberOfElements = 5\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSRPCClient = {\n"
            "    Class = EPICSPVA::EPICSRPCClient\n"
            "}";

    ConfigurationDatabase cdb;
    config.Seek(0LLU);
    StreamString err;
    StandardParser parser(config, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ok) {
        ReferenceT<Message> msgInitStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgInitStart->SetName("EPICSRPCServerStart");
        msgConfig.Write("Destination", "EPICSRPCServer");
        msgConfig.Write("Function", "Start");
        msgInitStart->Initialise(msgConfig);
        msgInitStart->SetExpectsReply(true);
        //MessageI::SendMessage(msgInitStart);
        Object notReallyUsed;
        MessageI::SendMessageAndWaitReply(msgInitStart, &notReallyUsed);
    }
    if (ok) {
        ReferenceT<Message> msgInitStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgInitStart->SetName("EPICSRPCClientStart");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "Start");
        msgInitStart->Initialise(msgConfig);
        MessageI::SendMessage(msgInitStart);
    }
    ReferenceT<Message> msgInit(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    uint32 seed;
    uint32 status;

    if (ok) {
        ConfigurationDatabase msgConfig;
        msgInit->SetName("EPICSPVA2V3");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msgInit->Initialise(msgConfig);
        ReferenceT<ConfigurationDatabase> cdb(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cdb->Write("qualifier", "init");
        msgInit->Insert(cdb);
        msgInit->SetExpectsReply(true);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msgInit, &notReallyUsed);
    }
    if (ok) {
        ReferenceT<StructuredDataI> replyStruct;
        replyStruct = msgInit->Get(1u);
        ok = replyStruct.IsValid();

        if (ok) {
            ok = replyStruct->Read("status", status);
            if (ok) {
                ok = (status == 1u);
            }
        }
        if (ok) {
            ok = replyStruct->Read("value", seed);
        }
    }

    ReferenceT<ConfigurationDatabase> serverStructure(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ok) {
        //The caput might fail if the server is still not fully setup
        ReferenceT<Message> msgLoad(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgLoad->SetName("EPICSPVA2V3");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msgLoad->Initialise(msgConfig);
        ReferenceT<ConfigurationDatabase> cdb(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cdb->CreateAbsolute("value");
        uint32 values[] = { 1, 2, 3 };
        cdb->Write("value1", values[0]);
        cdb->Write("value2", values[1]);
        cdb->CreateAbsolute("value.valueS");
        cdb->Write("value3", values[2]);
        uint32 valuesArr[] = { 4, 5, 6, 7, 3 };
        Vector<uint32> valuesVec(valuesArr);
        cdb->Write("value4", valuesVec);
        cdb->MoveToRoot();
        cdb->Write("qualifier", "load");
        msgLoad->Insert(cdb);
        msgLoad->SetExpectsReply(true);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msgLoad, &notReallyUsed);
        if (ok) {
            ReferenceT<StructuredDataI> replyStruct;
            replyStruct = msgLoad->Get(1u);
            ok = replyStruct.IsValid();

            if (ok) {
                ok = replyStruct->Read("status", status);
                if (ok) {
                    ok = (status == 0u);
                }
            }
        }
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool EPICSPVA2V3ServiceTest::Testrequest_False_value() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSRPCServer = {\n"
            "    Class = EPICSPVA::EPICSRPCServer\n"
            "    +EPICSPVA2V3 = {\n"
            "        Class = EPICSPVA::EPICSPVA2V3Service\n"
            "        Polynomial = 0x12345678 //32 bit polynomial to use in the CRC computation\n"
            "        CRCInitialValue = 0x0 //32 bit initial CRC value\n"
            "        CRCFinalXOR = 0xFFFFFFFF //32 bit value to XOR against the computed CRC\n"
            "        +Structure = { //Structure to be replied\n"
            "            Class = ReferenceContainer\n"
            "            +value1 = {//The leaf shall match against an existent EPICS3 PV.\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE1\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +value2 = {\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE2\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +valueS = {\n"
            "                Class = ReferenceContainer\n"
            "                +value3 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE3\"\n"
            "                    PVType = uint32\n"
            "                }\n"
            "                +value4 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE4ARR\"\n"
            "                    PVType = uint32\n"
            "                    NumberOfElements = 5\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSRPCClient = {\n"
            "    Class = EPICSPVA::EPICSRPCClient\n"
            "}";

    ConfigurationDatabase cdb;
    config.Seek(0LLU);
    StreamString err;
    StandardParser parser(config, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ok) {
        ReferenceT<Message> msgInitStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgInitStart->SetName("EPICSRPCServerStart");
        msgConfig.Write("Destination", "EPICSRPCServer");
        msgConfig.Write("Function", "Start");
        msgInitStart->Initialise(msgConfig);
        msgInitStart->SetExpectsReply(true);
        //MessageI::SendMessage(msgInitStart);
        Object notReallyUsed;
        MessageI::SendMessageAndWaitReply(msgInitStart, &notReallyUsed);
    }
    if (ok) {
        ReferenceT<Message> msgInitStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgInitStart->SetName("EPICSRPCClientStart");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "Start");
        msgInitStart->Initialise(msgConfig);
        MessageI::SendMessage(msgInitStart);
    }
    ReferenceT<Message> msgInit(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    uint32 seed;
    uint32 status;

    if (ok) {
        ConfigurationDatabase msgConfig;
        msgInit->SetName("EPICSPVA2V3");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msgInit->Initialise(msgConfig);
        ReferenceT<ConfigurationDatabase> cdb(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cdb->Write("qualifier", "init");
        msgInit->Insert(cdb);
        msgInit->SetExpectsReply(true);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msgInit, &notReallyUsed);
    }
    if (ok) {
        ReferenceT<StructuredDataI> replyStruct;
        replyStruct = msgInit->Get(1u);
        ok = replyStruct.IsValid();

        if (ok) {
            ok = replyStruct->Read("status", status);
            if (ok) {
                ok = (status == 1u);
            }
        }
        if (ok) {
            ok = replyStruct->Read("value", seed);
        }
    }

    ReferenceT<ConfigurationDatabase> serverStructure(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ok) {
        ReferenceT<Message> msgLoad(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgLoad->SetName("EPICSPVA2V3");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msgLoad->Initialise(msgConfig);
        ReferenceT<ConfigurationDatabase> cdb(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cdb->MoveToRoot();
        CRC<uint32> crc;
        uint32 pol = 0x12345678;
        crc.ComputeTable(pol);
        uint32 crcInitialValue = 0x0;
        uint32 hash = crc.Compute(reinterpret_cast<uint8 *>(&seed), sizeof(uint32), crcInitialValue, false);

        cdb->Write("hash", hash);
        cdb->Write("qualifier", "load");
        msgLoad->Insert(cdb);
        msgLoad->SetExpectsReply(true);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msgLoad, &notReallyUsed);
        if (ok) {
            ReferenceT<StructuredDataI> replyStruct;
            replyStruct = msgLoad->Get(1u);
            ok = replyStruct.IsValid();

            if (ok) {
                ok = replyStruct->Read("status", status);
                if (ok) {
                    ok = (status == 0u);
                }
            }
        }
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool EPICSPVA2V3ServiceTest::Testrequest_False_hash_mismatch() {
    using namespace MARTe;
    StreamString config = ""
            "+EPICSRPCServer = {\n"
            "    Class = EPICSPVA::EPICSRPCServer\n"
            "    +EPICSPVA2V3 = {\n"
            "        Class = EPICSPVA::EPICSPVA2V3Service\n"
            "        Polynomial = 0x12345678 //32 bit polynomial to use in the CRC computation\n"
            "        CRCInitialValue = 0x0 //32 bit initial CRC value\n"
            "        CRCFinalXOR = 0xFFFFFFFF //32 bit value to XOR against the computed CRC\n"
            "        +Structure = { //Structure to be replied\n"
            "            Class = ReferenceContainer\n"
            "            +value1 = {//The leaf shall match against an existent EPICS3 PV.\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE1\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +value2 = {\n"
            "                Class = EPICS::EPICSPV\n"
            "                PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE2\"\n"
            "                PVType = uint32\n"
            "            }\n"
            "            +valueS = {\n"
            "                Class = ReferenceContainer\n"
            "                +value3 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE3\"\n"
            "                    PVType = uint32\n"
            "                }\n"
            "                +value4 = {\n"
            "                    Class = EPICS::EPICSPV\n"
            "                    PVName = \"MARTE2::EPICSPVA2V3::TEST::VALUE4ARR\"\n"
            "                    PVType = uint32\n"
            "                    NumberOfElements = 5\n"
            "                }\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+EPICSRPCClient = {\n"
            "    Class = EPICSPVA::EPICSRPCClient\n"
            "}";

    ConfigurationDatabase cdb;
    config.Seek(0LLU);
    StreamString err;
    StandardParser parser(config, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ok) {
        ReferenceT<Message> msgInitStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgInitStart->SetName("EPICSRPCServerStart");
        msgConfig.Write("Destination", "EPICSRPCServer");
        msgConfig.Write("Function", "Start");
        msgInitStart->Initialise(msgConfig);
        msgInitStart->SetExpectsReply(true);
        //MessageI::SendMessage(msgInitStart);
        Object notReallyUsed;
        MessageI::SendMessageAndWaitReply(msgInitStart, &notReallyUsed);
    }
    if (ok) {
        ReferenceT<Message> msgInitStart(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgInitStart->SetName("EPICSRPCClientStart");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "Start");
        msgInitStart->Initialise(msgConfig);
        MessageI::SendMessage(msgInitStart);
    }
    ReferenceT<Message> msgInit(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    uint32 seed;
    uint32 status;

    if (ok) {
        ConfigurationDatabase msgConfig;
        msgInit->SetName("EPICSPVA2V3");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msgInit->Initialise(msgConfig);
        ReferenceT<ConfigurationDatabase> cdb(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cdb->Write("qualifier", "init");
        msgInit->Insert(cdb);
        msgInit->SetExpectsReply(true);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msgInit, &notReallyUsed);
    }
    if (ok) {
        ReferenceT<StructuredDataI> replyStruct;
        replyStruct = msgInit->Get(1u);
        ok = replyStruct.IsValid();

        if (ok) {
            ok = replyStruct->Read("status", status);
            if (ok) {
                ok = (status == 1u);
            }
        }
        if (ok) {
            ok = replyStruct->Read("value", seed);
        }
    }

    ReferenceT<Message> msgRead(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ConfigurationDatabase> serverStructure(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ok) {
        ConfigurationDatabase msgConfig;
        msgRead->SetName("EPICSPVA2V3");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msgRead->Initialise(msgConfig);
        ReferenceT<ConfigurationDatabase> cdb(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cdb->Write("qualifier", "read");
        msgRead->Insert(cdb);
        msgRead->SetExpectsReply(true);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msgRead, &notReallyUsed);
    }
    if (ok) {
        ReferenceT<StructuredDataI> replyStruct;
        replyStruct = msgRead->Get(1u);
        ok = replyStruct.IsValid();

        if (ok) {
            ok = replyStruct->Read("status", status);
            if (ok) {
                ok = (status == 1u);
            }
        }
        if (ok) {
            ok = replyStruct->MoveAbsolute("value.valueS");
            if (ok) {
                uint32 ignore;
                ok = replyStruct->Read("value3", ignore);
            }
            if (ok) {
                replyStruct->MoveAbsolute("value");
                ok = replyStruct->Copy(*serverStructure.operator ->());
            }
        }
    }
    if (ok) {
        ReferenceT<Message> msgLoad(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase msgConfig;
        msgLoad->SetName("EPICSPVA2V3");
        msgConfig.Write("Destination", "EPICSRPCClient");
        msgConfig.Write("Function", "");
        msgLoad->Initialise(msgConfig);
        ReferenceT<ConfigurationDatabase> cdb(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cdb->CreateAbsolute("value");
        uint32 values[] = { 1, 2, 3 };
        cdb->Write("value1", values[0]);
        cdb->Write("value2", values[1]);
        cdb->CreateAbsolute("value.valueS");
        cdb->Write("value3", values[2]);
        uint32 valuesArr[] = { 4, 5, 6, 7, 3 };
        Vector<uint32> valuesVec(valuesArr);
        cdb->Write("value4", valuesVec);
        cdb->MoveToRoot();
        CRC<uint32> crc;
        uint32 pol = 0x12345678;
        crc.ComputeTable(pol);
        uint32 crcInitialValue = 0x0;
        uint32 hash = crc.Compute(reinterpret_cast<uint8 *>(&seed), sizeof(uint32), crcInitialValue, false);
        hash = crc.Compute(reinterpret_cast<uint8 *>(&values[0]), 3 * sizeof(uint32), hash, false);
        hash = crc.Compute(reinterpret_cast<uint8 *>(&valuesArr[0]), 5 * sizeof(uint32), hash, false);

        cdb->Write("hash", hash);
        cdb->Write("qualifier", "load");
        msgLoad->Insert(cdb);
        msgLoad->SetExpectsReply(true);
        Object notReallyUsed;
        ok = MessageI::SendMessageAndWaitReply(msgLoad, &notReallyUsed);
        if (ok) {
            ReferenceT<StructuredDataI> replyStruct;
            replyStruct = msgLoad->Get(1u);
            ok = replyStruct.IsValid();

            if (ok) {
                ok = replyStruct->Read("status", status);
                if (ok) {
                    ok = (status == 0u);
                }
            }
        }
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}
