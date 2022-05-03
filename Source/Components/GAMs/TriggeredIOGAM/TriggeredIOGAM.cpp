/**
 * @file TriggeredIOGAM.cpp
 * @brief Source file for class TriggeredIOGAM
 * @date 25/03/2022
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
 * the class TriggeredIOGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "TriggeredIOGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
TriggeredIOGAM::TriggeredIOGAM() :
        GAM() {
    totalSignalsByteSize = 0u;
    outputBrokersAccel = NULL_PTR(BrokerI **);
    nOfOutputBrokers = 0u;
    triggerSignal = NULL_PTR(uint8 *);
    inputSignalsMemoryNoTrigger = NULL_PTR(uint8 *);
}

/*lint -e{1740} the triggerSignal and the inputSignalsMemoryNoTrigger are managed and freed by the framework core.*/
TriggeredIOGAM::~TriggeredIOGAM() {
    if (outputBrokersAccel != NULL_PTR(BrokerI **)) {
        delete [] outputBrokersAccel;
    }
}

bool TriggeredIOGAM::Setup() {
    uint32 n;
    uint32 inTotalSignalsByteSize = 0u;
    bool ret = true;
    for (n = 0u; (n < GetNumberOfInputSignals()) && (ret); n++) {
        uint32 inByteSize = 0u;
        uint32 inSamples = 1u;
        ret = GetSignalByteSize(InputSignals, n, inByteSize);
        if (ret) {
            ret = GetSignalNumberOfSamples(InputSignals, n, inSamples);
        }
        if (ret) {
            inByteSize *= inSamples;
            inTotalSignalsByteSize += inByteSize;
        }
    }
    inTotalSignalsByteSize -= static_cast<uint32>(sizeof(uint8)); //Trigger signal
    uint32 outTotalSignalsByteSize = 0u;
    for (n = 0u; (n < GetNumberOfOutputSignals()) && (ret); n++) {
        uint32 outByteSize = 0u;
        uint32 outSamples = 1u;
        ret = GetSignalByteSize(OutputSignals, n, outByteSize);
        if (ret) {
            ret = GetSignalNumberOfSamples(OutputSignals, n, outSamples);
        }
        if (ret) {
            outByteSize *= outSamples;
            outTotalSignalsByteSize += outByteSize;
        }
    }
    if (ret) {
        ret = (inTotalSignalsByteSize == outTotalSignalsByteSize);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError,
                         "GetSignalByteSize(InputSignals): %d != GetSignalByteSize(OutputSignals): %d",
                         inTotalSignalsByteSize, outTotalSignalsByteSize);
        }
    }
    if (ret) {
        totalSignalsByteSize = outTotalSignalsByteSize;
    }
    if (ret) {
        TypeDescriptor td = GetSignalType(InputSignals, 0u);
        ret = (td == UnsignedInteger8Bit);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The first signal (the Trigger signal), shall be of type uint8.");
        }
    }
    if (ret) {
        triggerSignal = reinterpret_cast<uint8 *>(GetInputSignalsMemory());
    }
    if (ret) {
        inputSignalsMemoryNoTrigger = reinterpret_cast<uint8 *>(GetInputSignalsMemory());
        inputSignalsMemoryNoTrigger = &inputSignalsMemoryNoTrigger[1u]; //skip the trigger signal
    }
 
    return ret;
}

/*lint -e{715} the setup of the broker accelerators is irrespective of the state names*/
bool TriggeredIOGAM::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    if (outputBrokersAccel == NULL_PTR(BrokerI **)) {
        nOfOutputBrokers = outputBrokers.Size();
        outputBrokersAccel = new BrokerI*[nOfOutputBrokers];
        for (uint32 n=0u; n<nOfOutputBrokers; n++) {
            outputBrokersAccel[n] = dynamic_cast<BrokerI *>(outputBrokers.Get(n).operator ->());
        }
    }

    return true;
}

/*lint -e{613} the Execute method call is conditional to the fact that the outputBrokersAccel memory exists (also protected by the fact that nOfOutputBrokers > 0) and that the inputSignalsMemoryNoTrigger is not NULL.*/
bool TriggeredIOGAM::Execute() {
    bool ret = true;
    if (*triggerSignal == 1u) {
        ret = MemoryOperationsHelper::Copy(GetOutputSignalsMemory(), reinterpret_cast<void *>(inputSignalsMemoryNoTrigger), totalSignalsByteSize);
        for (uint32 n=0u; (n<nOfOutputBrokers) && (ret); n++) {
            outputBrokersAccel[n]->SetEnabled(true);
        }
    }
    else {
        for (uint32 n=0u; n<nOfOutputBrokers; n++) {
            outputBrokersAccel[n]->SetEnabled(false);
        }
    }
    return ret;
}
CLASS_REGISTER(TriggeredIOGAM, "1.0")
}

