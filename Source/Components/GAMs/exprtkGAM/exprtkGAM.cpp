/**
 * @file MathExpressionGAM.cpp
 * @brief Source file for class MathExpressionGAM
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

 * @details This source file contains the definition of all the methods for the
 * class MathExpressionGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MathExpressionGAM.h"
#include "AdvancedErrorManagement.h"
#include <iostream>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MathExpressionGAM::MathExpressionGAM() : GAM(){

}

MathExpressionGAM::~MathExpressionGAM() {
}

bool MathExpressionGAM::Initialise(StructuredDataI & data) {
	
	bool ok = GAM::Initialise(data);
	
	// Debug
	REPORT_ERROR(ErrorManagement::Debug, "MathExpressionGAM: INITIALIZE");
	ok = data.Read("SleepTime", sleepTime);
	if (!ok) {
		REPORT_ERROR(ErrorManagement::Debug,
					 "Sleep time not set. By default it will be zeroed.");
		sleepTime = 0;
	}
	
	if (!ok) {
		REPORT_ERROR(ErrorManagement::InitialisationError,
					 "Initialization failed.");
		return ok;
	}
	
	/**
	 * Firstly, number of inputs and outputs is read from the configuration file.
	 */
	
	ok = data.MoveRelative("InputSignals");
	if(!ok){
		REPORT_ERROR(ErrorManagement::ParametersError,
					 "Cannot find InputSignals node in configuration file.");
		return ok;
	}
	
	numInputSignals = data.GetNumberOfChildren();
	
	// Back to the GAM node
	data.MoveToAncestor(1);
	
	ok = data.MoveRelative("OutputSignals");
	if(!ok){
		REPORT_ERROR(ErrorManagement::ParametersError,
					 "Cannot find OutputSignals node in configuration file.");
		return ok;
	}
	
	numOutputSignals = data.GetNumberOfChildren();
	
	/**
	 * Then expression of each output signal is read and stored.
	 */
	
	expressionString = new StreamString[numOutputSignals];
	
	for (uint32 sigIdx = 0; sigIdx < numOutputSignals; sigIdx++) {
		
		// Move the cursor to output signal
		ok = data.MoveToChild(sigIdx);
		if(!ok) {
			REPORT_ERROR(ErrorManagement::ParametersError,
							"OutputSignals node has no child.");
			return ok;
		}
		
		// Expression is read and stored in expressionStringArray
		ok = data.Read("Expression", expressionString[sigIdx]);
		if(!ok) {
			REPORT_ERROR(ErrorManagement::ParametersError,
							"Expression leaf is missing (or is not a string) for output signal %s.",
							data.GetName());
			return ok;
		}
		
		// Pointer must be reset to the GAM node
		ok = data.MoveToAncestor(1);
		if(!ok){
			REPORT_ERROR(ErrorManagement::InitialisationError,
						"Failed MoveToAncestor() from %s", data.GetName());
			return ok;
		}
		
	}
	
	// Pointer goes back to the GAM node
	ok = data.MoveToAncestor(1);
	if(!ok){
		REPORT_ERROR(ErrorManagement::InitialisationError,
					 "Failed MoveToAncestor() from %s", data.GetName());
		return ok;
	}
	
	return ok;
}

