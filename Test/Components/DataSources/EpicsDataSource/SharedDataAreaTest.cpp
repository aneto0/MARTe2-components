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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SharedDataArea.h"
#include "SharedDataAreaTest.h"

#include "GeneralDefinitions.h"
#include "Sleep.h"
#include "Threads.h"

#include "Platform.h"
#include "SharedDataAreaSupport.h"

template bool SharedDataAreaTest::TestProducerConsumerInSingleThread<int>(const char* const,
                                                                          const unsigned int);
template bool SharedDataAreaTest::TestProducerConsumerInSingleThread<double>(const char* const,
                                                                             const unsigned int);
template bool SharedDataAreaTest::TestProducerConsumerWithTwoThreads<int>(const char* const,
                                                                          const unsigned int);
template bool SharedDataAreaTest::TestProducerConsumerWithTwoThreads<double>(const char* const,
                                                                             const unsigned int);

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static void producerThreadFunction(ProducerThreadParams* params) {
    //Write all the sigblocks of the dataset to the shared data area,
    //pausing 10ms every eight iterations to slow down production:
    for (unsigned int i = 0; i < params->context->dataset.size; i++) {
        params->output->WriteSigblock(*(params->context->dataset.items[i]));
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

    // Read all the sigblocks written by the producer to the shared data area
    // and check if they have the same values than those from the dataset. It
    // will do a busy wait every time there is no new sigblocks on the shared
    // data area.
    {
        SDA::Sigblock* sigblock = SDA_NULL_PTR(SDA::Sigblock*);
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
            }
            while (!isSigblockFilled && !params->context->producerThreadEnd);
            if (isSigblockFilled) {
                bool sigblockFound;
                consumerThreadReads++;
                SearchSigblockIntoDataSet(params->context->dataset, sigblock, size, dataSetIndex, sigblockFound);
                errorFound = !sigblockFound;
            }
        }

        //Read last sigblock after producer has stopped:
        if (!errorFound && params->context->producerThreadEnd) {
            isSigblockFilled = params->input->ReadSigblock(*sigblock);
            if (isSigblockFilled) {
                bool sigblockFound;
                consumerThreadReads++;
                SearchSigblockIntoDataSet(params->context->dataset, sigblock, size, dataSetIndex, sigblockFound);
                errorFound = !sigblockFound;
            }
        }

        //Free memory of sigblock:
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
    bool ok = false;
    ok = TestProducerConsumerInSingleThread<int>("MARTe_TestConstructor_TestProducerConsumerWithSingleThreadUsingIntegers_1", 50);
    ok &= TestProducerConsumerInSingleThread<double>("MARTe_TestConstructor_TestProducerConsumerWithSingleThreadUsingDoubles_2", 25);
    ok &= TestProducerConsumerWithTwoThreads<int>("MARTe_TestConstructor_TestProducerConsumerWithTwoThreadsUsingIntegers_3", 75);
    ok &= TestProducerConsumerWithTwoThreads<double>("MARTe_TestConstructor_TestProducerConsumerWithTwoThreadsUsingDoubles_4");
    return ok;
}

bool SharedDataAreaTest::TestGetSigblockProducerInterface() {
    bool ok = false;
    ok = TestProducerConsumerInSingleThread<int>("MARTe_TestGetSigblockProducerInterface_TestProducerConsumerWithSingleThreadUsingIntegers_1", 50);
    ok &= TestProducerConsumerInSingleThread<double>("MARTe_TestGetSigblockProducerInterface_TestProducerConsumerWithSingleThreadUsingDoubles_2", 25);
    ok &= TestProducerConsumerWithTwoThreads<int>("MARTe_TestGetSigblockProducerInterface_TestProducerConsumerWithTwoThreadsUsingIntegers_3", 75);
    ok &= TestProducerConsumerWithTwoThreads<double>("MARTe_TestGetSigblockProducerInterface_TestProducerConsumerWithTwoThreadsUsingDoubles_4");
    return ok;
}

