/**
 * @file MDSWriter.cpp
 * @brief Source file for class MDSWriter
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
 * the class MDSWriter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "MDSWriter.h"
#include "MemoryMapAsyncOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static const int32 MDS_UNDEFINED_PULSE_NUMBER = -2;

MDSWriter::MDSWriter() :
        DataSourceI(),
        MessageI() {
    storeOnTrigger = false;
    numberOfPreTriggers = 0u;
    numberOfPostTriggers = 0u;
    numberOfBuffers = 0u;
    numberOfMDSSignals = 0u;
    timeSignalIdx = -1;
    nodes = NULL_PTR(MDSWriterNode **);
    dataSourceMemory = NULL_PTR(char8 *);
    offsets = NULL_PTR(uint32 *);
    cpuMask = 0xfu;
    stackSize = 0u;
    tree = NULL_PTR(MDSplus::Tree *);
    treeName = "";
    eventName = "";
    pulseNumber = MDS_UNDEFINED_PULSE_NUMBER;
    lastTimeRefreshCount = 0u;
    refreshEveryCounts = 0u;
    fatalTreeNodeError = false;
    brokerAsyncTrigger = NULL_PTR(MemoryMapAsyncTriggerOutputBroker *);
    filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

/*lint -e{1551} -e{1579} the destructor must guarantee that the MDSplus are deleted and the shared memory freed. The brokerAsyncTrigger is freed by the ReferenceT */
MDSWriter::~MDSWriter() {

    if (FlushSegments() != ErrorManagement::NoError) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to Flush the MDSWriterNodes");
    }
    if (nodes != NULL_PTR(MDSWriterNode **)) {
        uint32 n;
        for (n = 0u; (n < numberOfMDSSignals); n++) {
            delete nodes[n];
        }
        delete[] nodes;
    }
    if (dataSourceMemory != NULL_PTR(char8 *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(dataSourceMemory));
    }
    if (offsets != NULL_PTR(uint32 *)) {
        delete[] offsets;
    }
    if (tree != NULL_PTR(MDSplus::Tree *)) {
        delete tree;
    }

}

bool MDSWriter::AllocateMemory() {
    return true;
}

uint32 MDSWriter::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The signalAddress is independent of the bufferIdx.*/
bool MDSWriter::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (dataSourceMemory != NULL_PTR(char8 *));
    if (ok) {
        /*lint -e{613} dataSourceMemory cannot be NULL here*/
        char8 *memPtr = &dataSourceMemory[offsets[signalIdx]];
        signalAddress = reinterpret_cast<void *&>(memPtr);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction and on the storeOnTrigger property (which is load before).*/
const char8* MDSWriter::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8* brokerName = "";
    if (direction == OutputSignals) {
        if (storeOnTrigger) {
            brokerName = "MemoryMapAsyncTriggerOutputBroker";
        }
        else {
            brokerName = "MemoryMapAsyncOutputBroker";
        }
    }
    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: InputBrokers are not supported. Function returns false irrespectively of the parameters.*/
bool MDSWriter::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

bool MDSWriter::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    bool ok = true;
    if (storeOnTrigger) {
        ReferenceT<MemoryMapAsyncTriggerOutputBroker> brokerAsyncTriggerNew("MemoryMapAsyncTriggerOutputBroker");
        brokerAsyncTrigger = brokerAsyncTriggerNew.operator ->();
        ok = brokerAsyncTriggerNew->InitWithTriggerParameters(OutputSignals, *this, functionName, gamMemPtr,
                                                              numberOfBuffers, numberOfPreTriggers,
                                                              numberOfPostTriggers, cpuMask, stackSize);
        if (ok) {
            ok = outputBrokers.Insert(brokerAsyncTriggerNew);
        }
    }
    else {
        ReferenceT<MemoryMapAsyncOutputBroker> brokerAsync("MemoryMapAsyncOutputBroker");
        ok = brokerAsync->InitWithBufferParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers,
                                                   cpuMask, stackSize);
        if (ok) {
            ok = outputBrokers.Insert(brokerAsync);
        }
    }
    return ok;
}

