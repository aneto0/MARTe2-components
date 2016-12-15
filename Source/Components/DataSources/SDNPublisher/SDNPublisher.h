/**
 * @file SDNPublisher.h
 * @brief Header file for class SDNPublisher
 * @date 24/10/2016
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class SDNPublisher
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
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
 * The SDN core library uses topic name as key to establish matching communication channels
 * across all participants. Alternatively, the destination address required by the underlying
 * transport may be defined explicitly, in which case matching topic names would only be used
 * to support fault investigation purposes.

 * The configuration syntax is (signals names are only given as an example):
 * +Publisher = {
 *     Class = SDNPublisher
 *     Topic = <name> // The name is used to establish many-to-many communication channels
 *     Interface = <name> // The network interface name to be used
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
 */
class SDNPublisher : public DataSourceI {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     * @post
     *   Counter = 0
     *   Time = 0
     */
    SDNPublisher();

    /**
     * @brief Destructor. Stops the EmbeddedThread.
     */
    virtual ~SDNPublisher();

    /**
     * @brief .
     * @return .
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief See DataSourceI::AllocateMemory.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI& data);
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetSignalMemoryBuffers.
     * @return 1.
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                                       const uint32 bufferIdx,
                                       void *&signalAddress);

    /**
     * @brief .
     * @return .
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @brief .
     * @return .
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr);

    /**
     * @brief .
     * @return .
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);

    /**
     * @brief .
     * @return .
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief .
     * @return .
     */
    virtual bool Synchronise();

private:
    
    StreamString ifaceName; // Configuration parameter
    StreamString topicName; // Configuration parameter
    StreamString destAddr;  // Configuration parameter (optional)

    uint32 topicSize;  // The topic size computed from declared/connected input signals
    sdn::Topic *topic; // The topic reference
    sdn::Publisher *publisher; // The sdn::Publisher reference

};

} /* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SDNPUBLISHER_H_ */

