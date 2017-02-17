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
    numberOfElements = 0u;
    period = 0.f;
    phaseShift = 0u;
    node = NULL_PTR(MDSplus::TreeNode *);

    nOfExecuteCalls = 0U;

    decimatedMinMax = false;
    decimatedNodeName = "";
    decimatedNode = NULL_PTR(MDSplus::TreeNode *);
    typeMultiplier = 0u;

    bufferedData = NULL_PTR(char8 *);
    currentBuffer = 0u;
    makeSegmentAfterNWrites = 0u;
    minMaxResampleFactor = 0u;

    signalMemory = NULL_PTR(void *);
    timeSignalMemory = NULL_PTR(uint32 *);
    lastWriteTimeSignal = 0u;
    executePeriodMicroSecond = 0u;
    useTimeVector = false;

    start = 0.F;
    flush = false;
}

MDSWriterNode::~MDSWriterNode() {
    if (node != NULL_PTR(MDSplus::TreeNode *)) {
        //TODO check if the node should be deleted, or if this is done by the tree...
        delete node;
    }
    if (decimatedNode != NULL_PTR(MDSplus::TreeNode *)) {
        //TODO check if the node should be deleted, or if this is done by the tree...
        delete decimatedNode;
    }
    if (bufferedData != NULL_PTR(char8 *)) {
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
            decimatedMinMax = true;
            ok = (data.Read("MinMaxResampleFactor", minMaxResampleFactor));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "MinMaxResampleFactor shall be specified");
            }
            if (ok) {
                ok = (minMaxResampleFactor > 0u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "MinMaxResampleFactor shall be > 0");
                }
            }
        }
    }
    TypeDescriptor signalTypeDescriptor;
    StreamString signalType;
    if (ok) {
        ok = data.Read("Type", signalType);
    }
    if (ok) {
        signalTypeDescriptor = TypeDescriptor::GetTypeDescriptorFromTypeName(signalType.Buffer());
    }
    if (ok) {
        if (signalTypeDescriptor == SignedInteger16Bit) {
            nodeType = DTYPE_W;
        }
        else if (signalTypeDescriptor == SignedInteger32Bit) {
            nodeType = DTYPE_L;
        }
        else if (signalTypeDescriptor == SignedInteger64Bit) {
            nodeType = DTYPE_Q;
        }
        else if (signalTypeDescriptor == UnsignedInteger16Bit) {
            nodeType = DTYPE_WU;
        }
        else if (signalTypeDescriptor == UnsignedInteger32Bit) {
            nodeType = DTYPE_LU;
        }
        else if (signalTypeDescriptor == UnsignedInteger64Bit) {
            nodeType = DTYPE_QU;
        }
        else if (signalTypeDescriptor == Float32Bit) {
            nodeType = DTYPE_FLOAT;
        }
        else if (signalTypeDescriptor == Float64Bit) {
            nodeType = DTYPE_DOUBLE;
        }
        else {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "NodeType %s not supported for node with name %s", signalType.Buffer(), nodeName.Buffer())
            ok = false;
        }
    }
    if (ok) {
        ok = (data.Read("NumberOfElements", numberOfElements));

        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfElements shall be specified");
        }
    }
    if (ok) {
        ok = (numberOfElements > 0u);
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
        ok = (makeSegmentAfterNWrites > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "MakeSegmentAfterNWrites shall be > 0");
        }
    }
    if (ok) {
        if ((nodeType == DTYPE_W) || (nodeType == DTYPE_WU)) {
            typeMultiplier = sizeof(uint16);
        }
        else if ((nodeType == DTYPE_L) || (nodeType == DTYPE_LU)) {
            typeMultiplier = sizeof(uint32);
        }
        else if ((nodeType == DTYPE_Q) || (nodeType == DTYPE_QU)) {
            typeMultiplier = sizeof(uint64);
        }
        else if (nodeType == DTYPE_FLOAT) {
            typeMultiplier = sizeof(float32);
        }
        else if (nodeType == DTYPE_DOUBLE) {
            typeMultiplier = sizeof(float64);
        }

        uint32 bufferedDataSize = typeMultiplier;
        bufferedDataSize *= numberOfElements * makeSegmentAfterNWrites;

        bufferedData = reinterpret_cast<char *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(bufferedDataSize));

        executePeriodMicroSecond = numberOfElements * (period * 1e6);
    }
    return ok;
}

