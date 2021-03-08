/**
 * @file TcnTimeProvider.cpp
 * @brief Source file for class TcnTimeProvider
 * @date 31 ott 2019
 * @author pc
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
 * the class TcnTimeProvider (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <time.h>
#include <tcn.h>
#include <errno.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TCNTimeProvider.h"
#include "AdvancedErrorManagement.h"
#include "StreamString.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

TcnTimeProvider::TcnTimeProvider() {
    // Auto-generated constructor stub for TcnTimeProvider
    // TODO Verify if manual additions are needed
    tcnPoll = 0u;
}

TcnTimeProvider::~TcnTimeProvider() {
    // Auto-generated destructor stub for TcnTimeProvider
    // TODO Verify if manual additions are needed
}

bool TcnTimeProvider::Initialise(StructuredDataI &data) {
    bool ret = Object::Initialise(data);
    if (ret) {
        StreamString tcnDevice;
        ret = (data.Read("TcnDevice", tcnDevice));
        if (ret) {
            (void) tcn_register_device(tcnDevice.Buffer());
            (void) (tcn_init());
        }
        if (!data.Read("TcnPoll", tcnPoll)) {
            tcnPoll = 0u;
        }
    }
    return ret;
}

uint64 TcnTimeProvider::Counter() {
    uint64 tcnTime;
    tcn_get_time((hpn_timestamp_t*) (&tcnTime));
    return tcnTime;
}

float64 TcnTimeProvider::Period() {
    return (1.0 / Frequency());
}

uint64 TcnTimeProvider::Frequency() {
    return 1000000000ull;
}

void TcnTimeProvider::BusySleep(uint64 start,
                                uint64 delta) {

    if (tcnPoll == 0u) {
        uint64 startTicks = static_cast<uint64>((start) * (static_cast<float64>(HighResolutionTimer::Frequency()) / 1e9));
        uint64 deltaTicks = static_cast<uint64>((delta) * (static_cast<float64>(HighResolutionTimer::Frequency()) / 1e9));
        while ((HighResolutionTimer::Counter() - startTicks) < deltaTicks)
            ;
    }
    else {
        uint64 startTicks = start;
        uint64 deltaTicks = delta;
        while ((Counter() - startTicks) < deltaTicks)
            ;
    }

}

CLASS_REGISTER(TcnTimeProvider, "1.0")

}
