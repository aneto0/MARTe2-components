/**
 * @file SharedDataAreaTest.cpp
 * @brief Source file for class SharedDataAreaTest
 * @date 25/01/2017
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
 * the class SharedDataAreaTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <sstream>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SharedDataArea.h"
#include "SharedDataAreaTest.h"

#include "Sleep.h"
#include "Threads.h"
//#include "ThreadInformation.h"

template bool SharedDataAreaTest::TestProducerConsumerInSingleThread<int>(const char* const, const unsigned int);
template bool SharedDataAreaTest::TestProducerConsumerInSingleThread<double>(const char* const, const unsigned int);
template bool SharedDataAreaTest::TestProducerConsumerWithTwoThreads<int>(const char* const, const unsigned int);
template bool SharedDataAreaTest::TestProducerConsumerWithTwoThreads<double>(const char* const, const unsigned int);

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

struct DataSet {
	Sigblock** items;
	unsigned int size;
	DataSet(const unsigned int size):
		items(new Sigblock*[size]),
		size(size) {
	}
	~DataSet() {
		delete[] items;
	}
};

struct SharedContext {
	const unsigned int numberOfSignals;
	bool producerThreadEnd;  //Assumption: bool is atomic on Intel
	bool consumerThreadEnd;  //Assumption: bool is atomic on Intel
	bool consumerThreadSuccess;  //Assumption: bool is atomic on Intel
	DataSet dataset;
	SharedContext(const unsigned int numberOfSignals, const unsigned int maxTests) :
		numberOfSignals(numberOfSignals),
		producerThreadEnd(false),
		consumerThreadEnd(false),
		consumerThreadSuccess(false),
		dataset(maxTests)
	{
	}
	~SharedContext() {
	}
};

struct ProducerThreadParams {
	SharedDataArea::SigblockProducer* output;
	SharedContext* context;
	ProducerThreadParams():
		output(NULL_PTR(SharedDataArea::SigblockProducer*)),
		context(NULL_PTR(SharedContext*)) {
	}
};

struct ConsumerThreadParams {
	SharedDataArea::SigblockConsumer* input;
	SharedContext* context;
	ConsumerThreadParams():
		input(NULL_PTR(SharedDataArea::SigblockConsumer*)),
		context(NULL_PTR(SharedContext*)) {
	}
};

static Sigblock* MallocSigblock(std::size_t size) {
	char* mem = new char[size];
	std::memset(mem, '\0', size);
	return reinterpret_cast<Sigblock*>(mem);
}

static void FreeSigblock(Sigblock* sb) {
	char* mem = reinterpret_cast<char*>(sb);
	delete[] mem;
}

template<typename SignalType>
static void InitSigblock(Sigblock* sigblock, const unsigned int numberOfSignals, const SignalType seedValue) {
	char* rawPointer = reinterpret_cast<char*>(sigblock);
	SignalType generatedValue = seedValue;
	unsigned int index = 0;
	while (index < numberOfSignals) {
		SignalType* typedPointer = reinterpret_cast<SignalType*>(rawPointer);
		*typedPointer = generatedValue;
		rawPointer += sizeof(SignalType);
		generatedValue++;
		index++;
	}
}

template<typename SignalType>
static void InitDataSet(DataSet& dataset, std::size_t sigblockSize, const unsigned int numberOfSignals) {
	SignalType seedValue = 0;
	for (unsigned int i = 0; i < dataset.size; i++) {
		InitSigblock<SignalType>(dataset.items[i], numberOfSignals, seedValue);
		seedValue += static_cast<SignalType>(numberOfSignals);
	}
	printf("InitDataSet :: Generated %u sigblocks on dataset\n", dataset.size);
}

static void MallocDataSet(DataSet& dataset, std::size_t sigblockSize) {
	for (unsigned int i = 0; i < dataset.size; i++) {
		dataset.items[i] = MallocSigblock(sigblockSize);
	}
}

static void FreeDataSet(DataSet& dataset) {
	for (unsigned int i = 0; i < dataset.size; i++) {
		FreeSigblock(dataset.items[i]);
	}
}

template<typename SignalType>
static void GenerateMetadataForSigblock(Signal::Metadata sbd[], const unsigned int numberOfSignals) {
	for (unsigned int  i = 0u; (i < numberOfSignals); i++) {
		std::stringstream name;
		name << "Signal" << i;
		std::strncpy(sbd[i].name, name.str().c_str(), Signal::Metadata::NAME_MAX_LEN);
		sbd[i].size = sizeof(SignalType);
	}
}

/**
 * @brief search on dataset from last found
 */
