/**
 * @file OPCUAClientI.h
 * @brief Header file for class OPCUAClientI
 * @date 25/10/2019
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

 * @details This header file contains the declaration of the class OPCUAClientI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTI_H_
#define SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include "open62541.h"
/*lint -D__cplusplus*/
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief OPCUA Client Interface from which every other OPCUA client wrapper shall inherit.
 * @details This interface comprises all the functionalities to create a valid OPCUA Client.
 */
/*lint -e9150 no need to make non POD members private.*/
class OPCUAClientI {
public:

    /**
     * @brief Default Constructor. Creates a new client and set a default configuration.
     */
    OPCUAClientI();

    /**
     * @brief Default Destructor.
     */
    virtual ~OPCUAClientI();

    /**
     * @brief Sets the server address in order to connect to the right endpoint.
     * @param[in] address The address of the OPCUA Server
     */
    void SetServerAddress(StreamString address);

    /**
     * @brief Connects the Client to the Server
     * @pre SetServerAddress
     * @return true if the OPCUA connection return UA_STATUSCODE_GOOD
     */
    bool Connect();

    /**
     * @brief Connects the Client to the Server
     * @param[in] username
     * @param[in] password
     * @pre SetServerAddress
     * @return true if the OPCUA connection return UA_STATUSCODE_GOOD
     */
    bool Connect(StreamString & username, StreamString & password);

    /**
     * @brief Allocates the right amount of memory for every node to read or write.
     * @param[in] numberOfNodes The number of nodes to read/write
     */
    void SetValueMemories(const uint32 numberOfNodes);

    /**
     * @brief Allocates the right amount of memory for the OPCUA Bytestring (ExtensionObject)
     * @details When using complex data type extension, this function allocates the right amount of
     * memory for the Bytestring of the ExtensionObject to be read/wrote.
     * @param[in] bodyLength The number of bytes of the Bytestring
     */
    void SetDataPtr(const uint32 bodyLength);

    /**
     * @brief Links the value memories from the outside to the node memories of the class.
     * @details This function allows to connect the value memories of the signals allocated with the
     * SetValueMemories function, with a memory pointer provided in input. This is useful for DataSources or
     * Message Clients.
     * @param[in,out] mem The memory pointer to be connected to the node memory.
     * @param[in] idx The index of the current node.
     * @param[in] valueTd The TypeDescriptor associated with the current node value.
     * @param[in] nElem The number of elements of the current node.
     * @pre SetValueMemories || SetDataPtr
     * @return true if valueMemories != NULL_PTR(void**)
     */
    bool GetSignalMemory(void *&mem,
                         const uint32 idx,
                         const TypeDescriptor &valueTd,
                         const uint32 nElem);

    /**
     * @brief Encodes/Decodes the ExtensionObject Bytestring based on the structure information.
     * @details This class expects the children classes to implement a function to encode/decode the ExtensionObject
     * Bytestring mapping the dataPtr according to the OPCUA Standard Part 6 - Mappings.
     * @param[in] entryTypes The TypeDescriptor array associated to each node of the structure.
     * @param[in] entryArrayElements The array of number of elements associated to each node of the structure.
     * @param[in] entryNumberOfMembers The array of number of members associated to each node of the structure.
     * @param[in] entryArraySize The size of the previous arrays.
     * @param[in, out] nodeCounter The number of the current node.
     * @param[in, out] index The index of the current elements from the previous arrays.
     * @pre SetDataPtr
     * @return true if the ByteString has been encoded/decoded correctly.
     */
    virtual bool GetExtensionObjectByteString(const TypeDescriptor *const&entryTypes,
                                         const uint32 *const&entryArrayElements,
                                         const uint32 *const&entryNumberOfMembers,
                                         const uint32 entryArraySize,
                                         uint32 &nodeCounter,
                                         uint32 &index) = 0;

    /**
     * @brief Sets the OPCUA Service Request to be performed from the client wrapper.
     * @details This function uses the information from the node paths and the namespace indexes
     * to get the OPCUA NodeIds using OPCUA Browse Service Set. It also sets all the parameters of the read/write request
     * when needed.
     * @param[in] namespaceIndexes The array containing the namespace index for each node.
     * @param[in] nodePaths The array containing the browse path for each node.
     * @param[in] numberOfNodes The number of nodes to be browsed.
     * @return true if all the Browse Requests return UA_STATUSCODE_GOOD
     */
    virtual bool SetServiceRequest(const uint16 *const namespaceIndexes,
                                   StreamString *const nodePaths,
                                   const uint32 numberOfNodes) = 0;

    /**
     * @brief Returns the Server address. (For Testing Purposes)
     */
    StreamString GetServerAddress() const;

    /**
     * @brief Returns the value memories pointer. (For Testing Purposes)
     */
    void ** GetValueMemories();

    /**
     * @brief Returns the number of nodes set for this client. (For Testing Purposes)
     */
    uint32 GetNumberOfNodes() const;

    /**
     * @brief Returns the bytestring data pointer. (For Testing Purposes)
     */
    void * GetDataPtr();

protected:

    /**
     * @brief Moves back the data pointer of bodyLength bytes.
     * @param[in] bodyLength how many steps the pointer has to be moved
     */
    void SeekDataPtr(const uint32 bodyLength);

    /**
     * @brief Gets the reference associated to a specific OPCUA NodeId
     * @details This function uses the OPCUA Browse Service Set to get the references
     * associated to a NodeId.
     * @param[in] bReq The OPCUA BrowseRequest
     * @param[in] path the string containing the path in the Address Space
     * @param[in] namespaceIndex the namespace index of the current node
     * @param[in] numericNodeId the value of the NodeId if it is numeric
     * @param[in] stringNodeId the value of the NodId if it is string type
     * @return the numeric Reference NodeId.
     */
    uint32 GetReferences(const UA_BrowseRequest bReq,
                         const char8 *const path,
                         uint16 &namespaceIndex,
                         uint32 &numericNodeId,
                         char8 *&stringNodeId);

    /**
     * Holds the server address
     */
    StreamString serverAddress;

    /**
     * The array that stores all the pointers to the memories related to all the monitored Nodes.
     */
    void **valueMemories;

    /**
     * open62541 client declaration.
     */
    UA_Client *opcuaClient;

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

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTI_H_ */

