/**
 * @file EpicsInputDataSourceTest.cpp
 * @brief Source file for class EpicsInputDataSourceTest
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
 * the class EpicsInputDataSourceTest (public, protected, and private). Be aware that some
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
#include "EpicsInputDataSource.h"
#include "EpicsInputDataSourceTest.h"
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

bool EpicsInputDataSourceTest::TestConstructor() {
    using namespace MARTe;
    bool ok = true;
    EpicsInputDataSource target;
    //TODO: Check postcondition
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase() {
	return false;
}

bool EpicsInputDataSourceTest::TestSynchronise() {
	return false;
}

bool EpicsInputDataSourceTest::TestAllocateMemory() {
    using namespace MARTe;
    bool ok = true;
    EpicsInputDataSource target;
    ok &= INVARIANT(target);
    ok &= target.AllocateMemory();
    //TODO: Check postcondition
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsInputDataSourceTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    bool ok = true;
    EpicsInputDataSource target;
    ok &= INVARIANT(target);
    ok &= (target.GetNumberOfMemoryBuffers() == 1u);	//TODO: The postcondition of this method is the invariant of the class
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsInputDataSourceTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    EpicsInputDataSource test;
    uint32 *counter;
    uint32 *timer;
    test.GetSignalMemoryBuffer(0, 0, (void *&) counter);
    test.GetSignalMemoryBuffer(1, 0, (void *&) timer);
    bool ok = (*counter == 0);
    ok &= (*timer == 0);
    return ok;
}

bool EpicsInputDataSourceTest::TestGetSignalMemoryBuffer_False() {
    using namespace MARTe;
    EpicsInputDataSource test;
    uint32 *ptr;
    return !test.GetSignalMemoryBuffer(2, 0, (void *&) ptr);
}

bool EpicsInputDataSourceTest::TestGetBrokerName() {
    using namespace MARTe;
    bool ok = true;
    {
    	ConfigurationDatabase config;
    	StreamString brokerName;
    	EpicsInputDataSource target;
    	ok &= INVARIANT(target);
    	brokerName = target.GetBrokerName(config, InputSignals);
    	ok &= (brokerName == "MemoryMapSynchronisedInputBroker");
    	ok &= INVARIANT(target);
    }
    {
    	ConfigurationDatabase config;
    	StreamString brokerName;
    	EpicsInputDataSource target;
    	ok &= INVARIANT(target);
    	brokerName = target.GetBrokerName(config, OutputSignals);
    	ok &= (brokerName == "");
    	ok &= INVARIANT(target);
    }
    return ok;
}

bool EpicsInputDataSourceTest::TestGetInputBrokers() {
    using namespace MARTe;
    bool ok = true;
    char name[] = "Dummy"; //TODO: Add this function to *this, otherwise the call to GetInputBrokers() fails.
    char buffer[25];
    ReferenceContainer inputBrokers;
    char8* functionName = static_cast<char8*>(name);
    void* gamMemPtr = static_cast<void*>(buffer);
    EpicsInputDataSource target;
    ok &= INVARIANT(target);
    ok &= (target.GetInputBrokers(inputBrokers, functionName, gamMemPtr) == true);
    ok &= (inputBrokers.Size() == 1);
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsInputDataSourceTest::TestGetOutputBrokers() {
    using namespace MARTe;
    bool ok = true;
    ReferenceContainer outputBrokers;
    char8* functionName = NULL_PTR(char8*);
    void* gamMemPtr = NULL_PTR(void*);
    EpicsInputDataSource target;
    ok &= INVARIANT(target);
    ok &= (target.GetOutputBrokers(outputBrokers, functionName, gamMemPtr) == false);
    ok &= (outputBrokers.Size() == 0);
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsInputDataSourceTest::TestPrepareNextState() {
    using namespace MARTe;
    bool ok = true;
    char8* currentStateName = NULL_PTR(char8*);
    char8* nextStateName = NULL_PTR(char8*);
    EpicsInputDataSource target;
    ok &= INVARIANT(target);
    ok &= target.PrepareNextState(currentStateName, nextStateName);
    //TODO: Check postcondition
    ok &= INVARIANT(target);
    return ok;
}

bool EpicsInputDataSourceTest::TestExecute() {
	return false;
}

bool EpicsInputDataSourceTest::TestExecute_Busy() {
	return false;
}

bool EpicsInputDataSourceTest::TestInitialise_Empty() {
    using namespace MARTe;
    EpicsInputDataSource test;
    ConfigurationDatabase cdb;
    return test.Initialise(cdb);
}

bool EpicsInputDataSourceTest::TestInitialise_Default() {
    using namespace MARTe;
    EpicsInputDataSource test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "Default");
    return test.Initialise(cdb);
}

bool EpicsInputDataSourceTest::TestInitialise_Busy() {
    using namespace MARTe;
    EpicsInputDataSource test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "Busy");
    return test.Initialise(cdb);
}

bool EpicsInputDataSourceTest::TestInitialise_False() {
    using namespace MARTe;
    EpicsInputDataSource test;
    ConfigurationDatabase cdb;
    cdb.Write("SleepNature", "False");
    return !test.Initialise(cdb);
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_One_Signal_Per_GAM() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_False_MoreThan2Signals() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_False_No32BitsSignal1() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_False_No32BitsSignal2() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_False_NoFrequencySet() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_False_IntegerSignal1() {
	return false;
}

bool EpicsInputDataSourceTest::TestSetConfiguredDatabase_False_IntegerSignal2() {
	return false;
}
