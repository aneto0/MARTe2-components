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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool EPICSPVStructureDataITest::TestConstructor() {
    using namespace MARTe;
    EPICSPVStructureDataI test;
    /*EPICSPVStructureDataI test;
     test.InitStructure();
     test.CreateRelative("TEST1");
     return (test.GetNumberOfChildren() == 0u);*/
    epics::pvData::FieldCreatePtr fieldCreate = epics::pvData::getFieldCreate();
    epics::pvData::FieldBuilderPtr fieldBuilder = fieldCreate->createFieldBuilder();
    fieldBuilder->addNestedStructure("One")->addNestedStructure("Two")->endNested()->endNested()->addNestedStructure("OneP1")->endNested();
    epics::pvData::StructureConstPtr topStructure = fieldBuilder->createStructure();
    epics::pvData::PVDataCreatePtr pvDataCreate = epics::pvData::getPVDataCreate();
    epics::pvData::PVStructurePtr structPtr = pvDataCreate->createPVStructure(topStructure);
    structPtr->dumpValue(std::cout);
    test.SetStructure(structPtr);
    test.MoveAbsolute("One.Two");
    test.MoveToRoot();
    test.MoveAbsolute("One.Two");
    test.MoveToAncestor(1u);
    test.MoveRelative("Two");
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
    std::cout << test.GetChildName(0) << std::endl;
    /*test.MoveToAncestor(1u);
     structPtr->dumpValue(std::cout);
     test.MoveToAncestor(1u);
     structPtr->dumpValue(std::cout);*/
    return true;
}