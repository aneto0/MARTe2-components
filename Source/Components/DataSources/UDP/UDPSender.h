/**
 * @file UDPSender.h
 * @brief Header file for class UDPSender
 * @date 10/05/2021
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

 * @details This header file contains the declaration of the class UDPSender
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#if !defined  (UDP_SENDER_H_)
#define UDP_SENDER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "MemoryDataSourceI.h"
#include "ProcessorType.h"
#include "UDPSocket.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A DataSource which transmits given signals via UDP.
 *
 * The configuration syntax is (names are only given as an example):
 * +UDPSender = {
 *     Class = UDPDrv::UDPSender
 *     Address = "127.0.0.1" //Optional (default 127.0.0.1) The address of the UDPReciever
 *     Port = "44488" //Optional (default 44488) The port of the UDPReciever
 *     CPUMask = 15 //Compulsory. Affinity assigned to the threads responsible for asynchronously flush data.
 *     StackSize = 10000000 //Compulsory. Stack size of the thread above.
 *     Signals = {
 *          Trigger = { //Mandatory. Must be in first position.
 *              Type = uint8
 *          }
 *          Counter = { //Mandatory. Packet Counter. Must be in second position
 *              Type = uint64 //Mandatory (Note: Sender and Receiver must be same type),  int64 also supported.
 *          }
 *          Time = { //Mandatory. Current cycle timestamp. Must be in third position
 *               Type = uint64 //Mandatory (Note: Sender and Receiver must be same type),  int64 also supported.
 *          }
 *          Signal1 = { //One or more extra signals shall be defined
 *              Type = float32 //Mandatory (Note: Sender and Receiver must be same type). All other MARTe2 types are supported
 *          }
 *          Signal2 = {
 *             ...
 *          }
 *          ...
 *     }
 * }
 */
class UDPSender: public MemoryDataSourceI {
public:CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details Initialises all the optional parameters as described in the class description.
     */
    UDPSender();

    /**
     * @brief Destructor.
     * @details Closes the current UDP socket.
     */
    virtual ~UDPSender();

    /**
     * @brief Cuts the UDP message, then transmits them to the designated address and port.
     * @return true if the semaphore is successfully posted.
     */
    virtual bool Synchronise();

    /**
     * @brief See DataSourceI::AllocateMemory.
     * @return true.
     */
    //virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    //virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetSignalMemoryBuffer.
     */
    /*virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                                       const uint32 bufferIdx,
                                       void *&signalAddress);*/

    /**
     * @brief See DataSourceI::GetBrokerName.
     * @details Only Output signals are supported.
     * @return MemoryMapSynchronisedOutputBroker
     */
    virtual const char8* GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @return false.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8 *const functionName,
                                 void *const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @details It adds a MemoryMapSynchronisedOutputBroker instance to the outputBrokers.
     * @return true.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8 *const functionName,
                                  void *const gamMemPtr);

    /**
     * @brief Initialises the packet data and stores the time
     * @return true
     */
    virtual bool PrepareNextState(const char8 *const currentStateName,
                                  const char8 *const nextStateName);

    /**
     * @brief Final verification of all the parameters and opens the connection to the receiver server.
     * @details Ensures that the client connection to the reciever is established, and that a counter and timer variable are
     * defined and of the correct types/sizes
     * @return true if all parameters are correctly configured and connection opened to the receiver server.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI &data);

    /**
     * @brief Loads and verifies the configuration parameters detailed in the class description.
     * @return true if all the mandatory parameters are correctly specified and if the specified optional parameters have valid values.
     */
    virtual bool Initialise(StructuredDataI &data);

private:

    /**
     * The IP address to which the data will be transmitted to
     */
    StreamString udpServerAddress;

    /**
     * Number of pre buffers.
     */
    uint32 numberOfPreTriggers;

    /**
     * Number of post buffers.
     */
    uint32 numberOfPostTriggers;

    /**
     * The affinity of the thread that asynchronously flushes data into MDSplus.
     */
    ProcessorType cpuMask;

    /**
     * The size of the stack of the thread that asynchronously flushes data into DAN.
     */
    uint32 stackSize;

    /**
     * The port that the sender will transmit to
     */
    uint16 udpServerPort;


    /**
     * The socket that will connect to the receiver
     */
    UDPSocket * client;

    /**
     * Memory map asynchronously triggered broker.
     */
    ReferenceT<MemoryMapAsyncTriggerOutputBroker> brokerAsyncTrigger;
};
}
#endif
