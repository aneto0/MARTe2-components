/**
 * @file MathExpressionGAM.cpp
 * @brief Header file for class MathExpressionGAM
 * @date 12/06/2020
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

 * @details This header file contains the declaration of the class MathExpressionGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATHEXPRESSIONGAM_H_
#define MATHEXPRESSIONGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAM.h"
#include "MathExpressionParser.h"
#include "RuntimeEvaluator.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
 
 /**
 * @brief GAM to evaluate mathematical expressions at runtime using
 *        MARTe2 internal evaluator engine RuntimeEvaluator 
 * 
 * @details This GAM combines the provided input signals into an arbitrary
 * number of output signals in the way specified by the `Expression` leaf.
 * The `Expression` leaf must be a string containing the expressions
 * that will be evaluated during run-time in string format. The `Expression`
 * leaf is compulsory.
 * The `Expression` leaf must be in infix form and must be terminated 
 * by a comma, a semicolon or a `\n` (e.g. `ret = A + B;`).
 * For example, a MathExpressionGAM with two input signals `In1` and
 * `In2` that need to be summed up in the output signal `Out` will
 * have the following `Expression` leaf:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Expression = "Out = In1 + In2;"
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Multiple output signals can be handled by using more than one
 * assignment in the same expression:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Expression = "
 *               Out1 = In1 + In2;
 *               Out2 = 10*In1;
 *              "
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * The output variable of a previous assignment can be used as input
 * variable of a subsequent assignment:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Expression = "
 *               Out1 = In1 + In2;
 *               Out2 = 10*Out1;
 *              "
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Supported operators are the following:
 * 
 * | Operator | Meaning            |
 * | :------: | :----------------- |
 * | =        | Assignment         |
 * | &&       | AND                |
 * | \|\|     | OR                 |
 * | ^        | Exclusive OR       |
 * | !        | NOT                |
 * | <        | Less than          |
 * | >        | Greater than       |
 * | <=       | Less ot equal      |
 * | >=       | Greater or equal   |
 * | ==       | Equal              |
 * | !=       | Not equal          |
 * | +        | Sum                |
 * | -        | Subtraction        |
 * | *        | Multiplication     |
 * | /        | Division           |
 * | `sin()`  | Sine               |
 * | `cos()`  | Cosine             |
 * | `pow()`  | Power              |
 * | `(type)` | Typecast to `type` |
 * | , ; `\n` | End of expression  |
 * 
 * 
 * The GAM supports:
 *  - signals of any numeric type
 *  - scalar signals only
 * 
 * During initialisation, each variable in the expression is automatically
 * associated to the signal with the same name:
 * - variables on the right-hand side each expression are considered
 *   *input variables* and are associated to input signals. Note that
 *   each variable on the right-hand side must have an associated
 *   input signal with the same. An input variable with no input signal
 *   of the same name (and viceversa) will cause the GAM to fail initialisation.
 * - variables on the left-hand side of the expression are considered
 *   *output variables* and are associated to output signals.
 *   Output variables are not required to have a corresponding output
 *   signal since they can be used as bridge values between exepressions:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Expression = "
 *               Out1 = In1 + In2;
 *               temp = 2*In1;
 *               Out2 = Out1 + temp;
 *              "
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Constants are supported both in numeric and in literal form.
 * Constants in literal form must be defined within the same expression
 * as shown in the following example:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Expression = "
 *               G = 0.00000000006674;
 *               F = G*(m1 + m2)/pow(r,2);
 *              "
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * In the example above, `m1`, `m2` and `r` are input signals, `G` is
 * a literal constant and `2` is a numeric constant. Note that numeric
 * constants are treated as `float64` if not otherwise specified.
 * 
 * Particular care shall be placed in *type combinations*: some operations
 * on variables of different type may not be available based on the 
 * availability of a corresponding function in the functionRecord database
 * (see RuntimeEvaluator and RuntimeEvaluatorFunctions documentation
 * for details). Lack of an adequate function for treating a certain
 * type combinations can be overcome by using typecasts:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * G = 0.00000000006674;
 * F = (float64) G*((float64) m1 + (float64) m2)/pow((float64) r, (float64) 2);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * The configuration syntax is (signal names are only given as
 * an example and can be changed):
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * +ExprGAM = {
 *     Class = MathExpressionGAM
 *     Expression = "
 *                   pi = 3.14;
 *                   Out1 = ( In1 + (float32) In2 ) * ((float32) 10);
 *                   Out2 = (float64) Out1 + pi + 10;
 *                  "
 *     InputSignals = {               // As many as required.
 *         In1 = {
 *             Type = float32
 *             NumberOfElements = 1
 *         }
 *         In2 = {
 *             Type = int32
 *             NumberOfElements = 1
 *         }
 *     }
 *     OutputSignals = {              // As many as required.
 *         Out1 = {
 *             Type = float32
 *         }
 *         Out2 = {
 *             Type = float64
 *         }
 *     }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 */
class MathExpressionGAM: public GAM {
    
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    MathExpressionGAM();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MathExpressionGAM();
    
    /**
     * @brief     Initializes and retrieves informations from
     *            configuration file. 
     * @details   During the initialization phase, number of inputs
     *            and outputs are read from the configuration file
     *            and the `Expression` is stored.
     * @param[in] data the GAM configuration specified in the
     *                 configuration file.
     * @return    `true` on succeed.
     */
    virtual bool Initialise(StructuredDataI &data);
    
     /**
     * @brief   Checks parameters and compile the expressions. 
     * @details This method:
     *          1. checks if signal dimensions retrieved from
     *             the configuration file are compatible with the GAM
     *          2. set the types of each variable according
     *             to signal types
     *          3. set the memory location of each variable to be
     *             the same of the corresponding signal memory,
     *             so that no memcopy is required during execution
     *          4. compiles the expression (the expression does not
     *             need to be recompiled each time it is evaluated).
     * 
     * @return  `true` on succeed.
     * @pre     
     *          1. Initialise() == `true`
     *          2. all input variables can find an associated
     *            input signal of the same name.
     */
    virtual bool Setup();

    /**
     * @brief  Evaluates the expression. 
     * @return `true` on succeed.
     * @pre    
     *         1. Initialise() == `true`
     *         2. Setup() == `true`
     */
    virtual bool Execute();

protected:

    /**
     * @brief Pointer to the instance of the MathExpressionParser
     *        that will parse the input expression.
     */
    MathExpressionParser* mathParser;
    
    /**
     * @brief Pointer to the instance of the RuntimeEvaluator
     *        that will evaluate the input expression.
     */
    RuntimeEvaluator*     evaluator;

private:
    
    /**
     * @brief Structure to hold information about signals.
     */
    struct SignalStruct {
        
        StreamString    name;
        TypeDescriptor  type;
        uint32          numberOfElements;
        
    };
    
    /**
     * @brief Instances of SignalStruct for inputs and outputs.
     */
    //@{
    SignalStruct*  inputSignals;
    SignalStruct* outputSignals;
    //@}
    
    /**
     * @brief The expression to be evaluated.
     */
    StreamString expr;
    
};

} /* MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MATHEXPRESSIONGAM_H_ */

