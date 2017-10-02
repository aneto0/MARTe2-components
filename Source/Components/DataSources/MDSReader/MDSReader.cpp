/**
 * @file MDSReader.cpp
 * @brief Source file for class MDSReader
 * @date 22/09/2017
 * @author Llorenc Capella
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
 * the class MDSReader (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "MDSReader.h"
#include "MemoryMapSynchronisedInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MDSReader::MDSReader() {
    tree = NULL_PTR(MDSplus::Tree *);
    nodeName = NULL_PTR(StreamString *);
    nodes = NULL_PTR(MDSplus::TreeNode **);
    numberOfNodeNames = 0u;
    mdsNodeTypes = NULL_PTR(StreamString *);
    byteSizeSignals = NULL_PTR(uint32 *);
    shotNumber = 0;
    type = NULL_PTR(TypeDescriptor *);
    numberOfElements = NULL_PTR(uint32 *);
    dataSourceMemory = NULL_PTR(char8 *);
    offsets = NULL_PTR(uint32 *);
    time = 0.0;
    currentTime = time;
    frequency = 0.0;
    period = 0.0;
    maxNumberOfSegments = 0u;
    lastSegment = NULL_PTR(uint64 *);
    dataManagement = NULL_PTR(uint8 *);
    holeManagement = NULL_PTR(uint8 *);
    samplingTime = NULL_PTR(float64 *);
    lastValue = NULL_PTR(char8 *);
    offsetLastValue = NULL_PTR(uint32 *);
    elementsConsumed = NULL_PTR(uint32 *);
}

MDSReader::~MDSReader() {

    if (tree != NULL_PTR(MDSplus::Tree *)) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
    }

    if (nodeName != NULL_PTR(StreamString *)) {
        delete[] nodeName;
        nodeName = NULL_PTR(StreamString *);
    }

    if (nodes != NULL_PTR(MDSplus::TreeNode **)) {
        for (uint32 i = 0u; i < numberOfNodeNames; i++) {
            if (nodes[i] != NULL_PTR(MDSplus::TreeNode *)) {
                delete nodes[i];
                nodes[i] = NULL_PTR(MDSplus::TreeNode *);
            }
        }
        delete[] nodes;
        nodes = NULL_PTR(MDSplus::TreeNode **);
    }
    if (mdsNodeTypes != NULL_PTR(StreamString *)) {
        delete[] mdsNodeTypes;

    }
    if (byteSizeSignals != NULL_PTR(uint32 *)) {
        delete[] byteSizeSignals;

    }
    if (type != NULL_PTR(TypeDescriptor *)) {
        delete[] type;

    }
    if (offsets != NULL_PTR(uint32 *)) {
        delete[] offsets;
        offsets = NULL_PTR(uint32 *);
    }
    if (dataSourceMemory != NULL_PTR(char8 *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(dataSourceMemory));
        dataSourceMemory = NULL_PTR(char8 *);
    }
    if (maxNumberOfSegments != NULL_PTR(uint64 *)) {
        delete[] maxNumberOfSegments;
        maxNumberOfSegments = NULL_PTR(uint64 *);
    }
    if (lastSegment != NULL_PTR(uint64 *)) {
        delete[] lastSegment;
        lastSegment = NULL_PTR(uint64 *);
    }
    if (dataManagement != NULL_PTR(uint8 *)) {
        delete[] dataManagement;
        dataManagement = NULL_PTR(uint8 *);
    }
    if (holeManagement != NULL_PTR(uint8 *)) {
        delete[] holeManagement;
        holeManagement = NULL_PTR(uint8 *);
    }
    if (samplingTime != NULL_PTR(float64 *)) {
        delete[] samplingTime;
        samplingTime = NULL_PTR(float64 *);
    }
    if (lastValue != NULL_PTR(char8 *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(lastValue));
        lastValue = NULL_PTR(char8 *);
    }
    if (offsetLastValue != NULL_PTR(uint32 *)) {
        delete[] offsetLastValue;
        offsetLastValue = NULL_PTR(uint32 *);
    }
    if (elementsConsumed != NULL_PTR(uint32 *)) {
        delete[] elementsConsumed;
        elementsConsumed = NULL_PTR(uint32 *);
    }

}

bool MDSReader::Initialise(StructuredDataI& data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("TreeName", treeName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "TreeName shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("ShotNumber", shotNumber);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "ShotNumber shall be specified");
            shotNumber = -1;
        }
        else {
            bool cond1 = (shotNumber < -1);
            bool cond2 = (shotNumber == 0);
            ok = !(cond1 || cond2);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "ShotNumber shall be -1 (last available shot) or positive");
            }
        }
    }
    if (ok) {
        ok = OpenTree();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Error opening tree %s with shotNumber = %u\n", treeName.Buffer(), shotNumber);
        }
    }
    if (ok) { //readFrequency
        ok = data.Read("Frequency", frequency);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read frequency");
        }
        else {
            period = 1 / frequency;
        }
    }
    if (ok) {
        ok = data.MoveRelative("Signals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }
        if (ok) {
            ok = data.Copy(originalSignalInformation);
        }
        if (ok) {
            ok = originalSignalInformation.MoveToRoot();
        }
        //Do not allow to add signals in run-time
        if (ok) {
            ok = signalsDatabase.MoveRelative("Signals");
        }
        if (ok) {
            ok = signalsDatabase.Write("Locked", 1u);
        }
        if (ok) {
            ok = signalsDatabase.MoveToAncestor(1u);
        }
    }
    if (ok) {
        ok = data.MoveToRoot();
    }
    return ok;
}

bool MDSReader::SetConfiguredDatabase(StructuredDataI & data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSourceI::SetConfiguredDatabase(data) returned false");
    }

    if (ok) { // Check that only one GAM is Connected to the MDSReader
        uint32 auxNumberOfFunctions = GetNumberOfFunctions();
        ok = (auxNumberOfFunctions == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly one Function allowed to interact with this DataSourceI. number of Functions = %u",
                         auxNumberOfFunctions);
        }
    }
    if (ok) { //read number of nodes per function numberOfNodeNames
        //0u (second argument) because previously it is checked
        ok = GetFunctionNumberOfSignals(InputSignals, 0u, numberOfNodeNames);        //0u (second argument) because previously it is checked
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetFunctionNumberOfSignals() returned false");
        }
        if (ok) {
            ok = (numberOfNodeNames > 0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The number of signals must be positive");
            }
        }
    }
    if (ok) {        //allocate memory for lastSegment
        lastSegment = new uint64[numberOfNodeNames];
        for (uint32 i = 0u; i < numberOfNodeNames; i++) {
            lastSegment[i] = 0u;
        }
    }
    if (ok) {
        for (uint32 n = 0u; (n < numberOfNodeNames) && ok; n++) {
            uint32 nSamples;
            ok = GetFunctionSignalSamples(InputSignals, 0u, n, nSamples);
            if (ok) {
                ok = (nSamples == 1u);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples shall be exactly 1");
            }
        }
    }

    if (ok) {
        ok = (numberOfNodeNames == GetNumberOfSignals());
        if (!ok) {
            REPORT_ERROR(
                    ErrorManagement::ParametersError,
                    "GetFunctionNumberOfSignals != GetNumberOfSignals(). They should be the same since only one GAM can be connected to this DataSource. GetNumberOfSignals() = %u, numberOfNodeNames = %u",
                    GetNumberOfSignals(), numberOfNodeNames);
        }
    }
    if (ok) { //read nodeNames from originalSignalInformation
        nodeName = new StreamString[numberOfNodeNames];
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            ok = originalSignalInformation.MoveRelative(originalSignalInformation.GetChildName(i));
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to the children %u", auxIdx);
            }
            if (ok) {
                ok = originalSignalInformation.Read("NodeName", nodeName[i]);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "Parameter NodeName for signal %u not found", auxIdx);
                }
            }
            if (ok) {
                ok = originalSignalInformation.MoveToAncestor(1u);
                if (!ok) { //Should never happen
                    REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to the the immediate ancestor");
                }
            }
        }
    }
    if (ok) { //read nodeNames from originalSignalInformation
        nodeName = new StreamString[numberOfNodeNames];
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            ok = originalSignalInformation.MoveRelative(originalSignalInformation.GetChildName(i));
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to the children %u", auxIdx);
            }
            if (ok) {
                ok = originalSignalInformation.Read("NodeName", nodeName[i]);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "Parameter NodeName for signal %u not found", auxIdx);
                }
            }
            if (ok) {
                ok = originalSignalInformation.MoveToAncestor(1u);
                if (!ok) { //Should never happen
                    REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to the the immediate ancestor");
                }
            }
        }
    }
    if (ok) { //check node names are different
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            for (uint32 j = (i + 1u); (j < numberOfNodeNames) && ok; j++) {
                ok = (nodeName[i] != nodeName[j]);
                if (!ok) {
                    uint32 auxI = i;
                    uint32 auxJ = j;
                    REPORT_ERROR(ErrorManagement::ParametersError, "node name in the position %u is equal than the node name in the position %u", auxI, auxJ);
                }
            }
        }
    }
    if (ok) { //open nodes
        nodes = new MDSplus::TreeNode *[numberOfNodeNames];
        for (uint32 i = 0u; i < numberOfNodeNames; i++) { //Initialise to NULL pointer because automatically is not done and the destructor try to delete memory not allocated
            nodes[i] = NULL_PTR(MDSplus::TreeNode *);
        }
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            ok = OpenNode(i);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Error opening node %s. It may not exist \n", nodeName[i].Buffer());
            }
        }
    }
    if (ok) { //get MDSplus node type, validate type and get byte size of each node
        mdsNodeTypes = new StreamString[numberOfNodeNames];
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            ok = GetTypeNode(i);
            if (!ok) { //It should never happen because the node exist. But if the tree is corrupted it may launch the error
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Error getting type node %u \n", auxIdx);
            }
            if (ok) {
                ok = IsValidTypeNode(i);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type. mdsNodeTypes = %s. NodeName = %s\n", mdsNodeTypes[i].Buffer(),
                                 nodeName[i].Buffer());
                }
            }
        }
    }
    if (ok) { //read the type specified in the configuration file and compare against the mdsNodeTypes
        type = new TypeDescriptor[numberOfNodeNames];
        for (uint32 i = 0; (i < numberOfNodeNames) && ok; i++) {
            type[i] = GetSignalType(i);
            ok = !(type[i] == InvalidType);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid type");
            }
            if (ok) {
                ok = CheckTypeAgainstMdsNodeTypes(i);
                if (!ok) {
                    uint32 auxIdx = i;
                    TypeDescriptor auxStr = ConvertMDStypeToMARTeType(mdsNodeTypes[auxIdx]);
                    REPORT_ERROR(ErrorManagement::ParametersError, "signalTypes different to mdsNodeTypes for signal %u. type = %s, mdsNodeTypes = %s (%s)",
                                 auxIdx, TypeDescriptor::GetTypeNameFromTypeDescriptor(type[auxIdx]), mdsNodeTypes[auxIdx].Buffer(),
                                 TypeDescriptor::GetTypeNameFromTypeDescriptor(auxStr));
                }
            }
        }
    }
    if (ok) { //read number of elements
        numberOfElements = new uint32[numberOfNodeNames];
        for (uint32 i = 0; (i < numberOfNodeNames) && ok; i++) {
            ok = GetSignalNumberOfElements(i, numberOfElements[i]);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read NumberOfElements");
            }
            if (ok) {
                ok = numberOfElements[i] != 0u;
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfElements must be positive");
                }
            }
        }
    }
    if (ok) { //calculate samplingTime
        samplingTime = new float64[numberOfNodeNames];
        for (uint32 i = 0u; i < numberOfNodeNames; i++) {
            samplingTime[i] = 1.0 / (frequency * numberOfElements[i]);
        }
    }

    if (ok) { //Count and allocate memory
        offsets = new uint32[numberOfNodeNames];
        byteSizeSignals = new uint32[numberOfNodeNames];
        offsetLastValue = new uint32[numberOfNodeNames];
        //Count the number of bytes
        uint32 totalSignalMemory = 0u;
        uint32 sumLastValueMemory = 0u;
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            offsets[i] = totalSignalMemory;
            offsetLastValue[i] = sumLastValueMemory;
            uint32 nBytes = 0u;
            ok = GetSignalByteSize(i, nBytes);

            byteSizeSignals[i] = nBytes;
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Error while GetSignalByteSize() for signal %u", auxIdx);
            }
            totalSignalMemory += nBytes;
            sumLastValueMemory += type[i].numberOfBits / 8u;
        }
        //Allocate memory
        if (ok) {
            dataSourceMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalSignalMemory));
            lastValue = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(sumLastValueMemory));
        }
    }
    if (ok) {
        maxNumberOfSegments = new uint64[numberOfNodeNames];
        for (uint32 i = 0u; i < numberOfNodeNames; i++) {
            maxNumberOfSegments[i] = nodes[i]->getNumSegments();
        }
    }
    if (ok) { //read DataManagement from originalSignalInformation
        dataManagement = new uint8[numberOfNodeNames];
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            ok = originalSignalInformation.MoveRelative(originalSignalInformation.GetChildName(i));
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to the children %u", auxIdx);
            }
            if (ok) {
                ok = originalSignalInformation.Read("DataManagement", dataManagement[i]);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "Parameter DataManagement for signal %u not found", auxIdx);
                }
            }
            if (ok) {
                ok = (dataManagement[i] < 3);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(
                            ErrorManagement::ParametersError,
                            "Invalid DataManagement value. It could be 0 (nothing), 1 (linear interpolation), 2 (hold last value). dataManagement[%d] = %d",
                            auxIdx, dataManagement[auxIdx]);
                }
            }
            if (ok) { //check time of doing nothing option
                if (dataManagement[i] == 0u) {
                    float64 nodeSamplingTime = 0.0;
                    ok = GetNodeSamplingTime(i, nodeSamplingTime);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Error getting node sample time for nodeName = %s", nodeName[i].Buffer());
                    }
                    if (ok) {
                        ok = IsEqual(samplingTime[i], nodeSamplingTime);
                        if (!ok) {
                            REPORT_ERROR(
                                    ErrorManagement::ParametersError,
                                    "the sampling time of the node %s = %f is different than the sampling time calculated from the parameters = %f and the dataManagement = 0 (do nothing)",
                                    nodeName[i].Buffer(), nodeSamplingTime, samplingTime[i]);
                        }
                    }
                }
            }
            if (ok) {
                ok = originalSignalInformation.MoveToAncestor(1u);
                if (!ok) { //Should never happen
                    REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to the the immediate ancestor");
                }
            }
        }
    }
    if (ok) { //read hole management from originalSignalInformation
        holeManagement = new uint8[numberOfNodeNames];
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            ok = originalSignalInformation.MoveRelative(originalSignalInformation.GetChildName(i));
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to the children %u", auxIdx);
            }
            if (ok) {
                ok = originalSignalInformation.Read("HoleManagement", holeManagement[i]);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "Parameter HoleManagement for signal %u not found", auxIdx);
                }
            }
            if (ok) {
                ok = (holeManagement[i] < 2);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError,
                                 "Invalid HoleManagement value. It could be 0 (add 0), 1 (hold last value). dataManagement[%d], %d", auxIdx,
                                 dataManagement[auxIdx]);
                }
            }
            if (ok) {
                ok = originalSignalInformation.MoveToAncestor(1u);
                if (!ok) { //Should never happen
                    REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to the the immediate ancestor");
                }
            }
        }
    }
    if (ok) { //allocate memory elementsConsumed and set to 0
        elementsConsumed = new uint32[numberOfNodeNames];
        for (uint32 i = 0u; i < numberOfNodeNames; i++) {
            elementsConsumed[i] = 0u;
        }
    }
    return ok;
}

bool MDSReader::Synchronise() {
    bool ok = true;
    currentTime = time;
    for (uint32 i = 0u; i < numberOfNodeNames; i++) {
        GetDataNode(i);
    }
    time += period;
    return ok;
}

bool MDSReader::PrepareNextState(const char8 * const currentStateName,
                                 const char8 * const nextStateName) {
    return true;
}

bool MDSReader::AllocateMemory() {
    return true;
}

uint32 MDSReader::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool MDSReader::GetSignalMemoryBuffer(const uint32 signalIdx,
                                      const uint32 bufferIdx,
                                      void *&signalAddress) {
    bool ok = (dataSourceMemory != NULL_PTR(char8 *));
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "Memory not allocated");
    }
    if (ok) {
        /*lint -e{613} dataSourceMemory cannot be NULL here*/
        char8 *memPtr = &dataSourceMemory[offsets[signalIdx]];
        signalAddress = reinterpret_cast<void *&>(memPtr);
    }
    return ok;
}