static void SearchSigblockIntoDataSet(DataSet& dataset, Sigblock* sigblock, std::size_t sigblockSize, unsigned int& dataSetIndex, bool& sigblockFound) {
	sigblockFound = false;
	while (!sigblockFound && dataSetIndex < dataset.size) {
		sigblockFound = (std::memcmp(sigblock, dataset.items[dataSetIndex], sigblockSize) == 0);
		dataSetIndex++;
	}
}

static void producerThreadFunction(ProducerThreadParams* params) {
	//Write all the sigblocks of the dataset to the shared data area,
	//pausing 10ms every eight iterations to slow down production:
	for (unsigned int i = 0; i < params->context->dataset.size; i++) {
		params->output->WriteSigblock(*(params->context->dataset.items[i]));
		printf("producerThreadFunction:: serverProducer->WriteSigblock(*(params->context->dataset.items[%u]));\n", i);
		if (i % 8 == 0) {
			MARTe::Sleep::Sec(0.01);
		}
	}

	//Set thread's state as finished:
	params->context->producerThreadEnd = true;

	//End thread's life:
	MARTe::Threads::EndThread();
}

static void consumerThreadFunction(ConsumerThreadParams* params) {
	bool errorFound = false;
	unsigned int consumerThreadReads = 0;
	std::size_t size;

	printf("consumerThreadFunction :: after allocating sigblock\n");

	// Read all the sigblocks written by the producer to the shared data area
	// and check if they have the same values than those from the dataset. It
	// will do a busy wait every time there is no new sigblocks on the shared
	// data area.
	{
		Sigblock* sigblock = NULL_PTR(Sigblock*);
		bool isSigblockFilled = false;
		unsigned int dataSetIndex = 0;

		//Get sigblock's size:
		size = params->input->GetSigblockMetadata()->GetTotalSize();

		//Allocate memory for sigblock:
		sigblock = MallocSigblock(size);

		//Read sigblocks while producer is running:
		while (!errorFound && !params->context->producerThreadEnd) {
			do {
				isSigblockFilled = params->input->ReadSigblock(*sigblock);
			} while (!isSigblockFilled && !params->context->producerThreadEnd);
			if (isSigblockFilled) {
				bool sigblockFound;
				consumerThreadReads++;
				SearchSigblockIntoDataSet(params->context->dataset, sigblock, size, dataSetIndex, sigblockFound);
				printf("consumerThreadFunction while producerThreadFunction is running:: dataSetIndex=%u sigblockFound=%u\n", dataSetIndex, sigblockFound);
				errorFound = !sigblockFound;
			}
			printf("consumerThreadFunction :: loop isSigblockFilled=%u\n", isSigblockFilled);
		}

		//Read last sigblock after producer has stopped:
		if (!errorFound && params->context->producerThreadEnd) {
			isSigblockFilled = params->input->ReadSigblock(*sigblock);
			if (isSigblockFilled) {
				bool sigblockFound;
				consumerThreadReads++;
				SearchSigblockIntoDataSet(params->context->dataset, sigblock, size, dataSetIndex, sigblockFound);
				printf("consumerThreadFunction after producerThreadFunction has stopped:: dataSetIndex=%u sigblockFound=%u\n", dataSetIndex, sigblockFound);
				errorFound = !sigblockFound;
			}
			printf("consumerThreadFunction :: if isSigblockFilled=%u\n", isSigblockFilled);
		}

		//Free memory for sigblock:
		FreeSigblock(sigblock);
	}

	//Set thread's success status:
	params->context->consumerThreadSuccess = (!errorFound && (consumerThreadReads > 0));

	//Set thread's state as finished:
	params->context->consumerThreadEnd = true;

	//End thread's life:
	MARTe::Threads::EndThread();
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool SharedDataAreaTest::TestConstructor() {
	return true;
}

template<typename SignalType>
bool SharedDataAreaTest::TestProducerConsumerInSingleThread(const char* const shmName, const unsigned int maxTests) {
	DataSet dataset(maxTests);
	bool success = false;
	unsigned int max = 512; //capacity of the buffer (UINT_MAX+1 must be evenly divisible by max) UINT_MAX==4294967295
	const unsigned int numberOfSignals = 10;
	Signal::Metadata sbd[numberOfSignals];
	SharedDataArea sdaServer;
	SharedDataArea::SigblockProducer* producer;
	SharedDataArea sdaClient;
	SharedDataArea::SigblockConsumer* consumer;
	std::size_t size;

	//Check SHM's name:
	success = (std::strcmp(shmName, "") != 0);

	//Generate the metadata of the sigblocks to use in test:
	GenerateMetadataForSigblock<SignalType>(sbd, numberOfSignals);

	//Setup producer's interface to shared data area:
	sdaServer = SharedDataArea::BuildSharedDataAreaForMARTe(shmName, numberOfSignals, sbd, max);
	producer = sdaServer.GetSigblockProducerInterface();

	//Setup consumer's interface to shared data area:
	sdaClient = SharedDataArea::BuildSharedDataAreaForEPICS(shmName);
	consumer = sdaClient.GetSigblockConsumerInterface();

	//Get sigblock's size:
	size = producer->GetSigblockMetadata()->GetTotalSize();

	//Check coherence of size:
	success &= (producer->GetSigblockMetadata()->GetTotalSize() == consumer->GetSigblockMetadata()->GetTotalSize());

	//Allocate memory for dataset:
	MallocDataSet(dataset, size);

	//Initialize items of dataset:
	InitDataSet<SignalType>(dataset, size, numberOfSignals);

	// Write all the sigblocks of the dataset to the shared data area, checking
	// that they can be read and have the same values than those from the
	// dataset. They will be written and read taking turns (1 write, 1 read).
	{
		Sigblock* sigblock = NULL_PTR(Sigblock*);
		unsigned int i = 0;
		bool error = false;

		//Allocate memory for sigblock:
		sigblock = MallocSigblock(size);

		//Write and read sigblocks taking turns:
		while (i < maxTests && !error) {
			bool writingSucceeded;
			writingSucceeded = producer->WriteSigblock(*(dataset.items[i]));
			if (writingSucceeded) {
				bool readingSucceeded;
				readingSucceeded = consumer->ReadSigblock(*sigblock);
				if (readingSucceeded) {
					error = (std::memcmp(sigblock, dataset.items[i], size) != 0);
				}
				else {
					error = true;
				}
			}
			else {
				error = true;
			}
			printf("TestProducerConsumerWithSingleThread :: Write/Read dataset.items[%u] error=%u\n", i, error);
			i++;
		}

		//Free memory for sigblock:
		FreeSigblock(sigblock);

		//Check execution's status:
		success &= !error;
	}

	//Free memory of dataset:
	FreeDataSet(dataset);

	//Return test's execution status:
	return success;
}

template<typename SignalType>
bool SharedDataAreaTest::TestProducerConsumerWithTwoThreads(const char* const shmName, const unsigned int maxTests) {
	//How to achieve different paces: Configure consumer with less priority than producer or make consumer sleep often ...
	//And latency recording??

	bool success = false;
	const unsigned int max = 512; //capacity of the buffer (UINT_MAX+1 must be evenly divisible by max) UINT_MAX==4294967295
	SharedContext context(10, maxTests);
	ProducerThreadParams producerThreadParams;
	ConsumerThreadParams consumerThreadParams;
	Signal::Metadata sbd[context.numberOfSignals];
	SharedDataArea sdaServer;
	SharedDataArea::SigblockProducer* producer;
	SharedDataArea sdaClient;
	SharedDataArea::SigblockConsumer* consumer;
	std::size_t size;

	//Check SHM's name:
	success = (std::strcmp(shmName, "") != 0);

	//Generate the metadata of the sigblocks to use in test:
	GenerateMetadataForSigblock<SignalType>(sbd, context.numberOfSignals);

	//Setup producer's interface to shared data area:
	sdaServer = SharedDataArea::BuildSharedDataAreaForMARTe(shmName, context.numberOfSignals, sbd, max);
	producer = sdaServer.GetSigblockProducerInterface();

	//Setup producer's parameters:
	producerThreadParams.output = producer;
	producerThreadParams.context = &context;

	//Setup consumer's interface to shared data area:
	sdaClient = SharedDataArea::BuildSharedDataAreaForEPICS(shmName);
	consumer = sdaClient.GetSigblockConsumerInterface();

	//Setup consumer's parameters:
	consumerThreadParams.input = consumer;
	consumerThreadParams.context = &context;

	//Get sigblock's size:
	size = producer->GetSigblockMetadata()->GetTotalSize();

	//Check coherence of size:
	success &= (producer->GetSigblockMetadata()->GetTotalSize() == consumer->GetSigblockMetadata()->GetTotalSize());

	//Allocate memory for dataset:
	MallocDataSet(context.dataset, size);

	//Initialize items of dataset:
	InitDataSet<SignalType>(context.dataset, size, context.numberOfSignals);

	//Start producer's thread linked to producerThreadFunction:
	/*ThreadIdentifier producerThreadId =*/ MARTe::Threads::BeginThread((MARTe::ThreadFunctionType) producerThreadFunction, &producerThreadParams);

	//Start consumer's thread linked to consumerThreadFunction:
	/*ThreadIdentifier consumerThreadId =*/ MARTe::Threads::BeginThread((MARTe::ThreadFunctionType) consumerThreadFunction, &consumerThreadParams);

	//Busy wait until both threads end:
	while (!context.producerThreadEnd || !context.consumerThreadEnd) {};

	//Free memory of dataset:
	FreeDataSet(context.dataset);

	//Check execution's status:
	success &= context.consumerThreadSuccess;

	//Return test's execution status:
	return success;
}
