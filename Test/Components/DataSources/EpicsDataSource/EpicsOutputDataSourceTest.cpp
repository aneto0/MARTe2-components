/**
 * @file EpicsOutputDataSourceTest.cpp
 * @brief Source file for class EpicsOutputDataSourceTest
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

 * @details This source file contains the definition of all the methods for
 * the class EpicsOutputDataSourceTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "EpicsDataSourceSupport.h"
#include "EpicsOutputDataSource.h"
#include "EpicsOutputDataSourceTest.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "Platform.h"
#include "RealTimeApplication.h"
#include "SharedDataArea.h"
#include "SigblockDoubleBufferSupport.h"
#include "StandardParser.h"
#include "StreamString.h"

template bool EpicsOutputDataSourceTest::TestSynchronise<MARTe::uint32>();
template bool EpicsOutputDataSourceTest::TestSynchronise<MARTe::float32>();

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static MARTe::ConfigurationDatabase INV_CDB;

#define INVARIANT(obj) ( \
(obj.GetNumberOfMemoryBuffers() == 1u) and \
(std::strcmp(obj.GetBrokerName(INV_CDB, InputSignals), "") == 0) and \
(std::strcmp(obj.GetBrokerName(INV_CDB, OutputSignals), "MemoryMapSynchronisedOutputBroker") == 0) \
)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool EpicsOutputDataSourceTest::TestConstructor() {
    using namespace MARTe;
    bool ok = false;
    EpicsOutputDataSource target;

    //Check postcondition:
    ok = (target.GetSharedDataAreaName().Size() == 0);

    //Check invariant:
    ok &= INVARIANT(target);

    return ok;
}

template<typename SignalType>
bool EpicsOutputDataSourceTest::TestSynchronise() {
    using namespace MARTe;
    bool ok = false;
    MARTe::StreamString tmp_SharedDataAreaName; //TODO: Remove this when autorelease will be added to EpicsOutputDataSourceTest.

    {
    	const unsigned int maxTests = 30;
    	const char targetName[] = "EpicsOutputDataSourceTest_TestSynchronise";
    	EpicsOutputDataSource target;
    	DataSet dataset(maxTests);
    	SharedDataArea sdaClient;
    	SharedDataArea::SigblockConsumer* consumer;
    	const uint32 numberOfSignals = 5;
    	void* signals[numberOfSignals];

    	//Initialize the name of the data source:
    	target.SetName(targetName);

    	//Initialize signals configuration on data source:
    	ConfigurationDatabase cdb;
    	ok = BuildConfigurationDatabase(cdb, numberOfSignals);
    	ok &= target.SetConfiguredDatabase(cdb);

    	//Allocate memory of data source (it setups the shared data area):
    	target.AllocateMemory();
    	tmp_SharedDataAreaName = target.GetSharedDataAreaName(); //TODO: Remove this when autorelease will be added to EpicsOutputDataSourceTest.

    	//Cache an array of pointers to the signal's addresses:
    	for (uint32 i = 0; i < numberOfSignals; i++) {
    		target.GetSignalMemoryBuffer(i, 0, signals[i]);
    	}

    	//Setup producers's interface to shared data area:
    	sdaClient = SharedDataArea::BuildSharedDataAreaForEPICS(target.GetSharedDataAreaName().Buffer());
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
    				SignalType* cursig = reinterpret_cast<SignalType*>(signals[j]);
    				SignalType* refsig = reinterpret_cast<SignalType*>(dataset.items[i] + consumer->GetSigblockMetadata()->GetSignalOffsetByIndex(j));
    				*cursig = *refsig;
    			}

    			//Synchronise the output data source with the shared data area
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
    						SignalType* cursig = reinterpret_cast<SignalType*>(sigblock + consumer->GetSigblockMetadata()->GetSignalOffsetByIndex(j));
    						SignalType* refsig = reinterpret_cast<SignalType*>(dataset.items[i] + consumer->GetSigblockMetadata()->GetSignalOffsetByIndex(j));
    						error = (*cursig != *refsig);
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
    			i++;
    		}

    		//Check execution's status:
    		ok &= !error;
    	}

    	//Free memory of dataset:
    	FreeDataSet(dataset);

    }

    //Release shared data area: //TODO: Remove this when autorelease will be added to EpicsOutputDataSourceTest.
	Platform::DestroyShm(tmp_SharedDataAreaName.Buffer());

	return ok;
}

bool EpicsOutputDataSourceTest::TestAllocateMemory() {
    using namespace MARTe;
    bool ok = false;
    const char targetName[] = "EpicsOutputDataSourceTest_TestAllocateMemory";
    EpicsOutputDataSource target;

	//Initialize the name of the data source:
    target.SetName(targetName);

    //Initialize signals/functions configuration on data source:
    ConfigurationDatabase cdb;
    ok = BuildConfigurationDatabase(cdb, 1, 0);
    ok &= target.SetConfiguredDatabase(cdb);

	//Check class invariant:
    ok &= INVARIANT(target);

    //Execute the target method:
    ok &= target.AllocateMemory();

    //Check postcondition:
	SharedDataArea::BuildSharedDataAreaForEPICS(target.GetSharedDataAreaName().Buffer());

	//Check class invariant:
    ok &= INVARIANT(target);

    return ok;
}

bool EpicsOutputDataSourceTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    bool ok = false;
    EpicsOutputDataSource target;

	//Check class invariant:
    ok = INVARIANT(target);

    //Execute the target method:
    ok &= (target.GetNumberOfMemoryBuffers() == 1u);

    //Check postcondition:
    ok &= (target.GetNumberOfMemoryBuffers() == 1u);

	//Check class invariant:
    ok &= INVARIANT(target);

    return ok;
}

bool EpicsOutputDataSourceTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;

    //Declare and initialise variables:
	const char targetName[] = "EpicsOutputDataSourceTest_TestGetSignalMemoryBuffer";
	bool ok;
	EpicsOutputDataSource target;
	const uint32 numberOfSignals = 5;
	void* signals[numberOfSignals];

	//Initialize the name of the data source:
	target.SetName(targetName);

	//Initialize signals configuration on data source:
	ConfigurationDatabase cdb;
	ok = BuildConfigurationDatabase(cdb, numberOfSignals);
	ok &= target.SetConfiguredDatabase(cdb);

	//Allocate memory of data source (it setups the shared data area):
	target.AllocateMemory();

	//Cache an array of pointers to the signal's addresses:
	for (uint32 i = 0; i < numberOfSignals && ok; i++) {
		ok &= target.GetSignalMemoryBuffer(i, 0, signals[i]);
	}

	//Set values to signal:
	for (uint32 i = 0; i < numberOfSignals; i++) {
		uint32* signal = reinterpret_cast<uint32*>(signals[i]);
		*signal = i;
	}

	//Check values of signals:
	for (uint32 i = 0; i < numberOfSignals && ok; i++) {
		uint32* signal = reinterpret_cast<uint32*>(signals[i]);
		ok &= (*signal == i);
	}

	//Check out of bounds preconditions:
	{
		void* signal = NULL;
		ok &= !(target.GetSignalMemoryBuffer(numberOfSignals+1, 0, signal));
		ok &= !(target.GetSignalMemoryBuffer(0, 1, signal));
	}

    return ok;
}

bool EpicsOutputDataSourceTest::TestGetBrokerName() {
    using namespace MARTe;
    bool ok = false;
    //Check broker name for input signals:
    {
    	ConfigurationDatabase config;
    	StreamString brokerName;
    	EpicsOutputDataSource target;

    	//Check class invariant:
    	ok = INVARIANT(target);

        //Execute the target method:
    	brokerName = target.GetBrokerName(config, InputSignals);

        //Check postcondition:
    	ok &= (brokerName == "");

    	//Check class invariant:
    	ok &= INVARIANT(target);
    }
    //Check broker name for output signals:
    {
    	ConfigurationDatabase config;
    	StreamString brokerName;
    	EpicsOutputDataSource target;

    	//Check class invariant:
    	ok &= INVARIANT(target);

        //Execute the target method:
    	brokerName = target.GetBrokerName(config, OutputSignals);

        //Check postcondition:
    	ok &= (brokerName == "MemoryMapSynchronisedOutputBroker");

    	//Check class invariant:
    	ok &= INVARIANT(target);
    }
    return ok;
}

bool EpicsOutputDataSourceTest::TestGetInputBrokers() {
    using namespace MARTe;
    bool ok = false;
    ReferenceContainer inputBrokers;
    char8* functionName = NULL_PTR(char8*);
    void* gamMemPtr = NULL_PTR(void*);
    EpicsOutputDataSource target;

	//Check class invariant:
    ok = INVARIANT(target);

	//Execute target method:
    ok &= (target.GetInputBrokers(inputBrokers, functionName, gamMemPtr) == false);

	//check postcondition:
    ok &= (inputBrokers.Size() == 0);

	//Check class invariant:
    ok &= INVARIANT(target);

    return ok;
}

bool EpicsOutputDataSourceTest::TestGetOutputBrokers() {
    using namespace MARTe;
    bool ok = false;
    const char targetName[] = "EpicsOutputDataSourceTest_TestGetOutputBrokers";
    const uint32 numberOfSignals = 3;
    const uint32 numberOfFunctions = 5;
    char buffer[0]; //Size of buffer not relevant in this test.
    void* gamMemPtr = static_cast<void*>(buffer);
    EpicsOutputDataSource target;

	//Initialize the name of the data source:
    target.SetName(targetName);

    //Initialize signals/functions configuration on data source:
    ConfigurationDatabase cdb;
    ok = BuildConfigurationDatabase(cdb, numberOfSignals, numberOfFunctions);
    ok &= target.SetConfiguredDatabase(cdb);

    //Allocate memory for signals (it will be needed by brokers):
    ok &= target.AllocateMemory();

    //Check datasource's input broker retrieval for each function:
    for (uint32 i = 0; i < numberOfFunctions; i++) {
    	StreamString name;
    	ReferenceContainer outputBrokers;

    	//Set function name:
    	ok &= name.Printf("Function_%d", i);

    	//Check class invariant:
    	ok &= INVARIANT(target);

    	//Execute the target method:
    	ok &= (target.GetOutputBrokers(outputBrokers, name.Buffer(), gamMemPtr) == true);

    	//check postcondition:
    	ok &= (outputBrokers.Size() == 1);

    	//Check class invariant:
    	ok &= INVARIANT(target);
    }

    return ok;
}

bool EpicsOutputDataSourceTest::TestPrepareNextState() {
    using namespace MARTe;
    bool ok = false;
    char8* currentStateName = NULL_PTR(char8*);
    char8* nextStateName = NULL_PTR(char8*);
    EpicsOutputDataSource target;

	//Check class invariant:
    ok = INVARIANT(target);

	//Execute target method:
    ok &= (target.PrepareNextState(currentStateName, nextStateName) == true);

	//Check class invariant:
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsOutputDataSourceTest::TestGetSharedDataAreaName() {
    using namespace MARTe;
    bool ok = false;
    const char targetName[] = "EpicsOutputDataSourceTest_TestGetSharedDataAreaName";
    EpicsOutputDataSource target;

    //Check initial value of SharedDataAreaName is an empty string:
    ok = (target.GetSharedDataAreaName().Size() == 0);

	//Initialize the name of the data source:
    target.SetName(targetName);

    //Initialize signals/functions configuration on data source:
    ConfigurationDatabase cdb;
    ok = BuildConfigurationDatabase(cdb, 1, 0);
    ok &= target.SetConfiguredDatabase(cdb);

    //Allocate memory for the data source:
    ok &= target.AllocateMemory();

    //Check working value of SharedDataAreaName is not an empty string:
    ok &= (target.GetSharedDataAreaName().Size() > 0);

    return ok;
}
