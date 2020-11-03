/**
 * @file NI6368ADC.cpp
 * @brief Source file for class NI6368ADC
 * @date 12/01/2017
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
 * the class NI6368ADC (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "NI6368ADC.h"

#include <fcntl.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryMapInputBroker.h"
#include "NI6368ADCInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
NI6368ADC::NI6368ADC() :
        DataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this) {
    lastBufferIdx = 0u;
    cycleFrequency = 0.F;
    numberOfSamples = 0u;
    boardId = 0;
    boardFileDescriptor = -1;
    deviceName = "";
    counter = 0u;
    counterValue = NULL_PTR(uint32 *);
    timeValue = NULL_PTR(uint32 *);
    scanIntervalCounterDelay = 0u;
    scanIntervalCounterPeriod = 0u;
    numberOfADCsEnabled = 0u;
    dmaBufferSize = 0u;
    clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTERNALTIMING;
    clockSamplePolarity = XSERIES_AI_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE;
    clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTERNALTIMING;
    clockConvertPolarity = XSERIES_AI_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE;
    scanIntervalCounterSource = XSERIES_SCAN_INTERVAL_COUNTER_TB3;
    scanIntervalCounterPolarity = XSERIES_SCAN_INTERVAL_COUNTER_POLARITY_RISING_EDGE;
    currentBufferIdx = 0u;
    currentBufferOffset = 0u;
    nBytesInDMAFromStart = 0u;
    dmaOffset = 0u;
    dmaChannel = 0u;
    lastTimeValue = 0u;
    fastMuxSleepTime = 1e-3F;
    executionMode = NI6368ADC_EXEC_SPAWNED;
    dma = NULL_PTR(struct xseries_dma *);

    keepRunning = true;
    synchronising = false;
    cpuMask = 0u;
    uint32 n;
    for (n = 0u; n < NI6368ADC_MAX_CHANNELS; n++) {
        inputRange[n] = XSERIES_INPUT_RANGE_10V;
        adcEnabled[n] = false;
        channelsFileDescriptors[n] = -1;
        uint32 b;
        for (b = 0u; b < NUMBER_OF_BUFFERS; b++) {
            channelsMemory[b][n] = NULL_PTR(int16 *);
            channelsMemory[b][n] = NULL_PTR(int16 *);
        }
    }
    dmaReadBuffer = NULL_PTR(int16 *);
    if (!synchSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create EventSem.");
    }
    else {
        (void) (synchSem.Reset());
    }
    fastMux.Create();
    counterResetFastMux.Create();
}

/*lint -e{1551} -e{1740} the destructor must guarantee that the NI6368ADC SingleThreadService is stopped and that all the file descriptors are closed. The dma is freed by the xseries-lib*/
NI6368ADC::~NI6368ADC() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    uint32 n;
    if (boardFileDescriptor != -1) {
        if (xseries_stop_ai(boardFileDescriptor) < 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop acquisition.");
        }
    }
    for (n = 0u; n < NI6368ADC_MAX_CHANNELS; n++) {
        if (channelsFileDescriptors[n] != -1) {
            close(channelsFileDescriptors[n]);
        }
    }
    if (dma != NULL_PTR(struct xseries_dma *)) {
        xseries_dma_close(dma);
    }
    if (boardFileDescriptor != -1) {
        close(boardFileDescriptor);
    }
    for (n = 0u; n < NI6368ADC_MAX_CHANNELS; n++) {
        uint32 b;
        for (b = 0u; b < NUMBER_OF_BUFFERS; b++) {
            delete[] channelsMemory[b][n];
        }
    }
    if (dmaReadBuffer != NULL_PTR(int16 *)) {
        delete[] dmaReadBuffer;
    }
    if (counterValue != NULL_PTR(uint32 *)) {
        delete[] counterValue;
    }
    if (timeValue != NULL_PTR(uint32 *)) {
        delete[] timeValue;
    }
}

bool NI6368ADC::AllocateMemory() {
    return true;
}

uint32 NI6368ADC::GetNumberOfMemoryBuffers() {
    return NUMBER_OF_BUFFERS;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool NI6368ADC::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (signalIdx < (NI6368ADC_MAX_CHANNELS + NI6368ADC_HEADER_SIZE));
    if (ok) {
        if (signalIdx == 0u) {
            if (counterValue != NULL_PTR(uint32 *)) {
                signalAddress = reinterpret_cast<void *>(&counterValue[bufferIdx]);
            }
        }
        else if (signalIdx == 1u) {
            if (timeValue != NULL_PTR(uint32 *)) {
                signalAddress = reinterpret_cast<void *>(&timeValue[bufferIdx]);
            }
        }
        else {
            signalAddress = &(channelsMemory[bufferIdx][signalIdx - NI6368ADC_HEADER_SIZE][0]);
        }
    }
    return ok;
}

