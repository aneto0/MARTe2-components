/**
 * @file BaseLib2GAMTest.h
 * @brief Header file for class BaseLib2GAMTest
 * @date 2/12/2016
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class BaseLib2GAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASELIB2GAMTEST_H_
#define BASELIB2GAMTEST_H_

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
 * @brief Tests the BaseLib2GAM public methods.
 * @details It uses the BaseLib2 TypeConvertGAM to demonstrate the interface
 */
class BaseLib2GAMTest {
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
     * @brief Tests the Initialise method without specifying the GAMName.
     */
    bool TestInitialise_False_NoGAMName();

    /**
     * @brief Tests the Initialise method without specifying a wrong GAMName.
     */
    bool TestInitialise_False_WrongGAMName();

    /**
     * @brief Tests the Initialise method without specifying the GAMFunctionNumber.
     */
    bool TestInitialise_False_NoGAMFunctionNumber();

    /**
     * @brief Tests the Initialise method without specifying the BaseLib2Config.
     */
    bool TestInitialise_False_NoBaseLib2Config();

    /**
     * @brief Tests the Initialise method specifying an unloadable BaseLib2Config.
     */
    bool TestInitialise_False_WrongBaseLib2Config();

    /**
     * @brief Tests the Setup method.
     */
    bool TestSetup();

    /**
     * @brief Tests the Setup method with a mismatch in the input signals.
     */
    bool TestSetup_InputSignals_Mismatch();

    /**
     * @brief Tests the Setup method with a mismatch in the output signals.
     */
    bool TestSetup_OutputSignals_Mismatch();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests the Execute method with arrays.
     */
    bool TestExecute_Arrays();

    /**
     * @brief Tests the TranslateSignalName method.
     */
    bool TestTranslateSignalName();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASELIB2GAMTEST_H_ */

