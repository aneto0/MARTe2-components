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

#include "GAM.h"
#include "GAMScheduler.h"
#include "EpicsOutputDataSource.h"
#include "EpicsOutputDataSourceTest.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#define INVARIANT(obj) (obj.GetNumberOfMemoryBuffers() == 1u)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool EpicsOutputDataSourceTest::TestConstructor() {
    using namespace MARTe;
    bool ok = true;
    EpicsOutputDataSource target;
    //TODO: Check postcondition
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsOutputDataSourceTest::TestSetConfiguredDatabase() {
	return false;
}

bool EpicsOutputDataSourceTest::TestSynchronise() {
	return false;
}

bool EpicsOutputDataSourceTest::TestAllocateMemory() {
    using namespace MARTe;
    bool ok = true;
    EpicsOutputDataSource target;
    ok &= INVARIANT(target);
    ok &= target.AllocateMemory();
    //TODO: Check postcondition
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsOutputDataSourceTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    bool ok = true;
    EpicsOutputDataSource target;
    ok &= INVARIANT(target);
    ok &= (target.GetNumberOfMemoryBuffers() == 1u);	//TODO: The postcondition of this method is the invariant of the class
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsOutputDataSourceTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    EpicsOutputDataSource test;
    uint32 *counter;
    uint32 *timer;
    test.GetSignalMemoryBuffer(0, 0, (void *&) counter);
    test.GetSignalMemoryBuffer(1, 0, (void *&) timer);
    bool ok = (*counter == 0);
    ok &= (*timer == 0);
    return ok;
}

bool EpicsOutputDataSourceTest::TestGetSignalMemoryBuffer_False() {
    using namespace MARTe;
    EpicsOutputDataSource test;
    uint32 *ptr;
    return !test.GetSignalMemoryBuffer(2, 0, (void *&) ptr);
}

bool EpicsOutputDataSourceTest::TestGetBrokerName() {
    using namespace MARTe;
    bool ok = true;
    {
    	ConfigurationDatabase config;
    	StreamString brokerName;
    	EpicsOutputDataSource target;
    	ok &= INVARIANT(target);
    	brokerName = target.GetBrokerName(config, InputSignals);
    	ok &= (brokerName == "");
    	ok &= INVARIANT(target);
    }
    {
    	ConfigurationDatabase config;
    	StreamString brokerName;
    	EpicsOutputDataSource target;
    	ok &= INVARIANT(target);
    	brokerName = target.GetBrokerName(config, OutputSignals);
    	ok &= (brokerName == "MemoryMapSynchronisedOutputBroker");
    	ok &= INVARIANT(target);
    }
    return ok;
}

bool EpicsOutputDataSourceTest::TestGetInputBrokers() {
    using namespace MARTe;
    bool ok = true;
    char name[] = "Dummy"; //TODO: Add this function to *this, otherwise the call to GetInputBrokers() fails.
    char buffer[25];
    ReferenceContainer inputBrokers;
    char8* functionName = static_cast<char8*>(name);
    void* gamMemPtr = static_cast<void*>(buffer);
    EpicsOutputDataSource target;
    ok &= INVARIANT(target);
    ok &= (target.GetInputBrokers(inputBrokers, functionName, gamMemPtr) == true);
    ok &= (inputBrokers.Size() == 1);
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsOutputDataSourceTest::TestGetOutputBrokers() {
    using namespace MARTe;
    bool ok = true;
    ReferenceContainer outputBrokers;
    char8* functionName = NULL_PTR(char8*);
    void* gamMemPtr = NULL_PTR(void*);
    EpicsOutputDataSource target;
    ok &= INVARIANT(target);
    ok &= (target.GetOutputBrokers(outputBrokers, functionName, gamMemPtr) == false);
    ok &= (outputBrokers.Size() == 0);
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsOutputDataSourceTest::TestPrepareNextState() {
    using namespace MARTe;
    bool ok = true;
    char8* currentStateName = NULL_PTR(char8*);
    char8* nextStateName = NULL_PTR(char8*);
    EpicsOutputDataSource target;
    ok &= INVARIANT(target);
    ok &= target.PrepareNextState(currentStateName, nextStateName);
    //TODO: Check postcondition
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsOutputDataSourceTest::TestExecute() {
	return false;
}

bool EpicsOutputDataSourceTest::TestExecute_Busy() {
	return false;
}

bool EpicsOutputDataSourceTest::TestInitialise_Empty() {
    using namespace MARTe;
    EpicsOutputDataSource test;
    ConfigurationDatabase cdb;
    return test.Initialise(cdb);
}

bool EpicsOutputDataSourceTest::TestInitialise_Default() {
    using namespace MARTe;
    EpicsOutputDataSource test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "Default");
    return test.Initialise(cdb);
}

bool EpicsOutputDataSourceTest::TestInitialise_Busy() {
    using namespace MARTe;
    EpicsOutputDataSource test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "Busy");
    return test.Initialise(cdb);
}

bool EpicsOutputDataSourceTest::TestInitialise_False() {
    using namespace MARTe;
    EpicsOutputDataSource test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "False");
    return !test.Initialise(cdb);
}

bool EpicsOutputDataSourceTest::TestSetConfiguredDatabase_One_Signal_Per_GAM() {
	return false;
}

bool EpicsOutputDataSourceTest::TestSetConfiguredDatabase_False_MoreThan2Signals() {
	return false;
}

bool EpicsOutputDataSourceTest::TestSetConfiguredDatabase_False_No32BitsSignal1() {
	return false;
}

bool EpicsOutputDataSourceTest::TestSetConfiguredDatabase_False_No32BitsSignal2() {
	return false;
}

bool EpicsOutputDataSourceTest::TestSetConfiguredDatabase_False_NoFrequencySet() {
	return false;
}

bool EpicsOutputDataSourceTest::TestSetConfiguredDatabase_False_IntegerSignal1() {
	return false;
}

bool EpicsOutputDataSourceTest::TestSetConfiguredDatabase_False_IntegerSignal2() {
	return false;
}
