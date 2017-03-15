/**
 * @file SharedDataArea.h
 * @brief Header file for class SharedDataArea
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

 * @details This header file contains the declaration of the class SharedDataArea
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIGBLOCKPIPE_H_
#define SIGBLOCKPIPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Signal.h"
#include "Sigblock.h"
#include "SigblockDoubleBuffer.h"
#include "Types.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace SDA {

/**
 * @brief Interprocess shared data area handle.
 *
 * @details This class is a handle to an interprocess shared memory area meant
 * for interchanging sigblocks between two processes. It offers access through
 * two different interfaces, one for consuming and another for producing. By
 * requirement, this class does not have to buffer a queue of sigblocks, it is
 * enough maintaining a double buffer, as long as it assures that the consumer
 * always gets the fresher sigblock put by the producer.
 *
 * @warning If this class is going to be used by two different applications,
 * then both shall be compiled with the same compiler and version, otherwise
 * bugs could show up at run time (mainly those related to padding between
 * data members).
 */
class SharedDataArea {

private:

    /*
     * Implementation details:
     *   The implementation of this class relies on a fixed representation
     *   of the shared memory area which has to be only managed by this
     *   class. A private structure called Representation is used for
     *   representing that shared memory and managing its initialization and
     *   other stuff.
     *   Trick nº1: The interfaces SigblockConsumer and SigblockProducer
     *   both inherit privately from the Representation structure, so they
     *   do not need to hold a private member pointer to the shared memory
     *   area. That implies that no instances of them are going to be created,
     *   instead they are mapped to the shared memory area itself.
     *   Trick nº2: The Representation structure has a tail data member that
     *   is an array of chars with no elements (SDA::char8 member[]), which does
     *   not increase the size of the structure, but it allows to access the
     *   memory allocated beyond the end of the structure. This is needed
     *   because the number of signals is unknown at compile time and extra
     *   space is allocated for them at run time.
     *
     *   Implementation warning nº1:
     *     It is essential to not add the following C++ features to classes
     *     mapped to the shared memory area (Representation, SigblockConsumer,
     *     and SigblockProducer):
     *       · Member pointers
     *       · Virtual methods
     *       · Virtual inheritance
     *     The reason behind that is that each instance of this class will be
     *     executing on different processes and, although accessing the same
     *     shared memory area, they will use different memory address spaces.
     *     So, using features that add pointers to the object (implicitly as a
     *     virtual table or explicit as as a data member pointers), it will
     *     make the program to crash by segmentation fault or data corruption.
     *
     *   Implementation warning nº2:
     *     Related to the trick nº2, the existence of a special tail member
     *     in the Representation structure means that classes that inherit
     *     from Representation, like SigblockConsumer and SigblockProducer
     *     can not have any data members or the Representation's tail will
     *     overlap with them.
     */
    class Representation {
    public:
        /**
         * @brief Gets a raw pointer to header.
         */
        SDA::char8* RawHeader();
        /**
         * @brief Gets a raw pointer to items.
         */
        SDA::char8* RawItems();
        /**
         * @brief Gets a typed pointer to header.
         */
        SDA::Sigblock::Metadata* Header();
        /**
         * @brief Gets a typed pointer to items.
         */
        SDA::SigblockDoubleBuffer* Items();
        /**
         * @brief Queries if the shared data area is operational,
         * i.e. if it has a reader and a writer.
         */
        bool IsOperational() const;
        /**
         * @brief Initialises the pre-header area's attributes.
         */
        void FillPreHeader(const SDA::size_type sizeOfHeader);
        /**
         * @brief Initialises the header area's attributes.
         */
        void FillHeader(const SDA::uint32 signalsCount,
                        const SDA::Signal::Metadata signalsMetadata[]);
        /**
         * @brief Initialises the items area's attributes.
         */
        void FillItems(const SDA::size_type sizeOfSigblock);
        /**
         * Flag for marking if the shared data area has a reader linked to it.
         */
        bool hasReader;
        /**
         * Flag for marking if the shared data area has a writer linked to it.
         */
        bool hasWriter;
        /**
         * Counter of missed writes during operation of the shared data area.
         */
        SDA::uint64 droppedWrites;      //TODO PURGE??
        /**
         * Offset of the header area (beginning from rawmem's base address).
         */
        SDA::size_type offsetOfHeader;
        /**
         * Offset of the items area (beginning from rawmem's base address).
         */
        SDA::size_type offsetOfItems;
        /**
         * Placeholder for raw memory where header and items will be mapped.
         */
        /*lint -e{1501} The following data member has no size because it is
         * mapped onto a previously allocated memory, whose size is unknown
         * at compile time.*/
        SDA::char8 rawmem[];
    };

public:

    /**
     * @brief Consumer interface class
     * @description This class is an interface to the shared data area meant for
     * the consumer, so only reading of data is allowed. It also offers access
     * to the metadata. No instances of it can be created.
     */
    class SigblockConsumer: private Representation {
    public:

        /**
         * @brief Reads a sigblock from the shared data area.
         * @param[out] sb The sigblock holder where the signals from the
         * shared data area must be written.
         */
        bool ReadSigblock(SDA::Sigblock& sb);

