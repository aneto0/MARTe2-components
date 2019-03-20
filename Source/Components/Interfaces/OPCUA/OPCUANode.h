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

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include <cstring>
#include "/home/lporzio/open62541/build/open62541.h"
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
     * @return true if all the parameters and attributes are set correctly
     */
    virtual bool GetOPCVariable(OPCUANodeSettings &settings, TypeDescriptor nodeType, uint32 nodeNumber);

    /**
     * @see OPCUAReferenceContainer::IsNode
     * @return true
     */
    virtual bool IsNode();

private:

    /**
     * @brief Sets all the array related attributes and build the UA_Variant
     */
    void InitArray(OPCUANodeSettings &settings, const UA_DataType *type, uint64 nElem);

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

