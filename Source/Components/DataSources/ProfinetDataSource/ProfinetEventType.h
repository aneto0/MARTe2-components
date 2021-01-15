/**
 * @file ProfinetEventType.h
 * @brief   Bit flag enumeration to tell apart events.
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

 * @details This header file contains the declaration of the class ProfinetEventType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */


#ifndef DATASOURCES_PROFINET_PROFINETEVENTTYPE_H_
#define DATASOURCES_PROFINET_PROFINETEVENTTYPE_H_

#ifndef BIT
    #define BIT(n) (1U << (n))
#endif

namespace MARTe {
    /**
    * @details     Enumeration, structured as bit flag, to model occurring events in the underlying
    *              Profinet library. 
    */
    typedef enum profinetdatasource_eventtype {
        ProfinetEventReadyForData    = BIT (0),
        ProfinetEventTimer           = BIT (1),
        ProfinetEventAlarm           = BIT (2),
        ProfinetEventAbort           = BIT (15)
    }ProfinetDataSourceEventType;

}

#endif /* DATASOURCES_PROFINET_PROFINETEVENTTYPE_H_ */