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

