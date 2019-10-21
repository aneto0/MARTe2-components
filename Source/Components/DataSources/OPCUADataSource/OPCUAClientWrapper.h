/**
 * @file OPCUAClientWrapper.h
 * @brief Header file for class OPCUAClientWrapper
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

 * @details This header file contains the declaration of the class OPCUAClientWrapper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRAPPER_H_
#define SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRAPPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include "open62541.h"
/*lint -D__cplusplus*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Reference.h"
#include "StreamString.h"

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
     * @brief Default Constructor.
     */
    OPCUAClientWrapper();

    /**
     * @brief Default destructor.
     */
    ~OPCUAClientWrapper();

    /**
     * @brief Sets the server address to allow the client to connect to the right source.
     * @param[in] address The OPCUA Server address with port
     */
    void SetServerAddress(StreamString address);

    /**
     * @brief Call the OPCUA Connect service on the client.
     * @return true if the connection has been established successfully.
     * @pre SetServerAddress
     */
    bool Connect();

    /**
     * @brief Links the target Node memory with the signal memory in input
     * @param[in, out] mem The signal memory buffer
     * @param[in] index the number of the monitored node position within the array that manages them
     * @param[in] valueTd the TypeDescriptor associated to the value related to the node
     * @param[in] nElem the number of elements of the value related to the node
     * @param[in] nDimensions the number of dimensions of the value related to the node
     * @return true if the monitored node has been set.
     * @pre SetValueMemories
     */
    bool GetSignalMemory(void *&mem,
                         const uint32 idx,
                         const TypeDescriptor &valueTd,
                         const uint32 nElem,
                         const uint8 nDimensions);

    /**
     * @brief Browse the OPCUA Address Space and create the read and write request.
     * @details This method extracts all the node names from the paths and uses them to
     * browse the address space through a TranslateBrowsePathToNodeId service request. Then it sets the monitored
     * node id from the result.
     * It also creates the Read and Write service requests with the Monitored Node ID.
     * @param[in] namespaceIndexes the array with the namespace indexes of the nodes to be monitored.
     * @param[in] nodePaths the array with all the relative browse paths of the nodes to be monitored.
     * @param[in] numberOfnodes the size of the above arrays.
     * @return true if the result of the TranslateBrowsePathToNodeId service request is not empty.
     * @pre Connect
     */
    bool SetServiceRequest(const uint16 *const namespaceIndexes,
                           StreamString *const nodePaths,
                           const uint32 numberOfNodes);

    /**
     * @brief Browse the OPCUA Address Space in order to retrieve information about the method to be called and the owner object.
     * @details This method extracts all the node names from the paths and uses them to
     * browse the address space through a TranslateBrowsePathToNodeId service request. Then it sets the owner object and the
     * method node ids from the result.
     * @param[in] methodNamespaceIndex the namespace index of the method to be called.
     * @param[in] methodPath the relative browse path of the method to be called.
     * @return true if the result of the TranslateBrowsePathToNodeId service requests is not empty.
     * @pre Connect
     */
    bool SetMethodRequest(const uint16 methodNamespaceIndex,
                          StreamString const methodPath);

    /**
     * @brief Initialise the value memory pointers.
     */
    void SetValueMemories();

    /**
     * @brief Initialise the dataPtr pointer (for ExtensionObject).
     * @details Allocate a memory area of bodyLength bytes.
     * @param[in] bodyLength the length of the ByteString
     */
    void SetDataPtr(const uint32 bodyLength);

    /**
     * @brief Creates the OPCUA Subscription and Monitored Item request. Runs the Client iteration service.
     * @details This method manages the creation of Subscription and Monitored Item requests for the monitored node.
     * A data change callback is registered for the OPCUA Monitored Item Create Data Change service.
     * When all the services has been started, it runs the OPCUA Client Iteration service.
     * @return true if the server is still initialising or all the requests has been executed successfully.
     * @pre SetTargetNodes
     */
    bool Monitor() const;

    /**
     * @brief Calls the OPCUA Write service.
     * @details Gets the data from valueMemory and calls the OPCUA Write Value Attribute service
     * on the monitored nodes.
     * @pre SetServiceRequest, SetWriteRequest
     */
    bool Write();

    /**
     * @brief Calls the OPCUA Read service.
     * @details Gets the data from the OPCUA Secure Channel and writes them to the valueMemory of each monitoredNode.
     * @param[in] types the array with all the TypeDescriptor for each node to read.
     * @param[in] nElements the array with all the number of elements for each node to read.
     * @return true if the Read Service is executed correctly.
     * @pre SetServiceRequest
     */
    bool Read(const TypeDescriptor *const types,
              const uint32 *const nElements);

    /**
     * @brief Calls the OPCUA Call service.
     * @details Uses Method Call Service to send an Extension Object through the Secure Channel.
     * @return true if the Call Service is executed correctly.
     * @pre SetMethodRequest
     */
    bool MethodCall();

    /**
     * @brief Set the sampling time
     * @param[in] sampleTime The sampling time desired.
     */
    void SetSamplingTime(const float64 sampleTime);

    /**
     * @brief Gets the pointer to the monitored nodes array
     */
    UA_NodeId* GetMonitoredNodes();

    /**
     * @brief Set the operation mode.
     * @details mode could be "Read", "Write" or "MethodCall"
     */
    void SetOperationMode(const char8 *const modeType);

    /**
     * @brief Sets the OPC UA Write request
     * @details This method create the correct Write request based on the properties of the value to write in the address space.
     */
    void SetWriteRequest(const uint32 idx,
                         const uint8 nDimensions,
                         const uint32 nElements,
                         const TypeDescriptor &type);

    /**
     * @brief Retrieve information from the Secure Channel about the ExtensionObject to be sent.
     * @details This method creates a ReadRequest and saves the ExtensionObject.
     * @return true if the ReadRequest returns StatusCode GOOD
     */
    bool GetExtensionObject();

    /**
     * @brief Decode a ByteString according to the OPCUA Standard Part 6 - 5.2.6
     * @details This methods decodes a ByteString according to the OPC UA Standard and assigns the correct pointers
     * to the value memories.
     * @param[in] entryType the TypeDescriptor array associated to each Introspection entry
     * @param[in] entryArrayElement the array that stores number of elements associated to each Introspection entry
     * @param[in] entryNumberOfMembers the array that stores number of members associated to each Introspection entry
     * @param[in] entryArraySize the size of the previous arrays
     * @param[in, out] nodeCounter the number of the current node
     * @param[in, out] index the index indicating the element from which the research shall start
     * @return true if the ByteString data pointer has been decoded successfully and all the value memories have been assigned.
     */
    bool DecodeExtensionObjectByteString(const TypeDescriptor *const&entryType,
                                         const uint32 *const&entryArrayElement,
                                         const uint32 *const&entryNumberOfMembers,
                                         const uint32 entryArraySize,
                                         uint32 &nodeCounter,
                                         uint32 &index);

    /**
     * @brief Encode a ByteString according to the OPCUA Standard Part 6 - 5.2.6
     * @details This methods encode a ByteString according to the OPC UA Standard assigning the memory of each signal to a specific position
     * into the ByteString Data Pointer.
     * @param[in] entryType the TypeDescriptor array associated to each Introspection entry
     * @param[in] entryArrayElement the array that stores number of elements associated to each Introspection entry
     * @param[in] entryNumberOfMembers the array that stores number of members associated to each Introspection entry
     * @param[in] entryArraySize the size of the previous arrays
     * @param[in, out] nodeCounter the number of the current node
     * @param[in, out] index the index indicating the element from which the research shall start
     * @return true if the ByteString data pointer has been encode successfully and all the value memories have been assigned.
     */
    bool EncodeExtensionObjectByteString(const TypeDescriptor *const&entryType,
                                         const uint32 *const&entryArrayElement,
                                         const uint32 *const&entryNumberOfMembers,
                                         const uint32 entryArraySize,
                                         uint32 &nodeCounter,
                                         uint32 &index);

