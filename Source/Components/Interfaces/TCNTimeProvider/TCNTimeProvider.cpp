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
#include <tcn.h>
#include <errno.h>
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TCNTimeProvider.h"
#include "AdvancedErrorManagement.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

TCNTimeProvider::TCNTimeProvider() {
    tcnFrequency = 0u;
}

TCNTimeProvider::~TCNTimeProvider() {
    REPORT_ERROR(ErrorManagement::Information, "TCNTimeProvider disposed");
}

bool TCNTimeProvider::Initialise(StructuredDataI &data) {
    bool ret = Object::Initialise(data);
    
    if (ret) {
        StreamString tcnDevice;
        int tcnRetVal = 0;
        ret = (data.Read("TcnDevice", tcnDevice));
        if (ret) {
            tcnRetVal = tcn_register_device(tcnDevice.Buffer());
            switch(tcnRetVal) {
                case TCN_SUCCESS:
                    REPORT_ERROR(ErrorManagement::Information, "tcn_register_device succeeded! Registered @ %s", tcnDevice.Buffer());
                    break;
                case -ENOENT:
                    REPORT_ERROR(ErrorManagement::ParametersError, "tcn_register_device failed, configuration file does not exists (ENOENT)");           
                    ret = false;
                    break;
                case -ENODEV:
                    REPORT_ERROR(ErrorManagement::ParametersError, "tcn_register_device failed, device has not been recognized (ENODEV)");
                    ret = false;
                    break;
                case -ENOKEY:
                    REPORT_ERROR(ErrorManagement::ParametersError, "tcn_register_device failed, parsing error (ENOKEY)");
                    ret = false;
                    break;
                case -EPERM:
                    REPORT_ERROR(ErrorManagement::ParametersError, "tcn_register_device failed, error creating parser (EPERM)");
                    ret = false;
                    break;
                case -EBUSY:
                    REPORT_ERROR(ErrorManagement::ParametersError, "tcn_register_device failed, another concurrent init/reg or device already initialized (EBUSY)");
                    ret = false;
                    break;
                default:
                    REPORT_ERROR(ErrorManagement::ParametersError, "tcn_register_device failed, returned an unknown error (%d)", tcnRetVal);
                    ret = false;
                    break;
            }
            
            if (ret) {
                tcnRetVal = tcn_init();
                switch(tcnRetVal) {
                    case TCN_SUCCESS:
                        REPORT_ERROR(ErrorManagement::Information, "tcn_init successful!");
                        break;
                    case -EACCES:
                        REPORT_ERROR(ErrorManagement::FatalError, "tcn_init failed, configuration settings missing or invalid (EACCESS)");
                        ret = false;
                        break;
                    case -ENOSYS:
                        REPORT_ERROR(ErrorManagement::FatalError, "tcn_init failed, function not implemented by the tcn plugin (ENOSYS)");
                        ret = false;
                        break;
                    case -ENODEV:
                        REPORT_ERROR(ErrorManagement::FatalError, "tcn_init failed, invalid tcn device (ENODEV)");
                        ret = false;
                        break;
                    case -ENODATA:
                        REPORT_ERROR(ErrorManagement::FatalError, "tcn_init failed, timescale conversion table is missing or invalid (ENODATA)");
                        ret = false;
                        break;
                    default:
                        REPORT_ERROR(ErrorManagement::FatalError, "tcn_init failed, device specific error code (%d)", tcnRetVal);
                        ret = false;
                        break;
                }
            }
        }
        if(!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "Missing TcnDevice configuration parameter");
        }

        if(ret) {
            uint8 tcnPoll = 0u;
            if (data.Read("TcnPoll", tcnPoll)) {
                REPORT_ERROR(ErrorManagement::Information, "TcnPoll parameter is set to %d [Legacy Configuration Mode]", tcnPoll);
                if(tcnPoll == 0) {
                    BusySleepProvider = &TCNTimeProvider::NoPollBSP;
                }
                else {
                    BusySleepProvider = &TCNTimeProvider::PollBSP;
                }
            }
            else {
                StreamString tempOperationMode;
                if(data.Read("OperationMode", tempOperationMode)) {
                    if(tempOperationMode == "NoPollLegacyMode") {
                        REPORT_ERROR(ErrorManagement::Information, "No Poll legacy mode selected");
                        BusySleepProvider = &TCNTimeProvider::NoPollBSP;
                    }
                    else if(tempOperationMode == "PollLegacyMode") {
                        REPORT_ERROR(ErrorManagement::Information, "Poll legacy mode selected");
                        BusySleepProvider = &TCNTimeProvider::PollBSP;
                    }
                    else if(tempOperationMode == "WaitUntilMode") {
                        REPORT_ERROR(ErrorManagement::Information, "Wait until mode selected");
                        BusySleepProvider = &TCNTimeProvider::WaitUntilBSP;
                    }
                    else if(tempOperationMode == "WaitUntilHRMode") {
                        REPORT_ERROR(ErrorManagement::Information, "Wait until with high resolution counter mode selected");
                        BusySleepProvider = &TCNTimeProvider::WaitUntilHRBSP;
                    }
                    else if(tempOperationMode == "SleepMode") {
                        REPORT_ERROR(ErrorManagement::Information, "Sleep mode selected");
                        BusySleepProvider = &TCNTimeProvider::SleepBSP;
                    }
                    else if(tempOperationMode == "SleepHRMode") {
                        REPORT_ERROR(ErrorManagement::Information, "Sleep with high resolution counter mode selected");
                        BusySleepProvider = &TCNTimeProvider::SleepHRBSP;
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "An invalid operation mode was specified [%s]", tempOperationMode);
                        ret = false;
                    }
                    uint32 tempTolerance = 0u;

                    if(!data.Read("Tolerance", tempTolerance)) {
                        tolerance = TCNTIMEPROVIDER_DEFAULT_TOLERANCE;
                        REPORT_ERROR(ErrorManagement::Information, "Tolerance parameter omitted, defaulting to %d", tolerance);
                    }
                    else {
                        tolerance = tempTolerance;
                        REPORT_ERROR(ErrorManagement::Information, "Tolerance set to %d", tolerance);
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::Information, "No TcnPoll and no OperationMode parameter found, defaulting to Legacy NoPoll mode (TcnPoll = 0)");
                    BusySleepProvider = &TCNTimeProvider::NoPollBSP;
                }
            }
        }
    }
    return ret;
}

