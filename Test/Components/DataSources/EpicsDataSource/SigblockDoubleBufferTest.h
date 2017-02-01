/**
 * @file SigblockDoubleBufferTest.h
 * @brief Header file for class SigblockDoubleBufferTest
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

 * @details This header file contains the declaration of the class SigblockDoubleBufferTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIGBLOCKDOUBLEBUFFERTEST_H_
#define SIGBLOCKDOUBLEBUFFERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class SigblockDoubleBufferTest {
public:
	/**
	 * @brief TODO
	 */
	bool TestReset();

	/**
	 * @brief TODO
	 */
	bool TestGet();

	/**
	 * @brief TODO
	 */
	bool TestPut();

	/**
	 * @brief TODO
	 */
	template<typename SignalType>
	bool TestProducerConsumerInSingleThread(const char* const shmName, const unsigned int maxTests);

private:
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#include "SharedDataAreaSupport.h"

template bool SigblockDoubleBufferTest::TestProducerConsumerInSingleThread<int>(const char* const, const unsigned int);
template bool SigblockDoubleBufferTest::TestProducerConsumerInSingleThread<double>(const char* const, const unsigned int);

template<typename SignalType>
bool SigblockDoubleBufferTest::TestProducerConsumerInSingleThread(const char* const shmName, const unsigned int maxTests) {
	DataSet dataset(maxTests);
	bool success = false;
//!!!!!!!!!!	unsigned int max = 512; //capacity of the buffer (UINT_MAX+1 must be evenly divisible by max) UINT_MAX==4294967295
	const unsigned int numberOfSignals = 10;
	Signal::Metadata sbmd[numberOfSignals];
//	SharedDataArea sdaServer;
//	SharedDataArea::SigblockProducer* producer;
//	SharedDataArea sdaClient;
//	SharedDataArea::SigblockConsumer* consumer;
	std::size_t size;

	//////////////////////////////////
	char* rawmem;
	SigblockDoubleBuffer* producerBis;
	SigblockDoubleBuffer* consumerBis;
	//////////////////////////////////

	//Check SHM's name:
	success = (std::strcmp(shmName, "") != 0);

	//Generate the metadata of the sigblocks to use in test:
	GenerateMetadataForSigblock<SignalType>(sbmd, numberOfSignals);

	//////////////////////////////////
	rawmem = new char[1000];
	Sigblock::Metadata* header = reinterpret_cast<Sigblock::Metadata*>(rawmem);
	header->SetSignalsMetadata(numberOfSignals, sbmd);

	rawmem = new char[1000];
	SigblockDoubleBuffer* items = reinterpret_cast<SigblockDoubleBuffer*>(rawmem);
	items->Reset(0, header->GetTotalSize());
	producerBis = items;
	consumerBis = items;
	//////////////////////////////////

	//Setup producer's interface to shared data area:
//	sdaServer = SharedDataArea::BuildSharedDataAreaForMARTe(shmName, numberOfSignals, sbmd, max);
//	producer = sdaServer.GetSigblockProducerInterface();

	//Setup consumer's interface to shared data area:
//	sdaClient = SharedDataArea::BuildSharedDataAreaForEPICS(shmName);
//	consumer = sdaClient.GetSigblockConsumerInterface();

	//Get sigblock's size:
//	size = producer->GetSigblockMetadata()->GetTotalSize();

	//////////////////////////////////
	size = header->GetTotalSize();
	//////////////////////////////////

	//Check coherence of size:
//	success &= (producer->GetSigblockMetadata()->GetTotalSize() == consumer->GetSigblockMetadata()->GetTotalSize());

	//Allocate memory for dataset:
	MallocDataSet(dataset, size);

	//Initialize items of dataset:
	InitDataSet<SignalType>(dataset, numberOfSignals);

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
//			writingSucceeded = producer->WriteSigblock(*(dataset.items[i]));
			//////////////////////////////////
			writingSucceeded = producerBis->Put(*(dataset.items[i]));
			//////////////////////////////////
			if (writingSucceeded) {
				bool readingSucceeded;
//				readingSucceeded = consumer->ReadSigblock(*sigblock);
				//////////////////////////////////
				readingSucceeded = consumerBis->Get(*sigblock);
				//////////////////////////////////
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

	//////////////////////////////////
	delete[] rawmem;
	//////////////////////////////////

	//Free memory of dataset:
	FreeDataSet(dataset);

	//Return test's execution status:
	return success;
}

#endif /* SIGBLOCKDOUBLEBUFFERTEST_H_ */
