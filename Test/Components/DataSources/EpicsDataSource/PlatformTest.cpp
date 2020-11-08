/**
 * @file PlatformTest.cpp
 * @brief Source file for class PlatformTest
 * @date 31/01/2017
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
 * the class PlatformTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

//#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <wait.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Platform.h"
#include "PlatformTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * @brief Representation of the shared memory used by Platform class.
 */
struct ShmMapping {
    size_t size;
    char bytes[];
};

/**
 * @brief Representation of the memory used for interchanging test data.
 */
struct ShmData {
    bool ok;
    pid_t pid;
    char token[];
};

const char PlatformTest::DataSet::QTOKEN[] = "The quick brown fox jumps over the lazy dog";

const char PlatformTest::DataSet::RTOKEN[] = "god yzal eht revo spmuj xof nworb kciuq ehT";

PlatformTest::DataSet::DataSet() :
        tokenlen(std::strlen(QTOKEN)) {
}

const char* PlatformTest::DataSet::GetQueryToken() {
    return QTOKEN;
}

const char* PlatformTest::DataSet::GetResponseToken() {
    return RTOKEN;
}

const size_t PlatformTest::DataSet::GetTokenLen() {
    return tokenlen;
}

/**
 * @brief Executes the test on the slave's side.
 * @pre true
 * @post A shared memory has been created which contains 3 fields (the process's
 * status, the process' pid, and the query token).
 */
static bool MasterProcessPrologue(void*& shm,
                                  const char* const name,
                                  const char* const fullname,
                                  const char* qtoken,
                                  const size_t tokenlen) {

    static const size_t DATASIZE = (sizeof(ShmData) + tokenlen);
    static unsigned int SHMSIZE = (sizeof(ShmMapping) + DATASIZE);

    bool ok = false;

    //Create an SHM and get a raw pointer to it:
    shm = SDA::Platform::MakeShm(name, SHMSIZE);

    ok = (shm != NULL);
    ok &= (access(fullname, R_OK | W_OK) == 0);	//An alternative to call access function would be calling shm_open with O_RDONLY.

    if (ok) {
        ShmMapping* map = NULL;

        //Map a typed pointer to SHM:
        map = reinterpret_cast<ShmMapping*>(shm);
        ok &= (map->size == SHMSIZE);

        if (ok) {
            ShmData* data = NULL;
            pid_t pid = getpid();

            //Map a typed pointer to data:
            data = reinterpret_cast<ShmData*>(map->bytes);

            //Check default token:
            for (unsigned int i = 0; i < tokenlen; i++) {
                ok &= (data->token[i] == 0);
            }

            //Write check status:
            data->ok = ok;

            //Write process' pid:
            data->pid = pid;

            //Write query token:
            for (unsigned int i = 0; i < tokenlen; i++) {
                data->token[i] = qtoken[i];
            }
        }
    }
    return ok;
}

/**
 * @brief Executes the test on the master's side (epilogue stage).
 * @return true if child's status, child's pid, and response token have the
 * expected values.
 * @pre The shared memory is properly initialized with 3 fields (the child's
 * status, the child's pid, and the response token).
 * @post The shared memory remains untouched.
 */
static bool MasterProcessEpilogue(void* shm,
                                  const char* const name,
                                  const char* const fullname,
                                  pid_t cpid,
                                  const char* rtoken,
                                  const size_t tokenlen) {

    static const size_t DATASIZE = (sizeof(ShmData) + tokenlen);
    static unsigned int SHMSIZE = (sizeof(ShmMapping) + DATASIZE);

    bool ok = false;

    ok = (shm != NULL);

    if (ok) {
        ShmMapping* map = NULL;

        //Map a typed pointer to SHM:
        map = reinterpret_cast<ShmMapping*>(shm);
        ok &= (map->size == SHMSIZE);

        if (ok) {
            ShmData* data = NULL;

            //Map a typed pointer to data:
            data = reinterpret_cast<ShmData*>(map->bytes);

            //Check child's status:
            ok &= (data->ok == true);

            //Check child's pid:
            ok &= (data->pid == cpid);

            //Check response token:
            for (unsigned int i = 0; i < tokenlen; i++) {
                ok &= (data->token[i] == rtoken[i]);
            }

            //Dettach SHM:
            ok &= SDA::Platform::DettachShm(shm, map->size);

            //Destroy SHM:
            ok &= SDA::Platform::DestroyShm(name);
            ok &= (access(fullname, R_OK | W_OK) == -1);
        }
    }

    return ok;
}

