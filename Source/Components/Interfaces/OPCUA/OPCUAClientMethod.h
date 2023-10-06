/**
 * @file OPCUAClientMethod.h
 * @brief Header file for class OPCUAClientMethod
 * @date 28/10/2019
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
/**
 * @brief Wrapper of a OPCUA Client for writing data as structures to a OPCUA Server using method call
 * via MARTe Message System.
 * @details This class wraps all the functionalities to call methods on a OPCUA Server.
 * The class supports ExtensionObjects only.
 */
class OPCUAClientMethod: public OPCUAClientI {
public:

    /**
     * @brief Default Constructor. NOOP
     */
    OPCUAClientMethod();

    /**
     * @brief Default Destructor. NOOP
     */
    ~OPCUAClientMethod();

    /**
     * @see OPCUAClientI::SetServiceRequest
     */
    virtual bool SetServiceRequest(const uint16 *const namespaceIndexes,
                                   StreamString *const nodePaths,
                                   const uint32 numberOfNodes);

    /**
     * @see OPCUAClientI::GetExtensionObjectByteString
     */
    virtual bool GetExtensionObjectByteString(const TypeDescriptor *const&entryTypes,
                                              const uint32 *const&entryArrayElements,
                                              const uint32 *const&entryNumberOfMembers,
                                              const uint32 entryArraySize,
                                              uint32 &nodeCounter,
                                              uint32 &index);

    /**
     * @brief Sets the OPCUA Owner Object.
     * @details This function uses the information from the node paths and the namespace indexes
     * to get the OPCUA NodeIds using OPCUA Browse Service Set.
     * @param[in] methodNamespaceIndex The namespace index of the method node.
     * @param[in] nodePaths The browse path of the method node.
     * @return true if all the Browse Requests return UA_STATUSCODE_GOOD
     */
    bool SetObjectRequest(const uint16 methodNamespaceIndex,
                          StreamString methodPath);

    /**
     * @brief Sets the OPCUA Method.
     * @details This function uses the information from the node paths and the namespace indexes
     * to get the OPCUA NodeIds using OPCUA Browse Service Set.
     * @param[in] methodNamespaceIndex The namespace index of the method node.
     * @param[in] nodePaths The browse path of the method node.
     * @return true if all the Browse Requests return UA_STATUSCODE_GOOD
     */
    bool SetMethodRequest(const uint16 methodNamespaceIndex,
                          StreamString methodPath);

    /**
     * @brief Wraps a OPCUA Method Call.
     * @pre SetServiceRequest && SetObjectRequest && SetMethodRequest && SetExtensionObject
     * @return true if the method call returns UA_STATUSCODE_GOOD
     */
    bool MethodCall();

    /**
     * @brief Retrieve information from the Secure Channel about the ExtensionObject to be sent.
     * @details This method creates a ReadRequest and saves the ExtensionObject.
     * @return true if the ReadRequest returns UA_STATUSCODE_GOOD
     */
    bool SetExtensionObject();

    /**
     * @brief Gets the monitored Nodes pointer. (Testing purposes)
     */
    UA_NodeId * GetMonitoredNodes();

    /**
     * @brief Gets the extension object pointer. (Testing purposes)
     */
    UA_ExtensionObject * GetExtensionObject();

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

    /**
     * The array that stores the ExtensionObjects read from the Server setting up the Write Request.
     */
    UA_ExtensionObject *valuePtr;

    /**
     * The number of ExtensionObject to be written.
     */
    uint32 nOfEos;

    /**
     * The ReadResponse struct to be used during SetExtensionObject.
     */
    UA_ReadResponse readResponse;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTMETHOD_H_ */
	
