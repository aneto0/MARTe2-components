/**
 * @file TriggerOnChangeGAM.cpp
 * @brief Source file for class TriggerOnChangeGAM
 * @date 06/07/2018
 * @author Giuseppe Ferro
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
 * the class TriggerOnChangeGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TriggerOnChangeGAM.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

//#define NULL_PTR(x) NULL

TriggerOnChangeGAM::TriggerOnChangeGAM() :
        GAM() {

    state = NULL_PTR(uint32*);
    packetConfig = NULL_PTR(PacketField *);
    previousValue = NULL_PTR(uint8*);

    numberOfFields = 0u;

    commandIndex = NULL_PTR(uint32*);
    numberOfCommands = 0u;
    numberOfEvents = 0u;

    cntReplied = NULL_PTR(uint32*);
    cntTrigger = NULL_PTR(uint32*);

    currentValue = NULL_PTR(uint8*);
    ackValue = NULL_PTR(uint8*);

}

TriggerOnChangeGAM::~TriggerOnChangeGAM() {

    if (packetConfig != NULL_PTR(PacketField *)) {
        delete[] packetConfig;
    }

    if (previousValue != NULL_PTR(uint8*)) {
        delete[] previousValue;
    }

    if (commandIndex != NULL_PTR(uint32*)) {
        delete[] commandIndex;
    }
    if (cntReplied != NULL_PTR(uint32*)) {
        delete[] cntReplied;
    }
    if (cntTrigger != NULL_PTR(uint32*)) {
        delete[] cntTrigger;
    }

    state = NULL_PTR(uint32*);
    packetConfig = NULL_PTR(PacketField *);
    previousValue = NULL_PTR(uint8*);

    numberOfFields = 0u;

    commandIndex = NULL_PTR(uint32*);
    numberOfCommands = 0u;

    numberOfEvents = 0u;

    cntReplied = NULL_PTR(uint32*);
    cntTrigger = NULL_PTR(uint32*);

    currentValue = NULL_PTR(uint8*);
    ackValue = NULL_PTR(uint8*);
}

bool TriggerOnChangeGAM::Setup() {

    numberOfFields = 0u;
    bool ret = true;

    uint32 numberOfSignals = GetNumberOfInputSignals();

    for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
        uint32 numberOfElements = 0u;
        ret = GetSignalNumberOfElements(InputSignals, i, numberOfElements);
        if (ret) {
            numberOfFields += numberOfElements;
        }
    }

    uint32 packetSize = 0u;
    if (ret) {
        /*lint -e{613} NULL pointer checked.*/
        packetConfig = new PacketField[numberOfFields];
        ret = (packetConfig != NULL_PTR(PacketField*));

        uint32 n = 0u;

        /*lint -e{850} the loop variable is not modified within the loop*/
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            StreamString signalName;
            ret = GetSignalName(InputSignals, i, signalName);
            /*lint -e{613} NULL pointer checked.*/
            packetConfig[n].name = signalName;
            if (ret) {
                TypeDescriptor type = GetSignalType(InputSignals, i);
                /*lint -e{613} NULL pointer checked.*/
                packetConfig[n].type = type;
                /*lint -e{613} NULL pointer checked.*/
                bool isCommand = (StringHelper::CompareN(signalName.Buffer(), "Command", StringHelper::Length("Command")) == 0);
                /*lint -e{613} NULL pointer checked.*/
                packetConfig[n].isCommand = isCommand;
                /*lint -e{613} NULL pointer checked.*/
                if (packetConfig[n].isCommand) {
                    numberOfCommands++;
                }

                /*lint -e{613} NULL pointer checked.*/
                uint32 typeSize = (static_cast<uint32>(packetConfig[i].type.numberOfBits) / 8u);
                /*lint -e{613} NULL pointer checked.*/
                packetConfig[n].offset = packetSize;
                packetSize += typeSize;

                uint32 numberOfElements = 0u;
                ret = GetSignalNumberOfElements(InputSignals, i, numberOfElements);
                if (ret) {
                    if (numberOfElements > 1u) {
                        for (uint32 j = 0u; (j < numberOfElements) && (ret); j++) {
                            StreamString newSignalName = signalName;
                            ret = newSignalName.Printf("_%d", j);
                            if (ret) {
                                /*lint -e{613} NULL pointer checked.*/
                                packetConfig[n].name = newSignalName;
                                if (j > 0u) {
                                    /*lint -e{613} NULL pointer checked.*/
                                    packetConfig[n].type = type;
                                    /*lint -e{613} NULL pointer checked.*/
                                    packetConfig[n].isCommand = isCommand;
                                    /*lint -e{613} NULL pointer checked.*/
                                    if (packetConfig[n].isCommand) {
                                        numberOfCommands++;
                                    }
                                    /*lint -e{613} NULL pointer checked.*/
                                    packetConfig[n].offset = packetSize;
                                    packetSize += typeSize;
                                }
                            }
                            n++;
                        }
                    }
                    else {
                        n++;
                    }
                }
            }
        }
    }

    if (ret) {
        ret = (numberOfCommands > 0u);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError,
                         "The number of commands is 0. Please remember that the command signals must begin with \"Command\" prefix");
        }
    }

    if (ret) {
        previousValue = new uint8[packetSize];
        (void) MemoryOperationsHelper::Set(previousValue, '\0', packetSize);

        commandIndex = new uint32[numberOfCommands];
        cntTrigger = new uint32[numberOfCommands];
        cntReplied = new uint32[numberOfCommands];
        uint32 c = 0u;
        for (uint32 i = 0u; i < numberOfFields; i++) {
            /*lint -e{613} NULL pointer checked.*/
            if (packetConfig[i].isCommand) {
                commandIndex[c] = i;
                cntTrigger[c] = 0u;
                cntReplied[c] = 0u;
                c++;
            }
        }

//build the lookup table
        events = this->Find("Events");
        ret = events.IsValid();
        if (ret) {
            numberOfEvents = events->Size();
            for (uint32 i = 0u; i < numberOfEvents; i++) {
                ReferenceT<EventConditionTrigger> eventCondition = events->Get(i);
                ret = eventCondition.IsValid();
                if (ret) {
                    ret = eventCondition->SetPacketConfig(packetConfig, numberOfFields);
                }
            }
        }

    }

    if (ret) {
        //check that the state signal has siz #elements
        uint32 numberOfStateElements = 0u;
        uint32 stateIndex = GetNumberOfOutputSignals() - 1u;
        ret = GetSignalNumberOfElements(OutputSignals, stateIndex, numberOfStateElements);
        if (ret) {
            ret = (numberOfStateElements == numberOfCommands);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The number of elements of the last signal (internal state) must be %d but is %d",
                             numberOfCommands, numberOfStateElements);
            }
        }
        if (ret) {
            TypeDescriptor td = GetSignalType(OutputSignals, stateIndex);
            ret = (td == UnsignedInteger32Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The type of the last signal (internal state) must be uint32");
            }
        }
    }

    //match only the size (more flexibility)
    uint32 nOutputSignals = GetNumberOfOutputSignals() - 1u;
    if (ret) {
        uint32 inputSize = 0u;
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            uint32 signalSize = 0u;
            ret = GetSignalByteSize(InputSignals, i, signalSize);
            if (ret) {
                inputSize += signalSize;
            }
        }
        //dont count the last state signal
        uint32 outputSize = 0u;
        for (uint32 i = 0u; (i < nOutputSignals) && (ret); i++) {
            uint32 signalSize = 0u;
            ret = GetSignalByteSize(OutputSignals, i, signalSize);
            if (ret) {
                outputSize += signalSize;
            }
        }
        ret = (inputSize == outputSize);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "The input %d and output %d sizes (except the last state signal) must match", inputSize, outputSize);
        }
    }

    if (ret) {
        currentValue = reinterpret_cast<uint8*>(GetInputSignalsMemory());
        ackValue = reinterpret_cast<uint8*>(GetOutputSignalsMemory());
        state = reinterpret_cast<uint32*>(GetOutputSignalMemory(nOutputSignals));
        ret = ((currentValue != NULL_PTR(uint8*)) && (ackValue != NULL_PTR(uint8*)) && (state != NULL_PTR(uint32*)));
    }

    return ret;
}

