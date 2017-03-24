/**
 * @file EPICSPVBase.h
 * @brief Header file for class EPICSPVBase
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

 * @details This header file contains the declaration of the class EPICSPVBase
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVBASE_H_
#define EPICSPVBASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <cadef.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Object.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static const uint8 EPICS_EVENT_VALUE_IS_FUNCTION = 0u;
static const uint8 EPICS_EVENT_VALUE_IS_PARAMETER = 1u;
static const uint8 EPICS_EVENT_VALUE_IS_IGNORE = 2u;
static const uint8 EPICS_EVENT_VALUE_IS_NOT_SET = 3u;

class EPICSPVBase: public Object {
public:
    EPICSPVBase();

    virtual ~EPICSPVBase();

    virtual bool Initialise(StructuredDataI & data);

    ErrorManagement::ErrorType SetContext(struct ca_client_context * contextIn, float32 timeoutIn);

    /**
     * @brief TODO
     */
    void ValueChanged(StreamString &newValue);

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
    virtual chtype GetPVType() = 0;

    /**
     * @brief TODO. Change to BitField.
     */
    BitField GetMode();

protected:
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
    uint8 modeValue;

    /**
     * TODO
     */
    StreamString lastValue;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVCONTEXT_H_ */

