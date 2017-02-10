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

#include <cstddef>

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
		SDA::char8* RawHeader();
		SDA::char8* RawItems();
		Sigblock::Metadata* Header();
		SigblockDoubleBuffer* Items();
		bool IsOperational() const;
		void FillPreHeader(std::size_t sizeOfHeader, std::size_t sizeOfItems);
		void FillHeader(const SDA::uint32 signalsCount, const Signal::Metadata signalsMetadata[]);
		void FillItems(const SDA::uint32 bufferSize, std::size_t sizeOfSigblock);
	public:
		const std::size_t size;
		bool hasReader;
		bool hasWriter;
		SDA::uint64 droppedWrites;	//TODO PURGE??
		SDA::uint64 missedReads;	//TODO PURGE??
		std::size_t offsetOfHeader;
		std::size_t offsetOfItems;
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
	    bool ReadSigblock(Sigblock& sb);

	    /**
	     * @brief Gets a pointer to sigblock's metadata.
	     */
	    Sigblock::Metadata* GetSigblockMetadata();

	private:

	    /**
	     * @brief Default constructor
	     */
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
	    bool WriteSigblock(const Sigblock& sb);


	    /**
	     * @brief Gets a pointer to sigblock's metadata.
	     */
	    Sigblock::Metadata* GetSigblockMetadata();


	    /**
	     * @brief TODO Document it or purge the method
	     */
	    SDA::uint64 DroppedWrites() const;

	private:

	    /**
	     * @brief Default constructor
	     */
	    SigblockProducer();
	};

public:

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

public:

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
	 * @param[in] bufferSize			 max 10 by default?
	 * @pre An interprocess shared memory identified by the name parameter
	 * must not exist.
	 * @post The returned SharedDataArea points to a new interprocess shared
	 * memory, which is identified by the name parameter and conforms to the
	 * representation expected by a SharedDataArea object.
	 */
	static SharedDataArea BuildSharedDataAreaForMARTe(const SDA::char8* const name, const SDA::uint32 signalsCount, const Signal::Metadata signalsMetadata[], const SDA::uint32 bufferSize);

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
	static SharedDataArea BuildSharedDataAreaForEPICS(const SDA::char8* const name);

private:

	/**
	 * @brief Constructor which builds the instance linking it to an existing
	 * shared memory.
	 * @param[in] shm The pointer to the shared memory which honors the
	 * representation specified by the structure Representation.
	 */
	SharedDataArea(Representation* shm);

	/**
	 * The pointer to the shared memory area which conforms to the representation.
	 */
	Representation* shm;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline SDA::char8* SharedDataArea::Representation::RawHeader() {
	return (rawmem + offsetOfHeader);
}

inline SDA::char8* SharedDataArea::Representation::RawItems() {
	return (rawmem + offsetOfItems);
}

inline Sigblock::Metadata* SharedDataArea::Representation::Header() {
	return reinterpret_cast<Sigblock::Metadata*>(RawHeader());
}

inline SigblockDoubleBuffer* SharedDataArea::Representation::Items() {
	return reinterpret_cast<SigblockDoubleBuffer*>(RawItems());
}

inline bool SharedDataArea::Representation::IsOperational() const {
	return (hasReader && hasWriter);
};

#endif /* SIGBLOCKPIPE_H_ */
