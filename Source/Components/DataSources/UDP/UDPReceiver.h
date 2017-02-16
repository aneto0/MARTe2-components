/**
 * @file UDPReceiver.h
 * @brief Header file for class UDPReceiver
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

 * @details This header file contains the declaration of the class UDPReceiver
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#if !defined (UDP_RECEIVER_H_)
#define UDP_RECEIVER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "DataSourceI.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"
#include "SingleThreadService.h"
#include "UDPSocket.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe{
/**
 * @brief A DataSource which recieves given signals via UDP.
 *
 * The configuration syntax is (names are only given as an example):
 * +UDPReceiver = {
 *     Class = UDPDrv::UDPReceiver
 *     Port = "44488" //Optional (default 44488) The port of the UDPReciever
 *     Timeout = "5" //Optional (default infinite, also known as 0) The time that the reciever will listen on a defined port before timeout
 *     Signals = {
 *          Counter = { //Mandatory. Number of ticks since last state change.
 *              Type = uint32 //Mandatory (Note: Sender and Receiver must be same type). int32, int64, uint64 also supported.
 *          }
 *          Time = { //Mandatory. Elapsed time in micro-seconds since last state change.
 *               Type = uint32 //Mandatory (Note: Sender and Receiver must be same type). int32, int64, uint64 also supported.
 *          }
 *          Signal1 = { //One or more extra signals shall be defined
 *              Type = float32 //Mandatory (Note: Sender and Receiver must be same type). All other MARTe types are supported
 *          }
 *          Signal2 = {
 *             ...
 *          }
 *          ...
 *     }
 * }
 */
class UDPReceiver : public DataSourceI, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    
    /**
     * @brief PacketTypes.
     * @details The layout for the packet that will be recieved
     */
    
    /**
     * @brief Default constructor.
     * @details Initialises all the optional parameters as described in the class description.
     */
    UDPReceiver();

     /**
     * @brief Destructor.
     * @details Closes the current UDP socket and stops the embedded thread.
     */
    virtual ~UDPReceiver();

    /**
     * @brief Syncs the embedded thread, while waiting to recieve data
     * @return true if no errors persist
     */
    virtual bool Synchronise();

    /**
     * @brief See DataSourceI::AllocateMemory.
     * @return true.
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetSignalMemoryBuffer.
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                                            const uint32 bufferIdx,
                                            void*& signalAddress);
    
    /**
     * @brief See DataSourceI::GetBrokerName.
     * @details Only Input signals are supported.
     * @return MemoryMapInputBroker
     */
    virtual const char8* GetBrokerName(StructuredDataI& data,
                                            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details It adds a MemoryMapSynchronisedOutputBroker instance to the outputBrokers.
     * @return true.
     */                                        
    virtual bool GetInputBrokers(ReferenceContainer& inputBrokers,
                                    const char8* const functionName,
                                    void* const gamMemPtr);
    
    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @return false.
     */
    virtual bool GetOutputBrokers(ReferenceContainer& outputBrokers,
                                        const char8* const functionName,
                                        void* const gamMemPtr);

    /**
     * @brief Starts the EmbeddedThread and sets the counter and the time to zero.
     * @details See StatefulI::PrepareNextState. Starts the EmbeddedThread (if it was not already started) that waits to recieve UDP data.
     * Sets the counter and the time to zero.
     * @return true if the EmbeddedThread can be successfully started.
     */
    virtual bool PrepareNextState(const char8* const currentStateName,
                                        const char8* const nextStateName);
    
    /**
     * @brief Final verification of all the parameters and opens the connection to the receiver server.
     * @details Ensures that the client connection to the sender is established, and that a counter and timer variable are
     * defined and of the correct types/sizes
     * @return true if all parameters are correctly configured and connection opened to the receiver server.
     */                                    
    virtual bool SetConfiguredDatabase(StructuredDataI& data);

    /**
     * @brief Loads and verifies the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Recieve signals when UDP data is recieved.
     * @return any errors.
     */
    virtual ErrorManagement::ErrorType Execute(const ExecutionInfo& info); 
    
private:
    
    /**
     * The semaphore for the synchronisation between the EmbeddedThread and the Synchronise method.
     */
    EventSem synchSem;

    /**
     * The EmbeddedThread where the Execute method waits for the period to elapse.
     */
    SingleThreadService executor;

    /**
     * True if the server should remain running
     */
    bool keepRunning;

    /**
     * True if the server recieved data of the correct size (number of bytes defined in configuration file matches 
     * that to which is recieved)
     */
    bool dataRecievedCorrectSize;

    /**
     * True if data is recieved within the timeout period specified
     */
    bool dataRecieved;

    /**
     * The timeout for which the server will listen to the specified port
     */
    TimeoutType timeout;

    /**
     * The receiver will listen on
     */
    uint16 udpServerPort;

    /**
     * The IP address to which the data will be recieved on
     */
    StreamString udpServerAddress;

    /**
     * The number of signals that will be received
     */
    uint32 nOfSignals;

    /**
     * The total size in bytes of the expected packet that will be received
     */
    uint32 totalPacketSize;

    /**
     * An array storing the pointer offset (number of bytes) for each signal, 
     * EG to get the pointer offset  for signal 1 = signalsMemoryOffset[0]
     */
    uint32 *signalsMemoryOffset;

    /**
     * A pointer to memory that will contain all the information that will be sent
     */
    void *dataBuffer;
    
    /**
     * A pointer to the sequence number data that is stored within the databuffer
     */   
    uint64 *sequenceNumberPtr;
    
    /**
     * A pointer to the timer data that is stored within the databuffer
     */    
    uint64 *timerPtr;

    /**
     * The socket that will connect to the sender
     */ 
    UDPSocket server;

    bool synchronising;

};
}
#endif
