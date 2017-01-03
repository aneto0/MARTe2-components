/**
 * @file NI6259DIO.cpp
 * @brief Source file for class NI6259DIO
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
 * the class NI6259DIO (public, protected, and private). Be aware that some
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
#include "NI6259DIO.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
NI6259DIO::NI6259DIO() :
        DataSourceI() {
    boardId = 0u;
    boardFileDescriptor = -1;
    numberOfPortsEnabled = 0u;
    deviceName = "";
    uint32 n;
    for (n = 0u; n < PXI6259_NUMBER_OF_PORTS; n++) {
        portEnabled[n] = false;
        portFileDescriptors[n] = -1;
    }
}

NI6259DIO::~NI6259DIO() {
    uint32 n;
    for (n = 0u; n < PXI6259_NUMBER_OF_PORTS; n++) {
        if (portFileDescriptors[n] != -1) {
            close (portFileDescriptors[n]);
        }
    }
    if (boardFileDescriptor != -1) {
        close(boardFileDescriptor);
    }
}

bool NI6259DIO::AllocateMemory() {
    return true;
}

uint32 NI6259DIO::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool NI6259DIO::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void*& signalAddress) {
    bool ok = (signalIdx < (PXI6259_NUMBER_OF_PORTS));
    if (ok) {
        signalAddress = &(portValues[signalIdx]);
    }
    return ok;
}

const char8* NI6259DIO::GetBrokerName(StructuredDataI& data, const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == OutputSignals) {
        brokerName = "MemoryMapSynchronisedOutputBroker";
    }
    else {
        brokerName = "MemoryMapSynchronisedInputBroker";
    }
    return brokerName;
}

bool NI6259DIO::GetInputBrokers(ReferenceContainer& inputBrokers, const char8* const functionName, void* const gamMemPtr) {
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

bool NI6259DIO::GetOutputBrokers(ReferenceContainer& outputBrokers, const char8* const functionName, void* const gamMemPtr) {
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

bool NI6259DIO::PrepareNextState(const char8* const currentStateName, const char8* const nextStateName) {
    return true;
}

bool NI6259DIO::Initialise(StructuredDataI& data) {
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
        while ((i < PXI6259_NUMBER_OF_PORTS) && (ok)) {
            if (data.MoveRelative(data.GetChildName(i))) {
                uint32 portId;
                if (data.Read("PortId", portId)) {
                    ok = (portId < PXI6259_NUMBER_OF_PORTS);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Invalid PortId specified.");
                    }
                    if (ok) {
                        portEnabled[portId] = true;
                        numberOfPortsEnabled++;
                        portMask[portId] = 0u;
                        uint32 mask;
                        if (data.Read("Mask", mask)) {
                            portMask[portId] = mask;
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

bool NI6259DIO::SetConfiguredDatabase(StructuredDataI& data) {
    uint32 i;
    bool ok = DataSourceI::SetConfiguredDatabase(data);

    if (ok) {
        for (i = 0u; (i < numberOfPortsEnabled) && (ok); i++) {
            ok = (GetSignalType(i) == UnsignedInteger32Bit);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "All the DIO signals shall be of type UnsignedInteger32Bit");
        }
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
                REPORT_ERROR(ErrorManagement::ParametersError, "Only one sample shall be specified");
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
                REPORT_ERROR(ErrorManagement::ParametersError, "Only one sample shall be specified");
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
    pxi6259_dio_conf_t dioConfiguration = pxi6259_create_dio_conf();
    for (i = 0u; (i < PXI6259_NUMBER_OF_PORTS) && (ok); i++) {
        if (portEnabled[i]) {
            ok = (pxi6259_add_dio_channel(&dioConfiguration, static_cast<uint8>(i), portMask[i]) == 0);
            if (ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Port %d set with mask %d ", i, portMask[i])
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set configuration for port %d of device %s", i, fullDeviceName)
            }
        }
    }

    if (ok) {
        //Required to wait for devices to be available in /dev!
        Sleep::Sec(1.0);
        for (i = 0u; (i < PXI6259_NUMBER_OF_PORTS) && (ok); i++) {
            if (portEnabled[i]) {
                StreamString portDeviceName;
                ok = portDeviceName.Printf("%s.%d", fullDeviceName.Buffer(), i);
                if (ok) {
                    ok = portDeviceName.Seek(0ULL);
                }
                if (ok) {
                    portFileDescriptors[i] = open(portDeviceName.Buffer(), O_RDWR);
                    ok = (portFileDescriptors[i] > -1);
                    if (!ok) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not open device %s", portDeviceName)
                    }
                }
            }
        }
    }
    return ok;
}

bool NI6259DIO::Synchronise() {
    uint32 i = 0u;
    bool ok = true;
    for (i = 0u; (i < PXI6259_NUMBER_OF_PORTS) && (ok); i++) {
        if (portEnabled[i]) {
            ok = (write(portFileDescriptors[i], &(portValues[i]), sizeof(uint32)) == sizeof(uint32));
            if (ok) {
                ok = (read(portFileDescriptors[i], &(portValues[i]), sizeof(uint32)) == sizeof(uint32));
            }
        }
    }
    return ok;
}

CLASS_REGISTER(NI6259DIO, "1.0")
}

