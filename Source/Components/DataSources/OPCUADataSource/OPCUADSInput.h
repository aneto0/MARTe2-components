/**
 * @file OPCUADSInput.h
 * @brief Header file for class OPCUADSInput
 * @date Nov 16, 2018 TODO Verify the value and format of the date
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

#include "DataSourceI.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "MultiThreadService.h"
#include "OPCUAClientWrapper.h"
#include "AdvancedErrorManagement.h"
#include "MemoryMapSynchronisedInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief OPCUADSInput class. It is the OPCUA Input DataSource that manages the read-only OPCUA client life cycle.
 * @details This DataSource allows to read Node Variables from an OPCUA Server. Data is read
 * from an OPCUA Address Space in the context of a thread.
 * You must assign the actual name of the Node you want to read as signal name.
 * Since it uses the TranslateBrowsePathToNodeId service, you must indicate the relative browse path of the Address Space
 * starting from the OPCUA Object of interest inside the "Objects" folder.
 *
 * It supports int8/16/32/64, uint8/16/32/64, float32/64, char8 and Introspection Structure.
 *
 * Maximum number of signal supported is 1000.
 *
 * The configuration syntax is (names are only given as an example):
 * <pre>
 * +OPCUA = {
 *     Class = OPCUADataSource::OPCUADSInput
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
 *
 *
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
     * @details TODO
     */
    virtual ~OPCUADSInput();

    /**
     * @brief Loads and verifies all the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Gets the actual number of clients and sets the correct browse path for each signal. Starts the clients.
     * @details This method calculates the number of clients to start for each signal. Then it construct the actual browse paths
     * adding all the child nodes if the signal is an Introspection Structure.
     * It also sets the address for each client and starts them.
     * @return true if all the paths were successfully constructed and the thread service is running.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief See DataSourceI::AllocateMemory. NOOP.
     * @return true
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers. NOOP.
     * @return 1
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @details The memory buffer is obtained after a Browse request on the OPCUA Server's Address Space
     * @pre SetConfiguredDatabase
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief See DataSourceI::GetBrokerName.
     * @details Only InputSignals are supported.
     * @return MemoryMapInputBroker.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::PrepareNextState.
     * @return true
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Provides the context to create the OPCUA Subscription and Monitored Item services.
     * @return ErrorManagement::NoError if the OPCUA Subscription, the OPCUA Monitored Item request,
     * OPCUA Monitored Item create data change service are successfully created and the client is running with no error.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    virtual bool Synchronise();

private:

    MultiThreadService executor;

    OPCUAClientWrapper * masterClient;

    StreamString sync;

    StreamString serverAddress;

    uint32 numberOfClients;

    uint32 numberOfNodes;

    float64 samplingTime;

    /**
     * The array which stores all the browse paths for each
     * node to read
     */
    StreamString * paths;

    /**
     * Temporary array to store paths read from configuration
     */
    StreamString *tempPaths;

    /**
     * The array which stores all the namespaceIndexes for each
     * node to read
     */
    uint32 * namespaceIndexes;

    /**
     * Temporary array to store value read from configuration
     */
    uint32 *tempNamespaceIndexes;

    uint32 * nElements;

    TypeDescriptor * types;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSINPUT_H_ */

