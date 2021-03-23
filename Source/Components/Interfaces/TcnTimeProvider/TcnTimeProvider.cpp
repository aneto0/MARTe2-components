/**
 * @file TcnTimeProvider.cpp
 * @brief Source file for class TcnTimeProvider
 * @date 31/10/2019
 * @author Giuseppe Ferrò
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

 * @details This source file contains the definition of all the methods for
 * the class TcnTimeProvider (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <errno.h>
#include <tcn.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "StreamString.h"
#include "TcnTimeProvider.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

TcnTimeProvider::TcnTimeProvider() : TimeProvider() {
    tcnFrequency = TCNTIMEPROVIDER_DEFAULT_FREQUENCY;
    tolerance = TCNTIMEPROVIDER_DEFAULT_TOLERANCE;
    CounterProvider = &TcnTimeProvider::TCNCounter;
    BusySleepProvider = &TcnTimeProvider::NullDelegate;
}

TcnTimeProvider::~TcnTimeProvider() {
    int32 retVal = tcn_finalize();
    /*lint -e{1551} tcn_finalize is a C function and its return value is handled and shown */
    REPORT_ERROR(ErrorManagement::Information, "TCNTimeProvider disposed, tcn_finalize() returned %d", retVal);
}

bool TcnTimeProvider::InnerInitialize(StructuredDataI &data) {
    StreamString tcnDevice;
    int32 tcnRetVal = 0;
    bool ret = (data.Read("TcnDevice", tcnDevice));
    if (ret) {
        tcnRetVal = static_cast<int32>(tcn_register_device(tcnDevice.Buffer()));
        if(tcnRetVal == TCN_SUCCESS) {
            REPORT_ERROR(ErrorManagement::Information, "tcn_register_device succeeded! Registered @ %s", tcnDevice.Buffer());        
        }
        else {
            char8 tempErrorBuffer[255];
            const char8 *tempErrorBufferPtr = static_cast<const char8*>(tcn_strerror_r(static_cast<int32>(tcnRetVal), static_cast<char8*>(&tempErrorBuffer[0]), 255ul));
            StreamString errorString(tempErrorBufferPtr);
            REPORT_ERROR(ErrorManagement::FatalError, "tcn_register_device failed, %s", errorString.Buffer());
            ret = false;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "Missing TcnDevice parameter");
    }
        
    if (ret) {
        tcnRetVal = tcn_init();
        if(tcnRetVal == TCN_SUCCESS) {
            REPORT_ERROR(ErrorManagement::Information, "tcn_init successful!");            
        }
        else {
            char8 tempErrorBuffer[255];
            const char8 *tempErrorBufferPtr = static_cast<const char8*>(tcn_strerror_r(static_cast<int32>(tcnRetVal), static_cast<char8*>(&tempErrorBuffer[0]), 255ul));
            StreamString errorString(tempErrorBufferPtr);
            REPORT_ERROR(ErrorManagement::FatalError, "tcn_init failed, %s", errorString.Buffer());
            ret = false;
        }
    }

    if(ret) {
        uint64 tempTcnFrequency = 0u;
        if(data.Read("TcnFrequency", tempTcnFrequency)) {
            tcnFrequency = tempTcnFrequency;
            REPORT_ERROR(ErrorManagement::Information, "TcnFrequency manually set to %d, period going to be %f", tcnFrequency, Period());
        }
        else {
            tcnFrequency = TCNTIMEPROVIDER_DEFAULT_FREQUENCY;
            REPORT_ERROR(ErrorManagement::Warning, "TcnFrequency parameter omitted, defaulting to %d, period going to be %f", tcnFrequency, Period());
        }
    }

    if(ret) {
        uint8 tcnPoll = 0u;
        if (data.Read("TcnPoll", tcnPoll)) {
            REPORT_ERROR(ErrorManagement::Information, "TcnPoll parameter is set to %d [Legacy Configuration Mode]", tcnPoll);
            if(tcnPoll == 0u) {
                BusySleepProvider = &TcnTimeProvider::NoPollBSP;
                CounterProvider = &TcnTimeProvider::HRTCounter;
                tcnFrequency = HighResolutionTimer::Frequency();
                REPORT_ERROR(ErrorManagement::Warning, "TcnFrequency parameter overridden by HighResolutionTimer internal value %d", tcnFrequency);
            }
            else {
                BusySleepProvider = &TcnTimeProvider::PollBSP;
            }
        }
        else {
            StreamString tempOperationMode;
            if(data.Read("OperationMode", tempOperationMode)) {
                if(tempOperationMode == "NoPollLegacyMode") {
                    BusySleepProvider = &TcnTimeProvider::NoPollBSP;
                    CounterProvider = &TcnTimeProvider::HRTCounter;
                    tcnFrequency = HighResolutionTimer::Frequency();

                    REPORT_ERROR(ErrorManagement::Information, "No Poll legacy mode selected");
                    REPORT_ERROR(ErrorManagement::Warning, "TcnFrequency parameter overridden by HighResolutionTimer internal value %d", tcnFrequency);
                }
                else if(tempOperationMode == "PollLegacyMode") {
                    REPORT_ERROR(ErrorManagement::Information, "Poll legacy mode selected");
                    BusySleepProvider = &TcnTimeProvider::PollBSP;
                }
                else if(tempOperationMode == "WaitUntilMode") {
                    REPORT_ERROR(ErrorManagement::Information, "Wait until mode selected");
                    BusySleepProvider = &TcnTimeProvider::WaitUntilBSP;
                }
                else if(tempOperationMode == "WaitUntilHRMode") {
                    REPORT_ERROR(ErrorManagement::Information, "Wait until with high resolution counter mode selected");
                    BusySleepProvider = &TcnTimeProvider::WaitUntilHRBSP;
                }
                else if(tempOperationMode == "SleepMode") {
                    REPORT_ERROR(ErrorManagement::Information, "Sleep mode selected");
                    BusySleepProvider = &TcnTimeProvider::SleepBSP;
                }
                else if(tempOperationMode == "SleepHRMode") {
                    REPORT_ERROR(ErrorManagement::Information, "Sleep with high resolution counter mode selected");
                    BusySleepProvider = &TcnTimeProvider::SleepHRBSP;
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "An invalid operation mode was specified [%s]", tempOperationMode.Buffer());
                    ret = false;
                }
                uint32 tempTolerance = 0u;

                if(!data.Read("Tolerance", tempTolerance)) {
                    tolerance = TCNTIMEPROVIDER_DEFAULT_TOLERANCE;
                    REPORT_ERROR(ErrorManagement::Warning, "Tolerance parameter omitted, defaulting to %d", tolerance);
                }
                else {
                    tolerance = tempTolerance;
                    REPORT_ERROR(ErrorManagement::Information, "Tolerance set to %d", tolerance);
                }
            }
            else {
                BusySleepProvider = &TcnTimeProvider::NoPollBSP;
                CounterProvider = &TcnTimeProvider::HRTCounter;
                tcnFrequency = HighResolutionTimer::Frequency();

                REPORT_ERROR(ErrorManagement::Warning, "No TcnPoll and no OperationMode parameter found, defaulting to Legacy NoPoll mode (TcnPoll = 0)");
                REPORT_ERROR(ErrorManagement::Warning, "TcnFrequency parameter overridden by HighResolutionTimer internal value %d", tcnFrequency);
            }
        }
    }
    return ret;  
}

