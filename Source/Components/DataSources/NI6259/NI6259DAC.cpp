/**
 * @file NI6259DAC.cpp
 * @brief Source file for class NI6259DAC
 * @date 03/01/2017
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
 * the class NI6259DAC (public, protected, and private). Be aware that some
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
#include "NI6259DAC.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*lint -e1923 -e9024 allow to use #define for constants.*/
#define DAC_BASE_FREQ 20000000u
#define DAC_ONE_TWO_CH_MAX_FREQ 2860000u
#define DAC_THREE_FOUR_CH_MAX_FREQ 1250000u

#define INIT_HEADER_SIZE 0xFFFFFFFFu

#define ATTR_MAP_ENTRY(attr) { #attr , attr }
/*lint +e1923 +e9024 */
namespace MARTe {

//lint -e{9109} -e{770} AttributeMap is private to this file and independent from NI6259ADC.cpp 
struct AttributeMap {
    const char8 *name;
    pxi6259_segment_attribute attribute;
//lint -e{9109} -e{770} AttributeMap is private to this file and independent from NI6259ADC.cpp 
};

static struct AttributeMap attributeMap[] = {
ATTR_MAP_ENTRY(AO_CONTINUOUS),
ATTR_MAP_ENTRY(AO_SIGNAL_GENERATION),
ATTR_MAP_ENTRY(AO_START1_SOURCE_SELECT),
ATTR_MAP_ENTRY(AO_START1_POLARITY_SELECT),
ATTR_MAP_ENTRY(AO_UPDATE_SOURCE_SELECT),
ATTR_MAP_ENTRY(AO_UPDATE_POLARITY_SELECT),
ATTR_MAP_ENTRY(AO_UPDATE_PERIOD_DIVISOR),
ATTR_MAP_ENTRY(AO_UI_SOURCE_SELECT),
ATTR_MAP_ENTRY(AO_UI_SOURCE_POLARITY_SELECT),
ATTR_MAP_ENTRY(AO_NUMBER_OF_SAMPLES),
ATTR_MAP_ENTRY(AO_NUMBER_OF_BUFFERS),
ATTR_MAP_ENTRY(AO_FIFO_RETRANSMIT), { NULL_PTR(const char8* const), AI_CONTINUOUS } };

static pxi6259_segment_attribute GetAttribute(const char8 *const attrName) {
    uint32 i = 0u;
    pxi6259_segment_attribute attr = AI_CONTINUOUS;
    while (attributeMap[i].name != NULL) {
        if(StringHelper::Compare(attributeMap[i].name, attrName) == 0) {
            attr = attributeMap[i].attribute;
            break;
        }
        i++;
    }
    return attr;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

NI6259DAC::NI6259DAC() :
        DataSourceI() {
    boardId = 0u;
    boardFileDescriptor = -1;
    rootFileDescriptor = -1;

    deviceName = "";
    numberOfDACsEnabled = 0u;
    clockUpdateDivisor = 20u;
    triggerSet = false;
    clockUpdateSource = AO_UPDATE_SOURCE_SELECT_UI_TC;
    clockUpdatePolarity = AO_UPDATE_SOURCE_POLARITY_RISING_EDGE;
    uint32 n;
    for (n = 0u; n < NI6259DAC_MAX_CHANNELS; n++) {
        outputGain[n] = 1.0F;
        outputOffset[n] = 0.0F;
        channelsMemory[n] = NULL_PTR(float32*);
        outputPolarity[n] = AO_DAC_POLARITY_UNIPOLAR;
        dacEnabled[n] = false;
        channelsFileDescriptors[n] = -1;
    }
    baseSampleClockFrequency = 0u;
    numberOfSamplesT = 0u;

    headerSize = INIT_HEADER_SIZE;
    boardStatus = 0u;
}

/*lint -e{1551} the destructor must guarantee that the NI6259DAC file descriptors are closed.*/
NI6259DAC::~NI6259DAC() {
    if (boardFileDescriptor != -1) {
        if (pxi6259_stop_ao(boardFileDescriptor) < 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop acquisition.");
        }
    }
    for (uint32 n = 0u; n < NI6259DAC_MAX_CHANNELS; n++) {
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

bool NI6259DAC::AllocateMemory() {
    return true;
}

uint32 NI6259DAC::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool NI6259DAC::GetSignalMemoryBuffer(const uint32 signalIdx,
                                      const uint32 bufferIdx,
                                      void *&signalAddress) {
    bool ok = (headerSize != INIT_HEADER_SIZE);
    if (ok) {
        ok = (signalIdx < (NI6259DAC_MAX_CHANNELS + headerSize));
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

const char8* NI6259DAC::GetBrokerName(StructuredDataI &data,
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
bool NI6259DAC::PrepareNextState(const char8 *const currentStateName,
                                 const char8 *const nextStateName) {
    return true;
}

bool NI6259DAC::Initialise(StructuredDataI &data) {
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
        if (!data.Read("BaseSampleClockFrequency", baseSampleClockFrequency)) {
            baseSampleClockFrequency = DAC_BASE_FREQ;
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "BaseSampleClockFrequency not defined. Set to default %d", baseSampleClockFrequency);
        }
    }
    StreamString clockUpdateSourceStr;
    if (ok) {
        if (data.Read("ClockUpdateSource", clockUpdateSourceStr)) {
            if (clockUpdateSourceStr == "UI_TC") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_UI_TC;
            }
            else if (clockUpdateSourceStr == "PFI0") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI0;
            }
            else if (clockUpdateSourceStr == "PFI1") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI1;
            }
            else if (clockUpdateSourceStr == "PFI2") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI2;
            }
            else if (clockUpdateSourceStr == "PFI3") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI3;
            }
            else if (clockUpdateSourceStr == "PFI4") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI4;
            }
            else if (clockUpdateSourceStr == "PFI5") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI5;
            }
            else if (clockUpdateSourceStr == "PFI6") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI6;
            }
            else if (clockUpdateSourceStr == "PFI7") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI7;
            }
            else if (clockUpdateSourceStr == "PFI8") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI8;
            }
            else if (clockUpdateSourceStr == "PFI9") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI9;
            }
            else if (clockUpdateSourceStr == "PFI10") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI10;
            }
            else if (clockUpdateSourceStr == "PFI11") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI11;
            }
            else if (clockUpdateSourceStr == "PFI12") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI12;
            }
            else if (clockUpdateSourceStr == "PFI13") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI13;
            }
            else if (clockUpdateSourceStr == "PFI14") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI14;
            }
            else if (clockUpdateSourceStr == "PFI15") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_PFI15;
            }
            else if (clockUpdateSourceStr == "RTSI0") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_RTSI0;
            }
            else if (clockUpdateSourceStr == "RTSI1") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_RTSI1;
            }
            else if (clockUpdateSourceStr == "RTSI2") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_RTSI2;
            }
            else if (clockUpdateSourceStr == "RTSI3") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_RTSI3;
            }
            else if (clockUpdateSourceStr == "RTSI4") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_RTSI4;
            }
            else if (clockUpdateSourceStr == "RTSI5") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_RTSI5;
            }
            else if (clockUpdateSourceStr == "RTSI6") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_RTSI6;
            }
            else if (clockUpdateSourceStr == "RTSI7") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_RTSI7;
            }
            else if (clockUpdateSourceStr == "GPCRT0_OUT") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_GPCTR0_Out;
            }
            else if (clockUpdateSourceStr == "STAR_TRIGGER") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_STAR_TRIGEGR;
            }
            else if (clockUpdateSourceStr == "GPCTR1_OUT") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_GPCTR1_Out;
            }
            else if (clockUpdateSourceStr == "ANALOG_TRIGGER") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_ANALOG_TRIGGER;
            }
            else if (clockUpdateSourceStr == "LOW") {
                clockUpdateSource = AO_UPDATE_SOURCE_SELECT_LOW;
            }
            else {
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockUpdateSource %s", clockUpdateSourceStr.Buffer());
            }
        }
        else {
            clockUpdateSource = AO_UPDATE_SOURCE_SELECT_UI_TC;
            REPORT_ERROR(ErrorManagement::Information, "ClockUpdateSource not defined. Using default AO_UPDATE_SOURCE_SELECT_UI_TC");
        }
    }
    if (ok) {
        bool set = false;
        uint32 samplingFrequency = 0u;
        //lint -e{9013} SamplingFrequency and ClockUpdateDivisor are mutually exclusive
        if (data.Read("SamplingFrequency", samplingFrequency)) {
            ok = (samplingFrequency <= baseSampleClockFrequency) && (samplingFrequency > 0u);
            if (ok) {
                clockUpdateDivisor = (baseSampleClockFrequency / samplingFrequency);
                REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "ClockUpdateDivisor set to: %d", clockUpdateDivisor);
                float32 samplingF = static_cast<float32>(baseSampleClockFrequency) / static_cast<float32>(clockUpdateDivisor);
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
        if (ok) {
            if (!set) {
                clockUpdateDivisor = 20u;
                REPORT_ERROR(ErrorManagement::Information, "Nor SamplingFrequency or ClockUpdateDivisor defined. Setting ClockUpdateDivisor=%d",
                             clockUpdateDivisor);
            }
        }
    }
    StreamString clockUpdatePolarityStr;
    if (ok) {
        if (data.Read("ClockUpdatePolarity", clockUpdatePolarityStr)) {
            if (clockUpdatePolarityStr == "RISING_EDGE") {
                clockUpdatePolarity = AO_UPDATE_SOURCE_POLARITY_RISING_EDGE;
            }
            else if (clockUpdatePolarityStr == "FALLING_EDGE") {
                clockUpdatePolarity = AO_UPDATE_SOURCE_POLARITY_FALLING_EDGE;
            }
            else {
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockUpdatePolarity");
            }
        }
        else {
            clockUpdatePolarity = AO_UPDATE_SOURCE_POLARITY_RISING_EDGE;
            REPORT_ERROR(ErrorManagement::Information, "ClockUpdatePolarity not defined. Using default AO_UPDATE_SOURCE_POLARITY_RISING_EDGE");
        }
    }
    if (ok) {
        if (data.MoveRelative("Attributes")) {
            ok = attributes.Write("Attributes", data);
            if (ok) {
                ok = data.MoveToAncestor(1u);
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
                    ok = (channelId < NI6259DAC_MAX_CHANNELS);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Invalid ChannelId specified.");
                    }
                    if (ok) {
                        dacEnabled[channelId] = true;
                        numberOfDACsEnabled++;
                        StreamString polarity;
                        if (data.Read("OutputPolarity", polarity)) {
                            if (polarity == "Unipolar") {
                                outputPolarity[channelId] = AO_DAC_POLARITY_UNIPOLAR;
                            }
                            else if (polarity == "Bipolar") {
                                outputPolarity[channelId] = AO_DAC_POLARITY_BIPOLAR;
                            }
                            else {
                                ok = false;
                                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported OutputPolarity.");
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
        float32 samplingF = static_cast<float32>(baseSampleClockFrequency) / static_cast<float32>(clockUpdateDivisor);
        uint32 maxFreq = (numberOfDACsEnabled <= 2u) ? (DAC_ONE_TWO_CH_MAX_FREQ) : (DAC_THREE_FOUR_CH_MAX_FREQ);
        ok = (samplingF <= static_cast<float32>(maxFreq));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Sampling frequency (%f) shall be minor than %d", samplingF, maxFreq);
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

bool NI6259DAC::SetConfiguredDatabase(StructuredDataI &data) {
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
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "All the DAC signals shall be of type Float32Bit");
        }
    }

    uint32 nOfFunctions = GetNumberOfFunctions();
    uint32 functionIdx;
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
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not open device %s", fullDeviceName);
        }
    }
    pxi6259_ao_conf_t dacConfiguration = pxi6259_create_ao_conf();
    for (i = 0u; (i < NI6259DAC_MAX_CHANNELS) && (ok); i++) {
        if (dacEnabled[i]) {
            ok = (pxi6259_add_ao_channel(&dacConfiguration, static_cast<uint8_t>(i), outputPolarity[i]) == 0);
            uint32 ii = i;
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set configuration for channel %d of device %s", ii, fullDeviceName);
            }
        }
    }
    if (ok) {
        ok = (pxi6259_set_ao_attribute(&dacConfiguration, AO_SIGNAL_GENERATION, AO_SIGNAL_GENERATION_STATIC) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set the parameter AO_SIGNAL_GENERATION_STATIC %s", fullDeviceName);
        }
    }
    if (ok) {
        ok = (pxi6259_set_ao_attribute(&dacConfiguration, AO_CONTINUOUS, 0) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set the parameter AO_CONTINUOUS for device %s", fullDeviceName);
        }
    }
    if (ok) {
        //last 1 is for continuous
        ok = (pxi6259_set_ao_count(&dacConfiguration, numberOfSamplesT, 1u, 1u) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set the number of samples for device %s", fullDeviceName);
        }
    }
    if (ok) {
        ok = (pxi6259_set_ao_update_clk(&dacConfiguration, clockUpdateSource, clockUpdatePolarity, clockUpdateDivisor) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set pxi6259_set_ao_update_clk for device %s", fullDeviceName);
        }
    }
    if (ok) {
        //Add additional attributes
        if (attributes.MoveAbsolute("Attributes")) {
            for (i = 0u; (i < attributes.GetNumberOfChildren()) && ok; i++) {
                StreamString attrName = attributes.GetChildName(i);
                uint32 attrVal;
                ok = attributes.Read(attrName.Buffer(), attrVal);
                if (ok) {
                    pxi6259_segment_attribute attribute = GetAttribute(attrName.Buffer());
                    ok = (attribute != AI_CONTINUOUS);
                    if (ok) {
                        REPORT_ERROR(ErrorManagement::Information, "pxi6259_set_ao_attribute(&dacConfiguration, %s, %d)", attrName.Buffer(), attrVal);

                        ok = (pxi6259_set_ao_attribute(&dacConfiguration, attribute, attrVal) == 0);
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
        ok = (pxi6259_load_ao_conf(boardFileDescriptor, &dacConfiguration) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not load configuration for device %s", fullDeviceName);
        }
    }

    if (ok) {
        //Required to wait for devices to be available in /dev!
        Sleep::Sec(1.0F);
        for (i = 0u; (i < NI6259DAC_MAX_CHANNELS) && (ok); i++) {
            channelsMemory[i] = NULL_PTR(float32*);
            if (dacEnabled[i]) {
                StreamString channelDeviceName;
                channelsMemory[i] = new float32[numberOfSamplesT];
                uint32 ii = i;
                ok = channelDeviceName.Printf("%s.%d", fullDeviceName.Buffer(), ii);
                if (ok) {
                    ok = channelDeviceName.Seek(0ULL);
                }
                if (ok) {
                    channelsFileDescriptors[i] = open(channelDeviceName.Buffer(), O_RDWR);
                    ok = (channelsFileDescriptors[i] > -1);
                    if (!ok) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not open device %s", channelDeviceName);
                    }
                }
            }
        }
    }
    if (ok) {
        ok = (pxi6259_start_ao(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not start the device %s", fullDeviceName);
        }
    }
    return ok;
}

bool NI6259DAC::Synchronise() {
    uint32 i;
    bool ok = true;
    for (i = 0u; (i < NI6259DAC_MAX_CHANNELS) && (ok); i++) {
        if (dacEnabled[i]) {
            if (channelsMemory[i] != NULL_PTR(float32*)) {
                size_t samplesToWrite = numberOfSamplesT;
                for (uint32 j = 0u; (j < samplesToWrite) && (ok); j++) {
                    channelsMemory[i][j]=((channelsMemory[i][j] * outputGain[i])+outputOffset[i]);
                }
                ssize_t totalWritten = 0;
                while ((samplesToWrite > 0u) && (ok)) {
                    ssize_t samplesWritten = pxi6259_write_ao(channelsFileDescriptors[i], &(channelsMemory[i][totalWritten]), samplesToWrite);
                    ok = (samplesWritten >= 0);
                    if (ok) {
                        totalWritten += samplesWritten;
                        samplesToWrite -= static_cast<size_t>(samplesWritten);
                    }
                }
            }
        }
    }

    //lint -e{565} -e{1080} -e{1055} -e{516} -e{115} -e{1030} -e{40} -e{1013} symbols are in header file
    if (headerSize > 0u) {
        struct pxi6259_board_status boardStatusT;
        if (pxi6259_get_board_status(rootFileDescriptor, &boardStatusT) == 0) {
            boardStatus = boardStatusT.currentBoardStatus;
        }
    }

    return ok;
}

bool NI6259DAC::ReadAOConfiguration(pxi6259_ao_conf_t *const conf) const {
    bool ok = false;
    if (boardFileDescriptor > 0) {
        ok = (pxi6259_read_ao_conf(boardFileDescriptor, conf) == 0);
    }
    return ok;
}

CLASS_REGISTER(NI6259DAC, "1.0")
}

