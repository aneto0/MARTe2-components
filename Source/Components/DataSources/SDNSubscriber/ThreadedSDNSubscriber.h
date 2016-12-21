/**
 * @file ThreadedSDNSubscriber.h
 * @brief Header file for class ThreadedSDNSubscriber
 * @date 20/12/2016
 * @author Bertrand Bauvir
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
 *
 * @details This header file contains the declaration of the class ThreadedSDNSubscriber
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 *
 * @todo Extend the implementation to support distributing signals to multiple GAM.
 * @todo Inherit from (basic) SDNSubscriber for all common tasks.
 */

#ifndef THREADEDSDNSUBSCRIBER_H_
#define THREADEDSDNSUBSCRIBER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "DataSourceI.h"
#include "EventSem.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "SingleThreadService.h"

#include "sdn-api.h" /* SDN core library - API definition (sdn::core) */

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief A DataSource which receives signals transported over the ITER SDN.
 * @details The DataSource connects to the SDN network named interface and received topics
 * with configurable blocking with timeout behaviour.
 *
 * The SDN core library uses topic <name> as key to establish matching communication channels
 * across all participants. Alternatively, the destination address required by the underlying
 * transport may be defined explicitly, in which case matching topic names would only be used
 * to support fault investigation purposes.
 *
 * The configuration syntax is (signals names are only given as an example):
 * +Subscriber = {
 *     Class = ThreadedSDNSubscriber
 *     Topic = <name> // The name is used to establish many-to-many communication channels
 *     Interface = <name> // The network interface name to be used
 *     Address = <address>:<port> // Optional - Explicit destination address
 *     Mode = Default|Caching|Synchronising // Optional - Explicit synchronisation mode
 *     Timeout = <timeout_in_ms> // Optional - Used for synchronising mode semaphore
 *     Signals = {
 *         Counter = {
 *             Type = uint64
 *         }
 *         Timestamp = {
 *             Type = uint64
 *         }
 *         Setpoint = { // The device control command received for use in this RTApplication
 *             Type = double
 *         }
 *     }
 * }
 *
 * The data payload over the network is structured in the same way as the signal definition
 * order. Interoperability between distributed poarticipants require strict configuration control
 * of the payload definition.
 *
 * The DataSource relies on a MemoryMapInputBroker to interface to GAM signals. The DataSource
 * does not allocate memory, rather maps directly the signals to the SDN message payload directly.
 *
 * The DataSource can be used in asynchronous (caching) mode whereby the RT threads are
 * synchronized with an alternative method and the ThreadedSDNSubscriber holds whichever signal
 * samples were last received. This is currently the Default mode which can be left unspecified, or
 * explicitly selected with 'Default' or 'Caching' mode.
 */
class ThreadedSDNSubscriber : public DataSourceI, public EmbeddedServiceMethodBinderI {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @post
     *   mode = Default
     *   topic = NULL_PTR
     *   subscriber = NULL_PTR
     */
    ThreadedSDNSubscriber();

    /**
     * @brief Destructor. Releases resources.
     * @post
     *   topic = NULL_PTR
     *   subscriber = NULL_PTR
     */
    virtual ~ThreadedSDNSubscriber();

