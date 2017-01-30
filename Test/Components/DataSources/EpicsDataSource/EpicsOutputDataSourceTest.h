/**
 * @file EpicsOutputDataSourceTest.h
 * @brief Header file for class EpicsOutputDataSourceTest
 * @date 24/01/2017
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

 * @details This header file contains the declaration of the class EpicsOutputDataSourceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSOUTPUTDATASOURCETEST_H_
#define EPICSOUTPUTDATASOURCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests the EpicsOutputDataSourceTest public methods.
 */
class EpicsOutputDataSourceTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers method.
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the GetSignalMemoryBuffer method.
     */
    bool TestGetSignalMemoryBuffer();

    /**
     * @brief Tests the GetSignalMemoryBuffer method with an invalid signal index.
     */
    bool TestGetSignalMemoryBuffer_False();

    /**
     * @brief Tests the GetBrokerName method.
     */
    bool TestGetBrokerName();

    /**
     * @brief Tests the GetInputBrokers method.
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests the Execute method with Busy sleep.
     */
    bool TestExecute_Busy();

    /**
     * @brief Tests the PrepareNextState method.
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the Initialise method with an empty StructuredDataI.
     */
    bool TestInitialise_Empty();

    /**
     * @brief Tests the Initialise method with a Default SleepNature.
     */
    bool TestInitialise_Default();

    /**
     * @brief Tests the Initialise method  with a Busy SleepNature.
     */
    bool TestInitialise_Busy();

    /**
     * @brief Tests the Initialise method with an invalid SleepNature..
     */
    bool TestInitialise_False();

    /**
     * @brief Tests the SetConfiguredDatabase method.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the SetConfiguredDatabase method with only one signal specified on each GAM.
     */
    bool TestSetConfiguredDatabase_One_Signal_Per_GAM();

    /**
     * @brief Tests the SetConfiguredDatabase method specifying more than two signals.
     */
    bool TestSetConfiguredDatabase_False_MoreThan2Signals();

    /**
     * @brief Tests the SetConfiguredDatabase method specifying with a first signal that has more than 32 bits.
     */
    bool TestSetConfiguredDatabase_False_No32BitsSignal1();

    /**
     * @brief Tests the SetConfiguredDatabase method specifying with a first signal that is not (Un)SignedInteged.
     */
    bool TestSetConfiguredDatabase_False_IntegerSignal1();

    /**
     * @brief Tests the SetConfiguredDatabase method specifying with a second signal that has more than 32 bits.
     */
    bool TestSetConfiguredDatabase_False_No32BitsSignal2();

    /**
     * @brief Tests the SetConfiguredDatabase method specifying with a second signal that is not (Un)SignedInteged.
     */
    bool TestSetConfiguredDatabase_False_IntegerSignal2();

    /**
     * @brief Tests the SetConfiguredDatabase without setting any frequency.
     */
    bool TestSetConfiguredDatabase_False_NoFrequencySet();

    /**
     * @brief Tests ...
     */
    template<typename SignalType>
    bool Test1();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "EpicsOutputDataSource.h"
#include "SharedDataArea.h"
#include "SharedDataAreaSupport.h"

#include "EpicsDataSourceSupport.h"

template<typename SignalType>
bool EpicsOutputDataSourceTest::Test1() {
    using namespace MARTe;
    bool success = false;
    const unsigned int maxTests = 30;
    const char targetName[] = "EpicsOutputDataSourceTest_Test1";
    EpicsOutputDataSource target;
	DataSet dataset(maxTests);
	SharedDataArea sdaClient;
	SharedDataArea::SigblockConsumer* consumer;
	const uint32 numberOfSignals = 5;
	void* signals[numberOfSignals];

	//Initialize the name of the data source:
    target.SetName(targetName);

    //Initialize signals configuration on data source:
    success = SetConfiguredDatabase(target, numberOfSignals);

    //Allocate memory of data source (it setups the shared data area):
    target.AllocateMemory();

    //Cache an array of pointers to the signal's addresses:
    for (uint32 i = 0; i < numberOfSignals; i++) {
    	target.GetSignalMemoryBuffer(i, 0, signals[i]);
    }

	//Setup producers's interface to shared data area:
    StreamString shmName;
    shmName.Printf("MARTe_%s", targetName);
	sdaClient = SharedDataArea::BuildSharedDataAreaForEPICS(shmName.Buffer());
	consumer = sdaClient.GetSigblockConsumerInterface();

	//Allocate memory for dataset:
	MallocDataSet(dataset, consumer->GetSigblockMetadata()->GetTotalSize());

	//Initialize items of dataset:
	InitDataSet<SignalType>(dataset, numberOfSignals);

	//Write all the sigblocks of the dataset to the output data source,
	//checking that they can be read by the shared data area and have
	//the same values than those from the dataset. They will be written
	//and read taking turns (1 write, 1 read).
	{
		bool error = false;
		unsigned int i = 0;

		//Write and read sigblocks taking turns:
		while (i < dataset.size && !error) {
			bool writeOk;
			//Write the sigblock on the position i of the dataset to the output data source:
			for (uint32 j = 0; j < numberOfSignals; j++) {
				std::memcpy(signals[j], &dataset.items[j], sizeof(SignalType));
			}

			//Synchronize the output data source with the shared data area
			//(i.e. writes the signals of the output data source to the
			//shared data area as a sigblock):
			writeOk = target.Synchronise();

			if (writeOk) {
				Sigblock* sigblock = NULL_PTR(Sigblock*);
				bool readOk;

				//Allocate memory for sigblock:
				sigblock = MallocSigblock(consumer->GetSigblockMetadata()->GetTotalSize());

				//Read the next sigblock available on the shared data area:
				readOk = consumer->ReadSigblock(*sigblock);

				if (readOk) {

					//Check the values of the signals into the sigblock read
					//from the shared data area against those of the data set:
					unsigned int j = 0;
				    while (j < numberOfSignals && !error) {
				    	printf("signals[j] == %u\n", *(static_cast<uint32*>(signals[j])));
				    	error = (std::memcmp(signals[j], sigblock + consumer->GetSigblockMetadata()->GetSignalOffsetByIndex(j), sizeof(SignalType)) != 0);
				    	j++;
				    }
				}
				else {
					error = true;
				}

				//Free memory for sigblock:
				FreeSigblock(sigblock);
			}
			else {
				error = true;
			}
			printf("EpicsOutputDataSourceTest::Test1 -- Write/Read dataset.items[%u] error=%u\n", i, error);
			i++;
		}

		//Check execution's status:
		success &= !error;
	}

	//Free memory of dataset:
	FreeDataSet(dataset);

	return success;
}

#endif /* EPICSOUTPUTDATASOURCETEST_H_ */
