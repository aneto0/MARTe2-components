/**
 * @file NI6259ADC.cpp
 * @brief Source file for class NI6259ADC
 * @date 28/11/2016
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
 * the class NI6259ADC (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <fcntl.h>
#include <sched.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "NI6259ADC.h"
#include "NI6259ADCInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#define ADC_BASE_FREQ 20000000u
#define ADC_ONE_CH_MAX_FREQ 1250000u
#define ADC_MORE_CH_MAX_FREQ 1000000u

#define INDEPENDENT_THREAD 0u
#define REAL_TIME_THREAD 1u

#define ATTR_MAP_ENTRY(attr) { #attr , attr }
namespace MARTe {

struct AttributeMap {
    const char8 *name;
    pxi6259_segment_attribute attribute;
};

static struct AttributeMap attributeMap[] = {
ATTR_MAP_ENTRY(AI_NUMBER_OF_CHANNELS),
ATTR_MAP_ENTRY(AI_NO_OF_PRE_TRIG_SAMPLES),
ATTR_MAP_ENTRY(AI_NO_OF_POST_TRIG_SAMPLES),
ATTR_MAP_ENTRY(AI_CONTINUOUS),
ATTR_MAP_ENTRY(AI_EXTERNAL_CONVERT_CLOCK),
ATTR_MAP_ENTRY(AI_SIP_OUTPUT_SELECT),
ATTR_MAP_ENTRY(AI_EXTERNAL_MUX_PRESENT),
ATTR_MAP_ENTRY(AI_EXTMUX_OUTPUT_SELECT),
ATTR_MAP_ENTRY(AI_EXTERNAL_GATE_SELECT),
ATTR_MAP_ENTRY(AI_EXTERNAL_GATE_POLARITY),
ATTR_MAP_ENTRY(AI_BLOCKING_EXACT),
ATTR_MAP_ENTRY(AI_START_SELECT),
ATTR_MAP_ENTRY(AI_START_POLARITY),
ATTR_MAP_ENTRY(AI_REFERENCE_SELECT),
ATTR_MAP_ENTRY(AI_REFERENCE_POLARITY),
ATTR_MAP_ENTRY(AI_STOP_SELECT),
ATTR_MAP_ENTRY(AI_STOP_POLARITY),
ATTR_MAP_ENTRY(AI_SAMPLE_SELECT),
ATTR_MAP_ENTRY(AI_SAMPLE_POLARITY),
ATTR_MAP_ENTRY(AI_SAMPLE_PERIOD_DEVISOR),
ATTR_MAP_ENTRY(AI_SAMPLE_DELAY_DEVISOR),
ATTR_MAP_ENTRY(AI_CONVERT_SELECT),
ATTR_MAP_ENTRY(AI_CONVERT_POLARITY),
ATTR_MAP_ENTRY(AI_CONVERT_PERIOD_DEVISOR),
ATTR_MAP_ENTRY(AI_CONVERT_DELAY_DEVISOR),
ATTR_MAP_ENTRY(AI_CONVERT_OUTPUT_SELECT),
ATTR_MAP_ENTRY(AI_CONVERT_PULSE_WIDTH),
ATTR_MAP_ENTRY(AI_SI_SOURCE_SELECT),
ATTR_MAP_ENTRY(AI_ARM_SC),
ATTR_MAP_ENTRY(AI_ARM_SI),
ATTR_MAP_ENTRY(AI_ARM_SI2),
ATTR_MAP_ENTRY(AI_ARM_DIV), { NULL, AO_CONTINUOUS}
};

static pxi6259_segment_attribute GetAttribute(const char8 *attrName) {
    uint32 i = 0u;
    while (attributeMap[i].name != NULL) {
        if(StringHelper::Compare(attributeMap[i].name, attrName) == 0) {
            return attributeMap[i].attribute;
        }
        i++;
    }
    return AO_CONTINUOUS;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

NI6259ADC::NI6259ADC() :
        DataSourceI(),
        EmbeddedServiceMethodBinderI(),
        executor(*this) {
    numberOfSamples = 0u;
    boardId = 0u;
    samplingFrequency = 0u;
    convertFrequency = 0u;
    singleADCFrequency = 0.;
    boardFileDescriptor = -1;
    deviceName = "";
    counter = 0ull;
    sampleDelayDivisor = 0u;
    convertDelayDivisor = 0u;
    numberOfADCsEnabled = 0u;
    clockSampleSource = AI_SAMPLE_SELECT_SI_TC;
    clockSamplePolarity = AI_SAMPLE_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE;
    clockConvertSource = AI_CONVERT_SELECT_SI2TC;
    clockConvertPolarity = AI_CONVERT_POLARITY_RISING_EDGE;
    keepRunning = true;
    synchronising = false;
    cpuMask = 0u;
    counterResetFastMux.Create();
    fastBufMux.Create();
    allBufMux.Create();
    currentBufferIdx = 0u;
    lastBufferIdx = 0u;

    currentBufferOffset = 0u;
    lastTimeValue = 0ull;
    fastMuxSleepTime = 1e-3F;
    for (uint32 n = 0u; n < NUMBER_OF_BUFFERS; n++) {
        counterValue[n] = 0ull;
        timeValue[n] = 0ull;
    }
    dmaReadBuffer = NULL_PTR(int16*);
    dmaCalibBuffer = NULL_PTR(float32*);
    dma = NULL_PTR(struct pxi6259_dma*);
    dmaOffset = 0u;
    dmaChannel = 0u;
    synchType = 0u;
    sampleSize = 0u;

    calibrate = false;
    countSamples = false;
    resetOnBufferChange = false;
    changeBuffer = false;
    lastCounter = 0ull;
    for (uint32 n = 0u; n < NI6259ADC_MAX_CHANNELS; n++) {
        inputRange[n] = 1u;
        inputMode[n] = AI_CHANNEL_TYPE_RSE;
        inputPolarity[n] = AI_POLARITY_UNIPOLAR;
        adcEnabled[n] = false;
        uint32 b;
        for (b = 0u; b < NUMBER_OF_BUFFERS; b++) {
            fastMux[b].Create();
            channelsMemory[b][n] = NULL_PTR(uint8*);
        }
    }
    if (!synchSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create EventSem.");
    }
}

/*lint -e{1551} -e{1740} the destructor must guarantee that the NI6259ADC SingleThreadService is stopped and that all the file descriptors are closed. The dma is freed by the pxi-6259-lib*/
NI6259ADC::~NI6259ADC() {
    if (synchType == INDEPENDENT_THREAD) {
        if (!executor.Stop()) {
            Sleep::MSec(100);
            if (!executor.Stop()) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
            }
        }
    }
    uint32 n;
    if (boardFileDescriptor != -1) {
        if (pxi6259_stop_ai(boardFileDescriptor) < 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop acquisition.");
        }
    }
    if (dma != NULL_PTR(struct pxi6259_dma*)) {
        pxi6259_dma_close(dma);
    }
    if (boardFileDescriptor != -1) {
        close(boardFileDescriptor);
    }
    for (n = 0u; n < NI6259ADC_MAX_CHANNELS; n++) {
        uint32 b;
        for (b = 0u; b < NUMBER_OF_BUFFERS; b++) {
            delete[] channelsMemory[b][n];
        }
    }
    if (dmaReadBuffer != NULL_PTR(int16*)) {
        delete[] dmaReadBuffer;
    }
    if (dmaCalibBuffer != NULL_PTR(float32*)) {
        delete[] dmaCalibBuffer;
    }
}