bool MDSWriter::Synchronise() {
    uint32 n;
    if (nodes != NULL_PTR(MDSWriterNode **)) {
        for (n = 0u; (n < numberOfMDSSignals) && (!fatalTreeNodeError); n++) {
            fatalTreeNodeError = !nodes[n]->Execute();
            if (fatalTreeNodeError) {
                if (treeRuntimeErrorMsg.IsValid()) {
                    //Reset any previous replies
                    treeRuntimeErrorMsg->SetAsReply(false);
                    if (!MessageI::SendMessage(treeRuntimeErrorMsg, this)) {
                        StreamString destination = treeRuntimeErrorMsg->GetDestination();
                        StreamString function = treeRuntimeErrorMsg->GetFunction();
                        REPORT_ERROR(ErrorManagement::FatalError, "Could not send TreeRuntimeError message to %s [%s]",
                                     destination.Buffer(), function.Buffer());
                    }
                }
            }
        }
    }
    if ((HighResolutionTimer::Counter() - lastTimeRefreshCount) > refreshEveryCounts) {
        lastTimeRefreshCount = HighResolutionTimer::Counter();
        MDSplus::Event::setEvent(eventName.Buffer());
    }
    return !fatalTreeNodeError;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool MDSWriter::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool MDSWriter::Initialise(StructuredDataI& data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("NumberOfBuffers", numberOfBuffers);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfBuffers shall be specified");
    }
    if (ok) {
        ok = (numberOfBuffers > 0u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfBuffers shall be > 0u");
    }
    if (ok) {
        uint32 cpuMaskIn;
        ok = data.Read("CPUMask", cpuMaskIn);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "CPUMask shall be specified");
        }
        else {
            cpuMask = cpuMaskIn;
        }
    }
    if (ok) {
        ok = data.Read("StackSize", stackSize);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "StackSize shall be specified");
    }
    if (ok) {
        ok = (stackSize > 0u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "StackSize shall be > 0u");
    }
    uint32 storeOnTriggerU = 0u;
    if (ok) {
        ok = data.Read("StoreOnTrigger", storeOnTriggerU);
        storeOnTrigger = (storeOnTriggerU == 1u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "StoreOnTrigger shall be specified");
    }
    if (storeOnTrigger) {
        if (ok) {
            ok = data.Read("NumberOfPreTriggers", numberOfPreTriggers);

            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfPreTriggers shall be specified");
            }
        }
        if (ok) {
            ok = data.Read("NumberOfPostTriggers", numberOfPostTriggers);

            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfPostTriggers shall be specified");
            }
        }
    }
    if (ok) {
        ok = data.Read("TreeName", treeName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "TreeName shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("EventName", eventName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "EventName shall be specified");
        }
    }
    uint32 timeRefresh = 0u;
    if (ok) {
        ok = data.Read("TimeRefresh", timeRefresh);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "TimeRefresh shall be specified");
        }
    }

    if (ok) {
        //Optional parameter
        (void) (data.Read("PulseNumber", pulseNumber));
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
        refreshEveryCounts = timeRefresh * HighResolutionTimer::Frequency();
    }
    if (ok) {
        //Check if there are any Message elements set
        if (Size() > 0u) {
            ReferenceT<ReferenceContainer> msgContainer = Get(0u);
            if (msgContainer.IsValid()) {
                uint32 j;
                uint32 nOfMessages = msgContainer->Size();
                for (j = 0u; (j < nOfMessages) && (ok); j++) {
                    ReferenceT<Message> msg = msgContainer->Get(j);
                    ok = msg.IsValid();
                    if (ok) {
                        StreamString msgName = msg->GetName();
                        if (msgName == "TreeOpenedOK") {
                            treeOpenedOKMsg = msg;
                        }
                        else if (msgName == "TreeOpenedFail") {
                            treeOpenedFailMsg = msg;
                        }
                        else if (msgName == "TreeFlushed") {
                            treeFlushedMsg = msg;
                        }
                        else if (msgName == "TreeRuntimeError") {
                            treeRuntimeErrorMsg = msg;
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::ParametersError, "Message %s is not supported.",
                                         msgName.Buffer());
                            ok = false;
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Found an invalid Message in container %s",
                                     msgContainer->GetName());
                        ok = false;
                    }

                }
            }
        }
    }
    return ok;
}

