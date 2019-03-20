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
#include "OPCUAClientWrapper.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief OPCUADSInput class. It is the OPCUA Input DataSource that manages the read-only OPCUA client life cycle.
 * @details This DataSource allows to read Node Variables from an OPCUA Server. Data is read
 * from an OPCUA Address Space in the context of a thread or in the context of Synchronise method.
 * This class uses the OPC UA Read Service and the OPC UA Monitored Item Service.
 * You must assign the actual name of the Node you want to read as signal name.
 * Since it uses the TranslateBrowsePathToNodeId service, you must indicate the relative browse path of the Address Space
 * starting from the OPCUA Object of interest inside the "Objects" folder.
 *
 * It supports int8/16/32/64, uint8/16/32/64, float32/64, and Introspection Structure. Strings are not supported yet.
 *
 * The configuration syntax is (names are only given as an example):
 * <pre>
 * +OPCUA = {
 *     Class = OPCUADataSource::OPCUADSInput
 *     Address = "opc.tcp://192.168.130.20:4840" //The OPCUA Server Address
 *     SamplingTime = 1
 *     Synchronise = "yes" //"yes" if you want to use Synchronise method, "no" to enable SingleThreadService Execute method
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
class OPCUADSInput: public DataSourceI, public EmbeddedServiceMethodBinderI {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP
     */
OPCUADSInput    ();

    /**
     * @brief Default destructor.
     */
    virtual ~OPCUADSInput();

    /**
     * @brief Loads and verifies all the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Gets the actual number of nodes and sets the correct browse path for each signal. Starts the client if Thread Service is enabled.
     * @details This method constructs the actual browse paths for each node
     * adding all the child nodes if the signal is an Introspection Structure.
     * @return true if all the paths were successfully constructed and the thread service is running.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

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
     * @details Only InputSignals are supported.
     * @return MemoryMapInputBroker if Synchronise option is set to "no", otherwise it returns MemoryMapSynchronisedInputBroker.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @see DataSourceI::PrepareNextState
     * @return true
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Provides the context to create the OPCUA Subscription and Monitored Item request or the Read service request.
     * @return ErrorManagement::NoError if the OPCUA Subscription, the OPCUA Monitored Item request,
     * OPCUA Monitored Item create data change service are successfully created and the client is running with no error, or
     * the OPCUA Read Service has been executed correctly.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @see DataSourceI::Synchronise
     * @details Provides the context to create the OPCUA Subscription and Monitored Item request or the Read service request.
     * @return true if all the services are executed correctly.
     */
    virtual bool Synchronise();

    /**
     * @brief Gets the server address
     */
    const char8 * GetServerAddress();
    /**
     * @brief Gets the OPCUA Client pointer
     */
    OPCUAClientWrapper * GetClient();

private:

    /**
     * The Thread service executor
     */
    SingleThreadService executor;

    /**
     * Pointer to the Helper Class for the main Client
     */
    OPCUAClientWrapper * masterClient;

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
    uint32 numberOfSignals;

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
    StreamString * paths;

    /**
     * Temporary array to store paths read from configuration
     */
    StreamString *tempPaths;

    /**
     * The array that stores all the namespaceIndexes for each
     * node to read
     */
    uint32 * namespaceIndexes;

    /**
     * Temporary array to store value read from configuration
     */
    uint32 *tempNamespaceIndexes;

    /**
     * The array that stores the NumberOfElements for each node to read
     */
    uint32 * nElements;

    /**
     * The array that stores all the data types, as TypeDescritor, for each node to read
     */
    TypeDescriptor * types;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSINPUT_H_ */

