/**
 * @file ProfinetTimerHelper.h
 * @brief TimerHelper Service for Profinet cyclic handling.
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

 * @details     Service to call at consistent intervals the timer feature, which is needed
 *              to trigger the "Timer" event, which is then caught by the MainThread loop.
 *              Profinet underlying layers will see the rise of the event and deal with the
 *              synchronisation logic (aka Cyclic data).
 */

#ifndef DATASOURCES_PROFINET_PROFINETTIMERHELPER_H_
#define DATASOURCES_PROFINET_PROFINETTIMERHELPER_H_

#include "AdvancedErrorManagement.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "ITimerEntryPoint.h"
#include "SingleThreadService.h"

namespace MARTe {

    class ProfinetTimerHelper : public Object,
                                public EmbeddedServiceMethodBinderT<ProfinetTimerHelper> {

        private:
            SingleThreadService service;
            ITimerEntryPoint *entryPoint;
            ErrorManagement::ErrorType ThreadCallback(ExecutionInfo &info);
            float64 timerInterval;
            float64 lastInterval;

        public:
            CLASS_REGISTER_DECLARATION()
            ProfinetTimerHelper();
            virtual ~ProfinetTimerHelper();
            virtual bool Initialise(StructuredDataI &data);
            bool SetEntryPoint(ITimerEntryPoint *entryPoint);
            void SetTimerInterval(float64 intervalInSeconds);
            void Start();
            void Stop();
    };
    
}

#endif /* DATASOURCES_PROFINET_PROFINETTIMERHELPER_H_ */ 