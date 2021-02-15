/**
 * @file ProfinetMainThreadHelper.h
 * @brief Header file for class ProfinetMainThreadHelper
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

 * @details This header file contains the declaration of the class ProfinetMainThreadHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_PROFINETMAINTHREADHELPER_H_
#define DATASOURCES_PROFINET_PROFINETMAINTHREADHELPER_H_

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "EventSem.h"
#include "FastPollingMutexSem.h"
#include "IMainThreadEntryPoint.h"
//lint ++flb "Internals"
#include "ProfinetDataSourceTypes.h"
#include "ProfinetEventType.h"
//lint --flb
#include "SingleThreadService.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    /**
     * @brief Service to call at consistent intervals the MainThread entry point function
     * on the Profinet adapter.
     */
    class ProfinetMainThreadHelper :
        public Object,
        public EmbeddedServiceMethodBinderT<ProfinetMainThreadHelper> {
                    
        private:
            /**
             * @brief The service itself
             */
            SingleThreadService service;

            /**
             * @brief Pointer to the main-thread entry-point implementer class
             */
            IMainThreadEntryPoint *entryPoint;

            /**
             * @brief Thread callback which is cyclically executed by the service
             */
            ErrorManagement::ErrorType ThreadCallback(ExecutionInfo &info);

            /**
             * @brief Service timeout type
             */
            TimeoutType timeout;

            /**
             * @brief Service timeout expressed in seconds
             */
            float32 sleepTimeSeconds;

            /**
             * @brief Semaphore used to lock on event flag update
             */
            EventSem eventSemaphore;

            /**
             * @brief Mutex to make modifications to the event flag under protected region
            */
            FastPollingMutexSem flagMutex;

            /**
             * @brief The event flag
             */
            uint16 eventFlag;

        public:
            CLASS_REGISTER_DECLARATION()

            /**
             * @brief Constructs an instance of the ProfinetMainThreadHelper
             */
            ProfinetMainThreadHelper();

            /**
             * @brief Destructor for the instance
             */
            virtual ~ProfinetMainThreadHelper();

            /**
             * @brief Initializes the instance
             */
            virtual bool Initialise(StructuredDataI &data);

            /**
             * @brief Sets the entry point for the service to find the main thread function that has to be cyclically called.
             */
            bool SetEntryPoint(IMainThreadEntryPoint *entryPointParam);

            /**
             * @brief Starts the service
             */
            void Start();

            /**
             * @brief Stops the service
             */
            void Stop();

            /**
             * @brief Notifies the service an event happened on the Profinet stack side
             */
            void NotifyEvent(const ProfinetDataSourceEventType eventType);

            /**
             * @brief Sets the service Periodic interval
             * @param[in] seconds The periodic interval expressed in seconds
             */
            void SetPeriodicInterval(const float32 seconds);

            /**
             * @brief Sets the service Timeout
             * @param[in] seconds The service timeout expreseed in seconds
             */
            void SetTimeout(const float32 seconds);
    };

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_PROFINET_PROFINETMAINTHREADHELPER_H_ */

