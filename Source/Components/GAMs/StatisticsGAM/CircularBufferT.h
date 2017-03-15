/**
 * @file CircularBufferT.h
 * @brief Header file for class CircularBufferT
 * @date 08/03/2017
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

 * @details This header file contains the declaration of the class CircularBufferT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CircularBufferT_H_
#define CircularBufferT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "BufferT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The class provides templated memory buffer management to store data
 * in a circular way.
 *
 * @todo Provide locking mechanism through atomic operations.
 */

/*lint -e{1712} the implementation does not provide default constructor*/
template <typename Type> class CircularBufferT : public BufferT<Type> {
  public:

    /**
     * @brief Constructor. Allocates memory from the standard heap.
     * @post
     *   index = 0u
     */
    CircularBufferT(const uint32 bufferSize);

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~CircularBufferT(); 

    /**
     * @brief Accessor. Inserts data in buffer.
     * @return if buffer properly allocated.
     */
    bool PushData (Type& data);

    /**
     * @brief Accessor. Inserts data in buffer and return removed element.
     * @return if buffer properly allocated.
     */
    bool PushData (Type& data_in, Type& data_out);

  private:

    uint32 index;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <typename Type> CircularBufferT<Type>::CircularBufferT(const uint32 bufferSize) :
  BufferT<Type> (bufferSize) { 

    index = 0u;

}

template <typename Type> CircularBufferT<Type>::~CircularBufferT() { 
/* Nothing further to do than the default base class destructor */ 
}

template <typename Type> bool CircularBufferT<Type>::PushData(Type& data) { 

    bool ok = PutData(data, index);

    if (ok) {
        index++;
    }

    if (index == BufferT<Type>::GetSize()) {
        index = 0u;
    }

    return ok;
}

template <typename Type> bool CircularBufferT<Type>::PushData(Type& data_in, Type& data_out) { 

    bool ok = GetData(data_out, index);

    if (ok) {
        ok = PushData(data_in);
    }

    return ok;
}

} /* namespace MARTe */

#endif /* CircularBufferT_H_ */

