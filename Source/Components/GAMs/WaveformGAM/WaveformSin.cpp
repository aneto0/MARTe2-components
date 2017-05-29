/**
 * @file WaveformSin.cpp
 * @brief Source file for class WaveformSin
 * @date May 19, 2017
 * @author aneto
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
 * the class WaveformSin (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "FastMath.h"
#include "../WaveformGAM/WaveformSin.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

WaveformSin::WaveformSin() :
        Waveform() {
    amplitude = 0.0;
    frequency = 0.0;
    phase = 0.0;
    offset = 0.0;

}

WaveformSin::~WaveformSin() {

}

bool WaveformSin::Initialise(StructuredDataI& data) {
    bool ok = Waveform::Initialise(data);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error. Waveform returns an initialization error");
    }
    if (ok) {
        ok = data.Read("Amplitude", amplitude);
        if (ok) {
            ok = !(IsEqual(amplitude, 0.0));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "amplitude must be different from 0");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading Amplitude");
        }
    }
    if (ok) {
        ok = data.Read("Frequency", frequency);
        if (ok) {
            if (IsEqual(frequency, 0.0)) {
                REPORT_ERROR(ErrorManagement::Warning, "The frequency is 0. If phase = 0 the output will be always 0");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading Frequency");
        }
    }
    if (ok) {
        ok = data.Read("Phase", phase);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading Phase");
        }
    }
    if (ok) {
        ok = data.Read("Offset", offset);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading offset");
        }
    }
    return ok;
}

bool WaveformSin::GetUInt8Value() {
    return WaveformSin::GetValue<uint8>();
}

template<typename T>
bool WaveformSin::GetValue() {
    ucurrentTime = *inputTime;
    if (timeState == 0) {
        time0 = *inputTime;
        timeState++;
        for (uint32 i = 0; i < numberOfOutputElements; i++) {
            static_cast<T *>(outputValue[indexOutputSignal])[i] = static_cast<T>(0);
        }
    }
    else if (timeState == 1) {
        time1 = *inputTime;
        utimeIncrement = (time1 - time0) / numberOfOutputElements;
        timeIncrement = double(time1 - time0) / numberOfOutputElements / 1e6;
        timeState++;
    }
    if (timeState == 2) {
        for (uint32 i = 0; i < numberOfOutputElements; i++) {
            if (triggersOn) {
                if (numberOfStartTriggers > indexStartTriggersArray) {
                    if (startTriggerTime[indexStartTriggersArray] <= ucurrentTime) {
                        signalOn = true;
                        indexStartTriggersArray++;
                    }
                }
                if (indexStopTriggersArray < numberOfStopTriggers) {
                    if (stopTriggerTime[indexStopTriggersArray] <= ucurrentTime) {
                        signalOn = false;
                        indexStopTriggersArray++;
                    }
                }
            }
            else {
                signalOn = true;
            }
            if (signalOn) {
                static_cast<T *>(outputValue[indexOutputSignal])[i] = static_cast<T>(amplitude
                        * FastMath::Sin(2.0 * FastMath::PI * frequency * (*inputTime / 1e6 + timeIncrement * i) + phase) + offset);
            }
            else {
                static_cast<T *>(outputValue[indexOutputSignal])[i] = static_cast<T>(0);
            }
            ucurrentTime += utimeIncrement;
        }
    }
    return true;
}

bool WaveformSin::GetInt8Value() {
    return WaveformSin::GetValue<int8>();
}

bool WaveformSin::GetInt16Value() {
    return WaveformSin::GetValue<int16>();
}

bool WaveformSin::GetUInt16Value() {
    return WaveformSin::GetValue<uint16>();
}

bool WaveformSin::GetInt32Value() {
    return WaveformSin::GetValue<int32>();
}

bool WaveformSin::GetUInt32Value() {
    return WaveformSin::GetValue<uint32>();
}

bool WaveformSin::GetInt64Value() {
    return WaveformSin::GetValue<int64>();
}

bool WaveformSin::GetUInt64Value() {
    return WaveformSin::GetValue<uint64>();
}

bool WaveformSin::GetFloat32Value() {
    return WaveformSin::GetValue<float32>();
}

bool WaveformSin::GetFloat64Value() {
    return WaveformSin::GetValue<float64>();
}
CLASS_REGISTER(WaveformSin, "1.0")

}
