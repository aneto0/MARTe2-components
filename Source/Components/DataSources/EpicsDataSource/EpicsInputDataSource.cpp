/**
 * @file EpicsInputDataSource.cpp
 * @brief Source file for class EpicsInputDataSource
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
 * the class EpicsInputDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <cstdio>
#include <cstring>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "EpicsInputDataSource.h"

#include "AdvancedErrorManagement.h"
#include "FastPollingMutexSem.h"
#include "HeapManager.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "SharedDataArea.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

/**
 * @brief Gets the application's name.
 */
static MARTe::StreamString GetApplicationName() {
	using namespace MARTe;
	StreamString result;
    ObjectRegistryDatabase *objDb = NULL;
    objDb = ObjectRegistryDatabase::Instance();
    //TODO: Check objDb
    uint32 nOfObjs = objDb->Size();
    uint32 i;
    bool found = false;
    for (i = 0u; (i < nOfObjs) && (!found); i++) {
    	ReferenceT < RealTimeApplication > rtApp = objDb->Get(i);
    	found = rtApp.IsValid();
    	if (found) {
    		result = rtApp->GetName();
    	}
    }
	return result;
}

/**
 * @brief Builds the name for the shared memory area, based on application's
 * name and name passed as parameter.
 */
static MARTe::StreamString BuildSharedMemoryIdentifier(const MARTe::StreamString& name) {
	using namespace MARTe;
	StreamString result;
	//Add the initial mark:
	result += "/";
	//Add the MARTe's application name as prefix:
	result += GetApplicationName();
	//Add a separator mark:
	result += "_";
	//Add the name as last token:
	result += name;
	return result;
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

EpicsInputDataSource::EpicsInputDataSource() :
    DataSourceI(), consumer(NULL_PTR(SharedDataArea::SigblockConsumer*)), signals(NULL_PTR(Sigblock*)) {
}

EpicsInputDataSource::~EpicsInputDataSource() {
    printf("EpicsInputDataSource::~EpicsInputDataSource()\n");
    if (signals != NULL_PTR(Sigblock*)) {
    	void* mem = reinterpret_cast<void*>(signals);
		HeapManager::Free(mem);
		signals = static_cast<Sigblock*>(mem);
    }
    if (consumer != NULL_PTR(SharedDataArea::SigblockConsumer*)) {
    	//TODO: Release interprocess shared memory?
    	consumer = NULL_PTR(SharedDataArea::SigblockConsumer*);
    }
}

bool EpicsInputDataSource::Synchronise() {
	bool ok;
	ok = consumer->ReadSigblock(*signals);
	return ok;
}

bool EpicsInputDataSource::AllocateMemory() {
	bool ret = true;
	uint32 numberOfSignals = GetNumberOfSignals();
    StreamString shmId = BuildSharedMemoryIdentifier(GetName());
	unsigned int max = 512; //capacity of the buffer (UINT_MAX+1 must be evenly divisible by max) UINT_MAX==4294967295
	Signal::Metadata smd_for_init[numberOfSignals]; //sigblock description for initialization;

	//{for all signals in datasource add it to smd_for_init}
	ret = (numberOfSignals > 0u);
	for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
		uint32 memorySize;
		ret = GetSignalByteSize(i, memorySize);
		if (ret) {

			StreamString signalName;
			GetSignalName(i, signalName);

			std::strncpy(smd_for_init[i].name, signalName.Buffer(), Signal::Metadata::NAME_MAX_LEN);  //TODO Use MARTe strcpy
			smd_for_init[i].size = memorySize;
		}
	}

	SharedDataArea sbpm = SharedDataArea::BuildSharedDataAreaForMARTe(shmId.Buffer(), numberOfSignals, smd_for_init, max);
	consumer = sbpm.GetSigblockConsumerInterface();
	Sigblock::Metadata* sbmd = consumer->GetSigblockMetadata();
	void* mem = HeapManager::Malloc(sbmd->GetTotalSize());
	MemoryOperationsHelper::Set(mem, '\0', sbmd->GetTotalSize());
	signals = static_cast<Sigblock*>(mem);

	return ret;
}

uint32 EpicsInputDataSource::GetNumberOfMemoryBuffers() {
    return 0u;
}

bool EpicsInputDataSource::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
	Sigblock::Metadata* sbmd = consumer->GetSigblockMetadata();
	signalAddress = signals->GetSignalAddress(sbmd->GetSignalOffsetByIndex(signalIdx));
	REPORT_ERROR_PARAMETERS(ErrorManagement::Debug, "*** EpicsInputDataSource::GetSignalMemoryBuffer (v2) GetName()=%s signalAddress=%p signalIdx=%u offset=%i***\n", GetName(), signalAddress, signalIdx, sbmd->GetSignalOffsetByIndex(signalIdx));
	return true;
}

const char8 *EpicsInputDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == InputSignals) {
       return "MemoryMapSynchronisedInputBroker";
    }
    else {
       return "";
    }
}

bool EpicsInputDataSource::GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr) {
    ReferenceT<MemoryMapSynchronisedInputBroker> broker("MemoryMapSynchronisedInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool EpicsInputDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr) {
	return false;
}

bool EpicsInputDataSource::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);
    return ret;
}

bool EpicsInputDataSource::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    return true;
}

CLASS_REGISTER(EpicsInputDataSource, "1.0")

}
