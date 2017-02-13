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

#include "UDPReceiver.h"
#include "FastPollingMutexSem.h"
#include "AdvancedErrorManagement.h"
#include "BrokerI.h"
#include "ErrorManagement.h"
#include "ErrorInformation.h"
#include "GlobalObjectsDatabase.h"
#include "ConfigurationDatabase.h"

#include "MemoryMapInputBroker.h"

#include "Threads.h"
#include "UDPSocket.h"

namespace MARTe{

static uint16 udpServerPort = 44488u;
static StreamString udpServerAddress = "127.0.0.1";
static uint32 nOfSignals = 1u;
static uint32 totalPacketSize;
static uint32 *signalsByteSize;
UDPSocket server;

UDPReceiver::UDPReceiver(): DataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this){
    synchronising = true;
    keepRunning = true;
    dataRecievedCorrectSize = false;
    dataRecieved = false;
    UDPPacket.sequenceNumber = 0u;
    UDPPacket.timer = 0u ;
    UDPPacket.dataBuffer = NULL_PTR(void*);
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
    free(UDPPacket.dataBuffer);
    
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
    float64 timeout_input;
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
    found = data.Read("Timeout", timeout_input);
    if (!found){
        timeout = TTInfiniteWait;
        REPORT_ERROR(ErrorManagement::Information, "No timeout defined! Default to infinite");
    }else{
        if(timeout_input == 0.000000){
            timeout = TTInfiniteWait;
            REPORT_ERROR(ErrorManagement::Information, "Timeout set to infinite");
        }else{
            timeout.SetTimeoutSec(timeout_input);
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Timeout is set to %f seconds", timeout_input);
        }
    }
    return ok;
}

