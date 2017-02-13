/**
 * @file MDSWriterNode.h
 * @brief Header file for class MDSWriterNode
 * @date 10/02/2017
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

 * @details This header file contains the declaration of the class MDSWriterNode
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSWRITER_MDSWRITERNODE_H_
#define MDSWRITER_MDSWRITERNODE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "mdsobjects.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"
#include "StructuredDataI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief TODO
 */
namespace MARTe {
class MDSWriterNode {
public:
    MDSWriterNode();
    ~MDSWriterNode();

    /**
     * TODO
     * @brief Loads and verifies the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    bool Initialise(StructuredDataI & data);

    /**
     * TODO
     */
    bool Execute();

    /**
     * TODO
     */
    void SetSignalMemory(void *signalMemoryIn);

    /**
     * TODO
     */
    void SetTimeSignalMemory(void *timeSignalMemoryIn);

    /**
     * TODO
     */
    bool AllocateTreeNode(MDSplus::Tree *tree);

private:
    /**
     * The name of the MDSplus node
     */
    StreamString nodeName;

    /**
     * The node type (as defined in mdsobjects.h)
     */
    uint32 nodeType;

    /**
     * Number of samples to be stored on each write operation
     */
    uint32 numberOfSamples;

    /**
     * Period between samples
     */
    double period;

    /**
     * Period in micro-seconds
     */
    uint32 periodMicroSecond;

    /**
     * TODO
     */
    bool useTimeVector;

    /**
     * Phase shift (in samples) of the first sample
     */
    int32 phaseShift;

    /**
     * The MDSplus tree node where data is stored
     */
    MDSplus::TreeNode *node;

    /**
     * Number of times Write was called
     */
    uint64 nOfWriteCalls;

    /**
     * True if a decimated signal is also to be stored using makeSegmentMinMax
     */
    bool decimatedMinMax;

    /**
     * The decimated node for makeSegmentMinMax
     */
    StreamString decimatedNodeName;
    MDSplus::TreeNode *decimatedNode;

    /**
     * the number of Words allocated by this node (i.e. numberOfSamples * sizeof(nodeType) / sizeof(int32))
     */
    uint32 numberOfWords;

    /**
     * Data is stored in this buffer before triggering a makeSegment/makeSegmentMinMax
     */
    char *bufferedData;
    uint32 currentBuffer;
    uint32 makeSegmentAfterNWrites;
    uint32 minMaxResampleFactor;

    /**
     * TODO
     */
    void *signalMemory;

    /**
     *
     */
    uint32 *timeSignalMemory;
    uint32 lastWriteTimeSignal;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSWRITER_MDSWRITERNODE_H_ */
