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
#include "NI6259DIO.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * To keep MISRA happy about the signs...
 */
/*lint -e1923 -e9024 allow to use #define for constants.*/
#define PXI6259_NUMBER_OF_PORTS_ static_cast<uint32>(PXI6259_NUMBER_OF_PORTS)
#define INIT_HEADER_SIZE 0xFFFFFFFFu
/*lint +e1923 +e9024 */

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/*lint -efile(526,*NI6259DIO.cpp) symbols in header file.*/
/*lint -efile(628,*NI6259DIO.cpp) symbols in header file.*/

namespace MARTe {
NI6259DIO::NI6259DIO() :
        DataSourceI() {
    boardId = 0u;
    boardFileDescriptor = -1;
    rootFileDescriptor = -1;
    numberOfPortsEnabled = 0u;
    deviceName = "";
    triggerSet = false;
    uint32 n;
    for (n = 0u; n < PXI6259_NUMBER_OF_PORTS_; n++) {
        portEnabled[n] = false;
        portFileDescriptors[n] = -1;
        portMask[n] = 0u;
        portValues[n] = 0u;
        inversionMask[n]=0u;
    }
    headerSize = INIT_HEADER_SIZE;
    boardStatus = 0u;
}

NI6259DIO::~NI6259DIO() {
    uint32 n;
    for (n = 0u; n < PXI6259_NUMBER_OF_PORTS_; n++) {
        if (portFileDescriptors[n] != -1) {
            close(portFileDescriptors[n]);
        }
    }
    if (boardFileDescriptor != -1) {
        close(boardFileDescriptor);
    }
    if (rootFileDescriptor != -1) {
        close(rootFileDescriptor);
    }
}

bool NI6259DIO::AllocateMemory() {
    return true;
}

uint32 NI6259DIO::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool NI6259DIO::GetSignalMemoryBuffer(const uint32 signalIdx,
                                      const uint32 bufferIdx,
                                      void *&signalAddress) {
    bool ok = (headerSize != INIT_HEADER_SIZE);
    if (ok) {
        ok = (signalIdx < (PXI6259_NUMBER_OF_PORTS_+ headerSize));
    }
    if (ok) {
        if (signalIdx < headerSize) {
            if (signalIdx == 0u) {
                signalAddress = reinterpret_cast<void*>(&boardStatus);
            }
        }
        else {
            signalAddress = &(portValues[signalIdx - headerSize]);
        }
    }
    return ok;
}

const char8* NI6259DIO::GetBrokerName(StructuredDataI &data,
                                      const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8*);

    uint32 trigger = 0u;
    if (!data.Read("Trigger", trigger)) {
        trigger = 0u;
    }

    if (direction == OutputSignals) {
        if (trigger > 0u) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
            triggerSet = true;
        }
        else {
            brokerName = "MemoryMapOutputBroker";
        }
    }
    else {
        if (trigger > 0u) {
            brokerName = "MemoryMapSynchronisedInputBroker";
            triggerSet = true;
        }
        else {
            brokerName = "MemoryMapInputBroker";
        }
    }
    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: NOOP irrespectively of the input parameters.*/
bool NI6259DIO::PrepareNextState(const char8 *const currentStateName,
                                 const char8 *const nextStateName) {
    return true;
}

bool NI6259DIO::Initialise(StructuredDataI &data) {
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
    if (ok) {
        //Do not allow to add signals in run-time
        ok = signalsDatabase.MoveRelative("Signals");
        if (ok) {
            ok = signalsDatabase.Write("Locked", 1u);
        }
        if (ok) {
            ok = signalsDatabase.MoveToAncestor(1u);
        }
        if (ok) {
            ok = data.MoveRelative("Signals");
        }
        if (ok) {
            for (uint32 i = 0u; (i < data.GetNumberOfChildren()) && (ok); i++) {
                if (data.MoveRelative(data.GetChildName(i))) {
                    uint32 portId;
                    if (data.Read("PortId", portId)) {
                        if (headerSize == INIT_HEADER_SIZE) {
                            headerSize = i;
                        }
                        ok = (portId < PXI6259_NUMBER_OF_PORTS_);
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
                            if (!data.Read("InversionMask", inversionMask[portId])) {
                                inversionMask[portId] = 0u;
                            }
                        }
                    }
                    else {
                        ok = (headerSize == INIT_HEADER_SIZE);
                        if (!ok) {
                            REPORT_ERROR(ErrorManagement::InitialisationError, "Header signals must be defined before the channels");
                        }
                    }
                    (void) data.MoveToAncestor(1u);
                }
            }
            (void) data.MoveToAncestor(1u);
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }

        if (ok) {
            ok = (headerSize <= 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Header can be composed maximum by 1 signals: status");
            }
        }
    }
    return ok;
}