bool SharedDataAreaTest::TestGetSigblockConsumerInterface() {
    bool ok = false;
    ok = TestProducerConsumerInSingleThread<int>("MARTe_TestGetSigblockConsumerInterface_TestProducerConsumerWithSingleThreadUsingIntegers_1", 50);
    ok &= TestProducerConsumerInSingleThread<double>("MARTe_TestGetSigblockConsumerInterface_TestProducerConsumerWithSingleThreadUsingDoubles_2", 25);
    ok &= TestProducerConsumerWithTwoThreads<int>("MARTe_TestGetSigblockConsumerInterface_TestProducerConsumerWithTwoThreadsUsingIntegers_3", 75);
    ok &= TestProducerConsumerWithTwoThreads<double>("MARTe_TestGetSigblockConsumerInterface_TestProducerConsumerWithTwoThreadsUsingDoubles_4");
    return ok;
}

bool SharedDataAreaTest::TestBuildSharedDataAreaForMARTe() {
    bool ok = false;
    ok = TestProducerConsumerInSingleThread<int>("MARTe_TestBuildSharedDataAreaForMARTe_TestProducerConsumerWithSingleThreadUsingIntegers_1", 50);
    ok &= TestProducerConsumerInSingleThread<double>("MARTe_TestBuildSharedDataAreaForMARTe_TestProducerConsumerWithSingleThreadUsingDoubles_2", 25);
    ok &= TestProducerConsumerWithTwoThreads<int>("MARTe_TestBuildSharedDataAreaForMARTe_TestProducerConsumerWithTwoThreadsUsingIntegers_3", 75);
    ok &= TestProducerConsumerWithTwoThreads<double>("MARTe_TestBuildSharedDataAreaForMARTe_TestProducerConsumerWithTwoThreadsUsingDoubles_4");
    return ok;
}

bool SharedDataAreaTest::TestBuildSharedDataAreaForEPICS() {
    bool ok = false;
    ok = TestProducerConsumerInSingleThread<int>("MARTe_TestBuildSharedDataAreaForEPICS_TestProducerConsumerWithSingleThreadUsingIntegers_1", 50);
    ok &= TestProducerConsumerInSingleThread<double>("MARTe_TestBuildSharedDataAreaForEPICS_TestProducerConsumerWithSingleThreadUsingDoubles_2", 25);
    ok &= TestProducerConsumerWithTwoThreads<int>("MARTe_TestBuildSharedDataAreaForEPICS_TestProducerConsumerWithTwoThreadsUsingIntegers_3", 75);
    ok &= TestProducerConsumerWithTwoThreads<double>("MARTe_TestBuildSharedDataAreaForEPICS_TestProducerConsumerWithTwoThreadsUsingDoubles_4");
    return ok;
}

template<typename SignalType>
bool SharedDataAreaTest::TestProducerConsumerInSingleThread(const char* const shmName,
                                                            const unsigned int maxTests) {
    DataSet dataset(maxTests);
    bool ok = false;
    const unsigned int numberOfSignals = 10;
    SDA::Signal::Metadata sbmd[numberOfSignals];
    SDA::SharedDataArea sdaServer;
    SDA::SharedDataArea::SigblockProducer* producer;
    SDA::SharedDataArea sdaClient;
    SDA::SharedDataArea::SigblockConsumer* consumer;
    std::size_t size;

    //Check SHM's name:
    ok = (std::strcmp(shmName, "") != 0);

    //Generate the metadata of the sigblocks to use in test:
    GenerateMetadataForSigblock<SignalType>(sbmd, numberOfSignals);

    //Build the shared data area as server:
    ok = SDA::SharedDataArea::BuildSharedDataAreaForMARTe(sdaServer, shmName, numberOfSignals, sbmd);

    if (ok) {

        //Setup producer's interface to shared data area:
        producer = sdaServer.GetSigblockProducerInterface();

        //Build the shared data area as client:
        ok &= SDA::SharedDataArea::BuildSharedDataAreaForEPICS(sdaClient, shmName);

        if (ok) {

            //Setup consumer's interface to shared data area:
            consumer = sdaClient.GetSigblockConsumerInterface();

            //Get sigblock's size:
            size = producer->GetSigblockMetadata()->GetTotalSize();

            //Check coherence of size:
            ok &= (producer->GetSigblockMetadata()->GetTotalSize() == consumer->GetSigblockMetadata()->GetTotalSize());

            //Allocate memory for dataset:
            MallocDataSet(dataset, size);

            //Initialize items of dataset:
            InitDataSet<SignalType>(dataset, numberOfSignals);

            // Write all the sigblocks of the dataset to the shared data area, checking
            // that they can be read and have the same values than those from the
            // dataset. They will be written and read taking turns (1 write, 1 read).
            {
                SDA::Sigblock* sigblock = SDA_NULL_PTR(SDA::Sigblock*);
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
                    i++;
                }

                //Free memory for sigblock:
                FreeSigblock(sigblock);

                //Check execution's status:
                ok &= !error;
            }

            //Free memory of dataset:
            FreeDataSet(dataset);

        }
    }

    //Release shared data area from system
    ok &= SDA::Platform::DestroyShm(shmName);

    //Return test's execution status:
    return ok;
}

