/**
 * @file DoubleHandshakeMasterGAM.cpp
 * @brief Source file for class DoubleHandshakeMasterGAM
 * @date 01/08/2019
 * @author Giuseppe Ferrò
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
 * the class DoubleHandshakeMasterGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "DoubleHandshakeMasterGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

//#define NULL_PTR(x) NULL

DoubleHandshakeMasterGAM::DoubleHandshakeMasterGAM() :
        GAM() {
    numberOfInputCommands = 0u;
    inputs = NULL_PTR(uint8*);
    outputs = NULL_PTR(uint8*);
    inputCommandSize = NULL_PTR(uint32*);
    inputCommandOffset = NULL_PTR(uint32*);
    inputAckOffset = NULL_PTR(uint32*);
    inputClearTrigOffset = NULL_PTR(uint32*);
    outputCommandOffset = NULL_PTR(uint32*);
    state = NULL_PTR(uint8*);
    previousValue = NULL_PTR(uint8*);
    startSMCycleTime = NULL_PTR(uint64*);
    timeout = 0xFFFFFFFFFFFFFFFFu;
}

DoubleHandshakeMasterGAM::~DoubleHandshakeMasterGAM() {
    inputs = NULL_PTR(uint8*);
    outputs = NULL_PTR(uint8*);
    state = NULL_PTR(uint8*);
    if (inputCommandSize != NULL_PTR(uint32*)) {
        delete[] inputCommandSize;
    }
    if (inputCommandOffset != NULL_PTR(uint32*)) {
        delete[] inputCommandOffset;
    }
    if (inputAckOffset != NULL_PTR(uint32*)) {
        delete[] inputAckOffset;
    }
    if (inputClearTrigOffset != NULL_PTR(uint32*)) {
        delete[] inputClearTrigOffset;
    }
    if (outputCommandOffset != NULL_PTR(uint32*)) {
        delete[] outputCommandOffset;
    }
    if (previousValue != NULL_PTR(uint8*)) {
        delete[] previousValue;
    }
    if (startSMCycleTime != NULL_PTR(uint64*)) {
        delete[] startSMCycleTime;
    }
}

bool DoubleHandshakeMasterGAM::Initialise(StructuredDataI & data) {
    bool ret = GAM::Initialise(data);
    if (ret) {
        timeout = 0xFFFFFFFFFFFFFFFFu;
        float64 timeoutMsecs;
        if (data.Read("Timeout", timeoutMsecs)) {
            float64 timeoutSecs = (timeoutMsecs * 1e-3);
            float64 freq = static_cast<float64>(HighResolutionTimer::Frequency());
            float64 timeoutF=(timeoutSecs * freq);
            timeout = static_cast<uint64>(timeoutF);
        }
    }
    return ret;
}

bool DoubleHandshakeMasterGAM::Setup() {
    // INPUTS:
    // internal commands
    // acks from plc
    // clear triggers

    // OUTPUTS:
    // commands to PLC
    // state of the internal SM

    // constraints:
    // internal commands==commands to PLC
    // acks from PLC==internal commands
    // clear triggers==internal commands
    // state of the internal SM==internal commands
    uint32 numberOfInputAcks = 0u;
    uint32 numberOfClearTriggers = 0u;

    // internal command identifier
    const char8 *commandInId = "CommandIn";

    // ack from PLC identifier
    const char8 *ackInId = "AckIn";

    // clear trigger identifier
    const char8 *clearTrigInId = "ClearIn";
    uint32 commandInIdLen = StringHelper::Length(commandInId);
    uint32 ackIdLen = StringHelper::Length(ackInId);
    uint32 clearTrigInIdLen = StringHelper::Length(clearTrigInId);

    bool ret = true;
    // compute the number of commands, acks and clears
    for (uint32 i = 0u; (i < numberOfInputSignals) && (ret); i++) {
        uint32 numberOfElements = 0u;

        ret = GetSignalNumberOfElements(InputSignals, i, numberOfElements);
        StreamString signalName;

        if (ret) {
            ret = GetSignalName(InputSignals, i, signalName);
        }
        if (ret) {
            if (StringHelper::CompareN(signalName.Buffer(), commandInId, commandInIdLen) == 0) {
                numberOfInputCommands += numberOfElements;
            }

            if (StringHelper::CompareN(signalName.Buffer(), ackInId, ackIdLen) == 0) {
                numberOfInputAcks += numberOfElements;
            }

            if (StringHelper::CompareN(signalName.Buffer(), clearTrigInId, clearTrigInIdLen) == 0) {
                numberOfClearTriggers += numberOfElements;
            }
        }
    }

    // check the constraints
    if (ret) {
        ret = (numberOfInputCommands == numberOfInputAcks);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "The number of input commands (%d) must be equal to the number of acks (%d)", numberOfInputCommands,
                         numberOfInputAcks);
        }
    }
    if (ret) {
        ret = (numberOfInputCommands == numberOfClearTriggers);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "The number of input commands (%d) must be equal to the number of clear triggers (%d)",
                         numberOfInputCommands, numberOfClearTriggers);
        }
    }

    // compute offsets and sizes
    if (ret) {
        startSMCycleTime = new uint64[numberOfInputCommands];

        inputCommandSize = new uint32[numberOfInputCommands];
        inputCommandOffset = new uint32[numberOfInputCommands];
        inputAckOffset = new uint32[numberOfInputCommands];
        inputClearTrigOffset = new uint32[numberOfInputCommands];
        uint32 commandCounter = 0u;
        uint32 replyStatusCounter = 0u;
        uint32 ackCounter = 0u;
        uint32 offset = 0u;
        for (uint32 i = 0u; (i < numberOfInputSignals) && (ret); i++) {
            StreamString signalName;
            ret = GetSignalName(InputSignals, i, signalName);

            uint32 numberOfElements = 0u;
            if (ret) {
                ret = GetSignalNumberOfElements(InputSignals, i, numberOfElements);
            }
            TypeDescriptor td;
            if (ret) {
                td = GetSignalType(InputSignals, i);
                uint32 size = (static_cast<uint32>(td.numberOfBits) / 8u);

                if (StringHelper::CompareN(signalName.Buffer(), commandInId, commandInIdLen) == 0) {
                    for (uint32 j = 0u; j < numberOfElements; j++) {
                        /*lint -e{613} NULL pointer checked.*/
                        startSMCycleTime[commandCounter] = 0ull;
                        /*lint -e{613} NULL pointer checked.*/
                        inputCommandSize[commandCounter] = size;
                        /*lint -e{613} NULL pointer checked.*/
                        inputCommandOffset[commandCounter] = offset;
                        commandCounter++;
                        offset += size;
                    }
                }
                else if (StringHelper::CompareN(signalName.Buffer(), ackInId, ackIdLen) == 0) {
                    for (uint32 j = 0u; (j < numberOfElements) && (ret); j++) {
                        /*lint -e{613} NULL pointer checked.*/
                        ret = (size == inputCommandSize[ackCounter]);
                        if (ret) {
                            /*lint -e{613} NULL pointer checked.*/
                            inputAckOffset[ackCounter] = offset;
                            ackCounter++;
                            offset += size;
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::FatalError, "The size of the input commands must be equal to the size of the ack (%d)", ackCounter);
                        }
                    }
                }
                else if (StringHelper::CompareN(signalName.Buffer(), clearTrigInId, clearTrigInIdLen) == 0) {
                    for (uint32 j = 0u; j < numberOfElements; j++) {
                        /*lint -e{613} NULL pointer checked.*/
                        inputClearTrigOffset[replyStatusCounter] = offset;
                        replyStatusCounter++;
                        ret = (td == UnsignedInteger32Bit);
                        if (ret) {
                            offset += static_cast<uint32>(sizeof(uint32));
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::FatalError, "The type of the input clear triggers must be uint32");
                        }
                    }
                }
                else {
                    offset += size;
                }
            }

        }
        //create a copy of the buffer
        if (ret) {
            previousValue = new uint8[offset];
            ret = MemoryOperationsHelper::Set(previousValue, '\0', offset);
        }
    }

    //
    uint32 numberOfOutputCommands = 0u;
    uint32 numberOfOutputStatuses = 0u;
    uint32 stateSignalIndex = 0u;
    const char8 *commandOutId = "CommandOut";
    const char8 *stateOutId = "InternalState";
    uint32 commandOutIdLen = StringHelper::Length(commandOutId);
    uint32 stateIdLen = StringHelper::Length(stateOutId);

    //compute the number of
    if (ret) {
        for (uint32 i = 0u; (i < numberOfOutputSignals) && (ret); i++) {
            uint32 numberOfElements = 0u;
            ret = GetSignalNumberOfElements(OutputSignals, i, numberOfElements);

            StreamString signalName;
            if (ret) {
                ret = GetSignalName(OutputSignals, i, signalName);
            }
            if (ret) {
                if (StringHelper::CompareN(signalName.Buffer(), commandOutId, commandOutIdLen) == 0) {
                    numberOfOutputCommands += numberOfElements;
                }
                if (StringHelper::CompareN(signalName.Buffer(), stateOutId, stateIdLen) == 0) {
                    if (numberOfOutputStatuses == 0u) {
                        stateSignalIndex = i;
                    }
                    numberOfOutputStatuses += numberOfElements;
                }
            }
        }
    }

    if (ret) {
        ret = (numberOfInputCommands == numberOfOutputCommands);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "The number of input commands (%d) must be equal to the number of output commands (%d)",
                         numberOfInputCommands, numberOfOutputCommands);
        }
    }
    if (ret) {
        ret = (numberOfInputCommands == numberOfOutputStatuses);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "The number of input commands (%d) must be equal to the number of output statuses (%d)",
                         numberOfInputCommands, numberOfOutputStatuses);
        }
    }
    if (ret) {
        outputCommandOffset = new uint32[numberOfInputCommands];
        uint32 commandCounter = 0u;
        uint32 offset = 0u;
        /*lint -e{850} the index is not modified inside the loop*/
        for (uint32 i = 0u; (i < numberOfOutputSignals) && (ret); i++) {
            StreamString signalName;
            ret = GetSignalName(OutputSignals, i, signalName);

            uint32 numberOfElements = 0u;
            if (ret) {
                ret = GetSignalNumberOfElements(OutputSignals, i, numberOfElements);
            }
            TypeDescriptor td;
            if (ret) {
                td = GetSignalType(OutputSignals, i);
                uint32 size = (static_cast<uint32>(td.numberOfBits) / 8u);
                //uint32 commandIdLen = StringHelper::Length(commandOutId);
                if (StringHelper::CompareN(signalName.Buffer(), commandOutId, commandOutIdLen) == 0) {
                    for (uint32 j = 0u; (j < numberOfElements) && (ret); j++) {
                        /*lint -e{613} NULL pointer checked.*/
                        ret = (size == inputCommandSize[commandCounter]);
                        if (ret) {
                            /*lint -e{613} NULL pointer checked.*/
                            outputCommandOffset[commandCounter] = offset;
                            commandCounter++;
                            offset += size;
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::FatalError, "The size of the input commands must be equal to the size of the output commands (%d)",
                                         commandCounter);
                        }
                    }
                }
            }
            if(ret) {
                if (StringHelper::CompareN(signalName.Buffer(), stateOutId, stateIdLen) == 0) {
                    ret = (td == UnsignedInteger8Bit);
                    if (!ret) {
                        StreamString tempSignalName;
                        bool retSigName = GetSignalName(OutputSignals, i, tempSignalName);
                        REPORT_ERROR(ErrorManagement::FatalError, "The type of the internal state (%d) must be uint8 - %s", i, tempSignalName.Buffer());
                        if(!retSigName) {
                            REPORT_ERROR(ErrorManagement::FatalError, "Failure to get signal name from OutputSignals at index i", i);
                        }
                    }
                }
            }
        }
    }

    if (ret) {
        inputs = reinterpret_cast<uint8*>(GetInputSignalsMemory());
        outputs = reinterpret_cast<uint8*>(GetOutputSignalsMemory());
        state = reinterpret_cast<uint8*>(GetOutputSignalMemory(stateSignalIndex));
    }
    return ret;
}

