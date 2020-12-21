/**
 * @file ICyclicNotifiable.h
 * @brief Notification entry point for the cycle execution.
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

 * @details     Provides an interface which is used to push a notification every cycle execution.
 *              It is meant to decouple the Profinet Adapter from the DataSource specifics, in term of
 *              notification for each execution cycle.
 */

#ifndef DATASOURCES_PROFINET_ICYCLICNOTIFIABLE_H_
#define DATASOURCES_PROFINET_ICYCLICNOTIFIABLE_H_

namespace ProfinetDataSourceDriver
{
   
    class ICyclicNotifiable {
        public:
            virtual void NotifyCycle() = 0;
    };
}

#endif /* DATASOURCES_PROFINET_ICYCLICNOTIFIABLE_H_ */