        /**
         * @brief Gets a pointer to sigblock's metadata.
         */
        SDA::Sigblock::Metadata* GetSigblockMetadata();

    private:

        /**
         * @brief Default constructor
         */
        /*lint -e{1704} instances of this class are not instantiable*/
        SigblockConsumer();
    };

    /**
     * @brief Producer interface class
     * @description This class is an interface to the shared data area meant for
     * the producer, so only writing of data is allowed. It also offers access
     * to the metadata. No instances of it can be created.
     */
    class SigblockProducer: private Representation {
    public:

        /**
         * @brief Writes a sigblock to the shared data area.
         * @param[in] sb The sigblock container of the signals which must
         * written to the shared data area.
         */
        bool WriteSigblock(const SDA::Sigblock& sb);

        /**
         * @brief Gets a pointer to sigblock's metadata.
         */
        SDA::Sigblock::Metadata* GetSigblockMetadata();

        /**
         * @brief TODO Document it or purge the method
         */
        SDA::uint64 DroppedWrites() const;

    private:

        /**
         * @brief Default constructor
         */
        /*lint -e{1704} instances of this class are not instantiable*/
        SigblockProducer();
    };

    /**
     * @brief Gets an interface pointer to the shared data area as a sigblock
     * producer, that will be valid as long as the shared data area is alive.
     * @warning This method does not allocate memory for the pointee, so the
     * client must not deallocate its memory.
     */
    SigblockProducer* GetSigblockProducerInterface();

    /**
     * @brief Gets an interface pointer to the shared data area as a sigblock
     * consumer, that will be valid as long as the shared data area is alive.
     * @warning This method does not allocate memory for the pointee, so the
     * client must not deallocate its memory.
     */
    SigblockConsumer* GetSigblockConsumerInterface();

    /**
     * @brief Default constructor
     */
    SharedDataArea();

    /**
     * @brief This static method creates an interprocess shared memory and
     * returns it properly initialized and casted as a SharedDataArea object.
     * @details This method creates an interprocess shared memory identified
     * with the name parameter, initializes its members based on the metadata
     * supplied in the signalsMetadata parameter, and returns it casted as a
     * SharedDataArea object, i.e. it does not create a new C++ instance of
     * SharedDataArea, but it maps it to the interprocess shared memory.
     * @param[in] name The name of the interprocess shared memory.
     * @param[in] signalsCount The number of signals expected.
     * @param[in] signalsMetadata[] The metadata for each expected signal.
     * @pre An interprocess shared memory identified by the name parameter
     * must not exist.
     * @post The returned SharedDataArea points to a new interprocess shared
     * memory, which is identified by the name parameter and conforms to the
     * representation expected by a SharedDataArea object.
     */
    static bool BuildSharedDataAreaForMARTe(SharedDataArea& sda,
                                            const SDA::char8* const name,
                                            const SDA::uint32 signalsCount,
                                            const SDA::Signal::Metadata signalsMetadata[]);

    /**
     * @brief This static method joins an existent interprocess shared memory
     * and returns it casted as a SharedDataArea object.
     * @details This method joins an existent interprocess shared memory
     * identified with the name parameter, and returns it casted as a
     * SharedDataArea object, i.e. it does not create a new C++ instance of
     * SharedDataArea, but it maps it to the interprocess shared memory.
     * @param[in] name The name of the interprocess shared memory.
     * @pre An interprocess shared memory identified by the name parameter
     * must exist and must conform to the representation expected by a
     * SharedDataArea object.
     * @post The returned SharedDataArea points to an existent interprocess
     * shared memory, which is identified by the name parameter and conforms
     * to the representation expected by a SharedDataArea object.
     */
    static bool BuildSharedDataAreaForEPICS(SharedDataArea& sda,
                                            const SDA::char8* const name);

private:

    /**
     * The pointer to the shared memory area which conforms to the representation.
     */
    Representation* shm;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace SDA {

/*lint -e1066 Disabled because lint gets confused with standard C headers */

inline SDA::char8* SharedDataArea::Representation::RawHeader() {
    /*lint -e{9016} [MISRA C++ Rule 5-0-15] pointer arithmetic is needed in this case*/
    return (rawmem + offsetOfHeader);
}

inline SDA::char8* SharedDataArea::Representation::RawItems() {
    /*lint -e{9016} [MISRA C++ Rule 5-0-15] pointer arithmetic is needed in this case*/
    return (rawmem + offsetOfItems);
}

inline SDA::Sigblock::Metadata* SharedDataArea::Representation::Header() {
    /*lint -e{927} -e{826} [MISRA C++ Rule 5-2-7] cast from pointer to pointer needed in this case*/
    return reinterpret_cast<SDA::Sigblock::Metadata*>(RawHeader());
}

inline SDA::SigblockDoubleBuffer* SharedDataArea::Representation::Items() {
    /*lint -e{927} -e{826} [MISRA C++ Rule 5-2-7] cast from pointer to pointer needed in this case*/
    return reinterpret_cast<SDA::SigblockDoubleBuffer*>(RawItems());
}

inline bool SharedDataArea::Representation::IsOperational() const {
    return (hasReader && hasWriter);
}

/*lint +e1066 Enabled again after exception has been useful */

}

#endif /* SIGBLOCKPIPE_H_ */
