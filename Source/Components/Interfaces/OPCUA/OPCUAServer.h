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
/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include "open62541.h"
/*lint -D__cplusplus*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
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
 * @brief Interface class that manages the OPCUA Server life cycle.
 * @details A SingleThreadService which offers all the functionalities to read data from IntrospectionStructures
 * and to create the OPCUA Address Space correctly.
 *
 * All the nodes added in the AddressSpace will be of OPCUA Variable type.
 * 
 * Authentication type can be defined (either None or UserPassword). When
 * using the UserPassword option, the UserPassword property must also be
 * defined with a value equal to the path of the file holding the 
 * authentication keys. The file's structure is:
 * 
 * ```
 * AuthenticationKeys = {
 *     KeyA = {
 *         Username = "user1"
 *         Password = "password1"
 *     }
 *     KeyB = {
 *         Username = "user2"
 *         Password = "password2"
 *     }
 *     ...
 * }
 * ```
 *
 * The configuration syntax is  (names are only given as an example):
 * <pre>
 * +OPCUAServer
 *     Class = OPCUA::OPCUAServer
 *     Port = 4840 //Optional. Default is 4840
 *     CPUMask = 0x4
 *     Authentication = None | UserPassword
 *     UserPasswordFile = /path/to/the/file
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
     * @see Object::Initialise
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Provides the context to read information from the IntrospectionStructures and Initialise
     * the Address Space.
     * @details An OPCUA Object named "MARTe2_Variables" is created in which the nodes are added.
     * @see EmbeddedServiceMethodBinderI::Execute
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @brief Set the running mode
     * @param[in] running true to enable running mode
     */
    void SetRunning(bool const running);

    /**
     * @brief Provides running mode
     * @return true if server is currently running
     */
    const bool GetRunning() const;

    /**
     * @brief Provides CPU mask
     * @return Assigned CPU mask
     */
    const uint32 GetCPUMask() const;

    /**
     * @brief Provides Thread Stack Size
     * @return Assigned Thread Stack Size
     */
    const uint32 GetStackSize() const;

    /**
     * @brief Provides Port number where Server is listening
     * @return Assigned Port number
     */
    const uint16 GetPort() const;

    /**
     * The thread that manage the OPC UA Server functionalities.
     */
    SingleThreadService service;

private:

    /**
     * @brief Create the OPCUA Address Space starting from a OPCUAReferenceContainer.
     * @details Recursively read all the OPCUAReferenceContainer and create the OPCUAObject or OPCUANode.
     * All the NodeID will be numeric, starting from 3000.
     * @param[in] ref Reference that will serve as base of the Address Space.
     * @return true if all the nodes and object are added to the OPCUAServer correctly.
     */
    bool InitAddressSpace(ReferenceT<OPCUAReferenceContainer> ref);

    /**
     * @brief Read the structure recursively from the configuration file and retrieve all the informations about node types.
     * @param[out] refContainer the OPCUAReferenceContainer which will be the starting point for the recursive function
     * @param[in] intro the first introspection from which starting the research
     * @return true if the structure has been loaded correctly with no issues about the type or the format
     */
    bool GetStructure(ReferenceT<OPCUAReferenceContainer> refContainer, const Introspection * const intro);

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