bool MDSWriter::SetConfiguredDatabase(StructuredDataI& data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = data.MoveRelative("Signals");
    }
    //Only one and one GAM allowed to interact with this DataSourceI
    if (ok) {
        ok = (GetNumberOfFunctions() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "Exactly one Function allowed to interact with this DataSourceI");
        }
    }

    //Check signal properties and compute memory
    uint32 *signalSamples = NULL_PTR(uint32 *);
    uint32 totalSignalMemory = 0u;
    if (ok) {
        uint32 functionNumberOfSignals = 0u;
        uint32 n;
        if (GetFunctionNumberOfSignals(OutputSignals, 0u, functionNumberOfSignals)) {
            uint32 dataSourceIdx = 0u;
            signalSamples = new uint32[functionNumberOfSignals];
            for (n = 0u; (n < functionNumberOfSignals) && (ok); n++) { //get samples and save in the dataSource order
                StreamString GAMSignalName;
                uint32 nSamples;
                ok = GetFunctionSignalSamples(OutputSignals, 0u, n, nSamples);
                if (ok) { //Verify value
                    ok = (nSamples > 0u);
                    if (!ok) {
                        uint32 auxIdx = n;
                        REPORT_ERROR(ErrorManagement::ParametersError,
                                     "Number of samples for GAM index = %u must be > 0)", auxIdx);
                    }
                    if (ok) {
                        ok = GetFunctionSignalAlias(OutputSignals, 0u, n, GAMSignalName);
                        if (ok) {
                            ok = GetSignalIndex(dataSourceIdx, GAMSignalName.Buffer());
                        }
                        if (ok) {
                            signalSamples[dataSourceIdx] = nSamples;
                        }
                    }
                }
            }
        }
        offsets = new uint32[GetNumberOfSignals()];
        uint32 nOfSignals = GetNumberOfSignals();
        //Count the number of bytes
        for (n = 0u; (n < nOfSignals) && (ok); n++) {
            offsets[n] = totalSignalMemory;
            uint32 nBytes = 0u;
            //GetSignalByteSize(n, nBytes) return the number of Bytes per signal and SAMPLE!
            //It is done because one signal of the dataSource can supply different number of samples per two different GAMs
            ok = GetSignalByteSize(n, nBytes);
            /*lint -e{613} ok => signalSamples != NULL*/
            totalSignalMemory += nBytes * signalSamples[n];
        }
    }
    //Allocate memory
    if (ok) {
        dataSourceMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(
                totalSignalMemory));
    }

    float64 timeSignalMultiplier = 0.F;
    //Check the signal index of the timing signal.
    uint32 nOfSignals = GetNumberOfSignals();
    if (ok) {
        //Count the number of MDS+ signals
        uint32 n;
        for (n = 0u; (n < nOfSignals) && (ok); n++) {
            ok = data.MoveRelative(data.GetChildName(n));
            if (ok) {
                //Have to mix and match between the original setting of the DataSource signal (i.e. the one MDS+ related)
                //and the ones which are later added by the RealTimeApplicationConfigurationBuilder
                ok = originalSignalInformation.MoveRelative(originalSignalInformation.GetChildName(n));
            }
            StreamString nodeName;
            StreamString nodeType;
            if (ok) {
                ok = data.Read("Type", nodeType);
            }
            if (ok) {
                ok = originalSignalInformation.Write("Type", nodeType.Buffer());
            }
            if (ok) {
                uint32 nElements;
                ok = GetSignalNumberOfElements(n, nElements);
                if (ok) {
                    ok = originalSignalInformation.Write("NumberOfElements", nElements);
                }
            }
            if (ok) {
                /*lint -e{613} ok => signalSamples != NULL*/
                ok = originalSignalInformation.Write("Samples", signalSamples[n]);
            }
            if (ok) {                //Read dimensions. Matrix not supported yet!
                uint8 auxNumberOfDimensions = 32u; //invalid number
                ok = GetSignalNumberOfDimensions(n, auxNumberOfDimensions);
                if (ok) {
                    ok = (auxNumberOfDimensions == 0u) || (auxNumberOfDimensions == 1u);
                    if (!ok) {
                        uint32 auxIdx = n;
                        REPORT_ERROR(
                                ErrorManagement::ParametersError,
                                "Dimensions must be 0 (scalar) or 1 (vector). Matrix not supported yet. current dimension = %u for signal index = %u",
                                auxNumberOfDimensions, auxIdx);
                    }
                }
                if (ok) {
                    ok = originalSignalInformation.Write("NumberOfDimensions", auxNumberOfDimensions);
                }
            }
            if (originalSignalInformation.Read("NodeName", nodeName)) {

                //Dynamically add MDSWriteNodes to the list
                uint32 numberOfNodes = (numberOfMDSSignals + 1u);
                MDSWriterNode **tempNodes = new MDSWriterNode*[numberOfNodes];
                uint32 t;
                for (t = 0u; t < numberOfMDSSignals; t++) {
                    if (nodes != NULL_PTR(MDSWriterNode **)) {
                        tempNodes[t] = nodes[t];
                    }
                }
                tempNodes[numberOfMDSSignals] = new MDSWriterNode();
                ok = tempNodes[numberOfMDSSignals]->Initialise(originalSignalInformation);
                if (ok) {
                    if ((tempNodes != NULL_PTR(MDSWriterNode **)) && (dataSourceMemory != NULL_PTR(char8 *))
                            && (offsets != NULL_PTR(uint32 *))) {
                        tempNodes[numberOfMDSSignals]->SetSignalMemory(
                                reinterpret_cast<void *>(&dataSourceMemory[offsets[n]]));
                    }
                }
                delete[] nodes;
                nodes = tempNodes;
                numberOfMDSSignals++;
            }
            //Check if the signal is defined as a TimeSignal
            uint32 timeSignal;
            if (originalSignalInformation.Read("TimeSignal", timeSignal)) {
                if (timeSignalIdx != -1) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Only one TimeSignal shall be defined");
                    ok = false;
                }
                if (timeSignal > 0u) {
                    timeSignalIdx = static_cast<int32>(n);
                    if (!originalSignalInformation.Read("TimeSignalMultiplier", timeSignalMultiplier)) {
                        timeSignalMultiplier = 1e-6;
                        REPORT_ERROR(ErrorManagement::Warning, "No TimeSignalMultiplier was defined. Using default = %f", timeSignalMultiplier);
                    }
                }
            }
            if (ok) {
                ok = originalSignalInformation.MoveToAncestor(1u);
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
        ok = (numberOfMDSSignals > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The numberOfMDSSignals shall be > 0");
        }
    }
    //Check if a time signal was set
    bool useTimeSignal = (timeSignalIdx > -1);
    if (storeOnTrigger) {
        if (ok) {
            ok = (useTimeSignal);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError,
                             "StoreOnTrigger was specified but no TimeSignal was found");
            }
        }
    }
    if (useTimeSignal) {
        TypeDescriptor timeSignalType = GetSignalType(static_cast<uint32>(timeSignalIdx));
        if (ok) {
            ok = (timeSignalType.type == SignedInteger);
            if (!ok) {
                ok = (timeSignalType.type == UnsignedInteger);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "TimeSignal shall be an integer (signed or unsigned) from 8 to 64 bits");
            }
        }
        if (ok) {
            uint32 n;
            for (n = 0u; n < numberOfMDSSignals; n++) {
                if ((nodes != NULL_PTR(MDSWriterNode **)) && (dataSourceMemory != NULL_PTR(char8 *))
                        && (offsets != NULL_PTR(uint32 *))) {
                    nodes[n]->SetTimeSignalMemory(reinterpret_cast<void *>(&dataSourceMemory[offsets[timeSignalIdx]]), timeSignalType, timeSignalMultiplier);
                }
            }
        }
    }
    if (ok) {
        if (pulseNumber != MDS_UNDEFINED_PULSE_NUMBER) {
            ok = (OpenTree(pulseNumber) == ErrorManagement::NoError);
        }
    }
    if (signalSamples != NULL_PTR(uint32 *)) {
        delete[] signalSamples;
    }
    return ok;
}

