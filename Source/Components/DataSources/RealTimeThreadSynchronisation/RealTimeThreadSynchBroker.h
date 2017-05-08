/**
 * @file RealTimeThreadSynchBroker.h
 * @brief Header file for class RealTimeThreadSynchBroker
 * @date 24/04/2017
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class RealTimeThreadSynchBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMETHREADSYNCHBROKER_H_
#define REALTIMETHREADSYNCHBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EventSem.h"
#include "MemoryMapInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * TODO
 */
class RealTimeThreadSynchBroker : public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * TODO
     */
    RealTimeThreadSynchBroker();

    /**
     * TODO
     */
    virtual ~RealTimeThreadSynchBroker();

    /**
     * TODO
     */
    void SetFunctionIndex(DataSourceI *dataSourceIn, uint32 functionIdxIn);

    /**
     * TODO
     */
    bool AllocateMemory(char8 *dataSourceMemoryIn, uint32 *dataSourceMemoryOffsetsIn);

    /**
     * TODO
     */
    bool GetSignalMemoryBuffer(const uint32 signalIdx, void *&signalAddress);

    /**
     * TODO
     */
    bool AddSample();

    /**
     * TODO
     */
    bool Wait();

    /**
     * TODO
     */
    const char8 *const GetGAMName();

    /**
     * TODO
     */
    virtual bool Execute();

private:
    /**
     * TODO
     */
    uint32 numberOfDataSourceSignals;

    /**
     * TODO
     */
    char8 **signalMemory;

    /**
     * TODO
     */
    uint32 *signalSize;

    /**
     * TODO
     */
    char8 *dataSourceMemory;

    /**
     * TODO
     */
    uint32 *dataSourceMemoryOffsets;

    /**
     * TODO
     */
    DataSourceI *dataSource;

    /**
     * TODO
     */
    uint32 functionIdx;

    /**
     * TODO
     */
    uint32 numberOfSamples;

    /**
     * TODO
     */
    uint32 currentSample;

    /**
     * TODO
     */
    EventSem synchSem;

    /**
     * TODO
     */
    StreamString gamName;

};
}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMETHREADSYNCHBROKER_H_ */
	
