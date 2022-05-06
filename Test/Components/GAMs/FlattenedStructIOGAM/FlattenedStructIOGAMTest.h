/**
 * @file FlattenedStructIOGAMTest.h
 * @brief Header file for class FlattenedStructIOGAMTest
 * @date 30/04/2022
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

 * @details This header file contains the declaration of the class FlattenedStructIOGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FLATTENEDSTRUCTIOGAMTEST_H_
#define FLATTENEDSTRUCTIOGAMTEST_H_

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
 * @brief Tests the FlattenedStructIOGAM public methods.
 */
class FlattenedStructIOGAMTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Setup method.
     */
    bool TestSetup();

    /**
     * @brief Tests the Initialise method with more than one input signal.
     */
    bool TestInitialise_MoreThanOneInput();

    /**
     * @brief Tests the Initialise method without defining the signal type.
     */
    bool TestInitialise_NoType();

    /**
     * @brief Tests the Initialise method without defining the signal DataSource.
     */
    bool TestInitialise_NoDataSource();

    /**
     * @brief Tests the Setup method with no Trigger signal.
     */
    bool TestSetup_NoTrigger();

    /**
     * @brief Tests the Setup method with different signal sizes.
     */
    bool TestSetup_False_DifferentSize();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FLATTENEDSTRUCTIOGAMTEST_H_ */