bool MathExpressionGAM::Setup() {
	
	bool ok;
	
	//REPORT_ERROR(ErrorManagement::Debug, "SETUP");
	
	/**
	 * Firstly, types and dimensions of signals are retrieved and checked.
	 */
	inputNumOfElements  = new uint32[numInputSignals];
	inputSignalType     = new TypeDescriptor[numInputSignals];
	
	outputNumOfElements = new uint32[numOutputSignals];
	outputSignalType    = new TypeDescriptor[numOutputSignals];
	outputExpression    = new exprtk::expression<float64>[numOutputSignals];
	
	// Inputs
	for (uint32 sigIdx = 0; sigIdx < numInputSignals; sigIdx++) {
		
		inputSignalType[sigIdx] = GetSignalType(InputSignals, sigIdx);
		
		ok = GetSignalNumberOfElements(InputSignals, sigIdx, inputNumOfElements[sigIdx]);
		if (!ok) {
			
			REPORT_ERROR(ErrorManagement::Exception,
						 "Error in GetSignalNumberOfElements: signal %i does not exist.",
						 sigIdx);
			return ok;
			
		}
		
	}
	
	// Outputs
	for (uint32 sigIdx = 0; sigIdx < numOutputSignals; sigIdx++) {
		
		outputSignalType[sigIdx] = GetSignalType(OutputSignals, sigIdx);
		
		ok = GetSignalNumberOfElements(OutputSignals, sigIdx, outputNumOfElements[sigIdx]);
		if (!ok) {
			
			REPORT_ERROR(ErrorManagement::Exception,
						 "Error in GetSignalNumberOfElements: signal %i does not exist.",
						 sigIdx);
			return ok;
			
		}
		
	}
	
	/**
	 * Then a local memory is allocated that will contain float64 versions of
	 * input and output signals.
	 */
	
	localInputArray.resize(numInputSignals);
	
	for (uint32 sigIdx = 0; sigIdx < numInputSignals; sigIdx++) {
		
		localInputArray[sigIdx].resize(inputNumOfElements[sigIdx]);
		
	}
	
	localOutputArray.resize(numOutputSignals);
	
	for (uint32 sigIdx = 0; sigIdx < numOutputSignals; sigIdx++) {
		
		localOutputArray[sigIdx].resize(outputNumOfElements[sigIdx]);
		
	}
	
	/**
	 * Finally exprtk is invoked.
	 */
	
	// Now we preallocate an array to store the names
	StreamString xNames[numInputSignals];
	StreamString yNames[numOutputSignals];
	
	///1. The list of all variables concurring to form this output is retrieved.
	ok = GetSignalNames(InputSignals, xNames);
	if (!ok) {
		
		REPORT_ERROR(ErrorManagement::Exception,
					 "Something went wrong while getting input variable names.");
		
		return ok;
	}
	
	ok = GetSignalNames(OutputSignals, yNames);
	if (!ok) {
		
		REPORT_ERROR(ErrorManagement::Exception,
					 "Something went wrong while getting output variable names.");
		
		return ok;
	}
	
	///2. Variables are added to the symbol table. However, since exprtk can
	///   manage up to vectors, all variables are treated as vectors for the
	///   sake of simplicity.
	
	// Inputs
	for (uint32 sigIdx = 0; sigIdx < numInputSignals; sigIdx++) {
		
		// To add a variable both the name of the signal and its memory address
		// are passed to the function
		ok = symbolTable.add_vector(xNames[sigIdx].Buffer(), localInputArray[sigIdx]);
		if (!ok) {
			
			REPORT_ERROR(ErrorManagement::Exception,
						 "add_vector() returned false while registering input %i",
						 sigIdx);
			
			return ok;
		}
		
	}
	
	// Outputs
	for (uint32 sigIdx = 0; sigIdx < numOutputSignals; sigIdx++) {
		
		// To add a variable both the name of the signal and its memory address
		// are passed to the function
		ok = symbolTable.add_vector(yNames[sigIdx].Buffer(), localOutputArray[sigIdx]);
		if (!ok) {
			
			REPORT_ERROR(ErrorManagement::Exception,
						 "add_vector() returned false while registering output %i",
						 sigIdx);
			
			return ok;
		}
		
	}
	
	///3. The symbol table is passed to each expression and compiled.
	for (uint32 sigIdx = 0; sigIdx < numOutputSignals; sigIdx++) {
		
		outputExpression[sigIdx].register_symbol_table(symbolTable);
		
		ok = expressionParser.compile(expressionString[sigIdx].Buffer(), outputExpression[sigIdx]);
		if (!ok) {
			
			REPORT_ERROR(ErrorManagement::ParametersError,
						 "Compilation of expression for output n. %i failed. Check the expression syntax.",
						 sigIdx);
			
			// Print a log of each detected error.
			for (uint32 i = 0; i < expressionParser.error_count(); ++i) {
				
				exprtk::parser_error::type error = expressionParser.get_error(i);
				
				exprtk::parser_error::update_error(error,expressionString[sigIdx].Buffer());
				
				REPORT_ERROR(ErrorManagement::Information,
							 "Error[%i]. Position: row %i, col %i. Type: %s. Description: %s.",
							 i,
							 static_cast<uint32>(error.line_no),
							 static_cast<uint32>(error.column_no),
							 exprtk::parser_error::to_str(error.mode).c_str(),
							 error.diagnostic.c_str()
							);
				
			}
			
			return ok;
		}
		
	}
	
	return ok;
}

