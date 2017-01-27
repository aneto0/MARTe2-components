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
#if !defined (_UDP_RECEIVER)
#define _UDP_RECEIVER

#include "DataSourceI.h"
#include "CompilerTypes.h"

#include "SingleThreadService.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"

// If the driver is to be used as a receiver or as a transmitter


/// UDP Module Class
namespace MARTe{
class UDPReceiver : public DataSourceI, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    struct UDPMsgHeaderStruct{
    uint64 sequenceNumber;
    uint64 timer;
    AnyType *dataBuffer;
    };   
    
    UDPReceiver();

    virtual ~UDPReceiver();

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

    static void UDPServerThread(void* params);
    /**
    * Required for EmbeddedServiceMethodBinderI.h
    */
    ErrorManagement::ErrorType Execute(const ExecutionInfo& info);
        
    
private:
    enum UDPReceiverModuleType{
    UDP_MODULE_UNDEFINED   = -1,
    UDP_MODULE_RECEIVER    = 0,
    UDP_MODULE_TRANSMITTER = 1
    };
    /**
     * The two supported sleep natures.
     */
    enum LinuxTimerSleepNature {
        Default = 0,
        Busy = 1
    };
    /**
     * The selected sleep nature.
     */
    LinuxTimerSleepNature sleepNature;

    /**
     * The semaphore for the synchronisation between the EmbeddedThread and the Synchronise method.
     */
    EventSem synchSem;

    /**
     * The EmbeddedThread where the Execute method waits for the period to elapse.
     */
    SingleThreadService executor;

    /**
     * HighResolutionTimer::Counter() value after the last Sleep.
     */
    uint64 lastTimeTicks;

    /**
     * Sleeping period in units of ticks.
     */
    uint64 sleepTimeTicks;

    /**
     * Sleeping period.
     */
    uint32 timerPeriodUsecTime;

    /**
     * Index of the function which has the signal that synchronises on this DataSourceI.
     */
    uint32 synchronisingFunctionIdx;

    /**
     * True if this a synchronising data source
     */
    bool synchronising;

   /**
     * Counter and timer since last state change
     */
    uint32 *counterAndTimer;

    uint32 previousCount;

    UDPMsgHeaderStruct UDPPacket;

    bool keepRunning;

    bool dataRecievedCorrectSize;
    bool dataRecieved;


    float32 cycleFrequency;
};
}
#endif
