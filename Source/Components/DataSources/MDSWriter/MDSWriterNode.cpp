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
    numberOfSamples = 0u;
    numberOfDimensions = 0u;
    period = 0.F;
    phaseShift = 0;
    node = NULL_PTR(MDSplus::TreeNode*);

    nOfExecuteCalls = 0U;

    decimatedMinMax = false;
    decimatedNodeName = "";
    decimatedNode = NULL_PTR(MDSplus::TreeNode*);
    typeMultiplier = 0u;

    bufferedData = NULL_PTR(char8*);
    currentBuffer = 0u;
    makeSegmentAfterNWrites = 0u;
    minMaxResampleFactor = 0;

    signalMemory = NULL_PTR(void*);
    timeSignalMemory = NULL_PTR(void*);
    timeSignalType = UnsignedInteger32Bit;
    timeSignalMultiplier = 1e-6;
    lastWriteTimeSignal = 0u;
    executePeriod = 0u;
    useTimeVector = false;
    automaticSegmentation = false;

    start = 0.F;
    flush = false;

    segmentDim[0] = 0;
    segmentDim[1] = 0;
    segmentDim[2] = 0;
    discontinuityFactor = 0.;
}

/*lint -e{1551} -e{1740} the destructor must guarantee that the MDSplus TreeNode is deleted and the shared memory freed. The signalMemory and the timeSignalMemory are freed by the framework */
MDSWriterNode::~MDSWriterNode() {
    if (node != NULL_PTR(MDSplus::TreeNode*)) {
        //TODO check if the node should be deleted, or if this is done by the tree...
        delete node;
    }
    if (decimatedNode != NULL_PTR(MDSplus::TreeNode*)) {
        //TODO check if the node should be deleted, or if this is done by the tree...
        delete decimatedNode;
    }
    if (bufferedData != NULL_PTR(void*)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void*&>(bufferedData));
    }
}

