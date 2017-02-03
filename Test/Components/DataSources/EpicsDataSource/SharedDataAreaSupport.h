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

#include "CompilerTypes.h"
#include "SharedDataArea.h"
#include "SigblockSupport.h"
#include "SigblockDoubleBufferSupport.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

struct SharedContext {
	const unsigned int numberOfSignals;
	bool producerThreadEnd;  //Assumption: bool is atomic on Intel
	bool consumerThreadEnd;  //Assumption: bool is atomic on Intel
	bool consumerThreadSuccess;  //Assumption: bool is atomic on Intel
	DataSet dataset;
	SharedContext(const unsigned int numberOfSignals, const unsigned int maxTests);
	~SharedContext();
};

struct ProducerThreadParams {
	SharedDataArea::SigblockProducer* output;
	SharedContext* context;
	ProducerThreadParams();
};

struct ConsumerThreadParams {
	SharedDataArea::SigblockConsumer* input;
	SharedContext* context;
	ConsumerThreadParams();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline SharedContext::SharedContext(const unsigned int numberOfSignals, const unsigned int maxTests) :
	numberOfSignals(numberOfSignals),
	producerThreadEnd(false),
	consumerThreadEnd(false),
	consumerThreadSuccess(false),
	dataset(maxTests) {
}

inline SharedContext::~SharedContext() {
}

inline ProducerThreadParams::ProducerThreadParams():
	output(NULL_PTR(SharedDataArea::SigblockProducer*)),
	context(NULL_PTR(SharedContext*)) {
}

inline ConsumerThreadParams::ConsumerThreadParams():
	input(NULL_PTR(SharedDataArea::SigblockConsumer*)),
	context(NULL_PTR(SharedContext*)) {
}

#endif /* SHAREDDATAAREASUPPORT_H_ */
