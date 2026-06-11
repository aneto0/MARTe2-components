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
#include "OPCUAClientWrite.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Output DataSource class that manages the write-only OPCUA client life cycle.
 * @details This DataSource allows to write data to Node Variables managed by an OPCUA Server.
 * This class uses the OPC UA RegisteredWrite Service.
 * You must assign the actual name of the Node you want to read as signal name.
 * Since it uses the TranslateBrowsePathToNodeId service, you must indicate the relative browse path of the Address Space
 * starting from the OPCUA Object of interest inside the "Objects" folder.
 *
 * It supports int8/16/32/64, uint8/16/32/64, float32/64, and Introspection Structure. It also supports OPCUA ExtensionObject structure.
 *
 * At most one ExtensionObject is currently supported per DataSource instance. If an ExtensionObject is used no other basic type signals can be added to the DataSource instance.
 *
 * At most one Introspection Structure is currently supported per DataSource instance. If an Introspection StructuredDataI is used no other basic type signals can be added to the DataSource instance.
 *
 * Strings are not supported yet.
 *
 * Authentication type can be defined (either None or UserPassword). When
 * using the UserPassword option, the UserPassword property must also be
 * defined with a value equal to the path of the file holding the 
 * authentication key. The file's structure is:
 * 
 * ```
 * AuthenticationKey = {
 *     Username = "user1"
 *     Password = "password1"
 * }
 * ```
 * 
 * The configuration syntax is (names are only given as an example):
 * <pre>
 * +OPCUA = {
 *     Class = OPCUADataSource::OPCUADSOutput
 *     Address = "opc.tcp://192.168.130.20:4840" //The OPCUA Server Address
 *     Authentication = None | UserPassword
 *     UserPasswordFile = /path/to/the/file
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
 *     }
 * }
 * </pre>
 *
 * <pre>
 * +OPCUA = {
 *     Class = OPCUADataSource::OPCUADSOutput
 *     Address = "opc.tcp://192.168.130.20:4840" //The OPCUA Server Address
 *     Authentication = None | UserPassword
 *     UserPasswordFile = /path/to/the/file
 *     Signals = {
 *         NodeStructure1 = {
 *             Type = MyStructure
 *             NamespaceIndex = 3
 *             Path = Object3.Block1.Block2.NodeStructure1
 *         }
 *     }
 * }
 * </pre>
 *
 * In the case one wants to read a structure as an OPCUA ExtensionObject using the Complex DataType Extension, the syntax is the following:
 * <pre>
 * +OPCUA = {
 *     Class = OPCUADataSource::OPCUADSOutput
 *     Address = "opc.tcp://192.168.130.20:4840" //The OPCUA Server Address
 *     ...
 *     Signals = {
 *         NodeStructure1 = {
 *             Type = MyStructure
 *             NamespaceIndex = 3
 *             Path = Object3.Block1.Block2.NodeStructure1
 *             ExtensionObject = "yes"
 *         }
 *     }
 * }
 * </pre>
 * When using Complex DataType Extension, the DataSource only allows to write 1 structure. If you need to add more signals you must add another OPCUADSOuput DataSource to your real time application.
 *
 * The DataSource allows associating a timestamp signal to one or more output signals. When a timestamp signal is configured, its value is used to populate the OPC UA SourceTimestamp field when writing the associated nodes.
 *
 * The timestamp signals must be of type uint64 and represents the time value to be used as the OPC UA SourceTimestamp. The timestamp is defined as the number of nanoseconds ellapsed since the UTC Unix Epoch and it will be automatically converted to the OPCUA DateTime. The nodes with timestamps cannot be written to OPC UA as values (even if the Path and NamespaceIndex are specified). 
 * 
 * The timestamp signals can be associated explicitly to specific nodes, or globally as a default.
 * <pre>
 * +OPCUA = {
 *     Class = OPCUADataSource::OPCUADSOutput
 *     Address = "opc.tcp://192.168.130.20:4840" //The OPCUA Server Address
 *     Authentication = None | UserPassword
 *     UserPasswordFile = /path/to/the/file
 *     Signals = {
 *         Node1 = {
 *             Type = uint32
 *             NamespaceIndex = 1
 *             Path = Object1.Block1.Block2.Node1
 *         }
 *         TimestampNode1 = {
 *             Type = uint64 //Timestamp shall be uint64
 *             Timestamp = {"Node1"} //List of nodes to be timestamped using this signal as the time source
 *         }
 *         Node2 = { //Not timestamped
 *             Type = uint32
 *             NamespaceIndex = 1
 *             Path = Object1.Block1.Block2.Node1
 *         }
 *     }
 * }
 * </pre>
 * 
 * To timestamp all signals against a common timestamp signal, the property DefaultTimestampSignal = 1 shall be used. Note that the nodes with a Timestamp specified still have precedence (i.e. they will ignore the global timestamp signal).
 * <pre>
 * +OPCUA = {
 *     Class = OPCUADataSource::OPCUADSOutput
 *     Address = "opc.tcp://192.168.130.20:4840" //The OPCUA Server Address
 *     Authentication = None | UserPassword
 *     UserPasswordFile = /path/to/the/file
 *     Signals = {
 *         TimestampGlobal = {
 *             Type = uint64 //Timestamp shall be uint64
 *             DefaultTimestampSignal = 1
 *         }
 *         Node1 = {
 *             Type = uint32
 *             NamespaceIndex = 1
 *             Path = Object1.Block1.Block2.Node1
 *         }
 *         TimestampNode1 = {
 *             Type = uint64 //Timestamp shall be uint64
 *             Timestamp = {"Node1"} //List of nodes to be timestamped using this signal as the time source. The DefaultTimestampSignal is ignored for these nodes.
 *         }
 *         Node2 = { //Timestamped with the TimestampGlobal
 *             Type = uint32
 *             NamespaceIndex = 1
 *             Path = Object1.Block1.Block2.Node1
 *         }
 *     }
 * }
 * </pre>
 *
 * ExtensionObjects and Introspection Structures can also be timestamped. Only one Timestamp signal with the property DefaultTimestampSignal shall be set.
 *
 ** <pre>
 * +OPCUA = {
 *     Class = OPCUADataSource::OPCUADSOutput
 *     Address = "opc.tcp://192.168.130.20:4840" //The OPCUA Server Address
 *     ...
 *     Signals = {
 *           TimestampGlobal = {
 *             Type = uint64 //Timestamp shall be uint64
 *             DefaultTimestampSignal = 1
 *         }
 *         NodeStructure1 = {
 *             Type = MyStructure
 *             NamespaceIndex = 3
 *             Path = Object3.Block1.Block2.NodeStructure1
 *             ExtensionObject = "yes" //Remove to use as an Introspection Structure
 *         }
 *     }
 * }
 * </pre>

 */
