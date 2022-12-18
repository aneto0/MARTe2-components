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
/*Cannot include "sdn-header.h" otherwise lint gets lost in secondary includes.*/
/*lint -save -e9113 -e553
 * this is a macro that cannot be changed.
 */
#if UNIT_VERSION > UNIT_VERSION_UID(1,1,0)
#ifndef FEATURE_10840
#define FEATURE_10840
#endif
#endif
/*lint -restore*/
#if ((defined(LINT)) || (!defined(FEATURE_10840)))
namespace sdn {
//! @cond Doxygen_Suppress
 /*lint -estring(768, "sdn::Header_t::*") -e{970} -estring(754, "sdn::Header_t::*") -estring(770, "*sdn::Header_t*") -estring(9109, "*sdn::Header_t*")*/
typedef struct {
  /* see SDN core library */
  char     header_uid [4];
  /*lint -e{970}*/
  char     header_version [4]; /* see SDN core library */
  uint32_t header_size; /* see SDN core library */
  uint32_t topic_uid; /* see SDN core library */
  uint32_t topic_version; /* see SDN core library */
  uint32_t topic_size; /* see SDN core library */
  uint64_t topic_counter; /* see SDN core library */
  uint64_t send_time; /* see SDN core library */
  uint64_t recv_time; /* see SDN core library */

} Header_t;
}
//! @endcond
#endif

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief A DataSource which collects and publishes signals over the ITER SDN.
 * @details The DataSource collects its inputs signals into a SDN topic and publishes
 * the topic over the SDN network.
 *
 * The SDN core library uses topic "name" as key to establish matching communication channels
 * across all participants. Alternatively, the destination address required by the underlying
 * transport may be defined explicitly, in which case matching topic names would only be used
 * to support fault investigation purposes.
 *
 * The configuration syntax is (signals names are only given as an example):
 *
 * <pre>
 * +Publisher = {
 *     Class = SDNPublisher
 *     Topic = name // The name is used to establish many-to-many communication channels
 *     Interface = name // The network interface name to be used
 *     Address = address:port // Optional - Explicit destination address
 * \b ifdef FEATURE_10840
 *     SourcePort = port // Optional - Explicit source-side port to bind to
 *     NetworkByteOrder = 1 // Optional - Enforce On-the-wire network byte ordering
 * \b endif
 *     Signals = {
 *         Header = { //Optional. If present (i.e. if there is a signal named header) the sent packet header will be copied into this field (note that it can be later decomposed by GAMs using Ranges). It shall be the first signal.
 *             Type = uint8
 *             NumberOfElements = 48 //Must match the SDN header size
 *         }
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
 * </pre>
 *
 * The DataSource relies on a MemoryMap(Synchronised)OutputBroker to interface to GAM signals.
 * The DataSource does not allocate memory, rather maps directly the signals to the SDN message
 * payload directly.
 * \b ifdef FEATURE_10840
 * The DataSource has additional optional parameters to override the default publisher-side
 * source port and/or opt for publishing messages on the wire using network byte order. The valid
 * range for source port is 0 || [1024, 65535]. The default on-the-wire byte order is the host
 * native ordering (MemoryMapOutputBroker performs memory copies pf the signals directly to the
 * message payload) and may be overridden to use network byte order, in which case the DataSource
 * performs the necessary byte swaps before publication.
 * \b endif
 *
 * @warning The DataSource requires that one and only one signal be identified as synchronisation
 * point (i.e. only one signal must set Trigger = 1).
 *
 * @warning the DataSource does not support signal samples batching.
 *
 * @warning the data payload over the network is structured in the same way as the signal definition
 * order. Interoperability between distributed participants require strict configuration control
 * of the payload definition.
 */
class SDNPublisher: public DataSourceI {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @post
     *   topic = NULL_PTR &&
     *   publisher = NULL_PTR &&
     *   synchronising = false
     */
SDNPublisher    ();

    /**
     * @brief Destructor. Releases resources.
     * @post
     *   topic = NULL_PTR &&
     *   publisher = NULL_PTR
     */
    virtual ~SDNPublisher();

    /**
     * @brief Verifies and parses instance parameters.
     * @param[in] data configuration in the form:
     * <pre>
     * +Publisher = {
     *     Class = SDNPublisher
     *     Topic = name // The name is used to establish many-to-many communication channels
     *     Interface = name // The network interface name to be used, e.g. eth0
     *     Address = address:port // Optional - Explicit destination address
     * }
     * </pre>
     * @details The configuration parameters are subject to the following criteria:
     * The topic "name" is mandatory and can be any string. The "name" is used to associate the
     * publisher to an address and must be identical on all participants. The mapping between topic
     * "name" and address is done within the scope of the SDN core library and guaranteed to match
     * between all participants using the same topic "name". Alternatively, the destination address
     * can be explicitly defined using a topic "name" of the form 'sdn://address:port/name';
     * which is purposeful to establish e.g. a unicast connection.
     * The interface "name" is mandatory and verified to correspond to a valid named
     * interface on the host, e.g. eth0.
     * @return true if the criteria above is met.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief See DataSourceI::SetConfiguredDatabase.
     * @details The DataSource does not parse the attribute; rather, the method is overloaded to
     * perform signal validity checks outside the scope of the later SDNPublisher::AllocateMemory
     * which can be ensured that it is called with signal list previously validated.
     * @return false in case no signals are being configured, or in case there is no or more than 
     * one signal declared as synchronisation point.
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
     * @details The implementation is associated to MemoryMapOutputBroker or
     * MemoryMapSynchronisedOutputBroker depending on the signal properties.
     * @return MemoryMapOutputBroker or MemoryMapSynchronisedOutputBroker for OutputSignals and MemoryMapInputBroker if the signal name is Header and the direction is InputSignals.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details Adds a MemoryMapInputBroker if the SDN Header is to be read, NOOP otherwise.
     * @return true if the SDN Header is to be read, false otherwise.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @details The implementation provides MemoryMapOutputBroker instances
     * for non-synchronising GAMs, it provides both one MemoryMapOutputBroker
     * and one MemoryMapSynchronisedOutputBroker for the synchronising GAM.
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
     * message payload has been previously completely modified by the OutputBroker instances.
     * @warning It is for the application-specific configuration to ensure and organise ordering of the
     * GAMs so as to ensure proper payload update prior to publication, e.g. the synchronising
     * GAM is scheduled after all the non-synchronising GAMs contributing signals to the 
     * DataSource.
     * @return true or false in case of error within the SDN core library.
     */
    virtual bool Synchronise();

private:

    /**
     * Interface name configuration parameter
     */
    StreamString ifaceName;

    /**
     * Topic name configuration parameter
     */
    StreamString topicName;

    /**
     * Destination address configuration parameter
     */
    StreamString destAddr;

    /**
     * Number of input signals
     */
    uint32 nOfSignals;

    /**
     * Number of output synchronising signals
     */
    uint32 nOfTriggers;

    /**
     * The topic reference
     */
    sdn::Topic *topic;

    /**
     * The sdn::Publisher reference
     */
    sdn::Publisher *publisher;

    /**
     * The sdn::Publisher optional source port
     */
    uint32 sourcePort;

    /**
     * The sdn::Publisher optional wire byte order
     */
    bool networkByteOrder;

    /**
     * Accelerator for signal types (to be used when translating to different endianness)
     */
    uint16 *payloadNumberOfBits;

    /**
     * Accelerator for signal number of Elements (to be used when translating to different endianness)
     */
    uint32 *payloadNumberOfElements;

    /**
     * Accelerator for signal memory address (to be used when translating to different endianness)
     */
    void **payloadAddresses;

    /**
     * Read the SDN header as a signal?
    */
    bool sdnHeaderAsSignal;

    /**
     * How many topics in the socket buffer
     */
    uint32 socketBufferCapacity;
};

}
/* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SDNPUBLISHER_H_ */

