/**
 * @file LoggerDataSource.cpp
 * @brief Source file for class LoggerDataSource
 * @date 9/11/2016
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
 * the class LoggerDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "LoggerBroker.h"
#include "LoggerDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

LoggerDataSource::LoggerDataSource() :
    DataSourceI() {
    cyclePeriod = 0u;
}

LoggerDataSource::~LoggerDataSource() {
}

bool LoggerDataSource::Initialise(StructuredDataI & data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {
        if (!data.Read("CyclePeriod", cyclePeriod)) {
            cyclePeriod = 0u;
        }
    }
    return ret;
}

bool LoggerDataSource::Synchronise() {
    return false;
}

bool LoggerDataSource::AllocateMemory() {
    return true;
}

uint32 LoggerDataSource::GetNumberOfMemoryBuffers() {
    return 0u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Method not required by this DataSourceI implementation. (TODO move interface to a MemoryMappedDataSource).*/
bool LoggerDataSource::GetSignalMemoryBuffer(const uint32 signalIdx,
                                             const uint32 bufferIdx,
                                             void*& signalAddress) {
    return true;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Method not required by this DataSourceI implementation. (TODO move interface to a MemoryMappedDataSource).*/
const char8* LoggerDataSource::GetBrokerName(StructuredDataI& data,
                                             const SignalDirection direction) {
    return "LoggerBroker";
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Method not required by this DataSourceI implementation. (TODO move interface to a MemoryMappedDataSource).*/
bool LoggerDataSource::GetInputBrokers(ReferenceContainer& inputBrokers,
                                       const char8* const functionName,
                                       void* const gamMemPtr) {
    return false;
}

bool LoggerDataSource::GetOutputBrokers(ReferenceContainer& outputBrokers,
                                        const char8* const functionName,
                                        void* const gamMemPtr) {
    ReferenceT < LoggerBroker > broker("LoggerBroker");
    bool ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    if (ok) {
        broker->SetPeriod(cyclePeriod);
        ok = outputBrokers.Insert(broker);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: this DataSourceI implementation is independent of the states being changed.*/
bool LoggerDataSource::PrepareNextState(const char8 * const currentStateName,
                                        const char8 * const nextStateName) {
    return true;
}

CLASS_REGISTER(LoggerDataSource, "1.0")
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