bool NI6259DIO::SetConfiguredDatabase(StructuredDataI &data) {
    uint32 i;
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = triggerSet;
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "At least one Trigger signal shall be set.");
        }
    }

    if (ok) {
        ok = (GetNumberOfSignals() == (headerSize + numberOfPortsEnabled));
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
        for (i = 0u; (i < numberOfPortsEnabled) && (ok); i++) {
            ok = (GetSignalType(headerSize + i) == UnsignedInteger32Bit);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The DIO ports shall be uint32 type");
            }
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
        fullDeviceName += ".dio";
        ok = fullDeviceName.Seek(0LLU);
    }
    if (ok) {
        boardFileDescriptor = open(fullDeviceName.Buffer(), O_RDWR);
        ok = (boardFileDescriptor > -1);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not open device %s", fullDeviceName);
        }
    }
    pxi6259_dio_conf_t dioConfiguration = pxi6259_create_dio_conf();
    for (i = 0u; (i < PXI6259_NUMBER_OF_PORTS_) && (ok); i++) {
        if (portEnabled[i]) {
            ok = (pxi6259_add_dio_channel(&dioConfiguration, static_cast<uint8>(i), portMask[i]) == 0);
            uint32 ii = i;
            if (ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Port %d set with mask %d ", ii, portMask[i]);
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not set configuration for port %d of device %s", ii, fullDeviceName);
            }
        }
    }
    if (ok) {
        ok = (pxi6259_load_dio_conf(boardFileDescriptor, &dioConfiguration) == 0);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not load configuration for device %s", fullDeviceName);
        }
    }

    if (ok) {
        //Required to wait for devices to be available in /dev!
        Sleep::Sec(1.0F);
        for (i = 0u; (i < PXI6259_NUMBER_OF_PORTS_) && (ok); i++) {
            if (portEnabled[i]) {
                StreamString portDeviceName;
                uint32 ii = i;
                ok = portDeviceName.Printf("%s.%d", fullDeviceName.Buffer(), ii);
                if (ok) {
                    ok = portDeviceName.Seek(0ULL);
                }
                if (ok) {
                    portFileDescriptors[i] = open(portDeviceName.Buffer(), O_RDWR);
                    ok = (portFileDescriptors[i] > -1);
                    if (!ok) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "Could not open device %s", portDeviceName);
                    }
                }
            }
        }
    }
    return ok;
}

bool NI6259DIO::Synchronise() {
    uint32 i;
    bool ok = true;
    for (i = 0u; (i < PXI6259_NUMBER_OF_PORTS_) && (ok); i++) {
        if (portEnabled[i]) {
            uint32 portWrite=(portValues[i]^inversionMask[i]);
            ok = (write(portFileDescriptors[i], &portWrite, sizeof(uint32)) == static_cast<ssize_t>(sizeof(int32)));
            if (ok) {
                uint32 portRead;
                ok = (read(portFileDescriptors[i], &portRead, sizeof(uint32)) == static_cast<ssize_t>(sizeof(int32)));
                portValues[i]=(portRead^inversionMask[i]);
            }
        }
    }

    //lint -e{565} -e{1080} -e{1055} -e{516} -e{115} -e{1030} -e{40} -e{1013} symbols are in header file
    if (headerSize > 0u) {
        struct pxi6259_board_status boardStatusT;
        if (ok) {
            ok = (pxi6259_get_board_status(rootFileDescriptor, &boardStatusT) == 0);
        }
        boardStatus = boardStatusT.currentBoardStatus;
    }

    return ok;
}

bool NI6259DIO::ReadDIOConfiguration(pxi6259_dio_conf_t *const conf) const {
    bool ok = false;
    if (boardFileDescriptor > 0) {
        ok = (pxi6259_read_dio_conf(boardFileDescriptor, conf) == 0);
    }
    return ok;
}

CLASS_REGISTER(NI6259DIO, "1.0")
}

