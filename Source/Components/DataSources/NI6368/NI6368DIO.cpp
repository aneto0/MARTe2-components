/**
 * @file NI6368DIO.cpp
 * @brief Source file for class NI6368DIO
 * @date 30/01/2017
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
 * the class NI6368DIO (public, protected, and private). Be aware that some
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
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"

#include "NI6368DIO.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
NI6368DIO::NI6368DIO() :
        DataSourceI() {

    clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTERNAL;
    clockSamplePolarity = XSERIES_DI_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE;
    scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_TB3;
    scanIntervalCounterDelay = 0u;
    scanIntervalCounterPeriod = 0u;

    startTriggerSource = XSERIES_DO_START_TRIGGER_SW_PULSE;
    startTriggerPolarity = XSERIES_DO_POLARITY_RISING_EDGE;
    updateCounterSource = XSERIES_DO_UPDATE_COUNTER_UI_TC;
    updateCounterPolarity = XSERIES_DO_POLARITY_RISING_EDGE;
    updateIntervalCounterSource = XSERIES_OUTTIMER_UPDATE_INTERVAL_COUNTER_TB3;
    updateIntervalCounterPolarity = XSERIES_OUTTIMER_POLARITY_RISING_EDGE;
    updateIntervalCounterPeriodDivisor = 100000u;
    updateIntervalCounterDelay = 2u;

    boardId = 0u;
    boardFileDescriptor = -1;
    deviceName = "";
    outputsEnabled = false;
    inputsEnabled = false;
    inputPortMask = 0u;
    outputPortMask = 0u;
    portValue = 0u;
}

NI6368DIO::~NI6368DIO() {
    if (boardFileDescriptor != -1) {
        bool ok = (xseries_stop_di(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not stop digital input on device");
        }
        if (ok) {
            ok = (xseries_stop_do(boardFileDescriptor) == 0);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not stop digital output on device");
            }
        }
        close(boardFileDescriptor);
    }
}

bool NI6368DIO::AllocateMemory() {
    return true;
}

uint32 NI6368DIO::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool NI6368DIO::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    signalAddress = &portValue;
    return true;
}

const char8* NI6368DIO::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == OutputSignals) {
        brokerName = "MemoryMapSynchronisedOutputBroker";
        outputsEnabled = true;
    }
    else {
        brokerName = "MemoryMapSynchronisedInputBroker";
        inputsEnabled = true;
    }
    return brokerName;
}

bool NI6368DIO::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    ReferenceT<MemoryMapSynchronisedInputBroker> broker("MemoryMapSynchronisedInputBroker");
    bool ok = broker.IsValid();

    if (ok) {
        ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ok) {
        ok = inputBrokers.Insert(broker);
    }

    return ok;
}

bool NI6368DIO::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    ReferenceT<MemoryMapSynchronisedOutputBroker> broker("MemoryMapSynchronisedOutputBroker");
    bool ok = broker.IsValid();

    if (ok) {
        ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ok) {
        ok = outputBrokers.Insert(broker);
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP irrespectively of the input parameters.*/
bool NI6368DIO::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool NI6368DIO::Initialise(StructuredDataI& data) {
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
        ok = data.Read("InputPortMask", inputPortMask);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The InputPortMask shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("OutputPortMask", outputPortMask);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The OutputPortMask shall be specified");
        }
    }
    StreamString clockSampleSourceStr;
    if (ok) {
        ok = data.Read("ClockSampleSource", clockSampleSourceStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ClockSampleSource shall be specified");
        }
    }
    if (ok) {
        if (clockSampleSourceStr == "INTERNAL") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTERNAL;
        }
        else if (clockSampleSourceStr == "PFI0") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI0;
        }
        else if (clockSampleSourceStr == "PFI1") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI1;
        }
        else if (clockSampleSourceStr == "PFI2") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI2;
        }
        else if (clockSampleSourceStr == "PFI3") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI3;
        }
        else if (clockSampleSourceStr == "PFI4") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI4;
        }
        else if (clockSampleSourceStr == "PFI5") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI5;
        }
        else if (clockSampleSourceStr == "PFI6") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI6;
        }
        else if (clockSampleSourceStr == "PFI7") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI7;
        }
        else if (clockSampleSourceStr == "PFI8") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI8;
        }
        else if (clockSampleSourceStr == "PFI9") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI9;
        }
        else if (clockSampleSourceStr == "PFI10") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI10;
        }
        else if (clockSampleSourceStr == "PFI11") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI11;
        }
        else if (clockSampleSourceStr == "PFI12") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI12;
        }
        else if (clockSampleSourceStr == "PFI13") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI13;
        }
        else if (clockSampleSourceStr == "PFI14") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI14;
        }
        else if (clockSampleSourceStr == "PFI15") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PFI15;
        }
        else if (clockSampleSourceStr == "RTSI0") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI0;
        }
        else if (clockSampleSourceStr == "RTSI1") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI1;
        }
        else if (clockSampleSourceStr == "RTSI2") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI2;
        }
        else if (clockSampleSourceStr == "RTSI3") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI3;
        }
        else if (clockSampleSourceStr == "RTSI4") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI4;
        }
        else if (clockSampleSourceStr == "RTSI5") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI5;
        }
        else if (clockSampleSourceStr == "RTSI6") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI6;
        }
        else if (clockSampleSourceStr == "RTSI7") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_RTSI7;
        }
        else if (clockSampleSourceStr == "DIO_CHGDETECT") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_DIO_CHGDETECT;
        }
        else if (clockSampleSourceStr == "G0_OUT") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_G0_OUT;
        }
        else if (clockSampleSourceStr == "G1_OUT") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_G1_OUT;
        }
        else if (clockSampleSourceStr == "G2_OUT") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_G2_OUT;
        }
        else if (clockSampleSourceStr == "G3_OUT") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_G3_OUT;
        }
        else if (clockSampleSourceStr == "STAR_TRIGGER") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_STAR_TRIGGER;
        }
        else if (clockSampleSourceStr == "ATRIG") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_ATRIG;
        }
        else if (clockSampleSourceStr == "LOW") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_LOW;
        }
        else if (clockSampleSourceStr == "PXIE_DSTARA") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PXIE_DSTARA;
        }
        else if (clockSampleSourceStr == "PXIE_DSTARB") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_PXIE_DSTARB;
        }
        else if (clockSampleSourceStr == "G0_SAMPLECLK") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_G0_SAMPLECLK;
        }
        else if (clockSampleSourceStr == "G1_SAMPLECLK") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_G1_SAMPLECLK;
        }
        else if (clockSampleSourceStr == "G2_SAMPLECLK") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_G2_SAMPLECLK;
        }
        else if (clockSampleSourceStr == "G3_SAMPLECLK") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_G3_SAMPLECLK;
        }
        else if (clockSampleSourceStr == "AI_CONVERT") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_AI_CONVERT;
        }
        else if (clockSampleSourceStr == "AO_UPDATE") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_DO_UPDATE;
        }
        else if (clockSampleSourceStr == "DO_UPDATE") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_DO_UPDATE;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA0") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA0;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA1") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA1;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA2") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA2;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA3") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA3;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA4") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA4;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA5") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA5;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA6") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA6;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA7") {
            clockSampleSource = XSERIES_DI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA7;
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockSampleSource");
        }
    }
    StreamString clockSamplePolarityStr;
    if (ok) {
        ok = data.Read("ClockSamplePolarity", clockSamplePolarityStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ClockSamplePolarity shall be specified");
        }
    }
    if (ok) {
        if (clockSamplePolarityStr == "ACTIVE_HIGH_OR_RISING_EDGE") {
            clockSamplePolarity = XSERIES_DI_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE;
        }
        else if (clockSamplePolarityStr == "ACTIVE_LOW_OR_FALLING_EDGE") {
            clockSamplePolarity = XSERIES_DI_POLARITY_ACTIVE_LOW_OR_FALLING_EDGE;
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockSamplePolarity");
        }
    }
    StreamString scanIntervalCounterSourceStr;
    if (ok) {
        ok = data.Read("ScanIntervalCounterSource", scanIntervalCounterSourceStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ScanIntervalCounterSource shall be specified");
        }
    }
    if (ok) {
        if (scanIntervalCounterSourceStr == "COUNTER_TB3") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_TB3;
        }
        else if (scanIntervalCounterSourceStr == "COUNTER_TB2") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_TB2;
        }
        else if (scanIntervalCounterSourceStr == "COUNTER_TB1") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_TB1;
        }
        else if (scanIntervalCounterSourceStr == "PFI0") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI0;
        }
        else if (scanIntervalCounterSourceStr == "PFI1") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI1;
        }
        else if (scanIntervalCounterSourceStr == "PFI2") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI2;
        }
        else if (scanIntervalCounterSourceStr == "PFI3") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI3;
        }
        else if (scanIntervalCounterSourceStr == "PFI4") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI4;
        }
        else if (scanIntervalCounterSourceStr == "PFI5") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI5;
        }
        else if (scanIntervalCounterSourceStr == "PFI6") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI6;
        }
        else if (scanIntervalCounterSourceStr == "PFI7") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI7;
        }
        else if (scanIntervalCounterSourceStr == "PFI8") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI8;
        }
        else if (scanIntervalCounterSourceStr == "PFI9") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI9;
        }
        else if (scanIntervalCounterSourceStr == "PFI10") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI10;
        }
        else if (scanIntervalCounterSourceStr == "PFI11") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI11;
        }
        else if (scanIntervalCounterSourceStr == "PFI12") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI12;
        }
        else if (scanIntervalCounterSourceStr == "PFI13") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI13;
        }
        else if (scanIntervalCounterSourceStr == "PFI14") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI14;
        }
        else if (scanIntervalCounterSourceStr == "PFI15") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PFI15;
        }
        else if (scanIntervalCounterSourceStr == "RTSI0") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_RTSI0;
        }
        else if (scanIntervalCounterSourceStr == "RTSI1") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_RTSI1;
        }
        else if (scanIntervalCounterSourceStr == "RTSI2") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_RTSI2;
        }
        else if (scanIntervalCounterSourceStr == "RTSI3") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_RTSI3;
        }
        else if (scanIntervalCounterSourceStr == "RTSI4") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_RTSI4;
        }
        else if (scanIntervalCounterSourceStr == "RTSI5") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_RTSI5;
        }
        else if (scanIntervalCounterSourceStr == "RTSI6") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_RTSI6;
        }
        else if (scanIntervalCounterSourceStr == "RTSI7") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_RTSI7;
        }
        else if (scanIntervalCounterSourceStr == "PXI_CLK10") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_PXI_CLK10;
        }
        else if (scanIntervalCounterSourceStr == "STAR_TRIGGER") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_STAR_TRIGGER;
        }
        else if (scanIntervalCounterSourceStr == "ANALOG_TRIGGER") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_ANALOG_TRIGGER;
        }
        else if (scanIntervalCounterSourceStr == "DSTARA") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_DSTARA;
        }
        else if (scanIntervalCounterSourceStr == "DSTARB") {
            scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_DSTARB;
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ScanIntervalCounterSource");
        }
    }
    if (ok) {
        ok = data.Read("ScanIntervalCounterPeriod", scanIntervalCounterPeriod);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ScanIntervalCounterPeriod shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("ScanIntervalCounterDelay", scanIntervalCounterDelay);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ScanIntervalCounterDelay shall be specified");
        }
    }

    if (ok) {
        StreamString startTriggerSourceStr;
        ok = data.Read("StartTriggerSource", startTriggerSourceStr);
        if (ok) {
            if (startTriggerSourceStr == "SW_PULSE") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_SW_PULSE;
            }
            else if (startTriggerSourceStr == "PFI0") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI0;
            }
            else if (startTriggerSourceStr == "PFI1") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI1;
            }
            else if (startTriggerSourceStr == "PFI2") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI2;
            }
            else if (startTriggerSourceStr == "PFI3") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI3;
            }
            else if (startTriggerSourceStr == "PFI4") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI4;
            }
            else if (startTriggerSourceStr == "PFI5") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI5;
            }
            else if (startTriggerSourceStr == "PFI6") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI6;
            }
            else if (startTriggerSourceStr == "PFI7") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI7;
            }
            else if (startTriggerSourceStr == "PFI8") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI8;
            }
            else if (startTriggerSourceStr == "PFI9") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI9;
            }
            else if (startTriggerSourceStr == "RTSI0") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_RTSI0;
            }
            else if (startTriggerSourceStr == "RTSI1") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_RTSI1;
            }
            else if (startTriggerSourceStr == "RTSI2") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_RTSI2;
            }
            else if (startTriggerSourceStr == "RTSI3") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_RTSI3;
            }
            else if (startTriggerSourceStr == "RTSI4") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_RTSI4;
            }
            else if (startTriggerSourceStr == "RTSI5") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_RTSI5;
            }
            else if (startTriggerSourceStr == "RTSI6") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_RTSI6;
            }
            else if (startTriggerSourceStr == "AI_START2") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_AI_START2;
            }
            else if (startTriggerSourceStr == "AI_START1") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_AI_START1;
            }
            else if (startTriggerSourceStr == "STAR_TRIGGER") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_STAR_TRIGGER;
            }
            else if (startTriggerSourceStr == "PFI10") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI10;
            }
            else if (startTriggerSourceStr == "PFI11") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI11;
            }
            else if (startTriggerSourceStr == "PFI12") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI12;
            }
            else if (startTriggerSourceStr == "PFI13") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI13;
            }
            else if (startTriggerSourceStr == "PFI14") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI14;
            }
            else if (startTriggerSourceStr == "PFI15") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PFI15;
            }
            else if (startTriggerSourceStr == "RTSI7") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_RTSI7;
            }
            else if (startTriggerSourceStr == "PXIE_DSTARA") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PXIE_DSTARA;
            }
            else if (startTriggerSourceStr == "PXIE_DSTARB") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_PXIE_DSTARB;
            }
            else if (startTriggerSourceStr == "ANALOG_TRIGGER") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_ANALOG_TRIGGER;
            }
            else if (startTriggerSourceStr == "LOW") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_LOW;
            }
            else if (startTriggerSourceStr == "G0_OUT") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_G0_OUT;
            }
            else if (startTriggerSourceStr == "G1_OUT") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_G1_OUT;
            }
            else if (startTriggerSourceStr == "G2_OUT") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_G2_OUT;
            }
            else if (startTriggerSourceStr == "G3_OUT") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_G3_OUT;
            }
            else if (startTriggerSourceStr == "DIO_CHGDETECT") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_DIO_CHGDETECT;
            }
            else if (startTriggerSourceStr == "DI_START1") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_DI_START1;
            }
            else if (startTriggerSourceStr == "DI_START2") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_DI_START2;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA0") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_INTTRIGGERA0;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA1") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_INTTRIGGERA1;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA2") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_INTTRIGGERA2;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA3") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_INTTRIGGERA3;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA4") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_INTTRIGGERA4;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA5") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_INTTRIGGERA5;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA6") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_INTTRIGGERA6;
            }
            else if (startTriggerSourceStr == "INTTRIGGERA7") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_INTTRIGGERA7;
            }
            else if (startTriggerSourceStr == "FIFOCONDITION") {
                startTriggerSource = XSERIES_DO_START_TRIGGER_FIFOCONDITION;
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
        StreamString startTriggerPolarityStr;
        ok = data.Read("StartTriggerPolarity", startTriggerPolarityStr);
        if (ok) {
            if (startTriggerPolarityStr == "RISING_EDGE") {
                startTriggerPolarity = XSERIES_DO_POLARITY_RISING_EDGE;
            }
            else if (startTriggerPolarityStr == "FALLING_EDGE") {
                startTriggerPolarity = XSERIES_DO_POLARITY_FALLING_EDGE;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Invalid StartTriggerPolarity specified");
                ok = false;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "The StartTriggerPolarity shall be specified");
        }
    }
    if (ok) {
        StreamString updateCounterSourceStr;
        ok = data.Read("UpdateCounterSource", updateCounterSourceStr);
        if (ok) {
            if (updateCounterSourceStr == "UI_TC") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_UI_TC;
            }
            else if (updateCounterSourceStr == "PFI0") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI0;
            }
            else if (updateCounterSourceStr == "PFI1") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI1;
            }
            else if (updateCounterSourceStr == "PFI2") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI2;
            }
            else if (updateCounterSourceStr == "PFI3") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI3;
            }
            else if (updateCounterSourceStr == "PFI4") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI4;
            }
            else if (updateCounterSourceStr == "PFI5") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI5;
            }
            else if (updateCounterSourceStr == "PFI6") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI6;
            }
            else if (updateCounterSourceStr == "PFI7") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI7;
            }
            else if (updateCounterSourceStr == "PFI8") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI8;
            }
            else if (updateCounterSourceStr == "PFI9") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI9;
            }
            else if (updateCounterSourceStr == "RTSI0") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_RTSI0;
            }
            else if (updateCounterSourceStr == "RTSI1") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_RTSI1;
            }
            else if (updateCounterSourceStr == "RTSI2") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_RTSI2;
            }
            else if (updateCounterSourceStr == "RTSI3") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_RTSI3;
            }
            else if (updateCounterSourceStr == "RTSI4") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_RTSI4;
            }
            else if (updateCounterSourceStr == "RTSI5") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_RTSI5;
            }
            else if (updateCounterSourceStr == "RTSI6") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_RTSI6;
            }
            else if (updateCounterSourceStr == "G0_OUT") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_G0_OUT;
            }
            else if (updateCounterSourceStr == "G1_OUT") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_G1_OUT;
            }
            else if (updateCounterSourceStr == "STAR_TRIGGER") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_STAR_TRIGGER;
            }
            else if (updateCounterSourceStr == "PFI10") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI10;
            }
            else if (updateCounterSourceStr == "PFI11") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI11;
            }
            else if (updateCounterSourceStr == "PFI12") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI12;
            }
            else if (updateCounterSourceStr == "PFI13") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI13;
            }
            else if (updateCounterSourceStr == "PFI14") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI14;
            }
            else if (updateCounterSourceStr == "PFI15") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PFI15;
            }
            else if (updateCounterSourceStr == "RTSI7") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_RTSI7;
            }
            else if (updateCounterSourceStr == "G2_OUT") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_G2_OUT;
            }
            else if (updateCounterSourceStr == "G3_OUT") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_G3_OUT;
            }
            else if (updateCounterSourceStr == "ANALOG_TRIGGER") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_ANALOG_TRIGGER;
            }
            else if (updateCounterSourceStr == "LOW") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_LOW;
            }
            else if (updateCounterSourceStr == "PXIE_DSTARA") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PXIE_DSTARA;
            }
            else if (updateCounterSourceStr == "PXIE_DSTARB") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_PXIE_DSTARB;
            }
            else if (updateCounterSourceStr == "DIO_CHGDETECT") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_DIO_CHGDETECT;
            }
            else if (updateCounterSourceStr == "G0_SAMPLECLK") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_G0_SAMPLECLK;
            }
            else if (updateCounterSourceStr == "G1_SAMPLECLK") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_G1_SAMPLECLK;
            }
            else if (updateCounterSourceStr == "G2_SAMPLECLK") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_G2_SAMPLECLK;
            }
            else if (updateCounterSourceStr == "G3_SAMPLECLK") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_G3_SAMPLECLK;
            }
            else if (updateCounterSourceStr == "AI_CONVERT") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_AI_CONVERT;
            }
            else if (updateCounterSourceStr == "AI_START") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_AI_START;
            }
            else if (updateCounterSourceStr == "DI_CONVERT") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_DI_CONVERT;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA0") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA0;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA1") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA1;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA2") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA2;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA3") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA3;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA4") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA4;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA5") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA5;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA6") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA6;
            }
            else if (updateCounterSourceStr == "INTTRIGGERA7") {
                updateCounterSource = XSERIES_DO_UPDATE_COUNTER_INTTRIGGERA7;
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
                updateCounterPolarity = XSERIES_DO_POLARITY_RISING_EDGE;
            }
            else if (updateCounterPolarityStr == "FALLING_EDGE") {
                updateCounterPolarity = XSERIES_DO_POLARITY_FALLING_EDGE;
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
        ok = (updateIntervalCounterPeriodDivisor > 0);
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
        ok = (updateIntervalCounterDelay > 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The UpdateIntervalCounterDelay shall be > 0");
        }
    }

    if (ok) {
        ok = data.MoveRelative("Signals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }
        if (ok) {
            //Do not allow to add signals in run-time
            ok = data.Write("Locked", 1);
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

bool NI6368DIO::SetConfiguredDatabase(StructuredDataI& data) {
    uint32 i;
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = (GetSignalType(0) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "All the DIO signals shall be of type UnsignedInteger32Bit");
        }
    }
    uint32 nElements = 0u;
    if (ok) {
        ok = (GetSignalNumberOfElements(0, nElements));
    }
    if (ok) {
        ok = (nElements == 1u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError,
                     "All the DIO signals shall have one and only one element. (Input signals may have more than one sample).");
    }

    uint32 nOfFunctions = GetNumberOfFunctions();
    uint32 functionIdx;
    //Check that the number of samples is one and exactly one for all ports
    for (functionIdx = 0u; (functionIdx < nOfFunctions) && (ok); functionIdx++) {
        uint32 nOfSignals = 0u;
        ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfSignals);

        for (i = 0u; (i < nOfSignals) && (ok); i++) {
            uint32 nSamples = 0u;
            ok = GetFunctionSignalSamples(InputSignals, functionIdx, i, nSamples);
            if (ok) {
                ok = (nSamples == 1u);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Only one sample shall be specified for the input");
            }
        }

        if (ok) {
            ok = GetFunctionNumberOfSignals(OutputSignals, functionIdx, nOfSignals);
        }

        for (i = 0u; (i < nOfSignals) && (ok); i++) {
            uint32 nSamples = 0u;
            ok = GetFunctionSignalSamples(OutputSignals, functionIdx, i, nSamples);
            if (ok) {
                ok = (nSamples == 1u);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Only one sample shall be specified for the output");
            }
        }
    }

    StreamString fullDeviceName;
    //Configure the board
    if (ok) {
        ok = fullDeviceName.Printf("%s.%d.dio", deviceName.Buffer(), boardId);
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
        ok = (xseries_stop_di(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not xseries_stop_di on device %s", fullDeviceName)
        }
    }
    if (ok) {
        ok = (xseries_stop_do(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not xseries_stop_do on device %s", fullDeviceName)
        }
    }
    if (ok) {
        ok = (xseries_reset_dio(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not xseries_reset_dio on device %s", fullDeviceName)
        }
    }

    if (inputsEnabled) {
        xseries_di_conf_t diConfiguration = xseries_static_di(inputPortMask);

        if (ok) {
            ok = (xseries_set_di_sample_clock(&diConfiguration, clockSampleSource, clockSamplePolarity) == 0);
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set the sample clock for device %s", fullDeviceName)
            }
        }
        if (ok) {
            ok = (xseries_set_di_scan_interval_counter(&diConfiguration, scanIntervalCounterSource, scanIntervalCounterPeriod, scanIntervalCounterDelay) == 0);
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set the convert clock for device %s", fullDeviceName)
            }
        }
        if (ok) {
            ok = (xseries_load_di_conf(boardFileDescriptor, diConfiguration) == 0);
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not load xseries_load_di_conf for device %s", fullDeviceName)
            }
        }
    }
    if (outputsEnabled) {
        xseries_do_conf_t doConfiguration = xseries_static_do(outputPortMask);
        if (ok) {
            ok = (xseries_set_do_start_trigger(&doConfiguration, startTriggerSource, startTriggerPolarity, 1u) == 0);
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not xseries_set_ao_start_trigger for device %s", fullDeviceName)
            }
        }
        if (ok) {
            ok = (xseries_set_do_update_counter(&doConfiguration, updateCounterSource, updateCounterPolarity) == 0);
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not xseries_set_ao_update_counter for device %s", fullDeviceName)
            }
        }
        if (ok) {
            ok = (xseries_set_do_update_interval_counter(&doConfiguration, updateIntervalCounterSource, updateIntervalCounterPolarity,
                                                         updateIntervalCounterPeriodDivisor, updateIntervalCounterDelay) == 0);
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not xseries_set_ao_update_interval_counter for device %s", fullDeviceName)
            }
        }
        if (ok) {
            ok = (xseries_load_do_conf(boardFileDescriptor, doConfiguration) == 0);
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not load xseries_load_do_conf for device %s", fullDeviceName)
            }
        }
    }
    if (ok) {
        ok = (xseries_start_di(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not load xseries_start_di for device %s", fullDeviceName)
        }
    }
    if (ok) {
        ok = (xseries_start_do(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not load xseries_start_do for device %s", fullDeviceName)
        }
    }
    return ok;
}

bool NI6368DIO::Synchronise() {
    bool ok = (xseries_write_do(boardFileDescriptor, &portValue, 1u) == 1u);
    if (ok) {
        //EAGAIN is OK
        ssize_t ret = xseries_read_di(boardFileDescriptor, &portValue, 1u);
        if (ret != 0) {
            ok = (ret == EAGAIN);
        }
    }
    return ok;
}

bool NI6368DIO::ReadDIOConfiguration(xseries_di_conf_t * const confDI, xseries_do_conf_t * const confDO) const {
    bool ok = false;
    if (boardFileDescriptor > 0) {
        ok = (xseries_read_di_conf(boardFileDescriptor, confDI) == 0);
        if (ok) {
            ok = (xseries_read_do_conf(boardFileDescriptor, confDO) == 0);
        }
    }
    return ok;
}

CLASS_REGISTER(NI6368DIO, "1.0")
}

