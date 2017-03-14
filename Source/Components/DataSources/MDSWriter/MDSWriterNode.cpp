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
/*lint -estring(1960, "*MDSplus::*") -estring(1960, "*std::*") Ignore errors that do not belong to this DataSource namespace*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
MDSWriterNode::MDSWriterNode() {
    nodeName = "";
    nodeType = 0;
    numberOfElements = 0u;
    period = 0.F;
    phaseShift = 0;
    node = NULL_PTR(MDSplus::TreeNode *);

    nOfExecuteCalls = 0U;

    decimatedMinMax = false;
    decimatedNodeName = "";
    decimatedNode = NULL_PTR(MDSplus::TreeNode *);
    typeMultiplier = 0u;

    bufferedData = NULL_PTR(char8 *);
    currentBuffer = 0u;
    makeSegmentAfterNWrites = 0u;
    minMaxResampleFactor = 0;

    signalMemory = NULL_PTR(void *);
    timeSignalMemory = NULL_PTR(uint32 *);
    lastWriteTimeSignal = 0u;
    executePeriodMicroSecond = 0u;
    useTimeVector = false;

    start = 0.F;
    flush = false;
}

/*lint -e{1551} -e{1740} the destructor must guarantee that the MDSplus TreeNode is deleted and the shared memory freed. The signalMemory and the timeSignalMemory are freed by the framework */
MDSWriterNode::~MDSWriterNode() {
    if (node != NULL_PTR(MDSplus::TreeNode *)) {
        //TODO check if the node should be deleted, or if this is done by the tree...
        delete node;
    }
    if (decimatedNode != NULL_PTR(MDSplus::TreeNode *)) {
        //TODO check if the node should be deleted, or if this is done by the tree...
        delete decimatedNode;
    }
    if (bufferedData != NULL_PTR(void *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(bufferedData));
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
                ok = (minMaxResampleFactor > 0);
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
            phaseShift = 0;
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
        else {
            //A wrong type is already trapped before...
        }

        uint32 bufferedDataSize = static_cast<uint32>(typeMultiplier);
        bufferedDataSize *= numberOfElements * makeSegmentAfterNWrites;

        bufferedData = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(bufferedDataSize));

        float64 executePeriodMicroSecondF = static_cast<float64>(numberOfElements) * period * 1e6;
        executePeriodMicroSecondF += 0.5F;
        executePeriodMicroSecond = static_cast<uint32>(executePeriodMicroSecondF);

    }
    return ok;
}

bool MDSWriterNode::AllocateTreeNode(MDSplus::Tree * const tree) {
    bool ok = (tree != NULL_PTR(MDSplus::Tree *));
    try {
        if (node != NULL_PTR(MDSplus::TreeNode *)) {
            delete node;
        }
        if (decimatedNode != NULL_PTR(MDSplus::TreeNode *)) {
            delete decimatedNode;
        }

        /*lint -e{613} tree cannot be NULL as otherwise ok = false*/
        if (ok) {
            node = tree->getNode(nodeName.Buffer());
            node->deleteData();
            if (decimatedMinMax) {
                decimatedNode = tree->getNode(decimatedNodeName.Buffer());
                decimatedNode->deleteData();
            }
        }
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Failed opening node with name %s: %s", nodeName.Buffer(), exc.what())
        ok = false;
    }
    start = static_cast<float64>(phaseShift) * period;
    return ok;
}


bool MDSWriterNode::Flush() {
    flush = true;
    return Execute();
}