#if 0    /**
     * @brief Update the valueMemory with the new data.
     * @param[in] value The new data value to write into the valueMemory.
     * @pre GetSignalMemory
     */
    void UpdateMemory(UA_DataValue *value);
#endif

private:

    /**
     * @brief Move back the ByteString data pointer according to a specific offset
     */
    void SeekDataPtr(const uint32 bodyLength);

    /**
     * @brief Gets the OPCUA Reference Type Id for each node of the path.
     * @details This method uses the OPCUA Browse and BrowseNext services to obtain the OPCUA Reference Type
     * of the node declared in the path.
     */
    uint32 GetReferences(const UA_BrowseRequest bReq,
                         const char8 *const path,
                         uint16 &namespaceIndex,
                         uint32 &numericNodeId,
                         char8 *&stringNodeId);

    /**
     * @brief Wrapper of RegisterNodes OPCUA Service
     * @param[in] monitoredItems the nodes to be registered
     * @return true if the RegisterNodes Request return a StatusCode GOOD
     */
    bool RegisterNodes(UA_NodeId *const monitoredItems);

    /**
     * @brief Wrapper of UnregisterNodes OPCUA Service
     * @param[in] monitoredItems the nodes to be unregistered
     * @return true if the UnregisterNodes Request return a StatusCode GOOD
     */
    bool UnregisterNodes(UA_NodeId *const monitoredItems);

    /**
     * Holds the server address
     */
    StreamString serverAddress;

    /**
     * The array that stores all the pointers to the memories related to all the monitored Nodes.
     */
    void **valueMemories;

    /**
     * Holds the sampling time
     */
    float64 samplingTime;

    /**
     * The array that stores all the open62541 NodeIDs of the monitored nodes.
     */
    UA_NodeId *monitoredNodes;

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
     * open62541 client configuration structure.
     */
    UA_ClientConfig *config;

    /**
     * open62541 client declaration.
     */
    UA_Client *opcuaClient;

    /**
     * open62541 read request structure.
     */
    UA_ReadRequest readRequest;

    /**
     * open62541 read response structure.
     */
    UA_ReadResponse readResponse;

    /**
     * The array that stores all the open62541 read values from nodes to read
     */
    UA_ReadValueId *readValues;

    /**
     * open62541 write request structure
     */
    UA_WriteRequest writeRequest;

    /**
     * The array that stores all the open62541 write values for the nodes to write
     */
    UA_WriteValue *writeValues;

    /**
     * Holds the ClientWrapper running mode. "Read" or "Write"
     */
    StreamString mode;

    /**
     * Temporary open62541 void structure for internal operations.
     */
    UA_Variant *tempVariant;

    /**
     * OPCUA BrowseRequest for SetServiceRequest.
     */
    UA_BrowseRequest bReq;

    /**
     * OPCUA BrowseRequest for SetMethodRequest.
     */
    UA_BrowseRequest mbReq;

    /**
     * OPCUA BrowsePath for SetServiceRequest.
     */
    UA_BrowsePath browsePath;

    /**
     * OPCUA BrowsePath for SetMethodRequest.
     */
    UA_BrowsePath mBrowsePath;

    /**
     * OPCUA TranslateBrowsePathsToNodeIdsRequest for SetServiceRequest.
     */
    UA_TranslateBrowsePathsToNodeIdsRequest tbpReq;

    /**
     * OPCUA TranslateBrowsePathsToNodeIdsRequest for SetMethodRequest to get owner object.
     */
    UA_TranslateBrowsePathsToNodeIdsRequest objectTbpReq;

    /**
     * OPCUA TranslateBrowsePathsToNodeIdsRequest for SetMethodRequest to get the method.
     */
    UA_TranslateBrowsePathsToNodeIdsRequest methodTbpReq;

    /**
     * Number of nodes to be managed
     */
    uint32 nOfNodes;

    /**
     * The dataPtr associated to the Extension Object ByteString.
     */
    void *dataPtr;

    /**
     * A copy of the dataPtr associated to the Extension Object ByteString to perform operations.
     */
    uint8 *tempDataPtr;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRAPPER_H_ */

