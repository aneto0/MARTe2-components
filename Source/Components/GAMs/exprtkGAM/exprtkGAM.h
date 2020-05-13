/**
 * @file MathExpressionGAM.h
 * @brief Header file for class MathExpressionGAM
 * @date Jul 30, 2019
 * @author RFX team
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
#include "StructuredDataI.h"
#include "MessageI.h"

#include "exprtk.hpp"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A GAM that wraps the C++ exprtk library 
 * for run-time expression evaluation.
 * @details This GAM combines the provided input signals into an arbitrary
 * number of output signals in the way specified by the Expression leaf
 * of each output signal.
 * 
 * The Expression leaf must be a string containing the expression that will be
 * evaluated during run-time in string format. For details on how to write
 * a valid expression see the exprtk library documentation. The "Expression"
 * leaf is compulsory. Also note that the assignment operator is :=, while equal
 * sign is used for equality checks. For details on expression syntax
 * check exprtk documentation.
 * Multi-line expression are supported (and often required, such as when
 * vector handling must be performed). See example below or exprtk documentation
 * for details.
 * 
 * The GAM supports scalar and vector signals. Matrices are not supported.
 * When operations between vectors are carried out, result has the size of
 * the smallest vector. To perform operations between vector of different sizes
 * consider using a for-loop (see example below).
 * 
 * For the sake of simplicity, each signal is typecasted to float64 before
 * being passed to the library, and typecasted back to its native type
 * when copied in the GAM output memory.
 * 
 * The configuration syntax is (names and expression are only given as an example):
 * 
 * <pre>
 * +ExprGAM = {
 *     Class = MathExpressionGAM
 *     InputSignals = {               // As many as required.
 *         In1 = {
 *             Type = float64
 *             NumberOfElements = 1
 *         }
 *         In2 = {
 *             Type = int32
 *             NumberOfElements = 2
 *         }
 *     }
 *     OutputSignals = {                                // As many as required.
 *         Out1 = {                                     // Name can be changed.
 *             Expression = "Out1 := In1 + In2[0];"     // This must be one and only one string.
 *             Type = float32
 *         }
 *         Out2 = {
 *             Expression = "                                  // Multi-line expression
 *                           for (var i := 0; i < 2; i += 1)   // Vectors must be dealt with for-loops.
 *                           {
 *                               Out2[i] := In1 * In2[i];
 *                           }
 *                          "
 *             Type = float64
 *         }
 *     }
 * }
 * </pre>
 * 
 * @todo Add support for constants specified in configuration file.
 */

class MathExpressionGAM : public GAM {
public:
	CLASS_REGISTER_DECLARATION()
	
	MathExpressionGAM();
	
	virtual ~MathExpressionGAM();
	
	/**
	 * @brief Initializes and retrieves informations from configuration file. 
	 * @details During the initialization phase number of inputs and outputs are
	 * read from the configuration file and expression are stored.
	 * @param[in] data the GAM configuration specified in the configuration file.
	 * @return true on succeed.
	 * @pre Each output signal must have an "Expression" parameter.
	 */
	virtual bool Initialise(StructuredDataI & data);
	
	/**
	 * @brief Checks parameters and compile exprtk expressions. 
	 * @details This method:
	 * 1. checks if types and dimensions retrieved from the configuration file are correct,
	 * 2. allocates a local float64 recast of GAM memory,
	 * 3. uses exprtk library to parse and compile expressions.
	 * 
	 * Expressions are compiled here since one of strenghts of exprtk is that
	 * expressions do not need to be recompiled each time they are evaluated,
	 * even if variables in the expression change their value.
	 * @return true on succeed.
	 * @pre Initialise().
	 */
	virtual bool Setup();
	
	/**
	 * @brief Evaluates expressions from configuration file and outputs them. 
	 * @details During ecxecution, expressions for each output signal are
	 * evaluated with data from input signals and results are copied in output
	 * memory.
	 * @return true
	 * @pre
	 *	Initialise() &&
	 *	Setup()
	 */
	virtual bool Execute();
	
private:
	
	/**
	 * @name Debugging
	 */
	//@{
	float32 sleepTime;	//!< To put the execute method to sleep. Set to 0 to avoid sleep. @warning For debugging purpose only.
	//@}
	
	/**
	 * @name Signal data
	 * Informations on inputs and outputs in this GAM are stored here.
	 */
	//@{
	uint32          numInputSignals;		//!< Number of input signals retrieved from configuration file.
	uint32          numOutputSignals;		//!< Number of output signals retrieved from configuration file.
	TypeDescriptor* inputSignalType;
	TypeDescriptor* outputSignalType;
	uint32*         inputNumOfElements;
	uint32*         outputNumOfElements;
	//@}
	
	/**
	 * @name Local copy of signal memory
	 */
	//@{
	std::vector< std::vector<float64> > localInputArray;	//!< Vector of vectors where a float64 recast of inputs is stored locally.
	std::vector< std::vector<float64> > localOutputArray;	//!< Vector of vectors where a float64 recast of outputs is stored locally.
	//@}
	
	/**
	 * @name Expressions of output signals
	 * Expressions to be evaluated during run-time are stored here.
	 */
	//@{
	StreamString* expressionString;		//!< String expression for the output.
	uint64        numOfSignalVariables;	//!< Number of variables used in the expression.
	//@}
	
	/**
	 * @name exprtk library objects
	 */
	//@{
	exprtk::symbol_table<float64> symbolTable;		//<! Stores variables.
	exprtk::parser<float64>       expressionParser;	//<! Parses the expressions.
	exprtk::expression<float64>*  outputExpression;	//<! Stores one expression for each output signal declared in the configuration file.
	//@}
	
	/**
	 * @brief Lists all signal names.
	 * @param[out] names Pointer to an array that stores the retrieved names.
	 * Must be preallocated beforehand.
	 */
	bool GetSignalNames(const SignalDirection direction,
						StreamString* names);
	
};

} /* Namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif 
	
