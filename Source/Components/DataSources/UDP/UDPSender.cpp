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
static StreamString udpServerAddress;
static UDPSocket client;
static uint64 timerAtStateChange = 0u;
static uint32 memoryOffset = 0u;

UDPSender::UDPSender():DataSourceI(){
    UDPPacket.sequenceNumber = 0u;
    UDPPacket.timer = 0u ;
    UDPPacket.dataBuffer = NULL_PTR(AnyType*);
}

/*lint -e{1551} the destructor must guarantee that the client sending is closed.*/
UDPSender::~UDPSender(){
     if (!client.Close()){
         REPORT_ERROR(ErrorManagement::FatalError, "Could not close UDP sender.");
     }
}

bool UDPSender::Synchronise(){
    /*uint64 k = 0u;
    uint32 signalByteSize;
    void* p = static_cast<char*>((UDPPacket.dataBuffer[0]).GetDataPointer());
    GetSignalByteSize(2, signalByteSize);
    memcpy(&k,p,signalByteSize);
    REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "MEMORY data at %d with size %d with data before %d",p, signalByteSize, k);*/
    bool OK = true;
    const MARTe::uint32 udpServerExpectReadSize = nOfSignals * 8u;
    uint32 i;
    uint8 udpServerWriteBuffer[udpServerExpectReadSize];
    uint32 signalOffset = 0u;
    memoryOffset = 0u;
    for (i = 0u; i < udpServerExpectReadSize; i++){
        udpServerWriteBuffer[i] = 0u;
    }
    for (i = 0u; i < nOfSignals; i++){
        AnyType dataConv = new AnyType;
        if (i == 0u){
            dataConv = UDPPacket.sequenceNumber;
        } else if (i == 1u){
            UDPPacket.timer=HighResolutionTimer::Counter() - timerAtStateChange;
            dataConv = UDPPacket.timer;
        }else{
            dataConv = UDPPacket.dataBuffer[i-2u];
        }

        if (OK){

            uint32 signalByteSize = GetSignalType(i).numberOfBits / 8u;

            uint64 k;
            uint8 AnyTypetoUint8 [signalByteSize];
            for (k = 0u; k < signalByteSize; k++ ){
                AnyTypetoUint8[k] = 0u;
            }
            if ((i == 0u) || (i == 1u)){
                memcpy(static_cast<void*>(AnyTypetoUint8),dataConv.GetDataPointer(),signalByteSize);
                memcpy(&k,static_cast<char*>(dataConv.GetDataPointer()),signalByteSize);
                //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, " I am sending data: %d",k);
            }else{
                void* p = static_cast<char*>(dataConv.GetDataPointer()) + memoryOffset;
                memcpy(static_cast<void*>(AnyTypetoUint8),p,signalByteSize);
                memcpy(&k,p,signalByteSize);
                memoryOffset += signalByteSize;
                //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, " I am sending data: %d, located at memory loc: %d",k, p);
            }
            uint32 j;
            for(j = 0u; j < signalByteSize; j++){
                udpServerWriteBuffer[static_cast<uint32>(j + signalOffset)] = AnyTypetoUint8[j];
            }
            signalOffset += signalByteSize;
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
    }
    found = data.Read("Port", udpServerPort);
    if (!found){
        udpServerPort = 44488u;
        REPORT_ERROR(ErrorManagement::Information, "No Port defined! Default to 44488");
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
        ok = (GetNumberOfSignals() > 1u);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "At least two signals shall be configured");
    }
    if (ok) {
        ok = (GetSignalType(0u).numberOfBits == 32u);
        if (!ok) {
            ok = (GetSignalType(0u).numberOfBits == 64u);
            }
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "The first signal shall have 32 bits or 64 bits and %d were specified",
                                    uint16(GetSignalType(0u).numberOfBits))
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

bool UDPSender::AllocateMemory(){

    nOfSignals = GetNumberOfSignals();
    //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "numb of sending signals %d", nOfSignals);

    bool ok = (nOfSignals > 1u);
    uint32 totalPacketSize = 0u;

    if (ok){
        uint32 n;
        for (n = 2u; (n < nOfSignals) && (ok); n++){
            uint32 signalByteSize;
            ok = GetSignalByteSize(n, signalByteSize);
            if (ok){
                totalPacketSize += signalByteSize;
            }
        }
        //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "signal data size %d",(totalPacketSize));
        if (ok){
            UDPPacket.dataBuffer= new AnyType[totalPacketSize];
            uint32 i;
            for (i = 0u; i < (nOfSignals + 2u); i++){
                UDPPacket.dataBuffer[i] = 0;
            }
        }
    }else{
        REPORT_ERROR(ErrorManagement::ParametersError, "A minimum of two signals (counter and timer) must be specified!");
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
    //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "no of signals =  %d",GetNumberOfSignals());

    memoryOffset = 0u;
    bool ok = true;
    if (signalIdx <= (GetNumberOfSignals() -1u)){
        if (signalIdx == 0u) {
            signalAddress = &UDPPacket.sequenceNumber;
        }
        else if (signalIdx == 1u) {
            signalAddress = &UDPPacket.timer;
        }
        else if (signalIdx > 1u ) {
            uint32 i;
            for (i = 2u; i < signalIdx ; i++){      
                uint32 signalByteSize = 0u;
                ok = GetSignalByteSize(i, signalByteSize);
                if (ok){
                    memoryOffset += signalByteSize;
                }
            }
            signalAddress = static_cast<char*>((UDPPacket.dataBuffer[signalIdx - 2u]).GetDataPointer()) + memoryOffset;
            //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "signal =  %d",signalIdx);
            //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "pointer =  %d",(signalAddress));
            //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "memoryOffset =  %d",memoryOffset);


        }
        else{
            ok = false;
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
    uint32 i;
    memoryOffset = 0u;
        for (i = 0u; i < (nOfSignals - 2u); i++){
            uint64 k = 0u;
            uint32 signalByteSize;
            //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "MEMORY OFFSET at %d ",memoryOffset);
            void* p = static_cast<char*>((UDPPacket.dataBuffer[i]).GetDataPointer()) + memoryOffset;
            ok = GetSignalByteSize(i + 2u, signalByteSize);
            if (ok){
                memcpy(&k,p,signalByteSize);
                //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "MEMORY data at %d with size %d with data before %d",p, signalByteSize, k);
                memset(p, 0, signalByteSize);
                memcpy(&k,p,signalByteSize);
                //REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "MEMORY data at %d with size %d with data after %d",p, signalByteSize, k);
                memoryOffset += signalByteSize;
            }
        }
/*    uint64 k = 0u;
    uint32 signalByteSize;
    void* p = static_cast<char*>((UDPPacket.dataBuffer[0]).GetDataPointer());
    GetSignalByteSize(2, signalByteSize);
    memcpy(&k,p,signalByteSize);
    REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "MEMORY data at %d with size %d with data before %d",p, signalByteSize, k);*/
    return ok;
}

CLASS_REGISTER(UDPSender, "1.0")

}

