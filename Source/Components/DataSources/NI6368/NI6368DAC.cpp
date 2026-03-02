/**
 * @file NI6368DAC.cpp
 * @brief Source file for class NI6368DAC
 * @date 20/01/2017
 * @author Andre Neto
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
 * the class NI6368DAC (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <fcntl.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryMapSynchronisedOutputBroker.h"

#include "NI6368DAC.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
#define DAC_BASE_FREQ 100000000u
#define INIT_HEADER_SIZE 0xFFFFFFFFu

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
NI6368DAC::NI6368DAC() :
        DataSourceI() {
    boardId = 0u;
    boardFileDescriptor = -1;
    rootFileDescriptor = -1;
    deviceName = "";
    numberOfDACsEnabled = 0u;
    triggerSet = false;

    startTriggerSource = XSERIES_AO_START_TRIGGER_SW_PULSE;
    startTriggerPolarity = XSERIES_AO_POLARITY_RISING_EDGE;
    updateCounterSource = XSERIES_AO_UPDATE_COUNTER_UI_TC;
    updateCounterPolarity = XSERIES_AO_POLARITY_RISING_EDGE;
    clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB3;
    clockUpdatePolarity = XSERIES_OUTTIMER_POLARITY_RISING_EDGE;
    clockUpdateDivisor = 100u;
    clockUpdateDelay = 2u;
    baseSampleClockFrequency = 0u;
    for (uint32 n = 0u; n < NI6368DAC_MAX_CHANNELS; n++) {
        outputRange[n] = XSERIES_OUTPUT_RANGE_10V;
        dacEnabled[n] = false;
        channelsFileDescriptors[n] = -1;
        channelsMemory[n] = NULL_PTR(float32*);
        outputGain[n] = 1.0;
        outputOffset[n] = 0.0;
    }

    numberOfSamplesT = 0u;
    multiplexed = 0u;
    headerSize = INIT_HEADER_SIZE;
    boardStatus = 0u;
}

/*lint -e{1551} the destructor must guarantee that the NI6368DAC file descriptors are closed.*/
NI6368DAC::~NI6368DAC() {
    uint32 n;
    if (boardFileDescriptor != -1) {
        if (xseries_stop_ao(boardFileDescriptor) < 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop acquisition.");
        }
    }

    for (n = 0u; n < NI6368DAC_MAX_CHANNELS; n++) {
        if (channelsFileDescriptors[n] != -1) {
            close(channelsFileDescriptors[n]);
        }
        if (channelsMemory[n] != NULL_PTR(float32*)) {
            delete[] channelsMemory[n];
        }
    }

    if (boardFileDescriptor != -1) {
        close(boardFileDescriptor);
    }
    if (rootFileDescriptor != -1) {
        close(rootFileDescriptor);
    }
}

bool NI6368DAC::AllocateMemory() {
    return true;
}

uint32 NI6368DAC::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool NI6368DAC::GetSignalMemoryBuffer(const uint32 signalIdx,
                                      const uint32 bufferIdx,
                                      void *&signalAddress) {
    bool ok = (headerSize != INIT_HEADER_SIZE);
    if (ok) {
        ok = (signalIdx < (NI6368DAC_MAX_CHANNELS + headerSize));
    }
    if (ok) {
        if (signalIdx < headerSize) {
            if (signalIdx == 0u) {
                signalAddress = reinterpret_cast<void*>(&boardStatus);
            }
        }
        else {
            signalAddress = &(channelsMemory[signalIdx - headerSize][0]);
        }
    }
    return ok;
}

