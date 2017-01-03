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
    numberOfSamples = 0u;
    boardId = 0u;
    boardFileDescriptor = -1;
    deviceName = "";
    numberOfDACsEnabled = 0u;
    uint32 n;
    for (n = 0u; n < NI6259DAC_MAX_CHANNELS; n++) {
        outputPolarity[n] = AO_DAC_POLARITY_UNIPOLAR;
        dacEnabled[n] = false;
        channelsFileDescriptors[n] = -1;
        channelsMemory[n] = NULL_PTR(float32 *);
    }
}

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
    for (n = 0u; n < NI6259DAC_MAX_CHANNELS; n++) {
        if (channelsMemory[n] != NULL_PTR(float32 *)) {
            delete[] channelsMemory[n];
        }
    }
}

bool NI6259DAC::AllocateMemory() {
    return true;
}

uint32 NI6259DAC::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool NI6259DAC::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (signalIdx < (NI6259DAC_MAX_CHANNELS));
    if (ok) {
        signalAddress = &(channelsMemory[signalIdx][0]);
    }
    return ok;
}

const char8* NI6259DAC::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == OutputSignals) {
        brokerName = "MemoryMapSynchronisedOutputBroker";
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with OutputSignals");
    }
    return brokerName;
}

bool NI6259DAC::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
    return false;
}

bool NI6259DAC::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
    ReferenceT<MemoryMapOutputBroker> broker("MemoryMapSynchronisedOutputBroker");
    bool ok = broker.IsValid();

    if (ok) {
        ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ok) {
        ok = outputBrokers.Insert(broker);
    }

    return ok;
}

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

    //Get individual signal parameters
    uint32 i;
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
        for (i = 0u; (i < numberOfDACsEnabled) && (ok); i++) {
            ok = (GetSignalType(i) == Float32Bit);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "All the DAC signals shall be of type Float32Bit");
        }
    }

    //Check that if there is a synchronisation channel (i.e. that this board will serve a time source)
    uint32 nOfFunctions = GetNumberOfFunctions();
    uint32 functionIdx;
    //Check that the number of samples for all the signals is always the same
    for (functionIdx = 0u; (functionIdx < nOfFunctions) && (ok); functionIdx++) {
        uint32 nOfSignals = 0u;
        ok = GetFunctionNumberOfSignals(OutputSignals, functionIdx, nOfSignals);

        for (i = 0u; (i < nOfSignals) && (ok); i++) {
            uint32 signalIdx = 0u;
            uint32 nSamples = 0u;
            ok = GetFunctionSignalSamples(OutputSignals, functionIdx, i, nSamples);

            StreamString signalAlias;
            if (ok) {
                ok = GetFunctionSignalAlias(OutputSignals, functionIdx, i, signalAlias);
            }
            if (ok) {
                ok = GetSignalIndex(signalIdx, signalAlias.Buffer());
            }
            if (ok) {

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
            ok = (pxi6259_add_ao_channel(&dacConfiguration, i, outputPolarity[i]) == 0);
            if (ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Channel %d set with polarity %d ", i, outputPolarity[i])
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set configuration for channel %d of device %s", i, fullDeviceName)
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
        ok = (pxi6259_set_ao_attribute(&dacConfiguration, AO_CONTINUOUS, 0) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set the parameter AO_CONTINUOUS %s", fullDeviceName)
        }
    }
    if (ok) {
        ok = (pxi6259_set_ao_count(&dacConfiguration, numberOfSamples, 1, AO_CONTINUOUS_IGNORE_BC_TC) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set the number of samples for device %s", fullDeviceName)
        }
    }
    if (ok) {
        //Allocate memory
        for (i = 0u; (i < NI6259DAC_MAX_CHANNELS) && (ok); i++) {
            channelsMemory[i] = new float32[numberOfSamples];
        }
    }

    if (ok) {
        //Required to wait for devices to be available in /dev!
        Sleep::Sec(1.0);
        for (i = 0u; (i < NI6259DAC_MAX_CHANNELS) && (ok); i++) {
            if (dacEnabled[i]) {
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
    }
    if (ok) {
        ok = (pxi6259_start_ao(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not start the device %s", fullDeviceName)
        }
    }
    return ok;
}

#include <stdio.h>
bool NI6259DAC::Synchronise() {
    uint32 i = 0u;
    bool ok = true;
    for (i = 0u; (i < NI6259DAC_MAX_CHANNELS); i++) {
        if (dacEnabled[i]) {
            ok = (pxi6259_write_ao(channelsFileDescriptors[i], channelsMemory[i], numberOfSamples) >= 0);
        }
    }
    return ok;
}

CLASS_REGISTER(NI6259DAC, "1.0")
}