uint64 TCNTimeProvider::Counter() {
    uint64 tcnTime = 0u;
    hpn_timestamp_t tempTCNTime = 0u;

    int32 retVal = static_cast<int32>(tcn_get_time(&tempTCNTime));

    if(retVal != TCN_SUCCESS) {
        tempTCNTime = 0u;
        REPORT_ERROR(ErrorManagement::FatalError, "Counter() [tcn_get_time] is failing with error %d", retVal);
    }

    tcnTime = static_cast<uint64>(tempTCNTime);
    return tcnTime;
}

float64 TCNTimeProvider::Period() {
    return (1.0 / Frequency());
}

uint64 TCNTimeProvider::Frequency() {
    return tcnFrequency;
}

bool TCNTimeProvider::NullDelegate(uint64 start, uint64 delta) {
    REPORT_ERROR(ErrorManagement::FatalError, "Call to the null delegate with %d start and %d delta.", start, delta);
    REPORT_ERROR(ErrorManagement::FatalError, "Reached uninitialized portion of the code");
    return false;
}

bool TCNTimeProvider::NoPollBSP(uint64 start, uint64 delta) {
    while ((HighResolutionTimer::Counter() - start) < delta) {
        ;
    }

    //Since it relies on internal HRT, we can assume no failure
    return true;
}

bool TCNTimeProvider::PollBSP(uint64 start, uint64 delta) {
    bool retVal = true;    
        
    uint64 tempCounter = 0u;

    tempCounter = Counter();

    while ((tempCounter != 0u) && ((Counter() - start) < delta)) {
        tempCounter = Counter();
    }

    if(tempCounter == 0u) {
        REPORT_ERROR(ErrorManagement::FatalError, "Poll function failed due to 0 counter");
        retVal = false;
    }

    return retVal;
}

bool TCNTimeProvider::WaitUntilBSP(uint64 start, uint64 delta) {
    bool retVal = true;

    hpn_timestamp_t waitUntilDelta = static_cast<hpn_timestamp_t>(start + delta);
    int32 sleepResult = tcn_wait_until(waitUntilDelta, tolerance);
    if(sleepResult != TCN_SUCCESS) {
        REPORT_ERROR(ErrorManagement::FatalError, "Sleep providing function tcn_wait_until failing with error %d", sleepResult);
        retVal = false;
    }

    return retVal;
}

bool TCNTimeProvider::WaitUntilHRBSP(uint64 start, uint64 delta) {
    bool retVal = true;

    uint64 tempDelta = delta;

    hpn_timestamp_t waitUntilDeltaHR = (hpn_timestamp_t)(start + tempDelta);
    hpn_timestamp_t wakeUpTime = 0u;

    int32 sleepResult = static_cast<int32>(tcn_wait_until_hr(waitUntilDeltaHR, &wakeUpTime, tolerance));
    if(sleepResult != TCN_SUCCESS) {
        REPORT_ERROR(ErrorManagement::FatalError, "Sleep providing function tcn_sleep failing with error %d", sleepResult);
        retVal = false;
    }
    
    return retVal;
}

bool TCNTimeProvider::SleepBSP(uint64 start, uint64 delta) {
    bool retVal = true;
    hpn_timestamp_t tempDelta = static_cast<hpn_timestamp_t>(delta);

    int32 sleepResult = static_cast<int32>(tcn_sleep(tempDelta));
    if(sleepResult != TCN_SUCCESS) {
        REPORT_ERROR(ErrorManagement::FatalError, "Sleep providing function tcn_sleep failing with error %d", sleepResult);
        retVal = false;
    }

    return retVal;
}

bool TCNTimeProvider::SleepHRBSP(uint64 start, uint64 delta) {
    bool retVal = true;

    hpn_timestamp_t error = 0u;

    int32 sleepResult = static_cast<int32>(tcn_sleep_hr(static_cast<hpn_timestamp_t>(delta), &error, tolerance));
    if(sleepResult != TCN_SUCCESS) {
        REPORT_ERROR(ErrorManagement::FatalError, "Sleep providing function tcn_sleep_hr failing with error %d", sleepResult);
        retVal = false;
    }

    return retVal;
}

bool TCNTimeProvider::Sleep(uint64 start, uint64 delta) {
    return (this->*BusySleepProvider)(start, delta);
}

CLASS_REGISTER(TCNTimeProvider, "1.0")

}
