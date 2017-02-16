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

//#include <cstddef>
//#include <cerrno>
//#include <cstdio>
#ifndef LINT
#include <cstdlib>      //Import exit function.
#include <cstring>      //Import std::memset function
/* Including cstring.h is mandatory when using strerror()
 * function, otherwise a segmentation fault will arise. */
#endif
#include <fcntl.h>      //Import file O_* constants.
#include <sys/stat.h>   //Import file mode constants.
#include <sys/mman.h>   //Import POSIX shared memory functions.
#include <unistd.h>     //Import ftruncate function.

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Platform.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

    //TODO: Reduce permissions defined in OPEN_MODE to the minimum necessary set.
    /*lint -e{9130} the oflag argument of shm_open is defined as int and it can not be changed*/
    const SDA::uint32 OPEN_MODE = static_cast<SDA::uint32>(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace SDA {

void* Platform::MakeShm(const SDA::char8* const name, const SDA::size_type size) {

    void* result;

    SDA::int32 shm_fd;

    const SDA::uint32 OPEN_FLAGS = (static_cast<SDA::uint32>(O_CREAT) | static_cast<SDA::uint32>(O_EXCL) | static_cast<SDA::uint32>(O_RDWR));

    shm_fd = shm_open(name, static_cast<SDA::int32>(OPEN_FLAGS), OPEN_MODE);
    if (shm_fd == -1) {
//    	printf("*** shm_open error (server)  [%s]***\n", strerror(errno));
        (void)std::exit(EXIT_FAILURE);
    }

    SDA::int32 fret;
    fret = ftruncate(shm_fd, static_cast<off_t>(size));
    if (fret == -1) {
//    	printf("*** ftruncate error (server)  [%s]***\n", strerror(errno));
        (void)std::exit(EXIT_FAILURE);
    }

    /*lint -e{9130} the prot argument of mmap is defined as int and it can not be changed*/
    result = mmap(SDA_NULL_PTR(void*), size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, static_cast<off_t>(0));
    if (result == /*lint -e(1924) -e(923)*/MAP_FAILED) {
//    	printf("*** mmap error (server)  [%s]***\n", strerror(errno));
        (void)std::exit(EXIT_FAILURE);
    }

    (void)std::memset(result, 72, size);	//TODO: memset to 0!!

    *(static_cast<SDA::size_type*>(result)) = size;

    return result;
}

void* Platform::JoinShm(const SDA::char8* const name) {

    void* result;

    SDA::int32 shm_fd;

    SDA::size_type size; //Size of allocated shared memory, including the the size value itself.

    const SDA::uint32 OPEN_FLAGS = (static_cast<SDA::uint32>(O_RDWR));

    shm_fd = shm_open(name, static_cast<SDA::int32>(OPEN_FLAGS), OPEN_MODE);
    if (shm_fd == -1) {
//    	printf("*** shm_open error (server)  [%s]***\n", strerror(errno));
         (void)std::exit(EXIT_FAILURE);	//TODO: Return status instead of halting program.
    }

    void* tmp = mmap(SDA_NULL_PTR(void*), sizeof(SDA::size_type), PROT_READ /*| PROT_WRITE*/, MAP_SHARED, shm_fd, static_cast<off_t>(0));
    if (tmp == /*lint -e(1924) -e(923)*/MAP_FAILED) {
//    	printf("***pre mmap error (server)  [%s]***\n", strerror(errno));
        (void)std::exit(EXIT_FAILURE);
    }

    size = *(static_cast<SDA::size_type*>(tmp));

    SDA::int32 fret;
    fret = munmap(tmp, sizeof(SDA::size_type));
    if (fret == -1) {
//    	printf("***pre munmap error (server)  [%s]***\n", strerror(errno));
        (void)std::exit(EXIT_FAILURE);
    }

    /*lint -e{9130} the prot argument of mmap is defined as int and it can not be changed*/
    result = mmap(SDA_NULL_PTR(void*), size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, static_cast<off_t>(0));
    if (result == /*lint -e(1924) -e(923)*/MAP_FAILED) {
//    	printf("*** mmap error (server)  [%s]***\n", strerror(errno));
        (void)std::exit(EXIT_FAILURE);
    }

    return result;
}

void Platform::DettachShm(void* const shm_ptr, const SDA::size_type shm_size) {
	SDA::int32 fret;
    fret = munmap(shm_ptr, shm_size);
    if (fret == -1) {
//    	printf("*** munmap error (server)  [%s]***\n", strerror(errno));
        (void)std::exit(EXIT_FAILURE);
    }
}

void Platform::DestroyShm(const SDA::char8* const name) {
    SDA::int32 fret;
    fret = shm_unlink(name);
    if (fret == -1) {
//      printf("*** munmap error (server)  [%s]***\n", strerror(errno));
        (void)std::exit(EXIT_FAILURE);
    }
}

}