const char *MDSReader::GetBrokerName(StructuredDataI &data,
                                     const SignalDirection direction) {
    const char8* brokerName = "";
    if (direction == InputSignals) {
        brokerName = "MemoryMapSynchronisedInputBroker";
    }
    return brokerName;
}

bool MDSReader::GetInputBrokers(ReferenceContainer &inputBrokers,
                                const char8* const functionName,
                                void * const gamMemPtr) {
    bool ok;
    ReferenceT<MemoryMapSynchronisedInputBroker> broker("MemoryMapSynchronisedInputBroker");
    ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    if (ok) {
        ok = inputBrokers.Insert(broker);
    }
    return true;
}

//No output broker supported
bool MDSReader::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr) {
    return false;
}

bool MDSReader::OpenTree() {
    bool ret = true;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), shotNumber);
    }
    catch (MDSplus::MdsException &exc) {
        ret = false;
    }
    return ret;
}

bool MDSReader::OpenNode(uint32 idx) {
    bool ret = true;
    try {
        nodes[idx] = tree->getNode(nodeName[idx].Buffer());
    }
    catch (MDSplus::MdsException &exc) {
        ret = false;
    }
    return ret;
}

bool MDSReader::GetTypeNode(uint32 idx) {
    bool ret = true;
    try {
        mdsNodeTypes[idx] = nodes[idx]->getDType();
    }
    catch (MDSplus::MdsException &exc) {
        ret = false;
    }
    return ret;
}