bool DoubleHandshakeMasterGAM::Execute() {

    /*lint -e{850} the loop variable is not modified within the loop*/
    for (uint32 i = 0u; i < numberOfInputCommands; i++) {
        /*lint -e{613} NULL pointer checked.*/
        uint64 elapsed = (HighResolutionTimer::Counter() - startSMCycleTime[i]);

        /*lint -e{613} NULL pointer checked.*/
        uint32 inCommandOffset = inputCommandOffset[i];
        /*lint -e{613} NULL pointer checked.*/
        uint32 outCommandOffset = outputCommandOffset[i];
        /*lint -e{613} NULL pointer checked.*/
        uint32 commandSize = inputCommandSize[i];
        /*lint -e{613} NULL pointer checked.*/
        uint32 ackOffset = inputAckOffset[i];

        /*lint -e{613} NULL pointer checked.*/
        if (state[i] == ERROR) {
            //if reset from PLC return to wait initial state
            if (IsChanged(i) == -1) {
                /*lint -e{613} NULL pointer checked.*/
                state[i] = READY;
                REPORT_ERROR(ErrorManagement::Information, "GOTO READY[%d]", i);
            }
            /*lint -e{613} NULL pointer checked.*/
            (void) MemoryOperationsHelper::Copy(&previousValue[inCommandOffset], &inputs[inCommandOffset], commandSize);
            //REPORT_ERROR(ErrorManagement::Information, "ERROR[%d]", i);
        }

        //communication channel state machine
        /*lint -e{613} NULL pointer checked.*/
        else if (state[i] == READY) {
            //if the ack is different than zero and ack then go to error
            for (uint32 k = 0u; k < commandSize; k++) {
                uint32 ackIndex = (ackOffset + k);
                /*lint -e{613} NULL pointer checked.*/
                if (inputs[ackIndex] != 0u) {
                    REPORT_ERROR(ErrorManagement::Information, "GOTO ERROR[%d]", i);
                    /*lint -e{613} NULL pointer checked.*/
                    state[i] = ERROR;
                    break;
                }
            }
            if (state[i] == READY) {
                //check a difference with the previous
                // internal command changed... send to PLC the command
                bool rising = (IsChanged(i) == 1);
                bool onlyChanged = (IsChanged(i) == -2);
                //the signal is not falling to zero
                if (rising || onlyChanged) {
                    (void) MemoryOperationsHelper::Copy(&outputs[outCommandOffset], &inputs[inCommandOffset], commandSize);
                    REPORT_ERROR(ErrorManagement::Information, "GOTO SENDING[%d]", i);
                    /*lint -e{613} NULL pointer checked.*/
                    state[i] = SENDING;
                    startSMCycleTime[i] = HighResolutionTimer::Counter();
                }
                (void) MemoryOperationsHelper::Copy(&previousValue[inCommandOffset], &inputs[inCommandOffset], commandSize);
            }
        }
        /*lint -e{613} NULL pointer checked.*/
        else if (state[i] == SENDING) {
            if (elapsed > timeout) {
                REPORT_ERROR(ErrorManagement::Information, "GOTO ERROR[%d]", i);
                state[i] = ERROR;
            }
            else {
                //wait for the PLC acking the command
                //command acked... set status to trigger internal MARTe changes and go to clear
                if (MemoryOperationsHelper::Compare(&previousValue[inCommandOffset], &inputs[ackOffset], commandSize) == 0) {
                    REPORT_ERROR(ErrorManagement::Information, "GOTO CLEAR[%d]", i);
                    /*lint -e{613} NULL pointer checked.*/
                    state[i] = CLEAR;
                }
                else {
                    //if the ack is different than zero and ack then go to error
                    for (uint32 k = 0u; k < commandSize; k++) {
                        uint32 ackIndex = (ackOffset + k);
                        if (inputs[ackIndex] != 0u) {
                            REPORT_ERROR(ErrorManagement::Information, "GOTO ERROR[%d]", i);
                            /*lint -e{613} NULL pointer checked.*/
                            state[i] = ERROR;
                            break;
                        }
                    }
                }
            }
        }
        /*lint -e{613} NULL pointer checked.*/
        else if (state[i] == CLEAR) {
            if (elapsed > timeout) {
                REPORT_ERROR(ErrorManagement::Information, "GOTO ERROR[%d]", i);
                /*lint -e{613} NULL pointer checked.*/
                state[i] = ERROR;
            }
            else {
                //wait for internal state change and clear ack
                /*lint -e{927} -e{826} needed pointer to pointer conversion*/
                uint32 clearAck = *reinterpret_cast<uint32*>(&inputs[inputClearTrigOffset[i]]);
                //if the PLC change the ack go to error
                if (MemoryOperationsHelper::Compare(&previousValue[inCommandOffset], &inputs[ackOffset], commandSize) != 0) {
                    REPORT_ERROR(ErrorManagement::Information, "GOTO ERROR[%d]", i);
                    /*lint -e{613} NULL pointer checked.*/
                    state[i] = ERROR;
                }
                else {
                    if (clearAck == 0u) {
                        //clear the ack
                        (void) MemoryOperationsHelper::Set(&outputs[outCommandOffset], '\0', inputCommandSize[i]);
                        REPORT_ERROR(ErrorManagement::Information, "GOTO DONE[%d]", i);
                        /*lint -e{613} NULL pointer checked.*/
                        state[i] = DONE;
                    }
                }
            }
        }
        else if (state[i] == DONE) {
            if (elapsed > timeout) {
                REPORT_ERROR(ErrorManagement::Information, "GOTO ERROR[%d]", i);
                /*lint -e{613} NULL pointer checked.*/
                state[i] = ERROR;
            }
            else {
                //wait for the PLC clear ack
                if (MemoryOperationsHelper::Compare(&inputs[ackOffset], &outputs[outCommandOffset], commandSize) == 0) {
                    REPORT_ERROR(ErrorManagement::Information, "GOTO READY[%d]", i);
                    /*lint -e{613} NULL pointer checked.*/
                    state[i] = READY;
                }
                else {
                    //if the PLC change the ack go to error
                    if (MemoryOperationsHelper::Compare(&previousValue[inCommandOffset], &inputs[ackOffset], commandSize) != 0) {
                        /*lint -e{613} NULL pointer checked.*/
                        state[i] = ERROR;
                        REPORT_ERROR(ErrorManagement::Information, "GOTO ERROR[%d]", i);
                    }
                }
            }
        }
        else{

        }
    }

    return true;
}

