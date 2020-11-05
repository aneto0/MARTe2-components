/**
 * @file OPCUAClientITest.h
 * @brief Header file for class OPCUAClientITest
 * @date 28/10/2019
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

 * @details This header file contains the declaration of the class OPCUAClientITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTITEST_H_
#define TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class OPCUAClientITest {
public:

    /**
     * @brief Tests the constructor. NOOP
     */
    bool TestConstructor();

    /**
     * @brief Tests the SetServerAddress function.
     */
    bool Test_SetServerAddress();

    /**
     * @brief Tests the Connect function.
     */
    bool Test_Connect();

    /**
     * @brief Tests the SetValueMemories function.
     */
    bool Test_SetValueMemories();

    /**
     * @brief Tests the SetDataPtr function.
     */
    bool Test_SetDataPtr();

    /**
     * @brief Tests the GetSignalMemory function.
     */
    bool Test_GetSignalMemory();



};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_DATASOURCES_OPCUADATASOURCE_OPCUACLIENTITEST_H_ */
	
