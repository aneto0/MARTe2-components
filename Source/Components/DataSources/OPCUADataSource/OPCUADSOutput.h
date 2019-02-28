/**
 * @file OPCUADSOutput.h
 * @brief Header file for class OPCUADSOutput
 * @date 10 Jan 2019 TODO Verify the value and format of the date
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

 * @details This header file contains the declaration of the class OPCUADSOutput
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSOUTPUT_H_
#define SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSOUTPUT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "DataSourceI.h"
#include "MemoryMapAsyncOutputBroker.h"
#include "OPCUAClientWrapper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class OPCUADSOutput: public DataSourceI {

public:
    CLASS_REGISTER_DECLARATION()

OPCUADSOutput    ();

    virtual ~OPCUADSOutput();

    virtual bool Initialise(StructuredDataI & data);

    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    virtual bool AllocateMemory();

    uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    virtual bool Synchronise();

private:

    OPCUAClientWrapper * masterClient;

    StreamString serverAddress;

    uint32 numberOfClients;

    uint32 numberOfNodes;

    StreamString * paths;

    StreamString *tempPaths;

    uint32 * namespaceIndexes;

    uint32 *tempNamespaceIndexes;

    uint32 numberOfBuffers;

    uint32 cpuMask;

    uint32 stackSize;

    uint8 * nDimensions;

    uint32 * nElements;

    TypeDescriptor * types;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSOUTPUT_H_ */

