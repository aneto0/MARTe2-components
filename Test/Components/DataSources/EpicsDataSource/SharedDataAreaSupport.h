/**
 * @file SharedDataAreaSupport.h
 * @brief Header file for class SharedDataAreaSupport
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

 * @details This header file contains the declaration of the class SharedDataAreaSupport
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SHAREDDATAAREASUPPORT_H_
#define SHAREDDATAAREASUPPORT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <sstream>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
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

Sigblock* MallocSigblock(std::size_t size);

void FreeSigblock(Sigblock* sb);

template<typename SignalType>
void InitSigblock(Sigblock* sigblock, const unsigned int numberOfSignals, const SignalType seedValue);

template<typename SignalType>
void GenerateMetadataForSigblock(Signal::Metadata sbd[], const unsigned int numberOfSignals);

void MallocDataSet(DataSet& dataset, std::size_t sigblockSize);

void FreeDataSet(DataSet& dataset);

template<typename SignalType>
void InitDataSet(DataSet& dataset, const unsigned int numberOfSignals);

/**
 * @brief search on dataset from last found
 */
void SearchSigblockIntoDataSet(DataSet& dataset, Sigblock* sigblock, std::size_t sigblockSize, unsigned int& dataSetIndex, bool& sigblockFound);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline Sigblock* MallocSigblock(std::size_t size) {
	char* mem = new char[size];
	std::memset(mem, '\0', size);
	return reinterpret_cast<Sigblock*>(mem);
}

inline void FreeSigblock(Sigblock* sb) {
	char* mem = reinterpret_cast<char*>(sb);
	delete[] mem;
}

template<typename SignalType>
void InitSigblock(Sigblock* sigblock, const unsigned int numberOfSignals, const SignalType seedValue) {
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
void GenerateMetadataForSigblock(Signal::Metadata sbd[], const unsigned int numberOfSignals) {
	for (unsigned int  i = 0u; (i < numberOfSignals); i++) {
		std::stringstream name;
		name << "Signal" << i;
		std::strncpy(sbd[i].name, name.str().c_str(), Signal::Metadata::NAME_MAX_LEN);
		sbd[i].size = sizeof(SignalType);
	}
}

inline void MallocDataSet(DataSet& dataset, std::size_t sigblockSize) {
	for (unsigned int i = 0; i < dataset.size; i++) {
		dataset.items[i] = MallocSigblock(sigblockSize);
	}
}

inline void FreeDataSet(DataSet& dataset) {
	for (unsigned int i = 0; i < dataset.size; i++) {
		FreeSigblock(dataset.items[i]);
	}
}

template<typename SignalType>
void InitDataSet(DataSet& dataset, const unsigned int numberOfSignals) {
	SignalType seedValue = 0;
	for (unsigned int i = 0; i < dataset.size; i++) {
		InitSigblock<SignalType>(dataset.items[i], numberOfSignals, seedValue);
		seedValue += static_cast<SignalType>(numberOfSignals);
	}
	printf("InitDataSet :: Generated %u sigblocks on dataset\n", dataset.size);
}

inline void SearchSigblockIntoDataSet(DataSet& dataset, Sigblock* sigblock, std::size_t sigblockSize, unsigned int& dataSetIndex, bool& sigblockFound) {
	sigblockFound = false;
	while (!sigblockFound && dataSetIndex < dataset.size) {
		sigblockFound = (std::memcmp(sigblock, dataset.items[dataSetIndex], sigblockSize) == 0);
		dataSetIndex++;
	}
}

#endif /* SHAREDDATAAREASUPPORT_H_ */