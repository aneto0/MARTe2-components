/**
 * @file SigblockDoubleBufferTest.cpp
 * @brief Source file for class SigblockDoubleBufferTest
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
 * the class SigblockDoubleBufferTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SigblockDoubleBuffer.h"
#include "SigblockDoubleBufferTest.h"
#include "SigblockDoubleBufferSupport.h"
#include "SigblockSupport.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

template bool SigblockDoubleBufferTest::TestProducerConsumerInSingleThread<int>(const unsigned int,
                                                                                const unsigned int);
template bool SigblockDoubleBufferTest::TestProducerConsumerInSingleThread<double>(const unsigned int,
                                                                                   const unsigned int);

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool SigblockDoubleBufferTest::TestInit() {
    bool ok = false;
    ok = TestProducerConsumerInSingleThread<int>(10, 25);
    ok &= TestProducerConsumerInSingleThread<double>(10, 25);
    return ok;
}

bool SigblockDoubleBufferTest::TestGet() {
    bool ok = false;
    ok = TestProducerConsumerInSingleThread<int>(10, 25);
    ok &= TestProducerConsumerInSingleThread<double>(10, 25);
    return ok;
}

bool SigblockDoubleBufferTest::TestPut() {
    bool ok = false;
    ok = TestProducerConsumerInSingleThread<int>(10, 25);
    ok &= TestProducerConsumerInSingleThread<double>(10, 25);
    return ok;
}

template<typename SignalType>
bool SigblockDoubleBufferTest::TestProducerConsumerInSingleThread(const unsigned int numberOfSignals,
                                                                  const unsigned int maxTests) {
    DataSet dataset(maxTests);
    bool ok = false;
    SDA::Signal::Metadata rawMetadata[numberOfSignals];
    SDA::Sigblock::Metadata* metadata;
    std::size_t sizeOfSigblock;
    SDA::SigblockDoubleBuffer* buffer;
    SDA::SigblockDoubleBuffer* producer;
    SDA::SigblockDoubleBuffer* consumer;

    //Allocate memory for sigblock's metadata:
    metadata = MallocSigblockMetadata(numberOfSignals);

    //Check allocation of sigblock's metadata:
    ok = (metadata != NULL);

    if (ok) {

        //Generate the metadata for the sigblocks to use in test:
        GenerateMetadataForSigblock<SignalType>(rawMetadata, numberOfSignals);

        //Init testing metadata values:
        metadata->Init(numberOfSignals, rawMetadata);

        //Get sigblock's size:
        sizeOfSigblock = metadata->GetTotalSize();

        //Allocate memory for dataset:
        MallocDataSet(dataset, sizeOfSigblock);

        //Initialize items of dataset:
        InitDataSet<SignalType>(dataset, numberOfSignals);

        //Allocate memory for shared sigblock double buffer:
        buffer = MallocSigblockDoubleBuffer(sizeOfSigblock);

        //Check allocation of shared sigblock double buffer:
        ok = (buffer != NULL);

        if (ok) {

            //Initialize shared sigblock double buffer:
            buffer->Init(sizeOfSigblock);

            //Setup producer's pointer to shared sigblock double buffer:
            producer = buffer;

            //Setup consumer's pointer to shared sigblock double buffer:
            consumer = buffer;

            //Write all the sigblocks of the dataset to the shared sigblock
            //double buffer, checking that they can be read and have the same
            //values than those from the dataset. They will be written and
            //read taking turns (1 write, 1 read).
            {
                SDA::Sigblock* sigblock = NULL;
                unsigned int i = 0;
                bool error = false;

                //Allocate memory for sigblock:
                sigblock = MallocSigblock(sizeOfSigblock);

                //Check allocation of sigblock:
                ok = (sigblock != NULL);

                if (ok) {
                    //Write and read sigblocks taking turns:
                    while (i < maxTests && !error) {
                        bool writingSucceeded;
                        writingSucceeded = producer->Put(*(dataset.items[i]));
                        if (writingSucceeded) {
                            bool readingSucceeded;
                            readingSucceeded = consumer->Get(*sigblock);
                            if (readingSucceeded) {
                                error = (std::memcmp(sigblock, dataset.items[i], sizeOfSigblock) != 0);
                            }
                            else {
                                error = true;
                            }
                        }
                        else {
                            error = true;
                        }
                        i++;
                    }
                }

                //Free memory for sigblock:
                FreeSigblock(sigblock);

                //Check execution's status:
                ok &= !error;
            }
        }

        //Free memory of shared sigblock double buffer:
        FreeSigblockDoubleBuffer(buffer);

        //Free memory of dataset:
        FreeDataSet(dataset);

    }

    //Free memory of sigblock's metadata:
    FreeSigblockMetadata(metadata);

    //Return test's execution status:
    return ok;
}
