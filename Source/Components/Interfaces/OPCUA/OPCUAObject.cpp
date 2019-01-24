/**
 * @file OPCUAObject.cpp
 * @brief Source file for class OPCUAObject
 * @date 24/01/2019
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
 * the class OPCUAObject (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "OPCUAObject.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

OPCUAObject::OPCUAObject() :
        OPCUAReferenceContainer() {
    nodeId = NULL_PTR(char*);
    parentNodeId = NULL_PTR(char*);
    isFirstObject = false;
}

OPCUAObject::~OPCUAObject() {

}

bool OPCUAObject::GetOPCObject(OPCUAObjectSettings &settings) {
    bool ok = true;
    settings->attr = UA_ObjectAttributes_default;
    SetNodeId(GetName());
    settings->nodeId = UA_NODEID_STRING_ALLOC(1, GetName());
    char * localization = new char[strlen("en-US") + 1];
    StringHelper::Copy(localization, "en-US");
    char * readName = new char[StringHelper::Length(GetName()) + 1];
    StringHelper::Copy(readName, GetName());
    settings->nodeName = UA_QUALIFIEDNAME(1, readName);
    settings->attr.displayName = UA_LOCALIZEDTEXT(localization, readName);
    if (parentNodeId == NULL_PTR(char*)) {
        settings->parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    }
    else {
        settings->parentNodeId = UA_NODEID_STRING(1, parentNodeId);
    }
    settings->parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);

    return ok;
}

bool OPCUAObject::IsObject() {
    return true;
}

void OPCUAObject::SetFirst(const bool value) {
    isFirstObject = value;
}

const bool OPCUAObject::IsFirstObject() {
    return isFirstObject;
}

CLASS_REGISTER(OPCUAObject, "");

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

