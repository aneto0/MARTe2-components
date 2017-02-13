/**
 * @file MDSWriterNode.cpp
 * @brief Source file for class MDSWriterNode
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

 * @details This source file contains the definition of all the methods for
 * the class MDSWriterNode (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MDSWriterNode.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
MDSWriterNode::MDSWriterNode() {
    nodeName = "";
    nodeType = 0u;
    numberOfSamples = 0u;
    period = 0.f;
    phaseShift = 0u;
    node = NULL_PTR(MDSplus::TreeNode *);

    nOfWriteCalls = 0U;

    decimatedMinMax = false;
    decimatedNodeName = "";
    decimatedNode = NULL_PTR(MDSplus::TreeNode *);
    numberOfWords = 0u;

    bufferedData = NULL_PTR(char8 *);
    currentBuffer = 0u;
    makeSegmentAfterNWrites = 0u;
    minMaxResampleFactor = 0u;

    signalMemory = NULL_PTR(void *);
    timeSignalMemory = NULL_PTR(uint32 *);
    lastWriteTimeSignal = 0u;
    periodMicroSecond = 0u;
    useTimeVector = false;
}

MDSWriterNode::~MDSWriterNode() {
    if (node != NULL) {
        //TODO check if the node should be deleted, or if this is done by the tree...
        //delete node;
    }
    if (decimatedNode != NULL) {
        //TODO check if the node should be deleted, or if this is done by the tree...
        //delete node;
    }
    if (bufferedData != NULL) {
        free((void *&) bufferedData);
    }
}

bool MDSWriterNode::Initialise(StructuredDataI & data) {
    bool ok = data.Read("NodeName", nodeName);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "NodeName shall be specified");
    }
    if (ok) {
        if (data.Read("DecimatedNodeName", decimatedNodeName)) {
            decimatedMinMax = (decimatedNodeName.Size() > 0);
            ok = (data.Read("MinMaxResampleFactor", minMaxResampleFactor));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "MinMaxResampleFactor shall be specified");
            }
        }
    }
    StreamString typeOfData;
    if (ok) {
        ok = (data.Read("NodeType", typeOfData));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NodeType shall be specified");
        }
        if (typeOfData == "int32") {
            nodeType = DTYPE_L;
        }
        else if (typeOfData == "int16") {
            nodeType = DTYPE_W;
        }
        else if (typeOfData == "int64") {
            nodeType = DTYPE_Q;
        }
        else if (typeOfData == "float") {
            nodeType = DTYPE_FLOAT;
        }
        else if (typeOfData == "double") {
            nodeType = DTYPE_DOUBLE;
        }
        else {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "NodeType %s not supported for node with name %s", typeOfData.Buffer(), nodeName.Buffer())
            ok = false;
        }
    }
    if (ok) {
        ok = (data.Read("NumberOfElements", numberOfSamples));

        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfElements shall be specified");
        }
    }
    if (ok) {
        ok = (numberOfSamples > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfElements shall be > 0");
        }
    }
    if (ok) {
        ok = (data.Read("Period", period));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Period shall be specified");
        }
    }
    if (ok) {
        ok = (period > 0.);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Period shall be > 0");
        }
    }
    if (ok) {
        ok = (data.Read("SamplePhase", phaseShift));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "SamplePhase not specified. Using 0");
            phaseShift = 0u;
            ok = true;
        }
    }
    if (ok) {
        ok = (data.Read("MakeSegmentAfterNWrites", makeSegmentAfterNWrites));

        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "MakeSegmentAfterNWrites shall be specified");
        }
    }
    if (ok) {
        ok = (numberOfSamples > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "MakeSegmentAfterNWrites shall be > 0");
        }
    }
    if (ok) {
        uint32 typeMultiplier = 0u;

        if (nodeType == DTYPE_L) {
            typeMultiplier = 1;
        }
        else if (nodeType == DTYPE_W) {
            typeMultiplier = 1;
        }
        else if (nodeType == DTYPE_Q) {
            typeMultiplier = sizeof(uint64) / sizeof(uint32);
        }
        else if (nodeType == DTYPE_FLOAT) {
            typeMultiplier = sizeof(float) / sizeof(uint32);
        }
        else if (nodeType == DTYPE_DOUBLE) {
            typeMultiplier = sizeof(double) / sizeof(uint32);
        }
        numberOfWords = numberOfSamples * typeMultiplier;
        bufferedData = reinterpret_cast<char *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(
                makeSegmentAfterNWrites * numberOfWords * sizeof(int32)));

        //Note that we have to multiply by two because the minimum size is one word (sizeof(int32))
        if (nodeType == DTYPE_W) {
            numberOfSamples *= 2;
        }
        numberOfSamples *= makeSegmentAfterNWrites;
        periodMicroSecond = period * 1e6;
    }
    return true;
}

bool MDSWriterNode::AllocateTreeNode(MDSplus::Tree *tree) {
    bool ok = true;
    try {
        node = tree->getNode(nodeName.Buffer());
        node->deleteData();
        if (decimatedMinMax) {
            decimatedNode = tree->getNode(decimatedNodeName.Buffer());
            decimatedNode->deleteData();
        }
    }
    catch (MDSplus::MdsException &exc) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Failed opening node with name %s", nodeName.Buffer())
        ok = false;
    }
    return ok;
}

bool MDSWriterNode::Execute() {
    bool ok = true;
    if (currentBuffer < makeSegmentAfterNWrites) {
        ok = MemoryOperationsHelper::Copy(&bufferedData[currentBuffer * numberOfWords * sizeof(int32)], signalMemory, numberOfWords * sizeof(int32));
        currentBuffer++;
    }

    //If the number of writes is sufficient to create a segment do it.
    bool storeNow = (currentBuffer == (makeSegmentAfterNWrites));
    if (useTimeVector) {
        //If we are acquiring data based on events (which do not necessarily occur sequentially in time, trigger every time there is a change in the time vector)
        if ((*timeSignalMemory - lastWriteTimeSignal) != periodMicroSecond) {
            if (nOfWriteCalls > 0u) {
                storeNow = true;
            }
        }
        lastWriteTimeSignal = *timeSignalMemory;
    }

    //Sufficient data to make a segment
    if (storeNow) {
        currentBuffer = 0;
        double start = 0.;
        if (useTimeVector) {
            start = *timeSignalMemory;
        }
        else {
            start = nOfWriteCalls * numberOfSamples * period;
            start += phaseShift * period;
        }

        double end = start + ((numberOfSamples - 1) * period);
        MDSplus::Data *startD = new MDSplus::Float64(start);
        MDSplus::Data *endD = new MDSplus::Float64(end);
        MDSplus::Data *dimension = new MDSplus::Range(startD, endD, new MDSplus::Float64(period));
        MDSplus::Array *array = NULL;

        if (nodeType == DTYPE_W) {
            array = new MDSplus::Int16Array((int16 *) bufferedData, numberOfSamples);
        }
        else if (nodeType == DTYPE_L) {
            array = new MDSplus::Int32Array(((int32 *) bufferedData), numberOfSamples);
        }
        else if (nodeType == DTYPE_Q) {
            array = new MDSplus::Int64Array((int64_t *) bufferedData, numberOfSamples);
        }
        else if (nodeType == DTYPE_FLOAT) {
            array = new MDSplus::Float32Array((float *) bufferedData, numberOfSamples);
        }
        else if (nodeType == DTYPE_DOUBLE) {
            array = new MDSplus::Float64Array((double *) bufferedData, numberOfSamples);
        }
        if (array != NULL) {
            if (decimatedMinMax) {
                node->makeSegmentMinMax(startD, endD, dimension, array, decimatedNode, minMaxResampleFactor);
            }
            else {
                node->makeSegment(startD, endD, dimension, array);
            }
            MDSplus::deleteData(array);
        }
        MDSplus::deleteData(dimension);
        nOfWriteCalls++;
    }
    return ok;
}

void MDSWriterNode::SetSignalMemory(void *signalMemoryIn) {
    signalMemory = signalMemoryIn;
}

void MDSWriterNode::SetTimeSignalMemory(void *timeSignalMemoryIn) {
    timeSignalMemory = reinterpret_cast<uint32 *>(timeSignalMemoryIn);
    useTimeVector = (timeSignalMemory != NULL_PTR(uint32 *));

}

}

