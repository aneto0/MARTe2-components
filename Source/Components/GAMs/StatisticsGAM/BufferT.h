/**
 * @file BufferT.h
 * @brief Header file for class BufferT
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

 * @details This header file contains the declaration of the class BufferT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BufferT_H_
#define BufferT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MemoryOperationsHelper.h"
#include "GlobalObjectsDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The class provides templated memory buffer management to store and
 * retrieve data at index.
 *
 * @todo Provide locking mechanism through atomic operations.
 */

template <typename Type> class BufferT {
  public:

    /**
     * @brief Constructor. Allocates memory from the standard heap.
     */
    BufferT(const uint32 bufferSize);

    /**
     * @brief Destructor. Frees allocated memory.
     */
    virtual ~BufferT();

    /**
     * @brief Initialiser. Preloads templated array with default value.
     */
    bool Initialise(const Type& data);

    /**
     * @brief Accessor. Returns size of templated array.
     * @return size of array.
     */
    uint32 GetSize() const;

    /**
     * @brief Accessor. Rerieves data at index.
     * @return if valid index.
     */
    bool GetData(Type& data, const uint32 index);

    /**
     * @brief Accessor. Inserts data at index.
     * @return if valid index.
     */
    bool PutData(const Type& data, const uint32 index);

  private:

    uint32 size;
    Type * buffer;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <typename Type> BufferT<Type>::BufferT(const uint32 bufferSize) { 

    size = bufferSize; 
    buffer = NULL_PTR(Type *);

    uint32 memorySize = size * (uint32) sizeof(Type);

    buffer = (Type *) (GlobalObjectsDatabase::Instance()->GetStandardHeap())->Malloc(memorySize);

    bool ok = (buffer != NULL_PTR(Type *));

    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to allocate memory buffer");
    }

}

template <typename Type> BufferT<Type>::~BufferT() { 

    if (buffer != NULL_PTR(Type *)) {
        /*lint -e{1551} the implementation does not throw exception*/
        (GlobalObjectsDatabase::Instance()->GetStandardHeap())->Free((void*&) buffer);
    }

}

template <typename Type> bool BufferT<Type>::Initialise(const Type& data) { 

    bool ok = (buffer != NULL_PTR(Type *));

    uint32 index;

    for (index = 0u; (index < size) && (ok); index ++) {
        ok = PutData(data, index);
    }

    return ok; 
}

template <typename Type> uint32 BufferT<Type>::GetSize() const { return size; }

template <typename Type> bool BufferT<Type>::GetData(Type& data, const uint32 index) { 

    bool ok = (buffer != NULL_PTR(Type *));

    if (ok) {
        ok = (index < size);
    }

    if (ok) {
        data = buffer[index]; 
    }

    return ok; 
}

template <typename Type> bool BufferT<Type>::PutData(const Type& data, const uint32 index) { 

    bool ok = (buffer != NULL_PTR(Type *));

    if (ok) {
        ok = (index < size);
    }

    if (ok) {
        buffer[index] = data; 
    }

    return ok; 
}

} /* namespace MARTe */

#endif /* BufferT_H_ */