bool NI6259ADC::AllocateMemory() {
    return true;
}

uint32 NI6259ADC::GetNumberOfMemoryBuffers() {
    return NUMBER_OF_BUFFERS;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool NI6259ADC::GetSignalMemoryBuffer(const uint32 signalIdx,
                                      const uint32 bufferIdx,
                                      void *&signalAddress) {
    bool ok = (signalIdx < (NI6259ADC_MAX_CHANNELS + NI6259ADC_HEADER_SIZE));
    if (ok) {
        if (signalIdx == 0u) {
            signalAddress = reinterpret_cast<void*>(&counterValue[bufferIdx]);
        }
        else if (signalIdx == 1u) {
            signalAddress = reinterpret_cast<void*>(&timeValue[bufferIdx]);
        }
        else {
            signalAddress = &(channelsMemory[bufferIdx][signalIdx - NI6259ADC_HEADER_SIZE][0]);
        }
    }
    return ok;
}

const char8* NI6259ADC::GetBrokerName(StructuredDataI &data,
                                      const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8*);
    if (direction == InputSignals) {
        float32 frequency = 0.F;
        if (!data.Read("Frequency", frequency)) {
            frequency = -1.F;
        }

        uint32 trigger = 0u;
        if (!data.Read("Trigger", trigger)) {
            frequency = -1.F;
        }

        brokerName = "NI6259ADCInputBroker";
        if ((frequency > 0.F) || (trigger > 0u)) {
            synchronising = true;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with OutputSignals");
    }
    return brokerName;
}

bool NI6259ADC::GetInputBrokers(ReferenceContainer &inputBrokers,
                                const char8 *const functionName,
                                void *const gamMemPtr) {
    ReferenceT<NI6259ADCInputBroker> broker(new NI6259ADCInputBroker(this));
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
bool NI6259ADC::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                 const char8 *const functionName,
                                 void *const gamMemPtr) {
    return false;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the counter and the timer are always reset irrespectively of the states being changed.*/
bool NI6259ADC::PrepareNextState(const char8 *const currentStateName,
                                 const char8 *const nextStateName) {
    bool ok = (counterResetFastMux.FastLock() == ErrorManagement::NoError);
    if (ok) {
        counter = 0ull;
        uint32 b;
        for (b = 0u; b < NUMBER_OF_BUFFERS; b++) {
            counterValue[b] = 0ull;
            timeValue[b] = 0ull;
        }
        currentBufferOffset = 0u;
        for (uint32 n = 0u; n < numberOfADCsEnabled; n++) {
            uint32 memSize = sampleSize * numberOfSamples;
            MemoryOperationsHelper::Set(&channelsMemory[currentBufferIdx][n][0], '\0', memSize);
        }
    }
    counterResetFastMux.FastUnLock();
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "pxi6259_start_ai(boardFileDescriptor)");
        ok = (pxi6259_start_ai(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not start AI acquisition");
        }
    }

    if (ok) {
        if (synchType == INDEPENDENT_THREAD) {
            if (executor.GetStatus() == EmbeddedThreadI::OffState) {
                keepRunning = true;
                if (cpuMask != 0u) {
                    executor.SetPriorityClass(Threads::RealTimePriorityClass);
                    executor.SetCPUMask(cpuMask);
                }
                executor.SetName(GetName());
                ok = executor.Start();
            }
        }
    }
    return ok;
}

bool NI6259ADC::Synchronise() {
    ErrorManagement::ErrorType err(true);

    if (synchType == REAL_TIME_THREAD) {
        while (lastBufferIdx == currentBufferIdx) {
            ExecutionInfo info;
            info.SetStage(ExecutionInfo::MainStage);
            err = Execute(info);
        }
    }
    else {
        (void) fastBufMux.FastLock(TTInfiniteWait, fastMuxSleepTime);
        if (lastBufferIdx == currentBufferIdx) {
            err = !synchSem.Reset();
            fastBufMux.FastUnLock();
            if (err.ErrorsCleared()) {
                err = synchSem.Wait(TTInfiniteWait);
            }
        }
        else {
            fastBufMux.FastUnLock();
        }
    }

    if (timeValue != NULL_PTR(uint64*)) {
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

bool NI6259ADC::NonSynchronise() {
    ErrorManagement::ErrorType err(true);
    synchSem.Reset();

    bool isLastBuffer = false;

    (void) allBufMux.FastLock(TTInfiniteWait, fastMuxSleepTime);
    isLastBuffer = (lastBufferIdx == currentBufferIdx);
    if (isLastBuffer && (synchType == INDEPENDENT_THREAD)) {
        changeBuffer = true;
    }
    allBufMux.FastUnLock();

    uint8 currentBufferCache = currentBufferIdx;
    if (synchType == REAL_TIME_THREAD) {
        ExecutionInfo info;
        info.SetStage(ExecutionInfo::MainStage);
        err = Execute(info);
    }

    if (isLastBuffer) {
        if (synchType == REAL_TIME_THREAD) {
            if (currentBufferCache == currentBufferIdx) {
                lastCounter = counter;

                //change buffer for next read
                currentBufferOffset = 0u;
                currentBufferIdx++;
                if (currentBufferIdx == NUMBER_OF_BUFFERS) {
                    currentBufferIdx = 0u;
                }
                for (uint32 n = 0u; n < numberOfADCsEnabled; n++) {
                    uint32 memSize = sampleSize * numberOfSamples;
                    MemoryOperationsHelper::Set(&channelsMemory[currentBufferIdx][n][0], '\0', memSize);
                }
            }
        }
        else {
            synchSem.Wait(TTInfiniteWait);
        }
    }

    return err.ErrorsCleared();
}

bool NI6259ADC::Initialise(StructuredDataI &data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("SamplingFrequency", samplingFrequency);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The SamplingFrequency shall be specified");
        }
    }
    if (ok) {
        ok = (samplingFrequency > 0u) && (samplingFrequency <= ADC_ONE_CH_MAX_FREQ);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "SamplingFrequency must be (< 1.25 MHz) and (> 0)");
        }
    }
    if (ok) {
        ok = ((ADC_BASE_FREQ % samplingFrequency) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "SamplingFrequency must divide exactly the ADC base clock (20 MHz)");
        }
    }
    if (ok) {
        if (!data.Read("ConvertFrequency", convertFrequency)) {
            REPORT_ERROR(ErrorManagement::Information, "ConvertFrequency not specified. Setting same as SamplingFrequency (%d)", samplingFrequency);
            convertFrequency = samplingFrequency;
        }
    }
    if (ok) {
        ok = ((ADC_BASE_FREQ % convertFrequency) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "ConvertFrequency must divide exactly the ADC base clock (20 MHz)");
        }
    }
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
        if (!data.Read("SampleDelayDivisor", sampleDelayDivisor)) {
            if (!data.Read("DelayDivisor", sampleDelayDivisor)) {
                sampleDelayDivisor = 3u;
                REPORT_ERROR(ErrorManagement::Information, "SampleDelayDivisor not specified. Setting default %d", sampleDelayDivisor);
            }
        }
        if (!data.Read("ConvertDelayDivisor", convertDelayDivisor)) {
            if (!data.Read("DelayDivisor", convertDelayDivisor)) {

                convertDelayDivisor = 3u;
                REPORT_ERROR(ErrorManagement::Information, "ConvertDelayDivisor not specified. Setting default %d", convertDelayDivisor);
            }
        }
    }
    if (ok) {
        StreamString synchTypeStr;
        if (!data.Read("ExecutionMode", synchTypeStr)) {
            synchTypeStr = "IndependentThread";
            REPORT_ERROR(ErrorManagement::Information, "AcquireOn not specified. Setting default %s", synchTypeStr.Buffer());
        }
        if (synchTypeStr == "IndependentThread") {
            synchType = INDEPENDENT_THREAD;
        }
        else if (synchTypeStr == "RealTimeThread") {
            synchType = REAL_TIME_THREAD;
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "Invalid value of AcquireOn. Possible values are IndependentThread and RealTimeThread");
            ok = false;
        }
    }
    if (ok) {
        uint8 calibrateU8;
        if (!data.Read("Calibrate", calibrateU8)) {
            calibrateU8 = 0u;
            REPORT_ERROR(ErrorManagement::Information, "Calibrate not specified. Using default %d", calibrate);
        }
        calibrate = (calibrateU8 > 0u);
    }
    StreamString clockSampleSourceStr;
    if (ok) {
        if (!data.Read("ClockSampleSource", clockSampleSourceStr)) {
            clockSampleSourceStr = "SI_TC";
            REPORT_ERROR(ErrorManagement::Information, "ClockSampleSource not specified. Set default to %s", clockSampleSourceStr.Buffer());
        }
        if (clockSampleSourceStr == "SI_TC") {
            clockSampleSource = AI_SAMPLE_SELECT_SI_TC;
        }
        else if (clockSampleSourceStr == "PFI0") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI0;
        }
        else if (clockSampleSourceStr == "PFI1") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI1;
        }
        else if (clockSampleSourceStr == "PFI2") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI2;
        }
        else if (clockSampleSourceStr == "PFI3") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI3;
        }
        else if (clockSampleSourceStr == "PFI4") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI4;
        }
        else if (clockSampleSourceStr == "PFI5") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI5;
        }
        else if (clockSampleSourceStr == "PFI6") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI6;
        }
        else if (clockSampleSourceStr == "PFI7") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI7;
        }
        else if (clockSampleSourceStr == "PFI8") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI8;
        }
        else if (clockSampleSourceStr == "PFI9") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI9;
        }
        else if (clockSampleSourceStr == "PFI10") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI10;
        }
        else if (clockSampleSourceStr == "PFI11") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI11;
        }
        else if (clockSampleSourceStr == "PFI12") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI12;
        }
        else if (clockSampleSourceStr == "PFI13") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI13;
        }
        else if (clockSampleSourceStr == "PFI14") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI14;
        }
        else if (clockSampleSourceStr == "PFI15") {
            clockSampleSource = AI_SAMPLE_SELECT_PFI15;
        }
        else if (clockSampleSourceStr == "RTSI0") {
            clockSampleSource = AI_SAMPLE_SELECT_RTSI0;
        }
        else if (clockSampleSourceStr == "RTSI1") {
            clockSampleSource = AI_SAMPLE_SELECT_RTSI1;
        }
        else if (clockSampleSourceStr == "RTSI2") {
            clockSampleSource = AI_SAMPLE_SELECT_RTSI2;
        }
        else if (clockSampleSourceStr == "RTSI3") {
            clockSampleSource = AI_SAMPLE_SELECT_RTSI3;
        }
        else if (clockSampleSourceStr == "RTSI4") {
            clockSampleSource = AI_SAMPLE_SELECT_RTSI4;
        }
        else if (clockSampleSourceStr == "RTSI5") {
            clockSampleSource = AI_SAMPLE_SELECT_RTSI5;
        }
        else if (clockSampleSourceStr == "RTSI6") {
            clockSampleSource = AI_SAMPLE_SELECT_RTSI6;
        }
        else if (clockSampleSourceStr == "RTSI7") {
            clockSampleSource = AI_SAMPLE_SELECT_RTSI7;
        }
        else if (clockSampleSourceStr == "PULSE") {
            clockSampleSource = AI_SAMPLE_SELECT_PULSE;
        }
        else if (clockSampleSourceStr == "GPCRT0_OUT") {
            clockSampleSource = AI_SAMPLE_SELECT_GPCRT0_OUT;
        }
        else if (clockSampleSourceStr == "STAR_TRIGGER") {
            clockSampleSource = AI_SAMPLE_SELECT_STAR_TRIGGER;
        }
        else if (clockSampleSourceStr == "GPCTR1_OUT") {
            clockSampleSource = AI_SAMPLE_SELECT_GPCTR1_OUT;
        }
        else if (clockSampleSourceStr == "SCXI_TRIG1") {
            clockSampleSource = AI_SAMPLE_SELECT_SCXI_TRIG1;
        }
        else if (clockSampleSourceStr == "ANALOG_TRIGGER") {
            clockSampleSource = AI_SAMPLE_SELECT_ANALOG_TRIGGER;
        }
        else if (clockSampleSourceStr == "LOW") {
            clockSampleSource = AI_SAMPLE_SELECT_LOW;
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockSampleSource");
        }
    }
    StreamString clockSamplePolarityStr;
    if (ok) {
        if (!data.Read("ClockSamplePolarity", clockSamplePolarityStr)) {
            clockSamplePolarityStr = "ACTIVE_HIGH_OR_RISING_EDGE";
            REPORT_ERROR(ErrorManagement::Information, "The ClockSamplePolarity not specified. Set default to %s", clockSamplePolarityStr.Buffer());
        }
        if (clockSamplePolarityStr == "ACTIVE_HIGH_OR_RISING_EDGE") {
            clockSamplePolarity = AI_SAMPLE_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE;
        }
        else if (clockSamplePolarityStr == "ACTIVE_LOW_OR_FALLING_EDGE") {
            clockSamplePolarity = AI_SAMPLE_POLARITY_ACTIVE_LOW_OR_FALLING_EDGE;
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockSamplePolarity");
        }
    }
    StreamString clockConvertSourceStr;
    if (ok) {
        if (!data.Read("ClockConvertSource", clockConvertSourceStr)) {
            clockConvertSourceStr = "SI2TC";
            REPORT_ERROR(ErrorManagement::Information, "The ClockConvertSource not specified. Set default to %s", clockConvertSourceStr.Buffer());
        }
        if (clockConvertSourceStr == "SI2TC") {
            clockConvertSource = AI_CONVERT_SELECT_SI2TC;
        }
        else if (clockConvertSourceStr == "PFI0") {
            clockConvertSource = AI_CONVERT_SELECT_PFI0;
        }
        else if (clockConvertSourceStr == "PFI1") {
            clockConvertSource = AI_CONVERT_SELECT_PFI1;
        }
        else if (clockConvertSourceStr == "PFI2") {
            clockConvertSource = AI_CONVERT_SELECT_PFI2;
        }
        else if (clockConvertSourceStr == "PFI3") {
            clockConvertSource = AI_CONVERT_SELECT_PFI3;
        }
        else if (clockConvertSourceStr == "PFI4") {
            clockConvertSource = AI_CONVERT_SELECT_PFI4;
        }
        else if (clockConvertSourceStr == "PFI5") {
            clockConvertSource = AI_CONVERT_SELECT_PFI5;
        }
        else if (clockConvertSourceStr == "PFI6") {
            clockConvertSource = AI_CONVERT_SELECT_PFI6;
        }
        else if (clockConvertSourceStr == "PFI7") {
            clockConvertSource = AI_CONVERT_SELECT_PFI7;
        }
        else if (clockConvertSourceStr == "PFI8") {
            clockConvertSource = AI_CONVERT_SELECT_PFI8;
        }
        else if (clockConvertSourceStr == "PFI9") {
            clockConvertSource = AI_CONVERT_SELECT_PFI9;
        }
        else if (clockConvertSourceStr == "PFI10") {
            clockConvertSource = AI_CONVERT_SELECT_PFI10;
        }
        else if (clockConvertSourceStr == "PFI11") {
            clockConvertSource = AI_CONVERT_SELECT_PFI11;
        }
        else if (clockConvertSourceStr == "PFI12") {
            clockConvertSource = AI_CONVERT_SELECT_PFI12;
        }
        else if (clockConvertSourceStr == "PFI13") {
            clockConvertSource = AI_CONVERT_SELECT_PFI13;
        }
        else if (clockConvertSourceStr == "PFI14") {
            clockConvertSource = AI_CONVERT_SELECT_PFI14;
        }
        else if (clockConvertSourceStr == "PFI15") {
            clockConvertSource = AI_CONVERT_SELECT_PFI15;
        }
        else if (clockConvertSourceStr == "RTSI0") {
            clockConvertSource = AI_CONVERT_SELECT_RTSI0;
        }
        else if (clockConvertSourceStr == "RTSI1") {
            clockConvertSource = AI_CONVERT_SELECT_RTSI1;
        }
        else if (clockConvertSourceStr == "RTSI2") {
            clockConvertSource = AI_CONVERT_SELECT_RTSI2;
        }
        else if (clockConvertSourceStr == "RTSI3") {
            clockConvertSource = AI_CONVERT_SELECT_RTSI3;
        }
        else if (clockConvertSourceStr == "RTSI4") {
            clockConvertSource = AI_CONVERT_SELECT_RTSI4;
        }
        else if (clockConvertSourceStr == "RTSI5") {
            clockConvertSource = AI_CONVERT_SELECT_RTSI5;
        }
        else if (clockConvertSourceStr == "RTSI6") {
            clockConvertSource = AI_CONVERT_SELECT_RTSI6;
        }
        else if (clockConvertSourceStr == "RTSI7") {
            clockConvertSource = AI_CONVERT_SELECT_RTSI7;
        }
        else if (clockConvertSourceStr == "GPCRT0_OUT") {
            clockConvertSource = AI_CONVERT_SELECT_GPCRT0_OUT;
        }
        else if (clockConvertSourceStr == "STAR_TRIGGER") {
            clockConvertSource = AI_CONVERT_SELECT_STAR_TRIGGER;
        }
        else if (clockConvertSourceStr == "ANALOG_TRIGGER") {
            clockConvertSource = AI_CONVERT_SELECT_ANALOG_TRIGGER;
        }
        else if (clockConvertSourceStr == "LOW") {
            clockConvertSource = AI_CONVERT_SELECT_LOW;
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockConvertSource");
        }
    }
    StreamString clockConvertPolarityStr;
    if (ok) {
        if (!data.Read("ClockConvertPolarity", clockConvertPolarityStr)) {
            clockConvertPolarityStr = "RISING_EDGE";
            REPORT_ERROR(ErrorManagement::Information, "The ClockConvertPolarity not specified. Set default to %s", clockConvertPolarityStr.Buffer());
        }
        if (clockConvertPolarityStr == "RISING_EDGE") {
            clockConvertPolarity = AI_CONVERT_POLARITY_RISING_EDGE;
        }
        else if (clockConvertPolarityStr == "FALLING_EDGE") {
            clockConvertPolarity = AI_CONVERT_POLARITY_FALLING_EDGE;
        }
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockConvertPolarity");
        }
    }

    if (ok) {
        if (!data.Read("CPUs", cpuMask)) {
            REPORT_ERROR(ErrorManagement::Information, "No CPUs defined for %s", GetName());
        }
    }
