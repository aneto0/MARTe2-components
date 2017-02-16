/**
 * @file UDPReceiver.cpp
 * @brief Source file for class UDPReceiver
 * @date Jan 31, 2017 TODO Verify the value and format of the date
 * @author Danny TODO Verify the name and format of the author
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
#include "BrokerI.h"
#include "ConfigurationDatabase.h"
#include "ErrorInformation.h"
#include "ErrorManagement.h"
#include "FastPollingMutexSem.h"
#include "GlobalObjectsDatabase.h"
#include "MemoryMapInputBroker.h"
#include "Threads.h"
#include "UDPReceiver.h"

namespace MARTe{

UDPReceiver::UDPReceiver(): DataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this){
    synchronising = true;
    keepRunning = true;
    dataRecievedCorrectSize = false;
    dataRecieved = false;
    udpServerPort = 44488u;
    nOfSignals = 0u;
    udpServerAddress = "127.0.0.1";
    timerPtr = NULL_PTR(uint64*);
    sequenceNumberPtr = NULL_PTR(uint64*);
    dataBuffer = NULL_PTR(void*);
    signalsMemoryOffset = NULL_PTR(uint32*);
    if (!synchSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create EventSem.");
    }
}

/*
 * Destructor
 */
/*lint -e{1551} the destructor must guarantee that the thread and servers are closed.*/
UDPReceiver::~UDPReceiver(){
    if (!synchSem.Post()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not post EventSem.");
    }
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    if (!server.Close()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not stop the UDP reciever server.");
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(dataBuffer);
    if (signalsMemoryOffset == NULL_PTR(uint32*)){
        REPORT_ERROR(ErrorManagement::FatalError, "Variable \"signalsMemoryOffset\" was not initialised!");
    }else{
        signalsMemoryOffset = NULL_PTR(uint32*);
    }
}

/**
 * Required for DataSourceI.h
 */
bool UDPReceiver::Synchronise(){
    ErrorManagement::ErrorType err;
    if (synchronising) {
        err = synchSem.ResetWait(TTInfiniteWait);
    }    
    return err.ErrorsCleared();
}

bool UDPReceiver::Initialise(StructuredDataI &data) {
    bool ok = DataSourceI::Initialise(data);
    float64 timeoutInput;
    keepRunning = true;
    bool found = data.Read("Port", udpServerPort);
    if ((!found) || (udpServerPort == 0u)){
        udpServerPort = 44488u;
        REPORT_ERROR(ErrorManagement::Information, "No valid Port defined! Default to 44488");
    }else{
        if (udpServerPort <= 1024u){
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Port is set to %d, requires admin access", udpServerPort)
        }else{
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Port is set to %d", udpServerPort)
        }
    }
    found = data.Read("Timeout", timeoutInput);
    if (!found){
        timeout = TTInfiniteWait;
        REPORT_ERROR(ErrorManagement::Information, "No timeout defined! Default to infinite");
    }else{
        if(timeoutInput < 1e-7){
            timeout = TTInfiniteWait;
            REPORT_ERROR(ErrorManagement::Information, "Timeout set to infinite");
        }else{
            timeout.SetTimeoutSec(timeoutInput);
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Timeout is set to %f seconds", timeoutInput)
        }
    }
    return ok;
}

bool UDPReceiver::AllocateMemory(){
    nOfSignals = GetNumberOfSignals();

    bool ok = (nOfSignals > 2u);
    if (signalsMemoryOffset == NULL_PTR(uint32*)){
        REPORT_ERROR(ErrorManagement::FatalError, "Variable \"signalsMemoryOffset\" was not initialised!");
        ok = false;
    }else{
        if (ok){
            uint32 LastSignalByteSize = 0u;
            ok = GetSignalByteSize(nOfSignals - 1u, LastSignalByteSize);
            dataBuffer= GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(signalsMemoryOffset[nOfSignals - 1u] + LastSignalByteSize);
            sequenceNumberPtr = &((static_cast<uint64 *>(dataBuffer))[0]);
            timerPtr = &((static_cast<uint64 *>(dataBuffer))[1]);
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError, "A minimum of three signals (counter, timerPtrand one other signal) must be specified!");
        }  
    }
    return ok;
}

uint32 UDPReceiver::GetNumberOfMemoryBuffers(){
    return 1u;
}

//lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool UDPReceiver::GetSignalMemoryBuffer(const uint32 signalIdx,
                                         const uint32 bufferIdx,
                                         void*& signalAddress) {
    bool ok = true;
    if (signalIdx <= (GetNumberOfSignals() -1u)){
        if (dataBuffer == NULL_PTR(void*)){
            ok = false;
            REPORT_ERROR(ErrorManagement::FatalError, "Variable \"dataBuffer\" was not initialised!");
        }else{
            char8* dataBufferChar = static_cast<char8*>(dataBuffer);
            if (signalsMemoryOffset == NULL_PTR(uint32*)){
                ok = false;
                REPORT_ERROR(ErrorManagement::FatalError, "Variable \"signalsMemoryOffset\" was not initialised!");
            }else{
                signalAddress = static_cast<void *>(&dataBufferChar[signalsMemoryOffset[signalIdx]]);
            }
        }  
    }
    else{
        ok = false;
    }
    return ok;
                                         }

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the data is independent of the broker name.*/
const char8* UDPReceiver::GetBrokerName(StructuredDataI& data,
                                         const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == InputSignals) {
            brokerName = "MemoryMapInputBroker";
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with OutputSignals");
    }
    return brokerName;
}

