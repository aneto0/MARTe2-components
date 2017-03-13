/**
 * @file StatisticsGAMTest.cpp
 * @brief Source file for class StatisticsGAMTest
 * @date 30/01/2017
 * @author Bauvir Bertrand 
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
 * the class StatisticsGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "StatisticsGAM.h"
#include "StatisticsGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

StatisticsGAMTest::StatisticsGAMTest() {
}

StatisticsGAMTest::~StatisticsGAMTest() {
}

bool StatisticsGAMTest::TestConstructor() {
    using namespace MARTe;
    StatisticsGAM gam;

    bool ok = true;

    return ok;
}

bool StatisticsGAMTest::TestInitialise() {
    using namespace MARTe;
    StatisticsGAM gam;
    ConfigurationDatabase config;
    bool ok = gam.Initialise(config);
    return ok;
}

bool StatisticsGAMTest::TestInitialise_WindowSize() {
    using namespace MARTe;
    StatisticsGAM gam;
    ConfigurationDatabase config;

    uint32 size = 1000u;
    bool ok = true;

    ok &= config.Write("WindowSize", size);
    ok &= gam.Initialise(config);
    return ok;
}

bool StatisticsGAMTest::TestSetup() {
    using namespace MARTe;
    StatisticsGAM gam;

    bool ok = true;

    return ok;
}

bool StatisticsGAMTest::TestExecute() {
    using namespace MARTe;
    StatisticsGAM gam;

    bool ok = true;

    return ok;
}

bool StatisticsGAMTest::TestPrepareForNextState_Error() {
    using namespace MARTe;
    StatisticsGAM gam;

    bool ok = gam.PrepareNextState("FromCurrent","ToNext"); // Test the instantiated class;
    return !ok; // Expect failure since StatisticsHelperT<> class has not yet been instantiated
}

bool StatisticsGAMTest::TestPrepareForNextState_Success() {
    using namespace MARTe;
    StatisticsGAM gam;

    bool ok = gam.PrepareNextState("FromCurrent","ToNext"); // Test the instantiated class;
    return ok; // Expect failure since StatisticsHelperT<> class has not yet been instantiated
}

