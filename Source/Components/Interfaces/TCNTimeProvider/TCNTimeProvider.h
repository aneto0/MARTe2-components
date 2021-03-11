/**
 * @file TcnTimeProvider.h
 * @brief Header file for class TcnTimeProvider
 * @date 31 ott 2019
 * @author pc
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

 * @details This header file contains the declaration of the class TcnTimeProvider
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TCNTIMEPROVIDER_H_
#define 		SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TCNTIMEPROVIDER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TimeProvider.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

const uint8 TCNTIMEPROVIDER_DEFAULT_POLL = 0u;
const uint64 TCNTIMEPROVIDER_DEFAULT_FREQUENCY = 1000000000ull;
const uint32 TCNTIMEPROVIDER_DEFAULT_TOLERANCE = 1000u;

typedef enum {
    TCNTimeProvider_NoPollLegacyMode,
    TCNTimeProvider_PollLegacyMode,
    TCNTimeProvider_WaitUntilMode,
    TCNTimeProvider_WaitUntilHRMode
}TCNTimeProviderOperationMode;

class TCNTimeProvider: public TimeProvider {
public:
    CLASS_REGISTER_DECLARATION()

    TCNTimeProvider();

    virtual ~TcnTimeProvider();

    virtual bool Initialise(StructuredDataI &data);

    virtual uint64 Counter();

    virtual float64 Period();

    virtual uint64 Frequency();

    virtual void BusySleep(uint64 start,
                           uint64 delta);

private:
    TCNTimeProviderOperationMode operationMode;
    uint64 tcnFrequency;
    uint32 tolerance;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TCNTIMEPROVIDER_H_ */

