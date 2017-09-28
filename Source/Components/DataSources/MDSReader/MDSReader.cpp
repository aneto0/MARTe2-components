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
    byteSizeNodes = NULL_PTR(uint32 *);
    shotNumber = 0;
    type = NULL_PTR(TypeDescriptor *);
    numberOfElements = NULL_PTR(uint32 *);
    dataSourceMemory = NULL_PTR(char *);
    offsets = NULL_PTR(uint32 *);
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
    if (byteSizeNodes != NULL_PTR(uint32 *)) {
        delete[] byteSizeNodes;

    }
    if (type != NULL_PTR(TypeDescriptor *)) {
        delete[] type;

    }
    if (offsets != NULL_PTR(uint32 *)) {
        delete[] offsets;
        offsets = NULL_PTR(uint32 *);
    }
    if (dataSourceMemory != NULL_PTR(char *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(dataSourceMemory));
        dataSourceMemory = NULL_PTR(char *);
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
    /*
     if (ok) { //read nodeNames
     AnyType functionsArray = data.GetType("NodeNames");
     ok = functionsArray.GetDataPointer() != NULL_PTR(void *);
     if (!ok) {
     REPORT_ERROR(ErrorManagement::ParametersError, "NodeNames pointer not found. data.GetType(NodeNames) returned a NULL pointer");
     }
     if (ok) {
     numberOfNodeNames = functionsArray.GetNumberOfElements(0u);
     ok = (numberOfNodeNames > 0u);
     if (!ok) {
     REPORT_ERROR(ErrorManagement::ParametersError, "Number of nodes is 0");
     }
     if (ok) {
     nodeNames = new StreamString[numberOfNodeNames];
     Vector<StreamString> vector(nodeNames, numberOfNodeNames);
     ok = data.Read("NodeNames", vector);
     if (!ok) {
     REPORT_ERROR(ErrorManagement::ParametersError,
     "Error reading NodeNames. The name exist but it is not a vector of strings. Do you forget the brackets{}?");
     }
     }
     }
     }

     if (ok) { //open nodes
     nodes = new MDSplus::TreeNode *[numberOfNodeNames];
     for (uint32 i = 0u; i < numberOfNodeNames; i++) { //Initialise to NULL pointer because atomathically is not done and the destructor try to delete memory not allocated
     nodes[i] = NULL_PTR(MDSplus::TreeNode *);
     }
     for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
     ok = OpenNode(i);
     if (!ok) {
     REPORT_ERROR(ErrorManagement::ParametersError, "Error opening node %s. It may not exist \n", nodeNames[i].Buffer());
     }
     }
     }
     if (ok) { //get mds node type, validate type and get byte size of each node
     mdsNodeTypes = new StreamString[numberOfNodeNames];
     byteSizeNodes = new uint32[numberOfNodeNames];
     for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
     ok = GetTypeNode(i);
     if (!ok) { //It should never happen because the node exist. But if the tree is corrupted it may launch the error
     uint32 auxIdx = i;
     REPORT_ERROR(ErrorManagement::ParametersError, "Error getting type node %u \n", auxIdx);
     }
     if (ok) {
     ok = IsValidTypeNode(i);
     if (!ok) {
     REPORT_ERROR(ErrorManagement::ParametersError, "Type = %s. NodeName = %s\n", mdsNodeTypes[i].Buffer(), nodeNames[i].Buffer());
     }
     if (ok) {
     GetByteSize(i);
     }
     }
     }
     }
     if (ok) { //read signal types (optional). If they exist then check with the node types.
     AnyType functionsArray = data.GetType("SignalTypes");
     bool auxOk = functionsArray.GetDataPointer() != NULL_PTR(void *);
     if (!auxOk) {
     REPORT_ERROR(ErrorManagement::Information, "SignalTypes pointer not specified. Signal types will be obtained from the nodes");
     }
     if (auxOk) {
     uint32 aux = functionsArray.GetNumberOfElements(0u);
     ok = (numberOfNodeNames == aux);
     if (!ok) {
     REPORT_ERROR(ErrorManagement::ParametersError,
     "Number of SignalTypes is different from the number of node names. number of SignalTypes = %u, number of NodeNames = %u", aux,
     numberOfNodeNames);
     }
     if (ok) {
     signalTypes = new StreamString[numberOfNodeNames];
     Vector<StreamString> vector(signalTypes, numberOfNodeNames);
     ok = data.Read("SignalTypes", vector);
     if (!ok) { // it  should never happen because before data.GetType was called and succeed.
     REPORT_ERROR(ErrorManagement::ParametersError, "Error reading SignalTypes");
     }
     }
     if (ok) { //check SignalTypes against mdsNodeTypes
     for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
     ok = CheckSignalTypesAgainstMdsNodeTypes(i);
     if (!ok) {
     uint32 auxIdx = i;
     StreamString auxStr = ConvertMDStypeToMARTeType(mdsNodeTypes[auxIdx]);
     REPORT_ERROR(ErrorManagement::ParametersError,
     "signalTypes different to mdsNodeTypes for signal %u. signalTypes = %s, mdsNodeTypes = %s (%s)", auxIdx, signalTypes[auxIdx].Buffer(),
     mdsNodeTypes[auxIdx].Buffer(), auxStr.Buffer());
     }
     }
     }
     }
     }
     */
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
        byteSizeNodes = new uint32[numberOfNodeNames];
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
                if (ok) {
                    GetByteSize(i);
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
                                 auxIdx, type[auxIdx].type, mdsNodeTypes[auxIdx].Buffer(), auxStr.type);
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

    if (ok) { //Count and allocate memory
        offsets = new uint32[numberOfNodeNames];
        //Count the number of bytes
        uint32 totalSignalMemory = 0u;
        for (uint32 i = 0u; (i < numberOfNodeNames) && ok; i++) {
            offsets[i] = totalSignalMemory;
            uint32 nBytes = 0u;
            ok = GetSignalByteSize(i, nBytes);
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Error while GetSignalByteSize() for signal %u", auxIdx);
            }
            totalSignalMemory += nBytes;
        }
        //Allocate memory
        if (ok) {
            dataSourceMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalSignalMemory));
        }
    }

    return ok;
}

