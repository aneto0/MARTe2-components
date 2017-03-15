/**
 * @file SquareRootT.h
 * @brief Header file for SquareRootT computation
 * @date 10/03/2017
 * @author Bertrand Bauvir
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

 * @details This header file contains the declaration of the class SquareRootT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SquareRootT_H_
#define SquareRootT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <math.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <typename Type> inline Type SquareRoot (const Type x) { 

    if (x < (Type) 0) {
        REPORT_ERROR(ErrorManagement::Warning, "SquareRoot<Type> of negative number");
    }

    return sqrt(x); 
}

template <> inline uint8 SquareRoot<uint8>(const uint8 x) { /* From http://en.wikipedia.org/wiki/Methods_of_computing_square_roots */

    uint8 tmp = x;
    uint8 res = 0u;
    uint8 bit = static_cast<uint8>(1u << 6); // The second-to-top bit is set
 
    // "bit" starts at the highest power of four <= the argument.
    while (bit > tmp) {
        bit >>= 2;
    }
 
    while (bit != 0) 
      {
        if (tmp >= (res + bit)) { 
	    tmp -= res + bit; 
	    res = (res >> 1) + bit; 
	} else {
	    res >>= 1;
	}

        bit >>= 2;
      }

    return res;

}

template <> inline uint16 SquareRoot<uint16>(const uint16 x) { /* From http://en.wikipedia.org/wiki/Methods_of_computing_square_roots */

    uint16 tmp = x;
    uint16 res = 0u;
    uint16 bit = static_cast<uint16>(1u << 14); // The second-to-top bit is set
 
    // "bit" starts at the highest power of four <= the argument.
    while (bit > tmp) {
        bit >>= 2;
    }

    while (bit != 0) 
      {
        if (tmp >= (res + bit)) { 
	    tmp -= res + bit; 
	    res = (res >> 1) + bit; 
	}
        else {
	    res >>= 1;
	}

        bit >>= 2;
      }

    return res;

}

template <> inline uint32 SquareRoot<uint32>(const uint32 x) { /* From http://en.wikipedia.org/wiki/Methods_of_computing_square_roots */

    uint32 tmp = x;
    uint32 res = 0u;
    uint32 bit = static_cast<uint32>(1u << 30); // The second-to-top bit is set
 
    // "bit" starts at the highest power of four <= the argument.
    while (bit > tmp) {
        bit >>= 2;
    }

    while (bit != 0) 
      {
        if (tmp >= (res + bit)) {
	    tmp -= res + bit; 
	    res = (res >> 1) + bit; 
	} else {
	    res >>= 1;
	}

        bit >>= 2;
      }

    return res;

}

template <> inline uint64 SquareRoot<uint64>(const uint64 x) { /* From http://en.wikipedia.org/wiki/Methods_of_computing_square_roots */

    uint64 tmp = x;
    uint64 res = 0u;
    uint64 bit = static_cast<uint64>(1ul << 62); // The second-to-top bit is set
 
    // "bit" starts at the highest power of four <= the argument.
    while (bit > tmp) {
        bit >>= 2;
    }
 
    while (bit != 0) 
      {
        if (tmp >= (res + bit)) { 
	    tmp -= res + bit; 
	    res = (res >> 1) + bit; 
	} else {
	    res >>= 1;
	}

        bit >>= 2;
      }

    return res;

}

template <> inline int8 SquareRoot<int8>(const int8 x) {

    if (x < 0) {
        REPORT_ERROR(ErrorManagement::Warning, "SquareRoot<int8> of negative number");
    }

    uint8 tmp = static_cast<uint8>(x);
    int8 res = static_cast<int8>(SquareRoot<uint8>(tmp));

    return res; 
};

template <> inline int16 SquareRoot<int16>(const int16 x) {

    if (x < 0) {
        REPORT_ERROR(ErrorManagement::Warning, "SquareRoot<int16> of negative number");
    }

    uint16 tmp = static_cast<uint16>(x);
    int16 res = static_cast<int16>(SquareRoot<uint16>(tmp));

    return res; 
};

template <> inline int32 SquareRoot<int32>(const int32 x) {

    if (x < 0) {
        REPORT_ERROR(ErrorManagement::Warning, "SquareRoot<int32> of negative number");
    }

    uint32 tmp = static_cast<uint32>(x);
    int32 res = static_cast<int32>(SquareRoot<uint32>(tmp));

    return res; 
};

template <> inline int64 SquareRoot<int64>(const int64 x) {

    if (x < 0) {
        REPORT_ERROR(ErrorManagement::Warning, "SquareRoot<int64> of negative number");
    }

    uint64 tmp = static_cast<uint64>(x);
    int64 res = static_cast<int64>(SquareRoot<uint64>(tmp));

    return res; 
};

} /* namespace MARTe */

#endif /* SquareRootT_H_ */


