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

#include "AdvancedErrorManagement.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
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
}

/*
 * Destructor
 */
/*lint -e{1551} the destructor must guarantee that the thread and servers are closed.*/
UDPReceiver::~UDPReceiver() {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }

    if (!socket.Close()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not stop the UDP reciever server.");
    }
}

bool UDPReceiver::Initialise(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("Address", address);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Address or Multicast group must be specified");
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
            if (port <= 1024) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Port is set to %d, possible issues with values < 1024", port);
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
    }
    if (ok) {
        ok = data.Read("Sync", sync);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Information, "No Synchronisation. Using Thread execution.");
            ok = true;
        }
        if (ok) {
            if (sync > 1u) {
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "Sync: wrong value.");
            }
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
        ok = socket.Open();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not open socket!");
        }
    }
    if (ok) {
        ok = socket.Listen(port);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed listening on port number %d", port);
        }
    }
    if (ok) {
        ok = socket.Join(address.Buffer());
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed joining group %s", address.Buffer());
        }
    }
    if (sync == 0u) {
        executor.SetCPUMask(cpuMask);
        executor.SetStackSize(stackSize);
        executor.SetName(GetName());
        ok = (executor.Start() == ErrorManagement::NoError);
    }
//    if (ok) {
//        nOfSignals = GetNumberOfSignals();
//        uint16 i;
//        uint32 signalByteSize;
//        signalsMemoryOffset = new uint32[nOfSignals];
//        signalsMemoryOffset[0] = 0u;
//        signalsMemoryOffset[1] = 8u; // To account for sequenceNumber to be stored as uint64
//        signalsMemoryOffset[2] = 16u; // To account for timer to be stored as uint64
//        for (i = 3u; i < nOfSignals; i++) {
//            uint16 previousSignalIdx = i - 1u;
//            ok = GetSignalByteSize(previousSignalIdx, signalByteSize);
//            if (ok) {
//                signalsMemoryOffset[i] = signalsMemoryOffset[i - 1u] + signalByteSize;
//            }
//        }
//        uint32 lastSignalIdx = nOfSignals - 1u;
//        ok = GetSignalByteSize(lastSignalIdx, signalByteSize);
//        if (ok) {
//            totalPacketSize = signalsMemoryOffset[lastSignalIdx] + signalByteSize;
//        }
//        ok = (GetSignalType(0u).numberOfBits == 64u);
//        if (!ok) {
//            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The first signal shall have 64 bits and %d were specified",
//                                    uint16(GetSignalType(0u).numberOfBits));
//        }
//    }
//    if (ok) {
//        ok = (GetSignalType(0u).type == SignedInteger);
//        if (!ok) {
//            ok = (GetSignalType(0u).type == UnsignedInteger);
//        }
//        if (!ok) {
//            REPORT_ERROR(ErrorManagement::ParametersError, "The first signal shall SignedInteger or UnsignedInteger type");
//        }
//    }
//    if (ok) {
//        ok = (GetSignalType(1u).numberOfBits == 64u);
//        if (!ok) {
//            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The second signal shall have 64 bits and %d were specified",
//                                    uint16(GetSignalType(1u).numberOfBits));
//        }
//    }
//    if (ok) {
//        ok = (GetSignalType(1u).type == SignedInteger);
//        if (!ok) {
//            ok = (GetSignalType(1u).type == UnsignedInteger);
//        }
//        if (!ok) {
//            REPORT_ERROR(ErrorManagement::ParametersError, "The second signal shall SignedInteger or UnsignedInteger type");
//        }
//    }
    return ok;
}

/**
 * Required for DataSourceI.h
 */
