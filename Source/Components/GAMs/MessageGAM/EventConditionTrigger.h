/**
 * @file EventConditionTrigger.h
 * @brief Header file for class EventConditionTrigger
 * @date 06/07/2018
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

 * @details This header file contains the declaration of the class EventConditionTrigger
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EVENTCONDITION_H_
#define EVENTCONDITION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"
#include "FastPollingMutexSem.h"
#include "MessageI.h"
#include "ReferenceContainer.h"
#include "SingleThreadService.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief An accelerator structure which is used to hold handy information about a signal
 */
struct SignalMetadata {

    /**
     * @brief The MARTe2 name of the field
     */
    StreamString name;

    /**
     * @brief The MARTe2 type of the field
     */
    TypeDescriptor type;

    /**
     * @brief The offset of the field, starting from the context GAM memory
     */
    uint32 offset;

    /**
     * @brief Tells if the field is a command
     */
    bool isCommand;
};


/**
 * @brief Checks if the memory in input to the Check() function contains a combination of values specified by
 * the user in the configuration. In case of matching, it sends one or more messages (also specified in the configuration).
 *
 * @details The configuration of this object must contain a block called "EventTrigger" that contains the combination of variables and command
 * values that will trigger the event (an AND of all inputs for any given EventTrigger is performed).
 * Moreover this object is a container of Message objects that will be sent if the memory in input to the Check() function matches the values specified in the "EventTrigger" block.
 *
 * @details If the event is triggered, the Check() function will insert the Messages to be sent in a queue that will be consumed by a separated thread.
 * The function Replied() returns the number of replied messages because the reply can be immediate or not (if the Message is declared with IsIndirectReply=true).
 *
 * @details Follows a configuration example:
 * <pre>
 *       +Events = {
 *           CPUMask = 0x1 //Default = 0xff. The affinity of thread that is going to send the messages.
 *           Class = ReferenceContainer
 *           +Event1 = {
 *               Class = EventConditionTrigger
 *               EventTrigger = {
 *                   Command1 = 1
 *                   Signal1 = 2 //Both conditions must be true
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
 *                   Command1 = 2
 *               }
 *               +Message2 = {
 *                   Class = Message
 *                   Destination = Application.Data.Input
 *                   Function = \"TrigFun2\"
 *                   Mode = ExpectsReply
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
 *            }
 *        }
 * </pre>
 */
class EventConditionTrigger: public ReferenceContainer, public EmbeddedServiceMethodBinderI, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    EventConditionTrigger();

    /**
     * @brief Destructor
     */
    ~EventConditionTrigger();

    /**
     * @see ReferenceContainer::Initialise()
     * @details Checks if the block "EventTrigger" is present and stores a link to
     * \a data.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Stores a pointer to the list of SignalMetadata elements.
     * @details This function has to be called by the component that use this trigger
     * generator (i.e TriggerOnChangeGAM) to allow this object to know the metadata
     * associated to the variables that have to be checked.
     * @return true if all the signals required by the EventTrigger exist and have the expected types.
     */
    bool SetMetadataConfig(const SignalMetadata * const metadataConf,
                         const uint32 numberOfFields);

    /**
     * @brief Checks if the variable values declared in the "EventTrigger" block configuration
     * match within the \a memory area in input.
     * @details If the variables match, the Message objects contained will be added to a queue that
     * will be consumed within the Execute() function.
     * @param[in] memoryArea is the memory area to be checked.
     * @param[in] metadataIn is the metadata associated to the command that trigger this event.
     * It must be one of the variables declared in "EventTrigger" block.
     * @return true if the variables match within the \a memory area.
     */
    bool Check(const uint8 * const memoryArea,
               const SignalMetadata * const metadataIn);

    /**
     * @brief Returns the number of replies and reset the counter.
     * @return the number of replies to the sent messages.
     */
    uint32 Replied(const SignalMetadata * const metadataIn, const uint32 maxReplies);

    /**
     * @brief Consumes the queue of the messages to be sent. If the Message is declared with
     * IsIndirectReply=true, then it instantiate a filter to receive the asynchronous replies.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    /**
     * @see ReferenceContainer::Purge()
     * @details Stops the internal thread execution.
     */
    virtual void Purge(ReferenceContainer &purgeList);

    /**
     * @brief Gets the affinity of the thread which is going to be used to asynchronously send the messages.
     * @return the affinity of the thread which is going to be used to asynchronously send the messages.
     */
    const ProcessorType& GetCPUMask() const;

    /**
     * @brief Holds one of the variables values declared within the "EventTrigger"
     * block of the EventConditionTrigger object configuration.
     */
    class EventConditionField {
    public:

        /**
         * @brief Constructor
         */
        EventConditionField();

        /**
         * @brief Destructor
         */
        ~EventConditionField();

        /**
         * The signal metadata associated to this variable.
         */
        const SignalMetadata *signalMetadata;

        /**
         * The type and value of this variable.
         */
        AnyType at;
    };
protected:

    /**
     * Holds a link to the configuration database
     */
    ConfigurationDatabase signalsDatabase;

    /**
     * The list of variables declared in the "EventTrigger" block
     */
    EventConditionField *eventConditions;

    /**
     * The number of variables declared in the "EventTrigger" block
     */
    uint32 numberOfConditions;

    /**
     * A queue that contains the messages to be sent.
     */
    ReferenceContainer messageQueue;

    /**
     * A spinlock mutex semaphore to synchronise with
     * the internal thread that sends the messages.
     */
    FastPollingMutexSem spinLock;

    /**
     * The number of replied messages.
     */
    uint32 replied;

    /**
     * The internal thread executor.
     */
    SingleThreadService executor;

    /**
     * The affinity of the SingleThreadService.
     */
    ProcessorType cpuMask;

    /**
     * Event semaphore to wait for messages to be available.
     */
    EventSem eventSem;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EVENTCONDITION_H_ */

