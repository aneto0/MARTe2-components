/**
 * @file Signal.h
 * @brief Header file for class Signal
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

 * @details This header file contains the declaration of the class Signal
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <cstddef>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Types.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief This class represents the concept of a signal.
 * @details This class is not going to have any member because its content
 * depends on the type of the actual signal and it is not expected to provide
 * any standard access to its contents. Nevertheless it is helpful as a recall
 * of the signal concept and as a namespace for its metadata class.
 */
class Signal {
public:

	/**
	 * @brief This class contains the meta properties of a signal.
	 */
	class Metadata {
	public:

		/**
		 * The maximum length of the signal's name.
		 */
		static const SDA::uint32 NAME_MAX_LEN = 30u;

		/**
		 * The name of the signal.
		 */
		SDA::char8 name[NAME_MAX_LEN];

		/**
		 * The size of the signal's representation.
		 */
		std::size_t size;

		/**
		 * @brief Default constructor
		 */
		Metadata();
	};

private:

	/**
	 * @brief Default constructor
	 */
	Signal();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline Signal::Metadata::Metadata(): size(0u) {
	name[0] = '\0';
}

#endif /* SIGNAL_H_ */