bool MDSWriterNode::Initialise(StructuredDataI &data) {
    bool ok = data.Read("NodeName", nodeName);
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "NodeName shall be specified");
    }
    if (ok) { //AutomaticSegmentation
        uint8 auxBool;
        ok = (data.Read("AutomaticSegmentation", auxBool));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "AutomaticSegmentation must be specified");
        }
        if (ok) {
            if (auxBool == 1u) {
                automaticSegmentation = true;
            }
            else if (auxBool == 0u) {
                automaticSegmentation = false;
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "AutomaticSegmentation must be 0 (false) or 1 (true)");
                ok = false;
            }
        }
    }
    if (ok) {
        if (!automaticSegmentation) {
            if (data.Read("DecimatedNodeName", decimatedNodeName)) {
                decimatedMinMax = true;
                ok = (data.Read("MinMaxResampleFactor", minMaxResampleFactor));
                if (!ok) {
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "MinMaxResampleFactor shall be specified");
                }
                if (ok) {
                    ok = (minMaxResampleFactor > 0);
                    if (!ok) {
                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "MinMaxResampleFactor shall be > 0");
                    }
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
        if (!data.Read("DiscontinuityFactor", discontinuityFactor)) {
            discontinuityFactor = 0.;
        }

        signalTypeDescriptor = TypeDescriptor::GetTypeDescriptorFromTypeName(signalType.Buffer());
    }
    if (ok) {
        if (signalTypeDescriptor == SignedInteger8Bit) {
            nodeType = DTYPE_B;
        }
        else if (signalTypeDescriptor == SignedInteger16Bit) {
            nodeType = DTYPE_W;
        }
        else if (signalTypeDescriptor == SignedInteger32Bit) {
            nodeType = DTYPE_L;
        }
        else if (signalTypeDescriptor == SignedInteger64Bit) {
            nodeType = DTYPE_Q;
        }
        else if (signalTypeDescriptor == UnsignedInteger8Bit) {
            nodeType = DTYPE_BU;
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
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "NodeType %s not supported for node with name %s", signalType.Buffer(), nodeName.Buffer());
            ok = false;
        }
    }
    if (ok) {
        ok = (data.Read("NumberOfElements", numberOfElements));

        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "NumberOfElements shall be specified");
        }
    }
    if (ok) {
        ok = (numberOfElements > 0u);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "NumberOfElements shall be > 0");
        }
    }
    if (ok) {
        ok = (data.Read("Samples", numberOfSamples));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "NumberOfSamples shall be specified");
        }
    }
    if (ok) {
        ok = (numberOfSamples > 0u);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "NumberOfSamples shall be positive");
        }
    }
    if (ok) {
        ok = (data.Read("NumberOfDimensions", numberOfDimensions));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "NumberOfDimensions shall be specified");
        }
        if (ok) {
            ok = ((numberOfDimensions == 0u) || (numberOfDimensions == 1u));
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "NumberOfDimensions shall be > 0");
            }
        }
    }
    if (ok) {
        ok = (data.Read("Period", period));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Period shall be specified");
        }
    }
    if (ok) {
        ok = (period > 0.);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Period shall be > 0");
        }
    }
    if (ok) {
        ok = (data.Read("SamplePhase", phaseShift));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "SamplePhase not specified. Using 0");
            phaseShift = 0;
            ok = true;
        }
    }
    if (ok) {
        if (!automaticSegmentation) {
            ok = (data.Read("MakeSegmentAfterNWrites", makeSegmentAfterNWrites));

            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "MakeSegmentAfterNWrites shall be specified");
            }
        }
        else {
            makeSegmentAfterNWrites = 1u;
        }
    }
    if (ok) {
        uint32 segmentDimU = numberOfSamples * makeSegmentAfterNWrites;
        segmentDim[0] = static_cast<int32>(segmentDimU);
        if (numberOfDimensions == 0u) {
            segmentDim[1] = 1;
            segmentDim[2] = 1;
        }
        else if (numberOfDimensions == 1u) {
            segmentDim[1] = static_cast<int32>(numberOfElements);
            segmentDim[2] = 1;
        }
        else {
            //numberOfDimensions == 2u. TODO currently matrix are not supported.
        }
    }
    if (ok) {
        ok = (makeSegmentAfterNWrites > 0u);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "MakeSegmentAfterNWrites shall be > 0");
        }
    }
    if (ok) {
        if ((nodeType == DTYPE_B) || (nodeType == DTYPE_BU)) {
            typeMultiplier = sizeof(uint8);
        }
        else if ((nodeType == DTYPE_W) || (nodeType == DTYPE_WU)) {
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
        bufferedDataSize *= numberOfElements * makeSegmentAfterNWrites * numberOfSamples;

        bufferedData = reinterpret_cast<char8*>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(bufferedDataSize));
    }
    return ok;
}

