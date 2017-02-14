/**
 * @file Platform.cpp
 * @brief Source file for class Platform
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

 * @details This source file contains the definition of all the methods for
 * the class Platform (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring> //Including cstring.h is mandatory, otherwise you will get
				   //a segmentation fault when you use strerror() function.
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Platform.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace SDA {

void* Platform::MakeShm(const SDA::char8* const name, const size_t size) {

    void* result;

    SDA::int32 shm_fd;

    /*lint -e{9130} the oflag argument of shm_open is defined as int and it can not be changed*/
    shm_fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0666u);
    if (shm_fd == -1) {
//    	printf("*** shm_open error (server)  [%s]***\n", strerror(errno));
    	exit(EXIT_FAILURE);
    }

    SDA::int32 fret;
    fret = ftruncate(shm_fd, static_cast<off_t>(size));
    if (fret == -1) {
//    	printf("*** ftruncate error (server)  [%s]***\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /*lint -e{9130} the prot argument of mmap is defined as int and it can not be changed*/
    result = mmap(NULL_PTR(void*), size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, static_cast<off_t>(0));
    if (result == /*lint -e(1924) -e(923)*/MAP_FAILED) {
//    	printf("*** mmap error (server)  [%s]***\n", strerror(errno));
    	exit(EXIT_FAILURE);
    }

    std::memset(result, 72, size);	//TODO: memset to 0!!

    *(static_cast<size_t*>(result)) = size;

    return result;
}

void* Platform::JoinShm(const SDA::char8* const name) {

    void* result;

    SDA::int32 shm_fd;

    size_t size; //Size of allocated shared memory, including the the size value itself.

    shm_fd = shm_open(name, O_RDWR, 0666u);
    if (shm_fd == -1) {
//    	printf("*** shm_open error (server)  [%s]***\n", strerror(errno));
         exit(EXIT_FAILURE);	//TODO: Return status instead of halting program.
    }

    void* tmp = mmap(NULL_PTR(void*), sizeof(size_t), PROT_READ /*| PROT_WRITE*/, MAP_SHARED, shm_fd, static_cast<off_t>(0));
    if (tmp == /*lint -e(1924) -e(923)*/MAP_FAILED) {
//    	printf("***pre mmap error (server)  [%s]***\n", strerror(errno));
    	exit(EXIT_FAILURE);
    }

    size = *(static_cast<size_t*>(tmp));

    SDA::int32 fret;
    fret = munmap(tmp, sizeof(size_t));
    if (fret == -1) {
//    	printf("***pre munmap error (server)  [%s]***\n", strerror(errno));
    	exit(EXIT_FAILURE);
    }

    /*lint -e{9130} the prot argument of mmap is defined as int and it can not be changed*/
    result = mmap(NULL_PTR(void*), size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, static_cast<off_t>(0));
    if (result == /*lint -e(1924) -e(923)*/MAP_FAILED) {
//    	printf("*** mmap error (server)  [%s]***\n", strerror(errno));
    	exit(EXIT_FAILURE);
    }

    return result;
}

void Platform::DettachShm(void* const shm_ptr, const size_t shm_size) {
	SDA::int32 fret;
    fret = munmap(shm_ptr, shm_size);
    if (fret == -1) {
//    	printf("*** munmap error (server)  [%s]***\n", strerror(errno));
    	exit(EXIT_FAILURE);
    }
}

void Platform::DestroyShm(const SDA::char8* const name) {
    shm_unlink(name);
}

}
