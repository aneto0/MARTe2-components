/**
 * @file OPCUADSInput.h
 * @brief Header file for class OPCUADSInput
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

 * @details This header file contains the declaration of the class OPCUADSInput
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSINPUT_H_
#define SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSINPUT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "DataSourceI.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MultiThreadService.h"
#include "OPCUAClientRead.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Input DataSource class that manages the read-only OPCUA client life cycle.
 * @details This DataSource allows to read Node Variables from an OPCUA Server. Data is read
 * from an OPCUA Address Space in the context of a thread or in the context of Synchronise method.
 * This class uses the OPC UA RegisteredRead Service and the OPC UA Monitored Item Service.
 * You must assign the actual name of the Node you want to read as signal name.
 * Since it uses the TranslateBrowsePathToNodeId service, you must indicate the relative browse path of the Address Space
 * starting from the OPCUA Object of interest inside the "Objects" folder.
 *
 * It supports int8/16/32/64, uint8/16/32/64, float32/64, and Introspection Structure. It also support OPCUA ExtensionObject structure.
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
 *     Class = OPCUADataSource::OPCUADSInput
 *     Address = "opc.tcp://192.168.130.20:4840" //The OPCUA Server Address
 *     Authentication = None | UserPassword
 *     UserPasswordFile = /path/to/the/file
 *     ReadMode = "Read" //"Read" uses OPCUA Read Service, "Monitor" uses OPCUA MonitoredItem Service. (Optional) Default = "Read"
 *     SamplingTime = 1 //ms. Only if ReadMode is "Monitor"
 *     Synchronise = "yes" //"yes" uses the Synchronise method (and thus is executed in the context of the real-time thread, "no" to enable a decoupled SingleThreadService Execute method). Default = "no"
 *     CpuMask = 0xffu //(Optional) Only if Synchronise option is "no". Default = 0xffu
 *     StackSize = 10000000 //(Optional) Only if Synchronise option is "no". Default = THREADS_DEFAULT_STACKSIZE
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
 *
 * In the case one wants to read a structure as an OPCUA ExtensionObject using the Complex DataType Extension, the syntax is the following:
 * <pre>
 * +OPCUA = {
 *     Class = OPCUADataSource::OPCUADSInput
 *     Address = "opc.tcp://192.168.130.20:4840" //The OPCUA Server Address
 *     ...
 *     Synchronise = "yes"
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
 * When using Complex DataType Extension, the DataSource only allows to write 1 structure. If you need to add more signals you must add
 * another OPCUADSInput DataSource to your real time application.
 */
class OPCUADSInput: public DataSourceI, public EmbeddedServiceMethodBinderI {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP
     */
    OPCUADSInput();

    /**
     * @brief Default destructor.
     */
    virtual ~OPCUADSInput();

    /**
     * @brief Loads and verifies all the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     * @see DataSourceI::Initialise
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Gets the actual number of nodes and sets the correct browse path for each signal. Starts the client if Thread Service is enabled.
     * @details This method constructs the actual browse paths for each node
     * adding all the child nodes if the signal is an Introspection Structure.
     * @return true if all the paths were successfully constructed and the thread service is running.
     * @see DataSourceI::SetConfiguredDatabase
     */
    virtual bool SetConfiguredDatabase(StructuredDataI &data);

    /**
     * @see DataSourceI::AllocateMemory
     * @brief NOOP.
     * @return true
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
     * @see DataSourceI::GetBrokerName
     * @details Only InputSignals are supported.
     * @return MemoryMapInputBroker if Synchronise option is set to "no", otherwise it returns MemoryMapSynchronisedInputBroker.
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
     * @brief Provides the context to create the OPCUA Subscription and Monitored Item request or the Read service request.
     * @return ErrorManagement::NoError if the OPCUA Subscription, the OPCUA Monitored Item request,
     * OPCUA Monitored Item create data change service are successfully created and the client is running with no error, or
     * the OPCUA Read Service has been executed correctly.
     * @see EmbeddedServiceMethodBinderI::Execute
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo &info);

    /**
     * @details Provides the context to create the OPCUA Subscription and Monitored Item request or the Read service request.
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
    OPCUAClientRead * GetOPCUAClient();


private:

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
     * @param[in]  intro the first introspection from which starting the research
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
     * The Thread service executor
     */
    SingleThreadService executor;

    /**
     * Pointer to the Helper Class for the main Client
     */
    OPCUAClientRead * masterClient;

    /**
     * Holds the value of the configuration parameter ReadMode
     */
    StreamString readMode;

    /**
     * Holds the value of the configuration parameter ExtensionObject
     */
    StreamString *extensionObject;

    /**
     * Holds the value of the configuration parameter Synchronise
     */
    StreamString sync;

    /**
     * Holds the value of the configuration parameter Address
     */
    StreamString serverAddress;

    /**
     * The number of signals during initialise
     */
    uint32 nOfSignals;

    /**
     * The number of OPC UA Nodes
     */
    uint32 numberOfNodes;

    /**
     * Holds the value of the configuration parameter SamplingTime
     */
    float64 samplingTime;

    /**
     * The array that stores all the browse paths for each
     * node to read
     */
    StreamString *paths;

    /**
     * Temporary array to store paths read from configuration
     */
    StreamString *tempPaths;

    /**
     * The array that stores all the namespaceIndexes for each node to read
     */
    uint16 *namespaceIndexes;

    /**
     * Temporary array to store value read from configuration
     */
    uint16 *tempNamespaceIndexes;

    /**
     * Temporary array to store numberOfElements read from configuration
     */
    uint32 *tempNElements;

    /**
     * The array that stores the NumberOfElements for each node to read
     */
    uint32 *nElements;

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
     * The array that stores all the data types, as TypeDescritor, for each node to read
     */
    TypeDescriptor *types;

    /**
     * The array that stores the type name for structured data types (for ExtensionObject)
     */
    StreamString *structuredTypeNames;

    /**
     * CPU affinity number for the executor thread
     */
    uint32 cpuMask;

    /**
     * The stack size
     */
    uint32 stackSize;

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
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSINPUT_H_ */

