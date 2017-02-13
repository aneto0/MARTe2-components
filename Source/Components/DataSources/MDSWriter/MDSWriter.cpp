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
#include "MemoryMapAsyncOutputBroker.h"
#include "MemoryMapAsyncTriggerOutputBroker.h"
#include "MDSWriter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

MDSWriter::MDSWriter() {
    storeOnTrigger = false;
    numberOfPreTriggers = 0u;
    numberOfPostTriggers = 0u;
    numberOfBuffers = 0u;
    numberOfMDSSignals = 0u;
    totalSignalMemory = 0u;
    timeSignalIdx = -1;
    nodes = NULL_PTR(MDSWriterNode **);
    dataSourceMemory = NULL_PTR(char8 *);
    offsets = NULL_PTR(uint32 *);
    cpuMask = 0xf;
    stackSize = 0u;
    tree = NULL_PTR(MDSplus::Tree *);
    treeName = "";
    eventName = "";
    pulseNumber = 0;
    timeRefresh = 0u;
}

MDSWriter::~MDSWriter() {
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
    if (tree != NULL) {
        delete tree;
    }
}

bool MDSWriter::AllocateMemory() {
    return true;
}

uint32 MDSWriter::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool MDSWriter::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    return true;
}

const char8* MDSWriter::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8* brokerName = "MemoryMapAsyncOutputBroker";
    if (storeOnTrigger) {
        brokerName = "MemoryMapAsyncTriggerOutputBroker";
    }
    return brokerName;
}

bool MDSWriter::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

bool MDSWriter::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    ReferenceT<MemoryMapAsyncOutputBroker> brokerAsync;
    ReferenceT<MemoryMapAsyncTriggerOutputBroker> brokerAsyncTrigger;
    bool ok = true;
    if (storeOnTrigger) {
        ok = brokerAsyncTrigger->InitWithTriggerParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers, numberOfPreTriggers,
                                                           numberOfPostTriggers, cpuMask, stackSize);
        if (ok) {
            ok = outputBrokers.Insert(brokerAsyncTrigger);
        }
    }
    else {
        ok = brokerAsync->InitWithBufferParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers, cpuMask, stackSize);
        if (ok) {
            ok = outputBrokers.Insert(brokerAsync);
        }
    }
    return ok;
}

bool MDSWriter::Synchronise() {
    bool ok;
    uint32 n;
    if (nodes != NULL_PTR(MDSWriterNode **)) {
        for (n = 0u; (n < numberOfMDSSignals) && (ok); n++) {
            ok = nodes[n]->Execute();
        }
    }
    return ok;
}

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
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfPreTriggers shall be specified");
        }
        if (ok) {
            ok = data.Read("NumberOfPostTriggers", numberOfPostTriggers);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfPostTriggers shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("TreeName", treeName);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "TreeName shall be specified");
    }
    if (ok) {
        ok = data.Read("PulseNumber", pulseNumber);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "PulseNumber shall be specified");
    }
    if (ok) {
        ok = data.Read("EventName", eventName);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "EventName shall be specified");
    }
    if (ok) {
        ok = data.Read("EventName", eventName);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "EventName shall be specified");
    }
    if (ok) {
        ok = data.Read("TimeRefresh", timeRefresh);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "TimeRefresh shall be specified");
    }

    //Check for the latest pulse number
    if (pulseNumber == -1) {
        try {
            tree = new MDSplus::Tree(treeName.Buffer(), -1);
            pulseNumber = tree->getCurrent(treeName.Buffer());
            pulseNumber++;
            tree->setCurrent(treeName.Buffer(), pulseNumber);
            tree->createPulse(pulseNumber);
            delete tree;
        }
        catch (MDSplus::MdsException &exc) {
            pulseNumber = 1;
            delete tree;
            tree = NULL;
        }

    }
    //Create a pulse. It assumes that the tree template is already created!!
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
    }
    catch (MDSplus::MdsException &exc) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Failed opening tree %s with the pulseNUmber = %d. Trying to create pulse", treeName.Buffer(),
                                pulseNumber)
        delete tree;
        tree = NULL;
    }
    if (tree == NULL) {
        try {
            tree = new MDSplus::Tree(treeName.Buffer(), -1);
            tree->setCurrent(treeName.Buffer(), pulseNumber);
            tree->createPulse(pulseNumber);
        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Failed opening tree %s with the pulseNUmber = %d. Trying to create pulse",
                                    treeName.Buffer(), pulseNumber)
            delete tree;
            ok = false;
        }
    }
    delete tree;

    if (ok) {
        //Open a pulse.
        try {
            tree = new MDSplus::Tree(treeName.Buffer(), pulseNumber);
        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Failed opening tree %s with the pulseNUmber = %d. Trying to create pulse",
                                    treeName.Buffer(), pulseNumber)
            ok = false;
        }
    }
    return ok;
}