    /**
     * @brief Verifies and parses instance parameters.
     * @param[in] data configuration in the form:
     * +Subscriber = {
     *     Class = ThreadedSDNSubscriber
     *     Topic = <name> // The name is used to establish many-to-many communication channels
     *     Interface = <name> // The network interface name to be used, e.g. eth0
     *     Address = <address>:<port> // Optional - Explicit destination address
     *     Mode = Default|Caching|Synchronising // Optional - Explicit synchronisation mode
     * }
     * @details The configuration parameters are subject to the following criteria:
     * The topic <name> is mandatory and can be any string. The <name> is used to associate the
     * subscriber to an address and must be identical on all participants. The mapping between topic
     * <name> and address is done within the scope of the SDN core library and guaranteed to match
     * between all participants using the same topic <name>. Alternatively, the destination address
     * can be explicitly defined using a topic <name> of the form 'sdn://<address>:<port>/<name>';
     * which is purposeful to establish e.g. a unicast connection.
     * The interface <name> is mandatory and verified to correspond to a valid named interface on
     * the host, e.g. eth0.
     * The synchronisation mode is used to discriminate between caching (the thread updated the
     * signal memory with each received SDN message but the synchronisation of the RT thread is
     * managed with an alternative mechanism) vs. synchronising behaviour (the activity of the RT
     * thread is synchronised to the SDN reception).
     * @warning The unicast behaviour is selected by means of specifying any destination address
     * within the IPv4 unicast address range. The socket is bound to the named interface and the
     * address is not used.
     * @return true if criteria are met.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief See DataSourceI::SetConfiguredDatabase.
     * @details The DataSource does not parse the attribute; rather, the method is overloaded to
     * perform signal validity checks outside the scope of the later ThreadedSDNSubscriber::AllocateMemory
     * which can be ensured that it is called with signal list previously validated.
     * @return false in case no signals are being configured.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI& data);

    /**
     * @brief See DataSourceI::AllocateMemory.
     * @details The method instantiate a sdn::Topic and sdn::Subscriber, and used the transport message
     * buffer inside the sdn::Subscriber as memory for output signals.
     * @return false in case or exception inside the SDN core library.
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetSignalMemoryBuffers.
     * @details The method maps signals directly to addresses within the SDN message payload.
     * @return 1.
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                                       const uint32 bufferIdx,
                                       void *&signalAddress);

    /**
     * @brief See DataSourceI::GetBrokerName.
     * @details The implementation is associated to a MemoryMapInputBroker or .
     * MemoryMapSynchronisedInputBroker instances based on the DataSource synchronisation mode.
     * @return MemoryMapInputBroker or MemoryMapSynchronisedInputBroker.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details The implementation provides MemoryMapInputBroker or
     * MemoryMapSynchronisedInputBroker instances based on the DataSource synchronisation mode.
     * @return true if the BrokerI::Init is successful.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @details The implementation does not provide OutputBrokers.
     * @return false.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);

    /**
     * @brief See DataSourceI::PrepareNextState.
     * @details The method empties the receive buffer and starts the embedded thread before
     * returning. 
     * @return true or false if sdn::Subscriber has not been instantiated.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief See DataSourceI::Synchronise.
     * @details The method waits for a synchronisation semaphore which is posted by the 
     * EmbeddedThread upon successful message reception.
     * @return true or false in case of error within the SDN core library.
     */
    virtual bool Synchronise();

    /**
     * @brief Callback function for an EmbeddedThread.
     * @details The method calls sdn::Subscriber::Receive and posts an EventSem to
     * notify the Synchronise method.
     * @param[in] info not used.
     * @return NoError if the EventSem can be successfully posted.
     */
    virtual ErrorManagement::ErrorType Execute(const ExecutionInfo & info);

private:

    /**
     * The two supported sleep natures.
     */
    enum SynchronisingMode {
        Default = 0, // Caching, non-synchronising DataSource
        Synchronising = 1,
    };
    
    StreamString ifaceName; // Configuration parameter
    StreamString topicName; // Configuration parameter
    StreamString destAddr;  // Configuration parameter (optional)
    StreamString modeName;  // Configuration parameter (optional)

    uint32 nOfSignals; // Number of input signals
    TimeoutType TTTimeout; // Configurable timeout (msec)

    sdn::Topic *topic; // The topic reference
    sdn::Subscriber *subscriber; // The sdn::Subscriber reference

    SynchronisingMode mode;

    /**
     * The semaphore for the synchronisation between the EmbeddedThread and the Synchronise method.
     */
    EventSem synchronisingSem;

    /**
     * The EmbeddedThread where the Execute method waits for the SDN topic to be received.
     */
    SingleThreadService executor;

};

} /* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* THREADEDSDNSUBSCRIBER_H_ */

