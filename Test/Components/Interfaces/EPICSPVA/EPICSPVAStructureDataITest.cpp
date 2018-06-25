/**
 * @file EPICSCAClient.cpp
 * @brief Source file for class EPICSCAClient
 * @date 13/06/2018
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
 * the class EPICSCAClient (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <pv/rpcClient.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include <EPICSPVAStructureDataI.h>
#include "File.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "Vector.h"
#include "EPICSPVAStructureDataITest.h"
#include "EPICSRPCClient.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool EPICSPVAStructureDataITest::TestConstructor() {
    using namespace MARTe;
    /*EPICSPVAStructureDataI test;
     test.InitStructure();*/
    /*test.CreateAbsolute("A.B.C.D");
     test.CreateAbsolute("A.B.E");
     test.CreateAbsolute("C.F");
     uint32 please = 3;
     test.Write("Please", please);
     test.MoveAbsolute("A.B.C");
     float32 pleaseFloat = 3;
     test.Write("PleaseFloat", pleaseFloat);*/

    /*test.CreateAbsolute("_Message");
     test.Write("Class", "Message");
     test.Write("Destination", "ConfigurationManager");
     test.Write("Function", "purge");
     test.Write("Mode", "ExpectsReply");
     test.CreateRelative("_Payload");
     test.Write("Class", "ConfigurationDatabase");
     test.Write("Root", "TCPMessageProxy");
     test.FinaliseStructure();
     test.GetRootStruct()->dumpValue(std::cout);*/
    /*ConfigurationDatabase cdb;
     test.Copy(cdb);
     StreamString ss;
     ss.Printf("%!", cdb);
     std::cout << ss.Buffer() << std::endl;*/

    ReferenceT<EPICSRPCClient> rpcClient(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    rpcClient->SetName("EPICSRPCClient");
    rpcClient->Start();
    ObjectRegistryDatabase::Instance()->Insert(rpcClient);

    ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ConfigurationDatabase> payload(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    ConfigurationDatabase msgConfig;
    msg->SetName("MARTe2App1EPICSConfigurationLoader");
    msgConfig.Write("Destination", "EPICSRPCClient");
    msgConfig.Write("Function", "");
    //msgConfig.Write("Mode", "ExpectsReply");
    msg->Initialise(msgConfig);
    payload->CreateAbsolute("_Message");
    payload->Write("Class", "Message");
    payload->Write("Destination", "ConfigurationManager");
    payload->Write("Function", "purge");
    payload->CreateRelative("_Payload");
    payload->Write("Class", "ConfigurationDatabase");
    payload->Write("Root", "TCPMessageProxy");
    payload->MoveToRoot();
    msg->Insert(payload);

    MessageI::SendMessage(msg);

    Sleep::Sec(10.0);
    //epics::pvData::FieldCreatePtr fieldCreate = epics::pvData::getFieldCreate();
    //epics::pvData::FieldBuilderPtr fieldBuilder = fieldCreate->createFieldBuilder();
    /*fieldBuilder->addNestedStructure("One")->addNestedStructure("Two")->endNested()->addFixedArray("astringarr", epics::pvData::pvString, 5)->addFixedArray(
     "adoublearr", epics::pvData::pvDouble, 5)->add("adouble", epics::pvData::pvDouble)->add("astring", epics::pvData::pvString)->endNested()->addNestedStructure(
     "OneP1")->endNested();*/
    /*fieldBuilder->addNestedStructure("One")->addNestedStructure("Two")->endNested()->add("adouble", epics::pvData::pvDouble)->add(
     "astring", epics::pvData::pvString)->addFixedArray("adoublearr", epics::pvData::pvDouble, 5)->endNested()->addNestedStructure(
     "OneP1")->endNested();
     epics::pvData::PVStructurePtr structPtr = epics::pvData::getPVDataCreate()->createPVStructure(fieldBuilder->createStructure());*/
    //epics::pvData::PVStructurePtr structPtr = epics::pvData::getPVDataCreate()->createPVStructure(test.GetRootStruct());
    /*epics::pvData::shared_vector<double> out;
     out.resize(5);
     out[0] = -3;
     out[1] = 3;
     out[2] = -2;
     out[3] = 2;
     out[4] = -1;
     epics::pvData::shared_vector<const double> outF = freeze(out);
     epics::pvData::PVScalarArrayPtr scalarArrayPtr = std::tr1::dynamic_pointer_cast<epics::pvData::PVScalarArray>(structPtr->getSubField("One.adoublearr"));
     if (scalarArrayPtr) {
     scalarArrayPtr->putFrom<double>(outF);
     }*/

    //std::string name = (argc > 1) ? argv[1] : "anonymous";
    //arguments->getSubField<PVString>("personsname")->put(name);
    // Create an RPC client to the "helloService" service
    //epics::pvAccess::RPCClient::shared_pointer client = epics::pvAccess::RPCClient::create("MARTe2App1EPICSConfigurationLoader");
    //epics::pvAccess::RPCClient::shared_pointer client = epics::pvAccess::RPCClient::create("MARTe2App1EPICSConfigurationLoader");
    // Create an RPC request and block until response is received. There is
    // no need to explicitly wait for connection; this method takes care of it.
    // In case of an error, an exception is thrown.
    //epics::pvData::PVStructurePtr response = client->request(structPtr, 3.0);
    //response->dumpValue(std::cout);
#if 0
    EPICSPVAStructureDataI test2;
    test2.InitStructure();
    ConfigurationDatabase config;
    File f;
    f.Open("/home/aneto/Projects/tmp/RTApp-3T.cfg", BasicFile::ACCESS_MODE_R);
    StandardParser parser(f, config);
    parser.Parse();
    config.MoveToRoot();
    config.Copy(test2);
    test2.FinaliseStructure();
#endif

    /* test.CreateRelative("TEST1");
     return (test.GetNumberOfChildren() == 0u);*/
    /*epics::pvData::FieldCreatePtr fieldCreate = epics::pvData::getFieldCreate();
     epics::pvData::FieldBuilderPtr fieldBuilder = fieldCreate->createFieldBuilder();*/
    //fieldBuilder->addNestedStructure("One")->addNestedStructure("Two")->endNested()->addFixedArray("astringarr", epics::pvData::pvString, 5)->addFixedArray("adoublearr", epics::pvData::pvDouble, 5)->add("adouble", epics::pvData::pvDouble)->add("astring", epics::pvData::pvString)->endNested()->addNestedStructure("OneP1")->endNested();
#if 0
    test.SetStructure(structPtr);
    test.MoveAbsolute("One.Two");
    test.MoveToRoot();
    test.MoveAbsolute("One.Two");
    test.MoveToAncestor(1u);
    test.MoveRelative("Two");
    test.MoveToAncestor(1u);
    double doubleOut = 3.1415;
    double doubleIn;
    test.Write("adouble", doubleOut);
    test.Read("adouble", doubleIn);
    std::cout << "Read " << doubleIn << std::endl;

    const char8 *const strOut = "A string with space....";
    StreamString strIn;
    test.Write("astring", strOut);
    test.Read("astring", strIn);
    std::cout << "Read " << strIn.Buffer() << std::endl;

    double arrIn[5];
    double arrOut[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    test.Write("adoublearr", arrOut);
    test.Read("adoublearr", arrIn);
    std::cout << "Read " << arrIn << std::endl;

    const char8 *strArrOut[] = {"A", "B", "CCC", "AAVASVA", "BASDDSA"};
    StreamString *strArrIn = new StreamString[5];
    Vector<StreamString> strArrInV(strArrIn, 5);
    test.Write("astringarr", strArrOut);
    test.Read("astringarr", strArrInV);
    std::cout << "Read " << strArrIn[0].Buffer() << std::endl;
    std::cout << "Read " << strArrIn[1].Buffer() << std::endl;
    std::cout << "Read " << strArrIn[2].Buffer() << std::endl;
    std::cout << "Read " << strArrIn[3].Buffer() << std::endl;
    std::cout << "Read " << strArrIn[4].Buffer() << std::endl;
    delete [] strArrIn;

    test.MoveToAncestor(10u);
    test.MoveToRoot();
    test.MoveAbsolute("Three");
    test.MoveToChild(0);
    test.MoveToRoot();
    test.MoveToChild(1);
    std::cout << "NAME" << std::endl;
    std::cout << test.GetName() << std::endl;
    test.MoveToRoot();
    test.MoveToChild(2);
    std::cout << "NAME" << std::endl;
    std::cout << test.GetChildName(0) << std::endl;*/

    test.InitStructure();
    test.CreateAbsolute("A.B.C");
    test.CreateAbsolute("A.B.D.E");
    //test.CreateAbsolute("A");
    test.FinaliseStructure();
    test.MoveToRoot();

    /*test.MoveToAncestor(1u);
     structPtr->dumpValue(std::cout);
     test.MoveToAncestor(1u);
     structPtr->dumpValue(std::cout);*/
#endif
    return true;
}
