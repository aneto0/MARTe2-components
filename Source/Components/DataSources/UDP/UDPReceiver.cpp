/**
 * @file UDPReceiver.cpp
 * @brief Source file for class UDPReceiver
 * @date Jan 31, 2017 
 * @author Danny Sortino
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
#include "MemoryMapSynchronisedInputBroker.h"
#include "Threads.h"
#include "UDPReceiver.h"

namespace MARTe{

UDPReceiver::UDPReceiver(): DataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this){
    keepRunning = true;
    dataRecievedCorrectSize = false;
    dataRecieved = false;
    udpServerPort = 44488u;
    nOfSignals = 0u;
    udpServerAddress = "127.0.0.1";
    cpuMask = 0u;
    timerPtr = NULL_PTR(uint64*);
    sequenceNumberPtr = NULL_PTR(uint64*);
    dataBuffer = NULL_PTR(void*);
    signalsMemoryOffset = NULL_PTR(uint32*);
    synchronising = false;
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
    
    if (dataBuffer == NULL_PTR(void*)){
        REPORT_ERROR(ErrorManagement::FatalError, "Variable \"dataBuffer\" was not initialised!");
    }else{
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(dataBuffer);
        dataBuffer = NULL_PTR(void*);
    }
    
    if (sequenceNumberPtr == NULL_PTR(uint64*)){
        REPORT_ERROR(ErrorManagement::FatalError, "Variable \"sequenceNumberPtr\" was not initialised!");
    }else{
        sequenceNumberPtr = NULL_PTR(uint64*);
    }
    
    if (timerPtr == NULL_PTR(uint64*)){
        REPORT_ERROR(ErrorManagement::FatalError, "Variable \"timerPtr\" was not initialised!");
    }else{
        timerPtr = NULL_PTR(uint64*);
    }

    if (signalsMemoryOffset == NULL_PTR(uint32*)){
        REPORT_ERROR(ErrorManagement::FatalError, "Variable \"signalsMemoryOffset\" was not initialised!");
    }else{
        delete signalsMemoryOffset;
        signalsMemoryOffset = NULL_PTR(uint32*);
    }
}

/**
 * Required for DataSourceI.h
 */
bool UDPReceiver::Synchronise(){
    ErrorManagement::ErrorType err;
    if (synchronising){
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
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Port is set to %d, requires admin access", udpServerPort);
        }else{
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Port is set to %d", udpServerPort);
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
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Timeout is set to %f seconds", timeoutInput);
        }
    }
    if (ok){
        if (!data.Read("CPUs", cpuMask)){
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "No CPU's defined for %s", GetName());
        }
    }
    return ok;
}

