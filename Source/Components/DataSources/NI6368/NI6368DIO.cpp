/**
 * @file NI6368DIO.cpp
 * @brief Source file for class NI6368DIO
 * @date 30/01/2017
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
 * the class NI6368DIO (public, protected, and private). Be aware that some
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

#include "NI6368DIO.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*lint -e1923 allow macros*/
#define INIT_HEADER_SIZE 0xFFFFFFFFu
/*lint +e1923 */

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/*lint -efile(526,*NI6368DIO.cpp) symbols in header file.*/
/*lint -efile(628,*NI6368DIO.cpp) symbols in header file.*/
namespace MARTe {
NI6368DIO::NI6368DIO() :
        DataSourceI() {

    boardId = 0u;
    boardFileDescriptor = -1;
    pfiFileDescriptor = -1;
    rootFileDescriptor = -1;
    deviceName = "";
    for (uint32 i = 0u; i < PXI6363_NUMBER_OF_PORTS; i++) {
        portValue[i] = 0u;
        portMask[i] = 0u;
        portEnabled[i] = false;
        inversionMask[i] = 0u;
    }
    triggerSet = false;
    numberOfPortsEnabled = 0u;
    headerSize = INIT_HEADER_SIZE;
    boardStatus = 0u;
}

/*lint -e{1551} the destructor must guarantee that all the file descriptors are closed. */
NI6368DIO::~NI6368DIO() {
    if (boardFileDescriptor != -1) {
        bool ok = (xseries_stop_di(boardFileDescriptor) == 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not stop digital input on device");
        }
        if (ok) {
            ok = (xseries_stop_do(boardFileDescriptor) == 0);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Could not stop digital output on device");
            }
        }
        close(boardFileDescriptor);
    }
    if (pfiFileDescriptor != -1) {

        close(pfiFileDescriptor);
    }
    if (rootFileDescriptor != -1) {

        close(rootFileDescriptor);
    }

}

bool NI6368DIO::AllocateMemory() {
    return true;
}

