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
    address = "";
    port = 44488u;
    client = NULL_PTR(UDPSocket*);
    cpuMask = 0xfu;
    stackSize = 0u;

}

/*lint -e{1551} Justification: the destructor must guarantee that the client sending is closed.*/
UDPSender::~UDPSender() {
    if (client != NULL_PTR(UDPSocket*)) {
        if (!client->Close()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not close UDP sender.");
        }
        else {
            delete client;
        }
    }
}

bool UDPSender::Initialise(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::Initialise(data);
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
        ok = data.Read("Address", address);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No Address defined!");
        }
    }
    if (ok) {
        ok = data.Read("Port", port);
        if (!ok) {
            port = 44488u;
            REPORT_ERROR(ErrorManagement::Information, "No valid Port defined! Using Default 44488");
            ok = true;
        }
        if (port <= 1024u) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Port is set to %d, possible issues with values < 1024", port);
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
    const char8 *const dataBuffer = reinterpret_cast<char8*>(memory);
    bool ok = false;
    if (client != NULL_PTR(UDPSocket*)) {
        ok = client->Write(dataBuffer, totalMemorySize);
    }
    return ok;
}

bool UDPSender::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        /*lint -e{9007} Justification: no side effects on GetSignalType.*/
        ok = ((GetSignalType(1u) == SignedInteger64Bit) || (GetSignalType(1u) == UnsignedInteger64Bit) || (GetSignalType(1u) == UnsignedInteger32Bit));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Counter signal type not supported. Use uint64 or int64 or uint32");
        }
        if (ok) {
            /*lint -e{9007} Justification: no side effects on GetSignalType.*/
            ok = ((GetSignalType(2u) == SignedInteger64Bit) || (GetSignalType(2u) == UnsignedInteger64Bit) || (GetSignalType(2u) == UnsignedInteger32Bit));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Time signal type not supported. Use uint64 or int64 or uint32");
            }
        }
    }
    if (ok) {
        client = new UDPSocket();
        /*lint -e{613} Justification: the client cannot be a Null_PTR since it is allocated just before.*/
        ok = client->Open();
    }
    if (ok) {
        /*lint -e{613} Justification: the client cannot be a Null_PTR since it is allocated just before.*/
        ok = client->Connect(address.Buffer(), port);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The brokerName only depends on the direction */
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

const ProcessorType& UDPSender::GetCPUMask() const {
    return cpuMask;
}

uint32 UDPSender::GetNumberOfPostTriggers() const {
    return numberOfPostTriggers;
}

uint32 UDPSender::GetNumberOfPreTriggers() const {
    return numberOfPreTriggers;
}

uint32 UDPSender::GetStackSize() const {
    return stackSize;
}

uint16 UDPSender::GetPort() const {
    return port;
}

StreamString UDPSender::GetAddress() const {
    return address;
}

CLASS_REGISTER(UDPSender, "1.0")

}
