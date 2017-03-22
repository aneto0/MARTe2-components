/**
 * @file EPICSEventListener.h
 * @brief Header file for class EPICSEventListener
 * @date 21/03/2017
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class EPICSEventListener
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSEVENTLISTENER_H_
#define EPICSEVENTLISTENER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <mdsobjects.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MessageI.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief TODO
 *
 * +EPICSEventListeners = {
 *   Class = ReferenceContainer
 *   +PV_1 = {
 *     Class = EPICSEventListener
 *     Name = "MyPV1" //Name of the PV
 *     Type = uint32 //Type of the PV (currently uint32, float32 and string are supported)
 *     +Trigger1 = {
 *       Class = EPICSEventTrigger
 *       Value = 2 //Value at which to trigger or just send message??TODO
 *       +PrepareChangeToState2Msg = { //List of messages to trigger if the new value == Value
 *         Class = Message
 *         Destination = DemoApp
 *         Mode = ExpectsReply
 *         Function = PrepareNextState
 *         +Parameters = {
 *           Class = ConfigurationDatabase
 *           param1 = State2
 *         }
 *      }
 *      +StopCurrentStateExecutionMsg = {
 *         Class = Message
 *         Destination = DemoApp
 *         Function = StopCurrentStateExecution
 *         Mode = ExpectsReply
 *       }
 *     }
 *     +Trigger2 = {
 *       Class = EPICSEventTrigger
 *       Value = 3
 *       +StartCurrentStateExecutionMsg = {
 *         Class = Message
 *         Destination = StateMachine
 *         Function = START
 *         Mode = ExpectsReply
 *       }
 *     }
 *     ...
 *   }
 *   ...
 * }
 */
class EPICSEventListener: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief TODO
     */
EPICSEventListener    ();

    /**
     * @brief TODO
     */
    virtual ~EPICSEventListener();

    /**
     * @brief TODO.
     * @return TODO.
     */
    virtual bool Initialise(StructuredDataI & data);

private:
    /**
     *
     */
    ReferenceT<EPICSEventWrapper> mdsEvent;


};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSEVENTLISTENER_H_ */