bool UDPReceiver::AllocateMemory(){
    nOfSignals = GetNumberOfSignals();

    bool ok = (nOfSignals > 2u);
    totalPacketSize = 0u;
    //REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "numb of reciving signals %d", nOfSignals);
    if (ok){
        uint32 n;
        uint32 signalByteSize = 0u;
        for (n = 2u; (n < nOfSignals) && (ok); n++){
            ok = GetSignalByteSize(n, signalByteSize);
            if (ok){
                totalPacketSize += signalByteSize;
            }
        }
        UDPPacket.dataBuffer= GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalPacketSize);
        maximumMemoryAccess = totalPacketSize - signalByteSize;
        uint32 i;
/*        for (i = 0u; i < (nOfSignals - 2u); i++ ){
            UDPPacket.dataBuffer[i] = 0u;
        }*/
        
    }else{
        REPORT_ERROR(ErrorManagement::ParametersError, "A minimum of three signals (counter, timer and another signal) must be specified!");
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
    uint32 memoryOffset = 0u;
    bool ok = true;
    if (signalIdx <= (GetNumberOfSignals() -1u)){
        if (signalIdx == 0u) {
            signalAddress = &UDPPacket.sequenceNumber;
        }
        else if (signalIdx == 1u) {
            signalAddress = &UDPPacket.timer;
        }
        else{
            uint32 i;
            for (i = 2u; i < signalIdx ; i++){
                uint32 signalByteSize = 0u;
                ok = GetSignalByteSize(i, signalByteSize);
                if (ok){
                    memoryOffset += signalByteSize;
                }
            }
            if (memoryOffset <= maximumMemoryAccess){
                signalAddress = static_cast<void *>(static_cast<char*>(UDPPacket.dataBuffer) + memoryOffset);
            }else{
                ok = false;
                REPORT_ERROR(ErrorManagement::FatalError, "Tried to access memory larger than defined");
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
    //REPORT_ERROR(ErrorManagement::Information, "prepare next state");
    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
        keepRunning = true;
        ok = executor.Start();
    } 
    UDPPacket.sequenceNumber = 0u;
    UDPPacket.timer = 0u;
    //UDPPacket.dataBuffer = NULL_PTR(AnyType*);
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
        signalsByteSize = new uint32[GetNumberOfSignals()];
        for (i = 0u; i < GetNumberOfSignals(); i++){
            signalsByteSize[i] = GetSignalType(i).numberOfBits;
        }
        ok = (signalsByteSize[0u] == 32u);
        if (!ok) {
            ok = (signalsByteSize[0u] == 64u);
            }
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The first signal shall have 32 bits or 64 bits and %d were specified",
                                    uint16(signalsByteSize[0u]))
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
        ok = (signalsByteSize[1u] == 32u);
        if (!ok) {
            ok = (signalsByteSize[1u] == 64u);
        }
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The second signal shall have 32 bits or 64 bits and %d were specified",
                                            uint16(signalsByteSize[1u]))
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
        uint32 udpServerExpectReadSize = totalPacketSize + static_cast<uint32>(signalsByteSize[0u]/8u) + static_cast<uint32>(signalsByteSize[1u]/8u);
        uint8 udpServerBufferRead[udpServerExpectReadSize];
        memset(static_cast<void*>(udpServerBufferRead), 0, sizeof(udpServerBufferRead));
        uint32 udpServerReadSize = udpServerExpectReadSize;
        uint8 i;
        if (keepRunning) {
            //REPORT_ERROR(ErrorManagement::ParametersError, "Waiting to recieve data");
            dataRecieved = server.Read(reinterpret_cast<char8 *>(udpServerBufferRead), udpServerReadSize,  timeout);
        }
        dataRecieved = (udpServerReadSize > 0u);
        dataRecievedCorrectSize = (udpServerReadSize == udpServerExpectReadSize);

        if (!dataRecieved){
            REPORT_ERROR(ErrorManagement::ParametersError, "No data recieved");
            Sleep::Sec(20e-6);
        }else if(!dataRecievedCorrectSize){
            //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "I recieved UDP data of size!!!! = %d, but i expected size %d", udpServerReadSize, udpServerExpectReadSize);
            REPORT_ERROR(ErrorManagement::ParametersError, "Recieved data of inccorect size, ignoring it.");
            Sleep::Sec(100e-6);
        }else{

            uint32 signalOffset = 0u;
            uint32 memoryOffset = 0u;
            for (i = 0u; (i < nOfSignals) && keepRunning ; i++){
                uint32 size = signalsByteSize[i];
                uint32 signalByteSize = size / 8u;
                void* AnytypeData;

                if (i == 0u){
                    AnytypeData = &UDPPacket.sequenceNumber;
                }else if (i == 1u){
                    AnytypeData = &UDPPacket.timer;                    
                }else{  
                    AnytypeData = UDPPacket.dataBuffer;
                }

                uint32 noOfBytesForSignal = size/8u;
                uint8 dataConv[noOfBytesForSignal];
                memset(static_cast<void*>(dataConv), 0, sizeof(dataConv));
                uint32 counter;
                for (counter = 0u; counter < noOfBytesForSignal; counter++){

                    dataConv[counter] = udpServerBufferRead[signalOffset + counter];
                }
                if ((i == 0u) || (i == 1u)){
                    //REPORT_ERROR(ErrorManagement::ParametersError, "Doing memcpy1");
                    memcpy(AnytypeData,static_cast<void*>(dataConv),signalByteSize);
                    //REPORT_ERROR(ErrorManagement::ParametersError, "Done memcpy1");
                    //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "I recieved UDP data!!!! = %d", AnytypeData);
                }else{
                    if (memoryOffset <= maximumMemoryAccess){
                        void *p = static_cast<char*>(AnytypeData) + memoryOffset;
                        //REPORT_ERROR(ErrorManagement::ParametersError, "Doing memcpy2");
                        memcpy(p,static_cast<void*>(dataConv),signalByteSize);
                        //REPORT_ERROR(ErrorManagement::ParametersError, "finished memcpy2");
                        uint32 test;
                        memcpy(&test,p,signalByteSize);
                        //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "I recieved UDP data!!!! = %d", test);
                        memoryOffset += signalByteSize;
                    }else{
                        REPORT_ERROR(ErrorManagement::FatalError, "Tried to access memory larger than defined");
                    }
                }
                signalOffset += noOfBytesForSignal;
            }
        }
    }

    if (synchronising) {
            err = !synchSem.Post();
    }
    return err;
}
CLASS_REGISTER(UDPReceiver, "1.0")

}
