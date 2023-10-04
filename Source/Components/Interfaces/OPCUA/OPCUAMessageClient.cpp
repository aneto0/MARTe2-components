/**
 * @file OPCUAMessageClient.cpp
 * @brief Source file for class OPCUAMessageClient
 * @date 27/09/2019
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
 * the class OPCUAMessageClient (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "OPCUAMessageClient.h"
#include "TypeConversion.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*-e909 and -e9133 redefines bool. -e578 symbol ovveride in CLASS_REGISTER*/
/*lint -save -e909 -e9133 -e578*/
namespace MARTe {

OPCUAMessageClient::OPCUAMessageClient() :
        Object(),
        MessageI() {
    masterClient = NULL_PTR(OPCUAClientMethod*);
    nOfSignals = 0u;
    numberOfNodes = 0u;
    structuredTypeNames = NULL_PTR(StreamString*);
    tempPaths = NULL_PTR(StreamString*);
    tempNamespaceIndexes = NULL_PTR(uint16*);
    tempNElements = NULL_PTR(uint32*);
    serverAddress = "";
    entryArrayElements = NULL_PTR(uint32*);
    entryNumberOfMembers = NULL_PTR(uint32*);
    entryMemberNames = NULL_PTR(const char8**);
    entryArraySize = 0u;
    entryTypes = NULL_PTR(TypeDescriptor*);
    values = NULL_PTR(AnyType**);
    methodNamespaceIndex = 0u;
    methodPath = "";
    signalAddresses = NULL_PTR(void**);
    filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filter");
    }
}

OPCUAMessageClient::~OPCUAMessageClient() {
    if (structuredTypeNames != NULL_PTR(StreamString*)) {
        /*lint -e{1551} no exception on delete*/
        delete[] structuredTypeNames;
    }
    if (tempPaths != NULL_PTR(StreamString*)) {
        /*lint -e{1551} no exception on delete*/
        delete[] tempPaths;
    }
    if (tempNamespaceIndexes != NULL_PTR(uint16*)) {
        delete[] tempNamespaceIndexes;
    }
    if (tempNElements != NULL_PTR(uint32*)) {
        delete[] tempNElements;
    }
    if (entryArrayElements != NULL_PTR(uint32*)) {
        delete[] entryArrayElements;
    }
    if (entryNumberOfMembers != NULL_PTR(uint32*)) {
        delete[] entryNumberOfMembers;
    }
    if (entryMemberNames != NULL_PTR(const char8**)) {
        delete[] entryMemberNames;
    }
    if (entryTypes != NULL_PTR(TypeDescriptor*)) {
        delete[] entryTypes;
    }
    if (values != NULL_PTR(AnyType**)) {
        for (uint32 i = 0u; i < numberOfNodes; i++) {
            if (values[i] != NULL_PTR(AnyType*)) {
                /*lint -e{1551} no exception on delete*/
                delete[] reinterpret_cast<uint8*>(values[i]->GetDataPointer());
            }
            /*lint -e{1551} no exception on delete*/
            delete values[i];
        }
        delete[] values;
    }
    if (signalAddresses != NULL_PTR(void**)) {
        delete[] signalAddresses;
    }
    if (masterClient != NULL_PTR(OPCUAClientMethod*)) {
        /*lint -e{1551} no exception on delete*/
        delete masterClient;
    }
}

