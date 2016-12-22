/**
 * @file Platform.h
 * @brief Header file for class Platform
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

 * @details This header file contains the declaration of the class Platform
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <cstddef>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Shared memory manager
 * @details This class is a platform specific collection of static methods for
 * managing an interprocess shared memory, following a very simple model: one
 * single process creates it and another joins it after it has been created.
 * The shared memory is identified by a system wide unique name, and is mapped
 * to a process space memory address, so each process will access the same
 * memory through different addresses.
 */
class Platform {
public:

	/**
	 * @brief Makes an interprocess shared memory.
	 * @details Creates a new interprocess shared memory of a given size,
	 * which is mapped to the current process memory space, and returns its
	 * base memory address.
	 */
	static void* MakeShm(const char* name, const size_t size);

	/**
	 * @brief Joins an interprocess shared memory.
	 * @details Maps an existing interprocess shared memory to the current
	 * process memory space and returns its base memory address.
	 */
	static void* JoinShm(const char* name);

	/**
	 * @brief Unmaps an interprocess shared memory identified by its base
	 * address and a given size.
	 */
	static void DettachShm(void* shm_ptr, size_t shm_size);

	/**
	 * @brief Deletes an interprocess shared memory from the system,
	 * identified by its system wide unique name.
	 */
	static void DestroyShm(const char* name);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PLATFORM_H_ */
