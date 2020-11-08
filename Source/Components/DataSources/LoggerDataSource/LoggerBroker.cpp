/**
 * @file LoggerBroker.cpp
 * @brief Source file for class LoggerBroker
 * @date 09/11/2016
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
 * the class LoggerBroker (public, protected, and private). Be aware that some 
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
#include "CompilerTypes.h"
#include "LoggerBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {


LoggerBroker::LoggerBroker() :
    BrokerI() {
    signalNames = NULL_PTR(StreamString *);
    outputSignals = NULL_PTR(AnyType *);
    cycleCounter = 0u;
    cyclePeriod = 0u;
}

/*lint -e{1551} the destructor must guarantee that the signalNames and outputSignals are freed.*/
LoggerBroker::~LoggerBroker() {
    if (signalNames != NULL_PTR(StreamString *)) {
        delete[] signalNames;
    }
    if (outputSignals != NULL_PTR(AnyType *)) {
        delete[] outputSignals;
    }
}

bool LoggerBroker::Init(SignalDirection const direction,
                        DataSourceI& dataSourceIn,
                        const char8* const functionName,
                        void * const gamMemoryAddress) {
    bool ok = (direction == OutputSignals);
    if (ok) {
        ok = BrokerI::InitFunctionPointers(direction, dataSourceIn, functionName, gamMemoryAddress);
    }
    if (ok) {
        outputSignals = new AnyType[numberOfCopies];
        signalNames = new StreamString[numberOfCopies];
    }
    //Find the function
    uint32 functionIdx = 0u;
    if (ok) {
        ok = dataSourceIn.GetFunctionIndex(functionIdx, functionName);
    }

    uint32 nOfFunctionSignals = 0u;
    if (ok) {
        //All the signals from this functionName will interact with this dataSourceIn
        ok = dataSourceIn.GetFunctionNumberOfSignals(direction, functionIdx, nOfFunctionSignals);
    }
    uint32 n;
    uint32 c = 0u;
    for (n = 0u; (n < nOfFunctionSignals) && (ok); n++) {
        //Look for the nth signal of functionName that interacts with this Broker
        StreamString signalAlias;
        ok = dataSourceIn.GetFunctionSignalAlias(OutputSignals, functionIdx, n, signalAlias);

        //Get the signal index in the dataSourceIn where this signalName is located
        uint32 signalIdx = 0u;
        if (ok) {
            ok = dataSourceIn.GetSignalIndex(signalIdx, signalAlias.Buffer());
        }

        TypeDescriptor signalDesc = dataSourceIn.GetSignalType(signalIdx);
        uint32 numberOfRanges = 0u;
        //Get the number of ranges defined...
        if (ok) {
            ok = dataSourceIn.GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, n, numberOfRanges);
        }
        if (ok) {
            ok = (numberOfRanges > 0u);
        }
        uint8 numberOfDimensions = 0u;
        if (ok) {
            ok = dataSourceIn.GetSignalNumberOfDimensions(signalIdx, numberOfDimensions);
        }
        uint32 i;
        for (i = 0u; (i < numberOfRanges) && (ok); i++) {
            uint32 startIdx = 0u;
            uint32 size = 0u;
            uint32 endIdx = 0u;
            ok = dataSourceIn.GetFunctionSignalByteOffsetInfo(direction, functionIdx, n, i, startIdx, size);
            if (ok) {
                uint32 nOfBytes = static_cast<uint32> (signalDesc.numberOfBits) / 8u;
                if (nOfBytes > 0u) {
                    startIdx = startIdx / nOfBytes;
                    endIdx = startIdx + (size / nOfBytes);
                }
            }
            if (signalNames != NULL_PTR(StreamString *)) {
                ok = signalNames[c].Printf("%s [%d:%d]", signalAlias.Buffer(), startIdx, endIdx - 1u);
            }

            AnyType printType(signalDesc, 0u, GetFunctionPointer(c));
            if (outputSignals != NULL_PTR(AnyType *)) {
                outputSignals[c] = printType;
                outputSignals[c].SetNumberOfDimensions(numberOfDimensions);
                outputSignals[c].SetNumberOfElements(0u, endIdx - startIdx);
            }
            c++;
        }
    }
    return ok;
}

void LoggerBroker::SetPeriod(const uint32 cyclePeriodIn) {
    cyclePeriod = cyclePeriodIn;
}

bool LoggerBroker::Execute() {

    cycleCounter++;
    if (cycleCounter >= cyclePeriod) {

        uint32 n;
        for (n = 0u; n < numberOfCopies; n++) {
            if ((signalNames != NULL_PTR(StreamString *)) && (outputSignals != NULL_PTR(AnyType *))) {
                REPORT_ERROR(ErrorManagement::Information, "%s:%!", signalNames[n].Buffer(), outputSignals[n]);
            }
        }
        cycleCounter = 0u;
    }
    return true;
}

CLASS_REGISTER(LoggerBroker, "1.0")
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

