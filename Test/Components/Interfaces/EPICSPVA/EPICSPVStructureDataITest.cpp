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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "EPICSPVStructureDataI.h"
#include "EPICSPVStructureDataITest.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool EPICSPVStructureDataITest::TestConstructor() {
    using namespace MARTe;
    EPICSPVStructureDataI test;
    test.InitStructure();
    test.CreateAbsolute("A.B.C.D");
    test.CreateAbsolute("A.B.E");
    test.CreateAbsolute("C.F");
    uint32 please = 3;
    test.Write("Please", please);
    test.MoveAbsolute("A.B.C");
    float32 pleaseFloat = 3;
    test.Write("PleaseFloat", pleaseFloat);
    test.FinaliseStructure();
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