bool MDSWriter::SetConfiguredDatabase(StructuredDataI& data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.MoveRelative("Signals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }
        if (ok) {
            //Do not allow to add signals in run-time
            ok = data.Write("Locked", 1);
        }
    }
    //Check signal properties and compute memory
    if (ok) {
        //Do not allow samples
        uint32 functionNumberOfSignals = 0u;
        uint32 n;
        if (GetFunctionNumberOfSignals(OutputSignals, 0u, functionNumberOfSignals)) {
            for (n = 0u; (n < functionNumberOfSignals) && (ok); n++) {
                uint32 nSamples;
                ok = GetFunctionSignalSamples(OutputSignals, 0u, n, nSamples);
                if (ok) {
                    ok = (nSamples == 1u);
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples shall be exactly 1");
                }
            }
        }

        offsets = new uint32[GetNumberOfSignals()];
        uint32 numberOfSignals = GetNumberOfSignals();
        //Count the number of bytes
        for (n = 0u; (n < numberOfSignals) && (ok); n++) {
            offsets[n] = totalSignalMemory;
            uint32 nBytes = 0u;
            ok = GetSignalByteSize(n, nBytes);
            totalSignalMemory += nBytes;
        }
    }
    //Allocate memory
    if (ok) {
        dataSourceMemory = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalSignalMemory));
    }

    //Check the signal index of the timing signal.
    int32 timeSignalIdx = -1;
    uint32 numberOfSignals = GetNumberOfSignals();
    if (ok) {
        //Count the number of MDS+ signals
        uint32 n;
        for (n = 0u; (n < numberOfSignals) && (ok); n++) {
            if (data.MoveRelative(data.GetChildName(n))) {
                StreamString nodeName;
                if (data.Read("NodeName", nodeName)) {
                    //Dynamically add MDSWriteNodes to the list
                    MDSWriterNode **tempNodes = new MDSWriterNode*[numberOfMDSSignals + 1u];
                    uint32 t;
                    for (t = 0u; t < numberOfMDSSignals; t++) {
                        tempNodes[t] = nodes[t];
                    }
                    tempNodes[numberOfMDSSignals] = new MDSWriterNode();
                    ok = tempNodes[numberOfMDSSignals]->Initialise(data);
                    if (ok) {
                        tempNodes[numberOfMDSSignals]->SetSignalMemory(reinterpret_cast<void *>(dataSourceMemory[offsets[n]]));
                        tempNodes[numberOfMDSSignals]->AllocateTreeNode(tree);
                    }
                    delete[] nodes;
                    nodes = tempNodes;
                    numberOfMDSSignals++;
                }
                //Check if the signal is defined as a TimeSignal
                uint32 timeSignal;
                if (data.Read("TimeSignal", timeSignal)) {
                    if (timeSignalIdx != -1) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Only one TimeSignal shall be defined");
                        ok = false;
                    }
                    if (timeSignal > 0) {
                        timeSignalIdx = static_cast<int32>(n);
                    }
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
        ok = (numberOfMDSSignals > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The numberOfMDSSignals > 0");
        }
    }
    //Only one and one GAM allowed to interact with this DataSourceI
    if (ok) {
        ok = (GetNumberOfFunctions() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly one Function allowed to interact with this DataSourceI");
        }
    }

    //Check if a time signal was set
    if (storeOnTrigger) {
        if (ok) {
            ok = (timeSignalIdx > -1);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "StoreOnTrigger was specified but no TimeSignal was found");
        }
        if (ok) {
            ok = (GetSignalType(static_cast<uint32>(timeSignalIdx)) == UnsignedInteger32Bit);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "TimeSignal shall have type uint32");
        }
        if (ok) {
            uint32 n;
            for (n = 0u; n < numberOfMDSSignals; n++) {
                nodes[n]->SetTimeSignalMemory(reinterpret_cast<void *>(dataSourceMemory[offsets[static_cast<uint32>(timeSignalIdx)]]));
            }
        }
    }
    else {
        if (ok) {
            ok = (timeSignalIdx == -1);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "StoreOnTrigger was not specified but a TimeSignal was found");
        }
    }

    return ok;
}

CLASS_REGISTER(MDSWriter, "1.0")

}
