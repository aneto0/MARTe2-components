/**
 * @file UDPSender.cpp
 * @brief Source file for class UDPSender
 * @date 10/05/2021
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
 * the class UDPSender (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "BrokerI.h"
#include "MemoryMapAsyncTriggerOutputBroker.h"
#include "Shift.h"
#include "UDPSender.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

UDPSender::UDPSender() :
        MemoryDataSourceI() {
    numberOfPreTriggers = 0u;
    numberOfPostTriggers = 0u;
    udpServerAddress = "";
    udpServerPort = 44488u;
    client = NULL_PTR(UDPSocket*);

}

/*lint -e{1551} Justification: the destructor must guarantee that the client sending is closed.*/
UDPSender::~UDPSender() {
    if (!client->Close()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not close UDP sender.");
    } else {
        delete client;
    }
}

bool UDPSender::Initialise(StructuredDataI &data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        ok = data.Read("NumberOfPreTriggers", numberOfPreTriggers);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfPreTriggers shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("NumberOfPostTriggers", numberOfPostTriggers);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfPostTriggers shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("Address", udpServerAddress);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No Address defined!");
        }
    }
    if (ok) {
        ok = data.Read("Port", udpServerPort);
        if (!ok) {
            udpServerPort = 44488u;
            REPORT_ERROR(ErrorManagement::Information, "No valid Port defined! Using Default 44488");
            ok = true;
        }
        if (udpServerPort <= 1024u) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Port is set to %d, possible issues with values < 1024", udpServerPort);
        }
    }
    if (ok) {
        uint32 cpuMaskIn;
        ok = data.Read("CPUMask", cpuMaskIn);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "CPUMask shall be specified");
        }
        else {
            cpuMask = cpuMaskIn;
        }
    }
    if (ok) {
        ok = data.Read("StackSize", stackSize);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "StackSize shall be specified");
        }
    }
    if (ok) {
        ok = (stackSize > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "StackSize shall be > 0u");
        }
    }
    if (ok) {
        ok = data.MoveRelative("Signals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not move to the Signals section");
        }
    }
    //Do not allow to add signals in run-time
    if (ok) {
        ok = signalsDatabase.MoveRelative("Signals");
    }
    if (ok) {
        ok = signalsDatabase.Write("Locked", 1u);
    }
    StreamString tempName = "";
    if (ok) {
        tempName = signalsDatabase.GetChildName(0u);
        if (tempName != "Trigger") {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot detect signal \"Trigger\".");
            ok = false;
        }
        tempName = "";
    }
    if (ok) {
        tempName = signalsDatabase.GetChildName(1u);
        if (tempName != "Counter") {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot detect signal \"Counter\".");
            ok = false;
        }
        tempName = "";
    }
    if (ok) {
        tempName = signalsDatabase.GetChildName(2u);
        if (tempName != "Time") {
            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot detect signal \"Time\".");
            ok = false;
        }
        tempName = "";
    }

    return ok;
}

bool UDPSender::Synchronise() {
    const char8* const dataBuffer = reinterpret_cast<char8*>(memory);
    bool ok = client->Write(dataBuffer, totalMemorySize);
    return ok;
}

bool UDPSender::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = (GetSignalType(1u) == SignedInteger64Bit || GetSignalType(1u) == UnsignedInteger64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Counter signal type not supported. Use uint64 or int64");
        }
        if (ok) {
            ok = (GetSignalType(2u) == SignedInteger64Bit || GetSignalType(1u) == UnsignedInteger64Bit);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Time signal type not supported. Use uint64 or int64");
            }
        }
    }
    if(ok){
        client = new UDPSocket();
    }
    if (ok) {
        ok = client->Open();
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not open the client!");
    }
    if (ok) {
        ok = client->Connect(udpServerAddress.Buffer(), udpServerPort);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not connect the client to the server!");
    }
    return ok;
}

/*bool UDPSender::AllocateMemory() {

 nOfSignals = GetNumberOfSignals();
 bool ok = (nOfSignals > 3u);
 if (signalsMemoryOffset == NULL_PTR(uint32*)) {
 REPORT_ERROR(ErrorManagement::FatalError, "Variable \"signalsMemoryOffset\" was not initialised!");
 ok = false;
 }
 else {
 if (ok) {
 dataBuffer = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalPacketSize);
 sequenceNumberPtr = &((static_cast<uint64*>(dataBuffer))[0]);
 timerPtr = &((static_cast<uint64*>(dataBuffer))[1]);
 }
 else {
 REPORT_ERROR(ErrorManagement::ParametersError, "A minimum of three signals (counter, timer and one other signal) must be specified!");
 }

 }

 return ok;
 }*/

/*uint32 UDPSender::GetNumberOfMemoryBuffers() {
 return 1u;
 }*/

//lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
/*bool UDPSender::GetSignalMemoryBuffer(const uint32 signalIdx,
 const uint32 bufferIdx,
 void *&signalAddress) {
 bool ok = true;
 if (signalIdx <= (GetNumberOfSignals() - 1u)) {
 if (dataBuffer == NULL_PTR(void*)) {
 ok = false;
 REPORT_ERROR(ErrorManagement::FatalError, "Variable \"dataBuffer\" was not initialised!");
 }
 else {
 char8 *dataBufferChar = static_cast<char8*>(dataBuffer);
 if (signalsMemoryOffset == NULL_PTR(uint32*)) {
 ok = false;
 REPORT_ERROR(ErrorManagement::FatalError, "Variable \"signalsMemoryOffset\" was not initialised!");
 }
 else {
 signalAddress = static_cast<void*>(&dataBufferChar[signalsMemoryOffset[signalIdx]]);
 }
 }
 }
 else {
 ok = false;
 }
 return ok;
 }*/

const char8* UDPSender::GetBrokerName(StructuredDataI &data,
                                      const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8*);
    if (direction == OutputSignals) {
        brokerName = "MemoryMapAsyncTriggerOutputBroker";
    }
    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Input broker not used*/
bool UDPSender::GetInputBrokers(ReferenceContainer &inputBrokers,
                                const char8 *const functionName,
                                void *const gamMemPtr) {
    return false;
}

bool UDPSender::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                 const char8 *const functionName,
                                 void *const gamMemPtr) {
    brokerAsyncTrigger = ReferenceT<MemoryMapAsyncTriggerOutputBroker>("MemoryMapAsyncTriggerOutputBroker");
    bool ok = brokerAsyncTrigger->InitWithTriggerParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers, numberOfPreTriggers,
                                                            numberOfPostTriggers, cpuMask, stackSize);
    if (ok) {
        ok = outputBrokers.Insert(brokerAsyncTrigger);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the current and next state name are indepentent of the operation.*/
bool UDPSender::PrepareNextState(const char8 *const currentStateName,
                                 const char8 *const nextStateName) {
    return true;
}

CLASS_REGISTER(UDPSender, "1.0")

}
