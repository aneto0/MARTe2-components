/**
 * @file OPCUAMessageClient.cpp
 * @brief Source file for class OPCUAMessageClient
 * @date Sep 27, 2019 TODO Verify the value and format of the date
 * @author lporzio TODO Verify the name and format of the author
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

#include "OPCUAMessageClient.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

OPCUAMessageClient::OPCUAMessageClient() :
        Object(),
        MessageI() {
    masterClient = NULL_PTR(OPCUAClientWrapper*);
    nOfSignals = 0u;
    numberOfNodes = 0u;
    paths = NULL_PTR(StreamString*);
    structuredTypeNames = NULL_PTR(StreamString*);
    namespaceIndexes = NULL_PTR(uint16*);
    tempPaths = NULL_PTR(StreamString*);
    tempNamespaceIndexes = NULL_PTR(uint16*);
    tempNElements = NULL_PTR(uint32*);
    serverAddress = "";
    nElements = NULL_PTR(uint32*);
    entryArrayElements = NULL_PTR(uint32*);
    entryNumberOfMembers = NULL_PTR(uint32*);
    entryArraySize = 1u;
    types = NULL_PTR(TypeDescriptor*);
    entryTypes = NULL_PTR(TypeDescriptor*);
    extensionObject = NULL_PTR(StreamString*);
    filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filter");
    }
}

OPCUAMessageClient::~OPCUAMessageClient() {
    if (nElements != NULL_PTR(uint32*)) {
        delete[] nElements;
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
    if (structuredTypeNames != NULL_PTR(StreamString*)) {
        delete[] structuredTypeNames;
    }
    if (tempPaths != NULL_PTR(StreamString*)) {
        delete[] tempPaths;
    }
    if (tempNamespaceIndexes != NULL_PTR(uint16*)) {
        delete[] tempNamespaceIndexes;
    }
    if (masterClient != NULL_PTR(OPCUAClientWrapper*)) {
        delete masterClient;
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
    if (extensionObject != NULL_PTR(StreamString*)) {
        delete[] extensionObject;
    }
}

bool OPCUAMessageClient::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Read("Address", serverAddress);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the Address attribute");
        }
        if (ok) {
            ok = data.MoveRelative("Structure");
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
            }
        }
        /* Do not allow to add signals in run-time */
        if (ok) {
            ok = data.Write("Locked", 1u);
            nOfSignals = data.GetNumberOfChildren();
            tempPaths = new StreamString[nOfSignals];
            tempNamespaceIndexes = new uint16[nOfSignals];
            extensionObject = new StreamString[nOfSignals];
            tempNElements = new uint32[nOfSignals];
            structuredTypeNames = new StreamString[nOfSignals];
            for (uint32 i = 0u; (i < nOfSignals) && (ok); i++) {
                if (ok) {
                    ok = data.MoveRelative(data.GetChildName(i));
                }
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
                    data.Read("NumberOfElements", tempNElements[i]);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the NumberOfelements of signal %d", i);
                    }
                }
                if (ok) {
                    ok = data.Read("ExtensionObject", extensionObject[i]);
                    if (ok && (extensionObject[i] == "yes")) {
                        ok = data.Read("Type", structuredTypeNames[i]);
                        if (!ok) {
                            uint32 k = i;
                            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the Type attribute from signal %d", k);
                        }
                        entryArrayElements = new uint32[entryArraySize];
                        entryNumberOfMembers = new uint32[entryArraySize];
                        entryTypes = new TypeDescriptor[entryArraySize];
                        REPORT_ERROR(ErrorManagement::Information, "Reading Structure with OPC UA Complex DataType Extension");
                    }
                    else {
                        extensionObject[i] = "no";
                        ok = true;
                    }
                }
                if (ok) {
                    ok = data.MoveToAncestor(1u);
                }
            }
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Error while Initialisation.");
        }
        if (ok) {
            ok = data.MoveToAncestor(1u);
        }
        if (ok) {
            uint32 bodyLength = 0u;
            /* Getting Structure information for building OPCUA Request */
            for (uint32 j = 0u; j < nOfSignals; j++) {
                if (ok) {
                    const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(structuredTypeNames[j].Buffer());
                    if (cri != NULL_PTR(const ClassRegistryItem*)) {
                        const Introspection *intro = cri->GetIntrospection();
                        ok = (intro != NULL_PTR(const Introspection*));
                        if (ok) {
                            ok = GetStructure(intro, entryArrayElements, entryTypes, entryNumberOfMembers, entryArraySize);
                        }
                        if (ok) {
                            ok = GetBodyLength(intro, bodyLength);
                        }
                        if (ok) {
                            bodyLength *= tempNElements[j];
                        }

                        if (ok) {
                            ok = GetNumberOfNodes(intro, numberOfNodes);
                        }
                    }
                }
            }
            if (ok) {
                /* Setting up OPCUA Client */
                masterClient = new OPCUAClientWrapper();
                masterClient->SetOperationMode("Write");
                masterClient->SetServerAddress(serverAddress);
                ok = masterClient->Connect();
                if (ok) {
                    REPORT_ERROR(ErrorManagement::Information, "The connection with the OPCUA Server has been established successfully!");
                }
                ok = masterClient->SetTargetNodes(tempNamespaceIndexes, tempPaths, nOfSignals);
                if (ok) {
                    masterClient->SetValueMemories(numberOfNodes);
                    masterClient->SetDataPtr(bodyLength);
                    for (uint32 k = 0u; k < nOfSignals; k++) {
                        uint32 nodeCounter = 0u;
                        uint32 index;
                        for (uint32 j = 0u; j < tempNElements[k]; j++) {
                            index = 0u;
                            uint32 numberOfNodesForEachIteration = (numberOfNodes / tempNElements[k]) * (j + 1);
                            while (nodeCounter < numberOfNodesForEachIteration) {
                                masterClient->EncodeExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter,
                                                                              index);
                            }
                        }
                    }
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Cannot find one or more signals in the Server.");
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
    ErrorManagement::ErrorType err;
    if (masterClient != NULL_PTR(OPCUAClientWrapper*)) {
        masterClient->MethodCall();
    }
    return err;
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
                                      uint32 &arraySize) {
    bool ok = true;
    uint32 numberOfMembers = intro->GetNumberOfMembers();

    uint32 j;
    for (j = 0u; j < numberOfMembers; j++) {
        const IntrospectionEntry entry = intro->operator[](j);

        /* Updating entryArrayElements */
        uint32 *tempArray = new uint32[arraySize + 1u];
        for (uint32 k = 0u; k < arraySize; k++) {
            tempArray[k] = entryArrayElements[k];
        }
        tempArray[arraySize] = entry.GetNumberOfElements(0u);
        delete[] entryArrayElements;
        entryArrayElements = tempArray;

        /* Updating entryTypes */
        TypeDescriptor *tempTypes = new TypeDescriptor[arraySize + 1u];
        for (uint32 k = 0u; k < arraySize; k++) {
            tempTypes[k] = entryTypes[k];
        }
        tempTypes[arraySize] = entry.GetMemberTypeDescriptor();
        delete[] entryTypes;
        entryTypes = tempTypes;

        const char8 *const memberTypeName = entry.GetMemberTypeName();

        uint32 nMembers = 1u;

        bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
        if (isStructured) {
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
            ok = (cri != NULL_PTR(const ClassRegistryItem*));
            nMembers = cri->GetIntrospection()->GetNumberOfMembers();
            /* Updating entryNumberOfMembers */
            uint32 *tempMembers = new uint32[arraySize + 1u];
            for (uint32 k = 0u; k < arraySize; k++) {
                tempMembers[k] = entryNumberOfMembers[k];
            }
            tempMembers[arraySize] = nMembers;
            arraySize = arraySize + 1u;
            delete[] entryNumberOfMembers;
            entryNumberOfMembers = tempMembers;

            if (ok) {
                ok = GetStructure(cri->GetIntrospection(), entryArrayElements, entryTypes, entryNumberOfMembers, arraySize);
            }
        }
        else {
            /* Updating entryNumberOfMembers */
            uint32 *tempMembers = new uint32[arraySize + 1u];
            for (uint32 k = 0u; k < arraySize; k++) {
                tempMembers[k] = entryNumberOfMembers[k];
            }
            tempMembers[arraySize] = nMembers;
            arraySize = arraySize + 1u;

            delete[] entryNumberOfMembers;
            entryNumberOfMembers = tempMembers;
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
        uint32 nElem = 1u;
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
        uint32 nElem = 1u;
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
            for (uint32 h = 0u; h < nElem; h++) {
                nOfNodes++;
            }
        }
    }
    return ok;
}

CLASS_REGISTER(OPCUAMessageClient, "")
/*lint -e{1023} There is no ambiguity on the function to be called as the compiler can distinguish between both template definitions.*/
CLASS_METHOD_REGISTER(OPCUAMessageClient, OPCUAMethodCall)
}