bool MDSReader::IsValidTypeNode(uint32 idx) {
    bool ret = true;
    if (mdsNodeTypes[idx] == "DTYPE_BU") {

    }
    else if (mdsNodeTypes[idx] == "DTYPE_B") {

    }
    else if (mdsNodeTypes[idx] == "DTYPE_WU") {

    }
    else if (mdsNodeTypes[idx] == "DTYPE_W") {

    }
    else if (mdsNodeTypes[idx] == "DTYPE_LU") {

    }
    else if (mdsNodeTypes[idx] == "DTYPE_L") {

    }
    else if (mdsNodeTypes[idx] == "DTYPE_QU") {

    }
    else if (mdsNodeTypes[idx] == "DTYPE_Q") {

    }
    else if (mdsNodeTypes[idx] == "DTYPE_FS") {

    }
    else if (mdsNodeTypes[idx] == "DTYPE_FT") {

    }
    else {
        ret = false;
    }
    return ret;
}

bool MDSReader::CheckTypeAgainstMdsNodeTypes(uint32 idx) {
    bool ret = (type[idx] == ConvertMDStypeToMARTeType(mdsNodeTypes[idx]));
    return ret;
}

TypeDescriptor MDSReader::ConvertMDStypeToMARTeType(StreamString mdsType) {
    TypeDescriptor str;
    if (mdsType == "DTYPE_BU") {
        str = UnsignedInteger8Bit;
    }
    else if (mdsType == "DTYPE_B") {
        str = SignedInteger8Bit;
    }
    else if (mdsType == "DTYPE_WU") {
        str = UnsignedInteger16Bit;
    }
    else if (mdsType == "DTYPE_W") {
        str = SignedInteger16Bit;
    }
    else if (mdsType == "DTYPE_LU") {
        str = UnsignedInteger32Bit;
    }
    else if (mdsType == "DTYPE_L") {
        str = SignedInteger32Bit;
    }
    else if (mdsType == "DTYPE_QU") {
        str = UnsignedInteger64Bit;
    }
    else if (mdsType == "DTYPE_Q") {
        str = SignedInteger64Bit;
    }
    else if (mdsType == "DTYPE_FS") {
        str = Float32Bit;
    }
    else if (mdsType == "DTYPE_FT") {
        str = Float64Bit;
    }
    return str;

}

