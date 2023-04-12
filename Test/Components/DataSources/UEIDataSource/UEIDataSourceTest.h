/**
 * @file UEIDataSourceTest.h
 * @brief Header file for class UEIDataSourceTest
 * @date 10/06/2021
 * @author Luca Porzio
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

 * @details This header file contains the declaration of the class UEIDataSourceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_DATASOURCES_UEIDATASOURCETEST_H_
#define TEST_COMPONENTS_DATASOURCES_UEIDATASOURCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"
#include "ConfigurationDatabase.h"
#include "UEIDataSource.h"
#include "SafeMath.h"
#include "GlobalObjectsDatabase.h"
#include "StandardParser.h"
#include "UEIDAQMockup.h"
#include "RealTimeApplication.h"
#include "IOGAM.h"

using namespace MARTe;

class UEIDataSourceTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();
    bool TestInitialise();
    bool TestInitialise_NoDevice();
    bool TestInitialise_NoMap();
    bool TestInitialise_InvalidDevice();
    bool TestInitialise_InvalidMap();
    bool TestInitialise_TwoDSperMap();
    bool TestStartup();
    bool TestStartup_MismatchedSignalNumber();
    bool TestStartup_BadStatusSignal();
    bool TestStartup_BadTimestampSignal();
    bool TestStartup_MultipleElements();
    bool TestStartup_MultipleFunctions();
    bool TestStartup_MismatchedStatusSamples();
    bool TestStartup_MismatchedSampleNumber();
    bool TestStartup_MultipleSamplesDMap();
    bool TestStartup_SingleSamplesVMap();
    bool TestStartup_MultipleSamplesVMap();
    bool TestStartup_OutputSignals();
    bool TestStartup_InvalidMapType();
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_DATASOURCES_UEIDATASOURCETEST_H_ */
	