class OPCUADSOutput: public DataSourceI {

public:

    CLASS_REGISTER_DECLARATION()

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
     * @see DataSourceI::Initialise
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Gets the actual number of nodes and sets the correct browse path for each signal.
     * @details This method constructs the actual browse paths for each node
     * adding all the child nodes if the signal is an Introspection Structure.
     * @return true if all the paths were successfully constructed.
     * @see DataSourceI::SetConfiguredDatabase
     */
    virtual bool SetConfiguredDatabase(StructuredDataI &data);

    /**
     * @brief NOOP.
     * @return true
     * @see DataSourceI::AllocateMemory
     */
    virtual bool AllocateMemory();

    /**
     * @details The memory buffer is obtained after a Browse request on the OPCUA Server's Address Space through the OPCUAClientWrapper Helper Class.
     * @pre SetConfiguredDatabase
     * @see DataSourceI::GetSignalMemoryBuffer
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                                       const uint32 bufferIdx,
                                       void *&signalAddress);

    /**
     * @details Only OutputSignals are supported.
     * @return MemoryMapSynchronisedOutputBroker
     * @see DataSourceI::GetBrokerName
     */
    virtual const char8* GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @return true
     * @see DataSourceI::PrepareNextState
     */
    virtual bool PrepareNextState(const char8 *const currentStateName,
                                  const char8 *const nextStateName);

    /**
     * @details Provides the context to create the OPC UA Write service request.
     * @return true if all the services are executed correctly.
     * @see DataSourceI::Synchronise
     */
    virtual bool Synchronise();

    /**
     * @brief Gets the server address
     */
    const char8* GetServerAddress();

    /**
     * @brief Gets the OPCUA Client Pointer
     */
    OPCUAClientWrite * GetOPCUAClient();

private:

    /**
     * @brief Gets the actual number of nodes (OPCUA nodes - number of timestamp signals)
     */
    uint32 GetNumberOfNodes();

    /**
     * @brief Returns true if the signal at the input idx was declared as a Timestamp signal.
     */
    bool IsTimestampSignal(const uint32 idx);

    /**
     * @brief Returns true if the signal at the input idx was declared as a Timestamp signal with DefaultTimestampSignal = 1.
     */
    bool IsDefaultTimestampSignal(const uint32 idx);

    /**
     * @brief Helper method to populate the timestampDatabase with the mapping of the timestamp signals to the OPCUA nodes.
     */
    bool PopulateTimestampDatabase();