const char8* NI6368DAC::GetBrokerName(StructuredDataI &data,
                                      const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8*);
    float32 frequency = 0.F;
    if (!data.Read("Frequency", frequency)) {
        frequency = -1.F;
    }

    uint32 trigger = 0u;
    if (!data.Read("Trigger", trigger)) {
        trigger = 0u;
    }
    if (direction == OutputSignals) {
        if ((frequency >= 0.F) || (trigger > 0u)) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
            triggerSet = true;
        }
        else {
            brokerName = "MemoryMapOutputBroker";
        }
    }
    else if (direction == InputSignals) {
        if ((frequency >= 0.F) || (trigger > 0u)) {
            brokerName = "MemoryMapSynchronisedInputBroker";
            triggerSet = true;
        }
        else {
            brokerName = "MemoryMapInputBroker";
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible");
    }
    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP irrespectively of the input parameters.*/
bool NI6368DAC::PrepareNextState(const char8 *const currentStateName,
                                 const char8 *const nextStateName) {
    return true;
}

bool NI6368DAC::Initialise(StructuredDataI &data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("DeviceName", deviceName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The DeviceName shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("BoardId", boardId);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The BoardId shall be specified");
        }
    }
    if (!data.Read("IsMultiplexed", multiplexed)) {
        //Assume 6368 by default
        multiplexed = 0u;
    }
    if (ok) {
        if (!data.Read("BaseSampleClockFrequency", baseSampleClockFrequency)) {
            baseSampleClockFrequency = DAC_BASE_FREQ;
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "BaseSampleClockFrequency not defined. Set to default %d", baseSampleClockFrequency);
        }
    }
    if (ok) {
        StreamString startTriggerSourceStr;

        if (data.Read("StartTriggerSource", startTriggerSourceStr)) {
            if (startTriggerSourceStr == "SW_PULSE") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_SW_PULSE;
            }
            else if (startTriggerSourceStr == "PFI0") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI0;
            }
            else if (startTriggerSourceStr == "PFI1") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI1;
            }
            else if (startTriggerSourceStr == "PFI2") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI2;
            }
            else if (startTriggerSourceStr == "PFI3") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI3;
            }
            else if (startTriggerSourceStr == "PFI4") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI4;
            }
            else if (startTriggerSourceStr == "PFI5") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI5;
            }
            else if (startTriggerSourceStr == "PFI6") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI6;
            }
            else if (startTriggerSourceStr == "PFI7") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI7;
            }
            else if (startTriggerSourceStr == "PFI8") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI8;
            }
            else if (startTriggerSourceStr == "PFI9") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI9;
            }
            else if (startTriggerSourceStr == "RTSI0") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_RTSI0;
            }
            else if (startTriggerSourceStr == "RTSI1") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_RTSI1;
            }
            else if (startTriggerSourceStr == "RTSI2") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_RTSI2;
            }
            else if (startTriggerSourceStr == "RTSI3") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_RTSI3;
            }
            else if (startTriggerSourceStr == "RTSI4") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_RTSI4;
            }
            else if (startTriggerSourceStr == "RTSI5") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_RTSI5;
            }
            else if (startTriggerSourceStr == "RTSI6") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_RTSI6;
            }
            else if (startTriggerSourceStr == "AI_START2") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_AI_START2;
            }
            else if (startTriggerSourceStr == "AI_START1") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_AI_START1;
            }
            else if (startTriggerSourceStr == "STAR_TRIGGER") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_STAR_TRIGGER;
            }
            else if (startTriggerSourceStr == "PFI10") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI10;
            }
            else if (startTriggerSourceStr == "PFI11") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI11;
            }
            else if (startTriggerSourceStr == "PFI12") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI12;
            }
            else if (startTriggerSourceStr == "PFI13") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI13;
            }
            else if (startTriggerSourceStr == "PFI14") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI14;
            }
            else if (startTriggerSourceStr == "PFI15") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PFI15;
            }
            else if (startTriggerSourceStr == "RTSI7") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_RTSI7;
            }
            else if (startTriggerSourceStr == "PXIE_DSTARA") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PXIE_DSTARA;
            }
            else if (startTriggerSourceStr == "PXIE_DSTARB") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_PXIE_DSTARB;
            }
            else if (startTriggerSourceStr == "ANALOG_TRIGGER") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_ANALOG_TRIGGER;
            }
            else if (startTriggerSourceStr == "LOW") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_LOW;
            }
            else if (startTriggerSourceStr == "G0_OUT") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_G0_OUT;
            }
            else if (startTriggerSourceStr == "G1_OUT") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_G1_OUT;
            }
            else if (startTriggerSourceStr == "G2_OUT") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_G2_OUT;
            }
            else if (startTriggerSourceStr == "G3_OUT") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_G3_OUT;
            }
            else if (startTriggerSourceStr == "DIO_CHGDETECT") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_DIO_CHGDETECT;
            }
            else if (startTriggerSourceStr == "DI_START1") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_DI_START1;
            }
            else if (startTriggerSourceStr == "DI_START2") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_DI_START2;
            }
            else if (startTriggerSourceStr == "DO_START1") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_DO_START1;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA0") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_INTTRIGGERA0;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA1") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_INTTRIGGERA1;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA2") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_INTTRIGGERA2;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA3") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_INTTRIGGERA3;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA4") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_INTTRIGGERA4;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA5") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_INTTRIGGERA5;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA6") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_INTTRIGGERA6;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA7") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_INTTRIGGERA7;
            }
            else if (startTriggerSourceStr == "FIFOCONDITION") {
                startTriggerSource = XSERIES_AO_START_TRIGGER_FIFOCONDITION;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid StartTriggerSource specified");
                ok = false;
            }
        }
        else {
            startTriggerSource = XSERIES_AO_START_TRIGGER_SW_PULSE;
            REPORT_ERROR(ErrorManagement::Information, "StartTriggerSource not defined. Using default XSERIES_AO_START_TRIGGER_SW_PULSE");
        }
    }
    if (ok) {
        bool isConfigured = false;
        StreamString baseClockUpdateSourceStr;
        if (data.Read("BaseClockUpdateSource", baseClockUpdateSourceStr)) {
            isConfigured = true;
        }
        else if (data.Read("UpdateCounterSource", baseClockUpdateSourceStr)) {
            isConfigured = true;
        }
        else {
            updateCounterSource = XSERIES_AO_UPDATE_COUNTER_UI_TC;
            REPORT_ERROR(ErrorManagement::Information, "UpdateCounterSource not defined. Using default XSERIES_AO_UPDATE_COUNTER_UI_TC");
        }
        if (isConfigured) {
            if (baseClockUpdateSourceStr == "UI_TC") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_UI_TC;
            }
            else if (baseClockUpdateSourceStr == "PFI0") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI0;
            }
            else if (baseClockUpdateSourceStr == "PFI1") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI1;
            }
            else if (baseClockUpdateSourceStr == "PFI2") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI2;
            }
            else if (baseClockUpdateSourceStr == "PFI3") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI3;
            }
            else if (baseClockUpdateSourceStr == "PFI4") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI4;
            }
            else if (baseClockUpdateSourceStr == "PFI5") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI5;
            }
            else if (baseClockUpdateSourceStr == "PFI6") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI6;
            }
            else if (baseClockUpdateSourceStr == "PFI7") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI7;
            }
            else if (baseClockUpdateSourceStr == "PFI8") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI8;
            }
            else if (baseClockUpdateSourceStr == "PFI9") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI9;
            }
            else if (baseClockUpdateSourceStr == "RTSI0") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI0;
            }
            else if (baseClockUpdateSourceStr == "RTSI1") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI1;
            }
            else if (baseClockUpdateSourceStr == "RTSI2") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI2;
            }
            else if (baseClockUpdateSourceStr == "RTSI3") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI3;
            }
            else if (baseClockUpdateSourceStr == "RTSI4") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI4;
            }
            else if (baseClockUpdateSourceStr == "RTSI5") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI5;
            }
            else if (baseClockUpdateSourceStr == "RTSI6") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI6;
            }
            else if (baseClockUpdateSourceStr == "G0_OUT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G0_OUT;
            }
            else if (baseClockUpdateSourceStr == "G1_OUT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G1_OUT;
            }
            else if (baseClockUpdateSourceStr == "STAR_TRIGGER") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_STAR_TRIGGER;
            }
            else if (baseClockUpdateSourceStr == "PFI10") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI10;
            }
            else if (baseClockUpdateSourceStr == "PFI11") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI11;
            }
            else if (baseClockUpdateSourceStr == "PFI12") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI12;
            }
            else if (baseClockUpdateSourceStr == "PFI13") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI13;
            }
            else if (baseClockUpdateSourceStr == "PFI14") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI14;
            }
            else if (baseClockUpdateSourceStr == "PFI15") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI15;
            }
            else if (baseClockUpdateSourceStr == "RTSI7") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI7;
            }
            else if (baseClockUpdateSourceStr == "G2_OUT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G2_OUT;
            }
            else if (baseClockUpdateSourceStr == "G3_OUT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G3_OUT;
            }
            else if (baseClockUpdateSourceStr == "ANALOG_TRIGGER") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_ANALOG_TRIGGER;
            }
            else if (baseClockUpdateSourceStr == "LOW") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_LOW;
            }
            else if (baseClockUpdateSourceStr == "PXIE_DSTARA") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PXIE_DSTARA;
            }
            else if (baseClockUpdateSourceStr == "PXIE_DSTARB") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PXIE_DSTARB;
            }
            else if (baseClockUpdateSourceStr == "DIO_CHGDETECT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_DIO_CHGDETECT;
            }
            else if (baseClockUpdateSourceStr == "G0_SAMPLECLK") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G0_SAMPLECLK;
            }
            else if (baseClockUpdateSourceStr == "G1_SAMPLECLK") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G1_SAMPLECLK;
            }
            else if (baseClockUpdateSourceStr == "G2_SAMPLECLK") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G2_SAMPLECLK;
            }
            else if (baseClockUpdateSourceStr == "G3_SAMPLECLK") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G3_SAMPLECLK;
            }
            else if (baseClockUpdateSourceStr == "AI_CONVERT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_AI_CONVERT;
            }
            else if (baseClockUpdateSourceStr == "AI_START") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_AI_START;
            }
            else if (baseClockUpdateSourceStr == "DI_CONVERT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_DI_CONVERT;
            }
            else if (baseClockUpdateSourceStr == "DO_UPDATE") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_DO_UPDATE;
            }
            else if (baseClockUpdateSourceStr == "INTTRIGGERA0") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA0;
            }
            else if (baseClockUpdateSourceStr == "INTTRIGGERA1") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA1;
            }
            else if (baseClockUpdateSourceStr == "INTTRIGGERA2") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA2;
            }
            else if (baseClockUpdateSourceStr == "INTTRIGGERA3") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA3;
            }
            else if (baseClockUpdateSourceStr == "INTTRIGGERA4") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA4;
            }
            else if (baseClockUpdateSourceStr == "INTTRIGGERA5") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA5;
            }
            else if (baseClockUpdateSourceStr == "INTTRIGGERA6") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA6;
            }
            else if (baseClockUpdateSourceStr == "INTTRIGGERA7") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA7;
            }
            else if (baseClockUpdateSourceStr == "AUTOUPDATE") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_AUTOUPDATE;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid UpdateCounterSource specified");
                ok = false;
            }
        }
    }
    if (ok) {
        bool isConfigured = false;
        StreamString baseClockUpdatePolarityStr;
        if (data.Read("BaseClockUpdatePolarity", baseClockUpdatePolarityStr)) {
            isConfigured = true;
        }
        else if (data.Read("UpdateCounterPolarity", baseClockUpdatePolarityStr)) {
            isConfigured = true;
        }
        else {
            updateCounterPolarity = XSERIES_AO_POLARITY_RISING_EDGE;
            REPORT_ERROR(ErrorManagement::Information, "UpdateCounterPolarity not defined. Using default XSERIES_AO_POLARITY_RISING_EDGE");
        }
        if (isConfigured) {
            if (baseClockUpdatePolarityStr == "RISING_EDGE") {
                updateCounterPolarity = XSERIES_AO_POLARITY_RISING_EDGE;
            }
            else if (baseClockUpdatePolarityStr == "FALLING_EDGE") {
                updateCounterPolarity = XSERIES_AO_POLARITY_FALLING_EDGE;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid UpdateCounterPolarity specified");
                ok = false;
            }
        }
    }
    if (ok) {
        bool isConfigured = false;
        StreamString clockUpdateSourceStr;
        if (data.Read("ClockUpdateSource", clockUpdateSourceStr)) {
            isConfigured = true;
        }
        else if (data.Read("UpdateIntervalCounterSource", clockUpdateSourceStr)) {
            isConfigured = true;
        }
        else {
            clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB3;
            REPORT_ERROR(ErrorManagement::Information, "UpdateIntervalCounterSource not defined. Using default XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB3");
        }

        if (isConfigured) {
            if (clockUpdateSourceStr == "TB3") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB3;
            }
            else if (clockUpdateSourceStr == "PFI0") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI0;
            }
            else if (clockUpdateSourceStr == "PFI1") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI1;
            }
            else if (clockUpdateSourceStr == "PFI2") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI2;
            }
            else if (clockUpdateSourceStr == "PFI3") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI3;
            }
            else if (clockUpdateSourceStr == "PFI4") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI4;
            }
            else if (clockUpdateSourceStr == "PFI5") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI5;
            }
            else if (clockUpdateSourceStr == "PFI6") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI6;
            }
            else if (clockUpdateSourceStr == "PFI7") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI7;
            }
            else if (clockUpdateSourceStr == "PFI8") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI8;
            }
            else if (clockUpdateSourceStr == "PFI9") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI9;
            }
            else if (clockUpdateSourceStr == "RTSI0") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI0;
            }
            else if (clockUpdateSourceStr == "RTSI1") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI1;
            }
            else if (clockUpdateSourceStr == "RTSI2") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI2;
            }
            else if (clockUpdateSourceStr == "RTSI3") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI3;
            }
            else if (clockUpdateSourceStr == "RTSI4") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI4;
            }
            else if (clockUpdateSourceStr == "RTSI5") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI5;
            }
            else if (clockUpdateSourceStr == "RTSI6") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI6;
            }
            else if (clockUpdateSourceStr == "DSTARA") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_DSTARA;
            }
            else if (clockUpdateSourceStr == "TB2") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB2;
            }
            else if (clockUpdateSourceStr == "STAR_TRIGGER") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_STAR_TRIGGER;
            }
            else if (clockUpdateSourceStr == "PFI10") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI10;
            }
            else if (clockUpdateSourceStr == "PFI11") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI11;
            }
            else if (clockUpdateSourceStr == "PFI12") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI12;
            }
            else if (clockUpdateSourceStr == "PFI13") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI13;
            }
            else if (clockUpdateSourceStr == "PFI14") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI14;
            }
            else if (clockUpdateSourceStr == "PFI15") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI15;
            }
            else if (clockUpdateSourceStr == "RTSI7") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI7;
            }
            else if (clockUpdateSourceStr == "TB1") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB1;
            }
            else if (clockUpdateSourceStr == "PXI_CLK10") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PXI_CLK10;
            }
            else if (clockUpdateSourceStr == "ANALOG_TRIGGER") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_ANALOG_TRIGGER;
            }
            else if (clockUpdateSourceStr == "DSTARB") {
                clockUpdateSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_DSTARB;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid UpdateIntervalCounterSource specified");
                ok = false;
            }
        }
    }
    if (ok) {
        bool isConfigured = false;
        StreamString clockUpdatePolarityStr;
        if (data.Read("ClockUpdatePolarity", clockUpdatePolarityStr)) {
            isConfigured = true;
        }
        //maintain for backward compatibility
        else if (data.Read("UpdateIntervalCounterPolarity", clockUpdatePolarityStr)) {
            isConfigured = true;
        }
        else {
            clockUpdatePolarity = XSERIES_OUTTIMER_POLARITY_RISING_EDGE;
            REPORT_ERROR(ErrorManagement::Information, "UpdateIntervalCounterPolarity not defined. Using default XSERIES_OUTTIMER_POLARITY_RISING_EDGE");
        }

        if (isConfigured) {
            if (clockUpdatePolarityStr == "RISING_EDGE") {
                clockUpdatePolarity = XSERIES_OUTTIMER_POLARITY_RISING_EDGE;
            }
            else if (clockUpdatePolarityStr == "FALLING_EDGE") {
                clockUpdatePolarity = XSERIES_OUTTIMER_POLARITY_FALLING_EDGE;
            }
            else {
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockUpdatePolarity");
            }
        }

    }

    if (ok) {
        bool set = false;
        uint32 samplingFrequency = 0u;
        if (data.Read("SamplingFrequency", samplingFrequency)) {
            ok = (samplingFrequency <= baseSampleClockFrequency) && (samplingFrequency > 0u);
            if (ok) {
                clockUpdateDivisor = (baseSampleClockFrequency / samplingFrequency);
                REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "ClockUpdateDivisor set to: %d", clockUpdateDivisor);
                float32 samplingF = (baseSampleClockFrequency / (float32) clockUpdateDivisor);
                if ((baseSampleClockFrequency % samplingFrequency) > 0u) {
                    REPORT_ERROR(ErrorManagement::Warning, "SamplingFrequency does not divide exactly DAC base clock (%d). Set to %f", baseSampleClockFrequency,
                                 samplingF);
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "SamplingFrequency must be minor than DAC base clock (20 MHz) and greater than 0");
            }
            set = true;
        }
        else if (data.Read("ClockUpdateDivisor", clockUpdateDivisor)) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "ClockUpdateDivisor set to: %d", clockUpdateDivisor);
            set = true;
        }
        //maintain for backward compatibility
        else if (data.Read("UpdateIntervalCounterPeriodDivisor", clockUpdateDivisor)) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "ClockUpdateDivisor set to: %d", clockUpdateDivisor);
            set = true;
        }
        if (ok) {
            if (!set) {
                clockUpdateDivisor = 100u;
                REPORT_ERROR(ErrorManagement::Information,
                             "Nor SamplingFrequency or ClockUpdateDivisor or UpdateIntervalCounterPeriodDivisor defined. Setting ClockUpdateDivisor=%d",
                             clockUpdateDivisor);
            }
        }
    }

    if (ok) {
        clockUpdateDelay = 0u;

        if (data.Read("ClockUpdateDelay", clockUpdateDelay)) {
        }
        else if (data.Read("UpdateIntervalCounterDelay", clockUpdateDelay)) {
        }
        else {
            clockUpdateDelay = 2u;
            REPORT_ERROR(ErrorManagement::Information, "UpdateIntervalCounterDelay not defined. Using default 2");
        }
        if (ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "ClockUpdateDelay set to: %d", clockUpdateDelay);
            ok = (clockUpdateDelay > 0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The UpdateIntervalCounterDelay shall be > 0");
            }
        }
    }

    //Get individual signal parameters
    uint32 i = 0u;
    if (ok) {
        ok = data.MoveRelative("Signals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }
        //Do not allow to add signals in run-time
        if (ok) {
            ok = signalsDatabase.MoveRelative("Signals");
        }
        if (ok) {
            ok = signalsDatabase.Write("Locked", 1u);
        }
        if (ok) {
            ok = signalsDatabase.MoveToAncestor(1u);
        }
        for (i = 0u; (i < data.GetNumberOfChildren()) && ok; i++) {
            if (data.MoveRelative(data.GetChildName(i))) {
                uint32 channelId;
                if (data.Read("ChannelId", channelId)) {
                    if (headerSize == INIT_HEADER_SIZE) {
                        headerSize = i;
                    }
                    ok = (channelId < NI6368DAC_MAX_CHANNELS);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Invalid ChannelId specified.");
                    }
                    if (ok) {
                        dacEnabled[channelId] = true;
                        numberOfDACsEnabled++;
                        StreamString range;
                        if (data.Read("OutputRange", range)) {
                            if (range == "10") {
                                outputRange[channelId] = XSERIES_OUTPUT_RANGE_10V;
                            }
                            else if (range == "5") {
                                outputRange[channelId] = XSERIES_OUTPUT_RANGE_5V;
                            }
                            else if (range == "APFI0") {
                                outputRange[channelId] = XSERIES_OUTPUT_RANGE_APFI0;
                            }
                            else if (range == "APFI1") {
                                outputRange[channelId] = XSERIES_OUTPUT_RANGE_APFI1;
                            }
                            else {
                                ok = false;
                                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported OutputRange.");
                            }
                        }
                    }
                    if (ok) {
                        float32 gain;
                        if (data.Read("OutputGain", gain)) {
                            outputGain[channelId] = gain;
                        }
                        float32 offset;
                        if (data.Read("OutputOffset", offset)) {
                            outputOffset[channelId] = offset;
                        }
                    }
                }
                else {
                    ok = (headerSize == INIT_HEADER_SIZE);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Header signals must be defined before the channels");
                    }
                }
                if (ok) {
                    ok = data.MoveToAncestor(1u);
                }
            }
            else {
                break;
            }
        }

        if (ok) {
            ok = (headerSize <= 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Header can be composed maximum by 1 signals: status");
            }
        }
    }
    if (ok) {

        const uint32 pxi6363MaxFreq[NI6368DAC_MAX_CHANNELS] = { 2860000u, 2000000u, 1540000u, 1250000u };
        const uint32 pxi6368MaxFreq[NI6368DAC_MAX_CHANNELS] = { 3300000u, 3300000u, 3300000u, 3300000u };

        if (numberOfDACsEnabled > 0u) {
            float32 samplingF = (baseSampleClockFrequency / (float32) clockUpdateDivisor);
            uint8 maxFreqIdx = (numberOfDACsEnabled - 1u);
            uint32 maxFreq = (multiplexed > 0u) ? (pxi6363MaxFreq[maxFreqIdx]) : (pxi6368MaxFreq[maxFreqIdx]);
            ok = (samplingF <= maxFreq);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Sampling frequency (%f) shall be minor than %d", samplingF, maxFreq);
            }
        }

    }
    if (ok) {
        ok = data.MoveToAncestor(1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the parent section");
        }
    }
    return ok;
}

