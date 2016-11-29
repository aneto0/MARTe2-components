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
    synchronisingFunctionIdx = 0u;
    numberOfSamples = 0u;
    boardId = 0u;
    samplingFrequency = 0u;
    boardFileDescriptor = -1;
    deviceName = "";
    counter = 0u;
    time = 0u;
    delayDivisor = 0u;
    samplingPeriodMicroSeconds = 0u;
    clockSource = AI_SAMPLE_SELECT_SI_TC;
    clockPolarity = AI_SAMPLE_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE;
    keepRunning = true;
    uint32 n;
    for (n = 0u; n < NI6259ADC_CHANNELS; n++) {
        inputRange[n] = 1u;
        inputMode[n] = AI_CHANNEL_TYPE_RSE;
        inputPolarity[n] = AI_POLARITY_UNIPOLAR;
        channelsFileDescriptors[n] = -1;
        channelsMemory[n] = NULL_PTR(float32 *);
    }
    if (!synchSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create EventSem.");
    }
}

NI6259ADC::~NI6259ADC() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    uint32 n;
    for (n = 0u; n < NI6259ADC_CHANNELS; n++) {
        if (channelsFileDescriptors[n] != -1) {
            if (pxi6259_stop_ai(channelsFileDescriptors[n]) < 0) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not stop channel acquisition.");
            }
            close(channelsFileDescriptors[n]);
        }
    }
    for (n = 0u; n < NI6259ADC_CHANNELS; n++) {
        if (channelsMemory[n] != NULL_PTR(float32 *)) {
            delete[] channelsMemory[n];
        }
    }
}

bool NI6259ADC::AllocateMemory() {
    return true;
}