ErrorManagement::ErrorType MDSWriter::OpenTree(const int32 pulseNumberIn) {
    bool ok = true;
    pulseNumber = pulseNumberIn;
    if (tree != NULL_PTR(MDSplus::Tree *)) {
        try {
            //Check if the tree is still valid before flushing any segments. It might have been closed due to a fault in the meanwhile...
            MDSplus::Tree *treeTemp = new MDSplus::Tree(treeName.Buffer(), -1);
            delete treeTemp;
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR(ErrorManagement::Warning, "Tree %s is no longer valid. Error: %s", treeName.Buffer(),
                         exc.what());
            fatalTreeNodeError = true;
        }
        if (!fatalTreeNodeError) {
            if (FlushSegments() != ErrorManagement::NoError) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed to Flush the MDSWriterNodes");
            }
        }
        try {
            delete tree;
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR(ErrorManagement::Warning, "Failed deleting tree %s. Error: %s", treeName.Buffer(), exc.what());
        }
        tree = NULL_PTR(MDSplus::Tree *);
    }
    //Check for the latest pulse number
    if (pulseNumber == -1) {
        try {
            tree = new MDSplus::Tree(treeName.Buffer(), -1);
            pulseNumber = MDSplus::Tree::getCurrent(treeName.Buffer());
            pulseNumber++;
            MDSplus::Tree::setCurrent(treeName.Buffer(), pulseNumber);
            tree->createPulse(pulseNumber);
            delete tree;
            tree = NULL_PTR(MDSplus::Tree *);
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed opening tree %s to get last pulse number. Error: %s",
                         treeName.Buffer(), exc.what());
            if (tree != NULL_PTR(MDSplus::Tree *)) {
                delete tree;
            }
            tree = NULL_PTR(MDSplus::Tree *);
            ok = false;
        }

    }
    if (ok) {
        //Create a pulse. It assumes that the tree template is already created!!
        try {
            tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR(ErrorManagement::Warning,
                         "Failed opening tree %s with the pulseNumber = %d. Going to try to create pulse. Error: %s",
                         treeName.Buffer(), pulseNumber, exc.what());
            if (tree != NULL_PTR(MDSplus::Tree *)) {
                delete tree;
            }
            tree = NULL_PTR(MDSplus::Tree *);
        }
        if (tree == NULL_PTR(MDSplus::Tree *)) {
            try {
                tree = new MDSplus::Tree(treeName.Buffer(), -1);
                MDSplus::Tree::setCurrent(treeName.Buffer(), pulseNumber);
                tree->createPulse(pulseNumber);
            }
            catch (const MDSplus::MdsException &exc) {
                REPORT_ERROR(ErrorManagement::ParametersError,
                             "Failed creating tree %s with the pulseNUmber = %d. Error: %s", treeName.Buffer(),
                             pulseNumber, exc.what());
                ok = false;
            }
        }
        if (tree != NULL_PTR(MDSplus::Tree *)) {
            delete tree;
        }
        tree = NULL_PTR(MDSplus::Tree *);
    }
    if (ok) {
        //Open a pulse.
        try {
            tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        }
        catch (const MDSplus::MdsException &exc) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "Failed opening tree %s with the pulseNUmber = %d. Trying to create pulse. Error: %s",
                         treeName.Buffer(), pulseNumber, exc.what());
            ok = false;
            if (tree != NULL_PTR(MDSplus::Tree *)) {
                delete tree;
            }
            tree = NULL_PTR(MDSplus::Tree *);
        }
    }

    if (ok) {
        uint32 n;
        if (nodes != NULL_PTR(MDSWriterNode **)) {
            for (n = 0u; (n < numberOfMDSSignals) && (ok); n++) {
                ok = nodes[n]->AllocateTreeNode(tree);
            }
        }
    }
    if (ok) {
        if (brokerAsyncTrigger != NULL_PTR(MemoryMapAsyncTriggerOutputBroker *)) {
            brokerAsyncTrigger->ResetPreTriggerBuffers();
        }
    }
    if (ok) {
        if (treeOpenedOKMsg.IsValid()) {
            //Remove old pulse number from message.
            if (treeOpenedOKMsg->Size() > 0u) {
                ReferenceT<ConfigurationDatabase> cdbe = treeOpenedOKMsg->Get(0u);
                if (cdbe.IsValid()) {
                    (void) treeOpenedOKMsg->Delete(cdbe);
                }
            }
            //Reset any previous replies
            treeOpenedOKMsg->SetAsReply(false);
            ReferenceT<ConfigurationDatabase> cdbn(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            (void) cdbn->Write("param1", pulseNumber);
            (void) treeOpenedOKMsg->Insert(cdbn);
            if (!MessageI::SendMessage(treeOpenedOKMsg, this)) {
                StreamString destination = treeOpenedOKMsg->GetDestination();
                StreamString function = treeOpenedOKMsg->GetFunction();
                REPORT_ERROR(ErrorManagement::FatalError, "Could not send TreeOpenedOK message to %s [%s]",
                             destination.Buffer(), function.Buffer());
            }
        }
    }
    else {
        if (treeOpenedFailMsg.IsValid()) {
            //Reset any previous replies
            treeOpenedFailMsg->SetAsReply(false);
            if (!MessageI::SendMessage(treeOpenedFailMsg, this)) {
                StreamString destination = treeOpenedFailMsg->GetDestination();
                StreamString function = treeOpenedFailMsg->GetFunction();
                REPORT_ERROR(ErrorManagement::FatalError, "Could not send TreeOpenedFail message to %s [%s]",
                             destination.Buffer(), function.Buffer());
            }
        }
    }
    fatalTreeNodeError = !ok;
    ErrorManagement::ErrorType ret(ok);
    return ret;
}