bool MDSWriterNode::AllocateTreeNode(MDSplus::Tree *const tree) {
    bool ok = (tree != NULL_PTR(MDSplus::Tree*));
    try {
        if (node != NULL_PTR(MDSplus::TreeNode*)) {
            delete node;
        }

        /*lint -e{613} tree cannot be NULL as otherwise ok = false*/
        if (ok) {
            node = tree->getNode(nodeName.Buffer());
            node->deleteData();
        }
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed opening node with name %s: %s", nodeName.Buffer(), exc.what());
        ok = false;
    }
    if (ok) {
        if (decimatedNode != NULL_PTR(MDSplus::TreeNode*)) {
            delete decimatedNode;
        }
        if (decimatedMinMax) {
            try {
                /*lint -e{613} ok => tree != NULL*/
                decimatedNode = tree->getNode(decimatedNodeName.Buffer());
                decimatedNode->deleteData();
            }
            catch (const MDSplus::MdsException &exc) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed opening node with name %s: %s", decimatedNodeName.Buffer(), exc.what());
                ok = false;
            }
        }
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
    uint64 timeSignalTime = lastWriteTimeSignal;
    if ((ok) && (!flush)) {
        //If we are using a triggering source get the signal from a time source, as samples might not be continuous
        if (useTimeVector) {

            timeSignalTime = GetTimeSignalMemoryTime();
            if (currentBuffer == 0u) {
                if (timeSignalMemory != NULL_PTR(uint32 *)) {
                    start = static_cast<float64>(timeSignalTime) * timeSignalMultiplier;
                    start += static_cast<float64>(phaseShift) * period;
                }
            }
            //If we are acquiring data based on events (which do not necessarily occur sequentially in time, trigger every time there is a change in the time vector)
            //Only makes sense to check for this if segments are not written @ every Execute
            else {
                if (makeSegmentAfterNWrites > 1u) {
                    if (timeSignalMemory != NULL_PTR(uint32 *)) {
                        float64 executePeriodF = static_cast<float64>(executePeriod);
                        float64 rangeF = executePeriodF * discontinuityFactor;
                        uint64 range = static_cast<uint64>(rangeF);
                        if (lastWriteTimeSignal > 0u) {
                            discontinuityFound = ((timeSignalTime - lastWriteTimeSignal) > (executePeriod + range));
                            if (!discontinuityFound) {
                                if (range > executePeriod) {
                                    range = executePeriod;
                                }
                                discontinuityFound = ((timeSignalTime - lastWriteTimeSignal) < (executePeriod - range));
                            }
                        }
                    }
                }
            }
        }
        //If data is continuous store in the shared buffer that will be flushed as part of the next segment.
        //If data is not continuous this will trigger the creation of a new segment and this signal data will be added to the beginning of the next segment.
        if (!discontinuityFound) {
            if (currentBuffer < makeSegmentAfterNWrites) {
                if ((signalMemory != NULL_PTR(uint32 *)) && (bufferedData != NULL_PTR(void *))) {
                    uint32 signalIdx = currentBuffer * numberOfSamples * numberOfElements * static_cast<uint32>(typeMultiplier);
                    char8 *bufferedDataC = reinterpret_cast<char8*>(bufferedData);
                    ok = MemoryOperationsHelper::Copy(&bufferedDataC[signalIdx], signalMemory,
                                                      numberOfSamples * numberOfElements * static_cast<uint32>(typeMultiplier));
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
    bool storeNow = (discontinuityFound);
    if (!storeNow) {
        if (useTimeVector) {
            if (timeSignalMemory != NULL_PTR(uint32 *)) {
                lastWriteTimeSignal = timeSignalTime;
            }
        }
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
        if (automaticSegmentation) {
            ok = AddDataToSegment();
        }
        else {
            ok = ForceSegment();
        }
        nOfExecuteCalls++;

        //discontuityFound will only be triggered if makeSegmentAfterNWrites > 1, so Execute will be called again later
        //discontuityFound will only be triggered if signalMemory != NULL_PTR (i.e. that we are triggering based on event).
        if ((ok) && (discontinuityFound)) {
            if (useTimeVector) {
                if (timeSignalMemory != NULL_PTR(uint32 *)) {
                    lastWriteTimeSignal = timeSignalTime;
                }
            }
            if ((signalMemory != NULL_PTR(uint32 *)) && (bufferedData != NULL_PTR(void *))) {
                //currentBuffer had already been incremented. Copy the last buffer to the beginning
                char8 *bufferedDataC = reinterpret_cast<char8*>(bufferedData);
                ok = MemoryOperationsHelper::Copy(&bufferedDataC[0u], signalMemory, numberOfSamples * numberOfElements * static_cast<uint32>(typeMultiplier));
            }
            if (ok) {
                currentBuffer = 1u;
            }
            if (timeSignalMemory != NULL_PTR(uint32 *)) {
                //Update the start
                start = static_cast<float64>(timeSignalTime) * timeSignalMultiplier;
                start += static_cast<float64>(phaseShift) * period;
            }
        }
        else {
            currentBuffer = 0u;
        }
    }
    return ok;
}

//lint -e{429} startD, endD, dimension are freed by MDSplus upon deletion of dimension
bool MDSWriterNode::ForceSegment() {
    bool ok = true;
    //Notice that currentBuffer is not incremented if a discontinuity is found
    int32 numberOfSamplesPerSegment = static_cast<int32>(numberOfSamples) * static_cast<int32>(currentBuffer);
    segmentDim[0] = numberOfSamplesPerSegment;
    int32 numberOfSamplesPerSegmentM1 = numberOfSamplesPerSegment - 1;
    float64 numberOfSamplesPerSegmentF = static_cast<float64>(numberOfSamplesPerSegmentM1);
    float64 end = 0.;
    float64 newPeriod = period;
    if (!useTimeVector) {
        end = start + (numberOfSamplesPerSegmentF * period);
    }
    else {
        end = static_cast<float64>(lastWriteTimeSignal) * timeSignalMultiplier;
        float64 periodDelta = period;
        uint32 numberOfSamplesM1 = numberOfSamples - 1u;
        periodDelta *= static_cast<float64>(numberOfSamplesM1);
        end += periodDelta;
        if (!IsEqual(end, start)) {
            if (numberOfSamplesPerSegmentF > 0.) {
                newPeriod = (end - start) / numberOfSamplesPerSegmentF;
            }
        }
    }
    //lint -e{429} freed by MDSplus upon deletion of dimension
    MDSplus::Data *startD = new MDSplus::Float64(start);
    //lint -e{429} freed by MDSplus upon deletion of dimension
    MDSplus::Data *endD = new MDSplus::Float64(end);

    //lint -e{429} freed by MDSplus upon deletion of dimension
    MDSplus::Data *dimension = new MDSplus::Range(startD, endD, new MDSplus::Float64(newPeriod));
    //lint -e{429} freed by MDSplus upon deletion of array
    MDSplus::Array *array = NULL_PTR(MDSplus::Array*);

    if (!useTimeVector) {
        start += static_cast<float64>(numberOfSamplesPerSegment) * newPeriod;
    }
    if (nodeType == DTYPE_B) {
        array = new MDSplus::Int8Array(reinterpret_cast<char8*>(bufferedData), 2, &segmentDim[0]);
    }
    else if (nodeType == DTYPE_BU) {
        array = new MDSplus::Uint8Array(reinterpret_cast<uint8*>(bufferedData), 2, &segmentDim[0]);
    }
    else if (nodeType == DTYPE_W) {
        array = new MDSplus::Int16Array(reinterpret_cast<int16*>(bufferedData), 2, &segmentDim[0]);
    }
    else if (nodeType == DTYPE_WU) {
        array = new MDSplus::Uint16Array(reinterpret_cast<uint16*>(bufferedData), 2, &segmentDim[0]);
    }
    else if (nodeType == DTYPE_L) {
        array = new MDSplus::Int32Array(reinterpret_cast<int32*>(bufferedData), 2, &segmentDim[0]);
    }
    else if (nodeType == DTYPE_LU) {
        array = new MDSplus::Uint32Array(reinterpret_cast<uint32*>(bufferedData), 2, &segmentDim[0]);
    }
    else if (nodeType == DTYPE_Q) {
        array = new MDSplus::Int64Array(reinterpret_cast<int64_t*>(bufferedData), 2, &segmentDim[0]);
    }
    else if (nodeType == DTYPE_QU) {
        array = new MDSplus::Uint64Array(reinterpret_cast<uint64_t*>(bufferedData), 2, &segmentDim[0]);
    }
    else if (nodeType == DTYPE_FLOAT) {
        array = new MDSplus::Float32Array(reinterpret_cast<float32*>(bufferedData), 2, &segmentDim[0]);
    }
    else if (nodeType == DTYPE_DOUBLE) {
        array = new MDSplus::Float64Array(reinterpret_cast<float64*>(bufferedData), 2, &segmentDim[0]);
    }
    else {
        //An invalid nodeType is trapped before.
    }

    if (array != NULL_PTR(MDSplus::Array*)) {
        if (decimatedMinMax) {
            //lint -e{613} node is checked not to be null in the beginning of the function
            try {
                node->makeSegmentMinMax(startD, endD, dimension, array, decimatedNode, minMaxResampleFactor);
            }
            catch (const MDSplus::MdsException &exc) {
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "Failed makeSegmentMinMax Error: %s", exc.what());
                ok = false;
            }
        }
        else {
            //lint -e{613} node is checked not to be null in the beginning of the function
            try {
                node->makeSegment(startD, endD, dimension, array);
            }
            catch (const MDSplus::MdsException &exc) {
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "Failed makeSegment Error: %s", exc.what());
                ok = false;
            }
        }
        MDSplus::deleteData(array);
    }
    MDSplus::deleteData(dimension);
    return ok;
}

/*lint -e{613} -e{429} function only called if bufferedData != NULL. Custodial pointer value freed by mds+*/
bool MDSWriterNode::AddDataToSegment() {
    bool ok = true;

    //Notice that currentBuffer is not incremented if a discontinuity is found
    uint32 numberOfSamplesPerSegment = numberOfSamples * currentBuffer;
    //MDSpluse::putRow() only save one sample at the time
    segmentDim[0] = 1;
    for (uint32 i = 0u; i < numberOfSamplesPerSegment; i++) {
        int64_t auxCurrentTime = static_cast<int64>(start);
        if (!useTimeVector) {
            start += period;
        }
        if (numberOfElements > 1u) {
            int32 numberElementsInt32 = static_cast<int32>(numberOfElements);
            MDSplus::Array *value = NULL_PTR(MDSplus::Array*);
            if (nodeType == DTYPE_B) {
                value = new MDSplus::Int8Array(&(reinterpret_cast<char8*>(bufferedData)[i]), numberElementsInt32);
            }
            else if (nodeType == DTYPE_BU) {
                value = new MDSplus::Uint8Array(&(reinterpret_cast<uint8*>(bufferedData)[i]), numberElementsInt32);
            }
            else if (nodeType == DTYPE_W) {
                value = new MDSplus::Int16Array(&(reinterpret_cast<int16*>(bufferedData)[i]), numberElementsInt32);
            }
            else if (nodeType == DTYPE_WU) {
                value = new MDSplus::Uint16Array(&(reinterpret_cast<uint16*>(bufferedData)[i]), numberElementsInt32);
            }
            else if (nodeType == DTYPE_L) {
                value = new MDSplus::Int32Array(&(reinterpret_cast<int32*>(bufferedData)[i]), numberElementsInt32);
            }
            else if (nodeType == DTYPE_LU) {
                value = new MDSplus::Uint32Array(&(reinterpret_cast<uint32*>(bufferedData)[i]), numberElementsInt32);
            }
            else if (nodeType == DTYPE_Q) {
                value = new MDSplus::Int64Array(&(reinterpret_cast<int64_t*>(bufferedData)[i]), numberElementsInt32);
            }
            else if (nodeType == DTYPE_QU) {
                value = new MDSplus::Uint64Array(&(reinterpret_cast<uint64_t*>(bufferedData)[i]), numberElementsInt32);
            }
            else if (nodeType == DTYPE_FLOAT) {
                value = new MDSplus::Float32Array(&(reinterpret_cast<float32*>(bufferedData)[i]), numberElementsInt32);
            }
            else if (nodeType == DTYPE_DOUBLE) {
                value = new MDSplus::Float64Array(&(reinterpret_cast<float64*>(bufferedData)[i]), numberElementsInt32);
            }
            else {
                //An invalid nodeType is trapped before.
            }
            if (value != NULL_PTR(MDSplus::Array*)) {
                //lint -e{613} node is checked not to be null in the beginning of the function
                try {
                    node->putRow(value, &auxCurrentTime);
                }
                catch (const MDSplus::MdsException &exc) {
                    REPORT_ERROR_STATIC(ErrorManagement::Warning, "Failed putRow() Error: %s", exc.what());
                    ok = false;
                }
                MDSplus::deleteData(value);
            }
        }
        else {
            MDSplus::Scalar *value = NULL_PTR(MDSplus::Scalar*);
            if (nodeType == DTYPE_B) {
                value = new MDSplus::Int8(reinterpret_cast<char8*>(bufferedData)[i]);
            }
            else if (nodeType == DTYPE_BU) {
                value = new MDSplus::Uint8(reinterpret_cast<uint8*>(bufferedData)[i]);
            }
            else if (nodeType == DTYPE_W) {
                value = new MDSplus::Int16(reinterpret_cast<int16*>(bufferedData)[i]);
            }
            else if (nodeType == DTYPE_WU) {
                value = new MDSplus::Uint16(reinterpret_cast<uint16*>(bufferedData)[i]);
            }
            else if (nodeType == DTYPE_L) {
                value = new MDSplus::Int32(reinterpret_cast<int32*>(bufferedData)[i]);
            }
            else if (nodeType == DTYPE_LU) {
                value = new MDSplus::Uint32(reinterpret_cast<uint32*>(bufferedData)[i]);
            }
            else if (nodeType == DTYPE_Q) {
                value = new MDSplus::Int64(reinterpret_cast<int64_t*>(bufferedData)[i]);
            }
            else if (nodeType == DTYPE_QU) {
                value = new MDSplus::Uint64(reinterpret_cast<uint64_t*>(bufferedData)[i]);
            }
            else if (nodeType == DTYPE_FLOAT) {
                value = new MDSplus::Float32(reinterpret_cast<float32*>(bufferedData)[i]);
            }
            else if (nodeType == DTYPE_DOUBLE) {
                value = new MDSplus::Float64(reinterpret_cast<float64*>(bufferedData)[i]);
            }
            else {
                //An invalid nodeType is trapped before.
            }
            if (value != NULL_PTR(MDSplus::Scalar*)) {
                //lint -e{613} node is checked not to be null in the beginning of the function
                try {
                    node->putRow(value, &auxCurrentTime);
                }
                catch (const MDSplus::MdsException &exc) {
                    REPORT_ERROR_STATIC(ErrorManagement::Warning, "Failed putRow() Error: %s", exc.what());
                    ok = false;
                }
                MDSplus::deleteData(value);
            }
        }
    }
    return ok;
}

uint64 MDSWriterNode::GetTimeSignalMemoryTime() const {
    uint64 ret = 0u;
    if (timeSignalMemory != NULL_PTR(void*)) {
        if (timeSignalType == UnsignedInteger32Bit) {
            ret = *(reinterpret_cast<uint32*>(timeSignalMemory));
        }
        else if (timeSignalType == UnsignedInteger64Bit) {
            ret = *(reinterpret_cast<uint64*>(timeSignalMemory));
        }
        else if (timeSignalType == SignedInteger32Bit) {
            ret = static_cast<uint64>(*(reinterpret_cast<int32*>(timeSignalMemory)));
        }
        else if (timeSignalType == SignedInteger64Bit) {
            ret = static_cast<uint64>(*(reinterpret_cast<int64*>(timeSignalMemory)));
        }
        else if (timeSignalType == UnsignedInteger16Bit) {
            ret = *(reinterpret_cast<uint16*>(timeSignalMemory));
        }
        else if (timeSignalType == UnsignedInteger8Bit) {
            ret = static_cast<uint64>(*(reinterpret_cast<uint8*>(timeSignalMemory)));
        }
        else if (timeSignalType == SignedInteger16Bit) {
            ret = static_cast<uint64>(*(reinterpret_cast<int16*>(timeSignalMemory)));
        }
        else if (timeSignalType == SignedInteger8Bit) {
            ret = static_cast<uint64>(*(reinterpret_cast<int8*>(timeSignalMemory)));
        }
        else {
            //Should not be reachable
        }
    }

    return ret;
}

void MDSWriterNode::SetSignalMemory(void *const signalMemoryIn) {
    signalMemory = signalMemoryIn;
}

void MDSWriterNode::SetTimeSignalMemory(void *const timeSignalMemoryIn,
                                        const TypeDescriptor &timeSignalTypeIn,
                                        const float64 timeSignalMultiplierIn) {
    timeSignalMemory = timeSignalMemoryIn;
    timeSignalType = timeSignalTypeIn;
    useTimeVector = (timeSignalMemory != NULL_PTR(void*));
    float64 executePeriodF = static_cast<float64>(numberOfSamples) * period;
    if (timeSignalMultiplierIn > 0.F) {
        timeSignalMultiplier = timeSignalMultiplierIn;
        executePeriodF /= timeSignalMultiplierIn;
    }
    executePeriodF += 0.5F;
    executePeriod = static_cast<uint32>(executePeriodF);

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

uint32 MDSWriterNode::GetExecutePeriod() const {
    return executePeriod;
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
