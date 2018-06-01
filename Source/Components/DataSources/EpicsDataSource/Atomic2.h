/**
 * @file Atomic2.h
 * @brief Header file for module Atomic2
 * @date 21/12/2016
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

 * @details This header file contains the declaration of the module Atomic2
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ATOMIC2_H_
#define ATOMIC2_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

namespace SDA {

/*lint -e1066 Disabled because lint gets confused with standard C headers */

/**
 * @brief Atomic compare-and-swap operation
 * @details The pseudo code of this function is as follows:
 *  function CAS(ptr : pointer to int, old : int, new : int) returns bool {
 *    if *ptr ≠ old {
 *      return false
 *    }
 *    *ptr ← new
 *    return true
 *  }
 * @tparam T A fundamental scalar type
 * @param[in,out] ptr Target for comparison and assignment
 * @param[in] oldval Value to compare with *ptr
 * @param[in] newval Value to assign to *ptr if (*ptr==oldval)
 * @return true if the comparison (*ptr==oldval) is successful
 * and newval was written
 */
template<typename T>
inline bool CAS(volatile T* const ptr,
                const T oldval,
                const T newval);

/**
 * @brief Atomic exchange operation
 * @details Assigns the value of val to *ptr
 * @tparam T A fundamental scalar type
 * @param[in,out] ptr Target for assignment
 * @param[in] val Value to assign to *ptr
 * @return the previous value of *ptr
 */
template<typename T>
inline T XCHG(volatile T* const ptr,
              const T val);

/**
 * @brief Atomic read
 * @tparam T A fundamental scalar type
 * @param[in] b Source for reading
 * @return the value in *b
 */
template<typename T>
T READ(volatile const T* const b);

/**
 * @brief Atomic write
 * @tparam T A fundamental scalar type
 * @param[out] b Target for writing
 * @param[in] v Value to assign to *b
 */
template<typename T>
void WRITE(volatile T* const b,
           const T v);

/*lint +e1066 Enabled again after exception has been useful */

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace SDA {

/*lint -estring(1795,READ) [MISRA C++ Rule 14-7-1] The READ function is offered as per library basis.*/

/*lint -estring(1795,WRITE) [MISRA C++ Rule 14-7-1] The WRITE function is offered as per library basis.*/

/*lint -estring(526,__sync_bool_compare_and_swap) -estring(628,__sync_bool_compare_and_swap) -estring(746,__sync_bool_compare_and_swap) -estring(1055,__sync_bool_compare_and_swap) The __sync_bool_compare_and_swap function is a GCC built-in function, so it does not have declaration.*/

/*lint -estring(526,*__sync_lock_test_and_set) -estring(628,*__sync_lock_test_and_set) -estring(746,*__sync_lock_test_and_set) -estring(1055,*__sync_lock_test_and_set) The __sync_lock_test_and_set function is a GCC built-in function, so it does not have declaration.*/

/*
 * Note: In GCC, CAS is mapped to the __sync_bool_compare_and_swap function,
 * which is documented in builtins section for atomic access of GCC's
 * documentation. This builtin is a full memory barrier, so no memory operand
 * will be moved across the operation, either forward or backward. More info
 * on https://gcc.gnu.org/onlinedocs/gcc-4.4.3/gcc/Atomic-Builtins.html#Atomic
 * -Builtins
 */
template<typename T>
inline bool CAS(volatile T* const ptr,
                const T oldval,
                const T newval) {
    return static_cast<bool>(__sync_bool_compare_and_swap(ptr, oldval, newval));
}

/*
 * Note: In GCC, CAS is mapped to the __sync_lock_test_and_set function,
 * which is documented in builtins section for atomic access of GCC's
 * documentation. This builtin is not a full barrier, but rather an acquire
 * barrier.  This means that references after the builtin cannot move to (or
 * be speculated to) before the builtin, but previous memory stores may not
 * be globally visible yet, and previous memory loads may not yet be satisfied.
 * See more info on https://gcc.gnu.org/onlinedocs/gcc-4.4.3/gcc/Atomic-
 * Builtins.html#Atomic-Builtins
 */
template<typename T>
inline T XCHG(volatile T* const ptr,
              const T val) {
    return static_cast<T>(__sync_lock_test_and_set(ptr, val));
}

/*
 * Note: In GCC, READ uses the __sync_synchronize function, a full memory
 * barrier which is documented in builtins section for atomic access of GCC's
 * documentation. The reading is assumed atomic for fundamental scalar types.
 * See more info on https://gcc.gnu.org/onlinedocs/gcc-4.4.3/gcc/Atomic-
 * Builtins.html#Atomic-Builtins
 */
template<typename T>
T READ(volatile const T* const b) {
    T v = *b;
    (void) __sync_synchronize(); //Full memory barrier (ensures val pushed to memory)
    return v;
}

/*
 * Note: In GCC, WRITE uses the __sync_synchronize function, a full memory
 * barrier which is documented in builtins section for atomic access of GCC's
 * documentation. The writing is assumed atomic for fundamental scalar types.
 * See more info on https://gcc.gnu.org/onlinedocs/gcc-4.4.3/gcc/Atomic-
 * Builtins.html#Atomic-Builtins
 */
template<typename T>
void WRITE(volatile T* const b,
           const T v) {
    __sync_synchronize(); //Full memory barrier (read will return fresh value)
    *b = v;
}

}

#endif /* ATOMIC2_H_ */
