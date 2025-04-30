/**
 * @file ApplicationKiller.h
 * @brief Header file for class ApplicationKiller
 * @date 30/08/2019
 *
 * @copyright Copyright 2019 EPFL SPC Lausanne
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing,
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class ApplicationKillerGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SPCSHOTMANAGER_H_
#define SPCSHOTMANAGER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MessageI.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{
/*
 * @brief Based on message the ApplicationKiller stops MARTe in a controlled manner (if controlled by state machine).
 * @details The ApplicationKiller waits for the message to trigger the execution of the Kill function. When it is received the Kill function sends kill signal
 * to the current MARTe process to terminate the application.
 * The configuration syntax is (names are only given as an example):
 *
 * <pre>
 * +ApplicationKiller = {
 *   Class = ApplicationKiller
 * }
 *
 * +StateMachine = {
 *     Class = StateMachine
 *     +STATE0 = {
 *         ...
 *     }
 *     +STATE1 = {
 *         Class = ReferenceContainer
 *         +TERMINATE = {
 *             Class = StateMachineEvent
 *             NextState = "STATE1"
 *             NextStateError = "ERROR"
 *             Timeout = 0
 *             +StopCurrentStateExecution = {
 *                 Class = Message
 *                 Destination = MARTe
 *                 Function = StopCurrentStateExecution
 *                 Mode = ExpectsReply
 *             }
 *             +KillAppMsg = {
 *                 Class = Message
 *                 Destination = ApplicationKiller
 *                 Function = Kill //(Name mandatory)
 *                 Mode = ExpectsReply
 *             }
 *         }
 *     }
 *     +ERROR = {
 *         ...
 *     }
 * }
 * $MARTe = {
 *     Class = "RealTimeApplication"
 *     +Functions = {
 *         Class = "ReferenceContainer"
 *         ....
 *         +TriggerStopApplication = {
 *             Class = MessageGAM
 *             TriggerOnChange = 0
 *             +Events = {
 *                 Class = ReferenceContainer
 *                 +Event1 = {
 *                     Class = EventConditionTrigger
 *                     EventTrigger = {
 *                         CommandCounter = 1
 *                     }
 *                     +TriggerTerminate = {
 *                         Class = Message
 *                         Destination = StateMachine
 *                         Function = TERMINATE
 *                         Mode = ExpectsReply
 *                      }
 *                 }
 *             }
 *             InputSignals = {
 *                 CommandCounter = {
 *                     DataSource = DDB1
 *                     Type = uint8
 *                 }
 *             }
 *             OutputSignals = {
 *                 PendingMessages1 = {
 *                     DataSource = DDB1
 *                     Type = uint32
 *                 }
 *             }
 *         }
 *         +CreateTriggerMessage = {
 *             Class = MathExpressionGAM
 *             Expression = "CommandCounter = Counter > (uint32)MAX_COUNTER;" //Condition which, at the end, kills the application
 *             InputSignals = {// As many as required.
 *                 Counter = {
 *                     DataSource = DDB1
 *                     Type = uint32
 *                 }
 *             }
 *             OutputSignals = {
 *                 CommandCounter = {
 *                     DataSource = DDB1
 *                     Type = uint8
 *                 }
 *             }
 *         }
 *         ...
 *     }
 *     +Data {
 *         ...
 *     }
 *     +States {
 *         ...
 *     }
 * }
 * </pre>
 */
class ApplicationKiller: public ReferenceContainer, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    ApplicationKiller();
    /**
     * @brief Destructor
     */
    virtual ~ApplicationKiller();

    ErrorManagement::ErrorType Kill();

protected:

private:
    MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter> filter;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SPCSHOTMANAGER_H_ */

