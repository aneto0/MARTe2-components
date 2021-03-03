/**
 * @file HighResolutionTimeProvider.cpp
 * @brief Source file for class HighResolutionTimeProvider
 * @date 31 ott 2019
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

 * @details This source file contains the definition of all the methods for
 * the class HighResolutionTimeProvider (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HighResolutionTimeProvider.h"
#include "HighResolutionTimer.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

HighResolutionTimeProvider::HighResolutionTimeProvider() : TimeProvider() {
}

HighResolutionTimeProvider::~HighResolutionTimeProvider() {
}

uint64 HighResolutionTimeProvider::Counter() {
    return HighResolutionTimer::Counter();
}

float64 HighResolutionTimeProvider::Period() {
    return HighResolutionTimer::Period();
}

uint64 HighResolutionTimeProvider::Frequency() {
    return HighResolutionTimer::Frequency();
}

void HighResolutionTimeProvider::BusySleep(const uint64 start,
                                           const uint64 delta) {

    while ((HighResolutionTimer::Counter() - start) < delta) {
        ;
	}
}

CLASS_REGISTER(HighResolutionTimeProvider, "1.0")

}