bool UDPReceiver::Synchronise() {
    bool ok = false;
    char8 *const dataBuffer = reinterpret_cast<char8*>(memory);
    ok = socket.Read(dataBuffer, totalMemorySize, timeout);
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

//bool UDPReceiver::GetInputBrokers(ReferenceContainer &inputBrokers,
//                                  const char8 *const functionName,
//                                  void *const gamMemPtr) {
//    uint32 functionIdx = 0u;
//    uint32 numbOfSignals = 0u;
//    bool synchGAM = false;
//    bool ok = GetFunctionIndex(functionIdx, functionName);
//    if (ok) {
//        ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, numbOfSignals);
//    }
//
//    uint32 i;
//    float32 frequency = 0.F;
//    for (i = 0u; (i < numbOfSignals) && (ok) && (!synchGAM); i++) {
//        ok = GetFunctionSignalReadFrequency(InputSignals, functionIdx, i, frequency);
//        synchGAM = (frequency > 0.F);
//    }
//    if ((synchronising) && (synchGAM)) {
//        ReferenceT<MemoryMapSynchronisedInputBroker> brokerSync("MemoryMapSynchronisedInputBroker");
//        if (ok) {
//            ok = brokerSync.IsValid();
//        }
//        if (ok) {
//            ok = brokerSync->Init(InputSignals, *this, functionName, gamMemPtr);
//        }
//        if (ok) {
//            ok = inputBrokers.Insert(brokerSync);
//        }
//        uint32 nOfFunctionSignals = 0u;
//        if (ok) {
//            ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfFunctionSignals);
//        }
//        if (ok) {
//            if (nOfFunctionSignals > 1u) {
//                ReferenceT<MemoryMapInputBroker> brokerNotSync("MemoryMapInputBroker");
//                ok = brokerNotSync.IsValid();
//                if (ok) {
//                    ok = brokerNotSync->Init(InputSignals, *this, functionName, gamMemPtr);
//                }
//                if (ok) {
//                    ok = inputBrokers.Insert(brokerNotSync);
//                }
//            }
//        }
//    }
//    else {
//        ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
//        ok = broker.IsValid();
//        if (ok) {
//            ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
//        }
//        if (ok) {
//            ok = inputBrokers.Insert(broker);
//        }
//    }
//    return ok;
//}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: THe output broker is not needed*/
//bool UDPReceiver::GetOutputBrokers(ReferenceContainer &outputBrokers,
//                                   const char8 *const functionName,
//                                   void *const gamMemPtr) {
//    return false;
//}
/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the current and next state name are indepentent of the operation.*/
bool UDPReceiver::PrepareNextState(const char8 *const currentStateName,
                                   const char8 *const nextStateName) {
    bool ok = true;
//    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
//        keepRunning = true;
//        if (cpuMask != 0u) {
//            executor.SetCPUMask(cpuMask);
//        }
//        ok = executor.Start();
//    }
//    if (sequenceNumberPtr == NULL_PTR(uint64*)) {
//        ok = false;
//        REPORT_ERROR(ErrorManagement::FatalError, "Variable \"sequenceNumberPtr\" was not initialised!");
//    }
//    else {
//        *sequenceNumberPtr = 0u;
//    }
//    if (ok) {
//        if (timerPtr == NULL_PTR(uint64*)) {
//            ok = false;
//            REPORT_ERROR(ErrorManagement::FatalError, "Variable \"timerPtr\" was not initialised!");
//        }
//        else {
//            *timerPtr = 0u;
//        }
//    }
    return ok;
}

/**
 * Required for EmbeddedServiceMethodBinderI.h
 */
ErrorManagement::ErrorType UDPReceiver::Execute(ExecutionInfo &info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        char8 *const dataBuffer = reinterpret_cast<char8*>(memory);
        err = socket.Read(dataBuffer, totalMemorySize, timeout);
    }

//    ErrorManagement::ErrorType err;
//    if (info.GetStage() == ExecutionInfo::TerminationStage) {
//        keepRunning = false;
//    }
//    else {
//        uint32 udpServerExpectReadSize = totalPacketSize;
//        uint32 udpServerReadSize = udpServerExpectReadSize;
//        if (keepRunning) {
//            dataRecieved = server.Read(reinterpret_cast<char8*>(dataBuffer), udpServerReadSize, timeout);
//            dataRecieved = (udpServerReadSize > 0u);
//            dataRecievedCorrectSize = (udpServerReadSize == udpServerExpectReadSize);
//            if (!dataRecieved) {
//                REPORT_ERROR(ErrorManagement::ParametersError, "No data recieved");
//                Sleep::Sec(20e-6);
//            }
//            else if (!dataRecievedCorrectSize) {
//                REPORT_ERROR(ErrorManagement::ParametersError, "Recieved data of inccorect size, ignoring it.");
//                Sleep::Sec(100e-6);
//            }
//            else {
//
//            }
//        }
//        if (synchronising) {
//            err = !synchSem.Post();
//        }
//    }

    return err;
}

CLASS_REGISTER(UDPReceiver, "1.0")

}
