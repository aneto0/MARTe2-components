/**
 * @file OPCUADSOutput.cpp
 * @brief Source file for class OPCUADSOutput
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
 * the class OPCUADSOutput (public, protected, and private). Be aware that some 
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
#include "AuthUtils.h"
#include "File.h"
#include "StructuredDataIHelper.h"
#include "OPCUADSOutput.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/*-e909 and -e9133 redefines bool. -e578 symbol ovveride in CLASS_REGISTER*/
/*lint -save -e909 -e9133 -e578*/
namespace MARTe {

OPCUADSOutput::OPCUADSOutput() :
        DataSourceI() {
    masterClient = NULL_PTR(OPCUAClientWrite*);
    nOfSignals = 0u;
    numberOfNodes = 0u;
    isExtensionObject = false;
    paths = NULL_PTR(StreamString*);
    namespaceIndexes = NULL_PTR(uint16*);
    structuredTypeName = "";
    signalIdxMap = NULL_PTR(uint32*);
    timestampSignals = NULL_PTR(uint64*);
    serverAddress = "";
    entryArrayElements = NULL_PTR(uint32*);
    entryNumberOfMembers = NULL_PTR(uint32*);
    entryArraySize = 0u;
    entryTypes = NULL_PTR(TypeDescriptor*);
    nElements = NULL_PTR(uint32*);
    types = NULL_PTR(TypeDescriptor*);
    authenticate = false;
    username = "";
    password = "";
}

/*lint -e{1551} No exception thrown.*/
OPCUADSOutput::~OPCUADSOutput() {
    if (masterClient != NULL_PTR(OPCUAClientWrite*)) {
        delete masterClient;
    }
    if (nElements != NULL_PTR(uint32*)) {
        delete[] nElements;
    }
    if (types != NULL_PTR(TypeDescriptor*)) {
        delete[] types;
    }
    if (paths != NULL_PTR(StreamString*)) {
        delete[] paths;
    }
    if (namespaceIndexes != NULL_PTR(uint16*)) {
        delete[] namespaceIndexes;
    }
    if (signalIdxMap != NULL_PTR(uint32*)) {
        delete[] signalIdxMap;
    }
    if (timestampSignals != NULL_PTR(uint64*)) {
        delete[] timestampSignals;
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

bool OPCUADSOutput::Initialise(StructuredDataI &data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("Address", serverAddress);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot read the Address attribute");
        }
        StreamString authentication;
        if (ok) {
            if (!data.Read("Authentication", authentication)) {
                authentication = "None";
                REPORT_ERROR(ErrorManagement::Warning, "'Authentication' not defined.");
            }
        }
        if (ok) {
            if (authentication == "None") {
                authenticate = false;
                REPORT_ERROR(ErrorManagement::Information, "Using 'None' authentication.");
            }
            else if (authentication == "UserPassword") {
                authenticate = true;
                REPORT_ERROR(ErrorManagement::Information, "Using 'UserPassword' authentication.");
            }
            else {
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "'Authentication' parameter invalid (expected 'None' or 'UserPassword')!");
            }
        }
        if (ok && authenticate) {
            StreamString userPasswordFile;
            ok = data.Read("UserPasswordFile", userPasswordFile);
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "'UserPasswordFile' not defined!");
            }
            File f;
            if (ok) {
                (void) userPasswordFile.Seek(0LLU);
                ok = f.Open(userPasswordFile.Buffer(), BasicFile::ACCESS_MODE_R);
                if (!ok) {
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to open the file at path '%s'!", userPasswordFile.Buffer());
                }
            }
            if (ok) {
                ok = ReadAuthenticationKey(f, username, password);
            }
            if (f.IsOpen()) {
                ok = f.Close();
                if (!ok) {
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to close the 'UserPasswordFile'!");
                }
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
        }
        if (ok) {
            ok = data.Copy(tempSignalsDatabase);
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

uint32 OPCUADSOutput::GetNumberOfNodes() {
    uint32 numberOfNodes = 0u;
    uint32 numberOfSignals = GetNumberOfSignals();
    (void) timestampDatabase.MoveToRoot();
    uint32 numberOfTimestampSignals = timestampDatabase.GetNumberOfChildren();
    return numberOfSignals - numberOfTimestampSignals;
}

bool OPCUADSOutput::PopulateTimestampDatabase() {
    bool ok = tempSignalsDatabase.MoveToRoot();
    uint32 nNodes = tempSignalsDatabase.GetNumberOfChildren();
    StructuredDataIHelper helper(tempSignalsDatabase, this);
    for (uint32 i = 0u; (i < nNodes) && (ok); i++) {
        ok = tempSignalsDatabase.MoveRelative(tempSignalsDatabase.GetChildName(i));
        bool isDefaultTimestamp = false;
        if (ok) {
            uint32 defaultTimeStampSignal;
            isDefaultTimestamp = tempSignalsDatabase.Read("DefaultTimestampSignal", defaultTimeStampSignal);
            if (isDefaultTimestamp) {
                isDefaultTimestamp = (defaultTimeStampSignal == 1u);
            }
            if (isDefaultTimestamp) {
                uint32 ii = i;
                StreamString nname;
                //Identify the timestamp signal by index
                (void)nname.Printf("%d", ii);
                ok = timestampDatabase.CreateAbsolute(nname.Buffer());
                if (ok) {
                    ok = timestampDatabase.Write("DefaultTimestampSignal", defaultTimeStampSignal);
                }
            }
        }
        bool isTimestampSignal = isDefaultTimestamp;
        if (ok) {
            StreamString *timestampTargets = NULL_PTR(StreamString *);
            uint32 numberOfTimestampTargets;
            if (!isTimestampSignal) {
                isTimestampSignal = helper.ReadArray("Timestamp", timestampTargets, numberOfTimestampTargets);
                helper.ResetErrors();
            }
            if (isTimestampSignal) {
                numberOfNodes--;
                uint32 ii = i;
                StreamString nname;
                (void)nname.Printf("%d", ii);
                ok = timestampDatabase.CreateAbsolute(nname.Buffer());
                if (ok) {
                    ok = timestampDatabase.CreateRelative("TargetSignals");
                }
                for (uint32 n=0; (n<numberOfTimestampTargets) && (ok); n++) {
                    nname = "";
                    ii = n;
                    (void)nname.Printf("%d", ii);
                    ok = timestampDatabase.Write(nname.Buffer(), timestampTargets[n].Buffer());
                }
            }
        }
        if (ok) {
            ok = tempSignalsDatabase.MoveToAncestor(1u);
        }
    }
    return ok;
}

bool OPCUADSOutput::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = PopulateTimestampDatabase();
    }
    if (ok) {
        numberOfNodes = GetNumberOfNodes();
    }
    uint8 nDimensions = 0u;
    nElements = new uint32[numberOfNodes];
    types = new TypeDescriptor[numberOfNodes];

