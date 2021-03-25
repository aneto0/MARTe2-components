/**
 * @file ProfinetTimerHelper.h
 * @brief Header file for class ProfinetTimerHelper
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

 * @details This header file contains the declaration of the class ProfinetTimerHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_PROFINETTIMERHELPER_H_
#define DATASOURCES_PROFINET_PROFINETTIMERHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "ITimerEntryPoint.h"
#include "SingleThreadService.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
    /**
     * @brief   Service to call at consistent intervals the timer feature, which is needed
     *          to trigger the "Timer" event, which is then caught by the MainThread loop.
     *          Profinet underlying layers will see the rise of the event and deal with the
     *          synchronisation logic (aka Cyclic data).
     */
    class ProfinetTimerHelper : public Object,
                                public EmbeddedServiceMethodBinderT<ProfinetTimerHelper> {

        private:
            /**
             * @brief The service instance
             */
            SingleThreadService service;

            /**
             * @brief Pointer to the timer entry-point implementer class
             */
            ITimerEntryPoint *entryPoint;

            /**
             * @brief Thread callback which is cyclically executed by the service
             */
            ErrorManagement::ErrorType ThreadCallback(ExecutionInfo &info);
            
            /**
             * @brief Holds the timer interval
             */
            float64 timerInterval;

            /**
             * @brief Holds the last elapsed execution time, in order to be subtracted from the interval and
             * implement a cycle-by-cycle interval duration correction.
             */
            float64 lastInterval;

        public:
            CLASS_REGISTER_DECLARATION()

            /**
             * @brief Constructs an instance of the ProfinetMainThreadHelper
             */
            ProfinetTimerHelper();

            /**
             * @brief Destructor for the instance
             */
            virtual ~ProfinetTimerHelper();

            /**
             * @brief Initializes the instance
             */
            virtual bool Initialise(StructuredDataI &data);

            /**
             * @brief Sets the entry point for the service to find the timer function that has to be cyclically called.
             */
            bool SetEntryPoint(ITimerEntryPoint *entryPointParam);

            /**
             * @brief Sets the service Periodic interval
             * @param[in] intervalInSeconds The periodic interval expressed in seconds
             */
            void SetTimerInterval(const float64 intervalInSeconds);
            
            /**
             * @brief Starts the service
             */
            void Start();

            /**
             * @brief Stops the service
             */
            void Stop();
    };
    
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_PROFINET_PROFINETTIMERHELPER_H_ */