bool MDSReader::Synchronise() {

    return true;
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
    if (ok) {
        /*lint -e{613} dataSourceMemory cannot be NULL here*/
        char8 *memPtr = &dataSourceMemory[offsets[signalIdx]];
        signalAddress = reinterpret_cast<void *&>(memPtr);
    }
    return ok;
}

const char *MDSReader::GetBrokerName(StructuredDataI &data,
                                     const SignalDirection direction) {
    return "";
}

//TODO
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

void MDSReader::GetByteSize(uint32 idx) {
    if (mdsNodeTypes[idx] == "DTYPE_BU") {
        byteSizeNodes[idx] = 1u;
    }
    else if (mdsNodeTypes[idx] == "DTYPE_B") {
        byteSizeNodes[idx] = 1u;
    }
    else if (mdsNodeTypes[idx] == "DTYPE_WU") {
        byteSizeNodes[idx] = 2u;
    }
    else if (mdsNodeTypes[idx] == "DTYPE_W") {
        byteSizeNodes[idx] = 2u;
    }
    else if (mdsNodeTypes[idx] == "DTYPE_LU") {
        byteSizeNodes[idx] = 4u;
    }
    else if (mdsNodeTypes[idx] == "DTYPE_L") {
        byteSizeNodes[idx] = 4u;
    }
    else if (mdsNodeTypes[idx] == "DTYPE_QU") {
        byteSizeNodes[idx] = 8u;
    }
    else if (mdsNodeTypes[idx] == "DTYPE_Q") {
        byteSizeNodes[idx] = 8u;
    }
    else if (mdsNodeTypes[idx] == "DTYPE_FS") {
        byteSizeNodes[idx] = 4u;
    }
    else if (mdsNodeTypes[idx] == "DTYPE_FT") {
        byteSizeNodes[idx] = 8u;
    }
    return;
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
        str = Float32Bit;
    }
    return str;

}
CLASS_REGISTER(MDSReader, "1.0")
}

