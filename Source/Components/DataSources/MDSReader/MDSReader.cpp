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

/*lint -estring(1960, "*MDSplus::*") -estring(1960, "*std::*") Ignore errors that do not belong to this DataSource namespace*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MDSReader::MDSReader() :
        DataSourceI() {
    tree = NULL_PTR(MDSplus::Tree *);
    nodeName = NULL_PTR(StreamString *);
    nodes = NULL_PTR(MDSplus::TreeNode **);
    numberOfNodeNames = 0u;
    nOfInputSignals = 0u;
    nOfInputSignalsPerFunction = 0u;
    mdsNodeTypes = NULL_PTR(StreamString *);
    byteSizeSignals = NULL_PTR(uint32 *);
    shotNumber = 0;
    type = NULL_PTR(TypeDescriptor *);
    bytesType = NULL_PTR(uint32 *);
    numberOfElements = NULL_PTR(uint32 *);
    dataSourceMemory = NULL_PTR(char8 *);
    offsets = NULL_PTR(uint32 *);
    timeCycle = 0.0;
    currentTime = timeCycle;
    frequency = 0.0;
    period = 0.0;
    maxNumberOfSegments = NULL_PTR(uint32 *);
    lastSegment = NULL_PTR(uint32 *);
    dataManagement = NULL_PTR(uint8 *);
    holeManagement = NULL_PTR(uint8 *);
    samplingTime = NULL_PTR(float64 *);
    lastValue = NULL_PTR(char8 *);
    lastTime = NULL_PTR(float64 *);
    offsetLastValue = NULL_PTR(uint32 *);
    elementsConsumed = NULL_PTR(uint32 *);
    endNode = NULL_PTR(bool *);
    nodeSamplingTime = NULL_PTR(float64 *);
}

/*lint -e{1551} the destructor must guarantee that the MDSplus are deleted and the shared memory freed*/
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
    if (maxNumberOfSegments != NULL_PTR(uint32 *)) {
        delete[] maxNumberOfSegments;
        maxNumberOfSegments = NULL_PTR(uint32 *);
    }
    if (lastSegment != NULL_PTR(uint32 *)) {
        delete[] lastSegment;
        lastSegment = NULL_PTR(uint32 *);
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
    if (numberOfElements != NULL_PTR(uint32 *)) {
        delete[] numberOfElements;
        numberOfElements = NULL_PTR(uint32 *);
    }
    if (bytesType != NULL_PTR(uint32 *)) {
        delete[] bytesType;
        bytesType = NULL_PTR(uint32 *);
    }
    if (nodeSamplingTime != NULL_PTR(float64 *)) {
        delete[] nodeSamplingTime;
        nodeSamplingTime = NULL_PTR(float64 *);
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
        }
        else {
            ok = (shotNumber >= 0);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "ShotNumber shall be 0 (last available shot) or positive");
            }
        }
    }
    if (ok) {
        ok = OpenTree();
    }
    if (ok) { //readFrequency
        ok = data.Read("Frequency", frequency);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read frequency");
        }
        else {
            period = 1.0 / frequency;
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
        ok = data.MoveToAncestor(1u);
    }
    return ok;
}

