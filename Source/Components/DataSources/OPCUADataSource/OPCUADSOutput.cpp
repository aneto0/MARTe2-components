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
    numberOfNodes = 0u;
    isExtensionObject = false;
    paths = NULL_PTR(StreamString*);
    namespaceIndexes = NULL_PTR(uint16*);
    structuredTypeName = "";
    signalIdxMap = NULL_PTR(uint32*);
    timestampSignals = NULL_PTR(uint64*);
    timestampNodes = NULL_PTR(uint64**);
    serverAddress = "";
    entryArrayElements = NULL_PTR(uint32*);
    entryNumberOfMembers = NULL_PTR(uint32*);
    entryArraySize = 0u;
    entryTypes = NULL_PTR(TypeDescriptor*);
    nElements = NULL_PTR(uint32*);
    types = NULL_PTR(TypeDescriptor*);
    authenticate = false;
    triggerSignalSet = false;
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
    if (timestampSignals != NULL_PTR(uint64 *)) {
        delete[] timestampSignals;
    }
    if (timestampNodes != NULL_PTR(uint64 **)) {
        delete[] timestampNodes;
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
        uint32 nOfTempSignals = data.GetNumberOfChildren();
        StructuredDataIHelper helper(data, this);
        for (uint32 k=0u; (k<nOfTempSignals) && (ok); k++) {
            ok = data.MoveToChild(k);
            if (ok) {
                StreamString extensionObjectStr;
                bool isExtensionObjectSignal = data.Read("ExtensionObject", extensionObjectStr);
                if (isExtensionObjectSignal) {
                    isExtensionObjectSignal = (extensionObjectStr == "yes");
                }
                if (isExtensionObjectSignal) {
                    ok = !isExtensionObject;
                    if (ok) {
                        isExtensionObject = true;
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "At most one ExtensionObject shall be set per DataSource");
                    }
                }
                if (isExtensionObjectSignal) {
                    if (ok) {
                        ok = helper.Read("Type", structuredTypeName);
                    }
                    if (ok) {
                        paths = new StreamString[1u];
                        StreamString path;
                        ok = helper.Read("Path", path);
                        if (ok) {
                            paths[0] = path;
                        }
                    }
                    if (ok) {
                        namespaceIndexes = new uint16[1u];
                        uint16 namespaceIdx;
                        ok = helper.Read("NamespaceIndex", namespaceIdx);
                        if (ok) {
                            namespaceIndexes[0u] = namespaceIdx;
                        }
                    }
                    if (isExtensionObjectSignal) {
                        nElements = new uint32[1u];
                        types = new TypeDescriptor[1u];
                        uint32 numberOfElements;
                        (void) helper.Read("NumberOfElements", numberOfElements, 1u);
                        nElements[0u] = numberOfElements;
                    }
                }
            }
            if (ok) {
                ok = data.MoveToAncestor(1u);
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
    (void) timestampDatabase.MoveAbsolute("Sources");
    uint32 numberOfTimestampSignals = timestampDatabase.GetNumberOfChildren();
    return numberOfSignals - numberOfTimestampSignals;
}

bool OPCUADSOutput::PopulateTimestampDatabase() {
    bool ok = tempSignalsDatabase.MoveToRoot();
    if (ok) {
        ok = timestampDatabase.CreateAbsolute("Sources");
    }
    uint32 nSignalsTemp = tempSignalsDatabase.GetNumberOfChildren();
    StructuredDataIHelper helper(tempSignalsDatabase, this);
    bool defaultTimestampSignalFound = false;
    for (uint32 i = 0u; (i < nSignalsTemp) && (ok); i++) {
        ok = tempSignalsDatabase.MoveToChild(i);
        bool isDefaultTimestamp = false;
        if (ok) {
            uint32 defaultTimeStampSignal;
            isDefaultTimestamp = tempSignalsDatabase.Read("DefaultTimestampSignal", defaultTimeStampSignal);
            if (isDefaultTimestamp) {
                isDefaultTimestamp = (defaultTimeStampSignal == 1u);
            }
            if (isDefaultTimestamp) {
                ok = !defaultTimestampSignalFound;
                if (ok) {
                    uint32 ii = i;
                    if (isExtensionObject) {//The timesignal can be either the first or the last
                        if (ii > 0u) {
                            ii = GetNumberOfSignals() - 1u;
                        }
                    }
                    StreamString nname;
                    //Identify the timestamp signal by index
                    (void)nname.Printf("%d", ii);
                    ok = timestampDatabase.CreateRelative(nname.Buffer());
                    if (ok) {
                        ok = timestampDatabase.Write("DefaultTimestampSignal", defaultTimeStampSignal);
                    }
                    if (ok) {
                        ok = timestampDatabase.MoveToAncestor(1u);
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "At most one DefaultTimestampSignal shall be specified.");
                }
            }
        }
        bool isTimestampSignal = false;
        if (ok) {
            if (!isDefaultTimestamp) {
                StreamString *timestampTargets = NULL_PTR(StreamString *);
                uint32 numberOfTimestampTargets;
                isTimestampSignal = helper.ReadArray("Timestamp", timestampTargets, numberOfTimestampTargets);
                helper.ResetErrors();
                if (isTimestampSignal) {
                    uint32 ii = i;
                    StreamString nname;
                    (void)nname.Printf("%d", ii);
                    ok = timestampDatabase.CreateRelative(nname.Buffer());
                    for (uint32 n=0; (n<numberOfTimestampTargets) && (ok); n++) {
                        nname = "";
                        ii = n;
                        (void)nname.Printf("%d", ii);
                        ok = timestampDatabase.Write(nname.Buffer(), timestampTargets[n].Buffer());
                    }
                    if (ok) {
                        ok = timestampDatabase.MoveToAncestor(1u);
                    }
                }
            }
        }
        if (ok) {
            ok = tempSignalsDatabase.MoveToAncestor(1u);
        }
    }
    return ok;
}

uint32 OPCUADSOutput::SetupExtensionObject() {
    uint32 bodyLength = 0u;
    bool ok = false;
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
    if (!ok) {
        bodyLength = 0u;
    }
    return bodyLength;
}

bool OPCUADSOutput::SetupNodes() {
    uint8 nDimensions = 0u;
    nElements = new uint32[numberOfNodes];
    types = new TypeDescriptor[numberOfNodes];
    namespaceIndexes = new uint16[numberOfNodes];
    paths = new StreamString[numberOfNodes];
    bool ok = timestampDatabase.CreateAbsolute("NodesIdx");
    uint32 s=0u;
    for (uint32 k=0; (k < numberOfSignals) && (ok); k++) {
        bool isTimestampSignal = IsTimestampSignal(k);
        StreamString signalName;
        if (ok) {
            ok = GetSignalName(k, signalName);
        }

        if (!isTimestampSignal) {
            if (ok) {
                ok = GetSignalNumberOfDimensions(k, nDimensions);
            }
            if (ok) {
                ok = (nDimensions < 2u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Signal %s has Number Of Dimensions = %d. Multidimensional arrays not supported yet.", signalName.Buffer(), nDimensions);
                }
            }
            if (ok) {
                ok = GetSignalNumberOfElements(k, nElements[s]);
            }
            if (ok) {
                types[s] = GetSignalType(k);
                ok = !((types[s].type == CArray) || (types[s].type == BT_CCString) || (types[s].type == PCString) || (types[s].type == SString));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Type String is not supported yet.");
                }
            }
            if (ok) {
                ok = timestampDatabase.Write(signalName.Buffer(), s);
            }
            s++;
        }
        else {
            TypeDescriptor tt = GetSignalType(k);
            if (ok) {
                ok = (tt == UnsignedInteger64Bit);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Signal %s is a Timestamp signal and shall be of type uint64.", signalName.Buffer());
                }
            }
            uint32 nEl;
            if (ok) {
                ok = GetSignalNumberOfElements(k, nEl);
            }
            if (ok) {
                ok = (nEl < 2u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Signal %s is a Timestamp signal and shall be a scalar.", signalName.Buffer());
                }
            }
        }
    }
    return ok;

}

