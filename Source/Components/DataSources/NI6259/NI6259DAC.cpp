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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
NI6259DAC::NI6259DAC() :
        DataSourceI() {
    boardId = 0u;
    boardFileDescriptor = -1;
    deviceName = "";
    numberOfDACsEnabled = 0u;
    clockUpdateDivisor = 10u;
    triggerSet = false;
    clockUpdateSource = AO_UPDATE_SOURCE_SELECT_UI_TC;
    clockUpdatePolarity = AO_UPDATE_SOURCE_POLARITY_RISING_EDGE;
    uint32 n;
    for (n = 0u; n < NI6259DAC_MAX_CHANNELS; n++) {
        outputPolarity[n] = AO_DAC_POLARITY_UNIPOLAR;
        dacEnabled[n] = false;
        channelsFileDescriptors[n] = -1;
    }
    channelsMemory = NULL_PTR(float32 *);
}

/*lint -e{1551} the destructor must guarantee that the NI6259DAC file descriptors are closed.*/
NI6259DAC::~NI6259DAC() {
    uint32 n;
    if (boardFileDescriptor != -1) {
        if (pxi6259_stop_ao(boardFileDescriptor) < 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop acquisition.");
        }
    }
    for (n = 0u; n < NI6259DAC_MAX_CHANNELS; n++) {
        if (channelsFileDescriptors[n] != -1) {
            close(channelsFileDescriptors[n]);
        }
    }
    if (boardFileDescriptor != -1) {
        close(boardFileDescriptor);
    }
    if (channelsMemory != NULL_PTR(float32 *)) {
        delete[] channelsMemory;
    }
}

bool NI6259DAC::AllocateMemory() {
    return true;
}

uint32 NI6259DAC::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool NI6259DAC::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (signalIdx < (NI6259DAC_MAX_CHANNELS));
    if (ok) {
        if (channelsMemory != NULL_PTR(float32 *)) {
            signalAddress = &(channelsMemory[signalIdx]);
        }
    }
    return ok;
}

const char8* NI6259DAC::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
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
bool NI6259DAC::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

bool NI6259DAC::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
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
bool NI6259DAC::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool NI6259DAC::Initialise(StructuredDataI& data) {
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
    StreamString clockUpdateSourceStr;
    if (ok) {
        ok = data.Read("ClockUpdateSource", clockUpdateSourceStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ClockUpdateSource shall be specified");
        }
    }
    if (ok) {
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
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported ClockUpdateSource");
        }
    }
    if (ok) {
        if (data.Read("ClockUpdateDivisor", clockUpdateDivisor)) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "ClockUpdateDivisor set to: %d", clockUpdateDivisor)
        }
    }
    StreamString clockUpdatePolarityStr;
    if (ok) {
        ok = data.Read("ClockUpdatePolarity", clockUpdatePolarityStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The ClockUpdatePolarity shall be specified");
        }
    }
    if (ok) {
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
        while ((i < NI6259DAC_MAX_CHANNELS) && (ok)) {
            if (data.MoveRelative(data.GetChildName(i))) {
                uint32 channelId;
                if (data.Read("ChannelId", channelId)) {
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

bool NI6259DAC::SetConfiguredDatabase(StructuredDataI& data) {
    uint32 i;
    bool ok = DataSourceI::SetConfiguredDatabase(data);

    if (ok) {
        ok = triggerSet;
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "At least one Trigger signal shall be set.");
    }
    if (ok) {
        for (i = 0u; (i < numberOfDACsEnabled) && (ok); i++) {
            ok = (GetSignalType(i) == Float32Bit);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "All the DAC signals shall be of type Float32Bit");
        }
    }

    uint32 nOfFunctions = GetNumberOfFunctions();
    uint32 functionIdx;
    //Check that the number of samples for all the signals is one
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
                REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples shall be exactly one");
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
    pxi6259_ao_conf_t dacConfiguration = pxi6259_create_ao_conf();
    for (i = 0u; (i < NI6259DAC_MAX_CHANNELS) && (ok); i++) {
        if (dacEnabled[i]) {
            ok = (pxi6259_add_ao_channel(&dacConfiguration, static_cast<uint8_t>(i), outputPolarity[i]) == 0);
            uint32 ii = i;
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set configuration for channel %d of device %s", ii, fullDeviceName)
            }
        }
    }
    if (ok) {
        ok = (pxi6259_set_ao_attribute(&dacConfiguration, AO_SIGNAL_GENERATION, AO_SIGNAL_GENERATION_STATIC) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set the parameter AO_SIGNAL_GENERATION_STATIC %s", fullDeviceName)
        }
    }
    if (ok) {
        ok = (pxi6259_set_ao_count(&dacConfiguration, 1u, 1u, 0u) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set the number of samples for device %s", fullDeviceName)
        }
    }
    if (ok) {
        ok = (pxi6259_set_ao_update_clk(&dacConfiguration, clockUpdateSource, clockUpdatePolarity, clockUpdateDivisor) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set pxi6259_set_ao_update_clk for device %s", fullDeviceName)
        }
    }
    if (ok) {
        ok = (pxi6259_load_ao_conf(boardFileDescriptor, &dacConfiguration) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not load configuration for device %s", fullDeviceName)
        }
    }

    if (ok) {
        //Allocate memory
        channelsMemory = new float32[NI6259DAC_MAX_CHANNELS];
    }

    if (ok) {
        //Required to wait for devices to be available in /dev!
        Sleep::Sec(1.0);
        for (i = 0u; (i < NI6259DAC_MAX_CHANNELS) && (ok); i++) {
            if (dacEnabled[i]) {
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
        ok = (pxi6259_start_ao(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not start the device %s", fullDeviceName)
        }
    }
    return ok;
}

bool NI6259DAC::Synchronise() {
    uint32 i;
    bool ok = true;
    for (i = 0u; (i < NI6259DAC_MAX_CHANNELS) && (ok); i++) {
        if (dacEnabled[i]) {
            if (channelsMemory != NULL_PTR(float32 *)) {
                ok = (pxi6259_write_ao(channelsFileDescriptors[i], &(channelsMemory[i]), static_cast<size_t>(1u)) >= 0);
            }
        }
    }
    return ok;
}

bool NI6259DAC::ReadAOConfiguration(pxi6259_ao_conf_t * const conf) const {
    bool ok = false;
    if (boardFileDescriptor > 0) {
        ok = (pxi6259_read_ao_conf(boardFileDescriptor, conf) == 0);
    }
    return ok;
}

CLASS_REGISTER(NI6259DAC, "1.0")
}

