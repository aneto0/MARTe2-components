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
/*-e909 and -e9133 redefines bool. -e578 symbol ovveride in CLASS_REGISTER*/
/*lint -save -e909 -e9133 -e578*/
namespace MARTe {

OPCUAObject::OPCUAObject() :
        OPCUAReferenceContainer() {
    //parentNodeId = 0u;
}

OPCUAObject::~OPCUAObject() {

}

bool OPCUAObject::GetOPCObject(OPCUA::OPCUAObjectSettings &settings, const uint32 nodeNumber) {
    bool ok = true;
    settings->attr = UA_ObjectAttributes_default;
    SetNodeId(nodeNumber);
    //settings->nodeId = UA_NODEID_STRING_ALLOC(1, GetName());
    settings->nodeId = UA_NODEID_NUMERIC(1u, nodeNumber);
    /*lint -e{1055} -e{64} -estring(628, *UA_QUALIFIEDNAME*) -estring(526, *UA_QUALIFIEDNAME*) UA_QUALIFIEDNAME is declared in the open62541 library.*/
    settings->nodeName = UA_QUALIFIEDNAME(1u, const_cast<char8*>(GetName()));
    settings->attr.displayName = UA_LOCALIZEDTEXT(const_cast<char8*>("en-US"), const_cast<char8*>(GetName()));
    if (parentNodeId == 0u) {
        settings->parentNodeId = UA_NODEID_NUMERIC(0u, 85u); /* UA_NS0ID_OBJECTSFOLDER = 85 */
    }
    else {
        settings->parentNodeId = UA_NODEID_NUMERIC(1u, parentNodeId);
    }
    settings->parentReferenceNodeId = UA_NODEID_NUMERIC(0u, 35u); /* UA_NS0ID_ORGANIZES = 35 */
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
/*lint -restore*/
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