//Get individual signal parameters
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

        bool firstSignal = true;
        uint32 i = 0u;
        while ((i < (NI6259ADC_MAX_CHANNELS + NI6259ADC_HEADER_SIZE)) && (ok)) {
            if (data.MoveRelative(data.GetChildName(i))) {
                uint32 channelId;
                if (data.Read("ChannelId", channelId)) {
                    ok = (channelId < NI6259ADC_MAX_CHANNELS);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Invalid ChannelId specified.");
                    }
                    if (ok) {
                        adcEnabled[channelId] = true;
                        float32 range;
                        numberOfADCsEnabled++;
                        if (data.Read("InputRange", range)) {
                            if ((range > 9.99) && (range < 10.01)) {
                                inputRange[channelId] = 1u;
                            }
                            else if ((range > 4.99) && (range < 5.01)) {
                                inputRange[channelId] = 2u;
                            }
                            else if ((range > 1.99) && (range < 2.01)) {
                                inputRange[channelId] = 3u;
                            }
                            else if ((range > 0.99) && (range < 1.01)) {
                                inputRange[channelId] = 4u;
                            }
                            else if ((range > 0.499) && (range < 0.501)) {
                                inputRange[channelId] = 5u;
                            }
                            else if ((range > 0.199) && (range < 0.201)) {
                                inputRange[channelId] = 6u;
                            }
                            else if ((range > 0.099) && (range < 0.101)) {
                                inputRange[channelId] = 7u;
                            }
                            else {
                                ok = false;
                                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported InputRange.");
                            }
                        }
                        StreamString polarity;
                        if (data.Read("InputPolarity", polarity)) {
                            if (polarity == "Unipolar") {
                                inputPolarity[channelId] = AI_POLARITY_UNIPOLAR;
                            }
                            else if (polarity == "Bipolar") {
                                inputPolarity[channelId] = AI_POLARITY_BIPOLAR;
                            }
                            else {
                                ok = false;
                                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported InputPolarity.");
                            }
                        }
                        StreamString mode;
                        if (data.Read("InputMode", mode)) {
                            if (mode == "Differential") {
                                inputMode[channelId] = AI_CHANNEL_TYPE_DIFFERENTIAL;
                            }
                            else if (mode == "NRSE") {
                                inputMode[channelId] = AI_CHANNEL_TYPE_NRSE;
                            }
                            else if (mode == "RSE") {
                                inputMode[channelId] = AI_CHANNEL_TYPE_RSE;
                            }
                            else {
                                ok = false;
                                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported InputMode.");
                            }
                        }
                    }
                }
                else {
                    //get additional attributes of counter
                    if (firstSignal) {
                        uint8 countSamplesU8 = 0u;
                        if (!data.Read("CountSamples", countSamplesU8)) {
                            countSamplesU8 = 0u;
                        }
                        countSamples = (countSamplesU8 > 0u);
                        uint8 resetOnBufferChangeU8 = 0u;
                        if (!data.Read("ResetOnBufferChange", resetOnBufferChangeU8)) {
                            resetOnBufferChangeU8 = 0u;
                        }
                        resetOnBufferChange = (resetOnBufferChangeU8 > 0u);
                        if (resetOnBufferChange) {
                            countSamples = true;
                        }
                        firstSignal = false;
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
        if (ok) {
            ok = data.MoveToAncestor(1u);
        }
    }
    if (ok) {
        if (data.MoveRelative("Attributes")) {
            attributes.Write("Attributes", data);
            data.MoveToAncestor(1u);
        }
    }
    if (ok) {
        ok = ((numberOfADCsEnabled > 0) && (samplingFrequency <= ADC_MORE_CH_MAX_FREQ));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The SamplingFrequency must be <= 1 MHz when more than one channel is defined");
        }
    }
    if (ok) {
        if (convertFrequency != samplingFrequency) {
            ok = ((samplingFrequency * numberOfADCsEnabled) < convertFrequency);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError,
                             "The convert clock frequency must be greater than the sample clock frequency multplied for the number of chennels");
            }
            singleADCFrequency = static_cast<float64>(samplingFrequency);
        }
        else {
            singleADCFrequency = (samplingFrequency / static_cast<float64>(numberOfADCsEnabled));
        }

    }

    return ok;
}

