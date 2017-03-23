/**
 * @file EPICSPVContext.h
 * @brief Header file for class EPICSPVContext
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

 * @details This header file contains the declaration of the class EPICSPVContext
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVCONTEXT_H_
#define EPICSPVCONTEXT_H_

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
class EPICSPVContext: public Object {
public:
    EPICSPVContext();

    virtual ~EPICSPVContext();

    virtual bool Initialise(StructuredDataI & data);

    ErrorManagement::ErrorType SetContext(struct ca_client_context * contextIn, float32 timeoutIn);

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
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVCONTEXT_H_ */