bool TriggerOnChangeGAM::Execute() {

    /*lint -e{850} the loop variable is not modified within the loop*/
    for (uint32 i = 0u; i < numberOfCommands; i++) {
        /*lint -e{613} NULL pointer checked.*/
        if (state[i] == ERROR) {
            //if reset from PLC return to wait inital state
            if (IsChanged(i) == -1) {
                /*lint -e{613} NULL pointer checked.*/
                state[i] = READY;
            }
            /*lint -e{613} NULL pointer checked.*/
            (void) MemoryOperationsHelper::Copy(&previousValue[packetConfig[commandIndex[i]].offset], &currentValue[packetConfig[commandIndex[i]].offset],
                                                (static_cast<uint32>(packetConfig[commandIndex[i]].type.numberOfBits) / 8u));
            //REPORT_ERROR(ErrorManagement::Information, "ERROR[%d]", i);
        }

        //communication channel state machine
        /*lint -e{613} NULL pointer checked.*/
        if (state[i] == READY) {
//check a difference with the previous
            if (IsChanged(i) == 1) {
                //rising edge, send the message associated to the code
                for (uint32 j = 0u; j < numberOfEvents; j++) {
                    ReferenceT<EventConditionTrigger> eventCondition = events->Get(j);
                    if (eventCondition.IsValid()) {
                        if (eventCondition->Check(currentValue, &packetConfig[commandIndex[i]])) {
                            cntTrigger[i]++;
                        }
                    }
                }
                if (cntTrigger[i] > 0u) {
                    /*lint -e{613} NULL pointer checked.*/
                    state[i] = SENDING;
                }
                else {
                    //no message to be sent... but ack the value anyway
                    SetAck(i);
                    /*lint -e{613} NULL pointer checked.*/
                    state[i] = DONE;
                }
            }
            /*lint -e{613} NULL pointer checked.*/
            (void) MemoryOperationsHelper::Copy(&previousValue[packetConfig[commandIndex[i]].offset], &currentValue[packetConfig[commandIndex[i]].offset],
                                                (static_cast<uint32>(packetConfig[commandIndex[i]].type.numberOfBits) / 8u));
            //REPORT_ERROR(ErrorManagement::Information, "READY[%d]", i);
        }
        /*lint -e{613} NULL pointer checked.*/
        if (state[i] == SENDING) {
            if (IsChanged(i) != 0) {
                /*lint -e{613} NULL pointer checked.*/
                state[i] = ERROR;
            }
            else {
                for (uint32 j = 0u; j < numberOfEvents; j++) {
                    ReferenceT<EventConditionTrigger> eventCondition = events->Get(j);
                    if (eventCondition.IsValid()) {
                        uint32 nReplies = eventCondition->Replied();
                        /*lint -e{613} NULL pointer checked.*/
                        cntReplied[i] += nReplies;
                    }
                }
                /*lint -e{613} NULL pointer checked.*/
                if (cntReplied[i] == cntTrigger[i]) {
                    /*lint -e{613} NULL pointer checked.*/
                    cntReplied[i] = 0u;
                    /*lint -e{613} NULL pointer checked.*/
                    cntTrigger[i] = 0u;
                    SetAck(i);
                    /*lint -e{613} NULL pointer checked.*/
                    state[i] = DONE;
                }
            }
            (void) MemoryOperationsHelper::Copy(&previousValue[packetConfig[commandIndex[i]].offset], &currentValue[packetConfig[commandIndex[i]].offset],
                                                (static_cast<uint32>(packetConfig[commandIndex[i]].type.numberOfBits) / 8u));
            //REPORT_ERROR(ErrorManagement::Information, "SENDING[%d]", i);
        }
        /*lint -e{613} NULL pointer checked.*/
        if (state[i] == DONE) {
            int32 changed = IsChanged(i);
            if ((changed != 0) && (changed != -1)) {
                state[i] = ERROR;
            }
            else {
                if (changed == -1) {
                    state[i] = READY;
                    SetAck(i);
                }
            }

            /*lint -e{613} NULL pointer checked.*/
            (void) MemoryOperationsHelper::Copy(&previousValue[packetConfig[commandIndex[i]].offset], &currentValue[packetConfig[commandIndex[i]].offset],
                                                (static_cast<uint32>(packetConfig[commandIndex[i]].type.numberOfBits) / 8u));
            REPORT_ERROR(ErrorManagement::Information, "DONE[%d]", i);
        }
    }

    return true;
}