uint32 NI6368DIO::GetNumberOfMemoryBuffers() {
    return 1u;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool NI6368DIO::GetSignalMemoryBuffer(const uint32 signalIdx,
                                      const uint32 bufferIdx,
                                      void *&signalAddress) {
    bool ok = (headerSize != INIT_HEADER_SIZE);
    if (ok) {
        ok = (signalIdx < (PXI6363_NUMBER_OF_PORTS + headerSize));
    }
    if (ok) {
        if (signalIdx < headerSize) {
            if (signalIdx == 0u) {
                signalAddress = reinterpret_cast<void*>(&boardStatus);
            }
        }
        else {
            signalAddress = &(portValue[signalIdx - headerSize]);
        }
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The broker name depends only on the direction.*/
const char8* NI6368DIO::GetBrokerName(StructuredDataI &data,
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
bool NI6368DIO::PrepareNextState(const char8 *const currentStateName,
                                 const char8 *const nextStateName) {
    return true;
}

bool NI6368DIO::Initialise(StructuredDataI &data) {
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
                        ok = (portId < PXI6363_NUMBER_OF_PORTS);
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

bool NI6368DIO::SetConfiguredDatabase(StructuredDataI &data) {
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
        for (uint32 i = 0u; (i < numberOfPortsEnabled) && ok; i++) {
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

        for (uint32 i = 0u; (i < nOfSignals) && (ok); i++) {
            uint32 nSamples = 0u;
            ok = GetFunctionSignalSamples(InputSignals, functionIdx, i, nSamples);
            if (ok) {
                ok = (nSamples == 1u);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Only one sample shall be specified for the input");
            }
        }

        if (ok) {
            ok = GetFunctionNumberOfSignals(OutputSignals, functionIdx, nOfSignals);
        }

        for (uint32 i = 0u; (i < nOfSignals) && (ok); i++) {
            uint32 nSamples = 0u;
            ok = GetFunctionSignalSamples(OutputSignals, functionIdx, i, nSamples);
            if (ok) {
                ok = (nSamples == 1u);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Only one sample shall be specified for the output");
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
    if (ok) {
        if (portEnabled[0u]) {
            StreamString fullDeviceName = rootDeviceName;
            //Configure the board
            fullDeviceName += ".dio";
            ok = fullDeviceName.Seek(0LLU);
            if (ok) {
                boardFileDescriptor = open(fullDeviceName.Buffer(), O_RDWR);
                ok = (boardFileDescriptor > -1);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not open device %s", fullDeviceName);
                }
            }

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
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not open device %s", fullDeviceName);
                }
            }

            if (ok) {
                ok = (xseries_stop_di(boardFileDescriptor) == 0);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not xseries_stop_di on device %s", fullDeviceName);
                }
            }
            if (ok) {
                ok = (xseries_stop_do(boardFileDescriptor) == 0);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not xseries_stop_do on device %s", fullDeviceName);
                }
            }
            if (ok) {
                ok = (xseries_reset_dio(boardFileDescriptor) == 0);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not xseries_reset_dio on device %s", fullDeviceName);
                }
            }

            if (ok) {
                uint32 inputPortMask = ((~(portMask[0u])) & (0xFFFFFFFFu));
                xseries_di_conf_t diConfiguration = xseries_static_di(inputPortMask);
                ok = (xseries_load_di_conf(boardFileDescriptor, diConfiguration) == 0);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not load xseries_load_di_conf for device %s", fullDeviceName);
                }
            }
            if (ok) {
                xseries_do_conf_t doConfiguration = xseries_static_do(portMask[0u]);
                ok = (xseries_load_do_conf(boardFileDescriptor, doConfiguration) == 0);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not load xseries_load_do_conf for device %s", fullDeviceName);
                }
            }
            if (ok) {
                ok = (xseries_start_di(boardFileDescriptor) == 0);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not load xseries_start_di for device %s", fullDeviceName);
                }
            }
            if (ok) {
                ok = (xseries_start_do(boardFileDescriptor) == 0);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Could not load xseries_start_do for device %s", fullDeviceName);
                }
            }
        }
    }
    uint32 pfiMask = 0u;
    int32 trigFileDescriptor = -1;
    bool pfiPortsEnabled = false;
    if (ok) {
        for (uint32 i = 1u; i < PXI6363_NUMBER_OF_PORTS; i++) {
            if (portEnabled[i]) {
                if (pfiFileDescriptor < 0) {
                    StreamString pfiFullDeviceName = rootDeviceName;
                    if (ok) {
                        pfiFullDeviceName += ".pfi";
                        ok = pfiFullDeviceName.Seek(0LLU);
                    }

                    if (ok) {
                        pfiFileDescriptor = open(pfiFullDeviceName.Buffer(), O_RDWR);
                        ok = (pfiFileDescriptor > -1);
                        if (!ok) {
                            REPORT_ERROR(ErrorManagement::FatalError, "Could not open device %s", pfiFullDeviceName);
                        }
                    }
                    StreamString trigFullDeviceName;
                    if (ok) {
                        ok = trigFullDeviceName.Printf("%s.%d.trig", deviceName.Buffer(), boardId);
                    }
                    if (ok) {
                        ok = trigFullDeviceName.Seek(0LLU);
                    }

                    if (ok) {
                        trigFileDescriptor = open(trigFullDeviceName.Buffer(), O_RDWR);
                        ok = (trigFileDescriptor > -1);
                        if (!ok) {
                            REPORT_ERROR(ErrorManagement::FatalError, "Could not open device %s", trigFullDeviceName);
                        }
                    }

                    if (ok) {
                        ok = (xseries_free_pfi_lines(trigFileDescriptor, 0xFFFFu) == 0);
                        if (!ok) {
                            REPORT_ERROR(ErrorManagement::FatalError, "Cannot free all PFI lines");
                        }
                    }

                    if (ok) {
                        ok = (xseries_reset_pfi(trigFileDescriptor) == 0);
                        if (!ok) {
                            REPORT_ERROR(ErrorManagement::FatalError, "Cannot reset PFI segment");
                        }
                    }

                    pfiPortsEnabled = true;
                }
            }
            uint32 shift = (8u * (i - 1u));
            pfiMask |= (portMask[i] << shift);
        }

        if (ok) {
            if (pfiPortsEnabled) {
                ok = (xseries_set_pfi_for_output(trigFileDescriptor, pfiMask, XSERIES_PFI_OUTPUT_DO) == 0);
                if (ok) {
                    uint32 inputMask = (~(pfiMask)) & 0xFFFFu;
                    ok = (xseries_set_pfi_for_input(trigFileDescriptor, inputMask) == 0);
                }
            }
            close(trigFileDescriptor);
        }

    }

    return ok;
}

