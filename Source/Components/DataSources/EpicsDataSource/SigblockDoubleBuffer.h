/**
 * @file EpicsDataSource.h
 * @brief Header file for class EpicsDataSource
 * @date 01/12/2016
 * @author Ivan Herrero Molina
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

 * @details This header file contains the declaration of the class EpicsDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIGBLOCKDOUBLEBUFFER_H_
#define SIGBLOCKDOUBLEBUFFER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Sigblock.h"
#include "Types.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace SDA {

/**
 * @brief Double buffer of sigblocks
 *
 * @description This class implements a double buffer for interchange
 * sigblocks, meant for SC/SP (single consumer and single producer)
 * running on different threads or processes through shared memory.
 *
 * Features of the double buffer:
 * * It is wait-free on reading and writing sigblocks.
 * * It manages the concepts of front buffer and back buffer.
 * * If there is not a fresh sigblock for reading, i.e. the front
 * buffer has no data, it does not block and returns error, so the
 * consumer can retry later.
 * * It always writes to the back buffer and tries to swap front and
 * back buffers, but if front buffer is being accessed by the consumer, then
 * it does not block and returns TODO ¿error?. The next time the producer writes,
 * it will overwrite the back buffer and will try to swap again. Eventually,
 * when consumer had finished reading the front buffer, the next write will
 * swap the buffers, and the front buffer will have a fresh sigblock. In the
 * worst case, if the producer stops the consumer will be able to get the
 * penultimate sigblock put into the double buffer.
 */
class SigblockDoubleBuffer {
public:

	/**
	 * @brief Initialise the sigblock double buffer object.
	 * @param[in] sigblockSize The size of the sigblock
	 */
    void Init(const SDA::size_type sigblockSize);

    /**
     * @brief Gets a sigblock from the double buffer.
     * @param[out] sb The sigblock holder where the signals
     * from the double buffer must be written.
     */
    bool Get(SDA::Sigblock& item);

    /**
     * @brief Puts a sigblock into the double buffer.
	 * @param[in] sb The sigblock container of the signals
	 * which must written to the double buffer.
     */
    bool Put(const SDA::Sigblock& item);

    /**
     * @brief Gets the size of an instance parameterised by sigblock's size.
     * @param[in] sigblockSize The size of the sigblock
     */
    static SDA::size_type SizeOf(const SDA::size_type sigblockSize);

private:

    /**
     * @brief Default constructor
     */
    /*lint -e{1704} instances of this class are not instantiable*/
    SigblockDoubleBuffer();

    /**
     * The constant representing the number of buffers (actually two)
     */
	static const SDA::uint32 TWO = 2u;

	/**
	 * Enumeration for tracking the buffer status
	 */
	enum BufferStatus {FREE, FULL, WRITING, READING};

	/*
	 * Size of the sigblock
	 */
	SDA::size_type sizeOfSigblock;

    /**
     * Index of the current front buffer
     */
    SDA::uint32 frontbuffer;

    /**
     * Status of the frontbuffer.
     */
    BufferStatus status;

    /**
     * Memory holder for the two sigblocks
     */
	/*lint -e{1501} The following data member has no size because it is
	 * mapped onto a previously allocated memory, whose size is unknown
	 * at compile time.*/
    SDA::char8 buffer[];
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SIGBLOCKDOUBLEBUFFER_H_ */