bool TcnTimeProvider::Initialise(StructuredDataI &data) {
    bool ret = Object::Initialise(data);
    
    if (ret) {
        ret = InnerInitialize(data);
    }
    return ret;
}

bool TcnTimeProvider::BackwardCompatibilityInit(StructuredDataI &compatibilityData) {
    REPORT_ERROR(ErrorManagement::Warning, "Backward compatibility mode requested, Timer injecting parameters. Looking for TcnPoll value overriding");
    uint8 tcnPoll = 0u;
    if (compatibilityData.Read("TcnPoll", tcnPoll)) {
        REPORT_ERROR(ErrorManagement::Information, "TcnPoll parameter is set to %d [Legacy Configuration Mode]", tcnPoll);
        if(tcnPoll == 0u) {
            BusySleepProvider = &TcnTimeProvider::NoPollBSP;
            CounterProvider = &TcnTimeProvider::HRTCounter;
            tcnFrequency = HighResolutionTimer::Frequency();
            REPORT_ERROR(ErrorManagement::Warning, "TcnFrequency parameter overridden by HighResolutionTimer internal value %d", tcnFrequency);
        }
        else {
            BusySleepProvider = &TcnTimeProvider::PollBSP;
            uint64 tempTCNFrequency = 0u;
            if (compatibilityData.Read("TcnFrequency", tempTCNFrequency)) {
                tcnFrequency = tempTCNFrequency;
                REPORT_ERROR(ErrorManagement::Information, "TcnFrequency parameter is set to %d", tcnFrequency);
            }
            else {
                tcnFrequency = TCNTIMEPROVIDER_DEFAULT_FREQUENCY;
                REPORT_ERROR(ErrorManagement::Warning, "TcnFrequency parameter not injected, defaulting to %d", tcnFrequency);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "Backward compatibility mode requested but no TcnPoll parameter found. No override took place.");
    }
    //Backward compatibility has no way of failing.
    return true;
}

uint64 TcnTimeProvider::HRTCounter() const {
    return HighResolutionTimer::Counter();
}

uint64 TcnTimeProvider::TCNCounter() const {
    uint64 tcnTime;
    hpn_timestamp_t tempTCNTime = 0u;

    int32 retVal = static_cast<int32>(tcn_get_time(&tempTCNTime));

    if(retVal != TCN_SUCCESS) {
        tempTCNTime = 0u;
        REPORT_ERROR(ErrorManagement::FatalError, "Counter() [tcn_get_time] is failing with error %d", retVal);
    }

    tcnTime = static_cast<uint64>(tempTCNTime);

    return tcnTime;
}

uint64 TcnTimeProvider::Counter() {
    return (this->*CounterProvider)();
}

float64 TcnTimeProvider::Period() {
    float64 retVal = 1.0 / static_cast<float64>(tcnFrequency);
    return retVal;
}

uint64 TcnTimeProvider::Frequency() {
    return tcnFrequency;
}

/*lint -e{1762} The function is a generic delegate which needs to match other signatures No const can be added*/
bool TcnTimeProvider::NullDelegate(const uint64 start, const uint64 delta) {
    REPORT_ERROR(ErrorManagement::FatalError, "Call to the null delegate with %d start and %d delta.", start, delta);
    REPORT_ERROR(ErrorManagement::FatalError, "Reached uninitialized portion of the code");
    return false;
}

/*lint -e{1762} The function is a generic delegate which needs to match other signatures No const can be added*/
bool TcnTimeProvider::NoPollBSP(const uint64 start, const uint64 delta) {

    while ((HighResolutionTimer::Counter() - start) < delta) {
        ;
    }

    //Since it relies on internal HRT, we can assume no failure
    return true;
}

/*lint -e{1762} The function is a generic delegate which needs to match other signatures No const can be added*/
bool TcnTimeProvider::PollBSP(const uint64 start, const uint64 delta) {
    bool retVal = true;    
        
    uint64 tempCounter = Counter();

    while ((tempCounter != 0u) && ((tempCounter - start) < delta)) {
        tempCounter = Counter();
    }

    if(tempCounter == 0u) {
        REPORT_ERROR(ErrorManagement::FatalError, "Poll function failed due to 0 counter");
        retVal = false;
    }

    return retVal;
}

/*lint -e{1762} The function is a generic delegate which needs to match other signatures No const can be added*/
bool TcnTimeProvider::WaitUntilBSP(const uint64 start, const uint64 delta) {
    bool retVal = true;

    hpn_timestamp_t waitUntilDelta = static_cast<hpn_timestamp_t>(start + delta);
    int32 sleepResult = tcn_wait_until(waitUntilDelta, tolerance);
    if(sleepResult != TCN_SUCCESS) {
        REPORT_ERROR(ErrorManagement::FatalError, "Sleep providing function tcn_wait_until failing with error %d", sleepResult);
        retVal = false;
    }

    return retVal;
}

/*lint -e{1762} The function is a generic delegate which needs to match other signatures No const can be added*/
bool TcnTimeProvider::WaitUntilHRBSP(const uint64 start, const uint64 delta) {
    bool retVal = true;

    uint64 tempDelta = delta;

    hpn_timestamp_t waitUntilDeltaHR = static_cast<hpn_timestamp_t>(start + tempDelta);
    hpn_timestamp_t wakeUpTime = 0u;

    int32 sleepResult = static_cast<int32>(tcn_wait_until_hr(waitUntilDeltaHR, &wakeUpTime, tolerance));
    if(sleepResult != TCN_SUCCESS) {
        REPORT_ERROR(ErrorManagement::FatalError, "Sleep providing function tcn_sleep failing with error %d", sleepResult);
        retVal = false;
    }
    
    return retVal;
}

/*lint -e{715,1762} start value is ignored because SleepBSP only considers delta difference for sleeping */
bool TcnTimeProvider::SleepBSP(const uint64 start, const uint64 delta) {
    bool retVal = true;
    hpn_timestamp_t tempDelta = static_cast<hpn_timestamp_t>(delta);

    int32 sleepResult = static_cast<int32>(tcn_sleep(tempDelta));
    if(sleepResult != TCN_SUCCESS) {
        REPORT_ERROR(ErrorManagement::FatalError, "Sleep providing function tcn_sleep failing with error %d", sleepResult);
        retVal = false;
    }

    return retVal;
}

/*lint -e{715, 1762} start value is ignored because SleepHRBSP only considers delta difference for sleeping */
bool TcnTimeProvider::SleepHRBSP(const uint64 start, const uint64 delta) {
    bool retVal = true;

    hpn_timestamp_t error = 0u;

    int32 sleepResult = static_cast<int32>(tcn_sleep_hr(static_cast<hpn_timestamp_t>(delta), &error, tolerance));
    if(sleepResult != TCN_SUCCESS) {
        REPORT_ERROR(ErrorManagement::FatalError, "Sleep providing function tcn_sleep_hr failing with error %d", sleepResult);
        retVal = false;
    }

    return retVal;
}

bool TcnTimeProvider::Sleep(const uint64 start, const uint64 delta) {
    return (this->*BusySleepProvider)(start, delta);
}

CLASS_REGISTER(TcnTimeProvider, "1.0")

}