bool NI6368DAC::SetConfiguredDatabase(StructuredDataI &data) {
    uint32 i;
    bool ok = DataSourceI::SetConfiguredDatabase(data);

    if (ok) {
        ok = triggerSet;
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "At least one Trigger signal shall be set.");
        }
    }

    if (ok) {
        ok = (GetNumberOfSignals() == (headerSize + numberOfDACsEnabled));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Number of signals mismatch. All signals must be configured in data source");
        }
    }

    if (ok) {
        if (headerSize > 0u) {
            /*lint -e{9007} no side-effect from GetSignalType*/
            ok = (GetSignalType(0u) == UnsignedInteger32Bit);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The first signal (status) shall be of type UnsignedInteger32Bit");
            }
        }
    }

    if (ok) {
        for (i = 0u; (i < numberOfDACsEnabled) && (ok); i++) {
            ok = (GetSignalType(headerSize + i) == Float32Bit);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "All the DAC signals shall be of type Float32Bit");
            }
        }
    }

    uint32 nOfFunctions = GetNumberOfFunctions();
    uint32 functionIdx;
    //Check that the number of elements for all the signals is consistent
    for (functionIdx = 0u; (functionIdx < nOfFunctions) && (ok); functionIdx++) {
        uint32 signalIdx = 0u;
        uint32 nOfSignals = 0u;
        ok = GetFunctionNumberOfSignals(OutputSignals, functionIdx, nOfSignals);
        for (i = 0u; (i < nOfSignals) && (ok); i++) {
            uint32 nSamples = 0u;
            ok = GetFunctionSignalSamples(OutputSignals, functionIdx, i, nSamples);
            //Is the counter or the time signal?
            StreamString signalAlias;
            if (ok) {
                ok = GetFunctionSignalAlias(OutputSignals, functionIdx, i, signalAlias);
            }
            if (ok) {
                ok = GetSignalIndex(signalIdx, signalAlias.Buffer());
            }
            if (ok) {
                if (signalIdx < headerSize) {
                    ok = (nSamples == 1u);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "The header signal (%d) shall have one and only one sample", signalIdx);
                    }
                }
                else {
                    if (numberOfSamplesT == 0u) {
                        numberOfSamplesT = nSamples;
                    }
                    else {
                        ok = (nSamples == numberOfSamplesT);
                        if (!ok) {
                            REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples must be the same for all channels");
                        }
                    }
                }
            }
        }
    }

    StreamString rootDeviceName;
    //Configure the board
    if (ok) {
        ok = rootDeviceName.Printf("%s.%d", deviceName.Buffer(), boardId);
    }
    if (ok) {
        ok = rootDeviceName.Seek(0LLU);
    }
    if (ok) {
        rootFileDescriptor = open(rootDeviceName.Buffer(), O_RDWR);
        ok = (rootFileDescriptor > -1);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not open device %s", rootDeviceName);
        }
    }

    StreamString fullDeviceName = rootDeviceName;
    //Configure the board
    if (ok) {
        fullDeviceName += ".ao";
        ok = fullDeviceName.Seek(0LLU);
    }
    if (ok) {
        boardFileDescriptor = open(fullDeviceName.Buffer(), O_RDWR);
        ok = (boardFileDescriptor > -1);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not open device %s", fullDeviceName);
        }
    }
    if (ok) {
        bool stopped = (xseries_stop_ao(boardFileDescriptor) == 0);
        if (!stopped) {
            REPORT_ERROR(ErrorManagement::Warning, "Could not stop the device %s while starting", fullDeviceName);
        }
        bool reset = (xseries_reset_ao(boardFileDescriptor) == 0);
        if (!reset) {
            REPORT_ERROR(ErrorManagement::Warning, "Could not reset the device %s while starting", fullDeviceName);
        }
    }
    xseries_ao_conf_t dacConfiguration = xseries_static_ao();
    if (ok) {
        ok = (xseries_set_ao_start_trigger(&dacConfiguration, startTriggerSource, startTriggerPolarity, 1u) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not xseries_set_ao_start_trigger for device %s", fullDeviceName);
        }
    }
    if (ok) {
        ok = (xseries_set_ao_update_counter(&dacConfiguration, updateCounterSource, updateCounterPolarity) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not xseries_set_ao_update_counter for device %s", fullDeviceName);
        }
    }
    if (ok) {
        ok = (xseries_set_ao_update_interval_counter(&dacConfiguration, clockUpdateSource, clockUpdatePolarity, clockUpdateDivisor, clockUpdateDelay) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not xseries_set_ao_update_interval_counter for device %s", fullDeviceName);
        }
    }

    for (i = 0u; (i < NI6368DAC_MAX_CHANNELS) && (ok); i++) {
        if (dacEnabled[i]) {
            ok = (xseries_add_ao_channel(&dacConfiguration, static_cast<uint8_t>(i), outputRange[i]) == 0);
            uint32 ii = i;
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not set configuration for channel %d of device %s", ii, fullDeviceName);
            }
        }
    }
    if (ok) {
        ok = (xseries_load_ao_conf(boardFileDescriptor, dacConfiguration) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not load configuration for device %s", fullDeviceName);
        }
    }

    if (ok) {
        //Required to wait for devices to be available in /dev!
        Sleep::Sec(1.0F);
        for (i = 0u; (i < NI6368DAC_MAX_CHANNELS) && (ok); i++) {
            if (dacEnabled[i]) {
                //Allocate memory
                channelsMemory[i] = new float32[numberOfSamplesT];
                StreamString channelDeviceName;
                uint32 ii = i;
                ok = channelDeviceName.Printf("%s.%d", fullDeviceName.Buffer(), ii);
                if (ok) {
                    ok = channelDeviceName.Seek(0ULL);
                }
                if (ok) {
                    channelsFileDescriptors[i] = open(channelDeviceName.Buffer(), O_RDWR);
                    ok = (channelsFileDescriptors[i] > -1);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Could not open device %s", channelDeviceName);
                    }
                }
            }
        }
    }
    if (ok) {
        ok = (xseries_start_ao(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not start the device %s", fullDeviceName);
        }
    }
    return ok;
}

