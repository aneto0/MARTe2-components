/**
 * @file MemoryGateTest.cpp
 * @brief Source file for class MemoryGateTest
 * @date 24/07/2018
 * @author Giuseppe Ferro
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
 * the class MemoryGateTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "TcnTimeProviderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

bool TcnTimeProviderTest::PreInitialise(bool noPreInit) {
    bool retVal = true;
    timeProvider = new MARTe::TcnTimeProvider();

    retVal = tcnCfg.Write("TcnDevice", "/etc/opt/codac/tcn/tcn-default.xml");

    if(retVal && !noPreInit) {
        retVal = timeProvider->Initialise(tcnCfg);
    }

    return retVal; 
}

TcnTimeProviderTest::TcnTimeProviderTest(bool noPreInit) : TimeProviderTest() {
    PreInitialise(noPreInit);
}

TcnTimeProviderTest::TcnTimeProviderTest() : TimeProviderTest() {
    PreInitialise(false);   
}

TcnTimeProviderTest::~TcnTimeProviderTest() {

}

bool TcnTimeProviderTest::TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode mode) {
   
    StreamString operationModeString;
    operationModeString.Seek(0);

    bool skipOperationModeSet = false;

    switch(mode) {
        case TcnTimeProviderTestInitialiseMode_NoPollLegacyMode:
            operationModeString.Printf("%s", "NoPollLegacyMode");
            break;
        case TcnTimeProviderTestInitialiseMode_PollLegacyMode:
            operationModeString.Printf("%s", "PollLegacyMode");
            break;
        case TcnTimeProviderTestInitialiseMode_WaitUntilMode:
            operationModeString.Printf("%s", "WaitUntilMode");
            break;
        case TcnTimeProviderTestInitialiseMode_WaitUntilHRMode:
            operationModeString.Printf("%s", "WaitUntilHRMode");
            break;
        case TcnTimeProviderTestInitialiseMode_SleepMode:
            operationModeString.Printf("%s", "SleepMode");
            break;
        case TcnTimeProviderTestInitialiseMode_SleepHRMode:
            operationModeString.Printf("%s", "SleepHRMode");
            break;
        case TcnTimeProviderTestInitialiseMode_InvalidMode:
            operationModeString.Printf("%s", "InvalidMode");
            break;
        case TcnTimeProviderTestInitialiseMode_LegacyTcnPollTrue:
            tcnCfg.Write("TcnPoll", 1);
            skipOperationModeSet = true;
            break;
        case TcnTimeProviderTestInitialiseMode_LegacyTcnPollTrue:
            tcnCfg.Write("TcnPoll", 0);
            skipOperationModeSet = true;
            break;
    }

    if(!skipOperationModeSet) {
        tcnCfg.Write("OperationMode", operationModeString.Buffer());
    }

    timeProvider->Initialise(tcnCfg);

    bool testCounterRes = TestCounter();
    bool testPeriodRes = TestPeriod();
    bool testFrequencyRes = TestFrequency();
    bool testSleepRes = TestSleep();

    REPORT_ERROR_STATIC(testCounterRes?ErrorManagement::Information:ErrorManagement::FatalError, "Counter [%s]", testCounterRes?"passed":"failed");
    REPORT_ERROR_STATIC(testPeriodRes?ErrorManagement::Information:ErrorManagement::FatalError, "Period [%s]", testPeriodRes?"passed":"failed");
    REPORT_ERROR_STATIC(testFrequencyRes?ErrorManagement::Information:ErrorManagement::FatalError, "Frequency [%s]", testFrequencyRes?"passed":"failed");
    REPORT_ERROR_STATIC(testSleepRes?ErrorManagement::Information:ErrorManagement::FatalError, "Sleep [%s]", testSleepRes?"passed":"failed");

    return testCounterRes && testPeriodRes && testFrequencyRes && testSleepRes;
}

bool TcnTimeProviderTest::TestInitialise_LegacyTcnPollTrue() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_LegacyTcnPollTrue);
}

bool TcnTimeProviderTest::TestInitialise_LegacyTcnPollFalse() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_LegacyTcnPollFalse);
}

bool TcnTimeProviderTest::TestInitialise_NoPollLegacyMode() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_NoPollLegacyMode);
}

bool TcnTimeProviderTest::TestInitialise_PollLegacyMode() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_PollLegacyMode);
}

bool TcnTimeProviderTest::TestInitialise_WaitUntilMode() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_WaitUntilMode);
}

bool TcnTimeProviderTest::TestInitialise_WaitUntilHRMode() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_WaitUntilHRMode);
}

bool TcnTimeProviderTest::TestInitialise_SleepMode() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_SleepMode);
}

bool TcnTimeProviderTest::TestInitialise_SleepHRMode() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_SleepHRMode);
}

bool TcnTimeProviderTest::TestInitialise_InvalidMode_Fail() {
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_InvalidMode);
}

bool TcnTimeProviderTest::TestInitialise_WithTolerance() {
    tcnCfg.Write("Tolerance", 2000);
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_WaitUntilHRMode);
}

bool TcnTimeProviderTest::TestInitialise_WithInvalidTcnDevice_Fail() {
    retVal = tcnCfg.Write("TcnDevice", "/path/to/invalid/file.xml");
    return TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode_SleepMode);    
}

