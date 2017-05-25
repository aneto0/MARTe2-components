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
#include "math.h"
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
    time0 = 0u;
    time1 = 0u;
    timeIncrement = 0u;
    timeState = 0u;

}
WaveformSin::~WaveformSin() {

}

bool WaveformSin::Initialise(StructuredDataI& data) {
    bool ok = Waveform::Initialise(data);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error initialising the configuration data file");
    }
    if (ok) {
        ok = data.Read("Amplitude", amplitude);
        if (ok) {
            ok = (amplitude != 0.0);
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
            if (frequency == 0.0) {
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
    if (timeState == 0) {
        time0 = *inputTime;
        timeState++;
    }
    else if (timeState == 1) {
        time1 = *inputTime;
        timeIncrement = double(time1 - time0) / numberOfOutputElements / 1e6;
        timeState++;
    }
    if (timeState == 2) {
        for (uint32 j = 0; j < numberOfOutputSignals; j++) {
            for (uint32 i = 0; i < numberOfOutputElements; i++) {
                ucurrentTime += utimeIncrement;
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
                            numberOfStopTriggers++;
                        }
                    }
                }
                if (signalOn) {
                    static_cast<uint8 *>(outputValue[j])[i] = static_cast<uint8>(amplitude
                            * sin(2.0 * FastMath::PI * frequency * (*inputTime / 1e6 + timeIncrement * i) + phase) + offset);
                }
                else {
                    static_cast<uint8 *>(outputValue[j])[i] = 0;
                }
            }
        }
    }
    return true;
}

bool WaveformSin::GetInt8Value() {
    if (timeState == 0) {
        time0 = *inputTime;
        timeState++;
    }
    else if (timeState == 1) {
        time1 = *inputTime;
        timeIncrement = double(time1 - time0) / numberOfOutputElements / 1e6;
        timeState++;
    }
    if (timeState == 2) {
        for (uint32 j = 0; j < numberOfOutputSignals; j++) {
            for (uint32 i = 0; i < numberOfOutputElements; i++) {
                static_cast<int8 *>(outputValue[j])[i] = static_cast<int8>(amplitude
                        * sin(2.0 * FastMath::PI * frequency * (*inputTime / 1e6 + timeIncrement * i) + phase) + offset);
            }
        }
    }
    return true;
}

bool WaveformSin::GetInt16Value() {
    if (timeState == 0) {
        time0 = *inputTime;
        timeState++;
    }
    else if (timeState == 1) {
        time1 = *inputTime;
        timeIncrement = double(time1 - time0) / numberOfOutputElements / 1e6;
        timeState++;
    }
    if (timeState == 2) {
        for (uint32 j = 0; j < numberOfOutputSignals; j++) {
            for (uint32 i = 0; i < numberOfOutputElements; i++) {
                static_cast<int16 *>(outputValue[j])[i] = static_cast<int16>(amplitude
                        * sin(2.0 * FastMath::PI * frequency * (*inputTime / 1e6 + timeIncrement * i) + phase) + offset);
            }
        }
    }
    return true;
}

bool WaveformSin::GetUInt16Value() {
    if (timeState == 0) {
        time0 = *inputTime;
        timeState++;
    }
    else if (timeState == 1) {
        time1 = *inputTime;
        timeIncrement = double(time1 - time0) / numberOfOutputElements / 1e6;
        timeState++;
    }
    if (timeState == 2) {
        for (uint32 j = 0; j < numberOfOutputSignals; j++) {
            for (uint32 i = 0; i < numberOfOutputElements; i++) {
                static_cast<uint16 *>(outputValue[j])[i] = static_cast<uint16>(amplitude
                        * sin(2.0 * FastMath::PI * frequency * (*inputTime / 1e6 + timeIncrement * i) + phase) + offset);
            }
        }
    }
    return true;
}

bool WaveformSin::GetInt32Value() {
    if (timeState == 0) {
        time0 = *inputTime;
        timeState++;
    }
    else if (timeState == 1) {
        time1 = *inputTime;
        timeIncrement = double(time1 - time0) / numberOfOutputElements / 1e6;
        timeState++;
    }
    if (timeState == 2) {
        for (uint32 j = 0; j < numberOfOutputSignals; j++) {
            for (uint32 i = 0; i < numberOfOutputElements; i++) {
                static_cast<int32 *>(outputValue[j])[i] = static_cast<int32>(amplitude
                        * sin(2.0 * FastMath::PI * frequency * (*inputTime / 1e6 + timeIncrement * i) + phase) + offset);
            }
        }
    }
    return true;
}