bool OPCUADSOutput::MapNodeSignals() {
    bool ok = tempSignalsDatabase.MoveToRoot();
    StructuredDataIHelper helper(tempSignalsDatabase, this);
    if (ok) {
        signalIdxMap = new uint32[numberOfSignals];
    }
    uint32 k=0u;
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
    return ok;
}

bool OPCUADSOutput::MapExtensionObjectSignals() {
    bool ok = true;
    signalIdxMap = new uint32[numberOfSignals];
    uint32 nOfOriginalSignals = tempSignalsDatabase.GetNumberOfChildren();
    if (nOfOriginalSignals == 1u) {
        for (uint32 k=0u; k<numberOfSignals; k++) {
            signalIdxMap[k] = k;
        }
    }
    else if (nOfOriginalSignals == 2u) {
        if (IsDefaultTimestampSignal(0u)) {
            signalIdxMap[0u] = 0u; //timing signal
            for (uint32 k=1u; k<numberOfSignals; k++) {
                signalIdxMap[k] = k - 1u;
            }
        }
        else if (IsDefaultTimestampSignal(numberOfSignals - 1u)) {
            for (uint32 k=0u; k<numberOfSignals; k++) {
                signalIdxMap[k] = k;
            }
            signalIdxMap[numberOfSignals - 1u] = 0u; //With extension objects the timing signal will always be at index 0 of timestampNodes
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "At most two signals can be specified when using ExtensionObject and one shall be a timing signal with the property DefaultTimestampSignal");
        }
        if (ok) {
            timestampSignals = new uint64[1u];
        }
    }
    else {
        ok = false;
        REPORT_ERROR(ErrorManagement::ParametersError, "At most two signals can be specified when using ExtensionObject and one shall be a timing signal");
    }
    return ok;
}