bool MDSWriterNode::AllocateTreeNode(MDSplus::Tree *tree) {
    bool ok = true;
    try {
        if (node != NULL_PTR(MDSplus::TreeNode *)) {
            delete node;
        }
        if (decimatedNode != NULL_PTR(MDSplus::TreeNode *)) {
            delete decimatedNode;
        }

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
    start = phaseShift * period;
    return ok;
}

bool MDSWriterNode::Flush() {
    flush = true;
    return Execute();
}

bool MDSWriterNode::Execute() {
    bool ok = (node != NULL_PTR(MDSplus::TreeNode *));

    if ((ok) && (!flush)) {
        //If we are using a triggering source get the signal from a time source, as samples might not be continuous
        if (useTimeVector) {
            if (currentBuffer == 0u) {
                if (timeSignalMemory != NULL_PTR(uint32 *)) {
                    start = (*timeSignalMemory) * 1e-6;
                }
            }
        }
        if (currentBuffer < makeSegmentAfterNWrites) {
            if ((signalMemory != NULL_PTR(uint32 *)) && (bufferedData != NULL_PTR(char8 *))) {
                ok = MemoryOperationsHelper::Copy(&bufferedData[currentBuffer * numberOfElements * typeMultiplier], signalMemory,
                                                  numberOfElements * typeMultiplier);
            }
            else {
                ok = false;
            }
            if (ok) {
                currentBuffer++;
            }
        }
    }

    //If the number of writes is sufficient to create a segment do it.
    bool storeNow = (currentBuffer == (makeSegmentAfterNWrites));
    if (ok) {
        if (useTimeVector) {
            //If we are acquiring data based on events (which do not necessarily occur sequentially in time, trigger every time there is a change in the time vector)
            if ((*timeSignalMemory - lastWriteTimeSignal) != executePeriodMicroSecond) {
                if (nOfExecuteCalls > 0u) {
                    storeNow = true;
                }
            }
            lastWriteTimeSignal = *timeSignalMemory;
        }
        //If the data has to be flushed for the storeNow
        if (flush) {
            storeNow = (currentBuffer > 0u);
            flush = false;
        }
    }
    //Sufficient data to make a segment
    if ((ok) && (storeNow)) {
        uint32 numberOfElementsPerSegment = numberOfElements * currentBuffer;
        currentBuffer = 0;

        double end = start + ((numberOfElementsPerSegment - 1) * period);
        MDSplus::Data *startD = new MDSplus::Float64(start);
        MDSplus::Data *endD = new MDSplus::Float64(end);
        MDSplus::Data *dimension = new MDSplus::Range(startD, endD, new MDSplus::Float64(period));
        MDSplus::Array *array = NULL_PTR(MDSplus::Array *);

        if (!useTimeVector) {
            start += numberOfElementsPerSegment * period;
        }

        if (nodeType == DTYPE_W) {
            array = new MDSplus::Int16Array((int16 *) bufferedData, numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_WU) {
            array = new MDSplus::Uint16Array((uint16 *) bufferedData, numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_L) {
            array = new MDSplus::Int32Array(((int32 *) bufferedData), numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_LU) {
            array = new MDSplus::Uint32Array(((uint32 *) bufferedData), numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_Q) {
            array = new MDSplus::Int64Array((int64_t *) bufferedData, numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_QU) {
            array = new MDSplus::Uint64Array((uint64_t *) bufferedData, numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_FLOAT) {
            array = new MDSplus::Float32Array((float *) bufferedData, numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_DOUBLE) {
            array = new MDSplus::Float64Array((double *) bufferedData, numberOfElementsPerSegment);
        }
        if (array != NULL_PTR(MDSplus::Array *)) {
            if (decimatedMinMax) {
                node->makeSegmentMinMax(startD, endD, dimension, array, decimatedNode, minMaxResampleFactor);
            }
            else {
                node->makeSegment(startD, endD, dimension, array);
            }
            MDSplus::deleteData(array);
        }
        MDSplus::deleteData(dimension);
        nOfExecuteCalls++;
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

uint64 MDSWriterNode::GetNumberOfExecuteCalls() const {
    return nOfExecuteCalls;
}

bool MDSWriterNode::IsDecimatedMinMax() const {
    return decimatedMinMax;
}

const StreamString& MDSWriterNode::GetDecimatedNodeName() const {
    return decimatedNodeName;
}

uint32 MDSWriterNode::GetExecutePeriodMicroSecond() const {
    return executePeriodMicroSecond;
}

uint32 MDSWriterNode::GetMakeSegmentAfterNWrites() const {
    return makeSegmentAfterNWrites;
}

uint32 MDSWriterNode::GetMinMaxResampleFactor() const {
    return minMaxResampleFactor;
}

const StreamString& MDSWriterNode::GetNodeName() const {
    return nodeName;
}

uint32 MDSWriterNode::GetNodeType() const {
    return nodeType;
}

uint32 MDSWriterNode::GetNumberOfElements() const {
    return numberOfElements;
}

float64 MDSWriterNode::GetPeriod() const {
    return period;
}

int32 MDSWriterNode::GetPhaseShift() const {
    return phaseShift;
}

float64 MDSWriterNode::GetStart() const {
    return start;
}

uint32 MDSWriterNode::GetTypeMultiplier() const {
    return typeMultiplier;
}

bool MDSWriterNode::IsUseTimeVector() const {
    return useTimeVector;
}
}
