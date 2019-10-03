/**
 * @file OPCUADSOutput.h
 * @brief Header file for class OPCUADSOutput
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

 * @details This header file contains the declaration of the class OPCUADSOutput
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSOUTPUT_H_
#define SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSOUTPUT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "DataSourceI.h"
#include "MemoryMapAsyncOutputBroker.h"
#include "OPCUAClientWrapper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief OPCUADSOutput class. It is the OPCUA Output DataSource that manages the write-only OPCUA client life cycle.
 * @details This DataSource allows to write data to Node Variables managed by an OPCUA Server.
 * This class uses the OPC UA Write Service.
 * You must assign the actual name of the Node you want to read as signal name.
 * Since it uses the TranslateBrowsePathToNodeId service, you must indicate the relative browse path of the Address Space
 * starting from the OPCUA Object of interest inside the "Objects" folder.
 *
 * It supports int8/16/32/64, uint8/16/32/64, float32/64, and Introspection Structure. Strings are not supported yet.
 *
 * The configuration syntax is (names are only given as an example):
 * <pre>
 * +OPCUA = {
 *     Class = OPCUADataSource::OPCUADSOutput
 *     Address = "opc.tcp://192.168.130.20:4840" //The OPCUA Server Address
 *     Signals = {
 *         Node1 = {
 *             Type = uint32
 *             NamespaceIndex = 1
 *             Path = Object1.Block1.Block2.Node1
 *         }
 *         Node2 = {
 *             Type = float32
 *             NamespaceIndex = 2
 *             Path = Object2.Folder1.Node2
 *         }
 *         NodeStructure1 = {
 *             Type = MyStructure
 *             NamespaceIndex = 3
 *             Path = Object3.Block1.Block2.NodeStructure1
 *         }
 *     }
 * }
 * </pre>
 */
class OPCUADSOutput: public DataSourceI {

public:CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default Constructor
     */
    OPCUADSOutput();

    /**
     * @brief Default Destructor
     */
    virtual ~OPCUADSOutput();

    /**
     * @brief Loads and verifies all the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Gets the actual number of nodes and sets the correct browse path for each signal.
     * @details This method constructs the actual browse paths for each node
     * adding all the child nodes if the signal is an Introspection Structure.
     * @return true if all the paths were successfully constructed.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI &data);

    /**
     * @see DataSourceI::AllocateMemory
     * @brief NOOP.
     * @return true
     */
    virtual bool AllocateMemory();

    /**
     * @see DataSourceI::GetNumberOfMemoryBuffers
     * @details The memory buffer is obtained after a Browse request on the OPCUA Server's Address Space through the OPCUAClientWrapper Helper Class.
     * @pre SetConfiguredDatabase
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                                       const uint32 bufferIdx,
                                       void *&signalAddress);

    /**
     * @see DataSourceI::GetBrokerName
     * @details Only OutputSignals are supported.
     * @return MemoryMapSynchronisedOutputBroker
     */
    virtual const char8* GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @see DataSourceI::PrepareNextState
     * @return true
     */
    virtual bool PrepareNextState(const char8 *const currentStateName,
                                  const char8 *const nextStateName);

    /**
     * @see DataSourceI::Synchronise
     * @details Provides the context to create the OPC UA Write service request.
     * @return true if all the services are executed correctly.
     */
    virtual bool Synchronise();

    /**
     * @brief Gets the server address
     */
    const char8* GetServerAddress();

    /**
     * @brief Gets the OPCUA Client pointer
     */
    OPCUAClientWrapper* GetClient();

private:

    bool GetBodyLength(const Introspection *const intro,
                       uint32 &bodyLength);

    void GetStructureDimensions(const Introspection *const intro,
                                uint32 &arraySize);

    /**
     * @brief Read the structure recursively from the configuration file and retrieve all the informations about node types.
     */
    bool GetStructure(const Introspection *const intro,
                      uint32 *&entryArrayElements,
                      TypeDescriptor *&entryTypes,
                      uint32 *&entryNumberOfMembers,
                      uint32 &index);

    /**
     * Pointer to the Helper Class for the main Client
     */
    OPCUAClientWrapper *masterClient;

    /**
     * Holds the value of the configuration parameter Address
     */
    StreamString serverAddress;

    /**
     * The number of Signals during initialise
     */
    uint32 nOfSignals;

    /**
     * The number of OPC UA Nodes
     */
    uint32 numberOfNodes;

    /**
     * Holds the value of the configuration parameter ExtensionObject
     */
    StreamString *extensionObject;

    /**
     * The array that stores all the browse paths for each
     * node to write.
     */
    StreamString *paths;

    /**
     * Temporary array to store paths read from configuration
     */
    StreamString *tempPaths;

    /**
     * The array that stores all the namespaceIndexes for each
     * node to write
     */
    uint16 *namespaceIndexes;

    /**
     * Temporary array to store value read from configuration
     */
    uint16 *tempNamespaceIndexes;

    uint32 *tempNElements;

    /**
     * The array that stores the NumberOfElements for each IntrospectionEntry (for ExtensionObject)
     */
    uint32 *entryArrayElements;

    TypeDescriptor *entryTypes;

    uint32 *entryNumberOfMembers;

    uint32 entryArraySize;

    /**
     * The array that stores the data's number of dimension for each node to write
     */
    uint8 *nDimensions;

    /**
     * The array that stores the number of elements for each node to write
     */
    uint32 *nElements;

    /**
     * The array that stores all the data types, as TypeDescritor, for each node to write
     */
    TypeDescriptor *types;

    /**
     * The array that stores the type name for structured data types (for ExtensionObject)
     */
    StreamString *structuredTypeNames;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSOUTPUT_H_ */