bool NI6259ADC::SetConfiguredDatabase(StructuredDataI &data) {
    uint32 i;
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = (GetNumberOfSignals() > (NI6259ADC_HEADER_SIZE));
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "At least (%d) signals shall be configured (header + 1 ADC)", NI6259ADC_HEADER_SIZE + 1u);
    }

    //The type of counter shall be unsigned int32 or uint32
    if (ok) {
        ok = ((GetSignalType(0u) == UnsignedInteger64Bit) || (GetSignalType(0u) == UnsignedInteger32Bit));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "The first signal (counter) shall be of type UnsignedInteger64Bit or UnsignedInteger32Bit (deprecated)");
        }
    }
    //The type of time shall be unsigned int32 or uint32
    if (ok) {
        ok = ((GetSignalType(1u) == UnsignedInteger64Bit) || (GetSignalType(0u) == UnsignedInteger32Bit));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "The second signal (time) shall be of type UnsignedInteger64Bit or UnsignedInteger32Bit (deprecated)");
        }
    }
    TypeDescriptor channelTd = (calibrate) ? (Float32Bit) : (SignedInteger16Bit);
    sampleSize = (calibrate) ? (sizeof(float32)) : (sizeof(int16));
    if (ok) {
        for (i = 0u; (i < numberOfADCsEnabled) && (ok); i++) {
            ok = (GetSignalType(NI6259ADC_HEADER_SIZE + i) == channelTd);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "All the ADC signals shall be of type %s", TypeDescriptor::GetTypeNameFromTypeDescriptor(channelTd));
        }
    }

    //The current code would not work with more than one function interacting with this DataSourceI (see GetLastBufferIdx).
    //TODO reimplement with CircularBuffer from the core.
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
    REPORT_ERROR(ErrorManagement::Information, "pxi6259_create_ai_conf()");
    pxi6259_ai_conf_t adcConfiguration = pxi6259_create_ai_conf();
    for (i = 0u; (i < NI6259ADC_MAX_CHANNELS) && (ok); i++) {
        if (adcEnabled[i]) {
            REPORT_ERROR(ErrorManagement::Information, "pxi6259_add_ai_channel(&adcConfiguration, %d, %d, %d, %d, 0)", i, inputPolarity[i], inputRange[i],
                         inputMode[i]);
            ok = (pxi6259_add_ai_channel(&adcConfiguration, static_cast<uint8_t>(i), inputPolarity[i], inputRange[i], inputMode[i], 0) == 0);
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "Channel %d set with input range %d", i, inputRange[i]);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not set InputRange for channel %d of device %s", i, fullDeviceName);
            }
        }
    }
    if (ok) {
        uint32 sampleClockDivisor = (ADC_BASE_FREQ / samplingFrequency);
        REPORT_ERROR(ErrorManagement::Information, "pxi6259_set_ai_sample_clk(&adcConfiguration, %d, %d, %d, %d)", sampleClockDivisor, sampleDelayDivisor,
                     clockSampleSource, clockSamplePolarity);

        ok = (pxi6259_set_ai_sample_clk(&adcConfiguration, sampleClockDivisor, sampleDelayDivisor, clockSampleSource, clockSamplePolarity) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not set the clock for device %s", fullDeviceName);
        }
    }
    if (ok) {
        uint32 convertClockDivisor = (ADC_BASE_FREQ / convertFrequency);
        REPORT_ERROR(ErrorManagement::Information, "pxi6259_set_ai_convert_clk(&adcConfiguration, %d, %d, %d, %d)", convertClockDivisor, convertDelayDivisor,
                     clockConvertSource, clockConvertPolarity);

        ok = (pxi6259_set_ai_convert_clk(&adcConfiguration, convertClockDivisor, convertDelayDivisor, clockConvertSource, clockConvertPolarity) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not set the convert clock for device %s", fullDeviceName);
        }
    }
    if (ok) {

        //Add additional attributes
        if (attributes.MoveAbsolute("Attributes")) {
            for (uint32 i = 0u; (i < attributes.GetNumberOfChildren()) && ok; i++) {
                StreamString attrName = attributes.GetChildName(i);
                uint32 attrVal;
                ok = attributes.Read(attrName.Buffer(), attrVal);
                if (ok) {
                    pxi6259_segment_attribute attribute = GetAttribute(attrName.Buffer());
                    ok = (attribute != AO_CONTINUOUS);
                    if (ok) {
                        REPORT_ERROR(ErrorManagement::Information, "pxi6259_set_ai_attribute(&adcConfiguration, %s, %d)", attrName.Buffer(), attrVal);

                        ok = (pxi6259_set_ai_attribute(&adcConfiguration, attribute, attrVal) == 0);
                        if (!ok) {
                            REPORT_ERROR(ErrorManagement::FatalError, "Failed to set additional attribute %s to %d", attrName.Buffer(), attrVal);
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Could not find additional attribute %s", attrName.Buffer());
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not read additional attribute %s", attrName.Buffer());
                }
            }
            ok = attributes.MoveToAncestor(1u);
        }
    }

    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "pxi6259_load_ai_conf(boardFileDescriptor, &adcConfiguration)");
        ok = (pxi6259_load_ai_conf(boardFileDescriptor, &adcConfiguration) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not load configuration for device %s", fullDeviceName);
        }
    }
    if (ok) {
        if (calibrate) {
            REPORT_ERROR(ErrorManagement::Information, "pxi6259_get_ai_coefficient(boardFileDescriptor, &ai_coefs_all)");
            ok = (pxi6259_get_ai_coefficient(boardFileDescriptor, &ai_coefs_all) == 0);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed to get AI coefficient");
            }
        }
    }
    if (ok) {
        //Allocate memory
        for (i = 0u; (i < NI6259ADC_MAX_CHANNELS) && (ok); i++) {
            uint32 b;
            for (b = 0u; (b < NUMBER_OF_BUFFERS) && (ok); b++) {
                channelsMemory[b][i] = new uint8[numberOfSamples * sampleSize];
            }
        }
    }

    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "pxi6259_dma_init(boardId)");
        dma = pxi6259_dma_init(static_cast<int32>(boardId));
        ok = (dma != NULL_PTR(struct pxi6259_dma*));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not set the dma for device %s", fullDeviceName);
        }
    }
    if (ok) {
        //lint -e{613} dma cannot be null as otherwise ok would be false
        if (dma->ai.count > 0u) {
            dmaReadBuffer = new int16[dma->ai.count];
            if (calibrate) {
                dmaCalibBuffer = new float32[dma->ai.count];
            }
        }
    }

    return ok;
}