/**
 * -1--> end data (not found)
 * 0--> is not end of data but not found. Case signal trigger (the segment are not continuous)
 * 1--> found
 */

bool MDSReader::GetDataNode(uint32 nodeNumber) {
    bool ok = true;
    //MDSplus::Array *dataA = NULL_PTR(MDSplus::Array *);
    //MDSplus::Data *timeD = NULL_PTR(MDSplus::Data *);
    uint32 minSegment = 0u;
    int8 errorCodeMinSegment = -1;
    uint32 maxSegment = 0u;
    int8 errorCodeMaxSegment = -1;
    errorCodeMinSegment = FindSegment(time, minSegment, nodeNumber);
    ok = (errorCodeMinSegment != -1);
    if (ok) {
        errorCodeMaxSegment = FindSegment(time + period, maxSegment, nodeNumber);
    }
    if (ok) {
        // there are 4 options
        if ((errorCodeMinSegment == 0) && (errorCodeMaxSegment == 0)) { //in the time specified there is no segments
            if (holeManagement[nodeNumber] == 0u) {
                MemoryOperationsHelper::Set(reinterpret_cast<void *>(dataSourceMemory[nodeNumber]), 0, byteSizeSignals[nodeNumber]);
            }
            else {
                CopyTheSameValue(nodeNumber, numberOfElements[nodeNumber]);
            }
        }
        else if ((errorCodeMinSegment == 0) && (errorCodeMaxSegment == 1)) {
            AddValuesCopyData(nodeNumber, minSegment, maxSegment);
        }
        else if ((errorCodeMinSegment == 1) && (errorCodeMaxSegment == 0)) { //TODO
            //CopyDataAddValues(nodeNumber, minSegment, maxSegment);
        }
        else if ((errorCodeMinSegment == 1) && (errorCodeMaxSegment == 1)) {
            ok = CopyData(nodeNumber, minSegment);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Error while coping data from the tree to the dynamic memory");
            }
        }
        else if ((errorCodeMinSegment == 1) && (errorCodeMaxSegment == -1)) {
            ok = CopyRemainingData(nodeNumber, minSegment);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Error while coping data from the node = %s to the dynamic memory", nodeName[nodeNumber]);
            }
        }
    }
    //dataD = nodes[nodeNumber]->getSegment(minSegment);
    return ok;

}

int8 MDSReader::FindSegment(float64 t,
                            uint32 &segment,
                            uint32 nodeIdx) {
    bool find = false;
    MDSplus::Data *tminD;
    MDSplus::Data *tmaxD;
    int8 retVal = -1;
    float64 tmax = 0.0;
    for (uint64 i = lastSegment[nodeIdx]; (i < maxNumberOfSegments[nodeIdx]) && !find; i++) {
        nodes[i]->getSegmentLimits(i, &tminD, &tmaxD);
        tmax = tmaxD->getDouble();
        if (t < tmax) {
            find = true;
            if (t < tminD->getDouble()) {
                retVal = 0;
                //It is very important. Even the segment does not exist the index must be updated saying the next segment to be look for is this one.
                //At the same time it is used in the case that the tmin does not exist but tmax segment exist.
                segment = i;
                lastSegment[nodeIdx] = i;

            }
            else {
                retVal = 1;
                segment = i;
                lastSegment[nodeIdx] = i;
            }
        }
    }
    return retVal;
}

bool MDSReader::GetNodeSamplingTime(uint32 idx,
                                    float64 &tDiff) {
    bool ret = true;
    MDSplus::Data *timeD = nodes[idx]->getSegmentDim(0);
    int32 numberOfElementsPerSeg = 0;
    float64 *time = timeD->getDoubleArray(&numberOfElementsPerSeg);
    if (numberOfElementsPerSeg < 1) {
        ret = false;
    }
    else if (numberOfElementsPerSeg == 1) {
        MDSplus::Data *timeD2 = nodes[idx]->getSegmentDim(1);
        int32 numberOfElementsPerSeg2 = 0;
        float64 *time2 = timeD2->getDoubleArray(&numberOfElementsPerSeg2);
        if (numberOfElementsPerSeg2 < 1) {
            ret = false;
        }
        else {
            tDiff = time2[0] - time[0];
        }
    }
    else {
        tDiff = time[1] - time[0];
    }
    return ret;
}

