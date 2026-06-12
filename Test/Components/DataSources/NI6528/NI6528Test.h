/**
 * @file NI6528Test.h
 * @brief Header file for class NI6528Test
 * @date Mar 2, 2026 TODO Verify the value and format of the date
 * @author ferrog TODO Verify the name and format of the author
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

 * @details This header file contains the declaration of the class NI6528Test
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MARTE2_COMPONENTS_TEST_COMPONENTS_DATASOURCES_NI6528_NI6528TEST_H_
#define MARTE2_COMPONENTS_TEST_COMPONENTS_DATASOURCES_NI6528_NI6528TEST_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "NI6528.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
class NI6528Test {
public:
    bool TestConstructor();

    bool TestInitialise();

    bool TestInitialise_FalseDoublePort();

    bool TestInitialise_FalseInvalidPort();

    bool TestSetConfiguredDatabase();

    bool TestSetConfiguredDatabase_FalseNSignals();

    bool TestSetConfiguredDatabase_FalseSignalType();

    bool TestSynchronise();

    bool TestAllocateMemory();

    bool TestGetSignalMemoryBuffer();

    bool TestGetBrokerName();

    bool TestPrepareNextState();
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MARTE2_COMPONENTS_TEST_COMPONENTS_DATASOURCES_NI6528_NI6528TEST_H_ */
	