ErrorManagement::ErrorType NI6259ADC::CopyFromDMA(const size_t numberOfSamplesFromDMA) {
    ErrorManagement::ErrorType err;
    uint32 s = 0u;
    if (dmaReadBuffer != NULL_PTR(int16*)) {
        (void) (counterResetFastMux.FastLock());
        while (s < (numberOfSamplesFromDMA)) {
            (void) allBufMux.FastLock(TTInfiniteWait, fastMuxSleepTime);

            uint8 currentBufferIdxCache = currentBufferIdx;
            Lock(currentBufferIdxCache);
            if (calibrate) {
                if (dmaCalibBuffer != NULL_PTR(float32*)) {
                    MemoryOperationsHelper::Copy(&channelsMemory[currentBufferIdx][dmaChannel][currentBufferOffset * sampleSize], &dmaCalibBuffer[s],
                                                 sampleSize);
                }
            }
            else {
                MemoryOperationsHelper::Copy(&channelsMemory[currentBufferIdx][dmaChannel][currentBufferOffset * sampleSize], &dmaReadBuffer[s], sampleSize);
            }
            s++;
            dmaChannel++;

            if (dmaChannel == numberOfADCsEnabled) {
                dmaChannel = 0u;
                counter++;

                if (resetOnBufferChange) {
                    //counter starts from 1
                    counterValue[currentBufferIdx] = (counter - lastCounter);
                }
                else {
                    counterValue[currentBufferIdx] = (countSamples) ? (counter) : (counter / numberOfSamples);
                }

                float64 counterSamples = static_cast<float64>(counter);
                counterSamples *= 1000000.;
                counterSamples /= singleADCFrequency;
                if (timeValue != NULL_PTR(uint64*)) {
                    timeValue[currentBufferIdx] = static_cast<uint64>(counterSamples);
                }
                currentBufferOffset++;

                if ((currentBufferOffset == numberOfSamples) || (changeBuffer)) {
                    changeBuffer = false;
                    lastCounter = counter;
                    (void) fastBufMux.FastLock(TTInfiniteWait, fastMuxSleepTime);
                    currentBufferOffset = 0u;
                    uint8 oldBuff = currentBufferIdx;
                    currentBufferIdx++;
                    if (currentBufferIdx == NUMBER_OF_BUFFERS) {
                        currentBufferIdx = 0u;
                    }
                    Lock(currentBufferIdx);
                    for (uint32 n = 0u; n < numberOfADCsEnabled; n++) {
                        uint32 memSize = sampleSize * numberOfSamples;
                        MemoryOperationsHelper::Set(&channelsMemory[currentBufferIdx][n][0], '\0', memSize);
                    }
                    counterValue[currentBufferIdx] = 0ull;
                    timeValue[currentBufferIdx] = timeValue[oldBuff];
                    UnLock(currentBufferIdx);
                    err = !synchSem.Post();
                    fastBufMux.FastUnLock();
                }
            }
            UnLock(currentBufferIdxCache);
            allBufMux.FastUnLock();

        }
        counterResetFastMux.FastUnLock();
    }
    return err;
}