uint32 NI6259ADC::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool NI6259ADC::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (signalIdx < (NI6259ADC_CHANNELS + NI6259ADC_HEADER_SIZE));
    if (ok) {
        if (signalIdx == 0u) {
            signalAddress = reinterpret_cast<void *&>(counter);
        }
        else if (signalIdx == 1u) {
            signalAddress = reinterpret_cast<void *&>(time);
        }
        else {
            signalAddress = channelsMemory[signalIdx - NI6259ADC_HEADER_SIZE];
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
    bool ok = GetFunctionIndex(functionIdx, functionName);

    if (synchronisingFunctionIdx == functionIdx) {
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

bool NI6259ADC::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

bool NI6259ADC::Synchronise() {
    ErrorManagement::ErrorType err = synchSem.ResetWait(TTInfiniteWait);
    return err.ErrorsCleared();
}

bool NI6259ADC::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    counter = 0u;
    time = 0u;
    bool ok = true;
    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
        keepRunning = true;
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
        ok = (samplingFrequency != 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "SamplingFrequency cannot be zero");
        }
    }
    if (ok) {
        ok = (samplingFrequency <= 1000000u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "SamplingFrequency must be < 1 MHz");
        }
    }
    if (ok) {
        samplingPeriodMicroSeconds = 1000000u / samplingFrequency;

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
    StreamString clockSourceStr;
    if (ok) {
        ok = data.Read("ClockSource", clockSourceStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ClockSource shall be specified");
        }
    }
    if (ok) {
        if (clockSourceStr == "SI_TC") {
            clockSource = AI_SAMPLE_SELECT_SI_TC;
        }
        else if (clockSourceStr == "PFI0") {
            clockSource = AI_SAMPLE_SELECT_PFI0;
        }
        else if (clockSourceStr == "PFI1") {
            clockSource = AI_SAMPLE_SELECT_PFI1;
        }
        else if (clockSourceStr == "PFI2") {
            clockSource = AI_SAMPLE_SELECT_PFI2;
        }
        else if (clockSourceStr == "PFI3") {
            clockSource = AI_SAMPLE_SELECT_PFI3;
        }
        else if (clockSourceStr == "PFI4") {
            clockSource = AI_SAMPLE_SELECT_PFI4;
        }
        else if (clockSourceStr == "PFI5") {
            clockSource = AI_SAMPLE_SELECT_PFI5;
        }
        else if (clockSourceStr == "PFI6") {
            clockSource = AI_SAMPLE_SELECT_PFI6;
        }
        else if (clockSourceStr == "PFI7") {
            clockSource = AI_SAMPLE_SELECT_PFI7;
        }
        else if (clockSourceStr == "PFI8") {
            clockSource = AI_SAMPLE_SELECT_PFI8;
        }
        else if (clockSourceStr == "PFI9") {
            clockSource = AI_SAMPLE_SELECT_PFI9;
        }
        else if (clockSourceStr == "RTSI0") {
            clockSource = AI_SAMPLE_SELECT_RTSI0;
        }
        else if (clockSourceStr == "RTSI1") {
            clockSource = AI_SAMPLE_SELECT_RTSI1;
        }
        else if (clockSourceStr == "RTSI2") {
            clockSource = AI_SAMPLE_SELECT_RTSI2;
        }
        else if (clockSourceStr == "RTSI3") {
            clockSource = AI_SAMPLE_SELECT_RTSI3;
        }
        else if (clockSourceStr == "RTSI4") {
            clockSource = AI_SAMPLE_SELECT_RTSI4;
        }
        else if (clockSourceStr == "RTSI5") {
            clockSource = AI_SAMPLE_SELECT_RTSI5;
        }
        else if (clockSourceStr == "RTSI6") {
            clockSource = AI_SAMPLE_SELECT_RTSI6;
        }
        else if (clockSourceStr == "PULSE") {
            clockSource = AI_SAMPLE_SELECT_PULSE;
        }
        else if (clockSourceStr == "GPCRT0_OUT") {
            clockSource = AI_SAMPLE_SELECT_GPCRT0_OUT;
        }
        else if (clockSourceStr == "STAR_TRIGGER") {
            clockSource = AI_SAMPLE_SELECT_STAR_TRIGGER;
        }
        else if (clockSourceStr == "PFI10") {
            clockSource = AI_SAMPLE_SELECT_PFI10;
        }
        else if (clockSourceStr == "PFI11") {
            clockSource = AI_SAMPLE_SELECT_PFI11;
        }
        else if (clockSourceStr == "PFI12") {
            clockSource = AI_SAMPLE_SELECT_PFI12;
        }
        else if (clockSourceStr == "PFI13") {
            clockSource = AI_SAMPLE_SELECT_PFI13;
        }
        else if (clockSourceStr == "PFI14") {
            clockSource = AI_SAMPLE_SELECT_PFI14;
        }
        else if (clockSourceStr == "PFI15") {
            clockSource = AI_SAMPLE_SELECT_PFI15;
        }
        else if (clockSourceStr == "RTSI7") {
            clockSource = AI_SAMPLE_SELECT_RTSI7;
        }
        else if (clockSourceStr == "GPCTR1_OUT") {
            clockSource = AI_SAMPLE_SELECT_GPCTR1_OUT;
        }
        else if (clockSourceStr == "SCXI_TRIG1") {
            clockSource = AI_SAMPLE_SELECT_SCXI_TRIG1;
        }
        else if (clockSourceStr == "ANALOG_TRIGGER") {
            clockSource = AI_SAMPLE_SELECT_ANALOG_TRIGGER;
        }
        else if (clockSourceStr == "LOW") {
            clockSource = AI_SAMPLE_SELECT_LOW;
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockSource");
        }
    }
    StreamString clockPolarityStr;
    if (ok) {
        ok = data.Read("ClockPolarity", clockPolarityStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ClockPolarity shall be specified");
        }
    }
    if (ok) {
        if (clockPolarityStr == "ACTIVE_HIGH_OR_RISING_EDGE") {
            clockPolarity = AI_SAMPLE_POLARITY_ACTIVE_HIGH_OR_RISING_EDGE;
        }
        else if (clockPolarityStr == "ACTIVE_LOW_OR_FALLING_EDGE") {
            clockPolarity = AI_SAMPLE_POLARITY_ACTIVE_LOW_OR_FALLING_EDGE;
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockPolarity");
        }
    }

    //TODO Check the number of signals here as well!

    //Get individual signal parameters
    uint32 i;
    if (ok) {
        ok = data.MoveRelative("Signals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }
        for (i = 0u; (i < NI6259ADC_CHANNELS) && (ok); i++) {
            if (data.MoveRelative(data.GetChildName(i))) {
                StreamString range;
                if (data.Read("InputRange", range)) {
                    if (range == "10") {
                        inputRange[NI6259ADC_HEADER_SIZE + i] = 1u;
                    }
                    else if (range == "5") {
                        inputRange[NI6259ADC_HEADER_SIZE + i] = 2u;
                    }
                    else if (range == "2") {
                        inputRange[NI6259ADC_HEADER_SIZE + i] = 3u;
                    }
                    else if (range == "1") {
                        inputRange[NI6259ADC_HEADER_SIZE + i] = 4u;
                    }
                    else if (range == "500") {
                        inputRange[NI6259ADC_HEADER_SIZE + i] = 5u;
                    }
                    else if (range == "200") {
                        inputRange[NI6259ADC_HEADER_SIZE + i] = 6u;
                    }
                    else if (range == "100") {
                        inputRange[NI6259ADC_HEADER_SIZE + i] = 7u;
                    }
                    else {
                        ok = false;
                        REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported InputRange.");
                    }
                }
                StreamString polarity;
                if (data.Read("InputPolarity", polarity)) {
                    if (polarity == "Unipolar") {
                        inputPolarity[NI6259ADC_HEADER_SIZE + i] = AI_POLARITY_UNIPOLAR;
                    }
                    else if (polarity == "Bipolar") {
                        inputPolarity[NI6259ADC_HEADER_SIZE + i] = AI_POLARITY_BIPOLAR;
                    }
                    else {
                        ok = false;
                        REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported InputPolarity.");
                    }
                }
                StreamString mode;
                if (data.Read("InputMode", mode)) {
                    if (mode == "Differential") {
                        inputMode[NI6259ADC_HEADER_SIZE + i] = AI_CHANNEL_TYPE_DIFFERENTIAL;
                    }
                    else if (polarity == "NRSE") {
                        inputMode[NI6259ADC_HEADER_SIZE + i] = AI_CHANNEL_TYPE_NRSE;
                    }
                    else if (polarity == "RSE") {
                        inputMode[NI6259ADC_HEADER_SIZE + i] = AI_CHANNEL_TYPE_RSE;
                    }
                    else {
                        ok = false;
                        REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported InputMode.");
                    }
                }
                if (ok) {
                    ok = data.MoveToAncestor(1u);
                }
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
        ok = (GetNumberOfSignals() == (NI6259ADC_CHANNELS + NI6259ADC_HEADER_SIZE));
    }
    if (!ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Exactly (%d + %d) signals shall be configured", NI6259ADC_CHANNELS, NI6259ADC_HEADER_SIZE)
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
        for (i = 0u; (i < NI6259ADC_CHANNELS) && (ok); i++) {
            ok = (GetSignalType(NI6259ADC_HEADER_SIZE + i) == Float32Bit);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "All the ADC signals shall be of type Float32Bit");
        }
    }

    //Check that if there is a synchronisation channel (i.e. that this board will serve a time source)
    uint32 nOfFunctions = GetNumberOfFunctions();
    float32 frequency = -1.0F;
    bool found = false;
    uint32 functionIdx;
    for (functionIdx = 0u; (functionIdx < nOfFunctions) && (ok); functionIdx++) {
        uint32 nOfSignals = 0u;
        ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfSignals);

        if (ok) {
            for (i = 0u; (i < nOfSignals) && (ok) && (!found); i++) {
                ok = GetFunctionSignalReadFrequency(InputSignals, functionIdx, i, frequency);
                found = (frequency > 0.F);
                if (found) {
                    synchronisingFunctionIdx = functionIdx;
                }
            }
        }
    }
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
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not open device %s", fullDeviceName)
        }
    }
    pxi6259_ai_conf_t adcConfiguration = pxi6259_create_ai_conf();
    for (i = 0u; (i < NI6259ADC_CHANNELS) && (ok); i++) {
        ok = (pxi6259_add_ai_channel(&adcConfiguration, i, inputPolarity[i], inputRange[i], inputMode[i], 0) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set InputRange for channel %d of device %s", i, fullDeviceName)
        }
    }
    if (ok) {
        if (samplingFrequency != 0u) {
            int32 divisions = static_cast<int32>(20000000 / frequency + 0.5f);
            ok = (pxi6259_set_ai_sample_clk(&adcConfiguration, divisions, delayDivisor, clockSource, clockPolarity) == 0);
        }
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set the clock for device %s", fullDeviceName)
        }
    }
    if (ok) {
        ok = (pxi6259_load_ai_conf(boardFileDescriptor, &adcConfiguration) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not load configuration for device %s", fullDeviceName)
        }
    }
    if (ok) {
        //Allocate memory
        for (i = 0u; (i < NI6259ADC_CHANNELS) && (ok); i++) {
            channelsMemory[i] = new float32[numberOfSamples];
        }
    }

    if (ok) {
        //Required to wait for devices to be available in /dev!
        Sleep::Sec(1.0);
        for (i = 0u; (i < NI6259ADC_CHANNELS) && (ok); i++) {
            StreamString channelDeviceName;
            ok = channelDeviceName.Printf("%s.%d", fullDeviceName.Buffer(), i);
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

    return ok;
}

ErrorManagement::ErrorType NI6259ADC::Execute(const ExecutionInfo& info) {
    ErrorManagement::ErrorType err;
    if (info.GetStage() == ExecutionInfo::TerminationStage) {
        keepRunning = false;
    }
    else {
        uint32 i;
        for (i = 0u; (i < NI6259ADC_CHANNELS) && (keepRunning); i++) {
            uint32 readSamples = 0u;
            while ((readSamples < numberOfSamples) && (keepRunning)) {
                int32 currentSamples = pxi6259_read_ai(channelsFileDescriptors[i], &channelsMemory[i][readSamples], numberOfSamples - readSamples);
                if (currentSamples == 0) {
                    readSamples += currentSamples;
                }
            }
        }

        err = synchSem.Post();
        counter++;
        time = counter * numberOfSamples * samplingPeriodMicroSeconds;
    }

    return err;
}

CLASS_REGISTER(NI6259ADC, "1.0")
}

