/**
 * @file OPCUANode.h
 * @brief Header file for class OPCUANode
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

 * @details This header file contains the declaration of the class OPCUANode
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUANODE_H_
#define SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUANODE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include "open62541.h"
/*lint -D__cplusplus*/
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "ObjectRegistryDatabase.h"
#include "OPCUAReferenceContainer.h"
#include "ReferenceContainer.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Class that manages the OPCUA Node structure
 * @details The class inherit from OPCUAReferenceContainer and implements the GetOPCVariable method
 */
class OPCUANode: public OPCUAReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default Constructor
     */
    OPCUANode    ();

    /**
     * Default Destructor
     */
    ~OPCUANode();

    /**
     * @see OPCUAReferenceContainer::GetOPCVariable
     * @param[out] settings the OPCUANodeSettings.
     * @param[in] nType the node type.
     * @param[in] nNumber the node number.
     * @return true if all the parameters and attributes are set correctly
     */
    virtual bool GetOPCVariable(OPCUA::OPCUANodeSettings &settings, const TypeDescriptor nType, const uint32 nNumber);

    /**
     * @see OPCUAReferenceContainer::IsNode
     * @return true if node is a Variable
     */
    virtual bool IsNode();

private:

    /**
     * @brief Sets all the array related attributes and build the UA_Variant
     * @param[in] settings the OPCUANodeSettings to be filled with the array information.
     * @param[in] type the OPC UA data type related to the array
     * @param[in] nElem number of elements of the array
     * @return true if the OPC UA operation return a STATUSCODE GOOD
     */
    bool InitArray(OPCUA::OPCUANodeSettings const &settings, const UA_DataType * const type, const uint64 nElem);

    /**
     * Numeric ID of the parent reference for the current Node
     */
    uint32 parentReferenceNodeId;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUANODE_H_ */

