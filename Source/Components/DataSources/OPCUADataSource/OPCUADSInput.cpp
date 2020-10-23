/**
 * @file OPCUADSInput.cpp
 * @brief Source file for class OPCUADSInput
 * @date 12/03/2019
 * @author Luca Porzio
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
 * the class OPCUADSInput (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "OPCUADSInput.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/*-e909 and -e9133 redefines bool. -e578 symbol ovveride in CLASS_REGISTER*/
/*lint -save -e909 -e9133 -e578*/
namespace MARTe {

OPCUADSInput::OPCUADSInput() :
        DataSourceI(),
        EmbeddedServiceMethodBinderI(),
        executor(*this) {
    masterClient = NULL_PTR(OPCUAClientRead*);
    nOfSignals = 0u;
    numberOfNodes = 0u;
    paths = NULL_PTR(StreamString*);
    extensionObject = NULL_PTR(StreamString*);
    structuredTypeNames = NULL_PTR(StreamString*);
    namespaceIndexes = NULL_PTR(uint16*);
    tempPaths = NULL_PTR(StreamString*);
    tempNamespaceIndexes = NULL_PTR(uint16*);
    serverAddress = "";
    readMode = "";
    sync = "";
    samplingTime = 0.0;
    nElements = NULL_PTR(uint32*);
    tempNElements = NULL_PTR(uint32*);
    entryArrayElements = NULL_PTR(uint32*);
    entryNumberOfMembers = NULL_PTR(uint32*);
    entryArraySize = 0u;
    types = NULL_PTR(TypeDescriptor*);
    entryTypes = NULL_PTR(TypeDescriptor*);
    cpuMask = 0xffu;
    stackSize = THREADS_DEFAULT_STACKSIZE;
}

/*lint -e{1551} must stop the SingleThreadService in the destructor.*/
OPCUADSInput::~OPCUADSInput() {
    (void) executor.Stop();
    if (masterClient != NULL_PTR(OPCUAClientRead*)) {
        delete masterClient;
    }
    if (nElements != NULL_PTR(uint32*)) {
        delete[] nElements;
    }
    if (tempNElements != NULL_PTR(uint32*)) {
        delete[] tempNElements;
    }
    if (types != NULL_PTR(TypeDescriptor*)) {
        delete[] types;
    }
    if (namespaceIndexes != NULL_PTR(uint16*)) {
        delete[] namespaceIndexes;
    }
    if (paths != NULL_PTR(StreamString*)) {
        delete[] paths;
    }
    if (extensionObject != NULL_PTR(StreamString*)) {
        delete[] extensionObject;
    }
    if (structuredTypeNames != NULL_PTR(StreamString*)) {
        delete[] structuredTypeNames;
    }
    if (tempPaths != NULL_PTR(StreamString*)) {
        delete[] tempPaths;
    }
    if (tempNamespaceIndexes != NULL_PTR(uint16*)) {
        delete[] tempNamespaceIndexes;
    }
    if (entryArrayElements != NULL_PTR(uint32*)) {
        delete[] entryArrayElements;
    }
    if (entryNumberOfMembers != NULL_PTR(uint32*)) {
        delete[] entryNumberOfMembers;
    }
    if (entryTypes != NULL_PTR(TypeDescriptor*)) {
        delete[] entryTypes;
    }

}

bool OPCUADSInput::Initialise(StructuredDataI &data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("Address", serverAddress);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the Address attribute");
        }
        if (ok) {
            ok = data.Read("ReadMode", readMode);
            if (!ok) {
                readMode = "Read";
                REPORT_ERROR(ErrorManagement::Information, "ReadMode option is not enabled. Using Service Read.");
                ok = true;
            }
        }
        if (ok) {
            ok = data.Read("Synchronise", sync);
            if (!ok) {
                sync = "no";
                REPORT_ERROR(ErrorManagement::Information, "Synchronise option is not enabled. Using SingleThreadService.");
                ok = true;
            }
        }
        if (sync == "no") {
            if (ok) {
                ok = data.Read("CpuMask", cpuMask);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::Information, "CpuMask not set. Using default.");
                }
                ok = true;
            }
            if (ok) {
                ok = data.Read("StackSize", stackSize);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::Information, "StackSize not set. Using default.");
                }
                ok = true;
            }
        }
        if (ok) {
            ok = data.Read("SamplingTime", samplingTime);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::Information, "Sampling Time not set. Using default value: 250ms");
                samplingTime = 250.0;
                ok = true;
            }
        }
        if (ok) {
            ok = data.MoveRelative("Signals");
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
            }
        }
        if (ok) {
            ok = signalsDatabase.MoveRelative("Signals");
        }
        /* Do not allow to add signals in run-time */
        if (ok) {
            ok = signalsDatabase.Write("Locked", 1u);
            nOfSignals = (signalsDatabase.GetNumberOfChildren() - 1u);
            tempPaths = new StreamString[nOfSignals];
            tempNamespaceIndexes = new uint16[nOfSignals];
            extensionObject = new StreamString[nOfSignals];
            tempNElements = new uint32[nOfSignals];
            for (uint32 i = 0u; (i < nOfSignals) && (ok); i++) {
                ok = signalsDatabase.MoveRelative(signalsDatabase.GetChildName(i));
                if (ok) {
                    ok = signalsDatabase.Read("Path", tempPaths[i]);
                    if (!ok) {
                        uint32 k = i;
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the Path attribute from signal %d", k);
                    }
                }
                if (ok) {
                    ok = signalsDatabase.Read("NamespaceIndex", tempNamespaceIndexes[i]);
                    if (!ok) {
                        uint32 k = i;
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the NamespaceIndex attribute from signal %d", k);
                    }
                }
                if (ok) {
                    ok = signalsDatabase.Read("NumberOfElements", tempNElements[i]);
                    if (!ok) {
                        tempNElements[i] = 1u;
                        ok = true;
                        uint32 k = i;
                        REPORT_ERROR(ErrorManagement::Information, "No NumberOfElements set up for signal %d. Using N = 1", k);
                    }
                }
                if (ok) {
                    ok = signalsDatabase.Read("ExtensionObject", extensionObject[i]);
                    if ((extensionObject[i] == "yes") && ok) {
                        structuredTypeNames = new StreamString[nOfSignals];
                        ok = signalsDatabase.Read("Type", structuredTypeNames[i]);
                        if (!ok) {
                            uint32 k = i;
                            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the Type attribute from signal %d", k);
                        }
                        REPORT_ERROR(ErrorManagement::Information, "Reading Structure with OPC UA Complex DataType Extension");
                    }
                    else {
                        extensionObject[i] = "no";
                        ok = true;
                    }
                }
                if (ok) {
                    ok = signalsDatabase.MoveToAncestor(1u);
                }
            }
        }
        if (ok) {
            ok = signalsDatabase.MoveToAncestor(1u);
        }
        if (ok) {
            ok = data.MoveToAncestor(1u);
        }
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Error during Initialise!");
    }
    return ok;
}

