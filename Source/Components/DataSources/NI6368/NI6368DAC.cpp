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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
NI6368DAC::NI6368DAC() :
        DataSourceI() {
    boardId = 0u;
    boardFileDescriptor = -1;
    deviceName = "";
    numberOfDACsEnabled = 0u;
    triggerSet = false;

    startTriggerSource = XSERIES_AO_START_TRIGGER_SW_PULSE;
    startTriggerPolarity = XSERIES_AO_POLARITY_RISING_EDGE;
    updateCounterSource = XSERIES_AO_UPDATE_COUNTER_UI_TC;
    updateCounterPolarity = XSERIES_AO_POLARITY_RISING_EDGE;
    updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB3;
    updateIntervalCounterPolarity = XSERIES_OUTTIMER_POLARITY_RISING_EDGE;
    updateIntervalCounterPeriodDivisor = 100000u;
    updateIntervalCounterDelay = 2u;

    uint32 n;
    for (n = 0u; n < NI6368DAC_MAX_CHANNELS; n++) {
        outputRange[n] = XSERIES_OUTPUT_RANGE_10V;
        dacEnabled[n] = false;
        channelsFileDescriptors[n] = -1;
        numberOfElements[n] = 1u;
        channelsMemory[n] = NULL_PTR(float32 *);
    }
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
        if (channelsMemory[n] != NULL_PTR(float32 *)) {
            delete[] channelsMemory[n];
        }
    }
    if (boardFileDescriptor != -1) {
        close(boardFileDescriptor);
    }
}

bool NI6368DAC::AllocateMemory() {
    return true;
}

uint32 NI6368DAC::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool NI6368DAC::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (signalIdx < (NI6368DAC_MAX_CHANNELS));
    if (ok) {
        signalAddress = channelsMemory[signalIdx];
    }
    return ok;
}

const char8* NI6368DAC::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == OutputSignals) {
        uint32 trigger = 0u;
        if (!data.Read("Trigger", trigger)) {
            trigger = 0u;
        }

        if (trigger == 1u) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
            triggerSet = true;
        }
        else {
            brokerName = "MemoryMapOutputBroker";
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with InputSignals");
    }
    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns false irrespectively of the input parameters.*/
bool NI6368DAC::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