bool WaveformSin::GetUInt32Value() {
    if (timeState == 0) {
        time0 = *inputTime;
        timeState++;
    }
    else if (timeState == 1) {
        time1 = *inputTime;
        timeIncrement = double(time1 - time0) / numberOfOutputElements / 1e6;
        timeState++;
    }
    if (timeState == 2) {
        for (uint32 j = 0; j < numberOfOutputSignals; j++) {
            for (uint32 i = 0; i < numberOfOutputElements; i++) {
                static_cast<uint32 *>(outputValue[j])[i] = static_cast<uint32>(amplitude
                        * sin(2.0 * FastMath::PI * frequency * (*inputTime / 1e6 + timeIncrement * i) + phase) + offset);
            }
        }
    }
    return true;
}

bool WaveformSin::GetInt64Value() {
    if (timeState == 0) {
        time0 = *inputTime;
        timeState++;
    }
    else if (timeState == 1) {
        time1 = *inputTime;
        timeIncrement = double(time1 - time0) / numberOfOutputElements / 1e6;
        timeState++;
    }
    if (timeState == 2) {
        for (uint32 j = 0; j < numberOfOutputSignals; j++) {
            for (uint32 i = 0; i < numberOfOutputElements; i++) {
                static_cast<int64 *>(outputValue[j])[i] = static_cast<int64>(amplitude
                        * sin(2.0 * FastMath::PI * frequency * (*inputTime / 1e6 + timeIncrement * i) + phase) + offset);
            }
        }
    }
    return true;
}

bool WaveformSin::GetUInt64Value() {
    if (timeState == 0) {
        time0 = *inputTime;
        timeState++;
    }
    else if (timeState == 1) {
        time1 = *inputTime;
        timeIncrement = double(time1 - time0) / numberOfOutputElements / 1e6;
        timeState++;
    }
    if (timeState == 2) {
        for (uint32 j = 0; j < numberOfOutputSignals; j++) {
            for (uint32 i = 0; i < numberOfOutputElements; i++) {
                static_cast<uint64 *>(outputValue[j])[i] = static_cast<uint64>(amplitude
                        * sin(2.0 * FastMath::PI * frequency * (*inputTime / 1e6 + timeIncrement * i) + phase) + offset);
            }
        }
    }
    return true;
}

bool WaveformSin::GetFloat32Value() {
    if (timeState == 0) {
        time0 = *inputTime;
        timeState++;
    }
    else if (timeState == 1) {
        time1 = *inputTime;
        timeIncrement = double(time1 - time0) / numberOfOutputElements / 1e6;
        timeState++;
    }
    if (timeState == 2) {
        for (uint32 j = 0; j < numberOfOutputSignals; j++) {
            for (uint32 i = 0; i < numberOfOutputElements; i++) {
                static_cast<float32 *>(outputValue[j])[i] = static_cast<float32>(amplitude
                        * sin(2.0 * FastMath::PI * frequency * (*inputTime / 1e6 + timeIncrement * i) + phase) + offset);
            }
        }
    }
    return true;
}

bool WaveformSin::GetFloat64Value() {
    if (timeState == 0) {
        time0 = *inputTime;
        timeState++;
    }
    else if (timeState == 1) {
        time1 = *inputTime;
        timeIncrement = double(time1 - time0) / numberOfOutputElements / 1e6;
        timeState++;
    }
    if (timeState == 2) {
        for (uint32 j = 0; j < numberOfOutputSignals; j++) {
            for (uint32 i = 0; i < numberOfOutputElements; i++) {
                static_cast<float64 *>(outputValue[j])[i] = static_cast<float64>(amplitude
                        * sin(2.0 * FastMath::PI * frequency * (*inputTime / 1e6 + timeIncrement * i) + phase) + offset);
            }
        }
    }
    return true;
}
CLASS_REGISTER(WaveformSin, "1.0")

}
