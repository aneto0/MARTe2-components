/**
 * @file MessageGAM.cpp
 * @brief Source file for class MessageGAM
 * @date 26/07/2019
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
 * the class MessageGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "MessageGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MessageGAM::MessageGAM() :
        GAM(),
        MessageI() {
    numberOfFields = 0u;
    signalMetadata = NULL_PTR(SignalMetadata*);
    numberOfCommands = 0u;
    numberOfEvents = 0u;
    cntTrigger = NULL_PTR(uint32*);
    currentValue = NULL_PTR(uint8*);
    previousValue = NULL_PTR(uint8*);
    commandIndex = NULL_PTR(uint32*);
    trigOnChange = true;
    firstTimeAfterStateChange = true;
    firstTime = true;
}

MessageGAM::~MessageGAM() {
    if (signalMetadata != NULL_PTR(SignalMetadata*)) {
        delete[] signalMetadata;
    }
    if (previousValue != NULL_PTR(uint8*)) {
        delete[] previousValue;
    }
    if (commandIndex != NULL_PTR(uint32*)) {
        delete[] commandIndex;
    }
    cntTrigger = NULL_PTR(uint32*);
    currentValue = NULL_PTR(uint8*);
}

bool MessageGAM::Initialise(StructuredDataI &data) {
    bool ret = GAM::Initialise(data);
    if (ret) {
        uint8 tempTrigOnChange;
        if (!data.Read("TriggerOnChange", tempTrigOnChange)) {
            trigOnChange = true;
        }
        else {
            trigOnChange = (tempTrigOnChange != 0u);
        }
    }
    return ret;
}

bool MessageGAM::Setup() {

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
        signalMetadata = new SignalMetadata[numberOfFields];
        ret = (signalMetadata != NULL_PTR(SignalMetadata*));

        uint32 n = 0u;

        /*lint -e{850} the loop variable is not modified within the loop*/
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            StreamString signalName;
            ret = GetSignalName(InputSignals, i, signalName);
            /*lint -e{613} NULL pointer checked.*/
            signalMetadata[n].name = signalName;
            if (ret) {
                TypeDescriptor type = GetSignalType(InputSignals, i);
                /*lint -e{613} NULL pointer checked.*/
                signalMetadata[n].type = type;
                bool isCommand = (StringHelper::CompareN(signalName.Buffer(), "Command", StringHelper::Length("Command")) == 0);
                /*lint -e{613} NULL pointer checked.*/
                signalMetadata[n].isCommand = isCommand;
                /*lint -e{613} NULL pointer checked.*/
                if (signalMetadata[n].isCommand) {
                    numberOfCommands++;
                }

                /*lint -e{613} NULL pointer checked.*/
                uint32 typeSize = (static_cast<uint32>(signalMetadata[i].type.numberOfBits) / 8u);
                /*lint -e{613} NULL pointer checked.*/
                signalMetadata[n].offset = packetSize;
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
                                signalMetadata[n].name = newSignalName;
                                if (j > 0u) {
                                    /*lint -e{613} NULL pointer checked.*/
                                    signalMetadata[n].type = type;
                                    /*lint -e{613} NULL pointer checked.*/
                                    signalMetadata[n].isCommand = isCommand;
                                    /*lint -e{613} NULL pointer checked.*/
                                    if (signalMetadata[n].isCommand) {
                                        numberOfCommands++;
                                    }
                                    /*lint -e{613} NULL pointer checked.*/
                                    signalMetadata[n].offset = packetSize;
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
            REPORT_ERROR(ErrorManagement::InitialisationError, "The number of commands is 0");
        }
    }

    if (ret) {
        previousValue = new uint8[packetSize];
        (void) MemoryOperationsHelper::Set(previousValue, '\0', packetSize);
        commandIndex = new uint32[numberOfCommands];

        uint32 c = 0u;
        for (uint32 i = 0u; i < numberOfFields; i++) {
            /*lint -e{613} NULL pointer checked.*/
            if (signalMetadata[i].isCommand) {
                commandIndex[c] = i;
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
                    ret = eventCondition->SetMetadataConfig(signalMetadata, numberOfFields);
                }
            }
        }

    }

    if (ret) {
//check that the state signal has siz #elements
        uint32 totalNumberOfOutputs = 0u;
        for (uint32 i = 0u; (i < numberOfOutputSignals) && (ret); i++) {
            TypeDescriptor td = GetSignalType(OutputSignals, i);
            ret = (td == UnsignedInteger32Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The type of the output signal (reply state) must be uint32");
            }
            else {
                uint32 numberOfElements;
                ret = GetSignalNumberOfElements(OutputSignals, 0u, numberOfElements);
                totalNumberOfOutputs += numberOfElements;
            }

        }
        if (ret) {
            ret = (totalNumberOfOutputs == numberOfCommands);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The number of outputs %d must match the number of commands %d", totalNumberOfOutputs,
                             numberOfCommands);
            }
        }
    }

    if (ret) {
        currentValue = reinterpret_cast<uint8*>(GetInputSignalsMemory());
        cntTrigger = reinterpret_cast<uint32*>(GetOutputSignalsMemory());
    }

    return ret;
}