    uint32 s=0u;
    for (uint32 k=0; (k < numberOfSignals) && (ok); k++) {
        bool isTimestampSignal = IsTimestampSignal(k);
        if (!isTimestampSignal) {
            ok = GetSignalNumberOfDimensions(k, nDimensions);
            if ((nDimensions > 1u) && ok) {
                StreamString signalName;
                ok = GetSignalName(k, signalName);
                if (ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Signal %s has Number Of Dimensions = %d. Multidimensional arrays not supported yet.", signalName.Buffer(), nDimensions);
                    ok = false;
                }
            }
            if (ok) {
                ok = GetSignalNumberOfElements(k, nElements[s]);
            }
            if (ok) {
                types[s] = GetSignalType(k);
            }
            s++;
        }
    }

    uint32 bodyLength = 0u;
    if (isExtensionObject) {
        const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(structuredTypeName.Buffer());
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
                    bodyLength *= nElements[0u];
                }
            }
        }
    }
    else {
        char8 ignore;
        if (ok) {
            paths = new StreamString[numberOfNodes];
            namespaceIndexes = new uint16[numberOfNodes];
        }
        if (ok) {
            ok = tempSignalsDatabase.MoveToRoot();
        }
        uint32 k=0u;
        StructuredDataIHelper helper(tempSignalsDatabase, this);
        if (ok) {
            signalIdxMap = new uint32[numberOfSignals];
        }
        uint32 t=0u;
        for (uint32 i = 0u; (i < numberOfSignals) && (ok); i++) {
            bool isTimestampSignal = IsTimestampSignal(i);
            if (!isTimestampSignal) {
                ok = helper.MoveToChild(i);
                if (ok) {
                    ok = helper.Read("NamespaceIndex", namespaceIndexes[k]);
                }
                if (ok) {
                    ok = helper.Read("Path", paths[k]);
                }
                if (ok) {
                    ok = helper.MoveToAncestor(1u);
                }
                signalIdxMap[i] = k;
                k++;
            }
            else {
                signalIdxMap[i] = t;
                t++;
            }
        }
        if (t > 0u) {
            timestampSignals = new uint64[t];
        }
    }
    if (ok) {
        /* Setting up the master Client who will perform the operations */
        masterClient = new OPCUAClientWrite();
        masterClient->SetServerAddress(serverAddress);
        if (authenticate) {
            ok = masterClient->Connect(username, password);
        }
        else {
            ok = masterClient->Connect();
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not connect to the Server.");
        }
        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "The connection with the OPCUA Server has been established successfully!");
            if (!isExtensionObject) {
                ok = masterClient->SetServiceRequest(namespaceIndexes, paths, numberOfNodes);
                if (ok) {
                    masterClient->SetValueMemories(numberOfNodes);
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "SetServiceRequest Failed.");
                }
            }
            else {
                ok = (nElements != NULL_PTR(uint32*));
                if (ok) {
                    ok = masterClient->SetServiceRequest(namespaceIndexes, paths, numberOfNodes);
                }
                if (ok) {
                    masterClient->SetValueMemories(numberOfNodes);
                    masterClient->SetDataPtr(bodyLength);
                    for (uint32 k = 0u; (k < numberOfNodes) && (ok); k++) {
                        uint32 nodeCounter = 0u;
                        uint32 index;
                        for (uint32 j = 0u; (j < nElements[k]) && (ok); j++) {
                            index = 0u;
                            uint32 numberOfNodesForEachIteration = (numberOfNodes / nElements[k]) * (j + 1u);
                            while (nodeCounter < numberOfNodesForEachIteration) {
                                if (ok) {
                                    ok = masterClient->GetExtensionObjectByteString(entryTypes, entryArrayElements, entryNumberOfMembers, entryArraySize, nodeCounter, index);
                                }
                            }
                            if (ok) {
                                ok = masterClient->SetExtensionObject();
                            }
                        }
                    }
                }
            }
        }
    }
    if (ok) {
        tempSignalsDatabase.Purge();
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Error during configuration.");
    }
    return ok;
}

