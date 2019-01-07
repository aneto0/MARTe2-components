/**
 * @file OPCUAClientWrapper.h
 * @brief Header file for class OPCUAClientWrapper
 * @date 21 Nov 2018 TODO Verify the value and format of the date
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

 * @details This header file contains the declaration of the class OPCUAClientWrapper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRAPPER_H_
#define SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRAPPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "/home/lporzio/open62541/build/open62541.h"
#include "Reference.h"
#include "StreamString.h"
#include "AdvancedErrorManagement.h"
#include <string.h>

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class OPCUAClientWrapper {

public:

    OPCUAClientWrapper();

    ~OPCUAClientWrapper();

    bool Connect();

    bool GetSignalMemory(void *&mem);

    bool BrowseAddressSpace(uint32 namespaceIndex, StreamString nodePath);

    bool Monitor();

    void UpdateMemory(UA_DataValue *value);

    void StateCallback(UA_ClientState clientState);

    void SubscriptionInactivity();

private:

    bool SearchNode(UA_BrowseRequest request,
                    StreamString nodeName,
                    uint16 namespaceIndex,
                    uint32 numericNodeId = 0u,
                    unsigned char* stringNodeId = 0,
                    size_t length = 0u);

    bool Read();

    UA_Variant * outValueMem;

    void* valueMemory;

    UA_NodeId monitoredNode; //It is a node Variable

    char * nodeName;

    UA_ClientConfig config;

    UA_Client * opcuaClient;

    UA_CreateSubscriptionRequest request;

    UA_CreateSubscriptionResponse response;

    UA_MonitoredItemCreateRequest monitorRequest;

    UA_MonitoredItemCreateResult monitorResponse;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTWRAPPER_H_ */

