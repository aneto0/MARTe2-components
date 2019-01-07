/**
 * @file OPCUANode.cpp
 * @brief Source file for class OPCUANode
 * @date Nov 12, 2018 TODO Verify the value and format of the date
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
 * the class OPCUANode (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "OPCUANode.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

OPCUANode::OPCUANode() :
        OPCUAReferenceContainer() {
    nType = 0u;
    value = 0u;
    nodeId = NULL_PTR(const char*);
    parentNodeId = NULL_PTR(char*);
    parentReferenceNodeId = 1u;
}

OPCUANode::~OPCUANode() {

}

bool OPCUANode::GetOPCVariable(OPCUANodeSettings &settings) {
    bool ok = true;
    char * localization = new char[strlen("en-US") + 1];
    strcpy(localization, "en-US");

    //Read from cdb
    settings->nType = 1u;

    if (settings->nType == 1u) {
        settings->attr = UA_VariableAttributes_default;
        settings->attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

        //settings->value = value;
        settings->value = 54u;

        UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_UINT32]);

        nodeId = GetName();
        settings->nodeId = UA_NODEID_STRING_ALLOC(1, GetName());

        //TODO How to read strings from configuration file in MARTe?

        char * readName = new char[strlen(GetName()) + 1];
        strcpy(readName, GetName());
        settings->nodeName = UA_QUALIFIEDNAME(1, readName);
        settings->attr.displayName = UA_LOCALIZEDTEXT(localization, readName);

        if (parentNodeId == 0u) {
            settings->parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
            ok = true;
        }
        else {
            settings->parentNodeId = UA_NODEID_STRING(1, parentNodeId);
        }
        if (parentReferenceNodeId != 0u) {
            settings->parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
        }
        else {
            ok = false;
        }
    }

    return ok;
}

void OPCUANode::SetParent(const char* parentId) {
    parentNodeId = new char[strlen(parentId) + 1];
    StringHelper::Copy(parentNodeId, parentId);
}

const char* OPCUANode::GetNodeId() {
    return nodeId;
}

bool OPCUANode::IsNode() {
    return true;
}

CLASS_REGISTER(OPCUANode, "");

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

