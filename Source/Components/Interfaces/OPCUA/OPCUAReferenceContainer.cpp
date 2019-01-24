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

namespace MARTe {

OPCUAReferenceContainer::OPCUAReferenceContainer() :
        ReferenceContainer() {
    nodeType = TypeDescriptor();
    nodeId = NULL_PTR(char*);
    parentNodeId = NULL_PTR(char*);
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

bool OPCUAReferenceContainer::GetOPCVariable(OPCUANodeSettings &settings,
                                             TypeDescriptor nodeType) {
    return false;
}

void OPCUAReferenceContainer::SetParent(const char* parentId) {
    parentNodeId = new char[strlen(parentId) + 1];
    StringHelper::Copy(parentNodeId, parentId);
}

const char* OPCUAReferenceContainer::GetParentNodeId() {
    return parentNodeId;
}

void OPCUAReferenceContainer::SetNodeId(const char* newNodeId) {
    nodeId = new char[strlen(newNodeId) + 1];
    StringHelper::Copy(nodeId, newNodeId);
}

const char* OPCUAReferenceContainer::GetNodeId() {
    return nodeId;
}

void OPCUAReferenceContainer::SetFirst(const bool value) {

}

const bool OPCUAReferenceContainer::IsFirstObject() {
    return false;
}

void OPCUAReferenceContainer::SetNodeType(TypeDescriptor type) {
    nodeType = type;
}

TypeDescriptor OPCUAReferenceContainer::GetNodeType() {
    return nodeType;
}

void OPCUAReferenceContainer::SetNumberOfElements(const uint32 dimension,
                         const uint32 nElements) {
    numberOfElements[dimension] = nElements;
}

void OPCUAReferenceContainer::SetNumberOfDimensions(const uint8 nDimensions) {
    numberOfElements = new uint32[nDimensions];
    numberOfDimensions = nDimensions;
}

CLASS_REGISTER(OPCUAReferenceContainer, "");

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

