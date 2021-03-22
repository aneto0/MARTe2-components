/**
 * @file TimeProvider.h
 * @brief Header file for class TimeProvider
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

 * @details This header file contains the declaration of the class TimeProvider
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TIMEPROVIDER_H_
#define SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TIMEPROVIDER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
* @brief Interface for TimeProvider plugins on the Linux Timer DataSource
*/
class TimeProvider: public Object {
    public:

        /**
        * @brief Default constructor
        */
        TimeProvider();

        /**
        * @brief Destructor
        */
        virtual ~TimeProvider();

        /**
        * @brief Returns the actual ticks count, as elapsed nanoseconds
        * @return The actual elapsed nanoseconds from the source
        */
        virtual uint64 Counter() = 0;

        /**
        * @brief Returns the actual period expressed in nanoseconds between ticks of the internal source
        * @return The actual period between ticks, expressed in nanoseconds
        */
        virtual float64 Period() = 0;

        /**
        * @brief Returns the actual cpu clock frequency in Hz
        * @return The cpu clock frequency expressed in Hz
        */
        virtual uint64 Frequency() = 0;

        /**
        * @brief Sleeps for a given amount of time, which starts at start and elapses for delta ticks
        * The sleeping strategy is related to the plugin itself. The strategy can be fixed or user-selectable
        * depending on the support offered by the plugin and its backings.
        * @param[in] start Time marker for the sleep start
        * @param[in] delta Amount of ticks to sleep for
        */
        virtual bool Sleep(const uint64 start, const uint64 delta) = 0;

        /**
        * @brief Offers a way to maintain backward compatibility to current functionalities which require settings
        *        to be written in the parent container. As the current approach is to have specific configuration data
        *        inside the plugin instance, this function offers an entry point to bring configuration data to the inner
        *        plugin, without breaking things nor violating new interface and op design.
        * @param[in] compatibilityData Structured data where configuration is ported down to the plugin.
        * @return True if configuration operation succeeds. False otherwise.
        */
        virtual bool BackwardCompatibilityInit(StructuredDataI &compatibilityData) = 0;
    };
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_TIMEPROVIDER_H_ */

