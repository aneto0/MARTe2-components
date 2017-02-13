/**
 * @file UDPSender.cpp
 * @brief Source file for class UDPSender
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
 * the class UDPSender (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */


/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UDPSender.h"
#include "AdvancedErrorManagement.h"
#include "BrokerI.h"
#include "Shift.h"
#include "UDPSocket.h"
#include "MemoryMapSynchronisedOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

static uint32 nOfSignals = 0u;
static uint16 udpServerPort;
static UDPSocket client;
static uint32 *signalsByteSize;

UDPSender::UDPSender():DataSourceI(){
    UDPPacket.sequenceNumber = 0u;
    UDPPacket.timer = 0u ;
    UDPPacket.dataBuffer = NULL_PTR(void*);
    timerAtStateChange = 0u;
    udpServerAddress = "";
    maximumMemoryAccess = 0u;
}

/*lint -e{1551} the destructor must guarantee that the client sending is closed.*/
UDPSender::~UDPSender(){
     if (!client.Close()){
         REPORT_ERROR(ErrorManagement::FatalError, "Could not close UDP sender.");
     }
     free(UDPPacket.dataBuffer);
}

bool UDPSender::Synchronise(){
    bool OK = true;
    const MARTe::uint32 udpServerExpectReadSize = nOfSignals * 8u;
    uint32 i;
    uint8 udpServerWriteBuffer[udpServerExpectReadSize];
    uint32 signalOffset = 0u;
    uint32 memoryOffset = 0u;
    for (i = 0u; i < udpServerExpectReadSize; i++){
        udpServerWriteBuffer[i] = 0u;
    }
    for (i = 0u; i < nOfSignals; i++){
        void* dataConv;
        if (i == 0u){
            dataConv = &UDPPacket.sequenceNumber;
        } else if (i == 1u){
            UDPPacket.timer=HighResolutionTimer::Counter() - timerAtStateChange;
            dataConv = &UDPPacket.timer;
        }else{
            dataConv = UDPPacket.dataBuffer;
        }

        if (OK){
            uint32 signalByteSize = signalsByteSize[i] / 8u;
            if (signalByteSize > 0u){
                uint64 k;
                uint8 AnyTypetoUint8 [signalByteSize];
                for (k = 0u; k < signalByteSize; k++ ){
                    AnyTypetoUint8[k] = 0u;
                }
                if ((i == 0u) || (i == 1u)){
                    memcpy(static_cast<void*>(AnyTypetoUint8),dataConv,signalByteSize);
                    memcpy(&k,static_cast<char*>(dataConv),signalByteSize);
                }else{
                    if (memoryOffset <= maximumMemoryAccess){
                        void* p = static_cast<char*>(dataConv) + memoryOffset;
                        memcpy(static_cast<void*>(AnyTypetoUint8),p,signalByteSize);
                        memcpy(&k,static_cast<char*>(p),signalByteSize);
                        memoryOffset += signalByteSize;
                    }else{
                        REPORT_ERROR(ErrorManagement::FatalError, "Tried to access memory larger than defined");
                    }
                }
                uint32 j;
                for(j = 0u; j < signalByteSize; j++){
                    udpServerWriteBuffer[static_cast<uint32>(j + signalOffset)] = AnyTypetoUint8[j];
                }
                signalOffset += signalByteSize;
            }
        }

    }
    if (OK){
        uint32 bytesSent = udpServerExpectReadSize;
        OK = client.Write(reinterpret_cast<char8*>(udpServerWriteBuffer), bytesSent);
        UDPPacket.sequenceNumber++;
    }
    return OK;
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
        signalsByteSize = new uint32[GetNumberOfSignals()];
        for (i = 0u; i < GetNumberOfSignals(); i++){
            signalsByteSize[i] = GetSignalType(i).numberOfBits;
        }
        ok = (signalsByteSize[0u] == 32u);
        if (!ok) {
            ok = (signalsByteSize[0u]  == 64u);
            }
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The first signal shall have 32 bits or 64 bits and %d were specified",
                                    uint16(signalsByteSize[0u] ))
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

bool UDPSender::AllocateMemory(){

    nOfSignals = GetNumberOfSignals();
    bool ok = (nOfSignals > 2u);
    uint32 totalPacketSize = 0u;

    if (ok){
        uint32 n;
        uint32 signalByteSize = 0u;
        for (n = 2u; (n < nOfSignals) && (ok); n++){
            ok = GetSignalByteSize(n, signalByteSize);
            if (ok){
                totalPacketSize += signalByteSize;
            }
        }
        if (ok){
            UDPPacket.dataBuffer= GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(totalPacketSize);
            maximumMemoryAccess = totalPacketSize - signalByteSize;
            uint32 i;
        }
    }else{
        REPORT_ERROR(ErrorManagement::ParametersError, "A minimum of three signals (counter, timer and one other signal) must be specified!");
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
    uint32 memoryOffset = 0u;
    bool ok = true;
    if (signalIdx <= (GetNumberOfSignals() -1u)){
        if (signalIdx == 0u) {
            signalAddress = &UDPPacket.sequenceNumber;
        }
        else if (signalIdx == 1u) {
            signalAddress = &UDPPacket.timer;
        }
        else {
            uint32 i;
            for (i = 2u; i < signalIdx ; i++){      
                uint32 signalByteSize = 0u;
                ok = GetSignalByteSize(i, signalByteSize);
                if (ok){
                    memoryOffset += signalByteSize;
                }
            }
            if (memoryOffset <= maximumMemoryAccess){
                signalAddress = static_cast<void*>(static_cast<char*>(UDPPacket.dataBuffer) + memoryOffset);
            } else{
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
    UDPPacket.sequenceNumber = 0u;
    timerAtStateChange = HighResolutionTimer::Counter();
    UDPPacket.timer = 0u ;
    return ok;
}

CLASS_REGISTER(UDPSender, "1.0")

}

