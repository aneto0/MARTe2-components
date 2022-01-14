/**
 * @file UARTDataSource.h
 * @brief Header file for class UARTDataSource
 * @date 25/06/2018
 * @author Marta Baldris
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

 * @details This header file contains the declaration of the class UARTDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UARTDATASOURCE_H_
#define UARTDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BasicUART.h"
#include "EmbeddedServiceMethodBinderT.h"
/*lint -efile(766,UARTDataSource.h) EventSem.h is used in this file*/
#include "EventSem.h"
#include "MemoryDataSourceI.h"
#include "MessageI.h"
#include "SingleThreadService.h"
#include "StructuredDataI.h"
#include "StreamString.h"
#include "TimestampProvider.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A DataSource which implements a circular buffer implementation (using a MemoryMapMultiBufferInputBroker) to a CRIOUARTSerial.
 *
 * @details This DataSource produces two signals. The first shall be of type uint8 and will indicate if the data of the second signal is valid (e.g. it
 *  might have returned with a timeout). The second signal contains the data read from the serial port.
 *
 * The frequency at which data arrives will vary on the number of events received in a given period.
 *
 * An asynchronous thread reads from the UART and makes the data available to the RealTimeThread.
 *
 * The configuration syntax is (names are only given as an example, but the size and the exact number of signals shall be respected):
 *
 * <pre>
 *   +CRIOUART = {
 *     Class = UARTDataSource
 *     NumberOfBuffers = 500 //Number of buffer of the circular buffer
 *     PortName = "/dev/ttyUSB0" //Name of the UART port
 *     BaudRate = 115200 //BAUD UART rate
 *     Timeout = 200000 //Maximum time to wait for data
 *     CPUMask = 8 //Affinity of the CPU of where to read data from
 *     Signals = {
 *       DataOK = { //Compulsory - is the data valid?
 *         Type = uint8
 *         NumberOfElements = 1
 *       }
 *       Time = {
 *         Type = uint64 //Compulsory - time-stamp in nano-seconds at which the data was read.
 *         NumberOfElements = 1
 *       }
 *       Packet = { //Actual data to read
 *         Type = uint8
 *         NumberOfElements = 15
 *       }
 *     }
 *     +TimeProvider = { //Optional. If not provided HighResolutionTimestampProvider will be used.
 *       Class = TimestampProvider //A class that provides the absolute time in ns. It shall inherit from TimestampProvider.
 *     }
 *   }
 */
class UARTDataSource: public MemoryDataSourceI, public MessageI, public EmbeddedServiceMethodBinderT<UARTDataSource> {
public:

    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    UARTDataSource ();

    /**
     * @brief Destructor. Stops the Embedded thread which reads from the CRIOUARTSerial.
     */
    virtual ~UARTDataSource();

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
    ErrorManagement::ErrorType CRIOThreadCallback(ExecutionInfo &info);

    /**
     * @return "MemoryMapMultiBufferInputBroker"
     */
    virtual const char8* GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @return true.
     */
    virtual bool Synchronise();

    /**
     * @brief Start the asynchronous thread (if it was not started already).
     * @param[in] currentStateName not used.
     * @param[in] nextStateName not used.
     * @return true if the thread was successfully started.
     */
    virtual bool PrepareNextState(const char8 *const currentStateName,
                                  const char8 *const nextStateName);

    ErrorManagement::ErrorType StopAcquisition();

private:

    /**
     * Asynchronous thread executor.
     */
    SingleThreadService executor;

    /**
     * Last index read by the RTThread.
     */
    uint32 lastReadIdx;

    /**
     * Last index written by the asynchronous thread.
     */
    uint32 lastWrittenIdx;

    /**
     * Number of bytes to read from the UART.
     */
    uint32 packetByteSize;

    /**
     * RealTimeThread waits on the PrepareInputOffsets for data to be available.
     */
    EventSem eventSem;

    /**
     * Fast locking semaphore to reset the eventSem.
     */
    FastPollingMutexSem muxSem;

    /**
     * Marks that a given signal has been written.
     */
    bool *writeMark;

    /**
     * Synchronise the GetInputOffset with the TerminateInputCopy
     */
    bool lastDataCopyHadTimeout;

    /**
     * The UART interface.
     */
    BasicUART serial;

    /**
     * Timeout to wait for data to be available.
     */
    uint32 serialTimeout;

    /**
     * @brief Reference to the time provider plugin
     */
    ReferenceT<TimestampProvider> timeProvider;

    /**
     * Timeout to synchronise
     */
    uint32 timeoutToSynchronise;

    uint32 timeout;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UARTDATASOURCE_H_ */