int32 DoubleHandshakeMasterGAM::IsChanged(const uint32 cIdx) const {
//check if the command i had a rising event
    /*lint -e{613} NULL pointer checked.*/
    uint32 commandOffset = inputCommandOffset[cIdx];
    /*lint -e{613} NULL pointer checked.*/
    uint32 commandSize = inputCommandSize[cIdx];
    /*lint -e{613} NULL pointer checked.*/
    bool ret = (MemoryOperationsHelper::Compare(&inputs[commandOffset], &previousValue[commandOffset], commandSize) != 0);
    int32 res = -2;
    if (!ret) {
        res = 0;
    }
    if (ret) {
        bool rising = true;
        //check that previousValue is 0
        for (uint32 i = 0u; i < commandSize; i++) {
            uint32 index = (commandOffset + i);
            /*lint -e{613} NULL pointer checked.*/
            if (previousValue[index] != 0u) {
                rising = false;
                break;
            }
        }
        if (rising) {
            res = 1;
        }
        else {
            bool falling = true;
            //check that currentValue is 0
            for (uint32 i = 0u; i < commandSize; i++) {
                uint32 index = (commandOffset + i);
                /*lint -e{613} NULL pointer checked.*/
                if (inputs[index] != 0u) {
                    falling = false;
                    break;
                }
            }
            if (falling) {
                res = -1;
            }
        }
    }
    return res;
}

CLASS_REGISTER(DoubleHandshakeMasterGAM, "1.0")

}
