/**
 * @file SDNPublisher.h
 * @brief Header file for class SDNPublisher
 * @date 12/12/2016
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
 * @details This header file contains the declaration of the class SDNPublisher
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 *
 * @todo Nothing prevents the SDNPublisher DataSource to make the signals available
 * as InputSignals to other GAMs. The implementation may be extended to return
 * InputBrokers.
 * @todo Extend the implementation to support multiple GAM OutputSignals and make use
 * of Trigger signal declaration.
 * @todo Extend the implementation to support asynchronous publication in own thread 
 * whereby the SDN topic may be published at configurable rate and phase regardless
 * of the RT thread activity.
 */

#ifndef SDNPUBLISHER_H_
#define SDNPUBLISHER_H_

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
 * @brief A DataSource which collects and publishes signals over the ITER SDN.
 * @details The DataSource collects its inputs signals into a SDN topic and publishes
 * the topic over the SDN network.
 *
 * The SDN core library uses topic <name> as key to establish matching communication channels
 * across all participants. Alternatively, the destination address required by the underlying
 * transport may be defined explicitly, in which case matching topic names would only be used
 * to support fault investigation purposes.
 *
 * The configuration syntax is (signals names are only given as an example):
 * +Publisher = {
 *     Class = SDNPublisher
 *     Topic = <name> // The name is used to establish many-to-many communication channels
 *     Interface = <name> // The network interface name to be used
 *     Address = <address>:<port> // Optional - Explicit destination address
 *     Signals = {
 *         Counter = {
 *             Type = uint64
 *         }
 *         Timestamp = {
 *             Type = uint64
 *         }
 *         Setpoint = { // The device control command elaborated within this RTApplication
 *             Type = double
 *         }
 *     }
 * }
 *
 * The data payload over the network is structured in the same way as the signal definition
 * order. Interoperability between distributed poarticipants require strict configuration control
 * of the payload definition.
 *
 * The DataSource relies on a MemoryMapOutputBroker to interface to GAM signals. The DataSource
 * does not allocate memory, rather maps directly the signals to the SDN message payload directly.
 */
class SDNPublisher : public DataSourceI {

public:

    CLASS_REGISTER_DECLARATION()

/**
 * @brief Default constructor.
 * @post
 *   topic = NULL_PTR
 *   publisher = NULL_PTR
 *   synchronizing = false
 */
SDNPublisher();

    /**
     * @brief Destructor. Releases resources.
     * @post
     *   topic = NULL_PTR
     *   publisher = NULL_PTR
     */
    virtual ~SDNPublisher();

    /**
     * @brief Verifies and parses instance parameters.
     * @param[in] data configuration in the form:
     * +Publisher = {
     *     Class = SDNPublisher
     *     Topic = <name> // The name is used to establish many-to-many communication channels
     *     Interface = <name> // The network interface name to be used, e.g. eth0
     *     Address = <address>:<port> // Optional - Explicit destination address
     * }
     * @details The configuration parameters are subject to the following criteria:
     * The topic <name> is mandatory and can be any string. The <name> is used to associate the
     * publisher to an address and must be identical on all participants. The mapping between topic
     * <name> and address is done within the scope of the SDN core library and guaranteed to match
     * between all participants using the same topic <name>. Alternatively, the destination address
     * can be explicitly defined using a topic <name> of the form 'sdn://<address>:<port>/<name>';
     * which is purposeful to establish e.g. a unicast connection.
     * The interface <name> is mandatory and verified to correspond to a valid named
     * interface on the host, e.g. eth0.
     * @return true if criteria are met.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief See DataSourceI::SetConfiguredDatabase.
     * @details The DataSource does not parse the attribute; rather, the method is overloaded to
     * perform signal validity checks outside the scope of the later SDNPublisher::AllocateMemory
     * which can be ensured that it is called with signal list previously validated.
     * @return false in case no signals are being configured.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI& data);

    /**
     * @brief See DataSourceI::AllocateMemory.
     * @details The method instantiate a sdn::Topic and sdn::Publisher, and used the transport message
     * buffer inside the sdn::Publisher as memory for input signals.
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
     * @details The implementation is associated to a MemoryMapOutputBroker.
     * @return MemoryMapOutputBroker.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details The implementation does not provide InputBrokers.
     * @return false.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @details The implementation provides MemoryMapOutputBroker instances.
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
     * @details The method calls sdn::Publisher::Publish and relies on the fact that SDN
     * message payload has been previously modified by the OutputBroker instances.
     * @return true or false in case of error within the SDN core library.
     */
    virtual bool Synchronise();

private:

    StreamString ifaceName; // Configuration parameter
    StreamString topicName; // Configuration parameter
    StreamString destAddr;  // Configuration parameter (optional)

    uint32 nOfSignals; // Number of input signals
    uint32 nOfTriggers; // Number of input synchronising signals

    sdn::Topic *topic; // The topic reference
    sdn::Publisher *publisher; // The sdn::Publisher reference

};

} /* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SDNPUBLISHER_H_ */