int32 TriggerOnChangeGAM::IsChanged(const uint32 cIdx) const {
    //check if the command i had a rising event
    /*lint -e{613} NULL pointer checked.*/
    uint32 commandOffset = packetConfig[commandIndex[cIdx]].offset;
    /*lint -e{613} NULL pointer checked.*/
    uint32 commandSize = static_cast<uint32>(packetConfig[commandIndex[cIdx]].type.numberOfBits) / 8u;
    /*lint -e{613} NULL pointer checked.*/
    bool ret = (MemoryOperationsHelper::Compare(&currentValue[commandOffset], &previousValue[commandOffset], commandSize) != 0);
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
                if (currentValue[index] != 0u) {
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

void TriggerOnChangeGAM::SetAck(const uint32 cIdx) {

    //copy everything that isnt a command
    for (uint32 i = 0u; i < numberOfFields; i++) {
        /*lint -e{613} NULL pointer checked.*/
        if (!packetConfig[i].isCommand) {
            /*lint -e{613} NULL pointer checked.*/
            uint32 offset = packetConfig[i].offset;
            /*lint -e{613} NULL pointer checked.*/
            uint32 size = static_cast<uint32>(packetConfig[i].type.numberOfBits) / 8u;
            /*lint -e{613} NULL pointer checked.*/
            (void) MemoryOperationsHelper::Copy(&ackValue[offset], &currentValue[offset], size);
        }
    }
    //copy the command
    /*lint -e{613} NULL pointer checked.*/
    uint32 commandOffset = packetConfig[commandIndex[cIdx]].offset;
    /*lint -e{613} NULL pointer checked.*/
    uint32 commandSize = static_cast<uint32>(packetConfig[commandIndex[cIdx]].type.numberOfBits) / 8u;

    /*lint -e{613} NULL pointer checked.*/
    (void) MemoryOperationsHelper::Copy(&ackValue[commandOffset], &currentValue[commandOffset], commandSize);
}

CLASS_REGISTER(TriggerOnChangeGAM, "1.0")

}
