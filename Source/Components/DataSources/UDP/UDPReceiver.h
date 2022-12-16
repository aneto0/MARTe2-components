/**
 * @file UDPReceiver.h
 * @brief Header file for class UDPReceiver
 * @date 11/05/2021
 * @author Luca Porzio
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

#ifndef UDP_RECEIVER_H_
#define UDP_RECEIVER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "MemoryDataSourceI.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"
#include "SingleThreadService.h"
#include "BasicUDPSocket.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe{

typedef enum {
    UDPReceiverExecutionModeIndependent,
    UDPReceiverExecutionModeRealTime
} UDPReceiverExecutionMode;


/**
 * @brief A DataSource which receives given signals via UDP with Multicast support.
 *
 * The configuration syntax is (names are only given as an example):
 * +UDPReceiver = {
 *     Class = UDP::UDPReceiver
 *     Address = "230.168.129.30" //Optional. Only for Multicast group
 *     InterfaceAddress = "192.168.2.30" //Optional. Only for Multicast. Bind to the specific local address. If not set, the default IPv4 multicast interface will be used and packets may not be received.
 *     Port = "44488" //Optional. Default: 44488
 *     Timeout = "5.0" //Optional (seconds) The time the receiver will wait while listening before timing out. Default: Infinite
 *     ExecutionMode = RealTimeThread//Optional (default RealTimeThread)
 *       If ExecutionMode == IndependentThread a thread is spawned to read from the socket and the memory copy is immediate (i.e. it will not block).
 *       If ExecutionMode == RealTimeThread the DataSource socket read is blocking and handled in the context of the real-time thread.
 *     CPUMask = 0x1
 *     StackSize = 10000000
 *     Signals = {
 *          Signal2 = {
 *             Type = uint32 //Any MARTe2 type
 *          }
 *          ...
 *     }
 * }
 *
 * The Signals section is in practice a description of the structure of the UDP Packet read.
 */
class UDPReceiver : public MemoryDataSourceI, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    
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
     * @brief Syncs the embedded thread, while waiting to receive data
     * @return true if no errors persist
     */
    virtual bool Synchronise();
    
    /**
     * @brief See DataSourceI::GetBrokerName.
     * @details Only Input signals are supported.
     * @return MemoryMapSynchronisedInputBroker
     */
    virtual const char8* GetBrokerName(StructuredDataI& data,
                                            const SignalDirection direction);

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
     * @brief Allocates the MemoryDataSourceI memory buffer and spawns the thread if in IndependentThread mode.
     * @return true if MemoryDataSourceI::AllocateMemory returns true and the Service is successfully started (in IndependentThread mode).
     */
    virtual bool AllocateMemory();

    /**
     * @brief Indicates that the broker has finished to copy the data from the data source memory.
     * @return true
     */
    virtual bool BrokerCopyTerminated();

    /**
     * @brief Recieve signals when UDP data is recieved.
     * @return any errors.
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo& info);
    
    /**
     * @brief Gets the stack size of the thread which is going to be used to asynchronously store the data.
     * @return the stack size of the thread which is going to be used to asynchronously store the data.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Gets the port to be used by the BasicUDPSocket.
     * @return the port to be used by the BasicUDPSocket.
     */
    uint16 GetPort() const;

    /**
     * @brief Gets the address to be used by the BasicUDPSocket.
     * @return the address to be used by the BasicUDPSocket.
     */
    StreamString GetAddress() const;

    /**
     * @brief Gets the affinity of the thread which is going to be used to asynchronously store the data.
     * @return the affinity of the thread which is going to be used to asynchronously store the data.
     */
    const uint32 GetCPUMask() const;

    /**
     * @brief Gets the mode selector value.
     * @return the mode selector value.
     */
    const UDPReceiverExecutionMode GetExecutionMode() const;

private:

    /**
     * The EmbeddedThread where the Execute method waits for the period to elapse.
     */
    SingleThreadService executor;

    /**
     * The timeout for which the server will listen to the specified port
     */
    TimeoutType timeout;

    /**
     * The receiver will listen on
     */
    uint16 port;

    /**
     * The multicast IP address to which the data will be received on
     */
    StreamString address;

    /**
     * The local IP address of the interface to which the socket will bind.
     */
    StreamString interfaceAddress;

    /**
     * The socket that will connect to the sender
     */ 
    BasicUDPSocket* socket;
    
    /**
     * CPU affinity number for the executor thread
     */
    uint32 cpuMask;

    /**
     * The stack size
     */
    uint32 stackSize;

    /**
     * Holds the current execution mode of the datasource.
     */
    UDPReceiverExecutionMode executionMode;

    /**
     * Mux for the IndependentThread implementation.
     */
    FastPollingMutexSem muxIThread;

    /**
     * Copy in progres flag for the IndependentThread implementation.
     */
    bool copyInProgress;

    /**
     * Memory for the independent thread reading.
     */
    void *memoryIndependentThread;
};
}
#endif
