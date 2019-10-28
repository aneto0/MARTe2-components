/**
 * @file OPCUAClientI.h
 * @brief Header file for class OPCUAClientI
 * @date 25/10/2019
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

 * @details This header file contains the declaration of the class OPCUAClientI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTI_H_
#define SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
/*lint -u__cplusplus This is required as otherwise lint will get confused after including this header file.*/
#include "open62541.h"
/*lint -D__cplusplus*/
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

class OPCUAClientI {
public:

    OPCUAClientI();

    virtual ~OPCUAClientI();

    void SetServerAddress(StreamString address);

    bool Connect();

    void SetValueMemories();

    void SetDataPtr(const uint32 bodyLength);

    bool GetSignalMemory(void *&mem,
                         const uint32 idx,
                         const TypeDescriptor &valueTd,
                         const uint32 nElem);

    virtual bool GetExtensionObjectByteString(const TypeDescriptor *const&entryTypes,
                                         const uint32 *const&entryArrayElements,
                                         const uint32 *const&entryNumberOfMembers,
                                         const uint32 entryArraySize,
                                         uint32 &nodeCounter,
                                         uint32 &index) = 0;

    virtual bool SetServiceRequest(const uint16 *const namespaceIndexes,
                                   StreamString *const nodePaths,
                                   const uint32 numberOfNodes) = 0;


protected:

    void SeekDataPtr(const uint32 bodyLength);

    uint32 GetReferences(const UA_BrowseRequest bReq,
                         const char8 *const path,
                         uint16 &namespaceIndex,
                         uint32 &numericNodeId,
                         char8 *&stringNodeId);
    /**
     * Holds the server address
     */
    StreamString serverAddress;

    /**
     * The array that stores all the pointers to the memories related to all the monitored Nodes.
     */
    void **valueMemories;

    /**
     * open62541 client declaration.
     */
    UA_Client *opcuaClient;

    /**
     * Number of nodes to be managed
     */
    uint32 nOfNodes;

    /**
     * The dataPtr associated to the Extension Object ByteString.
     */
    void *dataPtr;

    /**
     * A copy of the dataPtr associated to the Extension Object ByteString to perform operations.
     */
    uint8 *tempDataPtr;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTI_H_ */

