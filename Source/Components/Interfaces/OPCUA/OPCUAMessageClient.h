/**
 * @file OPCUAMessageClient.h
 * @brief Header file for class OPCUAMessageClient
 * @date 27/09/2019
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

 * @details This header file contains the declaration of the class OPCUAMessageClient
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUAMESSAGECLIENT_H_
#define SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUAMESSAGECLIENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CLASSMETHODREGISTER.h"
#include "MessageI.h"
#include "OPCUAClientMethod.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief A Message Interface through a OPC UA Method Call Service
 * @details This class wraps a OPC UA Client, allowing to call OPC UA Methods on a OPC UA Server. This Interface accept a structure via MARTe2 Message mechanism
 * and maps it into an ExtensionObject ByteString according to the OPC UA Standard Part 6 - Mappings. The registered method allows this class to write the
 * structure into the selected OPC UA Server as an input argument of a Method. The registered MARTe2 method is OPCUAMethodCall.
 *
 * The Structure description has to be included into a node named "Structure". The method description has to be included into a node named "Method".
 *
 * The configuration syntax is (names are only given as an example):
 * <pre>
 * +OPCUAMessageClient = {
 *     Class = OPCUA::OPCUAMessageClient
 *     Address = "opc.tcp://192.168.130.20:4840" //The OPCUA Server Address
 *     Signals = {
 *         Structure = {
 *             SCU_Config = {
 *                 Type = MyStructuredType //Registered as IntrospectionStructure
 *                 NamespaceIndex = 1
 *                 Path = Object1.Block1.Block2.SCU_Config
 *                 NumberOfElements = 2
 *             }
 *         }
 *         Method = {
 *             NamespaceIndex = 3
 *             Path = Object1.Block1.Block2.Object.Method
 *         }
 *     }
 * }
 * </pre>
 */
class OPCUAMessageClient: public Object, public MessageI {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP
     */
    OPCUAMessageClient();

    /**
     * @brief Default destructor.
     */
    ~OPCUAMessageClient();

    /**
     * @brief Loads and verifies all the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Map a StructuredDataI into signal memories and send it through OPC UA as input argument of a Method Call.
     * @details Registered method to be called via MARTe2 Message mechanism. The StructuredDataI in input is mapped
     * and it is sent via OPC UA Method Call as input argument.
     * @param[in] data the StructureData received from the message
     * @return NoError if the structured has been transferred successfully or CommunicationError is the Method Call failed.
     */
    ErrorManagement::ErrorType OPCUAMethodCall(StructuredDataI &data);

    /**
     * @brief Returns the ServerAddress (for testing purposes)
     */
    const char8* GetServerAddress() const;

    /**
     * @brief Returns the OPCUA Client pointer (for testing purposes)
     */
    OPCUAClientMethod * GetOPCUAClient();

private:

    /**
     * @brief Read the structure recursively and gets informations about the dimension (for ExtensionObject).
     * @param[in] intro the first introspection from which starting the research
     * @param[out] arraySize the dimension of the structure
     */
    void GetStructureDimensions(const Introspection *const intro,
                                uint32 &arraySize);

    /**
     * @brief Read the structure recursively from the configuration file and retrieve all the informations about node types.
     * @param[in] intro The introspection from which starting the research
     * @param[out] entryArrayElements the array that holds the number of elements for each introspection entry
     * @param[out] entryTypes the array that holds the type for each introspection entry
     * @param[out] entryNumberOfMembers the array that holds the number of members for each introspection entry
     * @param[out] entryMemberNames the array that holds the member name for each introspection entry
     * @param[in,out] index which elements shall be taken from the previous arrays
     * @return true if the structure has been introspected correctly
     */
    bool GetStructure(const Introspection * const intro,
                      uint32 *&entryArrayElements,
                      TypeDescriptor *&entryTypes,
                      uint32 *&entryNumberOfMembers,
                      const char8 **&entryMemberNames,
                      uint32 &index);

    /**
     * @brief Read the structure recursively and gets informations about the length of the ByteString (for ExtensionObject).
     * @param[in] intro the first introspection from which starting the research
     * @param[out] bodyLength the length of the ByteString
     * @return true if the structure has been introspected correctly
     */
    bool GetBodyLength(const Introspection *const intro,
                       uint32 &bodyLength);

    /**
     * @brief Read the structure recursively and gets informations about the number of base type nodes (for ExtensionObject).
     * @param[in] intro the first introspection from which starting the research
     * @param[out] nOfNodes the number of nodes
     * @return true if the structure has been introspected correctly
     */
    bool GetNumberOfNodes(const Introspection *const intro,
                          uint32 &nOfNodes);

    /**
     * @brief Maps a StructuredDataI into the value memories to be sent.
     * @details Reads the StructuredDataI and assigns the values to the correct signal area memory.
     * @param[in] data the structure to be mapped
     * @param[in] values the array of values to be read from the StructuredDataI
     * @param[in,out] index which elements shall be taken from the previous array
     * @param[out] nodeCounter the number of the node to be processed.
     * @return true if the structure has been introspected correctly
     */
    bool MapStructuredData(StructuredDataI &data,
                           AnyType **&values,
                           uint32 &index,
                           uint32 &nodeCounter);

    /**
     * Pointer to the Helper Class for the main Client
     */
    OPCUAClientMethod *masterClient;

    /**
     * Holds the value of the configuration parameter Address
     */
    StreamString serverAddress;

    /**
     * The array that stores the NumberOfElements for each IntrospectionEntry (for ExtensionObject)
     */
    uint32 *entryArrayElements;

    /**
     * The array that stores the Type for each IntrospectionEntry (for ExtensionObject)
     */
    TypeDescriptor *entryTypes;

    /**
     * The array that stores the NumberOfMembers for each IntrospectionEntry (for ExtensionObject)
     */
    uint32 *entryNumberOfMembers;

    /**
     * The array that stores the member name for each IntrospectionEntry (for ExtensionObject)
     */
    const char8 **entryMemberNames;

    /**
     * Stores the array size of the structure properties array (for ExtensionObject)
     */
    uint32 entryArraySize;

    /**
     * Temporary array to store paths read from configuration
     */
    StreamString *tempPaths;

    /**
     * Temporary array to store value read from configuration
     */
    uint16 *tempNamespaceIndexes;

    /**
     * The array that stores the type name for structured data types (for ExtensionObject)
     */
    StreamString *structuredTypeNames;

    /**
     * The number of signals during initialise
     */
    uint32 nOfSignals;

    /**
     * The number of OPC UA Nodes
     */
    uint32 numberOfNodes;

    /**
     * Temporary array to store numberOfElements read from configuration
     */
    uint32 *tempNElements;

    /**
     * Pointer Array to store all the signal addresses.
     */
    void **signalAddresses;

    /**
     * Pointer Array to store values read from Message StructuredDataI input argument
     */
    AnyType **values;

    /**
     * The namespaceIndex of the method to be called
     */
    uint16 methodNamespaceIndex;

    /**
     * The browsePath of the method to be called
     */
    StreamString methodPath;

    /**
     * The MessageFilter
     */
    ReferenceT<RegisteredMethodsMessageFilter> filter;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUAMESSAGECLIENT_H_ */

