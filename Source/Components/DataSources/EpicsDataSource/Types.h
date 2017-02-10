/**
 * @file Types.h
 * @brief Header file for class Types
 * @date 10/02/2017
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

 * @details This header file contains the declaration of the class Types
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPES_H_
#define TYPES_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace SDA {

	/**
	 * Type alias for unsigned integers
	 */
	typedef unsigned int uint32;

	/**
	 * Type alias for unsigned integers
	 */
	typedef int int32;

	/**
	 * Type alias for unsigned integers
	 */
	typedef unsigned long long int uint64; //(since C++11)

	/**
	 * Type alias for characters
	 */
	typedef char char8;

	/**
	 * Type alias for bytes
	 */
	typedef char byte;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TYPES_H_ */