bool OPCUAMessageClient::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Read("Address", serverAddress);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the Address attribute");
        }
        /* Reading Method Informations */
        if (ok) {
            ok = data.MoveRelative("Method");
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Method section");
            }
        }
        if (ok) {
            ok = data.Read("NamespaceIndex", methodNamespaceIndex);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the NamespaceIndex attribute for the Method");
            }
        }
        if (ok) {
            ok = data.Read("Path", methodPath);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the Path attribute for the Method");
            }
        }
        if (ok) {
            ok = data.MoveToAncestor(1u);
        }
        /* Reading Structure Informations */
        if (ok) {
            ok = data.MoveRelative("Structure");
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Structure section");
            }
        }
        if (ok) {
            /* Do not allow to add signals in run-time */
            ok = data.Write("Locked", 1u);
            nOfSignals = data.GetNumberOfChildren() - 1u;
            tempPaths = new StreamString[nOfSignals];
            tempNamespaceIndexes = new uint16[nOfSignals];
            tempNElements = new uint32[nOfSignals];
            structuredTypeNames = new StreamString[nOfSignals];
            for (uint32 i = 0u; (i < nOfSignals) && (ok); i++) {
                ok = data.MoveRelative(data.GetChildName(i));
                if (ok) {
                    ok = data.Read("Path", tempPaths[i]);
                    if (!ok) {
                        uint32 k = i;
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the Path attribute from signal %d", k);
                    }
                }
                if (ok) {
                    ok = data.Read("NamespaceIndex", tempNamespaceIndexes[i]);
                    if (!ok) {
                        uint32 k = i;
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the NamespaceIndex attribute from signal %d", k);
                    }
                }
                if (ok) {
                    ok = data.Read("NumberOfElements", tempNElements[i]);
                    if (!ok) {
                        tempNElements[i] = 1u;
                        ok = true;
                        uint32 k = i;
                        REPORT_ERROR(ErrorManagement::Information, "No NumberOfElements set up for signal %d. Using N = 1", k);
                    }
                }
                if (ok) {
                    ok = data.Read("Type", structuredTypeNames[i]);
                    if (!ok) {
                        uint32 k = i;
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the Type attribute from signal %d", k);
                    }
                }
                if (ok) {
                    ok = data.MoveToAncestor(1u);
                }
            }
        }
        if (ok) {
            ok = data.MoveToAncestor(1u);
        }
        if (ok) {
            uint32 bodyLength = 0u;
            for (uint32 k = 0u; k < nOfSignals; k++) {
                if (structuredTypeNames != NULL_PTR(StreamString*)) {
                    const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(structuredTypeNames[k].Buffer());
                    if (cri != NULL_PTR(const ClassRegistryItem*)) {
                        const Introspection *intro = cri->GetIntrospection();
                        ok = (intro != NULL_PTR(const Introspection*));
                        if (ok) {
                            GetStructureDimensions(intro, entryArraySize);

                            entryArrayElements = new uint32[entryArraySize];
                            entryNumberOfMembers = new uint32[entryArraySize];
                            entryTypes = new TypeDescriptor[entryArraySize];
                            entryMemberNames = new const char8*[entryArraySize];

                            uint32 index = 0u;
                            ok = GetStructure(intro, entryArrayElements, entryTypes, entryNumberOfMembers, entryMemberNames, index);
                            if (ok) {
                                ok = GetBodyLength(intro, bodyLength);
                            }
                            if (ok) {
                                if (tempNElements != NULL_PTR(uint32*)) {
                                    bodyLength *= tempNElements[k];
                                }
                            }
                            ok = GetNumberOfNodes(intro, numberOfNodes);
                            if (ok) {
                                numberOfNodes *= tempNElements[k];
                                values = new AnyType*[numberOfNodes];
                                /* Initialising Signal memories */
                                signalAddresses = new void*[numberOfNodes];
                                for (uint32 j = 0u; j < numberOfNodes; j++) {
                                    signalAddresses[j] = NULL_PTR(void*);
                                    values[j] = NULL_PTR(AnyType*);
                                }
                            }
                        }
                    }
                }
            }
            if (ok) {
                /* Setting up OPCUA Client */
                masterClient = new OPCUAClientMethod();
                masterClient->SetServerAddress(serverAddress);
                ok = masterClient->Connect();
                if (ok) {
                    REPORT_ERROR(ErrorManagement::Information, "The connection with the OPCUA Server has been established successfully!");
                }
                if (ok) {
                    ok = masterClient->SetObjectRequest(methodNamespaceIndex, methodPath);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot find selected method in the Server.");
                    }
                }
                if (ok) {
                    ok = masterClient->SetMethodRequest(methodNamespaceIndex, methodPath);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot find selected method in the Server.");
                    }
                }
                if (ok) {
                    ok = masterClient->SetServiceRequest(tempNamespaceIndexes, tempPaths, nOfSignals);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot find one or more signals in the Server.");
                    }
                }
                if (ok) {
                    masterClient->SetValueMemories(numberOfNodes);
                    masterClient->SetDataPtr(bodyLength);
                    REPORT_ERROR(ErrorManagement::Information, "Encoding Structure (%d nodes) into ByteString", numberOfNodes);
                    for (uint32 k = 0u; k < nOfSignals; k++) {
                        uint32 nodeCounter = 0u;
                        uint32 index;
                        if (tempNElements != NULL_PTR(uint32*)) {
                            for (uint32 j = 0u; j < tempNElements[k]; j++) {
                                index = 0u;
                                uint32 numberOfNodesForEachIteration = (numberOfNodes / tempNElements[k]) * (j + 1u);
                                while (nodeCounter < numberOfNodesForEachIteration) {
                                    if (ok) {
                                        ok = masterClient->GetExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize,
                                                                                        nodeCounter, index);
                                    }
                                }
                            }
                        }
                    }
                    ok = masterClient->SetExtensionObject();
                }
            }
        }
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Error during Initialise!");
    }
    return ok;
}