bool MDSReader::SetConfiguredDatabase(StructuredDataI & data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSourceI::SetConfiguredDatabase(data) returned false");
    }
    if (ok) {
        ok = data.MoveRelative("Signals");
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
        ok = GetFunctionNumberOfSignals(InputSignals, 0u, nOfInputSignalsPerFunction);        //0u (second argument) because previously it is checked
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetFunctionNumberOfSignals() returned false");
        }
        if (ok) {
            ok = (nOfInputSignalsPerFunction > 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The number of signals must be at least 2 (1 node and one time)");
            }
        }
    }
    if (ok) {
        nOfInputSignals = GetNumberOfSignals();
        ok = (nOfInputSignals == nOfInputSignalsPerFunction);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "nOfInputSignals must be equal than number of signals per function (since only one function can be connected to this data source",
                         GetNumberOfSignals(), numberOfNodeNames);
        }
    }
    if (ok) {
        numberOfNodeNames = nOfInputSignals - 1u;
    }
    if (ok) {        //allocate memory for lastSegment
        lastSegment = new uint32[numberOfNodeNames];
        for (uint32 i = 0u; i < numberOfNodeNames; i++) {
            lastSegment[i] = 0u;
        }
    }
    if (ok) {
        for (uint32 n = 0u; (n < nOfInputSignals) && ok; n++) {
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
                //lint -e{613} Possible use of null pointer. Variable protected by ok bolean.
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
        type = new TypeDescriptor[nOfInputSignals];
        //lint -e{613} Possible use of null pointer. type previously allocated (see previous line).
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
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
    if (ok) { //read the type of the time. It should be uin64
        //lint -e{662} [MISRA C++ 5-0-16] Possible access out-of-bounds pointer. nOfInputSignals is always 1 unit larger than numberOfNodeNames.
        //lint -e{661} [MISRA C++ 5-0-16] Possible access out-of-bounds pointer. nOfInputSignals is always 1 unit larger than numberOfNodeNames.
        if (type != NULL_PTR(TypeDescriptor *)) {
            type[numberOfNodeNames] = GetSignalType(numberOfNodeNames);
            bool cond1 = (type[numberOfNodeNames] == UnsignedInteger64Bit);
            bool cond2 = (type[numberOfNodeNames] == UnsignedInteger32Bit);
            bool cond3 = (type[numberOfNodeNames] == SignedInteger32Bit);
            bool cond4 = (type[numberOfNodeNames] == SignedInteger64Bit);
            ok = cond1 || cond2 || cond3 || cond4;
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported timeCycle type. Possible timeCycle types are: uint64, int64, uin32 or int32\n");
            }
        }
        else {
            ok = false;
        }
    }

    if (ok) { //Compute the type byte size
        bytesType = new uint32[numberOfNodeNames];
        if ((bytesType != NULL_PTR(uint32 *)) && (type != NULL_PTR(TypeDescriptor *))) {
            for (uint32 i = 0u; i < numberOfNodeNames; i++) {
                bytesType[i] = static_cast<uint32>(type[i].numberOfBits) / 8u;
            }
        }
        else {
            ok = false;
        }
    }

    if (ok) { //read number of elements
        numberOfElements = new uint32[nOfInputSignals];
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
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
        if (ok) {
            //lint -e{661} [MISRA C++ 5-0-16] Possible access out-of-bounds. nOfInputSignals is always 1 unit larger than numberOfNodeNames.
            ok = GetSignalNumberOfElements(numberOfNodeNames, numberOfElements[numberOfNodeNames]);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read NumberOfElements");
            }
        }
        if (ok) {
            //lint -e{661} [MISRA C++ 5-0-16] Possible access out-of-bounds. nOfInputSignals is always 1 unit larger than numberOfNodeNames.
            ok = numberOfElements[numberOfNodeNames] == 1u;
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfElements for the timeCycle must be 1");
            }
        }
    }
    if (ok) { //calculate samplingTime
        samplingTime = new float64[numberOfNodeNames];
        if ((samplingTime != NULL_PTR(float64 *)) && (numberOfElements != NULL_PTR(uint32 *))) {
            for (uint32 i = 0u; i < numberOfNodeNames; i++) {
                samplingTime[i] = (1.0 / (frequency)) / static_cast<float64>(numberOfElements[i]);
            }
        }
        else {
            ok = false;
        }
    }
    //lint -e{661} [MISRA C++ 5-0-16] Possible access out-of-bounds. nOfInputSignals is always 1 unit larger than numberOfNodeNames.
    if (ok) { //Count and allocate memory for dataSourceMemory, lastValue and lastTime
        offsets = new uint32[nOfInputSignals];
        byteSizeSignals = new uint32[nOfInputSignals];
        offsetLastValue = new uint32[numberOfNodeNames];
        lastTime = new float64[numberOfNodeNames];
        //Count the number of bytes
        uint32 totalSignalMemory = 0u;
        uint32 sumLastValueMemory = 0u;
        if (type != NULL_PTR(TypeDescriptor *)) {
            if ((offsets != NULL_PTR(uint32 *)) && (byteSizeSignals != NULL_PTR(uint32 *))) {
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
                    sumLastValueMemory += static_cast<uint32>(type[i].numberOfBits) / 8u;
                }
                if (ok) { // count the time as well
                    offsets[numberOfNodeNames] = totalSignalMemory;
                    uint32 nBytes = 0u;
                    ok = GetSignalByteSize(numberOfNodeNames, nBytes);
                    byteSizeSignals[numberOfNodeNames] = nBytes;
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Error while GetSignalByteSize() for signal %u", numberOfNodeNames);
                    }
                    totalSignalMemory += nBytes;

                }
            }
            else {
                ok = false;
            }
        }
        else {
            ok = false;
        }

        //Allocate memory
        if (ok) {
            dataSourceMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalSignalMemory));
            lastValue = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(sumLastValueMemory));
        }
        if (ok) { //initialise lastValues and lastTimes
            ok = MemoryOperationsHelper::Set(reinterpret_cast<void *>(lastValue), static_cast<char8>(0), sumLastValueMemory);
            if ((lastTime != NULL_PTR(float64 *)) && (samplingTime != NULL_PTR(float64 *))) {
                for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
                    lastTime[i] = -samplingTime[i];
                }
            }
        }
    }
    if (ok) {
        maxNumberOfSegments = new uint32[numberOfNodeNames];
        if (nodes != NULL_PTR(MDSplus::TreeNode **)) {
            for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
                if (nodes[i] != NULL_PTR(MDSplus::TreeNode *)) {
                    int32 auxSeg = nodes[i]->getNumSegments();
                    if (auxSeg < 0) {
                        ok = false;
                    }
                    else {
                        maxNumberOfSegments[i] = static_cast<uint32>(auxSeg);
                    }
                }
                else {
                    ok = false;
                }
            }
        }
        else {
            ok = false;
        }
    }
    if (ok) { //read DataManagement from originalSignalInformation
        dataManagement = new uint8[numberOfNodeNames];
        nodeSamplingTime = new float64[numberOfNodeNames];
        //lint -e{613} Possible use of null pointer. The pointer usage is protected by the ok variable.
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
                ok = (dataManagement[i] < 3u);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(
                            ErrorManagement::ParametersError,
                            "Invalid DataManagement value. It could be 0 (nothing), 1 (linear interpolation), 2 (hold last value). dataManagement[%d] = %d",
                            auxIdx, dataManagement[auxIdx]);
                }
            }
            if (ok) {
                ok = GetNodeSamplingTime(i, nodeSamplingTime[i]);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Error getting node sample time for nodeName = %s", nodeName[i].Buffer());
                }
            }
            if (ok) { //check time of doing nothing option
                if (dataManagement[i] == 0u) {
//                    float64 nodeSamplingTime = 0.0;
                    ok = IsEqual(samplingTime[i], nodeSamplingTime[i]);
                    if (!ok) {
                        REPORT_ERROR(
                                ErrorManagement::ParametersError,
                                "the sampling time of the node %s = %.9f is different than the sampling time calculated from the parameters = %.9f and the dataManagement = 0 (do nothing)",
                                nodeName[i].Buffer(), nodeSamplingTime[i], samplingTime[i]);
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
                ok = (holeManagement[i] < 2u);
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
    bool ok;
    for (uint32 i = 0u; i < numberOfNodeNames; i++) {
        currentTime = timeCycle;
        endNode[i] = !GetDataNode(i);
    }
    PublishTime();
    ok = !AllNodesEnd();
    timeCycle += period;
    return ok;
}

//lint -e{613} Possible use of null pointer. Not Possible. If initialise fails this function is not called.
//lint -e{661} Possible access of out-of-bounds pointer. Initialisation garanties that all pointers are correctly initialised.
//lint -e{662} Possible creation of out-of-bounds pointer (4294967294 beyond end of data) by operator. Itialisation garanties that all pointers are correctly initialised.
//lint -e{927} cast from pointer to pointer [MISRA C++ Rule 5-2-7]. A pointer cast must be done...
//lint -e{826} Suspicious pointer-to-pointer conversion. char8 * to uint64 * convertion must be done...
void MDSReader::PublishTime() {
    float64 auxFloat = timeCycle * static_cast<float64>(1000000);
    if (type[numberOfNodeNames] == UnsignedInteger32Bit) {
        *reinterpret_cast<uint32 *>(&dataSourceMemory[offsets[numberOfNodeNames]]) = static_cast<uint32>(auxFloat);
    }
    else if (type[numberOfNodeNames] == SignedInteger32Bit) {
        //int32 auxInt = static_cast<int32>(auxFloat);
        //MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&dataSourceMemory[offsets[numberOfNodeNames]]), static_cast<void *>(&auxInt), 4u);
        *reinterpret_cast<int32 *>(&dataSourceMemory[offsets[numberOfNodeNames]]) = static_cast<int32>(auxFloat);
    }
    else if (type[numberOfNodeNames] == UnsignedInteger64Bit) {
        uint32 auxIdx = offsets[numberOfNodeNames];
        uint64 *ptr = reinterpret_cast<uint64 *>(&(dataSourceMemory[auxIdx]));
        *ptr = static_cast<uint64>(auxFloat);
    }
    else if (type[numberOfNodeNames] == SignedInteger64Bit) {
        uint32 auxIdx = offsets[numberOfNodeNames];
        char8 *ptr2 = &dataSourceMemory[auxIdx];
        int64 *ptr = reinterpret_cast<int64 *>(ptr2);
        *ptr = static_cast<int64>(auxFloat);
    }
    else {

    }
    return;
}

bool MDSReader::AllNodesEnd() const {
    bool ret = true;
    if (endNode != NULL_PTR(bool *)) {
        for (uint32 i = 0u; (i < numberOfNodeNames) && ret; i++) {
            ret = endNode[i];
        }
    }
    return ret;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
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

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
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

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction*/
const char8 *MDSReader::GetBrokerName(StructuredDataI &data,
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
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: InputBrokers are not supported. Function returns false irrespectively of the parameters.*/
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
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Fail opening tree %s with shotNumber %d:  %s", treeName.Buffer(), shotNumber, exc.what());
        ret = false;
    }
    return ret;
}

bool MDSReader::OpenNode(const uint32 idx) {
    bool ret = true;
    if ((nodeName != NULL_PTR(StreamString *)) && (nodes != NULL_PTR(MDSplus::TreeNode **)) && (tree != NULL_PTR(MDSplus::Tree *))) {
        try {
            nodes[idx] = tree->getNode(nodeName[idx].Buffer());
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed opening node %s: %s", nodeName[idx].Buffer(), exc.what());
            ret = false;
        }
    }
    return ret;
}

bool MDSReader::GetTypeNode(const uint32 idx) {
    bool ret = true;
    if (nodes != NULL_PTR(MDSplus::TreeNode **)) {
        if ((nodes[idx] != NULL_PTR(MDSplus::TreeNode *)) && (mdsNodeTypes != NULL_PTR(StreamString *)) && (nodeName != NULL_PTR(StreamString *))) {
            try {
                mdsNodeTypes[idx] = nodes[idx]->getDType();
            }
            catch (const MDSplus::MdsException &exc) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed getting type node from %s: %s", nodeName[idx].Buffer(), exc.what());
                ret = false;
            }
        }
        else {
            ret = false;
        }
    }
    else {
        ret = false;
    }
    return ret;
}

bool MDSReader::IsValidTypeNode(const uint32 idx) const {
    bool ret = true;
    if (mdsNodeTypes != NULL_PTR(StreamString *)) {
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
    }
    else {
        ret = false;
    }
    return ret;
}

bool MDSReader::CheckTypeAgainstMdsNodeTypes(const uint32 idx) const {
    bool ret = true;
    if ((type != NULL_PTR(TypeDescriptor *)) && (mdsNodeTypes != NULL_PTR(StreamString *))) {
        ret = (type[idx] == ConvertMDStypeToMARTeType(mdsNodeTypes[idx]));
    }
    else {
        ret = false;
    }
    return ret;
}

TypeDescriptor MDSReader::ConvertMDStypeToMARTeType(StreamString mdsType) const {
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
    else {

    }
    return str;

}

/**
 * -1--> end data (not found)
 * 0--> is not end of data but not found. Case signal trigger (the segment are not continuous)
 * 1--> time found in a segment
 */

bool MDSReader::GetDataNode(const uint32 nodeNumber) {
    bool ok;
//MDSplus::Array *dataA = NULL_PTR(MDSplus::Array *);
//MDSplus::Data *timeD = NULL_PTR(MDSplus::Data *);
    uint32 minSegment = 0u;
    int8 errorCodeMinSegment;
    uint32 maxSegment = 0u;
    int8 errorCodeMaxSegment = -1;
    errorCodeMinSegment = FindSegment(timeCycle, minSegment, nodeNumber);
    ok = (errorCodeMinSegment != -1);
    uint32 numberOfDiscontinuities = 0u;
    if (ok) {
        errorCodeMaxSegment = FindSegment(timeCycle + period, maxSegment, nodeNumber);
        numberOfDiscontinuities = CheckDiscontinuityOfTheSegments(nodeNumber, minSegment, maxSegment);
    }
    if (ok) {    //Decides how to copy data
// there are 4 options
        if ((errorCodeMinSegment == 0) && (errorCodeMaxSegment == 0)) {
            ok = AddValuesCopyDataAddValues(nodeNumber, minSegment, numberOfDiscontinuities);
        }
        else if ((errorCodeMinSegment == 0) && (errorCodeMaxSegment == 1)) {
            ok = AddValuesCopyData(nodeNumber, minSegment, numberOfDiscontinuities);
        }
        else if ((errorCodeMinSegment == 1) && (errorCodeMaxSegment == 0)) {
            ok = CopyDataAddValues(nodeNumber, minSegment, numberOfDiscontinuities, numberOfElements[nodeNumber], 0u);
        }
        else if ((errorCodeMinSegment == 1) && (errorCodeMaxSegment == 1)) {
            ok = CopyDataAddValuesCopyData(nodeNumber, minSegment, numberOfDiscontinuities);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Error while copying data from the tree to the dynamic memory");
            }
        }
        else if ((errorCodeMinSegment == 1) && (errorCodeMaxSegment == -1)) {
            ok = CopyRemainingData(nodeNumber, minSegment);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Error while copying data from the node = %s to the dynamic memory", nodeName[nodeNumber]);
            }
            //end node
            //ok = false;
        }
        else {
        }
    }
    else {    // add 0. No more data on the node.
        bool error = !MemoryOperationsHelper::Set(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber]]), static_cast<char8>(0),
                                                  byteSizeSignals[nodeNumber]);
        if (error) {
            REPORT_ERROR(ErrorManagement::FatalError, "Error while copying data from the node = %s to the dynamic memory", nodeName[nodeNumber]);
        }
        ok = false; //end data
    }
    return ok;
}

