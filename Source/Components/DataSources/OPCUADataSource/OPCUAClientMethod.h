/**
 * @file OPCUAClientMethod.h
 * @brief Header file for class OPCUAClientMethod
 * @date Oct 28, 2019 TODO Verify the value and format of the date
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

 * @details This header file contains the declaration of the class OPCUAClientMethod
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTMETHOD_H_
#define SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTMETHOD_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "OPCUAClientI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

class OPCUAClientMethod: public OPCUAClientI {
public:
    OPCUAClientMethod();

    ~OPCUAClientMethod();

    virtual bool SetServiceRequest(const uint16 *const namespaceIndexes,
                                   StreamString *const nodePaths,
                                   const uint32 numberOfNodes);

    virtual bool GetExtensionObjectByteString(const TypeDescriptor *const&entryTypes,
                                              const uint32 *const&entryArrayElements,
                                              const uint32 *const&entryNumberOfMembers,
                                              const uint32 entryArraySize,
                                              uint32 &nodeCounter,
                                              uint32 &index);

    bool SetObjectRequest(const uint16 methodNamespaceIndex,
                          StreamString methodPath);

    bool SetMethodRequest(const uint16 methodNamespaceIndex,
                          StreamString methodPath);

    bool MethodCall();

    bool GetExtensionObject();

private:

    /**
     * The array that stores all the open62541 NodeIDs of the monitored nodes.
     */
    UA_NodeId *monitoredNodes;

    /**
     * Temporary open62541 void structure for internal operations.
     */
    UA_Variant *tempVariant;

    /**
     * The NodeId of the method to be called in MethodCall function.
     */
    UA_NodeId methodNodeId;

    /**
     * The NodeId of the object that owns the method.
     */
    UA_NodeId objectMethod;

    /**
     * The array that stores all the ExtensionObjects when using Complex DataType Extensions.
     */
    UA_ExtensionObject *eos;

    UA_ExtensionObject *valuePtr;

    uint32 nOfEos;

    UA_ReadResponse readResponse;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTMETHOD_H_ */
	