bool OPCUADSOutput::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = triggerSignalSet;
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "One Trigger signal shall be set.");
        }
    }
    if (ok) {
        ok = PopulateTimestampDatabase();
    }
    uint32 bodyLength;
    if (ok) {
        numberOfNodes = GetNumberOfNodes();
        if (isExtensionObject) {
            bodyLength = SetupExtensionObject();
            ok = (bodyLength > 0u);
        }
        else {
            ok = SetupNodes();
        }
    }
    if (ok) {
        if (isExtensionObject) {
            ok = MapExtensionObjectSignals();
        }
        else {
            ok = MapNodeSignals();
        }
    }
    if (ok) {
        ok = PopulateTimestampNodes();
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
    }
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "The connection with the OPCUA Server has been established successfully!");
        uint32 numberOfNodesRemote = numberOfNodes;
        if (isExtensionObject) {
            numberOfNodesRemote = 1u;
        }
        ok = masterClient->SetServiceRequest(namespaceIndexes, paths, numberOfNodesRemote);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "SetServiceRequest Failed.");
        }
    }
    if (ok) {
        masterClient->SetValueMemories(numberOfNodes);
        if (isExtensionObject) {
            ok = (nElements != NULL_PTR(uint32*));
            if (ok) {
                masterClient->SetDataPtr(bodyLength);
                uint32 nodeCounter = 0u;
                uint32 index;
                for (uint32 j = 0u; (j < nElements[0u]) && (ok); j++) {
                    index = 0u;
                    uint32 numberOfNodesForEachIteration = (numberOfNodes / nElements[0u]) * (j + 1u);
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

    if (ok) {
        masterClient->SetSourceTimestamps(timestampNodes);
    }
    if (ok) {
        tempSignalsDatabase.Purge();
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Error during configuration.");
    }
    return ok;
}

bool OPCUADSOutput::FindTargetIndex(const char8 * const targetName, uint32 &idx) {
    ConfigurationDatabase timestampDatabaseTemp = timestampDatabase;
    bool ok = timestampDatabaseTemp.MoveAbsolute("NodesIdx");
    if (ok) {
        ok = timestampDatabaseTemp.Read(targetName, idx);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to find target signal to be timestamped: %s.", targetName);
        }
    }
    return ok;
}

bool OPCUADSOutput::PopulateTimestampNodes() {
    uint32 numberOfTimestampNodes = numberOfNodes;
    if (isExtensionObject) {
        numberOfTimestampNodes = 1u;
    }
    timestampNodes = new uint64*[numberOfTimestampNodes];
    for (uint32 k=0u; (k<numberOfTimestampNodes); k++) {
        timestampNodes[k] = NULL_PTR(uint64 *);
    }
    uint32 numberOfTimestampSources = 0u;
    bool ok = timestampDatabase.MoveAbsolute("Sources");
    if (ok) {
        numberOfTimestampSources = timestampDatabase.GetNumberOfChildren();
        if (isExtensionObject) {
            ok = (numberOfTimestampSources < 2u);
            if (!ok) {
                 REPORT_ERROR(ErrorManagement::ParametersError, "With ExtensionObject at most one Timestamp signal shall be used.");
            }
        }
    }
    bool defaultTimestampSignalFound = false;
    for (uint32 i=0u; (i<numberOfTimestampSources) && (ok); i++) {
        ok = timestampDatabase.MoveToChild(i);
        StreamString defaultTimestampSignal;
        bool isDefaultTimestamp = timestampDatabase.Read("DefaultTimestampSignal", defaultTimestampSignal);
        if (isDefaultTimestamp) {
            ok = !defaultTimestampSignalFound;
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "At most one Timestamp signal DefaultTimestampSignal shall be set");
            }
            for (uint32 k=0u; (k<numberOfTimestampNodes) && (ok); k++) {
                if (timestampNodes[k] == NULL_PTR(uint64 *)) { //Do not overwrite existing signals
                    timestampNodes[k] = &timestampSignals[i];
                }
            }
            defaultTimestampSignalFound = true;
        }
        else {
            ok = (!isExtensionObject);
            if (!ok) {
                 REPORT_ERROR(ErrorManagement::ParametersError, "With ExtensionObject the Timestamp signal shall use the property DefaultTimestampSignal.");
            }

            uint32 numberOfTargets = 0u;;
            if (ok) {
                numberOfTargets = timestampDatabase.GetNumberOfChildren();
            }
            for (uint32 k=0u; (k<numberOfTargets) && (ok); k++) {
                StreamString nodeName;
                if (ok) {
                    StreamString nn;
                    (void)nn.Printf("%s", k);
                    ok = timestampDatabase.Read(nn.Buffer(), nodeName);
                }
                uint32 nodeIdx = 0u;
                if (ok) {
                    ok = FindTargetIndex(nodeName.Buffer(), nodeIdx);
                }
                if (ok) {
                    timestampNodes[nodeIdx] = &timestampSignals[i];
                }
            }
        }
        if (ok) {
            ok = timestampDatabase.MoveToAncestor(1u);
        }
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
        /*lint -e{9007}  [MISRA C++ Rule 5-14-1] Justification: No side effects.*/
        bool isTimestampSignal = IsTimestampSignal(signalIdx);
        uint32 nodeIdx = signalIdxMap[signalIdx];
        if (!isTimestampSignal) {
            if (ok) {
                if (isExtensionObject) {
                    ok = masterClient->GetSignalMemory(signalAddress, nodeIdx, types[0u], nElements[0u]);
                }
                else {
                    ok = masterClient->GetSignalMemory(signalAddress, nodeIdx, types[nodeIdx], nElements[nodeIdx]);
                }
            }
            if (ok) {
                if (!isExtensionObject) {
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
            if (!triggerSignalSet) {
                triggerSignalSet = true;
                brokerName = "MemoryMapSynchronisedOutputBroker";
            }
            else {
                 REPORT_ERROR(ErrorManagement::ParametersError, "At most one Trigger signal shall be set");
            }
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
    timestampDatabase.Purge();
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
    ConfigurationDatabase timestampDatabaseTemp = timestampDatabase;
    bool isTimestampSignal = timestampDatabaseTemp.MoveAbsolute("Sources");
    if (isTimestampSignal) {
        (void) nn.Printf("%d", idx);
        isTimestampSignal = timestampDatabaseTemp.MoveRelative(nn.Buffer());
    }
    return isTimestampSignal;

}

bool OPCUADSOutput::IsDefaultTimestampSignal(const uint32 idx) {
    StreamString nn;
    ConfigurationDatabase timestampDatabaseTemp = timestampDatabase;
    bool isDefaultTimestampSignal = timestampDatabaseTemp.MoveAbsolute("Sources");
    if (isDefaultTimestampSignal) {
        (void) nn.Printf("%d", idx);
        isDefaultTimestampSignal = timestampDatabaseTemp.MoveRelative(nn.Buffer());
    }
    if (isDefaultTimestampSignal) {
        uint32 defaultTimeStampSignal;
        isDefaultTimestampSignal = timestampDatabaseTemp.Read("DefaultTimestampSignal", defaultTimeStampSignal);
        if (isDefaultTimestampSignal) {
            isDefaultTimestampSignal = (defaultTimeStampSignal == 1u);
        }
    } 
    return isDefaultTimestampSignal;

}

CLASS_REGISTER(OPCUADSOutput, "1.0")

}
/*lint -restore*/