void MDSReader::CopyTheSameValue(uint32 idxNumber,
                                 uint32 numberOfTimes) {
    if (type[idxNumber] == UnsignedInteger8Bit) {
        uint32 numberOfBytes = numberOfTimes;
        MemoryOperationsHelper::Set(reinterpret_cast<void *>(dataSourceMemory[idxNumber]), lastValue[offsetLastValue[idxNumber]], numberOfBytes);
    }
    else if (type[idxNumber] == SignedInteger8Bit) {
        uint32 numberOfBytes = numberOfTimes;
        MemoryOperationsHelper::Set(reinterpret_cast<void *>(dataSourceMemory[idxNumber]), lastValue[offsetLastValue[idxNumber]], numberOfBytes);
    }
    else if (type[idxNumber] == UnsignedInteger16Bit) {
        uint16 *ptr = reinterpret_cast<uint16 *>(dataSourceMemory[offsets[idxNumber]]);
        for (uint32 i = 0u; i < numberOfTimes; i++) {
            ptr[i] = lastValue[offsetLastValue[idxNumber]];
        }
    }
    else if (type[idxNumber] == SignedInteger16Bit) {
        int16 *ptr = reinterpret_cast<int16 *>(dataSourceMemory[offsets[idxNumber]]);
        for (uint32 i = 0u; i < numberOfTimes; i++) {
            ptr[i] = lastValue[offsetLastValue[idxNumber]];
        }
    }
    else if (type[idxNumber] == UnsignedInteger32Bit) {
        uint32 *ptr = reinterpret_cast<uint32 *>(dataSourceMemory[offsets[idxNumber]]);
        for (uint32 i = 0u; i < numberOfTimes; i++) {
            ptr[i] = lastValue[offsetLastValue[idxNumber]];
        }
    }
    else if (type[idxNumber] == SignedInteger32Bit) {
        int32 *ptr = reinterpret_cast<int32 *>(dataSourceMemory[offsets[idxNumber]]);
        for (uint32 i = 0u; i < numberOfTimes; i++) {
            ptr[i] = lastValue[offsetLastValue[idxNumber]];
        }
    }
    else if (type[idxNumber] == UnsignedInteger64Bit) {
        uint64 *ptr = reinterpret_cast<uint64 *>(dataSourceMemory[offsets[idxNumber]]);
        for (uint32 i = 0u; i < numberOfTimes; i++) {
            ptr[i] = lastValue[offsetLastValue[idxNumber]];
        }
    }
    else if (type[idxNumber] == SignedInteger64Bit) {
        int64 *ptr = reinterpret_cast<int64 *>(dataSourceMemory[offsets[idxNumber]]);
        for (uint32 i = 0u; i < numberOfTimes; i++) {
            ptr[i] = lastValue[offsetLastValue[idxNumber]];
        }
    }
    else if (type[idxNumber] == Float32Bit) {
        float32 *ptr = reinterpret_cast<float32 *>(dataSourceMemory[offsets[idxNumber]]);
        for (uint32 i = 0u; i < numberOfTimes; i++) {
            ptr[i] = lastValue[offsetLastValue[idxNumber]];
        }
    }
    else if (type[idxNumber] == Float64Bit) {
        float64 *ptr = reinterpret_cast<float64 *>(dataSourceMemory[offsets[idxNumber]]);
        for (uint32 i = 0u; i < numberOfTimes; i++) {
            ptr[i] = lastValue[offsetLastValue[idxNumber]];
        }
    }

    return;
}