bool OPCUADSInput::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    numberOfNodes = GetNumberOfSignals();
    uint8 nDimensions = 0u;
    nElements = new uint32[numberOfNodes];
    types = new TypeDescriptor[numberOfNodes];
    StreamString signalName;
    /* Check for signals with number of dimensions > 1. Multidimensional arrays not supported yet */
    for (uint32 k = 0u; k < numberOfNodes; k++) {
        ok = GetSignalNumberOfDimensions(k, nDimensions);
        if ((nDimensions > 1u) && ok) {
            ok = GetSignalName(k, signalName);
            if (ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Signal %s has Number Of Dimensions = %d. Multidimensional arrays not supported yet.",
                             signalName.Buffer(), nDimensions);
                ok = false;
            }
        }
        if (ok) {
            ok = GetSignalNumberOfElements(k, nElements[k]);
        }
        if (ok) {
            types[k] = GetSignalType(k);
        }
        signalName = "";
    }

    uint32 bodyLength = 0u;
    if (structuredTypeNames != NULL_PTR(StreamString*)) {
        for (uint32 k = 0u; k < nOfSignals; k++) {
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(structuredTypeNames[k].Buffer());
            if (cri != NULL_PTR(const ClassRegistryItem*)) {
                const Introspection *intro = cri->GetIntrospection();
                ok = (intro != NULL_PTR(const Introspection*));
                if (ok) {
                    GetStructureDimensions(intro, entryArraySize);

                    entryArrayElements = new uint32[entryArraySize];
                    entryNumberOfMembers = new uint32[entryArraySize];
                    entryTypes = new TypeDescriptor[entryArraySize];

                    uint32 index = 0u;
                    ok = GetStructure(intro, entryArrayElements, entryTypes, entryNumberOfMembers, index);
                    if (ok) {
                        ok = GetBodyLength(intro, bodyLength);
                    }
                    if (ok) {
                        bodyLength *= tempNElements[k];
                    }
                }
            }
        }
    }
    if (extensionObject != NULL_PTR(StreamString*)) {
        if (extensionObject[0u] == "no") {
            if (ok) {
                paths = new StreamString[numberOfNodes];
                namespaceIndexes = new uint16[numberOfNodes];
                StreamString sigName;
                StreamString pathToken;
                StreamString sigToken;
                char8 ignore;
                for (uint32 i = 0u; i < numberOfNodes; i++) {
                    sigName = "";
                    /* Getting the first name from the signal path */
                    ok = GetSignalName(i, sigName);
                    if (ok) {
                        ok = sigName.Seek(0LLU);
                    }
                    if (ok) {
                        ok = sigName.GetToken(sigToken, ".", ignore);
                    }
                    if (ok) {
                        for (uint32 j = 0u; j < nOfSignals; j++) {
                            StreamString lastToken;
                            sigToken = "";
                            if (tempPaths != NULL_PTR(StreamString*)) {
                                ok = tempPaths[j].Seek(0LLU);
                                if (ok) {
                                    do {
                                        ok = tempPaths[j].GetToken(lastToken, ".", ignore);
                                        if (ok) {
                                            sigToken = lastToken;
                                        }
                                        lastToken = "";
                                    }
                                    while (ok);
                                }
                                /* This cycle will save the last token found */
                                ok = tempPaths[j].Seek(0LLU);
                                if (ok) {
                                    do {
                                        ok = tempPaths[j].GetToken(lastToken, ".", ignore);
                                        if (ok) {
                                            sigToken = lastToken;
                                        }
                                        lastToken = "";
                                    }
                                    while (ok);
                                }
                                ok = tempPaths[j].Seek(0LLU);
                                if (ok) {
                                    do {
                                        pathToken = "";
                                        ok = tempPaths[j].GetToken(pathToken, ".", ignore);
                                        if ((paths != NULL_PTR(StreamString*)) && ok) {
                                            if ((namespaceIndexes != NULL_PTR(uint16*)) && (tempNamespaceIndexes != NULL_PTR(uint16*))) {
                                                if (pathToken == sigToken) {
                                                    if (numberOfNodes == nOfSignals) {
                                                        paths[j] = tempPaths[j];
                                                        namespaceIndexes[j] = tempNamespaceIndexes[j];
                                                    }
                                                    else {
                                                        paths[i] = tempPaths[j];
                                                        namespaceIndexes[i] = tempNamespaceIndexes[j];
                                                    }
                                                    ok = false; /* Exit from the cycle */
                                                }
                                            }
                                        }
                                    }
                                    while (ok);
                                }

                            }
                        }

                        /* Then we add to the path the remaining node names */
                        StreamString dotToken = ".";
                        do {
                            sigToken = "";
                            ok = sigName.GetToken(sigToken, ".", ignore);
                            if ((paths != NULL_PTR(StreamString*)) && ok) {
                                paths[i] += dotToken;
                                paths[i] += sigToken;
                            }
                        }
                        while (ok);
                        ok = true;
                    }
                }
            }
        }
    }
    if (ok) {
        /* Setting up the master Client who will perform the operations */
        masterClient = new OPCUAClientRead;
        masterClient->SetServerAddress(serverAddress);
        ok = masterClient->Connect();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot Connect to the Server.");
        }
        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "The connection with the OPCUA Server has been established successfully!");
            if (extensionObject != NULL_PTR(StreamString*)) {
                if (extensionObject[0u] == "no") {
                    ok = masterClient->SetServiceRequest(namespaceIndexes, paths, numberOfNodes);
                    if (ok) {
                        masterClient->SetValueMemories(numberOfNodes);
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "SetServiceRequest Failed.");
                    }
                }
                else {
                    ok = masterClient->SetServiceRequest(tempNamespaceIndexes, tempPaths, nOfSignals);
                    if (ok) {
                        masterClient->SetDataPtr(bodyLength);
                        masterClient->SetValueMemories(numberOfNodes);
                        for (uint32 k = 0u; k < nOfSignals; k++) {
                            uint32 nodeCounter = 0u;
                            uint32 index;
                            if (tempNElements != NULL_PTR(uint32*)) {
                                for (uint32 j = 0u; j < tempNElements[k]; j++) {
                                    index = 0u;
                                    uint32 numberOfNodesForEachIteration = (numberOfNodes / tempNElements[k]) * (j + 1u);
                                    while (nodeCounter < numberOfNodesForEachIteration) {
                                        if (ok) {
                                            ok = masterClient->GetExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers,
                                                                                            entryArraySize, nodeCounter, index);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ((sync == "no") && ok) {
        executor.SetCPUMask(cpuMask);
        executor.SetStackSize(stackSize);
        executor.SetName(GetName());
        ok = (executor.Start() == ErrorManagement::NoError);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Error during configuration.");
    }
    return ok;
}

bool OPCUADSInput::AllocateMemory() {
    return true;
}
/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool OPCUADSInput::GetSignalMemoryBuffer(const uint32 signalIdx,
                                         const uint32 bufferIdx,
                                         void *&signalAddress) {
    StreamString opcDisplayName;
    bool ok = GetSignalName(signalIdx, opcDisplayName);
    if (ok) {
        if (types != NULL_PTR(TypeDescriptor*)) {
            /*lint -e{9007}  [MISRA C++ Rule 5-14-1] Justification: No side effects.*/
            if ((types[signalIdx].type == CArray) || (types[signalIdx].type == BT_CCString) || (types[signalIdx].type == PCString)
                    || (types[signalIdx].type == SString)) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Type String is not supported yet.");
            }
            if ((nElements != NULL_PTR(uint32*)) && (masterClient != NULL_PTR(OPCUAClientRead*))) {
                ok = masterClient->GetSignalMemory(signalAddress, signalIdx, types[signalIdx], nElements[signalIdx]);
            }
        }
    }
    if (ok) {
        ok = (signalAddress != NULL_PTR(void*));
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction */
const char8* OPCUADSInput::GetBrokerName(StructuredDataI &data,
                                         const SignalDirection direction) {
    const char8 *brokerName = "";
    if (sync == "no") {
        if (direction == InputSignals) {
            brokerName = "MemoryMapInputBroker";
        }
    }
    else if (sync == "yes") {
        if (direction == InputSignals) {
            brokerName = "MemoryMapSynchronisedInputBroker";
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Impossible to define a broker. Check Synchronise option.");
    }
    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool OPCUADSInput::PrepareNextState(const char8 *const currentStateName,
                                    const char8 *const nextStateName) {
    return true;
}

ErrorManagement::ErrorType OPCUADSInput::Execute(ExecutionInfo &info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        if ((types != NULL_PTR(TypeDescriptor*)) && (nElements != NULL_PTR(uint32*)) && (masterClient != NULL_PTR(OPCUAClientRead*))) {
            bool ok;
            if (readMode == "Read") {
                ok = masterClient->Read(types, nElements);
                if (!ok) {
                    err = ErrorManagement::CommunicationError;
                }
            }
            else if (readMode == "Monitor") {
                err = ErrorManagement::UnsupportedFeature;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "ReadMode defines an unsupported service.");
            }
        }
    }
    return err;
}

bool OPCUADSInput::Synchronise() {
    bool ok = true;
    if (sync == "yes") {
        if ((types != NULL_PTR(TypeDescriptor*)) && (nElements != NULL_PTR(uint32*)) && (masterClient != NULL_PTR(OPCUAClientRead*))) {
            if (readMode == "Read") {
                ok = masterClient->Read(types, nElements);
            }
            else if (readMode == "Monitor") {
                ok = false;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "ReadMode defines an unsupported service.");
            }
        }
    }
    return ok;
}

const char8* OPCUADSInput::GetServerAddress() {
    return serverAddress.Buffer();
}

bool OPCUADSInput::GetBodyLength(const Introspection *const intro,
                                 uint32 &bodyLength) {
    bool ok = true;
    uint32 numberOfMembers = intro->GetNumberOfMembers();
    uint32 j;
    for (j = 0u; j < numberOfMembers; j++) {
        const IntrospectionEntry entry = intro->operator[](j);
        const char8 *const memberTypeName = entry.GetMemberTypeName();
        uint32 nElem;
        nElem = entry.GetNumberOfElements(0u);
        if (nElem > 1u) {
            bodyLength += 4u;
        }
        bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
        if (isStructured) {
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
            ok = (cri != NULL_PTR(const ClassRegistryItem*));
            for (uint32 h = 0u; h < nElem; h++) {
                if (ok) {
                    ok = GetBodyLength(cri->GetIntrospection(), bodyLength);
                }
            }
        }
        else {
            uint32 nOfBytes = entry.GetMemberTypeDescriptor().numberOfBits;
            nOfBytes /= 8u;
            nOfBytes *= nElem;
            bodyLength = bodyLength + nOfBytes;
        }
    }
    return ok;
}

void OPCUADSInput::GetStructureDimensions(const Introspection *const intro,
                                          uint32 &arraySize) {
    bool ok = true;
    uint32 numberOfMembers = intro->GetNumberOfMembers();

    for (uint32 j = 0u; j < numberOfMembers; j++) {
        arraySize++;
        const IntrospectionEntry entry = intro->operator[](j);
        const char8 *const memberTypeName = entry.GetMemberTypeName();
        bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
        if (isStructured) {
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
            ok = (cri != NULL_PTR(const ClassRegistryItem*));
            if (ok) {
                GetStructureDimensions(cri->GetIntrospection(), arraySize);
            }
        }
    }
}

bool OPCUADSInput::GetStructure(const Introspection * const intro,
                                uint32 *&entryArrayElements,
                                TypeDescriptor *&entryTypes,
                                uint32 *&entryNumberOfMembers,
                                uint32 &index) {
    bool ok = true;
    uint32 numberOfMembers = intro->GetNumberOfMembers();

    uint32 j;
    for (j = 0u; j < numberOfMembers; j++) {
        const IntrospectionEntry entry = intro->operator[](j);

        /* Updating entryArrayElements */
        entryArrayElements[index] = entry.GetNumberOfElements(0u);

        /* Updating entryTypes */
        entryTypes[index] = entry.GetMemberTypeDescriptor();

        const char8 *const memberTypeName = entry.GetMemberTypeName();
        uint32 nMembers = 1u;
        bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
        if (isStructured) {
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
            ok = (cri != NULL_PTR(const ClassRegistryItem*));
            nMembers = cri->GetIntrospection()->GetNumberOfMembers();
            /* Updating entryNumberOfMembers */
            entryNumberOfMembers[index] = nMembers;
            index = index + 1u;

            if (ok) {
                ok = GetStructure(cri->GetIntrospection(), entryArrayElements, entryTypes, entryNumberOfMembers, index);
            }
        }
        else {
            /* Updating entryNumberOfMembers */
            entryNumberOfMembers[index] = nMembers;
            index = index + 1u;
        }
    }
    return ok;
}

OPCUAClientRead* OPCUADSInput::GetOPCUAClient() {
    return masterClient;
}

CLASS_REGISTER(OPCUADSInput, "1.0");

}
/*lint -restore*/