    /**
     * @brief Helper method to populate the OPCUA timestamp nodes.
     */
    bool PopulateTimestampNodes();

    /**
     * @brief Find the signal idx against the targetName. This method is used to map the timestamping signal to the relevant OPCUA nodes.
     */
    bool FindTargetIndex(const char8 * const targetName, uint32 &idx);

    /**
     * @brief Helper method to setup the OPCUA nodes.
     */
    bool SetupNodes();

    /**
     * @brief Helper method to the Setup the OPCUA node as an ExtensionObject.
     * @return the size of the ExtensionObject
     */
    uint32 SetupExtensionObject();

    /**
     * Helper method to map the OPCUA nodes to the relevant MARTe signals.
     */
    bool MapNodeSignals();

    /**
     * Helper method to map the OPCUA ExtensionObject to the MARTe signals.
     */
    bool MapExtensionObjectSignals();

    /**
     * @brief Read the structure recursively and gets informations about the length of the ByteString (for ExtensionObject).
     * @param[in] intro the first introspection from which starting the research
     * @param[out] bodyLength the length of the ByteString
     * @return true if the structure has been introspected correctly
     */
    bool GetBodyLength(const Introspection *const intro,
                       uint32 &bodyLength);

    /**
     * @brief Read the structure recursively and gets informations about the dimension (for ExtensionObject).
     * @param[in] intro the first introspection from which starting the research
     * @param[out] arraySize the dimension of the structure
     */
    void GetStructureDimensions(const Introspection *const intro,
                                uint32 &arraySize);

    /**
     * @brief Read the structure recursively from the configuration file and retrieve all the informations about node types.
     * @param[in] intro the first introspection from which starting the research
     * @param[out] entryArrayElements the array that holds the number of elements for introspection entry
     * @param[out] entryTypes the array that holds the type for introspection entry
     * @param[out] entryNumberOfMembers the array that holds the number of members for introspection entry
     * @param[out] index which elements shall be taken from the previous arrays
     * @return true if the structure has been introspected correctly
     */
    bool GetStructure(const Introspection *const intro,
                      uint32 *&entryArrayElements,
                      TypeDescriptor *&entryTypes,
                      uint32 *&entryNumberOfMembers,
                      uint32 &index);

    /**
     * Validate that all the rules for the extension objects are met.
     */
    bool ValidateExtensionObject(); 

    /**
     * Validate that all the rules for the structured signals are met.
     */
    bool ValidateStructuredSignal(); 

    /**
     * Pointer to the Helper Class for the main Client
     */
    OPCUAClientWrite * masterClient;

    /**
     * Holds the value of the configuration parameter Address
     */
    StreamString serverAddress;

    /**
     * The number of OPC UA Nodes
     */
    uint32 numberOfNodes;

    /**
     * True if it holds an extensionObject 
     */
    bool isExtensionObject;

    /**
     * True if it holds a structured signal (that is not an extensionObject) 
     */
    bool isStructuredSignal;

    /**
     * Original index of the structured signal
     */
    uint32 structuredSignalIdx;

    /**
     * The array that stores all the browse paths for each
     * node to write.
     */
    StreamString *paths;

    /**
     * The array that stores all the namespaceIndexes for each
     * node to write
     */
    uint16 *namespaceIndexes;

    /**
     * Maps the nodes to the MARTe signals (given that they may be interleaved with timestamp signals).
     */
    uint32 *signalIdxMap;
    
    /**
     * Holds the memory for the timestamp signal values.
     */
    uint64 *timestampSignals;

    /**
     * Helper database to store the raw signals received at the Initialise.
     */
    ConfigurationDatabase tempSignalsDatabase;

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
     * The array that stores the array size of the structure properties array (for ExtensionObject)
     */
    uint32 entryArraySize;

    /**
     * The array that stores the number of elements for each node to write
     */
    uint32 *nElements;

    /**
     * The array that stores all the data types, as TypeDescritor, for each node to write
     */
    TypeDescriptor *types;

    /**
     * The array that stores the type name for structured data type 
     */
    StreamString structuredTypeName;

    /**
     * The flag defining if authentication is used when connecting to the server
     * 
     */
    bool authenticate;

    /**
     * The username used for authentication
     * 
     */
    StreamString username;

    /**
     * The password used for authentication
     * 
     */
    StreamString password;

    /**
     * Helper database to store the mapping of the timestamp signals to the nodes.
     */
    ConfigurationDatabase timestampDatabase;

    /**
     * For each node store the location of its timestamp signal pointer (may be NULL if not used).
     */
    uint64 **timestampNodes;

    /**
     * Trigger signal set?
     */
    bool triggerSignalSet;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSOUTPUT_H_ */

