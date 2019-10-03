/**
 * @file OPCUAMessageClient.h
 * @brief Header file for class OPCUAMessageClient
 * @date Sep 27, 2019 TODO Verify the value and format of the date
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
#include "ConfigurationDatabase.h"
#include "MessageI.h"
#include "OPCUAClientWrapper.h"
#include "RegisteredMethodsMessageFilter.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class OPCUAMessageClient: public Object, public MessageI {
public:CLASS_REGISTER_DECLARATION()

    OPCUAMessageClient();

    ~OPCUAMessageClient();

    virtual bool Initialise(StructuredDataI &data);

    ErrorManagement::ErrorType OPCUAMethodCall(StructuredDataI &data);

private:

    void GetStructureDimensions(const Introspection *const intro,
                                uint32 &arraySize);

    /**
     * @brief Read the structure recursively from the configuration file and retrieve all the informations about node types.
     */
    bool GetStructure(const Introspection *const intro,
                      uint32 *&entryArrayElements,
                      TypeDescriptor *&entryTypes,
                      uint32 *&entryNumberOfMembers,
                      uint32 &arraySize);

    bool GetBodyLength(const Introspection *const intro,
                       uint32 &bodyLength);

    bool GetNumberOfNodes(const Introspection *const intro,
                          uint32 &nOfNodes);

    /**
     * Pointer to the Helper Class for the main Client
     */
    OPCUAClientWrapper *masterClient;

    /**
     * Holds the value of the configuration parameter Address
     */
    StreamString serverAddress;

    /**
     * The array that stores the NumberOfElements for each IntrospectionEntry (for ExtensionObject)
     */
    uint32 *entryArrayElements;

    TypeDescriptor *entryTypes;

    uint32 *entryNumberOfMembers;

    uint32 entryArraySize;

    /**
     * Temporary array to store paths read from configuration
     */
    StreamString *tempPaths;

    /**
     * The array that stores all the namespaceIndexes for each
     * node to read
     */
    uint16 *namespaceIndexes;

    /**
     * Temporary array to store value read from configuration
     */
    uint16 *tempNamespaceIndexes;

    /**
     * The array that stores all the data types, as TypeDescritor, for each node to read
     */
    TypeDescriptor *types;

    /**
     * The array that stores the type name for structured data types (for ExtensionObject)
     */
    StreamString *structuredTypeNames;

    /**
     * The array that stores the NumberOfElements for each node to read
     */
    uint32 *nElements;

    /**
     * The array that stores all the browse paths for each
     * node to read
     */
    StreamString *paths;

    /**
     * The number of signals during initialise
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

    uint32 *tempNElements;

    ReferenceT<RegisteredMethodsMessageFilter> filter;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUAMESSAGECLIENT_H_ */

