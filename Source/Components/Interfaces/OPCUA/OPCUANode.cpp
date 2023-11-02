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
/*-e909 and -e9133 redefines bool. -e578 symbol ovveride in CLASS_REGISTER*/
/*lint -save -e909 -e9133 -e578*/
namespace MARTe {

OPCUANode::OPCUANode() :
        OPCUAReferenceContainer() {
    parentReferenceNodeId = 1u;
}

OPCUANode::~OPCUANode() {

}
/*lint -e{534} No returning value is ignored.*/
/*lint -e{746} -e{1055} -e{534} -e{516} UA_Variant_setScalar is defined in open62541.*/
bool OPCUANode::GetOPCVariable(OPCUA::OPCUANodeSettings &settings,
                               const TypeDescriptor nType,
                               const uint32 nNumber) {
    bool ok = true;
    settings->attr = UA_VariableAttributes_default;
    settings->attr.accessLevel = (0x1u)|static_cast<uint8>((0x1u<<1u));//static_cast<uint8>(static_cast<uint8>(0x01U) | ((static_cast<uint8>(0x01U))<<1u)); /* UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE */
    SetNodeId(nNumber);
    bool isArray = (numberOfDimensions > 0u);
    uint64 nElements = 1u;
    /* Setting the array parameters in the OPCUA Variable */
    if (isArray) {
        for (uint8 i = 0u; i < numberOfDimensions; i++) {
            nElements *= numberOfElements[i];
        }
    }
    if (nType == UnsignedInteger8Bit) {
        if (isArray) {
            ok = InitArray(settings, &UA_TYPES[UA_TYPES_BYTE], nElements);
        }
        else {
            /*lint -estring(628, *UA_Variant_setScalar*) -estring(526, *UA_Variant_setScalar*) UA_Variant_setScalar is defined in open62541 */
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_BYTE]);
        }
    }
    else if (nType == UnsignedInteger16Bit) {
        if (isArray) {
            ok = InitArray(settings, &UA_TYPES[UA_TYPES_UINT16], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_UINT16]);
        }
    }
    else if (nType == UnsignedInteger32Bit) {
        if (isArray) {
            ok = InitArray(settings, &UA_TYPES[UA_TYPES_UINT32], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_UINT32]);
        }
    }
    else if (nType == UnsignedInteger64Bit) {
        if (isArray) {
            ok = InitArray(settings, &UA_TYPES[UA_TYPES_UINT64], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_UINT64]);
        }
    }
    else if (nType == SignedInteger8Bit) {
        if (isArray) {
            ok = InitArray(settings, &UA_TYPES[UA_TYPES_SBYTE], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_SBYTE]);
        }
    }
    else if (nType == SignedInteger16Bit) {
        if (isArray) {
            ok = InitArray(settings, &UA_TYPES[UA_TYPES_INT16], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_INT16]);
        }
    }
    else if (nType == SignedInteger32Bit) {
        settings->attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
        if (isArray) {
            ok = InitArray(settings, &UA_TYPES[UA_TYPES_INT32], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_INT32]);
        }
    }
    else if (nType == SignedInteger64Bit) {
        if (isArray) {
            ok = InitArray(settings, &UA_TYPES[UA_TYPES_INT64], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_INT64]);
        }
    }
    else if (nType == Float32Bit) {
        if (isArray) {
            ok = InitArray(settings, &UA_TYPES[UA_TYPES_FLOAT], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_FLOAT]);
        }
    }
    else if (nType == Float64Bit) {
        if (isArray) {
            ok = InitArray(settings, &UA_TYPES[UA_TYPES_DOUBLE], nElements);
        }
        else {
            UA_Variant_setScalar(&settings->attr.value, &settings->value, &UA_TYPES[UA_TYPES_DOUBLE]);
        }
    }
    else {
        ok = false;
    }
#if 0
    else if ((nType.type == CArray) || (nType.type == BT_CCString) || (nType.type == PCString) || (nType.type == SString)) {
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
    if (ok) {
        //settings->nodeId = UA_NODEID_STRING_ALLOC(1, GetName());
        settings->nodeId = UA_NODEID_NUMERIC(1u, nNumber);
        /*char * readName = new char[strlen(GetName()) + 1];
         strcpy(readName, GetName());*/
        //StreamString readName = GetName();
        /*lint -e{1055} -e{64} -e{746} UA_QUALIFIEDNAME is declared in the open62541 library.*/
        settings->nodeName = UA_QUALIFIEDNAME(1, const_cast<char8*>(GetName()));
        settings->attr.displayName = UA_LOCALIZEDTEXT(const_cast<char8*>("en-US"), const_cast<char8*>(GetName()));
        if (parentNodeId == 0u) {
            settings->parentNodeId = UA_NODEID_NUMERIC(0u, 85u); /* UA_NS0ID_OBJECTSFOLDER = 85 */
            settings->parentReferenceNodeId = UA_NODEID_NUMERIC(0u, 35u); /* UA_NS0ID_ORGANIZES = 35 */
            parentReferenceNodeId = 0u;
            ok = true;
        }
        else {
            settings->parentNodeId = UA_NODEID_NUMERIC(1u, parentNodeId);
        }
        if (parentReferenceNodeId == 1u) {
            settings->parentReferenceNodeId = UA_NODEID_NUMERIC(0u, 47u); /* UA_NS0ID_HASCOMPONENT = 47 */
        }
    }

    return ok;
}

bool OPCUANode::IsNode() {
    return true;
}

bool OPCUANode::InitArray(OPCUA::OPCUANodeSettings const & settings,
                          const UA_DataType * const type,
                          const uint64 nElem) {
    bool ok = true;
    //settings->attr.valueRank = numberOfDimensions;
    settings->attr.valueRank = 1; /*numberOfDimensions */
    settings->attr.arrayDimensions = static_cast<uint32 *>(UA_Array_new(static_cast<osulong>(numberOfDimensions), type));
    settings->attr.arrayDimensionsSize = numberOfDimensions;
    settings->attr.value.arrayDimensions = static_cast<uint32 *>(UA_Array_new(static_cast<osulong>(numberOfDimensions), type));
    settings->attr.value.arrayDimensionsSize = numberOfDimensions;
    for (uint8 k = 0u; k < numberOfDimensions; k++) {
        settings->attr.arrayDimensions[k] = numberOfElements[k];
        settings->attr.value.arrayDimensions[k] = numberOfElements[k];
    }
    UA_StatusCode code = UA_Variant_setArrayCopy(&settings->attr.value, &settings->value, nElem, type);
    if (code != 0x00U) { /* UA_STATUSCODE_GOOD */
        ok = false;
    }
    return ok;
}

CLASS_REGISTER(OPCUANode, "");

}
/*lint -restore*/
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