bool NI6368DIO::Synchronise() {
    bool ok = true;

    /*lint -e{712} -e{747} -e{9117} accept coercion, loss of precision and change of sign*/
    if (portEnabled[0]) {
        //REPORT_ERROR(ErrorManagement::Information, "Write %0x on port 0", portValue[0]);
        uint32 pfiVal = (portValue[0] ^ inversionMask[0]);

        ok = (xseries_write_do(boardFileDescriptor, &pfiVal, 1u) == 1);
    }

    /*lint -e{712} -e{747} -e{9117} -e{9119} accept coercion, loss of precision and change of sign*/
    if (ok) {
        uint32 pfiVal = 0u;
        uint32 invMask = 0u;
        for (uint32 i = 1u; i < PXI6363_NUMBER_OF_PORTS; i++) {
            if (portEnabled[i]) {
                uint32 shift = (8u * (i - 1u));
                pfiVal |= (portValue[i] << shift);
                invMask |= ((inversionMask[i] & 0xFFu) << shift);
                //REPORT_ERROR(ErrorManagement::Information, "Write %0x on port %d", portValue[i], i);
            }
        }
        pfiVal ^= invMask;
        uint32 wbytes = xseries_write_pfi(pfiFileDescriptor, &pfiVal, 1u);
        //REPORT_ERROR(ErrorManagement::Information, "Write %0x on port 1-2", pfiVal);
        ok = (wbytes > 0u);
    }

    /*lint -e{712} -e{747} -e{9117} accept coercion, loss of precision and change of sign*/
    if (ok) {
        if (portEnabled[0]) {
            uint32 pfiVal = 0u;
            ssize_t ret = xseries_read_di(boardFileDescriptor, &pfiVal, 1u);
            portValue[0] = (pfiVal ^ inversionMask[0]);
            //REPORT_ERROR(ErrorManagement::Information, "Read %0x from port 0", portValue[0]);
            if (ret != 1) {
                ok = (ret == EAGAIN);
            }
        }
    }

    /*lint -e{712} -e{747} -e{9117} -e{9119} accept coercion, loss of precision and change of sign*/
    if (ok) {
        uint32 pfiVal = 0u;
        uint32 rbytes = xseries_read_pfi(pfiFileDescriptor, &pfiVal, 1u);
        //REPORT_ERROR(ErrorManagement::Information, "Read %0x from port 1-2", pfiVal);

        ok = (rbytes > 0u);
        if (ok) {
            for (uint32 i = 1u; i < PXI6363_NUMBER_OF_PORTS; i++) {
                if (portEnabled[i]) {
                    uint32 shift = (8u * (i - 1u));
                    portValue[i] = ((pfiVal >> shift) & 0xFFu) ^ (inversionMask[i] & 0xFFu);
                    //REPORT_ERROR(ErrorManagement::Information, "Read %0x from port %d", portValue[i], i);
                }
            }
        }
    }

    //lint -e{40} -e{1013} -e{522} -e{1055} symbols are in header file
    if (headerSize > 0u) {
        xseries_device_status_t devStatus;
        if (ok) {
            ok = (xseries_get_device_status(rootFileDescriptor, &devStatus) == 0);
        }
        boardStatus = devStatus.currentBoardStatus;
    }

    //REPORT_ERROR(ErrorManagement::Information, "=====================================");

    return ok;
}

bool NI6368DIO::ReadDIOConfiguration(xseries_di_conf_t *const confDI,
                                     xseries_do_conf_t *const confDO) const {
    bool ok = false;
    if (boardFileDescriptor > 0) {
        ok = (xseries_read_di_conf(boardFileDescriptor, confDI) == 0);
        if (ok) {
            ok = (xseries_read_do_conf(boardFileDescriptor, confDO) == 0);
        }
    }
    return ok;
}

CLASS_REGISTER(NI6368DIO, "1.0")
}

