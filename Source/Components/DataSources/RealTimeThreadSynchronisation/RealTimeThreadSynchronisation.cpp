/**
 * @file RealTimeThreadSynchronisation.cpp
 * @brief Source file for class RealTimeThreadSynchronisation
 * @date 24/04/2017
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
 * the class RealTimeThreadSynchronisation (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MemoryMapContextInputBroker.h"
#include "RealTimeThreadSynchBuffer.h"
#include "RealTimeThreadSynchronisation.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

RealTimeThreadSynchronisation::RealTimeThreadSynchronisation() :
        DataSourceI() {

}

RealTimeThreadSynchronisation::~RealTimeThreadSynchronisation() {

}

bool RealTimeThreadSynchronisation::AllocateMemory() {
    return true;
}

uint32 RealTimeThreadSynchronisation::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool RealTimeThreadSynchronisation::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    return true;
}

const char8 *RealTimeThreadSynchronisation::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    return "MemoryMapContextInputBroker";
}

bool RealTimeThreadSynchronisation::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    ReferenceT<MemoryMapContextInputBroker> broker("MemoryMapContextInputBroker");
    broker->SetContext(new RealTimeThreadSynchBuffer(this, ));
    bool ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    if (ok) {
        ok = inputBrokers.Insert(broker);
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: OutputBrokers are not supported. Function returns false irrespectively of the parameters.*/
bool RealTimeThreadSynchronisation::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP at StateChange, independently of the function parameters.*/
bool RealTimeThreadSynchronisation::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

CLASS_REGISTER(RealTimeThreadSynchronisation, "1.0")
}