const char8* NI6368ADC::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == InputSignals) {
        float32 frequency = 0.F;
        if (!data.Read("Frequency", frequency)) {
            frequency = -1.F;
        }

        brokerName = "NI6368ADCInputBroker";
        if (frequency > 0.F) {
            cycleFrequency = frequency;
            synchronising = true;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with OutputSignals");
    }
    return brokerName;
}

bool NI6368ADC::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    ReferenceT<NI6368ADCInputBroker> broker(new NI6368ADCInputBroker(this));
    bool ok = broker.IsValid();
    if (ok) {
        ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ok) {
        ok = inputBrokers.Insert(broker);
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns false irrespectively of the input parameters.*/
bool NI6368ADC::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

bool NI6368ADC::Synchronise() {
    ErrorManagement::ErrorType err(true);
    if (synchronising) {
        if (executionMode == NI6368ADC_EXEC_RTTHREAD) {
            ExecutionInfo info;
            info.SetStage(ExecutionInfo::MainStage);
            err = ErrorManagement::NotCompleted;
            while (err == ErrorManagement::NotCompleted) {
                err = Execute(info);
            }
            if (err == ErrorManagement::Completed) {
                err = ErrorManagement::NoError;
            }
        }
        else {
            (void) fastMux.FastLock(TTInfiniteWait, fastMuxSleepTime);
            if (lastBufferIdx == currentBufferIdx) {
                err = !synchSem.Reset();
                fastMux.FastUnLock();
                if (err.ErrorsCleared()) {
                    err = synchSem.Wait(TTInfiniteWait);
                }
            }
            else {
                fastMux.FastUnLock();
            }
        }
    }
    if (timeValue != NULL_PTR(uint32 *)) {
        if (lastTimeValue == timeValue[lastBufferIdx]) {
            if (lastTimeValue != 0u) {
                REPORT_ERROR(ErrorManagement::Warning, "Repeated time values. Last = %d Current = %d. lastBufferIdx = %d currentBufferIdx = %d", lastTimeValue,
                             timeValue[lastBufferIdx], lastBufferIdx, currentBufferIdx);
            }
        }
        lastTimeValue = timeValue[lastBufferIdx];
    }

    return err.ErrorsCleared();
}

//lint -e{613} -e{414} dma cannot be null as otherwise ResetDMA would not be called.
void NI6368ADC::ResetDMA() {
    size_t nBytesInDMA = xsereis_ai_dma_samples_in_buffer(dma);
    while (nBytesInDMA > 0u) {
        dmaOffset = dmaOffset + nBytesInDMA;
        dmaOffset %= dma->ai.count;
        nBytesInDMAFromStart += nBytesInDMA;
        dma->ai.last_transfer_count = nBytesInDMAFromStart;
        //lint -e{414} numberOfADCsEnabled > 0 guaranteed during configuration.
        dmaChannel = (dma->ai.last_transfer_count % numberOfADCsEnabled);
        nBytesInDMA = xsereis_ai_dma_samples_in_buffer(dma);
    }
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the counter and the timer are always reset irrespectively of the states being changed.*/
bool NI6368ADC::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    bool ok = (counterResetFastMux.FastLock() == ErrorManagement::NoError);
    if (ok) {
        if (executionMode == NI6368ADC_EXEC_RTTHREAD) {
            ResetDMA();
        } 
    }
    if (ok) {
        counter = 0u;
        uint32 b;
        for (b = 0u; b < NUMBER_OF_BUFFERS; b++) {
            if (counterValue != NULL_PTR(uint32 *)) {
                counterValue[b] = 0u;
            }
            if (timeValue != NULL_PTR(uint32 *)) {
                timeValue[b] = 0u;
            }
        }
        currentBufferOffset = 0u;
    }
    counterResetFastMux.FastUnLock();
    if (ok) {
        if (executionMode == NI6368ADC_EXEC_SPAWNED) {
            if (executor.GetStatus() == EmbeddedThreadI::OffState) {
                keepRunning = true;
                if (cpuMask != 0u) {
                    executor.SetCPUMask(cpuMask);
                }
                executor.SetName(GetName());
                ok = executor.Start();
            }
        }
    }
    return ok;
}

bool NI6368ADC::Initialise(StructuredDataI& data) {
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
        StreamString executionModeStr;
        if (!data.Read("ExecutionMode", executionModeStr)) {
            executionModeStr = "IndependentThread";
            REPORT_ERROR(ErrorManagement::Warning, "No ExecutionMode specified. Using IndependentThread");
        }
        if (executionModeStr == "IndependentThread") {
            executionMode = NI6368ADC_EXEC_SPAWNED;
        }
        else {
            executionMode = NI6368ADC_EXEC_RTTHREAD;
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
    StreamString clockSampleSourceStr;
    if (ok) {
        ok = data.Read("ClockSampleSource", clockSampleSourceStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ClockSampleSource shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("DMABufferSize", dmaBufferSize);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The DMABufferSize shall be specified");
        }
    }
    if (ok) {
        if (clockSampleSourceStr == "INTERNALTIMING") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTERNALTIMING;
        }
        else if (clockSampleSourceStr == "PFI0") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI0;
        }
        else if (clockSampleSourceStr == "PFI1") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI1;
        }
        else if (clockSampleSourceStr == "PFI2") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI2;
        }
        else if (clockSampleSourceStr == "PFI3") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI3;
        }
        else if (clockSampleSourceStr == "PFI4") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI4;
        }
        else if (clockSampleSourceStr == "PFI5") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI5;
        }
        else if (clockSampleSourceStr == "PFI6") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI6;
        }
        else if (clockSampleSourceStr == "PFI7") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI7;
        }
        else if (clockSampleSourceStr == "PFI8") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI8;
        }
        else if (clockSampleSourceStr == "PFI9") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI9;
        }
        else if (clockSampleSourceStr == "PFI10") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI10;
        }
        else if (clockSampleSourceStr == "PFI11") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI11;
        }
        else if (clockSampleSourceStr == "PFI12") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI12;
        }
        else if (clockSampleSourceStr == "PFI13") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI13;
        }
        else if (clockSampleSourceStr == "PFI14") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI14;
        }
        else if (clockSampleSourceStr == "PFI15") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI15;
        }
        else if (clockSampleSourceStr == "RTSI0") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI0;
        }
        else if (clockSampleSourceStr == "RTSI1") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI1;
        }
        else if (clockSampleSourceStr == "RTSI2") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI2;
        }
        else if (clockSampleSourceStr == "RTSI3") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI3;
        }
        else if (clockSampleSourceStr == "RTSI4") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI4;
        }
        else if (clockSampleSourceStr == "RTSI5") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI5;
        }
        else if (clockSampleSourceStr == "RTSI6") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI6;
        }
        else if (clockSampleSourceStr == "RTSI7") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI7;
        }
        else if (clockSampleSourceStr == "DIO_CHGDETECT") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_DIO_CHGDETECT;
        }
        else if (clockSampleSourceStr == "G0_OUT") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G0_OUT;
        }
        else if (clockSampleSourceStr == "G1_OUT") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G1_OUT;
        }
        else if (clockSampleSourceStr == "G2_OUT") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G2_OUT;
        }
        else if (clockSampleSourceStr == "G3_OUT") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G3_OUT;
        }
        else if (clockSampleSourceStr == "STAR_TRIGGER") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_STAR_TRIGGER;
        }
        else if (clockSampleSourceStr == "SCXI_TRIG1") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_SCXI_TRIG1;
        }
        else if (clockSampleSourceStr == "ATRIG") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_ATRIG;
        }
        else if (clockSampleSourceStr == "LOW") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_LOW;
        }
        else if (clockSampleSourceStr == "PXIE_DSTARA") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PXIE_DSTARA;
        }
        else if (clockSampleSourceStr == "PXIE_DSTARB") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PXIE_DSTARB;
        }
        else if (clockSampleSourceStr == "G0_SAMPLECLK") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G0_SAMPLECLK;
        }
        else if (clockSampleSourceStr == "G1_SAMPLECLK") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G1_SAMPLECLK;
        }
        else if (clockSampleSourceStr == "G2_SAMPLECLK") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G2_SAMPLECLK;
        }
        else if (clockSampleSourceStr == "G3_SAMPLECLK") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G3_SAMPLECLK;
        }
        else if (clockSampleSourceStr == "DI_CONVERT") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_DI_CONVERT;
        }
        else if (clockSampleSourceStr == "AO_UPDATE") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_AO_UPDATE;
        }
        else if (clockSampleSourceStr == "DO_UPDATE") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_DO_UPDATE;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA0") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA0;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA1") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA1;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA2") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA2;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA3") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA3;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA4") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA4;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA5") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA5;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA6") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA6;
        }
        else if (clockSampleSourceStr == "INTTRIGGERA7") {
            clockSampleSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA7;
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
            clockSamplePolarity = XSERIES_AI_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE;
        }
        else if (clockSamplePolarityStr == "ACTIVE_LOW_OR_FALLING_EDGE") {
            clockSamplePolarity = XSERIES_AI_POLARITY_ACTIVE_LOW_OR_FALLING_EDGE;
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockSamplePolarity");
        }
    }
    StreamString clockConvertSourceStr;
    if (ok) {
        ok = data.Read("ClockConvertSource", clockConvertSourceStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ClockConvertSource shall be specified");
        }
    }
    if (ok) {
        if (clockConvertSourceStr == "INTERNALTIMING") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTERNALTIMING;
        }
        else if (clockConvertSourceStr == "PFI0") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI0;
        }
        else if (clockConvertSourceStr == "PFI1") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI1;
        }
        else if (clockConvertSourceStr == "PFI2") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI2;
        }
        else if (clockConvertSourceStr == "PFI3") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI3;
        }
        else if (clockConvertSourceStr == "PFI4") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI4;
        }
        else if (clockConvertSourceStr == "PFI5") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI5;
        }
        else if (clockConvertSourceStr == "PFI6") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI6;
        }
        else if (clockConvertSourceStr == "PFI7") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI7;
        }
        else if (clockConvertSourceStr == "PFI8") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI8;
        }
        else if (clockConvertSourceStr == "PFI9") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI9;
        }
        else if (clockConvertSourceStr == "PFI10") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI10;
        }
        else if (clockConvertSourceStr == "PFI11") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI11;
        }
        else if (clockConvertSourceStr == "PFI12") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI12;
        }
        else if (clockConvertSourceStr == "PFI13") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI13;
        }
        else if (clockConvertSourceStr == "PFI14") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI14;
        }
        else if (clockConvertSourceStr == "PFI15") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PFI15;
        }
        else if (clockConvertSourceStr == "RTSI0") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI0;
        }
        else if (clockConvertSourceStr == "RTSI1") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI1;
        }
        else if (clockConvertSourceStr == "RTSI2") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI2;
        }
        else if (clockConvertSourceStr == "RTSI3") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI3;
        }
        else if (clockConvertSourceStr == "RTSI4") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI4;
        }
        else if (clockConvertSourceStr == "RTSI5") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI5;
        }
        else if (clockConvertSourceStr == "RTSI6") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI6;
        }
        else if (clockConvertSourceStr == "RTSI7") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_RTSI7;
        }
        else if (clockConvertSourceStr == "DIO_CHGDETECT") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_DIO_CHGDETECT;
        }
        else if (clockConvertSourceStr == "G0_OUT") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G0_OUT;
        }
        else if (clockConvertSourceStr == "G1_OUT") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G1_OUT;
        }
        else if (clockConvertSourceStr == "G2_OUT") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G2_OUT;
        }
        else if (clockConvertSourceStr == "G3_OUT") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G3_OUT;
        }
        else if (clockConvertSourceStr == "STAR_TRIGGER") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_STAR_TRIGGER;
        }
        else if (clockConvertSourceStr == "SCXI_TRIG1") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_SCXI_TRIG1;
        }
        else if (clockConvertSourceStr == "ATRIG") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_ATRIG;
        }
        else if (clockConvertSourceStr == "LOW") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_LOW;
        }
        else if (clockConvertSourceStr == "PXIE_DSTARA") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PXIE_DSTARA;
        }
        else if (clockConvertSourceStr == "PXIE_DSTARB") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_PXIE_DSTARB;
        }
        else if (clockConvertSourceStr == "G0_SAMPLECLK") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G0_SAMPLECLK;
        }
        else if (clockConvertSourceStr == "G1_SAMPLECLK") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G1_SAMPLECLK;
        }
        else if (clockConvertSourceStr == "G2_SAMPLECLK") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G2_SAMPLECLK;
        }
        else if (clockConvertSourceStr == "G3_SAMPLECLK") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_G3_SAMPLECLK;
        }
        else if (clockConvertSourceStr == "DI_CONVERT") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_DI_CONVERT;
        }
        else if (clockConvertSourceStr == "AO_UPDATE") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_AO_UPDATE;
        }
        else if (clockConvertSourceStr == "DO_UPDATE") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_DO_UPDATE;
        }
        else if (clockConvertSourceStr == "INTTRIGGERA0") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA0;
        }
        else if (clockConvertSourceStr == "INTTRIGGERA1") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA1;
        }
        else if (clockConvertSourceStr == "INTTRIGGERA2") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA2;
        }
        else if (clockConvertSourceStr == "INTTRIGGERA3") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA3;
        }
        else if (clockConvertSourceStr == "INTTRIGGERA4") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA4;
        }
        else if (clockConvertSourceStr == "INTTRIGGERA5") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA5;
        }
        else if (clockConvertSourceStr == "INTTRIGGERA6") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA6;
        }
        else if (clockConvertSourceStr == "INTTRIGGERA7") {
            clockConvertSource = XSERIES_AI_SAMPLE_CONVERT_CLOCK_INTTRIGGERA7;
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockConvertSource");
        }
    }
    StreamString clockConvertPolarityStr;
    if (ok) {
        ok = data.Read("ClockConvertPolarity", clockConvertPolarityStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ClockConvertPolarity shall be specified");
        }
    }
    if (ok) {
        if (clockConvertPolarityStr == "ACTIVE_HIGH_OR_RISING_EDGE") {
            clockConvertPolarity = XSERIES_AI_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE;
        }
        else if (clockConvertPolarityStr == "ACTIVE_LOW_OR_FALLING_EDGE") {
            clockConvertPolarity = XSERIES_AI_POLARITY_ACTIVE_LOW_OR_FALLING_EDGE;
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockConvertPolarity");
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
    StreamString scanIntervalCounterPolarityStr;
    if (ok) {
        ok = data.Read("ScanIntervalCounterPolarity", scanIntervalCounterPolarityStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ScanIntervalCounterPolarity shall be specified");
        }
    }
    if (ok) {
        if (scanIntervalCounterPolarityStr == "RISING_EDGE") {
            scanIntervalCounterPolarity = XSERIES_SCAN_INTERVAL_COUNTER_POLARITY_RISING_EDGE;
        }
        else if (scanIntervalCounterPolarityStr == "FALLING_EDGE") {
            scanIntervalCounterPolarity = XSERIES_SCAN_INTERVAL_COUNTER_POLARITY_FALLING_EDGE;
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ScanIntervalCounterPolarity");
        }
    }

    if (ok) {
        if (!data.Read("CPUs", cpuMask)) {
            REPORT_ERROR(ErrorManagement::Information, "No CPUs defined");
        }
        uint32 realTimeModeUInt = 0u;
        if (!data.Read("RealTimeMode", realTimeModeUInt)) {
            REPORT_ERROR(ErrorManagement::Information, "No RealTimeMode defined");
        }
        if (realTimeModeUInt == 1u) {
            fastMuxSleepTime = 0.0F;
        }
        else {
            fastMuxSleepTime = 1e-3F;
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
        uint32 maxChannelId = 0u;
        while ((i < (NI6368ADC_MAX_CHANNELS + NI6368ADC_HEADER_SIZE)) && (ok)) {
            if (data.MoveRelative(data.GetChildName(i))) {
                uint32 channelId;
                if (data.Read("ChannelId", channelId)) {
                    ok = (channelId < NI6368ADC_MAX_CHANNELS);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Invalid ChannelId specified.");
                    }
                    if (ok) {
                        //Channel id must be specified monotonically increasing otherwise there will be a mismatch in the DMA.
                        if (maxChannelId != 0u) {
                            ok = (channelId > maxChannelId);
                        }
                        if (!ok) {
                            REPORT_ERROR(ErrorManagement::ParametersError, "ChannelId must be monotonically increasing.");
                        }
                    }
                    if (ok) {
                        maxChannelId = channelId;
                        adcEnabled[channelId] = true;
                        float32 range;
                        numberOfADCsEnabled++;
                        if (data.Read("InputRange", range)) {
                            if ((range > 9.99) && (range < 10.01)) {
                                inputRange[channelId] = XSERIES_INPUT_RANGE_10V;
                            }
                            else if ((range > 4.99) && (range < 5.01)) {
                                inputRange[channelId] = XSERIES_INPUT_RANGE_5V;
                            }
                            else if ((range > 1.99) && (range < 2.01)) {
                                inputRange[channelId] = XSERIES_INPUT_RANGE_2V;
                            }
                            else if ((range > 0.99) && (range < 1.01)) {
                                inputRange[channelId] = XSERIES_INPUT_RANGE_1V;
                            }
                            else {
                                ok = false;
                                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported InputRange.");
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

bool NI6368ADC::SetConfiguredDatabase(StructuredDataI& data) {
    uint32 i;
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = (GetNumberOfSignals() > (NI6368ADC_HEADER_SIZE));
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "At least (%d) signals shall be configured (header + 1 ADC)", NI6368ADC_HEADER_SIZE + 1u);
    }
    //The type of counter shall be unsigned int32 or uint32
    if (ok) {
        ok = (GetSignalType(0u) == SignedInteger32Bit);
        if (!ok) {
            ok = (GetSignalType(0u) == UnsignedInteger32Bit);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The first signal (counter) shall be of type SignedInteger32Bit or UnsignedInteger32Bit");
        }
    }
    //The type of time shall be unsigned int32 or uint32
    if (ok) {
        ok = (GetSignalType(1u) == SignedInteger32Bit);
        if (!ok) {
            ok = (GetSignalType(1u) == UnsignedInteger32Bit);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The second signal (time) shall be of type SignedInteger32Bit or UnsignedInteger32Bit");
        }
    }
    if (ok) {
        for (i = 0u; (i < numberOfADCsEnabled) && (ok); i++) {
            ok = (GetSignalType(NI6368ADC_HEADER_SIZE + i) == SignedInteger16Bit);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "All the ADC signals shall be of type SignedInteger16Bit");
        }
    }

    //The current code would not work with more than one function interacting with this DataSourceI (see GetLastBufferIdx).
    //TODO reimplement with CircularBuffer from the core.
    if (ok) {
        ok = synchronising;
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The function interacting with this DataSourceI must be synchronising");
        }
    }
    uint32 nOfFunctions = GetNumberOfFunctions();
    if (ok) {
        ok = (nOfFunctions == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "At most one function shall interact with this DataSourceI");
        }
    }

    uint32 functionIdx;
    //Check that the number of samples for the counter and the time is one and that for the other signals is always the same
    for (functionIdx = 0u; (functionIdx < nOfFunctions) && (ok); functionIdx++) {
        uint32 nOfSignals = 0u;
        ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfSignals);

        for (i = 0u; (i < nOfSignals) && (ok); i++) {
            bool isCounter = false;
            bool isTime = false;
            uint32 signalIdx = 0u;
            uint32 nSamples = 0u;
            ok = GetFunctionSignalSamples(InputSignals, functionIdx, i, nSamples);

            //Is the counter or the time signal?
            StreamString signalAlias;
            if (ok) {
                ok = GetFunctionSignalAlias(InputSignals, functionIdx, i, signalAlias);
            }
            if (ok) {
                ok = GetSignalIndex(signalIdx, signalAlias.Buffer());
            }
            if (ok) {
                isCounter = (signalIdx == 0u);
                isTime = (signalIdx == 1u);
            }
            if (ok) {
                if (isCounter) {
                    if (nSamples > 1u) {
                        ok = false;
                        REPORT_ERROR(ErrorManagement::ParametersError, "The first signal (counter) shall have one and only one sample");
                    }
                }
                else if (isTime) {
                    if (nSamples > 1u) {
                        ok = false;
                        REPORT_ERROR(ErrorManagement::ParametersError, "The second signal (time) shall have one and only one sample");
                    }
                }
                else {
                    if (numberOfSamples == 0u) {
                        numberOfSamples = nSamples;
                    }
                    else {
                        if (numberOfSamples != nSamples) {
                            ok = false;
                            REPORT_ERROR(ErrorManagement::ParametersError, "All the ADC signals shall have the same number of samples");
                        }
                    }

                }
            }
        }
    }
    if (ok) {
        if (synchronising) {
            //numberOfADCsEnabled > 0 as otherwise it would have stopped before
            if (numberOfADCsEnabled > 0u) {
                float32 totalNumberOfSamplesPerSecond = (static_cast<float32>(numberOfSamples) * cycleFrequency);
                ok = (NI6368ADC_SAMPLING_FREQUENCY == static_cast<uint32>(totalNumberOfSamplesPerSecond));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError,
                                 "The numberOfSamples * cycleFrequency (%u) shall be equal to the ADCs acquisition frequency (%u)",
                                 totalNumberOfSamplesPerSecond, NI6368ADC_SAMPLING_FREQUENCY);
                }
            }
        }
    }
    StreamString fullDeviceName;
    //Configure the board
    if (ok) {
        ok = fullDeviceName.Printf("%s.%d.ai", deviceName.Buffer(), boardId);
    }
    if (ok) {
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
        bool stopped = (xseries_stop_ai(boardFileDescriptor) == 0);
        if (!stopped) {
            REPORT_ERROR(ErrorManagement::Warning, "Could not stop the device %s while starting", fullDeviceName);
        }
        bool reset = (xseries_reset_ai(boardFileDescriptor) == 0);
        if (!reset) {
            REPORT_ERROR(ErrorManagement::Warning, "Could not reset the device %s while starting", fullDeviceName);
        }
    }
    xseries_ai_conf_t adcConfiguration = xseries_continuous_ai();
    for (i = 0u; (i < NI6368ADC_MAX_CHANNELS) && (ok); i++) {
        if (adcEnabled[i]) {
            //6368 only supports differential inputs.
            ok = (xseries_add_ai_channel(&adcConfiguration, static_cast<uint8_t>(i), inputRange[i], XSERIES_AI_CHANNEL_TYPE_DIFFERENTIAL, 0u) == 0);
            uint32 ii = i;
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not set InputRange for channel %d of device %s", ii, fullDeviceName);
            }
        }
    }
    if (ok) {
        ok = (xseries_set_ai_sample_clock(&adcConfiguration, clockSampleSource, clockSamplePolarity, 1u) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not set the sample clock for device %s", fullDeviceName);
        }
    }
    if (ok) {
        ok = (xseries_set_ai_convert_clock(&adcConfiguration, clockConvertSource, clockConvertPolarity) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not set the convert clock for device %s", fullDeviceName);
        }
    }
    if (ok) {
        ok = (xseries_set_ai_scan_interval_counter(&adcConfiguration, scanIntervalCounterSource, scanIntervalCounterPolarity, scanIntervalCounterPeriod, scanIntervalCounterDelay) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not set the convert clock for device %s", fullDeviceName);
        }
    }
    if (ok) {
        ok = (xseries_set_ai_attribute(&adcConfiguration, XSERIES_AI_DMA_BUFFER_SIZE, dmaBufferSize) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not set the DMA buffer size for device %s", fullDeviceName);
        }
    }
    if (ok) {
        ok = (xseries_load_ai_conf(boardFileDescriptor, adcConfiguration) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not load configuration for device %s", fullDeviceName);
        }
    }
    if (ok) {
        //Allocate memory
        counterValue = new uint32[NUMBER_OF_BUFFERS];
        timeValue = new uint32[NUMBER_OF_BUFFERS];

        for (i = 0u; (i < NI6368ADC_MAX_CHANNELS) && (ok); i++) {
            uint32 b;
            for (b = 0u; (b < NUMBER_OF_BUFFERS) && (ok); b++) {
                channelsMemory[b][i] = new int16[numberOfSamples];
            }
        }
    }

    if (ok) {
        uint32 retry = 0u;
        const uint32 maxRetries = 10u;
        for (i = 0u; (i < NI6368ADC_MAX_CHANNELS) && (ok); i++) {
            if (adcEnabled[i]) {
                StreamString channelDeviceName;
                //Otherwise there is the perception that the Printf might modify i inside the for loop
                uint32 ii = i;
                ok = channelDeviceName.Printf("%s.%d", fullDeviceName.Buffer(), ii);
                if (ok) {
                    ok = channelDeviceName.Seek(0ULL);
                }
                if (ok) {
                    retry = 0u;
                    channelsFileDescriptors[i] = -1;
                    while ((channelsFileDescriptors[i] < 0) && (retry < maxRetries)) {
                        channelsFileDescriptors[i] = open(channelDeviceName.Buffer(), O_RDWR);
                        ok = (channelsFileDescriptors[i] > -1);
                        if (!ok) {
                            Sleep::Sec(0.1F);
                            retry++;
                        }
                    }
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Could not open device %s", channelDeviceName);
                    }
                }
            }
        }
    }
    if (ok) {
        dma = xseries_dma_init(boardId, 0);
        ok = (dma != NULL_PTR(struct xseries_dma *));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not set the dma for device %s", fullDeviceName);
        }
    }
    if (ok) {
        ok = (xseries_start_ai(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not start the device %s", fullDeviceName);
        }
    }
    if (ok) {
        //lint -e{613} dma cannot be null as otherwise ok would be false
        if (dma->ai.count > 0u) {
            dmaReadBuffer = new int16[dma->ai.count];
        }
    }
    return ok;
}