//lint -e{613} Possible use of null pointer. Not possible. If initialise fails this function is not called.
int8 MDSReader::FindSegment(const float64 t,
                            uint32 &segment,
                            const uint32 nodeIdx) {
    bool find = false;
    MDSplus::Data *tminD;
    MDSplus::Data *tmaxD;
    int8 retVal = -1;
    float64 tmax = 0.0;
    float64 tmin = 0.0;
    for (uint32 i = lastSegment[nodeIdx]; (i < maxNumberOfSegments[nodeIdx]) && (!find); i++) {
        nodes[nodeIdx]->getSegmentLimits(static_cast<int32>(i), &tminD, &tmaxD);
        tmax = tmaxD->getDouble();
        if (t <= tmax) {
            find = true;
            tmin = tminD->getDouble();
            if (t < (tmin - (nodeSamplingTime[nodeIdx] / 2.0))) { // (nodeSamplingTime[nodeIdx] / 2.0)) is due to float64 numeric errors
                //look the tmax Previous segment and verify if the difference is smaller than the
                if (i < 2u) {
                    retVal = 0;
                    //It is very important. Even the segment does not exist the index must be updated saying the next segment to be look for is this one.
                    //At the same time it is used in the case that the tmin does not exist but tmax segment exist.
                    segment = i;
                    lastSegment[nodeIdx] = i;
                }
                else {
                    MDSplus::Data *tminPreviousD;
                    MDSplus::Data *tmaxPreviousD;
                    float64 tmaxPrevious;
//                    float64 nodeSamplingTime = 0.0;
                    nodes[nodeIdx]->getSegmentLimits(static_cast<int32>(i) - 1, &tminPreviousD, &tmaxPreviousD);
                    tmaxPrevious = tmaxPreviousD->getDouble();
//                    GetNodeSamplingTime(nodeIdx, nodeSamplingTime);
                    if ((tmin - tmaxPrevious) > (nodeSamplingTime[nodeIdx] * 1.5)) { //1.5 due o numeric errors. if a samples i s not saved the difference should be nodeSamplingTime * 2
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
                    MDSplus::deleteData(tminPreviousD);
                    MDSplus::deleteData(tmaxPreviousD);
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

//lint -e{613} Possible use of null pointer. If initialisation fails this function is not called.
uint32 MDSReader::CheckDiscontinuityOfTheSegments(const uint32 nodeNumber,
                                                  const uint32 initialSegment,
                                                  const uint32 finalSegment) const {
    uint32 counter = 0u;
    int32 maxSegment = static_cast<int32>(finalSegment) + 1;
    MDSplus::Data *tminD = NULL_PTR(MDSplus::Data *);
    MDSplus::Data *tmaxD = NULL_PTR(MDSplus::Data *);

    float64 tmin = 0.0;
    float64 tmax;

//    float64 nodeTimeDiff;
//    GetNodeSamplingTime(nodeNumber, nodeTimeDiff);
    nodes[nodeNumber]->getSegmentLimits(static_cast<int32>(initialSegment), &tminD, &tmaxD);
    tmax = tmaxD->getDouble();
    for (int32 currentSegment = static_cast<int32>(initialSegment) + 1; currentSegment < maxSegment; currentSegment++) {
        nodes[nodeNumber]->getSegmentLimits(currentSegment, &tminD, &tmaxD);
        tmin = tminD->getDouble();
        if ((tmin - tmax) > (nodeSamplingTime[nodeNumber] * 1.5)) {
            counter++;
        }
        tmax = tmaxD->getDouble();
    }
    MDSplus::deleteData(tminD);
    MDSplus::deleteData(tmaxD);
    return counter;
}

//lint -e{613} Possible use of null pointer. It is no possible. If initialisation fails this function is not called.
bool MDSReader::GetNodeSamplingTime(const uint32 idx,
                                    float64 &tDiff) const {
    bool ret = true;
    MDSplus::Data *timeD = nodes[idx]->getSegmentDim(0);
    int32 numberOfElementsPerSeg = 0;
    float64 *timeNode = timeD->getDoubleArray(&numberOfElementsPerSeg);
    if (numberOfElementsPerSeg < 1) {
        ret = false;
    }
    else if (numberOfElementsPerSeg == 1) {
        if (maxNumberOfSegments[idx] > 1u) {
            MDSplus::Data *timeD2 = nodes[idx]->getSegmentDim(1);
            int32 numberOfElementsPerSeg2 = 0;
            float64 *time2 = timeD2->getDoubleArray(&numberOfElementsPerSeg2);
            tDiff = time2[0] - timeNode[0];
            deleteData(timeD2);
        }
        else {
            ret = false;
        }
    }
    else {
        tDiff = timeNode[1] - timeNode[0];
    }
    MDSplus::deleteData(timeD);
    return ret;
}

//lint -e{613} Possible use of null pointer. Not possible. If initialisation fails this function is not called.
void MDSReader::CopyTheSameValue(const uint32 idxNumber,
                                 const uint32 numberOfTimes,
                                 const uint32 samplesOffset) {

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
    else {

    }
    return;
}

//lint -e{613} Possible use of null pointer. If initialise fails this function is not called.
bool MDSReader::AddValuesCopyData(const uint32 nodeNumber,
                                  uint32 minSegment,
                                  const uint32 numberOfDiscontinuities) {

    uint32 remainingSamplesToCopy = numberOfElements[nodeNumber];
    uint32 numberOfSamplesActuallyCopied = 0u;
    uint32 numberOfSamplesToCopy = 0u;
    float64 tBeginningDiscontinuity = 0.0;
    float64 tEndDiscontinuity = 0.0;
    uint32 futureSegment = 0u;
    uint32 extraOffset = 0u;
    if (minSegment > 0u) {
        futureSegment = minSegment - 1u;
    }
    uint32 numberOfSamplesCopied = 0u;

    bool ret = FindDiscontinuity(nodeNumber, futureSegment, tBeginningDiscontinuity, tEndDiscontinuity);
    if (ret) {
        numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, tEndDiscontinuity);
        if (holeManagement[nodeNumber] == 0u) {
            uint32 numberOfBytes = numberOfSamplesToCopy * bytesType[nodeNumber];
            ret = MemoryOperationsHelper::Set(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber]]), static_cast<char8>(0), numberOfBytes);
        }
        else {
            CopyTheSameValue(nodeNumber, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        numberOfSamplesCopied += numberOfSamplesToCopy;
        remainingSamplesToCopy -= numberOfSamplesToCopy;
        currentTime += static_cast<float64>(numberOfSamplesToCopy) * samplingTime[nodeNumber];
    }
    for (uint32 i = 0u; (i < numberOfDiscontinuities) && ret; i++) {
        ret = FindDiscontinuity(nodeNumber, futureSegment, tBeginningDiscontinuity, tEndDiscontinuity);
        if (ret) {
            float64 auxTime = tBeginningDiscontinuity + samplingTime[nodeNumber];
            numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime);
            if (dataManagement[nodeNumber] == 0u) {
                numberOfSamplesActuallyCopied = MakeRawCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
                currentTime += static_cast<float64>(numberOfSamplesActuallyCopied) * samplingTime[nodeNumber];
            }
            else if (dataManagement[nodeNumber] == 1u) {
                numberOfSamplesActuallyCopied = LinearInterpolationCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
            }
            else {
                numberOfSamplesActuallyCopied = HoldCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
            }
            numberOfSamplesCopied += numberOfSamplesActuallyCopied;
            remainingSamplesToCopy -= numberOfSamplesActuallyCopied;
            ret = (numberOfSamplesActuallyCopied == numberOfSamplesToCopy);
            numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, tEndDiscontinuity);
        }
        if (ret) {
            extraOffset = numberOfSamplesCopied * bytesType[nodeNumber];
            if (holeManagement[nodeNumber] == 0u) {
                uint32 numberOfBytes = numberOfSamplesToCopy * bytesType[nodeNumber];
                uint32 auxIdx = offsets[nodeNumber] + extraOffset;
                ret = MemoryOperationsHelper::Set(reinterpret_cast<void *>(&dataSourceMemory[auxIdx]), static_cast<char8>(0), numberOfBytes);
            }
            else {
                CopyTheSameValue(nodeNumber, numberOfSamplesToCopy, numberOfSamplesCopied);
            }
            currentTime += static_cast<float64>(numberOfSamplesToCopy) * samplingTime[nodeNumber];
            numberOfSamplesCopied += numberOfSamplesToCopy;
            remainingSamplesToCopy -= numberOfSamplesToCopy;
            minSegment = futureSegment;
        }
    }
    if (ret) {
        numberOfSamplesToCopy = remainingSamplesToCopy;
        if (dataManagement[nodeNumber] == 0u) {
            numberOfSamplesActuallyCopied = MakeRawCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
            currentTime += static_cast<float64>(numberOfSamplesToCopy) * samplingTime[nodeNumber];
        }
        else if (dataManagement[nodeNumber] == 1u) {
            numberOfSamplesActuallyCopied = LinearInterpolationCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        else {
            numberOfSamplesActuallyCopied = HoldCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        numberOfSamplesCopied += numberOfSamplesActuallyCopied;
    }
    ret = (numberOfSamplesCopied == numberOfElements[nodeNumber]);
//nodes[nodeNumber]->getSegmentAndDimension(minSegment,dataA, timeD);
    return ret;
}

//lint -e{613} Possible use of null pointer. Not possible. If initialise fails this function is not called.
uint32 MDSReader::ComputeSamplesToCopy(const uint32 nodeNumber,
                                       const float64 tstart,
                                       const float64 tend) const {
    float64 auxFloat = (tend - tstart) / samplingTime[nodeNumber];
    uint32 samplesToCopy = static_cast<uint32>(auxFloat);
    VerifySamples(nodeNumber, samplesToCopy, tstart, tend);
    return samplesToCopy;

}

//lint -e{613} Possible use of null pointer. If the initialisation fails this function is not called.
void MDSReader::VerifySamples(const uint32 nodeNumber,
                              uint32 &samples,
                              const float64 tstart,
                              const float64 tend) const {

    float64 tinterval = tend - tstart;
    float64 samplesInterval = static_cast<float64>(samples) * samplingTime[nodeNumber];
    float64 diff = tinterval - samplesInterval;
    if (diff > (samplingTime[nodeNumber] * 0.9999999)) {
        samples++;
    }
    return;
}

//lint -e{613} Possible use of null pointer. If initialisation fails this function is not called.
bool MDSReader::CopyDataAddValues(const uint32 nodeNumber,
                                  uint32 minSegment,
                                  const uint32 numberOfDiscontinuities,
                                  const uint32 samplesToRead,
                                  const uint32 samplesRead) {
    bool ret = true;
    uint32 numberOfSamplesToCopy = 0u;
    uint32 remainingSamplesToCopy = samplesToRead;
    uint32 numberOfSamplesCopied = 0u;
    uint32 totalSamplesCopied = samplesRead;
    uint32 numberOfSamplesActuallyCopied = 0u;
    float64 tBeginningDiscontinuity = 0.0;
    float64 tEndDiscontinuity = 0.0;
    uint32 futureSegment = minSegment;
    for (uint32 i = 0u; (i < numberOfDiscontinuities) && ret; i++) {
        ret = FindDiscontinuity(nodeNumber, futureSegment, tBeginningDiscontinuity, tEndDiscontinuity);
        float64 auxTime = tBeginningDiscontinuity + samplingTime[nodeNumber];
        numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime);
        if (dataManagement[nodeNumber] == 0u) {
            numberOfSamplesActuallyCopied = MakeRawCopy(nodeNumber, minSegment, numberOfSamplesToCopy, totalSamplesCopied);
            currentTime += static_cast<float64>(numberOfSamplesActuallyCopied) * samplingTime[nodeNumber];
        }
        else if (dataManagement[nodeNumber] == 1u) {
            numberOfSamplesActuallyCopied = LinearInterpolationCopy(nodeNumber, minSegment, numberOfSamplesToCopy, totalSamplesCopied);
        }
        else {
            numberOfSamplesActuallyCopied = HoldCopy(nodeNumber, minSegment, numberOfSamplesToCopy, totalSamplesCopied);
        }
        numberOfSamplesCopied += numberOfSamplesActuallyCopied;
        totalSamplesCopied += numberOfSamplesActuallyCopied;
        remainingSamplesToCopy -= numberOfSamplesActuallyCopied;
        float64 auxFloat = (tEndDiscontinuity - tBeginningDiscontinuity) / samplingTime[nodeNumber];
        uint32 samplesToEndDiscontinuity = static_cast<uint32>(auxFloat);
        if (samplesToEndDiscontinuity > remainingSamplesToCopy) {
            numberOfSamplesToCopy = remainingSamplesToCopy;
        }
        else {
            numberOfSamplesToCopy = samplesToEndDiscontinuity;
        }
        if (holeManagement[nodeNumber] == 0u) {
            uint32 numberOfBytes = numberOfSamplesToCopy * bytesType[nodeNumber];
            uint32 extraOffset = totalSamplesCopied * bytesType[nodeNumber];
            uint32 auxIdx = offsets[nodeNumber] + extraOffset;
            ret = MemoryOperationsHelper::Set(reinterpret_cast<void *>(&dataSourceMemory[auxIdx]), static_cast<char8>(0), numberOfBytes);
        }
        else {
            CopyTheSameValue(nodeNumber, numberOfSamplesToCopy, totalSamplesCopied);
        }
        currentTime += static_cast<float64>(numberOfSamplesToCopy) * samplingTime[nodeNumber];
        numberOfSamplesCopied += numberOfSamplesToCopy;
        totalSamplesCopied += numberOfSamplesToCopy;
        remainingSamplesToCopy -= numberOfSamplesToCopy;
        minSegment = futureSegment;
    }
    if (ret) {
        ret = (numberOfSamplesCopied == samplesToRead);
    }
    return ret;
}

//lint -e{613} Possible use of null pointer. If initialisation fails this function is not called.
bool MDSReader::AddValuesCopyDataAddValues(const uint32 nodeNumber,
                                           const uint32 minSegment,
                                           const uint32 numberOfDiscontinuities) {
    bool ret;
    uint32 numberOfSamplesToCopy = 0u;
    uint32 numberOfSamplesCopied = 0u;
    float64 tBeginningDiscontinuity = 0.0;
    float64 tEndDiscontinuity = 0.0;
    uint32 remainingSamplesInTheSegment = 0u;
    uint32 futureSegment = 0u;
    if (minSegment > 0u) {
        futureSegment = minSegment - 1u;
    }
    else {
    }

    ret = FindDiscontinuity(nodeNumber, futureSegment, tBeginningDiscontinuity, tEndDiscontinuity);
    if (ret) {
        float64 auxTime = tEndDiscontinuity;                        //tBeginningDiscontinuity + samplingTime[nodeNumber];
        remainingSamplesInTheSegment = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime);
        if (remainingSamplesInTheSegment > numberOfElements[nodeNumber]) {
            numberOfSamplesToCopy = numberOfElements[nodeNumber];
        }
        else {
            numberOfSamplesToCopy = remainingSamplesInTheSegment;
        }
        //numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime);
        if (holeManagement[nodeNumber] == 0u) {
            uint32 bytesToCopy = numberOfSamplesToCopy * bytesType[nodeNumber];
            ret = MemoryOperationsHelper::Set(reinterpret_cast<void *>(&dataSourceMemory[offsets[nodeNumber]]), static_cast<char8>(0), bytesToCopy);
        }
        else {
            CopyTheSameValue(nodeNumber, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        numberOfSamplesCopied += numberOfSamplesToCopy;
        currentTime += static_cast<float64>(numberOfSamplesToCopy) * samplingTime[nodeNumber];
    }
    if (ret) {
        ret = CopyDataAddValues(nodeNumber, futureSegment, numberOfDiscontinuities, numberOfElements[nodeNumber] - numberOfSamplesCopied,
                                numberOfSamplesCopied);
    }
    return ret;
}

//lint -e{794} Conceivable use of null pointer. Not possible. If initialisation fails this function is not called.
//lint -e{613} possible use of null pointer. Not possible. If initiaisation fails this function is not called.
bool MDSReader::CopyDataAddValuesCopyData(const uint32 nodeNumber,
                                          uint32 minSegment,
                                          const uint32 numberOfDiscontinuities) {
    bool ret = true;
    uint32 numberOfSamplesToCopy = 0u;
    uint32 numberOfSamplesCopied = 0u;
    uint32 numberOfSamplesActuallyCopied = 0u;
    float64 tBeginningDiscontinuity = 0.0;
    float64 tEndDiscontinuity = 0.0;
    uint32 futureSegment = minSegment;
    for (uint32 i = 0u; (i < numberOfDiscontinuities) && ret; i++) {
        ret = FindDiscontinuity(nodeNumber, futureSegment, tBeginningDiscontinuity, tEndDiscontinuity);
        float64 auxTime = tBeginningDiscontinuity + samplingTime[nodeNumber];
        numberOfSamplesToCopy = ComputeSamplesToCopy(nodeNumber, currentTime, auxTime);
        if (dataManagement[nodeNumber] == 0u) {
            numberOfSamplesActuallyCopied += MakeRawCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
            currentTime += static_cast<float64>(numberOfSamplesActuallyCopied) * samplingTime[nodeNumber];
        }
        else if (dataManagement[nodeNumber] == 1u) {
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
                uint32 auxIndex = offsets[nodeNumber] + extraOffset;
                void * auxPtr = reinterpret_cast<void *>(&(dataSourceMemory[auxIndex]));
                ret = MemoryOperationsHelper::Set(auxPtr, static_cast<char8>(0), numberOfBytes);
                numberOfSamplesCopied += numberOfSamplesToCopy;
            }
            else {
                CopyTheSameValue(nodeNumber, numberOfSamplesToCopy, numberOfSamplesCopied);
                numberOfSamplesCopied += numberOfSamplesToCopy;
            }
            currentTime += static_cast<float64>(numberOfSamplesToCopy) * samplingTime[nodeNumber];
        }
        minSegment = futureSegment;
    }
    if (ret) {
        numberOfSamplesToCopy = numberOfElements[nodeNumber] - numberOfSamplesCopied;
        if (dataManagement[nodeNumber] == 0u) {
            numberOfSamplesActuallyCopied = MakeRawCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        else if (dataManagement[nodeNumber] == 1u) {
            numberOfSamplesActuallyCopied = LinearInterpolationCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        else {
            numberOfSamplesActuallyCopied = HoldCopy(nodeNumber, minSegment, numberOfSamplesToCopy, numberOfSamplesCopied);
        }
        if (numberOfSamplesActuallyCopied == numberOfSamplesToCopy) {
            numberOfSamplesCopied += numberOfSamplesActuallyCopied;
        }
        else {
            ret = false;
        }
    }
    if (ret) {
        ret = (numberOfSamplesCopied == numberOfElements[nodeNumber]);
    }
    return ret;
}

//lint -e{613} Possible use of null pointer. Not possible.If initialisation fails this function is not called
bool MDSReader::FindDiscontinuity(const uint32 nodeNumber,
                                  uint32 &segment,
                                  float64 &beginningTime,
                                  float64 &endTime) const {
    bool find = false;
//    bool error;
    MDSplus::Data *tminD = NULL_PTR(MDSplus::Data *);
    MDSplus::Data *tmaxD = NULL_PTR(MDSplus::Data *);

    float64 tmin;
    float64 tmax;

//    float64 nodeTimeDiff;
//    error = !GetNodeSamplingTime(nodeNumber, nodeTimeDiff);
    nodes[nodeNumber]->getSegmentLimits(static_cast<int32>(segment), &tminD, &tmaxD);
    tmin = tminD->getDouble();
//Playing with tolerances
    float64 auxDiff = tmin - currentTime;
    if (auxDiff > 0.00000001) {                        //tolerance is 1/100MHz
        endTime = tmin;
        find = true;
    }
    else {
        tmax = tmaxD->getDouble();
        int32 initialSemgnet = static_cast<int32>(segment) + 1;
        for (int32 currentSegment = initialSemgnet; (currentSegment < static_cast<int32>(maxNumberOfSegments[nodeNumber])) && (!find); currentSegment++) {
            nodes[nodeNumber]->getSegmentLimits(currentSegment, &tminD, &tmaxD);
            tmin = tminD->getDouble();
            if ((tmin - tmax) > (nodeSamplingTime[nodeNumber] * 1.5)) {
                beginningTime = tmax;
                endTime = tmin;
                find = true;
            }
            tmax = tmaxD->getDouble();
            segment = static_cast<uint32>(currentSegment);
        }
    }
    MDSplus::deleteData(tminD);
    MDSplus::deleteData(tmaxD);
    return find;
//    return (find && (!error));
}

//lint -e{613} Possible use of null pointer. Not possible. If initilisation fails this function is not called.
uint32 MDSReader::MakeRawCopy(const uint32 nodeNumber,
                              const uint32 minSeg,
                              const uint32 samplesToCopy,
                              const uint32 offsetSamples) {

    uint32 samplesCopied = 0u;
    if (type[nodeNumber] == UnsignedInteger8Bit) {
        samplesCopied = MakeRawCopyTemplate<uint8>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == SignedInteger8Bit) {
        samplesCopied = MakeRawCopyTemplate<int8>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == UnsignedInteger16Bit) {
        samplesCopied = MakeRawCopyTemplate<uint16>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == SignedInteger16Bit) {
        samplesCopied = MakeRawCopyTemplate<int16>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == UnsignedInteger32Bit) {
        samplesCopied = MakeRawCopyTemplate<uint32>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == SignedInteger32Bit) {
        samplesCopied = MakeRawCopyTemplate<int32>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == UnsignedInteger64Bit) {
        samplesCopied = MakeRawCopyTemplate<uint64>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == SignedInteger64Bit) {
        samplesCopied = MakeRawCopyTemplate<int64>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == Float32Bit) {
        samplesCopied = MakeRawCopyTemplate<float32>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else if (type[nodeNumber] == Float64Bit) {
        samplesCopied = MakeRawCopyTemplate<float64>(nodeNumber, minSeg, samplesToCopy, offsetSamples);
    }
    else {

    }
    return samplesCopied;
}

//lint -e{613} Possible use of null pointer. Not possible. If initialisation fails LinearInterpolationCopy() is not called.
uint32 MDSReader::LinearInterpolationCopy(const uint32 nodeNumber,
                                          const uint32 minSeg,
                                          const uint32 samplesToCopy,
                                          const uint32 offsetSamples) {

    uint32 samplesCopied = 0u;

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
    else {

    }
    return samplesCopied;
}

//lint -e{613} Possible use of null pointer. If initialisation fails the Synchronise is not called neither HoldCopy()
uint32 MDSReader::HoldCopy(const uint32 nodeNumber,
                           const uint32 minSeg,
                           const uint32 samplesToCopy,
                           const uint32 samplesOffset) {

    uint32 samplesCopied = 0u;
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
    else {

    }
    return samplesCopied;
}

//lint -e{613} Warning 613: Possible use of null pointer. All pointers are initialised previously. If initialisation fails the CopyRemainingData() is not called.
bool MDSReader::CopyRemainingData(const uint32 nodeNumber,
                                  const uint32 minSegment) {
    bool ret = false;
    uint32 samplesCopied;
    if (dataManagement[nodeNumber] == 0u) {
        samplesCopied = MakeRawCopy(nodeNumber, minSegment, numberOfElements[nodeNumber], 0u);
        ret = (samplesCopied <= numberOfElements[nodeNumber]);
    }
    else if (dataManagement[nodeNumber] == 1u) {
        samplesCopied = LinearInterpolationCopy(nodeNumber, minSegment, numberOfElements[nodeNumber], 0u);
        ret = (samplesCopied <= numberOfElements[nodeNumber]);
    }
    else {
        samplesCopied = HoldCopy(nodeNumber, minSegment, numberOfElements[nodeNumber], 0u);
        ret = (samplesCopied <= numberOfElements[nodeNumber]);
    }
    if ((samplesCopied > 0u) && ret) {
        uint32 bytesToCopy = (numberOfElements[nodeNumber] - samplesCopied) * bytesType[nodeNumber];
        uint32 extraOffset = samplesCopied * bytesType[nodeNumber];
        uint32 offset = offsets[nodeNumber] + extraOffset;
        ret = MemoryOperationsHelper::Set(reinterpret_cast<void *>(&(dataSourceMemory[offset])), static_cast<char8>(0), bytesToCopy);
    }
    return ret;
}

CLASS_REGISTER(MDSReader, "1.0")
}

