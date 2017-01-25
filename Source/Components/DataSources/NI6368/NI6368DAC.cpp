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
    //clockUpdateDivisor = 10u;
    triggerSet = false;
    numberOfSamples = 0u;
    //clockUpdateSource = AO_UPDATE_SOURCE_SELECT_UI_TC;
    //clockUpdatePolarity = AO_UPDATE_SOURCE_POLARITY_RISING_EDGE;
    uint32 n;
    for (n = 0u; n < NI6368DAC_MAX_CHANNELS; n++) {
        //outputPolarity[n] = AO_DAC_POLARITY_UNIPOLAR;
        outputRange[n] = XSERIES_OUTPUT_RANGE_10V;
        dacEnabled[n] = false;
        channelsFileDescriptors[n] = -1;
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
        while ((i < NI6368DAC_MAX_CHANNELS) && (ok)) {
            if (data.MoveRelative(data.GetChildName(i))) {
                uint32 channelId;
                if (data.Read("ChannelId", channelId)) {
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
            if (numberOfSamples == 0u) {
                numberOfSamples = nSamples;
            }
            else {
                if (numberOfSamples != nSamples) {
                    ok = false;
                    REPORT_ERROR(ErrorManagement::ParametersError, "All the DAC signals shall have the same number of samples");
                }
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
    xseries_ao_conf_t dacConfiguration = xseries_static_ao();
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
            //TODO CHECK THAT THE CHANNELID IS MONOTONIC AND IN ORDER. OTHERWISE THERE WILL BE A MISMATCH WITH THE DMA ORDER
            if (dacEnabled[i]) {
                if (ok) {
                    //Allocate memory
                    channelsMemory[i] = new float32[numberOfSamples];
                }
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
            if (channelsMemory[i] != NULL_PTR(float32 *)) {
                ok = (xseries_write_ao(channelsFileDescriptors[i], channelsMemory[i], static_cast<size_t>(numberOfSamples)) >= 0);
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

