/**
 * @file MemoryGate.h
 * @brief Header file for class MemoryGate
 * @date 23/07/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class MemoryGate
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYGATE_H_
#define MEMORYGATE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief Allows asynchronous communication between any MARTe components.
 *
 * @details The user can specify up to 64 buffers of memory by configuration and, optionally, the buffer memory size
 * that must be shared between all the components linked to this object. The function SetMemorySize() should be called
 * from all the linked components to agree the memory size and to instantiate it if it is not declared in the configuration.
 *
 * @details The readers call the function MemoryRead() that will fill the input buffer with the value of the last written available buffer.
 * If all the buffers are busy by writers, the MemoryRead() returns false. Parallel reads on the same buffer are allowed.
 *
 * @details The writers call the function MemoryWrite() that will flush the input buffer to the oldest written available buffer. If all
 * the buffers are busy by readers or other writers, the MemoryWrite() returns false.
 *
 * The configuration syntax is (names are only given as an example):
 *
 * <pre>
 *    +SharedMem = {
 *        Class = MemoryGate
 *        NumberOfBuffers = 10 //The number of buffers that should be in [1-64]. Default = 2
 *        ResetMSecTimeout = 10 //The timeout in milliseconds used by the mutex to lock all the buffers to change the write operations counters in case of write overflow). Default is 0xFFFFFFFF.\n
 *        MemorySize = 100 //The size of each buffer memory. If this parameter is not set or if it is equal to zero and the buffer size will be set by the first component that calls SetMemorySize().
 *    }
 * </pre>
 */
class MemoryGate: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    MemoryGate();

    /**
     * @brief Destructor
     */
    virtual ~MemoryGate();

    /**
     * @see ReferenceContainer::Initialise()
     * @details The user can specify the following configuration parameters:\n
     *   - NumberOfBuffers = N (the number of buffers that should be in [1-64]. Default = 2)\n
     *   - ResetMSecTimeout = N (the timeout in milliseconds used by the mutex to lock all the buffers to change the
     *     write operations counters in case of overflow). Default is 0xFFFFFFFF.\n
     *   - MemorySize = N (the size of each buffer memory. If this parameter is not set or if it is equal to zero,
     *     the buffer size will be set by the first component that calls SetMemorySize()).
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Checks if \a size in input is equal to the previous declared buffer size.
     * @details If by configuration the user has not declared the "MemorySize" parameter, or if it has
     * been declared equal to zero, then the first call to this function will set the buffer size and
     * allocates the memory. Successive calls will check if \a size in input is equal to the size of the
     * allocated buffer memory.
     * @param[in] size is the size of the buffer memory.
     * @return true if \a size in input is equal to the previous declared buffer size, false otherwise.
     */
    virtual bool SetMemorySize(uint32 size);

    /**
     * @brief The function called by readers.
     * @details Browses the buffers to peek the last written available and copy its value to \a
     * bufferToFill. If no buffer is available, it returns false.
     * @param[in] bufferToFill is the buffer to be filled with read data.
     * @return true if at least one buffer is available, false otherwise.
     */
    virtual bool MemoryRead(uint8 * const bufferToFill);


    /**
     * @brief The function called by writers.
     * @details Browses the buffers to peek the oldest written available one and copy on it the value of \a
     * bufferToFlush. If no buffer is available, it returns false.
     * @param[in] bufferToFlush is the buffer contains the data to be written.
     * @return true if at least one buffer is available, false otherwise.
     */
    virtual bool MemoryWrite(const uint8 * const bufferToFlush);


protected:

    /**
     * The total internal memory.
     */
    uint8 *mem;

    /**
     * The size of a buffer
     */
    uint32 memSize;

    /**
     * The number of buffers
     */
    uint32 numberOfBuffers;

    /**
     * Denoted the current number of readers for each buffer.
     */
    volatile int32 *spinlocksRead;

    /**
     * A semaphore to lock the buffer that is going to be written.
     */
    FastPollingMutexSem *spinlocksWrite;

    /**
     * Used to denote the oldest and newest written buffers
     */
    uint32 *whatIsNewestCounter;

    /**
     * The counter incremented for each signal at the end of the write operation
     */
    uint32 whatIsNewestGlobCounter;

    /**
     * Stores the signal offset in case of more write operations.
     */
    uint32 offsetStore;

    /**
     * The timeout in millisecs to lock the other \a spinlocksWrite semaphores
     * in case of overflow of \a whatIsNewestCounter (to avoid a priority inversion)
     */
    TimeoutType resetTimeout;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYGATE_H_ */

