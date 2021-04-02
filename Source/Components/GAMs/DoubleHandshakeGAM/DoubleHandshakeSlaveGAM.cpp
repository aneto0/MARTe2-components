/**
 * @file DoubleHandshakeSlaveGAM.cpp
 * @brief Source file for class DoubleHandshakeSlaveGAM
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
 * the class DoubleHandshakeSlaveGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "DoubleHandshakeSlaveGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

DoubleHandshakeSlaveGAM::DoubleHandshakeSlaveGAM() :
        GAM() {
    numberOfInputCommands = 0u;
    inputCommandSize = NULL_PTR(uint32*);
    inputCommandOffset = NULL_PTR(uint32*);
    outputAckOffset = NULL_PTR(uint32*);
    inputClearTrigOffset = NULL_PTR(uint32*);
    inputs = NULL_PTR(uint8*);
    outputs = NULL_PTR(uint8*);
    previousValue = NULL_PTR(uint8*);
    state = NULL_PTR(uint8*);
    startSMCycleTime = NULL_PTR(uint64*);
    timeout = 0xFFFFFFFFFFFFFFFFu;
}

DoubleHandshakeSlaveGAM::~DoubleHandshakeSlaveGAM() {
    inputs = NULL_PTR(uint8*);
    outputs = NULL_PTR(uint8*);
    state = NULL_PTR(uint8*);
    if (inputCommandSize != NULL_PTR(uint32*)) {
        delete[] inputCommandSize;
    }
    if (inputCommandOffset != NULL_PTR(uint32*)) {
        delete[] inputCommandOffset;
    }
    if (outputAckOffset != NULL_PTR(uint32*)) {
        delete[] outputAckOffset;
    }
    if (inputClearTrigOffset != NULL_PTR(uint32*)) {
        delete[] inputClearTrigOffset;
    }
    if (previousValue != NULL_PTR(uint8*)) {
        delete[] previousValue;
    }
    if (startSMCycleTime != NULL_PTR(uint64*)) {
        delete[] startSMCycleTime;
    }
}

bool DoubleHandshakeSlaveGAM::Initialise(StructuredDataI & data) {
    bool ret = GAM::Initialise(data);
    if (ret) {
        timeout = 0xFFFFFFFFFFFFFFFFu;
        uint32 timeoutMsecs;
        if (data.Read("Timeout", timeoutMsecs)) {
            float64 timeoutSecs = static_cast<float64>(timeoutMsecs) * 1.0e-3;
            float64 freq = static_cast<float64>(HighResolutionTimer::Frequency());
            float64 timeoutF = (timeoutSecs * freq);
            timeout = static_cast<uint64>(timeoutF);
        }
    }
    return ret;
}

bool DoubleHandshakeSlaveGAM::Setup() {
    // INPUTS:
    // commands from plc
    // clear triggers

    // OUTPUTS:
    // acks to PLC
    // state of the internal SM

    // constraints:
    // commands from PLC==clear triggers
    // commands from PLC==acks to PLC
    // commands from PLC==state of the internal SM
    uint32 numberOfClearTriggers = 0u;

    // internal command identifier
    const char8 *commandInId = "CommandIn";

    // clear trigger identifier
    const char8 *clearTrigInId = "ClearIn";
    uint32 commandInIdLen = StringHelper::Length(commandInId);
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

            if (StringHelper::CompareN(signalName.Buffer(), clearTrigInId, clearTrigInIdLen) == 0) {
                numberOfClearTriggers += numberOfElements;
            }
        }
    }

    if (ret) {
        // check the constraints
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
        inputClearTrigOffset = new uint32[numberOfInputCommands];
        uint32 commandCounter = 0u;
        uint32 replyStatusCounter = 0u;
        uint32 offset = 0u;
        for (uint32 i = 0u; (i < numberOfInputSignals) && (ret); i++) {
            StreamString signalName;

            ret = GetSignalName(InputSignals, i, signalName);

            uint32 numberOfElements = 0u;
            if (ret) {
                ret = GetSignalNumberOfElements(InputSignals, i, numberOfElements);
            }

            if (ret) {
                TypeDescriptor td = GetSignalType(InputSignals, i);
                uint32 size = static_cast<uint32>(td.numberOfBits) / 8u;

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
        if (ret) {
            //create a copy of the buffer
            previousValue = new uint8[offset];
            ret = MemoryOperationsHelper::Set(previousValue, '\0', offset);
        }
    }

    //
    uint32 numberOfOutputAcks = 0u;
    uint32 numberOfOutputStatuses = 0u;

    uint32 stateSignalIndex = 0u;

    // ack to PLC identifier
    const char8 *ackOutId = "AckOut";
    uint32 ackOutIdLen = StringHelper::Length(ackOutId);

    const char8 *stateOutId = "InternalState";
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
                if (StringHelper::CompareN(signalName.Buffer(), ackOutId, ackOutIdLen) == 0) {
                    numberOfOutputAcks += numberOfElements;
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
        ret = (numberOfInputCommands == numberOfOutputAcks);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "The number of input commands (%d) must be equal to the number of output acks (%d)",
                         numberOfInputCommands, numberOfOutputAcks);
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
        outputAckOffset = new uint32[numberOfInputCommands];
        uint32 ackCounter = 0u;

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

                if (StringHelper::CompareN(signalName.Buffer(), ackOutId, ackOutIdLen) == 0) {
                    for (uint32 j = 0u; (j < numberOfElements) && (ret); j++) {
                        /*lint -e{613} NULL pointer checked.*/
                        ret = (size == inputCommandSize[ackCounter]);
                        if (ret) {
                            /*lint -e{613} NULL pointer checked.*/
                            outputAckOffset[ackCounter] = offset;
                            ackCounter++;
                            offset += size;
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::FatalError, "The size of the input commands must be equal to the size of the output acks (%d)",
                                         ackCounter);
                        }
                    }
                }
            }
            if(ret) {
                if (StringHelper::CompareN(signalName.Buffer(), stateOutId, stateIdLen) == 0) {
                    ret = (td == UnsignedInteger8Bit);
                    if (!ret) {
                        StreamString tempSignalName;
                        bool signalNameRet = GetSignalName(OutputSignals, i, tempSignalName);
                        REPORT_ERROR(ErrorManagement::FatalError, "The type of the internal state (%d) must be uint8 - %s", i, tempSignalName.Buffer());
                        if(!signalNameRet) {
                            REPORT_ERROR(ErrorManagement::FatalError, "Cannot get output signal name at index %d", i);
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

bool DoubleHandshakeSlaveGAM::Execute() {

    /*lint -e{850} the loop variable is not modified within the loop*/
    for (uint32 i = 0u; i < numberOfInputCommands; i++) {
        /*lint -e{613} NULL pointer checked.*/
        uint64 elapsed = (HighResolutionTimer::Counter() - startSMCycleTime[i]);
        /*lint -e{613} NULL pointer checked.*/
        uint32 inCommandOffset = inputCommandOffset[i];
        /*lint -e{613} NULL pointer checked.*/
        uint32 commandSize = inputCommandSize[i];
        /*lint -e{613} NULL pointer checked.*/
        uint32 ackOffset = outputAckOffset[i];

        /*lint -e{613} NULL pointer checked.*/
        if (state[i] == ERROR) {
            //if reset from PLC return to wait initial state
            if (IsChanged(i) == -1) {
                /*lint -e{613} NULL pointer checked.*/
                state[i] = READY;
                (void) MemoryOperationsHelper::Set(&outputs[ackOffset], '\0', inputCommandSize[i]);
                (void) MemoryOperationsHelper::Set(&previousValue[inCommandOffset], '\0', inputCommandSize[i]);

                REPORT_ERROR(ErrorManagement::Information, "GOTO READY[%d]", i);
            }
            /*lint -e{613} NULL pointer checked.*/
            //REPORT_ERROR(ErrorManagement::Information, "ERROR[%d]", i);
        }

        //communication channel state machine
        /*lint -e{613} NULL pointer checked.*/
        else if (state[i] == READY) {

            // check if PLC sent command
            if (IsChanged(i) == 1) {
                (void) MemoryOperationsHelper::Copy(&previousValue[inCommandOffset], &inputs[inCommandOffset], commandSize);
                REPORT_ERROR(ErrorManagement::Information, "GOTO SENDING[%d]", i);
                /*lint -e{613} NULL pointer checked.*/
                startSMCycleTime[i] = HighResolutionTimer::Counter();
                /*lint -e{613} NULL pointer checked.*/
                state[i] = SENDING;
            }
        }
        /*lint -e{613} NULL pointer checked.*/
        else if (state[i] == SENDING) {
            if (elapsed > timeout) {
                REPORT_ERROR(ErrorManagement::Information, "GOTO ERROR[%d]", i);
                /*lint -e{613} NULL pointer checked.*/
                state[i] = ERROR;
            }
            else {
                //wait for the internal processing
                //go to error if plc change the command or reset
                if (MemoryOperationsHelper::Compare(&inputs[inCommandOffset], &previousValue[inCommandOffset], commandSize) != 0) {
                    REPORT_ERROR(ErrorManagement::Information, "GOTO ERROR[%d]", i);
                    /*lint -e{613} NULL pointer checked.*/
                    state[i] = ERROR;
                }
                else {
                    /*lint -e{927} -e{826} needed pointer to pointer conversion*/
                    uint32 clearTrig = *reinterpret_cast<uint32*>(&inputs[inputClearTrigOffset[i]]);
                    if (clearTrig == 0u) {
                        //send the ack
                        (void) MemoryOperationsHelper::Copy(&outputs[ackOffset], &inputs[inCommandOffset], inputCommandSize[i]);
                        REPORT_ERROR(ErrorManagement::Information, "GOTO DONE[%d]", i);
                        /*lint -e{613} NULL pointer checked.*/
                        state[i] = DONE;
                    }
                }
            }
        }
        /*lint -e{613} NULL pointer checked.*/
        else if (state[i] == DONE) {
            if (elapsed > timeout) {
                REPORT_ERROR(ErrorManagement::Information, "GOTO ERROR[%d]", i);
                /*lint -e{613} NULL pointer checked.*/
                state[i] = ERROR;
            }
            else {
                //wait for clear
                if (IsChanged(i) == -1) {
                    (void) MemoryOperationsHelper::Set(&outputs[ackOffset], '\0', inputCommandSize[i]);
                    (void) MemoryOperationsHelper::Set(&previousValue[inCommandOffset], '\0', inputCommandSize[i]);
                    REPORT_ERROR(ErrorManagement::Information, "GOTO READY[%d]", i);
                    /*lint -e{613} NULL pointer checked.*/
                    state[i] = READY;
                }
                else {
                    //if not for clear but changed... go to error
                    if (MemoryOperationsHelper::Compare(&inputs[inCommandOffset], &previousValue[inCommandOffset], commandSize) != 0) {
                        REPORT_ERROR(ErrorManagement::Information, "GOTO ERROR[%d]", i);
                        /*lint -e{613} NULL pointer checked.*/
                        state[i] = ERROR;
                    }
                }
            }
        }
        else{

        }
    }
    return true;
}

int32 DoubleHandshakeSlaveGAM::IsChanged(const uint32 cIdx) const {
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

CLASS_REGISTER(DoubleHandshakeSlaveGAM, "1.0")

}
