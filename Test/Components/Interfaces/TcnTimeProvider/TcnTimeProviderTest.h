/**
 * @file TcnTimeProviderTest.h
 * @brief Header file for class TcnTimeProviderTest
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

 * @details This header file contains the declaration of the class TcnTimeProviderTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TCNTIMEPROVIDER_TNCTIMEPROVIDERTEST_H_
#define TCNTIMEPROVIDER_TNCTIMEPROVIDERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "TcnTimeProvider.h"
#include "TimeProviderTest.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
* @brief Enumerates all the possible operating modes for the TcnTime Provider
*/
typedef enum {
    TcnTimeProviderTestInitialiseMode_LegacyTcnPollTrue,
    TcnTimeProviderTestInitialiseMode_LegacyTcnPollFalse,
    TcnTimeProviderTestInitialiseMode_NoPollLegacyMode,
    TcnTimeProviderTestInitialiseMode_PollLegacyMode,
    TcnTimeProviderTestInitialiseMode_WaitUntilMode,
    TcnTimeProviderTestInitialiseMode_WaitUntilHRMode,
    TcnTimeProviderTestInitialiseMode_SleepMode,
    TcnTimeProviderTestInitialiseMode_SleepHRMode,
    TcnTimeProviderTestInitialiseMode_InvalidMode
}TcnTimeProviderTestInitialiseMode;

/**
 * @brief Tests all the TcnTimeProvider operating modes
 */
class TcnTimeProviderTest : public TimeProviderTest {
    public:
        /**
        * @brief Constructs the Test Unit environment basics
        */
        TcnTimeProviderTest();

        /**
        * @brief Constructs the Test Unit environment, giving the ability to skip the pre initialization step
        * param[in] noPreinit true if implicit initialisation has to be skipped
        */
        TcnTimeProviderTest(bool noPreinit);

        /**
        * @brief Destructs the Test Unit environment
        */
        ~TcnTimeProviderTest();

        /**
        * @brief Tests the TCN Time Provider in No Poll legacy mode (TcnPoll = 0 - internal HRT usage)
        */
        bool TestInitialise_NoPollLegacyMode();

        /**
        * @brief Tests the TCN Time Provider in Poll legacy mode (TcnPoll = 1 - polling happens on the tcn underlying to get the counter)
        */
        bool TestInitialise_PollLegacyMode();

        /**
        * @brief Tests the TCN Time Provider using the internal tcn_wait_until mode
        */
        bool TestInitialise_WaitUntilMode();

        /**
        * @brief Tests the TCN Time Provider using the internal tcn_wait_until_hr mode
        */
        bool TestInitialise_WaitUntilHRMode();

        /**
        * @brief Tests the TCN Time Provider using the internal tcn_sleep mode
        */
        bool TestInitialise_SleepMode();

        /**
        * @brief Tests the TCN Time Provider using the internal tcn_sleep_hr mode
        */
        bool TestInitialise_SleepHRMode();

        /**
        * @brief Tries to force and expects failure for an invalid mode assignment
        */
        bool TestInitialise_InvalidMode_Fail();

        /**
        * @brief Tests the Legacy Poll mode ON explicitly
        */
        bool TestInitialise_LegacyTcnPollTrue();

        /**
        * @brief Tests the Legacy Poll mode OFF explicitly
        */
        bool TestInitialise_LegacyTcnPollFalse();

        /**
        * @brief Tests the tolerance parameter
        */
        bool TestInitialise_WithTolerance();

        /**
        * @brief Tries to initialize the TCN underlying library with an invalid TCN device
        */
        bool TestInitialise_WithInvalidTcnDevice_Fail();

        /**
        * @brief Tries to initialize the TCN underlying library without passing the TcnDevice parameter
        */
        bool TestInitialise_WithMissingTcnDevice_Fail();

        /**
        * @brief Tries to initialize the TCN underlying using a wrong XML
        */
        bool TestInitialise_WrongXmlPlugin_Fail();

        /**
        * @brief Explicitly passes the TCN frequency which is related to the effective provider
        */
        bool TestInitialise_WithFrequency();

        /**
        * @brief Tries an integrated run using TcnPoll=0 method
        */
        bool TestIntegrated_WithTcnPollEnabled();

        /**
        * @brief Tries an integrated run using TcnPoll=1 method
        */
        bool TestIntegrated_WithTcnPollDisabled();

    private:

        /**
        * @brief The configuration StructuredDataI which is built-up across tests
        */
        ConfigurationDatabase tcnCfg;

        /**
        * @brief Heart of the pre-initialization function, used from the constructors
        */
        bool PreInitialise(bool noPreInit);

        /**
        * @brief Heart of the configuration test unit which allows parametrized mode
        * @param[in] mode One of the enumerated operating modes for the TCN Provider
        */
        bool TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode mode);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TCNTIMEPROVIDER_TNCTIMEPROVIDERTEST_H_ */

