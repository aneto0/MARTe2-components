/**
 * @file NI6528.cpp
 * @brief Source file for class NI6528
 * @date 01/06/2026
 * @author Giuseppe Ferro
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
 * the class NI6528 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "NI6528.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*lint -e1923 allow macros*/
#define INVALID_PXI6528_PORT_ID_ 0xFFFFFFFFu
#define PXI6528_OUTPUT_START_PORT_IDX_ 3u
/*lint +e1923 */

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

NI6528::NI6528() :
        DataSourceI() {

    for (uint32 i = 0u; i < PXI6528_NUMBER_OF_PORTS_; i++) {
        value[i] = 0u;
        portEnabled[i] = INVALID_PXI6528_PORT_ID_;
        inversionMask[i] = 0u;
    }
    boardFileDescriptor = NULL_PTR(pxi6528_device_t);
    boardId = 0u;
}

/*lint -efile(1740,*NI6528.*) boardFileDescriptor is freed by the driver.*/
NI6528::~NI6528() {
    (void) pxi6528_close_device(boardFileDescriptor);
}

bool NI6528::Initialise(StructuredDataI &data) {
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
        if (data.MoveRelative("Signals")) {
            for (uint32 i = 0u; (i < data.GetNumberOfChildren()) && (ok); i++) {
                if (data.MoveRelative(data.GetChildName(i))) {
                    uint32 portId = 0u;
                    if (data.Read("PortId", portId)) {
                        ok = (portId < PXI6528_NUMBER_OF_PORTS_);
                        if (ok) {
                            ok = (portEnabled[portId] == INVALID_PXI6528_PORT_ID_);
                            if (ok) {
                                portEnabled[portId] = i;
                                if (!data.Read("InversionMask", inversionMask[portId])) {
                                    inversionMask[portId] = 0u;
                                }
                            }
                            else {
                                REPORT_ERROR(ErrorManagement::ParametersError, "PortId %d declared twice", portId);
                            }
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::ParametersError, "Invalid PortId specified.");
                        }
                    }
                    if (ok) {
                        ok = data.MoveToAncestor(1u);
                    }
                }
            }
            if (ok) {
                ok = data.MoveToAncestor(1u);
            }
        }
    }
    return ok;
}

bool NI6528::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = (DataSourceI::SetConfiguredDatabase(data));
    StreamString fullDeviceName;
    if (ok) {
        ok = fullDeviceName.Printf("%s.%d", deviceName.Buffer(), boardId);
    }
    if (ok) {
        ok = fullDeviceName.Seek(0LLU);
    }
    if (ok) {
        int32 ret = pxi6528_open_device(&boardFileDescriptor, fullDeviceName.Buffer(), O_NONBLOCK);
        ok = (ret == 0);
        if (!ok) {
            //lint -e64 strerror returns a string not an int
            StreamString err = strerror(-ret);
            REPORT_ERROR(ErrorManagement::FatalError, "Could not open device (%s) : %s", deviceName.Buffer(), err.Buffer());
        }
    }
    if (ok) {
        ok = (GetNumberOfSignals() <= PXI6528_NUMBER_OF_PORTS_);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The number of signals must be <= %d", PXI6528_NUMBER_OF_PORTS_);
        }
    }
    if (ok) {
        //lint -e{850} i is not modifed by REPORT_ERROR
        for (uint32 i = 0u; (i < numberOfSignals) && ok; i++) {
            ok = (GetSignalType(i) == UnsignedInteger8Bit);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "The signal %d type must be uint8", i);
            }
        }
    }

    return ok;
}

bool NI6528::AllocateMemory() {
    return true;
}

bool NI6528::Synchronise() {
    bool ok = true;

    for (uint32 port = 0u; (port < PXI6528_NUMBER_OF_PORTS_) && ok; port++) {
        if (portEnabled[port] != INVALID_PXI6528_PORT_ID_) {
            if (port < PXI6528_OUTPUT_START_PORT_IDX_) {
                uint8 val;
                int32 ret = pxi6528_read_port(boardFileDescriptor, static_cast<uint8>(port), &val, 1u);
                value[portEnabled[port]] = (val ^ inversionMask[port]);

                ok = (ret >= 0);
                if (!ok) {
                    //lint -e64 strerror returns a string not an int
                    StreamString err = strerror(-ret);
                    REPORT_ERROR(ErrorManagement::FatalError, "Could not write device (%s.%d) : %s", deviceName.Buffer(), boardId, err.Buffer());
                }
            }
            else {
                uint8 val = (value[portEnabled[port]] ^ inversionMask[port]);
                int32 ret = pxi6528_write_port(boardFileDescriptor, static_cast<uint8>(port), val);
                ok = (ret >= 0);
                if (!ok) {
                    //lint -e64 strerror returns a string not an int
                    StreamString err = strerror(-ret);
                    REPORT_ERROR(ErrorManagement::FatalError, "Could not write device (%s.%d) : %s", deviceName.Buffer(), boardId, err.Buffer());
                }
            }
        }
    }

    return ok;
}

//lint -e{715} bufferIdx is not required but the method is virtual
bool NI6528::GetSignalMemoryBuffer(const uint32 signalIdx,
                                   const uint32 bufferIdx,
                                   void *&signalAddress) {
    bool ret = (signalIdx < PXI6528_NUMBER_OF_PORTS_);
    if (ret) {
        signalAddress = &value[signalIdx];
    }
    return ret;
}

const char8* NI6528::GetBrokerName(StructuredDataI &data,
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

    bool isSync = ((frequency >= 0.F) || (trigger > 0u));
    if (direction == OutputSignals) {
        if (isSync) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
        }
        else {
            brokerName = "MemoryMapOutputBroker";
        }
    }
    else if (direction == InputSignals) {
        if (isSync) {
            brokerName = "MemoryMapSynchronisedInputBroker";
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

//lint -e{715} currentStateName and nextStateName are unreferenced because the DataSource uses single state
bool NI6528::PrepareNextState(const char8 *const currentStateName,
                              const char8 *const nextStateName) {
    return true;
}

CLASS_REGISTER(NI6528, "1.0")

}
