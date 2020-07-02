/**
 * @file MathExpressionGAM.cpp
 * @brief Source file for class MathExpressionGAM
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

 * @details This source file contains the definition of all the methods for
 * the class MathExpressionGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "MathExpressionGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MathExpressionGAM::MathExpressionGAM() :
        GAM() {
    
    mathParser    = NULL_PTR(MathExpressionParser*);
    evaluator     = NULL_PTR(RuntimeEvaluator*);
    inputSignals  = NULL_PTR(SignalStruct*);
    outputSignals = NULL_PTR(SignalStruct*);
}

MathExpressionGAM::~MathExpressionGAM() {
    
    if (mathParser != NULL) {
        delete mathParser;
    }
    if (evaluator != NULL) {
        delete evaluator;
    }
    if (inputSignals != NULL) {
        delete[] inputSignals;
    }
    if (outputSignals != NULL) {
        delete[] outputSignals;
    }
}

bool MathExpressionGAM::Initialise(StructuredDataI &data) {
    
    bool ok = GAM::Initialise(data);
    
    // Read the mathematical expression in infix form from the configuration file.
    if (ok) {
        ok = data.Read("Expression", expr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                "Cannot find Expression among MathExpressionGAM parameters.");
        }
    }
    
    // Parser initialization
    if (ok) {
        expr.Seek(0);
        mathParser = new MathExpressionParser(expr);
        ok = mathParser->Parse();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError,
                "Failed to parse input Expression.");
        }
    }
    
    // Evaluator initialization
    if (ok) {
        evaluator = new RuntimeEvaluator(mathParser->GetStackMachineExpression());
    }

    return ok;
    
}

bool MathExpressionGAM::Setup() {
    
    bool ok = true;
    
    // 0. Get information about signals
    inputSignals  = new SignalStruct[numberOfInputSignals];
    outputSignals = new SignalStruct[numberOfOutputSignals];
    
    for (uint32 signalIdx = 0u; (signalIdx < numberOfInputSignals) && ok; signalIdx++) {
        
        ok = GetSignalName(InputSignals, signalIdx, inputSignals[signalIdx].name);
        if (ok) {
            ok = GetSignalNumberOfElements(InputSignals, signalIdx, inputSignals[signalIdx].numberOfElements);
            inputSignals[signalIdx].type = GetSignalType(InputSignals, signalIdx);
        }
    }
    
    for (uint32 signalIdx = 0u; (signalIdx < numberOfOutputSignals) && ok; signalIdx++) {
        
        ok = GetSignalName(OutputSignals, signalIdx, outputSignals[signalIdx].name);
        if (ok) {
            ok = GetSignalNumberOfElements(OutputSignals, signalIdx, outputSignals[signalIdx].numberOfElements);
            outputSignals[signalIdx].type = GetSignalType(OutputSignals, signalIdx);
        }
    }
    
    // 1. Checks
    for (uint32 signalIdx = 0u; (signalIdx < numberOfInputSignals) && ok; signalIdx++) {
        ok = (inputSignals[signalIdx].numberOfElements == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::UnsupportedFeature,
                "Input signal %s has %u elements (> 1). Only scalar signals are supported.",
                (inputSignals[signalIdx].name).Buffer(), inputSignals[signalIdx].numberOfElements);
        } 
    }
    
    for (uint32 signalIdx = 0u; (signalIdx < numberOfOutputSignals) && ok; signalIdx++) {
        ok = (outputSignals[signalIdx].numberOfElements == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::UnsupportedFeature,
                "Output signal %s has %u elements (> 1). Only scalar signals are supported.",
                (outputSignals[signalIdx].name).Buffer(), outputSignals[signalIdx].numberOfElements);
        } 
    }
    
    // 2. Evaluator initialization
    if (ok) {
        ok = evaluator->ExtractVariables();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError,
                "Failed RuntimeEvaluator::ExtractVariables().");
        }
    }
    
    // look for input variable among input signals
    if (ok){
        for (uint32 signalIdx = 0u; (signalIdx < numberOfInputSignals) && (ok); signalIdx++) {
                
            ok = evaluator->SetInputVariableType(inputSignals[signalIdx].name, inputSignals[signalIdx].type);
            if (ok) {
                ok = evaluator->SetInputVariableMemory(inputSignals[signalIdx].name, GetInputSignalMemory(signalIdx));
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError,
                    "Can't associate input signal '%s': no variable of the same name in the expression.",
                    (inputSignals[signalIdx].name).Buffer());
            }
        }
    }

    // look for output variable among output signals
    if (ok){
        for (uint32 signalIdx = 0u; (signalIdx < numberOfOutputSignals) && (ok); signalIdx++) {
                
            ok = evaluator->SetOutputVariableType(outputSignals[signalIdx].name, outputSignals[signalIdx].type);
            if (ok) {
                ok = evaluator->SetOutputVariableMemory(outputSignals[signalIdx].name, GetOutputSignalMemory(signalIdx));
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError,
                    "Can't associate output signal '%s': no variable of the same name in the expression.",
                    (outputSignals[signalIdx].name).Buffer());
            }
        }
        
    }
    
    // 4. Check that all variables have been assigned to a signal or parameter.
    uint32 index;
    VariableInformation* var;
    
    // input variables
    index = 0u;
    while(evaluator->BrowseInputVariable(index, var) && ok) {
        
        if (StringHelper::CompareN((var->name).Buffer(), "Constant@", 9u) != 0) {    // exclude constants
            ok = (var->externalLocation != NULL);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError,
                             "Can't associate input variable '%s': no input signal of the same name.",
                             (var->name).Buffer());
            }
        }
        index++;
    }
    
    // output variables (only a warning is issued, internal output variables are allowed)
    index = 0u;
    while(evaluator->BrowseOutputVariable(index, var) && ok) {
        
        if (var->externalLocation == NULL) {
            REPORT_ERROR(ErrorManagement::Warning,
                        "Can't associate output variable '%s': no output signal of the same name. By default it is considered internal.",
                        (var->name).Buffer());
        }
        index++;
    }
    
    // 5. Compilation
    if (ok) {
        ok = evaluator->Compile();
        if(!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError,
                "Failed compilation (invalid syntax?) in expression: %s",
                expr.Buffer());
        } 
    }
    
    return ok;
}

bool MathExpressionGAM::Execute() {
    
    return evaluator->Execute();
    
}

CLASS_REGISTER(MathExpressionGAM, "1.0")

} /* namespace MARTe */

