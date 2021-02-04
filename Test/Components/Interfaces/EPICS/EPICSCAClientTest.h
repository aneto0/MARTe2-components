/**
 * @file EPICSCAClientTest.h
 * @brief Header file for class EPICSCAClientTest
 * @date 27/03/2017
 * @author Andre Neto
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
 *
 * @details This header file contains the declaration of the class EPICSCAClientTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSCACLIENTTEST_H_
#define EPICSCACLIENTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the EPICSCAClient public method.
 */
class EPICSCAClientTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method with the default parameters.
     */
    bool TestInitialise_Defaults();

    /**
     * @brief Tests the Initialise method without the default parameters.
     */
    bool TestInitialise_NoDefaults();

    /**
     * @brief Tests the GetCPUMask method.
     */
    bool TestGetCPUMask();

    /**
     * @brief Tests the GetStackSize method.
     */
    bool TestGetStackSize();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests the Start method.
     */
    bool TestStart();

    /**
     * @brief Tests the GetStatus method.
     */
    bool TestGetStatus();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSCACLIENTTEST_H_ */

