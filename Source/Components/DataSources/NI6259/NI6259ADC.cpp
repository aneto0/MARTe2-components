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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "NI6259ADC.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
NI6259ADC::NI6259ADC() :
        DataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this) {
    cycleFrequency = 0.F;
    numberOfSamples = 0u;
    boardId = 0u;
    samplingFrequency = 0u;
    boardFileDescriptor = -1;
    deviceName = "";
    counter = 0u;
    counterValue = 0u;
    timeValue = 0u;
    delayDivisor = 0u;
    samplingPeriodMicroSeconds = 0u;
    numberOfADCsEnabled = 0u;
    clockSampleSource = AI_SAMPLE_SELECT_SI_TC;
    clockSamplePolarity = AI_SAMPLE_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE;
    clockConvertSource = AI_CONVERT_SELECT_SI2TC;
    clockConvertPolarity = AI_CONVERT_POLARITY_RISING_EDGE;
    keepRunning = true;
    synchronising = false;
    cpuMask = 0u;
    counterResetFastMux.Create();
    uint32 n;
    for (n = 0u; n < NI6259ADC_MAX_CHANNELS; n++) {
        inputRange[n] = 1u;
        inputMode[n] = AI_CHANNEL_TYPE_RSE;
        inputPolarity[n] = AI_POLARITY_UNIPOLAR;
        adcEnabled[n] = false;
        channelsFileDescriptors[n] = -1;
        channelsMemory[n] = NULL_PTR(float32 *);
    }
    channelMemory = NULL_PTR(float32 *);
    if (!synchSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create EventSem.");
    }
}

/*lint -e{1551} the destructor must guarantee that the NI6259ADC SingleThreadService is stopped and that all the file descriptors are closed.*/
NI6259ADC::~NI6259ADC() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    uint32 n;
    if (boardFileDescriptor != -1) {
        if (pxi6259_stop_ai(boardFileDescriptor) < 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop acquisition.");
        }
    }
    for (n = 0u; n < NI6259ADC_MAX_CHANNELS; n++) {
        if (channelsFileDescriptors[n] != -1) {
            close(channelsFileDescriptors[n]);
        }
    }
    if (boardFileDescriptor != -1) {
        close(boardFileDescriptor);
    }
    for (n = 0u; n < NI6259ADC_MAX_CHANNELS; n++) {
        if (channelsMemory[n] != NULL_PTR(float32 *)) {
            delete[] channelsMemory[n];
        }
    }
    if (channelMemory != NULL_PTR(float32 *)) {
        delete[] channelMemory;
    }
}

bool NI6259ADC::AllocateMemory() {
    return true;
}

uint32 NI6259ADC::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool NI6259ADC::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (signalIdx < (NI6259ADC_MAX_CHANNELS + NI6259ADC_HEADER_SIZE));
    if (ok) {
        if (signalIdx == 0u) {
            signalAddress = reinterpret_cast<void *>(&counterValue);
        }
        else if (signalIdx == 1u) {
            signalAddress = reinterpret_cast<void *>(&timeValue);
        }
        else {
            signalAddress = &(channelsMemory[signalIdx - NI6259ADC_HEADER_SIZE][0]);
        }
    }
    return ok;
}

const char8* NI6259ADC::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == InputSignals) {
        float32 frequency = 0.F;
        if (!data.Read("Frequency", frequency)) {
            frequency = -1.F;
        }

        if (frequency > 0.F) {
            brokerName = "MemoryMapSynchronisedInputBroker";
            cycleFrequency = frequency;
            synchronising = true;
        }
        else {
            brokerName = "MemoryMapInputBroker";
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with OutputSignals");
    }
    return brokerName;
}

