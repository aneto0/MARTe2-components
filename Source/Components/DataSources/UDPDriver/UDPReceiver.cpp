//******************************************************************************
//
//    $Log$
//
//******************************************************************************

#include "UDPReceiver.h"
#include "Endianity.h"
#include "FastPollingMutexSem.h"
#include "AdvancedErrorManagement.h"
#include "BrokerI.h"
#include "ErrorManagement.h"
#include "ErrorInformation.h"
#include "GlobalObjectsDatabase.h"
#include "ConfigurationDatabase.h"

#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapInputBroker.h"

#include "Threads.h"
#include "UDPSocket.h"

namespace MARTe{

static bool serverOK = false;
static bool serverDone = false;
static uint32 udpServerPort = 44488;
static StreamString udpServerAddress = "127.0.0.1";
static uint32 nOfSignals = 1;
static uint32 totalPacketSize;
UDPSocket server;
uint32 memoryOffset = 0u;

UDPReceiver::UDPReceiver(): DataSourceI(), EmbeddedServiceMethodBinderI(), executor(*this){
    sleepNature = Default;
    synchronising = true;
    if (!synchSem.Create()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not create EventSem.");
    }
    

}

/*
 * Destructor
 */
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
        serverDone = true;
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
    keepRunning = true;
    bool found = data.Read("Port", udpServerPort);
    if (!found){
        udpServerPort = 44488;
        REPORT_ERROR(ErrorManagement::Information, "No port defined! Default to 44488");
    }
    return ok;
}

bool UDPReceiver::AllocateMemory(){
    nOfSignals = GetNumberOfSignals();

    bool ok = (nOfSignals > 1u);
    totalPacketSize = 0u;
    REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "numb of reciving signals %d", nOfSignals);
    if (ok){
        uint32 n;
        for (n = 2u; (n < nOfSignals) && (ok); n++){
            uint32 signalByteSize;
            GetSignalByteSize(n, signalByteSize);
            totalPacketSize += signalByteSize;
        }
        UDPPacket.dataBuffer= new AnyType[totalPacketSize];
        uint32 i;
        for (i = 0u; i <nOfSignals - 2; i++ ){
            UDPPacket.dataBuffer[i] = 0u;
        }
        
    }else{
        REPORT_ERROR(ErrorManagement::ParametersError, "A minimum of two signals (counter and timer) must be specified!");
    }
    return ok;
}

uint32 UDPReceiver::GetNumberOfMemoryBuffers(){
    return 1u;
}

bool UDPReceiver::GetSignalMemoryBuffer(const uint32 signalIdx,
                                         const uint32 bufferIdx,
                                         void*& signalAddress) {
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
        signalAddress = (void *)(static_cast<char*>((UDPPacket.dataBuffer[signalIdx - 2u]).GetDataPointer()) + memoryOffset);
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
    bool ok = false;
        ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
        ok = broker.IsValid();
        if (ok) {
            ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
        }
        if (ok) {
            ok = inputBrokers.Insert(broker);
        }
    return ok;
}

bool UDPReceiver::GetOutputBrokers(ReferenceContainer& outputBrokers,
                                    const char8* const functionName,
                                    void* const gamMemPtr) {
    return false;
}

bool UDPReceiver::PrepareNextState(const char8* const currentStateName,
                                    const char8* const nextStateName) {
    bool ok = true;
    REPORT_ERROR(ErrorManagement::Information, "prepare next state");
    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
        keepRunning = true;
        ok = executor.Start();
    } 
    UDPPacket.sequenceNumber = 0u;
    UDPPacket.timer = 0u;
    UDPPacket.dataBuffer = NULL_PTR(AnyType*);
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
        ok &= server.Listen(thisHost.GetPort());
    }
    if (!ok){
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not open the port!");
    }
    if (ok) {
        ok = (GetNumberOfSignals() > 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "At least two signals shall be configured");
        }
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


/**
 * Required for EmbeddedServiceMethodBinderI.h
 */
ErrorManagement::ErrorType UDPReceiver::Execute(const ExecutionInfo& info) {

    ErrorManagement::ErrorType err;
    if (info.GetStage() == ExecutionInfo::TerminationStage){
        keepRunning = false;
    }
    else{
        uint32 udpServerExpectReadSize = totalPacketSize + (GetSignalType(0).numberOfBits/8) + (GetSignalType(1).numberOfBits/8);
        uint8 udpServerBufferRead[udpServerExpectReadSize];
        uint32 udpServerReadSize = udpServerExpectReadSize;
        uint8 i;
        bool OK = true;
        for (i = 0; (i < udpServerExpectReadSize)&& keepRunning; i++){
            udpServerBufferRead[i] = 0;
        }
        if (keepRunning) {
            dataRecieved &= server.Read((char8 *) udpServerBufferRead, udpServerReadSize);
        }
        dataRecieved = (udpServerReadSize > 0u);
        dataRecievedCorrectSize = (udpServerReadSize == udpServerExpectReadSize);
        if (!dataRecieved){
            REPORT_ERROR(ErrorManagement::ParametersError, "No data recieved");
            Sleep::Sec(20e-6);
        }else if(!dataRecievedCorrectSize){
            REPORT_ERROR(ErrorManagement::ParametersError, "Recieved data of inccorect size, ignoring it.");
            Sleep::Sec(100e-6);
        }else{
            uint8 signalOffset = 0u;
            memoryOffset = 0u;
            for (i = 0; (i < nOfSignals) && keepRunning; i++){
                if (i >= 0 && OK){
                    uint32 signalByteSize = GetSignalType(i).numberOfBits / 8;
                    uint64 recievedData;
                    uint32 size = GetSignalType(i).numberOfBits;
                    AnyType AnytypeData = new AnyType;

                    if (i == 0){
                        AnytypeData = UDPPacket.sequenceNumber;
                    }else if (i == 1){
                        AnytypeData = UDPPacket.timer;                    
                    }else if (i > 1){  
                        AnytypeData = UDPPacket.dataBuffer[i -2];
                    }

                    uint32 noOfBytesForSignal = size/8;
                    uint8 dataConv[noOfBytesForSignal];
                    uint32 counter;
                    for (counter = 0; counter < noOfBytesForSignal; counter++){
                        dataConv[counter] = udpServerBufferRead[signalOffset + counter];
                    }
                    if (i == 0 || i == 1){
                        memcpy(AnytypeData.GetDataPointer(),dataConv,signalByteSize);
                        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "I recieved UDP data!!!! = %d", AnytypeData);
                    }else if (i >1){
                        void *p = static_cast<char*>(AnytypeData.GetDataPointer()) + memoryOffset;
                        memcpy(p,&dataConv,signalByteSize);
                        uint32 test;
                        memcpy(&test,p,signalByteSize);
                        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "I recieved UDP data!!!! = %d", test);
                        memoryOffset += signalByteSize;
                    }
                    signalOffset += noOfBytesForSignal;
                }else{
                    OK = false;
                    REPORT_ERROR(ErrorManagement::FatalError, "Tried to access negative signal.");
                }
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

