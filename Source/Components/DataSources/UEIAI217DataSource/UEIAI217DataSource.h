/**
 * @file UEIAI217DataSource.h
 * @brief Header file for class UEIAI217DataSource
 * @date 07/02/2023
 * @author Xavier Ruche
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

 * @details This header file contains the declaration of the class UEIAI217DataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIAI217DataSource_H_
#define UEIAI217DataSource_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EmbeddedServiceMethodBinderT.h"
#include "EventSem.h"
#include "MemoryDataSourceI.h"
#include "MessageI.h"
#include "SingleThreadService.h"
#include "StructuredDataI.h"
#include "StreamString.h"
#include "PDNA.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#define POINT_BY_POINT  0u
#define RTDMAP          1u
#define RTVMAP          2u
#define ADMAP           3u
#define AVMAP           4u


namespace MARTe {

/**
 * @brief TODO
 * Modes:
 * * Point-to-Point
 * * RtDMap
 * * RtVMap
 * * ADMap
 * * AVMap
 * @details 
 *
 * <pre>
 *   +UEIAI217 = {
 *     Class = UEIAI217DataSource
 *     Ip = {127.0.0.1}
 *     Signals = {
 *       OutputData = {
 *         Type = uint32
 *         NumberOfElements = 10
 *       }
 *     }
 *   }
 */
class UEIAI217DataSource: public MemoryDataSourceI, public MessageI, public EmbeddedServiceMethodBinderT<UEIAI217DataSource> {
public:

    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    UEIAI217DataSource ();

    /**
     * @brief Destructor. Stops the Embedded thread which reads from the CRIOUARTSerial.
     */
    virtual ~UEIAI217DataSource();

    /**
     * @brief Verifies that the configuration described above is correctly set.
     * @return true if the configuration respects all the rules described in the class description.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see Checks that the signal properties are as described in the class description.
     * @return true if the signal properties respect all the rules described in the class description.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI &data);

    /**
     * @see DataSourceI::GetInputOffset.
     */
    virtual bool GetInputOffset(const uint32 signalIdx,
                                const uint32 numberOfSamples,
                                uint32 &offset);

    /**
     * @see DataSourceI::PrepareInputOffsets.
     */
    virtual void PrepareInputOffsets();

    /**
     * @see DataSourceI::TerminateInputCopy
     */
    virtual bool TerminateInputCopy(const uint32 signalIdx,
                                    const uint32 offset,
                                    const uint32 numberOfSamples);

    /**
     * @brief Callback function for the thread which asynchronously reads from the UART.
     */
    ErrorManagement::ErrorType UEIAI217ThreadCallback(ExecutionInfo &info);

    /**
     * @return "MemoryMapMultiBufferInputBroker"
     */
    virtual const char8* GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @return true.
     */
    virtual bool Synchronise();

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);
    bool GetMapAddr();
    bool PollForNextPacket();

private:

    /**
     * Asynchronous thread executor.
     */
    SingleThreadService executor;

    /**
     * Variable to store the selected DAQ mode
     */
    uint8 DAQmode;

    /**
     * Variable to store the selected ip of UEIAI217
     */
    StreamString ip;

    /**
     * Variable to store the selected ip of UEIAI217
     */
    uint32* input_map;
    int32 mapid;
    int32 hd;
    uint32 poll_sleep_period;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIAI217DataSource_H_ */