ErrorManagement::ErrorType NI6259ADC::Execute(ExecutionInfo &info) {
    ErrorManagement::ErrorType err;
    if (info.GetStage() == ExecutionInfo::TerminationStage) {
        keepRunning = false;
    }
    else if (info.GetStage() == ExecutionInfo::StartupStage) {
        //Empty DMA buffer
        //Does not work. The returned nBytesInDMA is always > 0...
#if 0
        size_t nBytesInDMA = pxi6259_dma_samples_in_buffer(dma, dmaOffset);
        if ((dma != NULL_PTR(struct pxi6259_dma *)) && (numberOfADCsEnabled > 0u)) {
            while (nBytesInDMA > 0u) {
                dmaOffset = dmaOffset + nBytesInDMA;
                dmaOffset %= dma->ai.count;
                dmaChannel = (dma->ai.count % numberOfADCsEnabled);
                nBytesInDMA = pxi6259_dma_samples_in_buffer(dma, dmaOffset);
            }
        }
#endif
    }
    else {
        if ((dma != NULL_PTR(struct pxi6259_dma*)) && (dmaReadBuffer != NULL_PTR(int16*))) {
            size_t nSamplesInDMA = pxi6259_dma_samples_in_buffer(dma, static_cast<off_t>(dmaOffset));

            bool skip = (nSamplesInDMA == (dma->ai.count - 1u));

            if (!skip) {
                nSamplesInDMA = (nSamplesInDMA / numberOfADCsEnabled) * numberOfADCsEnabled;

                //REPORT_ERROR(ErrorManagement::Information, "Read %d samples", (uint32 )nSamplesInDMA);
                if (nSamplesInDMA > 0u) {
                    if (nSamplesInDMA > dma->ai.count) {
                        REPORT_ERROR(ErrorManagement::FatalError, "Overflow while reading from the ADC");
                    }
                    else {
                        if ((dmaOffset + nSamplesInDMA) > (dma->ai.count)) {
                            //Right part of the DMA
                            size_t samplesToCopy = (dma->ai.count - dmaOffset);
                            //Roll to the beginning
                            size_t samplesToCopyRoll = ((dmaOffset + nSamplesInDMA) - dma->ai.count);
                            MemoryOperationsHelper::Copy(&dmaReadBuffer[0], &dma->ai.data[dmaOffset], (sizeof(int16) * samplesToCopy));
                            MemoryOperationsHelper::Copy(&dmaReadBuffer[samplesToCopy], &dma->ai.data[0], (sizeof(int16) * samplesToCopyRoll));
                        }
                        else {
                            MemoryOperationsHelper::Copy(&dmaReadBuffer[0], &dma->ai.data[dmaOffset], (sizeof(int16) * nSamplesInDMA));
                        }

                        if (calibrate && (dmaCalibBuffer != NULL_PTR(float32*))) {
                            MemoryOperationsHelper::Copy(&dmaCalibBuffer[nSamplesInDMA / 2], &dmaReadBuffer[0], (sizeof(int16) * nSamplesInDMA));
                            pxi6259_calibration_ai(dmaCalibBuffer, nSamplesInDMA, 1, &ai_coefs_all);
                        }

                        err = CopyFromDMA(nSamplesInDMA);
                    }
                    dmaOffset = dmaOffset + nSamplesInDMA;
                    dmaOffset %= dma->ai.count;
                }
            }
        }
        sched_yield();
    }
    return err;
}

bool NI6259ADC::ReadAIConfiguration(pxi6259_ai_conf_t *const conf) const {
    bool ok = false;
    if (boardFileDescriptor > 0) {
        ok = (pxi6259_read_ai_conf(boardFileDescriptor, conf) == 0);
    }
    return ok;
}

uint8 NI6259ADC::GetLastBufferIdx() {
    uint8 toReadIdx = lastBufferIdx;

    //This is thread safe since this is executed in the context of the thread which calls Synchronise.
    lastBufferIdx++;
    if (lastBufferIdx == NUMBER_OF_BUFFERS) {
        lastBufferIdx = 0u;
    }
    return toReadIdx;
}

bool NI6259ADC::IsSynchronising() const {
    return synchronising;
}

void NI6259ADC::Lock(const uint32 idx) {
    (void) fastMux[idx].FastLock(TTInfiniteWait, fastMuxSleepTime);
}

void NI6259ADC::UnLock(const uint32 idx) {
    fastMux[idx].FastUnLock();
}

CLASS_REGISTER(NI6259ADC, "1.0")
}

