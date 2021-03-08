/**
 * @file HighResolutionTimeProvider.h
 * @brief Header file for class HighResolutionTimeProvider
 * @date 31/10/2019
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

 * @details This header file contains the declaration of the class HighResolutionTimeProvider
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_HIGHRESOLUTIONTIMEPROVIDER_H_
#define SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_HIGHRESOLUTIONTIMEPROVIDER_H_

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

namespace MARTe{
/**
* @brief Default plugin which provides time to the LinuxTimer DataSource.
         Relies on underlying HighResolutionTimer Counter() / Period() and Frequency 
         primitives and implements the sleep as a busy spin based on them.    
*/
    class HighResolutionTimeProvider: public TimeProvider {
        public:
    
            CLASS_REGISTER_DECLARATION()
    
            /**
            * @brief Default constructor
            */
            HighResolutionTimeProvider();

            /**
            * @brief Destructor
            */
            virtual ~HighResolutionTimeProvider();

            /**
            * @brief Returns the value of the internal ticks counter
            * @return The elapsed ticks in the internal counter
            */
            virtual uint64 Counter();

            /**
            * @brief Returns the value of the period for the internal provider
            * @return The provider period expressed in s
            */
            virtual float64 Period();

            /**
            * @brief Returns the value of the frequency for the internal provider
            * @return The provider frequency expressed in Hz
            */
            virtual uint64 Frequency();

            /**
            * @brief Sleeps using a busy wait, starting from an internal counter value, until the count elapses a delta ticks
            * @param[in] start Starting count
            * @param[in] delta Number of ticks to busy sleep
            */
            virtual void BusySleep(const uint64 start, const uint64 delta);
    };
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_HIGHRESOLUTIONTIMEPROVIDER_H_ */

