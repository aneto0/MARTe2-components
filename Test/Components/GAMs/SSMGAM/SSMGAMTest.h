/**
 * @file SSMGAMTest.h
 * @brief Header file for class SSMGAMTest
 * @date 01/09/2017
 * @author Llorenc Capella
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

 * @details This header file contains the declaration of the class SSMGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SSMGAMTEST_H_
#define SSMGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "SSMGAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class SSMGAMTest: public SSMGAM {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API SSMGAMTest)
public:
    SSMGAMTest();
    virtual ~SSMGAMTest();

    /**
     * @brief Test error messages of SSM::Initialise().
     * @details Call Initialise() without state matrix
     */
    bool TestInitialiseNoStateMatrix();

    /**
     * @brief Test error messages of SSM::Initialise().
     * @details define state matrix with dimension 1x2.
     */
    bool TestInitialiseStateMatrix1x2();

    /**
     * @brief Test error messages of SSM::Initialise().
     * @details Call Initialise() without input matrix
     */
    bool TestInitialiseNoInputMatrix();

    /**
     * @brief Test error messages of SSM::Initialise().
     * @details define state matrix with dimension 2x2 and
     * an input matrix with dimension 1x2.
     */
    bool TestInitialiseWrongInputMatrixDimension();

    /**
     * @brief Test error messages of SSM::Initialise().
     * @details Call Initialise() without output matrix
     */
    bool TestInitialiseNoOutputMatrix();

    /**
     * @brief Test error messages of SSM::Initialise().
     * @details define state matrix with dimension 2x2,
     * an input matrix with dimension 2x1 and output matrix dimension 2x1.
     */
    bool TestInitialiseWrongOutputMatrixDimension();

    /**
     * @brief Test error messages of SSM::Initialise().
     * @details Call Initialise() without feedthough matrix
     */
    bool TestInitialiseNoFeedthoughMatrix();

    /**
     * @brief Test error messages of SSM::Initialise().
     * @details define state matrix with dimension 2x2,
     * an input matrix with dimension 2x1, output matrix dimension 1x2
     * and feedthough matrix 2x1.
     */
    bool TestInitialiseWrongFeedthroughMatrixDimension();

    /**
     * @brief Test error messages of SSM::Initialise().
     * @details define state matrix with dimension 2x2,
     * an input matrix with dimension 2x1, output matrix dimension 1x2
     * and feedthough matrix 1x2.
     */
    bool TestInitialiseWrongFeedthroughMatrixDimension_2();

    /**
     * @brief Test SSM::Initialise().
     * @details define state matrix with dimension 2x2,
     * input matrix with dimension 2x1, output matrix dimension 1x2
     * and feedthough matrix 1x1.
     */
    bool TestInitialise();

    /**
     * @brief Test SSM::Initialise().
     * @details define state matrix with dimension 4x4,
     *  input matrix with dimension 4x2, output matrix dimension 1x4
     * and feedthough matrix 4x2.
     */
    bool TestInitialise_2();

    /**
     * @brief Test SSM::Setup() error messages.
     * @details number of input signals = 2
     */
    bool TestSetupWrongInputSignals();

    /**
     * @brief Test SSM::Setup() error messages.
     * @details number of output signals = 2
     */
    bool TestSetupWrongOutputsSignals();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupWrongInputType();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupWrongOutputType();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupNoInputElements();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupWrongInputElements();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupNoOutputElements();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupWrongOutputElements();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupNoInputDimensions();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupWrongInputDimensions();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupNoOutputDimensions();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupWrongOutputDimensions();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupNoInputSamples();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupWrongInputSamples();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupNoOutputSamples();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupWrongOutputSamples();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupWrongOutputSamples_2();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetupWrongOutputSamples_3();

    /**
     * @brief Test SSM::Setup() error messages.
     */
    bool TestSetup();

    /**
     * @brief Test SSM::Execute().
     */
    bool TestExecute();

    /*
    bool TestInitialiseoColumnsStateMatrix();//cannot be tested
    */
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SSMGAMTEST_H_ */