uint8 NI6368ADC::GetLastBufferIdx() {
    uint8 toReadIdx = lastBufferIdx;
    //This is thread safe since this is executed in the context of the thread which calls Synchronise.
    lastBufferIdx++;
    if (lastBufferIdx == NUMBER_OF_BUFFERS) {
        lastBufferIdx = 0u;
    }
    return toReadIdx;
}

bool NI6368ADC::IsSynchronising() const {
    return synchronising;
}

ErrorManagement::ErrorType NI6368ADC::CopyFromDMA(const size_t numberOfSamplesFromDMA) {
    ErrorManagement::ErrorType err = ErrorManagement::NotCompleted;
    uint32 s = 0u;
    if (dmaReadBuffer != NULL_PTR(int16 *)) {
        (void) (counterResetFastMux.FastLock());
        while (s < (numberOfSamplesFromDMA)) {
            channelsMemory[currentBufferIdx][dmaChannel][currentBufferOffset] = dmaReadBuffer[s];
            s++;
            dmaChannel++;

            if (dmaChannel == numberOfADCsEnabled) {
                dmaChannel = 0u;
                currentBufferOffset++;
                if (currentBufferOffset == numberOfSamples) {
                    currentBufferOffset = 0u;
                    //Don't wait if this fails. At most a cycle will be delayed.
                    (void) fastMux.FastLock(TTInfiniteWait, fastMuxSleepTime);
                    currentBufferIdx++;
                    if (currentBufferIdx == NUMBER_OF_BUFFERS) {
                        currentBufferIdx = 0u;
                    }
                    //This is required as otherwise it could copy the wrong counter value in the consumer thread.
                    if (counterValue != NULL_PTR(uint32 *)) {
                        counterValue[currentBufferIdx] = counter;
                    }
                    if (counter > 0u) {
                        uint64 counterSamples = counter;
                        counterSamples *= numberOfSamples;
                        counterSamples *= 1000000LLU;
                        counterSamples /= NI6368ADC_SAMPLING_FREQUENCY;
                        if (timeValue != NULL_PTR(uint32 *)) {
                            timeValue[currentBufferIdx] = static_cast<uint32>(counterSamples);
                        }
                    }
                    else {
                        if (timeValue != NULL_PTR(uint32 *)) {
                            timeValue[currentBufferIdx] = 0u;
                        }
                    }
                    if (synchronising) {
                        err = !synchSem.Post();
                        err = ErrorManagement::Completed;
                    }
                    fastMux.FastUnLock();
                    counter++;
                }
            }
        }
        counterResetFastMux.FastUnLock();
    }
    return err;
}

