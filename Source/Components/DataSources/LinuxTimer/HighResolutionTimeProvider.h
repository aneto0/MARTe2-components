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
            * @brief MARTe2 object initialisation
            */
            virtual bool Initialise(StructuredDataI &data);

            /**
            * @brief Returns the value of the internal ticks counter
            * @return The elapsed ticks in the internal counter
            */
            virtual uint64 Counter();

            /**
            * @brief Returns the actual period once every ticks occurs
            * @return The actual period between ticks
            */
            virtual float64 Period();

            /**
            * @brief Returns the actual cpu clock frequency, which in turn becomes the tick rate
            * @return The cpu clock frequency
            */
            virtual uint64 Frequency();

            /**
            * @brief Sleeps using a busy wait, starting from an internal counter value, until the count elapses a delta ticks
            * @param[in] start Starting count
            * @param[in] delta Number of ticks to busy sleep
            */
            virtual bool Sleep(const uint64 start, const uint64 delta);

            /**
            * @brief For full motivation of this method, see interface documentation. In this specific case, brings down
            *        to the plugin, the HRT related configuration parameters which still reside in the Datasource, which now is only in charge
            *        of the plugin instantiation.
            * @param[in] compatibilityData Data which is injected from the plugin management DataSource
            * @return True if configuration operation succeeds. False otherwise.
            */
            virtual bool BackwardCompatibilityInit(StructuredDataI &compatibilityData);

        private:
            /**
            * @brief Holds the percentage to sleep by yielding the cpu
            */
            uint8 yieldSleepPercentage;

            /**
            * @brief Pointer to the specific sleep strategy implementation
            */
            bool (HighResolutionTimeProvider::*SleepProvidingFunction)(uint64, uint64);
            
            /**
            * @brief Low level function to provide busy sleep
            */
            bool BusySleep(const uint64 start, const uint64 delta);

            /**
            * @brief Sleep yielding cpu for an amount and busy spinning for the remaining
            */
            bool SemiBusy(const uint64 start, const uint64 delta);

            /**
            * @brief Sleeps no more than the requested time
            */
            bool NoMore(const uint64 start, const uint64 delta);

            /**
            * @brief Null delegate as dummy for initial configuration, to avoid erratic default behaviour.
            * Essentially it does nothing, only fails.
            */
            bool NullDelegate(const uint64 start, const uint64 delta);

            /**
            * @brief Uniforms the configuration strategy for data coming both directly from the cfg or injected through
            *        the backward compatibility method.
            * @param[in] data Configuration data structure.
            * @return True if configuration operation succeeds. False otherwise.
            */
            bool InnerInitialize(StructuredDataI &data);

    };
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_DATASOURCES_LINUXTIMER_HIGHRESOLUTIONTIMEPROVIDER_H_ */

