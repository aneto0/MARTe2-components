/**
 * @file OPCUAServer.h
 * @brief Header file for class OPCUAServer
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

 * @details This header file contains the declaration of the class OPCUAServer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUASERVER_H_
#define SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUASERVER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "/home/lporzio/open62541/build/open62541.h"
#include "AdvancedErrorManagement.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "ObjectRegistryDatabase.h"
#include "OPCUANode.h"
#include "OPCUAObject.h"
#include "OPCUAReferenceContainer.h"
#include "ReferenceContainer.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief OPCUAServer class. It manages the OPCUA Server life cycle.
 * @details A SingleThreadService which offers all the functionalities to read data from IntrospectionStructures
 * and to create the OPCUA Address Space correctly.
 *
 * All the nodes added in the AddressSpace will be OPCUA Variable type.
 *
 * The configuration syntax is  (names are only given as an example):
 * <pre>
 * +OPCUAServer
 *     Class = OPCUA::OPCUAServer
 *     Port = 4840 //Optional. Default is 4840
 *     CPUMask = 0x4
 *     AddressSpace = {
 *         MyNodeStructure1 = {
 *             Type = MyIntrospectionStructure1
 *         }
 *         MyNodeStructure2 = {
 *             Type = MyIntrospectionStructure2
 *         }
 *         ...
 *         MyNode = {
 *             Type = float64
 *         }
 *     }
 * </pre>
 */
class OPCUAServer: public Object, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
/**
 * @brief Default Constructor
 * @details Creates OPCUAServerConfiguration and creates a new OPCUA Server instance from OPCUA configuration.
 */
OPCUAServer    ();

    /**
     * @brief Default Destructor
     * @details The OPCUA Server instance is deleted and the OPCUA configuration is cleared.
     */
    virtual ~OPCUAServer();

    /**
     * @brief Initialise the Object and reads the Thread parameters
     * @return true if the Object and Thread parameters are successfully initialised.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Provides the context to read information from the IntrospectionStructures and Initialise
     * the Address Space.
     * @details An OPCUA Object named "MARTe2_Variables" is created in which the nodes are added.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @brief Set the running mode
     */
    void SetRunning(bool const running);

    /**
     * @brief Provides running mode
     */
    const bool GetRunning();

    /**
     * @brief Provides CPU mask
     */
    const uint32 GetCPUMask();

    /**
     * @brief Provides Thread Stack Size
     */
    const uint32 GetStackSize();

    /**
     * @brief Provides Port number where Server is listen to
     */
    const uint16 GetPort();

    /**
     * @brief Create the OPCUA Address Space starting from a OPCUAReferenceContainer.
     * @details Recursively read all the OPCUAReferenceContainer and create the OPCUAObject or OPCUANode.
     * All the NodeID will be numeric, starting from 3000.
     * @return true if all the nodes and object are added to the OPCUAServer correctly.
     */
    bool InitAddressSpace(ReferenceT<OPCUAReferenceContainer> ref);

    /**
     * @brief Read the structure recursively from the configuration file and retrieve all the informations about node types.
     */
    bool GetStructure(ReferenceT<OPCUAReferenceContainer> refContainer, const Introspection *intro);

    /**
     * The thread that manage the OPC UA Server functionalities.
     */
    SingleThreadService service;

private:

    /**
     * open62541 server object declaration
     */
    UA_Server * opcuaServer;

    /**
     * open62541 server configuration structure
     */
    UA_ServerConfig * opcuaConfig;

    /**
     * The number to assign to each node during Address Space initialization
     */
    uint32 nodeNumber;

    /**
     * Running mode
     */
    bool opcuaRunning;

    /**
     * The port where the Server listen
     */
    uint16 port;

    /**
     * The number to set Cpu affinity
     */
    uint32 cpuMask;

    /**
     * The size of the stack memory for the SignleThreadService
     */
    uint32 stackSize;

    /**
     * Private copy of the Configuration Database
     */
    ConfigurationDatabase cdb;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUASERVER_H_ */

