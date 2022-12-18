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
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryMapInputBroker.h"
#include "UDPReceiver.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


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
    executionMode = UDPReceiverExecutionModeRealTime;
    socket = NULL_PTR(BasicUDPSocket*);
    muxIThread.Create();
    copyInProgress = false;
    memoryIndependentThread = NULL_PTR(void *);
}

/*lint -e{1551} the destructor must guarantee that the thread and servers are closed.*/
UDPReceiver::~UDPReceiver() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    if (socket != NULL_PTR(BasicUDPSocket*)) {
        if (!socket->Close()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop the UDP receiver.");
        }
        else {
            delete socket;
        }
    }
    if (memoryIndependentThread != NULL_PTR(void *)) {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(memoryIndependentThread);
    }
}

bool UDPReceiver::AllocateMemory() {
    bool ok = MemoryDataSourceI::AllocateMemory();
    if (executionMode == UDPReceiverExecutionModeIndependent) {
        memoryIndependentThread = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalMemorySize);
        if (ok) {
            executor.SetName(GetName());
            ok = (executor.Start() == ErrorManagement::NoError);
        }
    }
    return ok;
}

bool UDPReceiver::BrokerCopyTerminated() {
    if (muxIThread.FastLock() == ErrorManagement::NoError) {
        copyInProgress = false;
    }
    muxIThread.FastUnLock();
    return true;
}

bool UDPReceiver::Initialise(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::Initialise(data);
    if (ok) {
        StreamString tempExecModeStr;
        ok = data.Read("ExecutionMode", tempExecModeStr);
        if(!ok) {
            REPORT_ERROR(ErrorManagement::Information, "Execution mode was not specified, defaulting to IndependentThread mode");
            executionMode = UDPReceiverExecutionModeRealTime;
            ok = true;
        }
        else {
            if(tempExecModeStr == "IndependentThread") {
                executionMode = UDPReceiverExecutionModeIndependent;
                REPORT_ERROR(ErrorManagement::Information, "Running in IndependentThread mode");
            }
            else if(tempExecModeStr == "RealTimeThread") {
                executionMode = UDPReceiverExecutionModeRealTime;
                REPORT_ERROR(ErrorManagement::Information, "Running in RealTimeThread mode");
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Specified execution mode is not allowed");
                REPORT_ERROR(ErrorManagement::Information, "Allowed [IndependendThread, RealTimeThread], Specified %s", tempExecModeStr);
                ok = false;
            }
        }
    }

    if (ok) {
        ok = data.Read("Address", address);
        if (!ok) {
            address = "";
            ok = true;
            REPORT_ERROR(ErrorManagement::Information, "No Address specified");
        }
    }
    if (ok) {
        ok = data.Read("InterfaceAddress", interfaceAddress);
        if (!ok) {
            interfaceAddress = "";
            ok = true;
            REPORT_ERROR(ErrorManagement::Information, "No InterfaceAddress specified");
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
    if (executionMode == UDPReceiverExecutionModeIndependent) {
        if (ok) {
            ok = data.Read("CPUMask", cpuMask);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::Information, "No CPUMask defined. Using default 0x1u.");
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
        socket = new BasicUDPSocket;

        ok = socket->Open();
    }
    if (ok) {
        if (address.Size() > 0LLU) {
            ok = address.Seek(0LLU);
            if (ok) {
                char8 ignore;
                StreamString networkBlock = "";
                ok = address.GetToken(networkBlock, ".", ignore);
                if (ok) {
                    /*lint -e{1055} -e{746} -e{526} Justification: atoi returns int32*/
                    int32 netValue = static_cast<int32>(strtol(networkBlock.Buffer(), NULL_PTR(char8**), 10));
                    if ((netValue >= 224) && (netValue <= 239)) {
                        /* The net address belongs to the multicast address range therefore it must be a multicast group */
                        if (socket != NULL_PTR(BasicUDPSocket*)) {
                            if (interfaceAddress.Size() > 0LLU) {
                                ok = socket->Join(address.Buffer(), interfaceAddress.Buffer());
                            }
                            else {
                                ok = socket->Join(address.Buffer());
                            }
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
        if (socket != NULL_PTR(BasicUDPSocket*)) {
            ok = socket->Listen(port);
        }
    }
    if (executionMode == UDPReceiverExecutionModeIndependent) {
        executor.SetPriorityClass(Threads::RealTimePriorityClass);
        executor.SetCPUMask(cpuMask);
        executor.SetStackSize(stackSize);
    }

    return ok;
}

bool UDPReceiver::Synchronise() {
    bool ok = true;
    if (executionMode == UDPReceiverExecutionModeIndependent) {
        if (muxIThread.FastLock() == ErrorManagement::NoError) {
            copyInProgress = true;
        }
        muxIThread.FastUnLock();
    }
    else {
        char8 *const dataBuffer = reinterpret_cast<char8*>(memory);
        if (socket != NULL_PTR(BasicUDPSocket*)) {
            //Do not overwrite the read memory size
            uint32 sizeToRead = totalMemorySize;
            ok = socket->Read(dataBuffer, sizeToRead, timeout);
        }
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the data is independent of the broker name.*/
const char8* UDPReceiver::GetBrokerName(StructuredDataI &data,
                                        const SignalDirection direction) {
    const char8 *brokerName = "MemoryMapSynchronisedInputBroker";
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
        char8 *const dataBuffer = reinterpret_cast<char8*>(memoryIndependentThread);
        if (socket != NULL_PTR(BasicUDPSocket*)) {
            //Do not overwrite the read memory size
            uint32 sizeToRead = totalMemorySize;
            err.timeout = !socket->Read(dataBuffer, sizeToRead, timeout);
        }
        bool canCopyMemory = false;
        if (muxIThread.FastLock() == ErrorManagement::NoError) {
            canCopyMemory = !copyInProgress;
        }
        muxIThread.FastUnLock();
        if (canCopyMemory) {
            err.fatalError = !MemoryOperationsHelper::Copy(memory, dataBuffer, totalMemorySize);
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

const UDPReceiverExecutionMode UDPReceiver::GetExecutionMode() const {
    return executionMode;
}

CLASS_REGISTER(UDPReceiver, "1.0")

}
