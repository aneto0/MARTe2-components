/**
 * @file RealTimeThreadSynchBuffer.h
 * @brief Header file for class RealTimeThreadSynchBuffer
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

 * @details This header file contains the declaration of the class RealTimeThreadSynchBuffer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMETHREADSYNCHBUFFER_H_
#define REALTIMETHREADSYNCHBUFFER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EventSem.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * TODO
 */
class RealTimeThreadSynchBuffer {
public:
    /**
     * TODO
     */
    RealTimeThreadSynchBuffer(DataSourceI *dataSourceIn, uint32 functionIdxIn);

    /**
     * TODO
     */
    ~RealTimeThreadSynchBuffer();

    /**
     * TODO
     */
    bool AllocateMemory(uint32 sampleMemorySizeIn, uint32 *memoryOffSetsIn, void *dataSourceMemoryIn);

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

private:
    /**
     * TODO
     */
    char8 *memory;

    /**
     * TODO
     */
    void *dataSourceMemory;

    /**
     * TODO
     */
    uint32 *memoryOffsets;

    /**
     * TODO
     */
    uint32 sampleMemorySize;

    /**
     * TODO
     */
    uint32 numberOfSignals;

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

#endif /* REALTIMETHREADSYNCHBUFFER_H_ */
	