ErrorManagement::ErrorType OPCUAMessageClient::OPCUAMethodCall(StructuredDataI &data) {
    ErrorManagement::ErrorType err = ErrorManagement::IllegalOperation;
    bool ok = true;
    REPORT_ERROR(ErrorManagement::Information, "Mapping StructuredData");
    for (uint32 k = 0u; k < nOfSignals; k++) {
        uint32 nodeCounter = 0u;
        uint32 index;
        if (tempNElements != NULL_PTR(uint32*)) {
            for (uint32 j = 0u; j < tempNElements[k]; j++) {
                index = 0u;
                uint32 numberOfNodesForEachIteration = (numberOfNodes / tempNElements[k]) * (j + 1u);
                while (nodeCounter < numberOfNodesForEachIteration) {
                    ok = MapStructuredData(data, values, index, nodeCounter);
                    if (!ok) {
                        break;
                    }
                }
            }
        }
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "The received structure does not match the declared one!");
    }
    if (ok) {
        if (masterClient != NULL_PTR(OPCUAClientMethod*)) {
            REPORT_ERROR(ErrorManagement::Information, "OPCUA Method Call");
            ok = masterClient->MethodCall();
        }
        if (ok) {
            err = ErrorManagement::NoError;
        }
    }
    return err;
}

const char8* OPCUAMessageClient::GetServerAddress() const {
    StreamString addr = serverAddress;
    return addr.Buffer();
}