bool NI6368DAC::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    //Check if there is a Trigger signal for this function.
    uint32 functionIdx = 0u;
    uint32 nOfFunctionSignals = 0u;
    uint32 i;
    bool triggerGAM = false;
    bool ok = GetFunctionIndex(functionIdx, functionName);

    if (ok) {
        ok = GetFunctionNumberOfSignals(OutputSignals, functionIdx, nOfFunctionSignals);
    }
    uint32 trigger = 0u;
    for (i = 0u; (i < nOfFunctionSignals) && (ok) && (!triggerGAM); i++) {
        ok = GetFunctionSignalTrigger(OutputSignals, functionIdx, i, trigger);
        triggerGAM = (trigger == 1u);
    }
    if ((ok) && (triggerGAM)) {
        ReferenceT<MemoryMapSynchronisedOutputBroker> broker("MemoryMapSynchronisedOutputBroker");
        ok = broker.IsValid();

        if (ok) {
            ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
        }
        if (ok) {
            ok = outputBrokers.Insert(broker);
        }
        //Must also add the signals which are not triggering but that belong to the same GAM...
        if (ok) {
            if (nOfFunctionSignals > 1u) {
                ReferenceT<MemoryMapOutputBroker> brokerNotSync("MemoryMapOutputBroker");
                ok = brokerNotSync.IsValid();
                if (ok) {
                    ok = brokerNotSync->Init(OutputSignals, *this, functionName, gamMemPtr);
                }
                if (ok) {
                    ok = outputBrokers.Insert(brokerNotSync);
                }
            }
        }
    }
    else {
        ReferenceT<MemoryMapOutputBroker> brokerNotSync("MemoryMapOutputBroker");
        ok = brokerNotSync.IsValid();
        if (ok) {
            ok = brokerNotSync->Init(OutputSignals, *this, functionName, gamMemPtr);
        }
        if (ok) {
            ok = outputBrokers.Insert(brokerNotSync);
        }
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP irrespectively of the input parameters.*/
bool NI6368DAC::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool NI6368DAC::Initialise(StructuredDataI& data) {
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
    if (ok) {
        StreamString startTriggerSourceStr;
        ok = data.Read("StartTriggerSource", startTriggerSourceStr);
        if (ok) {
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
            REPORT_ERROR(ErrorManagement::ParametersError, "The StartTriggerSource shall be specified");
        }
    }
    if (ok) {
        StreamString updateCounterSourceStr;
        ok = data.Read("UpdateCounterSource", updateCounterSourceStr);
        if (ok) {
            if (updateCounterSourceStr == "UI_TC") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_UI_TC;
            }
            else if (updateCounterSourceStr == "PFI0") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI0;
            }
            else if (updateCounterSourceStr == "PFI1") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI1;
            }
            else if (updateCounterSourceStr == "PFI2") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI2;
            }
            else if (updateCounterSourceStr == "PFI3") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI3;
            }
            else if (updateCounterSourceStr == "PFI4") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI4;
            }
            else if (updateCounterSourceStr == "PFI5") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI5;
            }
            else if (updateCounterSourceStr == "PFI6") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI6;
            }
            else if (updateCounterSourceStr == "PFI7") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI7;
            }
            else if (updateCounterSourceStr == "PFI8") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI8;
            }
            else if (updateCounterSourceStr == "PFI9") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI9;
            }
            else if (updateCounterSourceStr == "RTSI0") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI0;
            }
            else if (updateCounterSourceStr == "RTSI1") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI1;
            }
            else if (updateCounterSourceStr == "RTSI2") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI2;
            }
            else if (updateCounterSourceStr == "RTSI3") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI3;
            }
            else if (updateCounterSourceStr == "RTSI4") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI4;
            }
            else if (updateCounterSourceStr == "RTSI5") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI5;
            }
            else if (updateCounterSourceStr == "RTSI6") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI6;
            }
            else if (updateCounterSourceStr == "G0_OUT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G0_OUT;
            }
            else if (updateCounterSourceStr == "G1_OUT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G1_OUT;
            }
            else if (updateCounterSourceStr == "STAR_TRIGGER") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_STAR_TRIGGER;
            }
            else if (updateCounterSourceStr == "PFI10") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI10;
            }
            else if (updateCounterSourceStr == "PFI11") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI11;
            }
            else if (updateCounterSourceStr == "PFI12") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI12;
            }
            else if (updateCounterSourceStr == "PFI13") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI13;
            }
            else if (updateCounterSourceStr == "PFI14") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI14;
            }
            else if (updateCounterSourceStr == "PFI15") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PFI15;
            }
            else if (updateCounterSourceStr == "RTSI7") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_RTSI7;
            }
            else if (updateCounterSourceStr == "G2_OUT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G2_OUT;
            }
            else if (updateCounterSourceStr == "G3_OUT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G3_OUT;
            }
            else if (updateCounterSourceStr == "ANALOG_TRIGGER") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_ANALOG_TRIGGER;
            }
            else if (updateCounterSourceStr == "LOW") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_LOW;
            }
            else if (updateCounterSourceStr == "PXIE_DSTARA") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PXIE_DSTARA;
            }
            else if (updateCounterSourceStr == "PXIE_DSTARB") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_PXIE_DSTARB;
            }
            else if (updateCounterSourceStr == "DIO_CHGDETECT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_DIO_CHGDETECT;
            }
            else if (updateCounterSourceStr == "G0_SAMPLECLK") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G0_SAMPLECLK;
            }
            else if (updateCounterSourceStr == "G1_SAMPLECLK") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G1_SAMPLECLK;
            }
            else if (updateCounterSourceStr == "G2_SAMPLECLK") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G2_SAMPLECLK;
            }
            else if (updateCounterSourceStr == "G3_SAMPLECLK") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_G3_SAMPLECLK;
            }
            else if (updateCounterSourceStr == "AI_CONVERT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_AI_CONVERT;
            }
            else if (updateCounterSourceStr == "AI_START") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_AI_START;
            }
            else if (updateCounterSourceStr == "DI_CONVERT") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_DI_CONVERT;
            }
            else if (updateCounterSourceStr == "DO_UPDATE") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_DO_UPDATE;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA0") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA0;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA1") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA1;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA2") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA2;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA3") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA3;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA4") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA4;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA5") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA5;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA6") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA6;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA7") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_INTTRIGGERA7;
            }
            else if (updateCounterSourceStr == "AUTOUPDATE") {
                updateCounterSource = XSERIES_AO_UPDATE_COUNTER_AUTOUPDATE;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid UpdateCounterSource specified");
                ok = false;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "The UpdateCounterSource shall be specified");
        }
    }
    if (ok) {
        StreamString updateCounterPolarityStr;
        ok = data.Read("UpdateCounterPolarity", updateCounterPolarityStr);
        if (ok) {
            if (updateCounterPolarityStr == "RISING_EDGE") {
                updateCounterPolarity = XSERIES_AO_POLARITY_RISING_EDGE;
            }
            else if (updateCounterPolarityStr == "FALLING_EDGE") {
                updateCounterPolarity = XSERIES_AO_POLARITY_FALLING_EDGE;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid UpdateCounterPolarity specified");
                ok = false;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "The UpdateCounterPolarity shall be specified");
        }
    }
    if (ok) {
        StreamString updateIntervalCounterSourceStr;
        ok = data.Read("UpdateIntervalCounterSource", updateIntervalCounterSourceStr);
        if (ok) {
            if (updateIntervalCounterSourceStr == "TB3") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB3;
            }
            else if (updateIntervalCounterSourceStr == "PFI0") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI0;
            }
            else if (updateIntervalCounterSourceStr == "PFI1") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI1;
            }
            else if (updateIntervalCounterSourceStr == "PFI2") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI2;
            }
            else if (updateIntervalCounterSourceStr == "PFI3") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI3;
            }
            else if (updateIntervalCounterSourceStr == "PFI4") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI4;
            }
            else if (updateIntervalCounterSourceStr == "PFI5") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI5;
            }
            else if (updateIntervalCounterSourceStr == "PFI6") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI6;
            }
            else if (updateIntervalCounterSourceStr == "PFI7") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI7;
            }
            else if (updateIntervalCounterSourceStr == "PFI8") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI8;
            }
            else if (updateIntervalCounterSourceStr == "PFI9") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI9;
            }
            else if (updateIntervalCounterSourceStr == "RTSI0") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI0;
            }
            else if (updateIntervalCounterSourceStr == "RTSI1") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI1;
            }
            else if (updateIntervalCounterSourceStr == "RTSI2") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI2;
            }
            else if (updateIntervalCounterSourceStr == "RTSI3") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI3;
            }
            else if (updateIntervalCounterSourceStr == "RTSI4") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI4;
            }
            else if (updateIntervalCounterSourceStr == "RTSI5") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI5;
            }
            else if (updateIntervalCounterSourceStr == "RTSI6") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI6;
            }
            else if (updateIntervalCounterSourceStr == "DSTARA") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_DSTARA;
            }
            else if (updateIntervalCounterSourceStr == "TB2") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB2;
            }
            else if (updateIntervalCounterSourceStr == "STAR_TRIGGER") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_STAR_TRIGGER;
            }
            else if (updateIntervalCounterSourceStr == "PFI10") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI10;
            }
            else if (updateIntervalCounterSourceStr == "PFI11") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI11;
            }
            else if (updateIntervalCounterSourceStr == "PFI12") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI12;
            }
            else if (updateIntervalCounterSourceStr == "PFI13") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI13;
            }
            else if (updateIntervalCounterSourceStr == "PFI14") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI14;
            }
            else if (updateIntervalCounterSourceStr == "PFI15") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PFI15;
            }
            else if (updateIntervalCounterSourceStr == "RTSI7") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_RTSI7;
            }
            else if (updateIntervalCounterSourceStr == "TB1") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB1;
            }
            else if (updateIntervalCounterSourceStr == "PXI_CLK10") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_PXI_CLK10;
            }
            else if (updateIntervalCounterSourceStr == "ANALOG_TRIGGER") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_ANALOG_TRIGGER;
            }
            else if (updateIntervalCounterSourceStr == "DSTARB") {
                updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_DSTARB;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid UpdateIntervalCounterSource specified");
                ok = false;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "The UpdateIntervalCounterSource shall be specified");
        }
    }
    if (ok) {
        StreamString updateIntervalCounterPolarityStr;
        ok = data.Read("UpdateIntervalCounterPolarity", updateIntervalCounterPolarityStr);
        if (ok) {
            if (updateIntervalCounterPolarityStr == "RISING_EDGE") {
                updateIntervalCounterPolarity = XSERIES_OUTTIMER_POLARITY_RISING_EDGE;
            }
            else if (updateIntervalCounterPolarityStr == "FALLING_EDGE") {
                updateIntervalCounterPolarity = XSERIES_OUTTIMER_POLARITY_FALLING_EDGE;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid UpdateIntervalCounterPolarity specified");
                ok = false;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "The UpdateIntervalCounterPolarity shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("UpdateIntervalCounterPeriodDivisor", updateIntervalCounterPeriodDivisor);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The UpdateIntervalCounterPeriodDivisor shall be specified");
        }
    }
    if (ok) {
        ok = (updateIntervalCounterPeriodDivisor > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The UpdateIntervalCounterPeriodDivisor shall be > 0");
        }
    }
    if (ok) {
        ok = data.Read("UpdateIntervalCounterDelay", updateIntervalCounterDelay);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The UpdateIntervalCounterDelay shall be specified");
        }
    }
    if (ok) {
        ok = (updateIntervalCounterDelay > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The UpdateIntervalCounterDelay shall be > 0");
        }
    }

    //Get individual signal parameters
    uint32 i = 0u;
    if (ok) {
        ok = data.MoveRelative("Signals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }
        if (ok) {
            //Do not allow to add signals in run-time
            ok = data.Write("Locked", 1);
        }
        uint32 maxChannelId = 0u;
        while ((i < NI6368DAC_MAX_CHANNELS) && (ok)) {
            if (data.MoveRelative(data.GetChildName(i))) {
                uint32 channelId;
                if (data.Read("ChannelId", channelId)) {
                    ok = (channelId < NI6368DAC_MAX_CHANNELS);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Invalid ChannelId specified.");
                    }
                    if (ok) {
                        //Channel id must be specified monotonically increasing otherwise there will be a mismatch between the signal memory and the order by which the channels are opened.
                        if (maxChannelId != 0u) {
                            ok = (channelId > maxChannelId);
                        }
                        if (!ok) {
                            REPORT_ERROR(ErrorManagement::ParametersError, "ChannelId must be monotonically increasing.");
                        }
                    }
                    if (ok) {
                        maxChannelId = channelId;
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
                }
                if (ok) {
                    ok = data.MoveToAncestor(1u);
                }
                i++;
            }
            else {
                break;
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

bool NI6368DAC::SetConfiguredDatabase(StructuredDataI& data) {
    uint32 i;
    bool ok = DataSourceI::SetConfiguredDatabase(data);

    if (ok) {
        ok = triggerSet;
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "At least one Trigger signal shall be set.");
    }
    if (ok) {
        uint32 j = 0u;
        for (i = 0u; (i < NI6368DAC_MAX_CHANNELS) && (ok); i++) {
            if (dacEnabled[i]) {
                ok = (GetSignalType(j) == Float32Bit);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "All the DAC signals shall be of type Float32Bit");
                }
                uint32 nElements = 0u;
                if (ok) {
                    ok = GetSignalNumberOfElements(j, nElements);
                    numberOfElements[i] = nElements;
                }
                j++;
            }
        }
    }

    uint32 nOfFunctions = GetNumberOfFunctions();
    uint32 functionIdx;
    //Check that the number of elements for all the signals is consistent
    for (functionIdx = 0u; (functionIdx < nOfFunctions) && (ok); functionIdx++) {
        uint32 nOfSignals = 0u;
        ok = GetFunctionNumberOfSignals(OutputSignals, functionIdx, nOfSignals);

        for (i = 0u; (i < nOfSignals) && (ok); i++) {
            uint32 nSamples = 0u;
            ok = GetFunctionSignalSamples(OutputSignals, functionIdx, i, nSamples);
            if (ok) {
                ok = (nSamples == 1u);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "All the DAC signals shall have at most one sample");
            }
        }
    }

    StreamString fullDeviceName;
//Configure the board
    if (ok) {
        ok = fullDeviceName.Printf("%s.%d.ao", deviceName.Buffer(), boardId);
    }
    if (ok) {
        ok = fullDeviceName.Seek(0LLU);
    }
    if (ok) {
        boardFileDescriptor = open(fullDeviceName.Buffer(), O_RDWR);
        ok = (boardFileDescriptor > -1);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not open device %s", fullDeviceName)
        }
    }
    if (ok) {
        bool stopped = (xseries_stop_ao(boardFileDescriptor) == 0);
        if (!stopped) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Could not stop the device %s while starting", fullDeviceName)
        }
        bool reset = (xseries_reset_ao(boardFileDescriptor) == 0);
        if (!reset) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Could not reset the device %s while starting", fullDeviceName)
        }
    }
    xseries_ao_conf_t dacConfiguration = xseries_static_ao();
    if (ok) {
        ok = (xseries_set_ao_start_trigger(&dacConfiguration, startTriggerSource, startTriggerPolarity, 1u) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not xseries_set_ao_start_trigger for device %s", fullDeviceName)
        }
    }
    if (ok) {
        ok = (xseries_set_ao_update_counter(&dacConfiguration, updateCounterSource, updateCounterPolarity) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not xseries_set_ao_update_counter for device %s", fullDeviceName)
        }
    }
    if (ok) {
        ok = (xseries_set_ao_update_interval_counter(&dacConfiguration, updateIntervalCounterSource, updateIntervalCounterPolarity,
                                                     updateIntervalCounterPeriodDivisor, updateIntervalCounterDelay) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not xseries_set_ao_update_interval_counter for device %s", fullDeviceName)
        }
    }

    for (i = 0u; (i < NI6368DAC_MAX_CHANNELS) && (ok); i++) {
        if (dacEnabled[i]) {
            ok = (xseries_add_ao_channel(&dacConfiguration, static_cast<uint8_t>(i), outputRange[i]) == 0);
            uint32 ii = i;
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set configuration for channel %d of device %s", ii, fullDeviceName)
            }
        }
    }
    if (ok) {
        ok = (xseries_load_ao_conf(boardFileDescriptor, dacConfiguration) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not load configuration for device %s", fullDeviceName)
        }
    }

    if (ok) {
        //Required to wait for devices to be available in /dev!
        Sleep::Sec(1.0);
        for (i = 0u; (i < NI6368DAC_MAX_CHANNELS) && (ok); i++) {
            if (dacEnabled[i]) {
                //Allocate memory
                channelsMemory[i] = new float32[numberOfElements[i]];
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
                        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not open device %s", channelDeviceName)
                    }
                }
            }
        }
    }
    if (ok) {
        ok = (xseries_start_ao(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not start the device %s", fullDeviceName)
        }
    }
    return ok;
}

bool NI6368DAC::Synchronise() {
    uint32 i;
    bool ok = true;
    for (i = 0u; (i < NI6368DAC_MAX_CHANNELS) && (ok); i++) {
        if (dacEnabled[i]) {
            size_t samplesToWrite = numberOfElements[i];
            if (channelsMemory[i] != NULL_PTR(float32 *)) {
                while ((samplesToWrite > 0u) && (ok)) {
                    ssize_t samplesWritten = xseries_write_ao(channelsFileDescriptors[i], channelsMemory[i], samplesToWrite);
                    if (samplesWritten < 0) {
                        ok = false;
                    }
                    else {
                        samplesToWrite -= static_cast<size_t>(samplesWritten);
                    }
                }
            }
        }
    }
    return ok;
}

bool NI6368DAC::ReadAOConfiguration(xseries_ao_conf_t * const conf) const {
    bool ok = false;
    if (boardFileDescriptor > 0) {
        ok = (xseries_read_ao_conf(boardFileDescriptor, conf) == 0);
    }
    return ok;
}

CLASS_REGISTER(NI6368DAC, "1.0")
}

