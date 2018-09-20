/**
 * @file TriggerOnChangeGAM.h
 * @brief Header file for class TriggerOnChangeGAM
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

 * @details This header file contains the declaration of the class TriggerOnChangeGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TRIGGERONCHANGEGAM_H_
#define TRIGGERONCHANGEGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EventConditionTrigger.h"
#include "GAM.h"
#include "ReferenceContainer.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Triggers MARTe::Message events on the basis of commands received in the input signals.
 *
 * @details This GAM must have the output memory size equal to the input memory size except for the last output memory area that must
 * contain the state of the internal state machine for each command and, therefore, have a size of [NumberOfCommands*sizeof(uint32)].
 *
 * @details This GAM must contain a ReferenceContainer called "Events" containing EventConditionTrigger objects (see EventConditionTrigger).
 * The EventConditionTrigger objects can be configured to send messages based on a combination of specific values of the GAM input signals.
 *
 * @details The input signals can be variables or commands. A command must be declared with a name that begins with the prefix "Command".
 * During the execution of the GAM, the signal values belonging to normal variables will be trivially copied from input to output. The input signal
 * values belonging to commands will be read and checked to eventually trigger specific configured events, and the command output will be
 * written by following a specific protocol (described below).
 *
 * @details This GAM implements the following communication protocol:\n
 *   1) A list of commands and variables is received. The output memory corresponding to the commands is set to zero (STATE == READY-0 ).\n
 *   2) For each command do:\n
 *   3) If the command changes from zero to a value, depending on the combination with the values\n
 *     of other commands and variables, one or more events will be triggered (see EventConditionTrigger) (STATE == SENDING-1)\n
 *   4) Once all the events have been triggered, the command is marked as "acknowledged" and its value is written in the output.\n
 *   5) Wait that the command in input returns equal to zero, then "acknowledge the acknowledged" by writing zero in the output corresponding to that command (STATE == DONE-2).\n
 *   6) Wait again for a command value change that will trigger new events (STATE == READY-0).
 *
 * @details The internal state machine will go in error state (STATE == ERROR-3) if:\n
 *   1) (STATE == SENDING) and the input command becomes zero or changes its value.\n
 *   2) (STATE == DONE) and the input command changes its value to something other than zero.\n
 * If (STATE == ERROR) a falling edge on the input command value (i.e. the input command must go from a non zero-value to a zero value) will bring the state to READY again. \n
 * As a consequence, if the (STATE == ERROR) and the command was already zero, to reset, the command must go to a non-zero value for the duration of at least one cycle.
 *
 * @details Follows an example of configuration:
 * <pre>
 *   +GAM1 = {
 *       Class = TriggerOnChangeGAM
 *       +Events = {
 *           Class = ReferenceContainer
 *           +Event1 = {
 *               Class = EventConditionTrigger
 *               EventTrigger = {
 *                   Command1 = 1
 *                   Signal1 = 2 //Command == 1 and Signal1 == 2 must be true to trigger the condition.
 *               }
 *               +Message1 = {
 *                   Class = Message
 *                   Destination = Application.Data.Input
 *                   Function = \"TrigFun1\"
 *                   Mode = ExpectsReply
 *               }
 *           }
 *           +Event2 = {
 *               Class = EventConditionTrigger
 *               EventTrigger = {
 *                  Command1 = 2
 *               }
 *               +Message2 = {
 *                  Class = Message
 *                  Destination = Application.Data.Input
 *                  Function = \"TrigFun2\"
 *                  Mode = ExpectsReply
 *               }
 *           }
 *           +Event3 = {
 *               Class = EventConditionTrigger
 *               EventTrigger = {
 *                   Command1 = 3
 *               }
 *               +Message3 = {
 *                   Class = Message
 *                   Destination = Application.Data.Input
 *                   Function = \"TrigFun3\"
 *                   Mode = ExpectsReply
 *               }
 *           }
 *       }
 *       InputSignals = {
 *            Command1 = {
 *                DataSource = Input
 *                Frequency = 1
 *                Type = uint32
 *            }
 *            Command2 = {
 *                DataSource = Input
 *                Type = uint32
 *            }
 *            Signal1 = {
 *                DataSource = Input
 *                Type = uint32
 *            }
 *       }
 *       OutputSignals = {
 *           CommandsAck = {
 *               DataSource = DDB1
 *               Type = uint32
 *               NumberOfDimensions = 1
 *               NumberOfElements = 2 //2 input commands
 *               Trigger = 1
 *           }
 *           Signal1 = {
 *                DataSource = DDB1
 *                Type = uint32
 *           }
 *           State = {
 *               DataSource = DDB1
 *               Type = uint32
 *               NumberOfDimensions = 1
 *               NumberOfElements = 2 //2 commands
 *               Trigger = 1
 *            }
 *       }
 *   }
 * </pre>
 */
class TriggerOnChangeGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    TriggerOnChangeGAM();

    /**
     * @brief Destructor
     */
    virtual ~TriggerOnChangeGAM();

    /**
     * @brief Initialises the GAM environment.
     * @details Separates the input signals in commands and variables and checks that OutputSize=InputSize+(NumberOfCommands*sizeof(uint32)).
     * The commands are recognised among the signal names that begins with the prefix "Command".
     */
    virtual bool Setup();

    /**
     * @brief Executes an internal state machine that allows to trigger events by checking a rising edge of the commands (i.e. from zero to a non-zero value) and send acknowledgments
     * of the commands themselves.
     * @details For each command the following state machine is implemented.\n
     *   - Start in state=READY(0). The command value in input must be zero during this state. If a rising edge is detected, the state machine
     *     goes in SENDING(1) state.\n
     *   - When the state machine is in state=SENDING(1), the contained EventTriggerCondition objects are queried to check if one or more events
     *     should be executed on the base of the combination of commands and variables defined in the configuration. Once all the events receive their
     *     replies, the value of the command is copied in output (that until now was kept to zero) acknowledging that the events have been executed.\n
     *     Then, the state machine can go in DONE(2) state. If the command change its values during this state, the state machine goes in ERROR(3) state.\n
     *   - When the state machine is in state=DONE(2), it waits that the command in input is reset to zero (falling edge) and set again the acknowledgment.
     *     of the command to zero. Then the state machine can return to READY(0) state, to start again a new cycle. If the command change its values to
     *     something different than zero during this state, the state machine goes in ERROR(3) state.\n
     *   - When the state machine is in state=ERROR(3), the detection of a falling edge of the command (namely a change from a value to zero) will
     *     cause the sate machine returning in READY(0) state.
     * @return true.
     */
    virtual bool Execute();

protected:


    /**
     * @brief Checks if the command with index \a cIdx has changed.
     * @param[in] cIdx is the command index.
     * @return -2 if the command has changed from (value1!=0) to (value2!=0), 0 if the command has not changed, -1 if the command
     * has changed from (value1!=0) to (value2==0), 1 if the command has changed from (value1==0) to (value2!=0).
     */
    int32 IsChanged(const uint32 cIdx) const;

    /**
     * @brief Sets the output memory relative to the input command (acknowledgment) equal to the input command value received.
     * @param[in] cIdx is the command index.
     */
    void SetAck(const uint32 cIdx);


    /**
     * READY state
     */
    static const uint32 READY=0u;

    /**
     * SENDING state
     */
    static const uint32 SENDING=1u;

    /**
     * DONE state
     */
    static const uint32 DONE=2u;

    /**
     * ERROR state
     */
    static const uint32 ERROR=3u;

    /**
     * Holds the last input memory value.
     */
    uint8 *previousValue;

    /**
     * Holds the state of the internal state machine
     * for each command
     */
    uint32 *state;

    /**
     * Used to store the property of the input signals,
     * namely the name, the offset, the type and if it
     * is a command or not.
     */
    PacketField *packetConfig;

    /**
     * The number of fields that is equal to
     * the number of input signals if all input signals
     * have NumberOfElements==1, otherwise is the sum
     * of the NumberOfElements of each input signals.
     */
    uint32 numberOfFields;

    /**
     * The container of the EventConditionTriggerObjects
     */
    ReferenceT<ReferenceContainer> events;

    /**
     * The number of events (equal to the size of \a events)
     */
    uint32 numberOfEvents;

    /**
     * Holds the command indices within the \a packetConfig array
     */
    uint32 *commandIndex;

    /**
     * The number of commands
     */
    uint32 numberOfCommands;

    /**
     * Counts the number of replied events for each command.
     */
    uint32 *cntReplied;

    /**
     * Counts the number of events triggered for each command.
     */
    uint32 *cntTrigger;

    /**
     * Holds the current input memory.
     */
    uint8 *currentValue;

    /**
     * Holds the current output memory.
     */
    uint8 *ackValue;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_GAMS_TRIGGERONCHANGEGAM_TRIGGERONCHANGEGAM_H_ */

