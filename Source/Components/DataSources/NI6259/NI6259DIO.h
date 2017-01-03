/**
 * @file NI6259DIO.h
 * @brief Header file for class NI6259DIO
 * @date 03/01/2017
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

 * @details This header file contains the declaration of the class NI6259DIO
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6259_NI6259DIO_H_
#define NI6259_NI6259DIO_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pxi-6259-lib.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EventSem.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief A DataSource which provides an input interface to the NI6259 boards.
 * @details The DataSource shall always be ...
 *
 * The configuration syntax is (names are only given as an example):
 * +NI6259_0 = {
 *     Class = NI6259::NI6259DIO
 *     TODO
 *     Signals = {
 *         TODO
 *     }
 * }
 */

namespace MARTe {
/**
 * @brief TODO
 */
class NI6259DIO: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief TODO
     */
NI6259DIO    ();

    /**
     * @brief TODO
     */
    virtual ~NI6259DIO();

    /**
     * @brief See DataSourceI::AllocateMemory.
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @return 1.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers.
     * @details Only InputSignals are supported.
     * @return MemoryMapSynchronisedInputBroker if frequency > 0, MemoryMapInputBroker otherwise.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief See DataSourceI::GetInputBrokers.
     * @details If the functionName is the one synchronising it adds a MemoryMapSynchronisedInputBroker instance to
     *  the inputBrokers, otherwise it adds a MemoryMapInputBroker instance to the inputBrokers.
     * @param[out] inputBrokers where the BrokerI instances have to be added to.
     * @param[in] functionName name of the function being queried.
     * @param[in] gamMemPtr the GAM memory where the signals will be read from.
     * @return true if the inputBrokers can be successfully configured.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief See DataSourceI::GetOutputBrokers.
     * @return false.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief TODO
     * @details See StatefulI::PrepareNextState. Starts the EmbeddedThread (if it was not already started) and loops
     * on the ExecuteMethod.
     * @return true if the EmbeddedThread can be successfully started.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief TODO
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Verifies that 32, and only 32, signal are set with the correct type.
     * @details TODO
     * @return TODO.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @details TODO
     */
    virtual bool Synchronise();
private:

    /**
     * The board identifier
     */
    uint32 boardId;

    /**
     * The board device name
     */
    StreamString deviceName;

    /**
     * The board file descriptor
     */
    int32 boardFileDescriptor;

    /**
     * The DIOs that are enabled
     */
    bool portEnabled[PXI6259_NUMBER_OF_PORTS];

    /**
     * The i/o mask of each port
     */
    uint32 portMask[PXI6259_NUMBER_OF_PORTS];

    /**
     * TODO
     */
    uint32 portValues[PXI6259_NUMBER_OF_PORTS];

    /**
     * TODO
     */
    uint32 numberOfPortsEnabled;

    /**
     * The ports file descriptors
     */
    int32 portFileDescriptors[PXI6259_NUMBER_OF_PORTS];

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6259_NI6259DIO_H_ */