bool UDPReceiver::AllocateMemory(){
   

    bool ok = (nOfSignals > 2u);
    if (signalsMemoryOffset == NULL_PTR(uint32*)){
        REPORT_ERROR(ErrorManagement::FatalError, "Variable \"signalsMemoryOffset\" was not initialised!");
        ok = false;
    }else{
        if (ok){
            uint32 LastSignalByteSize = 0u;
            ok = GetSignalByteSize(nOfSignals - 1u, LastSignalByteSize);
            dataBuffer = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(signalsMemoryOffset[nOfSignals - 1u] + LastSignalByteSize);
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
    if (signalIdx <= (nOfSignals -1u)){
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
        float32 frequency = 0.F;
        if (!data.Read("Frequency", frequency)){
            frequency = -1.F;
        }
        if (frequency > 0.F){
            brokerName = "MemoryMapSynchronisedInputBroker";
            synchronising = true;
        }else{
            brokerName = "MemoryMapInputBroker";
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with OutputSignals");
    }
    return brokerName;
}

bool UDPReceiver::GetInputBrokers(ReferenceContainer& inputBrokers,
                                   const char8* const functionName,
                                   void* const gamMemPtr) {
    uint32 functionIdx = 0u;
    uint32 numbOfSignals = 0u;
    bool synchGAM = false;
    bool ok = GetFunctionIndex(functionIdx, functionName);
    if (ok){
        ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, numbOfSignals);
    }

    uint32 i;
    float32 frequency = 0.F;
    for (i = 0u; (i < numbOfSignals) && (ok) && (!synchGAM); i++){
        ok = GetFunctionSignalReadFrequency(InputSignals, functionIdx, i, frequency);
        synchGAM = (frequency > 0.F);
    }
    if ((synchronising) && (synchGAM)){
        ReferenceT<MemoryMapSynchronisedInputBroker> brokerSync("MemoryMapSynchronisedInputBroker");
        if (ok) {
            ok = brokerSync.IsValid();
        }
        if (ok){
            ok = brokerSync->Init(InputSignals, *this, functionName, gamMemPtr);
        }
        if (ok){
            ok = inputBrokers.Insert(brokerSync);
        }
        uint32 nOfFunctionSignals = 0u;
        if (ok){
            ok = GetFunctionNumberOfSignals(InputSignals, functionIdx, nOfFunctionSignals);
        }
        if (ok){
            if (nOfFunctionSignals > 1u){
                ReferenceT<MemoryMapInputBroker> brokerNotSync("MemoryMapInputBroker");
                ok = brokerNotSync.IsValid();
                if (ok){
                    ok = brokerNotSync->Init(InputSignals, *this, functionName, gamMemPtr);
                }
                if (ok){
                    ok = inputBrokers.Insert(brokerNotSync);
                }
            }
        }
    }
    else{
        ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
        ok = broker.IsValid();
        if (ok) {
            ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
        }
        if (ok) {
            ok = inputBrokers.Insert(broker);
        }
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
        if (cpuMask != 0u){
            executor.SetCPUMask(cpuMask);
        }
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
        nOfSignals = GetNumberOfSignals();
        uint16 i;
        uint32 signalByteSize;
        signalsMemoryOffset = new uint32[nOfSignals];
        signalsMemoryOffset[0] = 0u;
        signalsMemoryOffset[1] = 8u;// To account for sequenceNumber to be stored as uint64
        signalsMemoryOffset[2] = 16u;// To account for timer to be stored as uint64
        for (i = 3u; i < nOfSignals; i++){
            uint16 previousSignalIdx = i - 1u;
            ok = GetSignalByteSize(previousSignalIdx, signalByteSize);
            if (ok) {
                signalsMemoryOffset[i] = signalsMemoryOffset[i - 1u] + signalByteSize;
            }
        }
        uint32 lastSignalIdx = nOfSignals - 1u;
        ok = GetSignalByteSize(lastSignalIdx, signalByteSize);
        if (ok){
            totalPacketSize = signalsMemoryOffset[lastSignalIdx] + signalByteSize;
        }
        ok = (GetSignalType(0u).numberOfBits == 64u);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The first signal shall have 64 bits and %d were specified",
                                    uint16(GetSignalType(0u).numberOfBits ));
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
        ok = (GetSignalType(1u).numberOfBits == 64u);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The second signal shall have 64 bits and %d were specified",
                                            uint16(GetSignalType(1u).numberOfBits));
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
        if (keepRunning) {
            dataRecieved = server.Read(reinterpret_cast<char8*>(dataBuffer), udpServerReadSize,  timeout);
            dataRecieved = (udpServerReadSize > 0u);
            dataRecievedCorrectSize = (udpServerReadSize == udpServerExpectReadSize);
            if (!dataRecieved){
                REPORT_ERROR(ErrorManagement::ParametersError, "No data recieved");
                Sleep::Sec(20e-6);
            }else if(!dataRecievedCorrectSize){
                REPORT_ERROR(ErrorManagement::ParametersError, "Recieved data of inccorect size, ignoring it.");
                Sleep::Sec(100e-6);
            }else{
                
            }
        }
        if (synchronising){
            err = !synchSem.Post();
        }
    }

    return err;
}
CLASS_REGISTER(UDPReceiver, "1.0")

}