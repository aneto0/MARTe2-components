/**
 * @file OPCUAClientRead.h
 * @brief Header file for class OPCUAClientRead
 * @date 28/10/2019
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

 * @details This header file contains the declaration of the class OPCUAClientRead
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTREAD_H_
#define SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTREAD_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "OPCUAClientI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Wrapper of a OPCUA Client for reading data to a OPCUA Server.
 * @details This class wraps all the functionalities to read data to a OPCUA Server.
 * The class supports both single nodes and ExtensionObjects.
 */
class OPCUAClientRead: public OPCUAClientI {
public:

    /**
     * @brief Default Constructor. NOOP.
     */
    OPCUAClientRead();

    /**
     * @brief Default Destructor. NOOP.
     */
    ~OPCUAClientRead();

    /**
     * @brief Calls the OPCUA Read service.
     * @details Gets the data from the OPCUA Secure Channel and writes them to the valueMemory of each monitoredNode.
     * @param[in] types the array with all the TypeDescriptor for each node to read.
     * @param[in] nElements the array with all the number of elements for each node to read.
     * @pre SetServiceRequest
     * @return true if the Read Service is executed correctly.
     */
    bool Read(const TypeDescriptor *const types,
              const uint32 *const nElements);

    /**
     * @see OPCUAClientI::SetServiceRequest
     */
    virtual bool SetServiceRequest(const uint16 *const namespaceIndexes,
                                   StreamString *const nodePaths,
                                   const uint32 numberOfNodes);

    /**
     * @see OPCUAClientI::GetExtensionObjectByteString
     */
    virtual bool GetExtensionObjectByteString(const TypeDescriptor *const&entryTypes,
                                              const uint32 *const&entryArrayElements,
                                              const uint32 *const&entryNumberOfMembers,
                                              const uint32 entryArraySize,
                                              uint32 &nodeCounter,
                                              uint32 &index);
    /**
     * @brief Gets the monitored Nodes pointer. (Testing purposes)
     */
    UA_NodeId * GetMonitoredNodes();


private:

    /**
     * @brief Wrapper of RegisterNodes OPCUA Service
     * @param[in] monitoredNodes the nodes to be registered
     * @return true if the RegisterNodes Request return UA_STATUSCODE_GOOD
     */
    bool RegisterNodes(const UA_NodeId *const monitoredNodes);

    /**
     * @brief Wrapper of UnregisterNodes OPCUA Service
     * @param[in] monitoredNodes the nodes to be unregistered
     * @return true if the UnregisterNodes Request return UA_STATUSCODE_GOOD
     */
    bool UnregisterNodes(const UA_NodeId *const monitoredNodes);

    /**
     * The array that stores all the open62541 NodeIDs of the monitored nodes.
     */
    UA_NodeId *monitoredNodes;

    /**
     * open62541 read request structure.
     */
    UA_ReadRequest readRequest;

    /**
     * open62541 readValues array.
     */
    UA_ReadValueId *readValues;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTREAD_H_ */