void OPCUAMessageClient::GetStructureDimensions(const Introspection *const intro,
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

bool OPCUAMessageClient::GetStructure(const Introspection *const intro,
                                      uint32 *&entryArrayElements,
                                      TypeDescriptor *&entryTypes,
                                      uint32 *&entryNumberOfMembers,
                                      const char8 **&entryMemberNames,
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

        /* Updating entryMemberNames */
        entryMemberNames[index] = entry.GetMemberName();

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
                ok = GetStructure(cri->GetIntrospection(), entryArrayElements, entryTypes, entryNumberOfMembers, entryMemberNames, index);
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

bool OPCUAMessageClient::GetBodyLength(const Introspection *const intro,
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

bool OPCUAMessageClient::GetNumberOfNodes(const Introspection *const intro,
                                          uint32 &nOfNodes) {
    bool ok = true;
    uint32 numberOfMembers = intro->GetNumberOfMembers();
    uint32 j;
    for (j = 0u; j < numberOfMembers; j++) {
        const IntrospectionEntry entry = intro->operator[](j);
        const char8 *const memberTypeName = entry.GetMemberTypeName();
        uint32 nElem;
        nElem = entry.GetNumberOfElements(0u);
        bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
        if (isStructured) {
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
            ok = (cri != NULL_PTR(const ClassRegistryItem*));
            for (uint32 h = 0u; h < nElem; h++) {
                if (ok) {
                    ok = GetNumberOfNodes(cri->GetIntrospection(), nOfNodes);
                }
            }
        }
        else {
            nOfNodes++;
        }
    }
    return ok;
}

bool OPCUAMessageClient::MapStructuredData(StructuredDataI &data,
                                           AnyType **&values,
                                           uint32 &index,
                                           uint32 &nodeCounter) {
    bool ok = true;
    if ((entryTypes != NULL_PTR(TypeDescriptor*)) && (entryArrayElements != NULL_PTR(uint32*)) && (entryMemberNames != NULL_PTR(const char8**))) {
        if (!entryTypes[index].isStructuredData) {
            if (entryArrayElements[index] == 1u) {
                uint32 nOfBytes = entryTypes[index].numberOfBits;
                nOfBytes /= 8u;
                values[nodeCounter] = new AnyType(entryTypes[index], 0u, HeapManager::Malloc(nOfBytes));
                ok = data.Read(entryMemberNames[index], *values[nodeCounter]);
                if (ok) {
                    if ((signalAddresses != NULL_PTR(void**)) && (masterClient != NULL_PTR(OPCUAClientMethod*))) {
                        ok = masterClient->GetSignalMemory(signalAddresses[nodeCounter], nodeCounter, entryTypes[index], entryArrayElements[index]);
                        if (ok) {
                            ok = MemoryOperationsHelper::Copy(signalAddresses[nodeCounter], values[nodeCounter]->GetDataPointer(), nOfBytes);
                        }
                    }
                }
            }
            else {
                uint32 nOfBytes = entryTypes[index].numberOfBits;
                nOfBytes /= 8u;
                nOfBytes *= entryArrayElements[index];
                uint8 *mem = new uint8[nOfBytes];
                values[nodeCounter] = new AnyType(entryTypes[index], 0u, mem);
                values[nodeCounter]->SetNumberOfDimensions(1u);
                values[nodeCounter]->SetNumberOfElements(0u, entryArrayElements[index]);
                AnyType source = data.GetType(entryMemberNames[index]);
                ok = TypeConvert(*values[nodeCounter], source);

                if (ok) {
                    if ((signalAddresses != NULL_PTR(void**)) && (masterClient != NULL_PTR(OPCUAClientMethod*))) {
                        ok = masterClient->GetSignalMemory(signalAddresses[nodeCounter], nodeCounter, entryTypes[index], entryArrayElements[index]);
                        if (ok) {
                            ok = MemoryOperationsHelper::Copy(signalAddresses[nodeCounter], values[nodeCounter]->GetDataPointer(), nOfBytes);
                        }
                    }
                }
                /*lint -e{429} mem is deleted into the destructor from Anytype->GetDataPointer*/
            }
            if (ok) {
                nodeCounter++;
                index++;
            }
        }
        else { /* if isStructuredData */
            if (entryNumberOfMembers != NULL_PTR(uint32*)) {
                uint32 newIndex;
                if (entryArrayElements[index] == 1u) {
                    index++;
                    for (uint32 j = 0u; j < entryNumberOfMembers[index - 1u]; j++) {
                        if (ok) {
                            ok = data.MoveRelative(entryMemberNames[index]);
                            if (ok) {
                                ok = MapStructuredData(data, values, index, nodeCounter);
                            }
                            if (ok) {
                                ok = data.MoveToAncestor(1u);
                            }
                        }
                    }
                }
                if (entryArrayElements[index] > 1u) {
                    StreamString childName;
                    if (ok) {
                        uint32 nMembers = entryNumberOfMembers[index];
                        for (uint32 i = 0u; i < entryArrayElements[index]; i++) {
                            newIndex = index + 1u;
                            for (uint32 j = 0u; j < entryNumberOfMembers[index]; j++) {
                                if (ok) {
                                    ok = childName.Seek(0LLU);
                                    if (ok) {
                                        childName = entryMemberNames[index];
                                        childName += "[";
                                        uint32 k = i;
                                        ok = childName.Printf("%d", k);
                                    }
                                    if(ok){
                                        childName += "]";
                                        ok = data.MoveRelative(childName.Buffer());
                                    }
                                    if (ok) {
                                        ok = MapStructuredData(data, values, newIndex, nodeCounter);
                                    }
                                    if (ok) {
                                        childName = "";
                                        ok = data.MoveToAncestor(1u);
                                    }
                                }
                            }
                        }
                        index += (nMembers + 1u);
                    }
                }
            }
        }
    }
    return ok;
}

OPCUAClientMethod* OPCUAMessageClient::GetOPCUAClient() {
    return masterClient;
}

CLASS_REGISTER(OPCUAMessageClient, "")
/*lint -e{1023} There is no ambiguity on the function to be called as the compiler can distinguish between both template definitions.*/
CLASS_METHOD_REGISTER(OPCUAMessageClient, OPCUAMethodCall)
}

/*lint -restore*/

