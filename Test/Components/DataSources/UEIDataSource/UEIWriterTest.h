/**
 * @file UEIWriterTest.h
 * @brief Header file for class UEIWriterTest
 * @date 02/05/2023
 * @author Xavier Ruche
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

 * @details This header file contains the declaration of the class UEIWriterTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_DATASOURCES_UEIWriterTEST_H_
#define TEST_COMPONENTS_DATASOURCES_UEIWriterTEST_H_

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
#include "UEIWriter.h"
#include "SafeMath.h"
#include "GlobalObjectsDatabase.h"
#include "StandardParser.h"
#include "UEIDAQMockup.h"
#include "RealTimeApplication.h"
#include "IOGAM.h"

using namespace MARTe;

class UEIWriterTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();
    bool TestInitialise();
    bool TestInitialise_TrueClass();
    bool TestInitialise_TwoDsPerMap();
    bool TestSimpleMethods();
    bool TestStartup();
    bool TestStartup_InvalidChannelNumber();
    bool TestStartup_InvalidNumberOfElements();
    bool TestStartup_InvalidNumberOfDimensions();
    bool TestStartup_MismatchedSampleNumber();
    bool TestStartup_MultipleDMapSamples();
    bool TestSync();
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_DATASOURCES_UEIWriterTEST_H_ */
	
