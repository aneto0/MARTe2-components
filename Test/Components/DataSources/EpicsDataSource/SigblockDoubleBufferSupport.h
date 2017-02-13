/**
 * @file SigblockDoubleBufferSupport.h
 * @brief Header file for class SigblockDoubleBufferSupport
 * @date 03/02/2017
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

 * @details This header file contains the declaration of the class SigblockDoubleBufferSupport
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIGBLOCKDOUBLEBUFFERSUPPORT_H_
#define SIGBLOCKDOUBLEBUFFERSUPPORT_H_

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

#include "CompilerTypes.h"
#include "SigblockDoubleBuffer.h"
#include "SigblockSupport.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

struct DataSet {
	SDA::Sigblock** items;
	unsigned int size;
	DataSet(const unsigned int size);
	~DataSet();
};

void MallocDataSet(DataSet& dataset, std::size_t sigblockSize);

void FreeDataSet(DataSet& dataset);

template<typename SignalType>
void InitDataSet(DataSet& dataset, const unsigned int numberOfSignals);

/**
 * @brief search on dataset from last found
 */
void SearchSigblockIntoDataSet(DataSet& dataset, SDA::Sigblock* sigblock, std::size_t sigblockSize, unsigned int& dataSetIndex, bool& sigblockFound);

SigblockDoubleBuffer* MallocSigblockDoubleBuffer(std::size_t sizeOfSigblock);

void FreeSigblockDoubleBuffer(SigblockDoubleBuffer*& sbdb);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline DataSet::DataSet(const unsigned int size):
	items(new SDA::Sigblock*[size]),
	size(size) {
}

inline DataSet::~DataSet() {
	delete[] items;
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

inline void SearchSigblockIntoDataSet(DataSet& dataset, SDA::Sigblock* sigblock, std::size_t sigblockSize, unsigned int& dataSetIndex, bool& sigblockFound) {
	sigblockFound = false;
	while (!sigblockFound && dataSetIndex < dataset.size) {
		sigblockFound = (std::memcmp(sigblock, dataset.items[dataSetIndex], sigblockSize) == 0);
		dataSetIndex++;
	}
}

inline SigblockDoubleBuffer* MallocSigblockDoubleBuffer(std::size_t sizeOfSigblock) {
	size_t fullsize = (sizeof(SigblockDoubleBuffer) + (sizeOfSigblock * 2));
	char* mem = new char[fullsize];
	std::memset(mem, '\0', fullsize);
	return reinterpret_cast<SigblockDoubleBuffer*>(mem);
}

inline void FreeSigblockDoubleBuffer(SigblockDoubleBuffer*& sbdb) {
	char* mem = reinterpret_cast<char*>(sbdb);
	delete[] mem;
	sbdb = NULL;
}

#endif /* SIGBLOCKDOUBLEBUFFERSUPPORT_H_ */