void MDSReader::AddValuesCopyData(uint32 nodeNumber,
                                  uint32 minSegment,
                                  uint32 maxSegment) {
    //MDSplus::Array *dataA = NULL_PTR(MDSplus::Array *);
    //MDSplus::Data *timeD = NULL_PTR(MDSplus::Data *);
    MDSplus::Data *tminD;
    float64 tmin;
    MDSplus::Data *tmaxD;
    nodes[nodeNumber]->getSegmentLimits(minSegment, &tminD, &tmaxD);
    tmin = tminD->getDouble();
    int32 numberOfSamplesTofill = static_cast<int32>((tmin - time) / samplingTime[nodeNumber]);
    uint32 remainingSamples = numberOfElements[nodeNumber] - numberOfSamplesTofill;
    if (holeManagement[nodeNumber] == 0u) {
        uint32 numberOfBytes = numberOfSamplesTofill * type[nodeNumber].numberOfBits / 8u;
        MemoryOperationsHelper::Set(reinterpret_cast<void *>(dataSourceMemory[nodeNumber]), 0, numberOfBytes);
    }
    else {
        CopyTheSameValue(nodeNumber, numberOfSamplesTofill);
    }
    if (dataManagement[nodeNumber] == 0) {
        MakeRawCopy(nodeNumber, minSegment, remainingSamples);
    }
    else if (dataManagement[nodeNumber] == 1) {    //TODO
        //GetLinearInterpolationCopy(nodeNumber, minSegment, maxSegment, remainingSamples);
    }
    else {        //TODO
                  //GetHoldValueCopy(nodeNumber, minSegment, maxSegment, remainingSamples);
    }

    //nodes[nodeNumber]->getSegmentAndDimension(minSegment,dataA, timeD);

    return;
}
uint32 MDSReader::MakeRawCopy(uint32 nodeNumber,
                              uint32 minSeg,
                              uint32 SamplesToCopy) {

    MDSplus::Data *dataD = NULL_PTR(MDSplus::Data *);
    int32 nElements = 0u;
    uint32 bytesToCopy = 0u;
    uint32 extraOffset = 0u;
    bool endSegment = false;
    uint32 samplesCopied = 0;
    uint32 remainingSamplesOnTheSegment = 0u;
    if (type[nodeNumber] == UnsignedInteger8Bit) {
        uint8* data = NULL_PTR(uint8 *);
        for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
            dataD = nodes[nodeNumber]->getSegment(currentSegment);
            data = dataD->getByteUnsignedArray(&nElements);
            remainingSamplesOnTheSegment = static_cast<uint32>(nElements) - elementsConsumed[nodeNumber];
            endSegment = remainingSamplesOnTheSegment <= SamplesToCopy;
            if (!endSegment) {        // no end of segment but no more data need to be copied
                bytesToCopy = SamplesToCopy * type[nodeNumber].numberOfBits / 8;
                samplesCopied += SamplesToCopy;
            }
            else {        // end segment but still more data must be copied
                bytesToCopy = remainingSamplesOnTheSegment * type[nodeNumber].numberOfBits / 8;
                samplesCopied += remainingSamplesOnTheSegment;

                //
            }
            extraOffset = (numberOfElements[nodeNumber] - SamplesToCopy) * type[nodeNumber].numberOfBits / 8;
            MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]),
                                         reinterpret_cast<void *>(&data[elementsConsumed[nodeNumber]]), bytesToCopy);

            //Update values
            if (!endSegment) {        // no end of segment but no more data need to be copied
                elementsConsumed[nodeNumber] += SamplesToCopy;
                SamplesToCopy = 0u;
            }
            else {        // end segment but still more data must be copied
                SamplesToCopy -= ((static_cast<uint32>(nElements)) - elementsConsumed[nodeNumber]);
                elementsConsumed[nodeNumber] = 0u;
            }
        }
    }
    else if (type[nodeNumber] == SignedInteger8Bit) {
        int8* data = NULL_PTR(int8 *);
        for (uint32 currentSegment = minSeg; (maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
            dataD = nodes[nodeNumber]->getSegment(currentSegment);
            data = reinterpret_cast<int8 *>(dataD->getByteArray(&nElements));
            remainingSamplesOnTheSegment = static_cast<uint32>(nElements) - elementsConsumed[nodeNumber];
            endSegment = remainingSamplesOnTheSegment <= SamplesToCopy;
            if (!endSegment) {        // no end of segment but no more data need to be copied
                bytesToCopy = SamplesToCopy * type[nodeNumber].numberOfBits / 8;
                samplesCopied += SamplesToCopy;
            }
            else {        // end segment but still more data must be copied
                bytesToCopy = remainingSamplesOnTheSegment * type[nodeNumber].numberOfBits / 8;
                bytesToCopy = remainingSamplesOnTheSegment * type[nodeNumber].numberOfBits / 8;
                samplesCopied += remainingSamplesOnTheSegment;
            }
            extraOffset = (numberOfElements[nodeNumber] - SamplesToCopy) * type[nodeNumber].numberOfBits / 8;
            MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]),
                                         reinterpret_cast<void *>(&data[elementsConsumed[nodeNumber]]), bytesToCopy);

            //Update values
            if (!endSegment) {        // no end of segment but no more data need to be copied
                elementsConsumed[nodeNumber] += SamplesToCopy;
                SamplesToCopy = 0u;
            }
            else {        // end segment but still more data must be copied
                SamplesToCopy -= ((static_cast<uint32>(nElements)) - elementsConsumed[nodeNumber]);
                elementsConsumed[nodeNumber] = 0u;
            }
        }

    }
    else if (type[nodeNumber] == UnsignedInteger16Bit) {
        uint16* data = NULL_PTR(uint16 *);
        for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
            dataD = nodes[nodeNumber]->getSegment(currentSegment);
            data = dataD->getShortUnsignedArray(&nElements);
            remainingSamplesOnTheSegment = static_cast<uint32>(nElements) - elementsConsumed[nodeNumber];
            endSegment = remainingSamplesOnTheSegment <= SamplesToCopy;
            if (!endSegment) {        // no end of segment but no more data need to be copied
                bytesToCopy = SamplesToCopy * type[nodeNumber].numberOfBits / 8;
                samplesCopied += SamplesToCopy;
            }
            else {        // end segment but still more data must be copied
                bytesToCopy = (static_cast<uint32>(nElements) - elementsConsumed[nodeNumber]) * type[nodeNumber].numberOfBits / 8;
                bytesToCopy = remainingSamplesOnTheSegment * type[nodeNumber].numberOfBits / 8;
                samplesCopied += remainingSamplesOnTheSegment;
            }
            extraOffset = (numberOfElements[nodeNumber] - SamplesToCopy) * type[nodeNumber].numberOfBits / 8;
            MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]),
                                         reinterpret_cast<void *>(&data[elementsConsumed[nodeNumber]]), bytesToCopy);

            //Update values
            if (!endSegment) {        // no end of segment but no more data need to be copied
                elementsConsumed[nodeNumber] += SamplesToCopy;
                SamplesToCopy = 0u;
            }
            else {        // end segment but still more data must be copied
                SamplesToCopy -= ((static_cast<uint32>(nElements)) - elementsConsumed[nodeNumber]);
                elementsConsumed[nodeNumber] = 0u;
            }
        }
    }
    else if (type[nodeNumber] == SignedInteger16Bit) {
        int16* data = NULL_PTR(int16 *);
        for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
            dataD = nodes[nodeNumber]->getSegment(currentSegment);
            data = dataD->getShortArray(&nElements);
            remainingSamplesOnTheSegment = static_cast<uint32>(nElements) - elementsConsumed[nodeNumber];
            endSegment = remainingSamplesOnTheSegment <= SamplesToCopy;
            if (!endSegment) {        // no end of segment but no more data need to be copied
                bytesToCopy = SamplesToCopy * type[nodeNumber].numberOfBits / 8;
                samplesCopied += SamplesToCopy;
            }
            else {        // end segment but still more data must be copied
                bytesToCopy = remainingSamplesOnTheSegment * type[nodeNumber].numberOfBits / 8;
                samplesCopied += remainingSamplesOnTheSegment;
            }
            extraOffset = (numberOfElements[nodeNumber] - SamplesToCopy) * type[nodeNumber].numberOfBits / 8;
            MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]),
                                         reinterpret_cast<void *>(&data[elementsConsumed[nodeNumber]]), bytesToCopy);

            //Update values
            if (!endSegment) {        // no end of segment but no more data need to be copied
                elementsConsumed[nodeNumber] += SamplesToCopy;
                SamplesToCopy = 0u;
            }
            else {        // end segment but still more data must be copied
                SamplesToCopy -= ((static_cast<uint32>(nElements)) - elementsConsumed[nodeNumber]);
                elementsConsumed[nodeNumber] = 0u;
            }
        }
    }
    else if (type[nodeNumber] == UnsignedInteger32Bit) {
        uint32* data = NULL_PTR(uint32 *);
        for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
            dataD = nodes[nodeNumber]->getSegment(currentSegment);
            data = dataD->getIntUnsignedArray(&nElements);
            remainingSamplesOnTheSegment = static_cast<uint32>(nElements) - elementsConsumed[nodeNumber];
            endSegment = remainingSamplesOnTheSegment <= SamplesToCopy;
            if (!endSegment) {        // no end of segment but no more data need to be copied
                bytesToCopy = SamplesToCopy * type[nodeNumber].numberOfBits / 8;
                samplesCopied += SamplesToCopy;
            }
            else {        // end segment but still more data must be copied
                bytesToCopy = remainingSamplesOnTheSegment * type[nodeNumber].numberOfBits / 8;
                samplesCopied += remainingSamplesOnTheSegment;
            }
            extraOffset = (numberOfElements[nodeNumber] - SamplesToCopy) * type[nodeNumber].numberOfBits / 8;
            MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]),
                                         reinterpret_cast<void *>(&data[elementsConsumed[nodeNumber]]), bytesToCopy);

            //Update values
            if (!endSegment) {        // no end of segment but no more data need to be copied
                elementsConsumed[nodeNumber] += SamplesToCopy;
                SamplesToCopy = 0u;
            }
            else {        // end segment but still more data must be copied
                SamplesToCopy -= ((static_cast<uint32>(nElements)) - elementsConsumed[nodeNumber]);
                elementsConsumed[nodeNumber] = 0u;
            }
        }
    }
    else if (type[nodeNumber] == SignedInteger32Bit) {
        int32* data = NULL_PTR(int32 *);
        for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
            dataD = nodes[nodeNumber]->getSegment(currentSegment);
            data = dataD->getIntArray(&nElements);
            remainingSamplesOnTheSegment = static_cast<uint32>(nElements) - elementsConsumed[nodeNumber];
            endSegment = remainingSamplesOnTheSegment <= SamplesToCopy;
            if (!endSegment) {        // no end of segment but no more data need to be copied
                bytesToCopy = SamplesToCopy * type[nodeNumber].numberOfBits / 8;
                samplesCopied += SamplesToCopy;
            }
            else {        // end segment but still more data must be copied
                bytesToCopy = remainingSamplesOnTheSegment * type[nodeNumber].numberOfBits / 8;
                samplesCopied += remainingSamplesOnTheSegment;
            }
            extraOffset = (numberOfElements[nodeNumber] - SamplesToCopy) * type[nodeNumber].numberOfBits / 8;
            MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]),
                                         reinterpret_cast<void *>(&data[elementsConsumed[nodeNumber]]), bytesToCopy);

            //Update values
            if (!endSegment) {        // no end of segment but no more data need to be copied
                elementsConsumed[nodeNumber] += SamplesToCopy;
                SamplesToCopy = 0u;
            }
            else {        // end segment but still more data must be copied
                SamplesToCopy -= ((static_cast<uint32>(nElements)) - elementsConsumed[nodeNumber]);
                elementsConsumed[nodeNumber] = 0u;
            }
        }
    }
    else if (type[nodeNumber] == UnsignedInteger64Bit) {
        uint64* data = NULL_PTR(uint64 *);
        for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
            dataD = nodes[nodeNumber]->getSegment(currentSegment);
            data = reinterpret_cast<uint64 *>(dataD->getLongUnsignedArray(&nElements));
            remainingSamplesOnTheSegment = static_cast<uint32>(nElements) - elementsConsumed[nodeNumber];
            endSegment = remainingSamplesOnTheSegment <= SamplesToCopy;
            if (!endSegment) {        // no end of segment but no more data need to be copied
                bytesToCopy = SamplesToCopy * type[nodeNumber].numberOfBits / 8;
                samplesCopied += SamplesToCopy;
            }
            else {        // end segment but still more data must be copied
                bytesToCopy = remainingSamplesOnTheSegment * type[nodeNumber].numberOfBits / 8;
                samplesCopied += remainingSamplesOnTheSegment;
            }
            extraOffset = (numberOfElements[nodeNumber] - SamplesToCopy) * type[nodeNumber].numberOfBits / 8;
            MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]),
                                         reinterpret_cast<void *>(&data[elementsConsumed[nodeNumber]]), bytesToCopy);

            //Update values
            if (!endSegment) {        // no end of segment but no more data need to be copied
                elementsConsumed[nodeNumber] += SamplesToCopy;
                SamplesToCopy = 0u;
            }
            else {        // end segment but still more data must be copied
                SamplesToCopy -= ((static_cast<uint32>(nElements)) - elementsConsumed[nodeNumber]);
                elementsConsumed[nodeNumber] = 0u;
            }
        }
    }
    else if (type[nodeNumber] == SignedInteger64Bit) {
        int64* data = NULL_PTR(int64 *);
        for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
            dataD = nodes[nodeNumber]->getSegment(currentSegment);
            data = reinterpret_cast<int64 *>(dataD->getLongArray(&nElements));
            remainingSamplesOnTheSegment = static_cast<uint32>(nElements) - elementsConsumed[nodeNumber];
            endSegment = remainingSamplesOnTheSegment <= SamplesToCopy;
            if (!endSegment) {        // no end of segment but no more data need to be copied
                bytesToCopy = SamplesToCopy * type[nodeNumber].numberOfBits / 8;
                samplesCopied += SamplesToCopy;
            }
            else {        // end segment but still more data must be copied
                bytesToCopy = remainingSamplesOnTheSegment * type[nodeNumber].numberOfBits / 8;
                samplesCopied += remainingSamplesOnTheSegment;
            }
            extraOffset = (numberOfElements[nodeNumber] - SamplesToCopy) * type[nodeNumber].numberOfBits / 8;
            MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]),
                                         reinterpret_cast<void *>(&data[elementsConsumed[nodeNumber]]), bytesToCopy);

            //Update values
            if (!endSegment) {        // no end of segment but no more data need to be copied
                elementsConsumed[nodeNumber] += SamplesToCopy;
                SamplesToCopy = 0u;
            }
            else {        // end segment but still more data must be copied
                SamplesToCopy -= ((static_cast<uint32>(nElements)) - elementsConsumed[nodeNumber]);
                elementsConsumed[nodeNumber] = 0u;
            }
        }
    }
    else if (type[nodeNumber] == Float32Bit) {
        float32* data = NULL_PTR(float32 *);
        for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
            dataD = nodes[nodeNumber]->getSegment(currentSegment);
            data = dataD->getFloatArray(&nElements);
            remainingSamplesOnTheSegment = static_cast<uint32>(nElements) - elementsConsumed[nodeNumber];
            endSegment = remainingSamplesOnTheSegment <= SamplesToCopy;
            if (!endSegment) {        // no end of segment but no more data need to be copied
                bytesToCopy = SamplesToCopy * type[nodeNumber].numberOfBits / 8;
                samplesCopied += SamplesToCopy;
            }
            else {        // end segment but still more data must be copied
                bytesToCopy = remainingSamplesOnTheSegment * type[nodeNumber].numberOfBits / 8;
                samplesCopied += remainingSamplesOnTheSegment;
            }
            extraOffset = (numberOfElements[nodeNumber] - SamplesToCopy) * type[nodeNumber].numberOfBits / 8;
            MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]),
                                         reinterpret_cast<void *>(&data[elementsConsumed[nodeNumber]]), bytesToCopy);

            //Update values
            if (!endSegment) {        // no end of segment but no more data need to be copied
                elementsConsumed[nodeNumber] += SamplesToCopy;
                SamplesToCopy = 0u;
            }
            else {        // end segment but still more data must be copied
                SamplesToCopy -= ((static_cast<uint32>(nElements)) - elementsConsumed[nodeNumber]);
                elementsConsumed[nodeNumber] = 0u;
            }
        }
    }
    else if (type[nodeNumber] == Float64Bit) {
        float64* data = NULL_PTR(float64 *);
        for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
            dataD = nodes[nodeNumber]->getSegment(currentSegment);
            data = dataD->getDoubleArray(&nElements);
            remainingSamplesOnTheSegment = static_cast<uint32>(nElements) - elementsConsumed[nodeNumber];
            endSegment = remainingSamplesOnTheSegment <= SamplesToCopy;
            if (!endSegment) {        // no end of segment but no more data need to be copied
                bytesToCopy = SamplesToCopy * type[nodeNumber].numberOfBits / 8;
                samplesCopied += SamplesToCopy;
            }
            else {        // end segment but still more data must be copied
                bytesToCopy = remainingSamplesOnTheSegment * type[nodeNumber].numberOfBits / 8;
                samplesCopied += remainingSamplesOnTheSegment;
            }
            extraOffset = (numberOfElements[nodeNumber] - SamplesToCopy) * type[nodeNumber].numberOfBits / 8;
            MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]),
                                         reinterpret_cast<void *>(&data[elementsConsumed[nodeNumber]]), bytesToCopy);

            //Update values
            if (!endSegment) {        // no end of segment but no more data need to be copied
                elementsConsumed[nodeNumber] += SamplesToCopy;
                SamplesToCopy = 0u;
            }
            else {        // end segment but still more data must be copied
                SamplesToCopy -= ((static_cast<uint32>(nElements)) - elementsConsumed[nodeNumber]);
                elementsConsumed[nodeNumber] = 0u;
            }
        }
    }
    return samplesCopied;
}

