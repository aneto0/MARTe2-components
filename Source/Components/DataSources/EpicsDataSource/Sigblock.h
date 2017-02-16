/**
 * @file Sigblock.h
 * @brief Header file for class Sigblock
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

 * @details This header file contains the declaration of the class Sigblock
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIGBLOCK_H_
#define SIGBLOCK_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Signal.h"
#include "Types.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace SDA {

/**
 * @brief This class represents the concept of a block of signals.
 * @details The signals hold by the sigblock do not have a pre-established
 * position into the sigblock, because it depends on the size of the actual
 * type of each one of them. So, the class offers access to the address of
 * each signal, being responsibility of the clients to deal with the binary
 * representation of each signal and cast it properly.
 */
class Sigblock {
public:

	/**
	 * @brief This class contains the meta properties of a sigblock.
	 * @details This class contains the meta properties of the each one of
	 * the signals of the sigblock, alongside a set of helper functions.
	 */
	class Metadata {
	public:

		/**
		 * @brief Sets the metadata
		 * @param[in] count Number of signals in the signalsMetadata
		 * array parameter.
		 * @param[in] metadata[] Array with the description of each
		 * signal present in the sigblock.
		 */
		void SetSignalsMetadata(const SDA::uint32 count, const SDA::Signal::Metadata metadata[]);

		/**
		 * @brief Gets the signals count.
		 */
		SDA::uint32 GetSignalsCount() const;

		/**
		 * @brief Gets the signal's index from its name.
		 * @param[in] name The name of the signal.
		 */
		SDA::uint32 GetSignalIndex(const SDA::char8* const name);

		/**
		 * @brief Gets the signal's name from its index.
		 * @param[in] The index of the signal.
		 */
		const SDA::char8* GetSignalName(const SDA::int32 index) const;

		/**
		 * @brief Gets the signal's offset from its index.
		 * @param[in] The index of the signal.
		 */
		SDA::uint32 GetSignalOffsetByIndex(const SDA::uint32 index) const;

		/**
		 * @brief Gets the signal's size from its index.
		 * @param[in] The index of the signal.
		 */
		SDA::size_type GetSignalSizeByIndex(const SDA::int32 index) const;

		/**
		 * @brief Gets the total size of the sigblock.
		 */
		SDA::size_type GetTotalSize() const;

		/**
		 * @brief Calculates the size needed to hold an instance of this class.
		 * @param[in] numSignals The number of signals to use in calculation.
		 */
		static SDA::size_type SizeOf(const SDA::uint32 numSignals);

	private:

		/**
		 * @brief Default constructor
		 */
	    /*lint -e{1704} instances of this class are not instantiable*/
		Metadata();

		/**
		 * @brief This class contains the meta properties of a signal and
		 * extra properties relative to their inclusion into a sigblock.
		 */
		/*lint -e{1790} SDA::Signal::Metadata is a POD*/
		class SignalMetadataEx: public SDA::Signal::Metadata {
		public:

			/**
			 * The offset of the signal respect the beginning of the sigblock.
			 */
		    SDA::size_type offset;
		};

		/**
		 * The number of signals described (i.e. the number
		 * of elements in the signalsMetadata array)
		 */
		SDA::uint32 signalsCount;

		/**
		 * Array which holds the list of signals' metadata (one
		 * metadata element for each signal).
		 */
		/*lint -e{1501} The following data member has no size because it is
		 * mapped onto a previously allocated memory, whose size is unknown
		 * at compile time.*/
		SignalMetadataEx signalsMetadata[];
	};

	/**
	 * @brief Gets the pointer of a signal into the sigblock from an offset.
	 * @param[in] offset The offset of the signal in bytes, relative to the
	 * beginning of the sigblock.
	 */
	void* GetSignalAddress(const SDA::size_type offset);
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SIGBLOCK_H_ */
