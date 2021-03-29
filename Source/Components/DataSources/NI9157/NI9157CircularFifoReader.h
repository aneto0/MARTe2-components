/**
 * @file NI9157CircularFifoReader.h
 * @brief Header file for class NI9157CircularFifoReader.
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 * @details This header file contains the declaration of the class 
 * NI9157CircularFifoReader with all of its public, protected and private
 * members. It may also include definitions for inline methods which need to
 * be visible to the compiler.
 */

#ifndef NI9157CIRCULARFIFOREADER_H_
#define NI9157CIRCULARFIFOREADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "CircularBufferThreadInputDataSource.h"
#include "CreateNI9157DeviceOperatorI.h"
#include "EventSem.h"
#include "MemoryOperationsHelper.h"
#include "MessageI.h"
#include "NI9157Device.h"
#include "NI9157DeviceOperatorDatabase.h"
#include "NI9157DeviceOperatorT.h"
#include "ObjectRegistryDatabase.h"
#include "Optim/NI9157MemoryOperationsHelper.h"
#include "RegisteredMethodsMessageFilter.h"
#include "SampleChecker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Circular buffer acquisition from NI-9157 FIFO.
 * @details This data source can hold only one signal (over the special InternalTimeStamp and ErrorCheck signals). Depending on the signal type, a NI9157DeviceOperator
 * object is created to manage the typed read operation from the NI-9157 FIFO. The name of the variable that identifies the FIFO must be defined by the user in the configuration
 * file.
 * @details An internal thread is started in the PrepareNextState method and the object starts acquiring from the FIFO and putting the received packets in a
 * circular buffer (see CircularBufferThreadInputDataSource). The Synchronise method, allow the Brokers to get the address of the last written packet in the FIFO.
 * @details If the signal vector parameter PacketMemberSizes is specified, a interleaved to flat operation (see InterleavedXFlat) is performed before putting the packet in the
 * circular buffer.
 * @details Follows a configuration example:
 * <pre>
 * +Drv1 = {
 *     Class = NI9157CircularFifoReader
 *     NumberOfBuffers = 80  //see CircularBufferThreadInputDataSource
 *     CpuMask = 2 //see CircularBufferThreadInputDataSource
 *     sleepInMutexSec = 1e-9 //see CircularBufferThreadInputDataSource
 *     NI9157DevicePath = NiDevice //the absolute path of the NI9157Device block in the configuration database
 *     CheckFrame = 1 //if the first element is the packet counter (default 0)
 *     NumberOfPacketsInFIFO = 20 //the number of packets in the host-side FIFO (default 10)
 *     FifoName = "NiFpga_TestGTD0001_TargetToHostFifoU64_FIFO" //the name of the FIFO variable
 *     NumOfFrameForSync = 2 //the number of packets to be used if the synchronisation is lost with the device (checking the packet counter). Default 2.
 *     RunNi = 1 //if the NI9157Device has to be started by this data source
 *     Timeout = 0xFFFFFFFFu //The miliseconds timeout used by the NiFifoRead method calls within the DriverRead method.
 *     NonBlockSleepT = 0.F //if 0.F, no NiFifoReader call is repeated within the same DriverRead method call. if >0.F, corresponds to the sleep time in seconds between NiReadFifo calls within DriverRead.
 *     CounterStep = 2000 //the gap between two consecutive packet counters (default 1)
 *     CheckCounterAfterNSteps = 2000 //when the counter must be checked. It must be multiple of CounterStep (default is equal to CounterStep).
 *     FirstPacketCounter = 1 //the first packet counter that should be acquired from the device.
 *     AcquireFromCounter = 1 //The first packet counter to be acquired. The data source wait this packet before starting to put packets in the circular buffer (default equal to FirstPacketCounter).
 *     +Signals = {
 *        Signal1 = {
 *           Type = uint32
 *           NumberOfElements = 500
 *           PacketMemberSizes = {4, 4, 4, 4}"
 *     }
 * }
 * </pre>
 */
class NI9157CircularFifoReader: public CircularBufferThreadInputDataSource, public MessageI  {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     * @post
     *   checkFrame = 0u;\n
     *   nFrameForSync = 0u;\n
     *   sampleByteSize = 0u;\n
     *   niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);\n
     *   fifoDev = 0u;\n
     *   totalReadSize = 0u;\n
     *   middleBuffer = NULL_PTR(uint8 *);\n
     *   runNi = 0u;\n
     *   acqTimeout = 0xFFFFFFFFu;\n
     *   nonBlockSleepT = 0.F;\n
     *   packetCounter = 1u;\n
     *   acquireFromCounter = 0u;\n
     *   nextPacketCheck = 1u;\n
     *   checkCounterAfterNSteps = 0u;\n
     *   counterStep = 1u;\n
     *   numberOfPacketsInFIFO = 10u;\n
     */
    NI9157CircularFifoReader();

    /**
     * @brief Destructor
     * @details Frees the \a niDeviceOperator pointer and the \a middleBuffer memory.
     */
    virtual ~NI9157CircularFifoReader();

    /**
     * @see CircularBufferThreadInputDataSource::Initialise
     * @details Follows a list of the parameters:
     *     NI9157DevicePath: the absolute path of the NI9157Device block in the configuration database\n
     *     NumberOfPacketsInFIFO: the number of packets in the host-side FIFO (default 10)\n
     *     FifoName: the name of the FIFO variable that can be found in the exported Labview header file.\n
     *     RunNi: if the NI9157Device has to be started by this data source\n
     *     CheckFrame: if the first element is the packet counter (default 0)\n
     *     if (CheckFrame == 1) then the following parameters mean:\n
     *       NumOfFrameForSync: the number of packets to be used if the synchronisation is lost with the device (checking the packet counter). Default 2.\n
     *       CounterStep: the difference between two consecutive packet counters (default 1)\n
     *       CheckCounterAfterNSteps: when the counter must be checked. It must be multiple of CounterStep (by default is equal to CounterStep).\n
     *       FirstPacketCounter: the first packet counter that should be acquired from the device.\n
     *       AcquireFromCounter> the first packet counter to be acquired. The data source wait this packet before starting to put packets in the circular buffer\n
     *       (by default is equal to FirstPacketCounter).
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Creates the \a niDeviceOperator and the \a middleBuffer memory.
     * @see CircularBufferThreadInputDataSource::SetConfiguredDatabase
     * @return false if the type of the signal is unsupported (not an integer or boolean type) causing the failure of the \a niDeviceOperator creation.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Configures the number of packets in the host-side FIFO and launches the internal thread.
     * @see CircularBufferThreadInputDataSource::PrepareNextState
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief The Synchronise method.
     * @details Calls the CircularBufferThreadInputDataSource Synchronise method.
     * @see CircularBufferThreadInputDataSource::Synchronise
     */
    virtual bool Synchronise();

    /**
     * @brief Acquires a packet from the NI-9157 device and puts it in the circular buffer.
     * @see CircularBufferThreadInputDataSource::DriverRead
     * @details If CheckFrame==1, the method checks is the first element of the packet is coherent. In particular:
     *   - the counter of the N-th packet should be (FirstPacketCounter+N*CounterStep).\n
     *   - the counter is checked after (CheckCounterAfterNSteps/CounterStep)s acquisitions.\n
     * If the packet counter does not respect these rules than \a NumOfFrameForSync packets are acquired in \a middleBuffer
     * and the method searches for consecutive packet numbers in these packets. If succeeds, the FIFO data is shifted in order to be
     * synchronised again on the packet counter as first element.
     * @details If the vector parameter PacketMemberSizes is present within the configuration of the signal in the data source block, then
     * the method MemoryOperationsHelper::InterleavedToFlat is called in order to de-interleave data before putting it in the circular buffer.
     */
    virtual bool DriverRead(char8 * const bufferToFill,
                            uint32 &sizeToRead,
                            const uint32 signalIdx);

    /**
     * @brief The StopAcquisition method.
     * @details If the thread is running, this thread attempts to stop it.
     */
    virtual ErrorManagement::ErrorType StopAcquisition();

    /**
     * @brief The StartAcquisition method. 
     * @details Configures the FIFO and strats the thread.
     */
    virtual ErrorManagement::ErrorType StartAcquisition();

protected:

    /**
     * If the packet counter should be checked
     */
    uint8 checkFrame;

    /**
     * The FIFO descriptor
     */
    uint32 fifoDev;

    /**
     * The number of packets to be used in case of lost of synchronisation
     */
    uint32 nFrameForSync;

    /**
     * The size of a FIFO element
     */
    uint8 sampleByteSize;

    /**
     * The reference to the NI9157 device
     */
    ReferenceT<NI9157Device> niDeviceBoard;

    /**
     * The operator to interface with the NI-9157 device
     */
    NI9157DeviceOperatorTI *niDeviceOperator;

    /**
     * The total size to be acquired per packet
     */
    uint32 totalReadSize;

    /**
     * A buffer used to acquire data and for eventual re-synchronisation
     */
    uint8* middleBuffer;

    /**
     * If the NI-9157 device must be started by this data source
     */
    uint8 runNi;

    /**
     * TODO - WAS MISSING.
     */
    uint8 runNiOriginal;

    /**
     * The number of packets in the host-side FIFO.
     */
    uint32 numberOfPacketsInFIFO;

    /**
     * The name of the fifo to be used in the MXI interface.
     */
    StreamString fifoName;

    /**
     * The samples frame checker.
     */
    ReferenceT<SampleChecker> checker;

    /**
     * The timeout used in the NiReadFifo calls of DriverRead in milliseconds.
     */
    uint32 acqTimeout;

    /**
     * The semaphore used in the Synchronise method.
     */
    EventSem eventSem;

    /**
     * The sleep time between NiReadFifo calls within DriverRead in seconds.
     */
    float32 nonBlockSleepT;
    
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157CIRCULARFIFOREADER_H_ */
