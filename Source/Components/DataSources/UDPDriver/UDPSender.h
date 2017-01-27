//*****************************************************************************
//
//    $Log$
//
//*****************************************************************************

/**
 * @file UDP driver which can be used to connect several MARTe systems or to 
 * interface into external systems. For data BasicTypeing see also the BasicTypeingDriver.
 * This code is heavily based in the ATMDrv, but in this case the header is implicit 
 * (not retrieved from the DDB) when used in UDP_MODULE_TRANSMITTER mode
 */
#if !defined (_UDP_SENDER)
#define _UDP_SENDER

#include "DataSourceI.h"
#include "CompilerTypes.h"

#include "EventSem.h"

// If the driver is to be used as a receiver or as a transmitter


/// UDP Module Class
namespace MARTe{
class UDPSender : public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()
    struct UDPMsgHeaderStruct{
    uint64 sequenceNumber = 0u;
    uint64 timer = 0u;
    AnyType *dataBuffer = NULL_PTR(AnyType*);
    };   
    
    UDPSender();

    virtual ~UDPSender();

    virtual bool Synchronise();

    virtual bool AllocateMemory();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                                            const uint32 bufferIdx,
                                            void*& signalAddress);
    virtual const char8* GetBrokerName(StructuredDataI& data,
                                            const SignalDirection direction);

    virtual bool GetInputBrokers(ReferenceContainer& inputBrokers,
                                    const char8* const functionName,
                                    void* const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer& outputBrokers,
                                        const char8* const functionName,
                                        void* const gamMemPtr);

    virtual bool PrepareNextState(const char8* const currentStateName,
                                        const char8* const nextStateName);

    virtual bool SetConfiguredDatabase(StructuredDataI& data);

    virtual bool Initialise(StructuredDataI &data);

private:

   /**
     * Counter and timer since last state change
     */
    uint32 *counterAndTimer;

    UDPMsgHeaderStruct UDPPacket;

    uint32 previousCount;


};
}
#endif
