/**
 * @file HighResolutionTimestampProvider.cpp
 * @brief Source file for class HighResolutionTimestampProvider
 * @date 10/09/2021
 * @author Luca Porzio
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

 * @details This source file contains the definition of all the methods for
 * the class HighResolutionTimestampProvider (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "HighResolutionTimestampProvider.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

HighResolutionTimestampProvider::HighResolutionTimestampProvider() :
        TimestampProvider() {

}

HighResolutionTimestampProvider::~HighResolutionTimestampProvider() {

}

uint64 HighResolutionTimestampProvider::Timestamp() {
    bool ok = HighResolutionTimer::GetTimeStamp(ts);
    uint64 timestamp = 0u;
    if (ok) {
        timestamp =  (static_cast<uint64>(ts.GetSeconds()) * 1000000000u) + (static_cast<uint64>(ts.GetMicroseconds()) * 1000u);
    }
    return timestamp;
}

CLASS_REGISTER(HighResolutionTimestampProvider, "1.0")
}