ErrorManagement::ErrorType NI6368ADC::Execute(ExecutionInfo& info) {
    ErrorManagement::ErrorType err;
    if (info.GetStage() == ExecutionInfo::TerminationStage) {
        keepRunning = false;
    }
    else if (info.GetStage() == ExecutionInfo::StartupStage) {
        //Empty DMA buffer
        if ((dma != NULL_PTR(struct xseries_dma *)) && (numberOfADCsEnabled > 0u)) {
            ResetDMA();
        }
    }
    else {
        if ((dma != NULL_PTR(struct xseries_dma *)) && (dmaReadBuffer != NULL_PTR(int16 *))) {
            size_t nBytesInDMA = xsereis_ai_dma_samples_in_buffer(dma);
            if (nBytesInDMA > 0u) {
                if (nBytesInDMA > dma->ai.count) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Overflow while reading from the ADC");
                }
                else {
                    if ((dmaOffset + nBytesInDMA) > (dma->ai.count)) {
                        //Right part of the DMA
                        size_t samplesToCopy = (dma->ai.count - dmaOffset);
                        //Roll to the beginning
                        size_t samplesToCopyRoll = ((dmaOffset + nBytesInDMA) - dma->ai.count);
                        memcpy(&dmaReadBuffer[0], &dma->ai.data[dmaOffset], (sizeof(int16) * samplesToCopy));
                        memcpy(&dmaReadBuffer[samplesToCopy], &dma->ai.data[0], (sizeof(int16) * samplesToCopyRoll));
                    }
                    else {
                        memcpy(&dmaReadBuffer[0], &dma->ai.data[dmaOffset], (sizeof(int16) * nBytesInDMA));
                    }
                    err = CopyFromDMA(nBytesInDMA);
                    if (executionMode == NI6368ADC_EXEC_SPAWNED) {
                        //Reset the error if this is being managed by another thread. The NotCompleted is only used in the Synchronise method 
                        //if the DataSource is being driven by the RealTimeThread
                        //lint -e{9007} no side effects on the right hand side if it is not evaluated.
                        if ((err == ErrorManagement::NotCompleted) || (err == ErrorManagement::Completed)) {
                            err = ErrorManagement::NoError;
                        }
                    }
                }
                dmaOffset = dmaOffset + nBytesInDMA;
                dmaOffset %= dma->ai.count;
                nBytesInDMAFromStart += nBytesInDMA;
                dma->ai.last_transfer_count = nBytesInDMAFromStart;
            }
        }
    }
    return err;
}

bool NI6368ADC::ReadAIConfiguration(xseries_ai_conf_t * const conf) const {
    bool ok = false;
    if (boardFileDescriptor > 0) {
        ok = (xseries_read_ai_conf(boardFileDescriptor, conf) == 0);
    }
    return ok;
}

CLASS_REGISTER(NI6368ADC, "1.0")
}