ErrorManagement::ErrorType MDSWriter::FlushSegments() {
    uint32 n;
    bool ok = true;
    if (brokerAsyncTrigger != NULL_PTR(MemoryMapAsyncTriggerOutputBroker *)) {
        ok = brokerAsyncTrigger->FlushAllTriggers();
    }
    if (nodes != NULL_PTR(MDSWriterNode **)) {
        for (n = 0u; ((n < numberOfMDSSignals) && (ok)); n++) {
            ok = nodes[n]->Flush();
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed to flush MDSWriterNode");
            }
        }
    }
    if (ok) {
        if (treeFlushedMsg.IsValid()) {
            //Reset any previous replies
            treeFlushedMsg->SetAsReply(false);
            if (!MessageI::SendMessage(treeFlushedMsg, this)) {
                StreamString destination = treeFlushedMsg->GetDestination();
                StreamString function = treeFlushedMsg->GetFunction();
                REPORT_ERROR(ErrorManagement::FatalError, "Could not send TreeFlushed message to %s [%s]",
                             destination.Buffer(), function.Buffer());
            }
        }
    }
    ErrorManagement::ErrorType err(ok);
    return err;
    /*lint -e{1762} function cannot be constant as it is registered as an RPC for CLASS_METHOD_REGISTER*/
}

