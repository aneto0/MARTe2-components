/**
 * @file OPCUANode.cpp
 * @brief Source file for class OPCUANode
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
    parentReferenceNodeId = 1u;
}

OPCUANode::~OPCUANode() {

}

bool OPCUANode::GetOPCVariable(OPCUANodeSettings &settings,
                               TypeDescriptor nodeType,
                               uint32 nodeNumber) {
    bool ok = true;
//    char * localization = new char[strlen("en-US") + 1];
//    strcpy(localization, "en-US");
    //StreamString localization = "en-US";
    settings->attr = UA_VariableAttributes_default;
    settings->attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    SetNodeId(nodeNumber);
    bool isArray = (numberOfDimensions > 0);
    uint64 nElements = 1u;
    /* Setting the array parameters in the OPCUA Variable */
    if (isArray) {
        for (uint8 i = 0u; i < numberOfDimensions; i++) {
            nElements *= numberOfElements[i];
        }
    }
    if (nodeType == UnsignedInteger8Bit) {
        if (isArray) {
            InitArray(settings, &UA_TYPES[UA_TYPES_BYTE], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_BYTE]);
        }
    }
    else if (nodeType == UnsignedInteger16Bit) {
        if (isArray) {
            InitArray(settings, &UA_TYPES[UA_TYPES_UINT16], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_UINT16]);
        }
    }
    else if (nodeType == UnsignedInteger32Bit) {
        if (isArray) {
            InitArray(settings, &UA_TYPES[UA_TYPES_UINT32], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_UINT32]);
        }
    }
    else if (nodeType == UnsignedInteger64Bit) {
        if (isArray) {
            InitArray(settings, &UA_TYPES[UA_TYPES_UINT64], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_UINT64]);
        }
    }
    else if (nodeType == SignedInteger8Bit) {
        if (isArray) {
            InitArray(settings, &UA_TYPES[UA_TYPES_SBYTE], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_SBYTE]);
        }
    }
    else if (nodeType == SignedInteger16Bit) {
        if (isArray) {
            InitArray(settings, &UA_TYPES[UA_TYPES_INT16], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_INT16]);
        }
    }
    else if (nodeType == SignedInteger32Bit) {
        settings->attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
        if (isArray) {
            InitArray(settings, &UA_TYPES[UA_TYPES_INT32], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_INT32]);
        }
    }
    else if (nodeType == SignedInteger64Bit) {
        if (isArray) {
            InitArray(settings, &UA_TYPES[UA_TYPES_INT64], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_INT64]);
        }
    }
    else if (nodeType == Float32Bit) {
        if (isArray) {
            InitArray(settings, &UA_TYPES[UA_TYPES_FLOAT], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_FLOAT]);
        }
    }
    else if (nodeType == Float64Bit) {
        if (isArray) {
            InitArray(settings, &UA_TYPES[UA_TYPES_DOUBLE], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_DOUBLE]);
        }
    }
#if 0
    else if ((nodeType.type == CArray) || (nodeType.type == BT_CCString) || (nodeType.type == PCString) || (nodeType.type == SString)) {
        if (isArray) {
            //InitArray(settings, &UA_TYPES[UA_TYPES_STRING], nElements);
            ok = false;
        }
        else {
            //UA_String defString = UA_STRING_ALLOC("NULL");
            //THE STRING IS NOT INITIALISED.
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_STRING]);

        }
    }
#endif
    //settings->nodeId = UA_NODEID_STRING_ALLOC(1, GetName());
    settings->nodeId = UA_NODEID_NUMERIC(1, nodeNumber);
    /*char * readName = new char[strlen(GetName()) + 1];
    strcpy(readName, GetName());*/
    //StreamString readName = GetName();
    settings->nodeName = UA_QUALIFIEDNAME(1, const_cast<char8*>(GetName()));
    settings->attr.displayName = UA_LOCALIZEDTEXT(const_cast<char8*>("en-US"), const_cast<char8*>(GetName()));

    if (parentNodeId == 0u) {
        settings->parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
        settings->parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
        parentReferenceNodeId = 0u;
        ok = true;
    }
    else {
        settings->parentNodeId = UA_NODEID_NUMERIC(1, parentNodeId);
    }
    if (parentReferenceNodeId == 1u) {
        settings->parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
    }
    return ok;
}

bool OPCUANode::IsNode() {
    return true;
}

void OPCUANode::InitArray(OPCUANodeSettings &settings,
                          const UA_DataType *type,
                          uint64 nElem) {
    settings->attr.valueRank = numberOfDimensions;
    settings->attr.arrayDimensions = static_cast<uint32 *>(UA_Array_new(numberOfDimensions, type));
    settings->attr.arrayDimensionsSize = numberOfDimensions;
    settings->attr.value.arrayDimensions = static_cast<uint32 *>(UA_Array_new(numberOfDimensions, type));
    settings->attr.value.arrayDimensionsSize = numberOfDimensions;
    for (uint8 k = 0u; k < numberOfDimensions; k++) {
        settings->attr.arrayDimensions[k] = numberOfElements[k];
        settings->attr.value.arrayDimensions[k] = numberOfElements[k];
    }
    UA_Variant_setArrayCopy(&settings->attr.value, &settings->value, nElem, type);
}

CLASS_REGISTER(OPCUANode, "");

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

