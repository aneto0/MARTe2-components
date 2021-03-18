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

 * @details This header file contains the declaration of the class MemoryGateTest
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
 * @brief Test all the TcnTimeProvider methods
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

class TcnTimeProviderTest : public TimeProviderTest {
    public:
        TcnTimeProviderTest();
        TcnTimeProviderTest(bool noPreinit);

        ~TcnTimeProviderTest();

        bool TestInitialise_NoPollLegacyMode();
        bool TestInitialise_PollLegacyMode();
        bool TestInitialise_WaitUntilMode();
        bool TestInitialise_WaitUntilHRMode();
        bool TestInitialise_SleepMode();
        bool TestInitialise_SleepHRMode();
        bool TestInitialise_InvalidMode_Fail();
        bool TestInitialise_LegacyTcnPollTrue();
        bool TestInitialise_LegacyTcnPollFalse();
        bool TestInitialise_WithTolerance();
        bool TestInitialise_WithInvalidTcnDevice_Fail();
        bool TestInitialise_WithMissingTcnDevice_Fail();

    private:
        ConfigurationDatabase tcnCfg;
        bool PreInitialise(bool noPreInit);
        bool TestInitialise_ConfigurableMode(TcnTimeProviderTestInitialiseMode mode);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TCNTIMEPROVIDER_TNCTIMEPROVIDERTEST_H_ */

