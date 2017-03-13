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

/*lint -efile(766, Types.h) std::size_t is actually imported from cstddef*/
/*lint -e451 */
#include <cstddef>    //Import std::size_t
/*lint +e451 */

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*lint -save -e9026, function-like macro defined. The aim is to reduce the clutter in the code
 * This avoids replacing ptr = static<MyObject *>(NULL) with ptr = SDA_NULL_PTR(MyObject *)*/
#define SDA_NULL_PTR(ptr) static_cast<ptr>(0)
/*lint -restore */

#ifndef NULL
#define NULL SDA_NULL_PTR(void *)
#endif

namespace SDA {

/*lint -e1708*/

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
 * Type alias for size_t
 */
typedef std::size_t size_type;

/**
 * Maximum value for unsigned integers of 32 bits
 */
static const uint32 MAX_UINT32 = 0xFFFFFFFFU;

/*lint +e1708*/

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TYPES_H_ */