/**
 * @brief Executes the test on the slave's side.
 * @pre The shared memory is properly initialized with 3 fields (the parent's
 * status, the parent's pid, and the query token).
 * @post The shared memory has new values in its 3 fields (the child's status,
 * the child's pid, and the response token).
 */
static void SlaveProcess(const char* const name,
                         const char* const fullname,
                         pid_t ppid,
                         const char* qtoken,
                         const char* rtoken,
                         const size_t tokenlen) {

    static const size_t DATASIZE = (sizeof(ShmData) + tokenlen);
    static unsigned int SHMSIZE = (sizeof(ShmMapping) + DATASIZE);

    bool ok = false;
    void* shm = NULL;

    //Join an SHM and get a raw pointer to it:
    shm = SDA::Platform::JoinShm(name);
    ok = (shm != NULL);

    if (ok) {
        ShmMapping* map = NULL;

        //Map a typed pointer to SHM:
        map = reinterpret_cast<ShmMapping*>(shm);
        ok &= (map->size == SHMSIZE);
        if (ok) {
            ShmData* data = NULL;

            //Map a typed pointer to data:
            data = reinterpret_cast<ShmData*>(map->bytes);

            //Check parent's status:
            ok &= (data->ok == true);

            //Check parent's pid:
            ok &= (data->pid == ppid);

            //Check query token:
            for (unsigned int i = 0; i < tokenlen; i++) {
                ok &= (data->token[i] == qtoken[i]);
            }

            //Write check status:
            data->ok = ok;

            //Write process' pid:
            data->pid = getpid();

            //Write response token:
            for (unsigned int i = 0; i < tokenlen; i++) {
                data->token[i] = rtoken[i];
            }

            //Dettach SHM:
            ok &= SDA::Platform::DettachShm(shm, map->size);
        }
    }
}

bool PlatformTest::TestMasterSlaveWithTwoProcesses(const char* const name,
                                                   const char* const fullname) {
    bool ok = false;

    PlatformTest::DataSet dataset;

    void* shm = NULL;

    ok = MasterProcessPrologue(shm, name, fullname, dataset.GetQueryToken(), dataset.GetTokenLen());

    pid_t pid = getpid();
    pid_t cpid = fork();

    if (cpid == -1) {
        ok = false;
    }
    else if (cpid == 0) {
        SlaveProcess(name, fullname, pid, dataset.GetQueryToken(), dataset.GetResponseToken(), dataset.GetTokenLen());
        std::exit(EXIT_SUCCESS);
    }
    else {
        int status;
        if (ok) {
            ok = (waitpid(cpid, &status, 0) == cpid);
            ok &= MasterProcessEpilogue(shm, name, fullname, cpid, dataset.GetResponseToken(), dataset.GetTokenLen());
        }
    }
    return ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool PlatformTest::TestMakeShm(const char* const name,
                               const char* const fullname) {
    bool ok = false;
    ok = TestMasterSlaveWithTwoProcesses(name, fullname);
    return ok;
}

bool PlatformTest::TestJoinShm(const char* const name,
                               const char* const fullname) {
    bool ok = false;
    ok = TestMasterSlaveWithTwoProcesses(name, fullname);
    return ok;
}

bool PlatformTest::TestDettachShm(const char* const name,
                                  const char* const fullname) {
    bool ok = false;
    ok = TestMasterSlaveWithTwoProcesses(name, fullname);
    return ok;
}

bool PlatformTest::TestDestroyShm(const char* const name,
                                  const char* const fullname) {
    bool ok = false;
    ok = TestMasterSlaveWithTwoProcesses(name, fullname);
    return ok;
}