bool MDSReader::CopyData(uint32 nodeNumber,
                         uint32 minSegment) {
    bool ret = false;
    uint32 samplesCopied = 0;
    if (dataManagement[nodeNumber] == 0) {
        samplesCopied = MakeRawCopy(nodeNumber, minSegment, numberOfElements[nodeNumber]);
    }
    else if (dataManagement[nodeNumber] == 0) {
        samplesCopied = LinearInterpolationCopy(nodeNumber, minSegment, numberOfElements[nodeNumber]);
    }
    else {

    }
    ret = (samplesCopied == numberOfElements[nodeNumber]);
    return ret;
}

uint32 MDSReader::LinearInterpolationCopy(uint32 nodeNumber,
                                          uint32 minSeg,
                                          uint32 SamplesToCopy) {
    MDSplus::Array *dataD = NULL_PTR(MDSplus::Array *);
    MDSplus::Data *timeNodeD = NULL_PTR(MDSplus::Data *);
    float64 *timeNode = NULL_PTR(float64 *);
    int32 nElements = 0u;
    uint32 extraOffset = (numberOfElements[nodeNumber] - SamplesToCopy) * type[nodeNumber].numberOfBits / 8;
    ;
    bool endSegment = false;
    uint32 samplesCopied = 0;
    //In this case it is the number of samples that can be created from this segment.
    uint32 remainingSamplesOnTheSegment = 0u;
    if (type[nodeNumber] == UnsignedInteger8Bit) {
        uint8* data = NULL_PTR(uint8 *);
        for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
            nodes[nodeNumber]->getSegmentAndDimension(currentSegment, dataD, timeNodeD);
            data = dataD->getByteUnsignedArray(&nElements);
            timeNode = timeNodeD->getDoubleArray(&nElements);
            remainingSamplesOnTheSegment = static_cast<uint32>((timeNode[nElements] - currentTime) / samplingTime[nodeNumber]);
            endSegment = (remainingSamplesOnTheSegment <= SamplesToCopy);
            if (!endSegment) {        //no end of segment but no more data need to be copied
                samplesCopied += SamplesToCopy;
            }
            else {        // end segment but still more data must be copied
                samplesCopied += remainingSamplesOnTheSegment;
            }
            for (uint32 i = 0u; i < samplesCopied; i++) {
                while ((currentTime > timeNode[elementsConsumed[nodeNumber] + 1]) && (elementsConsumed[nodeNumber] < static_cast<uint32>(nElements))) {
                    elementsConsumed[nodeNumber]++;
                }

                SampleInterpolation<uint8>(currentTime, data[elementsConsumed[nodeNumber] - 1], data[elementsConsumed[nodeNumber]],
                                           timeNode[elementsConsumed[nodeNumber] - 1], timeNode[elementsConsumed[nodeNumber]],
                                           &dataSourceMemory[offsets[nodeNumber] + extraOffset]);
                extraOffset += 1;
                currentTime += samplingTime[nodeNumber];
                SamplesToCopy--;
            }
        }
    }

    return samplesCopied;

}

template<typename T>
bool MDSReader::SampleInterpolation(float64 ct,
                                    T data1,
                                    T data2,
                                    float64 t1,
                                    float64 t2,
                                    char8 * ptr) {
    bool ret = t2 > t1;
    if (ret) {
        float64 slope = (data2 - data1) / (t2 - t1);
        *ptr = static_cast<T>(slope*(ct - t1) + data1);
    }
    return ret;
}
bool MDSReader::CopyRemainingData(uint32 nodeNumber,
                                  uint32 minSegment) {
    bool ret = false;
    uint32 samplesCopied;
    if (dataManagement[nodeNumber] == 0) {
        samplesCopied = MakeRawCopy(nodeNumber, minSegment, numberOfElements[nodeNumber]);
        ret = (samplesCopied <= numberOfElements[nodeNumber]);
    }
    else if (dataManagement[nodeNumber] == 0) {

    }
    else {

    }
    return ret;
}

CLASS_REGISTER(MDSReader, "1.0")
}

