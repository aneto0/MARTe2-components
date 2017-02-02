/**
 * @file PlatformTest.cpp
 * @brief Source file for class PlatformTest
 * @date 31/01/2017
 * @author Ivan Herrero
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

#include <cstdio>
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

struct ShmMapping {
	size_t size;
	char bytes[];
};

struct ShmData {
	pid_t pid;
	bool ok;
	char token[];
};

static char TOKEN[] = "The quick brown fox jumps over the lazy dog";
static char RTOKEN[] = "god yzal eht revo spmuj xof nworb kciuq ehT";
static const size_t TOKENLEN = std::strlen(TOKEN);

static bool MasterProcessPrologue(void*& shm, const char* const name, const char* const fullname) {

	static const size_t DATASIZE = (sizeof(ShmData) + TOKENLEN);
	static unsigned int SHMSIZE = (sizeof(ShmMapping) + DATASIZE);

	bool ok = false;

	//Create an SHM and get a raw pointer to it:
	shm = Platform::MakeShm(name, SHMSIZE);

	ok = (shm != NULL);
	ok &= (access(fullname, R_OK | W_OK) == 0);

	if (ok) {
		ShmMapping* map = NULL;
		//Map a typed pointer to SHM:
		map = reinterpret_cast<ShmMapping*>(shm);
		ok &= (map->size == SHMSIZE);

		if (ok) {
			ShmData* data = NULL;

			//Map a typed pointer to data:
			data = reinterpret_cast<ShmData*>(map->bytes);

			//Check default token:
			for (unsigned int i = 0; i < TOKENLEN; i++) {
				ok &= (data->token[i] == 72);
			}

			//Write original token:
			for (unsigned int i = 0; i < TOKENLEN; i++) {
				data->token[i] = TOKEN[i];
			}

			//Check original token:
			for (unsigned int i = 0; i < TOKENLEN; i++) {
				ok &= (data->token[i] == TOKEN[i]);
			}
		}
	}
	return ok;
}

static bool MasterProcessEpilogue(void* shm, const char* const name, const char* const fullname, pid_t pid) {

	static const size_t DATASIZE = (sizeof(ShmData) + TOKENLEN);
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

			//Check response token:
			for (unsigned int i = 0; i < TOKENLEN; i++) {
				ok &= (data->token[i] == RTOKEN[i]);
			}

			//check child's pid
			ok &= (data->pid == pid);

			//check child's ok
			ok &= (data->ok == true);

			//Dettach SHM:
			Platform::DettachShm(shm, map->size);

			//Destroy SHM:
			Platform::DestroyShm(name);
			ok &= (access(fullname, R_OK | W_OK) == -1);
		}
	}

	return ok;
}

static void SlaveProcess(const char* const name, const char* const fullname) {

	static const size_t DATASIZE = (sizeof(ShmData) + TOKENLEN);
	static unsigned int SHMSIZE = (sizeof(ShmMapping) + DATASIZE);

	bool ok = false;
	void* shm = NULL;
	//wait until shm ready
	while (access(fullname, F_OK) == -1) {
	}
//	... perhaps not initialized, yet???

	//Join an SHM and get a raw pointer to it:
	shm = Platform::JoinShm(name);
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

			//Check original token:
			for (unsigned int i = 0; i < TOKENLEN; i++) {
				ok &= (data->token[i] == TOKEN[i]);
			}

			//Write response token:
			for (unsigned int i = 0; i < TOKENLEN; i++) {
				data->token[i] = RTOKEN[i];
			}

			//Write PID:
			data->pid = getpid();

			//Write status
			data->ok = ok;

			//Dettach SHM:
			Platform::DettachShm(shm, map->size);
		}
	}
}

//bool TestMasterSlaveInSingleThread(const char* const shmName, const unsigned int maxTests = 100) {
//	void* shmMaster = NULL;
//	void* shmSlave = NULL;
//
//}
//
//bool TestMasterSlaveWithTwoThreads(const char* const shmName, const unsigned int maxTests = 100) {
//
//}

bool TestMasterSlaveWithTwoProcesses(const char* const name, const char* const fullname) {
	bool ok = false;

	void* shm = NULL;

	ok = MasterProcessPrologue(shm, name, fullname);

	pid_t pid = fork();

	if (pid == -1) {
		ok = false;
	}
	else if (pid == 0) {
		SlaveProcess(name, fullname);
		std::exit(EXIT_SUCCESS);
	}
	else {
		int status;
		(void)waitpid(pid, &status, 0);

		ok &= MasterProcessEpilogue(shm, name, fullname, pid);
	}
	return ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool PlatformTest::TestMakeShm(const char* const name, const char* const fullname) {
	bool ok = false;
	ok = TestMasterSlaveWithTwoProcesses(name, fullname);
	return ok;
}

bool PlatformTest::TestJoinShm(const char* const name, const char* const fullname) {
	bool ok = false;
	ok = TestMasterSlaveWithTwoProcesses(name, fullname);
	return ok;
}

bool PlatformTest::TestDettachShm(const char* const name, const char* const fullname) {
	bool ok = false;
	ok = TestMasterSlaveWithTwoProcesses(name, fullname);
	return ok;
}

bool PlatformTest::TestDestroyShm(const char* const name, const char* const fullname) {
	bool ok = false;
	ok = TestMasterSlaveWithTwoProcesses(name, fullname);
	return ok;
}
