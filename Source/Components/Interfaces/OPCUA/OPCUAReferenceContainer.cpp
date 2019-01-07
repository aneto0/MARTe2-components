/**
 * @file OPCUAReferenceContainer.cpp
 * @brief Source file for class OPCUAReferenceContainer
 * @date Nov 20, 2018 TODO Verify the value and format of the date
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
 * the class OPCUAReferenceContainer (public, protected, and private). Be aware that some 
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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

OPCUAReferenceContainer::OPCUAReferenceContainer() : ReferenceContainer() {

}

OPCUAReferenceContainer::~OPCUAReferenceContainer() {

}

bool OPCUAReferenceContainer::IsNode() {
    return false;
}

bool OPCUAReferenceContainer::IsObject() {
    return false;
}

bool OPCUAReferenceContainer::GetOPCObject(OPCUAObjectSettings &settings) {
    return false;
}

bool OPCUAReferenceContainer::GetOPCVariable(OPCUANodeSettings &settings) {
    return false;
}

void OPCUAReferenceContainer::SetParent(const char* parentId) {

}

const char* OPCUAReferenceContainer::GetNodeId() {
    return 0u;
}

CLASS_REGISTER(OPCUAReferenceContainer, "");

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