bool OPCUADSOutput::AllocateMemory() {
    return true;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool OPCUADSOutput::GetSignalMemoryBuffer(const uint32 signalIdx,
                                          const uint32 bufferIdx,
                                          void *&signalAddress) {
    bool ok = (masterClient != NULL_PTR(OPCUAClientWrite*)) && (nElements != NULL_PTR(uint32*));
    if (ok) {
        ok = (types != NULL_PTR(TypeDescriptor*));
    }
    if (ok) {
        ok = !((types[signalIdx].type == CArray) || (types[signalIdx].type == BT_CCString) || (types[signalIdx].type == PCString) || (types[signalIdx].type == SString));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Type String is not supported yet.");
        }
    }
    if (ok) {
        /*lint -e{9007}  [MISRA C++ Rule 5-14-1] Justification: No side effects.*/
        bool isTimestampSignal = IsTimestampSignal(signalIdx);
        uint32 nodeIdx = signalIdxMap[signalIdx];
        if (!isTimestampSignal) {
            ok = masterClient->GetSignalMemory(signalAddress, nodeIdx, types[nodeIdx], nElements[nodeIdx]);
            if ((!isExtensionObject) && ok) {
                uint8 nDimensions;
                if (nElements[nodeIdx] > 1u) {
                    nDimensions = 1u;
                }
                else {
                    nDimensions = 0u;
                }
                masterClient->SetWriteRequest(nodeIdx, nDimensions, nElements[nodeIdx], types[nodeIdx]);
            }
        }
        else {
            signalAddress = &timestampSignals[nodeIdx];
        }
        if (ok) {
            ok = (signalAddress != NULL_PTR(void*));
        }
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction */
const char8* OPCUADSOutput::GetBrokerName(StructuredDataI &data,
                                         const SignalDirection direction) {

    const char8 *brokerName = NULL_PTR(const char8*);

    if (direction == OutputSignals) {

        uint32 trigger = 0u;

        if (!data.Read("Trigger", trigger)) {
            trigger = 0u;
        }

        if (trigger == 1u) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
        
        }
        else {
            brokerName = "MemoryMapOutputBroker";
        }

    }
    else {
        
         REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with InputSignals");
        
    }

    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool OPCUADSOutput::PrepareNextState(const char8 *const currentStateName,
                                     const char8 *const nextStateName) {
    return true;
}

bool OPCUADSOutput::Synchronise() {
    bool ok = true;
    if (masterClient != NULL_PTR(OPCUAClientWrite*)) {
        ok = masterClient->Write();
    }
    return ok;
}

const char8* OPCUADSOutput::GetServerAddress() {
    return serverAddress.Buffer();
}

OPCUAClientWrite* OPCUADSOutput::GetOPCUAClient() {
    return masterClient;
}

bool OPCUADSOutput::GetBodyLength(const Introspection *const intro,
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

void OPCUADSOutput::GetStructureDimensions(const Introspection *const intro,
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

bool OPCUADSOutput::GetStructure(const Introspection *const intro,
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

bool OPCUADSOutput::IsTimestampSignal(const uint32 idx) {
    StreamString nn;
    (void) nn.Printf("%d", idx);
    bool isTimestampSignal = timestampDatabase.MoveAbsolute(nn.Buffer());
    return isTimestampSignal;

}

CLASS_REGISTER(OPCUADSOutput, "1.0")

}
/*lint -restore*/
