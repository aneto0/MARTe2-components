/**
 * @file NI6368ADCInputBroker.cpp
 * @brief Source file for class NI6368ADCInputBroker
 * @date 13/01/2017
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
 * the class NI6368ADCInputBroker (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI6368ADCInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

NI6368ADCInputBroker::NI6368ADCInputBroker() :
        BrokerI() {
    adcBoard = NULL_PTR(NI6368ADC *);
    copyTable = NULL_PTR(NI6368CopyTableEntry *);
}

NI6368ADCInputBroker::NI6368ADCInputBroker(NI6368ADC * const adcBoardIn) :
        BrokerI() {
    adcBoard = adcBoardIn;
    copyTable = NULL_PTR(NI6368CopyTableEntry *);
}

/*lint -e{1540} the adcBoard is freed by the DataSource*/
NI6368ADCInputBroker::~NI6368ADCInputBroker() {

}

bool NI6368ADCInputBroker::Init(const SignalDirection direction, DataSourceI &dataSourceIn, const char8 * const functionName, void * const gamMemoryAddress) {
    DataSourceI *dataSource = &dataSourceIn;

    bool ret = InitFunctionPointers(direction, dataSourceIn, functionName, gamMemoryAddress);

    const ClassProperties * properties = GetClassProperties();
    if (ret) {
        ret = (properties != NULL);
    }
    const char8* brokerClassName = NULL_PTR(const char8*);
    if (ret) {
        brokerClassName = properties->GetName();
        ret = (brokerClassName != NULL);
    }

    if (ret) {
        ret = (numberOfCopies > 0u);
    }
    if (ret) {
        copyTable = new NI6368CopyTableEntry[numberOfCopies];
    }
    uint32 functionIdx = 0u;
    if (ret) {
        ret = dataSource->GetFunctionIndex(functionIdx, functionName);
    }
    uint32 functionNumberOfSignals = 0u;
    if (ret) {
        ret = dataSource->GetFunctionNumberOfSignals(direction, functionIdx, functionNumberOfSignals);
    }
    //The same signal can be copied from different ranges. A MemoryMapStatefulBrokerCopyTableEntry is added for each signal range.
    uint32 c = 0u;
    uint32 n;
    uint32 numberOfBuffers = NUMBER_OF_BUFFERS;
    if (adcBoard != NULL_PTR(NI6368ADC *)) {
        numberOfBuffers = adcBoard->GetNumberOfMemoryBuffers();
    }
    for (n = 0u; (n < functionNumberOfSignals) && (ret); n++) {
        if (dataSource->IsSupportedBroker(direction, functionIdx, n, brokerClassName)) {
            uint32 numberOfByteOffsets = 0u;
            ret = dataSource->GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, n, numberOfByteOffsets);

            StreamString functionSignalName;
            if (ret) {
                ret = dataSource->GetFunctionSignalAlias(direction, functionIdx, n, functionSignalName);
            }
            uint32 signalIdx = 0u;
            if (ret) {
                ret = dataSource->GetSignalIndex(signalIdx, functionSignalName.Buffer());
            }
            //Take into account different ranges for the same signal
            uint32 bo;
            for (bo = 0u; (bo < numberOfByteOffsets) && (ret); bo++) {
                if (copyTable != NULL_PTR(NI6368CopyTableEntry *)) {
                    copyTable[c].copySize = GetCopyByteSize(c);
                    copyTable[c].gamPointer = GetFunctionPointer(c);
                    copyTable[c].dataSourceOffset = GetCopyOffset(c);
                    uint32 b;
                    for (b = 0u; (b < numberOfBuffers) && (ret); b++) {
                        void *dataSourceSignalAddress;
                        ret = dataSource->GetSignalMemoryBuffer(signalIdx, b, dataSourceSignalAddress);
                        if (ret) {
                            copyTable[c].dataSourcePointer[b] = dataSourceSignalAddress;
                        }
                    }
                }
                c++;
            }
        }

    }
    return ret;
}

bool NI6368ADCInputBroker::Execute() {
    uint32 n;
    bool ret = true;
    if (adcBoard != NULL_PTR(NI6368ADC *)) {
        if (adcBoard->IsSynchronising()) {
            ret = adcBoard->Synchronise();
        }

        uint32 idx = adcBoard->GetLastBufferIdx();
        void *dataSourceSignalPointer;
        for (n = 0u; (n < numberOfCopies) && (ret); n++) {
            if (copyTable != NULL_PTR(NI6368CopyTableEntry *)) {
                dataSourceSignalPointer = copyTable[n].dataSourcePointer[idx];
                ret = MemoryOperationsHelper::Copy(copyTable[n].gamPointer, dataSourceSignalPointer, copyTable[n].copySize);
            }
        }
    }
    return ret;
}

CLASS_REGISTER(NI6368ADCInputBroker, "1.0")

}

