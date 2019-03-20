/**
 * @file OPCUAReferenceContainer.h
 * @brief Header file for class OPCUAReferenceContainer
 * @date 12/03/2019
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

#include "/home/lporzio/open62541/build/open62541.h"
#include "OPCUATypes.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Extension of the base ReferenceContainer class in order to add functionalities for OPCUA Server configuration.
 * @details Parent Class of OPCUANode and OPCUAObject classes. It extends the base class ReferenceContainer
 * adding set and get common function to OPCUA Nodes and Objects.
 */
class OPCUAReferenceContainer: public ReferenceContainer {

public:
    CLASS_REGISTER_DECLARATION()

/**
 * @brief Default Constructor
 */
OPCUAReferenceContainer    ();

    /**
     * @brief Default Destructor
     */
    ~OPCUAReferenceContainer();

    /**
     * @brief Checks if a Node is an object.
     * @return false
     */
    virtual bool IsObject();

    /**
     * @brief Checks if a Node is a Variable
     * @return false
     */
    virtual bool IsNode();

    /**
     * @brief Generate a new OPCUA NodeID for the Node Variable and sets all the NodeProperties
     * @details Sets all the required attributes to build a new NodeID for the current Node Variable, such as
     * qualified name, localization, value type and all the hierarchies properties (parentNodeId and References).
     * @param[out] settings The NodeProperties struct to be populated with all the data available
     * @param[in] nodeType The TypeDescriptor of the data type the Node will manage
     * @param[in] nodeNumber The numeric ID to be assigned to the OPCUA NodeID related to the current Node Variable
     * @return false
     */
    virtual bool GetOPCVariable(OPCUANodeSettings &settings, TypeDescriptor nodeType, uint32 nodeNumber);

    /**
     * @brief Generate a new OPCUA NodeID for the Node Object and sets all the ObjectProperties
     * @details Sets all the required attributes to build a new NodeID for the current Node Variable, such as
     * qualified name, localization and the right reference
     * @param[out] settings The ObjectProperties to be populated with all the data available
     * @param[in] nodeNumber The numeric ID to be assigned to the OPCUA NodeID related to the current Node Object
     * @return false
     */
    virtual bool GetOPCObject(OPCUAObjectSettings &settings, uint32 nodeNumber);

    /**
     * @brief Set the related parameter if the current Node is the first of the Address Space to be constructed
     */
    virtual void SetFirst(const bool value);

    /**
     * @brief Checks if the current Node is the first Object to be constructed in the Address Space
     */
    virtual const bool IsFirstObject();

    /**
     * @brief Sets the numeric ID of the ParentNodeID for the current Node
     */
    void SetParent(const uint32 parentId);

    /**
     * @brief Gets the numeric ID of the parentNodeID for the current Node
     */
    const uint32 GetParentNodeId();

    /**
     *
     * @brief Sets the numeric node ID for the current Node
     */
    void SetNodeId(const uint32 newNodeId);

    /**
     * @brief Gets the numeric node ID for the current Node
     */
    const uint32 GetNodeId();

    /**
     * @brief Sets the data type to be managed by the current Node
     */
    void SetNodeType(TypeDescriptor type);

    /**
     * @brief Gets the data type managed by the current Node
     * @return The TypeDescriptor of the data type
     */
    TypeDescriptor GetNodeType();

    /**
     * @brief Sets the number of elements of the data managed by the current Node
     * @param[in] dimension The number of dimensions
     * @param[in] nElements The number of Elements
     */
    void SetNumberOfElements(const uint32 dimension, const uint32 nElements);

    /**
     * @brief Sets the number of dimensions of the data managed by the current Node
     * @param[in] nDimensions The number of dimensions
     */
    void SetNumberOfDimensions(const uint8 nDimensions);

    /**
     * @brief Gets the array that stores numbers of elements of the current node
     */
    uint32* GetNumberOfElements();

    /**
     * @brief Gets the number of dimensions of the current node
     */
    const uint8 GetNumberOfDimensions();


protected:

    /**
     * The TypeDescriptor associated to the data managed by the current Node
     */
    TypeDescriptor nodeType;

    /**
     * The numeric ID of the current Node
     */
    uint32 nodeId;

    /**
     * The numeric ID of the parent of the current Node
     */
    uint32 parentNodeId;

    /**
     * The number of Dimensions of the data managed by the current Node
     */
    uint8 numberOfDimensions;

    /**
     * The number of Elements of the data managed by the current Node
     */
    uint32* numberOfElements;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUAREFERENCECONTAINER_H_ */

