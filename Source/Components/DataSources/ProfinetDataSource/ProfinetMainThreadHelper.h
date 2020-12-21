/**
 * @file    ProfinetMainThreadHelper.h
 * @brief   Main Thread helper to call Profinet library entry point.
 * @date 
 * @author
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

 * @details     Service to call at consistent intervals the MainThread entry point function
 *              on the Profinet adapter.
 */

#ifndef DATASOURCES_PROFINET_PROFINETMAINTHREADHELPER_H_
#define DATASOURCES_PROFINET_PROFINETMAINTHREADHELPER_H_

#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "EventSem.h"
#include "FastPollingMutexSem.h"
#include "IMainThreadEntryPoint.h"
#include "pnetdstypes.h"
#include "ProfinetEventType.h"
#include "SingleThreadService.h"

namespace MARTe {

    class ProfinetMainThreadHelper :
        public Object,
        public EmbeddedServiceMethodBinderT<ProfinetMainThreadHelper> {
                    
        private:
            SingleThreadService service;
            IMainThreadEntryPoint *entryPoint;
            ErrorManagement::ErrorType ThreadCallback(ExecutionInfo &info);
            TimeoutType timeout;
            float32 sleepTimeSeconds;
            EventSem eventSemaphore;
            FastPollingMutexSem flagMutex;
            uint16 eventFlag;

        public:
            CLASS_REGISTER_DECLARATION()
            ProfinetMainThreadHelper();
            virtual ~ProfinetMainThreadHelper();
            virtual bool Initialise(StructuredDataI &data);
            bool SetEntryPoint(IMainThreadEntryPoint *entryPoint);
            void Start();
            void Stop();
            void NotifyEvent(ProfinetDataSourceEventType eventType);
            void SetPeriodicInterval(float64 seconds);
            void SetTimeout(float64 seconds);
    };

}

#endif /* DATASOURCES_PROFINET_PROFINETMAINTHREADHELPER_H_ */