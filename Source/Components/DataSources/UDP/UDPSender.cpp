/**
 * @file UDPSender.cpp
 * @brief Source file for class UDPSender
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
#include "MemoryMapSynchronisedOutputBroker.h"
#include "Shift.h"
#include "UDPSender.h"



/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{


UDPSender::UDPSender():DataSourceI(){
    timerPtr = NULL_PTR(uint64*);
    sequenceNumberPtr = NULL_PTR(uint64*);
    dataBuffer = NULL_PTR(void*);
    signalsMemoryOffset = NULL_PTR(uint32*);
    timerAtStateChange = 0u;
    udpServerAddress = "";
    nOfSignals = 0u;
    udpServerPort = 44488u;
    totalPacketSize = 0u;

}  


/*lint -e{1551} Justification: the destructor must guarantee that the client sending is closed.*/
UDPSender::~UDPSender(){
    if (!client.Close()){
        REPORT_ERROR(ErrorManagement::FatalError, "Could not close UDP sender.");
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(dataBuffer);

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
        signalsMemoryOffset = NULL_PTR(uint32*);
    }
}


bool UDPSender::Synchronise(){
    bool ok = true;
    const uint32 udpServerExpectReadSize = totalPacketSize;
    uint32 bytesSent = udpServerExpectReadSize;
    if (timerPtr == NULL_PTR(uint64*)){
        ok = false;
        REPORT_ERROR(ErrorManagement::FatalError, "Variable \"timerPtr\" was not initialised!");
    }else{
        uint64 counterDifference = HighResolutionTimer::Counter() - timerAtStateChange;
        float64 timeDifference = static_cast<float64>(counterDifference) * HighResolutionTimer::Period();
        uint64 timeDifferenceMicroSeconds = static_cast<uint64>(timeDifference * 1000000LLU);
        *timerPtr = timeDifferenceMicroSeconds;
    }
    if (ok){
        ok = client.Write(reinterpret_cast<char8*>(dataBuffer), bytesSent);
    }
    if (ok){
        if (sequenceNumberPtr == NULL_PTR(uint64*)){
            ok = false;
            REPORT_ERROR(ErrorManagement::FatalError, "Variable \"sequenceNumberPtr\" was not initialised!");
        }else{
            *sequenceNumberPtr +=1u;
        }
    }
    return ok;
}


bool UDPSender::Initialise(StructuredDataI &data) {
    bool ok = DataSourceI::Initialise(data);
       
    bool found = data.Read("TargetAddress", udpServerAddress);
    if (!found){
        udpServerAddress = "127.0.0.1";
        REPORT_ERROR(ErrorManagement::Information, "No TargetAddress defined! Default to 127.0.0.1");
    }else{
        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "TargetAddress set to %s", udpServerAddress.Buffer());
    }
    found = data.Read("Port", udpServerPort);
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
    return ok;
}

bool UDPSender::SetConfiguredDatabase(StructuredDataI& data) {
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    if (ok){
        ok = client.Open();
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not open the client!");
    }
    if (ok){
            ok = client.Connect(const_cast<char8*>(udpServerAddress.Buffer()), udpServerPort);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not connect the client to the server!");
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
        nOfSignals = GetNumberOfSignals();
        signalsMemoryOffset = new uint32[nOfSignals];
        signalsMemoryOffset[0] = 0u;
        for (i = 1u; i < GetNumberOfSignals(); i++){
            uint16 previousSignalIdx = i - 1u;
            ok = GetSignalByteSize(previousSignalIdx , signalByteSize);
            if (ok) {
                signalsMemoryOffset[i] = signalsMemoryOffset[i - 1u] + signalByteSize;
            }
        }
        uint32 LastSignalByteSize = 0u;
        ok = GetSignalByteSize(nOfSignals - 1u, LastSignalByteSize);
        if (ok){
            totalPacketSize = signalsMemoryOffset[nOfSignals - 1u] + LastSignalByteSize;
        }
        ok = (GetSignalType(0u).numberOfBits == 64u);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The first signal shall have 32 bits or 64 bits and %d were specified",
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
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The second signal shall have 32 bits or 64 bits and %d were specified",
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

bool UDPSender::AllocateMemory(){

    nOfSignals = GetNumberOfSignals();
    bool ok = (nOfSignals > 2u);
    if (signalsMemoryOffset == NULL_PTR(uint32*)){
        REPORT_ERROR(ErrorManagement::FatalError, "Variable \"signalsMemoryOffset\" was not initialised!");
        ok = false;
    }else{
        if (ok){
            dataBuffer= GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalPacketSize);
            sequenceNumberPtr = &((static_cast<uint64 *>(dataBuffer))[0]);
            timerPtr = &((static_cast<uint64 *>(dataBuffer))[1]);
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError, "A minimum of three signals (counter, timer and one other signal) must be specified!");
        }
        
    }
    
    return ok;
}

uint32 UDPSender::GetNumberOfMemoryBuffers(){
    return 1u;
}

//lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: The memory buffer is independent of the bufferIdx.*/
bool UDPSender::GetSignalMemoryBuffer(const uint32 signalIdx,
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

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the data is indepentent of the broker.*/
const char8* UDPSender::GetBrokerName(StructuredDataI& data,
                                         const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == OutputSignals) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "DataSource not compatible with InputSignals");
    }
    return brokerName;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Input broker not used*/
bool UDPSender::GetInputBrokers(ReferenceContainer& inputBrokers,
                                   const char8* const functionName,
                                   void* const gamMemPtr) {
    return false;
}

bool UDPSender::GetOutputBrokers(ReferenceContainer& outputBrokers,
                                    const char8* const functionName,
                                    void* const gamMemPtr) {
    ReferenceT<BrokerI> broker("MemoryMapSynchronisedOutputBroker");
    bool ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    if (ok) {
        ok = outputBrokers.Insert(broker);
    }
    return ok;
}

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: the current and next state name are indepentent of the operation.*/
bool UDPSender::PrepareNextState(const char8* const currentStateName,
                                    const char8* const nextStateName) {
    bool ok = true;
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
    if (ok){
        timerAtStateChange = HighResolutionTimer::Counter();
    }
    return ok;
}

CLASS_REGISTER(UDPSender, "1.0")

}