bool NI6259ADC::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    //Check if this function has a synchronisation point (i.e. a signal which has Frequency > 0)
    uint32 functionIdx = 0u;
    uint32 nOfSignals = 0u;

    bool synchGAM = false;
    bool ok = GetFunctionIndex(functionIdx, functionName);
    if (ok) {
        ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfSignals);
    }

    uint32 i;
    float32 frequency = 0.F;
    for (i = 0u; (i < nOfSignals) && (ok) && (!synchGAM); i++) {
        ok = GetFunctionSignalReadFrequency(InputSignals, functionIdx, i, frequency);
        synchGAM = (frequency > 0.F);
    }
    if ((synchronising) && (synchGAM)) {
        ReferenceT<MemoryMapSynchronisedInputBroker> brokerSync("MemoryMapSynchronisedInputBroker");
        if (ok) {
            ok = brokerSync.IsValid();
        }
        if (ok) {
            ok = brokerSync->Init(InputSignals, *this, functionName, gamMemPtr);
        }
        if (ok) {
            ok = inputBrokers.Insert(brokerSync);
        }
        uint32 nOfFunctionSignals = 0u;
        if (ok) {
            ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfFunctionSignals);
        }
        //Must also add the signals which are not synchronous but that belong to the same GAM...
        if (ok) {
            if (nOfFunctionSignals > 1u) {
                ReferenceT<MemoryMapInputBroker> brokerNotSync("MemoryMapInputBroker");
                ok = brokerNotSync.IsValid();
                if (ok) {
                    ok = brokerNotSync->Init(InputSignals, *this, functionName, gamMemPtr);
                }
                if (ok) {
                    ok = inputBrokers.Insert(brokerNotSync);
                }
            }
        }
    }
    else {
        ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
        ok = broker.IsValid();
        if (ok) {
            ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
        }
        if (ok) {
            ok = inputBrokers.Insert(broker);
        }
    }

    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: returns false irrespectively of the input parameters.*/
bool NI6259ADC::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

bool NI6259ADC::Synchronise() {
    ErrorManagement::ErrorType err;
    if (synchronising) {
        err = synchSem.ResetWait(TTInfiniteWait);
    }
    return err.ErrorsCleared();
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the counter and the timer are always reset irrespectively of the states being changed.*/
bool NI6259ADC::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    (void) counterResetFastMux.FastLock();
    counter = 0u;
    counterValue = 0u;
    timeValue = 0u;
    counterResetFastMux.FastUnLock();
    bool ok = true;
    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
        keepRunning = true;
        if (cpuMask != 0u) {
            executor.SetCPUMask(cpuMask);
        }
        ok = executor.Start();
    }
    return ok;
}

