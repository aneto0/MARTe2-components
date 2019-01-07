/**
 * @file OPCUATypes.h
 * @brief Header file for class OPCUATypes
 * @date 10 Dec 2018 TODO Verify the value and format of the date
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

 * @details This header file contains the declaration of the class OPCUATypes
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUATYPES_H_
#define SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUATYPES_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "/home/lporzio/open62541/build/open62541.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/* TODO Add struct for others OPCUA Node Type. */

struct NodeProperties {
    MARTe::uint32 nType;
    UA_VariableAttributes attr;
    UA_UInt32 value;
    UA_NodeId nodeId;
    UA_QualifiedName nodeName;
    UA_NodeId parentNodeId;
    UA_NodeId parentReferenceNodeId;
};

typedef struct NodeProperties * OPCUANodeSettings;

struct ObjectProperties {
    MARTe::uint32 nType;
    UA_ObjectAttributes attr;
    UA_NodeId nodeId;
    UA_QualifiedName nodeName;
};

typedef struct ObjectProperties * OPCUAObjectSettings;

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUATYPES_H_ */

