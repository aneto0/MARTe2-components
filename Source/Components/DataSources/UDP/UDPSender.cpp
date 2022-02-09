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
#include "MemoryMapSynchronisedOutputBroker.h"
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
    client = NULL_PTR(BasicUDPSocket*);
    cpuMask = 0xffffffffu;
    stackSize = 0u;
    executionMode = UDPSenderExecutionModeIndependent;
}

/*lint -e{1551} Justification: the destructor must guarantee that the client sending is closed.*/
UDPSender::~UDPSender() {
    if (client != NULL_PTR(BasicUDPSocket*)) {
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
        StreamString tempExecModeStr;
        ok = data.Read("ExecutionMode", tempExecModeStr);
        if(!ok) {
            REPORT_ERROR(ErrorManagement::Information, "Execution mode was not specified, defaulting to IndependentThread mode");
            executionMode = UDPSenderExecutionModeIndependent;
            ok = true;
        }
        else {
            if(tempExecModeStr == "IndependentThread") {
                executionMode = UDPSenderExecutionModeIndependent;
                REPORT_ERROR(ErrorManagement::Information, "Running in IndependentThread mode");
            }
            else if(tempExecModeStr == "RealTimeThread") {
                executionMode = UDPSenderExecutionModeRealTime;
                REPORT_ERROR(ErrorManagement::Information, "Running in RealTimeThread mode");
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Specified execution mode is not allowed");
                REPORT_ERROR(ErrorManagement::Information, "Allowed [IndependentThread, RealTimeThread], Specified %s", tempExecModeStr.Buffer());
                ok = false;
            }
        }
    }
    if (ok) {
        ok = data.Read("NumberOfPreTriggers", numberOfPreTriggers);
        if (!ok) {
            ok = (executionMode == UDPSenderExecutionModeRealTime);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfPreTriggers shall be specified");
            }
        }
        else {
            if(executionMode == UDPSenderExecutionModeRealTime) {
                REPORT_ERROR(ErrorManagement::Warning, "NumberOfPreTriggers was specified but it will be ignored in RealTimeThread mode");
            }
        }
    }
    if (ok) {
        ok = data.Read("NumberOfPostTriggers", numberOfPostTriggers);
        if (!ok) {
            ok = (executionMode == UDPSenderExecutionModeRealTime);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfPostTriggers shall be specified");
            }
        }
        else {
            if(executionMode == UDPSenderExecutionModeRealTime) {
                REPORT_ERROR(ErrorManagement::Warning, "NumberOfPostTriggers was specified but it will be ignored in RealTimeThread mode");
            }
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
            if(executionMode == UDPSenderExecutionModeIndependent) {
                cpuMaskIn = 0xFFFFFFFFu;
                REPORT_ERROR(ErrorManagement::Information, "CPUMask was not specified, defaulting to 0xFFFFFFFFu");
            }
            ok = true;
        }
        else {
            if(executionMode == UDPSenderExecutionModeRealTime) {
                REPORT_ERROR(ErrorManagement::Warning, "CPUMask parameter not expected when in RealTimeThread mode, ignoring");
            }
        }
        cpuMask = cpuMaskIn;
    }
    if (ok) {
        ok = data.Read("StackSize", stackSize);
        if (!ok) {
            if(executionMode == UDPSenderExecutionModeIndependent) {
                stackSize = THREADS_DEFAULT_STACKSIZE;
                REPORT_ERROR(ErrorManagement::Warning, "StackSize was not specified, defaulting to MARTe2 value (%d)", THREADS_DEFAULT_STACKSIZE);
            }
            ok = true;
        }
        else {
            if(executionMode == UDPSenderExecutionModeRealTime) {
                REPORT_ERROR(ErrorManagement::Warning, "StackSize parameter not expected when in RealTimeThread mode, ignoring");
            }
        }
    }
    if (ok) {
        if (executionMode == UDPSenderExecutionModeIndependent) {
            ok = (stackSize > 0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "StackSize shall be > 0u when running in IndependentThread mode");
            }
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
        if (executionMode == UDPSenderExecutionModeIndependent) {
            tempName = signalsDatabase.GetChildName(0u);
            if (tempName != "Trigger") {
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot detect signal \"Trigger\".");
                ok = false;
            }
        }
    }
    return ok;
}

bool UDPSender::Synchronise() {
    const char8 *const dataBuffer = reinterpret_cast<char8*>(memory);
    bool ok = false;
    if (client != NULL_PTR(BasicUDPSocket*)) {
        ok = client->Write(dataBuffer, totalMemorySize);
    }
    return ok;
}

bool UDPSender::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        client = new BasicUDPSocket();
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
        if(executionMode == UDPSenderExecutionModeIndependent) {
            brokerName = "MemoryMapAsyncTriggerOutputBroker";
        }
        else if(executionMode == UDPSenderExecutionModeRealTime) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
        }
        //Cannot be reached
        else{
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "UDPSender does not support InputSignals");
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
    bool ok = true;

    if(executionMode == UDPSenderExecutionModeIndependent) {
        ReferenceT<MemoryMapAsyncTriggerOutputBroker> brokerAsyncTrigger = 
            ReferenceT<MemoryMapAsyncTriggerOutputBroker>("MemoryMapAsyncTriggerOutputBroker");
        ok = brokerAsyncTrigger->InitWithTriggerParameters(OutputSignals, *this, functionName, gamMemPtr, numberOfBuffers, numberOfPreTriggers,
                                                                numberOfPostTriggers, cpuMask, stackSize);
        if (ok) {
            ok = outputBrokers.Insert(brokerAsyncTrigger);
        }
    }
    else if (executionMode == UDPSenderExecutionModeRealTime) {
        ReferenceT<MemoryMapSynchronisedOutputBroker> memMapSynchOutBroker("MemoryMapSynchronisedOutputBroker");
        ok = memMapSynchOutBroker.IsValid();
        if(!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failure while instantiating MemoryMapSynchronisedOutputBroker");
        }

        if (ok) {
            ok = memMapSynchOutBroker->Init(OutputSignals, *this, functionName, gamMemPtr, true);
        }
        if(!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failure while initialising MemoryMapSynchronisedOutputBroker");
        }

        ok = outputBrokers.Insert(memMapSynchOutBroker);
        if(!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failure while inserting MemoryMapSynchronisedOutputBroker");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Execution mode specifies an unallowed mode");
        ok = false;
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