bool MDSWriterNode::Execute() {
    bool ok = (node != NULL_PTR(MDSplus::TreeNode *));

    //discontinuityFound only meaningful when useTimeVector = true and tracks event that might not be continuous but that would
    //otherwise fit inside the same segment.
    bool discontinuityFound = false;
    if ((ok) && (!flush)) {
        //If we are using a triggering source get the signal from a time source, as samples might not be continuous
        if (useTimeVector) {
            if (currentBuffer == 0u) {
                if (timeSignalMemory != NULL_PTR(uint32 *)) {
                    start = static_cast<float64>(*timeSignalMemory) * 1e-6;
                    start += static_cast<float64>(phaseShift) * period;
                }
            }
            //If we are acquiring data based on events (which do not necessarily occur sequentially in time, trigger every time there is a change in the time vector)
            //Only makes sense to check for this if segments are not written @ every Execute
            else {
                if (makeSegmentAfterNWrites > 1u) {
                    if (timeSignalMemory != NULL_PTR(uint32 *)) {
                        if ((*timeSignalMemory - lastWriteTimeSignal) != executePeriodMicroSecond) {
                            if (lastWriteTimeSignal > 0u) {
                                discontinuityFound = true;
                            }
                        }
                    }
                }
            }
            if (timeSignalMemory != NULL_PTR(uint32 *)) {
                lastWriteTimeSignal = *timeSignalMemory;
            }
        }
        //If data is continuous store in the shared buffer that will be flushed as part of the next segment.
        //If data is not continuous this will trigger the creation of a new segment and this signal data will be added to the beginning of the next segment.
        if (!discontinuityFound) {
            if (currentBuffer < makeSegmentAfterNWrites) {
                if ((signalMemory != NULL_PTR(uint32 *)) && (bufferedData != NULL_PTR(void *))) {
                    uint32 signalIdx = currentBuffer * numberOfElements * static_cast<uint32>(typeMultiplier);
                    char8 *bufferedDataC = reinterpret_cast<char8 *>(bufferedData);
                    ok = MemoryOperationsHelper::Copy(&bufferedDataC[signalIdx], signalMemory, numberOfElements * static_cast<uint32>(typeMultiplier));
                }
                else {
                    ok = false;
                }
                if (ok) {
                    currentBuffer++;
                }
            }
        }
    }

    //If a discontinuity was found trigger the storage of data.
    bool storeNow = discontinuityFound;
    if (!storeNow) {
        //If the number of writes is sufficient to create a segment do it.
        storeNow = (currentBuffer == (makeSegmentAfterNWrites));
    }
    if (ok) {
        //If the data has to be flushed for the storeNow
        if (flush) {
            storeNow = (currentBuffer > 0u);
            flush = false;
        }
    }
    //Sufficient data to make a segment
    if ((ok) && (storeNow)) {
        //Notice that currentBuffer is not incremented if a discontinuity is found
        int32 numberOfElementsPerSegment = static_cast<int32>(numberOfElements) * static_cast<int32>(currentBuffer);

        int32 numberOfElementsPerSegmentM1 = numberOfElementsPerSegment - 1;
        float64 numberOfElementsPerSegmentF = static_cast<float64>(numberOfElementsPerSegmentM1);
        float64 end = start + (numberOfElementsPerSegmentF * period);
        //lint -e{429} freed by MDSplus upon deletion of dimension
        MDSplus::Data *startD = new MDSplus::Float64(start);
        //lint -e{429} freed by MDSplus upon deletion of dimension
        MDSplus::Data *endD = new MDSplus::Float64(end);
        //lint -e{429} freed by MDSplus upon deletion of dimension
        MDSplus::Data *dimension = new MDSplus::Range(startD, endD, new MDSplus::Float64(period));
        //lint -e{429} freed by MDSplus upon deletion of array
        MDSplus::Array *array = NULL_PTR(MDSplus::Array *);

        if (!useTimeVector) {
            start += static_cast<float64>(numberOfElementsPerSegment) * period;
        }
        if (nodeType == DTYPE_W) {
            array = new MDSplus::Int16Array(reinterpret_cast<int16 *>(bufferedData), numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_WU) {
            array = new MDSplus::Uint16Array(reinterpret_cast<uint16 *>(bufferedData), numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_L) {
            array = new MDSplus::Int32Array(reinterpret_cast<int32 *>(bufferedData), numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_LU) {
            array = new MDSplus::Uint32Array(reinterpret_cast<uint32 *>(bufferedData), numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_Q) {
            array = new MDSplus::Int64Array(reinterpret_cast<int64_t *>(bufferedData), numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_QU) {
            array = new MDSplus::Uint64Array(reinterpret_cast<uint64_t *>(bufferedData), numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_FLOAT) {
            array = new MDSplus::Float32Array(reinterpret_cast<float32*>(bufferedData), numberOfElementsPerSegment);
        }
        else if (nodeType == DTYPE_DOUBLE) {
            array = new MDSplus::Float64Array(reinterpret_cast<float64*>(bufferedData), numberOfElementsPerSegment);
        }
        else {
            //An invalid nodeType is trapped before.
        }
        if (array != NULL_PTR(MDSplus::Array *)) {
            if (decimatedMinMax) {
                //lint -e{613} node is checked not to be null in the beginning of the function
                node->makeSegmentMinMax(startD, endD, dimension, array, decimatedNode, minMaxResampleFactor);
            }
            else {
                //lint -e{613} node is checked not to be null in the beginning of the function
                node->makeSegment(startD, endD, dimension, array);
            }
            MDSplus::deleteData(array);
        }
        MDSplus::deleteData(dimension);
        nOfExecuteCalls++;

        //discontuityFound will only be triggered if makeSegmentAfterNWrites > 1, so Execute will be called again later
        //discontuityFound will only be triggered if signalMemory != NULL_PTR (i.e. that we are triggering based on event).
        if (discontinuityFound) {
            if ((signalMemory != NULL_PTR(uint32 *)) && (bufferedData != NULL_PTR(void *))) {
                //currentBuffer had already been incremented. Copy the last buffer to the beginning
                char8 *bufferedDataC = reinterpret_cast<char8 *>(bufferedData);
                ok = MemoryOperationsHelper::Copy(&bufferedDataC[0u], signalMemory, numberOfElements * static_cast<uint32>(typeMultiplier));
            }
            if (ok) {
                currentBuffer = 1u;
            }
            if (timeSignalMemory != NULL_PTR(uint32 *)) {
                //Update the start
                start = static_cast<float64>(*timeSignalMemory) * 1e-6;
                start += static_cast<float64>(phaseShift) * period;
            }
        }
        else {
            currentBuffer = 0u;
        }
        //lint -e{429} startD, endD, dimension are freed by MDSplus upon deletion of dimension
    }
    return ok;
}

void MDSWriterNode::SetSignalMemory(void * const signalMemoryIn) {
    signalMemory = signalMemoryIn;
}

void MDSWriterNode::SetTimeSignalMemory(void * const timeSignalMemoryIn) {
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

int32 MDSWriterNode::GetMinMaxResampleFactor() const {
    return minMaxResampleFactor;
}

const StreamString& MDSWriterNode::GetNodeName() const {
    return nodeName;
}

int32 MDSWriterNode::GetNodeType() const {
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
    return static_cast<uint32>(typeMultiplier);
}

bool MDSWriterNode::IsUseTimeVector() const {
    return useTimeVector;
}
}
