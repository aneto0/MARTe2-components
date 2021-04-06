/**
 * @file MessageGAM.h
 * @brief Header file for class MessageGAM
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

 * @details This header file contains the declaration of the class MessageGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_GAMS_MESSAGEGAM_MESSAGEGAM_H_
#define SOURCE_COMPONENTS_GAMS_MESSAGEGAM_MESSAGEGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EventConditionTrigger.h"
#include "GAM.h"
#include "MessageI.h"
#include "StatefulI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief Triggers MARTe::Message events on the basis of commands received in the input signals.
 *
 * @details This GAM must contain a ReferenceContainer called "Events" containing EventConditionTrigger objects (see EventConditionTrigger).
 * The EventConditionTrigger objects can be configured to send messages based on a combination of specific values of the GAM input signals.
 *
 * @details The input signals can be variables or commands. A command must be declared with a name that begins with the prefix "Command".
 * When a command changes, the MessageGAM interrogates all the contained EventConditionTrigger and messages are sent if the specified conditions are
 * matched. In output this GAM provides the number of the messages waiting for reply for each command.\n
 * The GAM has two main behaviour modes, which can be controlled using the TriggerOnChange option:
 * - TriggerOnChange enabled: the GAM must see a change in its command value input during its state lifecycle. Seeing a change during a state lifecycle means
 *                            that every state change (e.g. PrepareNextState) the GAM stores the current value to wait for a change happening in the future, by comparing
 *                            past value with current, during that specific state context. In other words, the GAM has to see and "edge" in the command value to trigger a message.
 * - TriggerOnChange disabled: the GAM does not need to see an edge in command value to trigger the message, even across state changes.
 * As the GAM keeps track of sent messages and received replies, if the message sent as a consequence of a triggering event is still awaiting for a reply, no further message will
 * be sent until the reply acknowledgement.
 * Constraints:\n
 *   [number of commands] == [number of output signals]
 *   [output signals type] == uint32
 *
 * @details Follows an example of configuration:
 * <pre>
 *   +GAM1 = {
 *       Class = TriggerOnChangeGAM
 *       TriggerOnChange = 1 //Defaults to 1 = true if not present. Allowed values 0 == false or disabled, != 0 == true or enabled (see above)
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
 *           PendingMessages1 = {
 *                DataSource = DDB1
 *                Type = uint32
 *           }
 *           PendingMessages2 = {
 *               DataSource = DDB1
 *               Type = uint32
 *            }
 *       }
 *   }
 * </pre>
 */
class MessageGAM: public GAM, public MessageI, public StatefulI {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    MessageGAM();

    /**
     * @brief Destructor
     */
    virtual ~MessageGAM();

    /**
    * @brief Standard Initialise function
    */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Setup the GAM.
     * @details Checks the constraints and prepares the internal variables for the execution.
     */
    virtual bool Setup();


    /**
    * @brief Informs the GAM that a state change is going on. Current and next names are ignored, the GAM uses the state signaling to
    *        implement a specific (configurable) behaviour on state changes
    */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief When a command signal changes, calls the EventConditionTriggers to send the specified messages if the value
     * of the signals matches with the defined ones.
     */
    virtual bool Execute();

    /**
     * @brief Retrieves the number of commands.
     * @return the number of commands.
     */
    uint32 GetNumberOfCommands() const ;

    /**
     * @brief Retrieves the number of events.
     * @return the number of contained EventConditionTrigger objects.
     */
    uint32 GetNumberOfEvents() const ;

    /**
     * @see ReferenceContainer::Purge
     */
    virtual void Purge(ReferenceContainer &purgeList);

protected:

    /**
     * @brief Checks if a command signal has changed with respect the previous cycle.
     * @param[in] cIdx the command signal index.
     * @return -1 if the commands changed to zero, 1 if the command changed from zero, -2 if the command changed, 0 if the command did not change.
     */
    virtual bool IsChanged(const uint32 cIdx) const;

    /**
     * The total number of variables
     */
    uint32 numberOfFields;

    /**
     * A vector to store the variables meta-data.
     */
    SignalMetadata *signalMetadata;

    /**
     * Accelerator to the EventConditionTrigger container
     */
    ReferenceT<ReferenceContainer> events;

    /**
     * The number of commands
     */
    uint32 numberOfCommands;

    /**
     * The number of EventConditionTrigger objects
     */
    uint32 numberOfEvents;

    /**
     * The number of pending messages
     */
    uint32 *cntTrigger;

    /**
     * The current signal value
     */
    uint8 *currentValue;

    /**
     * Stores the index of the commands in the input buffer.
     */
    uint32 *commandIndex;

    /**
     * The previous signal value
     */
    uint8 *previousValue;

    /**
    * Holds internal value for the trigger on change configuration setting
    */
    bool trigOnChange;

    /**
    * True only on first Execute pass after a state change, used to handle the trigger on change setting.
    */
    bool firstTimeAfterStateChange;

    /**
    * @brief True only on the very Execute pass. Used to avoid message on first cycle.
    */
    bool firstTime;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_GAMS_MESSAGEGAM_MESSAGEGAM_H_ */

