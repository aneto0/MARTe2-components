/**
 * @file ITimerEntryPoint.h
 * @brief Interface to the Timer Service EntryPoint.
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

 * @details     Provides the entry point for the timer service, which will
 *              call in a loop the TimerTick method at a specific frequency
 *              imposed by the Profinet communication configuration.
 */


#ifndef DATASOURCES_PROFINET_ITIMERENTRYPOINT_H_
#define DATASOURCES_PROFINET_ITIMERENTRYPOINT_H_

namespace MARTe {

    class ITimerEntryPoint {
        public:
            virtual void TimerTick() = 0;
    };

}


#endif /* DATASOURCES_PROFINET_ITIMERENTRYPOINT_H_ */