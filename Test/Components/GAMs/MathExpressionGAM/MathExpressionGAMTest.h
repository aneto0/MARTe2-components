/**
 * @file MathExpressionGAMTest.h
 * @brief Header file for class IOGAMGTest
 * @date 03/06/2020
 * @author RFX
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

 * @details This header file contains the declaration of the class IOGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATHEXPRESSIONGAMTEST_H_
#define MATHEXPRESSIONGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MathExpressionGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the MathExpressionGAM public methods.
 */
class MathExpressionGAMTest {
    
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();
    
    /**
     * @brief Tests the Initialise() method.
     */
    bool TestInitialise(const char8* expression);
    
    /**
     * @brief Tests the Setup method.
     */
    bool TestSetup();

    /**
     * @brief   Tests the Setup method.
     * @details This test fails since one of the input signals can't
     *          be associated with a variable (there is no variable
     *          with the same name as the input signal).
     */
    bool TestSetup_Failed_InputSignalMissingVariable();
    
    /**
     * @brief   Tests the Setup method.
     * @details This test fails since one of the output signals can't
     *          be associated with a variable (there is no variable
     *          with the same name as the output signal).
     */
    bool TestSetup_Failed_OutputSignalMissingVariable();
    
    /**
     * @brief   Tests the Setup method.
     * @details This test fails since one of the right-hand side variables can't
     *          be associated with an input signal (there is no input signal
     *          with the same name as the variable).
     */
    bool TestSetup_Failed_VariableMissingInputSignal();
    
    /**
     * @brief   Tests the Setup method.
     * @details This test succeed since even if one of the left-hand side variables can't
     *          be associated with an output signal (there is no output signal
     *          with the same name as the variable), this is not considered
     *          a problem (a left-hand side variable with no associated
     *          output signal is considered a temporary variable).
     */
    bool TestSetup_Succeed_VariableMissingOutputSignal();
    
    /**
     * @brief   Tests the Setup method.
     * @details This test fails since MathExpressionGAM supports only
     *          scalar signals.
     */
    bool TestSetup_Failed_NonScalarInput();
    
    /**
     * @brief   Tests the Setup method.
     * @details This test fails since MathExpressionGAM supports only
     *          scalar signals.
     */
    bool TestSetup_Failed_NonScalarOutput();
    
    /**
     * @brief   Tests the memory mapping.
     * @details Checks that memory mapping between MathExpressionGAM
     *          and the underlying RuntimeEvaluator is done properly.
     */
    bool TestMemory();
    
    /**
     * @brief   Tests the type mapping.
     * @details Checks that type mapping between MathExpressionGAM
     *          and the underlying RuntimeEvaluator is done properly.
     */
    bool TestTypes();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute_SingleExpression();
    
    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute_MultipleExpressions();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MATHEXPRESSIONGAMTEST_H_ */

