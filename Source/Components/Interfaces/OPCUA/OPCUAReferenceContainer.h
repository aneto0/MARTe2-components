/**
 * @file OPCUAReferenceContainer.h
 * @brief Header file for class OPCUAReferenceContainer
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

 * @details This header file contains the declaration of the class OPCUAReferenceContainer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUAREFERENCECONTAINER_H_
#define SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUAREFERENCECONTAINER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainer.h"
#include "OPCUATypes.h"
#include "/home/lporzio/open62541/build/open62541.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Struct which defines all the Node Properties to be defined
 * @details UA_NodeId could be one of the IdType defined by the OPCUA standard
 */

namespace MARTe {

class OPCUAReferenceContainer: public ReferenceContainer {

public:
    CLASS_REGISTER_DECLARATION()

OPCUAReferenceContainer    ();

    ~OPCUAReferenceContainer();

    virtual bool IsObject();

    virtual bool IsNode();

    virtual bool GetOPCVariable(OPCUANodeSettings &settings, TypeDescriptor nodeType, uint32 nodeNumber);

    virtual bool GetOPCObject(OPCUAObjectSettings &settings, uint32 nodeNumber);

    virtual void SetFirst(const bool value);

    virtual const bool IsFirstObject();

    void SetParent(const uint32 parentId);

    const uint32 GetParentNodeId();

    void SetNodeId(const uint32 newNodeId);

    const uint32 GetNodeId();

    void SetNodeType(TypeDescriptor type);

    TypeDescriptor GetNodeType();

    void SetNumberOfElements(const uint32 dimension, const uint32 nElements);

    void SetNumberOfDimensions(const uint8 nDimensions);


protected:

    TypeDescriptor nodeType;

    uint32 nodeId;

    uint32 parentNodeId;

    uint8 numberOfDimensions;

    uint32* numberOfElements;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUAREFERENCECONTAINER_H_ */

