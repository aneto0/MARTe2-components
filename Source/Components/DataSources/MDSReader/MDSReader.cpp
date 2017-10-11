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
    bytesType = NULL_PTR(uint32 *);
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
    lastTime = NULL_PTR(float64 *);
    offsetLastValue = NULL_PTR(uint32 *);
    elementsConsumed = NULL_PTR(uint32 *);
    endNode = NULL_PTR(bool *);
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
    if (lastTime != NULL_PTR(float64 *)) {
        delete lastTime;
        lastTime = NULL_PTR(float64 *);
    }
    if (offsetLastValue != NULL_PTR(uint32 *)) {
        delete[] offsetLastValue;
        offsetLastValue = NULL_PTR(uint32 *);
    }
    if (elementsConsumed != NULL_PTR(uint32 *)) {
        delete[] elementsConsumed;
        elementsConsumed = NULL_PTR(uint32 *);
    }
    if (endNode != NULL_PTR(bool *)) {
        delete[] endNode;
        endNode = NULL_PTR(bool *);
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

    if (ok) { //Compute the type byte size
        bytesType = new uint32[numberOfNodeNames];
        for (uint32 i = 0u; i < numberOfNodeNames; i++) {
            bytesType[i] = type[i].numberOfBits / 8;
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
            samplingTime[i] = (1.0 / (frequency)) / numberOfElements[i];
        }
    }

    if (ok) { //Count and allocate memory for dataSourceMemory, lastValue and lastTime
        offsets = new uint32[numberOfNodeNames];
        byteSizeSignals = new uint32[numberOfNodeNames];
        offsetLastValue = new uint32[numberOfNodeNames];
        lastTime = new float64[numberOfNodeNames];
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
        if (ok) { //initialise lastValues and lastTimes
            MemoryOperationsHelper::Set(reinterpret_cast<void *>(lastValue), 0, sumLastValueMemory);
            for (uint32 i = 0u; i < numberOfNodeNames; i++) {
                lastTime[i] = -samplingTime[i];
            }
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
                                 "Invalid HoleManagement value. It could be 0 (add 0), 1 (hold last value). dataManagement[%d] = %d", auxIdx,
                                 holeManagement[auxIdx]);
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
    if (ok) {
        endNode = new bool[numberOfNodeNames];
        for (uint32 i = 0u; i < numberOfNodeNames; i++) {
            endNode[i] = false;
        }
    }
    return ok;
}

bool MDSReader::Synchronise() {
    bool ok = true;
    for (uint32 i = 0u; i < numberOfNodeNames; i++) {
        currentTime = time;
        endNode[i] = !GetDataNode(i);
    }
    ok = !AllNodesEnd();
    time += period;
    return ok;
}

bool MDSReader::AllNodesEnd(){
    bool ret = true;
    for(uint32 i = 0u; (i < numberOfNodeNames) && ret; i++){
        ret = endNode[i];
    }
    return ret;
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
 * 1--> time found in a segment
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
    uint32 numberOfDiscontinuities = 0u;
    if (ok) {
        errorCodeMaxSegment = FindSegment(time + period, maxSegment, nodeNumber);
        numberOfDiscontinuities = CheckDiscontinuityOfTheSegments(nodeNumber, minSegment, maxSegment);
    }
    if (ok) {    //Decides how to copy data
// there are 4 options
        if ((errorCodeMinSegment == 0) && (errorCodeMaxSegment == 0)) {
            AddValuesCopyDataAddValues(nodeNumber, minSegment, numberOfDiscontinuities);
        }
        else if ((errorCodeMinSegment == 0) && (errorCodeMaxSegment == 1)) {
            AddValuesCopyData(nodeNumber, minSegment, maxSegment, numberOfDiscontinuities);
        }
        else if ((errorCodeMinSegment == 1) && (errorCodeMaxSegment == 0)) {
            CopyDataAddValues(nodeNumber, minSegment, maxSegment, numberOfDiscontinuities, numberOfElements[nodeNumber], 0u);
        }
        else if ((errorCodeMinSegment == 1) && (errorCodeMaxSegment == 1)) {
            CopyDataAddValuesCopyData(nodeNumber, minSegment, numberOfDiscontinuities);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Error while coping data from the tree to the dynamic memory");
            }
        }
        else if ((errorCodeMinSegment == 1) && (errorCodeMaxSegment == -1)) {
            ok = CopyRemainingData(nodeNumber, minSegment);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Error while coping data from the node = %s to the dynamic memory", nodeName[nodeNumber]);
            }
            //end node
            ok = false;
        }
    }
    else {
        MemoryOperationsHelper::Set(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber]]), 0, byteSizeSignals[nodeNumber]);
    }
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
    float64 tmin = 0.0;
    for (uint64 i = lastSegment[nodeIdx]; (i < maxNumberOfSegments[nodeIdx]) && !find; i++) {
        nodes[nodeIdx]->getSegmentLimits(i, &tminD, &tmaxD);
        tmax = tmaxD->getDouble();
        if (t < tmax) {
            find = true;
            tmin = tminD->getDouble();
            if (t < tmin) {
                //look the tmax Previous segment and verify if the difference is smaller than the
                if (i < 2) {
                    retVal = 0;
                    //It is very important. Even the segment does not exist the index must be updated saying the next segment to be look for is this one.
                    //At the same time it is used in the case that the tmin does not exist but tmax segment exist.
                    segment = i;
                    lastSegment[nodeIdx] = i;
                }
                else {
                    MDSplus::Data *tminPreviousD;
                    MDSplus::Data *tmaxPreviousD;
                    float64 tmaxPrevious = 0.0;
                    float64 nodeSamplingTime = 0.0;
                    nodes[nodeIdx]->getSegmentLimits(i - 1, &tminPreviousD, &tmaxPreviousD);
                    tmaxPrevious = tmaxPreviousD->getDouble();
                    GetNodeSamplingTime(nodeIdx, nodeSamplingTime);
                    if ((tmin - tmaxPrevious) > (nodeSamplingTime * 1.5)) { //1.5 due o numeric errors. if a samples i s not saved the difference should be nodeSamplingTime * 2
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
            else {
                retVal = 1;
                segment = i;
                lastSegment[nodeIdx] = i;
            }
        }
        MDSplus::deleteData(tminD);
        MDSplus::deleteData(tmaxD);
    }
    return retVal;
}

uint32 MDSReader::CheckDiscontinuityOfTheSegments(uint32 nodeNumber,
                                                  uint32 initialSegment,
                                                  uint32 finalSegment) {
    uint32 counter = 0;
    int32 maxSegment = static_cast<int32>(finalSegment + 1u);
    MDSplus::Data *tminD = NULL_PTR(MDSplus::Data *);
    MDSplus::Data *tmaxD = NULL_PTR(MDSplus::Data *);

    float64 tmin = 0.0;
    float64 tmax = 0.0;

    float64 nodeTimeDiff;
    GetNodeSamplingTime(nodeNumber, nodeTimeDiff);
    nodes[nodeNumber]->getSegmentLimits(initialSegment, &tminD, &tmaxD);
    tmax = tmaxD->getDouble();
    for (int32 currentSegment = static_cast<int32>(initialSegment + 1u); currentSegment < maxSegment; currentSegment++) {
        nodes[nodeNumber]->getSegmentLimits(currentSegment, &tminD, &tmaxD);
        tmin = tminD->getDouble();
        if ((tmin - tmax) > nodeTimeDiff * 1.5) {
            counter++;
        }
        tmax = tmaxD->getDouble();
    }
    MDSplus::deleteData(tminD);
    MDSplus::deleteData(tmaxD);
    return counter;
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
                                 uint32 numberOfTimes,
                                 uint32 samplesOffset) {

    if (type[idxNumber] == UnsignedInteger8Bit) {
        CopyTheSameValueTemplate<uint8>(idxNumber, numberOfTimes, samplesOffset);
    }
    else if (type[idxNumber] == SignedInteger8Bit) {
        CopyTheSameValueTemplate<int8>(idxNumber, numberOfTimes, samplesOffset);
    }
    else if (type[idxNumber] == UnsignedInteger16Bit) {
        CopyTheSameValueTemplate<uint16>(idxNumber, numberOfTimes, samplesOffset);
    }
    else if (type[idxNumber] == SignedInteger16Bit) {
        CopyTheSameValueTemplate<int16>(idxNumber, numberOfTimes, samplesOffset);
    }
    else if (type[idxNumber] == UnsignedInteger32Bit) {
        CopyTheSameValueTemplate<uint32>(idxNumber, numberOfTimes, samplesOffset);
    }
    else if (type[idxNumber] == SignedInteger32Bit) {
        CopyTheSameValueTemplate<int32>(idxNumber, numberOfTimes, samplesOffset);
    }
    else if (type[idxNumber] == UnsignedInteger64Bit) {
        CopyTheSameValueTemplate<uint64>(idxNumber, numberOfTimes, samplesOffset);
    }
    else if (type[idxNumber] == SignedInteger64Bit) {
        CopyTheSameValueTemplate<int64>(idxNumber, numberOfTimes, samplesOffset);
    }
    else if (type[idxNumber] == Float32Bit) {
        CopyTheSameValueTemplate<float32>(idxNumber, numberOfTimes, samplesOffset);
    }
    else if (type[idxNumber] == Float64Bit) {
        CopyTheSameValueTemplate<float64>(idxNumber, numberOfTimes, samplesOffset);
    }
    return;
}

template<typename T>
void MDSReader::CopyTheSameValueTemplate(uint32 idxNumber,
                                         uint32 numberOfTimes,
                                         uint32 samplesOffset) {
    uint32 extraOffset = samplesOffset * bytesType[idxNumber];
    T *ptr = reinterpret_cast<T *>(&dataSourceMemory[offsets[idxNumber] + extraOffset]);
    for (uint32 i = 0u; i < numberOfTimes; i++) {
        ptr[i] = *reinterpret_cast<T *>(&lastValue[offsetLastValue[idxNumber]]);
    }
}

bool MDSReader::AddValuesCopyData(uint32 nodeNumber,
                                  uint32 minSegment,
                                  uint32 maxSegment,
                                  uint32 numberOfDiscontinuities) {
    uint32 remainingSamplesToCopy = numberOfElements[nodeNumber];

    uint32 numberOfSamplesActuallyCopied = 0u;
    uint32 numberOfSamplesToCopy = 0u;
    float64 tBeginningDiscontinuity = 0.0;
    float64 tEndDiscontinuity = 0.0;
    uint32 futureSegment = 0u;
    uint32 extraOffset = 0;
    if (minSegment > 0) {
        futureSegment = minSegment - 1;
    }
    uint32 numberOfSamplesCopied = 0u;
    bool ret = true;

    FindDisconinuity(nodeNumber, futureSegment, tBeginningDiscontinuity, tEndDiscontinuity);
    numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, tEndDiscontinuity);
    extraOffset = numberOfSamplesCopied * bytesType[nodeNumber];
    if (holeManagement[nodeNumber] == 0u) {
        uint32 numberOfBytes = numberOfSamplesToCopy * bytesType[nodeNumber];
        MemoryOperationsHelper::Set(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]), 0, numberOfBytes);
    }
    else {
        CopyTheSameValue(nodeNumber, numberOfSamplesToCopy, numberOfSamplesCopied);
    }
    numberOfSamplesCopied += numberOfSamplesToCopy;
    remainingSamplesToCopy -= numberOfSamplesToCopy;
    currentTime += numberOfSamplesToCopy * samplingTime[nodeNumber];

    for (uint32 i = 0u; (i < numberOfDiscontinuities) && ret; i++) {
        FindDisconinuity(nodeNumber, futureSegment, tBeginningDiscontinuity, tEndDiscontinuity);
        float64 auxTime = tBeginningDiscontinuity + samplingTime[nodeNumber];
        numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime);
        if (dataManagement[nodeNumber] == 0) {
            numberOfSamplesActuallyCopied = MakeRawCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
            currentTime += numberOfSamplesActuallyCopied * samplingTime[nodeNumber];
        }
        else if (dataManagement[nodeNumber] == 1) {
            numberOfSamplesActuallyCopied = LinearInterpolationCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        else {
            numberOfSamplesActuallyCopied = HoldCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        numberOfSamplesCopied += numberOfSamplesActuallyCopied;
        remainingSamplesToCopy -= numberOfSamplesActuallyCopied;
        ret = (numberOfSamplesActuallyCopied == numberOfSamplesToCopy);
        numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, tEndDiscontinuity);
        if (ret) {
            extraOffset = numberOfSamplesCopied * bytesType[nodeNumber];
            if (holeManagement[nodeNumber] == 0u) {
                uint32 numberOfBytes = numberOfSamplesToCopy * bytesType[nodeNumber];
                MemoryOperationsHelper::Set(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]), 0, numberOfBytes);
            }
            else {
                CopyTheSameValue(nodeNumber, numberOfSamplesToCopy, numberOfSamplesCopied);
            }
            currentTime += numberOfSamplesToCopy * samplingTime[nodeNumber];
            numberOfSamplesCopied += numberOfSamplesToCopy;
            remainingSamplesToCopy -= numberOfSamplesToCopy;
            minSegment = futureSegment;
        }
    }

    numberOfSamplesToCopy = remainingSamplesToCopy;
    if (dataManagement[nodeNumber] == 0) {
        numberOfSamplesActuallyCopied = MakeRawCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        currentTime += numberOfSamplesToCopy * samplingTime[nodeNumber];
    }
    else if (dataManagement[nodeNumber] == 1) {
        numberOfSamplesActuallyCopied = LinearInterpolationCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
    }
    else {
        numberOfSamplesActuallyCopied = HoldCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
    }
    numberOfSamplesCopied += numberOfSamplesActuallyCopied;
    ret = (numberOfSamplesCopied == numberOfElements[nodeNumber]);
//nodes[nodeNumber]->getSegmentAndDimension(minSegment,dataA, timeD);
    return ret;
}

uint32 MDSReader::ComputeSamplesToCopy(uint32 nodeNumber,
                                       float64 tstart,
                                       float64 tend) {
    uint32 samplesToCopy = static_cast<int32>((tend - tstart) / samplingTime[nodeNumber]);
    VerifySamples(nodeNumber, samplesToCopy, tstart, tend);
    return samplesToCopy;

}

void MDSReader::VerifySamples(uint32 nodeNumber,
                              uint32 &samples,
                              float64 tstart,
                              float64 tend) {

    float64 tinterval = tend - tstart;
    float64 samplesInterval = samples * samplingTime[nodeNumber];
    float64 diff = tinterval - samplesInterval;
    if (diff > samplingTime[nodeNumber] * 0.9999999) {
        samples++;
    }
    return;
}
bool MDSReader::CopyDataAddValues(uint32 nodeNumber,
                                  uint32 minSegment,
                                  uint32 maxSegment,
                                  uint32 numberOfDiscontinuities,
                                  uint32 samplesToRead,
                                  uint32 samplesRead) {
    int32 numberOfSamplesToCopy = 0u;
    uint32 remainingSamplesToCopy = samplesToRead;
    uint32 numberOfSamplesCopied = 0;
    uint32 totalSamplesCopied = samplesRead;
    uint32 numberOfSamplesActuallyCopied = 0u;
    float64 tBeginningDiscontinuity = 0.0;
    float64 tEndDiscontinuity = 0.0;
    uint32 futureSegment = minSegment;
    for (uint32 i = 0u; i < numberOfDiscontinuities; i++) {
        FindDisconinuity(nodeNumber, futureSegment, tBeginningDiscontinuity, tEndDiscontinuity);
        float64 auxTime = tBeginningDiscontinuity + samplingTime[nodeNumber];
        numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime);
        if (dataManagement[nodeNumber] == 0) {
            numberOfSamplesActuallyCopied = MakeRawCopy(nodeNumber, minSegment, numberOfSamplesToCopy, totalSamplesCopied);
            currentTime += numberOfSamplesActuallyCopied * samplingTime[nodeNumber];
        }
        else if (dataManagement[nodeNumber] == 1) {
            numberOfSamplesActuallyCopied = LinearInterpolationCopy(nodeNumber, minSegment, numberOfSamplesToCopy, totalSamplesCopied);
        }
        else {
            numberOfSamplesActuallyCopied = HoldCopy(nodeNumber, minSegment, numberOfSamplesToCopy, totalSamplesCopied);
        }
        numberOfSamplesCopied += numberOfSamplesActuallyCopied;
        totalSamplesCopied += numberOfSamplesActuallyCopied;
        remainingSamplesToCopy -= numberOfSamplesActuallyCopied;
        uint32 samplesToEndDiscontinuity = (tEndDiscontinuity - tBeginningDiscontinuity) / samplingTime[nodeNumber];
        if (samplesToEndDiscontinuity > remainingSamplesToCopy) {
            numberOfSamplesToCopy = remainingSamplesToCopy;
        }
        else {
            numberOfSamplesToCopy = samplesToEndDiscontinuity;
        }
        if (holeManagement[nodeNumber] == 0u) {
            uint32 numberOfBytes = numberOfSamplesToCopy * bytesType[nodeNumber];
            uint32 extraOffset = numberOfSamplesCopied * bytesType[nodeNumber];
            MemoryOperationsHelper::Set(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]), 0, numberOfBytes);
        }
        else {
            CopyTheSameValue(nodeNumber, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        currentTime += numberOfSamplesToCopy * samplingTime[nodeNumber];
        numberOfSamplesCopied += numberOfSamplesToCopy;
        totalSamplesCopied += numberOfSamplesToCopy;
        remainingSamplesToCopy -= numberOfSamplesToCopy;
        minSegment = futureSegment;
    }
    return (numberOfSamplesCopied == samplesToRead);
}

bool MDSReader::AddValuesCopyDataAddValues(uint32 nodeNumber,
                                           uint32 minSegment,
                                           uint32 numberOfDiscontinuities) {
    bool ret = true;
    uint32 numberOfSamplesToCopy = 0u;
    uint32 numberOfSamplesCopied = 0u;
    float64 tBeginningDiscontinuity = 0.0;
    float64 tEndDiscontinuity = 0.0;
    uint32 futureSegment = 0;
    if (minSegment > 0) {
        futureSegment = minSegment - 1;
    }
    else {
    }

    FindDisconinuity(nodeNumber, futureSegment, tBeginningDiscontinuity, tEndDiscontinuity);
    float64 auxTime = tBeginningDiscontinuity + samplingTime[nodeNumber];
    numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime);
    if (holeManagement[nodeNumber] == 0u) {
        MemoryOperationsHelper::Set(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber]]), 0, byteSizeSignals[nodeNumber]);
    }
    else {
        CopyTheSameValue(nodeNumber, numberOfElements[nodeNumber], numberOfSamplesCopied);
    }
    numberOfSamplesCopied += numberOfSamplesToCopy;
    currentTime += numberOfSamplesToCopy * samplingTime[nodeNumber];
    ret = CopyDataAddValues(nodeNumber, futureSegment, futureSegment, numberOfDiscontinuities, numberOfElements[nodeNumber] - numberOfSamplesCopied,
                            numberOfSamplesCopied);
    return ret;
}

bool MDSReader::CopyDataAddValuesCopyData(uint32 nodeNumber,
                                          uint32 minSegment,
                                          uint32 numberOfDiscontinuities) {
    bool ret = true;
    uint32 numberOfSamplesToCopy = 0u;
    uint32 numberOfSamplesCopied = 0u;
    uint32 numberOfSamplesActuallyCopied = 0u;
    float64 tBeginningDiscontinuity = 0.0;
    float64 tEndDiscontinuity = 0.0;
    uint32 futureSegment = minSegment;
    for (uint32 i = 0u; (i < numberOfDiscontinuities) && ret; i++) {
        FindDisconinuity(nodeNumber, futureSegment, tBeginningDiscontinuity, tEndDiscontinuity);
        float64 auxTime = tBeginningDiscontinuity + samplingTime[nodeNumber];
        numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime);
        if (dataManagement[nodeNumber] == 0) {
            numberOfSamplesActuallyCopied += MakeRawCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
            currentTime += numberOfSamplesActuallyCopied * samplingTime[nodeNumber];
        }
        else if (dataManagement[nodeNumber] == 1) {
            numberOfSamplesActuallyCopied += LinearInterpolationCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        else {
            numberOfSamplesActuallyCopied += HoldCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        if (numberOfSamplesActuallyCopied != numberOfSamplesToCopy) {
            ret = false;

        }
        numberOfSamplesCopied += numberOfSamplesActuallyCopied;
        if (ret) {
            numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, tEndDiscontinuity);
            if (holeManagement[nodeNumber] == 0u) {
                uint32 numberOfBytes = numberOfSamplesToCopy * bytesType[nodeNumber];
                uint32 extraOffset = numberOfSamplesCopied * bytesType[nodeNumber];
                MemoryOperationsHelper::Set(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]), 0, numberOfBytes);
                numberOfSamplesCopied += numberOfSamplesToCopy;
            }
            else {
                CopyTheSameValue(nodeNumber, numberOfSamplesToCopy, numberOfSamplesCopied);
                numberOfSamplesCopied += numberOfSamplesToCopy;
            }
            currentTime += numberOfSamplesToCopy * samplingTime[nodeNumber];
        }
        minSegment = futureSegment;
    }
    if (ret) {
        numberOfSamplesToCopy = numberOfElements[nodeNumber] - numberOfSamplesCopied;
        if (dataManagement[nodeNumber] == 0) {
            numberOfSamplesActuallyCopied += MakeRawCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        else if (dataManagement[nodeNumber] == 1) {
            numberOfSamplesActuallyCopied += LinearInterpolationCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        else {
            numberOfSamplesActuallyCopied += HoldCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        if (numberOfSamplesActuallyCopied == numberOfSamplesToCopy) {
            numberOfSamplesCopied += numberOfSamplesActuallyCopied;
        }
        else {
            ret = false;
        }
    }
    return ret;
}

bool MDSReader::FindDisconinuity(uint32 nodeNumber,
                                 uint32 &segment,
                                 float64 &beginningTime,
                                 float64 &endTime) {
    bool find = false;

    MDSplus::Data *tminD = NULL_PTR(MDSplus::Data *);
    MDSplus::Data *tmaxD = NULL_PTR(MDSplus::Data *);

    float64 tmin = 0.0;
    float64 tmax = 0.0;

    float64 nodeTimeDiff;
    GetNodeSamplingTime(nodeNumber, nodeTimeDiff);
    nodes[nodeNumber]->getSegmentLimits(segment, &tminD, &tmaxD);
    tmin = tminD->getDouble();
//Playing with tolerances
    float64 auxDiff = tmin - currentTime;
    if (auxDiff > 0.00000001) {                        //tolerance is 1/100MHz
        endTime = tmin;
        find = true;
    }
    else {
        tmax = tmaxD->getDouble();

        for (int32 currentSegment = static_cast<int32>(segment + 1u); (currentSegment < maxNumberOfSegments[nodeNumber]) && !find; currentSegment++) {
            nodes[nodeNumber]->getSegmentLimits(currentSegment, &tminD, &tmaxD);
            tmin = tminD->getDouble();
            if ((tmin - tmax) > nodeTimeDiff * 1.5) {
                beginningTime = tmax;
                endTime = tmin;
                find = true;
            }
            tmax = tmaxD->getDouble();
            segment = currentSegment;
        }
    }
    MDSplus::deleteData(tminD);
    MDSplus::deleteData(tmaxD);
    return find;

}
uint32 MDSReader::MakeRawCopy(uint32 nodeNumber,
                              uint32 minSeg,
                              uint32 SamplesToCopy,
                              uint32 OffsetSamples) {

    uint32 samplesCopied = 0;
    if (type[nodeNumber] == UnsignedInteger8Bit) {
        samplesCopied = MakeRawCopyTemplate<uint8>(nodeNumber, minSeg, SamplesToCopy, OffsetSamples);
    }
    else if (type[nodeNumber] == SignedInteger8Bit) {
        samplesCopied = MakeRawCopyTemplate<int8>(nodeNumber, minSeg, SamplesToCopy, OffsetSamples);
    }
    else if (type[nodeNumber] == UnsignedInteger16Bit) {
        samplesCopied = MakeRawCopyTemplate<uint16>(nodeNumber, minSeg, SamplesToCopy, OffsetSamples);
    }
    else if (type[nodeNumber] == SignedInteger16Bit) {
        samplesCopied = MakeRawCopyTemplate<int16>(nodeNumber, minSeg, SamplesToCopy, OffsetSamples);
    }
    else if (type[nodeNumber] == UnsignedInteger32Bit) {
        samplesCopied = MakeRawCopyTemplate<uint32>(nodeNumber, minSeg, SamplesToCopy, OffsetSamples);
    }
    else if (type[nodeNumber] == SignedInteger32Bit) {
        samplesCopied = MakeRawCopyTemplate<int32>(nodeNumber, minSeg, SamplesToCopy, OffsetSamples);
    }
    else if (type[nodeNumber] == UnsignedInteger64Bit) {
        samplesCopied = MakeRawCopyTemplate<uint64>(nodeNumber, minSeg, SamplesToCopy, OffsetSamples);
    }
    else if (type[nodeNumber] == SignedInteger64Bit) {
        samplesCopied = MakeRawCopyTemplate<int64>(nodeNumber, minSeg, SamplesToCopy, OffsetSamples);
    }
    else if (type[nodeNumber] == Float32Bit) {
        samplesCopied = MakeRawCopyTemplate<float32>(nodeNumber, minSeg, SamplesToCopy, OffsetSamples);
    }
    else if (type[nodeNumber] == Float64Bit) {
        samplesCopied = MakeRawCopyTemplate<float64>(nodeNumber, minSeg, SamplesToCopy, OffsetSamples);
    }
    return samplesCopied;
}

template<typename T>
uint32 MDSReader::MakeRawCopyTemplate(uint32 nodeNumber,
                                      uint32 minSeg,
                                      uint32 SamplesToCopy,
                                      uint32 OffsetSamples) {

    MDSplus::Data *dataD = NULL_PTR(MDSplus::Data *);
    int32 nElements = 0u;
    uint32 bytesToCopy = 0u;
    uint32 extraOffset = OffsetSamples * bytesType[nodeNumber];
    bool endSegment = false;
    uint32 samplesCopied = 0;
    uint32 remainingSamplesOnTheSegment = 0u;
    T* data = NULL_PTR(T *);
    for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (SamplesToCopy != 0); currentSegment++) {
        dataD = nodes[nodeNumber]->getSegment(currentSegment);
        if (type[nodeNumber] == UnsignedInteger8Bit) {
            data = reinterpret_cast<T *>(dataD->getByteUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger8Bit) {
            data = reinterpret_cast<T *>(dataD->getByteArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger16Bit) {
            data = reinterpret_cast<T *>(dataD->getShortUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger16Bit) {
            data = reinterpret_cast<T *>(dataD->getShortArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger32Bit) {
            data = reinterpret_cast<T *>(dataD->getIntUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger32Bit) {
            data = reinterpret_cast<T *>(dataD->getIntArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger64Bit) {
            data = reinterpret_cast<T *>(dataD->getLongUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger64Bit) {
            data = reinterpret_cast<T *>(dataD->getLongArray(&nElements));
        }
        else if (type[nodeNumber] == Float32Bit) {
            data = reinterpret_cast<T *>(dataD->getFloatArray(&nElements));
        }
        else if (type[nodeNumber] == Float64Bit) {
            data = reinterpret_cast<T *>(dataD->getDoubleArray(&nElements));
        }

        remainingSamplesOnTheSegment = static_cast<uint32>(nElements) - elementsConsumed[nodeNumber];
        endSegment = remainingSamplesOnTheSegment <= SamplesToCopy;
        if (!endSegment) {        // no end of segment but no more data need to be copied
            bytesToCopy = SamplesToCopy * bytesType[nodeNumber];
            samplesCopied += SamplesToCopy;
        }
        else {        // end segment but still more data must be copied
            bytesToCopy = remainingSamplesOnTheSegment * bytesType[nodeNumber];
            samplesCopied += remainingSamplesOnTheSegment;

            //
        }
        MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]),
                                     reinterpret_cast<void *>(&data[elementsConsumed[nodeNumber]]), bytesToCopy);
        extraOffset += bytesToCopy;

//Update values
        if (!endSegment) {        // no end of segment but no more data need to be copied
            elementsConsumed[nodeNumber] += SamplesToCopy;
            SamplesToCopy = 0u;
        }
        else {        // end segment but still more data must be copied
            SamplesToCopy -= ((static_cast<uint32>(nElements)) - elementsConsumed[nodeNumber]);
            elementsConsumed[nodeNumber] = 0u;
        }
        *reinterpret_cast<T *>(&lastValue[offsetLastValue[nodeNumber]]) = data[nElements - 1];
        MDSplus::deleteData(dataD);
        delete data;

    }
    return samplesCopied;
}

uint32 MDSReader::LinearInterpolationCopy(uint32 nodeNumber,
                                          uint32 minSeg,
                                          uint32 samplesToCopy,
                                          uint32 offsetSamples) {

    uint32 samplesCopied = 0;

    if (type[nodeNumber] == UnsignedInteger8Bit) {
        samplesCopied = LinearInterpolationCopyTemplate<uint8>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == SignedInteger8Bit) {
        samplesCopied = LinearInterpolationCopyTemplate<int8>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == UnsignedInteger16Bit) {
        samplesCopied = LinearInterpolationCopyTemplate<uint16>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == SignedInteger16Bit) {
        samplesCopied = LinearInterpolationCopyTemplate<int16>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == UnsignedInteger32Bit) {
        samplesCopied = LinearInterpolationCopyTemplate<uint32>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == SignedInteger32Bit) {
        samplesCopied = LinearInterpolationCopyTemplate<int32>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == UnsignedInteger64Bit) {
        samplesCopied = LinearInterpolationCopyTemplate<uint64>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == SignedInteger64Bit) {
        samplesCopied = LinearInterpolationCopyTemplate<int64>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == Float32Bit) {
        samplesCopied = LinearInterpolationCopyTemplate<float32>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == Float64Bit) {
        samplesCopied = LinearInterpolationCopyTemplate<float64>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    return samplesCopied;
}

template<typename T>
uint32 MDSReader::LinearInterpolationCopyTemplate(uint32 nodeNumber,
                                                  uint32 minSeg,
                                                  uint32 samplesToCopy,
                                                  uint32 offsetSamples) {

    MDSplus::Array *dataD = NULL_PTR(MDSplus::Array *);
    MDSplus::Data *timeNodeD = NULL_PTR(MDSplus::Data *);
    float64 *timeNode = NULL_PTR(float64 *);
    int32 nElements = 0u;
    uint32 extraOffset = offsetSamples * bytesType[nodeNumber];
    bool endSegment = false;
    uint32 samplesCopied = 0;
    uint32 iterations = 0u;
    uint32 remainingSamplesOnTheSegment = 0u;

    T* data = NULL_PTR(T *);
    for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (samplesToCopy != 0); currentSegment++) {
        nodes[nodeNumber]->getSegmentAndDimension(currentSegment, dataD, timeNodeD);
        if (type[nodeNumber] == UnsignedInteger8Bit) {
            data = reinterpret_cast<T *>(dataD->getByteUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger8Bit) {
            data = reinterpret_cast<T *>(dataD->getByteArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger16Bit) {
            data = reinterpret_cast<T *>(dataD->getShortUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger16Bit) {
            data = reinterpret_cast<T *>(dataD->getShortArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger32Bit) {
            data = reinterpret_cast<T *>(dataD->getIntUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger32Bit) {
            data = reinterpret_cast<T *>(dataD->getIntArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger64Bit) {
            data = reinterpret_cast<T *>(dataD->getLongUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger64Bit) {
            data = reinterpret_cast<T *>(dataD->getLongArray(&nElements));
        }
        else if (type[nodeNumber] == Float32Bit) {
            data = reinterpret_cast<T *>(dataD->getFloatArray(&nElements));
        }
        else if (type[nodeNumber] == Float64Bit) {
            data = reinterpret_cast<T *>(dataD->getDoubleArray(&nElements));
        }
        timeNode = timeNodeD->getDoubleArray(&nElements);
        float64 auxTime = timeNode[nElements - 1] + samplingTime[nodeNumber];
        remainingSamplesOnTheSegment = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime); //static_cast<uint32>(1 + ((timeNode[nElements - 1] - currentTime) / samplingTime[nodeNumber]));
        endSegment = (remainingSamplesOnTheSegment <= samplesToCopy);
        if (!endSegment) {        //no end of segment but no more data need to be copied
            samplesCopied += samplesToCopy;
            iterations = samplesToCopy;
        }
        else {        // end segment but still more data must be copied
            samplesCopied += remainingSamplesOnTheSegment;
            iterations = remainingSamplesOnTheSegment;
        }
        float64 outputInterpolation = 0.0;
        for (uint32 i = 0u; i < iterations; i++) {
            while ((currentTime >= timeNode[elementsConsumed[nodeNumber]]) && (elementsConsumed[nodeNumber] < static_cast<uint32>(nElements - 1))) {
                elementsConsumed[nodeNumber]++;
            }
            if (elementsConsumed[nodeNumber] == 0u) {
                SampleInterpolation<T>(currentTime, *reinterpret_cast<T *>(&lastValue[offsetLastValue[nodeNumber]]), data[elementsConsumed[nodeNumber]],
                                       lastTime[nodeNumber], timeNode[elementsConsumed[nodeNumber]], &outputInterpolation);

            }
            else {
                SampleInterpolation<T>(currentTime, data[elementsConsumed[nodeNumber] - 1], data[elementsConsumed[nodeNumber]],
                                       timeNode[elementsConsumed[nodeNumber] - 1], timeNode[elementsConsumed[nodeNumber]], &outputInterpolation);
            }
            if ((type[nodeNumber] == Float32Bit) || (type[nodeNumber] == Float64Bit)) {
                *reinterpret_cast<T *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]) = static_cast<T>(outputInterpolation);
            }
            else {
                *reinterpret_cast<T *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]) = static_cast<T>(round(outputInterpolation));
            }
            extraOffset += bytesType[nodeNumber];
            currentTime += samplingTime[nodeNumber];
            samplesToCopy--;
        }
        *reinterpret_cast<T *>(&(lastValue[offsetLastValue[nodeNumber]])) = data[nElements - 1];
        lastTime[nodeNumber] = timeNode[nElements - 1];
        if (endSegment) {
            elementsConsumed[nodeNumber] = 0u;
        }
        MDSplus::deleteData(dataD);
        MDSplus::deleteData(timeNodeD);
        delete data;
        delete timeNode;
    }
    return samplesCopied;

}

uint32 MDSReader::HoldCopy(uint32 nodeNumber,
                           uint32 minSeg,
                           uint32 samplesToCopy,
                           uint32 samplesOffset) {

    uint32 samplesCopied = 0;
    if (type[nodeNumber] == UnsignedInteger8Bit) {
        samplesCopied = HoldCopyTemplate<uint8>(nodeNumber, minSeg, samplesToCopy, samplesOffset);
    }
    else if (type[nodeNumber] == SignedInteger8Bit) {
        samplesCopied = HoldCopyTemplate<int8>(nodeNumber, minSeg, samplesToCopy, samplesOffset);
    }
    else if (type[nodeNumber] == UnsignedInteger16Bit) {
        samplesCopied = HoldCopyTemplate<uint16>(nodeNumber, minSeg, samplesToCopy, samplesOffset);
    }
    else if (type[nodeNumber] == SignedInteger16Bit) {
        samplesCopied = HoldCopyTemplate<int16>(nodeNumber, minSeg, samplesToCopy, samplesOffset);
    }
    else if (type[nodeNumber] == UnsignedInteger32Bit) {
        samplesCopied = HoldCopyTemplate<uint32>(nodeNumber, minSeg, samplesToCopy, samplesOffset);
    }
    else if (type[nodeNumber] == SignedInteger32Bit) {
        samplesCopied = HoldCopyTemplate<int32>(nodeNumber, minSeg, samplesToCopy, samplesOffset);
    }
    else if (type[nodeNumber] == UnsignedInteger64Bit) {
        samplesCopied = HoldCopyTemplate<uint64>(nodeNumber, minSeg, samplesToCopy, samplesOffset);
    }
    else if (type[nodeNumber] == SignedInteger64Bit) {
        samplesCopied = HoldCopyTemplate<int64>(nodeNumber, minSeg, samplesToCopy, samplesOffset);
    }
    else if (type[nodeNumber] == Float32Bit) {
        samplesCopied = HoldCopyTemplate<float32>(nodeNumber, minSeg, samplesToCopy, samplesOffset);
    }
    else if (type[nodeNumber] == Float64Bit) {
        samplesCopied = HoldCopyTemplate<float64>(nodeNumber, minSeg, samplesToCopy, samplesOffset);
    }
    return samplesCopied;
}

template<typename T>
uint32 MDSReader::HoldCopyTemplate(uint32 nodeNumber,
                                   uint32 minSeg,
                                   uint32 samplesToCopy,
                                   uint32 samplesOffset) {

    MDSplus::Array *dataD = NULL_PTR(MDSplus::Array *);
    MDSplus::Data *timeNodeD = NULL_PTR(MDSplus::Data *);
    float64 *timeNode = NULL_PTR(float64 *);
    int32 nElements = 0u;
    uint32 extraOffset = samplesOffset * bytesType[nodeNumber];
    bool endSegment = false;
    uint32 samplesCopied = 0;
    uint32 iterations = 0u;
    uint32 remainingSamplesOnTheSegment = 0u;

    T* data = NULL_PTR(T *);
    for (uint32 currentSegment = minSeg; (currentSegment < maxNumberOfSegments[nodeNumber]) && (samplesToCopy != 0); currentSegment++) {
        nodes[nodeNumber]->getSegmentAndDimension(currentSegment, dataD, timeNodeD);
        if (type[nodeNumber] == UnsignedInteger8Bit) {
            data = reinterpret_cast<T *>(dataD->getByteUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger8Bit) {
            data = reinterpret_cast<T *>(dataD->getByteArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger16Bit) {
            data = reinterpret_cast<T *>(dataD->getShortUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger16Bit) {
            data = reinterpret_cast<T *>(dataD->getShortArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger32Bit) {
            data = reinterpret_cast<T *>(dataD->getIntUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger32Bit) {
            data = reinterpret_cast<T *>(dataD->getIntArray(&nElements));
        }
        else if (type[nodeNumber] == UnsignedInteger64Bit) {
            data = reinterpret_cast<T *>(dataD->getLongUnsignedArray(&nElements));
        }
        else if (type[nodeNumber] == SignedInteger64Bit) {
            data = reinterpret_cast<T *>(dataD->getLongArray(&nElements));
        }
        else if (type[nodeNumber] == Float32Bit) {
            data = reinterpret_cast<T *>(dataD->getFloatArray(&nElements));
        }
        else if (type[nodeNumber] == Float64Bit) {
            data = reinterpret_cast<T *>(dataD->getDoubleArray(&nElements));
        }
        timeNode = timeNodeD->getDoubleArray(&nElements);
        T auxTime = timeNode[nElements - 1] + samplingTime[nodeNumber];
        remainingSamplesOnTheSegment = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime); //static_cast<uint32>(1 + ((timeNode[nElements - 1] - currentTime) / samplingTime[nodeNumber]));
        endSegment = (remainingSamplesOnTheSegment <= samplesToCopy);
        if (!endSegment) {        //no end of segment but no more data need to be copied
            samplesCopied += samplesToCopy;
            iterations = samplesToCopy;
        }
        else {        // end segment but still more data must be copied
            samplesCopied += remainingSamplesOnTheSegment;
            iterations = remainingSamplesOnTheSegment;
        }
        for (uint32 i = 0u; i < iterations; i++) {
            while ((currentTime >= timeNode[elementsConsumed[nodeNumber]]) && (elementsConsumed[nodeNumber] < static_cast<uint32>(nElements - 1))) {
                elementsConsumed[nodeNumber]++;
            }
            if (elementsConsumed[nodeNumber] == 0u) {
                float64 diff1 = currentTime - lastTime[nodeNumber];
                float64 diff2 = timeNode[0] - currentTime;
                if (diff1 < diff2) {
                    *reinterpret_cast<T *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]) =
                            *reinterpret_cast<T *>(&lastValue[offsetLastValue[nodeNumber]]);
                }
                else {
                    *reinterpret_cast<T *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]) = data[0];
                }
            }
            else {
                float64 diff1 = currentTime - timeNode[elementsConsumed[nodeNumber] - 1];
                float64 diff2 = timeNode[elementsConsumed[nodeNumber]] - currentTime;
                if (diff1 < diff2) {
                    *reinterpret_cast<T *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]) = data[elementsConsumed[nodeNumber] - 1];
                }
                else {
                    *reinterpret_cast<T *>(&dataSourceMemory[offsets[nodeNumber] + extraOffset]) = data[elementsConsumed[nodeNumber]];
                }
            }
            extraOffset += bytesType[nodeNumber];
            currentTime += samplingTime[nodeNumber];
            samplesToCopy--;
        }
        *reinterpret_cast<T *>(&lastValue[offsetLastValue[nodeNumber]]) = data[nElements - 1];
        lastTime[nodeNumber] = timeNode[nElements - 1];
        if (endSegment) {
            elementsConsumed[nodeNumber] = 0u;
        }
        MDSplus::deleteData(dataD);
        MDSplus::deleteData(timeNodeD);
        delete data;
        delete timeNode;
    }
    return samplesCopied;
}

template<typename T>
bool MDSReader::SampleInterpolation(float64 ct,
                                    T data1,
                                    T data2,
                                    float64 t1,
                                    float64 t2,
                                    float64 * ptr) {
    bool ret = t2 > t1;
    if (ret) {
        float64 slope = (data2 - data1) / (t2 - t1);
        *ptr = slope * (ct - t1) + data1;
    }
    return ret;
}
bool MDSReader::CopyRemainingData(uint32 nodeNumber,
                                  uint32 minSegment) {
    bool ret = false;
    uint32 samplesCopied;
    if (dataManagement[nodeNumber] == 0) {
        samplesCopied = MakeRawCopy(nodeNumber, minSegment, numberOfElements[nodeNumber], 0);
        ret = (samplesCopied <= numberOfElements[nodeNumber]);
    }
    else if (dataManagement[nodeNumber] == 0) {
        samplesCopied = LinearInterpolationCopy(nodeNumber, minSegment, numberOfElements[nodeNumber], 0);
        ret = (samplesCopied <= numberOfElements[nodeNumber]);
    }
    else {
        samplesCopied = HoldCopy(nodeNumber, minSegment, numberOfElements[nodeNumber], 0);
        ret = (samplesCopied <= numberOfElements[nodeNumber]);
    }
    if(samplesCopied > 0u){
        uint32 bytesToCopy = (numberOfElements[nodeNumber] - samplesCopied) * bytesType[nodeNumber];
        MemoryOperationsHelper::Set(reinterpret_cast<void *>(&(dataSourceMemory[offsets[nodeNumber]])), 0, bytesToCopy);
    }
    return ret;
}

CLASS_REGISTER(MDSReader, "1.0")
}

