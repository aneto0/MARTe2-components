/**
 * @file SDNSubscriber.h
 * @brief Header file for class SDNSubscriber
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
 * @details This header file contains the declaration of the class SDNSubscriber
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SDNSUBSCRIBER_H_
#define SDNSUBSCRIBER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "DataSourceI.h"

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
 *     Class = SDNSubscriber
 *     Topic = <name> // The name is used to establish many-to-many communication channels
 *     Interface = <name> // The network interface name to be used
 *     Address = <address>:<port> // Optional - Explicit destination address
 *     Timeout = <timeout_in_ns> // Optional - Blocking sdn::Subscriber::Receive if absent
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
 * The DataSource can be used in asynchronous mode whereby the RT threads are synchronized with an
 * alternative method and the SDNSubscriber holds whichever signal samples were ast received. This
 * is currently specified with 'Timeout = 0'.
 */
class SDNSubscriber : public DataSourceI {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @post
     *   topic = NULL_PTR
     *   subscriber = NULL_PTR
     */
    SDNSubscriber();

    /**
     * @brief Destructor. Releases resources.
     * @post
     *   topic = NULL_PTR
     *   subscriber = NULL_PTR
     */
    virtual ~SDNSubscriber();

    /**
     * @brief Verifies and parses instance parameters.
     * @param[in] data configuration in the form:
     * +Subscriber = {
     *     Class = SDNSubscriber
     *     Topic = <name> // The name is used to establish many-to-many communication channels
     *     Interface = <name> // The network interface name to be used, e.g. eth0
     *     Address = <address>:<port> // Optional - Explicit destination address
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
     * @warning The unicast behaviour is selected by means of specifying any destination address
     * within the IPv4 unicast address range. The socket is bound to the named interface and the
     * address is not used.
     * @return true if criteria are met.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief See DataSourceI::SetConfiguredDatabase.
     * @details The DataSource does not parse the attribute; rather, the method is overloaded to
     * perform signal validity checks outside the scope of the later SDNSubscriber::AllocateMemory
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
     * @details The implementation is associated to a MemoryMapInputBroker.
     * @return MemoryMapInputBroker.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details The implementation provides MemoryMapInputBroker instances.
     * @return false.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @details The implementation does not provide OutputBrokers.
     * @return true if the BrokerI::Init is successful.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);

    /**
     * @brief See DataSourceI::PrepareNextState.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief See DataSourceI::Synchronise.
     * @details The method calls sdn::Subscriber::Receive. The current implementation
     * is limited to blocking reception with configurable timeout. A future version
     * would manage reception asynchronously in own thread.
     * @return true or false in case of error within the SDN core library.
     */
    virtual bool Synchronise();

private:
    
    StreamString ifaceName; // Configuration parameter
    StreamString topicName; // Configuration parameter
    StreamString destAddr;  // Configuration parameter (optional)

    uint32 nOfSignals; // Number of input signals
    bool blocking; // Blocking behaviour
    uint64 timeout; // Configurable timeout (nsec)

    sdn::Topic *topic; // The topic reference
    sdn::Subscriber *subscriber; // The sdn::Subscriber reference

};

} /* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SDNSUBSCRIBER_H_ */

