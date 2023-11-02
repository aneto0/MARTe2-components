/**
 * @file OPCUAReferenceContainer.cpp
 * @brief Source file for class OPCUAReferenceContainer
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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/*-e909 and -e9133 redefines bool. -e578 symbol ovveride in CLASS_REGISTER*/
/*lint -save -e909 -e9133 -e578*/
namespace MARTe {

OPCUAReferenceContainer::OPCUAReferenceContainer() :
        ReferenceContainer() {
    nodeType = TypeDescriptor();
    nodeId = 0u;
    parentNodeId = 0u;
    numberOfElements = NULL_PTR(uint32 *);
    numberOfDimensions = 0u;
    isFirstObject = false;
}

OPCUAReferenceContainer::~OPCUAReferenceContainer() {
    if (numberOfElements != NULL_PTR(uint32 *)) {
        delete[] numberOfElements;
    }
}

bool OPCUAReferenceContainer::IsNode() {
    return false;
}

bool OPCUAReferenceContainer::IsObject() {
    return false;
}

bool OPCUAReferenceContainer::GetOPCObject(OPCUA::OPCUAObjectSettings &settings,
                                           const uint32 nodeNumber) {
    settings->nodeId = UA_NODEID_NUMERIC(1u, nodeNumber);
    return false;
}

/*lint -e{715} variables not used as it is supposed to be extended by subclasses.*/
bool OPCUAReferenceContainer::GetOPCVariable(OPCUA::OPCUANodeSettings &settings,
                                             const TypeDescriptor nType,
                                             const uint32 nNumber) {
    return false;
}

void OPCUAReferenceContainer::SetParent(const uint32 parentId) {
    parentNodeId = parentId;
}

const uint32 OPCUAReferenceContainer::GetParentNodeId() const {
    return parentNodeId;
}

void OPCUAReferenceContainer::SetNodeId(const uint32 newNodeId) {
    nodeId = newNodeId;
}

const uint32 OPCUAReferenceContainer::GetNodeId() const {
    return nodeId;
}

/*lint -e{715} variables not used as it is supposed to be extended by subclasses.*/
void OPCUAReferenceContainer::SetFirst(const bool value) {
}

const bool OPCUAReferenceContainer::IsFirstObject() {
    return false;
}

void OPCUAReferenceContainer::SetNodeType(const TypeDescriptor &type) {
    nodeType = type;
}

TypeDescriptor OPCUAReferenceContainer::GetNodeType() const {
    return nodeType;
}

void OPCUAReferenceContainer::SetNumberOfElements(const uint32 dimension,
                                                  const uint32 nElements) {
    if ((numberOfElements != NULL_PTR(uint32 *)) && (dimension < numberOfDimensions)) {
        numberOfElements[dimension] = nElements;
    }
}

void OPCUAReferenceContainer::SetNumberOfDimensions(const uint8 nDimensions) {
    if (numberOfElements == NULL_PTR(uint32 *)) {
        numberOfElements = new uint32[nDimensions];
        numberOfDimensions = nDimensions;
    }
}

uint32* OPCUAReferenceContainer::GetNumberOfElements() {
    return numberOfElements;
}

const uint8 OPCUAReferenceContainer::GetNumberOfDimensions() const {
    return numberOfDimensions;
}

CLASS_REGISTER(OPCUAReferenceContainer, "");

}
/*lint -restore*/
