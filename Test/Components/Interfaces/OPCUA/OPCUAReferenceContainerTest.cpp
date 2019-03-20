/**
 * @file OPCUAReferenceContainerTest.cpp
 * @brief Source file for class OPCUAReferenceContainerTest
 * @date 12 Mar 2019 TODO Verify the value and format of the date
 * @author lporzio TODO Verify the name and format of the author
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
 * the class OPCUAReferenceContainerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "OPCUAReferenceContainer.h"
#include "OPCUAReferenceContainerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

bool OPCUAReferenceContainerTest::TestConstructor() {
    using namespace MARTe;
    OPCUAReferenceContainer orc;
    uint8 nDimensions = orc.GetNumberOfDimensions();
    return (nDimensions == 0u);
}

bool OPCUAReferenceContainerTest::Test_IsObject() {
    using namespace MARTe;
    OPCUAReferenceContainer orc;
    return !(orc.IsObject());
}

bool OPCUAReferenceContainerTest::Test_IsNode() {
    using namespace MARTe;
    OPCUAReferenceContainer orc;
    return !(orc.IsNode());
}

bool OPCUAReferenceContainerTest::Test_GetOPCVariable() {
    using namespace MARTe;
    OPCUAReferenceContainer orc;
    OPCUANodeSettings settings = new NodeProperties;
    TypeDescriptor td;
    uint32 nodeNumber = 3000u;
    return !(orc.GetOPCVariable(settings, td, nodeNumber));
}

bool OPCUAReferenceContainerTest::Test_GetOPCObject() {
    using namespace MARTe;
    OPCUAReferenceContainer orc;
    OPCUAObjectSettings settings = new ObjectProperties;
    uint32 nodeNumber = 3000u;
    return !(orc.GetOPCObject(settings, nodeNumber));
}

bool OPCUAReferenceContainerTest::Test_SetGetParent() {
    using namespace MARTe;
    OPCUAReferenceContainer orc;
    uint32 parentNumber = 1u;
    orc.SetParent(parentNumber);
    uint32 check = orc.GetParentNodeId();
    return (check == 1u);
}

bool OPCUAReferenceContainerTest::Test_SetGetNodeId() {
    using namespace MARTe;
    OPCUAReferenceContainer orc;
    uint32 Number = 1u;
    orc.SetNodeId(Number);
    uint32 check = orc.GetNodeId();
    return (check == 1u);
}

bool OPCUAReferenceContainerTest::Test_SetGetNodeType() {
    using namespace MARTe;
    OPCUAReferenceContainer orc;
    orc.SetNodeType(TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
    TypeDescriptor td = orc.GetNodeType();
    const char8* typeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(td);
    return (StringHelper::Compare(typeStr, "uint32") == 0);
}

bool OPCUAReferenceContainerTest::Test_IsFirst() {
    using namespace MARTe;
    OPCUAReferenceContainer orc;
    return !(orc.IsFirstObject());
}

bool OPCUAReferenceContainerTest::Test_SetFirst() {
    using namespace MARTe;
    OPCUAReferenceContainer orc;
    orc.SetFirst(true);
    return !(orc.IsFirstObject());
}

bool OPCUAReferenceContainerTest::Test_NumberOfElementsDimensions() {
    using namespace MARTe;
    OPCUAReferenceContainer orc;
    orc.SetNumberOfDimensions(2u);
    orc.SetNumberOfElements(0u, 2u);
    orc.SetNumberOfElements(1u, 2u);
    uint8 nd = orc.GetNumberOfDimensions();
    uint32* ne = orc.GetNumberOfElements();
    return (nd == 2u && ne[0] == 2u && ne[1] == 2u);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

