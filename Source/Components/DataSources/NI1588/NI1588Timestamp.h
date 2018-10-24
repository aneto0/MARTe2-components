/**
 * @file NI1588Timestamp.h
 * @brief Header file for class NI1588Timestamp
 * @date 14/05/2018
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class NI1588Timestamp
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI1588TIMESTAMP_H_
#define NI1588TIMESTAMP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <poll.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CircularBufferThreadInputDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Circular buffer time stamp acquisition using the NI-1588 PCI-Express board.
 *
 * @details This data source allows to time stamp events on each input terminal of the NI-1588 board: TerminalPFI0, TerminalPFI1, TerminalPFI2.
 * Optionally, this data source also provides the following
 * signals to check the type of event triggered on each of the three input terminals: EventPFI0, EventPFI1, EventPFI2 (0=RISING EVENT | 1=FALLING_EVENT)
 *
 * @details The optional ErrorCheck signal, coming from the implementation of CircularBufferThreadInputDataSource
 * is populated as follows:
 *    ErrorCheck |= 0x4  //timeout expired waiting for a event\n
 *    ErrorCheck |= 0x8  //more than one event has occurred at the same time
 *
 * @details The configuration syntax is (names and signal quantities are only given as an example and * means optional):
 *
 * <pre>
 * +NI1588Timestamp_0 = {
 *     Class = NI1588Timestamp
 *     NumberOfBuffers = N //The number of buffers of the circular buffer (@see CircularBufferThreadInputDataSource).
 *     *NiSyncDeviceNumber = The logical device number of the board: default is 0.
 *     *CpuMask = 0x1 //The CPUs where the internal thread is allowed to run: default is 0xFFFF.
 *     *ReceiverThreadPriority = 0-31 //The priority of the internal thread: default is 31.
 *     *PollMsecTimeout = -1 //The timeout in milliseconds to wait for an event. A negative number means infinite timeout, default is -1.
 *     Signals = {
 *         *TerminalPFI0 = { //The name of the terminal. Possible values are: TerminalPFI0, TerminalPFI1 and TerminalPFI2. It will contain the time-stamp read in nanoseconds.
 *             Type = uint64 //Must be uint64
 *             *CaptureEvent = NISYNC_EDGE_RISING //The type of event to capture. Possible values are NISYNC_EDGE_RISING, NISYNC_EDGE_FALLING or NISYNC_EDGE_ANY. Default is NISYNC_EDGE_ANY.
 *             *Samples = 1 //Note that Samples > 1 is supported. @see CircularBufferThreadInputDataSource
 *         }
 *         *TerminalPFI1 = {
 *             Type = uint64
 *             *CaptureEvent = NISYNC_EDGE_FALLING
 *         }
 *         *TerminalPFI2 = {
 *             Type = uint64
 *             *CaptureEvent = NISYNC_EDGE_ANY
 *         }
 *         *EventPFI0 = { //The type of event that was captured for the terminal PFI0. Possible values are EventPFI0, EventPFI1, EventPFI2. The read value will be either 0=RISING EVENT or 1=FALLING_EVENT
 *             Type = uint8 //Must be uint8
 *         }
 *         *EventPFI1 = {
 *             Type = uint8
 *         }
 *         *EventPFI2 = {
 *             Type = uint8
 *         }
 *         *InternalTimeStamp = { //The time-stamp as measured by the
 *             Type = uint64
 *             NumberOfDimensions = 1
 *             NumberOfElements = N //the number of signals belonging to this dataSource (not counting InternalTimeStamp and ErrorCheck)
 *         }
 *         *ErrorCheck = {
 *             Type = uint32
 *             NumberOfDimensions = 1
 *             NumberOfElements = N //the number of signals belonging to this dataSource (not counting InternalTimeStamp and ErrorCheck)
 *         }
 *     }
 * }

 * </pre>
 */
class NI1588Timestamp: public CircularBufferThreadInputDataSource {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     */
    NI1588Timestamp();

    /**
     * @brief Destructor
     * @details Frees the internal state variables memory
     */
    virtual ~NI1588Timestamp();

    /**
     * @see CircularBufferThreadInputDataSource::Initialise
     * @details Calls the CircularBufferThreadInputDataSource::Initialise and reads the NiSyncDeviceNumber and the PollMsecTimeout optional parameters.
     * @return true if CircularBufferThreadInputDataSource::Initialise returns true.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see CircularBufferThreadInputDataSource::SetConfiguredDatabase
     * @details Checks that the signals defined in the configuration respect the requirements defined in the class description. In particular:
     *   TerminalPFI0: Type=uint64, NumberOfElements=1
     *   TerminalPFI1: Type=uint64, NumberOfElements=1
     *   TerminalPFI2: Type=uint64, NumberOfElements=1
     *   EventPFI0: Type=uint8, NumberOfElements=1
     *   EventPFI1: Type=uint8, NumberOfElements=1
     *   EventPFI2: Type=uint8, NumberOfElements=1
     *  If none of the signals above is defined this method returns an error. No more signals than the ones above (and the optional signals supported by
     *  CircularBufferThreadInputDataSource) must be defined.
     *
     *  @return true if the requirements above are met and if CircularBufferThreadInputDataSource::SetConfiguredDatabase returns true.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @see CircularBufferThreadInputDataSource::PrepareNextState
     * @details Consumes all the data stored in the internal board FIFO to be ready to read fresh ones during the real time execution.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief Performs the read of a time-stamp from the NI-1588 board.
     *
     * @details Waits that at least one event occurs on the terminals pool defined. It reads the last time-stamp value occurred on each of the terminals and
     * it adds it to the internal buffer of the relative signal.
     *
     * @details The bits of the uint32 signal ErrorCheck, if existent, are set as follows:
     *    bit 2: timeout expired while waiting an event on terminals\n
     *    bit 3: more than one event occurred on the terminal between two calls of this method. Only the last one is saved in the buffer (get latest policy)\n
     * In the first case, when timeout occurred, the function returns setting \a sizeToRead equal to zero, and no data will be put
     * in the circular buffer by the internal thread.
     *
     * @return true if the time-stamp value could be read from any of the terminals.
     */
    virtual bool DriverRead(char8 * const bufferToFill,
                            uint32 &sizeToRead,
                            const uint32 signalIdx);

protected:

    /**
     * How much time (in milliseconds) to wait for a new
     * event.
     */
    int32 pollTimeout;

    /**
     * The terminals file descriptors
     */
    struct pollfd pollfd[3];

    /**
     * The event occurred: 0=RISING, 1=FALLING
     */
    uint8 capturedEventType[3];

    /**
     * Used to store the six supported signal in the right order.
     */
    uint32 signalIndexes[6];

    /**
     * Used to store the time stamp of the three input terminals
     */
    uint64 signalTs[3];

    /**
     * The device number
     */
    int32 devNum;

    /**
     * A mask used to check if all the signals have been stored in the internal buffer before a new
     * read from the device.
     */
    uint8 arrivedMask;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /*NI1588TIMESTAMP_H_ */

