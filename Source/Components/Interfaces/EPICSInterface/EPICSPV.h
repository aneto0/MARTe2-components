/**
 * @file EPICSPV.h
 * @brief Header file for class EPICSPV
 * @date 23/03/2017
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class EPICSPV
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPV_H_
#define EPICSPV_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <cadef.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MessageI.h"
#include "Object.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Describes an EPICS PV.
 * @details This class wraps an EPICS PV, allowing to caput and caget values from it.
 * This class is capable of triggering Messages as a response to a PV value change. The new PV value can
 * either be: 1) the name of the Function to be called; 2) or an ID of a Function to be called (see FunctionMap);
 *  3) or the input parameter of a pre-defined Function to be called; or 4) the value is to be ignored and a pre-defined Function is to be called with no parameters.
 * The configuration syntax is (names are only given as an example):
 * +PV_1 = {
 *   Class = EPICSInterface::EPICSPV
 *   PVName = PV_ONE //Compulsory. Name of the EPICS PV.
 *   PVType = uint32 //Compulsory. The PV type. Supported types are uint32, int32, float32, float64 and string.
 *   Event = { //Optional. Information about the message to be triggered every-time the EPICS PV value.
 *     Destination = StateMachine //Compulsory. Destination of the message.
 *     PVValue = Function //Compulsory. Can either be Function, Parameter or Ignore.
 *                        //If Function the PV value will be used as the name of the Function to call.
 *                        //If Parameter the PV value will be used as the parameter of the Function to call. This implies that the Function parameter must be set.
 *                        //If Ignore, the PV value will not be used and the Function will always be called.
 *     Function = STOP //Compulsory if FunctionMap is not defined. Shall not be set if FunctionMap is defined.
 *     FunctionMap = {{"1", "RUN"}, {"0", "STOP"}} //Optional if PVValue == Function. If defined then the PV value (first column of the matrix) will be used to map the Function name (second column of the matrix).
 *   }
 * }
 *
 * The CAPut and CAGet class methods are registered as call-backs. The parameter to put/get shall be encoded as param1 in a StructuredDataI attached to the message.
 */
class EPICSPV: public Object, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()
    EPICSPV();

    virtual ~EPICSPV();

    virtual bool Initialise(StructuredDataI & data);

    void SetContext(struct ca_client_context * contextIn);

    /**
     * @brief TODO
     */
    void HandlePVEvent(const void *dbr);

    /**
     * @brief TODO
     */
    void GetPVName(StreamString &name);

    /**
     * @brief TODO
     */
    chid GetPVChid();

    /**
     * @brief TODO
     */
    void SetPVChid(chid pvChidIn);

    /**
     * @brief TODO
     */
    chtype GetPVType();

    /*lint ++flb*/
    union EventMode {
        /**
         * Mode is not set
         */
        BitBoolean<uint8, 0u> notSet;

        /**
         * Value is to be used as the function name
         */
        BitBoolean<uint8, 1u> function;

        /**
         * Value is to be used as the function parameter
         */
        BitBoolean<uint8, 2u> parameter;

        /**
         * Value is to be ignored
         */
        BitBoolean<uint8, 3u> ignore;

        /**
         * Unmapped area
         */
        BitRange<uint8, 4u, 4u> unMapped;

        /**
         * Output as uint16
         */
        uint8 asUint8;
    };
    /*lint --flb*/

    /**
     * @brief TODO. Change to BitField.
     */
    EventMode GetMode();

    /**
     * TODO
     */
    ErrorManagement::ErrorType CAPut(StructuredDataI &data);

    /**
     * TODO
     */
    ErrorManagement::ErrorType CAGet(StructuredDataI &data);

private:
    /**
     *
     */
    void TriggerEventMessage(StreamString &newValue);

    /**
     *
     */
    struct ca_client_context * context;

    /**
     *
     */
    float32 timeout;

    /**
     * TODO
     */
    StreamString pvName;

    /**
     * TODO
     */
    chid pvChid;

    /**
     * TODO
     */
    chtype pvType;

    /**
     * TODO
     */
    TypeDescriptor pvTypeDesc;

    /**
     * TODO
     */
    StreamString destination;

    /**
     * TODO
     */
    StreamString function;

    /**
     * TODO
     */
    StreamString *functionMap[2];

    /**
     * TODO
     */
    uint32 nOfFunctionMaps;

    /**
     * TODO
     */
    EventMode eventMode;

    /**
     * TODO
     */
    StreamString pvMemoryStr;

    /**
     *
     */
    void *pvMemory;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVCONTEXT_H_ */

