/**
 * @file OPCUAClientWrapper.h
 * @brief Header file for class OPCUAClientWrapper
 * @date 21 Nov 2018 TODO Verify the value and format of the date
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

 * @details This header file contains the declaration of the class OPCUAClientWrapper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRAPPER_H_
#define SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRAPPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "/home/lporzio/open62541/build/open62541.h"
#include "Reference.h"
#include "StreamString.h"
#include "AdvancedErrorManagement.h"
#include <string.h>

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Helper class for OPCUA DataSource that wraps the OPCUA Client functionalities.
 * @details This class is a wrapper for the OPCUA Client that the OPCUA DataSource will create
 * for each signal. It offers all the methods to read/write Nodes from an OPCUA Address Space and
 * to manage the Monitored Item and Subscription services with the callbacks.
 */

class OPCUAClientWrapper {

public:

    /**
     * @brief Default constructor.
     */
    OPCUAClientWrapper(const char8* const modeType);

    /**
     * @brief Default destructor.
     */
    ~OPCUAClientWrapper();

    /**
     * @brief Sets the server address to allow the client to connect to the right source.
     * @param[in] address The OPCUA Server address with port
     */
    void SetServerAddress(char* address);

    /**
     * @brief Call the OPCUA Connect service on the client.
     * @return true if the connection has been established successfully.
     * @pre SetServerAddress
     */
    bool Connect();

    /**
     * @brief Links the target Node memory with the signal memory in input
     * @param[in] mem The signal memory buffer
     * @return true if the monitored node has been set.
     */
    bool GetSignalMemory(void *&mem,
                         uint32 index,
                         const TypeDescriptor & valueTd,
                         const uint32 nElem,
                         const uint8 nDimensions);

    /**
     * @brief Browse the OPCUA Address Space and sets the monitored node declared by the path.
     * @details This method extracts the all the node names from the path and uses them to
     * browse the create a TranslateBrowsePathToNodeId service request. Then it sets the monitored
     * node id from the result.
     * @param[in] namespaceIndex The namespace index of the node to be monitored.
     * @param[in] nodePath The relative browse path of the node to be monitored.
     * @return true if the result of the TranslateBrowsePathToNodeId service request is not empty.
     * @pre Connect
     */
    bool SetTargetNodes(uint32 * namespaceIndexes,
                        StreamString * nodePaths,
                        uint32 numberOfNodes);

    /**
     * @brief Creates the OPCUA Subscription and Monitored Item request. Runs the Client iteration service.
     * @details This method manages the creation of Subscription and Monitored Item requests for the monitored node.
     * A data change callback is registered for the OPCUA Monitored Item Create Data Change service.
     * When all the services has been started, it runs the OPCUA Client Iteration service.
     * @return true if the server is still initialising or all the requests has been executed successfully.
     * @pre BrowseAddressSpace
     */
    bool Monitor();

    /**
     * @brief Calls the OPCUA Write Value Attribute service.
     * @details Gets the data from valueMemory and calls the OPCUA Write Value Attribute service
     * on the monitored node.
     * @pre BrowseAddressSpace
     */
    bool Write(uint32 numberOfNodes);

    /**
     * @brief Update the valueMemory with the new data.
     * @param[in] value The new data value to write into the valueMemory.
     * @pre GetSignalMemory
     */
    void UpdateMemory(UA_DataValue *value);

    /**
     * @brief [DEBUG ONLY] Prints the client state.
     * param[in] clientState The OPCUA Client State
     */
    void StateCallback(UA_ClientState clientState);

    /**
     * @brief [DEBUG ONLY] Prints the client subscription state. NOOP
     */
    void SubscriptionInactivity();

    void SetMaxNumberOfChar(uint32 nElements);

    void SetSamplingTime(float64 sampleTime);

    bool Read(uint32 numberOfNodes,
              TypeDescriptor * types,
              uint32 * nElements);

private:

    /**
     * @brief Gets the OCUA Reference Type Id for each node of the path.
     * @details This method uses the OPCUA Browse and BrowseNext services to obtain the OPCUA Reference Type
     * of the node declared in the path.
     */
    uint32 GetReferenceType(UA_BrowseRequest bReq,
                            char* path,
                            uint16 &namespaceIndex,
                            uint32 &numericNodeId,
                            char* &stringNodeId);

    void SetWriteRequest(uint32 idx,
                         uint8 nDimensions,
                         uint32 nElements,
                         TypeDescriptor type);

    char* serverAddress;

    UA_Variant * outValueMem;

    void** valueMemories;

    uint32 numberOfCharElements;

    float64 samplingTime;

    UA_NodeId * monitoredNodes;

    UA_ClientConfig config;

    UA_Client * opcuaClient;

    UA_CreateSubscriptionRequest request;

    UA_CreateSubscriptionResponse response;

    UA_MonitoredItemCreateRequest monitorRequest;

    UA_MonitoredItemCreateResult monitorResponse;

    UA_ReadRequest readRequest;

    UA_ReadResponse readResponse;

    UA_ReadValueId * readValues;

    UA_WriteRequest writeRequest;

    UA_WriteValue * writeValues;

    StreamString mode;

    UA_Variant * tempVariant;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRAPPER_H_ */

