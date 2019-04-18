/**
 * @file OPCUADSInputTest.h
 * @brief Header file for class OPCUADSInputTest
 * @date 15/03/2019
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

 * @details This header file contains the declaration of the class OPCUADSInputTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSINPUTTEST_H_
#define TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSINPUTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class OPCUADSInputTest {
public:

    bool TestConstructor();

    bool TestInitialise_Default();

    bool TestInitialise_Introspection();

    bool TestInitialise_Sync();

    bool TestInitialise_SyncMonitor();

    bool TestInitialise_SyncWrongMode();

    bool TestInitialise_Thread();

    bool TestInitialise_ThreadMonitor();

    bool TestInitialise_ThreadWrongMode();

    bool TestInitialise_NoAddress();

    bool TestInitialise_NoSignals();

    bool TestInitialise_NoPath();

    bool TestInitialise_NoNamespaceIndex();

    bool Test_NumberOfDimensionsGreater1();

    bool Test_FailSetTargetNodes();

    bool Test_Read();

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUADSINPUTTEST_H_ */
	
