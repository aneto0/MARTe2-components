/**
 * @file IProfinetEventNotifiable.h
 * @brief Header file for class IProfinetEventNotifiable
 * @date 15/01/2021
 * @author Giuseppe Avon
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

 * @details This header file contains the declaration of the class (interface) IProfinetEventiNotifiable
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_IPROFINETEVENTNOTIFIABLE_H_
#define DATASOURCES_PROFINET_IPROFINETEVENTNOTIFIABLE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include "ProfinetEventType.h"

namespace MARTe {
    /**
     * @brief   Provides the notification entry point for the adapter, in order
     *          to allow bubbling-up of the events generated inside the Profinet underlying
     *          level.
     */
    class IProfinetEventNotifiable {
        public:
            virtual ~IProfinetEventNotifiable() {}
            /**
             * @brief Notification entry point for the listener (implementer).
             * @param[in] eventType The type of event generated in the underlying level.
             */
            virtual void NotifyEvent(ProfinetDataSourceEventType eventType) = 0;
    };

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_PROFINET_IPROFINETEVENTNOTIFIABLE_H_ */