const ProcessorType& MDSWriter::GetCPUMask() const {
    return cpuMask;
}

const StreamString& MDSWriter::GetEventName() const {
    return eventName;
}

uint32 MDSWriter::GetNumberOfBuffers() const {
    return numberOfBuffers;
}

uint32 MDSWriter::GetNumberOfMdsSignals() const {
    return numberOfMDSSignals;
}

uint32 MDSWriter::GetNumberOfPostTriggers() const {
    return numberOfPostTriggers;
}

uint32 MDSWriter::GetNumberOfPreTriggers() const {
    return numberOfPreTriggers;
}

int32 MDSWriter::GetPulseNumber() const {
    return pulseNumber;
}

uint64 MDSWriter::GetRefreshEveryCounts() const {
    return refreshEveryCounts;
}

uint32 MDSWriter::GetStackSize() const {
    return stackSize;
}

bool MDSWriter::IsStoreOnTrigger() const {
    return storeOnTrigger;
}

int32 MDSWriter::GetTimeSignalIdx() const {
    return timeSignalIdx;
}

const StreamString& MDSWriter::GetTreeName() const {
    return treeName;
}

CLASS_REGISTER(MDSWriter, "1.0")
CLASS_METHOD_REGISTER(MDSWriter, FlushSegments)
CLASS_METHOD_REGISTER(MDSWriter, OpenTree)

}
