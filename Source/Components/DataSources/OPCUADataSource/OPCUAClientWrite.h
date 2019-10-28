/**
 * @file OPCUAClientWrite.h
 * @brief Header file for class OPCUAClientWrite
 * @date Oct 28, 2019 TODO Verify the value and format of the date
 * @author lporzio TODO Verify the name and format of the author
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

 * @details This header file contains the declaration of the class OPCUAClientWrite
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRITE_H_
#define SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRITE_H_

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

class OPCUAClientWrite: public OPCUAClientI {
public:

    OPCUAClientWrite();

    ~OPCUAClientWrite();

    virtual bool SetServiceRequest(const uint16 *const namespaceIndexes,
                                   StreamString *const nodePaths,
                                   const uint32 numberOfNodes);

    virtual bool GetExtensionObjectByteString(const TypeDescriptor *const&entryTypes,
                                              const uint32 *const&entryArrayElements,
                                              const uint32 *const&entryNumberOfMembers,
                                              const uint32 entryArraySize,
                                              uint32 &nodeCounter,
                                              uint32 &index);

    /**
     * @brief Retrieve information from the Secure Channel about the ExtensionObject to be sent.
     * @details This method creates a ReadRequest and saves the ExtensionObject.
     * @return true if the ReadRequest returns StatusCode GOOD
     */
    bool GetExtensionObject();

    /**
     * @brief Sets the OPC UA Write request
     * @details This method create the correct Write request based on the properties of the value to write in the address space.
     */
    void SetWriteRequest(const uint32 idx,
                         const uint8 nDimensions,
                         const uint32 nElements,
                         const TypeDescriptor &type);

    /**
     * @brief Calls the OPCUA Write service.
     * @details Gets the data from valueMemory and calls the OPCUA Write Value Attribute service
     * on the monitored nodes.
     * @pre SetServiceRequest, SetWriteRequest
     */
    bool Write();

private:

    /**
     * @brief Wrapper of RegisterNodes OPCUA Service
     * @param[in] monitoredItems the nodes to be registered
     * @return true if the RegisterNodes Request return a StatusCode GOOD
     */
    bool RegisterNodes(UA_NodeId *const monitoredItems);

    /**
     * @brief Wrapper of UnregisterNodes OPCUA Service
     * @param[in] monitoredItems the nodes to be unregistered
     * @return true if the UnregisterNodes Request return a StatusCode GOOD
     */
    bool UnregisterNodes(UA_NodeId *const monitoredItems);

    /**
     * The array that stores all the open62541 NodeIDs of the monitored nodes.
     */
    UA_NodeId *monitoredNodes;

    /**
     * open62541 write request structure
     */
    UA_WriteRequest writeRequest;

    /**
     * The array that stores all the open62541 write values for the nodes to write
     */
    UA_WriteValue *writeValues;

    /**
     * Temporary open62541 void structure for internal operations.
     */
    UA_Variant *tempVariant;

    /**
     * The array that stores all the ExtensionObjects when using Complex DataType Extensions.
     */
    UA_ExtensionObject *eos;

    UA_ExtensionObject *valuePtr;

    uint32 nOfEos;

    UA_ReadResponse readResponse;

};

}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRITE_H_ */
	