bool MathExpressionGAM::Execute() {
	
	//REPORT_ERROR(ErrorManagement::Debug, "EXECUTE");
	
	/// GAM memory is copied in local memory as a vector of float64.
	for (uint32 sigIdx = 0; sigIdx < numInputSignals; sigIdx++) {
		
		for (uint32 elemIdx = 0; elemIdx < inputNumOfElements[sigIdx]; elemIdx++) {
			
			if (inputSignalType[sigIdx]==UnsignedInteger8Bit) {
				
				localInputArray[sigIdx][elemIdx] = static_cast<float64>( *( ((uint8*)GetInputSignalMemory(sigIdx)) + elemIdx ) );
				
			} else if (inputSignalType[sigIdx]==UnsignedInteger16Bit) {
				
				localInputArray[sigIdx][elemIdx] = static_cast<float64>( *( ((uint16*)GetInputSignalMemory(sigIdx)) + elemIdx ) );
				
			} else if (inputSignalType[sigIdx]==UnsignedInteger32Bit) {
				
				localInputArray[sigIdx][elemIdx] = static_cast<float64>( *( ((uint32*)GetInputSignalMemory(sigIdx)) + elemIdx ) );
				
			} else if (inputSignalType[sigIdx]==UnsignedInteger64Bit) {
				
				localInputArray[sigIdx][elemIdx] = static_cast<float64>( *( ((uint64*)GetInputSignalMemory(sigIdx)) + elemIdx ) );
				
			} else if (inputSignalType[sigIdx]==SignedInteger8Bit) {
				
				localInputArray[sigIdx][elemIdx] = static_cast<float64>( *( ((int8*)GetInputSignalMemory(sigIdx)) + elemIdx ) );
				
			} else if (inputSignalType[sigIdx]==SignedInteger16Bit) {
				
				localInputArray[sigIdx][elemIdx] = static_cast<float64>( *( ((int16*)GetInputSignalMemory(sigIdx)) + elemIdx ) );
				
			} else if (inputSignalType[sigIdx]==SignedInteger32Bit) {
				
				localInputArray[sigIdx][elemIdx] = static_cast<float64>( *( ((int32*)GetInputSignalMemory(sigIdx)) + elemIdx ) );
				
			} else if (inputSignalType[sigIdx]==SignedInteger64Bit) {
				
				localInputArray[sigIdx][elemIdx] = static_cast<float64>( *( ((int64*)GetInputSignalMemory(sigIdx)) + elemIdx ) );
				
			} else if (inputSignalType[sigIdx]==Float32Bit) {
				
				localInputArray[sigIdx][elemIdx] = static_cast<float64>( *( ((float32*)GetInputSignalMemory(sigIdx)) + elemIdx ) );
				
			} else if (inputSignalType[sigIdx]==Float64Bit) {
				
				localInputArray[sigIdx][elemIdx] = static_cast<float64>( *( ((float64*)GetInputSignalMemory(sigIdx)) + elemIdx ) );
				
			} else {
				
				REPORT_ERROR(ErrorManagement::Exception,
							 "Error while loading from GAM memory. Signal no. %i: unsupported type.",
							 sigIdx);
				
			}
			
		}
		
	}
	
	/// The expression is then evaluated and its value is printed.
	for (uint32 sigIdx = 0; sigIdx < numOutputSignals; sigIdx++) {
		
		outputExpression[sigIdx].value();
		
	}
	
	/// Finally, result of the evaluation is copied to GAM memory
	/// after being recasted to its original type.
	for (uint32 sigIdx = 0; sigIdx < numOutputSignals; sigIdx++) {
		
		for (uint32 elemIdx = 0; elemIdx < outputNumOfElements[sigIdx]; elemIdx++) {
				
			if (outputSignalType[sigIdx]==UnsignedInteger8Bit) {
				
				*( ((uint8*)GetOutputSignalMemory(sigIdx)) + elemIdx ) = static_cast<uint8>( localOutputArray[sigIdx][elemIdx] );
				
			} else if (outputSignalType[sigIdx]==UnsignedInteger16Bit) {
				
				*( ((uint16*)GetOutputSignalMemory(sigIdx)) + elemIdx ) = static_cast<uint16>( localOutputArray[sigIdx][elemIdx] );
				
			} else if (outputSignalType[sigIdx]==UnsignedInteger32Bit) {
				
				*( ((uint32*)GetOutputSignalMemory(sigIdx)) + elemIdx ) = static_cast<uint32>( localOutputArray[sigIdx][elemIdx] );
				
			} else if (outputSignalType[sigIdx]==UnsignedInteger64Bit) {
				
				*( ((uint64*)GetOutputSignalMemory(sigIdx)) + elemIdx ) = static_cast<uint64>( localOutputArray[sigIdx][elemIdx] );
				
			} else if (outputSignalType[sigIdx]==SignedInteger8Bit) {
				
				*( ((int8*)GetOutputSignalMemory(sigIdx)) + elemIdx ) = static_cast<int8>( localOutputArray[sigIdx][elemIdx] );
				
			} else if (outputSignalType[sigIdx]==SignedInteger16Bit) {
				
				*( ((int16*)GetOutputSignalMemory(sigIdx)) + elemIdx ) = static_cast<int16>( localOutputArray[sigIdx][elemIdx] );
				
			} else if (outputSignalType[sigIdx]==SignedInteger32Bit) {
				
				*( ((int32*)GetOutputSignalMemory(sigIdx)) + elemIdx ) = static_cast<int32>( localOutputArray[sigIdx][elemIdx] );
				
			} else if (outputSignalType[sigIdx]==SignedInteger64Bit) {
				
				*( ((int64*)GetOutputSignalMemory(sigIdx)) + elemIdx ) = static_cast<int64>( localOutputArray[sigIdx][elemIdx] );
				
			} else if (outputSignalType[sigIdx]==Float32Bit) {
				
				*( ((float32*)GetOutputSignalMemory(sigIdx)) + elemIdx ) = static_cast<float32>( localOutputArray[sigIdx][elemIdx] );
				
			} else if (outputSignalType[sigIdx]==Float64Bit) {
				
				*( ((float64*)GetOutputSignalMemory(sigIdx)) + elemIdx ) = static_cast<float64>( localOutputArray[sigIdx][elemIdx] );
				
			} else {
				
				REPORT_ERROR(ErrorManagement::Exception,
							 "Error while writing on GAM memory. Signal no. %i: unsupported type.",
							 sigIdx);
				
			}
			
		}
		
	}
	
	// Sleep for debugging
	MARTe::Sleep::Sec(1);
	
	return true;
}

CLASS_REGISTER(MathExpressionGAM, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool MathExpressionGAM::GetSignalNames(const SignalDirection direction,
									   StreamString* names) {
	
	bool   ok;
	uint32 numOfSignals;
	
	if (direction==InputSignals) {
		
		numOfSignals = GetNumberOfInputSignals();
		
	} else if (direction==OutputSignals) {
		
		numOfSignals = GetNumberOfOutputSignals();
		
	}
	
	for (uint32 i = 0; i < numOfSignals; i++) {
		
		ok = GetSignalName(direction, i, names[i]);
		if (!ok) return ok;
		
	}
	
	return ok;
	
}

} /* namespace MARTe */