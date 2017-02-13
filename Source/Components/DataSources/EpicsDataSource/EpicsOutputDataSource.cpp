/**
 * @file EpicsOutputDataSource.cpp
 * @brief Source file for class EpicsOutputDataSource
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
 * the class EpicsOutputDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <cstdio>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "EpicsOutputDataSource.h"

#include "AdvancedErrorManagement.h"
#include "FastPollingMutexSem.h"
#include "HeapManager.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "SharedDataArea.h"
#include "StringHelper.h"
#include "Threads.h"
#include "Types.h"

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
    if (!found) {
		result = "MARTeApp";
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

EpicsOutputDataSource::EpicsOutputDataSource() :
    DataSourceI(), producer(NULL_PTR(SDA::SharedDataArea::SigblockProducer*)), signals(NULL_PTR(SDA::Sigblock*)) {
}

EpicsOutputDataSource::~EpicsOutputDataSource() {
//    printf("EpicsOutputDataSource::~EpicsOutputDataSource()\n");
    if (signals != NULL_PTR(SDA::Sigblock*)) {
    	void* mem = reinterpret_cast<void*>(signals);
		HeapManager::Free(mem);
		signals = NULL_PTR(SDA::Sigblock*); //static_cast<SDA::Sigblock*>(mem);
    }
    if (producer != NULL_PTR(SDA::SharedDataArea::SigblockProducer*)) {
    	//TODO: Release interprocess shared memory?
    	producer = NULL_PTR(SDA::SharedDataArea::SigblockProducer*);
    }
}

bool EpicsOutputDataSource::Synchronise() {
	bool ok = true;
	ok = producer->WriteSigblock(*signals);
	return ok;
}

bool EpicsOutputDataSource::AllocateMemory() {
	bool ret = true;
	uint32 numberOfSignals = GetNumberOfSignals();
	sharedDataAreaName = BuildSharedMemoryIdentifier(GetName());
	SDA::uint32 max = 512u; //capacity of the buffer (UINT_MAX+1 must be evenly divisible by max) UINT_MAX==4294967295
	SDA::Signal::Metadata smd_for_init[numberOfSignals]; //sigblock description for initialization;

	//{for all signals in datasource add it to smd}
	ret = (numberOfSignals > 0u);
	for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
		uint32 memorySize;
		ret = GetSignalByteSize(i, memorySize);
		if (ret) {

			StreamString signalName;
			GetSignalName(i, signalName);

			MARTe::StringHelper::CopyN(smd_for_init[i].name, signalName.Buffer(), SDA::Signal::Metadata::NAME_MAX_LEN);
			smd_for_init[i].size = memorySize;
		}
	}

	SDA::SharedDataArea sbpm = SDA::SharedDataArea::BuildSharedDataAreaForMARTe(sharedDataAreaName.Buffer(), numberOfSignals, smd_for_init, max);
	producer = sbpm.GetSigblockProducerInterface();
	SDA::Sigblock::Metadata* sbmd = producer->GetSigblockMetadata();
	void* mem = HeapManager::Malloc(sbmd->GetTotalSize());
	MemoryOperationsHelper::Set(mem, '\0', sbmd->GetTotalSize());
	signals = static_cast<SDA::Sigblock*>(mem);

	return ret;
}

uint32 EpicsOutputDataSource::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool EpicsOutputDataSource::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
	bool ok = false;

	ok = ((signalIdx < GetNumberOfSignals()) && (bufferIdx < GetNumberOfMemoryBuffers()));

    if (ok) {
    	SDA::Sigblock::Metadata* sbmd = producer->GetSigblockMetadata();
    	signalAddress = signals->GetSignalAddress(sbmd->GetSignalOffsetByIndex(signalIdx));
//    	REPORT_ERROR_PARAMETERS(ErrorManagement::Debug, "*** EpicsOutputDataSource::GetSignalMemoryBuffer (v2) GetName()=%s signalAddress=%p signalIdx=%u offset=%i***\n", GetName(), signalAddress, signalIdx, sbmd->GetSignalOffsetByIndex(signalIdx));
    }

    return ok;
}

const char8 *EpicsOutputDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == InputSignals) {
    	brokerName = "";
    }
    else {
    	brokerName = "MemoryMapSynchronisedOutputBroker";
    }
    return brokerName;
}

bool EpicsOutputDataSource::GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr) {
	return false;
}

bool EpicsOutputDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr) {
	ReferenceT<MemoryMapSynchronisedOutputBroker> broker("MemoryMapSynchronisedOutputBroker");
	bool ret = broker.IsValid();
	if (ret) {
		ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
	}
	if (ret) {
		ret = outputBrokers.Insert(broker);
	}
	return ret;
}

bool EpicsOutputDataSource::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    return true;
}

MARTe::StreamString EpicsOutputDataSource::GetSharedDataAreaName() const {
	return sharedDataAreaName;
}

CLASS_REGISTER(EpicsOutputDataSource, "1.0")

}
