/**
 * @file LoggerBroker.cpp
 * @brief Source file for class LoggerBroker
 * @date Nov 9, 2016 TODO Verify the value and format of the date
 * @author aneto TODO Verify the name and format of the author
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
#include "LoggerBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

LoggerBroker::LoggerBroker() {
    signalNames = NULL_PTR(StreamString *);
    outputSignals = NULL_PTR(AnyType *);
}

LoggerBroker::~LoggerBroker() {
    if (signalNames != NULL_PTR(StreamString *)) {
        delete[] signalNames;
    }
    if (outputSignals != NULL_PTR(AnyType *)) {
        delete[] outputSignals;
    }
}

bool LoggerBroker::Init(SignalDirection direction,
                        DataSourceI& dataSourceIn,
                        const char8* const functionName,
                        void* gamMemoryAddress) {
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
    for (n = 0u; (n < nOfFunctionSignals) && (ok); n++) {
        //Look for the nth signal of functionName that interacts with this Broker
        StreamString signalAlias;
        ok = dataSourceIn.GetFunctionSignalAlias(OutputSignals, functionIdx, n, signalAlias);

        //Get the signal index in the dataSourceIn where this signalName is located
        uint32 signalIdx;
        if (ok) {
            ok = dataSourceIn.GetSignalIndex(signalIdx, signalAlias.Buffer());
        }
        uint32 numberOfArrayElements = 0u;
        //Check if the signal is an array...
        if (ok) {
            ok = dataSourceIn.GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, signalIdx, numberOfArrayElements);
        }
        if (ok) {
            ok = (numberOfArrayElements > 0u);
        }
        if (ok) {
            uint32 i;
            TypeDescriptor signalDesc = dataSourceIn.GetSignalType(signalIdx);
            for (i = 0u; (i < numberOfArrayElements) && (ok); i++) {
                AnyType printType(signalDesc, 0, GetFunctionPointer(n));
                outputSignals[n] = printType;
                if (numberOfArrayElements == 1u) {
                    signalNames[n] = signalAlias;
                    ok = signalNames[n].Seek(0u);
                }
                else {
                    uint32 startIdx = 0u;
                    uint32 size = 0u;
                    uint32 endIdx = 0u;
                    dataSourceIn.GetFunctionSignalByteOffsetInfo(direction, functionIdx, signalIdx, i, startIdx, size);
                    uint32 nOfBytes = signalDesc.numberOfBits / 8u;
                    if (nOfBytes > 0u) {
                        endIdx = startIdx + (size / nOfBytes);
                    }
                    signalNames[n].Printf("%s [%d:%d]", signalAlias.Buffer(), startIdx, endIdx);
                }
            }
        }
    }

    return ok;
}

bool LoggerBroker::Execute() {
    uint32 n;
    for (n = 0u; n < numberOfCopies; n++) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "%s:%!", signalNames[n].Buffer(), outputSignals[n])
    }
    return true;
}

CLASS_REGISTER(LoggerBroker, "1.0")
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