bool NI6259ADC::Initialise(StructuredDataI& data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("SamplingFrequency", samplingFrequency);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The SamplingFrequency shall be specified");
        }
    }
    if (ok) {
        ok = (samplingFrequency <= 1000000u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "SamplingFrequency must be < 1 MHz");
        }
    }
    if (ok) {
        if (samplingFrequency != 0u) {
            samplingPeriodMicroSeconds = 1000000u / samplingFrequency;
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "SamplingFrequency cannot be zero");
            ok = false;
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
        ok = data.Read("DelayDivisor", delayDivisor);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The DelayDivisor shall be specified");
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
        ok = data.Read("ClockSamplePolarity", clockSamplePolarityStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ClockSamplePolarity shall be specified");
        }
    }
    if (ok) {
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
        ok = data.Read("ClockConvertSource", clockConvertSourceStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ClockConvertSource shall be specified");
        }
    }
    if (ok) {
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
        ok = data.Read("ClockConvertPolarity", clockConvertPolarityStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ClockConvertPolarity shall be specified");
        }
    }
    if (ok) {
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

bool NI6259ADC::SetConfiguredDatabase(StructuredDataI& data) {
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
            ok = (GetSignalType(NI6259ADC_HEADER_SIZE + i) == Float32Bit);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "All the ADC signals shall be of type Float32Bit");
        }
    }

    uint32 nOfFunctions = GetNumberOfFunctions();
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
            //numberOfADCsEnabled > 0 as otherwise it would be stopped
            if (numberOfADCsEnabled > 0u) {
                uint32 singleADCFrequency = samplingFrequency / numberOfADCsEnabled;
                float32 totalNumberOfSamplesPerSecond = (static_cast<float32>(numberOfSamples) * cycleFrequency);
                ok = (singleADCFrequency == static_cast<uint32>(totalNumberOfSamplesPerSecond));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "singleADCFrequency (%u) shall be equal to numberOfSamples * cycleFrequency (%u)", singleADCFrequency, totalNumberOfSamplesPerSecond);
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
    pxi6259_ai_conf_t adcConfiguration = pxi6259_create_ai_conf();
    for (i = 0u; (i < NI6259ADC_MAX_CHANNELS) && (ok); i++) {
        if (adcEnabled[i]) {
            ok = (pxi6259_add_ai_channel(&adcConfiguration, static_cast<uint8_t>(i), inputPolarity[i], inputRange[i], inputMode[i], 0u) == 0);
            uint32 ii = i;
            if (ok) {
                REPORT_ERROR(ErrorManagement::Information, "Channel %d set with input range %d", ii, inputRange[i]);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not set InputRange for channel %d of device %s", ii, fullDeviceName);
                    }
                }
            }
    if (ok) {
        if (numberOfADCsEnabled == 1u) {
            ok = (pxi6259_set_ai_convert_clk(&adcConfiguration, 16u, delayDivisor, clockConvertSource, clockConvertPolarity) == 0);
        }
        else {
            ok = (pxi6259_set_ai_convert_clk(&adcConfiguration, 20u, delayDivisor, clockConvertSource, clockConvertPolarity) == 0);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not set the convert clock for device %s", fullDeviceName);
        }
    }
    if (ok) {
        if (numberOfADCsEnabled == 1u) {
            ok = (pxi6259_set_ai_sample_clk(&adcConfiguration, 16u, delayDivisor, clockSampleSource, clockSamplePolarity) == 0);
        }
        else {
            ok = (pxi6259_set_ai_sample_clk(&adcConfiguration, 20u, delayDivisor, clockSampleSource, clockSamplePolarity) == 0);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not set the clock for device %s", fullDeviceName);
        }
    }
    if (ok) {
        ok = (pxi6259_load_ai_conf(boardFileDescriptor, &adcConfiguration) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not load configuration for device %s", fullDeviceName);
        }
    }
    if (ok) {
        //Allocate memory
        for (i = 0u; (i < NI6259ADC_MAX_CHANNELS) && (ok); i++) {
            channelsMemory[i] = new float32[numberOfSamples];
        }
        channelMemory = new float32[numberOfSamples];
    }

    if (ok) {
        //Required to wait for devices to be available in /dev!
        Sleep::Sec(1.0);
        for (i = 0u; (i < NI6259ADC_MAX_CHANNELS) && (ok); i++) {
            if (adcEnabled[i]) {
                StreamString channelDeviceName;
                //Otherwise there is the perception that the Printf might modify i inside the for loop
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
        ok = (pxi6259_start_ai(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not start the device %s", fullDeviceName);
        };
    }
    return ok;
}

ErrorManagement::ErrorType NI6259ADC::Execute(const ExecutionInfo& info) {
    ErrorManagement::ErrorType err;
    (void) counterResetFastMux.FastLock();
    if (info.GetStage() == ExecutionInfo::TerminationStage) {
        keepRunning = false;
    }
    else {
        uint32 i;
        for (i = 0u; (i < NI6259ADC_MAX_CHANNELS) && (keepRunning); i++) {
            if (adcEnabled[i]) {
                size_t readSamples = 0u;
                while ((readSamples < numberOfSamples) && (keepRunning)) {
                    size_t leftSamples = static_cast<size_t>(numberOfSamples) - readSamples;
                    //ssize_t currentSamples = pxi6259_read_ai(channelsFileDescriptors[i], &(channelsMemory[i][readSamples]), leftSamples);
                    //Unfortunately a buffered memory has to be used to read from the ADC. Using directly channelsMemory[i][readSamples] was
                    //corrupting the memory if this was being copied by the broker while pxi6259_read_ai was being called.
                    if (channelMemory != NULL_PTR(float32 *)) {
                        ssize_t currentSamples = pxi6259_read_ai(channelsFileDescriptors[i], &(channelMemory[readSamples]), leftSamples);
                        if (currentSamples > 0) {
                            readSamples += static_cast<size_t>(currentSamples);
                            //Needs to sleep while waiting for data, otherwise it will get stuck on pxi6259_read_ai.
                            //Do not try to further optimise this (e.g. by only sleeping once or so, as it will stop working!).
                            if (i == 0u) {
                                Sleep::Sec(20e-6);
                            }
                        }
                        else {
                            Sleep::Sec(100e-6);
                            REPORT_ERROR(ErrorManagement::ParametersError, "Failed reading from ADC");
                        }
                    }
                }
            }
            if (keepRunning) {
                keepRunning = MemoryOperationsHelper::Copy(channelsMemory[i], channelMemory, numberOfSamples * static_cast<uint32>(sizeof(float32)));
            }
        }
        counterValue = counter;
        timeValue = counter * numberOfSamples * samplingPeriodMicroSeconds;
        if (synchronising) {
            err = !synchSem.Post();
        }
        counter++;
    }
    counterResetFastMux.FastUnLock();
    return err;
}

bool NI6259ADC::ReadAIConfiguration(pxi6259_ai_conf_t * const conf) const {
    bool ok = false;
    if (boardFileDescriptor > 0) {
        ok = (pxi6259_read_ai_conf(boardFileDescriptor, conf) == 0);
    }
    return ok;
}

CLASS_REGISTER(NI6259ADC, "1.0")
}

