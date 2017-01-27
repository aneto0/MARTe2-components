//******************************************************************************
//
//    $Log$
//
//******************************************************************************

#include "UDPSender.h"
#include "AdvancedErrorManagement.h"
#include "BrokerI.h"
#include "Shift.h"
#include "UDPSocket.h"
#include "MemoryMapOutputBroker.h"

namespace MARTe{


static uint32 nOfSignals = 0;
static uint32 udpServerPort;
static StreamString udpServerAddress;
static UDPSocket client;
static uint64 timerAtStateChange = 0u;
static uint64 memoryOffset = 0u;

UDPSender::UDPSender():DataSourceI(){}

/*
 * Destructor
 */
UDPSender::~UDPSender(){
     if (!client.Close()){
         REPORT_ERROR(ErrorManagement::FatalError, "Could not close UDP sender.");
     }
}

/**
 * Required for DataSourceI.h
 */
bool UDPSender::Synchronise(){
    
    bool OK = true;
    const MARTe::uint32 udpServerExpectReadSize = nOfSignals * 8;
    uint8 i;
    uint8 udpServerWriteBuffer[udpServerExpectReadSize];
    uint32 signalOffset = 0;
    memoryOffset = 0u;
    for (i = 0; i < nOfSignals; i++){
        AnyType dataConv = new AnyType;
        if (i == 0){
            dataConv = UDPPacket.sequenceNumber;
        } else if (i == 1){
            UDPPacket.timer=HighResolutionTimer::Counter() - timerAtStateChange;
            dataConv = UDPPacket.timer;
        }else if (i > 1){

            bool ok = (GetSignalType(i).numberOfBits == 32u);
            uint32 size = GetSignalType(i).numberOfBits;
            

            dataConv = UDPPacket.dataBuffer[i-2];

        }else{
            OK = false;
            REPORT_ERROR(ErrorManagement::FatalError, "Tried to access negative signal.");
        }

        if (OK){

            uint32 signalByteSize = GetSignalType(i).numberOfBits / 8;

            uint64 k = 0;
            uint8 AnyTypetoUint8 [signalByteSize];
            for (k = 0u; k <signalByteSize; k++ ){
                AnyTypetoUint8[k] = 0u;
            }
            if (i == 0 || i == 1){
            memcpy(&AnyTypetoUint8,dataConv.GetDataPointer(),signalByteSize);
            memcpy(&k,static_cast<char*>(dataConv.GetDataPointer()),signalByteSize);

            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, " I am sending data: %d",k);

            
            }else if (i > 1){
            memcpy(&AnyTypetoUint8,static_cast<char*>(dataConv.GetDataPointer()) + memoryOffset,signalByteSize);

            memcpy(&k,static_cast<char*>(dataConv.GetDataPointer()) + memoryOffset,signalByteSize);
            memoryOffset += signalByteSize;
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, " I am sending data: %d",k);
    
            }

            uint32 j;
            for(j = 0; j < signalByteSize; j++){
                udpServerWriteBuffer[j + signalOffset] = AnyTypetoUint8[j];
            }
            signalOffset += signalByteSize;
        }

    }
    if (OK){
        uint32 bytesSent = udpServerExpectReadSize;
        client.Write((char8 *) udpServerWriteBuffer, bytesSent);
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
        udpServerPort = 44488;
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
            ok = client.Connect((char8*)udpServerAddress.Buffer(), udpServerPort);
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
    return ok;
}

bool UDPSender::AllocateMemory(){

    nOfSignals = GetNumberOfSignals();
    REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "numb of sending signals %d", nOfSignals);

    bool ok = (nOfSignals > 1u);
    uint32 totalPacketSize = 0u;

    if (ok){
        uint32 n;
        for (n = 2u; (n < nOfSignals) && (ok); n++){
            uint32 signalByteSize;
            GetSignalByteSize(n, signalByteSize);
            totalPacketSize += signalByteSize;
        }
        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "signal data size %d",(totalPacketSize));
        UDPPacket.dataBuffer= new AnyType[totalPacketSize];
        uint32 i;
        for (i = 0u; i <nOfSignals - 2; i++ ){
            UDPPacket.dataBuffer[i] = i;
        }

    }else{
        REPORT_ERROR(ErrorManagement::ParametersError, "A minimum of two signals (counter and timer) must be specified!");
    }
    return ok;
}

uint32 UDPSender::GetNumberOfMemoryBuffers(){
    return 1u;
}

bool UDPSender::GetSignalMemoryBuffer(const uint32 signalIdx,
                                         const uint32 bufferIdx,
                                         void*& signalAddress) {
    REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "no of signals =  %d",GetNumberOfSignals());

    memoryOffset = 0u;
    bool ok = true;
    if (signalIdx <= (GetNumberOfSignals() -1)){
        if (signalIdx == 0u) {
            signalAddress = &UDPPacket.sequenceNumber;
        }
        else if (signalIdx == 1u) {
            signalAddress = &UDPPacket.timer;
        }
        else if (signalIdx > 1u ) {
            uint32 i;
            for (i = 2u; i < signalIdx ; i++){      
                uint32 signalByteSize;
                GetSignalByteSize(i, signalByteSize);
                memoryOffset += signalByteSize;
            }
            signalAddress = static_cast<char*>((UDPPacket.dataBuffer[signalIdx - 2u]).GetDataPointer()) + memoryOffset;
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "signal =  %d",signalIdx);
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "pointer =  %d",(signalAddress));
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "memoryOffset =  %d",memoryOffset);



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

const char8* UDPSender::GetBrokerName(StructuredDataI& data,
                                         const SignalDirection direction) {
    return "MemoryMapOutputBroker";
}

bool UDPSender::GetInputBrokers(ReferenceContainer& inputBrokers,
                                   const char8* const functionName,
                                   void* const gamMemPtr) {
    return false;
}

bool UDPSender::GetOutputBrokers(ReferenceContainer& outputBrokers,
                                    const char8* const functionName,
                                    void* const gamMemPtr) {
    ReferenceT<BrokerI> broker("MemoryMapOutputBroker");
    bool ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    if (ok) {
        ok = outputBrokers.Insert(broker);
    }
    return ok;
}

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