bool UDPReceiver::GetInputBrokers(ReferenceContainer& inputBrokers,
                                   const char8* const functionName,
                                   void* const gamMemPtr) {
    ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
    bool ok = broker.IsValid();
    if (ok) {
        ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ok) {
        ok = inputBrokers.Insert(broker);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: THe output broker is not needed*/
bool UDPReceiver::GetOutputBrokers(ReferenceContainer& outputBrokers,
                                    const char8* const functionName,
                                    void* const gamMemPtr) {
    return false;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the current and next state name are indepentent of the operation.*/
bool UDPReceiver::PrepareNextState(const char8* const currentStateName,
                                    const char8* const nextStateName) {
    bool ok = true;
    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
        keepRunning = true;
        ok = executor.Start();
    } 
    if (sequenceNumberPtr == NULL_PTR(uint64*)){
        ok = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Variable \"sequenceNumberPtr\" was not initialised!");
    }else{    
        *sequenceNumberPtr = 0u;
    }    
    if (ok){
        if (timerPtr == NULL_PTR(uint64*)){
            ok = false;
            REPORT_ERROR(ErrorManagement::FatalError, "Variable \"timerPtr\" was not initialised!");
        }else{
            *timerPtr= 0u ;
        }
    }    
    return ok;
}

bool UDPReceiver::SetConfiguredDatabase(StructuredDataI& data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok){
        ok = server.Open();
    }
    if (!ok){
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not open the server!");
    }
    if (ok) {
        InternetHost thisHost(udpServerPort, udpServerAddress.Buffer());
        ok = server.Listen(thisHost.GetPort());
    }
    if (!ok){
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not open the port!");
    }
    if (ok) {
        ok = (GetNumberOfSignals() > 2u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "At least three signals shall be configured");
    }
    if (ok) {
        uint16 i;
        uint32 signalByteSize;
        signalsMemoryOffset = new uint32[GetNumberOfSignals()];
        signalsMemoryOffset[0] = 0u;
        signalsMemoryOffset[1] = 8u;// To account for sequenceNumber to be stored as uint64
        signalsMemoryOffset[2] = 16u;// To account for timer to be stored as uint64
        for (i = 3u; i < GetNumberOfSignals(); i++){
            uint16 previousSignalIdx = i - 1u;
            ok = GetSignalByteSize(previousSignalIdx, signalByteSize);
            if (ok) {
                signalsMemoryOffset[i] = signalsMemoryOffset[i - 1u] + signalByteSize;
            }
        }
        uint16 lastSignalIdx = GetNumberOfSignals() - 1u;
        GetSignalByteSize(lastSignalIdx, signalByteSize);
        totalPacketSize = signalsMemoryOffset[lastSignalIdx] + signalByteSize;
        ok = (GetSignalType(0u).numberOfBits == 32u);
        if (!ok) {
            ok = (GetSignalType(0u).numberOfBits == 64u);
            }
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The first signal shall have 32 bits or 64 bits and %d were specified",
                                    uint16(GetSignalType(0u).numberOfBits ))
        }
    }
    if (ok) {
        ok = (GetSignalType(0u).type == SignedInteger);
        if (!ok) {
            ok = (GetSignalType(0u).type == UnsignedInteger);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The first signal shall SignedInteger or UnsignedInteger type");
        }
    }
    if (ok) {
        ok = (GetSignalType(1u).numberOfBits == 32u);
        if (!ok) {
            ok = (GetSignalType(1u).numberOfBits == 64u);
        }
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The second signal shall have 32 bits or 64 bits and %d were specified",
                                            uint16(GetSignalType(1u).numberOfBits))
        }
    }
    if (ok) {
        ok = (GetSignalType(1u).type == SignedInteger);
        if (!ok) {
            ok = (GetSignalType(1u).type == UnsignedInteger);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The second signal shall SignedInteger or UnsignedInteger type");
        }
    }
    return ok;
}


/**
 * Required for EmbeddedServiceMethodBinderI.h
 */
ErrorManagement::ErrorType UDPReceiver::Execute(const ExecutionInfo& info) {

    ErrorManagement::ErrorType err;
    if (info.GetStage() == ExecutionInfo::TerminationStage){
        keepRunning = false;
    }
    else{
        uint32 udpServerExpectReadSize = totalPacketSize;
        uint32 udpServerReadSize = udpServerExpectReadSize;
        uint8 i;
        if (keepRunning) {
            dataRecieved = server.Read(reinterpret_cast<char8*>(dataBuffer), udpServerReadSize,  timeout);
        }
        dataRecieved = (udpServerReadSize > 0u);
        dataRecievedCorrectSize = (udpServerReadSize == udpServerExpectReadSize);
        if (!dataRecieved){
            REPORT_ERROR(ErrorManagement::ParametersError, "No data recieved");
            Sleep::Sec(20e-6);
        }else if(!dataRecievedCorrectSize){
            REPORT_ERROR(ErrorManagement::ParametersError, "Recieved data of inccorect size, ignoring it.");
            Sleep::Sec(100e-6);
        }
    }

    if (synchronising) {
            err = !synchSem.Post();
    }
    return err;
}
CLASS_REGISTER(UDPReceiver, "1.0")

}