bool NI6368DAC::Synchronise() {
    uint32 i;
    bool ok = true;
    for (i = 0u; (i < NI6368DAC_MAX_CHANNELS) && (ok); i++) {
        if (dacEnabled[i]) {
            if (channelsMemory[i] != NULL_PTR(float32*)) {
                size_t samplesToWrite = numberOfSamplesT;
                for (uint32 j = 0u; (j < samplesToWrite) && (ok); j++) {
                    channelsMemory[i][j]=((channelsMemory[i][j] * outputGain[i])+outputOffset[i]);
                }
                ssize_t totalWritten = 0;
                while ((samplesToWrite > 0u) && (ok)) {
                    ssize_t samplesWritten = xseries_write_ao(channelsFileDescriptors[i], &(channelsMemory[i][totalWritten]), samplesToWrite);
                    ok = (samplesWritten >= 0);
                    if (ok) {
                        totalWritten += samplesWritten;
                        samplesToWrite -= static_cast<size_t>(samplesWritten);
                    }
                }
            }
        }
    }

    if (headerSize > 0u) {
        xseries_device_status_t devStatus;
        xseries_get_device_status(rootFileDescriptor, &devStatus);
        boardStatus = devStatus.currentBoardStatus;
    }

    return ok;
}

bool NI6368DAC::ReadAOConfiguration(xseries_ao_conf_t *const conf) const {
    bool ok = false;
    if (boardFileDescriptor > 0) {
        ok = (xseries_read_ao_conf(boardFileDescriptor, conf) == 0);
    }
    return ok;
}

CLASS_REGISTER(NI6368DAC, "1.0")
}

