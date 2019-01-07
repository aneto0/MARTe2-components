/**
 * @file OPCUAObject.cpp
 * @brief Source file for class OPCUAObject
 * @date Nov 16, 2018 TODO Verify the value and format of the date
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
    nodeId = NULL_PTR(const char*);
}

OPCUAObject::~OPCUAObject() {

}

bool OPCUAObject::GetOPCObject(OPCUAObjectSettings &settings) {
    bool ok = true;
    settings->attr = UA_ObjectAttributes_default;
    settings->nType = 0u;
    if (ok) {
        nodeId = GetName();
        settings->nodeId = UA_NODEID_STRING_ALLOC(1, GetName());
        char * localization = new char[strlen("en-US") + 1];
        strcpy(localization, "en-US");
        char * readName = new char[strlen(GetName()) + 1];
        strcpy(readName, GetName());
        settings->nodeName = UA_QUALIFIEDNAME(1, readName);
        settings->attr.displayName = UA_LOCALIZEDTEXT(localization, readName);
    }

    return ok;
}

const char* OPCUAObject::GetNodeId() {
    return nodeId;
}

bool OPCUAObject::IsObject() {
    return true;
}

CLASS_REGISTER(OPCUAObject, "");

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