/*lint -e{715} PrepareNextState function is independent from current and next state name. State change only triggers a flag */
bool MessageGAM::PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName) {

    firstTimeAfterStateChange = trigOnChange;

    if(firstTime) {
        firstTimeAfterStateChange = false;
        firstTime = false;
    }

    REPORT_ERROR(ErrorManagement::Information, "\nCalled MessageGAM::PrepareNextState\n");
    
    return true;
}

bool MessageGAM::Execute() {

    if (firstTimeAfterStateChange) {
        for (uint32 i = 0u; i < numberOfCommands; i++) {
            /*lint -e{613} NULLity of pointer was afore checked */
            (void) MemoryOperationsHelper::Copy(&previousValue[signalMetadata[commandIndex[i]].offset], &currentValue[signalMetadata[commandIndex[i]].offset],
                                                (static_cast<uint32>(signalMetadata[commandIndex[i]].type.numberOfBits) / 8u));

            /*lint -e{613} NULLity of pointer was afore checked */
            REPORT_ERROR(ErrorManagement::Information, "\nFirst Time %s u--> %u\n", signalMetadata[commandIndex[i]].name.Buffer(), currentValue[signalMetadata[commandIndex[i]].offset],
                         previousValue[signalMetadata[commandIndex[i]].offset]);
        }

        firstTimeAfterStateChange = false;
    }
    else {
        for (uint32 i = 0u; i < numberOfCommands; i++) {

            /*lint -e{613} NULL pointer checked.*/
            if (cntTrigger[i] > 0u) {
                for (uint32 j = 0u; j < numberOfEvents; j++) {
                    ReferenceT<EventConditionTrigger> eventCondition = events->Get(j);
                    if (eventCondition.IsValid()) {
                        uint32 nReplies = eventCondition->Replied(&signalMetadata[commandIndex[i]], cntTrigger[i]);
                        //TODO: Verify if nReplies can become greater than cntTrigger[i] and have it over-decrement
                        /*lint -e{613} NULL pointer checked.*/
                        cntTrigger[i] -= nReplies;
                    }
                }
            }

            /*lint -e{613} NULL pointer checked.*/
            bool check = (cntTrigger[i] == 0u);

            bool trigEvent = false;
            
            if (!trigOnChange) { //If TriggerOnChange is disabled, we only check if no messages are awaiting for reply
                //check means there are no sent messages which are awaiting for a reply
                trigEvent = check;
            }
            else { //Otherwise, even if messages are pending, we add messages if the signal has changed
                if(check) {                
                    trigEvent = IsChanged(i);
                }
            }

            if (trigEvent) {
                //rising edge, send the message associated to the code
                for (uint32 j = 0u; j < numberOfEvents; j++) {
                    ReferenceT<EventConditionTrigger> eventCondition = events->Get(j);
                    if (eventCondition.IsValid()) {
                        /*lint -e{613} NULL pointer checked.*/
                        if (eventCondition->Check(currentValue, &signalMetadata[commandIndex[i]])) {
                            uint32 nMessages = eventCondition->Size();
                            //trigger all the messages of that event
                            /*lint -e{613} NULL pointer checked.*/
                            cntTrigger[i] += nMessages;
                        }
                    }
                }
                /*lint -e{613} NULL pointer checked.*/
                (void) MemoryOperationsHelper::Copy(&previousValue[signalMetadata[commandIndex[i]].offset], &currentValue[signalMetadata[commandIndex[i]].offset],
                                                    (static_cast<uint32>(signalMetadata[commandIndex[i]].type.numberOfBits) / 8u));
            }
        }
    }
    return true;
}

bool MessageGAM::IsChanged(const uint32 cIdx) const {
    //check if the command i had a rising event
    /*lint -e{613} NULL pointer checked.*/
    uint32 commandOffset = signalMetadata[commandIndex[cIdx]].offset;
    /*lint -e{613} NULL pointer checked.*/
    uint32 commandSize = static_cast<uint32>(signalMetadata[commandIndex[cIdx]].type.numberOfBits) / 8u;
    /*lint -e{613} NULL pointer checked.*/
    return (MemoryOperationsHelper::Compare(&currentValue[commandOffset], &previousValue[commandOffset], commandSize) != 0);
}

uint32 MessageGAM::GetNumberOfCommands() const {
    return numberOfCommands;
}

uint32 MessageGAM::GetNumberOfEvents() const {
    return numberOfEvents;

}

void MessageGAM::Purge(ReferenceContainer &purgeList) {
    if (events.IsValid()) {
        events->Purge(purgeList);
    }
    ReferenceContainer::Purge(purgeList);
}

CLASS_REGISTER(MessageGAM, "1.0")
}

