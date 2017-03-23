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
 *     PVName = "MyPV1" //Compulsory. Name of the PV
 *     PVType = uint32 //Compulsory. Type of the PV (currently int32, float32 and string are supported)
 *     Destination = StateMachine //Compulsory. Destination of the message
 *     ModeValue = ValueIsFunction //Compulsory. What is the meaning of the PV value: ValueIsFunction means that the value will be used as the Message function name. ValueIsParameter means that the value will be set in param1 of the function (which implies that the function must be defined)
 *     Function = FunctionToCall //Function to be called. Compulsory if ModeValue=ValueIsParameter. If ModeValue=ValueIsFunction, this parameter shall not be set.
 *     FunctionMap = {{"0", "START"}, {"1", "STOP"}, ...} //Optional. The PV value will be used to translate the PV value into a Function name.
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
    TypeDescriptor pvType;

    /**
     *
     */
    StreamString pvName;

    /**
     *
     */
    StreamString destination;

    /**
     *
     */
    StreamString functioName;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSEVENTLISTENER_H_ */