template<typename SignalType>
bool SharedDataAreaTest::TestProducerConsumerWithTwoThreads(const char* const shmName,
                                                            const unsigned int maxTests) {
    /* Note: This test could be improved configuring the threads to run at different paces,
     * setting different priorities or making consumer sleep more often, for instance.*/

    bool ok = false;
    SharedContext context(10, maxTests);
    ProducerThreadParams producerThreadParams;
    ConsumerThreadParams consumerThreadParams;
    SDA::Signal::Metadata sbmd[context.numberOfSignals];
    SDA::SharedDataArea sdaServer;
    SDA::SharedDataArea::SigblockProducer* producer;
    SDA::SharedDataArea sdaClient;
    SDA::SharedDataArea::SigblockConsumer* consumer;
    std::size_t size;
    MARTe::ThreadIdentifier producerThreadId;
    MARTe::ThreadIdentifier consumerThreadId;

    //Check SHM's name:
    ok = (std::strcmp(shmName, "") != 0);

    //Generate the metadata of the sigblocks to use in test:
    GenerateMetadataForSigblock<SignalType>(sbmd, context.numberOfSignals);

    //Build the shared data area as server:
    ok = SDA::SharedDataArea::BuildSharedDataAreaForMARTe(sdaServer, shmName, context.numberOfSignals, sbmd);

    if (ok) {

        //Setup producer's interface to shared data area:
        producer = sdaServer.GetSigblockProducerInterface();

        //Setup producer's parameters:
        producerThreadParams.output = producer;
        producerThreadParams.context = &context;

        //Build the shared data area as client:
        ok &= SDA::SharedDataArea::BuildSharedDataAreaForEPICS(sdaClient, shmName);

        if (ok) {

            //Setup consumer's interface to shared data area:
            consumer = sdaClient.GetSigblockConsumerInterface();

            //Setup consumer's parameters:
            consumerThreadParams.input = consumer;
            consumerThreadParams.context = &context;

            //Get sigblock's size:
            size = producer->GetSigblockMetadata()->GetTotalSize();

            //Check coherence of size:
            ok &= (producer->GetSigblockMetadata()->GetTotalSize() == consumer->GetSigblockMetadata()->GetTotalSize());

            //Allocate memory for dataset:
            MallocDataSet(context.dataset, size);

            //Initialize items of dataset:
            InitDataSet<SignalType>(context.dataset, context.numberOfSignals);

            //Start producer's thread linked to producerThreadFunction:
            producerThreadId = MARTe::Threads::BeginThread((MARTe::ThreadFunctionType) producerThreadFunction, &producerThreadParams);

            //Check producer's thread id:
            ok = (producerThreadId != MARTe::InvalidThreadIdentifier);

            //Start consumer's thread linked to consumerThreadFunction:
            consumerThreadId = MARTe::Threads::BeginThread((MARTe::ThreadFunctionType) consumerThreadFunction, &consumerThreadParams);

            //Check consumer's thread id:
            ok = (consumerThreadId != MARTe::InvalidThreadIdentifier);

            //Busy wait until both threads end:
            MARTe::uint32 nTimes = 50;
            while ((nTimes > 0) && (!context.producerThreadEnd || !context.consumerThreadEnd)) {
                nTimes--;
                MARTe::Sleep::MSec(100);
            };

            //Free memory of dataset:
            FreeDataSet(context.dataset);

            //Check execution's status:
            ok &= context.consumerThreadSuccess;

        }

    }

    //Release shared data area from system
    ok &= SDA::Platform::DestroyShm(shmName);

    //Return test's execution status:
    return ok;
}
