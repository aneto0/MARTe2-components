/**
 * @file UDPReceiver.cpp
 * @brief Source file for class UDPReceiver
 * @date 11/05/2021
 * @author Luca Porzio
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
 * the class UDPReceiver (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#include "stdlib.h"

#include "AdvancedErrorManagement.h"
#include "MemoryMapInputBroker.h"
#include "UDPReceiver.h"

namespace MARTe {

UDPReceiver::UDPReceiver() :
        MemoryDataSourceI(),
        EmbeddedServiceMethodBinderI(),
        executor(*this) {
    address = "";
    port = 0u;
    timeout = TTInfiniteWait;
    cpuMask = 0u;
    stackSize = THREADS_DEFAULT_STACKSIZE;
    sync = 0u;
    socket = NULL_PTR(UDPSocket*);
}

/*lint -e{1551} the destructor must guarantee that the thread and servers are closed.*/
UDPReceiver::~UDPReceiver() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    if (socket != NULL_PTR(UDPSocket*)) {
        if (!socket->Close()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop the UDP receiver.");
        }
        else {
            delete socket;
        }
    }
}

bool UDPReceiver::AllocateMemory() {
    bool ok = MemoryDataSourceI::AllocateMemory();
    if (sync == 0u) {
        if (ok) {
            executor.SetName(GetName());
            ok = (executor.Start() == ErrorManagement::NoError);
        }
    }
    return ok;
}

bool UDPReceiver::Initialise(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("Address", address);
        if (!ok) {
            address = "NULL";
            ok = true;
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Using Multicast interface.");
        }
    }
    if (ok) {
        ok = data.Read("Port", port);
        if (!ok) {
            port = 44488u;
            REPORT_ERROR(ErrorManagement::Information, "No valid Port defined! Using Default 44488");
            ok = true;
        }
        else {
            if (port <= 1024u) {
                REPORT_ERROR(ErrorManagement::Warning, "Port is set to %d, possible issues with values < 1024", port);
            }
        }
    }
    if (ok) {
        float64 timeoutVal;
        ok = data.Read("Timeout", timeoutVal);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "No Timeout defined! Using Infinite");
            ok = true;
        }
        else {
            timeout.SetTimeoutSec(timeoutVal);
        }
    }
    if (ok) {
        ok = data.Read("Sync", sync);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "No Synchronisation. Using Thread execution.");
            ok = true;
        }
    }
    if (sync == 0u) {
        if (ok) {
            ok = data.Read("CpuMask", cpuMask);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::Information, "No CpuMask defined. Using default 0x1u.");
                ok = true;
            }
        }
        if (ok) {
            ok = data.Read("StackSize", stackSize);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::Information, "No StackSize defined. Using default thread stack size.");
                ok = true;
            }
        }
    }
    return ok;
}

bool UDPReceiver::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        socket = new UDPSocket;

        ok = socket->Open();
    }
    if (ok) {
        if (address != "NULL") {
            ok = address.Seek(0LLU);
            if (ok) {
                char8 ignore;
                StreamString networkBlock = "";
                ok = address.GetToken(networkBlock, ".", ignore);
                if (ok) {
                    /*lint -e{1055} Justification: atoi returns int32*/
                    int32 netValue = static_cast<int32>(strtol(networkBlock.Buffer(), NULL_PTR(char8**), 10));
                    if ((netValue >= 224) && (netValue <= 239)) {
                        /* The net address belongs to the multicast address range therefore it must be a multicast group */
                        if (socket != NULL_PTR(UDPSocket*)) {
                            ok = socket->Join(address.Buffer());
                        }
                    }
                    else {
                        ok = false;
                        REPORT_ERROR(ErrorManagement::ParametersError, "No valid address for Multicast group.");
                    }
                }
            }
        }
    }
    if (ok) {
        if (socket != NULL_PTR(UDPSocket*)) {
            ok = socket->Listen(port);
        }
    }
    if (sync == 0u) {
        executor.SetCPUMask(cpuMask);
        executor.SetStackSize(stackSize);
    }

    return ok;
}

bool UDPReceiver::Synchronise() {
    bool ok = false;
    char8 *const dataBuffer = reinterpret_cast<char8*>(memory);
    if (socket != NULL_PTR(UDPSocket*)) {
        ok = socket->Read(dataBuffer, totalMemorySize, timeout);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the data is independent of the broker name.*/
const char8* UDPReceiver::GetBrokerName(StructuredDataI &data,
                                        const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8*);
    if (sync == 0u) {
        if (direction == InputSignals) {
            brokerName = "MemoryMapInputBroker";
        }
    }
    else {
        if (direction == InputSignals) {
            brokerName = "MemoryMapSynchronisedInputBroker";
        }
    }
    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the current and next state name are indepentent of the operation.*/
bool UDPReceiver::PrepareNextState(const char8 *const currentStateName,
                                   const char8 *const nextStateName) {
    bool ok = true;
    return ok;
}

ErrorManagement::ErrorType UDPReceiver::Execute(ExecutionInfo &info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        char8 *const dataBuffer = reinterpret_cast<char8*>(memory);
        bool ok = false;
        if (socket != NULL_PTR(UDPSocket*)) {
            ok = socket->Read(dataBuffer, totalMemorySize, timeout);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "Connection Timeout.");
        }
    }
    return err;
}

uint32 UDPReceiver::GetStackSize() const {
    return stackSize;
}

uint16 UDPReceiver::GetPort() const {
    return port;
}

StreamString UDPReceiver::GetAddress() const {
    return address;
}

const uint32 UDPReceiver::GetCPUMask() const {
    return cpuMask;
}

const uint32 UDPReceiver::GetSync() const {
    return sync;
}

CLASS_REGISTER(UDPReceiver, "1.0")

}
